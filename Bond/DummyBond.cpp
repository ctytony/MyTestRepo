/***************************************************************
DummyBond.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MachineData.h"
#include "Program.h"
#include "Reference.h"
#include "Wire.h"
#include "BondPara.h"
#include "BallPara.h"
#include "Bond.h"
#include "DummyBond.h"
#include "UsgControl.h"
#include "EfoControl.h"
#include "EfoProfile.h"
#include "BondObjectDlg.h"
#include "servo.h"
#include "Debug.h"

// CDummyBond
CDummyBond theDummyBond;

CDummyBond::CDummyBond()
{
    m_cFabType = 0;
    m_nDummyBondPointIndex = 0;
}

CDummyBond::~CDummyBond()
{
}

int CDummyBond::PerformDummyBond(char cFabType, TBL_POS stBondPos, bool bPopMsg)	//v3.1.T207 edit
{
    short nStatus = FOK;
	theBond.PreFireEFO();	//fire efo before bonding
    m_cFabType = cFabType;

    theServo.MoveTable2(stBondPos, true, false);	//v3.1.T207 edit
    GenerateDummyBondInfo();
    SetUsgInfo();
    SendUsgInfo();

	theServo.WaitTableStop();
	theServo.ResetBondProgress();

    if (theServo.RunBuffer(4) != FOK)
    {
		theBond.SetErrNo(7);
        return BOND_SERVO_ERROR;
    }
    Sleep(20);

    if (theBond.CheckBondProgress(7, false) == FOK)	//v1.5.T113 edit, v1.5.T115 edit
    {
        if (thePgm.IsCopperSequence() == false)
        {
            Sleep(10);
			nStatus = (theEFO.IsSparkSuccess() == true ? FOK : FNOK);	//v1.6.T338 add
            if ((nStatus == FNOK) && bPopMsg)
            {
				CStringList sList;
				if (theEFO.GetErrorCode() == SPARK_OPEN)
					sList.AddTail(theMsg.GetMsg(MSG_SPARK_OPEN));
				else
					sList.AddTail(theMsg.GetMsg(MSG_SPARK_GAP_LARGE));
                theMsg.ShowMsg(ERROR_MSG, sList);
            }
        }
    }

	TBL_POS stPos;
	stPos.dx = -theMcData.GetRcd()->stBondTipOs.dx;
	stPos.dy = -theMcData.GetRcd()->stBondTipOs.dy;
	theServo.MoveTable2(stPos, false);	//v3.1.T207 edit

	theServo.StopBuffer(4);
    Sleep(30);

    return nStatus;
}

void CDummyBond::SetUsgInfo(void)
{
	ST_DYNAMIC_USG_INFO stUsgInfo;
    DUMMY_BOND_PARA stDummyBond;
    theBondPara.GetDummyBondPara(&stDummyBond);
    BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();

    stUsgInfo.nStandbyPower[1] = 0;
    stUsgInfo.nStandbyTime[1]  = 0;
    stUsgInfo.nCtactPower[1]   = round(pstBondPara->fCtactPower[1] * 2.55);
    stUsgInfo.nCtactTime[1]    = pstBondPara->nCtactTime[1] * 10;

    stUsgInfo.nBasePower[1]    = round(stDummyBond.fBondPower * 2.55);
    stUsgInfo.nBaseTime[1]     = stDummyBond.nBondTime * 10;

    stUsgInfo.nReleasePower[1] = round(pstBondPara->fReleasePower[1] * 2.55);
    stUsgInfo.nReleaseTime[1]  = pstBondPara->nReleaseTime[1] * 10;

    // 2nd bond power delay
    stUsgInfo.nPowerDelay[1]   = pstBondPara->nPowerDelay[1] * 10;
    if (stUsgInfo.nBaseTime[1] <= 0)
    {
        stUsgInfo.nBaseTime[1]   = 0;
        stUsgInfo.nPowerDelay[1] = 0;
    }
    else if (stUsgInfo.nPowerDelay[1] >= stUsgInfo.nBaseTime[1])
        stUsgInfo.nPowerDelay[1] = stUsgInfo.nBaseTime[1] - 10;

    stUsgInfo.nBaseTime[1] -= stUsgInfo.nPowerDelay[1];

    stUsgInfo.nScrubOffsetTime2     = 0;
    stUsgInfo.nScrubPower[1]        = 0;
    stUsgInfo.nScrubTime[1]         = 0;

    // 2nd bond ramp time calculation
    if (stUsgInfo.nCtactTime[1] > 0)
    {
        stUsgInfo.nCtactRampTime[1] = round((double)stUsgInfo.nCtactTime[1] * (double)pstBondPara->nCtactPowerRamp[1] / 100.0);
        if (stUsgInfo.nCtactRampTime[1] < 1)
            stUsgInfo.nCtactRampTime[1] = 1;

        stUsgInfo.nCtactTime[1] -= stUsgInfo.nCtactRampTime[1];
    }
    else
        stUsgInfo.nCtactRampTime[1] = 0;

    if (stUsgInfo.nBaseTime[1] > 0)
    {
        stUsgInfo.nBaseRampTime[1] = round((double)stUsgInfo.nBaseTime[1] * (double)pstBondPara->nBasePowerRamp[1] / 100.0);
        if (stUsgInfo.nBaseRampTime[1] < 1)
            stUsgInfo.nBaseRampTime[1] = 1;

        stUsgInfo.nBaseTime[1] -= stUsgInfo.nBaseRampTime[1];
    }
    else
        stUsgInfo.nBaseRampTime[1] = 0;

    if (stUsgInfo.nReleaseTime[1] > 0)
    {
        stUsgInfo.nRlsRampTime[1] = round((double)stUsgInfo.nReleaseTime[1] * (double)pstBondPara->nReleasePowerRamp[1] / 100.0);
        if (stUsgInfo.nRlsRampTime[1] < 1)
            stUsgInfo.nRlsRampTime[1] = 1;

        stUsgInfo.nReleaseTime[1] -= stUsgInfo.nRlsRampTime[1];
    }
    else
        stUsgInfo.nRlsRampTime[1] = 0;

    theBond.CapillaryWearOutPowerCompensate(&stUsgInfo.nStandbyPower[1]);
    theBond.CapillaryWearOutPowerCompensate(&stUsgInfo.nCtactPower[1]);
    theBond.CapillaryWearOutPowerCompensate(&stUsgInfo.nBasePower[1]);
    theBond.CapillaryWearOutPowerCompensate(&stUsgInfo.nReleasePower[1]);
	
	//---------------profile specification---------------------------------------------------------

    // 1st bond profile 0 or 4   
	m_anIndex[0] = 0;

    // Standby
	m_anUsgTime1[0]=0;
	theBond.PowerCompensation(&stUsgInfo.nStandbyPower[1]);
	m_anUsgPower1[0] = (BYTE)(stUsgInfo.nStandbyPower[1]);

	m_anUsgTime1[1]  = (WORD)(stUsgInfo.nStandbyTime[1]);
	m_anUsgPower1[1] = (BYTE)(stUsgInfo.nStandbyPower[1]);
    
    // Contact boost
    m_anUsgTime1[2]  = 0;
    m_anUsgTime1[3]  = 0;

 	m_anUsgPower1[2] = 0;
	m_anUsgPower1[3] = 0;

    // Ctact
	m_anUsgTime1[4]  = (WORD)(stUsgInfo.nCtactRampTime[1]);
	theBond.PowerCompensation(&stUsgInfo.nCtactPower[1]);
	m_anUsgPower1[4] = (BYTE)(stUsgInfo.nCtactPower[1]);

	m_anUsgTime1[5]  = (WORD)(stUsgInfo.nCtactTime[1]);
	m_anUsgPower1[5] = (BYTE)(stUsgInfo.nCtactPower[1]);
    
    // Scrub
    m_anUsgTime1[6]  = 0;
    m_anUsgTime1[7]  = 0;

    m_anUsgPower1[6] = 0;
    m_anUsgPower1[7] = 0;

    // Power delay
	if (stUsgInfo.nPowerDelay[0] > 0)
    {
        m_anUsgTime1[8] = USG_MIN_RAMP_TIME;
	    m_anUsgTime1[9] = (WORD)(stUsgInfo.nPowerDelay[1] - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime1[8] = 0;
	    m_anUsgTime1[9] = 0;
    }

	m_anUsgPower1[8] = 0;
	m_anUsgPower1[9] = 0;

    // Base boost
    m_anUsgTime1[10]  = 0;
    m_anUsgTime1[11]  = 0;

	m_anUsgPower1[10] = 0;
	m_anUsgPower1[11] = 0;

    // Base
	m_anUsgTime1[12]  = (WORD)(stUsgInfo.nBaseRampTime[1]);
	theBond.PowerCompensation(&stUsgInfo.nBasePower[1]);
	m_anUsgPower1[12] = (BYTE)(stUsgInfo.nBasePower[1]);

	m_anUsgTime1[13]  = (WORD)(stUsgInfo.nBaseTime[1]);
	m_anUsgPower1[13] = (BYTE)(stUsgInfo.nBasePower[1]);

    // Release
	m_anUsgTime1[14]  = (WORD)(stUsgInfo.nRlsRampTime[1]);
	theBond.PowerCompensation(&stUsgInfo.nReleasePower[1]);
	m_anUsgPower1[14] = (BYTE)(stUsgInfo.nReleasePower[1]);

	m_anUsgTime1[15]  = (WORD)(stUsgInfo.nReleaseTime[1]);
	m_anUsgPower1[15] = (BYTE)(stUsgInfo.nReleasePower[1]);
	
    m_anUsgTime1[16]  = 0;
    m_anUsgPower1[16] = 0;

    m_anUsgTime1[17]  = 0;
    m_anUsgPower1[17] = 0;

    m_anUsgTime1[18]  = 0;
    m_anUsgPower1[18] = 0;

    m_anUsgTime1[19]  = 0;
    m_anUsgPower1[19] = 0;

    // 2nd bond profile 2 or 6 ---------------------------------------------------------
    m_anIndex[1] = 2;

    //standby
	m_anUsgTime2[0]=0;
	theBond.PowerCompensation(&stUsgInfo.nStandbyPower[1]);
	m_anUsgPower2[0] = (BYTE)(stUsgInfo.nStandbyPower[1]);

	m_anUsgTime2[1]  = (WORD)(stUsgInfo.nStandbyTime[1]);
	m_anUsgPower2[1] = (BYTE)(stUsgInfo.nStandbyPower[1]);

    // Ctact
	m_anUsgTime2[2]  = (WORD)(stUsgInfo.nCtactRampTime[1]);
	theBond.PowerCompensation(&stUsgInfo.nCtactPower[1]);
	m_anUsgPower2[2] = (BYTE)(stUsgInfo.nCtactPower[1]);

	m_anUsgTime2[3]  = (WORD)(stUsgInfo.nCtactTime[1]);
	m_anUsgPower2[3] = (BYTE)(stUsgInfo.nCtactPower[1]);
    
    // 2nd bond Scrub offset
    m_anUsgTime2[4]  = 0;
    m_anUsgTime2[5]  = 0;

	m_anUsgPower2[4] = 0;
	m_anUsgPower2[5] = 0;

    // Scrub
    m_anUsgTime2[6]  = 0;
    m_anUsgTime2[7]  = 0;

    m_anUsgPower2[6] = 0;
	m_anUsgPower2[7] = 0;

    // Power delay
	if (stUsgInfo.nPowerDelay[1] > 0)
    {
        m_anUsgTime2[8] = USG_MIN_RAMP_TIME;
	    m_anUsgTime2[9] = (WORD)(stUsgInfo.nPowerDelay[1] - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime2[8] = 0;
	    m_anUsgTime2[9] = 0;
    }

	m_anUsgPower2[8] = 0;
	m_anUsgPower2[9] = 0;

    //Base
	m_anUsgTime2[10]  = (WORD)(stUsgInfo.nBaseRampTime[1]);
	theBond.PowerCompensation(&stUsgInfo.nBasePower[1]);
	m_anUsgPower2[10] = (BYTE)(stUsgInfo.nBasePower[1]);

	m_anUsgTime2[11]  = (WORD)(stUsgInfo.nBaseTime[1]);
	m_anUsgPower2[11] = (BYTE)(stUsgInfo.nBasePower[1]);

    // Release
	m_anUsgTime2[12]  = (WORD)(stUsgInfo.nRlsRampTime[1]);
    theBond.PowerCompensation(&stUsgInfo.nReleasePower[1]);
	m_anUsgPower2[12] = (BYTE)(stUsgInfo.nReleasePower[1]);

	m_anUsgTime2[13]  = (WORD)(stUsgInfo.nReleaseTime[1]);
	m_anUsgPower2[13] = (BYTE)(stUsgInfo.nReleasePower[1]);
}

void CDummyBond::SendUsgInfo(void)
{
	theUSG.SetProfile(m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
    Sleep(10);
    theUSG.SetProfile(m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
}

int CDummyBond::GenerateDummyBondInfo(void)
{
    long lCtactLvl, lSrchLvl, lTailLen;
    DUMMY_BOND_PARA stDummyBond;
    theBondPara.GetDummyBondPara(&stDummyBond);

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();

	//v3.1.T60 delete
	//short nTailLength;
	//if (m_chFabType == 0)
	//	nTailLength = thePgm.GetRcd()->nTailLengthUm;
	//else
	//	nTailLength = thePgm.GetRcd()->nFab2TailLengthUm;
	short nTailLen = theEfoProfile.GetProfile(m_cFabType)->nTailLen_x1um;	//v3.1.T60 add
    lTailLen = round((double)(nTailLen) / theServo.GetResolZ());

	lSrchLvl = round((pstMcData->lResetLevel - 2000) / theServo.GetResolZ());
    DUMMY_BOND_AREA_RCD stDummyArea;
    theBondPara.GetDummyBondAreaRcd(&stDummyArea);
    if (stDummyArea.bBondAreaValid)
    {
        lCtactLvl = stDummyArea.lCtactLvl;
        lSrchLvl = lCtactLvl + round(2000.0 / theServo.GetResolZ());
    }
    else if (thePgm.IsProgramValid())
        lCtactLvl = theRef.GetRcd(0)->lCtactLvl;
    else
        lCtactLvl = pstMcData->lForceCalibLvl;

    //lCtactSrchHt = lCtactLvl + round(500.0 / theServo.GetResolZ());

    int anData[MAX_INT_VARIABLE];

	anData[SRCH_LVL1] = lSrchLvl;
	anData[SRCH_LVL2] = lSrchLvl;	//v3.1.T262 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
	anData[SRCH_SPEED2] = stDummyBond.nSrchSpeed;
#else
    anData[SRCH_SPEED2] = -stDummyBond.nSrchSpeed * 2500;
#endif
    anData[SRCH_TOL2] = stDummyBond.nSrchTol;

	double dForceFactor = 1000.0;	//v3.1.T293 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
	dForceFactor = 1.0;
#endif
    anData[CTACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstBondPara->nCtactForce[1], lCtactLvl) * dForceFactor);
    anData[CTACT_TIME2] = pstBondPara->nCtactTime[1];

    anData[BASE_FORCE2] = round(theMcData.Calc3PtBondForceDac(-stDummyBond.nBondForce, lCtactLvl) * dForceFactor);
    anData[BASE_TIME2] = stDummyBond.nBondTime;

    anData[RLS_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstBondPara->nReleaseForce[1], lCtactLvl) * dForceFactor);
    anData[RLS_TIME2] = pstBondPara->nReleaseTime[1];

    anData[TAIL_HEIGHT] = lTailLen;
    anData[RESET_LVL] = pstMcData->lResetLevel;

    if (theDebug.IsDebugWaveform() || theDebug.IsDebugWireClamp()
		|| theMcConfig.IsDebugModeWaveform())	//v3.1.T462 add
        anData[SCOPE_ENABLE] = 1;
    else
        anData[SCOPE_ENABLE] = 0;

    anData[SPARK_DELAY] = 20;
    anData[SRCH_DELAY2] = pstBondPara->nCtactSrchDelay[1];

    anData[PRE_IMPACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstBondPara->nPreCtactForce[1], lCtactLvl) * dForceFactor);

    anData[COPPER_SEQUENCE] = thePgm.IsCopperSequence();
    anData[SEARCH_MODE] = pstMcData->nSearchMode;

    anData[DAMPGAIN2] = pstMcData->nDampGain2;
    anData[PRE_IMPACT_TIME2] = pstBondPara->nPreCtactTime[1];

	//v3.1.T34 delete
	//if (m_chFabType == 0)
	//   anData[FAB_TYPE] = FAB_TYPE_1;
	//else
	//   anData[FAB_TYPE] = FAB_TYPE_2;
	anData[FAB_TYPE] = m_cFabType;		//v3.1.T34 add

    int nStatus = theServo.SendIntDataToServo(anData, MAX_INT_VARIABLE, 4);
    if (nStatus != FOK) return nStatus;

    return 0;
}

int CDummyBond::CapillaryUnplug(void)
{
    char chFabType = 0;
    short i, nRowSize, nColumnSize, nRow, nColumn;
    bool bBkupCuSequence, bMultiFab = false;
    int iStatus;
    short nBondObject = BUMPOPT_WIRE_ONLY;
    TBL_POS stDummyBondPos;
    WIRE_RECORD stWireRcd;
    ST_BUMP_BALL stBumpBall;
    DUMMY_BOND_AREA_RCD stDummyBondAreaRcd;

    theBondPara.GetDummyBondAreaRcd(&stDummyBondAreaRcd);

    if (stDummyBondAreaRcd.bBondAreaValid == false) return 0;
    
	theWire.GetRcd(&stWireRcd, theBond.m_nCurWire);
	switch (stWireRcd.nBumpType)	//v3.1.T88 add
	{
	case BUMP_BOS:
	{
		theBallPara.GetBbosBall(&stBumpBall);
		if (stBumpBall.chFabType > 0)
			bMultiFab = true;
		break;
	}
	case BUMP_SOB:
	{
		theBallPara.GetBsobBall(&stBumpBall);
		if (stBumpBall.chFabType > 0)
			bMultiFab = true;
		break;
	}
	}

    if (bMultiFab == true)
    {
        CBondObjectDlg dlg(1);
        dlg.SetBondObject(BUMPOPT_WIRE_ONLY);
        dlg.DoModal();

        nBondObject = (short)dlg.GetBondObject();

        if (nBondObject == BUMPOPT_WIRE_ONLY)
            chFabType = 0;
        else
            chFabType = stBumpBall.chFabType;
    }

	bBkupCuSequence = thePgm.IsCopperSequence();
	thePgm.SetCopperSequence(false);

    nRowSize = (short)((stDummyBondAreaRcd.stLeftTopPos.dy - stDummyBondAreaRcd.stRightBottomPos.dy) * theServo.GetResolXY()) / DUMMY_BOND_PITCH;
    nColumnSize = (short)((stDummyBondAreaRcd.stLeftTopPos.dx - stDummyBondAreaRcd.stRightBottomPos.dx) * theServo.GetResolXY()) / DUMMY_BOND_PITCH;

    for (i = 0; i < AUTO_UNPLUG_TIMES; i++)
    {
		if (nColumnSize == 0)
		{
			nRow = 0;
			nColumn = 0;
		}
		else
		{
			nRow = m_nDummyBondPointIndex / nColumnSize + 1;
			nColumn = m_nDummyBondPointIndex % nRowSize + 1;
		}

        stDummyBondPos.dx = stDummyBondAreaRcd.stLeftTopPos.dx - DUMMY_BOND_PITCH * nColumn / theServo.GetResolXY();
        stDummyBondPos.dy = stDummyBondAreaRcd.stLeftTopPos.dy - DUMMY_BOND_PITCH * nRow / theServo.GetResolXY();
        theServo.MoveTable2(stDummyBondPos);	//v3.1.T207 edit
        iStatus = theDummyBond.PerformDummyBond(chFabType, stDummyBondPos, false);

        m_nDummyBondPointIndex++;
        if (m_nDummyBondPointIndex >= nRowSize * nColumnSize)
            m_nDummyBondPointIndex = 0;

        if (iStatus == FOK) break;
    }

	thePgm.SetCopperSequence(bBkupCuSequence);

	if (iStatus != FOK)
    {
		CStringList sList;
        sList.AddTail(theMsg.GetMsg(MSG_CAP_UNPLUG_FAIL));
        theMsg.ShowMsg(WARNING_MSG, sList);
    }
    else
    {
        if (bBkupCuSequence == true)
        {
			if (nColumnSize == 0)
			{
				nRow = 0;
				nColumn = 0;
			}
			else
			{
				nRow = m_nDummyBondPointIndex / nColumnSize + 1;
				nColumn = m_nDummyBondPointIndex % nRowSize + 1;
			}

            stDummyBondPos.dx = stDummyBondAreaRcd.stLeftTopPos.dx - DUMMY_BOND_PITCH * nColumn / theServo.GetResolXY();
            stDummyBondPos.dy = stDummyBondAreaRcd.stLeftTopPos.dy - DUMMY_BOND_PITCH * nRow / theServo.GetResolXY();
            theServo.MoveTable2(stDummyBondPos);	//v3.1.T207 edit
            theDummyBond.PerformDummyBond(chFabType, stDummyBondPos, false);
            m_nDummyBondPointIndex++;
        }
    }

    return iStatus;
}
