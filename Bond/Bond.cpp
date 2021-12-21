/***************************************************************
Bond.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "NonSym2.h"
#include "MsgDef.h"
#include "DataStruct.h"
#include "MathCalc.h"
#include "servo.h"	//v3.1.T82 add
#if _SERVO_TYPE == 1
	#include "..\Library\ACS\ACS_SC\acsc.h"
#else if _SERVO_TYPE == 0
	#include "..\Library\ACS\ACS_PCI\acsc.h"
#endif
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "Wire.h"
#include "BondPara.h"
#include "BallPara.h"
#include "LoopPara.h"
#include "Bond.h"
#include "Mtncalc.h"
#include "DebugDlg.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "ScanKeyThread.h"
#include "BondStatistics.h"
#include "TowerLight.h"
#include "BsdControl.h"
#include "VisionSystem.h"
#include "CorrectBondDlg.h"
#include "PositionAdjustDlg.h"
#include "DigitalIO.h"
#include "BSDDebug.h"
#include "MachineConfig.h"
#include "ContactLevel.h"
#include "StepperControl.h"
#include "UsgControl.h"
#include "EfoControl.h"
#include "LoopShape.h"
#include "PrAlign.h"
#include "DummyBond.h"
#include "EfoProfile.h"
#include "stdexcept"
#include "VEIndex.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "CuIndex.h"
#include "CuClamp.h"
#include "CuOnload.h"
#include "CuOffload.h"
#include "R2RIndex.h"
#include "Debug.h"
#include <memory>
#include "DlgElevCtrl.h"
#include "LogTrace.h"
#include "ForceSwitch.h"
#include "TopEliteIndex.h"
#include "Busy.h"	//v3.1.T461 add

CBond theBond;

// CBond

CBond::CBond()
: m_nCurWire(1)
, m_nRestartWire(1)
, m_nBondedWire(0)
, m_nCurRow(1)
, m_nCurCol(1)
, m_nBondMode(BONDMODE_IDLE)
, m_nBondType(BONDTYPE_RESTART)
, m_nBondBuffer(0)
, m_bCheckNsop(true)
, m_bCheckNsol(true)
, m_bCheckFab(true)
, m_bDryRun(false)
, m_bBondNoVision(true)
, m_bAutoIndex(true)
, m_bCompleteBond(false)
, m_bIll(false)
, m_nCurTrack(INNER_TRACK)
, m_nBondStickSample(0)
, m_nStickDetectBlock(0)
, m_iUSGError(0)
, m_iHeaterWarnSkip(0)
, m_bPrOn1stEnterDone(false)
, m_bRequestStopBond(false)
, m_nAutoSkipDieCount(0)
, m_nEndWire(0)
, m_bAutoDefineBond(false)
, m_nWaveformCaptureCount(0)
, m_nCurMatRow(1)
, m_nCurMatCol(1)
, m_bUpdateBondData(false)
, m_lTotalAutoSkipDieCount(0)
, m_nStopCount(0)
, m_bStopBondReady(false)	//v3.1.T461 add
{
	m_stCurTblPos.dx = 0.0;
	m_stCurTblPos.dy = 0.0;

	ResetBkPrCounts();

	m_bRedoPR = FALSE;		//V1.1.W250 add
	m_nSkipNumPerIndex = 0;	//V1.1.W262 add
	m_nSerialSkipNum = 0;	//V1.1.W265 add
}

CBond::~CBond()
{
}

// CBond member functions

int CBond::InitUsgProfile()
{
    ST_USG_INFO stUsgInfo;
	USG_STATIC_PARA_RCD stUsgPara;
    theBondPara.GetUSGParaRcd(&stUsgPara);
    stUsgInfo.nBondingSitPower = stUsgPara.nSeatPower;
    stUsgInfo.nBondingSitTime  = (short)(stUsgPara.cSeatPowerTime * 10);
    stUsgInfo.nFeedPower       = stUsgPara.nFeedPower;
    stUsgInfo.nFeedTime        = (short)(stUsgPara.cFeedPowerTime * 10);
    stUsgInfo.nTailPower       = stUsgPara.nTailPower;
    stUsgInfo.nTailTime        = (short)(stUsgPara.cTailPowerTime * 10);

    ST_USG_PROFILE stUsgProfile[3];

    // profile 7 - seat power
    stUsgProfile[0].cIndex           = 7;
    stUsgProfile[0].stVertex1.cLevel = (BYTE)(stUsgInfo.nBondingSitPower);
    stUsgProfile[0].stVertex1.nTime  = 0;
    stUsgProfile[0].stVertex2.cLevel = (BYTE)(stUsgInfo.nBondingSitPower);
    stUsgProfile[0].stVertex2.nTime  = (WORD)(stUsgInfo.nBondingSitTime + 1);	//x0.1ms dynamic time, triggered by event sync
    stUsgProfile[0].stVertex3.cLevel = 0;
    stUsgProfile[0].stVertex3.nTime  = (WORD)(stUsgInfo.nBondingSitTime + 2);	//dynamic time, triggered by event stop
    stUsgProfile[0].stVertex4.cLevel = 0;
    stUsgProfile[0].stVertex4.nTime  = (WORD)(stUsgInfo.nBondingSitTime + 3);

    // profile 3 - feed power
    stUsgProfile[1].cIndex           = 3;
    stUsgProfile[1].stVertex1.cLevel = (BYTE)(stUsgInfo.nFeedPower);
    stUsgProfile[1].stVertex1.nTime  = 0;
    stUsgProfile[1].stVertex2.cLevel = (BYTE)(stUsgInfo.nFeedPower);
    stUsgProfile[1].stVertex2.nTime  = (WORD)(stUsgInfo.nFeedTime + 1);		//x0.1ms dynamic time , triggered by event sync
    stUsgProfile[1].stVertex3.cLevel = 0;
    stUsgProfile[1].stVertex3.nTime  = stUsgProfile[1].stVertex2.nTime + 1;	//dynamic time , triggered by event stop
    stUsgProfile[1].stVertex4.cLevel = 0;
    stUsgProfile[1].stVertex4.nTime  = stUsgProfile[1].stVertex2.nTime + 2;

    // profile 1 - tail power
    stUsgProfile[2].cIndex           = 1;
    stUsgProfile[2].stVertex1.cLevel = (BYTE)(stUsgInfo.nTailPower);
    stUsgProfile[2].stVertex1.nTime  = 0;
    stUsgProfile[2].stVertex2.cLevel = (BYTE)(stUsgInfo.nTailPower);
    stUsgProfile[2].stVertex2.nTime  = (WORD)(stUsgInfo.nTailTime + 1);		//x0.1ms   dynamic time , triggered by event sync
    stUsgProfile[2].stVertex3.cLevel = 0;
    stUsgProfile[2].stVertex3.nTime  = stUsgProfile[2].stVertex2.nTime + 1;	//dynamic time , triggered by event stop
    stUsgProfile[2].stVertex4.cLevel = 0;
    stUsgProfile[2].stVertex4.nTime  = stUsgProfile[2].stVertex2.nTime + 2;

    // send all profile for first time
    theUSG.SetSmallProfile(stUsgProfile[0]);
    Sleep_x1ms(5);
    theUSG.SetSmallProfile(stUsgProfile[1]);
    Sleep_x1ms(5);
    theUSG.SetSmallProfile(stUsgProfile[2]);
    Sleep_x1ms(5);

    m_nBondBuffer = 0;
    if (CalcUsgProfile(m_nCurWire, BUMPSEQ_WIRE) == BOND_WIRE_LENGTH_ERROR)
		return BOND_WIRE_LENGTH_ERROR;	//V1.1.W254 edit
	SetUsgProfile(m_nCurWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
    Sleep_x1ms(20);
    SetUsgProfile(m_nCurWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
    Sleep_x1ms(20);

    m_nBondBuffer = 1;
    if (CalcUsgProfile(m_nCurWire, BUMPSEQ_WIRE) == BOND_WIRE_LENGTH_ERROR)
		return BOND_WIRE_LENGTH_ERROR;	//V1.1.W254 edit
    SetUsgProfile(m_nCurWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
    Sleep_x1ms(20);
	SetUsgProfile(m_nCurWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
    Sleep_x1ms(20);

    m_nBondBuffer = 0;
    theUSG.Standby();

	return 0;
}

int CBond::CalcUsgProfile(short nWireNum, short nBumpSeq)	//v3.1.T93 edit
{
	if (nWireNum <= 0) return FOK;	//v3.1.T407sc add

	const WIRE_RECORD *pstWire = theWire.GetRcd(nWireNum);
	//if ((pstWire->nBumpType == BUMP_NONE) && (nBumpSeq == BUMPSEQ_BALL))	//v3.1.T94 delete
	if (pstWire->nBumpType == BUMP_NONE) nBumpSeq = BUMPSEQ_WIRE;	//v3.1.T94 add

	ST_BUMP_BALL stBumpBall;
	ST_BUMP_SHAPE stBumpShape;
	ST_BUMP_WIRE stBumpWire;
	switch (nBumpSeq)	//v3.1.T94 add
	{
	case BUMPSEQ_WIRE:
	{
		switch (pstWire->nBumpType)	//v3.1.T88 add
		{
		case BUMP_BOS:
			theBallPara.GetBbosWire(&stBumpWire);
			break;
		case BUMP_SOB:
			theBallPara.GetBsobWire(&stBumpWire);
			break;
		case BUMP_BSB:	//v3.1.T103 add
			if (nBumpSeq == BUMPSEQ_BALL)	//BSOB
				theBallPara.GetBsobWire(&stBumpWire);
			else	//BBOS
				theBallPara.GetBbosWire(&stBumpWire);
			break;
		}
		break;
	}
	case BUMPSEQ_BALL:
	{
		switch (pstWire->nBumpType)	//v3.1.T88 add
		{
		case BUMP_BOS:
		{
			theBallPara.GetBbosBall(&stBumpBall);
			theBallPara.GetBbosShape(&stBumpShape);
			stBumpShape.chShapeMode = pstWire->cBbosShapeBumpShapeMode;	//v3.1.T362 add
			break;
		}
		case BUMP_SOB:
		{
			theBallPara.GetBsobBall(&stBumpBall);
			theBallPara.GetBsobShape(&stBumpShape);
			stBumpShape.chShapeMode = pstWire->cBsobShapeBumpShapeMode;	//v3.1.T362 add
			break;
		}
		case BUMP_BSB:	//v3.1.T103 add
		{
			if (nBumpSeq == BUMPSEQ_BALL)	//BSOB
			{
				theBallPara.GetBsobBall(&stBumpBall);
				theBallPara.GetBsobShape(&stBumpShape);
				stBumpShape.chShapeMode = pstWire->cBsobShapeBumpShapeMode;	//v3.1.T362 add
			}
			else	//BBOS
			{
				theBallPara.GetBbosBall(&stBumpBall);
				theBallPara.GetBbosShape(&stBumpShape);
				stBumpShape.chShapeMode = pstWire->cBbosShapeBumpShapeMode;	//v3.1.T362 add
			}
			break;
		}
		}
		break;
	}
	}

	double dMoveDist, dJerk, dMaxAcc, dMaxVel;
	short nBumpCutPowerDelay, nBumpCutPower, nBumpCutPowerTime;
	SERVO_SPEED_PROFILE stServoSpeed;
	ST_DYNAMIC_USG_INFO stUsgInfo;
	// Get bond parameter record for wire, bump, bump wire
	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();
	switch (nBumpSeq)	//v3.1.T94 add
	{
	case BUMPSEQ_WIRE:
	{
		stUsgInfo.nStandbyPower[0] = round((pstBondPara->fStandbyPower[0] + pstWire->fPrePowerOffset[0]) * 2.55);
		stUsgInfo.nStandbyTime[0]  = SYNC_WAITING_TIME;
		stUsgInfo.nCtactPower[0]   = round((pstBondPara->fCtactPower[0] + pstWire->fCtactPowerOffset[0]) * 2.55);
		stUsgInfo.nCtactTime[0]    = (pstBondPara->nCtactTime[0] + pstWire->nCtactTimeOffset[0]) * 10;
	    
		stUsgInfo.nBasePower[0]    = round((pstBondPara->fBasePower[0] + pstWire->fPowerOffset[0]) * 2.55);
		stUsgInfo.nBaseTime[0]     = (pstBondPara->nBaseTime[0] + pstWire->nTimeOffset[0]) * 10;
		if (m_nBondMode == BONDMODE_CORRECT)
		{
			stUsgInfo.nStandbyPower[0] += round(pstBondPara->fCrtStandbyPowerOffset[0] * 2.55);	//v3.0.T268 add

			stUsgInfo.nBaseTime[0] += pstBondPara->nCrtBondTimeOffset1 * 10;
			if (stUsgInfo.nBaseTime[0] < 0) stUsgInfo.nBaseTime[0] = 0;

			stUsgInfo.nBasePower[0] += round(pstBondPara->fCrtBondPowerOffset1 * 2.55);
			if (stUsgInfo.nBasePower[0] < 0) stUsgInfo.nBasePower[0] = 0;
			else if (stUsgInfo.nBasePower[0] > 100) stUsgInfo.nBasePower[0] = 100;
		}
	    
		// 1st bond power delay
		stUsgInfo.nPowerDelay[0] = pstBondPara->nPowerDelay[0] * 10;
		if (stUsgInfo.nBaseTime[0] <= 0)
		{
			stUsgInfo.nBaseTime[0] = 0;
			stUsgInfo.nPowerDelay[0] = 0;
		}
		else if (stUsgInfo.nPowerDelay[0] >= stUsgInfo.nBaseTime[0])
			stUsgInfo.nPowerDelay[0] = stUsgInfo.nBaseTime[0] - 10;

		stUsgInfo.nBaseTime[0] -= stUsgInfo.nPowerDelay[0];

		// 1st bond release power
		stUsgInfo.nReleasePower[0] = round(pstBondPara->fReleasePower[0] * 2.55);
		stUsgInfo.nReleaseTime[0]  = pstBondPara->nReleaseTime[0] * 10;
	    
		// 1st bond impact boost power
		if (pstBondPara->ch1stBondImpactBoostEnable == 1)
		{
			if (stUsgInfo.nCtactTime[0] == 0)
			{
				stUsgInfo.nCtactBoostPower1 = stUsgInfo.nCtactPower[0];
				stUsgInfo.nCtactBoostTime1 = 0;
			}
			else
			{
				if ((pstBondPara->chImpactBoostDuration1 == 0)
					|| (stUsgInfo.nCtactTime[0] < (BOOST_DURATION_LONG * 10.0)))
					stUsgInfo.nCtactBoostTime1 = round(BOOST_DURATION_SHORT * 10.0);
				else
					stUsgInfo.nCtactBoostTime1 = round(BOOST_DURATION_LONG * 10.0);

				stUsgInfo.nCtactTime[0] -= stUsgInfo.nCtactBoostTime1;

				if (pstBondPara->chImpactBoostAmplitude1 == 0)
					stUsgInfo.nCtactBoostPower1 = round((double)stUsgInfo.nCtactPower[0] * BOOST_AMPLITUDE_LOW);
				else if (pstBondPara->chImpactBoostAmplitude1 == 1)
					stUsgInfo.nCtactBoostPower1 = round((double)stUsgInfo.nCtactPower[0] * BOOST_AMPLITUDE_MIDDLE);
				else
					stUsgInfo.nCtactBoostPower1 = round((double)stUsgInfo.nCtactPower[0] * BOOST_AMPLITUDE_HIGH);
			}
		}
		else
		{
			stUsgInfo.nCtactBoostPower1 = stUsgInfo.nCtactPower[0];
			stUsgInfo.nCtactBoostTime1 = 0;
		}

		// 1st bond boost power
		if (pstBondPara->ch1stBondBoostEnable == 1)
		{
			if (stUsgInfo.nBaseTime[0] == 0)
			{
				stUsgInfo.nBaseBoostPower1 = stUsgInfo.nBasePower[0];
				stUsgInfo.nBaseBoostTime1 = 0;
			}
			else
			{
				if ((pstBondPara->chBoostDuration1 == 0)
					|| (stUsgInfo.nBaseTime[0] < (BOOST_DURATION_LONG * 10.0)))
					stUsgInfo.nBaseBoostTime1 = round(BOOST_DURATION_SHORT * 10.0);
				else
					stUsgInfo.nBaseBoostTime1 = round(BOOST_DURATION_LONG * 10.0);

				stUsgInfo.nBaseTime[0] -= stUsgInfo.nBaseBoostTime1;

				if (pstBondPara->chBoostAmplitude1 == 0)
					stUsgInfo.nBaseBoostPower1 = round((double)stUsgInfo.nBasePower[0] * BOOST_AMPLITUDE_LOW);
				else if (pstBondPara->chBoostAmplitude1 == 1)
					stUsgInfo.nBaseBoostPower1 = round((double)stUsgInfo.nBasePower[0] * BOOST_AMPLITUDE_MIDDLE);
				else
					stUsgInfo.nBaseBoostPower1 = round((double)stUsgInfo.nBasePower[0] * BOOST_AMPLITUDE_HIGH);
			}
		}
		else
		{
			stUsgInfo.nBaseBoostPower1 = stUsgInfo.nBasePower[0];
			stUsgInfo.nBaseBoostTime1 = 0;
		}

		SCRUB_PARA_RCD stScrubRcd;
		if (pstWire->ch1stBondScrubMode > 0)
		{
			theBondPara.GetBondScrubRcd(FIRST_BOND, pstWire->ch1stBondScrubMode - 1, &stScrubRcd);

			pstBondPara->b1stBondScrubEnable = true;
			pstBondPara->ch1stScrubDirection = stScrubRcd.chScrubDirection;
			pstBondPara->n1stScrubAmplitude  = stScrubRcd.nScrubAmplitude;
			pstBondPara->n1stScrubCycle = stScrubRcd.nScrubCycle;
			pstBondPara->n1stScrubForce = stScrubRcd.nScrubForce;
			pstBondPara->f1stScrubPower = stScrubRcd.fScrubPower;
			pstBondPara->n1stScrubSpeed = stScrubRcd.nScrubSpeed;
		}
		else
			pstBondPara->b1stBondScrubEnable = false;

		// 1st bond scrub power
		if (pstBondPara->b1stBondScrubEnable == true)
		{
			stUsgInfo.nScrubPower[0] = round(pstBondPara->f1stScrubPower * 2.55);

			theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
			dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0 * pstBondPara->n1stScrubSpeed / 100.0;
			dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY() * pstBondPara->n1stScrubSpeed / 100.0;
			dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();

			dMoveDist = pstBondPara->n1stScrubAmplitude / theServo.GetResolXY();
			short nScrubT1 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
	        
			dMoveDist = pstBondPara->n1stScrubAmplitude * 2.0 / theServo.GetResolXY();
			short nScrubT2 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

			stUsgInfo.nScrubTime[0] = (nScrubT1 * 2 + 
				(pstBondPara->n1stScrubCycle * 2 - 1) * nScrubT2) * 10;
		}
		else
		{
			stUsgInfo.nScrubPower[0] = 0;
			stUsgInfo.nScrubTime[0] = 0;
		}

		// 1st bond ramp time calculation
		if (stUsgInfo.nCtactTime[0] > 0)
		{
			stUsgInfo.nCtactRampTime[0] = round((double)stUsgInfo.nCtactTime[0] * (double)pstBondPara->nCtactPowerRamp[0] / 100.0);
			if (stUsgInfo.nCtactRampTime[0] < 1) stUsgInfo.nCtactRampTime[0] = 1;

			stUsgInfo.nCtactTime[0] -= stUsgInfo.nCtactRampTime[0];
		}
		else
			stUsgInfo.nCtactRampTime[0] = 0;

		if (stUsgInfo.nBaseTime[0] > 0)
		{
			stUsgInfo.nBaseRampTime[0] = round((double)stUsgInfo.nBaseTime[0] * (double)pstBondPara->nBasePowerRamp[0] / 100.0);
			if (stUsgInfo.nBaseRampTime[0] < 1) stUsgInfo.nBaseRampTime[0] = 1;

			stUsgInfo.nBaseTime[0] -= stUsgInfo.nBaseRampTime[0];
		}
		else
			stUsgInfo.nBaseRampTime[0] = 0;

		if (stUsgInfo.nReleaseTime[0] > 0)
		{
			stUsgInfo.nRlsRampTime[0] = round((double)stUsgInfo.nReleaseTime[0] * (double)pstBondPara->nReleasePowerRamp[0] / 100.0);
			if (stUsgInfo.nRlsRampTime[0] < 1) stUsgInfo.nRlsRampTime[0] = 1;

			stUsgInfo.nReleaseTime[0] -= stUsgInfo.nRlsRampTime[0];
		}
		else
			stUsgInfo.nRlsRampTime[0] = 0;

		switch (pstWire->nBumpType)	//v3.1.T88 add
		{
		case BUMP_SOB:
		case BUMP_BSB:	//v3.1.T103 add
		{
			stUsgInfo.nStandbyPower[1] = round((stBumpWire.fStandbyPower + pstWire->fBsobWirePreCtactPowerOffset) * 2.55);	//v3.1.T401 edit
			stUsgInfo.nStandbyTime[1]  = SYNC_WAITING_TIME;
			stUsgInfo.nCtactPower[1]   = round((stBumpWire.fCtactPower + pstWire->fBsobWireCtactPowerOffset) * 2.55);	//v3.1.T401 edit
			stUsgInfo.nCtactTime[1]    = (stBumpWire.nCtactTime + pstWire->nBsobWireCtactTimeOffset) * 10;				//v3.1.T401 edit
			//stUsgInfo.nBasePower[1]    = round((stBumpWire.fBasePower + pstWire->fBumpWirePowerOffset) * 2.55);		//v3.1.T401 delete
			stUsgInfo.nBasePower[1]    = round((stBumpWire.fBasePower + pstWire->fBsobWireBondPowerOffset) * 2.55);		//v3.1.T401 edit
			stUsgInfo.nBaseTime[1]     = (stBumpWire.nBaseTime + pstWire->nBsobWireBondTimeOffset) * 10;				//v3.1.T401 edit
			stUsgInfo.nReleasePower[1] = round((stBumpWire.fReleasePower + pstWire->fBsobWireRlsPowerOffset) * 2.55);	//v3.1.T401 edit
			stUsgInfo.nReleaseTime[1]  = (stBumpWire.nReleaseTime + pstWire->nBsobWireRlsTimeOffset) * 10;				//v3.1.T401 edit
			break;
		}
		default:
		{
			stUsgInfo.nStandbyPower[1] = round((pstBondPara->fStandbyPower[1] + pstWire->fPrePowerOffset[1]) * 2.55);
			stUsgInfo.nStandbyTime[1]  = SYNC_WAITING_TIME;
			stUsgInfo.nCtactPower[1]   = round((pstBondPara->fCtactPower[1] + pstWire->fCtactPowerOffset[1]) * 2.55);
			stUsgInfo.nCtactTime[1]    = (pstBondPara->nCtactTime[1]+pstWire->nCtactTimeOffset[1]) * 10;
			stUsgInfo.nBasePower[1]    = round((pstBondPara->fBasePower[1] + pstWire->fPowerOffset[1]) * 2.55);
			stUsgInfo.nBaseTime[1]     = (pstBondPara->nBaseTime[1] + pstWire->nTimeOffset[1]) * 10;
			stUsgInfo.nReleasePower[1] = round(pstBondPara->fReleasePower[1] * 2.55);
			stUsgInfo.nReleaseTime[1]  = pstBondPara->nReleaseTime[1] * 10;
		}
		}

		if (m_nBondMode == BONDMODE_CORRECT)
		{
			stUsgInfo.nStandbyPower[1] += round(pstBondPara->fCrtStandbyPowerOffset[1] * 2.55);	//v3.0.T268 add

			stUsgInfo.nBaseTime[1]  += pstBondPara->nCrtBondTimeOffset2 * 10;
			if (stUsgInfo.nBaseTime[1] < 0) stUsgInfo.nBaseTime[1] = 0;

			stUsgInfo.nBasePower[1] += round(pstBondPara->fCrtBondPowerOffset2 * 2.55);
			if (stUsgInfo.nBasePower[1] < 0) stUsgInfo.nBasePower[1] = 0;
			else if (stUsgInfo.nBasePower[1] > 100) stUsgInfo.nBasePower[1] = 100;
		}

		short nBondShift, nTailShift, nShiftSpeed;
		switch (pstWire->nBumpType)	//v3.1.T88 add
		{
		case BUMP_SOB:
		case BUMP_BSB:	//v3.1.T103 add
		{
			nBondShift  = stBumpWire.nBondShift;
			nTailShift  = stBumpWire.nTailShift;
			nShiftSpeed = stBumpWire.nShiftSpeed;
			break;
		}
		default:
		{
			short n = ((nWireNum - 1) % thePgm.GetUnitTotalWire()) + 1;
			if ((n < WIRE_NUM)	//v3.1.T568 add
				&& (theMcConfig.GetBondShift2(n) != 0))	//v3.0.T219 add
				nBondShift = theMcConfig.GetBondShift2(n);
			else
				nBondShift = pstBondPara->nBondShift2;
			nTailShift  = pstBondPara->nTailShift;
			nShiftSpeed = pstBondPara->nShiftSpeed;
		}
		}

		theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
		dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0 * nShiftSpeed / 100.0;
		dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY() * nShiftSpeed / 100.0;
		dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();

		//if ((pstWire->nBumpType != BUMP_SOB) && (abs(pstBondPara->nBondShift) > 0))
		if (abs(nBondShift) > 0)
		{
			dMoveDist = abs(nBondShift) / theServo.GetResolXY();
			short nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
			if (stUsgInfo.nBaseTime[1] < nMoveTime) stUsgInfo.nBaseTime[1] = nMoveTime;
		}

		if (abs(nTailShift) > 0)
		{
			dMoveDist = abs(nTailShift) / theServo.GetResolXY();
			short nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
			if (stUsgInfo.nReleaseTime[1] < nMoveTime) stUsgInfo.nReleaseTime[1] = nMoveTime;
		}

		// Calculate power factor
		double dWireLen = Calc2PtDist(pstWire->stTchPos[0], pstWire->stTchPos[1]) * theServo.GetResolXY();
		if (dWireLen == 0)
		{
			PopWireLenZeroError(nWireNum);
			return BOND_WIRE_LENGTH_ERROR;
		}
		double dCosTheta = fabs(pstWire->stTchPos[1].dx - pstWire->stTchPos[0].dx) * theServo.GetResolXY() / dWireLen;
		stUsgInfo.nBasePower[1] += round(pstBondPara->fPowerFactor * dCosTheta * 2.55);

		// 2nd bond power delay
		stUsgInfo.nPowerDelay[1] = pstBondPara->nPowerDelay[1] * 10;
		if (stUsgInfo.nBaseTime[1] <= 0)
		{
			stUsgInfo.nBaseTime[1] = 0;
			stUsgInfo.nPowerDelay[1] = 0;
		}
		else if (stUsgInfo.nPowerDelay[1] >= stUsgInfo.nBaseTime[1])
			stUsgInfo.nPowerDelay[1] = stUsgInfo.nBaseTime[1] - 10;

		stUsgInfo.nBaseTime[1] -= stUsgInfo.nPowerDelay[1];

		if (pstWire->ch2ndBondScrubMode > 0)
		{
			theBondPara.GetBondScrubRcd(SECOND_BOND, pstWire->ch2ndBondScrubMode - 1, &stScrubRcd);

			pstBondPara->b2ndBondScrubEnable = true;
			pstBondPara->ch2ndScrubDirection = stScrubRcd.chScrubDirection;
			pstBondPara->n2ndScrubAmplitude  = stScrubRcd.nScrubAmplitude;
			pstBondPara->n2ndScrubPosOffset1 = stScrubRcd.nScrubPosOffset1;
			pstBondPara->n2ndScrubPosOffset2 = stScrubRcd.nScrubPosOffset2;
			pstBondPara->n2ndScrubCycle = stScrubRcd.nScrubCycle;
			pstBondPara->n2ndScrubForce = stScrubRcd.nScrubForce;
			pstBondPara->f2ndScrubPower = stScrubRcd.fScrubPower;
			pstBondPara->n2ndScrubSpeed = stScrubRcd.nScrubSpeed;
		}
		else
			pstBondPara->b2ndBondScrubEnable = false;

		if ((pstWire->nBumpType != BUMP_SOB)
			&& (pstWire->nBumpType != BUMP_BSB)	//v3.1.T103 add
			&& (pstBondPara->b2ndBondScrubEnable))
		{
			stUsgInfo.nScrubPower[1] = round(pstBondPara->f2ndScrubPower * 2.55);

			theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
			dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0 * pstBondPara->n2ndScrubSpeed / 100.0;
			dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY() * pstBondPara->n2ndScrubSpeed / 100.0;
			dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();

			dMoveDist = pstBondPara->n2ndScrubPosOffset1 / theServo.GetResolXY();
			stUsgInfo.nScrubOffsetTime2 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

			dMoveDist = pstBondPara->n2ndScrubAmplitude / theServo.GetResolXY();
			short nScrubT1 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
	        
			dMoveDist = pstBondPara->n2ndScrubAmplitude * 2.0 / theServo.GetResolXY();
			short nScrubT2 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

			dMoveDist = pstBondPara->n2ndScrubPosOffset2 / theServo.GetResolXY();
			short nScrubOffset2Time = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

			stUsgInfo.nScrubTime[1] = (nScrubT1 * 2 + 
									   (pstBondPara->n2ndScrubCycle * 2 - 1) * nScrubT2 +
									   nScrubOffset2Time) * 10;
		}
		else
		{
			stUsgInfo.nScrubOffsetTime2 = 0;
			stUsgInfo.nScrubPower[1] = 0;
			stUsgInfo.nScrubTime[1] = 0;
		}

		// 2nd bond ramp time calculation
		if (stUsgInfo.nCtactTime[1] > 0)
		{
			stUsgInfo.nCtactRampTime[1] = round((double)stUsgInfo.nCtactTime[1] * (double)pstBondPara->nCtactPowerRamp[1] / 100.0);
			if (stUsgInfo.nCtactRampTime[1] < 1) stUsgInfo.nCtactRampTime[1] = 1;

			stUsgInfo.nCtactTime[1] -= stUsgInfo.nCtactRampTime[1];
		}
		else
			stUsgInfo.nCtactRampTime[1] = 0;

		if (stUsgInfo.nBaseTime[1] > 0)
		{
			stUsgInfo.nBaseRampTime[1] = round((double)stUsgInfo.nBaseTime[1] * (double)pstBondPara->nBasePowerRamp[1] / 100.0);
			if (stUsgInfo.nBaseRampTime[1] < 1) stUsgInfo.nBaseRampTime[1] = 1;

			stUsgInfo.nBaseTime[1] -= stUsgInfo.nBaseRampTime[1];
		}
		else
			stUsgInfo.nBaseRampTime[1] = 0;

		if (stUsgInfo.nReleaseTime[1] > 0)
		{
			stUsgInfo.nRlsRampTime[1] = round((double)stUsgInfo.nReleaseTime[1] * (double)pstBondPara->nReleasePowerRamp[1] / 100.0);
			if (stUsgInfo.nRlsRampTime[1] < 1) stUsgInfo.nRlsRampTime[1] = 1;

			stUsgInfo.nReleaseTime[1] -= stUsgInfo.nRlsRampTime[1];
		}
		else
			stUsgInfo.nRlsRampTime[1] = 0;

		//v1.1.288.T13 add
		if (theDebug.IsDebugTransducer())
		{
			m_sTemp.Format(_T("CapWearoutPower1/2_%d/%d"),
								CalcCapiWearOutPower1(),
								CalcCapiWearOutPower2());
			theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		}

		//v1.1.288.T11 edit
		CompensateCapiWearOutPower1(&stUsgInfo.nStandbyPower[0]);
		CompensateCapiWearOutPower1(&stUsgInfo.nCtactPower[0]);
		CompensateCapiWearOutPower1(&stUsgInfo.nBasePower[0]);
		CompensateCapiWearOutPower1(&stUsgInfo.nReleasePower[0]);
		CompensateCapiWearOutPower2(&stUsgInfo.nStandbyPower[1]);
		CompensateCapiWearOutPower2(&stUsgInfo.nCtactPower[1]);
		CompensateCapiWearOutPower2(&stUsgInfo.nBasePower[1]);
		CompensateCapiWearOutPower2(&stUsgInfo.nReleasePower[1]);
		break;
	}
	default:
	{
		stUsgInfo.nStandbyPower[0]  = round((stBumpBall.fStandbyPower[0] + pstWire->fBump1stPreCtactPowerOffset) * 2.55);
		stUsgInfo.nStandbyTime[0]   = SYNC_WAITING_TIME;
		stUsgInfo.nCtactPower[0]    = round((stBumpBall.fCtactPower[0] + pstWire->fBumpCtactPowerOffset[0]) * 2.55);
		stUsgInfo.nCtactTime[0]     = (stBumpBall.nCtactTime[0] + pstWire->nBumpCtactTimeOffset[0]) * 10;
		stUsgInfo.nBasePower[0]     = round((stBumpBall.fBasePower[0] + pstWire->fBumpPowerOffset[0]) * 2.55);
		stUsgInfo.nBaseTime[0]      = (stBumpBall.nBaseTime[0] + pstWire->nBumpTimeOffset[0]) * 10;
		stUsgInfo.nPowerDelay[0]    = 0;
		stUsgInfo.nReleasePower[0]  = stBumpBall.nRlsPower[0];
		stUsgInfo.nReleaseTime[0]   = stBumpBall.nRlsTime[0] * 10;
		stUsgInfo.nCtactBoostPower1 = stUsgInfo.nCtactPower[0];
		stUsgInfo.nCtactBoostTime1  = 0;
		stUsgInfo.nBaseBoostPower1  = stUsgInfo.nBasePower[0];
		stUsgInfo.nBaseBoostTime1   = 0;

		if (((pstWire->nBumpType == BUMP_SOB)
				|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T103 add
			&& (pstWire->chBumpBondScrubMode > 0))
		{
			SCRUB_PARA_RCD stScrubRcd;
			theBondPara.GetBondScrubRcd(FIRST_BOND, pstWire->chBumpBondScrubMode - 1, &stScrubRcd);
			stBumpBall.b1stBondScrubEnable = true;
			stBumpBall.ch1stScrubDirection = stScrubRcd.chScrubDirection;
			stBumpBall.n1stScrubAmplitude  = stScrubRcd.nScrubAmplitude;
			stBumpBall.n1stScrubCycle = stScrubRcd.nScrubCycle;
			stBumpBall.n1stScrubForce = stScrubRcd.nScrubForce;
			stBumpBall.f1stScrubPower = stScrubRcd.fScrubPower;
			stBumpBall.n1stScrubSpeed = stScrubRcd.nScrubSpeed;
		}
		else
			stBumpBall.b1stBondScrubEnable = false;

		// 1st bond scrub power
		if (stBumpBall.b1stBondScrubEnable == true)
		{
			stUsgInfo.nScrubPower[0] = round(stBumpBall.f1stScrubPower * 2.55);

			theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
			dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0 * stBumpBall.n1stScrubSpeed / 100.0;
			dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY() * stBumpBall.n1stScrubSpeed / 100.0;
			dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();

			dMoveDist = stBumpBall.n1stScrubAmplitude / theServo.GetResolXY();
			short nScrubT1 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
	        
			dMoveDist = stBumpBall.n1stScrubAmplitude * 2.0 / theServo.GetResolXY();
			short nScrubT2 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

			stUsgInfo.nScrubTime[0] = (nScrubT1 * 2 + 
				(stBumpBall.n1stScrubCycle * 2 - 1) * nScrubT2) * 10;
		}
		else
		{
			stUsgInfo.nScrubPower[0] = 0;
			stUsgInfo.nScrubTime[0] = 0;
		}

		// 1st bond ramp time calculation
		if (stUsgInfo.nCtactTime[0] > 0)
		{
			stUsgInfo.nCtactRampTime[0] = round((double)stUsgInfo.nCtactTime[0] * (double)pstBondPara->nCtactPowerRamp[0] / 100.0);
			if (stUsgInfo.nCtactRampTime[0] < 1) stUsgInfo.nCtactRampTime[0] = 1;

			stUsgInfo.nCtactTime[0] -= stUsgInfo.nCtactRampTime[0];
		}
		else
			stUsgInfo.nCtactRampTime[0] = 0;

		if (stUsgInfo.nBaseTime[0] > 0)
		{
			stUsgInfo.nBaseRampTime[0] = round((double)stUsgInfo.nBaseTime[0] * (double)pstBondPara->nBasePowerRamp[0] / 100.0);
			if (stUsgInfo.nBaseRampTime[0] < 1) stUsgInfo.nBaseRampTime[0] = 1;

			stUsgInfo.nBaseTime[0] -= stUsgInfo.nBaseRampTime[0];
		}
		else
			stUsgInfo.nBaseRampTime[0] = 0;

		if (stUsgInfo.nReleaseTime[0] > 0)
		{
			stUsgInfo.nRlsRampTime[0] = round((double)stUsgInfo.nReleaseTime[0] * (double)pstBondPara->nReleasePowerRamp[0] / 100.0);
			if (stUsgInfo.nRlsRampTime[0] < 1) stUsgInfo.nRlsRampTime[0] = 1;

			stUsgInfo.nReleaseTime[0] -= stUsgInfo.nRlsRampTime[0];
		}
		else
			stUsgInfo.nRlsRampTime[0] = 0;

		// Cutting power
		nBumpCutPower = stBumpShape.nCuttingPower;

		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_1, &stServoSpeed);
		dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolZ() / 1000.0 * stBumpShape.nMotionSpeed / 100.0;
		dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolZ() * stBumpShape.nMotionSpeed / 100.0;
		dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolZ() * stBumpShape.nMotionSpeed / 100.0;

		dMoveDist = stBumpShape.nBaseHeight;
		nBumpCutPowerDelay = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
		dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0 * stBumpShape.nMotionSpeed / 100.0;
		dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY() * stBumpShape.nMotionSpeed / 100.0;
		dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();

		if (stBumpShape.chShapeMode == BUMP_SHAPE_FAST)
			dMoveDist = (double)(stBumpShape.nSpanDist + stBumpShape.nScrubDist) / theServo.GetResolXY();
		else
			dMoveDist = (double)(stBumpShape.nSpanDist) / theServo.GetResolXY();
		short nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
		nBumpCutPowerDelay += round((double)nMoveTime * (double)stBumpShape.nCuttingPowerDelay / 100.0);
		nBumpCutPowerTime = round((double)nMoveTime * (100.0 - (double)stBumpShape.nCuttingPowerDelay) / 100.0);

		stUsgInfo.nStandbyPower[1]  = round(stBumpBall.fStandbyPower[1] * 2.55);
		stUsgInfo.nStandbyTime[1]   = SYNC_WAITING_TIME;
		stUsgInfo.nCtactPower[1]    = round((stBumpBall.fCtactPower[1] + pstWire->fBumpCtactPowerOffset[1]) * 2.55);
		stUsgInfo.nCtactTime[1]     = (stBumpBall.nCtactTime[1] + pstWire->nBumpCtactTimeOffset[1]) * 10;
		stUsgInfo.nBasePower[1]     = round((stBumpBall.fBasePower[1] + pstWire->fBumpPowerOffset[1]) * 2.55);
		stUsgInfo.nBaseTime[1]      = (stBumpBall.nBaseTime[1] + pstWire->nBumpTimeOffset[1]) * 10;
		stUsgInfo.nPowerDelay[1]    = 0;
		stUsgInfo.nReleasePower[1]  = stBumpBall.nRlsPower[1];
		stUsgInfo.nReleaseTime[1]   = stBumpBall.nRlsTime[1];
		stUsgInfo.nScrubOffsetTime2 = 0;
		stUsgInfo.nScrubPower[1]    = 0;
		stUsgInfo.nScrubTime[1]     = 0;

		// 2nd bond ramp time calculation
		if (stUsgInfo.nCtactTime[1] > 0)
		{
			stUsgInfo.nCtactRampTime[1] = round((double)stUsgInfo.nCtactTime[1] * (double)pstBondPara->nCtactPowerRamp[1] / 100.0);
			if (stUsgInfo.nCtactRampTime[1] < 1) stUsgInfo.nCtactRampTime[1] = 1;

			stUsgInfo.nCtactTime[1] -= stUsgInfo.nCtactRampTime[1];
		}
		else
			stUsgInfo.nCtactRampTime[1] = 0;

		if (stUsgInfo.nBaseTime[1] > 0)
		{
			stUsgInfo.nBaseRampTime[1] = round((double)stUsgInfo.nBaseTime[1] * (double)pstBondPara->nBasePowerRamp[1] / 100.0);
			if (stUsgInfo.nBaseRampTime[1] < 1) stUsgInfo.nBaseRampTime[1] = 1;

			stUsgInfo.nBaseTime[1] -= stUsgInfo.nBaseRampTime[1];
		}
		else
			stUsgInfo.nBaseRampTime[1] = 0;

		//stUsgInfo.nRlsRampTime[1] = 0;
		if (stUsgInfo.nReleaseTime[1] > 0)
		{
			stUsgInfo.nRlsRampTime[1] = round((double)stUsgInfo.nReleaseTime[1] * (double)pstBondPara->nReleasePowerRamp[1] / 100.0);
			if (stUsgInfo.nRlsRampTime[1] < 1) stUsgInfo.nRlsRampTime[1] = 1;

			stUsgInfo.nReleaseTime[1] -= stUsgInfo.nRlsRampTime[1];
		}
		else
			stUsgInfo.nRlsRampTime[1] = 0;

		if (stBumpBall.bTailBondEnable == false)
		{
			stUsgInfo.nStandbyPower[1] = 0;
			stUsgInfo.nCtactTime[1] = 0;
			stUsgInfo.nCtactRampTime[1] = 0;
			stUsgInfo.nBaseTime[1] = 10;
			stUsgInfo.nBaseRampTime[1] = 1;
			stUsgInfo.nCtactPower[1] = 0;
			stUsgInfo.nBasePower[1] = 0;
			stUsgInfo.nReleasePower[1] = 0;
		}

		//v1.1.288.T13 add
		if (theDebug.IsDebugTransducer())
		{
			m_sTemp.Format(_T("CapWearoutPower1/2_%d/%d"),
								CalcCapiWearOutPower1(),
								CalcCapiWearOutPower2());
			theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		}

		//v1.1.288.T11 edit
		CompensateCapiWearOutPower1(&stUsgInfo.nStandbyPower[0]);
		CompensateCapiWearOutPower1(&stUsgInfo.nCtactPower[0]);
		CompensateCapiWearOutPower1(&stUsgInfo.nBasePower[0]);
		CompensateCapiWearOutPower1(&stUsgInfo.nReleasePower[0]);
		CompensateCapiWearOutPower2(&stUsgInfo.nStandbyPower[1]);
		CompensateCapiWearOutPower2(&stUsgInfo.nCtactPower[1]);
		CompensateCapiWearOutPower2(&stUsgInfo.nBasePower[1]);
		CompensateCapiWearOutPower2(&stUsgInfo.nReleasePower[1]);
	}
	}

	//---------------profile specification---------------------------------------------------------

    // 1st bond profile 0 or 4   
	m_anIndex[0] = m_nBondBuffer * 4;
	
    // Standby
	m_anUsgTime1[0]  = 0;
	PowerCompensation(&stUsgInfo.nStandbyPower[0]);
	m_anUsgPower1[0] = (BYTE)(stUsgInfo.nStandbyPower[0]);

	m_anUsgTime1[1]  = (WORD)(stUsgInfo.nStandbyTime[0]);
	m_anUsgPower1[1] = (BYTE)(stUsgInfo.nStandbyPower[0]);
    
    // Contact boost
	if (stUsgInfo.nCtactBoostTime1 > 0)
    {
        m_anUsgTime1[2] = USG_MIN_RAMP_TIME;
	    m_anUsgTime1[3] = (WORD)(stUsgInfo.nCtactBoostTime1 - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime1[2] = 0;
	    m_anUsgTime1[3] = 0;
    }

    PowerCompensation(&stUsgInfo.nCtactBoostPower1);
	m_anUsgPower1[2] = (BYTE)(stUsgInfo.nCtactBoostPower1);
	m_anUsgPower1[3] = (BYTE)(stUsgInfo.nCtactBoostPower1);

    // Ctact
	m_anUsgTime1[4]  = (WORD)(stUsgInfo.nCtactRampTime[0]);
	PowerCompensation(&stUsgInfo.nCtactPower[0]);
	m_anUsgPower1[4] = (BYTE)(stUsgInfo.nCtactPower[0]);

	m_anUsgTime1[5]  = (WORD)(stUsgInfo.nCtactTime[0]);
	m_anUsgPower1[5] = (BYTE)(stUsgInfo.nCtactPower[0]);
    
    // Scrub
	if (stUsgInfo.nScrubTime[0] > 0)
    {
        m_anUsgTime1[6] = USG_MIN_RAMP_TIME;
	    m_anUsgTime1[7] = (WORD)(stUsgInfo.nScrubTime[0] - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime1[6] = 0;
	    m_anUsgTime1[7] = 0;
    }

	PowerCompensation(&stUsgInfo.nScrubPower[0]);
	m_anUsgPower1[6] = (BYTE)(stUsgInfo.nScrubPower[0]);
	m_anUsgPower1[7] = (BYTE)(stUsgInfo.nScrubPower[0]);

    // Power delay
	if (stUsgInfo.nPowerDelay[0] > 0)
    {
        m_anUsgTime1[8] = USG_MIN_RAMP_TIME;
	    m_anUsgTime1[9] = (WORD)(stUsgInfo.nPowerDelay[0] - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime1[8] = 0;
	    m_anUsgTime1[9] = 0;
    }

	m_anUsgPower1[8] = 0;
	m_anUsgPower1[9] = 0;

    // Base boost
	if (stUsgInfo.nBaseBoostTime1 > 0)
    {
        m_anUsgTime1[10] = USG_MIN_RAMP_TIME;
	    m_anUsgTime1[11] = (WORD)(stUsgInfo.nBaseBoostTime1 - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime1[10] = 0;
	    m_anUsgTime1[11] = 0;
    }

	PowerCompensation(&stUsgInfo.nBaseBoostPower1);
	m_anUsgPower1[10] = (BYTE)(stUsgInfo.nBaseBoostPower1);
	m_anUsgPower1[11] = (BYTE)(stUsgInfo.nBaseBoostPower1);

    // Base
	m_anUsgTime1[12]  = (WORD)(stUsgInfo.nBaseRampTime[0]);
	PowerCompensation(&stUsgInfo.nBasePower[0]);
	m_anUsgPower1[12] = (BYTE)(stUsgInfo.nBasePower[0]);

	m_anUsgTime1[13]  = (WORD)(stUsgInfo.nBaseTime[0]);
	m_anUsgPower1[13] = (BYTE)(stUsgInfo.nBasePower[0]);

    // Release
	m_anUsgTime1[14]  = (WORD)(stUsgInfo.nRlsRampTime[0]);
	PowerCompensation(&stUsgInfo.nReleasePower[0]);
	m_anUsgPower1[14] = (BYTE)(stUsgInfo.nReleasePower[0]);

	m_anUsgTime1[15]  = (WORD)(stUsgInfo.nReleaseTime[0]);
	m_anUsgPower1[15] = (BYTE)(stUsgInfo.nReleasePower[0]);
	
	switch (nBumpSeq)	//v3.1.T94 add
	{
	case BUMPSEQ_WIRE:
	{
		m_anUsgTime1[16]  = 0;
		m_anUsgPower1[16] = 0;

		m_anUsgTime1[17]  = 0;
		m_anUsgPower1[17] = 0;

		m_anUsgTime1[18]  = 0;
		m_anUsgPower1[18] = 0;

		m_anUsgTime1[19]  = 0;
		m_anUsgPower1[19] = 0;
		break;
	}
	default:
	{
		m_anUsgTime1[16]  = USG_MIN_RAMP_TIME;
		m_anUsgPower1[16] = 0;

		m_anUsgTime1[17]  = (WORD)nBumpCutPowerDelay * 10 - USG_MIN_RAMP_TIME;
		m_anUsgPower1[17] = 0;

		m_anUsgTime1[18]  = USG_MIN_RAMP_TIME;
		m_anUsgPower1[18] = (BYTE)nBumpCutPower;

		m_anUsgTime1[19]  = (WORD)nBumpCutPowerTime * 10 - USG_MIN_RAMP_TIME;
		m_anUsgPower1[19] = (BYTE)nBumpCutPower;
	}
	}

    // 2nd bond profile 2 or 6 ---------------------------------------------------------
    m_anIndex[1] = 2 + (m_nBondBuffer * 4);

    //standby
	m_anUsgTime2[0]  = 0;
	PowerCompensation(&stUsgInfo.nStandbyPower[1]);
	m_anUsgPower2[0] = (BYTE)(stUsgInfo.nStandbyPower[1]);

	m_anUsgTime2[1]  = (WORD)(stUsgInfo.nStandbyTime[1]);
	m_anUsgPower2[1] = (BYTE)(stUsgInfo.nStandbyPower[1]);

    // Ctact
	m_anUsgTime2[2]  = (WORD)(stUsgInfo.nCtactRampTime[1]);
	PowerCompensation(&stUsgInfo.nCtactPower[1]);
	m_anUsgPower2[2] = (BYTE)(stUsgInfo.nCtactPower[1]);

	m_anUsgTime2[3]  = (WORD)(stUsgInfo.nCtactTime[1]);
	m_anUsgPower2[3] = (BYTE)(stUsgInfo.nCtactPower[1]);
    
    // 2nd bond Scrub offset
	if (stUsgInfo.nScrubOffsetTime2 > 0)
    {
        m_anUsgTime2[4] = USG_MIN_RAMP_TIME;
	    m_anUsgTime2[5] = (WORD)(stUsgInfo.nScrubOffsetTime2 - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime2[4] = 0;
	    m_anUsgTime2[5] = 0;
    }

	m_anUsgPower2[4] = 0;
	m_anUsgPower2[5] = 0;

    // Scrub
	if (stUsgInfo.nScrubTime[1] > 0)
    {
        m_anUsgTime2[6] = USG_MIN_RAMP_TIME;
	    m_anUsgTime2[7] = (WORD)(stUsgInfo.nScrubTime[1] - USG_MIN_RAMP_TIME);
    }
    else
    {
        m_anUsgTime2[6] = 0;
	    m_anUsgTime2[7] = 0;
    }

    PowerCompensation(&stUsgInfo.nScrubPower[1]);
	m_anUsgPower2[6] = (BYTE)(stUsgInfo.nScrubPower[1]);
	m_anUsgPower2[7] = (BYTE)(stUsgInfo.nScrubPower[1]);

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
	PowerCompensation(&stUsgInfo.nBasePower[1]);
	m_anUsgPower2[10] = (BYTE)(stUsgInfo.nBasePower[1]);

	m_anUsgTime2[11]  = (WORD)(stUsgInfo.nBaseTime[1]);
	m_anUsgPower2[11] = (BYTE)(stUsgInfo.nBasePower[1]);

    // Release
	m_anUsgTime2[12]  = (WORD)(stUsgInfo.nRlsRampTime[1]);
    PowerCompensation(&stUsgInfo.nReleasePower[1]);
	m_anUsgPower2[12] = (BYTE)(stUsgInfo.nReleasePower[1]);

	m_anUsgTime2[13]  = (WORD)(stUsgInfo.nReleaseTime[1]);
	m_anUsgPower2[13] = (BYTE)(stUsgInfo.nReleasePower[1]);

	//v3.0.T410 add - rebond
	for (short i=14; i<=17; i++)
	{
		m_anUsgTime2[i]  = 0;
		m_anUsgPower2[i] = 0;
	}
	if (theMcConfig.IsRebondEnable())
	{
		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_4, &stServoSpeed);
		dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolZ() / 1000.0;
		dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolZ();
		dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolZ();
		short nLoopHtTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel,
								theMcConfig.GetRebondLoopHt());
		short nBaseHtTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel,
								theMcConfig.GetRebondLoopHt() - theMcConfig.GetRebondBaseHt());

		theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
		dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0;
		dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY();
		dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();
		dMoveDist = (double)theMcConfig.GetRebondSpanLen() / theServo.GetResolXY();
		short nSpanLenTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		m_anUsgTime2[14]  = USG_MIN_RAMP_TIME;
		m_anUsgPower2[14] = 0;
		m_anUsgTime2[15]  = (nLoopHtTime + nBaseHtTime + nSpanLenTime) * 10 - USG_MIN_RAMP_TIME;	//v3.0.T413 edit
		m_anUsgPower2[15] = 0;

		m_anUsgTime2[16]  = USG_MIN_RAMP_TIME;
		m_anUsgPower2[16] = (BYTE)theMcConfig.GetRebondPower();
		m_anUsgTime2[17]  = theMcConfig.GetRebondTime() * 10 - USG_MIN_RAMP_TIME;	//v3.0.T413 edit
		m_anUsgPower2[17] = (BYTE)theMcConfig.GetRebondPower();
	}

	return 0;
}

//v1.5.175.W1 add
void CBond::CollectData(ST_WIRE_INFO *pstWireInfo,
						ST_LOOP_PARA *pstLoopPara,
						long lCtactLvl1, long lCtactLvl2,
						short nSrchHt2, DOUBLE *pdArray)
{
	//1st bond x
	*pdArray = round(pstWireInfo->stBondPos[0].dx);

	//1st bond y
	*(pdArray+1) = round(pstWireInfo->stBondPos[0].dy);

	//2nd bond x
	*(pdArray+2) = round(pstWireInfo->stTrajEndPos.dx);

	//2nd bond y
	*(pdArray+3) = round(pstWireInfo->stTrajEndPos.dy);

	//Contact level
	*(pdArray+4) = lCtactLvl1;
	*(pdArray+5) = lCtactLvl2;

	//Loop height
	*(pdArray+6) = pstLoopPara->stLoopBase.nLoopHt;

	//Loop height correction
	*(pdArray+7) = pstLoopPara->stLoopBase.nLoopHtCorrect;

	//Neck distance
	*(pdArray+8) = pstLoopPara->stLoopBase.nRevDist;

	//Neck angle
	*(pdArray+9) = pstLoopPara->stLoopBase.nRevAngle;

	//Search height
	*(pdArray+10) = nSrchHt2;

	//Kink num
	if (pstLoopPara->stLoopBase.nKinkNum < 2) return;

	//Span length
	*(pdArray+11) = pstLoopPara->stLoopBase.nKink2SpanLen;

	//Height factor
	*(pdArray+12) = pstLoopPara->stLoopBase.nKink2HtFactor;
}

//v1.6.T146 add
void CBond::CalcTail(char cFabType)
{
	FORCE_SWITCH_PARA stForceSwitch;
	theForceSwitch.GetRcd(&stForceSwitch);
	short nTailTime, nTailBreakTime, nTailBreakDist;
	if (cFabType == 1)
	{
		nTailTime = stForceSwitch.nFab2TailTime;
		nTailBreakTime = stForceSwitch.nFab2TailBreakTime;
		nTailBreakDist = stForceSwitch.nFab2TailBreakDist;
	}
	else
	{
		nTailTime = stForceSwitch.nTailTime;
		nTailBreakTime = stForceSwitch.nTailBreakTime;
		nTailBreakDist = stForceSwitch.nTailBreakDist;
	}

	//short nTailLength = thePgm.GetTailLength_x1um();	//v3.1.T60 delete
	short nTailLen = theEfoProfile.GetProfile(0)->nTailLen_x1um;	//v3.1.T60 add

	double dZT[100];
	double d;
	int i = 1; 
	while (i <= nTailTime)
	{
		d = i / (nTailTime + 1.0);
		dZT[i] = nTailLen * 32 * (d * d / 4 - d * d * d / 6 - d / 16 + 1.0 / 192);
		if (d < 0.25) 
			dZT[i] = nTailLen * 32 * (d * d * d / 6);
		if (d > 0.75) 
			dZT[i] = nTailLen * 32 * (d * d * d / 6 - d * d / 2 + d / 2 - 26.0 / 192);
		i = i + 1;
	}

	i = 1;
	short nTailTotalTime;
	//if (stBondParaRcd.bTailBreakEnable && nTailBreakTime > 0)
	if ((nTailBreakDist >= 0) && (nTailBreakTime > 0))
	{
		while (i <= nTailBreakTime + 1)
		{
			dZT[nTailTime+i] = dZT[nTailTime+i-1] + nTailBreakDist / nTailBreakTime;
			i = i + 1;
		}
		nTailTotalTime = nTailBreakTime + nTailTime;
	}
	else
	{
		dZT[nTailTime+1] = dZT[nTailTime];
		nTailTotalTime = nTailTime;
	}

	dZT[0] = 0;
	i = 1; 
	double dFilter = 0.5;
	double dPNZZ[100];
	while (i <= nTailTotalTime)
	{
		dPNZZ[i] = dFilter * dZT[i] + (1 - dFilter) / 2 * (dZT[i-1] + dZT[i+1]);
		i = i + 1;
	}

	i = 1;
	while (i <= nTailTotalTime)
	{
		m_adTail[i-1] = dPNZZ[i] - dPNZZ[i-1];                
		i = i + 1;
	}

	m_adTail[96] = 1;
	m_adTail[98] = nTailTime;
	m_adTail[99] = nTailTotalTime;
}

//v1.5.175.W1 add
void CBond::CalcMZ1(const WIRE_RECORD *pstWire, ST_WIRE_INFO *pstWireInfo)
{
	ST_LOOP_PARA stLoopPara;
	LOOP_MODE_ACS_PARA stLoopAcsMode;
	theLoopPara.GetRcd(pstWire->nLoopMode, &stLoopPara);
	theLoopPara.GetLoopAcsModePara(pstWire->nLoopMode, &stLoopAcsMode);	//v1.5.175.P6  add

	int TZ1, TZ3, TZ4, TMZ1, TMZ3, TMZ4, TMXY0, TMT;
	int RAWSPDXY, RAWSPDZ, INDX2, TM1, TM3, TM4, XYDLY;
	double LOOPT1, LOOPT2, LOOPT, THETA, FILTER1, INDX1;
	double PreSpd, PreSrchDly;

	double ZT[500] = {};
	double PNXY[300] = {};
	double PNX[300] = {};
	double PNY[300] = {};
	double PNZ[300] = {};
	double PNZZ[500] = {};

	RAWSPDXY = 25000; 
	RAWSPDZ = 300;
	FILTER1 = 0.5;
	PreSpd = 0;

	XYDLY = stLoopAcsMode.nXyDelay;
	double ARC_RD = stLoopAcsMode.nArcRD / 100.0;
	double ARC_RD2 = stLoopAcsMode.nArcRD2 / 100.0;
	double ARC_LPTP = stLoopAcsMode.nArcLoopTop / 100.0;
	double ARC_TRAJ = stLoopAcsMode.nArcTraj / 100.0;
	double ARC_BEL = stLoopAcsMode.nArcBelle / 100.0;
	double SPD_RH = stLoopAcsMode.nSpdRH / 100.0;
	double SPD_RD = stLoopAcsMode.nSpdRD / 100.0;
	double SPD_BELHT = stLoopAcsMode.nSpdBelleHt / 100.0;
	double SPD_BELDIST = stLoopAcsMode.nSpdBelleDist / 100.0;
	double SPD_HL = stLoopAcsMode.nSpdHL / 100.0;
	double SPD_RD2 = stLoopAcsMode.nSpdRD2 / 100.0;
	double SPD_SL = stLoopAcsMode.nSpdSL / 100.0;
	double SPD_TRAJ = stLoopAcsMode.nSpdTraj / 100.0;

	double WLx = (pstWireInfo->stBondPos[1].dx - pstWireInfo->stBondPos[0].dx) * theServo.GetResolXY();
	double WLy = (pstWireInfo->stBondPos[1].dy - pstWireInfo->stBondPos[0].dy) * theServo.GetResolXY();
	const REF_RECORD *pstRef1 = theRef.GetRcd(pstWire->nRefNum[0]);
	const REF_RECORD *pstRef2 = theRef.GetRcd(pstWire->nRefNum[1]);
	double CD = pstRef2->lCtactLvl - pstRef1->lCtactLvl + stLoopPara.stLoopBase.nCtactLvlOffset;
	double WL = sqrt(WLx * WLx + WLy * WLy);

	double SHT2 = theBondPara.GetRcd()->nSrchHeight2;

	double LC = stLoopAcsMode.nLoopCorrect + pstWire->nWireLengthCorrect;
	double RH;
	double RD;
	double NECK = stLoopAcsMode.nNeckAngle;

	double RDA;
	if (0 == stLoopAcsMode.iRDadjUnit)
		RDA = stLoopAcsMode.nRHadj / 100.0;      
	else
		RDA = stLoopAcsMode.nRHadj;

	double RH_OP = stLoopPara.stLoopBase.nRevHt;
	double LH = stLoopAcsMode.nLoopHeight1;
	double Alfa = stLoopAcsMode.nAlpha / 100.0;
	double Delta = (1 - Alfa) * LH;

	double SL_ADJ;
	if (0 == stLoopAcsMode.iSLadjUnit)
		SL_ADJ = stLoopAcsMode.nSLadj / 100.0;
	else
		SL_ADJ = stLoopAcsMode.nSLadj;      

	double HL;
	double FL;
	if (0 == stLoopAcsMode.iFlatLengthUnit)
		FL = stLoopAcsMode.nFlatLength / 100.0 * WL;
	else
		FL = stLoopAcsMode.nFlatLength;

	double KINKHT = stLoopAcsMode.nKinkHt;

	double FL_ADJ;
	if (0 == stLoopAcsMode.iFLadjUnit)
		FL_ADJ = stLoopAcsMode.nFLadj / 100.0;
	else
		FL_ADJ = stLoopAcsMode.nFLadj; 

	double HL_ADJ;
	if (0 == stLoopAcsMode.iHLadjUnit)
		HL_ADJ = stLoopAcsMode.nHLadj / 100.0;
	else
		HL_ADJ = stLoopAcsMode.nHLadj; 

	double AC = stLoopAcsMode.nAngleComp;

	double PKH = 0;
	THETA = atan2(WLy, WLx);

	PreSrchDly = 0;
	PreSpd = 0;

	double L = (Alfa + (1 - Alfa) * PI / 2) * LH;
	double X0 = pstWireInfo->stBondPos[0].dx * theServo.GetResolXY();
	double Y0 = pstWireInfo->stBondPos[0].dy * theServo.GetResolXY();
	double XF = pstWireInfo->stBondPos[1].dx * theServo.GetResolXY();
	double YF = pstWireInfo->stBondPos[1].dy * theServo.GetResolXY();
	double LA1 = 0;
	double LA2 = 0;

	double XYR = 0 / 180 * PI;
	double XYM = stLoopAcsMode.nAboveLoopTop;

	double NECKADJ;
	if (0 == stLoopAcsMode.iNeckHtUnit)
		NECKADJ = stLoopAcsMode.nNeckHtAdj / 100.0;
	else
		NECKADJ = stLoopAcsMode.nNeckHtAdj;

	RH = L * cos(int(NECK) * PI / 180);
	RD = L * sin(int(NECK) * PI / 180);
	double SL = sqrt((WL - Delta) * (WL - Delta) + (LH - CD) * (LH - CD));
	double CA = atan((LH - CD) / (WL - Delta)) * 180 / PI;

	HL = pow(((FL - Delta) * (FL - Delta) + KINKHT * KINKHT), 0.5);
	SL = pow(((WL - Delta) * (WL - Delta) + (LH - CD) * (LH - CD)), 0.5);
	CA = atan((LH - CD) / (WL - Delta)) * 180 / PI;

	double P[3][10];

	if (pstWireInfo->bKink2Enable == false)
	{
		P[0][0]= X0; 
		P[1][0]= Y0; 
		P[2][0]= 0; 
		P[0][1]= X0; 
		P[1][1]= Y0;

		P[2][1]= L; 
		if (0 == stLoopAcsMode.iRDadjUnit) 
			P[2][1] = P[2][1] + RDA * (L - RH); 
		else 
			P[2][1] = P[2][1] + RDA; 

		double a = cos(THETA);
		P[0][2] = -RD * cos(THETA + LA1) + X0;
		P[1][2] = -RD * sin(THETA + LA1) + Y0;

		if (0 == stLoopAcsMode.iNeckHtUnit)
			P[2][2] = RH + (L - RH) * NECKADJ; 
		else
			P[2][2] = RH + NECKADJ; 

		P[0][7] = P[0][2]; 
		P[1][7] = P[1][2];
		if (0 == stLoopAcsMode.iSLadjUnit)
			P[2][7] = P[2][2] + (1 + SL_ADJ) * SL; 
		else 
			P[2][7] = P[2][2] + SL + SL_ADJ;

		P[0][8] = XYM * cos(THETA + XYR) + X0; 
		P[1][8] = XYM * sin(THETA + XYR) + Y0;
		P[2][8] = pow(((RH + SL + LC) * (RH + SL + LC) + RD * RD), 0.5);
		P[0][9] = XF;
		P[1][9] = YF;
		P[2][9] = SHT2 + CD;

		P[3][0] = 0;         
		P[3][1] = 0;                     
		P[3][2] = -RD;     
		P[3][7] = P[3][2]; 
		P[3][8] = XYM;    
		P[3][9] = WL;

		TM1 = int(pow((sqrt((P[0][2]-P[0][0]) *(P[0][2]-P[0][0]) + (P[1][2]-P[1][0]) * (P[1][2]-P[1][0]))*32/(RAWSPDXY*SPD_RD)),1.0/3)*40);  //need max of two Eric170908
		TM4 = int(pow((sqrt( (P[0][7]-P[0][9]) *(P[0][7]-P[0][9]) + (P[1][7]-P[1][9]) * (P[1][7]-P[1][9]))*32/(RAWSPDXY*SPD_TRAJ)),1.0/3)*40);
		TZ1 = int(pow((abs(P[2][1])/(RAWSPDZ*SPD_RH)/32),1.0/3)*40);
		TZ4 = int(pow((abs(P[2][7]-P[2][2])/(RAWSPDZ*SPD_SL)/32),1.0/3)*40);

		INDX2 = 0;
		while (INDX2 <= XYDLY)
		{
			PNXY[INDX2] = 0; 
			PNX[INDX2] = X0; 
			PNY[INDX2] = Y0; 
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1;
		while (INDX2 <= TM1)
		{
			INDX1= (double)INDX2/(TM1+1);
			PNX[INDX2+XYDLY] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[0][2]-P[0][0])+P[0][0];
			PNY[INDX2+XYDLY] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[1][2]-P[1][0])+P[1][0];
			PNXY[INDX2+XYDLY] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[3][2]-P[3][0])+P[3][0];
			if (INDX1 < 0.25)
			{
				PNX[INDX2+XYDLY] = 32*INDX1*INDX1*INDX1/6*(P[0][2]-P[0][0])+P[0][0];
				PNY[INDX2+XYDLY] = 32*INDX1*INDX1*INDX1/6*(P[1][2]-P[1][0])+P[1][0];
				PNXY[INDX2+XYDLY] = 32*INDX1*INDX1*INDX1/6*(P[3][2]-P[3][0])+P[3][0];
			}
			if (INDX1 > 0.75)
			{
				PNX[INDX2+XYDLY] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[0][2]-P[0][0])+P[0][0]; 
				PNY[INDX2+XYDLY] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[1][2]-P[1][0])+P[1][0];
				PNXY[INDX2+XYDLY] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[3][2]-P[3][0])+P[3][0];
			}
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1; TMXY0 = -1; 
		while (INDX2 <= TM4)
		{
			INDX1= (double)INDX2/(TM4+1);
			PNX[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[0][9]-P[0][7])+P[0][7];
			PNY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[1][9]-P[1][7])+P[1][7];
			PNXY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[3][9]-P[3][7])+P[3][7];

			if (INDX1 < 0.25)
			{
				PNX[INDX2+XYDLY+TM1] = 32*INDX1*INDX1*INDX1/6*(P[0][9]-P[0][7])+P[0][7]; 
				PNY[INDX2+XYDLY+TM1] = 32*INDX1*INDX1*INDX1/6*(P[1][9]-P[1][7])+P[1][7];
				PNXY[INDX2+XYDLY+TM1] = 32*INDX1*INDX1*INDX1/6*(P[3][9]-P[3][7])+P[3][7];
			}
			if (INDX1 > 0.75)
			{
				PNX[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[0][9]-P[0][7])+P[0][7]; 
				PNY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[1][9]-P[1][7])+P[1][7];
				PNXY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[3][9]-P[3][7])+P[3][7];
			}

			if (TMXY0 == -1)
				if (PNXY[INDX2+XYDLY+TM1] > XYM) 
					TMXY0 = INDX2+XYDLY+TM1-1;
			INDX2 = INDX2 + 1;
		}

		TMZ1 = 1;
		TMZ4 = TM1+XYDLY-((TZ4-1)/2); 
		if (TMZ4 < TMZ1 + TZ1 + 3) 
			TMZ4 = TMZ1 + TZ1 + 3;
		PNZZ[0] = 0;
		TMT = XYDLY + TM1 + TM4;

		INDX2 = 1;
		while (INDX2 <= TZ1)
		{
			INDX1 = (double)INDX2/(TZ1+1);
			PNZZ[INDX2] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*P[2][1];
			if (INDX1 < 0.25) 
				PNZZ[INDX2] = 32*INDX1*INDX1*INDX1/6*P[2][1];
			if (INDX1 > 0.75)
				PNZZ[INDX2] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*P[2][1];
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1;
		while (INDX2 <= TZ4)
		{
			INDX1= (double)INDX2/(TZ4+1);
			PNZZ[INDX2-1+TMZ4] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[2][7]-P[2][2])+P[2][2];
			if (INDX1 < 0.25)
				PNZZ[INDX2-1+TMZ4] = 32*INDX1*INDX1*INDX1/6*(P[2][7]-P[2][2])+P[2][2];
			if (INDX1 > 0.75)
				PNZZ[INDX2-1+TMZ4] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[2][7]-P[2][2])+P[2][2];
			INDX2 = INDX2 + 1;
		}

		INDX2 = TZ1 + TMZ1;
		while (INDX2 < TMZ4)
		{
			if (ARC_RD > 1) 
				PNZZ[INDX2] = pow((PNXY[INDX2]-PNXY[TZ1])/(PNXY[TMZ4]-PNXY[TZ1]) , ARC_RD)*(PNZZ[TMZ4]-PNZZ[TZ1])+PNZZ[TZ1];
			else 
				PNZZ[INDX2] = (1-pow(1-(PNXY[INDX2]-PNXY[TZ1])/(PNXY[TMZ4]-PNXY[TZ1]),1/ARC_RD))*(PNZZ[TMZ4]-PNZZ[TZ1])+PNZZ[TZ1];

			INDX2 = INDX2 + 1;
		}

		INDX2 = TMZ4 + TZ4;
		PNZZ[TMXY0] = P[2][8]; 
		while (INDX2 < TMXY0)
		{
			if (ARC_LPTP > 1) 
				PNZZ[INDX2] = pow((PNXY[INDX2]-PNXY[TZ4+TMZ4-1])/(PNXY[TMXY0]-PNXY[TZ4+TMZ4-1]) , ARC_LPTP)*(PNZZ[TMXY0]-PNZZ[TZ4+TMZ4-1])+PNZZ[TZ4+TMZ4-1];
			else 
				PNZZ[INDX2] = (1-pow(1-(PNXY[INDX2]-PNXY[TZ4+TMZ4-1])/(PNXY[TMXY0]-PNXY[TZ4+TMZ4-1]),1/ARC_LPTP))*(PNZZ[TMXY0]-PNZZ[TZ4+TMZ4-1])+PNZZ[TZ4+TMZ4-1];
			INDX2 = INDX2 + 1;
		}

		INDX2 = TMXY0 + 1;
		PNZZ[TMT+1] = P[2][9];
		PNXY[TMT+1] = P[3][9];
		while (INDX2 <= TMT)
		{
			if (ARC_TRAJ > 1)
				PNZZ[INDX2] = pow((PNXY[INDX2]-PNXY[TMXY0])/(PNXY[TMT+1]-PNXY[TMXY0]) , ARC_TRAJ)*(PNZZ[TMT+1]-PNZZ[TMXY0])+PNZZ[TMXY0];
			else 
				PNZZ[INDX2] = (1-pow(1-(PNXY[INDX2]-PNXY[TMXY0])/(PNXY[TMT+1]-PNXY[TMXY0]),1/ARC_TRAJ))*(PNZZ[TMT+1]-PNZZ[TMXY0])+PNZZ[TMXY0];

			INDX2 = INDX2 + 1;
		}

		LOOPT2 = TMT;
		if (PreSpd > 0) 
		{
			if (PreSrchDly > 6) 
				PreSrchDly = 6;
			if (PreSrchDly >= 0)
			{
				INDX2 = int(PreSrchDly); 
				while (INDX2 > 0)
				{
					PNZZ[TMT-INDX2+1] = PNZZ[TMT-INDX2]-PreSpd * (PreSrchDly - INDX2 + 1); 
					INDX2 = INDX2 - 1;
				}
				PNZZ[TMT+1] = PNZZ[TMT] - PreSpd; 
				LOOPT2 = TMT - PreSrchDly;
			}
		}

		INDX2 = 1;
		while (INDX2 <= TMT)
		{
			ZT[INDX2] = FILTER1 * PNZZ[INDX2] + (1 - FILTER1) / 2 * (PNZZ[INDX2-1] + PNZZ[INDX2+1]);
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1;
		while (INDX2 <= TMT)
		{
			PNZZ[INDX2] = ZT[INDX2];
			INDX2 = INDX2 + 1;
		}

		LOOPT = TMT; LOOPT1 = TMXY0;

		INDX2 = 1;
		while (INDX2 <= TMT)
		{
			m_adMZ1[0][INDX2-1] = (PNX[INDX2] - PNX[INDX2-1]) / theServo.GetResolXY();
			m_adMZ1[1][INDX2-1] = (PNY[INDX2] - PNY[INDX2-1]) / theServo.GetResolXY();
			m_adMZ1[2][INDX2-1] = PNZZ[INDX2] - PNZZ[INDX2-1];
			INDX2 = INDX2 + 1;
		}

		while (INDX2 < 300)
		{
			m_adMZ1[0][INDX2 - 1] = 0;
			m_adMZ1[1][INDX2 - 1] = 0;
			m_adMZ1[2][INDX2 - 1] = 0;
			INDX2 = INDX2 + 1;
		}

		m_adMZ1[2][299] = LOOPT;
		m_adMZ1[2][298] = LOOPT1;
		m_adMZ1[2][297] = LOOPT2;
		m_adMZ1[2][296] = 1;

		//General
		m_adMZ1[2][251] = LH;
		m_adMZ1[2][252] = LC;
		m_adMZ1[2][253] = Alfa;
		m_adMZ1[2][254] = XYDLY;
		//kink1
		m_adMZ1[2][255] = NECK;
		m_adMZ1[2][256] = RDA;
		m_adMZ1[2][257] = NECKADJ;  
		//kink2
		m_adMZ1[2][258] = FL;
		m_adMZ1[2][259] = HL_ADJ;
		m_adMZ1[2][260] = FL_ADJ;
		m_adMZ1[2][261] = AC;
		//kink3
		m_adMZ1[2][262] = 1;   //belle ht
		m_adMZ1[2][263] = 1;   //belle dist
		m_adMZ1[2][264] = 1;   //belle angle
		m_adMZ1[2][265] = 1;   //belle after 2nd kink
		//Landing
		m_adMZ1[2][266] = SL_ADJ;
		m_adMZ1[2][267] = XYM;
		m_adMZ1[2][268] = 1;   //ctact lvl offset
		m_adMZ1[2][269] = 1;   //pull dist h
		m_adMZ1[2][270] = 1;   //pull dist v
		m_adMZ1[2][271] = 1;   //pull dist v time
		//S and A
		m_adMZ1[2][272] = SPD_RH;
		m_adMZ1[2][273] = SPD_RD;
		m_adMZ1[2][274] = SPD_BELHT;
		m_adMZ1[2][275] = SPD_BELDIST;
		m_adMZ1[2][276] = SPD_HL;
		m_adMZ1[2][277] = SPD_RD2;
		m_adMZ1[2][278] = SPD_SL;
		m_adMZ1[2][279] = SPD_TRAJ;
		m_adMZ1[2][280] = ARC_RD;
		m_adMZ1[2][281] = ARC_RD2;
		m_adMZ1[2][282] = ARC_BEL;
		m_adMZ1[2][283] = ARC_LPTP;
		m_adMZ1[2][284] = ARC_TRAJ;

		m_adMZ1[2][295] = 1;
		m_adMZ1[2][299] = LOOPT;
		m_adMZ1[2][298] = LOOPT1;
		m_adMZ1[2][297] = LOOPT2;

		if (theDebug.IsDebugStepMotion())
			m_adMZ1[2][296] = 1;
		else
			m_adMZ1[2][296] = 1;
	}
	else
	{
		HL = pow(((FL-Delta)*(FL-Delta)+KINKHT*KINKHT),0.5);
		SL = pow(((WL-FL)*(WL-FL)+(LH-CD+KINKHT)*(LH-CD+KINKHT)),0.5);
		CA = atan((LH-CD+KINKHT)/(WL-FL))*180/PI;
		double RD2 = HL*sin((CA+AC)/180*PI);

		P[0][0] = X0;
		P[1][0] = Y0;
		P[2][0] = 0;
		P[0][1] = X0;
		P[1][1] = Y0;

        P[2][1] = L;
		if (0 == stLoopAcsMode.iRDadjUnit) 
			P[2][1] = P[2][1]+RDA*(L-RH); 
		else 
			P[2][1] = P[2][1]+RDA; 

		P[0][2] = -RD*cos(THETA+LA1)+X0;
		P[1][2] = -RD*sin(THETA+LA1)+Y0; 

		if (0 == stLoopAcsMode.iNeckHtUnit)
			P[2][2] = RH+(L-RH)*NECKADJ; 
		else
			P[2][2] = RH+NECKADJ; 

		P[0][5] = P[0][2];
		P[1][5] = P[1][2]; 
		if (0 == stLoopAcsMode.iHLadjUnit)
			P[2][5] = P[2][2]+(HL-HL*cos((AC+CA)/180*PI))*HL_ADJ+HL; 
		else
			P[2][5] = P[2][2]+HL+ HL_ADJ;
		P[0][6] = P[0][5]-RD2*cos(THETA+LA2); 
		P[1][6] = P[1][5]-RD2*sin(THETA+LA2);
		if (0 == stLoopAcsMode.iFLadjUnit) 
			P[2][6] = P[2][2]+(HL)*cos((CA+AC)/180*PI)+FL_ADJ*((HL)-(HL)*cos((CA+AC)/180*PI));
		else
			P[2][6] = P[2][2]+(HL)*cos((CA+AC)/180*PI)+FL_ADJ; 

		P[0][7] = P[0][6];
		P[1][7] = P[1][6];

		if (0 == stLoopAcsMode.iSLadjUnit)
			P[2][7] = P[2][2]+(1+SL_ADJ)*SL; 
		else 
			P[2][7] = P[2][2]+SL+SL_ADJ;

		P[0][8] = XYM*cos((THETA+XYR)/180*PI)+X0; 
		P[1][8] = XYM*sin((THETA+XYR)/180*PI)+Y0; 
		P[2][8] = pow(((RH+HL*cos((CA+AC)/180*PI)+SL+LC)*(RH+HL*cos((CA+AC)/180*PI)+SL+LC)+(RD+RD2)*(RD+RD2)),0.5);
		P[0][9] = XF;
		P[1][9] = YF;
		P[2][9] = SHT2+CD;

		P[3][0] = 0;         
		P[3][1] = 0;                      
		P[3][2] = -RD;     
		P[3][5] = P[3][2]; 
		P[3][6] = -RD-RD2; 
		P[3][7] = P[3][6]; 
		P[3][8] = XYM;    
		P[3][9] = WL;

		TM1 = int(pow((sqrt((P[0][2]-P[0][0]) *(P[0][2]-P[0][0]) + (P[1][2]-P[1][0]) * (P[1][2]-P[1][0]))*32/(RAWSPDXY*SPD_RD)),1.0/3)*40);
		TM3 = int(pow((sqrt((P[0][6]-P[0][5]) *(P[0][6]-P[0][5]) + (P[1][6]-P[1][5]) * (P[1][6]-P[1][5]))*32/(RAWSPDXY*SPD_RD2)),1.0/3)*40);
		TM4 = int(pow((sqrt((P[0][7]-P[0][9]) *(P[0][7]-P[0][9]) + (P[1][7]-P[1][9]) * (P[1][7]-P[1][9]))*32/(RAWSPDXY*SPD_TRAJ)),1.0/3)*40);

		TZ1 = int(pow((abs(P[2][1])/(RAWSPDZ*SPD_RH)/32),1.0/3)*40);

		TZ3 = int(pow((abs(P[2][5]-P[2][2])/(RAWSPDZ*SPD_HL)/32),1.0/3)*40);
		TZ4 = int(pow((abs(P[2][7]-P[2][6])/(RAWSPDZ*SPD_SL)/32),1.0/3)*40);

		INDX2 = 0;
		while (INDX2 <= XYDLY)
		{
			PNXY[INDX2] = 0; PNX[INDX2] = X0; PNY[INDX2] = Y0; 
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1;
		while (INDX2 <= TM1)
		{
			INDX1 = (double)INDX2/(TM1+1);
			PNX[INDX2+XYDLY] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[0][2]-P[0][0])+P[0][0];
			PNY[INDX2+XYDLY] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[1][2]-P[1][0])+P[1][0];
			PNXY[INDX2+XYDLY] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[3][2]-P[3][0])+P[3][0];
			if (INDX1 < 0.25)
			{
				PNX[INDX2+XYDLY] = 32*INDX1*INDX1*INDX1/6*(P[0][2]-P[0][0])+P[0][0];
				PNY[INDX2+XYDLY] = 32*INDX1*INDX1*INDX1/6*(P[1][2]-P[1][0])+P[1][0];
				PNXY[INDX2+XYDLY] = 32*INDX1*INDX1*INDX1/6*(P[3][2]-P[3][0])+P[3][0];
			}
			if (INDX1 > 0.75)
			{
				PNX[INDX2+XYDLY] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[0][2]-P[0][0])+P[0][0]; 
				PNY[INDX2+XYDLY] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[1][2]-P[1][0])+P[1][0];
				PNXY[INDX2+XYDLY] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[3][2]-P[3][0])+P[3][0];
			}
			INDX2= INDX2 + 1;
		}
		INDX2 = 1;
		while (INDX2 <= TM3)
		{
			INDX1 = (double)INDX2/(TM3+1);
			PNX[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[0][6]-P[0][5])+P[0][5];
			PNY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[1][6]-P[1][5])+P[1][5];
			PNXY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[3][6]-P[3][5])+P[3][5];
			if (INDX1 < 0.25)
			{
				PNX[INDX2+XYDLY+TM1] = 32*INDX1*INDX1*INDX1/6*(P[0][6]-P[0][5])+P[0][5]; 
				PNY[INDX2+XYDLY+TM1] = 32*INDX1*INDX1*INDX1/6*(P[1][6]-P[1][5])+P[1][5];
				PNXY[INDX2+XYDLY+TM1] = 32*INDX1*INDX1*INDX1/6*(P[3][6]-P[3][5])+P[3][5];
			}
			if (INDX1>0.75) 
			{
				PNX[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[0][6]-P[0][5])+P[0][5]; 
				PNY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[1][6]-P[1][5])+P[1][5];
				PNXY[INDX2+XYDLY+TM1] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[3][6]-P[3][5])+P[3][5];

			}
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1; TMXY0 = -1; 
		while (INDX2 <= TM4)
		{
			INDX1= (double)INDX2/(TM4+1);
			PNX[INDX2+XYDLY+TM1+TM3] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[0][9]-P[0][7])+P[0][7];
			PNY[INDX2+XYDLY+TM1+TM3] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[1][9]-P[1][7])+P[1][7];
			PNXY[INDX2+XYDLY+TM1+TM3] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[3][9]-P[3][7])+P[3][7];
			if (INDX1 < 0.25)
			{
				PNX[INDX2+XYDLY+TM1+TM3] = 32*INDX1*INDX1*INDX1/6*(P[0][9]-P[0][7])+P[0][7];
				PNY[INDX2+XYDLY+TM1+TM3] = 32*INDX1*INDX1*INDX1/6*(P[1][9]-P[1][7])+P[1][7];
				PNXY[INDX2+XYDLY+TM1+TM3] = 32*INDX1*INDX1*INDX1/6*(P[3][9]-P[3][7])+P[3][7];
			}
			if (INDX1 > 0.75)
			{
				PNX[INDX2+XYDLY+TM1+TM3] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[0][9]-P[0][7])+P[0][7];
				PNY[INDX2+XYDLY+TM1+TM3] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[1][9]-P[1][7])+P[1][7];
				PNXY[INDX2+XYDLY+TM1+TM3] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[3][9]-P[3][7])+P[3][7];
			}

			if (TMXY0 == -1)
				if (PNXY[INDX2+XYDLY+TM1+TM3] > XYM) 
					TMXY0 = INDX2+XYDLY+TM1+TM3-1;

			INDX2 = INDX2 + 1;
		}

		TMZ1 = 1;
		TMZ3 = TM1+XYDLY-int((TZ3-1)/2); 
		if (TMZ3 < TMZ1+TZ1+3) 
			TMZ3 = TMZ1+TZ1+3;
		TMZ4 = TM1+TM3+XYDLY-int((TZ4-1)/2); 
		if (TMZ4 < TMZ3+TZ3+3) 
			TMZ4 = TMZ3+TZ3+3;
		PNZZ[0] = 0; TMT = XYDLY+TM1+TM3+TM4;

		INDX2 = 1;
		while (INDX2 <= TZ1)
		{
			INDX1 = (double)INDX2/(TZ1+1);
			PNZZ[INDX2] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*P[2][1];
			if (INDX1 < 0.25) 
				PNZZ[INDX2] = 32*INDX1*INDX1*INDX1/6*P[2][1];
			if (INDX1 > 0.75)
				PNZZ[INDX2] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*P[2][1];
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1;
		while (INDX2 <= TZ3)
		{
			INDX1 = (double)INDX2/(TZ3+1);
			PNZZ[INDX2-1+TMZ3] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[2][5]-P[2][2])+P[2][2];
			if (INDX1 < 0.25)
				PNZZ[INDX2-1+TMZ3] = 32*INDX1*INDX1*INDX1/6*(P[2][5]-P[2][2])+P[2][2];
			if (INDX1 > 0.75)
				PNZZ[INDX2-1+TMZ3] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[2][5]-P[2][2])+P[2][2];
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1;
		while (INDX2 <= TZ4)
		{
			INDX1 = (double)INDX2/(TZ4+1);
			PNZZ[INDX2-1+TMZ4] = 32*(INDX1*INDX1/4-INDX1*INDX1*INDX1/6-INDX1/16+1.0/192)*(P[2][7]-P[2][6])+P[2][6];
			if (INDX1 < 0.25)
				PNZZ[INDX2-1+TMZ4] = 32*INDX1*INDX1*INDX1/6*(P[2][7]-P[2][6])+P[2][6];
			if (INDX1 > 0.75)
				PNZZ[INDX2-1+TMZ4] = 32*(INDX1*INDX1*INDX1/6-INDX1*INDX1/2+INDX1/2-26.0/192)*(P[2][7]-P[2][6])+P[2][6];
			INDX2 = INDX2 + 1;
		}

		INDX2 = TZ1+TMZ1; 
		while (INDX2 < TMZ3)
		{
			if (ARC_RD > 1) 
				PNZZ[INDX2] = pow((PNXY[INDX2]-PNXY[TZ1])/(PNXY[TMZ3]-PNXY[TZ1]), ARC_RD) * (PNZZ[TMZ3] - PNZZ[TZ1]) + PNZZ[TZ1];
			else 
				PNZZ[INDX2] = (1 - pow(1-(PNXY[INDX2]-PNXY[TZ1])/(PNXY[TMZ3]-PNXY[TZ1]), 1/ARC_RD)) * (PNZZ[TMZ3] - PNZZ[TZ1]) + PNZZ[TZ1];
			INDX2 = INDX2 + 1;
		}

		INDX2 = TMZ3 + TZ3; 
		while (INDX2 < TMZ4)
		{
			if (ARC_RD2 > 1) 
				PNZZ[INDX2] = pow((PNXY[INDX2]-PNXY[TZ3+TMZ3-1])/(PNXY[TMZ4]-PNXY[TZ3+TMZ3-1]), ARC_RD2) * (PNZZ[TMZ4] - PNZZ[TZ3+TMZ3-1]) + PNZZ[TZ3+TMZ3-1];
			else 
				PNZZ[INDX2] = (1 - pow(1-(PNXY[INDX2]-PNXY[TZ3+TMZ3-1])/(PNXY[TMZ4]-PNXY[TZ3+TMZ3-1]), 1/ARC_RD2)) * (PNZZ[TMZ4] - PNZZ[TZ3+TMZ3-1]) + PNZZ[TZ3+TMZ3-1];
			INDX2 = INDX2 + 1;
		}

		INDX2 = TMZ4 + TZ4;
		PNZZ[TMXY0] = P[2][8]; 
		while (INDX2 < TMXY0)
		{
			if (ARC_LPTP > 1) 
				PNZZ[INDX2] = pow((PNXY[INDX2]-PNXY[TZ4+TMZ4-1])/(PNXY[TMXY0]-PNXY[TZ4+TMZ4-1]) , ARC_LPTP)*(PNZZ[TMXY0]-PNZZ[TZ4+TMZ4-1])+PNZZ[TZ4+TMZ4-1];
			else 
				PNZZ[INDX2] = (1-pow(1-(PNXY[INDX2]-PNXY[TZ4+TMZ4-1])/(PNXY[TMXY0]-PNXY[TZ4+TMZ4-1]),1/ARC_LPTP))*(PNZZ[TMXY0]-PNZZ[TZ4+TMZ4-1])+PNZZ[TZ4+TMZ4-1];
			INDX2 = INDX2 + 1;
		}

		INDX2 = TMXY0 + 1;
		PNZZ[TMT+1] = P[2][9];
		PNXY[TMT+1] = P[3][9];
		while (INDX2 <= TMT)
		{
			if (ARC_TRAJ > 1) 
				PNZZ[INDX2] = pow((PNXY[INDX2]-PNXY[TMXY0])/(PNXY[TMT+1]-PNXY[TMXY0]) , ARC_TRAJ)*(PNZZ[TMT+1]-PNZZ[TMXY0])+PNZZ[TMXY0];
			else 
				PNZZ[INDX2] = (1-pow(1-(PNXY[INDX2]-PNXY[TMXY0])/(PNXY[TMT+1]-PNXY[TMXY0]),1/ARC_TRAJ))*(PNZZ[TMT+1]-PNZZ[TMXY0])+PNZZ[TMXY0];
			INDX2 = INDX2 + 1;
		}

		LOOPT2 = TMT;
		if (PreSpd > 0) 
		{
			if (PreSrchDly > 6) 
				PreSrchDly = 6;
			if (PreSrchDly >= 0)
			{
				INDX2 = int(PreSrchDly); 
				while (INDX2 > 0)
				{
					PNZZ[TMT-INDX2+1] = PNZZ[TMT-INDX2] - PreSpd * (PreSrchDly - INDX2 + 1);
					INDX2 = INDX2 - 1;
				}
				PNZZ[TMT+1] = PNZZ[TMT] - PreSpd; 
				LOOPT2 = TMT - PreSrchDly;
			}
		}

		INDX2 = 1;
		while (INDX2 <= TMT)
		{
			ZT[INDX2] = FILTER1 * PNZZ[INDX2] + (1 - FILTER1) / 2 * (PNZZ[INDX2-1] + PNZZ[INDX2+1]);
			INDX2 = INDX2 + 1;
		}

		INDX2 = 1;
		while (INDX2 <= TMT)
		{
			PNZZ[INDX2] = ZT[INDX2];
			INDX2 = INDX2 + 1;
		}

		LOOPT = TMT;
		LOOPT1 = TMXY0;
		INDX2 = 1;
		while (INDX2 <= TMT)
		{
			m_adMZ1[0][INDX2-1] = (PNX[INDX2] - PNX[INDX2-1])/theServo.GetResolXY();
			m_adMZ1[1][INDX2-1] = (PNY[INDX2] - PNY[INDX2-1])/theServo.GetResolXY();
			m_adMZ1[2][INDX2-1] = PNZZ[INDX2] - PNZZ[INDX2-1];
			INDX2 = INDX2 + 1;
		}

		m_adMZ1[2][299] = LOOPT;
		m_adMZ1[2][298] = LOOPT1;
		m_adMZ1[2][297] = LOOPT2;
		m_adMZ1[2][296] = 1;
		m_adMZ1[2][295] = 2;
		while (INDX2 < 300)
		{
			m_adMZ1[0][INDX2 - 1] = 0;
			m_adMZ1[1][INDX2 - 1] = 0;
			m_adMZ1[2][INDX2 - 1] = 0;
			INDX2 = INDX2 + 1;
		}

		//General
		m_adMZ1[2][251] = LH;
		m_adMZ1[2][252] = LC;
		m_adMZ1[2][253] = Alfa;
		m_adMZ1[2][254] = XYDLY;
		
		//kink1
		m_adMZ1[2][255] = NECK;
		m_adMZ1[2][256] = RDA;
		m_adMZ1[2][257] = NECKADJ;   //neck height adj
		
		//kink2
		m_adMZ1[2][258] = FL;
		m_adMZ1[2][259] = HL_ADJ;
		m_adMZ1[2][260] = FL_ADJ;
		m_adMZ1[2][261] = AC;
		
		//kink3
		m_adMZ1[2][262] = 1;   //belle ht
		m_adMZ1[2][263] = 1;   //belle dist
		m_adMZ1[2][264] = 1;   //belle angle
		m_adMZ1[2][265] = 1;   //belle after 2nd kink
		
		//Landing
		m_adMZ1[2][266] = SL_ADJ;
		m_adMZ1[2][267] = XYM;
		m_adMZ1[2][268] = 1;   //ctact lvl offset
		m_adMZ1[2][269] = 1;   //pull dist h
		m_adMZ1[2][270] = 1;   //pull dist v
		m_adMZ1[2][271] = 1;   //pull dist v time
		
		//Spd and Arc
		m_adMZ1[2][272] = SPD_RH;
		m_adMZ1[2][273] = SPD_RD;
		m_adMZ1[2][274] = SPD_BELHT;
		m_adMZ1[2][275] = SPD_BELDIST;
		m_adMZ1[2][276] = SPD_HL;
		m_adMZ1[2][277] = SPD_RD2;
		m_adMZ1[2][278] = SPD_SL;
		m_adMZ1[2][279] = SPD_TRAJ;
		m_adMZ1[2][280] = ARC_RD;
		m_adMZ1[2][281] = ARC_RD2;
		m_adMZ1[2][282] = ARC_BEL;
		m_adMZ1[2][283] = ARC_LPTP;
		m_adMZ1[2][284] = ARC_TRAJ;

		for (int i = 0; i< 3; i++)
		{
			m_adMZ1[i][299] = LOOPT;
			m_adMZ1[i][298] = LOOPT1;
			m_adMZ1[i][297] = LOOPT2;
			if (theDebug.IsDebugStepMotion())
				m_adMZ1[i][296] = 1;
			else
				m_adMZ1[i][296] = 1;
		}
	}

	CollectData(pstWireInfo, &stLoopPara,
				pstRef1->lCtactLvl, pstRef2->lCtactLvl,
				theBondPara.GetRcd()->nSrchHeight2, &m_adMZ1[2][200]);
}

int CBond::CalcWireInfo(short nWireNum, TBL_POS stCurTblPos,
			ST_WIRE_INFO *pstWireInfo, CString &sDebugInfo, bool bDebug)
{
	const MACHINE_DATA *pstMcData = theMcData.GetRcd();
	const PGM_RECORD *pstPgm = thePgm.GetRcd();
	const WIRE_RECORD *pstWire = theWire.GetRcd(nWireNum);
	if (bDebug) sDebugInfo.AppendFormat(_T("Wire Num: %d\r\n"), nWireNum);

	const MATRIX_RECORD *pstMat1 = theMatrix.GetRcd(pstWire->nRefNum[0]);
	const MATRIX_RECORD *pstMat2 = theMatrix.GetRcd(pstWire->nRefNum[1]);
	if ((pstMat1->nVisionStatus != VISION_SUCCESS) ||
		(pstMat2->nVisionStatus != VISION_SUCCESS))
	{
		return BOND_NO_ALIGNMENT_ERROR;
	}

	const REF_RECORD *pstRef1 = theRef.GetRcd(pstWire->nRefNum[0]);
	const REF_RECORD *pstRef2 = theRef.GetRcd(pstWire->nRefNum[1]);
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("Ref Num: [%d, %d]\r\n"),
			pstWire->nRefNum[0], pstWire->nRefNum[1]);
		sDebugInfo.AppendFormat(_T("PR Status: [%d, %d]\r\n"),
			pstMat1->nVisionStatus, pstMat2->nVisionStatus);
	}

	//Calculate the bond point position
	if (m_nBondMode == BONDMODE_CORRECT)
		theWire.CalcBondPoint(nWireNum, &pstWireInfo->stBondPos[0], &pstWireInfo->stBondPos[1], true);
	else
		theWire.CalcBondPoint(nWireNum, &pstWireInfo->stBondPos[0], &pstWireInfo->stBondPos[1]);

	bool bCtactValid = false;
	int nWireCtactLvl;
	short nCtactBufferIndex = thePgm.CalcCtactBufferIndex(nWireNum,
								m_nCurRow, m_nCurCol, theMHS.GetCurBondUnit());
	long lTempCtactLvl1, lTempCtactLvl2;
	long lCtactLvl1, lCtactLvl2;
	lCtactLvl1 = lTempCtactLvl1 = pstRef1->lCtactLvl;
	lCtactLvl2 = lTempCtactLvl2 = pstRef2->lCtactLvl;

	bCtactValid = theCtact.GetCtactLvl(nCtactBufferIndex, FIRST_BOND, &nWireCtactLvl);
	if (bCtactValid) lCtactLvl1 = nWireCtactLvl;

	bCtactValid = theCtact.GetCtactLvl(nCtactBufferIndex, SECOND_BOND, &nWireCtactLvl);
	if (bCtactValid) lCtactLvl2 = nWireCtactLvl;

	if (thePgm.GetMultiBtos() == BTOS_DIR_ENABLE_X)
	{
		pstWireInfo->stBondPos[0].dx += pstRef1->stBtos.dx;
		pstWireInfo->stBondPos[0].dy += pstRef1->stBtos.dy;
		long lCtactDiff = lTempCtactLvl1 - lCtactLvl1;
		pstWireInfo->stBondPos[0].dx -= lCtactDiff * theServo.GetResolZ() * tan(NEW_OPTICS_X_ANGLE) / theServo.GetResolXY();

		pstWireInfo->stBondPos[1].dx += pstRef2->stBtos.dx;
		pstWireInfo->stBondPos[1].dy += pstRef2->stBtos.dy;
		lCtactDiff = lTempCtactLvl2 - lCtactLvl2;
		pstWireInfo->stBondPos[1].dx -= lCtactDiff * theServo.GetResolZ() * tan(NEW_OPTICS_X_ANGLE) / theServo.GetResolXY();
	}
	else if (thePgm.GetMultiBtos() == BTOS_DIR_ENABLE_Y)
	{
		pstWireInfo->stBondPos[0].dx += pstRef1->stBtos.dx;
		pstWireInfo->stBondPos[0].dy += pstRef1->stBtos.dy;
		long lCtactDiff = lTempCtactLvl1 - lCtactLvl1;
		pstWireInfo->stBondPos[0].dy += lCtactDiff * theServo.GetResolZ() * tan(NEW_OPTICS_Y_ANGLE) / theServo.GetResolXY();

		pstWireInfo->stBondPos[1].dx += pstRef2->stBtos.dx;
		pstWireInfo->stBondPos[1].dy += pstRef2->stBtos.dy;
		lCtactDiff = lTempCtactLvl2 - lCtactLvl2;
		pstWireInfo->stBondPos[1].dy += lCtactDiff * theServo.GetResolZ() * tan(NEW_OPTICS_Y_ANGLE) / theServo.GetResolXY();
	}
	else
	{
		double dLenCompen = CalcTransducerLenCompen(nWireNum);	//v1.1.288.T2 add
		pstWireInfo->stBondPos[0].dx += pstMcData->stBondTipOs.dx;
		pstWireInfo->stBondPos[0].dy += pstMcData->stBondTipOs.dy + dLenCompen;	//v1.1.288.T2 edit
		pstWireInfo->stBondPos[1].dx += pstMcData->stBondTipOs.dx;
		pstWireInfo->stBondPos[1].dy += pstMcData->stBondTipOs.dy + dLenCompen;	//v1.1.288.T2 add
	}

	if (pstMcData->bZeroBtosOptics == true)
	{
		double dCtactDiff = lCtactLvl2 - lCtactLvl1;
		pstWireInfo->stBondPos[1].dx += round(dCtactDiff / theServo.GetResolXY() * tan(18.0 / 180.0 * PI));
	}

	pstWireInfo->nBumpType = pstWire->nBumpType;

	// Calculate 2nd bond point offset for bsob or bbos
	ST_BUMP_WIRE stBumpWire;
	switch (pstWire->nBumpType)	//v3.1.T88 add
	{
	case BUMP_BOS:
	{
		theBallPara.GetBbosWire(&stBumpWire);
		pstWireInfo->stBondPos[1] = CalcPosAlongWire(
						pstWireInfo->stBondPos[1], pstWireInfo->stBondPos[0],
						-stBumpWire.nBondPointOffset / theServo.GetResolXY());	//v3.1.T372 add
		break;
	}
	case BUMP_SOB:
	case BUMP_BSB:	//v3.1.T103 add
	{
		theBallPara.GetBsobWire(&stBumpWire);
		pstWireInfo->stBondPos[1] = CalcPosAlongWire(
						pstWireInfo->stBondPos[1], pstWireInfo->stBondPos[0],
						-stBumpWire.nBondPointOffset / theServo.GetResolXY());	//v3.1.T372 add
		break;
	}
	}

	RoundPosition(&pstWireInfo->stBondPos[0]);
	RoundPosition(&pstWireInfo->stBondPos[1]);

	double dWireLen = Calc2PtDist(pstWireInfo->stBondPos[0], pstWireInfo->stBondPos[1]) * theServo.GetResolXY();
	if (bDebug) sDebugInfo.AppendFormat(_T("Wire Len: %.3fmm\r\n"), dWireLen/1000.0);

	if (theDebug.IsDebugWireCycleTime()
		&& ((theMcConfig.GetWireCycleTime() == 0)
			|| (theMcConfig.GetWireCycleTime() == 2)))
	{
		CString sInfo;
		sInfo.Format(_T("W%d_%.3fmm"), nWireNum, dWireLen/1000.0);
		theDebug.AddInfo(sInfo);
	}

	if (dWireLen > 8000)
		return BOND_WIRE_LENGTH_ERROR;
	else if (dWireLen == 0)	//V1.1.W254 edit
	{
		PopWireLenZeroError(nWireNum);
		return BOND_WIRE_LENGTH_ERROR;
	}
	double dCosTheta = fabs(pstWireInfo->stBondPos[1].dx
		- pstWireInfo->stBondPos[0].dx) * theServo.GetResolXY() / dWireLen;

	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();

	bool bTailBreakEnable = pstBondPara->bTailBreakEnable;
	if (pstWire->nBumpType > BUMP_NONE)
		bTailBreakEnable = stBumpWire.bTailBreakEnable;
	if (bTailBreakEnable)
	{
		pstWireInfo->nTailBreakDist  = pstBondPara->nTailBreakDist;
		pstWireInfo->nTailBreakSpeed = pstBondPara->nTailBreakSpeed;
	}
	else
	{
		pstWireInfo->nTailBreakDist  = 0;
		pstWireInfo->nTailBreakSpeed = 0;
	}

	pstWireInfo->nStandbyPower[0] = round((pstBondPara->fStandbyPower[0] + pstWire->fPrePowerOffset[0]) * 2.55);
	pstWireInfo->nPreCtactForce[0] = pstBondPara->nPreCtactForce[0] + pstWire->nPreForceOffset[0];
	pstWireInfo->nPreCtactTime[0] = pstBondPara->nPreCtactTime[0] + pstWire->nPreTimeOffset[0];
	pstWireInfo->nCtactTime[0] = pstBondPara->nCtactTime[0] + pstWire->nCtactTimeOffset[0];
	pstWireInfo->nCtactForce[0] = pstBondPara->nCtactForce[0] + pstWire->n1stCtactForceOffset;
	pstWireInfo->nCtactPower[0] = round((pstBondPara->fCtactPower[0] + pstWire->fCtactPowerOffset[0]) * 2.55);
	pstWireInfo->nBaseTime[0] = pstBondPara->nBaseTime[0] + pstWire->nTimeOffset[0];
	if (theMcConfig.IsExceedTime1(((nWireNum - 1) % 5) + 1, pstWireInfo->nBaseTime[0]))
		return BOND_EXCEED_TIME_1;
#if _SERVO_TYPE == 1	//v3.0.T485 add
	pstWireInfo->nBaseTime[0] += 1;	//v1.5.T43 edit, v1.5.T59 edit
#endif
	pstWireInfo->nPowerDelay[0] = pstBondPara->nPowerDelay[0];
	if (pstWireInfo->nPowerDelay[0] >= pstWireInfo->nBaseTime[0])
		pstWireInfo->nPowerDelay[0] = pstWireInfo->nBaseTime[0] - 1;

	pstWireInfo->nBaseForce[0] = pstBondPara->nBaseForce[0] + pstWire->nForceOffset[0];
	if (theMcConfig.IsExceedForce1(((nWireNum - 1) % 5) + 1, pstWireInfo->nBaseForce[0]))
		return BOND_EXCEED_FORCE_1;

#if _SERVO_TYPE < 2		//v3.1.T303 add
	if (pstWireInfo->nCtactTime[0] == 0)
		pstWireInfo->nCtactForce[0] = pstWireInfo->nBaseForce[0];
#endif

	pstWireInfo->nBasePower[0] = round((pstBondPara->fBasePower[0] + pstWire->fPowerOffset[0]) * 2.55);
	if (theMcConfig.IsExceedPower1(((nWireNum - 1) % 5) + 1, pstWireInfo->nBasePower[0]))
		return BOND_EXCEED_POWER_1;
	pstWireInfo->nReleaseTime[0] = pstBondPara->nReleaseTime[0];
	pstWireInfo->nReleaseForce[0] = pstBondPara->nReleaseForce[0];
	pstWireInfo->nReleasePower[0] = round(pstBondPara->fReleasePower[0] * 2.55);

	if (m_nBondMode == BONDMODE_CORRECT)
	{
		pstWireInfo->nStandbyPower[0] += round(pstBondPara->fCrtStandbyPowerOffset[0] * 2.55);	//v3.0.T265 add

		pstWireInfo->nBaseTime[0]  += pstBondPara->nCrtBondTimeOffset1;
		if (pstWireInfo->nBaseTime[0] < 0) pstWireInfo->nBaseTime[0] = 0;

		pstWireInfo->nBaseForce[0] += pstBondPara->nCrtBondForceOffset1;
		if (pstWireInfo->nBaseForce[0] < 0) pstWireInfo->nBaseForce[0] = 0;

		pstWireInfo->nBasePower[0] += round(pstBondPara->fCrtBondPowerOffset1 * 2.55);
	    
		if (pstWireInfo->nBasePower[0] < 0) pstWireInfo->nBasePower[0] = 0;
		else if (pstWireInfo->nBasePower[0] > 255) pstWireInfo->nBasePower[0] = 255;
	}

	if (pstBondPara->ch1stBondBoostEnable == 1)
	{
		if (pstWireInfo->nCtactTime[0] == 0)
		{
			pstWireInfo->n1stCtactBoostPower = pstWireInfo->nCtactPower[0];
			pstWireInfo->n1stCtactBoostTime  = 0;
		}
		else
		{
			if ((pstBondPara->chBoostDuration1 == 0) || (pstWireInfo->nCtactTime[0] < BOOST_DURATION_LONG))
				pstWireInfo->n1stCtactBoostTime = round(BOOST_DURATION_SHORT * 10.0);
			else
				pstWireInfo->n1stCtactBoostTime = round(BOOST_DURATION_LONG * 10.0);

			if (pstBondPara->chBoostAmplitude1 == 0)
				pstWireInfo->n1stCtactBoostPower = round(pstWireInfo->nCtactPower[0] * BOOST_AMPLITUDE_LOW);
			else if (pstBondPara->chBoostAmplitude1 == 1)
				pstWireInfo->n1stCtactBoostPower = round(pstWireInfo->nCtactPower[0] * BOOST_AMPLITUDE_MIDDLE);
			else
				pstWireInfo->n1stCtactBoostPower = round(pstWireInfo->nCtactPower[0] * BOOST_AMPLITUDE_HIGH);
		}

		if (pstWireInfo->nBaseTime[0] - pstWireInfo->nPowerDelay[0] == 0)
		{
			pstWireInfo->n1stBaseBoostPower = pstWireInfo->nBasePower[0];
			pstWireInfo->n1stBaseBoostTime  = 0;
		}
		else
		{
			if ((pstBondPara->chBoostDuration1 == 0) || (pstWireInfo->nBaseTime[0] - pstWireInfo->nPowerDelay[0] < BOOST_DURATION_LONG))
				pstWireInfo->n1stBaseBoostTime = round(BOOST_DURATION_SHORT * 10.0);
			else
				pstWireInfo->n1stBaseBoostTime = round(BOOST_DURATION_LONG * 10.0);

			if (pstBondPara->chBoostAmplitude1 == 0)
				pstWireInfo->n1stBaseBoostPower = round(pstWireInfo->nBasePower[0] * BOOST_AMPLITUDE_LOW);
			else if (pstBondPara->chBoostAmplitude1 == 1)
				pstWireInfo->n1stBaseBoostPower = round(pstWireInfo->nBasePower[0] * BOOST_AMPLITUDE_MIDDLE);
			else
				pstWireInfo->n1stBaseBoostPower = round(pstWireInfo->nBasePower[0] * BOOST_AMPLITUDE_HIGH);
		}
	}
	else
	{
		pstWireInfo->n1stCtactBoostPower = pstWireInfo->nCtactPower[0];
		pstWireInfo->n1stCtactBoostTime = 0;
		pstWireInfo->n1stBaseBoostPower = pstWireInfo->nBasePower[0];
		pstWireInfo->n1stBaseBoostTime = 0;
	}

	switch (pstWire->nBumpType)
	{
	case BUMP_SOB:
	case BUMP_BSB:	//v3.1.T103 add
	{
		pstWireInfo->nStandbyPower[1] = round((stBumpWire.fStandbyPower + pstWire->fBsobWirePreCtactPowerOffset) * 2.55);	//v3.1.T401 edit
		pstWireInfo->nPreCtactForce[1] = stBumpWire.nPreCtactForce + pstWire->nBsobWirePreCtactForceOffset;		//v3.1.T401 add
		pstWireInfo->nPreCtactTime[1] = stBumpWire.nPreCtactTime + pstWire->nBsobWirePreCtactTimeOffset;			//v3.1.T401 add
		pstWireInfo->nCtactTime[1] = stBumpWire.nCtactTime + pstWire->nBsobWireCtactTimeOffset;			//v3.1.T401 edit
		pstWireInfo->nCtactForce[1] = stBumpWire.nCtactForce + pstWire->nBsobWireCtactForceOffset;			//v3.1.T401 edit
		pstWireInfo->nCtactPower[1] = round((stBumpWire.fCtactPower + pstWire->fBsobWireCtactPowerOffset) * 2.55);	//v3.1.T401 edit

		//pstWireInfo->nCtactTime[1] = 0;
	    
		pstWireInfo->nBaseTime[1] = stBumpWire.nBaseTime + pstWire->nBsobWireBondTimeOffset;		//v3.1.T401 edit
		if (theMcConfig.IsExceedTime2(((nWireNum - 1) % 5) + 1, pstWireInfo->nBaseTime[1]))
			return BOND_EXCEED_TIME_2;
#if _SERVO_TYPE == 1	//v3.0.T485 add
		pstWireInfo->nBaseTime[1] += 1;	//v1.5.T43 edit, v1.5.T59 edit
#endif
		//pstWireInfo->nBasePower[1] = round((stBumpWire.fBasePower + pstWire->fBumpWirePowerOffset) * 2.55);	//v3.1.T401 delete
		pstWireInfo->nBasePower[1] = round((stBumpWire.fBasePower + pstWire->fBsobWireBondPowerOffset) * 2.55);	//v3.1.T401 edit
		if (theMcConfig.IsExceedPower2(((nWireNum - 1) % 5) + 1, pstWireInfo->nBasePower[1]))
			return BOND_EXCEED_POWER_2;
		pstWireInfo->nBaseForce[1] = stBumpWire.nBaseForce + pstWire->nBsobWireBondForceOffset;	//v3.1.T401 edit
		if (theMcConfig.IsExceedForce2(((nWireNum - 1) % 5) + 1, pstWireInfo->nBaseForce[1]))
			return BOND_EXCEED_FORCE_2;
	    
		//pstWireInfo->nReleaseTime[1] = 0;
		pstWireInfo->nReleaseTime[1] = stBumpWire.nReleaseTime + pstWire->nBsobWireRlsTimeOffset;		//v3.1.T401 edit
		pstWireInfo->nReleaseForce[1] = stBumpWire.nReleaseForce + pstWire->nBsobWireRlsForceOffset;	//v3.1.T401 edit
		pstWireInfo->nReleasePower[1] = round((stBumpWire.fReleasePower + pstWire->fBsobWireRlsPowerOffset) * 2.55);	//v3.1.T401 edit
		break;
	}
	default:
	{
		pstWireInfo->nStandbyPower[1] = round((pstBondPara->fStandbyPower[1] + pstWire->fPrePowerOffset[1]) * 2.55);
		pstWireInfo->nPreCtactForce[1] = pstBondPara->nPreCtactForce[1] + pstWire->nPreForceOffset[1];
		pstWireInfo->nPreCtactTime[1] = pstBondPara->nPreCtactTime[1] + pstWire->nPreTimeOffset[1];
		pstWireInfo->nCtactTime[1] = pstBondPara->nCtactTime[1] + pstWire->nCtactTimeOffset[1];
		pstWireInfo->nCtactForce[1] = pstBondPara->nCtactForce[1] + pstWire->n2ndCtactForceOffset;
		pstWireInfo->nCtactPower[1] = round((pstBondPara->fCtactPower[1] + pstWire->fCtactPowerOffset[1]) * 2.55);
		pstWireInfo->nBaseTime[1] = pstBondPara->nBaseTime[1] + pstWire->nTimeOffset[1];
		if (theMcConfig.IsExceedTime2(((nWireNum - 1) % 5) + 1, pstWireInfo->nBaseTime[1]))
			return BOND_EXCEED_TIME_2;
#if _SERVO_TYPE == 1	//v3.0.T485 add
		pstWireInfo->nBaseTime[1] += 1;	//v1.5.T43 edit, v1.5.T59 edit
#endif
		pstWireInfo->nPowerDelay[1] = pstBondPara->nPowerDelay[1];
		if (pstWireInfo->nPowerDelay[1] >= pstWireInfo->nBaseTime[1])
			pstWireInfo->nPowerDelay[1] = pstWireInfo->nBaseTime[1] - 1;
	    
		pstWireInfo->nBaseForce[1]    = pstBondPara->nBaseForce[1] + pstWire->nForceOffset[1];
		if (theMcConfig.IsExceedForce2(((nWireNum - 1) % 5) + 1, pstWireInfo->nBaseForce[1]))
			return BOND_EXCEED_FORCE_2;

		pstWireInfo->nBasePower[1] = round((pstBondPara->fBasePower[1] + pstWire->fPowerOffset[1]) * 2.55);
		if (theMcConfig.IsExceedPower2(((nWireNum - 1) % 5) + 1, pstWireInfo->nBasePower[1]))
			return BOND_EXCEED_POWER_2;
		pstWireInfo->nReleaseTime[1]  = pstBondPara->nReleaseTime[1];
		pstWireInfo->nReleaseForce[1] = pstBondPara->nReleaseForce[1];
		pstWireInfo->nReleasePower[1] = round(pstBondPara->fReleasePower[1] * 2.55);
	}
	}

	//Power, force factor
	pstWireInfo->nBaseForce[1] += round(pstBondPara->nForceFactor * dCosTheta);
	pstWireInfo->nBasePower[1] += round(pstBondPara->fPowerFactor * dCosTheta * 2.55);

	if (m_nBondMode == BONDMODE_CORRECT)
	{
		pstWireInfo->nStandbyPower[1] += round(pstBondPara->fCrtStandbyPowerOffset[1] * 2.55);	//v3.0.T265 add

		pstWireInfo->nBaseTime[1] += pstBondPara->nCrtBondTimeOffset2;
		if (pstWireInfo->nBaseTime[1] < 0) pstWireInfo->nBaseTime[1] = 0;
	    
		pstWireInfo->nBaseForce[1] += pstBondPara->nCrtBondForceOffset2;
		if (pstWireInfo->nBaseForce[1] < 0) pstWireInfo->nBaseForce[1] = 0;

		pstWireInfo->nBasePower[1] += round(pstBondPara->fCrtBondPowerOffset2 * 2.55);
		if (pstWireInfo->nBasePower[1] < 0) pstWireInfo->nBasePower[1] = 0;
		else if (pstWireInfo->nBasePower[1] > 255) pstWireInfo->nBasePower[1] = 255;
	}

	pstWireInfo->nWclTailClosePretrig     = pstBondPara->nWclTailClosePretrig;
	pstWireInfo->nWclLoopPeakClosePretrig = pstBondPara->nWclLoopPeakClosePretrig;
	pstWireInfo->nWclLoopPeakCloseTime    = pstBondPara->nWclLoopPeakCloseTime;
	pstWireInfo->nWcl1stBondOpenPretrig   = pstBondPara->nWcl1stBondOpenPretrig;
	pstWireInfo->nWcl2ndBondOpenPretrig   = pstBondPara->nWcl2ndBondOpenPretrig;
	pstWireInfo->nShiftSpeed              = pstBondPara->nShiftSpeed;

#if _SERVO_TYPE < 2		//v3.1.T303 add
	if (pstWireInfo->nCtactTime[1] == 0)
		pstWireInfo->nCtactForce[1] = pstWireInfo->nBaseForce[1];
#endif

	//v1.1.288.T13 add
	if (theDebug.IsDebugTransducer())
	{
		m_sTemp.Format(_T("CapWearoutPower1/2_%d/%d"),
							CalcCapiWearOutPower1(),
							CalcCapiWearOutPower2());
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
	}

	//v1.1.288.T11 edit
	CompensateCapiWearOutPower1(&pstWireInfo->nStandbyPower[0]);
	CompensateCapiWearOutPower1(&pstWireInfo->nCtactPower[0]);
	CompensateCapiWearOutPower1(&pstWireInfo->nBasePower[0]);
	CompensateCapiWearOutPower1(&pstWireInfo->nReleasePower[0]);
	CompensateCapiWearOutPower2(&pstWireInfo->nStandbyPower[1]);
	CompensateCapiWearOutPower2(&pstWireInfo->nCtactPower[1]);
	CompensateCapiWearOutPower2(&pstWireInfo->nBasePower[1]);
	CompensateCapiWearOutPower2(&pstWireInfo->nReleasePower[1]);

	pstWireInfo->nCtactPowerRamp[0]   = pstBondPara->nCtactPowerRamp[0];
	pstWireInfo->nCtactPowerRamp[1]   = pstBondPara->nCtactPowerRamp[1];
	pstWireInfo->nBasePowerRamp[0]    = pstBondPara->nBasePowerRamp[0];
	pstWireInfo->nBasePowerRamp[1]    = pstBondPara->nBasePowerRamp[1];
	pstWireInfo->nReleasePowerRamp[0] = pstBondPara->nReleasePowerRamp[0];
	pstWireInfo->nReleasePowerRamp[1] = pstBondPara->nReleasePowerRamp[1];

	USG_STATIC_PARA_RCD stUsgPara;
	theBondPara.GetUSGParaRcd(&stUsgPara);
	pstWireInfo->nFeedPower      = (short)(round(stUsgPara.nFeedPower * 2.55));
	pstWireInfo->nFeedPowerTime  = (short)stUsgPara.cFeedPowerTime;

	SERVO_SPEED_PROFILE stSpeedProfile;
	theServo.GetSpeedProfile(MOTOR_X, 0, &stSpeedProfile);
	double dJerk = stSpeedProfile.lJerk / theServo.GetResolXY()
					/ 1000.0 * pstBondPara->nShiftSpeed / 100.0;
	double dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY()
					* pstBondPara->nShiftSpeed / 100.0;
	double dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();

	double dMoveDist;
	short nMoveTime;
	pstWireInfo->stBondShiftPos1 = pstWireInfo->stBondPos[0];
	if (abs(pstBondPara->nBondShift1) > 0)
	{
		pstWireInfo->bBondShift1Enable = true;
		pstWireInfo->stBondShiftPos1.dx -= round(pstBondPara->nBondShift1
													/ theServo.GetResolXY());

		dMoveDist = abs(pstBondPara->nBondShift1) / theServo.GetResolXY();
		nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
		if (pstWireInfo->nBaseTime[0] < nMoveTime)
			pstWireInfo->nBaseTime[0] = nMoveTime;
	}
	else
		pstWireInfo->bBondShift1Enable = false;

	short nPreShift, nBondShift, nTailShift, nShiftSpeed;
	switch (pstWire->nBumpType)	//v3.1.T88 add
	{
	case BUMP_SOB:
	case BUMP_BSB:	//v3.1.T103 add
	{
		nPreShift   = stBumpWire.nPreShift;
		nBondShift  = stBumpWire.nBondShift;
		nTailShift  = stBumpWire.nTailShift;
		nShiftSpeed = stBumpWire.nShiftSpeed;
		break;
	}
	default:
	{
		nPreShift = pstBondPara->nPreShift2 + pstWire->nPreShiftOffset2;
		short n = ((nWireNum - 1) % thePgm.GetUnitTotalWire()) + 1;
		if ((n < WIRE_NUM)	//v3.1.T568 add
			&& (theMcConfig.GetBondShift2(n) != 0))	//v3.0.T219 add
			nBondShift = theMcConfig.GetBondShift2(n);
		else
			nBondShift = pstBondPara->nBondShift2;
		nTailShift = pstBondPara->nTailShift;
		nShiftSpeed = pstBondPara->nShiftSpeed;
	}
	}

	dJerk = stSpeedProfile.lJerk / theServo.GetResolXY()
				/ 1000.0 * nShiftSpeed / 100.0;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY()
				* nShiftSpeed / 100.0;
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();

	pstWireInfo->stBondShiftPos2 = pstWireInfo->stBondPos[1];

	if (abs(nBondShift) > 0)
	{
		pstWireInfo->stPullPos = CalcPosAlongWire(
						pstWireInfo->stBondPos[1], pstWireInfo->stBondPos[0],
						nBondShift / theServo.GetResolXY());	//v3.1.T372 add

		dMoveDist = abs(nBondShift) / theServo.GetResolXY();
		nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
		if (pstWireInfo->nBaseTime[1] < nMoveTime)
			pstWireInfo->nBaseTime[1] = nMoveTime;
	}
	else
		pstWireInfo->stPullPos = pstWireInfo->stBondShiftPos2;

	//v3.0.T403 add
	if (theMcConfig.IsRebondEnable())
		pstWireInfo->stRebondPos = CalcPosAlongWire(
						pstWireInfo->stBondPos[1], pstWireInfo->stBondPos[0],
						theMcConfig.GetRebondSpanLen() / theServo.GetResolXY());	//v3.1.T372 add

	if (pstMcData->bTscEnable)
		pstWireInfo->stPullPos.dy -= pstMcData->fTscFactor / theServo.GetResolXY();

	if (pstWire->ch1stBondScrubMode > 0)
	{
		SCRUB_PARA_RCD stScrubRcd;
		theBondPara.GetBondScrubRcd(FIRST_BOND, pstWire->ch1stBondScrubMode - 1, &stScrubRcd);
		pstBondPara->b1stBondScrubEnable = true;
		pstBondPara->ch1stScrubDirection = stScrubRcd.chScrubDirection;
		pstBondPara->n1stScrubAmplitude  = stScrubRcd.nScrubAmplitude;
		pstBondPara->n1stScrubCycle = stScrubRcd.nScrubCycle;
		pstBondPara->n1stScrubForce = stScrubRcd.nScrubForce;
		pstBondPara->f1stScrubPower = stScrubRcd.fScrubPower;
		pstBondPara->n1stScrubSpeed = stScrubRcd.nScrubSpeed;
	}
	else
		pstBondPara->b1stBondScrubEnable = false;

	if (pstBondPara->b1stBondScrubEnable == true)
	{
		pstWireInfo->b1stBondScrubEnable = true;

		short nScrubAngle = 0;
		if (pstBondPara->ch1stScrubDirection == 2)
		{
			pstWireInfo->st1stScrubAmp.dx = pstBondPara->n1stScrubAmplitude / theServo.GetResolXY();
			pstWireInfo->st1stScrubAmp.dy = pstBondPara->n1stScrubAmplitude / theServo.GetResolXY();
		}
		else
		{
			if (pstBondPara->ch1stScrubDirection == 0)
				nScrubAngle = 90;
			else
				nScrubAngle = 0;

			pstWireInfo->st1stScrubAmp = CalcPosAlongWire(
				pstWireInfo->stBondShiftPos1, pstWireInfo->stBondPos[1],
				pstBondPara->n1stScrubAmplitude / theServo.GetResolXY(), nScrubAngle);	//v3.1.T372 add

			pstWireInfo->st1stScrubAmp.dx -= pstWireInfo->stBondShiftPos1.dx;
			pstWireInfo->st1stScrubAmp.dy -= pstWireInfo->stBondShiftPos1.dy;
		}

		pstWireInfo->n1stScrubCycle = pstBondPara->n1stScrubCycle;
		pstWireInfo->n1stScrubForce = pstBondPara->n1stScrubForce;
		pstWireInfo->n1stScrubPower = round(pstBondPara->f1stScrubPower * 2.55);
		pstWireInfo->n1stScrubSpeed = pstBondPara->n1stScrubSpeed;

		switch (theMcConfig.GetMhsType())	//v3.0.152 edit
		{
		case MHS_VLED: case MHS_VLED_ELITE:
			 theServo.GetSpeedProfile(MOTOR_Y, 0, &stSpeedProfile);
			 break;
		default:	//v3.0.T254 add
			 theServo.GetSpeedProfile(MOTOR_X, 0, &stSpeedProfile);
		}

		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY()
					/ 1000.0 * pstBondPara->n1stScrubSpeed / 100.0;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY()
					* pstBondPara->n1stScrubSpeed / 100.0;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();

		dMoveDist = pstBondPara->n1stScrubAmplitude / theServo.GetResolXY();
		short nScrubT1 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
	    
		dMoveDist = pstBondPara->n1stScrubAmplitude * 2.0 / theServo.GetResolXY();
		short nScrubT2 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		if (pstBondPara->ch1stScrubDirection == 2)
			pstWireInfo->n1stScrubYDelay = nScrubT1 / 2;
		else
			pstWireInfo->n1stScrubYDelay = 0;

		pstWireInfo->n1stScrubTime = nScrubT1 * 2 + 
			(pstWireInfo->n1stScrubCycle * 2 - 1) * nScrubT2 + pstWireInfo->n1stScrubYDelay;
	}
	else
	{
		pstWireInfo->st1stScrubAmp.dx = 0.0;
		pstWireInfo->st1stScrubAmp.dy = 0.0;
		pstWireInfo->b1stBondScrubEnable = false;
		pstWireInfo->n1stScrubCycle      = 0;
		pstWireInfo->n1stScrubForce      = 0;
		pstWireInfo->n1stScrubPower      = 0;
		pstWireInfo->n1stScrubTime       = 0;
		pstWireInfo->n1stScrubSpeed      = 100;
	}

	if (pstWire->ch2ndBondScrubMode > 0)
	{
		SCRUB_PARA_RCD stScrubRcd;
		theBondPara.GetBondScrubRcd(SECOND_BOND, pstWire->ch2ndBondScrubMode - 1, &stScrubRcd);
		pstBondPara->b2ndBondScrubEnable = true;
		pstBondPara->ch2ndScrubDirection = stScrubRcd.chScrubDirection;
		pstBondPara->n2ndScrubAmplitude  = stScrubRcd.nScrubAmplitude;
		pstBondPara->n2ndScrubPosOffset1 = stScrubRcd.nScrubPosOffset1;
		pstBondPara->n2ndScrubPosOffset2 = stScrubRcd.nScrubPosOffset2;
		pstBondPara->n2ndScrubCycle = stScrubRcd.nScrubCycle;
		pstBondPara->n2ndScrubForce = stScrubRcd.nScrubForce;
		pstBondPara->f2ndScrubPower = stScrubRcd.fScrubPower;
		pstBondPara->n2ndScrubSpeed = stScrubRcd.nScrubSpeed;
	}
	else
		pstBondPara->b2ndBondScrubEnable = false;

	if ((pstWire->nBumpType != BUMP_SOB)
		&& (pstWire->nBumpType != BUMP_BSB)	//v3.1.T103 add
		&& (pstBondPara->b2ndBondScrubEnable))
	{
		pstWireInfo->b2ndBondScrubEnable = true;

		pstWireInfo->st2ndScrubOffset2 = pstWireInfo->stPullPos;
		if (abs(pstBondPara->n2ndScrubPosOffset2) > 0)
		{
			pstWireInfo->st2ndScrubPos = CalcPosAlongWire(
					pstWireInfo->st2ndScrubOffset2, pstWireInfo->stBondPos[0],
					pstBondPara->n2ndScrubPosOffset2 / theServo.GetResolXY());	//v3.1.T372 add
		}
		else
			pstWireInfo->st2ndScrubPos = pstWireInfo->st2ndScrubOffset2;

		if (abs(pstBondPara->n2ndScrubPosOffset1) > 0)
		{
			pstWireInfo->stPullPos = CalcPosAlongWire(
					pstWireInfo->st2ndScrubPos, pstWireInfo->stBondPos[0],
					pstBondPara->n2ndScrubPosOffset1 / theServo.GetResolXY());	//v3.1.T372 add
		}
		else
			pstWireInfo->stPullPos = pstWireInfo->st2ndScrubPos;

		short nScrubAngle = 0;
		if (pstBondPara->ch2ndScrubDirection == 0)
			nScrubAngle = 90;
		else
			nScrubAngle = 0;

		pstWireInfo->st2ndScrubAmp = CalcPosAlongWire(
			pstWireInfo->st2ndScrubPos, pstWireInfo->stBondPos[0],
			pstBondPara->n2ndScrubAmplitude / theServo.GetResolXY(), nScrubAngle);	//v3.1.T372 add

		pstWireInfo->st2ndScrubAmp.dx -= pstWireInfo->st2ndScrubPos.dx;
		pstWireInfo->st2ndScrubAmp.dy -= pstWireInfo->st2ndScrubPos.dy;
		pstWireInfo->n2ndScrubCycle = pstBondPara->n2ndScrubCycle;
		pstWireInfo->n2ndScrubForce = pstBondPara->n2ndScrubForce;
		pstWireInfo->n2ndScrubPower = round(pstBondPara->f2ndScrubPower * 2.55);
		pstWireInfo->n2ndScrubSpeed = pstBondPara->n2ndScrubSpeed;

		theServo.GetSpeedProfile(MOTOR_X, 0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY()
					/ 1000.0 * pstBondPara->n2ndScrubSpeed / 100.0;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY()
					* pstBondPara->n2ndScrubSpeed / 100.0;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();

		dMoveDist = pstBondPara->n2ndScrubPosOffset1 / theServo.GetResolXY();
		pstWireInfo->n2ndScrubOffsetTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		dMoveDist = pstBondPara->n2ndScrubAmplitude / theServo.GetResolXY();
		short nScrubT1 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
	    
		dMoveDist = pstBondPara->n2ndScrubAmplitude * 2.0 / theServo.GetResolXY();
		short nScrubT2 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		dMoveDist = pstBondPara->n2ndScrubPosOffset2 / theServo.GetResolXY();
		short nScrubOffset2Time = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		pstWireInfo->n2ndScrubTime = nScrubT1 * 2 + 
			(pstWireInfo->n2ndScrubCycle * 2 - 1) * nScrubT2 +
			nScrubOffset2Time;
	}
	else
	{
		pstWireInfo->st2ndScrubOffset2 = pstWireInfo->stPullPos;
		pstWireInfo->st2ndScrubPos = pstWireInfo->stPullPos;
		pstWireInfo->st2ndScrubAmp.dx = 0.0;
		pstWireInfo->st2ndScrubAmp.dy = 0.0;
		pstWireInfo->b2ndBondScrubEnable = false;
		pstWireInfo->n2ndScrubCycle      = 0;
		pstWireInfo->n2ndScrubForce      = 0;
		pstWireInfo->n2ndScrubPower      = 0;
		pstWireInfo->n2ndScrubOffsetTime = 0;
		pstWireInfo->n2ndScrubTime       = 0;
		pstWireInfo->n2ndScrubSpeed      = 100;
	}

	theServo.GetSpeedProfile(MOTOR_X, 0, &stSpeedProfile);
	dJerk = stSpeedProfile.lJerk / theServo.GetResolXY()
				/ 1000.0 * nShiftSpeed / 100.0;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY()
				* nShiftSpeed / 100.0;
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();

	pstWireInfo->stPreShiftPos = pstWireInfo->stPullPos;
	if (abs(nPreShift) > 0)
	{
		pstWireInfo->stPullPos = CalcPosAlongWire(
						pstWireInfo->stPullPos, pstWireInfo->stBondPos[0],
						nPreShift / theServo.GetResolXY());	//v3.1.T372 add

		dMoveDist = abs(nPreShift) / theServo.GetResolXY();
		nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
		if (pstWireInfo->nPreCtactTime[1] < nMoveTime)
			pstWireInfo->nPreCtactTime[1] = nMoveTime;
	}

	if (abs(nTailShift) > 0)
	{
		pstWireInfo->stTailShiftPos = CalcPosAlongWire(
							pstWireInfo->stBondPos[1], pstWireInfo->stBondPos[0],
							-nTailShift / theServo.GetResolXY());	//v3.1.T372 add

		dMoveDist = abs(nTailShift) / theServo.GetResolXY();
		nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
		if (pstWireInfo->nReleaseTime[1] < nMoveTime)
			pstWireInfo->nReleaseTime[1] = nMoveTime;
	}
	else
		pstWireInfo->stTailShiftPos = pstWireInfo->stBondShiftPos2;

	RoundPosition(&pstWireInfo->stBondShiftPos1);
	RoundPosition(&pstWireInfo->stBondShiftPos2);
	RoundPosition(&pstWireInfo->stPreShiftPos);
	RoundPosition(&pstWireInfo->stPullPos);
	RoundPosition(&pstWireInfo->st2ndScrubPos);
	RoundPosition(&pstWireInfo->st2ndScrubAmp);
	RoundPosition(&pstWireInfo->st2ndScrubOffset2);
	RoundPosition(&pstWireInfo->stTailShiftPos);

	if (m_bDryRun)
	{
		lCtactLvl1 += round(DRYRUN_CTACT_OFFSET);
		lCtactLvl2 += round(DRYRUN_CTACT_OFFSET);
	}

	pstWireInfo->nCtactLvl1 = lCtactLvl1;
	pstWireInfo->nCtactLvl2 = lCtactLvl2;

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(pstWire->nLoopMode, &stLoopPara);
	pstWireInfo->nCtactLvl2 += stLoopPara.stLoopBase.nCtactLvlOffset;
	double dCtactLvl1 = pstWireInfo->nCtactLvl1 * theServo.GetResolZ();
	double dCtactLvl2 = pstWireInfo->nCtactLvl2 * theServo.GetResolZ();

	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("Ctact Level: [%d, %d]\r\n"), lCtactLvl1, lCtactLvl2);
		sDebugInfo.AppendFormat(_T("Loop Group: %d\r\n"), pstWire->nLoopMode);
	}

	dWireLen = Calc2PtDist(pstWireInfo->stBondPos[0], 
		pstWireInfo->stPullPos) * theServo.GetResolXY();

	//v3.1.T71 edit
	ST_LOOP_SHAPE_PARA stLoopShape;
	if (theMcConfig.GetLoopShapeType() == 1)	//v3.1.T333 add
		theLoopShape.CalcLoopShapeEx(dWireLen, dCtactLvl1, dCtactLvl2,
			pstWire->nLoopMode, pstWire->nWireLengthCorrect, &stLoopShape);
	else
		theLoopShape.CalcLoopShape(dWireLen, dCtactLvl1, dCtactLvl2,
			pstWire->nLoopMode, pstWire->nWireLengthCorrect, &stLoopShape);

	pstWireInfo->nKink1WireFeedSpeed = stLoopShape.nWireFeedSpeed;
	pstWireInfo->nKink1WireBentSpeed = stLoopShape.nWireBentSpeed;
	pstWireInfo->nPreKinkWireFeedSpeed = stLoopShape.nPreKinkWireFeedSpeed;
	pstWireInfo->nPreKinkWireBentSpeed = stLoopShape.nPreKinkWireBentSpeed;
	pstWireInfo->nKink2WireFeedSpeed = stLoopShape.nWireFeedSpeed2;
	pstWireInfo->nKink2WireBentSpeed = stLoopShape.nWireBentSpeed2;
	pstWireInfo->nKink3WireFeedSpeed = stLoopShape.nWireFeedSpeed3;
	pstWireInfo->nKink3WireBentSpeed = stLoopShape.nWireBentSpeed3;
	pstWireInfo->nKink4WireFeedSpeed = stLoopShape.nTwistWireFeedSpeed;
	pstWireInfo->nTwistRevBentSpeed  = stLoopShape.nTwistRevBentSpeed;
	pstWireInfo->nTwistFwdBentSpeed  = stLoopShape.nTwistFwdBentSpeed;
	pstWireInfo->nTrajectorySpeed    = stLoopShape.nTrajSpeed;
	pstWireInfo->nPullSpeed          = stLoopShape.nPullSpeed;

	if ((pstWire->bFastLoop == true) &&
		(stLoopShape.bPreKinkEnable == false) &&
		(stLoopShape.bKink2Enable == false))
		pstWireInfo->bFastLoop = true;
	else
		pstWireInfo->bFastLoop = false;

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("--------------------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("Pre T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstWireInfo->nPreCtactTime[0], pstWireInfo->nPreCtactTime[1],
			pstWireInfo->nPreCtactForce[0], pstWireInfo->nPreCtactForce[1],
			pstWireInfo->nStandbyPower[0], pstWireInfo->nStandbyPower[1]);
		sDebugInfo.AppendFormat(_T("Ctact T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstWireInfo->nCtactTime[0], pstWireInfo->nCtactTime[1],
			pstWireInfo->nCtactForce[0], pstWireInfo->nCtactForce[1],
			pstWireInfo->nCtactPower[0], pstWireInfo->nCtactPower[1]);
		sDebugInfo.AppendFormat(_T("Base T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstWireInfo->nBaseTime[0], pstWireInfo->nBaseTime[1],
			pstWireInfo->nBaseForce[0], pstWireInfo->nBaseForce[1],
			pstWireInfo->nBasePower[0], pstWireInfo->nBasePower[1]);
		sDebugInfo.AppendFormat(_T("Rls T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstWireInfo->nReleaseTime[0], pstWireInfo->nReleaseTime[1],
			pstWireInfo->nReleaseForce[0], pstWireInfo->nReleaseForce[1],
			pstWireInfo->nReleasePower[0], pstWireInfo->nReleasePower[1]);
	}

	double dLoopLvl = lCtactLvl1 + stLoopShape.dLoopHt / theServo.GetResolZ();

	//rev ht
	double dRevLvl = lCtactLvl1 + stLoopShape.dRevHt / theServo.GetResolZ();
	pstWireInfo->nRevHt = round(dRevLvl - lCtactLvl1);
	//rev dist ht
	double dRevDistLvl = lCtactLvl1 + stLoopShape.dRevKinkHt / theServo.GetResolZ();
	pstWireInfo->nRevDistHt = round(dRevDistLvl - dRevLvl);
	//rev ht 2
	pstWireInfo->nRevHt2 = (int)(stLoopShape.dRevHt2 / theServo.GetResolZ());	//v3.1.T350 add

	pstWireInfo->bPreKinkEnable = stLoopShape.bPreKinkEnable;
	//prekink ht
	double dPreKinkLvl = dRevDistLvl + stLoopShape.dPreKinkHt / theServo.GetResolZ();
	pstWireInfo->nPreKinkHt = round(dPreKinkLvl - dRevDistLvl);
	//prekink dist ht
	double dPreKinkDistLvl = dPreKinkLvl + stLoopShape.dPreKinkDistHt / theServo.GetResolZ();
	pstWireInfo->nPreKinkDistHt = round(dPreKinkDistLvl - dPreKinkLvl);
	//prekink ht 2
	pstWireInfo->nPreKinkHt2 = (int)(stLoopShape.dPreKinkHt2 / theServo.GetResolZ());	//v3.1.T341 add

	pstWireInfo->bKink2Enable = stLoopShape.bKink2Enable;
	//kink2 ht
	double dKink2Lvl = dPreKinkDistLvl + stLoopShape.dKink2Ht / theServo.GetResolZ();
	pstWireInfo->nKink2Ht = round(dKink2Lvl - dPreKinkDistLvl);
	//kink2 dist ht
	double dKink2DistLvl  = dPreKinkDistLvl + stLoopShape.dKink2KinkHt / theServo.GetResolZ();
	pstWireInfo->nKink2DistHt = round(dKink2DistLvl - dKink2Lvl);
	//kink2 ht 2
	pstWireInfo->nKink2Ht2 = (int)(stLoopShape.dKink2Ht2 / theServo.GetResolZ());	//v3.1.T341 add

	pstWireInfo->bKink3Enable = stLoopShape.bKink3Enable;
	//kink3 ht
	double dKink3Lvl = dKink2DistLvl + stLoopShape.dKink3Ht / theServo.GetResolZ();
	pstWireInfo->nKink3Ht = round(dKink3Lvl - dKink2DistLvl);
	//kink3 dist ht
	double dKink3DistLvl  = dKink2DistLvl + stLoopShape.dKink3KinkHt / theServo.GetResolZ();
	pstWireInfo->nKink3DistHt = round(dKink3DistLvl - dKink3Lvl);
	//kink2 ht 2
	pstWireInfo->nKink3Ht2 = (int)(stLoopShape.dKink3Ht2 / theServo.GetResolZ());	//v3.1.T341 add

	pstWireInfo->bTwistEnable = stLoopShape.bTwistEnable;
	//twist rev ht
	double dTwistLvl = dKink3DistLvl + stLoopShape.dTwistRevHt / theServo.GetResolZ();
	pstWireInfo->nTwistHt = round(dTwistLvl - dKink3DistLvl);
	//twist rev dist ht
	double dTwistRevDistLvl = dKink3DistLvl + stLoopShape.dTwistKinkHt / theServo.GetResolZ();
	pstWireInfo->nTwistRevDistHt = round(dTwistRevDistLvl - dTwistLvl);
	//twist fwd dist ht
	double dTwistFwdDistLvl = dTwistRevDistLvl + stLoopShape.dTwistFwdDistHt / theServo.GetResolZ();
	pstWireInfo->nTwistFwdDistHt = round(dTwistFwdDistLvl - dTwistRevDistLvl);

	//loop top ht
	double dLoopTopLvl = dTwistFwdDistLvl + stLoopShape.dSlopeHt / theServo.GetResolZ();
	pstWireInfo->nSlopeLen = round(dLoopTopLvl - dTwistFwdDistLvl);

	//above loop top ht
	pstWireInfo->cLoopTopMode = stLoopShape.cLoopTopMode;
	double dAboveLoopTopLvl = dLoopTopLvl + stLoopShape.dLoopTopHt / theServo.GetResolZ();
	pstWireInfo->nAboveLoopTopHt = round(dAboveLoopTopLvl - dLoopTopLvl);

	if (pstWireInfo->bFastLoop)
	{
		pstWireInfo->nSlopeLen = round(dLoopTopLvl - lCtactLvl1);
		pstWireInfo->nRevHt = 0;
		pstWireInfo->nRevDistHt = 0;
		//stLoopShape.dRevDist1Length = 0.0;
		stLoopShape.dPreKinkDistLen = 0.0;
	}

	short nSrchHt1, nSrchHt2;
	nSrchHt1 = pstBondPara->nSrchHeight1 + pstWire->nSrchHtOffset[0];	//v3.0.T2 edit
	if ((pstWire->nBumpType == BUMP_SOB)
		|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T103 add
		nSrchHt2 = stBumpWire.nSrchHeight; //stLoopShape.nSrchHeight2;
	else
		nSrchHt2 = pstBondPara->nSrchHeight2 + pstWire->nSrchHtOffset[1];	//v3.0.T2 edit

	//v1.6.T151 add
	pstWireInfo->nSrchHt1 = (int)(nSrchHt1 / theServo.GetResolZ());
	pstWireInfo->nSrchHt2 = (int)(nSrchHt2 / theServo.GetResolZ());
	pstWireInfo->nSrchSpeed1 = pstBondPara->nSrchSpeed1;
	if ((pstWire->nBumpType == BUMP_SOB)
		|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T103 add
		pstWireInfo->nSrchSpeed2 = stBumpWire.nSrchSpeed2;
	else
		pstWireInfo->nSrchSpeed2 = pstBondPara->nSrchSpeed2;

	if (m_bDryRun)
	{
		pstWireInfo->nSrchTol1   = 0;
		pstWireInfo->nSrchTol2   = 0;
		pstWireInfo->nSrchSpeed1 = 0;
		pstWireInfo->nSrchSpeed2 = 0;
	}
	else
	{
		pstWireInfo->nSrchTol1 = pstBondPara->nSrchTol1;
		if ((pstWire->nBumpType == BUMP_SOB)
			|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T103 add
			pstWireInfo->nSrchTol2 = stBumpWire.nSrchTol2;
		else
			pstWireInfo->nSrchTol2 = pstBondPara->nSrchTol2;
	}

	pstWireInfo->nSrchDelay[0] = pstBondPara->nCtactSrchDelay[0];
	if ((pstWire->nBumpType == BUMP_SOB)
		|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T103 add
		pstWireInfo->nSrchDelay[1] = stBumpWire.nCtactSrchDelay;
	else
		pstWireInfo->nSrchDelay[1] = pstBondPara->nCtactSrchDelay[1];

	if (pstPgm->bFabTest == false)
		pstWireInfo->nSrchLvl1 = lCtactLvl1 + round(nSrchHt1 / theServo.GetResolZ());
	else
		pstWireInfo->nSrchLvl1 = round(dLoopTopLvl);

	double dSrchLvl2 = pstWireInfo->nCtactLvl2 + nSrchHt2 / theServo.GetResolZ();
	pstWireInfo->nSrchLvl2 = round(dSrchLvl2);	//v3.1.T260 add

	//v3.1.T373 delete
	//if (pstWireInfo->cLoopTopMode == ABOVE_LOOP_TOP_DISABLE)
	//	pstWireInfo->lTrajHeight = round(dSrchLvl2 - dLoopTopLvl);
	//else if ((pstWireInfo->cLoopTopMode == ABOVE_LOOP_TOP_NORMAL)
	//	|| (pstWireInfo->cLoopTopMode == ABOVE_LOOP_TOP_FAST))
	//	pstWireInfo->lTrajHeight = round(dSrchLvl2 - dAboveLoopTopLvl);

	//v3.1.T381 add
	const WIRE_RECORD *pstNextWire;
	if (nWireNum >= thePgm.GetTotalWire())
		pstNextWire = theWire.GetRcd(1);
	else
		pstNextWire = theWire.GetRcd(nWireNum + 1);
	pstWireInfo->chFabType = pstNextWire->cWireFabType;	//v3.1.T381 edit
	switch (pstWireInfo->nBumpType)	//v3.1.T88 add
	{
	case BUMP_BOS:
	case BUMP_BSB:	//v3.1.T103 add
	{
		if (m_nBumpBondOption != BUMPOPT_WIRE_ONLY)
			pstWireInfo->chFabType = pstWire->cBbosFabType;	//v3.1.T381 edit
		break;
	}
	case BUMP_SOB:
	{
		if (m_nBumpBondOption != BUMPOPT_WIRE_ONLY)
			pstWireInfo->chFabType = pstNextWire->cBsobFabType;
		break;
	}
	}
	if (bDebug) sDebugInfo.AppendFormat(_T("FAB Type: %d\r\n"), pstWireInfo->chFabType);

	//v3.1.T60 delete
	//short nTailLength;
	//if (pstWireInfo->chFabType == 0)
	//	nTailLength = pstPgm->nTailLengthUm;
	//else
	//	nTailLength = pstPgm->nFab2TailLengthUm;
	short nTailLen = theEfoProfile.GetProfile(pstWireInfo->chFabType)->nTailLen_x1um;	//v3.1.T60 add
	double dTailLvl = pstWireInfo->nCtactLvl2 + nTailLen / theServo.GetResolZ();
	pstWireInfo->nTailHt = round(dTailLvl - pstWireInfo->nCtactLvl2);

	if (pstPgm->bTailMonitor == true)
		pstWireInfo->nSparkDelay = 400;
	else
		pstWireInfo->nSparkDelay = pstPgm->nSparkDelay;

	pstWireInfo->nResetLvl = pstMcData->lResetLevel;
	if (pstMcData->bTailCtrlMode)
		pstWireInfo->nTailHt = 0;

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("--------------------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("Srch Ht: [%d, %d]\r\n"),
						pstWireInfo->nSrchHt1, pstWireInfo->nSrchHt2);
		sDebugInfo.AppendFormat(_T("Srch Level: [%d, %d]\r\n"),
						pstWireInfo->nSrchLvl1, pstWireInfo->nSrchLvl2);
		sDebugInfo.AppendFormat(_T("Srch Speed: [%d, %d]\r\n"),
						pstWireInfo->nSrchSpeed1, pstWireInfo->nSrchSpeed2);
		sDebugInfo.AppendFormat(_T("Rev Ht/DistHt: [%d, %d]\r\n"),
						pstWireInfo->nRevHt, pstWireInfo->nRevDistHt);
		sDebugInfo.AppendFormat(_T("PreKink Ht/DistHt: [%d, %d]\r\n"),
						pstWireInfo->nPreKinkHt, pstWireInfo->nPreKinkDistHt);
		sDebugInfo.AppendFormat(_T("Kink2 Ht/DistHt: [%d, %d]\r\n"),
						pstWireInfo->nKink2Ht, pstWireInfo->nKink2DistHt);
		sDebugInfo.AppendFormat(_T("Kink3 Ht/DistHt: [%d, %d]\r\n"),
						pstWireInfo->nKink3Ht, pstWireInfo->nKink3DistHt);
		sDebugInfo.AppendFormat(_T("Twist Ht: %d\r\n"), pstWireInfo->nTwistHt);
		sDebugInfo.AppendFormat(_T("Twist Rev Dist Ht: %d\r\n"), pstWireInfo->nTwistRevDistHt);
		sDebugInfo.AppendFormat(_T("Twist Fwd Dist Ht: %d\r\n"), pstWireInfo->nTwistFwdDistHt);
		sDebugInfo.AppendFormat(_T("Loop Top Ht: %d\r\n"), pstWireInfo->nSlopeLen);
		sDebugInfo.AppendFormat(_T("Above Loop Top Ht: %d\r\n"), pstWireInfo->nAboveLoopTopHt);
		//sDebugInfo.AppendFormat(_T("Traj Ht: %d\r\n"), pstWireInfo->lTrajHeight);	//v3.1.T373 delete
		sDebugInfo.AppendFormat(_T("Tail Ht: %d\r\n"), pstWireInfo->nTailHt);
		sDebugInfo.AppendFormat(_T("Reset Level: %d\r\n"), pstWireInfo->nResetLvl);
	}

	//v3.1.T190 add
	short nLAngle = theMcConfig.GetNeckLAngle();
	if (nLAngle == 0) nLAngle = stLoopShape.nLAngle;
	pstWireInfo->stRevPos = CalcPosAlongWire(
				pstWireInfo->stBondPos[0], pstWireInfo->stBondPos[1],
				-stLoopShape.dRevDistLen / theServo.GetResolXY(), nLAngle);	//v3.1.T372 add
	pstWireInfo->stRevDist2 = CalcDistAlongWire(
				pstWireInfo->stRevPos, pstWireInfo->stBondPos[1],
				-stLoopShape.dRevSpanLen2 / theServo.GetResolXY());	//v3.1.T575 add

	//v3.1.T190 add
	nLAngle = theMcConfig.GetPreKinkLAngle();
	if (nLAngle == 0) nLAngle = stLoopShape.nLAngle;
	pstWireInfo->stPreKinkPos = CalcPosAlongWire(
				pstWireInfo->stRevPos, pstWireInfo->stBondPos[1],
				-stLoopShape.dPreKinkDistLen / theServo.GetResolXY(), nLAngle);	//v3.1.T372 add
	pstWireInfo->stPreKinkDist2 = CalcDistAlongWire(
				pstWireInfo->stPreKinkPos, pstWireInfo->stBondPos[1],
				-stLoopShape.dPreKinkSpanLen2 / theServo.GetResolXY());	//v3.1.T575 add

	//v3.1.T190 add
	nLAngle = theMcConfig.GetKink2LAngle();
	if (nLAngle == 0) nLAngle = stLoopShape.nLAngle;
	pstWireInfo->stKink2Pos = CalcPosAlongWire(
				pstWireInfo->stPreKinkPos, pstWireInfo->stBondPos[1],
				-stLoopShape.dKink2DistLen / theServo.GetResolXY(), nLAngle);	//v3.1.T372 add
	pstWireInfo->stKink2Dist2 = CalcDistAlongWire(
				pstWireInfo->stKink2Pos, pstWireInfo->stBondPos[1],
				-stLoopShape.dKink2SpanLen2 / theServo.GetResolXY());	//v3.1.T375 add

	//v3.1.T190 add
	nLAngle = theMcConfig.GetKink3LAngle();
	if (nLAngle == 0) nLAngle = stLoopShape.nLAngle;
	pstWireInfo->stKink3Pos = CalcPosAlongWire(
				pstWireInfo->stKink2Pos, pstWireInfo->stBondPos[1],
				-stLoopShape.dKink3DistLen / theServo.GetResolXY(), nLAngle);	//v3.1.T372 add
	pstWireInfo->stKink3Dist2 = CalcDistAlongWire(
				pstWireInfo->stKink3Pos, pstWireInfo->stBondPos[1],
				-stLoopShape.dKink3SpanLen2 / theServo.GetResolXY());	//v3.1.T375 add

	pstWireInfo->stTwistRevKinkPos = CalcPosAlongWire(
				pstWireInfo->stKink3Pos, pstWireInfo->stBondPos[1],
				-stLoopShape.dTwistRevDistLen / theServo.GetResolXY(), stLoopShape.nLAngle);	//v3.1.T372 add

	pstWireInfo->stTwistFwdKinkPos = CalcPosAlongWire(
				pstWireInfo->stTwistRevKinkPos, pstWireInfo->stBondPos[1],
				-stLoopShape.dTwistFwdDistLen / theServo.GetResolXY(), stLoopShape.nLAngle);	//v3.1.T372 add

	pstWireInfo->stTrajEndPos = CalcPosAlongWire(
				pstWireInfo->stPullPos, pstWireInfo->stBondPos[0],
				stLoopShape.nPullDist / theServo.GetResolXY());	//v3.1.T372 add

	pstWireInfo->stLoopTopPos = pstWireInfo->stTwistFwdKinkPos;
	pstWireInfo->stAboveLoopTopPos = pstWireInfo->stBondPos[0];

	RoundPosition(&pstWireInfo->stRevPos);
	RoundPosition(&pstWireInfo->stPreKinkPos);
	RoundPosition(&pstWireInfo->stKink2Pos);
	RoundPosition(&pstWireInfo->stKink3Pos);
	RoundPosition(&pstWireInfo->stTwistRevKinkPos);
	RoundPosition(&pstWireInfo->stTwistFwdKinkPos);
	RoundPosition(&pstWireInfo->stLoopTopPos);
	RoundPosition(&pstWireInfo->stAboveLoopTopPos);
	RoundPosition(&pstWireInfo->stTrajEndPos);

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("--------------------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("Shift Pos1: [%.2f, %.2f]\r\n"),
			pstWireInfo->stBondShiftPos1.dx, pstWireInfo->stBondShiftPos1.dy);
		sDebugInfo.AppendFormat(_T("Bond Pos1: [%.2f, %.2f]\r\n"),
			pstWireInfo->stBondPos[0].dx, pstWireInfo->stBondPos[0].dy);
		sDebugInfo.AppendFormat(_T("Rev Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stRevPos.dx, pstWireInfo->stRevPos.dy);
		sDebugInfo.AppendFormat(_T("PreKink Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stPreKinkPos.dx, pstWireInfo->stPreKinkPos.dy);
		sDebugInfo.AppendFormat(_T("Kink2 Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stKink2Pos.dx, pstWireInfo->stKink2Pos.dy);
		sDebugInfo.AppendFormat(_T("Kink3 Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stKink3Pos.dx, pstWireInfo->stKink3Pos.dy);
		sDebugInfo.AppendFormat(_T("Twist Rev Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stTwistRevKinkPos.dx, pstWireInfo->stTwistRevKinkPos.dy);
		sDebugInfo.AppendFormat(_T("Twist Fwd Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stTwistFwdKinkPos.dx, pstWireInfo->stTwistFwdKinkPos.dy);
		sDebugInfo.AppendFormat(_T("Loop Top Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stLoopTopPos.dx, pstWireInfo->stLoopTopPos.dy);
		sDebugInfo.AppendFormat(_T("Traj End Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stTrajEndPos.dx, pstWireInfo->stTrajEndPos.dy);
		sDebugInfo.AppendFormat(_T("Pull Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stPullPos.dx, pstWireInfo->stPullPos.dy);
		sDebugInfo.AppendFormat(_T("Pre Shift Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stPreShiftPos.dx, pstWireInfo->stPreShiftPos.dy);
		sDebugInfo.AppendFormat(_T("Bond Pos2: [%.2f, %.2f]\r\n"),
			pstWireInfo->stBondPos[1].dx, pstWireInfo->stBondPos[1].dy);
		sDebugInfo.AppendFormat(_T("Tail Shift Pos: [%.2f, %.2f]\r\n"),
			pstWireInfo->stTailShiftPos.dx, pstWireInfo->stTailShiftPos.dy);
	}

	double dT1, dT2, dT3, dC1, dC2, dC3, dSpeed;
	short nTotalTime, i;	//v1.5.T146 edit
	long lMoveDist;

	ST_LOOP_CONFIG_PARA stLoopConfig;
	theLoopPara.GetConfigRcd(&stLoopConfig);
	if (theDebug.IsDebugStepMotion())
	{
		stLoopConfig.nNeckHtPremove    = 0;
		stLoopConfig.nNeckDistPremove  = 0;
		stLoopConfig.nKink2HtPremove   = 0;
		stLoopConfig.nKink2DistPremove = 0;
		stLoopConfig.nLoopTopPremove   = 0;
	}
	else if (theMcConfig.IsPremoveGroupEnable())	//v3.1.T317 add
	{
		short nLoopMode = theWire.GetRcd(nWireNum)->nLoopMode;
		if (nLoopMode > 0)
		{
			stLoopConfig.nNeckHtPremove = theMcConfig.GetNeckHtPremove(nLoopMode);
			stLoopConfig.nNeckDistPremove = theMcConfig.GetNeckDistPremove(nLoopMode);
		}
	}

	if (pstWireInfo->bPreKinkEnable)
		stLoopConfig.nNeckDistPremove = 0;

	//rev height time
	dMoveDist = pstWireInfo->nRevHt;
	dSpeed = pstWireInfo->nKink1WireFeedSpeed / 100.0;
	theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_2, &stSpeedProfile);
	dJerk = stSpeedProfile.lJerk / theServo.GetResolZ() / 1000.0 * dSpeed;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolZ() * dSpeed;
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolZ();
	short nRevHtTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

	//rev distance time
	double dTravelX = fabs(pstWireInfo->stBondPos[0].dx - pstWireInfo->stRevPos.dx);
	double dTravelY = fabs(pstWireInfo->stBondPos[0].dy - pstWireInfo->stRevPos.dy);

	dSpeed = pstWireInfo->nKink1WireBentSpeed / 100.0;
	theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &stSpeedProfile);
	dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
	short nMoveTimeX = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelX);

	theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &stSpeedProfile);
	dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
	short nMoveTimeY = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelY);
	short nRevDistTime = max(nMoveTimeX, nMoveTimeY);

	//rev distance height time
	short nRevDistHtTime = nRevDistTime;
	pstWireInfo->nArcRevDelay = 0;
	if (stLoopPara.stLoopBase.nRevArcDelay > 0)
	{
#if _SERVO_TYPE == 2	//v3.1.T546 add
		pstWireInfo->nArcRevDelay = stLoopPara.stLoopBase.nRevArcDelay;
#else
		pstWireInfo->nArcRevDelay = (short)(nRevDistHtTime
			* stLoopPara.stLoopBase.nRevArcDelay / 100.0);
		nRevDistHtTime -= pstWireInfo->nArcRevDelay;
#endif
	}

	//v3.1.T326 add, v3.1.T327 edit
	short nPreKinkDistHtTime = 0;
	pstWireInfo->nArcPreKinkDelay = 0;
	if (pstWireInfo->bPreKinkEnable)
	{
		//prekink dist time
		dTravelX = fabs(pstWireInfo->stRevPos.dx - pstWireInfo->stPreKinkPos.dx);
		dTravelY = fabs(pstWireInfo->stRevPos.dy - pstWireInfo->stPreKinkPos.dy);

		dSpeed = pstWireInfo->nPreKinkWireBentSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeX = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelX);

		theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeY = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelY);

		//prekink distance height time
		nPreKinkDistHtTime = max(nMoveTimeX, nMoveTimeY);	//z time follow xy
		if (stLoopPara.stLoopBase.nPreKinkArcDelay > 0)
		{
#if _SERVO_TYPE == 2	//v3.1.T546 add
			pstWireInfo->nArcPreKinkDelay = stLoopPara.stLoopBase.nPreKinkArcDelay;
#else
			pstWireInfo->nArcPreKinkDelay = (short)(nPreKinkDistHtTime
				* stLoopPara.stLoopBase.nPreKinkArcDelay / 100.0);
			nPreKinkDistHtTime -= pstWireInfo->nArcPreKinkDelay;
#endif
		}
	}

	pstWireInfo->nArcKink2Delay = 0;
	short nKink2HtTime = 0;
	short nKink2DistTime = 0;
	short nKink2DistHtTime = 0;
	if (pstWireInfo->bKink2Enable)
	{
		//kink2 height time
		dMoveDist = pstWireInfo->nKink2Ht;
		dSpeed = pstWireInfo->nKink2WireFeedSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_2, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolZ() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolZ() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolZ();
		nKink2HtTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		//kink2 distance time
		dTravelX = fabs(pstWireInfo->stKink2Pos.dx - pstWireInfo->stRevPos.dx);
		dTravelY = fabs(pstWireInfo->stKink2Pos.dy - pstWireInfo->stRevPos.dy);

		dSpeed = pstWireInfo->nKink2WireBentSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeX = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelX);

		theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeY = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelY);
		nKink2DistTime = max(nMoveTimeX, nMoveTimeY);

		//kink2 distance height time
		nKink2DistHtTime = nKink2DistTime;
		if (stLoopPara.stLoopBase.nKink2ArcDelay > 0)
		{
#if _SERVO_TYPE == 2	//v3.1.T546 add
			pstWireInfo->nArcKink2Delay = stLoopPara.stLoopBase.nKink2ArcDelay;
#else
			pstWireInfo->nArcKink2Delay = (short)(nKink2DistHtTime
				* stLoopPara.stLoopBase.nKink2ArcDelay / 100.0);
			nKink2DistHtTime -= pstWireInfo->nArcKink2Delay;
#endif
		}
	}

	pstWireInfo->nArcKink3Delay = 0;
	short nKink3HtTime = 0;
	short nKink3DistTime = 0;
	short nKink3DistHtTime = 0;
	if (pstWireInfo->bKink3Enable)
	{
		//kink3 height time
		dMoveDist = pstWireInfo->nKink3Ht;
		dSpeed = pstWireInfo->nKink3WireFeedSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_2, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolZ() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolZ() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolZ();
		nKink3HtTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		//kink3 distance time
		dTravelX = fabs(pstWireInfo->stKink3Pos.dx - pstWireInfo->stKink2Pos.dx);
		dTravelY = fabs(pstWireInfo->stKink3Pos.dy - pstWireInfo->stKink2Pos.dy);

		dSpeed = pstWireInfo->nKink3WireBentSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeX = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelX);

		theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeY = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelY);
		nKink3DistTime = max(nMoveTimeX, nMoveTimeY);

		//kink3 distance height time
		nKink3DistHtTime = nKink3DistTime;
		if (stLoopPara.stLoopBase.nKink3ArcDelay > 0)
		{
#if _SERVO_TYPE == 2	//v3.1.T546 add
			pstWireInfo->nArcKink3Delay = stLoopPara.stLoopBase.nKink3ArcDelay;
#else
			pstWireInfo->nArcKink3Delay = (short)(nKink3DistHtTime
				* stLoopPara.stLoopBase.nKink3ArcDelay / 100.0);
			nKink3DistHtTime -= pstWireInfo->nArcKink3Delay;
#endif
		}
	}

	short nTwistHtTime = 0;
	short nTwistRevDistTime = 0;
	short nTwistRevDistHtTime = 0;
	short nTwistFwdDistTime = 0;
	short nTwistFwdDistHtTime = 0;
	if (pstWireInfo->bTwistEnable)
	{
		//Reverse height2 bond head time
		dMoveDist = pstWireInfo->nTwistHt;
		dSpeed = pstWireInfo->nKink4WireFeedSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_2, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolZ() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolZ() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolZ();
		nTwistHtTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

		//Twist reverse distance table time
		dTravelX = fabs(pstWireInfo->stTwistRevKinkPos.dx - pstWireInfo->stKink3Pos.dx);
		dTravelY = fabs(pstWireInfo->stTwistRevKinkPos.dy - pstWireInfo->stKink3Pos.dy);

		dSpeed = pstWireInfo->nTwistRevBentSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeX = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelX);

		theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeY = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelY);
		nTwistRevDistTime = max(nMoveTimeX, nMoveTimeY);

		//Twist Reverse distance bond head time
		dMoveDist = pstWireInfo->nTwistRevDistHt;
		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_2, &dJerk, &dMaxAcc, &dMaxVel);
		CalcProfCycloidal(dMoveDist, dMaxAcc, dMaxVel, 0.0, &dT1, &dT2, &dT3, &dC1, &dC2, &dC3);
		nTwistRevDistHtTime = round((dT1 + dT2 + dT3) / theServo.GetServoSampleRate() * 1000.0);	//v1.5.T128 edit

		//Twist forward distance table time
		dTravelX = fabs(pstWireInfo->stTwistFwdKinkPos.dx - pstWireInfo->stTwistRevKinkPos.dx);
		dTravelY = fabs(pstWireInfo->stTwistFwdKinkPos.dy - pstWireInfo->stTwistRevKinkPos.dy);

		dSpeed = pstWireInfo->nTwistFwdBentSpeed / 100.0;
		theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeX = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelX);

		theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
		nMoveTimeY = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelY);
		nTwistFwdDistTime = max(nMoveTimeX, nMoveTimeY);

		//Twist Reverse distance bond head time
		dMoveDist = pstWireInfo->nTwistFwdDistHt;
		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_2, &dJerk, &dMaxAcc, &dMaxVel);
		CalcProfCycloidal(dMoveDist, dMaxAcc, dMaxVel, 0.0, &dT1, &dT2, &dT3, &dC1, &dC2, &dC3);
		nTwistFwdDistHtTime = round((dT1 + dT2 + dT3) / theServo.GetServoSampleRate() * 1000.0);	//v1.5.T128 edit
	}

	short nLoopTopTime = 0;
	if (pstWireInfo->cLoopTopMode != ABOVE_LOOP_TOP_FAST)
	{
		//Loop top bond head time
		dMoveDist = pstWireInfo->nSlopeLen;
		theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_2, &stSpeedProfile);
		dJerk = stSpeedProfile.lJerk / theServo.GetResolZ() / 1000.0;
		dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolZ();
		dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolZ();
		nLoopTopTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
	}

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("--------------------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("Rev Ht/Dist/DistHt Time: [%d, %d, %d]\r\n"),
						nRevHtTime, nRevDistTime, nRevDistHtTime);
		sDebugInfo.AppendFormat(_T("PreKink DistHt Time: %d\r\n"), nPreKinkDistHtTime);	//v3.1.T326 add
		sDebugInfo.AppendFormat(_T("Kink2 Ht/Dist/DistHt Time: [%d, %d, %d]\r\n"),
						nKink2HtTime, nKink2DistTime, nKink2DistHtTime);
		sDebugInfo.AppendFormat(_T("Kink3 Ht/Dist/DistHt Time: [%d, %d, %d]\r\n"),
						nKink3HtTime, nKink3DistTime, nKink3DistHtTime);
		sDebugInfo.AppendFormat(_T("Twist Ht Time: %d\r\n"), nTwistHtTime);
		sDebugInfo.AppendFormat(_T("Twist Rev Dist/DistHt Time: [%d, %d]\r\n"),
									nTwistRevDistTime, nTwistRevDistHtTime);
		sDebugInfo.AppendFormat(_T("Twist Fwd Dist/DistHt Time: [%d, %d]\r\n"),
									nTwistFwdDistTime, nTwistFwdDistHtTime);
		sDebugInfo.AppendFormat(_T("Loop Top Time: %d\r\n"), nLoopTopTime);
	}

	// Neck height pretrigger
	if (pstWireInfo->bFastLoop)
	{	
		pstWireInfo->nRevHtPremove = round(stLoopConfig.nNeckHtPremove / 5.0);
	}
	else
	{
		pstWireInfo->nRevHtPremove = round(nRevHtTime
					* stLoopConfig.nNeckHtPremove / 100.0);	//v3.1.T75 add
		if ((stLoopConfig.nNeckHtPremove > 0)
			&& (nRevDistTime > nRevDistHtTime))
		{
			//pstWireInfo->nNeckHtPretrig = round((double)nNeckHtTime * (double)stLoopConfig.nNeckHtPretrig / 100.0);	//v3.1.T75 delete
			if (pstWireInfo->nRevHtPremove + nRevDistHtTime > nRevDistTime)
				pstWireInfo->nRevHtPremove = nRevDistTime - nRevDistHtTime;
		}
		//v3.1.T75 delete
		//else
		//	pstWireInfo->nNeckHtPretrig = 0;
	}

	short nRevHtPremove = pstWireInfo->nRevHtPremove;
	if (nRevHtPremove < 0) nRevHtPremove = 0;
	if (stLoopConfig.nNeckDistPremove > 0)
	{
		if (nRevHtPremove + nRevDistHtTime < nRevDistTime)
		{
			if (pstWireInfo->bKink2Enable)
			{
				pstWireInfo->nRevDistPremove = round(nKink2HtTime
									* stLoopConfig.nNeckDistPremove / 100.0);
				nTotalTime = nRevHtPremove + nRevDistHtTime
									+ pstWireInfo->nRevDistPremove;
				if (nTotalTime > nRevDistTime)
					pstWireInfo->nRevDistPremove = nRevDistTime
									- nRevHtPremove - nRevDistHtTime;
				else
					nRevDistHtTime = nRevDistTime - nRevHtPremove
									- pstWireInfo->nRevDistPremove;
			}
			else if (pstWireInfo->cLoopTopMode != ABOVE_LOOP_TOP_FAST)
			{
				pstWireInfo->nRevDistPremove = round(nLoopTopTime
									* stLoopConfig.nNeckDistPremove / 100.0);
				nTotalTime = nRevHtPremove + nRevDistHtTime
									+ pstWireInfo->nRevDistPremove;
				if (nTotalTime > nRevDistTime)
					pstWireInfo->nRevDistPremove = nRevDistTime
									- nRevHtPremove - nRevDistHtTime;
				else
					nRevDistHtTime = nRevDistTime - nRevHtPremove
									- pstWireInfo->nRevDistPremove;
			}
			else
			{
				pstWireInfo->nRevDistPremove = 0;
				nRevDistHtTime = nRevDistTime - nRevHtPremove;
			}
		}
		else
		{
			pstWireInfo->nRevDistPremove = 0;
			nRevDistHtTime = nRevDistTime - nRevHtPremove;
		}
	}
	else
	{
		//pstWireInfo->nNeckDistPretrig = 0;
		pstWireInfo->nRevDistPremove = stLoopConfig.nNeckDistPremove;
		nRevDistHtTime = nRevDistTime - nRevHtPremove;
	}

	if (pstWireInfo->bKink2Enable)
	{
		if ((stLoopConfig.nKink2HtPremove > 0) && (nKink2DistTime > nKink2DistHtTime))
		{
			pstWireInfo->nKink2HtPremove = round(nKink2HtTime
								* stLoopConfig.nKink2HtPremove / 100.0);
			if (pstWireInfo->nRevDistPremove + pstWireInfo->nKink2HtPremove > nKink2HtTime)
				pstWireInfo->nKink2HtPremove = nKink2HtTime - pstWireInfo->nRevDistPremove;

			if (pstWireInfo->nKink2HtPremove + nKink2DistHtTime > nKink2DistTime)
				pstWireInfo->nKink2HtPremove = nKink2DistTime - nKink2DistHtTime;
		}
		else
			//pstWireInfo->nRevHt2Pretrig = 0;
			pstWireInfo->nKink2HtPremove = stLoopConfig.nKink2HtPremove;

		if (stLoopConfig.nKink2DistPremove > 0)
		{
			if (pstWireInfo->nKink2HtPremove + nKink2DistHtTime < nKink2DistTime)
			{
				if (pstWireInfo->bKink3Enable == true)
				{
					pstWireInfo->nKink2DistPremove = round(nKink3HtTime
									* stLoopConfig.nKink2DistPremove / 100.0);
					nTotalTime = pstWireInfo->nKink2HtPremove + nKink2DistHtTime
									+ pstWireInfo->nKink2DistPremove;
					if (nTotalTime > nKink2DistTime)
						pstWireInfo->nKink2DistPremove = nKink2DistTime
								- pstWireInfo->nKink2HtPremove - nKink2DistHtTime;
					else
						nKink2DistHtTime = nKink2DistTime
											- pstWireInfo->nKink2HtPremove
											- pstWireInfo->nKink2DistPremove;
				}
				else if (pstWireInfo->bTwistEnable)
				{
					pstWireInfo->nKink2DistPremove = round(nTwistHtTime
									* stLoopConfig.nKink2DistPremove / 100.0);
					nTotalTime = pstWireInfo->nKink2HtPremove + nKink2DistHtTime
											+ pstWireInfo->nKink2DistPremove;
					if (nTotalTime > nKink2DistTime)
						pstWireInfo->nKink2DistPremove = nKink2DistTime
											- pstWireInfo->nKink2HtPremove
											- nKink2DistHtTime;
					else
						nKink2DistHtTime = nKink2DistTime
											- pstWireInfo->nKink2HtPremove
											- pstWireInfo->nKink2DistPremove;
				}
				else
				{
					if (pstWireInfo->cLoopTopMode != ABOVE_LOOP_TOP_FAST)
					{
						pstWireInfo->nKink2DistPremove = round(nLoopTopTime
									* stLoopConfig.nKink2DistPremove / 100.0);
						nTotalTime = pstWireInfo->nKink2HtPremove
							+ nKink2DistHtTime + pstWireInfo->nKink2DistPremove;
						if (nTotalTime > nKink2DistTime)
							pstWireInfo->nKink2DistPremove = nKink2DistTime
								- pstWireInfo->nKink2HtPremove - nKink2DistHtTime;
						else
							nKink2DistHtTime = nKink2DistTime
											- pstWireInfo->nKink2HtPremove
											- pstWireInfo->nKink2DistPremove;
					}
					else
					{
						pstWireInfo->nKink2DistPremove = 0;
						nKink2DistHtTime = nKink2DistTime - pstWireInfo->nKink2HtPremove;
					}
				}
			}
			else
			{
				pstWireInfo->nKink2DistPremove = 0;
				nKink2DistHtTime = nKink2DistTime - pstWireInfo->nKink2HtPremove;
			}
		}
		else
		{
			//pstWireInfo->nRevDist2Pretrig = 0;
			pstWireInfo->nKink2DistPremove = stLoopConfig.nKink2DistPremove;
			nKink2DistHtTime = nKink2DistTime - pstWireInfo->nKink2HtPremove;
		}
	}
	else
	{
		pstWireInfo->nKink2HtPremove = 0;
		pstWireInfo->nKink2DistPremove = 0;
	}

	if (pstWireInfo->bKink3Enable == true)
	{
		if ((stLoopConfig.nKink3HtPremove > 0) && (nKink3DistTime > nKink3DistHtTime))
		{
			pstWireInfo->nKink3HtPremove = round(nKink3HtTime
										* stLoopConfig.nKink3HtPremove / 100.0);
			if (pstWireInfo->nKink2DistPremove + pstWireInfo->nKink3HtPremove > nKink3HtTime)
				pstWireInfo->nKink3HtPremove = nKink3HtTime - pstWireInfo->nKink2DistPremove;

			if (pstWireInfo->nKink3HtPremove + nKink3DistHtTime > nKink3DistTime)
				pstWireInfo->nKink3HtPremove = nKink3DistTime - nKink3DistHtTime;
		}
		else
			pstWireInfo->nKink3HtPremove = 0;

		if (stLoopConfig.nKink3DistPremove > 0)
		{
			if (pstWireInfo->nKink3HtPremove + nKink3DistHtTime < nKink3DistTime)
			{
				if (pstWireInfo->bTwistEnable)
				{
					pstWireInfo->nKink3DistPremove = round(nTwistHtTime
									* stLoopConfig.nKink3DistPremove / 100.0);
					nTotalTime = pstWireInfo->nKink3HtPremove + nKink3DistHtTime
											+ pstWireInfo->nKink3DistPremove;
					if (nTotalTime > nKink3DistTime)
						pstWireInfo->nKink3DistPremove = nKink3DistTime
							- pstWireInfo->nKink3HtPremove - nKink3DistHtTime;
					else
						nKink3DistHtTime = nKink3DistTime
											- pstWireInfo->nKink3HtPremove
											- pstWireInfo->nKink3DistPremove;
				}
				else
				{
					if (pstWireInfo->cLoopTopMode != ABOVE_LOOP_TOP_FAST)
					{
						pstWireInfo->nKink3DistPremove = round(nLoopTopTime
									* stLoopConfig.nKink3DistPremove / 100.0);
						nTotalTime = pstWireInfo->nKink3HtPremove
							+ nKink3DistHtTime + pstWireInfo->nKink3DistPremove;
						if (nTotalTime > nKink3DistTime)
							pstWireInfo->nKink3DistPremove = nKink3DistTime
								- pstWireInfo->nKink3HtPremove - nKink3DistHtTime;
						else
							nKink3DistHtTime = nKink3DistTime
											- pstWireInfo->nKink3HtPremove
											- pstWireInfo->nKink3DistPremove;
					}
					else
					{
						pstWireInfo->nKink3DistPremove = 0;
						nKink3DistHtTime = nKink3DistTime - pstWireInfo->nKink3HtPremove;
					}
				}
			}
			else
			{
				pstWireInfo->nKink3DistPremove = 0;
				nKink3DistHtTime = nKink3DistTime - pstWireInfo->nKink3HtPremove;
			}
		}
		else
		{
			pstWireInfo->nKink3DistPremove = 0;
			nKink3DistHtTime = nKink3DistTime - pstWireInfo->nKink3HtPremove;
		}
	}
	else
	{
		pstWireInfo->nKink3HtPremove = 0;
		pstWireInfo->nKink3DistPremove = 0;
	}

	if (pstWireInfo->bTwistEnable)
	{
		if ((stLoopConfig.nTwistHtPremove > 0) && (nTwistRevDistTime > nTwistRevDistHtTime))
		{
			pstWireInfo->nTwistHtPremove = round(nTwistHtTime
									* stLoopConfig.nTwistHtPremove / 100.0);
			if (pstWireInfo->nKink3DistPremove + pstWireInfo->nTwistHtPremove > nTwistHtTime)
				pstWireInfo->nTwistHtPremove = nTwistHtTime - pstWireInfo->nKink3DistPremove;

			if (pstWireInfo->nTwistHtPremove + nTwistRevDistHtTime > nTwistRevDistTime)
				pstWireInfo->nTwistHtPremove = nTwistRevDistTime - nTwistRevDistHtTime;

			nTwistRevDistHtTime = nTwistRevDistTime - pstWireInfo->nTwistHtPremove;
		}
		else
			pstWireInfo->nTwistHtPremove = 0;

		if (stLoopConfig.nTwistFwdDistPremove > 0)
		{
			if (nTwistFwdDistHtTime < nTwistFwdDistTime)
			{
				if (pstWireInfo->cLoopTopMode != ABOVE_LOOP_TOP_FAST)
				{
					pstWireInfo->nTwistFwdDistPremove = round(nLoopTopTime
								* stLoopConfig.nTwistFwdDistPremove / 100.0);
					nTotalTime = nTwistFwdDistHtTime + pstWireInfo->nTwistFwdDistPremove;
					if (nTotalTime > nTwistFwdDistTime)
						pstWireInfo->nTwistFwdDistPremove =
									nTwistFwdDistTime - nTwistFwdDistHtTime;
					else
						nTwistFwdDistHtTime = nTwistFwdDistTime
									- pstWireInfo->nTwistFwdDistPremove;
				}
				else
				{
					pstWireInfo->nTwistFwdDistPremove = 0;
					nTwistFwdDistHtTime = nTwistFwdDistTime;
				}
			}
			else
			{
				pstWireInfo->nTwistFwdDistPremove = 0;
				nTwistFwdDistHtTime = nTwistFwdDistTime;
			}
		}
		else
		{
			pstWireInfo->nTwistFwdDistPremove = 0;
			nTwistFwdDistHtTime = nTwistFwdDistTime;
		}
	}
	else
	{
		pstWireInfo->nTwistHtPremove = 0;
		pstWireInfo->nTwistFwdDistPremove = 0;
	}

	if (stLoopConfig.nLoopTopPremove > 0)
	{
		if (pstWireInfo->cLoopTopMode == ABOVE_LOOP_TOP_DISABLE)
		{
			if (pstWireInfo->bTwistEnable)
			{
				if (pstWireInfo->nTwistFwdDistPremove < nLoopTopTime)
				{
					pstWireInfo->nLoopTopPremove = round(nLoopTopTime
									* stLoopConfig.nLoopTopPremove / 100.0);

					if (pstWireInfo->nTwistFwdDistPremove + pstWireInfo->nLoopTopPremove > nLoopTopTime)
						pstWireInfo->nLoopTopPremove = nLoopTopTime - pstWireInfo->nTwistFwdDistPremove;
				}
				else
					pstWireInfo->nLoopTopPremove = 0;
			}
			else if (pstWireInfo->bKink3Enable == true)
			{
				if (pstWireInfo->nKink3DistPremove < nLoopTopTime)
				{
					pstWireInfo->nLoopTopPremove = round(nLoopTopTime
									* stLoopConfig.nLoopTopPremove / 100.0);

					if (pstWireInfo->nKink3DistPremove + pstWireInfo->nLoopTopPremove > nLoopTopTime)
						pstWireInfo->nLoopTopPremove = nLoopTopTime - pstWireInfo->nKink3DistPremove;
				}
				else
					pstWireInfo->nLoopTopPremove = 0;
			}
			else if (pstWireInfo->bKink2Enable == true)
			{
				if (pstWireInfo->nKink2DistPremove < nLoopTopTime)
				{
					pstWireInfo->nLoopTopPremove = round(nLoopTopTime
									* stLoopConfig.nLoopTopPremove / 100.0);

					if (pstWireInfo->nKink2DistPremove + pstWireInfo->nLoopTopPremove > nLoopTopTime)
						pstWireInfo->nLoopTopPremove = nLoopTopTime - pstWireInfo->nKink2DistPremove;
				}
				else
					pstWireInfo->nLoopTopPremove = 0;
			}
			else
			{
				if (pstWireInfo->nRevDistPremove < nLoopTopTime)
				{
					pstWireInfo->nLoopTopPremove = round(nLoopTopTime
									* stLoopConfig.nLoopTopPremove / 100.0);

					if (pstWireInfo->nRevDistPremove + pstWireInfo->nLoopTopPremove > nLoopTopTime)
						pstWireInfo->nLoopTopPremove = nLoopTopTime - pstWireInfo->nRevDistPremove;
				}
				else
					pstWireInfo->nLoopTopPremove = 0;
			}
		}
		else
			pstWireInfo->nLoopTopPremove = 0;
	}
	else
		//pstWireInfo->nLoopTopPremove = 0;		//v3.1.T395 delete
		pstWireInfo->nLoopTopPremove = stLoopConfig.nLoopTopPremove;	//v3.1.T395 add

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("---Premove----------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("Rev Ht/Dist: [%d, %d]\r\n"),
			pstWireInfo->nRevHtPremove, pstWireInfo->nRevDistPremove);
		sDebugInfo.AppendFormat(_T("Kink2 Ht/Dist: [%d, %d]\r\n"),
			pstWireInfo->nKink2HtPremove, pstWireInfo->nKink2DistPremove);
		sDebugInfo.AppendFormat(_T("Kink3 Ht/Dist: [%d, %d]\r\n"),
			pstWireInfo->nKink3HtPremove, pstWireInfo->nKink3DistPremove);
		sDebugInfo.AppendFormat(_T("Twist Ht/FwdDist: [%d, %d]\r\n"),
			pstWireInfo->nTwistHtPremove, pstWireInfo->nTwistFwdDistPremove);
		sDebugInfo.AppendFormat(_T("Loop Top: %d\r\n"), pstWireInfo->nLoopTopPremove);
	}

	short nSkewPercent;
	pstWireInfo->nRevDistHtPoint = 0;
	if (pstWireInfo->bFastLoop == false)
	{
#if _SERVO_TYPE == 2	//v3.1.T547 add
		pstWireInfo->nRevDistHtPoint = stLoopPara.stLoopBase.nRevArcFactor;
#else
		dMoveDist = pstWireInfo->nRevDistHt;
		dT1 = dT3 = round(nRevDistHtTime * theServo.GetServoSampleRate() / 2000.0);
		dT2 = 0;
		pstWireInfo->nRevDistHtPoint = round(dT1 + dT2 + dT3);
		nSkewPercent = stLoopPara.stLoopBase.nRevArcFactor;
		if (nSkewPercent == 100)
			Calc_generate_OBW_profile(dMoveDist, dT1, dT2, dT3, pstWireInfo->adRevDistHtMotion);
		else
			CalcArcMotionZ(dMoveDist, dT1, pstWireInfo->adRevDistHtMotion, nSkewPercent);

		for (i = pstWireInfo->nRevDistHtPoint - 1; i > 0; i--)
		{
			pstWireInfo->adRevDistHtMotion[i] =
				pstWireInfo->adRevDistHtMotion[i] - pstWireInfo->adRevDistHtMotion[i - 1];
		}
#endif
	}

	//v3.1.T326 add
	pstWireInfo->nPreKinkDistHtPoint = 0;
	if (pstWireInfo->bPreKinkEnable)
	{
#if _SERVO_TYPE == 2	//v3.1.T537 add
		pstWireInfo->nPreKinkDistHtPoint = stLoopPara.stLoopBase.nPreKinkArcFactor;
#else
		nSkewPercent = stLoopPara.stLoopBase.nPreKinkArcFactor;		//v3.1.T407 add
		dMoveDist = pstWireInfo->nPreKinkDistHt;
		dT1 = dT3 = round(nPreKinkDistHtTime * theServo.GetServoSampleRate() / 2000.0);
		pstWireInfo->nPreKinkDistHtPoint = round(dT1 + dT3);
		CalcArcMotionZ(dMoveDist, dT1, pstWireInfo->adPreKinkDistHtMotion, nSkewPercent);

		for (i = pstWireInfo->nPreKinkDistHtPoint - 1; i > 0; i--)
		{
			pstWireInfo->adPreKinkDistHtMotion[i] =
				pstWireInfo->adPreKinkDistHtMotion[i] - pstWireInfo->adPreKinkDistHtMotion[i - 1];
		}
#endif
	}

	pstWireInfo->nKink2DistHtPoint = 0;	//v3.1.T545 add
	if (pstWireInfo->bKink2Enable)
	{
#if _SERVO_TYPE == 2	//v3.1.T545 add
		pstWireInfo->nKink2DistHtPoint = stLoopPara.stLoopBase.nKink2ArcFactor;
#else
		dMoveDist = pstWireInfo->nKink2DistHt;
		dT1 = dT3 = round(nKink2DistHtTime * theServo.GetServoSampleRate() / 2000.0);	//v1.5.T128 edit
		dT2 = 0;
		pstWireInfo->nKink2DistHtPoint = round(dT1 + dT2 + dT3);
		nSkewPercent = stLoopPara.stLoopBase.nKink2ArcFactor;
		if (nSkewPercent == 100)
			Calc_generate_OBW_profile(dMoveDist, dT1, dT2, dT3, pstWireInfo->adKink2DistHtMotion);
		else
			CalcArcMotionZ(dMoveDist, dT1, pstWireInfo->adKink2DistHtMotion, nSkewPercent);

		for (i = pstWireInfo->nKink2DistHtPoint - 1; i > 0; i--)
		{
			pstWireInfo->adKink2DistHtMotion[i] =
				pstWireInfo->adKink2DistHtMotion[i] - pstWireInfo->adKink2DistHtMotion[i - 1];
		}
#endif
	}

	pstWireInfo->nKink3DistHtPoint = 0;	//v3.1.T545 add
	if (pstWireInfo->bKink3Enable)
	{
#if _SERVO_TYPE == 2	//v3.1.T545 add
		pstWireInfo->nKink3DistHtPoint = stLoopPara.stLoopBase.nKink3ArcFactor;
#else
		dMoveDist = pstWireInfo->nKink3DistHt;
		dT1 = dT3 = round(nKink3DistHtTime * theServo.GetServoSampleRate() / 2000.0);	//v1.5.T128 edit
		dT2 = 0;
		pstWireInfo->nKink3DistHtPoint = round(dT1 + dT2 + dT3);
		nSkewPercent = stLoopPara.stLoopBase.nKink3ArcFactor;
		if (nSkewPercent == 100)
			Calc_generate_OBW_profile(dMoveDist, dT1, dT2, dT3, pstWireInfo->adKink3DistHtMotion);
		else
			CalcArcMotionZ(dMoveDist, dT1, pstWireInfo->adKink3DistHtMotion, nSkewPercent);

		for (i = pstWireInfo->nKink3DistHtPoint - 1; i > 0; i--)
		{
			pstWireInfo->adKink3DistHtMotion[i] =
				pstWireInfo->adKink3DistHtMotion[i] - pstWireInfo->adKink3DistHtMotion[i - 1];
		}
#endif
	}

	if (pstWireInfo->bTwistEnable)
	{
		dMoveDist = pstWireInfo->nTwistRevDistHt;
		dT1 = dT3 = round(nTwistRevDistHtTime * theServo.GetServoSampleRate() / 800.0);	//v1.5.T128 edit
		dT2 = 0;
		pstWireInfo->nTwistRevDistZPoint = round(dT1 + dT2 + dT3);
		Calc_generate_OBW_profile(dMoveDist, dT1, dT2, dT3, pstWireInfo->adTwistRevDistZMotion);

		for (i = pstWireInfo->nTwistRevDistZPoint - 1; i > 0; i--)
		{
			pstWireInfo->adTwistRevDistZMotion[i] = pstWireInfo->adTwistRevDistZMotion[i] - pstWireInfo->adTwistRevDistZMotion[i - 1];
		}

		dMoveDist = pstWireInfo->nTwistFwdDistHt;
		dT1 = dT3 = round(nTwistFwdDistHtTime * theServo.GetServoSampleRate() / 2000.0);	//v1.5.T128 edit
		dT2 = 0;
		pstWireInfo->nTwistFwdDistZPoint = round(dT1 + dT2 + dT3);
		Calc_generate_OBW_profile(dMoveDist, dT1, dT2, dT3, pstWireInfo->adTwistFwdDistZMotion);

		for (i = pstWireInfo->nTwistFwdDistZPoint - 1; i > 0; i--)
		{
			pstWireInfo->adTwistFwdDistZMotion[i] = pstWireInfo->adTwistFwdDistZMotion[i] - pstWireInfo->adTwistFwdDistZMotion[i - 1];
		}
	}

	lMoveDist = round(fabs(pstWireInfo->stBondPos[0].dx - stCurTblPos.dx) * theServo.GetResolXY());
	pstWireInfo->nFirstMotionProfileX = GetTableAutoSpeedProfile(lMoveDist);	//v1.5.T146 edit

	lMoveDist = round(fabs(pstWireInfo->stBondPos[0].dy - stCurTblPos.dy) * theServo.GetResolXY());
	pstWireInfo->nFirstMotionProfileY = GetTableAutoSpeedProfile(lMoveDist);	//v1.5.T146 edit

	double dTrajStartLvl;
	TBL_POS stTrajStartPos;
	if (pstWireInfo->cLoopTopMode == ABOVE_LOOP_TOP_DISABLE)
	{
		stTrajStartPos = pstWireInfo->stLoopTopPos;
		dTrajStartLvl  = dLoopTopLvl;
	}
	else if ((pstWireInfo->cLoopTopMode == ABOVE_LOOP_TOP_NORMAL)
			|| (pstWireInfo->cLoopTopMode == ABOVE_LOOP_TOP_FAST))
	{
		stTrajStartPos = pstWireInfo->stAboveLoopTopPos;
		dTrajStartLvl  = dAboveLoopTopLvl;
	}

	dTravelX = pstWireInfo->stTrajEndPos.dx - stTrajStartPos.dx;
	dTravelY = pstWireInfo->stTrajEndPos.dy - stTrajStartPos.dy;

	dSpeed = pstWireInfo->nTrajectorySpeed / 100.0; 
	theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &stSpeedProfile);
	dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
	nMoveTimeX = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelX);

	theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &stSpeedProfile);
	dJerk = stSpeedProfile.lJerk / theServo.GetResolXY() / 1000.0 * dSpeed;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolXY() * dSpeed;
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolXY();
	nMoveTimeY = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTravelY);

	double adTrajX[500];
	if (nMoveTimeX > nMoveTimeY)
	{
		theServo.GetSpeedProfile(MOTOR_X, SPEED_PROFILE_0, &dJerk, &dMaxAcc, &dMaxVel);
		dJerk *= dSpeed;
		dMaxAcc *= dSpeed;
		Calc3rdOrderMotionProfile(dJerk, dMaxAcc, dMaxVel, dTravelX, &nTotalTime, adTrajX);
	}
	else
	{
		theServo.GetSpeedProfile(MOTOR_Y, SPEED_PROFILE_0, &dJerk, &dMaxAcc, &dMaxVel);
		dJerk *= dSpeed;
		dMaxAcc *= dSpeed;
		Calc3rdOrderMotionProfile(dJerk, dMaxAcc, dMaxVel, dTravelY, &nTotalTime, adTrajX);
	}

	double dWireAngleX = pstWireInfo->stTrajEndPos.dx - stTrajStartPos.dx;
	double dWireAngleY = pstWireInfo->stTrajEndPos.dy - stTrajStartPos.dy;
	double dWireAngle = atan2(dWireAngleY, dWireAngleX);
	double dSinAngle = sin(dWireAngle);
	double dCosAngle = cos(dWireAngle);
	double adTrajPos[2][500];
	try
	{
		if (nMoveTimeX > nMoveTimeY)
		{
			if (dCosAngle == 0) //throw "w.temp.34 dCosAngle==0" ;
			{
				AfxMessageBox(_T("temp.34, dCosAngle = 0"));
				return 0;
			}
			for (i = 0; i < nTotalTime; i++)
				adTrajPos[0][i] = fabs(adTrajX[i] / dCosAngle);
		}
		else
		{
			if (dSinAngle == 0) //throw "w.temp.35 dSinAngle==0" ;
			{
				AfxMessageBox(_T("temp.35, dCosAngle = 0"));
				return 0;
			}
			for (i = 0; i < nTotalTime; i++)
				adTrajPos[0][i] = fabs(adTrajX[i] / dSinAngle);
		}
	}
	catch (...)
	{
		return BOND_CALCULATE_ERROR;
	}

	double dTrajDist = Calc2PtDist(stTrajStartPos, pstWireInfo->stTrajEndPos);
	double dPreDist = Calc2PtDist(pstWireInfo->stBondPos[0], stTrajStartPos);

	dWireLen = Calc2PtDist(pstWireInfo->stBondPos[0], pstWireInfo->stTrajEndPos) * theServo.GetResolXY();

	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		CString str;
		str.Format(_T("CalcW_%d_Len_%.2f"), nWireNum, dWireLen);	//v3.1.T467 edit
		theDebug.AddInfo(str);
		str.Format(_T("Traj/PreDist_%.2f_%.2f"), dTrajDist, dPreDist);	//v3.1.T467 edit
		theDebug.AddInfo(str);
	}

	if (stLoopShape.cTrajMode == 1)
		dLoopLvl = lCtactLvl1;

	/* v3.1.T420 delete
	double dR0, dR1, dR2, dA, dB;
	dA  = dPreDist * dPreDist;
	dB  = (dTrajStartLvl - dLoopLvl) * theServo.GetResolZ() / theServo.GetResolXY();
	dR0 = dB;
	dB  = dB * dB;
	dR1 = dR0;
	double dSlope;
	*/
	//v3.1.T420 add
	double dR1 = (dTrajStartLvl - dLoopLvl) * theServo.GetResolZ() / theServo.GetResolXY();
	double dR2, dSlope;
	if (dLoopLvl > dSrchLvl2)
	{
		dR2 = dWireLen / theServo.GetResolXY();
		dSlope = sqrt(dR1 * dR1 + dR2 * dR2);
	}
	else
	{
		//v3.1.T420 delete
		//dA = dWireLen * dWireLen;
		//dB = (dSrchLvl2 - dLoopLvl) * theServo.GetResolZ();
		//v3.1.T420 add
		double dA = dWireLen * dWireLen;
		double dB = (dSrchLvl2 - dLoopLvl) * theServo.GetResolZ();

		dR2 = sqrt(dA + dB * dB) / theServo.GetResolXY();
		double dHt = dR1 - dB / theServo.GetResolXY();
		dSlope = sqrt(dHt * dHt + dA / theServo.GetResolXY() / theServo.GetResolXY());
	}

	if (dSlope == 0) //throw "w.temp.36" ;
	{
		AfxMessageBox(_T("temp.36, dSlope = 0"));
		return 0;
	}
	double dSinTheta = dWireLen / theServo.GetResolXY() / dSlope;
	double dMinR = dR1 * dSinTheta;

	double dTblPos;
	short nStartPoint = 0;
	short nSearchDelay;
	pstWireInfo->nTrajFactor = stLoopShape.nTrajFactor;	//v3.1.T318 add
	for (i = 0; i < nTotalTime; i++)
	{
		dTblPos = adTrajPos[0][i] - dPreDist;
		if (dTblPos > 0.0)
		{
			dSinAngle = (dR1 - sqrt(dR1 * dR1 + 4.0 * (dR2 - dR1) * dTblPos)) / (2.0 * (dR1 - dR2));
			dCosAngle = cos(asin(dSinAngle));
			double dSin2Angle = sin(asin(dSinAngle) * 2.0);

			double dOffsetR = (dR1 - dMinR) * dSin2Angle
					* (100.0 - pstWireInfo->nTrajFactor) / 100.0;	//v3.1.T318 edit

			double dR = dR1 - dOffsetR + (dR2 - dR1) * dSinAngle;
			//adTrajPos[1][i] = (dR * dCosAngle - dR0) * theServo.GetResolXY() / theServo.GetResolZ();	//v3.1.T420 delete
			adTrajPos[1][i] = (dR * dCosAngle - dR1) * theServo.GetResolXY() / theServo.GetResolZ();	//v3.1.T420 add
			if (adTrajPos[1][i] > 0.0)
			{
				adTrajPos[1][i] = 0.0;
				nSearchDelay = i;
			}
		}
		else
		{
			adTrajPos[1][i] = 0;
			nStartPoint = i;
			nSearchDelay = i;
		}
	}

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("--------------------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("Traj Srch Delay: %d\r\n"), nSearchDelay);
		sDebugInfo.AppendFormat(_T("Traj End Pos: %.2f\r\n"), adTrajPos[1][nTotalTime - 1]);
	}

	if (pstWireInfo->nLoopTopPremove > nSearchDelay / 4)
		pstWireInfo->nLoopTopPremove = nSearchDelay / 4;

	/*
	FILE *fp1;

	fopen_s(&fp1, "D:\\WBData\\TrajData1.txt", "wt");

	fprintf(fp1, "XY            Z\n");

	for (i = 0; i < nTotalTime; i++)
	{
		fprintf(fp1, "%.2f, %.2f\n", adTrajPos[0][i], adTrajPos[1][i]);
	}

	fclose(fp1);
	*/

	theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_3, &dJerk, &dMaxAcc, &dMaxVel);

	//v3.1.T436 add
	NON_SYM_CURVE_INPUT stNonSymCurveInput;
	stNonSymCurveInput.dDist = (dSrchLvl2 - dTrajStartLvl) * theServo.GetResolZ();
	stNonSymCurveInput.dMaxAcc = dMaxAcc;
	stNonSymCurveInput.dMaxVel = dMaxVel;
	stNonSymCurveInput.dVend = pstWireInfo->nSrchSpeed2 * (-2500.0) / theServo.GetServoSampleRate();	//v1.5.T128 edit
	stNonSymCurveInput.iFlagObjRule = NON_SYM_RULE_WEIGHTED_INV_RMS_ERR;
	stNonSymCurveInput.iPlotFlag = 2;
		stNonSymCurveInput.nFitLen = nTotalTime - nStartPoint;
	stNonSymCurveInput.pdTargetCurve = &adTrajPos[1][nStartPoint];
	stNonSymCurveInput.nTargetLen = nTotalTime - nStartPoint;

	//v3.1.T565 add
	if (theMcConfig.IsLoopTopPremove())
	{
		if (pstWireInfo->nLoopTopPremove < 0)
		{
			pstWireInfo->nLoopTopPremove = round(nTotalTime * pstWireInfo->nLoopTopPremove / 100.0);
			dT1 = round((nTotalTime + pstWireInfo->nLoopTopPremove)
					* 2000.0 / theServo.GetServoSampleRate());
		}
		else
			dT1 = round(nTotalTime * 2000.0 / theServo.GetServoSampleRate());
		CalcArcMotionZ(stNonSymCurveInput.dDist, dT1, pstWireInfo->adTrajPosZ, pstWireInfo->nTrajFactor);
		pstWireInfo->nTrajPointZ = round(dT1 + dT1);
		for (short i = pstWireInfo->nTrajPointZ - 1; i > 0; i--)
			pstWireInfo->adTrajPosZ[i] = pstWireInfo->adTrajPosZ[i] - pstWireInfo->adTrajPosZ[i - 1];
	}

	NON_SYM_CURVE_OUTPUT stNonSymCurveOutput;	//v3.1.T436 add
	short nError = CalcNonSymCurve(&stNonSymCurveInput, &stNonSymCurveOutput);
	if (nError != 0)
	{
		if (bDebug)
			sDebugInfo.AppendFormat(_T("CalcNonSymCurve Error: %d\r\n"), nError);

		FILE *fp;
		fopen_s(&fp, "D:\\WBData\\TrajDataErr.txt", "wt");
		fprintf(fp, "Wire: %d, Error: %d\n", nWireNum, nError);	//v1.5.T110 edit
		//v3.1.T437 add
		fprintf(fp, "Z Dist: %f, Vel: %f, Acc: %f\n",
			stNonSymCurveInput.dDist, stNonSymCurveInput.dMaxVel,
			stNonSymCurveInput.dMaxAcc);
		fprintf(fp, "FitLen: %d, StartPt: %d\n",
			stNonSymCurveInput.nFitLen, nStartPoint);

		fprintf(fp, "TotalTime: %d\n", nTotalTime);		//v1.5.T130 add
		for (i = 0; i < nTotalTime; i++)
			fprintf(fp, "%.2f, %.2f\n", adTrajPos[0][i], adTrajPos[1][i]);
		fclose(fp);

		return BOND_CALCULATE_ERROR;
	}

	//v1.5.T110 add
	if (bDebug)
	{
		FILE *fp;
		fopen_s(&fp, "D:\\WBData\\TrajData.txt", "wt");
		fprintf(fp, "Wire: %d, TotalTime: %d\n", nWireNum, nTotalTime);	//v1.5.T133 edit
		//v3.1.T437 add
		fprintf(fp, "Z Dist: %f, Vel: %f, Acc: %f\n",
			stNonSymCurveInput.dDist, stNonSymCurveInput.dMaxVel,
			stNonSymCurveInput.dMaxAcc);
		fprintf(fp, "FitLen: %d, StartPt: %d\n",
			stNonSymCurveInput.nFitLen, nStartPoint);

		for (i = 0; i < nTotalTime; i++)
			fprintf(fp, "%.2f, %.2f\n", adTrajPos[0][i], adTrajPos[1][i]);
		fclose(fp);
	}

	pstWireInfo->nTrajPoint = nStartPoint + 1
								+ stNonSymCurveOutput.stNonSymProfInput.uiT1
								+ stNonSymCurveOutput.stNonSymProfInput.uiT2
								+ stNonSymCurveOutput.stNonSymProfInput.uiT3;

	for (i = nTotalTime; i < pstWireInfo->nTrajPoint; i++)
		adTrajPos[0][i] = adTrajPos[0][nTotalTime - 1];

	//v3.1.T436 add
	double adDistProf[1024];
	double adVelProf[1024];
	double adAccProf[1024];
	double adJerkProf[1024];
	NON_SYM_PROFILE_OUTPUT stNonSymProfOutput;
	stNonSymProfOutput.pdAccProf  = adAccProf;
	stNonSymProfOutput.pdDistProf = adDistProf;
	stNonSymProfOutput.pdJerkProf = adJerkProf;
	stNonSymProfOutput.pdVelProf  = adVelProf;

	CalcNonSymProfile(&stNonSymCurveOutput.stNonSymProfInput, &stNonSymProfOutput);

	for (i = nStartPoint; i < pstWireInfo->nTrajPoint; i++)
		adTrajPos[1][i] = adDistProf[i - nStartPoint];

	//v3.1.T405 delete
	//if (bDebug)
	//{
	//	sDebugInfo.AppendFormat(_T("TrajPt: %d, TotalTime: %d\r\n"), pstWireInfo->nTrajPoint, nTotalTime);
	//	sDebugInfo.AppendFormat(_T("TrajEnd: %.3f\r\n"), adTrajPos[1][pstWireInfo->nTrajPoint - 1]);
	//}

	dWireAngleX = pstWireInfo->stTrajEndPos.dx - stTrajStartPos.dx;
	dWireAngleY = pstWireInfo->stTrajEndPos.dy - stTrajStartPos.dy;
	dWireAngle = atan2(dWireAngleY, dWireAngleX);
	dSinAngle = sin(dWireAngle);
	dCosAngle = cos(dWireAngle);

	pstWireInfo->adTrajPos[0] = adTrajPos[0][0] * dCosAngle;
	pstWireInfo->adTrajPos[pstWireInfo->nTrajPoint] = adTrajPos[0][0] * dSinAngle;
	pstWireInfo->adTrajPos[pstWireInfo->nTrajPoint * 2] = adTrajPos[1][0];

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("Traj Pt: %d, Total Time: %d\r\n"),
								pstWireInfo->nTrajPoint, nTotalTime);
		sDebugInfo.AppendFormat(_T("Traj End: %.3f\r\n"),
								adTrajPos[1][pstWireInfo->nTrajPoint - 1]);
	}

	for (i = 1; i < pstWireInfo->nTrajPoint; i++)
	{
		pstWireInfo->adTrajPos[i] = (adTrajPos[0][i] - adTrajPos[0][i-1]) * dCosAngle;
		pstWireInfo->adTrajPos[pstWireInfo->nTrajPoint + i] = (adTrajPos[0][i] - adTrajPos[0][i-1]) * dSinAngle;
		pstWireInfo->adTrajPos[pstWireInfo->nTrajPoint * 2 + i] = adTrajPos[1][i] - adTrajPos[1][i-1];
	}

	for (i = nStartPoint; i < pstWireInfo->nTrajPoint; i++)
		 pstWireInfo->adTrajPos[pstWireInfo->nTrajPoint * 2 + i] = adVelProf[i - nStartPoint];

	//Search height motion command generation
	dTrajDist = pstWireInfo->nSrchLvl1 - pstWireInfo->nResetLvl;

	theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_1, &stSpeedProfile);
	dJerk = stSpeedProfile.lJerk / theServo.GetResolZ() / 1000.0;
	dMaxAcc = stSpeedProfile.nMaxAcc / theServo.GetResolZ();
	dMaxVel = stSpeedProfile.nMaxVel / theServo.GetResolZ();

#if _SERVO_TYPE == 2	//v3.1.T270 add
	pstWireInfo->nSrchHtTime = pstPgm->nHeadTimeOffset;
#else
	pstWireInfo->nSrchHtTime = pstPgm->nHeadTimeOffset
				+ Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dTrajDist);
#endif

	//v3.1.T461 add
	pstWireInfo->nNextWireExist = 0;
	/* v3.1.T518 delete
	if (m_bRequestStopBond)
	{
		m_bRequestStopBond = false;
		m_bStopBondReady = true;
	}
	else
	{
	*/
		if ((pstWireInfo->nBumpType == BUMP_BOS)
			|| (pstWireInfo->nBumpType == BUMP_BSB))	//v3.1.T103 add
			pstWireInfo->nNextWireExist = 1;
		else if (m_bRequestStopBond)	//v3.1.T518 add
		{
			m_bRequestStopBond = false;
			m_bStopBondReady = true;
		}
		else
		{
			if (nWireNum >= thePgm.GetTotalWire())
				pstWireInfo->nNextWireExist = 0;
			else
			{
				short nNextWire = nWireNum + 1;
				pstWireInfo->nNextWireExist = CheckAheadAlignDone(&nNextWire);

				if (pstWireInfo->nNextWireExist == 2)
					pstWireInfo->nNextWireExist = 0;
				else
				{
					if (((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
						&& (thePgm.GetUnitTotalWire() == 1))
					{
						pstWireInfo->nNextWireExist = 1;
					}
					else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
					{
						if ((thePgm.GetUnitTotalWire() == 1)
							&& (pstPgm->nPrAheadCol == 1))
							pstWireInfo->nNextWireExist = 0;
						else
							pstWireInfo->nNextWireExist = 1;
					}
				}
			}
		}
	//}

	if (theMcConfig.GetAcsBufferType() == 1)	//v1.6.T4 add
		CalcMZ1(pstWire, pstWireInfo);

	return BOND_SUCCESS;
}

int CBond::CalcBallInfo(short nWireNum, TBL_POS stCurPos,
			ST_BALL_INFO *pstBallInfo, CString &sDebugInfo,
			bool bDebug, EN_BUMP_SEQ nBumpSeq)
{
	const WIRE_RECORD *pstWire = theWire.GetRcd(nWireNum);
    if (bDebug) sDebugInfo.AppendFormat(_T("WireNum: %d\r\n"), nWireNum);

	REF_RECORD stRefRcd1, stRefRcd2;
	theRef.GetRcd(&stRefRcd1, pstWire->nRefNum[0]);
	theRef.GetRcd(&stRefRcd2, pstWire->nRefNum[1]);    
	if (bDebug) sDebugInfo.AppendFormat(_T("RefNum: [%d, %d]\r\n"), pstWire->nRefNum[0], pstWire->nRefNum[1]);

	if ((theMatrix.GetRcd(pstWire->nRefNum[0])->nVisionStatus != VISION_SUCCESS) ||
		(theMatrix.GetRcd(pstWire->nRefNum[1])->nVisionStatus != VISION_SUCCESS))
		return BOND_NO_ALIGNMENT_ERROR;

	ST_BUMP_BALL stBumpBall;	//v3.1.T410 add
	ST_BUMP_SHAPE stBumpShape;
	pstBallInfo->nBumpType = pstWire->nBumpType;
	if ((pstBallInfo->nBumpType == BUMP_BOS)
		|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL2)))
	{	//BBOS
		theBallPara.GetBbosBall(&stBumpBall);	//v3.1.T410 add
		theBallPara.GetBbosShape(&stBumpShape);
		stBumpShape.chShapeMode = pstWire->cBbosShapeBumpShapeMode;	//v3.1.T362 add
		stBumpShape.nBaseHeight += pstWire->nBbosShapeBaseHeightOffset;
		stBumpShape.nSpanDist += pstWire->nBbosShapeSpanDistOffset;
		stBumpShape.nScrubDist += pstWire->nBbosShapeScrubDistOffset;
		stBumpShape.nBallThickness += pstWire->nBbosShapeBallThicknessOffset;
	}
	else if ((pstBallInfo->nBumpType == BUMP_SOB)
			|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL)))
	{	//BSOB
		theBallPara.GetBsobBall(&stBumpBall);	//v3.1.T410 add
		theBallPara.GetBsobShape(&stBumpShape);
		stBumpShape.chShapeMode = pstWire->cBsobShapeBumpShapeMode;	//v3.1.T362 add
		stBumpShape.nBaseHeight += pstWire->nBsobShapeBaseHeightOffset;
		stBumpShape.nSpanDist += pstWire->nBsobShapeSpanDistOffset;
		stBumpShape.nScrubDist += pstWire->nBsobShapeScrubDistOffset;
		stBumpShape.nBallThickness += pstWire->nBsobShapeBallThicknessOffset;
	}

    //Calculate the bond point position
	TBL_POS stBondPt[2];
	if (m_nBondMode == BONDMODE_CORRECT)
		theWire.CalcBondPoint(nWireNum, &stBondPt[0], &stBondPt[1], true);
	else
		theWire.CalcBondPoint(nWireNum, &stBondPt[0], &stBondPt[1]);

	//v3.1.T372 add, v3.1.T410 delete
	//short nPosOffset = 0;
	//if ((pstBallInfo->nBumpType == BUMP_BOS)
	//	|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL2)))
	//	nPosOffset = theMcConfig.GetBbosPosOffset();	//v3.1.T410 delete
	//else if ((pstBallInfo->nBumpType == BUMP_SOB)
	//		|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL)))
	//	nPosOffset = theMcConfig.GetBsobPosOffset();
	if (stBumpBall.nPosOffset != 0)
	{
		TBL_POS stPosOffset = CalcDistAlongWire(stBondPt[0], stBondPt[1], stBumpBall.nPosOffset);
		stBondPt[0].dx += stPosOffset.dx;
		stBondPt[0].dy += stPosOffset.dy;
		stBondPt[1].dx += stPosOffset.dx;
		stBondPt[1].dy += stPosOffset.dy;
	}

	int nWireCtactLvl;
	short nCtactBufferIndex = thePgm.CalcCtactBufferIndex(nWireNum,
								m_nCurRow, m_nCurCol, theMHS.GetCurBondUnit());
	long lTempCtactLvl1 = stRefRcd1.lCtactLvl;
	long lTempCtactLvl2 = stRefRcd2.lCtactLvl;

	bool bCtactValid = theCtact.GetCtactLvl(nCtactBufferIndex, FIRST_BOND, &nWireCtactLvl);
	if (bCtactValid) stRefRcd1.lCtactLvl = nWireCtactLvl;

	bCtactValid = theCtact.GetCtactLvl(nCtactBufferIndex, SECOND_BOND, &nWireCtactLvl);
	if (bCtactValid) stRefRcd2.lCtactLvl = nWireCtactLvl;

	if (thePgm.GetMultiBtos() == BTOS_DIR_ENABLE_X)
	{
		stBondPt[0].dx += stRefRcd1.stBtos.dx;
		stBondPt[0].dy += stRefRcd1.stBtos.dy;
		long lCtactDiff =  lTempCtactLvl1 - stRefRcd1.lCtactLvl;
		stBondPt[0].dx -= lCtactDiff * theServo.GetResolZ() * tan((double)NEW_OPTICS_X_ANGLE) / theServo.GetResolXY();

		stBondPt[1].dx += stRefRcd2.stBtos.dx;
		stBondPt[1].dy += stRefRcd2.stBtos.dy;
		lCtactDiff =  lTempCtactLvl2 - stRefRcd2.lCtactLvl;
		stBondPt[1].dx -= lCtactDiff * theServo.GetResolZ() * tan((double)NEW_OPTICS_X_ANGLE) / theServo.GetResolXY();
	}
	else if (thePgm.GetMultiBtos() == BTOS_DIR_ENABLE_Y)
	{
		stBondPt[0].dx += stRefRcd1.stBtos.dx;
		stBondPt[0].dy += stRefRcd1.stBtos.dy;
		long lCtactDiff = lTempCtactLvl1 - stRefRcd1.lCtactLvl;
		stBondPt[0].dx += lCtactDiff * theServo.GetResolZ() * tan((double)NEW_OPTICS_Y_ANGLE) / theServo.GetResolXY();

		stBondPt[1].dx += stRefRcd2.stBtos.dx;
		stBondPt[1].dy += stRefRcd2.stBtos.dy;
		lCtactDiff =  lTempCtactLvl2 - stRefRcd2.lCtactLvl;
		stBondPt[1].dx += lCtactDiff * theServo.GetResolZ() * tan((double)NEW_OPTICS_Y_ANGLE) / theServo.GetResolXY();
	}
	else
	{
		double dLenCompen = CalcTransducerLenCompen(nWireNum);	//v1.1.288.T2 add
		const MACHINE_DATA *pstMcData = theMcData.GetRcd();
		stBondPt[0].dx += pstMcData->stBondTipOs.dx;
		stBondPt[0].dy += pstMcData->stBondTipOs.dy + dLenCompen;	//v1.1.288.T2 edit
		stBondPt[1].dx += pstMcData->stBondTipOs.dx;
		stBondPt[1].dy += pstMcData->stBondTipOs.dy + dLenCompen;	//v1.1.288.T2 edit
	}

    pstBallInfo->stBondPos[0] = stBondPt[1];
    RoundPosition(&pstBallInfo->stBondPos[0]);

	pstBallInfo->stBondPos[1] = CalcPosAlongWire(
					stBondPt[1], stBondPt[0],
					-stBumpShape.nSpanDist / theServo.GetResolXY(),
					-stBumpShape.nDirectionAngle);	//v3.1.T372 add
    RoundPosition(&pstBallInfo->stBondPos[1]);

    pstBallInfo->chBumpShapeMode = stBumpShape.chShapeMode;
    if (pstBallInfo->chBumpShapeMode == BUMP_SHAPE_FLEX)
    {
		pstBallInfo->stFlexPos = CalcPosAlongWire(
					pstBallInfo->stBondPos[0], pstBallInfo->stBondPos[1],
					-stBumpShape.nFlexLength / theServo.GetResolXY());	//v3.1.T372 add
    }
    else
        pstBallInfo->stFlexPos = pstBallInfo->stBondPos[0];
    RoundPosition(&pstBallInfo->stFlexPos);

	pstBallInfo->stScrubPos = CalcPosAlongWire(
					pstBallInfo->stBondPos[1], pstBallInfo->stBondPos[0],
					-stBumpShape.nScrubDist / theServo.GetResolXY());	//v3.1.T372 add
    RoundPosition(&pstBallInfo->stScrubPos);

    double dWireLen = Calc2PtDist(pstBallInfo->stBondPos[0], pstBallInfo->stBondPos[1]) * theServo.GetResolXY();
    if (bDebug) sDebugInfo.AppendFormat(_T("WireLen: %.2f\r\n"), dWireLen);
    if (dWireLen > 1000) return BOND_WIRE_LENGTH_ERROR;

	//ST_BUMP_BALL stBumpBall;	//v3.1.T410 delete
	if ((pstBallInfo->nBumpType == BUMP_BOS)
		|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL2)))
	{
		//theBallPara.GetBbosBall(&stBumpBall);	//v3.1.T410 delete
		pstBallInfo->nStandbyPower[0]  = round((stBumpBall.fStandbyPower[0] + pstWire->fBbosBump1stPreCtactPowerOffset) * 2.55);
		pstBallInfo->nPreCtactForce[0] = stBumpBall.nPreCtactForce[0] + pstWire->nBbosBump1stPreCtactForceOffset;
		pstBallInfo->nPreCtactTime[0]  = stBumpBall.nPreCtactTime[0] + pstWire->nBbosBump1stPreCtactTimeOffset;
		pstBallInfo->nCtactTime[0]     = stBumpBall.nCtactTime[0] + pstWire->nBbosBumpCtactTimeOffset[0];
		pstBallInfo->nCtactForce[0]    = stBumpBall.nCtactForce[0] + pstWire->nBbosBumpCtactForceOffset[0];
		pstBallInfo->nCtactPower[0]    = round((stBumpBall.fCtactPower[0] + pstWire->fBbosBumpCtactPowerOffset[0]) * 2.55);
		pstBallInfo->nBaseTime[0]      = stBumpBall.nBaseTime[0] + pstWire->nBbosBumpTimeOffset[0];
		pstBallInfo->nBaseForce[0]     = stBumpBall.nBaseForce[0] + pstWire->nBbosBumpForceOffset[0];
		pstBallInfo->nBasePower[0]     = round((stBumpBall.fBasePower[0] + pstWire->fBbosBumpPowerOffset[0]) * 2.55);
	}
	else if ((pstBallInfo->nBumpType == BUMP_SOB)
			|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL)))
	{
		//theBallPara.GetBsobBall(&stBumpBall);	//v3.1.T410 delete
		pstBallInfo->nStandbyPower[0]  = round((stBumpBall.fStandbyPower[0] + pstWire->fBump1stPreCtactPowerOffset) * 2.55);
		pstBallInfo->nPreCtactForce[0] = stBumpBall.nPreCtactForce[0] + pstWire->nBump1stPreCtactForceOffset;
		pstBallInfo->nPreCtactTime[0]  = stBumpBall.nPreCtactTime[0] + pstWire->nBump1stPreCtactTimeOffset;
		pstBallInfo->nCtactTime[0]     = stBumpBall.nCtactTime[0] + pstWire->nBumpCtactTimeOffset[0];
		pstBallInfo->nCtactForce[0]    = stBumpBall.nCtactForce[0] + pstWire->nBumpCtactForceOffset[0];
		pstBallInfo->nCtactPower[0]    = round((stBumpBall.fCtactPower[0] + pstWire->fBumpCtactPowerOffset[0]) * 2.55);
		pstBallInfo->nBaseTime[0]      = stBumpBall.nBaseTime[0] + pstWire->nBumpTimeOffset[0];
		pstBallInfo->nBaseForce[0]     = stBumpBall.nBaseForce[0] + pstWire->nBumpForceOffset[0];
		pstBallInfo->nBasePower[0]     = round((stBumpBall.fBasePower[0] + pstWire->fBumpPowerOffset[0]) * 2.55);
	}

	if (theMcConfig.IsExceedTime1(((nWireNum - 1) % 5) + 1, pstBallInfo->nBaseTime[0]))
		return BOND_EXCEED_TIME_1;
#if _SERVO_TYPE == 1	//v3.0.T485 add
	pstBallInfo->nBaseTime[0] += 1;	//v1.5.T43 edit, v1.5.T59 edit
#endif
    pstBallInfo->nPowerDelay[0]    = 0;
	if (theMcConfig.IsExceedForce1(((nWireNum - 1) % 5) + 1, pstBallInfo->nBaseForce[0]))
		return BOND_EXCEED_FORCE_1;

#if _SERVO_TYPE < 2		//v3.1.T303 add
	if (pstBallInfo->nCtactTime[0] == 0)
        pstBallInfo->nCtactForce[0] = pstBallInfo->nBaseForce[0];
#endif

	if (theMcConfig.IsExceedPower1(((nWireNum - 1) % 5) + 1, pstBallInfo->nBasePower[0]))
		return BOND_EXCEED_POWER_1;
    pstBallInfo->nReleaseTime[0]   = stBumpBall.nRlsTime[0];
    pstBallInfo->nReleaseForce[0]  = stBumpBall.nRlsForce[0];
    pstBallInfo->nReleasePower[0]  = stBumpBall.nRlsPower[0];
    pstBallInfo->nStandbyPower[1]  = round(stBumpBall.fStandbyPower[1] * 2.55);
    pstBallInfo->nPreCtactForce[1] = stBumpBall.nPreCtactForce[1];
    pstBallInfo->nPreCtactTime[1]  = stBumpBall.nPreCtactTime[1];

	if ((pstBallInfo->nBumpType == BUMP_BOS)
		|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL2)))
	{
		pstBallInfo->nCtactTime[1]  = stBumpBall.nCtactTime[1] + pstWire->nBbosBumpCtactTimeOffset[1];
		pstBallInfo->nCtactForce[1] = stBumpBall.nCtactForce[1] + pstWire->nBbosBumpCtactForceOffset[1];
		pstBallInfo->nCtactPower[1] = round((stBumpBall.fCtactPower[1] + pstWire->fBbosBumpCtactPowerOffset[1]) * 2.55);
		pstBallInfo->nBaseTime[1]   = stBumpBall.nBaseTime[1] + pstWire->nBbosBumpTimeOffset[1];
		pstBallInfo->nBaseForce[1]  = stBumpBall.nBaseForce[1] + pstWire->nBbosBumpForceOffset[1];
		pstBallInfo->nBasePower[1]  = round((stBumpBall.fBasePower[1] + pstWire->fBbosBumpPowerOffset[1]) * 2.55);
	}
	else if ((pstBallInfo->nBumpType == BUMP_SOB)
			|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL)))
	{
		pstBallInfo->nCtactTime[1]  = stBumpBall.nCtactTime[1] + pstWire->nBumpCtactTimeOffset[1];
		pstBallInfo->nCtactForce[1] = stBumpBall.nCtactForce[1] + pstWire->nBumpCtactForceOffset[1];
		pstBallInfo->nCtactPower[1] = round((stBumpBall.fCtactPower[1] + pstWire->fBumpCtactPowerOffset[1]) * 2.55);
		pstBallInfo->nBaseTime[1]   = stBumpBall.nBaseTime[1] + pstWire->nBumpTimeOffset[1];
		pstBallInfo->nBaseForce[1]  = stBumpBall.nBaseForce[1] + pstWire->nBumpForceOffset[1];
		pstBallInfo->nBasePower[1]  = round((stBumpBall.fBasePower[1] + pstWire->fBumpPowerOffset[1]) * 2.55);
	}

	if (theMcConfig.IsExceedTime2(((nWireNum - 1) % 5) + 1, pstBallInfo->nBaseTime[1]))
		return BOND_EXCEED_TIME_2;
#if _SERVO_TYPE == 1	//v3.0.T485 add
	pstBallInfo->nBaseTime[1] += 1;	//v1.5.T43 edit, v1.5.T59 edit
#endif
	pstBallInfo->nPowerDelay[1] = 0;
	if (theMcConfig.IsExceedForce2(((nWireNum - 1) % 5) + 1, pstBallInfo->nBaseForce[1]))
		return BOND_EXCEED_FORCE_2;

#if _SERVO_TYPE < 2		//v3.1.T303 add
	if (pstBallInfo->nCtactTime[1] == 0)
        pstBallInfo->nCtactForce[1] = pstBallInfo->nBaseForce[1];
#endif

	if (theMcConfig.IsExceedPower2(((nWireNum - 1) % 5) + 1, pstBallInfo->nBasePower[1]))
		return BOND_EXCEED_POWER_2;
    pstBallInfo->nReleaseTime[1]  = stBumpBall.nRlsTime[1];
    pstBallInfo->nReleaseForce[1] = stBumpBall.nRlsForce[1];
    pstBallInfo->nReleasePower[1] = stBumpBall.nRlsPower[1];

    pstBallInfo->bTailBondEnable = stBumpBall.bTailBondEnable;
    if (pstBallInfo->bTailBondEnable == false)
    {
        pstBallInfo->nStandbyPower[1] = 0;
        pstBallInfo->nCtactTime[1] = 0;
        pstBallInfo->nBaseTime[1] = 0;
    }

	//v1.1.288.T13 add
	if (theDebug.IsDebugTransducer())
	{
		m_sTemp.Format(_T("CapWearoutPower1/2_%d/%d"),
					CalcCapiWearOutPower1(), CalcCapiWearOutPower2());
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
	}

	//v1.1.288.T11 edit
	CompensateCapiWearOutPower1(&pstBallInfo->nStandbyPower[0]);
    CompensateCapiWearOutPower1(&pstBallInfo->nCtactPower[0]);
    CompensateCapiWearOutPower1(&pstBallInfo->nBasePower[0]);
    CompensateCapiWearOutPower1(&pstBallInfo->nReleasePower[0]);
    CompensateCapiWearOutPower2(&pstBallInfo->nStandbyPower[1]);
    CompensateCapiWearOutPower2(&pstBallInfo->nCtactPower[1]);
    CompensateCapiWearOutPower2(&pstBallInfo->nBasePower[1]);
    CompensateCapiWearOutPower2(&pstBallInfo->nReleasePower[1]);

	const BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();
	pstBallInfo->nWclTailClosePretrig     = pstBondPara->nWclTailClosePretrig;
	pstBallInfo->nWclLoopPeakClosePretrig = pstBondPara->nWclLoopPeakClosePretrig;
	pstBallInfo->nWclLoopPeakCloseTime    = pstBondPara->nWclLoopPeakCloseTime;
	pstBallInfo->nWcl1stBondOpenPretrig   = pstBondPara->nWcl1stBondOpenPretrig;
	pstBallInfo->nWcl2ndBondOpenPretrig   = pstBondPara->nWcl2ndBondOpenPretrig;
	pstBallInfo->nCtactPowerRamp[0]       = pstBondPara->nCtactPowerRamp[0];
	pstBallInfo->nCtactPowerRamp[1]       = pstBondPara->nCtactPowerRamp[1];
	pstBallInfo->nBasePowerRamp[0]        = pstBondPara->nBasePowerRamp[0];
	pstBallInfo->nBasePowerRamp[1]        = pstBondPara->nBasePowerRamp[1];
	pstBallInfo->nReleasePowerRamp[0]     = pstBondPara->nReleasePowerRamp[0];
	pstBallInfo->nReleasePowerRamp[1]     = pstBondPara->nReleasePowerRamp[1];

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("--------------------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("Pre T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstBallInfo->nPreCtactTime[0], pstBallInfo->nPreCtactTime[1],
			pstBallInfo->nPreCtactForce[0], pstBallInfo->nPreCtactForce[1],
			pstBallInfo->nStandbyPower[0], pstBallInfo->nStandbyPower[1]);
		sDebugInfo.AppendFormat(_T("Ctact T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstBallInfo->nCtactTime[0], pstBallInfo->nCtactTime[1],
			pstBallInfo->nCtactForce[0], pstBallInfo->nCtactForce[1],
			pstBallInfo->nCtactPower[0], pstBallInfo->nCtactPower[1]);
		sDebugInfo.AppendFormat(_T("Base T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstBallInfo->nBaseTime[0], pstBallInfo->nBaseTime[1],
			pstBallInfo->nBaseForce[0], pstBallInfo->nBaseForce[1],
			pstBallInfo->nBasePower[0], pstBallInfo->nBasePower[1]);
		sDebugInfo.AppendFormat(_T("Rls T/F/P: [%d, %d] [%d, %d] [%d, %d]\r\n"),
			pstBallInfo->nReleaseTime[0], pstBallInfo->nReleaseTime[1],
			pstBallInfo->nReleaseForce[0], pstBallInfo->nReleaseForce[1],
			pstBallInfo->nReleasePower[0], pstBallInfo->nReleasePower[1]);
	}

	if (((pstBallInfo->nBumpType == BUMP_SOB)
			|| (pstBallInfo->nBumpType == BUMP_BSB))	//v3.1.T103 add
		&& (pstWire->chBumpBondScrubMode > 0))
	{
		SCRUB_PARA_RCD stScrubRcd;
		theBondPara.GetBondScrubRcd(FIRST_BOND, pstWire->chBumpBondScrubMode - 1, &stScrubRcd);
		stBumpBall.b1stBondScrubEnable = true;
		stBumpBall.ch1stScrubDirection = stScrubRcd.chScrubDirection;
		stBumpBall.n1stScrubAmplitude  = stScrubRcd.nScrubAmplitude;
		stBumpBall.n1stScrubCycle      = stScrubRcd.nScrubCycle;
		stBumpBall.n1stScrubForce      = stScrubRcd.nScrubForce;
		stBumpBall.f1stScrubPower      = stScrubRcd.fScrubPower;
		stBumpBall.n1stScrubSpeed      = stScrubRcd.nScrubSpeed;
	}
	else
		stBumpBall.b1stBondScrubEnable = false;

	double dMoveDist, dMaxVel, dMaxAcc, dJerk;
	SERVO_SPEED_PROFILE stServoSpeed;
    if (stBumpBall.b1stBondScrubEnable == true)
    {
        pstBallInfo->b1stBondScrubEnable = true;

        short nScrubAngle = 0;
        if (stBumpBall.ch1stScrubDirection == 2)
        {
			pstBallInfo->st1stScrubAmp.dx = stBumpBall.n1stScrubAmplitude / theServo.GetResolXY();
			pstBallInfo->st1stScrubAmp.dy = stBumpBall.n1stScrubAmplitude / theServo.GetResolXY();
        }
        else
        {
            if (stBumpBall.ch1stScrubDirection == 0)
                nScrubAngle = 90;
            else
                nScrubAngle = 0;

			pstBallInfo->st1stScrubAmp = CalcPosAlongWire(
				pstBallInfo->stBondPos[0], stBondPt[0],
				-stBumpBall.n1stScrubAmplitude / theServo.GetResolXY(), nScrubAngle);	//v3.1.T372 add

            pstBallInfo->st1stScrubAmp.dx -= pstBallInfo->stBondPos[0].dx;
            pstBallInfo->st1stScrubAmp.dy -= pstBallInfo->stBondPos[0].dy;
        }

        pstBallInfo->n1stScrubCycle = stBumpBall.n1stScrubCycle;
        pstBallInfo->n1stScrubForce = stBumpBall.n1stScrubForce;
        pstBallInfo->n1stScrubPower = round(stBumpBall.f1stScrubPower * 2.55);
        pstBallInfo->n1stScrubSpeed = stBumpBall.n1stScrubSpeed;

		switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
		{
		case MHS_VLED: case MHS_VLED_ELITE:
			theServo.GetSpeedProfile(MOTOR_Y, 0, &stServoSpeed);
			break;
		default:	//v3.0.T254 add
			theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
		}
        dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0 * stBumpBall.n1stScrubSpeed / 100.0;
        dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY() * stBumpBall.n1stScrubSpeed / 100.0;
        dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();
		dMoveDist = stBumpBall.n1stScrubAmplitude / theServo.GetResolXY();
		short nScrubT1 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
		dMoveDist = stBumpBall.n1stScrubAmplitude * 2.0 / theServo.GetResolXY();
		short nScrubT2 = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
        if (stBumpBall.ch1stScrubDirection == 2)
            pstBallInfo->n1stScrubYDelay = nScrubT2 / 2;
        else
            pstBallInfo->n1stScrubYDelay = 0;
        pstBallInfo->n1stScrubTime = nScrubT1 * 2 + 
            (pstBallInfo->n1stScrubCycle * 2 - 1) * nScrubT2 + pstBallInfo->n1stScrubYDelay;
    }
    else
    {
        pstBallInfo->st1stScrubAmp.dx    = 0.0;
        pstBallInfo->st1stScrubAmp.dy    = 0.0;
        pstBallInfo->n1stScrubYDelay     = 0;
        pstBallInfo->b1stBondScrubEnable = false;
        pstBallInfo->n1stScrubCycle      = 0;
        pstBallInfo->n1stScrubForce      = 0;
        pstBallInfo->n1stScrubPower      = 0;
        pstBallInfo->n1stScrubTime       = 0;
        pstBallInfo->n1stScrubSpeed      = 100;
    }

    if (m_bDryRun)
    {
        stRefRcd1.lCtactLvl += round(DRYRUN_CTACT_OFFSET);
        stRefRcd2.lCtactLvl += round(DRYRUN_CTACT_OFFSET);
    }
    pstBallInfo->nCtactLvl1 = stRefRcd2.lCtactLvl;
    pstBallInfo->nCtactLvl2 = stRefRcd2.lCtactLvl;
    if (bDebug) sDebugInfo.AppendFormat(_T("CtactLevel: [%d, %d]\r\n"), pstBallInfo->nCtactLvl1, pstBallInfo->nCtactLvl2);

    double dBaseHtLvl, dFlexKinkLvl, dFlexHtLvl;
    dBaseHtLvl = (double)pstBallInfo->nCtactLvl1 + (double)stBumpShape.nBaseHeight / theServo.GetResolZ();
    if (pstBallInfo->chBumpShapeMode == BUMP_SHAPE_FLEX)
    {
        double dFlexAngle = (double)stBumpShape.nFlexAngle * PI / 180.0;
        dFlexKinkLvl = dBaseHtLvl - stBumpShape.nFlexLength * tan(dFlexAngle) / theServo.GetResolZ();

        if ((dFlexKinkLvl - pstBallInfo->nCtactLvl1) < (10.0 / theServo.GetResolZ()))
            dFlexKinkLvl = pstBallInfo->nCtactLvl1 + 10.0 / theServo.GetResolZ();

        dFlexHtLvl = dFlexKinkLvl + (double)stBumpShape.nFlexHeight / theServo.GetResolZ();
    }
    else
        dFlexKinkLvl = dFlexHtLvl = dBaseHtLvl;

	//v3.1.T330 edit
    double dSpanHtLvl = pstBallInfo->nCtactLvl2 + round(stBumpShape.nBallThickness / theServo.GetResolZ());;
    double dBaseLvl2 = dSpanHtLvl;
	if (theMcConfig.GetBumpBaseHt2() > 0)
		dBaseLvl2 = pstBallInfo->nCtactLvl2 + round(theMcConfig.GetBumpBaseHt2() / theServo.GetResolZ());

    pstBallInfo->nMotionSpeed = stBumpShape.nMotionSpeed;
    pstBallInfo->nSrchSpeed1 = stBumpBall.nSrchSpeed1;
    pstBallInfo->nSrchSpeed2 = stBumpBall.nSrchSpeed2;
    
    if (m_bDryRun)
    {
        pstBallInfo->nSrchTol1   = 0;
        pstBallInfo->nSrchTol2   = 0;
        pstBallInfo->nSrchSpeed1 = 0;
        pstBallInfo->nSrchSpeed2 = 0;
    }
    else
    {
        pstBallInfo->nSrchTol1 = stBumpBall.nSrchTol1;
        pstBallInfo->nSrchTol2 = stBumpBall.nSrchTol2;
    }

    pstBallInfo->nSrchDelay[0] = stBumpBall.nCtactSrchDelay1;
    pstBallInfo->nSrchDelay[1] = 2;
    pstBallInfo->nSrchLvl1 = pstBallInfo->nCtactLvl1
		+ round((double)stBumpBall.nSrchHeight1 / theServo.GetResolZ());
    pstBallInfo->nBaseHt = round(dBaseHtLvl) - pstBallInfo->nCtactLvl1;
    pstBallInfo->nFlexSpanHt = round(dFlexKinkLvl - dBaseHtLvl);
    pstBallInfo->nFlexHt = round(dFlexHtLvl - dFlexKinkLvl);
    pstBallInfo->nSpanHt = round(dSpanHtLvl - dFlexHtLvl);
    pstBallInfo->nBaseHt2 = round(dBaseLvl2 - dSpanHtLvl);	//v3.1.T330 edit
    pstBallInfo->nBumpCutPower = stBumpShape.nCuttingPower;

    dMoveDist = stBumpShape.nBaseHeight;
    theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_1, &stServoSpeed);
    dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolZ() / 1000.0 * stBumpShape.nMotionSpeed / 100.0;
    dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolZ() * stBumpShape.nMotionSpeed / 100.0;
    dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolZ() * stBumpShape.nMotionSpeed / 100.0;
    pstBallInfo->nBumpCutPowerDelay = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);

    theServo.GetSpeedProfile(MOTOR_X, 0, &stServoSpeed);
    dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolXY() / 1000.0 * stBumpShape.nMotionSpeed / 100.0;
    dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolXY() * stBumpShape.nMotionSpeed / 100.0;
    dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolXY();
	double dPartDist = 0.0;
    if (pstBallInfo->chBumpShapeMode == BUMP_SHAPE_FAST)
    {
        dMoveDist = (double)(stBumpShape.nSpanDist + stBumpShape.nScrubDist) / theServo.GetResolXY();
        dPartDist = (double)stBumpShape.nSpanDist / theServo.GetResolXY();
    }
    else
        dMoveDist = (double)(stBumpShape.nSpanDist) / theServo.GetResolXY();
	short nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
    pstBallInfo->nBumpCutPowerDelay += round((double)nMoveTime * (double)stBumpShape.nCuttingPowerDelay / 100.0);
    pstBallInfo->nBumpCutPowerTime = round((double)nMoveTime * (100.0 - (double)stBumpShape.nCuttingPowerDelay) / 100.0);

    if ((pstBallInfo->chBumpShapeMode == BUMP_SHAPE_FAST)
		&& (fabs(dPartDist) < fabs(dMoveDist)))
        Calc3rdOrderPartialMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist, dPartDist, &nMoveTime);

	//v3.1.T332 add, v3.1.T410 delete
	//if (theMcConfig.GetArcSpanDelay() > 0)
	//{
	//	pstBallInfo->nArcSpanDelay = (short)(nMoveTime * theMcConfig.GetArcSpanDelay() / 100.0);
	//	nMoveTime -= pstBallInfo->nArcSpanDelay;
	//}
	//v3.1.T410 add
	if (stBumpBall.nArcSpanDelay > 0)
	{
		pstBallInfo->nArcSpanDelay = (short)(nMoveTime * stBumpBall.nArcSpanDelay / 100.0);
		nMoveTime -= pstBallInfo->nArcSpanDelay;
	}

	//v3.1.T384 add, v3.1.T410 delete
	//pstBallInfo->nSpanLenDelay = 0;
	//if ((pstBallInfo->nBumpType == BUMP_SOB)
	//	|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL)))
	//	pstBallInfo->nSpanLenDelay = theMcConfig.GetBsobSpanLenDelay();
	//else if ((pstBallInfo->nBumpType == BUMP_BOS)
	//		|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL2)))
	//	pstBallInfo->nSpanLenDelay = theMcConfig.GetBbosSpanLenDelay();
	pstBallInfo->nSpanLenDelay = stBumpBall.nSpanLenDelay;	//v3.1.T410 add

	dMoveDist = pstBallInfo->nSpanHt;
    double dT1, dT2, dT3;
    dT1 = dT3 = round((double)nMoveTime * theServo.GetServoSampleRate() / 2000.0);	//v1.5.T128 edit
    dT2 = 0;
    pstBallInfo->nSpanDistZPoint = round(dT1 + dT2 + dT3);
	//v3.1.T331 add
	//short nSkewPercent = theMcConfig.GetArcSpan();	//v3.1.T410 delete
	short nSkewPercent = stBumpBall.nArcSpan;		//v3.1.T410 add
	if (nSkewPercent == 100)
		Calc_generate_OBW_profile(dMoveDist, dT1, dT2, dT3, pstBallInfo->adSpanDistZMotion);
	else
		CalcArcMotionZ(dMoveDist, dT1, pstBallInfo->adSpanDistZMotion, nSkewPercent);

    for (short i = pstBallInfo->nSpanDistZPoint - 1; i > 0; i--)
        pstBallInfo->adSpanDistZMotion[i] = pstBallInfo->adSpanDistZMotion[i] - pstBallInfo->adSpanDistZMotion[i - 1];

	//v3.1.T381 add
	const WIRE_RECORD *pstNextWire;
	if (nWireNum >= thePgm.GetTotalWire())
		pstNextWire = theWire.GetRcd(1);
	else
		pstNextWire = theWire.GetRcd(nWireNum + 1);
	pstBallInfo->chFabType = 0;
	if ((pstBallInfo->nBumpType == BUMP_SOB)
		|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL)))
	{
		if (m_nBumpBondOption == BUMPOPT_NORMAL)
			pstBallInfo->chFabType = pstWire->cWireFabType;
		else
			pstBallInfo->chFabType = pstNextWire->cBsobFabType;
	}
	else if ((pstBallInfo->nBumpType == BUMP_BOS)
			|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL2)))
	{
		if (m_nBumpBondOption == BUMPOPT_NORMAL)
			pstBallInfo->chFabType = pstNextWire->cWireFabType;
		else
			pstBallInfo->chFabType = pstNextWire->cBbosFabType;		//v3.1.T381 edit
	}
	if (bDebug) sDebugInfo.AppendFormat(_T("FabType: %d\r\n"), pstBallInfo->chFabType);

	//v3.1.T60 delete
	//short nTailLen;
	//if (pstBallInfo->chFabType == 0)
	//	nTailLen = pstPgm->nTailLengthUm;
	//else
	//	nTailLen = pstPgm->nFab2TailLengthUm;
	short nTailLen = theEfoProfile.GetProfile(pstBallInfo->chFabType)->nTailLen_x1um;	//v3.1.T60 add
    double dTailLvl = dBaseLvl2 + (double)nTailLen / theServo.GetResolZ();
    pstBallInfo->nTailHt = round(dTailLvl - dBaseLvl2);

	const PGM_RECORD *pstPgm = thePgm.GetRcd();
	if (pstPgm->bTailMonitor == true)
		pstBallInfo->nSparkDelay = 400;
	else
		pstBallInfo->nSparkDelay = pstPgm->nSparkDelay;

    pstBallInfo->nResetLvl = theMcData.GetRcd()->lResetLevel;

    if (stBumpBall.bTailBreakEnable == true)
    {
        pstBallInfo->nTailBreakDist  = pstBondPara->nTailBreakDist;
        pstBallInfo->nTailBreakSpeed = pstBondPara->nTailBreakSpeed;
    }
    else
    {
        pstBallInfo->nTailBreakDist = 0;
        pstBallInfo->nTailBreakSpeed = 0;
    }

	//v3.1.T405 edit
	if (bDebug)
	{
		sDebugInfo.AppendFormat(_T("--------------------------------------------------\r\n"));
		sDebugInfo.AppendFormat(_T("SrchLevel1: %d\r\n"), pstBallInfo->nSrchLvl1);
		//v3.1.T576 add
		sDebugInfo.AppendFormat(_T("SrchSpeed: [%d, %d]\r\n"),
							pstBallInfo->nSrchSpeed1, pstBallInfo->nSrchSpeed2);
		sDebugInfo.AppendFormat(_T("SrchTol: [%d, %d]\r\n"),
							pstBallInfo->nSrchTol1, pstBallInfo->nSrchTol2);
		sDebugInfo.AppendFormat(_T("SrchDelay: [%d, %d]\r\n"),
							pstBallInfo->nSrchDelay[0], pstBallInfo->nSrchDelay[1]);

		sDebugInfo.AppendFormat(_T("BondPos1: [%.2f, %.2f]\r\n"),
			pstBallInfo->stBondPos[0].dx, pstBallInfo->stBondPos[0].dy);
		sDebugInfo.AppendFormat(_T("BaseHt: %d\r\n"), pstBallInfo->nBaseHt);
		sDebugInfo.AppendFormat(_T("FlexPos: [%.2f, %.2f]\r\n"),
			pstBallInfo->stFlexPos.dx, pstBallInfo->stFlexPos.dy);
		sDebugInfo.AppendFormat(_T("FlexSpanHt: %d\r\n"), pstBallInfo->nFlexSpanHt);
		sDebugInfo.AppendFormat(_T("FlexHt: %d\r\n"), pstBallInfo->nFlexHt);
		sDebugInfo.AppendFormat(_T("BondPos2: [%.2f, %.2f]\r\n"),
			pstBallInfo->stBondPos[1].dx, pstBallInfo->stBondPos[1].dy);
		sDebugInfo.AppendFormat(_T("SpanHt: %d\r\n"), pstBallInfo->nSpanHt);
		sDebugInfo.AppendFormat(_T("BaseHt2: %d\r\n"), pstBallInfo->nBaseHt2);	//v3.1.T330 edit
		sDebugInfo.AppendFormat(_T("ScrubPos: [%.2f, %.2f]\r\n"),
			pstBallInfo->stScrubPos.dx, pstBallInfo->stScrubPos.dy);
		sDebugInfo.AppendFormat(_T("MotionSpeed: %d\r\n"), pstBallInfo->nMotionSpeed);
		sDebugInfo.AppendFormat(_T("TailHt: %d\r\n"), pstBallInfo->nTailHt);
		sDebugInfo.AppendFormat(_T("ResetLevel: %d\r\n"), pstBallInfo->nResetLvl);
		sDebugInfo.AppendFormat(_T("ScrubTime: %d\r\n"),  pstBallInfo->n1stScrubTime);
	}

    long lMoveDist = round(fabs(pstBallInfo->stBondPos[0].dx - stCurPos.dx) * theServo.GetResolXY());
    pstBallInfo->nFirstMotionProfileX = GetTableAutoSpeedProfile(lMoveDist);	//v1.5.T146 edit

    lMoveDist = round(fabs(pstBallInfo->stBondPos[0].dy - stCurPos.dy) * theServo.GetResolXY());
    pstBallInfo->nFirstMotionProfileY = GetTableAutoSpeedProfile(lMoveDist);	//v1.5.T146 edit

    //Search height motion command generation
    dMoveDist = pstBallInfo->nSrchLvl1 - pstBallInfo->nResetLvl;

    theServo.GetSpeedProfile(MOTOR_Z, SPEED_PROFILE_1, &stServoSpeed);
    dJerk = (double)(stServoSpeed.lJerk) / theServo.GetResolZ() / 1000.0;
    dMaxAcc = (double)(stServoSpeed.nMaxAcc) / theServo.GetResolZ();
    dMaxVel = (double)(stServoSpeed.nMaxVel) / theServo.GetResolZ();

#if _SERVO_TYPE == 2	//v3.1.T270 add
	pstBallInfo->nSrchHtTime = pstPgm->nHeadTimeOffset;
#else
    pstBallInfo->nSrchHtTime = pstPgm->nHeadTimeOffset
					+ Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
#endif

	//v3.1.T511 add
	pstBallInfo->nNextWireExist = 0;
	/* v3.1.T519 delete
	if (m_bRequestStopBond)
	{
		m_bRequestStopBond = false;
		m_bStopBondReady = true;
	}
	else
	{
	*/
		if (((pstBallInfo->nBumpType == BUMP_SOB)
			|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL)))
			&& (m_nBumpBondOption != BUMPOPT_BALL_ONLY))	//v3.1.T571 add
			pstBallInfo->nNextWireExist = 1;
		else if (m_bRequestStopBond)	//v3.1.T519 add
		{
			m_bRequestStopBond = false;
			m_bStopBondReady = true;
		}
		else if ((pstBallInfo->nBumpType == BUMP_BOS)
				|| (pstBallInfo->nBumpType == BUMP_SOB)	//v3.1.T573 add
				|| ((pstBallInfo->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL2)))
		{
			if (nWireNum >= thePgm.GetTotalWire())
				pstBallInfo->nNextWireExist = 0;
			else
			{
				short nNextWireNum = nWireNum + 1;
				pstBallInfo->nNextWireExist = CheckAheadAlignDone(&nNextWireNum);
				if (pstBallInfo->nNextWireExist == 2)
					pstBallInfo->nNextWireExist = 0;
				else
				{
					short nUnitTotalWire = thePgm.GetUnitTotalWire();
					if (((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
						&& (nUnitTotalWire == 1))
						pstBallInfo->nNextWireExist = 1;
					else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
					{
						if ((nUnitTotalWire == 1) && (pstPgm->nPrAheadCol == 1))
							pstBallInfo->nNextWireExist = 0;
						else
							pstBallInfo->nNextWireExist = 1;
					}
				}
			}
		}
	//}

    return BOND_SUCCESS;
}

BYTE CBond::CheckPerIndexSkipNumLimit()		//V1.1.W286 edit
{
	if (thePgm.GetSkipNumPerIndex() == 0) return TRUE;
	if (m_nSkipNumPerIndex <= thePgm.GetSkipNumPerIndex()) return TRUE;

	short nMatNum = 0;
	nMatNum = thePgm.GetMatNum(m_nCurMatRow, m_nCurMatCol);
	if (thePgm.GetNextMatrix(theMHS.GetCurBondUnit(), nMatNum, &nMatNum) == FOK)
		return TRUE;

	theTowerLight.EnableErrorLight();
	theTowerLight.EnableBlinkRedLight(true);

	m_nSkipNumPerIndex=0;
	short nMsg = MSG_SKIP_PER_INDEX_EXCEED_LIMIT;

	nMsg=theMsg.ShowMsg(ERROR_MSG, 1, &nMsg, true, false)!=SHOW_MSG_RESULT_STOP;

	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableBondingLight();

	return (BYTE)nMsg;
}

// Perform auto bond sequence
void CBond::PerformAutoBond(EN_BOND_MODE nBondMode)
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		theTopOnloader.SetFirstIndex(true, true);
		break;
	case MHS_CU_ELITE:
		theCuEliteOnloader.SetFirstIndex(true, true);
		break;
	}

	try
	{
	m_nBondMode = nBondMode;

	theVisionSys.ClearDrawing();
	short nStatus = InitAutoBond();

	m_bRequestStopBond = false;
	m_bStopBondReady = false;	//v3.1.T461 add

	//fire efo before bonding
	PreFireEFO();

	//v1.6.T166 add
	if (theMcConfig.GetAcsBufferType() == 1)
	{
		theServo.StopBuffer(7);
		theServo.StopBuffer(9);
		Sleep_x1ms(500);
		theServo.RunBuffer(7);
		theServo.RunBuffer(9);
	}
	//v1.6.T360 add
	else if (theMcConfig.GetAcsBufferType() == 2)
	{
		theServo.StopBuffer(9);
		Sleep_x1ms(500);
		theServo.RunBuffer(9);
	}

	if (nBondMode == BONDMODE_AUTO)
		m_PanelLight.SetRelayState(FALSE);
	m_AirTensioner.SetRelayState(TRUE);	//v3.1.T439 add

	CHTimer timer;
	while (nStatus != QUIT_AUTO_BOND)
	{
		/* v3.1.T461 delete
		if (m_bRequestStopBond == true)
		{  
			m_bRequestStopBond = false;
			break; 
		}
		*/

		switch (nStatus)
		{
		case START_AUTO_BOND:
		{
			if (theMcConfig.IsTimeStampCycle())
			{
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("beg_StartAutoBond..."));
				timer.StartTimer();
			}
			nStatus = StartAutoBond();
			if (theMcConfig.IsTimeStampCycle())
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("end_StartAutoBond"), timer.GetTime_x1ms());
			if (theDebug.IsDebugIO()) Sleep(1);
			break;
		}
		case PERFORM_VISION_ALIGN:
		{
			if (theMcConfig.IsTimeStampCycle())
			{
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("beg_PR..."));
				timer.StartTimer();
			}
			theApp.SetRedLamp(true);

			//v3.0.T123 add
			if ((theMcConfig.GetHidePrResultTime() > 0)
				&& theVisionSys.IsShowResult()
				&& (m_ShowPrTimer.GetTime_x1s() > theMcConfig.GetHidePrResultTime()))
					theVisionSys.SetShowResult(false);

			if (thePgm.IsMultiPr())
				nStatus = PerformMultiPrAlignment(false);
			else
				nStatus = PerformVisionAlignment(false);
			theApp.SetRedLamp(false);
			if (theMcConfig.IsTimeStampCycle())
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("end_PR"), timer.GetTime_x1ms());
			break;
		}
		case PERFORM_WIRE_BOND:
		{
			if (theMcConfig.IsTimeStampCycle())
			{
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("beg_WireBond..."));
				timer.StartTimer();
			}
			nStatus = PerformWireBond();
			if (theMcConfig.IsTimeStampCycle())
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("end_WireBond"), timer.GetTime_x1ms());
			break;
		}
		case COMPLETE_UNIT_UPDATE:
		{
			if (theMcConfig.IsTimeStampCycle())
			{
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("beg_UnitUpdate..."));
				timer.StartTimer();
			}
			nStatus = CompleteUnitUpdate();
			if (theMcConfig.IsTimeStampCycle())
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("end_UnitUpdate"), timer.GetTime_x1ms());
			if (theDebug.IsDebugIO()) Sleep(1);
			break;
		}
		case AUTO_BOND_INFORM_INDEX:
		{
			if (theMcConfig.IsTimeStampCycle())
			{
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("beg_Index..."));
				timer.StartTimer();
			}
			nStatus = CheckPerIndexSkipNumLimit() ? AutoBondInformIndex() : QUIT_AUTO_BOND;	//V1.1.W262 edit
			if (theMcConfig.IsTimeStampCycle())
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("end_Index"), timer.GetTime_x1ms());
			if (theDebug.IsDebugIO()) Sleep(1);
			break;
		}
		case AUTO_BOND_AFTER_INDEX:
		{
			theApp.SetRedLamp(true);
			if (theMcConfig.IsTimeStampCycle())
			{
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("beg_AfterIndex..."));
				timer.StartTimer();
			}
			nStatus = AutoBondAfterIndex();
			if (theMcConfig.IsTimeStampCycle())
				ShowDebugMsg(TIME_STAMP_CYCLE, _T("end_AfterIndex..."), timer.GetTime_x1ms());
			theApp.SetRedLamp(false);
			break;
		}
		default:
			break;
		}//switch
	}//while

	QuitAutoBond();
	if (nBondMode == BONDMODE_AUTO)
		m_PanelLight.SetRelayState(TRUE);
	}
	catch(const char* e)
	{
		::AfxMessageBox((CString)e,MB_OK);
	}
	catch(std::logic_error e)
	{
		::AfxMessageBox(_T(" / error "),MB_OK);
	}
	catch (...)
	{
		AfxMessageBox(_T("where are you "),MB_OK);
	}
}

int CBond::InitAutoBond(void)
{
    bool bIndexComplete = true;

    theServo.StartServoTimerDebug(theDebug.GetDebugMode());

	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableBondingLight();

	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_R2R:
	{
		TOP_DEVICE_RECORD stDeviceRcd;
		theDevice.GetTopRcd(&stDeviceRcd);

		if (m_bAutoIndex)
		{
			theMHS.SetAutoIndex(true);

			if (theMHS.GetCurBondUnit() == 0)
			{
				theServo.MoveBondHeadToSafePos();
				Sleep(10);

				theR2RTrack.ResetTrackErrorCode(1);
				theR2RTrack.ResetTrackErrorCode(0);		               
				//theMHS.SetRequestIndex(true);
				//Sleep(200);

				//bIndexComplete = WaitIndexComplete();

				//if (bIndexComplete == false)
				//	return QUIT_AUTO_BOND;
			
                if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
                {
                    short nStatus, nNextMatrix;
                    nStatus = thePgm.GetNextMatrix(1, 0, &nNextMatrix);
                    if (nStatus == FOK)
                    {
                        thePgm.GetMatRowCol(nNextMatrix, &m_nCurMatRow, &m_nCurMatCol);
                    }
                }
            }
            else
			{
				if (theMHS.IsLastLf() == false)
				{
					if (theR2RTrack.GetTopTrackEvent() == INDEX_EVENT_IDLE)
					    theR2RTrack.SubmitIndexEvent(INDEX_EVENT_STANDBY_NEXT_LF);
				}
				else
				{
					do 
					{
						Sleep_x1ms(10);
					} while (theStepper.IsBusy(theR2RTrack.m_ucIndexerStepperId));
				}
			}
		}
		break;
	}
	case MHS_TOP_LED:
	{
		TOP_OFFLOAD_RCD stOffloadData;
		theTopOffloader.GetRcd(&stOffloadData);

		if (theTopTrack.IsTrackNeedClear() && m_bAutoIndex)
		{
			if (theMHS.ClearTrack() != FOK)
				return QUIT_AUTO_BOND;
		}

		if (theTopOffloader.IsJamNotHandled())
		{
			CDlgElevCtrl objElev;
			if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)) return QUIT_AUTO_BOND;

			TOP_TRACK_DATA stTrackData;
			theTopTrack.GetRcd(&stTrackData);
			if (theTopOffloader.GetCurSlot() < stOffloadData.lOffloadSlotNum)
			{
				theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot() + 1
											+ stTrackData.iOutputMagazineSlotSequence);
				theTopOffloader.WaitMoveMagHolder();
			}
			else
				theTopOffloader.EjectOffloadMag();
		}

		if (m_bAutoIndex)
		{
			theMHS.SetAutoIndex(true);

			if (theMHS.GetCurBondUnit() == 0)
			{
				theServo.MoveBondHeadToSafePos();
				Sleep_x1ms(10);

				theTopTrack.ResetErrorCode(1);
				theTopTrack.ResetErrorCode(0);
			
				//pusher
				int iPusherStatus;
				iPusherStatus = theTopOnloader.m_InjectPusher.TriggerForwardMotion();
				iPusherStatus += theTopOnloader.m_InjectPusher.TriggerBacktoHome();
				if (iPusherStatus != FOK)
				{
					short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
					theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
					return QUIT_AUTO_BOND;
				}

				theTopOnloader.ResetAfterInit();
				theTopTrack.RequestOnload();

				theMHS.SetRequestIndex(true);
				Sleep_x1ms(200);

				bIndexComplete = WaitIndexComplete();
				if (bIndexComplete == false)
					return QUIT_AUTO_BOND;
			
                if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
                {
                    short nStatus, nNextMatrix;
                    nStatus = thePgm.GetNextMatrix(1, 0, &nNextMatrix);
                    if (nStatus == FOK)
                        thePgm.GetMatRowCol(nNextMatrix, &m_nCurMatRow, &m_nCurMatCol);
                }
            }
            else
			{
				if (theMHS.IsLastLf() == false)
				{
					if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
					    theTopTrack.SubmitIndexEvent(INDEX_EVENT_STANDBY_NEXT_LF);
				}
				else
				{
					do 
					{
						Sleep_x1ms(10);
					} while (theStepper.IsBusy(theTopTrack.GetIndexStepID()));
				}
			}
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		if (theTEIndex.IsTrackNeedClear() && m_bAutoIndex)
		{
			if (theMHS.ClearTrack() != FOK)
				return QUIT_AUTO_BOND;
		}

		TOP_OFFLOAD_RCD stOffloadData;
		theTopOffloader.GetRcd(&stOffloadData);
		if (theTopOffloader.IsJamNotHandled())
		{
			CDlgElevCtrl objElev;
			if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)) return QUIT_AUTO_BOND;

			if (theTopOffloader.GetCurSlot() < stOffloadData.lOffloadSlotNum)
			{
				theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot() + 1
										+ theTEIndex.GetRcd()->iOutputMagazineSlotSequence);
				theTopOffloader.WaitMoveMagHolder();
			}
			else
				theTopOffloader.EjectOffloadMag();
		}

		if (m_bAutoIndex)
		{
			theMHS.SetAutoIndex(true);

			if (theMHS.GetCurBondUnit() == 0)
			{
				theServo.MoveBondHeadToSafePos();
				Sleep_x1ms(10);

				theTEIndex.ResetErrorCode(1);
				theTEIndex.ResetErrorCode(0);
			
				//pusher
				int iPusherStatus;
				iPusherStatus = theTopOnloader.m_InjectPusher.TriggerForwardMotion();
				iPusherStatus += theTopOnloader.m_InjectPusher.TriggerBacktoHome();
				if (iPusherStatus != FOK)
				{
					short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
					theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
					return QUIT_AUTO_BOND;
				}

				theTopOnloader.ResetAfterInit();
				theTEIndex.RequestOnload();

				theMHS.SetRequestIndex(true);
				Sleep_x1ms(200);

				bIndexComplete = WaitIndexComplete();
				if (bIndexComplete == false)
					return QUIT_AUTO_BOND;
			
                if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
                {
                    short nStatus, nNextMatrix;
                    nStatus = thePgm.GetNextMatrix(1, 0, &nNextMatrix);
                    if (nStatus == FOK)
                        thePgm.GetMatRowCol(nNextMatrix, &m_nCurMatRow, &m_nCurMatCol);
                }
            }
            else
			{
				if (theMHS.IsLastLf() == false)
				{
					if (theTEIndex.GetEvent() == INDEX_EVENT_IDLE)
					    theTEIndex.SubmitIndexEvent(INDEX_EVENT_STANDBY_NEXT_LF);
				}
				else
				{
					do 
					{
						Sleep_x1ms(10);
					} while (theStepper.IsBusy(theTEIndex.GetIndexStepID()));
				}
			}
		}
		break;
	}
	case MHS_FOXCONN:
	{
		if (m_bAutoIndex)
		{
			theMHS.SetAutoIndex(true);

			if (theMHS.GetCurBondUnit() == 0)
			{
				theServo.MoveBondHeadToSafePos();
				Sleep_x1ms(10);

				theTopTrack.ResetErrorCode(1);
				theTopTrack.ResetErrorCode(0);
			
				theMHS.SetRequestIndex(true);
				Sleep_x1ms(200);

				bIndexComplete = WaitIndexComplete();
				if (bIndexComplete == false)
					return QUIT_AUTO_BOND;
			
                if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
                {
                    short nStatus, nNextMatrix;
                    nStatus = thePgm.GetNextMatrix(1, 0, &nNextMatrix);
                    if (nStatus == FOK)
                        thePgm.GetMatRowCol(nNextMatrix, &m_nCurMatRow, &m_nCurMatCol);
                }
            }
   //         else
			//{
			//	if (theMHS.IsLastLf() == false)
			//	{
			//		if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
			//		    theTopTrack.SubmitIndexEvent(INDEX_EVENT_STANDBY_NEXT_LF);
			//	}
			//	else
			//	{
			//		do 
			//		{
			//			Sleep_x1ms(10);
			//		} while (theStepper.IsBusy(theTopTrack.GetIndexStepID()));
			//	}
			//}
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		CUELITE_DEVICE_RECORD stCuEliteDevicePara;
	    theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

		//track is not cleared
		if ((theCuEliteTrack.GetTrackNeedClearFlag() == TRUE) && m_bAutoIndex)
		{
			if (theMHS.ClearTrack() != FOK)
				return QUIT_AUTO_BOND;
		}
		 //output jam
		if (theCuEliteOffloader.GetOffloadJamHandleFlag() == TRUE)
		{
			theCuEliteTrack.theEjectSnr.UpdateSensorState();
			if (theCuEliteTrack.theEjectSnr.GetSensorState() == FALSE)
			{
				CUELITE_TRACK_DATA stTrackData;
				theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);
				if (theCuEliteOffloader.GetCurrentSlotNum() < stCuEliteDevicePara.lMagSlotNum)
				{
					theCuEliteOffloader.MoveTotheSlot(theCuEliteOffloader.GetCurrentSlotNum() + 1 + stTrackData.iOutputMagazineSlotSequence); //v3.3.D179 , false
					theCuEliteOffloader.WaitMagHoledrMoveFinish();
				}
				else
					theCuEliteOffloader.EjectOffloadMag();
			}
			else
			{
				short nMsgNum, anMsgArray[10];
				nMsgNum = 1;
				anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;
				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
				return QUIT_AUTO_BOND;
			}
		}

		//start ....
		if (m_bAutoIndex)
		{
			theMHS.SetAutoIndex(true);
            //track is empty, init leadframe
			if (theMHS.GetCurBondUnit() == 0)
			{
				theServo.MoveBondHeadToSafePos();
				Sleep(10);

				theCuEliteTrack.ResetTrackErrorCode(1);
				theCuEliteTrack.ResetTrackErrorCode(0);

				//pusher
				int iPusherStatus;
				iPusherStatus = theCuEliteOnloader.theInjectPusher.TriggerForwardMotion();
				iPusherStatus += theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
				if (iPusherStatus != FOK)
				{
					short nMsgNum, anMsgArray[10];
					nMsgNum = 1;
					anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
					theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
					return QUIT_AUTO_BOND;
				}

				//new LF follows if sensor is off
				//theCuEliteTrack.theInjectSnr.UpdateSensorState();
				//if(theCuEliteTrack.theInjectSnr.GetSensorState() == FALSE)
				{
					//Sleep(100);
					theCuEliteOnloader.ResetOnloadInitStatus();
					theCuEliteTrack.RequestOnloadEvent();
				}
				               
				theMHS.SetRequestIndex(true);
				Sleep(200);

				bIndexComplete = WaitIndexComplete();
				if (bIndexComplete == false)
					return QUIT_AUTO_BOND;
			
                if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
                {
                    short nStatus, nNextMatrix;
                    nStatus = thePgm.GetNextMatrix(1, 0, &nNextMatrix);
                    if (nStatus == FOK)
                        thePgm.GetMatRowCol(nNextMatrix, &m_nCurMatRow, &m_nCurMatCol);
                }
            }
            else	//track is not empty 
			{
				if (theCuEliteTrack.InjectEnabled() == TRUE)
				{
					theCuEliteOnloader.theInjectPusher.TriggerForwardMotion();
					theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
				}

				if (theMHS.IsLastLf() == false)	//not last leadframe
				{   
					if (theCuEliteTrack.GetCuEliteTrackEvent() == INDEX_EVENT_IDLE)
					    theCuEliteTrack.SubmitIndexEvent(INDEX_EVENT_STANDBY_NEXT_LF);
				}
				else	//last leadframe
				{
					do 
					{
						Sleep_x1ms(10);
					} while (theStepper.IsBusy(CUELITE_DEVICE_INDEXER));
				}
			}
		}
		break;
	}
	case MHS_VLED_ELITE:
	{
		if (m_bPrOn1stEnterDone == false)
		{
			if ((theMHS.GetCurBondUnit() != 0) && m_bAutoIndex)
				theMHS.ClearTrack();

			m_bPrOn1stEnterDone = true;
		}

       	if (m_bAutoIndex)
		{
			theMHS.SetAutoIndex(true);

			if (theMHS.GetCurBondUnit() == 0)
			{
				theVEliteTrack.theLeftClaw.SetIndexEmpty(true);
				theMHS.SetRequestIndex(true);
				Sleep(200);

				bIndexComplete = WaitIndexComplete();

				if (bIndexComplete == false)
					return QUIT_AUTO_BOND;
			}
		}
		break;
	}
	case MHS_VLED:
	{
		if (m_bPrOn1stEnterDone == false)
		{
			if ((theMHS.GetCurBondUnit() != 0) && m_bAutoIndex)
				theMHS.ClearTrack();

			m_bPrOn1stEnterDone = true;
		}

       	if (m_bAutoIndex)
		{
			//theVledTrack.ResetIndexEvent();
			theMHS.SetAutoIndex(true);

			if (theMHS.GetCurBondUnit() == 0)
			{
				theVledTrack.theLeftClaw.SetIndexEmpty(true);
				theMHS.SetRequestIndex(true);
				Sleep(200);

				bIndexComplete = WaitIndexComplete();
				if (bIndexComplete == false)
					return QUIT_AUTO_BOND;
			}
		}
		break;
	}
	}
   
    if (m_bBondNoVision == false)
        theVisionSys.EnableVideo(false);

    theScanKeyThread.DisableJoyStick();
    CheckAheadAlignDone(&m_nCurWire);
    theServo.SetServoControlParameter(false);

    if ((m_nCurWire == 1) && (m_nBondMode != BONDMODE_CORRECT))
    {
        REF_RECORD stRefRcd;
        TBL_POS stStepRepeatOffset;

        if (thePgm.GetDeviceSeq() == PGM_MATRIX)
            thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, m_nCurRow, m_nCurCol, &stStepRepeatOffset);
        else
            thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

		if (thePgm.IsLeadPrEnable())
            theRef.GetRcd(&stRefRcd, 0);
        else
            theRef.GetRcd(&stRefRcd, 1);

        theVisionSys.UpdateLightingParameter(stRefRcd.stVisionLightPara[0].nCoaxRedLight, stRefRcd.stVisionLightPara[0].bSideRedLight, stRefRcd.stVisionLightPara[0].nCoaxBlueLight, stRefRcd.stVisionLightPara[0].bSideBlueLight);
        theVisionSys.ChangeLensMag(stRefRcd.nVisionAlignMag[0]);

        stRefRcd.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
        stRefRcd.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
        m_stCurTblPos = stRefRcd.stVisionAlignPos[0];

		theServo.MoveTable2(stRefRcd.stVisionAlignPos[0]);	//v3.1.T52 edit
    }
	else if ((m_nCurWire == thePgm.GetTotalWire())
				&& (m_nCurWire <= m_nBondedWire))
        ;
    else
        MoveToWirePoint(m_nCurWire, FIRST_BOND);

    theServo.GetTablePosition(&m_stCurTblPos);

	if (theDebug.IsDebugBondTiming() || theDebug.IsDebugVisionTiming())
    {
        theDebug.ClearInfo();	//v3.0.T96 edit
    }

	//set usg profile
	short nBumpSeq;
	WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);
	if (((pstWire->nBumpType == BUMP_SOB)	//v3.1.T90 edit
			|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T94 add
		&& (m_nBumpBondOption != BUMPOPT_WIRE_ONLY))
		nBumpSeq = BUMPSEQ_BALL;
	else
		nBumpSeq = BUMPSEQ_WIRE;
	m_nBondBuffer = 0;
	CalcUsgProfile(m_nCurWire, nBumpSeq);
	SetUsgProfile(m_nCurWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendUsgProf1(1)_Buf"), m_nBondBuffer);
	Sleep(10);
	SetUsgProfile(m_nCurWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendUsgProf2"));
	
    if (theDebug.IsDebugContactLevel()) theDebug.ClearInfo();	//v3.0.T96 edit

	if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
		|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
		|| (theMcConfig.GetMhsType() == MHS_R2R)
		|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
	{
		if ((m_nBondMode != BONDMODE_CORRECT)
			&& (m_IdleTimer.GetTime_x1s() > 60))
			ResetMatrixPrStatus(m_nCurWire);
	}

    //long lCtactLmt = theRef.GetMinCtactLevel();
    //lCtactLmt -= 500;

    theBSD.ClearWireFeedCount();	//v1.6.T159 edit
    m_nWaveformCaptureCount = 0;

	int nIdleTime = round(m_IdleTimer.GetTime_x1s());	//v3.0.T115 add

	//v1.1.288.T16 add - accumulate m_nIdleTime_x1s
	int nSetIdleTime = theMcData.GetTransducerIdleTime() * 60;
	if (nIdleTime > nSetIdleTime)
		m_nIdleTime_x1s = nSetIdleTime;
	else
	{
		m_nIdleTime_x1s += nIdleTime;
		if (m_nIdleTime_x1s > nSetIdleTime)
			m_nIdleTime_x1s = nSetIdleTime;
	}

	m_BondTimer.StartTimer();
	m_dLastBondTime = 0;		//v1.5.T54 add

    return START_AUTO_BOND;
}

// Quit auto bond
int CBond::QuitAutoBond(void)
{
	CHTimer timer;
	timer.StartTimer();

	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED: case MHS_R2R:
	{
		while ((timer.GetTime_x1s() < 3)
				&& ((theTopTrack.GetEvent() != INDEX_EVENT_IDLE)
					|| (theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
					|| (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)))
		{
			Sleep_x1ms(10);
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		while ((timer.GetTime_x1s() < 3)
				&& ((theTEIndex.GetEvent() != INDEX_EVENT_IDLE)
					|| (theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
					|| (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)))
		{
			Sleep_x1ms(10);
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		while ((timer.GetTime_x1s() < 3)
				&& ((theCuEliteTrack.GetCuEliteTrackEvent() != INDEX_EVENT_IDLE)
					|| (theCuEliteOnloader.GetOnloadEvent() != TOP_ONLOAD_EVENT_IDLE)
					|| (theCuEliteOffloader.GetOffloadEvent() != TOP_OFFLOAD_EVENT_IDLE)))
		{
			Sleep_x1ms(10);
		}
		break;
	}
	}

    theMHS.SetAutoIndex(false);

    theStat.SetMachineState(MCSTATE_STANDBY);
	m_IdleTimer.StartTimer();

	int nBondTime = round(m_BondTimer.GetTime_x1s());	//v3.0.T115 add

	//v1.1.288.T16 add - deduct m_nIdleTime_x1s
	if (nBondTime > theMcData.GetTransducerIdleTime() * 60)
		m_nIdleTime_x1s = 0;
	else
	{
		m_nIdleTime_x1s -= nBondTime;
		if (m_nIdleTime_x1s < 0) m_nIdleTime_x1s = 0;
	}

	theServo.StopServoTimerDebug(theDebug.GetDebugMode());
    theServo.TriggerWireClampProfile(WCL_HOLD_WIRE_CLOSE);
    MoveBondHeadToFireLevel();
    
    theServo.SetServoControlParameter();
    theServo.SetServoSpeedProfile(MOTOR_Z, 0);

	theTowerLight.EnableBlinkRedLight(false);
    theTowerLight.EnableIdleLight();

    //theScanKeyThread.BeginThread();

    short nUnitTotalWire, nUnitNum;

    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
    {
		if (thePgm.GetTotalUnit() == 0)
		{
			AfxMessageBox(_T("QuitAutoBond nTotalUnit = 0"));
			return 0;
		}
        nUnitTotalWire = thePgm.GetTotalWire() / thePgm.GetTotalUnit();
        nUnitNum = (m_nCurWire - 1) / nUnitTotalWire + 1;
        thePgm.GetRowCol(nUnitNum, &m_nCurRow, &m_nCurCol);
    }

    theVisionSys.EnableVideo(true);
	m_nBondMode = BONDMODE_IDLE; //BOND_MODE_STOP;

	if (!theMcConfig.IsBypassMHS()			//v3.1.T172 add
		&& !theMcConfig.IsBypassServo())	//v3.1.T132 edit
		theScanKeyThread.EnableJoyStick();

	theVisionDisplay.EnableDisplay(true);
    theVisionDisplay.SetDisplayWire(m_nCurWire);
    theVisionDisplay.SetDisplayBondPt(FIRST_BOND);
    theVisionDisplay.UpdateVisionDisplay(true);
    Sleep(50);

    return 0;
}

// Start auto bond
int CBond::StartAutoBond(void)
{
    short anMsgArray[10];
    long lStopSelected;

    m_nBondBuffer = 0;
    m_nStickDetectBlock = 0;

    //theStat.RegisterUnitStartTime();

	if ((m_bDryRun == false) && (!theMcConfig.IsBypassAll()))
	{
		//check heater
        if ((theScanKeyThread.m_HeaterDetector.GetSensorState() == SENSOR_ON)
			&& (m_iHeaterWarnSkip != SHOW_MSG_RESULT_SKIP)
			&& (theMcConfig.GetMhsType() != MHS_FOXCONN))	//v3.0.T210 add
        {
            theServo.SetServoControlParameter();

            anMsgArray[0] = MSG_HEATER_TEMP_OUT_RANGE;
            m_iHeaterWarnSkip = theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray, false, true);

            return QUIT_AUTO_BOND;
        }

        //check vacuum
		switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
		{
		case MHS_CU_ELITE:
        {
			if (theMcConfig.IsCheckVacuum())	//v3.0.T338 add
			{
				if (theMcConfig.GetCheckVacDelay() > 0)	//v3.0.T402 add
					Sleep_x1ms(theMcConfig.GetCheckVacDelay());
				if (theCuEliteLFClamp.CheckVacuumStatus() == FNOK)
				{
					theServo.SetServoControlParameter();
					anMsgArray[0] = MSG_VACUUM_PRESSURE_LOW;
					theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
					theCuEliteLFClamp.m_VacuumValve.SetRelayState(RELAY_OFF);	//v3.0.T341 add
					return QUIT_AUTO_BOND;
				}
			}
			break;
        }
		case MHS_TOP_LED: case MHS_R2R:
		case MHS_TOP_ELITE:	//v3.0.T410 add
        {
			if (theMcConfig.IsCheckVacuum())	//v3.0.T338 add
			{
				if (theMcConfig.GetCheckVacDelay() > 0)	//v3.0.T402 add
					Sleep_x1ms(theMcConfig.GetCheckVacDelay());
				if (theTopClamp.CheckVacuumStatus() == FNOK)
				{
					theServo.SetServoControlParameter();
					anMsgArray[0] = MSG_VACUUM_PRESSURE_LOW;
					theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
					theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
					return QUIT_AUTO_BOND;
				}
			}
			break;
        }
		}

		//check capillary
        if (theStat.IsCapillaryWearOut() == true)
        {
            theTowerLight.EnableWarningLight(true);
            //theTowerLight.EnableErrorLight();

            theServo.SetServoControlParameter();

            anMsgArray[0] = MSG_CAP_WEAR_OUT;
            anMsgArray[1] = MSG_CHANGE_CAP;
            theMsg.ShowMsg(WARNING_MSG, 2, anMsgArray);

            theTowerLight.EnableWarningLight(false);

            return QUIT_AUTO_BOND;
        }
    }

    if (m_bAutoIndex)
    {
        if (theMHS.ShowError() != FOK)
			return QUIT_AUTO_BOND;
    }

    if (m_nBondMode == BONDMODE_AUTO)
    {
        m_nBondType = BONDTYPE_RESTART;
        m_nCurWire = m_nRestartWire;
        if ((m_nCurWire == thePgm.GetTotalWire())
			&& (m_nCurWire <= m_nBondedWire))
            return AUTO_BOND_INFORM_INDEX;
    }
    else
    {
        m_nBondType = BONDTYPE_SINGLE;
        if (m_nCurWire <= m_nBondedWire)
        {
            if (m_nBondMode != BONDMODE_CORRECT)
            {
                theServo.SetServoControlParameter();

                anMsgArray[0] = MSG_WIRE_BONDED;
                anMsgArray[1] = MSG_CONFIRM_TO_CONTINUE;

                if (m_nCurWire == thePgm.GetTotalWire())
                {
                    lStopSelected = theMsg.ShowMsg(WARNING_MSG, 2, anMsgArray, true, true);
                    if (lStopSelected == SHOW_MSG_RESULT_STOP)
                        return QUIT_AUTO_BOND;
                    else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
                    {
                        theServo.SetServoControlParameter(false);
                        return AUTO_BOND_INFORM_INDEX;
                    }
                }
                else
                {
                    lStopSelected = theMsg.ShowMsg(WARNING_MSG, 2, anMsgArray, true);
                    if (lStopSelected == SHOW_MSG_RESULT_STOP)
                        return QUIT_AUTO_BOND;
                }
            }
        }
    }

    if (thePgm.GetDeviceSeq() == PGM_MATRIX)
    {
        if ((m_nCurRow == 1) && (m_nCurCol == 1))
            theStat.SetUnitStartTime(1);
    }
    else
    {
		short nUnitTotalWire, nUnitNum;

        nUnitTotalWire = thePgm.GetUnitTotalWire();
	    if( nUnitTotalWire == 0)
		{
			AfxMessageBox(_T("temp.2, UnitTotalWire == 0"));
			return 0;
		}
        nUnitNum = (m_nCurWire - 1) / nUnitTotalWire + 1;
        theStat.SetUnitStartTime(nUnitNum);
    }

    theStat.SetMachineState(MCSTATE_PRODUCTION);

	if ((m_nCurWire == 1) && (m_nBondMode != BONDMODE_CORRECT))
	{
		ResetMatrixPrStatus(m_nCurWire);
	}

	return m_nBondMode == BONDMODE_CORRECT ? PERFORM_WIRE_BOND : PERFORM_VISION_ALIGN;
}

// Perform manual alignment when vision alignment fail
int CBond::PerformManualAlignment(short nRefNum)
{
    int lStopSelected;
    CString str;
    CStringList sList;
    REF_RECORD stRefRcd;
    MATRIX_RECORD stMatrix;
    TBL_POS stStepRepeatOffset, stTargetPt, stPtOffset, stMovePos;

	const PGM_RECORD *pstPgm = thePgm.GetRcd();
    theRef.GetRcd(&stRefRcd, nRefNum);
    theMatrix.GetRcd(&stMatrix, nRefNum);

    theServo.SetServoControlParameter();

    theVisionSys.EnableVideo(true);
    //theScanKeyThread.EnableJoyStick();

    if (thePgm.GetDeviceSeq() == PGM_MATRIX)
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, m_nCurRow, m_nCurCol, &stStepRepeatOffset);
    else
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

    stRefRcd.stManuAlignPos[0].dx += stStepRepeatOffset.dx;
    stRefRcd.stManuAlignPos[0].dy += stStepRepeatOffset.dy;
    stRefRcd.stManuAlignPos[1].dx += stStepRepeatOffset.dx;
    stRefRcd.stManuAlignPos[1].dy += stStepRepeatOffset.dy;

    stMovePos = stRefRcd.stManuAlignPos[0];
    if (stMatrix.nVisionStatus == VISION_SUCCESS)
        theMatrix.TransformPos(nRefNum, &stMovePos);
	theServo.MoveTable2(stMovePos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T52 edit

    theVisionSys.UpdateLightingParameter(stRefRcd.stManuAlignLightPara[0].nCoaxRedLight, 
        stRefRcd.stManuAlignLightPara[0].bSideRedLight, 
        stRefRcd.stManuAlignLightPara[0].nCoaxBlueLight, 
        stRefRcd.stManuAlignLightPara[0].bSideBlueLight);
    theVisionSys.ChangeLensMag(stRefRcd.nManuAlignMag[0]);
    theVisionSys.ShowVisionPattern(
						(short)stRefRcd.ulManuAlignPatternID[0]);

	theServo.WaitTableStop();

    //theScanKeyThread.BeginThread();

    sList.RemoveAll();
    sList.AddTail(theMsg.GetMsg(MSG_MOVE_TO_1ST_ALIGN_PT));

	short nUnitTotalDie, nTotalDieNum;

	if ((thePgm.GetDeviceSeq() == PGM_MATRIX)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)) 
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
		nTotalDieNum = theRef.GetTotalRefNum();

		if (nRefNum % nUnitTotalDie == 0)
			sList.AddTail(theMsg.GetMsg(MSG_LEAD));
		else
		{
			str.Format(_T("%s: %d"), theMsg.GetMsg(MSG_DIE),
										nRefNum % nUnitTotalDie);
			sList.AddTail(str);
		}
	}
	else
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie();

		if (nRefNum == 0)
			sList.AddTail(theMsg.GetMsg(MSG_LEAD));
		else
		{
			str.Format(_T("%s: %d"), theMsg.GetMsg(MSG_DIE),
									(nRefNum-1) % nUnitTotalDie + 1);
			sList.AddTail(str);
		}
	}

    //add speed button
    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true,false,true);
    if (lStopSelected != SHOW_MSG_RESULT_OK)
    {
        theVisionSys.ClearDrawing();
        //theScanKeyThread.DisableJoyStick();

        return lStopSelected;
    }

    theServo.GetTablePosition(&stMatrix.stVisionAlignPos[0]);

    stPtOffset.dx = stMatrix.stVisionAlignPos[0].dx - stRefRcd.stManuAlignPos[0].dx;
    stPtOffset.dy = stMatrix.stVisionAlignPos[0].dy - stRefRcd.stManuAlignPos[0].dy;

	if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
		|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
		|| (theMcConfig.GetMhsType() == MHS_R2R)
		|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
		&& (stRefRcd.nRefType == REF_TYPE_LEAD))
	{
		double rowoffset = abs(pstPgm->stUnitRowOffset.dy) > abs(pstPgm->stUnitColOffset.dy) ? pstPgm->stUnitRowOffset.dy : pstPgm->stUnitColOffset.dy;
		double coloffset = abs(pstPgm->stUnitRowOffset.dx) > abs(pstPgm->stUnitColOffset.dx) ? pstPgm->stUnitRowOffset.dx : pstPgm->stUnitColOffset.dx;
		if (abs(stPtOffset.dx) > abs(coloffset/2))
		{
			if (stRefRcd.stManuAlignPos[0].dx > stRefRcd.stManuAlignPos[1].dx)
				stPtOffset.dx += coloffset;
			else
				stPtOffset.dx -= coloffset;
		}
		if (abs(stPtOffset.dy) > abs(rowoffset/2))
		{
			if (stRefRcd.stManuAlignPos[0].dy > stRefRcd.stManuAlignPos[1].dy)
				stPtOffset.dy -= rowoffset;
			else
				stPtOffset.dy += rowoffset;
		}
		stMatrix.stVisionAlignPos[0].dx = stPtOffset.dx + stRefRcd.stManuAlignPos[0].dx;
		stMatrix.stVisionAlignPos[0].dy = stPtOffset.dy + stRefRcd.stManuAlignPos[0].dy;
	}
	
    stTargetPt.dx = stRefRcd.stManuAlignPos[1].dx + stPtOffset.dx;
    stTargetPt.dy = stRefRcd.stManuAlignPos[1].dy + stPtOffset.dy;
	theServo.MoveTable2(stTargetPt, MTN_ABS, MTN_NO_WAIT);	//v3.1.T52 edit

    //theVisionSys.SetSearchSize(stRefRcd.nManuSrchSizeX[1], stRefRcd.nManuSrchSizeY[1]);
    theVisionSys.UpdateLightingParameter(stRefRcd.stManuAlignLightPara[1].nCoaxRedLight, stRefRcd.stManuAlignLightPara[1].bSideRedLight, stRefRcd.stManuAlignLightPara[1].nCoaxBlueLight, stRefRcd.stManuAlignLightPara[1].bSideBlueLight);
    theVisionSys.ChangeLensMag(stRefRcd.nManuAlignMag[1]);
    theVisionSys.ShowVisionPattern(
						(short)stRefRcd.ulManuAlignPatternID[1]);

	theServo.WaitTableStop();

    sList.RemoveAll();
    sList.AddTail(theMsg.GetMsg(MSG_MOVE_TO_2ND_ALIGN_PT));

	if ((thePgm.GetDeviceSeq() == PGM_MATRIX)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD))
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;

		if (nRefNum % nUnitTotalDie == 0)
			sList.AddTail(theMsg.GetMsg(MSG_LEAD));
		else
		{
			str.Format(_T("%s: %d"), theMsg.GetMsg(MSG_DIE),
										nRefNum % nUnitTotalDie);
			sList.AddTail(str);
		}
	}
	else
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie();

		if (nRefNum == 0)
			sList.AddTail(theMsg.GetMsg(MSG_LEAD));
		else
		{
			str.Format(_T("%s: %d"), theMsg.GetMsg(MSG_DIE),
									(nRefNum-1) % nUnitTotalDie + 1);
			sList.AddTail(str);
		}
	}

    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true,false,true);
    if (lStopSelected != SHOW_MSG_RESULT_OK)
    {
        theVisionSys.ClearDrawing();
        //theScanKeyThread.DisableJoyStick();

        return lStopSelected;
    }

    theServo.GetTablePosition(&stMatrix.stVisionAlignPos[1]);

	if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
		|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
		|| (theMcConfig.GetMhsType() == MHS_R2R)
		|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
		&& (stRefRcd.nRefType == REF_TYPE_LEAD))
	{
		stPtOffset.dx = stMatrix.stVisionAlignPos[1].dx - stRefRcd.stManuAlignPos[1].dx;
		stPtOffset.dy = stMatrix.stVisionAlignPos[1].dy - stRefRcd.stManuAlignPos[1].dy;

		double rowoffset = abs(pstPgm->stUnitRowOffset.dy) > abs(pstPgm->stUnitColOffset.dy) ? pstPgm->stUnitRowOffset.dy : pstPgm->stUnitColOffset.dy;
		double coloffset = abs(pstPgm->stUnitRowOffset.dx) > abs(pstPgm->stUnitColOffset.dx) ? pstPgm->stUnitRowOffset.dx : pstPgm->stUnitColOffset.dx;
		if (abs(stPtOffset.dx) > abs(coloffset/2))
		{
			if (stRefRcd.stManuAlignPos[0].dx > stRefRcd.stManuAlignPos[1].dx)
				stPtOffset.dx -= coloffset;
			else
				stPtOffset.dx += coloffset;
		}
		if (abs(stPtOffset.dy) > abs(rowoffset/2))
		{
			if (stRefRcd.stManuAlignPos[0].dy > stRefRcd.stManuAlignPos[1].dy)
				stPtOffset.dy += rowoffset;
			else
				stPtOffset.dy -= rowoffset;
		}
		stMatrix.stVisionAlignPos[1].dx = stPtOffset.dx + stRefRcd.stManuAlignPos[1].dx;
		stMatrix.stVisionAlignPos[1].dy = stPtOffset.dy + stRefRcd.stManuAlignPos[1].dy;
	}

    double dRefAlignDist, dMatrixAlignDist, dAlignTol;
    dRefAlignDist = Calc2PtDist(stRefRcd.stManuAlignPos[0], stRefRcd.stManuAlignPos[1]);
    dMatrixAlignDist = Calc2PtDist(stMatrix.stVisionAlignPos[0], stMatrix.stVisionAlignPos[1]);
    
    if (stRefRcd.nRefType == REF_TYPE_LEAD)
        dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
    else
        dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

    if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
    {
        sList.RemoveAll();
        if (stRefRcd.nRefType == 0)
		    sList.AddTail(theMsg.GetMsg(MSG_LEAD_TOL_FAILURE));
        else
	        sList.AddTail(theMsg.GetMsg(MSG_DIE_TOL_FAILURE));
        lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true, false, true);
        if (lStopSelected != SHOW_MSG_RESULT_OK)
        {
            theVisionSys.ClearDrawing();
            //theScanKeyThread.DisableJoyStick();

            return lStopSelected;
        }
        else
            PerformManualAlignment(nRefNum);
    }
    else
    {
        stMatrix.nVisionStatus = VISION_SUCCESS;
        theMatrix.CalcMatrix(&stMatrix, nRefNum, MANUAL_PR_ALIGN);
    }

    theServo.SetServoControlParameter(false);

    if (m_bBondNoVision == false)
        theVisionSys.EnableVideo(false);

    theVisionSys.ClearDrawing();

    //theScanKeyThread.DisableJoyStick();

    //theScanKeyThread.StopThread();

    return FOK;
}

int CBond::PerformMultiPrAlignment(bool bSlowMove, short nVisionAction)
{
	theVisionDisplay.EnableDisplay(true);

    if (m_bBondNoVision && (nVisionAction == VISION_ACTION_AUTO))
        return PERFORM_WIRE_BOND;

	short i, k, nLastIndex, nUnitNum, nUnitTotalDie, nUnitBaseRef;
	TOP_DEVICE_RECORD *pstTopDev = theDevice.GetTopRcd();
    if (pstTopDev->nHalfIndexOn == 0)
    {
        short nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
        if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
        {
            nLastIndex = pstTopDev->nDeviceColmNum / pstTopDev->nWndUnitNum + 1;
            if (theTopTrack.GetCurBondUnit() == nLastIndex)
            {
                if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
					|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
                {
                    for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                    {
                        for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                        {
                            nUnitNum = thePgm.GetUnitNum(i, k);
                            nUnitTotalDie = thePgm.GetUnitTotalDie();

                            nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                            for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                            {
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                            }
                        }
                    }
                }
                else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
                {
                    for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                    {
                        for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                        {
                            nUnitNum = thePgm.GetUnitNum(i, k);
                            nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;

                            nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie;
                            for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                            {
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                            }
                        }
                    }
                }
            }
        }
	}

    short nStartRef = 0;
    short nLastRef = theRef.GetTotalRefNum() - 1;
    GetPerformPrRef(m_nCurWire, &nStartRef, &nLastRef);
	if (nVisionAction == VISION_ACTION_ALL_UNIT)
	{
		nStartRef = 0;
		nLastRef = theRef.GetTotalRefNum() - 1;
	}
    else if (nVisionAction == VISION_ACTION_SINGLE_UNIT)
    {
		nStartRef = 0;
        nLastRef = thePgm.GetUnitTotalDie();
    }

    for (i = nStartRef; i <= nLastRef; i++)
    {
        if (theMatrix.GetRcd(i)->nVisionStatus != VISION_SUCCESS)
        {
            nStartRef = i;
            break;
        }
    }
    if (i > nLastRef)	//all alignment done
        return PERFORM_WIRE_BOND;

    for (i = nLastRef; i >= nStartRef; i--)
    {
        if (theMatrix.GetRcd(i)->nVisionStatus == VISION_SUCCESS)
            nLastRef -= 1;
        else
            break;
    }
    if (nLastRef < nStartRef)
        return PERFORM_WIRE_BOND;

	TBL_POS stCurPos;
    theServo.GetTablePosition(&stCurPos);

    TBL_POS stStepRepeatOffset;
    if (thePgm.GetDeviceSeq() == PGM_MATRIX)
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, m_nCurRow, m_nCurCol, &stStepRepeatOffset);
    else
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

	short nInc = 1;
	int iMultiStart = nStartRef;
	int iMultiEnd = nStartRef;
	int iDieNum = thePgm.GetUnitTotalDie();
	short nSrchRange, nPrPassScore;
	int lVisionError, nMsgAction;
	double dMoveDist = 0.0;
	CStringList sList;
	TBL_POS stVisionOffset, stPrPos, stNextPrPos;
	PR_SRCH_RESULT stSrchResult;
	short nLastProfileX = 0, nLastProfileY = 0, nMoveProfile;
	long lMoveDist;
	WIRE_RECORD *pstWire;
	const PGM_RECORD *pstPgm = thePgm.GetRcd();
	REF_RECORD stCurRef, stNextRef;

	for (short j = nStartRef; j <= nLastRef; j++)
	{
		if ((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
		{
			if (j == 0)  //lead PR
			{
				//for the high power 3rd index
				if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (pstTopDev->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDev->nHalfIndexOn))
				{
					pstWire = theWire.GetRcd(m_nCurWire);
					if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
					{
						if ((pstWire->nRefNum[0] == 0) || (pstWire->nRefNum[1] == 0))
							nInc = max(pstWire->nRefNum[0], pstWire->nRefNum[1]);
						else
							nInc = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);
					}

					i = 0;
					j = nInc - 1;
					iMultiEnd = i;
				}
                else
    			{
    				i = 0;
    				nInc = nLastRef;
					iMultiStart = nLastRef;
					iMultiEnd = nLastRef;
    			}
			}
			else  //after lead PR
			{
				if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
						&& (pstTopDev->nHalfIndexOn > 0)
						&& (theTopTrack.GetCurBondUnit() == pstTopDev->nHalfIndexOn))
				{
					i = j;
					nInc = 1;

					if (i >= iMultiEnd && i < nLastRef)
					{
						iMultiStart = iMultiEnd;
						iMultiEnd = iMultiStart + nInc * iDieNum;
						if (iMultiEnd > nLastRef)
							iMultiEnd = nLastRef;
						stPrPos = theRef.GetMultiPrPos(iMultiStart, iMultiEnd);
						stNextPrPos = theRef.GetMultiPrPos(iMultiStart + nInc * iDieNum, iMultiEnd + nInc * iDieNum);
						if (iMultiEnd == nLastRef)
							stNextPrPos = stPrPos;
						stPrPos.dx += stStepRepeatOffset.dx;
						stPrPos.dy += stStepRepeatOffset.dy;
						stNextPrPos.dx += stStepRepeatOffset.dx;
						stNextPrPos.dy += stStepRepeatOffset.dy;
					}
				}
    			else
    			{
					if (nStartRef == 0)
    				{
    				   i = nLastRef - j + 1;
    				   nInc = -1;

					   if (i <= iMultiEnd && i > nStartRef)
					   {
						   iMultiStart = iMultiEnd;
						   iMultiEnd = iMultiStart + nInc * iDieNum;
						   if (iMultiEnd < 0)
							   iMultiEnd = iMultiStart;
						   stPrPos = theRef.GetMultiPrPos(iMultiStart, iMultiEnd);
						   stNextPrPos = theRef.GetMultiPrPos(iMultiStart + nInc * iDieNum, iMultiEnd + nInc * iDieNum);
						   if (iMultiEnd == 0)
							   stNextPrPos = stPrPos;
						   stPrPos.dx += stStepRepeatOffset.dx;
						   stPrPos.dy += stStepRepeatOffset.dy;
						   stNextPrPos.dx += stStepRepeatOffset.dx;
						   stNextPrPos.dy += stStepRepeatOffset.dy;
						}
    				}
    				else
    				{
    					i = j;
    					nInc = 1;

						if (i >= iMultiEnd && i < nLastRef)
						{
							iMultiStart = iMultiEnd;
							iMultiEnd = iMultiStart + nInc * iDieNum;
							if (iMultiEnd > nLastRef)
								iMultiEnd = nLastRef;
							stPrPos = theRef.GetMultiPrPos(iMultiStart, iMultiEnd);
							stNextPrPos = theRef.GetMultiPrPos(iMultiStart + nInc * iDieNum, iMultiEnd + nInc * iDieNum);
							if (iMultiEnd == nLastRef)
								stNextPrPos = stPrPos;
							stPrPos.dx += stStepRepeatOffset.dx;
							stPrPos.dy += stStepRepeatOffset.dy;
							stNextPrPos.dx += stStepRepeatOffset.dx;
							stNextPrPos.dy += stStepRepeatOffset.dy;							
						}
					}
				}
			}
		}

        theRef.GetRcd(&stCurRef, i);
        stCurRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
        stCurRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

		for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
		{
			stCurRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
			stCurRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
			stCurRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
			stCurRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
		}

		MATRIX_RECORD stMatrix;
		theMatrix.GetRcd(&stMatrix, i);

        if (j < nLastRef)
        {
	        theRef.GetRcd(&stNextRef, i + nInc);
            stNextRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
            stNextRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

			for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
			{
				stNextRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
				stNextRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
				stNextRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
				stNextRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
			}
        }

        if (stMatrix.nVisionStatus == VISION_SKIP_DIE) continue;

		//if ((i == nStartRef) || (pstPgm->nVisionPreMove == 0))
        {
			if ((thePgm.GetDeviceSeq() != PGM_SINGLE)
				&& (thePgm.GetDeviceSeq() != PGM_MATRIX)
				&& (i == 0))  //lead PR
				stPrPos = stCurRef.stVisionAlignPos[0];

			theServo.GetTablePosition(&stCurPos);
			dMoveDist = Calc2PtDist(stCurPos, stPrPos) * theServo.GetResolXY();
			if (dMoveDist > 10.0)
            {
				long lMoveDistX = round(fabs(stCurPos.dx - stPrPos.dx) * theServo.GetResolXY());
				long lMoveDistY = round(fabs(stCurPos.dy - stPrPos.dy) * theServo.GetResolXY());

                nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
                theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, bSlowMove);

                nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
                theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, bSlowMove);
                Sleep(1);

                if (bSlowMove)
                {
                    nLastProfileX = 4;
                    nLastProfileY = 4;
                }

				theServo.FastMoveTable2(stPrPos);
            }
			stCurPos = theRef.GetMultiPrPos();
			stCurPos = stPrPos;
        }

		theVisionSys.EnableVideo(false);
		theVisionSys.UpdateLightingParameter(
					stCurRef.stVisionLightPara[0].nCoaxRedLight,
					stCurRef.stVisionLightPara[0].bSideRedLight,
					stCurRef.stVisionLightPara[0].nCoaxBlueLight,
					stCurRef.stVisionLightPara[0].bSideBlueLight);
        theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[0]);
		theServo.WaitTableStop();
        
        if (dMoveDist > VISION_LONG_DIST)
			Sleep(pstPgm->nVisionLongMoveDelay);
        else if (dMoveDist > 100.0)
			Sleep(pstPgm->nVisionMoveDelay);

		if (i == iMultiStart || i == 0)
			theVisionSys.GrabImage();
        if (stCurRef.nVisionAlgorithm[0] == 1)
            theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

        if (((stCurRef.nRefType == 0) && (pstPgm->nPrAlignMode == 1)) || //Single point alignment without PR angle
            (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT))
        {
			if ((j < nLastRef) && (i == iMultiEnd - nInc) && (i != 0) && pstPgm->nVisionPreMove == 1)
            {
				theServo.GetTablePosition(&stCurPos);
				lMoveDist = round(fabs(stCurPos.dx - stNextPrPos.dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep(1);
				}

				lMoveDist = round(fabs(stCurPos.dy - stNextPrPos.dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep(1);
				}

				theServo.FastMoveTable2(stNextPrPos);
			}

			if (stCurRef.nRefType == 0)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
					(short)stCurRef.ulVisionAlignPatternID[0],
					pstPgm->nAutoSkipScore);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPrPassScore, &stSrchResult);

            theMHS.SetThreadStop(false);

			if (lVisionError == FOK)
			{
				m_nAutoSkipDieCount = 0;

				stMatrix.nVisionStatus = VISION_SUCCESS;
				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
				stMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
				stMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
				stMatrix.stVisionAlignPos[1].dx = stMatrix.stVisionAlignPos[0].dx + stCurRef.stVisionAlignPos[1].dx - stCurRef.stVisionAlignPos[0].dx;
				stMatrix.stVisionAlignPos[1].dy = stMatrix.stVisionAlignPos[0].dy + stCurRef.stVisionAlignPos[1].dy - stCurRef.stVisionAlignPos[0].dy;
				if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
				{
					stMatrix.dCosAln = cos(stSrchResult.dAngle);
					stMatrix.dSinAln = sin(stSrchResult.dAngle);
				}
				theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
			}
      
			//PR FAIL, SHOW ERROR MSG
            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie())
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);

					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					theStat.SetMachineState(MCSTATE_STANDBY);

					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					if (!thePgm.IsAutoSkipDie() && !thePgm.IsAutoSkipUnit())
					{
						theServo.SetServoControlParameter();
						nMsgAction = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}
					else
					{
						if (((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
							|| (thePgm.GetDeviceSeq() == PGM_HYBRID)
							|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
							|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
							&& ((i == 0) || (m_nAutoSkipDieCount > 4)))
						{
							theServo.SetServoControlParameter();
							nMsgAction = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						}
						else
						{
							nMsgAction = SHOW_MSG_RESULT_SKIP;
							m_nAutoSkipDieCount++;
							m_lTotalAutoSkipDieCount++;
						}
					}

					theMHS.SetOxidationProtect(false);
	                
					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (nMsgAction != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == 0)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == 0)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
						}
					}

					if (nMsgAction == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (nMsgAction == SHOW_MSG_RESULT_OK)
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
					else
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						stMatrix.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);
	                
					theServo.GetTablePosition(&stCurPos);

					continue;
				}
			}
		}
        else if (pstPgm->nPrAlignMode == 0) //Two point alignment
		{
            if (stCurRef.nRefType == 0)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPrPassScore, &stSrchResult);
            theMHS.SetThreadStop(false);
            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie())
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);

					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
				
					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == 0)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}

					lMoveDist = round(fabs(stCurPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep(1);
					}

					lMoveDist = round(fabs(stCurPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep(1);
					}

					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);
					dMoveDist = Calc2PtDist(stCurPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurPos = stCurRef.stVisionAlignPos[0];
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					nMsgAction = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);
					if (nMsgAction == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (nMsgAction == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						stMatrix.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
	                
					theServo.GetTablePosition(&stCurPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					continue;
				}
            }
            else
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
				stMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
				stMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
            }

            //if (pstPgm->nVisionPreMove == 0)
            {
				if ((thePgm.GetDeviceSeq() != PGM_SINGLE)
					&& (thePgm.GetDeviceSeq() != PGM_MATRIX)
					&& (i == 0))  //lead PR
					stPrPos = stCurRef.stVisionAlignPos[1];

				theServo.GetTablePosition(&stCurPos);
				dMoveDist = Calc2PtDist(stCurPos, stPrPos) * theServo.GetResolXY();
				if (dMoveDist > 100.0)
				{
					lMoveDist = round(fabs(stCurPos.dx - stPrPos.dx) * theServo.GetResolXY());
					
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep(1);
					}

					lMoveDist = round(fabs(stCurPos.dy - stPrPos.dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep(1);
					}

					theServo.FastMoveTable2(stPrPos);

					dMoveDist = Calc2PtDist(stCurPos, stPrPos) * theServo.GetResolXY();
				}
            }

            theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[1].nCoaxRedLight, stCurRef.stVisionLightPara[1].bSideRedLight, stCurRef.stVisionLightPara[1].nCoaxBlueLight, stCurRef.stVisionLightPara[1].bSideBlueLight);
            theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

			theServo.WaitTableStop();

            if (dMoveDist > VISION_LONG_DIST)
                Sleep(pstPgm->nVisionLongMoveDelay);
            else
                Sleep(pstPgm->nVisionMoveDelay);
            
            theVisionSys.EnableVideo(false);
			if (i == 0)
				theVisionSys.GrabImage();

            if (stCurRef.nVisionAlgorithm[1] == 1)
                theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

			if ((j < nLastRef) && (i == iMultiEnd - nInc) && (i != 0) && pstPgm->nVisionPreMove == 1)
			{
				theServo.GetTablePosition(&stCurPos);
				lMoveDist = round(fabs(stCurPos.dx - stNextPrPos.dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep(1);
				}

				lMoveDist = round(fabs(stCurPos.dy - stNextPrPos.dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep(1);
				}

				theServo.FastMoveTable2(stNextPrPos);
			}

            if (stCurRef.nRefType == 0)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);
            lVisionError = theVisionSys.SearchVisionPattern(
					(short)stCurRef.ulVisionAlignPatternID[1],
					nSrchRange, nPrPassScore, &stSrchResult);
            if (lVisionError != FOK)
			{
				theTowerLight.EnableErrorLight();
				theTowerLight.EnableBlinkRedLight(true);

                if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                {
                    if (stCurRef.nRefType == 0)
                        theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                    else
                        theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
                }

                lMoveDist = round(fabs(stCurPos.dx - stPrPos.dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep(1);
                }

                lMoveDist = round(fabs(stCurPos.dy - stPrPos.dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep(1);
                }

				theServo.FastMoveTable2(stPrPos);
				dMoveDist = Calc2PtDist(stCurPos, stPrPos) * theServo.GetResolXY();
				stCurPos = stCurRef.stVisionAlignPos[1];
                
				theMHS.SetOxidationProtect(true);
				PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
                sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
                theServo.SetServoControlParameter();
                nMsgAction = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
                theMHS.SetOxidationProtect(false);

                if (nMsgAction == SHOW_MSG_RESULT_STOP)
                {
                    theVisionSys.EnableVideo(true);
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableIdleLight();
                    return QUIT_AUTO_BOND;
                }
                else if (nMsgAction == SHOW_MSG_RESULT_SKIP)
                {
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableBondingLight();
					ProcessKeyboard();	//v2.0.T38 add
					theServo.WaitTableStop();

                    stMatrix.nVisionStatus = VISION_SKIP_DIE;
                    theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);

                    if (m_nBondMode == BONDMODE_STOP)
                        return QUIT_AUTO_BOND;

                    theServo.SetServoControlParameter();
                }
                else
                {
					if (isManualAlignmentStop(i))
                        return QUIT_AUTO_BOND;
                }

                theServo.GetTablePosition(&stCurPos);
                if ((m_nBondMode != BONDMODE_IDLE)
					&& (m_nBondMode != BONDMODE_STOP))
                    theStat.SetMachineState(MCSTATE_PRODUCTION);
                continue;
            }
            else
			{
                stMatrix.nVisionStatus = VISION_SUCCESS;
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[1], &stSrchResult, &stVisionOffset);
				stMatrix.stVisionAlignPos[1].dx = stPrPos.dx + stVisionOffset.dx;
				stMatrix.stVisionAlignPos[1].dy = stPrPos.dy + stVisionOffset.dy;

                double dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0], stCurRef.stVisionAlignPos[1]);
                double dMatrixAlignDist = Calc2PtDist(stMatrix.stVisionAlignPos[0], stMatrix.stVisionAlignPos[1]);
                double dAlignTol;
                if (stCurRef.nRefType == REF_TYPE_LEAD)
                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                else
                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					if (thePgm.IsAutoSkipDie())
					{
						theMatrix.SetVisionStatus(stCurRef.nRefNum, VISION_SKIP_DIE);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);

						stMatrix.nVisionStatus = VISION_NOT_DONE;

						if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
						{
							if (stCurRef.nRefType == 0)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}

						sList.RemoveAll();
						if (stCurRef.nRefType == 0)
							sList.AddTail(theMsg.GetMsg(MSG_LEAD_TOL_FAILURE));
						else
							sList.AddTail(theMsg.GetMsg(MSG_DIE_TOL_FAILURE));
						sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
						theMHS.SetOxidationProtect(true);
						theServo.SetServoControlParameter();
						nMsgAction = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						theMHS.SetOxidationProtect(false);

						if (nMsgAction == SHOW_MSG_RESULT_STOP)
						{
							theVisionSys.EnableVideo(true);
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableIdleLight();
							return QUIT_AUTO_BOND;
						}
						else if (nMsgAction == SHOW_MSG_RESULT_SKIP)
						{
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableBondingLight();
							ProcessKeyboard();	//v2.0.T38 add
							theServo.WaitTableStop();

							stMatrix.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);

							if (m_nBondMode == BONDMODE_STOP)
								return QUIT_AUTO_BOND;

							theServo.SetServoControlParameter();
						}
						else
						{
							if (isManualAlignmentStop(i))
								return QUIT_AUTO_BOND;
						}

						theServo.GetTablePosition(&stCurPos);
						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
							theStat.SetMachineState(MCSTATE_PRODUCTION);
						continue;
					}
                }
                else
                    theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
            }
        }
    }

	short nAheadAlignDone = CheckAheadAlignDone(&m_nCurWire);
	if (nAheadAlignDone == 0)
		return PERFORM_VISION_ALIGN;
	else if (nAheadAlignDone == 2)
		return COMPLETE_UNIT_UPDATE;

    return PERFORM_WIRE_BOND;
}

/*	optimization function, perform Manual Alignment
	return: true  ------  Quit Bond processing
			false ------  Continue next function  */
bool CBond::isManualAlignmentStop(short nCurRec)
{
	theVisionSys.ClearDrawing();	//v3.1.T489 add
		bool bJoyStickValid = theScanKeyThread.IsJoyStickValid(); 
		theScanKeyThread.EnableJoyStick(); 
		theTowerLight.EnableBlinkRedLight(false);
		theTowerLight.EnableIdleLight();
		int lStopSelected = PerformManualAlignment(nCurRec);
		if (bJoyStickValid == false)
			theScanKeyThread.DisableJoyStick(); 

		theTowerLight.EnableBlinkRedLight(false);
		if (lStopSelected != SHOW_MSG_RESULT_OK)
		{
			theTowerLight.EnableIdleLight(); 
			return true;
		}
		
		theTowerLight.EnableBondingLight();
		
		return false;
}

/*	show Die overlap essage, perform Manual Alignment
	return: true  ------  Quit Bond processing
			false ------  Continue next function  */
bool CBond::isQuitManualAlign(short nSelectRec)
{
    int lStopSelected;
	REF_RECORD stCurRef;
    PR_SRCH_RESULT stSrchResult;
    MATRIX_RECORD stMatrixRec;
    CString str;
    CStringList sList;

    theRef.GetRcd(&stCurRef, nSelectRec);

	sList.RemoveAll();

	str.Format(_T("%s-%s%d"), theMsg.GetMsg(MSG_DIE),
					theMsg.GetMsg(MSG_REF), stCurRef.nRefNum);
	sList.AddTail(str);
	sList.AddTail(theMsg.GetMsg(MSG_PR_OVERLAP));
    sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
	lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
    if (lStopSelected == SHOW_MSG_RESULT_STOP)
		return true;
	if (lStopSelected == SHOW_MSG_RESULT_SKIP)
		return false;
	if (lStopSelected == SHOW_MSG_RESULT_OK)
	{
		theMatrix.GetRcd(&stMatrixRec, nSelectRec);
		stMatrixRec.nVisionStatus = VISION_FAILURE;
		theMatrix.CalcMatrix(&stMatrixRec, nSelectRec, VISION_PR_ALIGN);

		return isManualAlignmentStop(nSelectRec);
    }
	return false;
}

/*	check Die overlap in a single unit, 
	return: true  ------  Quit Bond processing
			false ------  Continue next function  */
bool CBond::isCheckDieOverlap(short nCurRec, short nSequenceType)
{
#define DIE_OVERLAP_TOL		100		//unit is um
	double dMatrixAlignDist, dAlignTol;
	bool isDieOverlap =false;
	short nCount, nLastRef = theRef.GetTotalRefNum() - 1;
    MATRIX_RECORD stMatrixCur, stMatrix2;
	
	theMatrix.GetRcd(&stMatrixCur, nCurRec);
	dAlignTol = DIE_OVERLAP_TOL/ theServo.GetResolXY();
	short nUnitTotalDie = thePgm.GetUnitTotalDie();
	short nCurUnitFirstRef = nCurRec - (nCurRec-1)%nUnitTotalDie;
    if (nSequenceType == PGM_DIE_LEAD_AHEAD)
        nCurUnitFirstRef = nCurRec - (nCurRec-1)%(nUnitTotalDie+1);
	// search all dies within a small unit
	for (nCount = nCurUnitFirstRef; nCount < nCurUnitFirstRef+nUnitTotalDie; nCount++ )
	{
		if (nCount ==nCurRec) //skip same Die
			continue;
		theMatrix.GetRcd(&stMatrix2, nCount);
		if ((stMatrix2.nVisionStatus == VISION_SUCCESS))
		{
			dMatrixAlignDist = Calc2PtDist(stMatrixCur.stVisionAlignPos[0],
											stMatrix2.stVisionAlignPos[0]);

			if (dMatrixAlignDist < dAlignTol) // check 1st point whether overlap
			{	//confirm overlap happens
				isDieOverlap = true;
				break;
			}
		}
	}
		
	if (!isDieOverlap) //not overlap
		return false;

	if (isQuitManualAlign(nCount))
		return true;

	if (isQuitManualAlign(nCurRec))
		return true;
	return false;
}

BYTE CBond::CheckSerialSkipNumLimit(PGM_RECORD *pstPgm)	//V1.1.W286 edit
{
	if (pstPgm->nSerialSkipNum == 0) return TRUE;
	if (m_nSerialSkipNum <= pstPgm->nSerialSkipNum) return TRUE;

	theTowerLight.EnableErrorLight();
	theTowerLight.EnableBlinkRedLight(true);

	m_nSerialSkipNum = 0;
	short nMsg = MSG_SERIAL_SKIP_LIMIT;
	nMsg = theMsg.ShowMsg(ERROR_MSG, 1, &nMsg, true, false) != SHOW_MSG_RESULT_STOP;

	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableBondingLight();

	return (BYTE)nMsg;
}

void CBond::ProcessKeyboard()	//V1.4.W20 add, v2.0.T34 rename
{
    MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if ((msg.message == WM_KEYDOWN)
			|| (msg.message == WM_KEYUP))
		{
			if (theMcConfig.GetKeyPadType() > 0)	//v2.0.T38 edit
			{
				if (theKeyPad.HotKeyHandler(&msg) == TRUE) break;
			}
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//v3.0.T329 add
void CBond::ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, double dParam)
{
	if (((enDebugMode == DEBUG_BOND_TIMING) && (theDebug.IsDebugBondTiming()
					|| theMcConfig.IsDebugModeBondTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_VISION_TIMING) && (theDebug.IsDebugVisionTiming()
					|| theMcConfig.IsDebugModeVisionTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_SERVO_TIMING) && theDebug.IsDebugServoTiming())	//v3.1.T310 add
		|| ((enDebugMode == DEBUG_WORK_HOLDER) && theDebug.IsDebugWorkHolder())	//v3.0.T370 add
		|| ((enDebugMode == TIME_STAMP_CYCLE) && theMcConfig.IsTimeStampCycle())
		|| ((enDebugMode == TIME_STAMP_PR) && theMcConfig.IsTimeStampPR())
		|| ((enDebugMode == TIME_STAMP_BOND) && theMcConfig.IsTimeStampBond()))
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		m_sTemp.AppendFormat(_T("%.3f_%s_%.0fms"), GetElapseBondTime(), sMsg, dParam);
		theDebug.AddInfo(m_sTemp);
		m_dLastBondTime = m_dCurBondTime;
	}
}

//v1.6.T234 edit
void CBond::ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg)
{
	if (((enDebugMode == DEBUG_BOND_TIMING) && (theDebug.IsDebugBondTiming()
					|| theMcConfig.IsDebugModeBondTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_VISION_TIMING) && (theDebug.IsDebugVisionTiming()
					|| theMcConfig.IsDebugModeVisionTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_SERVO_TIMING) && theDebug.IsDebugServoTiming())	//v3.1.T310 add
		|| ((enDebugMode == DEBUG_WORK_HOLDER) && theDebug.IsDebugWorkHolder())	//v3.0.T370 add
		|| ((enDebugMode == TIME_STAMP_CYCLE) && theMcConfig.IsTimeStampCycle())
		|| ((enDebugMode == TIME_STAMP_PR) && theMcConfig.IsTimeStampPR())
		|| ((enDebugMode == TIME_STAMP_BOND) && theMcConfig.IsTimeStampBond()))
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		m_sTemp.AppendFormat(_T("%.3f_%s"), GetElapseBondTime(), sMsg);
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		m_dLastBondTime = m_dCurBondTime;
	}
}

//v1.5.T57 add, v1.6.T234 edit
void CBond::ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam)
{
	if (((enDebugMode == DEBUG_BOND_TIMING) && (theDebug.IsDebugBondTiming()
					|| theMcConfig.IsDebugModeBondTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_VISION_TIMING) && (theDebug.IsDebugVisionTiming()
					|| theMcConfig.IsDebugModeVisionTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_SERVO_TIMING) && theDebug.IsDebugServoTiming())	//v3.1.T310 add
		|| ((enDebugMode == DEBUG_WORK_HOLDER) && theDebug.IsDebugWorkHolder())	//v3.0.T370 add
		|| ((enDebugMode == TIME_STAMP_CYCLE) && theMcConfig.IsTimeStampCycle())
		|| ((enDebugMode == TIME_STAMP_PR) && theMcConfig.IsTimeStampPR())
		|| ((enDebugMode == TIME_STAMP_BOND) && theMcConfig.IsTimeStampBond()))
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		if (sMsg == _T("PrStatus"))		//v3.0.T303 add
		{
			m_sTemp.AppendFormat(_T("%.3f_%s_%d"),
					GetElapseBondTime() + theMcConfig.GetSrchPrShowTimeOffset(),	//v3.0.T303 edit
					sMsg, nParam);
		}
		else
		{
			m_sTemp.AppendFormat(_T("%.3f_%s_%d"),
					GetElapseBondTime(), sMsg, nParam);
		}
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		m_dLastBondTime = m_dCurBondTime;
	}
}

//v1.6.T234 add
void CBond::ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, double dParam2, double dParam3, double dParam4)
{
	if (((enDebugMode == DEBUG_BOND_TIMING) && (theDebug.IsDebugBondTiming()
					|| theMcConfig.IsDebugModeBondTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_VISION_TIMING) && (theDebug.IsDebugVisionTiming()
					|| theMcConfig.IsDebugModeVisionTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_SERVO_TIMING) && theDebug.IsDebugServoTiming())	//v3.1.T310 add
		|| ((enDebugMode == DEBUG_WORK_HOLDER) && theDebug.IsDebugWorkHolder())	//v3.0.T370 add
		|| ((enDebugMode == TIME_STAMP_CYCLE) && theMcConfig.IsTimeStampCycle())
		|| ((enDebugMode == TIME_STAMP_PR) && theMcConfig.IsTimeStampPR())
		|| ((enDebugMode == TIME_STAMP_BOND) && theMcConfig.IsTimeStampBond()))
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		m_sTemp.AppendFormat(_T("%.3f_%s_%d_%.2f_%.2f_%.2f"),
							GetElapseBondTime(), sMsg,
							nParam1, dParam2, dParam3, dParam4);
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		m_dLastBondTime = m_dCurBondTime;
	}
}

//v1.6.T234 add
void CBond::ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, double dParam2, double dParam3)
{
	if (((enDebugMode == DEBUG_BOND_TIMING) && (theDebug.IsDebugBondTiming()
					|| theMcConfig.IsDebugModeBondTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_VISION_TIMING) && (theDebug.IsDebugVisionTiming()
					|| theMcConfig.IsDebugModeVisionTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_SERVO_TIMING) && theDebug.IsDebugServoTiming())	//v3.1.T310 add
		|| ((enDebugMode == DEBUG_WORK_HOLDER) && theDebug.IsDebugWorkHolder())	//v3.0.T370 add
		|| ((enDebugMode == TIME_STAMP_CYCLE) && theMcConfig.IsTimeStampCycle())
		|| ((enDebugMode == TIME_STAMP_PR) && theMcConfig.IsTimeStampPR())
		|| ((enDebugMode == TIME_STAMP_BOND) && theMcConfig.IsTimeStampBond()))
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		m_sTemp.AppendFormat(_T("%.3f_%s_%d_%.2f_%.2f"),
			GetElapseBondTime(), sMsg, nParam1, dParam2, dParam3);
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		m_dLastBondTime = m_dCurBondTime;
	}
}

//v1.6.T234 add
void CBond::ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, int nParam2, double dParam3)
{
	if (((enDebugMode == DEBUG_BOND_TIMING) && (theDebug.IsDebugBondTiming()
					|| theMcConfig.IsDebugModeBondTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_VISION_TIMING) && (theDebug.IsDebugVisionTiming()
					|| theMcConfig.IsDebugModeVisionTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_SERVO_TIMING) && theDebug.IsDebugServoTiming())	//v3.1.T310 add
		|| ((enDebugMode == DEBUG_WORK_HOLDER) && theDebug.IsDebugWorkHolder())	//v3.0.T370 add
		|| ((enDebugMode == TIME_STAMP_CYCLE) && theMcConfig.IsTimeStampCycle())
		|| ((enDebugMode == TIME_STAMP_PR) && theMcConfig.IsTimeStampPR())
		|| ((enDebugMode == TIME_STAMP_BOND) && theMcConfig.IsTimeStampBond()))
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		m_sTemp.AppendFormat(_T("%.3f_%s_%d_%d_%.2f"),
			GetElapseBondTime(), sMsg, nParam1, nParam2, dParam3);
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		m_dLastBondTime = m_dCurBondTime;
	}
}

//v1.5.T114 add, v1.6.T234 edit
void CBond::ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, int nParam2)
{
	if (((enDebugMode == DEBUG_BOND_TIMING) && (theDebug.IsDebugBondTiming()
					|| theMcConfig.IsDebugModeBondTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_VISION_TIMING) && (theDebug.IsDebugVisionTiming()
					|| theMcConfig.IsDebugModeVisionTime()))	//v3.1.T480 add
		|| ((enDebugMode == DEBUG_SERVO_TIMING) && theDebug.IsDebugServoTiming())	//v3.1.T310 add
		|| ((enDebugMode == DEBUG_WORK_HOLDER) && theDebug.IsDebugWorkHolder())	//v3.0.T370 add
		|| ((enDebugMode == TIME_STAMP_CYCLE) && theMcConfig.IsTimeStampCycle())
		|| ((enDebugMode == TIME_STAMP_PR) && theMcConfig.IsTimeStampPR())
		|| ((enDebugMode == TIME_STAMP_BOND) && theMcConfig.IsTimeStampBond()))
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		m_sTemp.AppendFormat(_T("%.3f_%s_%d_%d"),
			GetElapseBondTime(), sMsg, nParam1, nParam2);
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		m_dLastBondTime = m_dCurBondTime;
	}
}

int CBond::DieAheadAlign(bool bSlowMove, EN_VISION_ACTION nVisionAction)	//V1.4.W20 add
{
    short i, k, nStartRef, nLastRef;
	short nSrchRange, nPassScore;
    int nSrchErr, lStopSelected;
    double dMoveDist = 0.0;
    CStringList sList;
    TBL_POS stPrOS, stStepRepeatOS, stCurTblPos, stPrPos;
    REF_RECORD stCurRef, stNextRef;
    MATRIX_RECORD stMatrix;
    PR_SRCH_RESULT stSrchRes;
	TOP_DEVICE_RECORD *pstTopDevice; 
    CUELITE_DEVICE_RECORD stCuEliteDevicePara;
    short nLastProfileX = 0, nLastProfileY = 0, nMoveProfile;
    long lMoveDist, lMoveDistX, lMoveDistY;
    short nLastIndexCol, nLastIndex, nUnitNum, nUnitTotalDie, nUnitBaseRef;
	PGM_RECORD *pstPgm = thePgm.GetRcd();

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("DieAheadAlign"));

	theVisionDisplay.EnableDisplay(false);

    if (m_bBondNoVision && (nVisionAction == VISION_ACTION_AUTO))
        return PERFORM_WIRE_BOND;

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_TOP_LED: case MHS_VLED: case MHS_VLED_ELITE:
	case MHS_TOP_ELITE:	//v3.0.T410 add
    {
		pstTopDevice = theDevice.GetTopRcd();
        if (pstTopDevice->nHalfIndexOn == 0)
        {
            nLastIndexCol = theDevice.GetLastIndexWndColumn();
            if (nLastIndexCol < thePgm.GetDeviceColNum())
            {
	            if (pstTopDevice->nWndUnitNum == 0) //throw "w.temp.37" ;
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}
            }
        }
		break;
    }
	case MHS_R2R:
    {
		pstTopDevice = theDevice.GetTopRcd();
        if (pstTopDevice->nHalfIndexOn == 0)
        {
            nLastIndexCol = theDevice.GetLastIndexWndColumn();
            if (nLastIndexCol < thePgm.GetDeviceColNum())
            {
	            if (pstTopDevice->nWndUnitNum == 0) //throw "w.temp.37" ;
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}

                nLastIndex = pstTopDevice->nDeviceColmNum
								/ pstTopDevice->nWndUnitNum + 1;
                if (theR2RTrack.GetCurrentBondUnit() == nLastIndex)
                {
                    for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                    {
                        for (k = nLastIndexCol + 1; k <= thePgm.GetDeviceColNum(); k++)
                        {
                            nUnitNum = thePgm.GetUnitNum(i, k);
                            nUnitTotalDie = thePgm.GetUnitTotalDie();

                            nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                            for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                            {
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                            }
                        }
                    }
                }
            }
        }
		break;
    }
	case MHS_CU_ELITE:
    {
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

        if (stCuEliteDevicePara.nHalfIndexOn == 0)
        {
            nLastIndexCol = theDevice.GetLastIndexWndColumn();
            if (nLastIndexCol < thePgm.GetDeviceColNum())
            {
			    if (stCuEliteDevicePara.nWndUnitNum == 0) //throw "w.temp.38" ;
				{
					AfxMessageBox(_T("temp.38, nWndUnitNum == 0"));
					return 0;
				}

                nLastIndex = stCuEliteDevicePara.nDeviceColmNum / stCuEliteDevicePara.nWndUnitNum + 1;
                if (theCuEliteTrack.GetCurrentBondUnit() == nLastIndex)
                {
                    for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                    {
                        for (k = nLastIndexCol + 1; k <= thePgm.GetDeviceColNum(); k++)
                        {
                            nUnitNum = thePgm.GetUnitNum(i, k);
                            nUnitTotalDie = thePgm.GetUnitTotalDie();

                            nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                            for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                            {
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                            }
                        }
                    }
                }
            }
        }
		break;
    }
	}

    nStartRef = 0;
    nLastRef = theRef.GetTotalRefNum() - 1;
    GetPerformPrRef(m_nCurWire, &nStartRef, &nLastRef);

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Check PR"), nStartRef, nLastRef);

	if (nVisionAction == VISION_ACTION_ALL_UNIT)
	{
		nStartRef = 0;
		nLastRef = theRef.GetTotalRefNum() - 1;
	}
    else if (nVisionAction == VISION_ACTION_SINGLE_UNIT)
    {
		nStartRef = 0;
        nLastRef = thePgm.GetUnitTotalDie();
    }

    for (i = nStartRef; i <= nLastRef; i++)
    {
        if ((theMatrix.GetRcd(i)->nVisionStatus != VISION_SUCCESS)
			&& (theMatrix.GetRcd(i)->nVisionStatus != VISION_SKIP_DIE))
        {
            nStartRef = i;
            break;
        }
    }

    if (i > nLastRef) //All alignments already done
        return PERFORM_WIRE_BOND;

    for (i = nLastRef; i >= nStartRef; i--)
    {
        if ((theMatrix.GetRcd(i)->nVisionStatus == VISION_SUCCESS)
			|| (theMatrix.GetRcd(i)->nVisionStatus == VISION_SKIP_DIE))
            nLastRef -= 1;
        else
            break;
    }

    if (nLastRef < nStartRef) return PERFORM_WIRE_BOND;

    ShowDebugMsg(DEBUG_VISION_TIMING, _T("Perform PR"), nStartRef, nLastRef);
    theServo.GetTablePosition(&stCurTblPos);
	thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOS);

	short nInc = 1;
    for (short j = nStartRef; j <= nLastRef; j++)
	{
		//Default Reverse
		switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
		{
		case MHS_TOP_LED: case MHS_R2R:
		case MHS_VLED: case MHS_VLED_ELITE:
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			pstTopDevice = theDevice.GetTopRcd();
			break;
		}
		case MHS_CU_ELITE:
		{
	        theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
			break;
		}
		}
		
		if (j == 0)  //lead PR
		{
			//for the high power 3rd index
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (pstTopDevice->nHalfIndexOn > 0)
                    && (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (pstTopDevice->nHalfIndexOn > 0)
                    && (theTEIndex.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (pstTopDevice->nHalfIndexOn > 0)
                    && (theR2RTrack.GetCurrentBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (pstTopDevice->nHalfIndexOn > 0)
                    && (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (pstTopDevice->nHalfIndexOn > 0)
                    && (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn)))
			{
				WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);	//v3.1.T90 edit
				if ((pstWire->nRefNum[0] == 0) || (pstWire->nRefNum[1] == 0))
					nInc = max(pstWire->nRefNum[0], pstWire->nRefNum[1]);
				else
					nInc = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);

				i = 0;
				j = nInc - 1;
				//nInc=nLastRef;
			}
            else
			{
				i = 0;
				nInc = nLastRef;
			}
		}
		else  //after lead PR
		{
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTEIndex.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theR2RTrack.GetCurrentBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (pstTopDevice->nHalfIndexOn > 0)
                    && (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (pstTopDevice->nHalfIndexOn > 0)
                    && (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn)))
			{
				i = j;
				nInc = 1;
			}
			else
			{
				if (nStartRef == 0)
				{
				   i = nLastRef - j + 1;
				   nInc = -1;
				}
				else
				{
					i = j;
					nInc = 1;
				}
			}
		}

		stCurRef = *theRef.GetRcd(i);
        stCurRef.stVisionAlignPos[0].dx += stStepRepeatOS.dx;
        stCurRef.stVisionAlignPos[0].dy += stStepRepeatOS.dy;
        stCurRef.stVisionAlignPos[1].dx += stStepRepeatOS.dx;
        stCurRef.stVisionAlignPos[1].dy += stStepRepeatOS.dy;

		for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
		{
			stCurRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOS.dx; 
			stCurRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOS.dy;
			stCurRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOS.dx;
			stCurRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOS.dy;
		}

		stMatrix = *theMatrix.GetRcd(i);
        if (stMatrix.nVisionStatus == VISION_SKIP_DIE) continue;	//V1.1.W250 add

        if (j < nLastRef)
        {
			stNextRef = *theRef.GetRcd(i + nInc);
            stNextRef.stVisionAlignPos[0].dx += stStepRepeatOS.dx;
            stNextRef.stVisionAlignPos[0].dy += stStepRepeatOS.dy;
            stNextRef.stVisionAlignPos[1].dx += stStepRepeatOS.dx;
            stNextRef.stVisionAlignPos[1].dy += stStepRepeatOS.dy;

			for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
			{
				stNextRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOS.dx; 
				stNextRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOS.dy;
				stNextRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOS.dx;
				stNextRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOS.dy;
			}
        }

        if (stMatrix.nVisionStatus == VISION_SUCCESS) continue;

		theVisionSys.UpdateLightingParameter(
					stCurRef.stVisionLightPara[0].nCoaxRedLight,
					stCurRef.stVisionLightPara[0].bSideRedLight,
					stCurRef.stVisionLightPara[0].nCoaxBlueLight,
					stCurRef.stVisionLightPara[0].bSideBlueLight);

        lMoveDistX = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
        lMoveDistY = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());

		dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
		if ((lMoveDistX > 0) || (lMoveDistY > 0))	//V1.1.W303 edit
		{
			nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
			theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, bSlowMove);

			nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
			theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, bSlowMove);
			Sleep_x1ms(1);

			if (bSlowMove == true)
			{
				nLastProfileX = 4;
				nLastProfileY = 4;
			}

			//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
			theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
		}

        stCurTblPos = stCurRef.stVisionAlignPos[0];

		ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
								stCurRef.nRefNum,
								stCurRef.stVisionAlignPos[0].dx,
								stCurRef.stVisionAlignPos[0].dy);

		stPrPos = stCurRef.stVisionAlignPos[0];
        theVisionSys.EnableVideo(false);

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("start ref"), stCurRef.nRefNum);

		theServo.WaitTableStop();
        
		//pr srch delay
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
        if (dMoveDist > VISION_LONG_DIST)
			Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
        else if (dMoveDist > 100.0)
			Sleep_x1ms(pstPgm->nVisionMoveDelay);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

		theVisionSys.GrabImage();
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

        if (stCurRef.nVisionAlgorithm[0] == 1)
            theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

        if (((stCurRef.nRefType == REF_TYPE_LEAD) && (pstPgm->nPrAlignMode == 1)) || //Single point alignment without PR angle
            (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT))
        {
            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
				Sleep_x1ms(20);
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_UpdateLightPara"));

                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
							stNextRef.nRefNum,
							stNextRef.stVisionAlignPos[0].dx,
							stNextRef.stVisionAlignPos[0].dy);
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            nSrchErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), nSrchErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), nSrchErr);

            theMHS.SetThreadStop(false);

			BOOL bRedoPrEnable = FALSE;
			switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
			{
			case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				bRedoPrEnable = FALSE;
				break;
			}
			case MHS_VLED: case MHS_VLED_ELITE:
			{
                bRedoPrEnable = pstPgm->bPrRetry;
				break;
			}
			}

            if ((nSrchErr != FOK) && bRedoPrEnable) 
			{
                nSrchErr = RetryPR(
					&stPrPos, stCurRef.ulVisionAlignPatternID[0],
					stCurRef.nVisionAlgorithm[0],
					stCurRef.stVisionLightPara[0].nImageThreshold,
					nPassScore, &stSrchRes);
                
                if (nSrchErr != FOK)
                {
                    lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITH_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0], MTN_ABS, MTN_WAIT);	//v3.1.T56 edit
					stPrPos = stCurRef.stVisionAlignPos[0];
					stCurTblPos = stCurRef.stVisionAlignPos[0];

                    theVisionSys.SetCameraExposureTime(pstPgm->nRetryExposureTime);
                    nSrchErr = RetryPR(
						&stPrPos, stCurRef.ulVisionAlignPatternID[0],
						stCurRef.nVisionAlgorithm[0],
						stCurRef.stVisionLightPara[0].nImageThreshold,
						nPassScore, &stSrchRes);
                    theVisionSys.SetCameraExposureTime(pstPgm->nExposureTime);
                }
            }

			if (nSrchErr == FOK)
			{
				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrResult"),
							stCurRef.nRefNum, stSrchRes.dOffsetX,
							stSrchRes.dOffsetY, stSrchRes.dAngle);

                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/Offset"),
					stCurRef.nRefNum, stPrOS.dx, stPrOS.dy);

                stMatrix.nVisionStatus = VISION_SUCCESS;
                m_nAutoSkipDieCount = 0;

				stMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stPrOS.dx;
				stMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stPrOS.dy;
				stMatrix.stVisionAlignPos[1].dx = stMatrix.stVisionAlignPos[0].dx + stCurRef.stVisionAlignPos[1].dx - stCurRef.stVisionAlignPos[0].dx;
				stMatrix.stVisionAlignPos[1].dy = stMatrix.stVisionAlignPos[0].dy + stCurRef.stVisionAlignPos[1].dy - stCurRef.stVisionAlignPos[0].dy;

				if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
				{
					stMatrix.dCosAln = cos(stSrchRes.dAngle);
					stMatrix.dSinAln = sin(stSrchRes.dAngle);
				}

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/MatPos"),
								stCurRef.nRefNum,
								stMatrix.stVisionAlignPos[0].dx,
								stMatrix.stVisionAlignPos[0].dy);

                theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
				
				//check current Die overlap
				if ((j > 0) && (stCurRef.nRefType == REF_TYPE_DIE)
					&& isCheckDieOverlap(i, thePgm.GetDeviceSeq()))
					return QUIT_AUTO_BOND;
			}

			if ((nSrchErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nSrchErr = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, &stMatrix, i, j, nLastRef, nLastProfileX, nLastProfileY, nSrchRange, nPassScore);

			//PR failed, show error or skip die/unit
            if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if (m_bRedoPR == FALSE)
						theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if (m_bRedoPR == FALSE)
						theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
					theStat.SetMachineState(MCSTATE_STANDBY);
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);

					theMHS.SetOxidationProtect(false);
	                
					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
							{
								if (pstPgm->nManualSkip == 0)
									theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
								else
									SetSkipUnit(i);
							}
						}
					}

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_OK)
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
					else
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();

						theServo.WaitTableStop();

						stMatrix.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);
	                
					theServo.GetTablePosition(&stCurTblPos);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
		}
        else if ((stCurRef.nRefType == REF_TYPE_DIE) && (pstPgm->bDiePrMultiSrch == true)) //Two point alignment with multi search
		{
            double dRefAlignDist, dMatrixAlignDist, dAlignTol;

            //Pre move to next point
            if ((j < nLastRef) && pstPgm->nVisionPreMove == 1)
            {
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
            }

            nSrchRange = pstPgm->nDieSearchRange;
            nPassScore = pstPgm->nPrPassScore;
			
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
            nSrchErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPassScore, &stSrchRes);
            theMHS.SetThreadStop(false);
            if (nSrchErr == FOK)
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
                stMatrix.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stPrOS.dx;
                stMatrix.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stPrOS.dy;

                nSrchErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSrchRange, nPassScore, &stSrchRes);
                if (nSrchErr == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);

                    stMatrix.nVisionStatus = VISION_SUCCESS;
                    m_nAutoSkipDieCount = 0;

                    stMatrix.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[0].dx + stPrOS.dx;
                    stMatrix.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[0].dy + stPrOS.dy;

                    dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0],
												stCurRef.stVisionAlignPos[1]);
                    dMatrixAlignDist = Calc2PtDist(stMatrix.stVisionAlignPos[0],
												stMatrix.stVisionAlignPos[1]);
                    if (stCurRef.nRefType == REF_TYPE_LEAD)
                        dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                    else
                        dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                    if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                        nSrchErr = FNOK;
                    else
                        theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
                }
            }

			if (nSrchErr != FOK && stCurRef.nBakUpIndex > 0) 
			{
				int iTryTimes;

				for (iTryTimes = 0; iTryTimes < MAX_BAK_UP_PR_NUM; iTryTimes++)
				{
					if (stCurRef.stBakUpPr[iTryTimes].bValid == true)
					{
						theVisionSys.UpdateLightingParameter(
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxBlueLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dx)
							* theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dy)
							* theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep_x1ms(1);
                        }

						theServo.FastMoveTable2(stCurRef.stBakUpPr[iTryTimes].stAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos,
							stCurRef.stBakUpPr[iTryTimes].stAlignPos[0])
							* theServo.GetResolXY();
						stPrPos = stCurRef.stBakUpPr[iTryTimes].stAlignPos[0];

						theServo.WaitTableStop();

						if (dMoveDist > VISION_LONG_DIST)
							Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
						else
							Sleep_x1ms(pstPgm->nVisionMoveDelay);

						theVisionSys.EnableVideo(false);
						theVisionSys.GrabImage();

						if (stCurRef.stBakUpPr[iTryTimes].nAlgo[0] == 0)
							nSrchErr = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[0],
								nSrchRange, nPassScore, &stSrchRes, iTryTimes);

                        if (nSrchErr == FOK)
                        {
                            theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
                            stMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stPrOS.dx;
                            stMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stPrOS.dy;

							theVisionSys.UpdateLightingParameter(
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxBlueLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideBlueLight);
							theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

                            nSrchErr = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[1],
								nSrchRange, nPassScore, &stSrchRes, iTryTimes);
                            if (nSrchErr == FOK)
                            {
                                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
                                
                                stMatrix.nVisionStatus = VISION_SUCCESS;
                                m_nAutoSkipDieCount = 0;

                                stMatrix.stVisionAlignPos[1].dx = stPrPos.dx + stPrOS.dx;
                                stMatrix.stVisionAlignPos[1].dy = stPrPos.dy + stPrOS.dy;

                                dRefAlignDist = Calc2PtDist(
									stCurRef.stBakUpPr[iTryTimes].stAlignPos[0],
									stCurRef.stBakUpPr[iTryTimes].stAlignPos[1]);
                                dMatrixAlignDist = Calc2PtDist(
									stMatrix.stVisionAlignPos[0],
									stMatrix.stVisionAlignPos[1]);
                                if (stCurRef.nRefType == REF_TYPE_LEAD)
                                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                                else
                                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                                    nSrchErr = FNOK;
                                else
                                   theMatrix.CalcMatrix(&stMatrix, i, iTryTimes+2);
                            }
                        }
					}

					if (nSrchErr == FOK)
					{
						IcremBkPrCounts(iTryTimes);
						break;
					}
				}
				
				if (nSrchErr == FOK)
				{
					//Pre move to next point
					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep_x1ms(1);
                        }

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}
                }
			}

            if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);

					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nSrchErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

	                theStat.SetMachineState(MCSTATE_STANDBY);

		            lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
			        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				    if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
	                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
		                Sleep_x1ms(1);
			        }

				    lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
	                if (nMoveProfile != nLastProfileY)
		            {
			            nLastProfileY = nMoveProfile;
				        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					    Sleep_x1ms(1);
	                }

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					if (!thePgm.IsAutoSkipDie()
						&& !thePgm.IsAutoSkipUnit())
			        {
				        theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
	                }
					else
					{
				        theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}

		            if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
				    {
					    if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
	                        if (stCurRef.nRefType == REF_TYPE_LEAD)
		                        theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
			                else
				                theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					    }
						else
	                    {
		                    if (stCurRef.nRefType == REF_TYPE_LEAD)
			                    theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
				            else
					            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
						}
	                }

		            theMHS.SetOxidationProtect(false);
			        if (lStopSelected == 1)
				    {
					    theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
	                    theTowerLight.EnableIdleLight();
		                return QUIT_AUTO_BOND;
			        }
					else if(lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
						}
						else
						{
							SetSkipUnit(i);
						}

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

	                    theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
						    return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

	                if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
		            {
						theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);

				        lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
	                    if (nMoveProfile != nLastProfileX)
		                {
			                nLastProfileX = nMoveProfile;
				            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					        Sleep_x1ms(1);
						}

	                    lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
					    {
				            nLastProfileY = nMoveProfile;
			                theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
        }
        else if (pstPgm->nPrAlignMode == 0) //Two point alignment
		{
            //Pre move to next point
            if (pstPgm->nVisionPreMove == 1)
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPassScore = pstPgm->nPrPassScore;
            }
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            nSrchErr = theVisionSys.SearchVisionPattern(
					(short)stCurRef.ulVisionAlignPatternID[0],
					nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), nSrchErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), nSrchErr);
            theMHS.SetThreadStop(false);
			if ((nSrchErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nSrchErr = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, &stMatrix, i, j, nLastRef, nLastProfileX, nLastProfileY, nSrchRange, nPassScore);

            if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}
	                
					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						stMatrix.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
	                
					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
            }
            else
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
                stMatrix.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stPrOS.dx;
                stMatrix.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stPrOS.dy;
            }

            if (pstPgm->nVisionPreMove == 0)
            {
				theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[1].nCoaxRedLight, stCurRef.stVisionLightPara[1].bSideRedLight, stCurRef.stVisionLightPara[1].nCoaxBlueLight, stCurRef.stVisionLightPara[1].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

			theServo.WaitTableStop();

			//pr srch delay
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
            if (dMoveDist > VISION_LONG_DIST)
                Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
            else
                Sleep_x1ms(pstPgm->nVisionMoveDelay);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

            theVisionSys.EnableVideo(false);
            theVisionSys.GrabImage();
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

            if (stCurRef.nVisionAlgorithm[1] == 1)
                theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveNextAlignPos1"));

                dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
            nSrchErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat2"), nSrchErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat2"), nSrchErr);
			if ((nSrchErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nSrchErr = TryBackupPR(1, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, &stMatrix, i, j, nLastRef, nLastProfileX, nLastProfileY, nSrchRange, nPassScore);

            if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[1];

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
						}
						else
						{
							SetSkipUnit(i);
						}

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter();
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
            }
            else
			{
                double dRefAlignDist, dMatrixAlignDist, dAlignTol;
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[1], &stSrchRes, &stPrOS);

                stMatrix.nVisionStatus = VISION_SUCCESS;
                stMatrix.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[1].dx + stPrOS.dx;
                stMatrix.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[1].dy + stPrOS.dy;

                dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0],
											stCurRef.stVisionAlignPos[1]);
                dMatrixAlignDist = Calc2PtDist(stMatrix.stVisionAlignPos[0],
											stMatrix.stVisionAlignPos[1]);
                if (stCurRef.nRefType == REF_TYPE_LEAD)
                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                else
                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
						nSrchErr == VISION_ERROR_NO_PATTERN)
					{
						theMatrix.SetVisionStatus(stCurRef.nRefNum, VISION_SKIP_DIE);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
					{
						SetSkipUnit(i);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);

						stMatrix.nVisionStatus = VISION_NOT_DONE;

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}

						sList.RemoveAll();
						sList.AddTail(theMsg.GetMsg(stCurRef.nRefType == REF_TYPE_LEAD ? MSG_LEAD_TOL_FAILURE : MSG_DIE_TOL_FAILURE));
						sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

						theMHS.SetOxidationProtect(true);
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						theMHS.SetOxidationProtect(false);

						if (lStopSelected == SHOW_MSG_RESULT_STOP)
						{
							theVisionSys.EnableVideo(true);
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableIdleLight();
							return QUIT_AUTO_BOND;
						}
						else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
						{
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableBondingLight();
							ProcessKeyboard();
							theServo.WaitTableStop();

							if (pstPgm->nManualSkip == 0)
							{
								stMatrix.nVisionStatus = VISION_SKIP_DIE;
								theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
							}
							else
							{
								SetSkipUnit(i);
							}

							if (m_nBondMode == BONDMODE_STOP)
								return QUIT_AUTO_BOND;

							theServo.SetServoControlParameter();
						}
						else
						{
							if (isManualAlignmentStop(i))
								return QUIT_AUTO_BOND;
						}

						theServo.GetTablePosition(&stCurTblPos);

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
							theStat.SetMachineState(MCSTATE_PRODUCTION);

						if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
						{
							lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileX)
							{
								nLastProfileX = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
								Sleep_x1ms(1);
							}

							lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileY)
							{
								nLastProfileY = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
								Sleep_x1ms(1);
							}

							//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
							theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
							dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
							stCurTblPos = stNextRef.stVisionAlignPos[0];
						}

						continue;
					}
                }
                else
                    theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
            }

			//PR failed, show error or skip die/unit
            if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nSrchErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
					theStat.SetMachineState(MCSTATE_STANDBY);
					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
				}
            }
        }

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end ref"), stCurRef.nRefNum);
    }

	if (m_bIll) thePrAlign.PerformInteliLeadLocate(true);

	short nAheadAlignDone = CheckAheadAlignDone(&m_nCurWire);
    if (nAheadAlignDone == 0)
        return PERFORM_VISION_ALIGN;
    else if (nAheadAlignDone == 2)
	{
		if (thePgm.IsAutoSkipDie())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef)
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG,sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		else if (thePgm.IsAutoSkipUnit())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef / thePgm.GetUnitTotalDie())
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG,sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}

        return COMPLETE_UNIT_UPDATE;
	}

	return PERFORM_WIRE_BOND;
}

int CBond::MatrixDieAheadAlign(bool bSlowMove, EN_VISION_ACTION nVisionAction)
{
	short i, k, nStartRef, nLastRef, nAheadAlignDone, nSearchRange, nPrPassScore;
	int lVisionError, lStopSelected;
	double dMoveDist = 0.0;
	CStringList sList;
	TBL_POS stVisionOffset, stStepRepeatOffset, stCurTblPos, stPrPos;
	REF_RECORD stCurRef, stNextRef;
	MATRIX_RECORD stMatrix1;
	PR_SRCH_RESULT stSrchResult;
	TOP_DEVICE_RECORD stTopDevicePara; 
	CUELITE_DEVICE_RECORD stCuEliteDevicePara;
	short nLastProfileX = 0, nLastProfileY = 0, nMoveProfile;
	long lMoveDist, lMoveDistX, lMoveDistY;
	short nLastIndexWndColumn, nLastIndex, nUnitNum, nUnitTotalDie, nUnitBaseRef;
	PGM_RECORD *pstPgm = thePgm.GetRcd();

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("MatrixDieAheadAlign"));

	theVisionDisplay.EnableDisplay(false);

	if (m_bBondNoVision && (nVisionAction == VISION_ACTION_AUTO))
		return PERFORM_WIRE_BOND;

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_TOP_LED: case MHS_VLED: case MHS_VLED_ELITE:
    {
        theDevice.GetTopRcd(&stTopDevicePara);

        //if (stTopDevicePara.nHalfIndexOn == 0)	//V1.1.W256 delete
        {
            nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
            if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
            {
	            if (stTopDevicePara.nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}

                nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                if (theTopTrack.GetCurBondUnit() == nLastIndex)
                {
                    for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                    {
                        for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                        {
                            nUnitNum = thePgm.GetUnitNum(i, k);
                            nUnitTotalDie = thePgm.GetUnitTotalDie();

                            nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                            for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                            {
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                            }
                        }
                    }
                }
            }
        }
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
		nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
		if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
		{
	        theDevice.GetTopRcd(&stTopDevicePara);
			if (stTopDevicePara.nWndUnitNum == 0)
			{
				AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
				return 0;
			}

			nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
			if (theTEIndex.GetCurBondUnit() == nLastIndex)
			{
				for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
				{
					for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
					{
						nUnitNum = thePgm.GetUnitNum(i, k);
						nUnitTotalDie = thePgm.GetUnitTotalDie();

						nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
						for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
						{
							theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
						}
					}
				}
			}
		}
		break;
    }
	case MHS_R2R:
    {
        theDevice.GetTopRcd(&stTopDevicePara);

        if (stTopDevicePara.nHalfIndexOn == 0)
        {
            nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
            if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
            {
	            if(stTopDevicePara.nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}

                nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                if (theR2RTrack.GetCurrentBondUnit() == nLastIndex)
                {
                    for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                    {
                        for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                        {
                            nUnitNum = thePgm.GetUnitNum(i, k);
                            nUnitTotalDie = thePgm.GetUnitTotalDie();

                            nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                            for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                            {
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                            }
                        }
                    }
                }
            }
        }
		break;
    }
	case MHS_CU_ELITE:
    {
        theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

        if (stCuEliteDevicePara.nHalfIndexOn == 0)
        {
            nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
            if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
            {
			    if (stCuEliteDevicePara.nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.38, nWndUnitNum == 0"));
					return 0;
				}
                nLastIndex = stCuEliteDevicePara.nDeviceColmNum / stCuEliteDevicePara.nWndUnitNum + 1;
                if (theCuEliteTrack.GetCurrentBondUnit() == nLastIndex)
                {
                    for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                    {
                        for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                        {
                            nUnitNum = thePgm.GetUnitNum(i, k);
                            nUnitTotalDie = thePgm.GetUnitTotalDie();

                            nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                            for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                            {
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                            }
                        }
                    }
                }
            }
        }
		break;
    }
	}

    nStartRef = 0;
    nLastRef = theRef.GetTotalRefNum() - 1;
    
    GetPerformPrRef(m_nCurWire, &nStartRef, &nLastRef);

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Check PR"), nStartRef, nLastRef);

	if (nVisionAction == VISION_ACTION_ALL_UNIT)
	{
		nStartRef = 0;
		nLastRef = theRef.GetTotalRefNum() - 1;
	}
    else if (nVisionAction == VISION_ACTION_SINGLE_UNIT)
    {
		nStartRef = 0;
        nLastRef = thePgm.GetUnitTotalDie();
    }

    for (i = nStartRef; i <= nLastRef; i++)
    {
        if ((theMatrix.GetRcd(i)->nVisionStatus != VISION_SUCCESS) &&
            (theMatrix.GetRcd(i)->nVisionStatus != VISION_SKIP_DIE))
        {
            nStartRef = i;
            break;
        }
    }

    if (i > nLastRef) //All alignments already done
        return PERFORM_WIRE_BOND;

    for (i = nLastRef; i >= nStartRef; i--)
    {
        if ((theMatrix.GetRcd(i)->nVisionStatus == VISION_SUCCESS) ||
            (theMatrix.GetRcd(i)->nVisionStatus == VISION_SKIP_DIE))
            nLastRef -= 1;
        else
            break;
    }

    if (nLastRef < nStartRef)
        return PERFORM_WIRE_BOND;

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Perform PR"), nStartRef, nLastRef);

    theServo.GetTablePosition(&stCurTblPos);

    thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

	short nInc = 1;
    for (short j = nStartRef; j <= nLastRef; j++)
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
		{
		case MHS_TOP_LED: case MHS_R2R:
		case MHS_VLED: case MHS_VLED_ELITE:
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
	        theDevice.GetTopRcd(&stTopDevicePara);
			break;
		}
		case MHS_CU_ELITE:
		{
	        theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
			break;
		}
		}
		
		if (j == 0)  //lead PR
		{
			//for the high power 3rd index
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (stTopDevicePara.nHalfIndexOn > 0)
                    && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (stTopDevicePara.nHalfIndexOn > 0)
                    && (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (stTopDevicePara.nHalfIndexOn > 0)
                    && (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (stTopDevicePara.nHalfIndexOn > 0)       
                    && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (stTopDevicePara.nHalfIndexOn > 0)       
                    && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn)))
			{
				WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);	//v3.1.T90 edit
				if ((pstWire->nRefNum[0] == 0) || (pstWire->nRefNum[1] == 0))
					nInc = max(pstWire->nRefNum[0], pstWire->nRefNum[1]);
				else
					nInc = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);

				i = 0;
				j = nInc - 1;
				//nInc=nLastRef;
			}
            else
			{
				i = 0;
				nInc = nLastRef;
			}
		}
		else  //after lead PR
		{
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
			    || ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (stTopDevicePara.nHalfIndexOn > 0)       
                    && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (stTopDevicePara.nHalfIndexOn > 0)       
                    && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn)))
			{
				i = j;
				nInc = 1;
			}
			else
			{
				if (nStartRef == 0)
				{
				   i = nLastRef - j + 1;
				   nInc = -1;
				}
				else
				{
					i = j;
					nInc = 1;
				}
			}
		}

        theRef.GetRcd(&stCurRef, i);
        stCurRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
        stCurRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

		for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
		{
			stCurRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
			stCurRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
			stCurRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
			stCurRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
		}

        theMatrix.GetRcd(&stMatrix1, i);

        if(stMatrix1.nVisionStatus==VISION_SKIP_DIE)continue;	//V1.1.W250 add

        if (j < nLastRef)
        {
	        theRef.GetRcd(&stNextRef, i + nInc);
            stNextRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
            stNextRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

			for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
			{
				stNextRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
				stNextRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
				stNextRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
				stNextRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
			}
        }

        if (stMatrix1.nVisionStatus == VISION_SUCCESS)
        {
            continue;
        }

		theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[0].nCoaxRedLight, stCurRef.stVisionLightPara[0].bSideRedLight, stCurRef.stVisionLightPara[0].nCoaxBlueLight, stCurRef.stVisionLightPara[0].bSideBlueLight);

        lMoveDistX = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
        lMoveDistY = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());

		dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
		if ((lMoveDistX > 0) || (lMoveDistY > 0))
        {
            nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, bSlowMove);

            nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, bSlowMove);
            Sleep_x1ms(1);

            if (bSlowMove == true)
            {
                nLastProfileX = 4;
                nLastProfileY = 4;
            }

			//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
			theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
        }

        stCurTblPos = stCurRef.stVisionAlignPos[0];

		ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
								stCurRef.nRefNum,
								stCurRef.stVisionAlignPos[0].dx,
								stCurRef.stVisionAlignPos[0].dy);

		stPrPos = stCurRef.stVisionAlignPos[0];
        theVisionSys.EnableVideo(false);

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("start ref"), stCurRef.nRefNum);
		theServo.WaitTableStop();
        
		//pr srch delay
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
        if (dMoveDist > VISION_LONG_DIST)
			Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
        else if (dMoveDist > 100.0)
			Sleep_x1ms(pstPgm->nVisionMoveDelay);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

		theVisionSys.GrabImage();
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

        if (stCurRef.nVisionAlgorithm[0] == 1)
            theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

        if (((stCurRef.nRefType == REF_TYPE_LEAD) && (pstPgm->nPrAlignMode == 1)) || //Single point alignment without PR angle
            (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT))
        {
            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
				Sleep_x1ms(20);
				theVisionSys.UpdateLightingParameter(
									stNextRef.stVisionLightPara[0].nCoaxRedLight,
									stNextRef.stVisionLightPara[0].bSideRedLight,
									stNextRef.stVisionLightPara[0].nCoaxBlueLight,
									stNextRef.stVisionLightPara[0].bSideBlueLight);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_UpdateLightPara"));

                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
							stNextRef.nRefNum,
							stNextRef.stVisionAlignPos[0].dx,
							stNextRef.stVisionAlignPos[0].dy);
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSearchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSearchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), lVisionError);

            theMHS.SetThreadStop(false);

			BOOL bRedoPrEnable = FALSE;
			switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
			{
			case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				bRedoPrEnable = FALSE;
				break;
			}
			case MHS_VLED: case MHS_VLED_ELITE:
			{
                bRedoPrEnable = pstPgm->bPrRetry;
				break;
			}
			}

            if ((lVisionError != FOK) && bRedoPrEnable) 
			{
                lVisionError = RetryPR(&stPrPos, stCurRef.ulVisionAlignPatternID[0], stCurRef.nVisionAlgorithm[0], stCurRef.stVisionLightPara[0].nImageThreshold, nPrPassScore, &stSrchResult);
                
                if (lVisionError != FOK)
                {
                    lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITH_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0], MTN_ABS, MTN_WAIT);	//v3.1.T56 edit
					stPrPos = stCurRef.stVisionAlignPos[0];
					stCurTblPos = stCurRef.stVisionAlignPos[0];

                    theVisionSys.SetCameraExposureTime(pstPgm->nRetryExposureTime);

                    lVisionError = RetryPR(&stPrPos, stCurRef.ulVisionAlignPatternID[0], stCurRef.nVisionAlgorithm[0], stCurRef.stVisionLightPara[0].nImageThreshold, nPrPassScore, &stSrchResult);

                    theVisionSys.SetCameraExposureTime(pstPgm->nExposureTime);
                }
            }

			if (lVisionError == FOK)
			{
				m_nSerialSkipNum=0;		//V1.1.W265 add

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrResult"),
					stCurRef.nRefNum, stSrchResult.dOffsetX,
					stSrchResult.dOffsetY, stSrchResult.dAngle);

                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/Offset"),
					stCurRef.nRefNum, stVisionOffset.dx, stVisionOffset.dy);

                stMatrix1.nVisionStatus = VISION_SUCCESS;
                m_nAutoSkipDieCount = 0;

				stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
				stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
				stMatrix1.stVisionAlignPos[1].dx = stMatrix1.stVisionAlignPos[0].dx + stCurRef.stVisionAlignPos[1].dx - stCurRef.stVisionAlignPos[0].dx;
				stMatrix1.stVisionAlignPos[1].dy = stMatrix1.stVisionAlignPos[0].dy + stCurRef.stVisionAlignPos[1].dy - stCurRef.stVisionAlignPos[0].dy;

				if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
				{
					stMatrix1.dCosAln = cos(stSrchResult.dAngle);
					stMatrix1.dSinAln = sin(stSrchResult.dAngle);
				}

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/MatPos"),
							stCurRef.nRefNum,
							stMatrix1.stVisionAlignPos[0].dx,
							stMatrix1.stVisionAlignPos[0].dy);

                theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
				
				//check current Die overlap
				if ((j > 0) && (stCurRef.nRefType == REF_TYPE_DIE)
					&& isCheckDieOverlap(i,thePgm.GetDeviceSeq()))
					return QUIT_AUTO_BOND;
			}

			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, 
												&stMatrix1, i, j, nLastRef, nLastProfileX, 
												nLastProfileY, nSearchRange, nPrPassScore);

			//PR failed, show error or skip die/unit
            if (lVisionError != FOK)
			{
				if ((m_bRedoPR == FALSE) && thePgm.IsAutoSkipDie() 
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))	//V1.1.W265 edit
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					m_nSerialSkipNum++;		//V1.1.W265 add
					m_nSkipNumPerIndex++;	//V1.1.W262 add
					theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit

					if (CheckSerialSkipNumLimit(pstPgm) == FALSE)
						return QUIT_AUTO_BOND;		//V1.1.W265 add
				}
				else if ((m_bRedoPR == FALSE) && thePgm.IsAutoSkipUnit() 
							&& (stCurRef.nRefType == REF_TYPE_DIE)
							&& (lVisionError == VISION_ERROR_NO_PATTERN))	//V1.1.W265 edit
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					m_nSerialSkipNum++;		//V1.1.W265 add
					m_nSkipNumPerIndex++;	//V1.1.W262 add
					theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit

					if (CheckSerialSkipNumLimit(pstPgm) == FALSE)
						return QUIT_AUTO_BOND;		//V1.1.W265 add
				}
				else
				{
					if (pstPgm->nAutoSkipDieUnit == 0)		//v3.0.T323 add
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);
					}

					theStat.SetMachineState(MCSTATE_STANDBY);

					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
	                
					theServo.SetServoControlParameter();

					if ((pstPgm->nAutoSkipDieUnit == 1)
						&& (theMcConfig.GetMhsType() == MHS_FOXCONN))
						lStopSelected = SHOW_MSG_RESULT_SKIP;	//v3.0.T323 add
					else
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);

					theMHS.SetOxidationProtect(false);
	                
					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else if (m_bRedoPR == FALSE)	//V1.1.W265 edit
							{
								if (pstPgm->nManualSkip == 0)
									theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);	//V1.1.W262 add
								else
									SetSkipUnit(i);

								m_nSerialSkipNum++;
								m_nSkipNumPerIndex++;
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);

								if (CheckSerialSkipNumLimit(pstPgm) == FALSE)
									return QUIT_AUTO_BOND;
							}
						}
					}

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_OK)
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
					else
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}

					if ((m_nBondMode != BONDMODE_IDLE) 
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);
	                
					theServo.GetTablePosition(&stCurTblPos);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
		}
        else if ((stCurRef.nRefType == REF_TYPE_DIE) && (pstPgm->bDiePrMultiSrch == true)) //Two point alignment with multi search
		{
            double dRefAlignDist, dMatrixAlignDist, dAlignTol;

            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
            }

            nSearchRange = pstPgm->nDieSearchRange;
            nPrPassScore = pstPgm->nPrPassScore;
			
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
            theMHS.SetThreadStop(false);
            if (lVisionError == FOK)
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

                lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSearchRange, nPrPassScore, &stSrchResult);
                if (lVisionError == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

                    stMatrix1.nVisionStatus = VISION_SUCCESS;
                    m_nAutoSkipDieCount = 0;

                    stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                    stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

                    dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0],
												stCurRef.stVisionAlignPos[1]);
                    dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0],
												stMatrix1.stVisionAlignPos[1]);
                    if (stCurRef.nRefType == REF_TYPE_LEAD)
                        dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                    else
                        dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                    if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                        lVisionError = FNOK;
                    else
                        theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
                }
            }

			if (lVisionError != FOK && stCurRef.nBakUpIndex > 0) 
			{
				int iTryTimes;

				for (iTryTimes = 0; iTryTimes < MAX_BAK_UP_PR_NUM; iTryTimes++)
				{
					if (stCurRef.stBakUpPr[iTryTimes].bValid == true)
					{
						theVisionSys.UpdateLightingParameter(
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxBlueLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dx)
							* theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dy)
							* theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep_x1ms(1);
                        }

						theServo.FastMoveTable2(stCurRef.stBakUpPr[iTryTimes].stAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos,
							stCurRef.stBakUpPr[iTryTimes].stAlignPos[0])
							* theServo.GetResolXY();
						stPrPos = stCurRef.stBakUpPr[iTryTimes].stAlignPos[0];

						theServo.WaitTableStop();

						if (dMoveDist > VISION_LONG_DIST)
							Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
						else
							Sleep_x1ms(pstPgm->nVisionMoveDelay);

						theVisionSys.EnableVideo(false);
						theVisionSys.GrabImage();

						if (stCurRef.stBakUpPr[iTryTimes].nAlgo[0] == 0)
							lVisionError = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[0],
								nSearchRange, nPrPassScore, &stSrchResult, iTryTimes);

                        if (lVisionError == FOK)
                        {
                            theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                            stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
                            stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;

							theVisionSys.UpdateLightingParameter(
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxBlueLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideBlueLight);
							theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

                            lVisionError = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[1],
								nSearchRange, nPrPassScore, &stSrchResult, iTryTimes);
                            if (lVisionError == FOK)
                            {
                                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                                
                                stMatrix1.nVisionStatus = VISION_SUCCESS;
                                m_nAutoSkipDieCount = 0;

                                stMatrix1.stVisionAlignPos[1].dx = stPrPos.dx + stVisionOffset.dx;
                                stMatrix1.stVisionAlignPos[1].dy = stPrPos.dy + stVisionOffset.dy;

                                dRefAlignDist = Calc2PtDist(
											stCurRef.stBakUpPr[iTryTimes].stAlignPos[0],
											stCurRef.stBakUpPr[iTryTimes].stAlignPos[1]);
                                dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0],
																stMatrix1.stVisionAlignPos[1]);
                                if (stCurRef.nRefType == REF_TYPE_LEAD)
                                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                                else
                                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                                    lVisionError = FNOK;
                                else
                                   theMatrix.CalcMatrix(&stMatrix1, i, iTryTimes+2);
                            }
                        }
					}

					if (lVisionError == FOK)
					{
						IcremBkPrCounts(iTryTimes);
						break;
					}
				}
				
				if (lVisionError == FOK)
				{
					//Pre move to next point
					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep_x1ms(1);
                        }

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}
                }
			}

            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);

					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					theStat.SetMachineState(MCSTATE_STANDBY);

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					if (!thePgm.IsAutoSkipDie() && !thePgm.IsAutoSkipUnit())
					{
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}
					else
					{
						if ((i == 0) || (m_nAutoSkipDieCount > 4))
						{
							theServo.SetServoControlParameter();
							lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						}
						else
						{
							lStopSelected = SHOW_MSG_RESULT_SKIP;
							m_nAutoSkipDieCount++;
						}
					}

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
						}
					}

					theMHS.SetOxidationProtect(false);
					if (lStopSelected == 1)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix1.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
						}
						else
						{
							SetSkipUnit(i);
						}

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
        }
        else if (pstPgm->nPrAlignMode == 0) //Two point alignment
		{
            //Pre move to next point
            if (pstPgm->nVisionPreMove == 1)
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSearchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSearchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), lVisionError);
            theMHS.SetThreadStop(false);
			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos,
											&stMatrix1, i, j, nLastRef, nLastProfileX,
											nLastProfileY, nSearchRange, nPrPassScore);

            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}
	                
					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);
					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
	                
					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
            }
            else
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;
            }

            if (pstPgm->nVisionPreMove == 0)
            {
				theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[1].nCoaxRedLight, stCurRef.stVisionLightPara[1].bSideRedLight, stCurRef.stVisionLightPara[1].nCoaxBlueLight, stCurRef.stVisionLightPara[1].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

			theServo.WaitTableStop();

			//pr srch delay
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
            if (dMoveDist > VISION_LONG_DIST)
                Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
            else
                Sleep_x1ms(pstPgm->nVisionMoveDelay);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

            theVisionSys.EnableVideo(false);
            theVisionSys.GrabImage();
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

            if (stCurRef.nVisionAlgorithm[1] == 1)
                theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveNextAlignPos1"));

                dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSearchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSearchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat2"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat2"), lVisionError);
			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(1, pstPgm, &stCurRef, &stNextRef, &stCurTblPos,
												&stMatrix1, i, j, nLastRef, nLastProfileX,
												nLastProfileY, nSearchRange, nPrPassScore);

            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[1];
	                
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
	                
					theMHS.SetOxidationProtect(false);
					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix1.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
						}
						else
						{
							SetSkipUnit(i);
						}

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter();
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
            }
            else
			{
                double dRefAlignDist, dMatrixAlignDist, dAlignTol;
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[1], &stSrchResult, &stVisionOffset);

                stMatrix1.nVisionStatus = VISION_SUCCESS;
                stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[1].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[1].dy + stVisionOffset.dy;

                dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0], stCurRef.stVisionAlignPos[1]);
                dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0], stMatrix1.stVisionAlignPos[1]);
                if (stCurRef.nRefType == REF_TYPE_LEAD)
                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                else
                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					if (thePgm.IsAutoSkipDie()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
					{
						theMatrix.SetVisionStatus(stCurRef.nRefNum, VISION_SKIP_DIE);

						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else if (thePgm.IsAutoSkipUnit()
							&& (stCurRef.nRefType == REF_TYPE_DIE))
					{
						SetSkipUnit(i);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);

						stMatrix1.nVisionStatus = VISION_NOT_DONE;

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}

						theMHS.SetOxidationProtect(true);
						theServo.SetServoControlParameter();
						sList.RemoveAll();
						sList.AddTail(theMsg.GetMsg(stCurRef.nRefType == REF_TYPE_LEAD ? MSG_LEAD_TOL_FAILURE : MSG_DIE_TOL_FAILURE));
						sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						theMHS.SetOxidationProtect(false);

						if (lStopSelected == SHOW_MSG_RESULT_STOP)
						{
							theVisionSys.EnableVideo(true);
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableIdleLight();
							return QUIT_AUTO_BOND;
						}
						else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
						{
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableBondingLight();

							ProcessKeyboard();	//v2.0.T38 add
							theServo.WaitTableStop();

							if (pstPgm->nManualSkip == 0)
							{
								stMatrix1.nVisionStatus = VISION_SKIP_DIE;
								theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
							}
							else
							{
								SetSkipUnit(i);
							}

							if (m_nBondMode == BONDMODE_STOP)
								return QUIT_AUTO_BOND;

							theServo.SetServoControlParameter();
						}
						else
						{
							if (isManualAlignmentStop(i))
								return QUIT_AUTO_BOND;
						}

						theServo.GetTablePosition(&stCurTblPos);

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
							theStat.SetMachineState(MCSTATE_PRODUCTION);

						if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
						{
							lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileX)
							{
								nLastProfileX = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
								Sleep_x1ms(1);
							}

							lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileY)
							{
								nLastProfileY = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
								Sleep_x1ms(1);
							}

							//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
							theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
							dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
							stCurTblPos = stNextRef.stVisionAlignPos[0];
						}

						continue;
					}
                }
                else
                    theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
            }

			//PR failed, show error or skip die/unit
            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
					theStat.SetMachineState(MCSTATE_STANDBY);
					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
				}
            }
        }

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end ref"), stCurRef.nRefNum);
    }

	if (m_bIll) thePrAlign.PerformInteliLeadLocate(true);

    nAheadAlignDone = CheckAheadAlignDone(&m_nCurWire);
    if (nAheadAlignDone == 0)
        return PERFORM_VISION_ALIGN;
    else if (nAheadAlignDone == 2)
	{
		if (thePgm.IsAutoSkipDie())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef)
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG, sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		else if (thePgm.IsAutoSkipUnit())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef / thePgm.GetUnitTotalDie())
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG, sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
        return COMPLETE_UNIT_UPDATE;
	}

	return PERFORM_WIRE_BOND;
}

// Perform vision alignment before bonding
int CBond::PerformVisionAlignment(bool bSlowMove, EN_VISION_ACTION nVisionAction)
{
	switch (thePgm.GetDeviceSeq())
	{
	case PGM_DIE_AHEAD:
		if (theMcConfig.IsMultiPR())
			return DieAheadAlign2(bSlowMove, nVisionAction);	//v3.1.T490 add
		else
			return DieAheadAlign(bSlowMove, nVisionAction);		//V1.4.W20 add
	case PGM_MAT_DIE_AHEAD:
		if (theMcConfig.IsMultiPR())
			return MatrixDieAheadAlign2(bSlowMove, nVisionAction);	//v3.1.T491 add
		else
			return MatrixDieAheadAlign(bSlowMove, nVisionAction);
	case PGM_MATRIX:
		return MatrixAlign(bSlowMove, nVisionAction);	//V3.0.T279 add
	}

	try
	{
    short i, k, nStartRef, nLastRef, nAheadAlignDone, nSearchRange, nPrPassScore;
    int lVisionError, lStopSelected;
    double dMoveDist = 0.0;
    CString str;
    CStringList sList;
    TBL_POS stVisionOffset, stStepRepeatOffset, stCurTblPos, stPrPos;
    REF_RECORD stCurRef, stNextRef;
    MATRIX_RECORD stMatrix1, stMatrix2;
    PR_SRCH_RESULT stSrchResult;
	TOP_DEVICE_RECORD stTopDevicePara; 
    CUELITE_DEVICE_RECORD stCuEliteDevicePara;
    short nLastProfileX = 0, nLastProfileY = 0, nMoveProfile;
    long  lMoveDist, lMoveDistX, lMoveDistY;
    short nLastIndexWndColumn, nLastIndex, nUnitNum, nUnitTotalDie, nUnitBaseRef;
    PGM_RECORD *pstPgm = thePgm.GetRcd();

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("PerformVisionAlignment"));

	theVisionDisplay.EnableDisplay(false);

    if (m_bBondNoVision && (nVisionAction == VISION_ACTION_AUTO))
        return PERFORM_WIRE_BOND;

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_TOP_LED: case MHS_VLED: case MHS_VLED_ELITE:
    {
        theDevice.GetTopRcd(&stTopDevicePara);

        if (stTopDevicePara.nHalfIndexOn == 0)
        {
            nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
            if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
            {

	            if(stTopDevicePara.nWndUnitNum == 0) //throw "w.temp.37" ;
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}
                nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                if (theTopTrack.GetCurBondUnit() == nLastIndex)
                {
                    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
                        //|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						//|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie();

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                    else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                }
            }
        }
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
        theDevice.GetTopRcd(&stTopDevicePara);
        if (stTopDevicePara.nHalfIndexOn == 0)
        {
            nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
            if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
            {
	            if (stTopDevicePara.nWndUnitNum == 0) //throw "w.temp.37" ;
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}
                nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                if (theTopTrack.GetCurBondUnit() == nLastIndex)
                {
                    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
                        //|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						//|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie();

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                    else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                }
            }
        }
		break;
	}
	case MHS_R2R:
    {
        theDevice.GetTopRcd(&stTopDevicePara);

        if (stTopDevicePara.nHalfIndexOn == 0)
        {
            nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
            if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
            {

	            if(stTopDevicePara.nWndUnitNum == 0) //throw "w.temp.37" ;
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}
                nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                if (theR2RTrack.GetCurrentBondUnit() == nLastIndex)
                {
                    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie();

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                    else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                }
            }
        }
		break;
    }
	case MHS_CU_ELITE:
    {
        theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

        if (stCuEliteDevicePara.nHalfIndexOn == 0)
        {
            nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
            if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
            {
			    if(stCuEliteDevicePara.nWndUnitNum == 0) //throw "w.temp.38" ;
				{
					AfxMessageBox(_T("temp.38, nWndUnitNum == 0"));
					return 0;
				}
                nLastIndex = stCuEliteDevicePara.nDeviceColmNum / stCuEliteDevicePara.nWndUnitNum + 1;
                if (theCuEliteTrack.GetCurrentBondUnit() == nLastIndex)
                {
                    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie();

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                    else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
                    {
                        for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
                        {
                            for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
                            {
                                nUnitNum = thePgm.GetUnitNum(i, k);
                                nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;

                                nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie;
                                for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
                                {
									theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
                                }
                            }
                        }
                    }
                }
            }
        }
		break;
    }
	}

    nStartRef = 0;
    nLastRef = theRef.GetTotalRefNum() - 1;
    
    GetPerformPrRef(m_nCurWire, &nStartRef, &nLastRef);

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Check PR"), nStartRef, nLastRef);

	if (nVisionAction == VISION_ACTION_ALL_UNIT)
	{
		nStartRef = 0;
		nLastRef = theRef.GetTotalRefNum() - 1;
	}
    else if (nVisionAction == VISION_ACTION_SINGLE_UNIT)
    {
		nStartRef = 0;
        nLastRef = thePgm.GetUnitTotalDie();
    }

    for (i = nStartRef; i <= nLastRef; i++)
    {
        theMatrix.GetRcd(&stMatrix2, i);
        if ((stMatrix2.nVisionStatus != VISION_SUCCESS) &&
            (stMatrix2.nVisionStatus != VISION_SKIP_DIE))
        {
            nStartRef = i;
            break;
        }
    }

    if (i > nLastRef) //All alignments already done
        return PERFORM_WIRE_BOND;

    for (i = nLastRef; i >= nStartRef; i--)
    {
        theMatrix.GetRcd(&stMatrix2, i);
        if ((stMatrix2.nVisionStatus == VISION_SUCCESS) ||
            (stMatrix2.nVisionStatus == VISION_SKIP_DIE))
            nLastRef -= 1;
        else
            break;
    }

    if (nLastRef < nStartRef) return PERFORM_WIRE_BOND;

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Perform PR"), nStartRef, nLastRef);

    //theScanKeyThread.StopThread();

    theServo.GetTablePosition(&stCurTblPos);

    if (thePgm.GetDeviceSeq() == PGM_MATRIX)
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, m_nCurRow, m_nCurCol, &stStepRepeatOffset);
    else
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

	short nInc = 1;
    for (short j = nStartRef; j <= nLastRef; j++)
	{
        //theMHS.SetThreadStop(true);
		//Default Reverse
		if (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		{
			i = nLastRef + nStartRef - j;
			nInc = -1;
		}
		else if ((thePgm.GetDeviceSeq() == PGM_HYBRID) 
				|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
		{
			switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
			{
			case MHS_TOP_LED: case MHS_R2R:
			case MHS_VLED: case MHS_VLED_ELITE:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{					
		        theDevice.GetTopRcd(&stTopDevicePara);
				break;
			}
			case MHS_CU_ELITE:
			{	
		        theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
				break;
			}
			}
			
			if (j == 0)  //lead PR
			{
				//for the high power 3rd index
				if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
						&& (stTopDevicePara.nHalfIndexOn > 0)
                        && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				    || ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
						&& (stTopDevicePara.nHalfIndexOn > 0)
                        && (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				    || ((theMcConfig.GetMhsType() == MHS_R2R)
						&& (stTopDevicePara.nHalfIndexOn > 0)
                        && (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
					|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
						&& (stCuEliteDevicePara.nHalfIndexOn > 0)
						&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
					|| ((theMcConfig.GetMhsType() == MHS_VLED) 
						&& (stTopDevicePara.nHalfIndexOn > 0)       
                        && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
					|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
						&& (stTopDevicePara.nHalfIndexOn > 0)       
                        && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn)))
				{
					WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);	//v3.1.T90 edit
					if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
					{
						if ((pstWire->nRefNum[0] == 0) || (pstWire->nRefNum[1] == 0))
							nInc = max(pstWire->nRefNum[0], pstWire->nRefNum[1]);
						else
							nInc = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);
					}

					i = 0;
					j = nInc - 1;
					//nInc=nLastRef;
				}
                else
    			{
    				i = 0;
    				nInc = nLastRef;
    			}
			}
			else  //after lead PR
			{
				if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
						&& (stTopDevicePara.nHalfIndexOn > 0)
						&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				    || ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
						&& (stTopDevicePara.nHalfIndexOn > 0)
						&& (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				    || ((theMcConfig.GetMhsType() == MHS_R2R)
						&& (stTopDevicePara.nHalfIndexOn > 0)
						&& (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
					|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
						&& (stCuEliteDevicePara.nHalfIndexOn > 0)
						&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
					|| ((theMcConfig.GetMhsType() == MHS_VLED) 
						&& (stTopDevicePara.nHalfIndexOn > 0)       
                        && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
					|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
						&& (stTopDevicePara.nHalfIndexOn > 0)       
                        && (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn)))
				{
					i = j;
					nInc = 1;
				}
    			else
    			{
					if (nStartRef == 0)
    				{
    				   i = nLastRef - j + 1;
    				   nInc = -1;
    				}
    				else
    				{
    					i = j;
    					nInc = 1;
					}
				}
			}
		}
		else  //for matrix or other sequence
		{
			i = j;
			nInc = 1;
		}

		//offline debug
		//if ((j < 10) && (theApp.IsOffline()))
		//{
		//	str.Format(_T("j/i/nInc = %d/%d/%d"), j , i, nInc);
		//	::AfxMessageBox(str);
		//}

        theRef.GetRcd(&stCurRef, i);
        stCurRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
        stCurRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

		for(k = 0; k < MAX_BAK_UP_PR_NUM; k++)
		{
			stCurRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
			stCurRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
			stCurRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
			stCurRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
		}

        theMatrix.GetRcd(&stMatrix1, i);

        if(stMatrix1.nVisionStatus==VISION_SKIP_DIE)continue;	//V1.1.W250 add

        if (j < nLastRef)
        {
	        theRef.GetRcd(&stNextRef, i + nInc);
            stNextRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
            stNextRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

			for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
			{
				stNextRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
				stNextRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
				stNextRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
				stNextRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
			}
        }

        if (stMatrix1.nVisionStatus == VISION_SUCCESS)
        {
            continue;
        }

		theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[0].nCoaxRedLight, stCurRef.stVisionLightPara[0].bSideRedLight, stCurRef.stVisionLightPara[0].nCoaxBlueLight, stCurRef.stVisionLightPara[0].bSideBlueLight);

		//if ((i == nStartRef) || (pstPgm->nVisionPreMove == 0))
        {
            lMoveDistX = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
            lMoveDistY = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());

			dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
			if ((lMoveDistX > 0) || (lMoveDistY > 0))
			//if (dMoveDist > 100.0)
            {
                nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
                theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, bSlowMove);

                //lMoveDistY = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
                theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, bSlowMove);
                Sleep_x1ms(1);

                if (bSlowMove == true)
                {
                    nLastProfileX = 4;
                    nLastProfileY = 4;
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
            }

            stCurTblPos = stCurRef.stVisionAlignPos[0];

			ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
								stCurRef.nRefNum,
								stCurRef.stVisionAlignPos[0].dx,
								stCurRef.stVisionAlignPos[0].dy);
        }

		stPrPos = stCurRef.stVisionAlignPos[0];
        theVisionSys.EnableVideo(false);

		//theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[0].nCoaxRedLight, stCurRef.stVisionLightPara[0].bSideRedLight, stCurRef.stVisionLightPara[0].nCoaxBlueLight, stCurRef.stVisionLightPara[0].bSideBlueLight);
        //theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[0]);

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("start ref"), stCurRef.nRefNum);
		theServo.WaitTableStop();
        
		//pr srch delay
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
        if (dMoveDist > VISION_LONG_DIST)
			Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
        else if (dMoveDist > 100.0)
			Sleep_x1ms(pstPgm->nVisionMoveDelay);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

		theVisionSys.GrabImage();
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

        if (stCurRef.nVisionAlgorithm[0] == 1)
            theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

        if (((stCurRef.nRefType == REF_TYPE_LEAD) && (pstPgm->nPrAlignMode == 1)) || //Single point alignment without PR angle
            (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT))
        {
            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
				Sleep_x1ms(20);
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_UpdateLightPara"));

                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
				//stCurTblPos = stNextRef.stVisionAlignPos[0];

				//theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
                //theVisionSys.ChangeLensMag(stNextRef.nVisionAlignMag[0]);

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
							stNextRef.nRefNum,
							stNextRef.stVisionAlignPos[0].dx,
							stNextRef.stVisionAlignPos[0].dy);
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSearchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSearchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), lVisionError);

            theMHS.SetThreadStop(false);

			BOOL bRedoPrEnable = FALSE;
			switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
			{
			case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				bRedoPrEnable = FALSE;
				break;
			}
			case MHS_VLED: case MHS_VLED_ELITE:
			{
                bRedoPrEnable = pstPgm->bPrRetry;
				break;
			}
			}

            if ((lVisionError != FOK) && bRedoPrEnable) 
			{
                lVisionError = RetryPR(&stPrPos,
                                       stCurRef.ulVisionAlignPatternID[0],
                                       stCurRef.nVisionAlgorithm[0],
                                       stCurRef.stVisionLightPara[0].nImageThreshold,
                                       nPrPassScore,
                                       &stSrchResult);
                
                if (lVisionError != FOK)
                {
                    lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITH_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0], MTN_ABS, MTN_WAIT);	//v3.1.T56 edit
					stPrPos = stCurRef.stVisionAlignPos[0];
					stCurTblPos = stCurRef.stVisionAlignPos[0];

                    theVisionSys.SetCameraExposureTime(pstPgm->nRetryExposureTime);

                    lVisionError = RetryPR(&stPrPos,
                                           stCurRef.ulVisionAlignPatternID[0],
                                           stCurRef.nVisionAlgorithm[0],
                                           stCurRef.stVisionLightPara[0].nImageThreshold,
                                           nPrPassScore,
                                           &stSrchResult);
                    
                    theVisionSys.SetCameraExposureTime(pstPgm->nExposureTime);
                }
            }

			if (lVisionError == FOK)
			{
				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrResult"),
						stCurRef.nRefNum, stSrchResult.dOffsetX,
						stSrchResult.dOffsetY, stSrchResult.dAngle);

                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/Offset"),
					stCurRef.nRefNum, stVisionOffset.dx, stVisionOffset.dy);

                stMatrix1.nVisionStatus = VISION_SUCCESS;
                m_nAutoSkipDieCount = 0;

				//stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
				//stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;
				stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
				stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
				stMatrix1.stVisionAlignPos[1].dx = stMatrix1.stVisionAlignPos[0].dx + stCurRef.stVisionAlignPos[1].dx - stCurRef.stVisionAlignPos[0].dx;
				stMatrix1.stVisionAlignPos[1].dy = stMatrix1.stVisionAlignPos[0].dy + stCurRef.stVisionAlignPos[1].dy - stCurRef.stVisionAlignPos[0].dy;

				if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
				{
					stMatrix1.dCosAln = cos(stSrchResult.dAngle);
					stMatrix1.dSinAln = sin(stSrchResult.dAngle);
				}

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/MatPos"),
							stCurRef.nRefNum,
							stMatrix1.stVisionAlignPos[0].dx,
							stMatrix1.stVisionAlignPos[0].dy);

                theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
				
				//check current Die overlap
				if ((j > 0) && (stCurRef.nRefType == REF_TYPE_DIE)
					&& isCheckDieOverlap(i, thePgm.GetDeviceSeq()))
					return QUIT_AUTO_BOND;
			}

			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, &stMatrix1, i, j, nLastRef, nLastProfileX, nLastProfileY, nSearchRange, nPrPassScore);

			//do backup PR 3 times
			/*if (lVisionError != FOK && stCurRef.nBakUpIndex >0)
			{
				int iTryTimes;
				for (iTryTimes = 0; iTryTimes < MAX_BAK_UP_PR_NUM; iTryTimes++)
				{
					if (stCurRef.stBakUpPr[iTryTimes].bValid == true)
					{
                        lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep(1);
                        }

                        theServo.FastMoveTable(&stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						dMoveDist = Calc2PtDist(&stCurTblPos, &stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0];
						stPrPos = stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0];

						theVisionSys.UpdateLightingParameter(stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].nCoaxRedLight, stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].bSideRedLight, stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].nCoaxBlueLight, stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].bSideBlueLight);
						theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[0]);

						theServo.SyncMotion(MOTOR_X);
						theServo.SyncMotion(MOTOR_Y);
						if (dMoveDist > VISION_LONG_DIST)
							Sleep(pstPgm->nVisionLongMoveDelay);
						else
							Sleep(pstPgm->nVisionMoveDelay);

						theVisionSys.EnableVideo(false);
						theVisionSys.GrabImage();

						if(stCurRef.stBakUpPr[iTryTimes].nBkupVisionAlgorithm[0] == 0)
						{
							theLog.WriteLine(_T("PR-Search1.2"));
							lVisionError = theVisionSys.SearchVisionPattern(stCurRef.stBakUpPr[iTryTimes].ulVisionBkupPatternID[0], nSearchRange, nPrPassScore, &stSrchResult, iTryTimes);
						}
					}

					if(lVisionError == FOK)
					{
						IcremBkPrCounts(iTryTimes);
						break;
					}
				}
				
				if (lVisionError == FOK)
				{
					//Pre move to next point
					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
                        lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep(1);
                        }

                        theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						dMoveDist   = Calc2PtDist(&stCurTblPos, &stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                        theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
                        theVisionSys.ChangeLensMag(stNextRef.nVisionAlignMag[0]);
						//stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

					stMatrix1.nVisionStatus = VISION_SUCCESS;
                    m_nAutoSkipDieCount = 0;
					
					stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
					stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
					stMatrix1.stVisionAlignPos[1].dx = stMatrix1.stVisionAlignPos[0].dx + stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[1].dx - stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0].dx;
					stMatrix1.stVisionAlignPos[1].dy = stMatrix1.stVisionAlignPos[0].dy + stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[1].dy - stCurRef.stBakUpPr[iTryTimes].stVisionBkupAlignPos[0].dy;

					if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
					{
						stMatrix1.dCosAln = cos(stSrchResult.dAngle);
						stMatrix1.dSinAln = sin(stSrchResult.dAngle);
					}

					theMatrix.CalcMatrix(&stMatrix1, i, iTryTimes+2);
				}
			}*/

			//PR failed, show error or skip die/unit
            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					lVisionError == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if(m_bRedoPR==FALSE)theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE && 
					lVisionError == VISION_ERROR_NO_PATTERN)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if(m_bRedoPR==FALSE)theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

				theStat.SetMachineState(MCSTATE_STANDBY);

				theMHS.SetOxidationProtect(true);

				PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

                //sList.AddTail(str);

                sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

				//if (!thePgm.IsAutoSkipDie() && !thePgm.IsAutoSkipUnit())
    //            {
                    theServo.SetServoControlParameter();
                    lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
    //            }
    //            else
    //            {
    //                if (((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD) ||
    //                     (thePgm.GetDeviceSeq() == PGM_HYBRID) ||
    //                     (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID) ||
    //                     (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)) &&
    //                    ((i == 0) || (m_nAutoSkipDieCount > 4)))
    //                {
    //                    theServo.SetServoControlParameter();
    //                    lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
    //                }
    //                else
    //                {
    //                    lStopSelected = SHOW_MSG_RESULT_SKIP;
    //                    m_nAutoSkipDieCount++;
    //                }
    //            }

				theMHS.SetOxidationProtect(false);
                
				if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                {
                    if (lStopSelected != SHOW_MSG_RESULT_SKIP)
                    {
                        if (stCurRef.nRefType == REF_TYPE_LEAD)
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                        else
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
                    }
                    else
                    {
                        if (stCurRef.nRefType == REF_TYPE_LEAD)
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                        else
						{
							if (pstPgm->nManualSkip == 0)
							{
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
							}
							else
							{
								SetSkipUnit(i);
							}
						}
                    }
                }

                if (lStopSelected == SHOW_MSG_RESULT_STOP)
                {
                    theVisionSys.EnableVideo(true);
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableIdleLight();
                    return QUIT_AUTO_BOND;
                }
                else if (lStopSelected == SHOW_MSG_RESULT_OK)
                {
					if (isManualAlignmentStop(i))
                        return QUIT_AUTO_BOND;
                }
                else
                {
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableBondingLight();

					ProcessKeyboard();	//v2.0.T38 add
					theServo.WaitTableStop();

                    stMatrix1.nVisionStatus = VISION_SKIP_DIE;
                    theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

                    if (m_nBondMode == BONDMODE_STOP)
                        return QUIT_AUTO_BOND;

                    theServo.SetServoControlParameter(false);
                }

                if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                    theStat.SetMachineState(MCSTATE_PRODUCTION);
                
                theServo.GetTablePosition(&stCurTblPos);

                if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
                {
					theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                    lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                    //stCurTblPos = stNextRef.stVisionAlignPos[0];
                    //theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
                    //theVisionSys.ChangeLensMag(stNextRef.nVisionAlignMag[0]);
                }

                continue;
				}
			}
		}

//---------------------------------------------------------------------------
        else if ((stCurRef.nRefType == REF_TYPE_DIE) && (pstPgm->bDiePrMultiSrch == true)) //Two point alignment with multi search
			//||(thePgm.GetDeviceSeq()==PGM_DIE_AHEAD)||(thePgm.GetDeviceSeq()==PGM_DIE_LEAD_AHEAD))
		{
            double dRefAlignDist, dMatrixAlignDist, dAlignTol;

            //Pre move to next point
            if ((j < nLastRef) && pstPgm->nVisionPreMove == 1)
            {
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                //stCurTblPos = stNextRef.stVisionAlignPos[0];
                //theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
                //theVisionSys.ChangeLensMag(stNextRef.nVisionAlignMag[0]);
            }

            nSearchRange = pstPgm->nDieSearchRange;
            nPrPassScore = pstPgm->nPrPassScore;
			
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
            theMHS.SetThreadStop(false);
            if (lVisionError == FOK)
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

                lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSearchRange, nPrPassScore, &stSrchResult);
                if (lVisionError == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

                    stMatrix1.nVisionStatus = VISION_SUCCESS;
                    m_nAutoSkipDieCount = 0;

                    stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                    stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

                    dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0], stCurRef.stVisionAlignPos[1]);
                    dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0], stMatrix1.stVisionAlignPos[1]);
                    if (stCurRef.nRefType == REF_TYPE_LEAD)
                        dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                    else
                        dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                    if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                        lVisionError = FNOK;
                    else
                        theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
                }
            }

			if (lVisionError != FOK && stCurRef.nBakUpIndex > 0) 
			{
				int iTryTimes;

				for (iTryTimes = 0; iTryTimes < MAX_BAK_UP_PR_NUM; iTryTimes++)
				{
					if (stCurRef.stBakUpPr[iTryTimes].bValid == true)
					{
						theVisionSys.UpdateLightingParameter(
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxBlueLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dx)
							* theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dy)
							* theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep_x1ms(1);
                        }

						theServo.FastMoveTable2(stCurRef.stBakUpPr[iTryTimes].stAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos,
							stCurRef.stBakUpPr[iTryTimes].stAlignPos[0])
							* theServo.GetResolXY();
						stPrPos = stCurRef.stBakUpPr[iTryTimes].stAlignPos[0];

						//theVisionSys.UpdateLightingParameter(stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].nCoaxRedLight, stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].bSideRedLight, stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].nCoaxBlueLight, stCurRef.stBakUpPr[iTryTimes].stVisionBkupLightPara[0].bSideBlueLight);
						//theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[0]);

						theServo.WaitTableStop();

						if (dMoveDist > VISION_LONG_DIST)
							Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
						else
							Sleep_x1ms(pstPgm->nVisionMoveDelay);

						theVisionSys.EnableVideo(false);
						theVisionSys.GrabImage();

						if (stCurRef.stBakUpPr[iTryTimes].nAlgo[0] == 0)
							lVisionError = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[0],
								nSearchRange, nPrPassScore, &stSrchResult, iTryTimes);

                        if (lVisionError == FOK)
                        {
                            theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                            stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
                            stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;

							theVisionSys.UpdateLightingParameter(
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxBlueLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideBlueLight);
							theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

                            lVisionError = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[1],
								nSearchRange, nPrPassScore, &stSrchResult, iTryTimes);
                            if (lVisionError == FOK)
                            {
                                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                                
                                stMatrix1.nVisionStatus = VISION_SUCCESS;
                                m_nAutoSkipDieCount = 0;

                                stMatrix1.stVisionAlignPos[1].dx = stPrPos.dx + stVisionOffset.dx;
                                stMatrix1.stVisionAlignPos[1].dy = stPrPos.dy + stVisionOffset.dy;

                                dRefAlignDist = Calc2PtDist(
										stCurRef.stBakUpPr[iTryTimes].stAlignPos[0],
										stCurRef.stBakUpPr[iTryTimes].stAlignPos[1]);
                                dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0],
																stMatrix1.stVisionAlignPos[1]);
                                if (stCurRef.nRefType == REF_TYPE_LEAD)
                                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                                else
                                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                                    lVisionError = FNOK;
                                else
                                   theMatrix.CalcMatrix(&stMatrix1, i, iTryTimes+2);
                            }
                        }
					}

					if (lVisionError == FOK)
					{
						IcremBkPrCounts(iTryTimes);
						break;
					}
				}
				
				if (lVisionError == FOK)
				{
					//Pre move to next point
					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep_x1ms(1);
                        }

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                        //theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
                        //theVisionSys.ChangeLensMag(stNextRef.nVisionAlignMag[0]);
						//stCurTblPos = stNextRef.stVisionAlignPos[0];
					}
                }
			}

            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					lVisionError == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);

					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

                theStat.SetMachineState(MCSTATE_STANDBY);

                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
				stCurTblPos = stCurRef.stVisionAlignPos[0];
				theMHS.SetOxidationProtect(true);

				PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

                sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

				if (!thePgm.IsAutoSkipDie() && !thePgm.IsAutoSkipUnit())
                {
                    theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
                }
				else
				{
					if (((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
						&& ((i == 0) || (m_nAutoSkipDieCount > 4)))
					{
                        theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}
					else
                    {
						lStopSelected = SHOW_MSG_RESULT_SKIP;
                        m_nAutoSkipDieCount++;
                    }
				}

                if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                {
                    if (lStopSelected != SHOW_MSG_RESULT_SKIP)
                    {
                        if (stCurRef.nRefType == REF_TYPE_LEAD)
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                        else
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
                    }
                    else
                    {
                        if (stCurRef.nRefType == REF_TYPE_LEAD)
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                        else
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
                    }
                }

                theMHS.SetOxidationProtect(false);
                if (lStopSelected == 1)
                {
                    theVisionSys.EnableVideo(true);
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableIdleLight();
                    return QUIT_AUTO_BOND;
                }
				else if(lStopSelected == SHOW_MSG_RESULT_SKIP)
				{
					theTowerLight.EnableBlinkRedLight(false);
					theTowerLight.EnableBondingLight();

					ProcessKeyboard();	//v2.0.T38 add
					theServo.WaitTableStop();

					//stMatrix1.nVisionStatus = VISION_SKIP_DIE;
					//theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

					if (pstPgm->nManualSkip == 0)
					{
						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
					}
					else
					{
						SetSkipUnit(i);
					}

					if (m_nBondMode == BONDMODE_STOP)
						return QUIT_AUTO_BOND;

                    theServo.SetServoControlParameter(false);
				}
				else
				{
					if (isManualAlignmentStop(i))
                        return QUIT_AUTO_BOND;
				}

				theServo.GetTablePosition(&stCurTblPos);

				if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					theStat.SetMachineState(MCSTATE_PRODUCTION);

                if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
                {
					theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                    lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                    //stCurTblPos = stNextRef.stVisionAlignPos[0];
                    //theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
                    //theVisionSys.ChangeLensMag(stNextRef.nVisionAlignMag[0]);
                }

                continue;
				}
			}
        }
        else if ((pstPgm->nPrAlignMode == 0) //Two point alignment
					|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD) 
					|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD))
		{
            //Pre move to next point
            if (pstPgm->nVisionPreMove == 1)
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSearchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSearchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), lVisionError);
            theMHS.SetThreadStop(false);
			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, &stMatrix1, i, j, nLastRef, nLastProfileX, nLastProfileY, nSearchRange, nPrPassScore);

            /*if ((lVisionError != FOK) && (stCurRef.bVisionBkupLight[0] == true))
			{
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep(1);
                }

                theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
                dMoveDist   = Calc2PtDist(&stCurTblPos, &stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[0];

                theVisionSys.UpdateLightingParameter(stCurRef.stVisionBkupLightPara[0].nCoaxRedLight, stCurRef.stVisionBkupLightPara[0].bSideRedLight, stCurRef.stVisionBkupLightPara[0].nCoaxBlueLight, stCurRef.stVisionBkupLightPara[0].bSideBlueLight);
                theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[0]);

                theServo.SyncMotion(MOTOR_X);
                theServo.SyncMotion(MOTOR_Y);
                if (dMoveDist > VISION_LONG_DIST)
                    Sleep(pstPgm->nVisionLongMoveDelay);
                else
                    Sleep(pstPgm->nVisionMoveDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();
                if (stCurRef.nVisionAlgorithm[0] == 1)
                    theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

				theVisionSys.SetPassScore(stCurRef.ulVisionAlignPatternID[0], pstPgm->nAutoSkipScore);

				theLog.WriteLine(_T("PR-Search1.6"));
                lVisionError = theVisionSys.SearchVisionPattern(stCurRef.ulVisionAlignPatternID[0], nSearchRange, nPrPassScore, &stSrchResult);
                if (lVisionError == FOK)
                {
                    if (pstPgm->nVisionPreMove == 1)
                    {
                        lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep(1);
                        }

                        theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
                        dMoveDist   = Calc2PtDist(&stCurTblPos, &stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                        stCurTblPos = stCurRef.stVisionAlignPos[1];
                    }
                }
            }*/

            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					lVisionError == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

                if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                {
                    if (stCurRef.nRefType == REF_TYPE_LEAD)
                        theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                    else
                        theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
                }

				//if (m_nBondMode == BONDMODE_CORRECT)
			    //theStat.SetMachineState(MCSTATE_STANDBY);
                
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
				stCurTblPos = stCurRef.stVisionAlignPos[0];
				theMHS.SetOxidationProtect(true);

				PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

                sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
                theServo.SetServoControlParameter();
                lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
                theMHS.SetOxidationProtect(false);
                if (lStopSelected == SHOW_MSG_RESULT_STOP)
                {
                    theVisionSys.EnableVideo(true);
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableIdleLight();
                    return QUIT_AUTO_BOND;
                }
                else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
                {
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableBondingLight();

					ProcessKeyboard();	//v2.0.T38 add
					theServo.WaitTableStop();

                    stMatrix1.nVisionStatus = VISION_SKIP_DIE;
                    theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

                    if (m_nBondMode == BONDMODE_STOP)
                        return QUIT_AUTO_BOND;

                    theServo.SetServoControlParameter(false);
                }
                else
                {
					if (isManualAlignmentStop(i))
                        return QUIT_AUTO_BOND;
                }
                
                theServo.GetTablePosition(&stCurTblPos);

                if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                    theStat.SetMachineState(MCSTATE_PRODUCTION);

                if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
                {
					theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

                    lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stNextRef.stVisionAlignPos[0];
                }

                continue;
				}
            }
            else
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
                stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;
            }

            if (pstPgm->nVisionPreMove == 0)
            {
				theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[1].nCoaxRedLight, stCurRef.stVisionLightPara[1].bSideRedLight, stCurRef.stVisionLightPara[1].nCoaxBlueLight, stCurRef.stVisionLightPara[1].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

            //theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[1].nCoaxRedLight, stCurRef.stVisionLightPara[1].bSideRedLight, stCurRef.stVisionLightPara[1].nCoaxBlueLight, stCurRef.stVisionLightPara[1].bSideBlueLight);
            //theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

			theServo.WaitTableStop();

			//pr srch delay
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
            if (dMoveDist > VISION_LONG_DIST)
                Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
            else
                Sleep_x1ms(pstPgm->nVisionMoveDelay);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

            theVisionSys.EnableVideo(false);
            theVisionSys.GrabImage();
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

            if (stCurRef.nVisionAlgorithm[1] == 1)
                theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveNextAlignPos1"));

                dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                //stCurTblPos = stNextRef.stVisionAlignPos[0];
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSearchRange = pstPgm->nLeadSearchRange;
                nPrPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSearchRange = pstPgm->nDieSearchRange;
                nPrPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
            lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat2"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat2"), lVisionError);
			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(1, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, &stMatrix1, i, j, nLastRef, nLastProfileX, nLastProfileY, nSearchRange, nPrPassScore);

            /*if ((lVisionError != FOK) && (stCurRef.bVisionBkupLight[1] == true))
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep(1);
                }

                theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
                dMoveDist   = Calc2PtDist(&stCurTblPos, &stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];

                theVisionSys.UpdateLightingParameter(stCurRef.stVisionBkupLightPara[1].nCoaxRedLight, stCurRef.stVisionBkupLightPara[1].bSideRedLight, stCurRef.stVisionBkupLightPara[1].nCoaxBlueLight, stCurRef.stVisionBkupLightPara[1].bSideBlueLight);
                theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

                theServo.SyncMotion(MOTOR_X);
                theServo.SyncMotion(MOTOR_Y);
                if (dMoveDist > VISION_LONG_DIST)
                    Sleep(pstPgm->nVisionLongMoveDelay);
                else
                    Sleep(pstPgm->nVisionMoveDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();
                if (stCurRef.nVisionAlgorithm[1] == 1)
                    theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

				theVisionSys.SetPassScore(stCurRef.ulVisionAlignPatternID[1],pstPgm->nAutoSkipScore);
				theLog.WriteLine(_T("PR-Search2.4"));
                lVisionError = theVisionSys.SearchVisionPattern(stCurRef.ulVisionAlignPatternID[1], nSearchRange, nPrPassScore, &stSrchResult);
                if (lVisionError == FOK)
                {
                    //Pre move to next point
                    if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
                    {
                        lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep(1);
                        }

                        theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
                        dMoveDist   = Calc2PtDist(&stCurTblPos, &stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                        stCurTblPos = stNextRef.stVisionAlignPos[0];
                    }
                }
            }*/

            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					lVisionError == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

                if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                {
                    if (stCurRef.nRefType == REF_TYPE_LEAD)
                        theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                    else
                        theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
                }

				//if (m_nBondMode == BONDMODE_CORRECT)
			    //theStat.SetMachineState(MCSTATE_STANDBY);
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileX)
                {
                    nLastProfileX = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                if (nMoveProfile != nLastProfileY)
                {
                    nLastProfileY = nMoveProfile;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
				stCurTblPos = stCurRef.stVisionAlignPos[1];
                
				theMHS.SetOxidationProtect(true);

				PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

                sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

                theServo.SetServoControlParameter();
                lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
                
                theMHS.SetOxidationProtect(false);
                if (lStopSelected == SHOW_MSG_RESULT_STOP)
                {
                    theVisionSys.EnableVideo(true);
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableIdleLight();
                    return QUIT_AUTO_BOND;
                }
                else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
                {
					theTowerLight.EnableBlinkRedLight(false);
                    theTowerLight.EnableBondingLight();

					ProcessKeyboard();	//v2.0.T38 add
					theServo.WaitTableStop();

                    //stMatrix1.nVisionStatus = VISION_SKIP_DIE;
                    //theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

					if (pstPgm->nManualSkip == 0)
					{
						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
					}
					else
					{
						SetSkipUnit(i);
					}

                    if (m_nBondMode == BONDMODE_STOP)
                        return QUIT_AUTO_BOND;

                    theServo.SetServoControlParameter();
                }
                else
                {
					if (isManualAlignmentStop(i))
                        return QUIT_AUTO_BOND;
                }

                theServo.GetTablePosition(&stCurTblPos);

                if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                    theStat.SetMachineState(MCSTATE_PRODUCTION);

                if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
                {
                    lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stNextRef.stVisionAlignPos[0];
                }

                continue;
				}
            }
            else
			{
                double dRefAlignDist, dMatrixAlignDist, dAlignTol;
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[1], &stSrchResult, &stVisionOffset);

                stMatrix1.nVisionStatus = VISION_SUCCESS;
                stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[1].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[1].dy + stVisionOffset.dy;

                dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0], stCurRef.stVisionAlignPos[1]);
                dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0], stMatrix1.stVisionAlignPos[1]);
                if (stCurRef.nRefType == REF_TYPE_LEAD)
                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                else
                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
						lVisionError == VISION_ERROR_NO_PATTERN)
					{
						theMatrix.SetVisionStatus(stCurRef.nRefNum, VISION_SKIP_DIE);

						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
					{
						SetSkipUnit(i);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else
					{
                    //theTowerLight.EnableIdleLight();
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);

                    stMatrix1.nVisionStatus = VISION_NOT_DONE;

                    if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                    {
                        if (stCurRef.nRefType == REF_TYPE_LEAD)
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
                        else
                            theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
                    }

					//if (m_nBondMode == BONDMODE_CORRECT)
			        //theStat.SetMachineState(MCSTATE_STANDBY);

                    sList.RemoveAll();
                    if (stCurRef.nRefType == REF_TYPE_LEAD)
		                sList.AddTail(theMsg.GetMsg(MSG_LEAD_TOL_FAILURE));
                    else
			            sList.AddTail(theMsg.GetMsg(MSG_DIE_TOL_FAILURE));
                    sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					theMHS.SetOxidationProtect(true);

                    theServo.SetServoControlParameter();
                    lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);

                    theMHS.SetOxidationProtect(false);

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix1.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
						}
						else
						{
							SetSkipUnit(i);
						}

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter();
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

                    theServo.GetTablePosition(&stCurTblPos);

                    if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
                        theStat.SetMachineState(MCSTATE_PRODUCTION);

                    if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
                    {
                        lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileX)
                        {
                            nLastProfileX = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                        nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                        if (nMoveProfile != nLastProfileY)
                        {
                            nLastProfileY = nMoveProfile;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                            Sleep_x1ms(1);
                        }

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
                    }

                    continue;
					}
                }
                else
                    theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
            }

			//PR failed, show error or skip die/unit
            if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					lVisionError == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					theStat.SetMachineState(MCSTATE_STANDBY);

					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
				}
            }
        }

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end ref"), stCurRef.nRefNum);
    }

	if (m_bIll == TRUE) thePrAlign.PerformInteliLeadLocate(true);

    //theServo.SetServoControlParameter();

  //  if ((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD) ||
  //      (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD) ||
  //      (thePgm.GetDeviceSeq() == PGM_SINGLE) || 
		//(thePgm.GetDeviceSeq() == PGM_HYBRID))
  //  {

        nAheadAlignDone = CheckAheadAlignDone(&m_nCurWire);
        if (nAheadAlignDone == 0)
            return PERFORM_VISION_ALIGN;
        else if (nAheadAlignDone == 2)
		{
			if (thePgm.IsAutoSkipDie())
			{
				if (m_lTotalAutoSkipDieCount == nLastRef)
				{
					theServo.MoveBondHeadToSafePos();
					m_lTotalAutoSkipDieCount = 0;
					sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
					if (theMsg.ShowMsgX(ERROR_MSG,sList,0,0,1) == SHOW_MSG_RESULT_STOP)
						return QUIT_AUTO_BOND;
				}
				else
					m_lTotalAutoSkipDieCount = 0;
			}
			else if (thePgm.IsAutoSkipUnit())
			{
				if (m_lTotalAutoSkipDieCount == nLastRef / thePgm.GetUnitTotalDie())
				{
					theServo.MoveBondHeadToSafePos();
					m_lTotalAutoSkipDieCount = 0;
					sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
					if (theMsg.ShowMsgX(ERROR_MSG, sList,0,0,1) == SHOW_MSG_RESULT_STOP)
						return QUIT_AUTO_BOND;
				}
				else
					m_lTotalAutoSkipDieCount = 0;
			}
            return COMPLETE_UNIT_UPDATE;
		}
		return PERFORM_WIRE_BOND;
	}
	catch (const char* e)
	{
		::AfxMessageBox((CString)e,MB_OK);
	}
	catch (std::logic_error e)
	{
		::AfxMessageBox(_T(" / error "),MB_OK);
	}
	catch (...)
	{
		::AfxMessageBox(_T("where are you "),MB_OK);
	}

	return PERFORM_WIRE_BOND;
}

// Perform wire bonding both for single or auto
int CBond::PerformWireBond(void)	//v3.1.T50 clean
{
	short anMsgArray[1];
    bool bUnplugSuccess = false;
    short nBondStatus = FOK, nAheadAlignDone = 1;
    short nTotalWire = thePgm.GetTotalWire();

	//check vacuum
	if (theMcConfig.GetMhsType() == MHS_TOP_LED)	//v3.0.T156 add
	{
		TOP_TRACK_DATA stTrackData;
		theTopTrack.GetRcd(&stTrackData);
		if (stTrackData.bVacuumControl && (theTopClamp.CheckVacuumStatus() == FNOK)
			&& theMcConfig.IsCheckVacuum())	//v3.0.T338 add
		{
			theServo.SetServoControlParameter(); 
			anMsgArray[0] = MSG_VACUUM_PRESSURE_LOW;
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
			theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
			return QUIT_AUTO_BOND;
		}
	}
	else if	(theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add, v3.0.T419 edit
	{
		if (theTEIndex.GetRcd()->bVacuumControl && (theTopClamp.CheckVacuumStatus() == FNOK)
			&& theMcConfig.IsCheckVacuum())
		{
			theServo.SetServoControlParameter(); 
			anMsgArray[0] = MSG_VACUUM_PRESSURE_LOW;
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
			theTopClamp.SetVacuumOn(false);
			return QUIT_AUTO_BOND;
		}
	}

    theScanKeyThread.StopThread(); 

	const WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);
    if ((m_nBondType == BONDTYPE_SINGLE)
		&& (pstWire->nBumpType != BUMP_NONE)
		&& (m_nBumpBondOption == BUMPOPT_NORMAL))
        m_nBondType = BONDTYPE_RESTART;

	theServo.RunBuffer(0);

    if ((m_nBondMode == BONDMODE_AUTO)
		|| (m_nBondType == BONDTYPE_RESTART))
		theServo.RunBuffer(1);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_RunServoProgram"));

	 if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
		|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
		|| (theMcConfig.GetMhsType() == MHS_R2R)
		|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
	 {
		if (((pstWire->nBumpType == BUMP_SOB)
				|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T103 add
			&& (m_nBumpBondOption != BUMPOPT_WIRE_ONLY))
			MoveToWirePoint(m_nCurWire, SECOND_BOND, false, false);
		else
			MoveToWirePoint(m_nCurWire, FIRST_BOND, false, false);

		theServo.GetTablePosition(&m_stCurTblPos);
	}

	bool bStartBond = true;
	while (m_nCurWire <= m_nEndWire)
	{
		pstWire = theWire.GetRcd(m_nCurWire);
        if (((pstWire->nBumpType == BUMP_SOB)
			|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T88 add
			&& (m_nBumpBondOption != BUMPOPT_WIRE_ONLY))
            nBondStatus = BondOneBall(bStartBond);

		if (nBondStatus == BOND_NEFO_FAIL) break;	//v3.1.T580 add
		if (nBondStatus == BOND_WIRE_LENGTH_ERROR)
			return QUIT_AUTO_BOND;	//V1.1.W254 add

        if ((nBondStatus == FOK)
			&& (m_nBumpBondOption != BUMPOPT_BALL_ONLY))
            nBondStatus = BondOneWire(bStartBond);

		if (nBondStatus == BOND_NEFO_FAIL) break;	//v3.1.T580 add
		if (nBondStatus == BOND_WIRE_LENGTH_ERROR)
			return QUIT_AUTO_BOND;	//V1.1.W254 add

		//v3.1.T101 delete
		//if ((nBondStatus == FOK)
		//	&& ((pstWire->nBumpType == BUMP_BOS)
		//		|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T88 add
		//	&& (m_nBumpBondOption != BUMPOPT_WIRE_ONLY))
		//	nBondStatus = BondOneBall(bStartBond);

		//v3.1.T101 add
		if ((nBondStatus == FOK)
			&& (m_nBumpBondOption != BUMPOPT_WIRE_ONLY))
		{
			if (pstWire->nBumpType == BUMP_BOS)
				nBondStatus = BondOneBall(bStartBond);
			else if (pstWire->nBumpType == BUMP_BSB)
				nBondStatus = BondOneBall(bStartBond, BUMPSEQ_BALL2);
		}

		if (nBondStatus == BOND_NEFO_FAIL) break;	//v3.1.T580 add
		if (nBondStatus == BOND_WIRE_LENGTH_ERROR)
			return QUIT_AUTO_BOND;	//V1.1.W254 add
        
		bStartBond = false;	//after bond first wire

		if (m_nMinRef == 0) m_nMinRef = pstWire->nRefNum[0];	//V1.5.W145 add
		if (m_nMaxRef < pstWire->nRefNum[0])
			m_nMaxRef = pstWire->nRefNum[0];	//V1.5.W145 add

        if (m_nCurWire > m_nBondedWire)
            m_nBondedWire = m_nCurWire;		

        if (nBondStatus == FOK)
        {
			if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			{
				static int nRefresh = 0;
				if (nRefresh > 10)
				{
					theStepperIO.UpdateInputState(CANADDR_IO);
					m_ThermalSensorX.UpdateSensorStateEx();
					m_ThermalSensorY.UpdateSensorStateEx();

#if _SERVO_TYPE != 2	//v3.1.T268 add
					if (m_ThermalSensorX.GetSensorState() == SENSOR_OFF)
						nBondStatus = BOND_X_MOTOR_OVERHEAT;
					else if (m_ThermalSensorY.GetSensorState() == SENSOR_OFF)
					{
						if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
							|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
							|| (theMcConfig.GetMhsType() == MHS_R2R)
							|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
							nBondStatus = BOND_Y_MOTOR_OVERHEAT;
					}
#endif
					nRefresh = 0;
				}
				nRefresh++;
			}
			else
			{
				m_ThermalSensorX.UpdateSensorState();
				m_ThermalSensorY.UpdateSensorState();

#if _SERVO_TYPE != 2	//v3.1.T268 add
				if (m_ThermalSensorX.GetSensorState() == SENSOR_OFF)
					nBondStatus = BOND_X_MOTOR_OVERHEAT;
				else if (m_ThermalSensorY.GetSensorState() == SENSOR_OFF)
				{
					if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
						|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
						|| (theMcConfig.GetMhsType() == MHS_R2R)
						|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
						nBondStatus = BOND_Y_MOTOR_OVERHEAT;
				}
#endif
			}
        }

        if (nBondStatus)
        {
            if (m_nCurWire <= nTotalWire)
                m_nCurWire++;
            if (m_nRestartWire < m_nCurWire)
                m_nRestartWire = m_nCurWire;
            break;
        }
        else if (m_nBondMode == BONDMODE_AUTO)
        {
            if (m_nCurWire <= nTotalWire)
                m_nCurWire++;
            m_nRestartWire = m_nCurWire;
        }
        else
        {
			theTowerLight.EnableBlinkRedLight(false);
            theTowerLight.EnableIdleLight();
            
            if (m_nCurWire <= nTotalWire)
                m_nCurWire++;
            if (m_nRestartWire < m_nCurWire)
                m_nRestartWire = m_nCurWire;

            theServo.SetServoControlParameter();
            theServo.SetServoSpeedProfile(MOTOR_Z, 0);
			Sleep(200);
            break;
        }

        if (m_nCurWire <= nTotalWire)
		{			
            nAheadAlignDone = CheckAheadAlignDone(&m_nCurWire);
            if (nAheadAlignDone != 1)
            {
                if (m_nRestartWire < m_nCurWire)
                    m_nRestartWire = m_nCurWire;
                break;
            }
            else if (m_nBondType == BONDTYPE_NORMAL)
            {
                if ((thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
					&& (thePgm.GetRcd()->nPrAheadCol == 1)
					&& (thePgm.GetUnitTotalWire() == 1))
                {
                    nAheadAlignDone = 0;
                    break;
                }
            }
        }

		ShowDebugMsg(DEBUG_VISION_TIMING, _T("CheckAheadWire/AlignDone"), m_nCurWire, nAheadAlignDone);

		if (nBondStatus == AUTO_SAMPLING_DONE) break;
    }

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("WireBonded"));
    
    if (m_nCurWire > nTotalWire)
        m_nCurWire = nTotalWire;
    if (m_nRestartWire > nTotalWire)
        m_nRestartWire = nTotalWire;

    theServo.TriggerRestartWire(false);
    
    if (nBondStatus)
    {
        if ((nBondStatus == BOND_NEFO_FAIL)
			|| (nBondStatus == BOND_1ST_NONSTICK)
			|| (nBondStatus == BOND_2ND_NONSTICK)
			|| (nBondStatus == BOND_TAIL_SHORT))
            Sleep(30);
        else
            Sleep(200);
    }

    theServo.StopBuffer(0);
    theServo.StopBuffer(1);

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_StopBuffer"));

    theScanKeyThread.BeginThread();

    theServo.TriggerWireClampProfile(WCL_HOLD_WIRE_CLOSE);

    if (nBondStatus)
    {
        theServo.SetServoControlParameter();
        theServo.SetServoSpeedProfile(MOTOR_Z, 0);

        MoveBondHeadToFireLevel();

        if (thePgm.GetRcd()->bCapillaryAutoPlug
			&& ((nBondStatus == BOND_NEFO_FAIL)
				|| (nBondStatus == BOND_1ST_NONSTICK)
				|| (nBondStatus == BOND_2ND_NONSTICK)
				|| (nBondStatus == BOND_TAIL_SHORT)))
        {
            int iStatus = theDummyBond.CapillaryUnplug();
            if (iStatus == FOK)
            {
                bUnplugSuccess = true;
                nBondStatus = FOK;
            }
        }

        if (nBondStatus == BOND_WIRE_END)
            theTowerLight.EnableWarningLight(true);

        if (nBondStatus)
        {
			theTowerLight.EnableErrorLight();
			theTowerLight.EnableBlinkRedLight(true);

            ReportBondError(nBondStatus);

            if (nBondStatus == BOND_WIRE_END)
                theTowerLight.EnableWarningLight(false);

			theTowerLight.EnableBlinkRedLight(false);
            theTowerLight.EnableIdleLight();
        }
    }

    if (m_nBondMode != BONDMODE_AUTO)
    {
        MoveBondHeadToFireLevel();
        MoveToWirePoint(m_nCurWire, FIRST_BOND);
    }

    if (m_nBondMode == BONDMODE_BTOS)
    {
        theScanKeyThread.EnableJoyStick();
        UpdateBondTipOffset(m_nBondedWire);
        theScanKeyThread.DisableJoyStick();
    }

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("bond completed"));
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("bond completed"));

    if ((m_nBondMode != BONDMODE_AUTO) || nBondStatus
		|| (m_nEndWire < nTotalWire))
        return QUIT_AUTO_BOND;

	if (nAheadAlignDone == 0)
	{
		theServo.SetServoControlParameter();
		theServo.SetServoSpeedProfile(MOTOR_Z, 0);
		MoveBondHeadToFireLevel();	//v3.1.T482 add
		return START_AUTO_BOND;
	}

	if (bUnplugSuccess) return START_AUTO_BOND;

	return COMPLETE_UNIT_UPDATE;
}

// Update bonding status when complete bond one unit
int CBond::CompleteUnitUpdate(void)
{
	short i, nBondedUnit, nCapCount, nBumpBallCount = 0;
	short nTotalWire = thePgm.GetTotalWire();
	short nUnitTotalWire = thePgm.GetUnitTotalWire();

    for (i = 1; i <= nUnitTotalWire; i++)
    {
        if (theWire.GetRcd(i)->nBumpType != BUMP_NONE)
            nBumpBallCount++;
    }

	switch (theMcConfig.GetMhsType())
	{
	case MHS_TOP_LED:
	{
		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);

		if ((stTopDevicePara.nHalfIndexOn > 0)
			&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
				nBondedUnit = thePgm.GetTotalUnit() / 2;

			if (m_bDryRun == FALSE)
				nCapCount = ((nUnitTotalWire * 2) + nBumpBallCount) * nBondedUnit;
			else
				nCapCount = 0;
		}
		else
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
			{
                short nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                if (theTopTrack.GetCurBondUnit() == nLastIndex)
				{
					nBondedUnit = theDevice.GetLastIndexWndColumn() * thePgm.GetDeviceRowNum();		//v1.6.T371 add
					nTotalWire = nUnitTotalWire * nBondedUnit;	//v2.0.T108sc add
				}
				else
					nBondedUnit = thePgm.GetTotalUnit();
			}

			if (m_bDryRun == false)
                nCapCount = (nUnitTotalWire * 2 + nBumpBallCount) * nBondedUnit;
			else
				nCapCount = 0;
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);

		if ((stTopDevicePara.nHalfIndexOn > 0)
			&& (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
				nBondedUnit = thePgm.GetTotalUnit() / 2;

			if (m_bDryRun == FALSE)
				nCapCount = ((nUnitTotalWire * 2) + nBumpBallCount) * nBondedUnit;
			else
				nCapCount = 0;
		}
		else
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
			{
                short nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                if (theTEIndex.GetCurBondUnit() == nLastIndex)
				{
					nBondedUnit = theDevice.GetLastIndexWndColumn() * thePgm.GetDeviceRowNum();		//v1.6.T371 add
					nTotalWire = nUnitTotalWire * nBondedUnit;
				}
				else
					nBondedUnit = thePgm.GetTotalUnit();
			}

			if (m_bDryRun == false)
                nCapCount = (nUnitTotalWire * 2 + nBumpBallCount) * nBondedUnit;
			else
				nCapCount = 0;
		}
		break;
	}
	case MHS_R2R:
	{
		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);

		if ((stTopDevicePara.nHalfIndexOn > 0)
			&& (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
				nBondedUnit = thePgm.GetTotalUnit() / 2;

			if (m_bDryRun == false)
				nCapCount = (nUnitTotalWire * 2 + nBumpBallCount) * nBondedUnit;
			else
				nCapCount = 0;
		}
		else
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
				nBondedUnit = thePgm.GetTotalUnit();

			if (m_bDryRun == false)
                nCapCount = (nUnitTotalWire * 2 + nBumpBallCount) * nBondedUnit;
			else
				nCapCount = 0;
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		CUELITE_DEVICE_RECORD stCuEliteDevicePara;
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

		if ((stCuEliteDevicePara.nHalfIndexOn > 0)
			&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
				nBondedUnit = thePgm.GetTotalUnit() / 2;

			if (m_bDryRun == false)
				nCapCount = (nUnitTotalWire * 2 + nBumpBallCount) * nBondedUnit; 
			else
				nCapCount = 0;
		}
		else
		{
			if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
				nBondedUnit = 1;
			else
				nBondedUnit = thePgm.GetTotalUnit();

			if (m_bDryRun == false)
                nCapCount = (nUnitTotalWire * 2 + nBumpBallCount) * nBondedUnit;
			else
				nCapCount = 0;
		}
		break;
	}
	default:
	{
		if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
			|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
			nBondedUnit = 1;
		else
			nBondedUnit = thePgm.GetTotalUnit();

		if (m_bDryRun == false)
            nCapCount = (nUnitTotalWire * 2 + nBumpBallCount) * nBondedUnit;
		else
			nCapCount = 0;

		if (thePgm.GetRcd()->bBkPrAutoSwap == true)
		{
			short i;

			if( (thePgm.GetDeviceSeq() == PGM_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
			{
				for (i = 0; i < MAX_BAK_UP_PR_NUM; i++)
				{
					if (BkUpPrUseTimes[i] >= round(nBondedUnit * 0.5))
					{
						SwapBkUpPrToMain(1, i); 
						break;
					}
				}
			}
		}
	}
	}
    
    theStat.SetBondedUnit(nBondedUnit, nTotalWire, nCapCount);

	ResetBkPrCounts();

	if (m_bCompleteBond
		|| ((theMcConfig.GetAutoBondTimeout() > 0)
			&& (m_BondTimer.GetTime_x1min() > theMcConfig.GetAutoBondTimeout())))	//v1.6.T269 add, v1.6.T282 edit
	{
		if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
			|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
			|| (theMcConfig.GetMhsType() == MHS_R2R)
			|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
		{
			theServo.MoveTable2(theMcData.GetRcd()->stTbSafeStandby);	//v3.1.T52 edit
		}

        return QUIT_AUTO_BOND;
	}
    else
        return AUTO_BOND_INFORM_INDEX;
}

// Inform work holder to index a new lead frame
int CBond::AutoBondInformIndex(void)
{
	m_nCurWire = 1;
	m_nRestartWire = 1;
	m_nBondedWire = 0;
	ResetMinMaxRef();	//V1.5.W145 add
	m_bAutoDefineBond = false;

	short nCurUnit;

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_FOXCONN:
	{
		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
			if (nCurUnit < thePgm.GetTotalUnit())
			{
				nCurUnit++;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
			}
			else
			{
				nCurUnit = 1; 
				m_nCurRow = 1;
				m_nCurCol = 1;
				theServo.MoveTable2(theMcData.GetRcd()->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
				if (m_bAutoIndex) theMHS.SetRequestIndex(true);
			}
		}
		else 
		{
			nCurUnit = 1; 
			m_nCurRow = 1;
			m_nCurCol = 1;

            short nIndex, nTotalIndex, nStatus, nCurMatrix, nNextMatrix;

            if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
            {
                nCurMatrix = thePgm.GetMatNum(m_nCurMatRow, m_nCurMatCol);
                nIndex = theMHS.GetCurBondUnit();
                nStatus = thePgm.GetNextMatrix(nIndex, nCurMatrix, &nNextMatrix);
                if (nStatus == FOK)
                {
                    nCurMatrix = nNextMatrix;
                    thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
		        }
                else
                {
					nTotalIndex = 3;

                    if (m_bAutoIndex)
                    {
                        if (nIndex < nTotalIndex)
							nIndex++;
                        else
							nIndex = 1;
                    }

                    nStatus = thePgm.GetNextMatrix(nIndex, 0, &nNextMatrix);
                    if (nStatus == FOK)
                    {
                        nCurMatrix = nNextMatrix;

						if (nIndex == nTotalIndex)
						{
							int i = 0; short j = 0;
							do 
							{
								i++;
								nStatus = thePgm.GetNextMatrix(nIndex, i, &j);
							} while (nStatus == FOK);
						}

                        thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
                    }

					theServo.MoveTable2(theMcData.GetRcd()->stTbSafeStandby,
							MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit

					if (m_bAutoIndex)
					{
						m_nSkipNumPerIndex = 0;
            			theMHS.SetRequestIndex(true);
					}
            	}
            }
            else
            {
				theServo.MoveTable2(theMcData.GetRcd()->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
                if (m_bAutoIndex) theMHS.SetRequestIndex(true);
            }
        }
		break;
    }
	case MHS_TOP_LED:
	{
		const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
		MACHINE_DATA* pstMcData = theMcData.GetRcd();

		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
			if (nCurUnit < thePgm.GetTotalUnit())
			{
				nCurUnit++;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
			}
			else 
			{
				if ((pstDevice->nHalfIndexOn > 0) &&
					(theTopTrack.GetCurBondUnit() + 1 == pstDevice->nHalfIndexOn))
				{
					nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
					thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
				}
				else
				{
					nCurUnit = 1; 
					m_nCurRow = 1;
					m_nCurCol = 1;
				}

				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit

				if (m_bAutoIndex) theMHS.SetRequestIndex(true);
			}
		}
		else 
		{
			if ((pstDevice->nHalfIndexOn > 0)
				&& (theTopTrack.GetCurBondUnit() + 1 == pstDevice->nHalfIndexOn)
				&& (thePgm.GetDeviceSeq() != PGM_MAT_DIE_AHEAD)
				&& (thePgm.GetDeviceSeq() != PGM_MAT_HYBRID))
			{
				nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);

				m_nBondedWire = thePgm.GetUnitTotalWire() * (nCurUnit - 1);
				m_nCurWire = m_nBondedWire + 1;
				m_nRestartWire = m_nBondedWire + 1;

				ResetMatrixPrStatus(1);
			}
            else
			{
				 nCurUnit = 1; 
				 m_nCurRow = 1;
				 m_nCurCol = 1;
	        }

            short nIndex, nTotalIndex, nStatus, nCurMatrix, nNextMatrix;

            if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
            {
                nCurMatrix = thePgm.GetMatNum(m_nCurMatRow, m_nCurMatCol);
                nIndex = theMHS.GetCurBondUnit();
                nStatus = thePgm.GetNextMatrix(nIndex, nCurMatrix, &nNextMatrix);
                if (nStatus == FOK)
                {
                    nCurMatrix = nNextMatrix;
                    thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
		        }
                else
                {
                    TOP_DEVICE_RECORD stTopDevicePara;
                    theDevice.GetTopRcd(&stTopDevicePara);
                    if (stTopDevicePara.nDeviceColmNum % stTopDevicePara.nWndUnitNum != 0)
                    {
			            if (stTopDevicePara.nWndUnitNum == 0) //throw "w.temp.39" ;
						{
							AfxMessageBox(_T("temp.39, nWndUnitNum == -1"));
							return 0;
						}
                        nTotalIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                    }
                    else
                    {
			            if (stTopDevicePara.nWndUnitNum == 0) //throw "w.temp.40" ;
						{
							AfxMessageBox(_T("temp.40, nWndUnitNum == 0"));
							return 0;
						}
                        nTotalIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum;
                    }

                    if (m_bAutoIndex)
                    {
                        if (nIndex < nTotalIndex)
							nIndex++;
                        else
							nIndex = 1;
                    }

                    nStatus = thePgm.GetNextMatrix(nIndex, 0, &nNextMatrix);
                    if (nStatus == FOK)
                    {
                        nCurMatrix = nNextMatrix;

						if (nIndex == nTotalIndex)
						{
							int i = 0; short j = 0;
							do 
							{
								i++;
								nStatus = thePgm.GetNextMatrix(nIndex, i, &j);
							} while (nStatus == FOK);

							if ((stTopDevicePara.nHalfIndexOn > 0)
								&& (theTopTrack.GetCurBondUnit() + 1 == stTopDevicePara.nHalfIndexOn)
								&& (nCurMatrix == i)
								&& (thePgm.GetMatrixCol() % 2 == 0))
							{
								nCurMatrix = i/2 + 1;
							}
						}

                        thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
                    }

					theServo.MoveTable2(pstMcData->stTbSafeStandby,
							MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit

            		if (m_bAutoIndex)	//V1.1.W286 edit
					{
						m_nSkipNumPerIndex = 0;
            			theMHS.SetRequestIndex(true);
					}
            	}
            }
            else
            {
				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit

                if (m_bAutoIndex)
                    theMHS.SetRequestIndex(true);
            }
        }
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
		MACHINE_DATA* pstMcData = theMcData.GetRcd();

		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
			if (nCurUnit < thePgm.GetTotalUnit())
			{
				nCurUnit++;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
			}
			else 
			{
				if ((pstDevice->nHalfIndexOn > 0) &&
					(theTEIndex.GetCurBondUnit() + 1 == pstDevice->nHalfIndexOn))
				{
					nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
					thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
				}
				else
				{
					nCurUnit = 1; 
					m_nCurRow = 1;
					m_nCurCol = 1;
				}

				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
				if (m_bAutoIndex) theMHS.SetRequestIndex(true);
			}
		}
		else 
		{
			if ((pstDevice->nHalfIndexOn > 0)
				&& (theTEIndex.GetCurBondUnit() + 1 == pstDevice->nHalfIndexOn)
				&& (thePgm.GetDeviceSeq() != PGM_MAT_DIE_AHEAD)
				&& (thePgm.GetDeviceSeq() != PGM_MAT_HYBRID))
			{
				nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);

				m_nBondedWire = thePgm.GetUnitTotalWire() * (nCurUnit - 1);
				m_nCurWire = m_nBondedWire + 1;
				m_nRestartWire = m_nBondedWire + 1;

				ResetMatrixPrStatus(1);
			}
            else
			{
				 nCurUnit = 1; 
				 m_nCurRow = 1;
				 m_nCurCol = 1;
	        }

            short nIndex, nTotalIndex, nStatus, nCurMatrix, nNextMatrix;

            if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
            {
                nCurMatrix = thePgm.GetMatNum(m_nCurMatRow, m_nCurMatCol);
                nIndex = theMHS.GetCurBondUnit();
                nStatus = thePgm.GetNextMatrix(nIndex, nCurMatrix, &nNextMatrix);
                if (nStatus == FOK)
                {
                    nCurMatrix = nNextMatrix;
                    thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
		        }
                else
                {
                    TOP_DEVICE_RECORD stTopDevicePara;
                    theDevice.GetTopRcd(&stTopDevicePara);
                    if (stTopDevicePara.nDeviceColmNum % stTopDevicePara.nWndUnitNum != 0)
                    {
			            if (stTopDevicePara.nWndUnitNum == 0) //throw "w.temp.39" ;
						{
							AfxMessageBox(_T("temp.39, nWndUnitNum == -1"));
							return 0;
						}
                        nTotalIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                    }
                    else
                    {
			            if (stTopDevicePara.nWndUnitNum == 0) //throw "w.temp.40" ;
						{
							AfxMessageBox(_T("temp.40, nWndUnitNum == 0"));
							return 0;
						}
                        nTotalIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum;
                    }

                    if (m_bAutoIndex)
                    {
                        if (nIndex < nTotalIndex)
							nIndex++;
                        else
							nIndex = 1;
                    }

                    nStatus = thePgm.GetNextMatrix(nIndex, 0, &nNextMatrix);
                    if (nStatus == FOK)
                    {
                        nCurMatrix = nNextMatrix;

						if (nIndex == nTotalIndex)
						{
							int i = 0; short j = 0;
							do 
							{
								i++;
								nStatus = thePgm.GetNextMatrix(nIndex, i, &j);
							} while (nStatus == FOK);

							if ((stTopDevicePara.nHalfIndexOn > 0)
								&& (theTEIndex.GetCurBondUnit() + 1 == stTopDevicePara.nHalfIndexOn)
								&& (nCurMatrix == i)
								&& (thePgm.GetMatrixCol() % 2 == 0))
							{
								nCurMatrix = i/2 + 1;
							}
						}

                        thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
                    }

					theServo.MoveTable2(pstMcData->stTbSafeStandby,
							MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit

            		if (m_bAutoIndex)
					{
						m_nSkipNumPerIndex = 0;
            			theMHS.SetRequestIndex(true);
					}
            	}
            }
            else
            {
				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
                if (m_bAutoIndex) theMHS.SetRequestIndex(true);
            }
        }
		break;
    }
	case MHS_R2R:
	{
		TOP_DEVICE_RECORD  stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);

		MACHINE_DATA* pstMcData = theMcData.GetRcd();

		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
			if (nCurUnit < thePgm.GetTotalUnit())
			{
				nCurUnit++;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
			}
			else 
			{

				if(stTopDevicePara.nHalfIndexOn > 0 && 
					theR2RTrack.GetCurrentBondUnit() + 1 == stTopDevicePara.nHalfIndexOn)
				{
					nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
					thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
				}
				else
				{
					nCurUnit = 1; 
					m_nCurRow = 1;
					m_nCurCol = 1;
				}

				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
				if (m_bAutoIndex) theMHS.SetRequestIndex(true);
			}
		}
		else 
		{
			if(stTopDevicePara.nHalfIndexOn > 0 && 
				theR2RTrack.GetCurrentBondUnit() + 1 == stTopDevicePara.nHalfIndexOn)
			{
				nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);

				m_nBondedWire = thePgm.GetUnitTotalWire() * (nCurUnit - 1);
				m_nCurWire = m_nBondedWire + 1;
				m_nRestartWire = m_nBondedWire + 1;

				ResetMatrixPrStatus(1);
			}
            else
			{
				 nCurUnit = 1; 
				 m_nCurRow = 1;
				 m_nCurCol = 1;
	        }

            short nIndex, nTotalIndex, nStatus, nCurMatrix, nNextMatrix;

            if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
            {
                nCurMatrix = thePgm.GetMatNum(m_nCurMatRow, m_nCurMatCol);

                nIndex = theMHS.GetCurBondUnit();

                nStatus = thePgm.GetNextMatrix(nIndex, nCurMatrix, &nNextMatrix);

                //if (nCurMatrix < nTotalMatrix)
                if (nStatus == FOK)
                {
                    //nCurMatrix++;
                    //thePgm.GetRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
                    nCurMatrix = nNextMatrix;
                    thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
		        }
                else
                {
                    TOP_DEVICE_RECORD stTopDevicePara;
                    theDevice.GetTopRcd(&stTopDevicePara);

                    if(stTopDevicePara.nDeviceColmNum % stTopDevicePara.nWndUnitNum != 0)
                    {
			            if(stTopDevicePara.nWndUnitNum==0) //throw "w.temp.39" ;
						{
							AfxMessageBox(_T("temp.39, nWndUnitNum == 0"));
							return 0;
						}
                        nTotalIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
                    }
                    else
                    {
			            if(stTopDevicePara.nWndUnitNum==0) //throw "w.temp.40" ;
						{
							AfxMessageBox(_T("temp.40, nWndUnitNum == 0"));
							return 0;
						}
                        nTotalIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum;
                    }

                    if (m_bAutoIndex)
                    {
                        if (nIndex < nTotalIndex)
                            nIndex++;
                        else
                            nIndex = 1;
                    }

                    nStatus = thePgm.GetNextMatrix(nIndex, 0, &nNextMatrix);
                    if (nStatus == FOK)
                    {
                        nCurMatrix = nNextMatrix;
                        thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
                        //m_nCurMatRow = 1;
                        //m_nCurMatCol = 1;
                    }

					theServo.MoveTable2(pstMcData->stTbSafeStandby,
							MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
					if (m_bAutoIndex) theMHS.SetRequestIndex(true);
            	}
            }
            else
            {
				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
				if (m_bAutoIndex) theMHS.SetRequestIndex(true);
            }
        }
		break;
    }
	case MHS_CU_ELITE:
	{
		CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

		MACHINE_DATA* pstMcData = theMcData.GetRcd();

		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
			if (nCurUnit < thePgm.GetTotalUnit())
			{
				nCurUnit++;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
			}
			else 
			{
				if(stCuEliteDevicePara.nHalfIndexOn > 0 && 
					theCuEliteTrack.GetCurrentBondUnit() + 1 == stCuEliteDevicePara.nHalfIndexOn)
				{
					nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
					thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
				}
				else
				{
					nCurUnit = 1; 
					m_nCurRow =1;
					m_nCurCol=1;

				}

				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
				if (m_bAutoIndex) theMHS.SetRequestIndex(true);
			}
		}
		else //for other bonding sequence, like single,die ahead...
		{         
			if(stCuEliteDevicePara.nHalfIndexOn > 0 && 
				theCuEliteTrack.GetCurrentBondUnit() + 1 == stCuEliteDevicePara.nHalfIndexOn)
			{
				nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);

				m_nBondedWire = thePgm.GetUnitTotalWire() * (nCurUnit - 1);
				m_nCurWire = m_nBondedWire + 1;
				m_nRestartWire = m_nBondedWire + 1;

				ResetMatrixPrStatus(1);
			}
            else
			{
				 nCurUnit = 1; 
				 m_nCurRow =1;
				 m_nCurCol=1;
	        }

            short nIndex, nTotalIndex, nStatus, nCurMatrix, nNextMatrix;

            if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
            {
                nCurMatrix = thePgm.GetMatNum(m_nCurMatRow, m_nCurMatCol);

                nIndex = theMHS.GetCurBondUnit();

                nStatus = thePgm.GetNextMatrix(nIndex, nCurMatrix, &nNextMatrix);

                //if (nCurMatrix < nTotalMatrix)
                if (nStatus == FOK)
                {
                    //nCurMatrix++;
                    //thePgm.GetRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
                    nCurMatrix = nNextMatrix;
                    thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
		        }
                else
                {
                    CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
                    theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

                    if(stCuEliteDevicePara.nDeviceColmNum % stCuEliteDevicePara.nWndUnitNum != 0)
                    {
	                    if(stCuEliteDevicePara.nWndUnitNum==0) //throw "w.temp.41" ;
						{
							AfxMessageBox(_T("temp.41, nWndUnitNum == 0"));
							return 0;
						}
                        nTotalIndex = stCuEliteDevicePara.nDeviceColmNum / stCuEliteDevicePara.nWndUnitNum + 1;
                    }
                    else
                    {
                        if(stCuEliteDevicePara.nWndUnitNum==0) //throw "w.temp.42" ;
						{
							AfxMessageBox(_T("temp.42, nWndUnitNum == 0"));
							return 0;
						}
                        nTotalIndex = stCuEliteDevicePara.nDeviceColmNum / stCuEliteDevicePara.nWndUnitNum;
                    }

                    if (m_bAutoIndex)
                    {
                        if (nIndex < nTotalIndex)
                            nIndex++;
                        else
                            nIndex = 1;
                    }

                    nStatus = thePgm.GetNextMatrix(nIndex, 0, &nNextMatrix);
                    if (nStatus == FOK)
                    {
                        nCurMatrix = nNextMatrix;
                        thePgm.GetMatRowCol(nCurMatrix, &m_nCurMatRow, &m_nCurMatCol);
                        //m_nCurMatRow = 1;
                        //m_nCurMatCol = 1;
                    }

					theServo.MoveTable2(pstMcData->stTbSafeStandby,
							MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
					if (m_bAutoIndex) theMHS.SetRequestIndex(true);
            	}
            }
            else
            {
				theServo.MoveTable2(pstMcData->stTbSafeStandby,
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
				if (m_bAutoIndex) theMHS.SetRequestIndex(true);
            }
        }
		break;
    }
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
			if (nCurUnit < thePgm.GetTotalUnit())
			{
				nCurUnit++;
				thePgm.GetRowCol(nCurUnit, &m_nCurRow, &m_nCurCol);
			}
			else if (m_bAutoIndex)
			{
				theMHS.SetRequestIndex(true);
				//theStat.RegisterUnitEndTime();
				nCurUnit = 1;
				m_nCurRow =1;
				m_nCurCol=1;
			}
			else
			{
				nCurUnit = 1;
				m_nCurRow =1;
				m_nCurCol=1;
			}
		}
		else if (m_bAutoIndex)
		{
			theMHS.SetRequestIndex(true);
			//theStat.RegisterUnitEndTime();
			nCurUnit = 1; 
			m_nCurRow = 1;
			m_nCurCol = 1;
		}
		else
		{
			nCurUnit = 1;
			m_nCurRow = 1;
			m_nCurCol = 1;
		}
		break;
	}
	}

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("inform index"));

    return AUTO_BOND_AFTER_INDEX;
}

// Handle functions after index
int CBond::AutoBondAfterIndex(void)
{
    REF_RECORD  stRefRcd;
    TBL_POS     stStepRepeatOffset;
    bool        bIndexComplete = true;

    if (thePgm.GetDeviceSeq() == PGM_MATRIX)
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, m_nCurRow, m_nCurCol, &stStepRepeatOffset);
    else
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

    if (thePgm.IsLeadPrEnable() == true)
        theRef.GetRcd(&stRefRcd, 0);
    else
        theRef.GetRcd(&stRefRcd, 1);

    stRefRcd.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
    stRefRcd.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
    m_stCurTblPos = stRefRcd.stVisionAlignPos[0];

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
    		if (!(m_nCurRow == 1 && m_nCurCol == 1))
            {
				theServo.MoveTable2(stRefRcd.stVisionAlignPos[0],
						MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
            }
		}
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		theServo.MoveTable2(stRefRcd.stVisionAlignPos[0],
							MTN_ABS, MTN_NO_WAIT, MTN_FAST);	//v3.1.T52 edit
		break;
	}
	}
	    
    theVisionSys.UpdateLightingParameter(stRefRcd.stVisionLightPara[0].nCoaxRedLight, stRefRcd.stVisionLightPara[0].bSideRedLight, stRefRcd.stVisionLightPara[0].nCoaxBlueLight, stRefRcd.stVisionLightPara[0].bSideBlueLight);
    theVisionSys.ChangeLensMag(stRefRcd.nVisionAlignMag[0]);
	
	//set usg profile
	short nBumpSeq;
	WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);
	if (((pstWire->nBumpType == BUMP_SOB)
			|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T94 add
		&& (m_nBumpBondOption != BUMPOPT_WIRE_ONLY))
		nBumpSeq = BUMPSEQ_BALL;
	else
		nBumpSeq = BUMPSEQ_WIRE;
	m_nBondBuffer = 0;
	CalcUsgProfile(m_nCurWire, nBumpSeq);
	SetUsgProfile(m_nCurWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendUsgProf1(2)_Buf"), m_nBondBuffer);
	Sleep(10);
	SetUsgProfile(m_nCurWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendUsgProf2"));

	theServo.WaitTableStop();

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_WaitIndexComplete"));
    if (m_bAutoIndex)
        bIndexComplete = WaitIndexComplete();

	//theStat.RegisterUnitEndTime();

    if (bIndexComplete == false)
    {
        return QUIT_AUTO_BOND;
    }
    else
    {
        //m_nCurWire = 1;
        //m_nRestartWire = 1;

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_WaitIndexComplete"));

		SINGLE_CAP_TRACK_RCD stLfTrackRcd;
        theVledTrack.GetLfTrackRcd(&stLfTrackRcd);

		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			if ((m_nCurRow == 1) && (m_nCurCol == 1))
				theStat.SetUnitEndTime(stLfTrackRcd.nLeadFrameUnits);
		}
		else
			theStat.SetUnitEndTime(stLfTrackRcd.nLeadFrameUnits);

        return START_AUTO_BOND;
    }
}

// Wait track index complete
bool CBond::WaitIndexComplete()
{
    MSG msg;
    bool bIndexInProgress = true, bUpdateTableServo = false;
    int lElapseTime, lStartTime = ::GetCurrentTime();
    short nMhsErrStatus = FOK;

    if (theDebug.IsDebugWorkHolder()) theDebug.ClearInfo();	//v3.0.T96 edit

    while (bIndexInProgress)
	{
        bIndexInProgress = theMHS.IsRequestIndex();

		//avoid index overlap
		if (theMHS.ShowError() != FOK) nMhsErrStatus = FNOK;

		if (nMhsErrStatus != FOK) break;
        if (bIndexInProgress == false) break;

        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if ((msg.message == WM_KEYDOWN) || (msg.message == WM_KEYUP))
            {
                if (theMcConfig.GetKeyPadType() == 1)
                {
                    if (theKeyPad.HotKeyHandler(&msg) == TRUE) break;
                }
            }
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        lElapseTime = ::GetCurrentTime() - lStartTime;
        if (lElapseTime > TIMEOUT_5) break;

        if ((bUpdateTableServo == false) && (lElapseTime > 150))
        {
            theServo.SetServoControlParameter();
            bUpdateTableServo = true;
        }

        if (m_nBondMode == BONDMODE_STOP) break;

		switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
		{
		case MHS_CU_ELITE:
        {
            if (theCuEliteTrack.GetIndexPrManuAlignFlag() == INDEX_PR_ENTER_MANU_ALIGN)
		    {
			    theCuEliteTrack.PerformIndexPrprogress();
			    lStartTime = GetCurrentTime();
		    }
		    else if (theCuEliteTrack.GetIndexPrManuAlignFlag() == INDEX_PR_MANU_ALIGN_STOP)
		    {
			    //theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK);
			    break;
		    }
			break;
        }
		case MHS_R2R:
        {
		    if (theR2RTrack.GetIndexPrManuAlignFlag() == INDEX_PR_ENTER_MANU_ALIGN)
		    {
			    theR2RTrack.PerformIndexPrprogress();
			    lStartTime = GetCurrentTime();
		    }
		    else if (theR2RTrack.GetIndexPrManuAlignFlag() == INDEX_PR_MANU_ALIGN_STOP)
		    {
			    //theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK);
			    break;
		    }
			break;
        }
		case MHS_TOP_LED:
        {
		    if (theTopTrack.GetIndexPrManualAlign() == INDEX_PR_ENTER_MANU_ALIGN)
		    {
			    theTopTrack.PerformPrIndex();
			    lStartTime = GetCurrentTime();
		    }
		    else if (theTopTrack.GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_STOP)
		    {
			    //theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK);
			    break;
		    }
			break;
        }
		case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
        {
		    if (theTEIndex.GetIndexPrManualAlign() == INDEX_PR_ENTER_MANU_ALIGN)
		    {
			    theTEIndex.PerformPrIndex();
			    lStartTime = GetCurrentTime();
		    }
			break;
        }
		}

        Sleep(1);
    }
  
    if (m_nBondMode == BONDMODE_STOP)
	{
        m_nBondMode = BONDMODE_IDLE;
		return false;
	}

	if (nMhsErrStatus != FOK) return false;

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_CU_ELITE:
    {
        if (theCuEliteTrack.GetIndexPrManuAlignFlag() == INDEX_PR_MANU_ALIGN_STOP)
	    {
		    theCuEliteTrack.SetIndexPrManuAlignFlag(INDEX_PR_MANU_ALIGN_OK);
		    return false;
	    }
		break;
    }
	case MHS_R2R:
    {
	    if (theR2RTrack.GetIndexPrManuAlignFlag() == INDEX_PR_MANU_ALIGN_STOP)
	    {
		    theR2RTrack.SetIndexPrManuAlignFlag(INDEX_PR_MANU_ALIGN_OK);
		    return false;
	    }
		break;
    }
	case MHS_TOP_LED:
    {
	    if (theTopTrack.GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_STOP)
	    {
		    theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK);
		    return false;
	    }
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
	    if (theTEIndex.GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_STOP)
	    {
		    theTEIndex.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK);
		    return false;
	    }
		break;
    }
	}

    if (bIndexInProgress && (m_nBondMode != BONDMODE_IDLE))
	{
		CStringList sList;
        sList.AddTail(theMsg.GetMsg(MSG_INDEX_TIMEOUT));
        theMsg.ShowMsg(WARNING_MSG, sList);
	    return false;
	}
	else
    {
        if (bUpdateTableServo == true)
            theServo.SetServoControlParameter(false);
        
        return true;
    }
}

int CBond::BondOneWire(bool bStartBond)
{
	//for testing purpose, comment out after test, do not delete
	/*
	if (theMcConfig.IsBypassAll())
	{
		//test 1
		//SetErrNo(m_nCurWire);
		//return BOND_SERVO_ERROR;

		//test 2
		//SetErrBondProgress(m_nCurWire, 0);
		//return BOND_TIEMOUT;

		//test 3
		CString sMsg;
		//sMsg.Format(_T("W%d_R%d_C%d_U%d"), m_nCurWire,
		//					m_nCurRow, m_nCurCol,
		//					theMHS.GetCurBondUnit());
		//::AfxMessageBox(sMsg);

		short nCtactBufferIndex = thePgm.CalcCtactBufferIndex(
										m_nCurWire, m_nCurRow,
										m_nCurCol, 1);
		//nCtactBufferIndex = 1;
		int nCtactLvl = -6700 - (rand() / 100);
		sMsg.Format(_T("W%d_Ctact_%d"), nCtactBufferIndex, nCtactLvl);
		::AfxMessageBox(sMsg);
		int nStatus;
		nStatus = theCtact.AddRcd(nCtactBufferIndex, FIRST_BOND, nCtactLvl);
		nStatus = theCtact.AddRcd(nCtactBufferIndex, SECOND_BOND, nCtactLvl);

		return FOK;
	}
	*/

	if (theMcConfig.IsBypassAll()) return FOK;

	ShowDebugMsg(TIME_STAMP_BOND, _T("beg_BondOneWire_W"), m_nCurWire);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_BondOneWire_W"), m_nCurWire);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("beg_BondOneWire_W"), m_nCurWire);

	int nBondResult, nErr;
    CString sTemp;
	//v3.1.T105 delete
    //ST_WIRE_INFO stCurWireInfo, stNextWireInfo;
    //ST_BALL_INFO stNextBallInfo;
    
	const WIRE_RECORD *pstCurWire = theWire.GetRcd(m_nCurWire);

	//v3.1.T105 delete
    //stCurWireInfo.bScopeEnable = false;
    //stNextWireInfo.bScopeEnable = false;
    //stNextBallInfo.bScopeEnable = false;

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("BondType"), m_nBondType);	//v3.1.T505 add
	if ((m_nBondType == BONDTYPE_SINGLE)
		|| (m_nBondType == BONDTYPE_RESTART))
    {
        //theServo.GetTablePosition(&m_stCurTblPos);

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_CalcWireInfo"));
		//look for next non-skip wire
		ST_WIRE_INFO stCurWireInfo;		//v3.1.T105 add
		stCurWireInfo.bScopeEnable = false;	//v3.1.T105 add
		do
		{
			nErr = CalcWireInfo(m_nCurWire, m_stCurTblPos, &stCurWireInfo, sTemp, false);
			if (nErr == BOND_NO_ALIGNMENT_ERROR)
			{
				m_nCurWire++;
				if (m_nCurWire > m_nEndWire)
				{
					nBondResult = FOK;
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("LastWire"));
					goto QUIT;
				}
			}
			else if (nErr != BOND_SUCCESS)
			{
				nBondResult = nErr;
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcWireInfo_Err"), nBondResult);
				goto QUIT;
			}
		} while (nErr == BOND_NO_ALIGNMENT_ERROR);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_CalcWireInfo_W"), m_nCurWire);	//v3.1.T300 edit

        if (m_nBondType == BONDTYPE_SINGLE)
            stCurWireInfo.nNextWireExist = 0;

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("NextWireExist"), stCurWireInfo.nNextWireExist);

        if (theDebug.IsDebugWaveform()
			|| theMcConfig.IsDebugModeWaveform()	//v3.1.T462 add
			|| theDebug.IsDebugWireClamp()
			|| theDebug.IsDebugSpWaveform())
            stCurWireInfo.bScopeEnable = true;
        else
            stCurWireInfo.bScopeEnable = false;

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SendWireInfo_W"), m_nCurWire);	//v3.1.T300 edit
		//theApp.SetRedLamp(true);
		if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
		{
			CString str;
			str.Format(_T("SendCurW_%d_StartBond_%d"), m_nCurWire, bStartBond);
			theDebug.AddInfo(str);
		}
        nErr = SendWireInfoToServo(&stCurWireInfo, bStartBond, m_nCurWire);
		//theApp.SetRedLamp(false);
        if (nErr != FOK)
		{
			nBondResult = nErr;
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("SendWireInfo_Err"), nBondResult);
			goto QUIT;
		}
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendWireInfo"));

		if (!m_bStopBondReady)	//v3.1.T461 add
		{
			if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
				theDebug.AddInfo(_T("TrigRestart"));
			theServo.TriggerRestartWire(true);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerRestartWire"));
		}
    }

    if (thePgm.IsCopperSequence())
    {
		//nErr = CheckBondProgress(1);
        //if (nErr != FOK)
        //    return nErr;

		if (m_bPreFireEfoSuccess)
		{
			//not check efo first time if prefire success
			m_bPreFireEfoSuccess = false;
		}
		else
		{
			if ((nErr == FOK)
				&& (m_bDryRun == false) && m_bCheckFab)
			{
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_IsSparkSuccess_2"));
				if (theMcConfig.IsEfoErrIO())	//v3.1.T434 add
				{
					if (theServo.GetInput(0) == 0)	//efo fail
					{
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("IoErr"));	//v3.1.T471 add
						//if (theEFO.IsSparkSuccess() == false)	//v3.1.T500 delete
						{
							nBondResult = BOND_NEFO_FAIL;
							//ShowDebugMsg(DEBUG_BOND_TIMING, _T("SparkFail"));	//v3.1.T500 delete
							goto QUIT;
						}
					}
				}
				else
				{
					if (theEFO.IsSparkSuccess() == false)	//v1.6.T338 add
					{
						nBondResult = BOND_NEFO_FAIL;
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("SparkFail"));
						goto QUIT;
					}
				}
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_IsSparkSuccess_2"));
			}
		}
    }

	if (theDebug.IsDebugLfCycleTime())
	{
		if (m_bLfCycleStarted == false)
		{
			m_nLfCycleUnit = theTopTrack.GetCurBondUnit();
			m_nLfCycleWire = m_nCurWire;
			m_nLfCycleMatRow = m_nCurMatRow;
			m_nLfCycleMatCol = m_nCurMatCol;
			m_LfCycleTimer.StartTimer();
			m_bLfCycleStarted = true;
			m_dLfCyclePrevTime = 0;
			m_dLfCycleTime = 0;
		}
		else
		{
			double dCurTime;
			if ((theTopTrack.GetCurBondUnit() == m_nLfCycleUnit)
					&& (m_nCurWire == m_nLfCycleWire)
					&& (m_nCurMatRow == m_nLfCycleMatRow)
					&& (m_nCurMatCol == m_nLfCycleMatCol))
			{
				dCurTime = m_LfCycleTimer.GetTime_x1s();
				sTemp.Format(_T("I%d_R%d_C%d_W%d_%.3f_%.3fs"),
						m_nLfCycleUnit, m_nLfCycleMatRow,
						m_nLfCycleMatCol, m_nLfCycleWire,
						dCurTime, dCurTime - m_dLfCyclePrevTime);
				theDebug.AddInfo(sTemp);
				m_dLfCycleTime = dCurTime;		//for display in stat
				m_LfCycleTimer.StartTimer();	//reset timer for next cycle
				m_dLfCyclePrevTime = 0;
			}
			else if (m_nCurWire == m_nLfCycleWire)
			{
				dCurTime = m_LfCycleTimer.GetTime_x1s();
				sTemp.Format(_T("I%d_R%d_C%d_W%d_%.3f_%.3fs"),
						theTopTrack.GetCurBondUnit(),
						m_nCurMatRow, m_nCurMatCol, m_nCurWire,
						dCurTime, dCurTime - m_dLfCyclePrevTime);
				theDebug.AddInfo(sTemp);
				m_dLfCyclePrevTime = dCurTime;
			}
		}
	}
	m_WireCycleTimer.StartTimer();
	nErr = TriggerBonding(BUMPSEQ_WIRE);
	if (nErr != FOK)
	{
		nBondResult = nErr;
		//ShowDebugMsg(DEBUG_BOND_TIMING, _T("TriggerBonding_Err"), nBondResult);	//v3.1.T406 delete
		goto QUIT;
	}

	//die pr look ahead
	if (theMcConfig.GetDieAheadPR() != 2)
	{
		nErr = DieAheadPR();
		if (nErr != FOK)
		{
			SetErrNo(1);
			nBondResult = nErr;
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("DieAheadPR_Err"), nBondResult);
			goto QUIT;
		}
	}

	bool bNextWireExist = false;	//v3.1.T450 edit
	short nNextWire = m_nCurWire;	//v3.1.T100 edit
	/* v3.1.T450 delete
	if ((m_nBondType == BONDTYPE_SINGLE)
		|| ((pstCurWire->nBumpType == BUMP_SOB)
			&& (m_nBondMode != BONDMODE_AUTO)))
	{
        bNextWireExist = false;
	}
	else
	*/
	//v3.1.T450 add
	if (((m_nBondType == BONDTYPE_SINGLE)
		|| ((pstCurWire->nBumpType == BUMP_SOB)
			&& (m_nBondMode != BONDMODE_AUTO))) == false)
	{
		if ((pstCurWire->nBumpType == BUMP_BOS)
			|| (pstCurWire->nBumpType == BUMP_BSB))	//v3.1.T101 add
            bNextWireExist = true;
		else
		{
			//if (m_nCurWire >= thePgm.GetTotalWire())
			/* v3.1.T450 delete
			if (m_nCurWire >= m_nEndWire)
                bNextWireExist = false;
			else
			*/
			if (m_nCurWire < m_nEndWire)	//v3.1.T450 add
			{
				nNextWire = m_nCurWire + 1;
                bNextWireExist = true;
			}
		}
	}

	/* v3.1.T461 delete
    if (bNextWireExist && m_bRequestStopBond)
    {
        if (((pstCurWire->nBumpType != BUMP_BOS)
				&& (pstCurWire->nBumpType != BUMP_BSB))	//v3.1.T101 add
			|| (m_nBumpBondOption == BUMPOPT_WIRE_ONLY))
        {
            bNextWireExist = false;
            m_nBondMode = BONDMODE_STOP;
            m_bRequestStopBond = false;

			//v3.1.T449 add
			TriggerBonding(BUMPSEQ_WIRE);
			m_nStopCount++;
			CString str;
			str.Format(_T("RequestStop_%d_W_%d"), m_nStopCount, m_nCurWire), 
			theDebug.AddInfo(str);
        }
    }
	*/
	//v3.1.T461 add
    if (m_bStopBondReady)
    {
        if (((pstCurWire->nBumpType != BUMP_BOS)
				&& (pstCurWire->nBumpType != BUMP_BSB))	//v3.1.T101 add
			|| (m_nBumpBondOption == BUMPOPT_WIRE_ONLY))
        {
            m_bStopBondReady = false;
            bNextWireExist = false;
            m_nBondMode = BONDMODE_STOP;
        }
    }

	if (bNextWireExist)
    {
        if (CheckAheadAlignDone(&nNextWire) == 1)	//align done
		{
            if (((nNextWire == m_nCurWire)
					&& ((pstCurWire->nBumpType == BUMP_BOS)
						|| (pstCurWire->nBumpType == BUMP_BSB)))	//v3.1.T101 add
				|| (theWire.GetRcd(nNextWire)->nBumpType == BUMP_SOB)
				|| (theWire.GetRcd(nNextWire)->nBumpType == BUMP_BSB))	//v3.1.T135 add
            {
				//v3.1.T126 add
				short nBumpSeq = BUMPSEQ_BALL;
				if ((nNextWire == m_nCurWire)	//v3.1.T135 add
					&& (pstCurWire->nBumpType == BUMP_BSB))
					nBumpSeq = BUMPSEQ_BALL2;

				//look for next non-skip ball
				ST_BALL_INFO stNextBallInfo;			//v3.1.T105 add
				stNextBallInfo.bScopeEnable = false;	//v3.1.T105 add
				do
				{
					nErr = CalcBallInfo(nNextWire, m_stCurTblPos,
									&stNextBallInfo, sTemp, false,
									(EN_BUMP_SEQ)nBumpSeq);	//v3.1.T126 edit
					if (nErr == BOND_NO_ALIGNMENT_ERROR)
					{
						nNextWire++;
						if (nNextWire > m_nEndWire) bNextWireExist = false;
					}
					else if (nErr != BOND_SUCCESS)
					{
						nBondResult = nErr;
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcBallInfoNext_Err"), nBondResult);
						goto QUIT;
					}
				} while ((nErr == BOND_NO_ALIGNMENT_ERROR) && bNextWireExist);

				if (bNextWireExist)
				{
					if ((theDebug.IsDebugWaveform()
						|| theMcConfig.IsDebugModeWaveform()	//v3.1.T462 add
						|| theDebug.IsDebugWireClamp()
						|| theDebug.IsDebugSpWaveform())
						&& (nNextWire % 4 == 0))
					{
						stNextBallInfo.bScopeEnable = true;
					}

					m_stCurTblPos = stNextBallInfo.stScrubPos;

					ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SendBallInfoNext"));
					nErr = SendBallInfoToServo(&stNextBallInfo, false, nNextWire);
					if (nErr != FOK)
					{
						nBondResult = nErr;
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("SendBallInfoNext_Err"), nBondResult);
						goto QUIT;
					}
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendBallInfoNext"));

					//set usg profile
					if (CalcUsgProfile(nNextWire, nBumpSeq) == BOND_WIRE_LENGTH_ERROR)	//v3.1.T126 edit
					{
						nBondResult = BOND_WIRE_LENGTH_ERROR;	//V1.1.W254 edit
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcUsgProfNext_Err"), nBondResult);
						goto QUIT;
					}
					SetUsgProfile(nNextWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf1(3)_Buf"), m_nBondBuffer);
					Sleep(10);
					SetUsgProfile(nNextWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf2"));

					if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
						theDebug.AddInfo(_T("TrigRestart(2)"));
					theServo.TriggerRestartWire(true);
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerRestartWire(2)"));
				}
            }
            else
            {
				//look for next non-skip wire
				//ST_WIRE_INFO stCurWireInfo, stNextWireInfo;	//v3.1.T105 add, v3.1.T301 delete
				ST_WIRE_INFO stNextWireInfo;	//v3.1.T301 add
				stNextWireInfo.bScopeEnable = false;			//v3.1.T105 add
				do
				{
	                nErr = CalcWireInfo(nNextWire, m_stCurTblPos, &stNextWireInfo, sTemp, false);
					if (nErr == BOND_NO_ALIGNMENT_ERROR)
					{
						nNextWire++;
						if (nNextWire > m_nEndWire) bNextWireExist = false;
					}
					else if (nErr != BOND_SUCCESS)
					{
						nBondResult = nErr;
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcWireInfoNext_Err"), nBondResult);
						goto QUIT;
					}
				} while ((nErr == BOND_NO_ALIGNMENT_ERROR) && bNextWireExist);

				if (bNextWireExist)
				{
					if ((theDebug.IsDebugWaveform()
						|| theMcConfig.IsDebugModeWaveform()	//v3.1.T462 add
						|| theDebug.IsDebugWireClamp()
						|| theDebug.IsDebugSpWaveform())
						&& (nNextWire % 4 == 0))
					{
						stNextWireInfo.bScopeEnable = true;
					}

					m_stCurTblPos = stNextWireInfo.stTailShiftPos;

					//ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SendWireInfoNext"), stCurWireInfo.nNextWireExist);	//v3.1.T301 delete
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SendWireInfoNext_W"), nNextWire);	//v3.1.T301 add, v3.1.T302 edit
					if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
					{
						CString str;
						str.Format(_T("SendNextW_%d"), nNextWire);
						theDebug.AddInfo(str);
					}
					nErr = SendWireInfoToServo(&stNextWireInfo, false, nNextWire);
					if (nErr != FOK)
					{
						nBondResult = nErr;
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("SendWireInfoNext_Err"), nBondResult);
						goto QUIT;
					}
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendWireInfoNext"));

					//set usg profile
					if (CalcUsgProfile(nNextWire, BUMPSEQ_WIRE) == BOND_WIRE_LENGTH_ERROR)
					{
						nBondResult = BOND_WIRE_LENGTH_ERROR;	//V1.1.W254 edit
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcUsgProfNext_Err"), nBondResult);
						goto QUIT;
					}
					SetUsgProfile(nNextWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf1(4)_Buf"), m_nBondBuffer);
					Sleep(10);
					SetUsgProfile(nNextWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf2"));

					if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
						theDebug.AddInfo(_T("TrigRestart(3)"));
					theServo.TriggerRestartWire(true);
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerRestartWire(3)"));
				}
            }
        }
        else	//nAheadAlignDone != 1
        {
            nErr = CheckBondProgress(1, 1);
            if (nErr != FOK)
			{
				nBondResult = nErr;
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("CheckBondProgress(1)_Err"), nBondResult);
				goto QUIT;
			}

			if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
				theDebug.AddInfo(_T("TrigRestart(4)"));
            if (theServo.TriggerRestartWire(false) != FOK)
			{
				SetErrNo(2);
				nBondResult = BOND_SERVO_ERROR;
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("TriggerRestartWire(4)_Err"), nBondResult);
				goto QUIT;
			}
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerRestartWire(4)"));
        }
    }

	//die pr look ahead
	if (theMcConfig.GetDieAheadPR() == 2)
	{
		nErr = DieAheadPR();
		if (nErr != FOK)
		{
			SetErrNo(1);
			nBondResult = nErr;
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("DieAheadPR_Err"), nBondResult);
			goto QUIT;
		}
	}

	//lead pr look ahead
    if (PerformLeadAheadPR() != FOK)
    {
		SetErrNo(3);
        nBondResult = BOND_SERVO_ERROR;
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("LeadAheadPR_Err"), nBondResult);
		goto QUIT;
    }

    nBondResult = CheckBondResult(BUMPSEQ_WIRE, bNextWireExist);	//v3.1.T506 edit

QUIT:
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("end_BondOneWire_Err"), nBondResult);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_BondOneWire_Err"), nBondResult);
	ShowDebugMsg(TIME_STAMP_BOND, _T("end_BondOneWire_Err"), nBondResult);
    return nBondResult;
}

int CBond::BondOneBall(bool bStartBond, EN_BUMP_SEQ nBumpSeq)
{
	if (theMcConfig.IsBypassAll()) return FOK;

	ShowDebugMsg(TIME_STAMP_BOND, _T("beg_BondOneBall_W"), m_nCurWire);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_BondOneBall_W"), m_nCurWire);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("beg_BondOneBall_W"), m_nCurWire);

	short nBondResult;	//v3.1.T100 edit
    int nErr;
    CString str;
	//v3.1.T104 delete
    //ST_WIRE_INFO stNextWireInfo;
    //ST_BALL_INFO stCurBallInfo, stNextBallInfo;

	const WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);	//v3.0.T438 edit

	//v3.1.T104 delete
    //stNextWireInfo.bScopeEnable = false;
    //stCurBallInfo.bScopeEnable = false;
    //stNextBallInfo.bScopeEnable = false;

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("BondType"), m_nBondType);	//v3.1.T505 add
    if ((m_nBondType == BONDTYPE_SINGLE)
		|| (m_nBondType == BONDTYPE_RESTART))
    {
        //theServo.GetTablePosition(&m_stCurTblPos);

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_CalcBallInfo"));
		//look for next non-skip ball
		ST_BALL_INFO stCurBallInfo;		//v3.1.T104 add
		stCurBallInfo.bScopeEnable = false;	//v3.1.T104 add
		do
		{
			nErr = CalcBallInfo(m_nCurWire, m_stCurTblPos, &stCurBallInfo,
								str, false, nBumpSeq);
			if (nErr == BOND_NO_ALIGNMENT_ERROR)
			{
				m_nCurWire++;
				if (m_nCurWire > m_nEndWire)
				{
					nBondResult = FOK;
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("LastWire"));
					goto QUIT;
				}
			}
			else if (nErr != BOND_SUCCESS)
			{
				nBondResult = nErr;
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcBallInfo_Err"), nBondResult);
				goto QUIT;
			}
		} while (nErr == BOND_NO_ALIGNMENT_ERROR);

        if (m_nBondType == BONDTYPE_SINGLE)
            stCurBallInfo.nNextWireExist = 0;
        
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_CalcBallInfo"));
        
        if (theDebug.IsDebugWaveform()
			|| theMcConfig.IsDebugModeWaveform()	//v3.1.T462 add
			|| theDebug.IsDebugWireClamp()
			|| theDebug.IsDebugSpWaveform())
            stCurBallInfo.bScopeEnable = true;
        else
            stCurBallInfo.bScopeEnable = false;

        nErr = SendBallInfoToServo(&stCurBallInfo, bStartBond, m_nCurWire);
        if (nErr != FOK)
		{
			nBondResult = nErr;
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("SendBallInfo_Err"), nBondResult);
			goto QUIT;
		}
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendBallInfo"));

		if (!m_bStopBondReady)	//v3.1.T503 add
		{
			theServo.TriggerRestartWire(true);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerRestartWire"));
		}
    }

    if (thePgm.IsCopperSequence())
    {
        nErr = CheckBondProgress(1, 2);
        if (nErr != FOK)
		{
			nBondResult = nErr;
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("CheckBondProgress(1)_Err"), nBondResult);
			goto QUIT;
		}

		if (m_bPreFireEfoSuccess)
		{
			//not check efo first time if prefire success
			m_bPreFireEfoSuccess = false;
			theEFO.ClearRxCount();	//v1.5.T75 edit
		}
		else
		{
			if ((nErr == FOK) && (m_bDryRun == false) && m_bCheckFab)
			{
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_IsSparkSuccess_3"));	//v3.1.T469 add
				if (theMcConfig.IsEfoErrIO())	//v3.1.T434 add
				{
					if (theServo.GetInput(0) == 0)	//efo fail
					{
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("IoErr"));	//v3.1.T471 add
						//if (theEFO.IsSparkSuccess() == false)	//v3.1.T500 delete
						{
							nBondResult = BOND_NEFO_FAIL;
							//ShowDebugMsg(DEBUG_BOND_TIMING, _T("SparkFail"));	//v3.1.T500 delete
							goto QUIT;
						}
					}
				}
				else
				{
					if (theEFO.IsSparkSuccess() == false)	//v1.6.T338 add
					{
						nBondResult = BOND_NEFO_FAIL;
						ShowDebugMsg(DEBUG_BOND_TIMING, _T("SparkFail"));
						goto QUIT;
					}
				}
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_IsSparkSuccess_3"));	//v3.1.T469 add
			}
		}
    }

	m_WireCycleTimer.StartTimer();
    nErr = TriggerBonding(nBumpSeq);	//v3.1.T135 edit
    if (nErr != FOK)
	{
		nBondResult = nErr;
		goto QUIT;
	}

	bool bNextWireExist = false;	//v3.1.T503 edit
	short nNextWire = m_nCurWire;	//v3.1.T100 edit
	/* v3.1.T503 delete
    if ((m_nBondType == BONDTYPE_SINGLE)
		|| ((pstWire->nBumpType == BUMP_BOS)
			&& (m_nBondMode != BONDMODE_AUTO))
		|| ((pstWire->nBumpType == BUMP_BSB)	//v3.1.T101 add
			&& (nBumpSeq == BUMPSEQ_BALL2)
			&& (m_nBondMode != BONDMODE_AUTO)))
    {
        bNextWireExist = false;
    }
    else
	*/
	//v3.1.T503 add
    if (((m_nBondType == BONDTYPE_SINGLE)
		|| ((pstWire->nBumpType == BUMP_BOS)
			&& (m_nBondMode != BONDMODE_AUTO))
		|| ((pstWire->nBumpType == BUMP_BSB)	//v3.1.T101 add
			&& (nBumpSeq == BUMPSEQ_BALL2)
			&& (m_nBondMode != BONDMODE_AUTO))) == false)
    {
		switch (pstWire->nBumpType)	//v3.1.T88 add
		{
		case BUMP_SOB:
		{
			if (GetBumpBondOption() == BUMPOPT_BALL_ONLY)
			{
				//if (m_nCurWire >= thePgm.GetTotalWire())
				/* v3.1.T503 delete
				if (m_nCurWire >= m_nEndWire)
					bNextWireExist = false;
				else
				*/
				if (m_nCurWire < m_nEndWire)	//v3.1.T503 add
				{
					nNextWire = m_nCurWire + 1;
					bNextWireExist = true;
				}
			}
			else
				bNextWireExist = true;
			break;
		}
		case BUMP_BSB:	//v3.1.T101 add
		{
			if ((GetBumpBondOption() == BUMPOPT_BALL_ONLY)
				|| (nBumpSeq == BUMPSEQ_BALL2))
			{
				/* v3.1.T503 delete
				if (m_nCurWire >= m_nEndWire)
					bNextWireExist = false;
				else
				*/
				if (m_nCurWire < m_nEndWire)	//v3.1.T503 add
				{
					nNextWire = m_nCurWire + 1;
					bNextWireExist = true;
				}
			}
			else
				bNextWireExist = true;
			break;
		}
		//default:		//v3.1.T503 delete
		case BUMP_BOS:	//v3.1.T503 add
		{
			//if (m_nCurWire >= thePgm.GetTotalWire())
			/* v3.1.T503 delete
			if (m_nCurWire >= m_nEndWire)
				bNextWireExist = false;
			else
			*/
			if (m_nCurWire < m_nEndWire)	//v3.1.T503 add
			{
				nNextWire = m_nCurWire + 1;
				bNextWireExist = true;
			}
		}
		}
    }

	/* v3.1.T503 delete
    if (bNextWireExist && m_bRequestStopBond)
    {
        if ((pstWire->nBumpType != BUMP_SOB)
			&& !((pstWire->nBumpType == BUMP_BSB) && (nBumpSeq == BUMPSEQ_BALL))	//v3.1.T125 add
			|| (m_nBumpBondOption == BUMPOPT_BALL_ONLY))
        {
            bNextWireExist = false;
            m_nBondMode = BONDMODE_STOP;
            m_bRequestStopBond = false;
        }
    }
	*/
	//v3.1.T503 add
    if (m_bStopBondReady)
    {
		//v3.1.T511 edit
        if ((pstWire->nBumpType == BUMP_BOS)
			|| ((pstWire->nBumpType == BUMP_BSB)
				&& (nBumpSeq == BUMPSEQ_BALL2))
			|| (m_nBumpBondOption == BUMPOPT_BALL_ONLY))
        {
            m_bStopBondReady = false;
            bNextWireExist = false;
            m_nBondMode = BONDMODE_STOP;
        }
    }

    if (bNextWireExist)
    {
        if (CheckAheadAlignDone(&nNextWire) == 1)	//v3.1.T102 edit
		{
			WIRE_RECORD *pstNextWire = theWire.GetRcd(nNextWire);	//v3.1.T103 add
            if ((nNextWire > m_nCurWire)
				&& ((pstNextWire->nBumpType == BUMP_SOB)		//v3.0.T438 edit
					|| (m_nBumpBondOption == BUMPOPT_BALL_ONLY)	//v3.1.T510 add
					|| ((pstNextWire->nBumpType == BUMP_BSB)	//v3.1.T103 add
						&& (nBumpSeq == BUMPSEQ_BALL2))))
            {
				ST_BALL_INFO stNextBallInfo;			//v3.1.T104 add
				stNextBallInfo.bScopeEnable = false;	//v3.1.T104 add
                nErr = CalcBallInfo(nNextWire, m_stCurTblPos,
									&stNextBallInfo, str, false);
                if (nErr != FOK)
				{
					nBondResult = nErr;
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcBallInfoNext_Err"), nBondResult);
					goto QUIT;
				}

                if ((nNextWire >= thePgm.GetTotalWire()) && (GetBumpBondOption() == BUMPOPT_BALL_ONLY))
                    stNextBallInfo.nNextWireExist = 0;

                m_stCurTblPos = stNextBallInfo.stScrubPos;

				ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SendBallInfoNext"));
                nErr = SendBallInfoToServo(&stNextBallInfo, false, nNextWire);
                if (nErr != FOK)
				{
					nBondResult = nErr;
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("SendBallInfoNext_Err"), nBondResult);
					goto QUIT;
				}
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendBallInfoNext"));

				//set usg profile
				if (CalcUsgProfile(nNextWire, BUMPSEQ_BALL) == BOND_WIRE_LENGTH_ERROR)
				{
					nBondResult = BOND_WIRE_LENGTH_ERROR;	//V1.1.W254 edit
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcUsgProfNext_Err"), nBondResult);
					goto QUIT;
				}
				SetUsgProfile(nNextWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf1(5)_Buf"), m_nBondBuffer);
				Sleep(10);
				SetUsgProfile(nNextWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf2"));

                theServo.TriggerRestartWire(true);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerRestartWire"));
			}
            else
            {
				ST_WIRE_INFO stNextWireInfo;			//v3.1.T104 add
				stNextWireInfo.bScopeEnable = false;	//v3.1.T104 add
                nErr = CalcWireInfo(nNextWire, m_stCurTblPos, &stNextWireInfo, str, false);
                if (nErr != FOK)
				{
					nBondResult = nErr;
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcWireInfoNext_Err"), nBondResult);
					goto QUIT;
				}

                m_stCurTblPos = stNextWireInfo.stTailShiftPos;
                
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SendWireInfoNext"));
                nErr = SendWireInfoToServo(&stNextWireInfo, false, nNextWire);
                if (nErr != FOK)
				{
					nBondResult = nErr;
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("SendWireInfoNext_Err"), nBondResult);
					goto QUIT;
				}
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SendWireInfoNext"));

				//set usg profile
				if (CalcUsgProfile(nNextWire, BUMPSEQ_WIRE) == BOND_WIRE_LENGTH_ERROR)
				{
					nBondResult = BOND_WIRE_LENGTH_ERROR;	//V1.1.W254 edit
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("CalcUsgProfNext_Err"), nBondResult);
					goto QUIT;
				}
				SetUsgProfile(nNextWire, m_anUsgTime1, m_anUsgPower1, m_anIndex[0], USG_1ST_BOND_SEG_NUM);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf1(6)_Buf"), m_nBondBuffer);
				Sleep(10);
				SetUsgProfile(nNextWire, m_anUsgTime2, m_anUsgPower2, m_anIndex[1], USG_2ND_BOND_SEG_NUM);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SetUsgProf2"));
                
                theServo.TriggerRestartWire(true);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerRestartWire"));
            }
        }
        else
        {
            nErr = CheckBondProgress(1, 3);
            if (nErr != FOK)
			{
				nBondResult = nErr;
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("CheckBondProgress(1)_Err"), nBondResult);
				goto QUIT;
			}

            if (theServo.TriggerRestartWire(false) != FOK)
            {
				SetErrNo(4);
                nBondResult = BOND_SERVO_ERROR;
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("TriggerRestartWire_Err"), nBondResult);
				goto QUIT;
	        }
	    }
    }
	nBondResult = CheckBondResult(nBumpSeq, bNextWireExist);	//v3.1.T506 edit

QUIT:
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("end_BondOneBall_Err"), nBondResult);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_BondOneBall_Err"), nBondResult);
	ShowDebugMsg(TIME_STAMP_BOND, _T("end_BondOneBall_Err"), nBondResult);
    return nBondResult;
}

void CBond::ReportBondError(short nError)
{
	//v3.1.T439 add
	if (nError != BOND_SUCCESS)
		m_AirTensioner.SetRelayState(FALSE);

	CString sMsg;
	switch (nError)
	{
	case BOND_TIMEOUT:
	{
		sMsg.Format(_T("%s (%d, %d)"),
			theMsg.GetMsg(MSG_BOND_TIMEOUT),
			m_anErrBondProgress[0], m_anErrBondProgress[1]);
		theStat.SetMachineState(MCSTATE_STANDBY);
		theLog.WriteLong2(_T("Bond Timeout (%d, %d)"), m_anErrBondProgress[0], m_anErrBondProgress[1], true);
		break;
	}
	case BOND_SERVO_ERROR:
	{
		sMsg.Format(_T("%s (%d)"),
			theMsg.GetMsg(MSG_SERVO_ERROR), m_nErrNo);
		theStat.SetMachineState(MCSTATE_STANDBY);
		theLog.WriteLong(_T("Servo Error (%d)"), m_nErrNo, true);
		break;
	}
	case BOND_NEFO_FAIL:
	{
		if (theEFO.GetErrorCode() == SPARK_OPEN)
			sMsg = theMsg.GetEngMsg(MSG_SPARK_OPEN);
		else if (theEFO.GetErrorCode() == SPARK_GAP_LARGE)
			sMsg = theMsg.GetEngMsg(MSG_SPARK_GAP_LARGE);
		else if (theEFO.GetErrorCode() == SPARK_MISSING_RESULT)
			sMsg = theMsg.GetEngMsg(MSG_SPARK_MISSING_RESULT);
		else
			sMsg = theMsg.GetEngMsg(MSG_SPARK_UNKNOWN_ERR);
		theLog.WriteLine(sMsg, true);

		if (theEFO.GetErrorCode() == SPARK_OPEN)
			sMsg = theMsg.GetMsg(MSG_SPARK_OPEN);
		else if (theEFO.GetErrorCode() == SPARK_GAP_LARGE)
			sMsg = theMsg.GetMsg(MSG_SPARK_GAP_LARGE);
		else if (theEFO.GetErrorCode() == SPARK_MISSING_RESULT)
			sMsg = theMsg.GetMsg(MSG_SPARK_MISSING_RESULT);
		else
			sMsg = theMsg.GetMsg(MSG_SPARK_UNKNOWN_ERR);

		if (m_nBondMode == BONDMODE_AUTO)
			theStat.SetMachineState(MCSTATE_STANDBY, STATERR_EFO_OPEN);
		else
			theStat.SetMachineState(MCSTATE_STANDBY);
		theEFO.ClearRxCount();	//v1.5.T75 edit
		break;
	}
	case BOND_1ST_NONSTICK:
	{
		sMsg = theMsg.GetEngMsg(MSG_1ST_BOND_NONSTICK);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_1ST_BOND_NONSTICK);
		if (m_nBondMode == BONDMODE_AUTO)
			theStat.SetMachineState(MCSTATE_STANDBY, STATERR_NSOP);
		else
			theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_2ND_NONSTICK:
	{
		sMsg = theMsg.GetEngMsg(MSG_2ND_BOND_NONSTICK);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_2ND_BOND_NONSTICK);
		if (m_nBondMode == BONDMODE_AUTO)
		   theStat.SetMachineState(MCSTATE_STANDBY, STATERR_NSOL);
		else
			theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_TAIL_SHORT:
	{
		sMsg = theMsg.GetEngMsg(MSG_TAIL_TOO_SHORT);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_TAIL_TOO_SHORT);
		if (m_nBondMode == BONDMODE_AUTO)
			theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SHORT_TAIL);
		else
			theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_CALCULATE_ERROR:
	{
		sMsg = theMsg.GetEngMsg(MSG_BOND_CALC_ERR);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_BOND_CALC_ERR);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_BSD_COMMUNICATION_ERROR:
	{
		sMsg = theMsg.GetEngMsg(MSG_BSD_COMM_ERR);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_BSD_COMM_ERR);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_SEQUENCE_ERROR:
	{
		sMsg = theMsg.GetEngMsg(MSG_BOND_CALC_ERR);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_BOND_CALC_ERR);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_NO_ALIGNMENT_ERROR:
	{
		sMsg = theMsg.GetEngMsg(MSG_NO_ALIGNMENT);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_NO_ALIGNMENT);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_WIRE_LENGTH_ERROR:
	{
		sMsg = theMsg.GetEngMsg(MSG_WIRE_LEN_ERR);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_WIRE_LEN_ERR);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_WIRE_END:
	{
		sMsg = theMsg.GetEngMsg(MSG_WIRE_END_ERR);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_WIRE_END_ERR);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case AUTO_SAMPLING_DONE:
		return;
	case BOND_USG_ERROR:	//USG ME240 Error
	{
		sMsg = theMsg.GetEngMsg(MSG_USG_ERR);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_USG_ERR);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_NO_WIRE_FEED_ERROR:
	{
		sMsg = theMsg.GetEngMsg(MSG_WIRE_FEED_ERR);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_WIRE_FEED_ERR);
		theStat.SetMachineState(MCSTATE_STANDBY, STATERR_WIRE_FEED);	//V1.1.W251 edit
		break;
	}
	case BOND_X_MOTOR_OVERHEAT:
	{
		sMsg = theMsg.GetEngMsg(MSG_X_MOTOR_OVERHEAT);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_X_MOTOR_OVERHEAT);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_Y_MOTOR_OVERHEAT:
	{
		sMsg = theMsg.GetEngMsg(MSG_Y_MOTOR_OVERHEAT);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_Y_MOTOR_OVERHEAT);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_EXCEED_POWER_1:
	{
		sMsg = theMsg.GetEngMsg(MSG_EXCEED_POWER_1);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_EXCEED_POWER_1);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_EXCEED_POWER_2:
	{
		sMsg = theMsg.GetEngMsg(MSG_EXCEED_POWER_2);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_EXCEED_POWER_2);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_EXCEED_FORCE_1:
	{
		sMsg = theMsg.GetEngMsg(MSG_EXCEED_FORCE_1);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_EXCEED_FORCE_1);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_EXCEED_FORCE_2:
	{
		sMsg = theMsg.GetEngMsg(MSG_EXCEED_FORCE_2);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_EXCEED_FORCE_2);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_EXCEED_TIME_1:
	{
		sMsg = theMsg.GetEngMsg(MSG_EXCEED_TIME_1);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_EXCEED_TIME_1);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BOND_EXCEED_TIME_2:
	{
		sMsg = theMsg.GetEngMsg(MSG_EXCEED_TIME_2);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_EXCEED_TIME_2);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BONDERR_1ST_CTACT_LOW:
	{
		sMsg = theMsg.GetEngMsg(MSG_1ST_CTACT_LOW);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_1ST_CTACT_LOW);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BONDERR_1ST_CTACT_HIGH:
	{
		sMsg = theMsg.GetEngMsg(MSG_1ST_CTACT_HIGH);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_1ST_CTACT_HIGH);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BONDERR_2ND_CTACT_LOW:
	{
		sMsg = theMsg.GetEngMsg(MSG_2ND_CTACT_LOW);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_2ND_CTACT_LOW);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	case BONDERR_2ND_CTACT_HIGH:
	{
		sMsg = theMsg.GetEngMsg(MSG_2ND_CTACT_HIGH);
		theLog.WriteLine(sMsg, true);

		sMsg = theMsg.GetMsg(MSG_2ND_CTACT_HIGH);
		theStat.SetMachineState(MCSTATE_STANDBY);
		break;
	}
	default:
		break;
	}

	CStringList sList;
	sList.AddTail(sMsg);
	theMHS.SetOxidationProtect(true);
	theMsg.ShowMsg(ERROR_MSG, sList);
	theMHS.SetOxidationProtect(false);
}

int CBond::SendWireInfoToServo(ST_WIRE_INFO *pstWireInfo, bool bStartBond, int nWireNum)
{
    if (theMcConfig.IsBypassAll()) return 0;

    int anData[MAX_INT_VARIABLE];
    int lStatus = FOK;

	if (bStartBond)
		anData[START_BOND] = 1;
	else
		anData[START_BOND] = 0;
	CString str;
	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		str.Format(_T("SendW_%d_Buf_%d"), nWireNum, m_nBondBuffer);
		theDebug.AddInfo(str);
	}

	anData[BOND_POS1_X] = round(pstWireInfo->stBondPos[0].dx);
    anData[BOND_POS1_Y] = round(pstWireInfo->stBondPos[0].dy);

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_VLED: case MHS_VLED_ELITE:
    {
        if (thePgm.IsCopperSequence())
            anData[HEAD_TIME] = pstWireInfo->nSrchHtTime + 4;
        else
            anData[HEAD_TIME] = pstWireInfo->nSrchHtTime;
		break;
    }
	default:	//v3.0.T254 add
#if _SERVO_TYPE == 2	//v3.1.T270 add
		anData[HEAD_TIME] = pstWireInfo->nSrchHtTime;
#else
		anData[HEAD_TIME] = pstWireInfo->nSrchHtTime - 4;
#endif
	}

	anData[SRCH_LVL1] = pstWireInfo->nSrchLvl1;
	anData[SRCH_LVL2] = pstWireInfo->nSrchLvl2;	//v3.1.T260 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
	anData[SRCH_SPEED1] = pstWireInfo->nSrchSpeed1;
#else
	if ((theMcConfig.GetAcsBufferType() == 1)
		|| (theMcConfig.GetSrchSpeedStep() == 1))	//v3.0.T23 add
		anData[SRCH_SPEED1] = -pstWireInfo->nSrchSpeed1 * 500;	//v1.6.T18 add
	else
		anData[SRCH_SPEED1] = -pstWireInfo->nSrchSpeed1 * 2500;	//5000;
#endif
    anData[SRCH_TOL1] = pstWireInfo->nSrchTol1;

	double dForceFactor = 1000.0;	//v3.1.T293 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
	dForceFactor = 1.0;
#endif
	//v3.1.T374 edit
	if (m_bDryRun)
	{
		anData[PRE_IMPACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nSrchLvl1) * dForceFactor);
		anData[CTACT_FORCE1]      = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nSrchLvl1) * dForceFactor);
		anData[BASE_FORCE1]       = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nSrchLvl1) * dForceFactor);
		anData[RLS_FORCE1]        = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nSrchLvl1) * dForceFactor);
	}
	else
	{
		anData[PRE_IMPACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nPreCtactForce[0],
											pstWireInfo->nCtactLvl1) * dForceFactor);
		anData[CTACT_FORCE1]      = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nCtactForce[0],
											pstWireInfo->nCtactLvl1) * dForceFactor);
		anData[BASE_FORCE1]       = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nBaseForce[0],
											pstWireInfo->nCtactLvl1) * dForceFactor);
		anData[RLS_FORCE1]        = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nReleaseForce[0],
											pstWireInfo->nCtactLvl1) * dForceFactor);
	}
	anData[PRE_IMPACT_TIME1] = pstWireInfo->nPreCtactTime[0];
	anData[CTACT_TIME1]      = pstWireInfo->nCtactTime[0];
	anData[BASE_TIME1]       = pstWireInfo->nBaseTime[0];
	anData[RLS_TIME1]        = pstWireInfo->nReleaseTime[0];

    anData[REV_HT]      = pstWireInfo->nRevHt;
    anData[REV_DIST_X]  = round(pstWireInfo->stRevPos.dx - pstWireInfo->stBondPos[0].dx);
    anData[REV_DIST_Y]  = round(pstWireInfo->stRevPos.dy - pstWireInfo->stBondPos[0].dy);
#if _SERVO_TYPE == 2
    anData[REV_DIST_HT] = pstWireInfo->nRevDistHt;	//v3.1.T547 add
#endif
    anData[REV_HT2]     = pstWireInfo->nRevHt2;		//v3.1.T350 add
	anData[REV_DIST2_X] = round(pstWireInfo->stRevDist2.dx);	//v3.1.T575 add
	anData[REV_DIST2_Y] = round(pstWireInfo->stRevDist2.dy);	//v3.1.T575 add
	//v3.1.T374 add
	anData[REV_HT_PREMOVE]    = pstWireInfo->nRevHtPremove;
	anData[REV_DIST_PREMOVE]  = pstWireInfo->nRevDistPremove;
	anData[REV_DIST_HT_POINT] = pstWireInfo->nRevDistHtPoint;

    anData[PRE_KINK_ENABLE]  = pstWireInfo->bPreKinkEnable;
    anData[PRE_KINK_HT]      = pstWireInfo->nPreKinkHt;
    anData[PRE_KINK_DIST_X]  = round(pstWireInfo->stPreKinkPos.dx - pstWireInfo->stRevPos.dx);
    anData[PRE_KINK_DIST_Y]  = round(pstWireInfo->stPreKinkPos.dy - pstWireInfo->stRevPos.dy);
    anData[PRE_KINK_DIST_HT] = pstWireInfo->nPreKinkDistHt;
    anData[PRE_KINK_HT2]     = pstWireInfo->nPreKinkHt2;	//v3.1.T342 add
	anData[PRE_KINK_DIST2_X] = round(pstWireInfo->stPreKinkDist2.dx);	//v3.1.T575 add
	anData[PRE_KINK_DIST2_Y] = round(pstWireInfo->stPreKinkDist2.dy);	//v3.1.T575 add
	//v3.1.T374 add
    anData[PRE_KINK_HT_PREMOVE]    = theMcConfig.GetPreKinkHtPremove();
    anData[PRE_KINK_DIST_PREMOVE]  = theMcConfig.GetPreKinkDistPremove();
	anData[PRE_KINK_DIST_HT_POINT] = pstWireInfo->nPreKinkDistHtPoint;

    anData[TRAJ_DIST_X]  = round(pstWireInfo->stTrajEndPos.dx);
    anData[TRAJ_DIST_Y]  = round(pstWireInfo->stTrajEndPos.dy);
	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		str.Format(_T("TrajEndPos_%d_%d"), anData[TRAJ_DIST_X], anData[TRAJ_DIST_Y]);
		theDebug.AddInfo(str);
	}
	anData[KINK2_ENABLE]  = (pstWireInfo->bKink2Enable ? 1 : 0);	//v3.1.T374 edit
	anData[KINK2_HT]      = pstWireInfo->nKink2Ht;
	anData[KINK2_DIST_X]  = round(pstWireInfo->stKink2Pos.dx - pstWireInfo->stPreKinkPos.dx);
	anData[KINK2_DIST_Y]  = round(pstWireInfo->stKink2Pos.dy - pstWireInfo->stPreKinkPos.dy);
#if _SERVO_TYPE == 2
	anData[KINK2_DIST_HT] = pstWireInfo->nKink2DistHt;	//v3.1.T538 add
#endif
	anData[KINK2_HT2]     = pstWireInfo->nKink2Ht2;	//v3.1.T341 add
	anData[KINK2_DIST2_X] = round(pstWireInfo->stKink2Dist2.dx);	//v3.1.T375 add
	anData[KINK2_DIST2_Y] = round(pstWireInfo->stKink2Dist2.dy);	//v3.1.T375 add
	//v3.1.T374 add
	anData[KINK2_HT_PREMOVE]   = pstWireInfo->nKink2HtPremove;
	anData[KINK2_DIST_PREMOVE] = pstWireInfo->nKink2DistPremove;
	anData[KINK2_DIST_HT_POINT] = pstWireInfo->nKink2DistHtPoint;

	anData[KINK3_ENABLE]  = (pstWireInfo->bKink3Enable ? 1 : 0);	//v3.1.T374 edit
	anData[KINK3_HT]      = pstWireInfo->nKink3Ht;
	anData[KINK3_DIST_X]  = round(pstWireInfo->stKink3Pos.dx - pstWireInfo->stKink2Pos.dx);
	anData[KINK3_DIST_Y]  = round(pstWireInfo->stKink3Pos.dy - pstWireInfo->stKink2Pos.dy);
#if _SERVO_TYPE == 2
	anData[KINK3_DIST_HT] = pstWireInfo->nKink3DistHt;	//v3.1.T538 add
#endif
	anData[KINK3_HT2]     = pstWireInfo->nKink3Ht2;	//v3.1.T341 add
	anData[KINK3_DIST2_X] = round(pstWireInfo->stKink3Dist2.dx);	//v3.1.T375 add
	anData[KINK3_DIST2_Y] = round(pstWireInfo->stKink3Dist2.dy);	//v3.1.T375 add
	anData[KINK3_HT_PREMOVE]   = pstWireInfo->nKink3HtPremove;
	anData[KINK3_DIST_PREMOVE] = pstWireInfo->nKink3DistPremove;
	anData[KINK3_DIST_HT_POINT] = pstWireInfo->nKink3DistHtPoint;

	anData[TWIST_ENABLE] = (pstWireInfo->bTwistEnable ? 1 : 0);	//v3.1.T374 edit
    anData[TWIST_HT] = pstWireInfo->nTwistHt;
    anData[TWIST_REV_DIST_X] = round(pstWireInfo->stTwistRevKinkPos.dx - pstWireInfo->stKink3Pos.dx);
    anData[TWIST_REV_DIST_Y] = round(pstWireInfo->stTwistRevKinkPos.dy - pstWireInfo->stKink3Pos.dy);
    anData[REV_HT4_PREMOVE] = pstWireInfo->nTwistHtPremove;
    anData[TWIST_REV_DIST_Z_POINT] = pstWireInfo->nTwistRevDistZPoint;

	anData[TWIST_FWD_DIST_X]  = round(pstWireInfo->stTwistFwdKinkPos.dx - pstWireInfo->stTwistRevKinkPos.dx);
    anData[TWIST_FWD_DIST_Y]  = round(pstWireInfo->stTwistFwdKinkPos.dy - pstWireInfo->stTwistRevKinkPos.dy);
    anData[TWIST_FWD_DIST_PREMOVE] = pstWireInfo->nTwistFwdDistPremove;
    anData[TWIST_FWD_DIST_Z_POINT] = pstWireInfo->nTwistFwdDistZPoint;

    //anData[REV_DIST2_Z]  = pstWireInfo->lRevDist2Height;
    anData[SLOPE_LENGTH] = pstWireInfo->nSlopeLen;
#if _SERVO_TYPE == 2	//v3.1.T460 add
	anData[SRCH_SPEED2] = pstWireInfo->nSrchSpeed2;
#else
	if ((theMcConfig.GetAcsBufferType() == 1)
		|| (theMcConfig.GetSrchSpeedStep() == 1))	//v3.0.T23 add
		anData[SRCH_SPEED2] = -pstWireInfo->nSrchSpeed2 * 500;	//v1.6.T18 add
	else
		anData[SRCH_SPEED2] = -pstWireInfo->nSrchSpeed2 * 2500;	//5000;
#endif
    anData[SRCH_TOL2] = pstWireInfo->nSrchTol2;

	//v3.1.T374 edit
    if (m_bDryRun)
    {
        anData[PRE_IMPACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nCtactLvl2) * dForceFactor);
        anData[CTACT_FORCE2]      = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nCtactLvl2) * dForceFactor);
        anData[BASE_FORCE2]       = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nCtactLvl2) * dForceFactor);
        anData[RLS_FORCE2]        = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE,
											pstWireInfo->nCtactLvl2) * dForceFactor);
    }
    else
    {
        anData[PRE_IMPACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nPreCtactForce[1],
											pstWireInfo->nCtactLvl2) * dForceFactor);
        anData[CTACT_FORCE2]      = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nCtactForce[1],
											pstWireInfo->nCtactLvl2) * dForceFactor);
        anData[BASE_FORCE2]       = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nBaseForce[1],
											pstWireInfo->nCtactLvl2) * dForceFactor);
        anData[RLS_FORCE2]        = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->nReleaseForce[1],
											pstWireInfo->nCtactLvl2) * dForceFactor);
    }
    anData[PRE_IMPACT_TIME2] = pstWireInfo->nPreCtactTime[1];
	anData[CTACT_TIME2]      = pstWireInfo->nCtactTime[1];
    anData[BASE_TIME2]       = pstWireInfo->nBaseTime[1];
    anData[RLS_TIME2]        = pstWireInfo->nReleaseTime[1];

    anData[TAIL_HEIGHT] = pstWireInfo->nTailHt;
    anData[RESET_LVL]   = pstWireInfo->nResetLvl;
    
    if (theDebug.IsDebugWaveform()
		|| theMcConfig.IsDebugModeWaveform()	//v3.1.T462 add
		|| theDebug.IsDebugWireClamp()
		|| theDebug.IsDebugSpWaveform())
    {
        if (theDebug.IsDebugSpWaveform())
            anData[SCOPE_ENABLE] = 2;
        else 
        {
			anData[SCOPE_ENABLE] = (m_nWaveformCaptureCount == 0 ? 1 : 0);	//v3.1.T374 edit
            m_nWaveformCaptureCount++;
            if (m_nWaveformCaptureCount > theMcConfig.GetWaveformCaptureCount())	//v3.0.T468 edit
                m_nWaveformCaptureCount = 0;
        }
    }
    else
        anData[SCOPE_ENABLE] = 0;

    anData[FIRST_MOTION_PROFILE_X] = pstWireInfo->nFirstMotionProfileX;
	anData[MOTION_STEP_DEBUG] = (theDebug.IsDebugStepMotion() ? 1 : 0);	//v3.1.T374 edit
    anData[NEXT_WIRE_EXIST]  = pstWireInfo->nNextWireExist;
	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		str.Format(_T("NextWireExist_%d"), anData[NEXT_WIRE_EXIST]);
		theDebug.AddInfo(str);
	}

	anData[SPARK_DELAY]      = pstWireInfo->nSparkDelay;
    anData[BOND_SHIFT_SPEED] = pstWireInfo->nShiftSpeed;
    anData[BOND_SHIFT1_X]    = round(pstWireInfo->stBondShiftPos1.dx);
    anData[BOND_SHIFT1_Y]    = round(pstWireInfo->stBondShiftPos1.dy);
    anData[BOND_SHIFT2_X]    = round(pstWireInfo->stBondShiftPos2.dx);
    anData[BOND_SHIFT2_Y]    = round(pstWireInfo->stBondShiftPos2.dy);
    anData[PULL_POS_X]       = round(pstWireInfo->stPullPos.dx);
    anData[PULL_POS_Y]       = round(pstWireInfo->stPullPos.dy);
    anData[TAIL_CREEP_X]     = round(pstWireInfo->stTailShiftPos.dx);
    anData[TAIL_CREEP_Y]     = round(pstWireInfo->stTailShiftPos.dy);
    anData[SRCH_DELAY1]      = pstWireInfo->nSrchDelay[0];
    anData[SRCH_DELAY2]      = pstWireInfo->nSrchDelay[1];
    anData[WCL_TAIL_CLOSE_PRETRIG]    = pstWireInfo->nWclTailClosePretrig;
    anData[WCL_PEAK_CLOSE_PRETRIG]    = pstWireInfo->nWclLoopPeakClosePretrig;
    anData[WCL_1ST_BOND_OPEN_PRETRIG] = pstWireInfo->nWcl1stBondOpenPretrig;
    anData[WCL_2ND_BOND_OPEN_PRETRIG] = pstWireInfo->nWcl2ndBondOpenPretrig;

	anData[SCRUB2_ENABLE] = (pstWireInfo->b2ndBondScrubEnable ? 1 : 0);	//v3.1.T374 edit
    anData[SCRUB2_SPEED] = pstWireInfo->n2ndScrubSpeed;
    anData[SCRUB2_CYCLE] = pstWireInfo->n2ndScrubCycle;
    if (m_bDryRun)
        anData[SCRUB2_FORCE] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstWireInfo->nCtactLvl2) * dForceFactor);
    else
        anData[SCRUB2_FORCE] = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->n2ndScrubForce, pstWireInfo->nCtactLvl2) * dForceFactor);
    anData[SCRUB2_TIME]     = pstWireInfo->n2ndScrubOffsetTime + pstWireInfo->n2ndScrubTime;
    anData[SCRUB2_POS_X]    = round(pstWireInfo->st2ndScrubPos.dx);
    anData[SCRUB2_POS_Y]    = round(pstWireInfo->st2ndScrubPos.dy);
    anData[SCRUB2_OFFSET_X] = round(pstWireInfo->st2ndScrubOffset2.dx);
    anData[SCRUB2_OFFSET_Y] = round(pstWireInfo->st2ndScrubOffset2.dy);
    anData[SCRUB2_AMP_X]    = round(pstWireInfo->st2ndScrubAmp.dx);
    anData[SCRUB2_AMP_Y]    = round(pstWireInfo->st2ndScrubAmp.dy);

	anData[BOND_SHIFT1_ENABLE] = pstWireInfo->bBondShift1Enable;
    if (m_bDryRun)
    {
        anData[SRCH_LMT1] = -20000;
        anData[SRCH_LMT2] = -20000;
    }
    else
    {
        anData[SRCH_LMT1] = pstWireInfo->nCtactLvl1 - 500;
        anData[SRCH_LMT2] = pstWireInfo->nCtactLvl2 - 500;
    }
	//v1.6.T16 add
	anData[INT_WIRE_NUM] = ((nWireNum - 1) % thePgm.GetUnitTotalWire()) + 1;	//v1.6.T24 edit
	anData[BUMP_TYPE] = pstWireInfo->nBumpType;

	anData[BALL_BOND_ENABLE]      = 0;
    anData[LOOP_TOP_MODE]         = pstWireInfo->cLoopTopMode;
    anData[ABOVE_LOOP_TOP_POS_X]  = round(pstWireInfo->stAboveLoopTopPos.dx - pstWireInfo->stLoopTopPos.dx);
    anData[ABOVE_LOOP_TOP_POS_Y]  = round(pstWireInfo->stAboveLoopTopPos.dy - pstWireInfo->stLoopTopPos.dy);
    anData[ABOVE_LOOP_TOP_HEIGHT] = pstWireInfo->nAboveLoopTopHt;
	if (theMcConfig.IsLoopTopPremove())
		anData[LOOP_TOP_PREMOVE]  = pstWireInfo->nLoopTopPremove / 4;	//v3.1.T565 add
	else
		anData[LOOP_TOP_PREMOVE]  = pstWireInfo->nLoopTopPremove;	//v3.1.T374 add

	short nCtrlBlkX, nCtrlBlkY;
	theServo.GetTableCtrlBlock(pstWireInfo->stBondPos[0], &nCtrlBlkX, &nCtrlBlkY);
	anData[TABLE_X_CTRL_BLOCK] = nCtrlBlkX;
	anData[TABLE_Y_CTRL_BLOCK] = nCtrlBlkY;

	//v3.1.T374 delete
	//anData[NECK_HT_PREMOVE]    = pstWireInfo->nNeckHtPremove;
	//anData[NECK_DIST_PREMOVE]  = pstWireInfo->nNeckDistPremove;
	//anData[KINK2_HT_PREMOVE]   = pstWireInfo->nKink2HtPremove;
	//anData[KINK2_DIST_PREMOVE] = pstWireInfo->nKink2DistPremove;
	//anData[LOOP_TOP_PREMOVE]   = pstWireInfo->nLoopTopPremove;

	//v3.1.T227 add, v3.1.T374 delete
	//anData[PRE_KINK_HT_PREMOVE] = theMcConfig.GetPreKinkHtPremove();
	//anData[PRE_KINK_DIST_PREMOVE] = theMcConfig.GetPreKinkDistPremove();

	//v3.1.T374 delete
	//anData[NECK_DIST_Z_POINT] = pstWireInfo->nNeckDistZPoint;
	//anData[KINK2_DIST_Z_POINT] = pstWireInfo->nRevDist2ZPoint;
	//anData[PRE_KINK_DIST_Z_POINT] = pstWireInfo->nPreKinkDistZPoint;	//v3.1.T326 add
	anData[ARC_REV_DELAY] = pstWireInfo->nArcRevDelay;	//v3.1.T407 add
	anData[ARC_PRE_KINK_DELAY] = pstWireInfo->nArcPreKinkDelay;		//v3.1.T328 add
	anData[ARC_KINK2_DELAY] = pstWireInfo->nArcKink2Delay;	//v3.1.T337 add
	anData[ARC_KINK3_DELAY] = pstWireInfo->nArcKink3Delay;	//v3.1.T337 add

	anData[COPPER_SEQUENCE] = (thePgm.IsCopperSequence() ? 1 : 0);	//v3.1.T374 edit

    MACHINE_DATA *pstMcData = theMcData.GetRcd();
    anData[DAMPGAIN] = pstMcData->nDampGain;
    anData[SEARCH_MODE] = pstMcData->nSearchMode;
    anData[RCD_SAMPLE] = pstMcData->chRcdSample;
    anData[RCD_TOL] = pstMcData->chRcdTol;

	anData[SCRUB1_ENABLE] = (pstWireInfo->b1stBondScrubEnable ? 1 : 0);	//v3.1.T374 edit
    anData[SCRUB1_SPEED] = pstWireInfo->n1stScrubSpeed;
    anData[SCRUB1_CYCLE] = pstWireInfo->n1stScrubCycle;
    if (m_bDryRun)
        anData[SCRUB1_FORCE] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstWireInfo->nCtactLvl1) * dForceFactor);
    else
        anData[SCRUB1_FORCE] = round(theMcData.Calc3PtBondForceDac(-pstWireInfo->n1stScrubForce, pstWireInfo->nCtactLvl1) * dForceFactor);
    anData[SCRUB1_TIME] = pstWireInfo->n1stScrubTime;
    anData[SCRUB1_AMP_X] = round(pstWireInfo->st1stScrubAmp.dx);
    anData[SCRUB1_AMP_Y] = round(pstWireInfo->st1stScrubAmp.dy);

	if (theMcConfig.IsLoopTopPremove())	//v3.1.T565 add
		anData[TRAJ_POINT] = pstWireInfo->nTrajPointZ;
	else
	{
		if ((pstWireInfo->nLoopTopPremove > 0) && (pstWireInfo->nWclLoopPeakClosePretrig < 0))
			anData[TRAJ_POINT] = pstWireInfo->nTrajPoint - pstWireInfo->nLoopTopPremove * 4 -
						abs(pstWireInfo->nWclLoopPeakClosePretrig) * 2 - 6;
		else
			anData[TRAJ_POINT] = pstWireInfo->nTrajPoint - pstWireInfo->nLoopTopPremove * 4 - 6;
	}

    anData[FIRST_MOTION_PROFILE_Y] = pstWireInfo->nFirstMotionProfileY;
    anData[DAMPGAIN2] = pstMcData->nDampGain2;
	anData[FAB_TEST] = (thePgm.IsFabTest() ? 1 : 0);	//v3.1.T374 edit

    anData[TAIL_BREAK_DIST]  = pstWireInfo->nTailBreakDist;
    anData[TAIL_BREAK_SPEED] = pstWireInfo->nTailBreakSpeed * 2500;

    anData[PRE_IMPACT_SHIFT_X] = round(pstWireInfo->stPreShiftPos.dx);
    anData[PRE_IMPACT_SHIFT_Y] = round(pstWireInfo->stPreShiftPos.dy);

	//v3.1.T34 delete
	//if (pstWireInfo->chFabType == 0)
	//	anData[FAB_TYPE] = FAB_TYPE_1;
	//else
	//	anData[FAB_TYPE] = FAB_TYPE_2;
	anData[FAB_TYPE] = pstWireInfo->chFabType;	//v3.1.T34 add

    anData[SCRUB1_Y_DELAY] = pstWireInfo->n1stScrubYDelay;
    anData[TAIL_BOND_ENABLE] = 1;

    anData[REV_HT_SPEED]         = pstWireInfo->nKink1WireFeedSpeed;
    anData[REV_DIST_SPEED]       = pstWireInfo->nKink1WireBentSpeed;
    anData[PRE_KINK_HT_SPEED]    = pstWireInfo->nPreKinkWireFeedSpeed;
    anData[PRE_KINK_DIST_SPEED]  = pstWireInfo->nPreKinkWireBentSpeed;
    anData[KINK2_HT_SPEED]       = pstWireInfo->nKink2WireFeedSpeed;
    anData[KINK2_DIST_SPEED]     = pstWireInfo->nKink2WireBentSpeed;
    anData[KINK3_HT_SPEED]       = pstWireInfo->nKink3WireFeedSpeed;
    anData[KINK3_DIST_SPEED]     = pstWireInfo->nKink3WireBentSpeed;
    anData[TWIST_HT_SPEED]       = pstWireInfo->nKink4WireFeedSpeed;
    anData[TWIST_REV_DIST_SPEED] = pstWireInfo->nTwistRevBentSpeed;
    anData[TWIST_FWD_DIST_SPEED] = pstWireInfo->nTwistFwdBentSpeed;
    anData[TRAJ_SPEED]           = pstWireInfo->nTrajectorySpeed;
    anData[PULL_SPEED]           = pstWireInfo->nPullSpeed;

	anData[FAST_LOOP] = (pstWireInfo->bFastLoop ? 1 : 0);	//v3.1.T374 edit

	//v1.6.T135 add
	if (theMcConfig.GetAcsBufferType() == 1)
	{
		//FORCE_SWITCH_PARA* pstForceSwitch = theForceSwitch.GetRcd();
		//anData[FORCE_SWITCH]  = pstForceSwitch->iForceSwitch;     //v1.6.P136sc edit, v3.1.T341 delete
		//anData[IMPACT_SPEED1] = pstForceSwitch->nSpeed1;	//v3.1.T341 delete
		//anData[IMPACT_SPEED2] = pstForceSwitch->nSpeed2;	//v3.1.T341 delete
		//anData[IMPACT_SPEED3] = pstForceSwitch->nSpeed3;	//v3.1.T337 delete
		//anData[IMPACT_MODE1]  = pstForceSwitch->nMode1;	//v3.1.T337 delete
		//anData[IMPACT_MODE2]  = pstForceSwitch->nMode2;	//v3.1.T332 delete
		//anData[IMPACT_MODE3]  = pstForceSwitch->nMode3;	//v3.1.T328 delete
	}

	//v1.6.T151 add
	anData[SRCH_HT1] = pstWireInfo->nSrchHt1;
	anData[SRCH_HT2] = pstWireInfo->nSrchHt2;

	//v2.0.T99 add
	anData[TAIL_DELAY] = theMcConfig.GetTailDelay();

	//v1.6.T254 add, v3.1.T315 delete
	//anData[SOFTSRCH1_ENABLE]        = (int)theMcConfig.IsSoftSrchEnable(0);
	//anData[SOFTSRCH1_SPEED]         = theMcConfig.GetSoftSrchSpeed(0);
	//anData[SOFTSRCH1_POSERR]        = theMcConfig.GetSoftSrchPosErr(0);
	//anData[SOFTSRCH1_POSERR_SERIES] = theMcConfig.GetSoftSrchPosErrSeries(0);
	//anData[SOFTSRCH2_ENABLE]        = (int)theMcConfig.IsSoftSrchEnable(1);
	//anData[SOFTSRCH2_SPEED]         = theMcConfig.GetSoftSrchSpeed(1);
	//anData[SOFTSRCH2_POSERR]        = theMcConfig.GetSoftSrchPosErr(1);
	//anData[SOFTSRCH2_POSERR_SERIES] = theMcConfig.GetSoftSrchPosErrSeries(1);

	//v3.1.T315 add
	anData[RH_POS_GAIN]       = 0;
	anData[RH_VEL_GAIN]       = 0;
	anData[RH_VEL_INTEGRATOR] = 0;
	anData[RH_AFF]            = 0;
	anData[RD_POS_GAIN]       = 0;
	anData[RD_VEL_GAIN]       = 0;
	anData[RD_VEL_INTEGRATOR] = 0;
	anData[RD_AFF]            = 0;
	if (theMcConfig.IsControlGroupEnable())
	{
		short nLoopMode = theWire.GetRcd(nWireNum)->nLoopMode;
		if (nLoopMode > 0)	//v3.1.T316 add
		{
			anData[RH_POS_GAIN]       = theMcConfig.GetPosGainRH(nLoopMode);
			anData[RH_VEL_GAIN]       = theMcConfig.GetVelGainRH(nLoopMode);
			anData[RH_VEL_INTEGRATOR] = theMcConfig.GetVelIntegratorRH(nLoopMode);
			anData[RH_AFF]            = theMcConfig.GetAffRH(nLoopMode);
			anData[RD_POS_GAIN]       = theMcConfig.GetPosGainRD(nLoopMode);
			anData[RD_VEL_GAIN]       = theMcConfig.GetVelGainRD(nLoopMode);
			anData[RD_VEL_INTEGRATOR] = theMcConfig.GetVelIntegratorRD(nLoopMode);
			anData[RD_AFF]            = theMcConfig.GetAffRD(nLoopMode);
		}
	}

	anData[TRAJ_FACTOR] = pstWireInfo->nTrajFactor;			//v3.1.T318 add
	//anData[BUF2_PARAM1] = theMcConfig.GetBuf2Param(0);	//v3.1.T318 delete
	//anData[BUF2_PARAM2] = theMcConfig.GetBuf2Param(1);	//v3.1.T326 delete

	//v3.0.T400 add
	anData[REBOND_LOOP_HT]   = theMcConfig.GetRebondLoopHt();
	anData[REBOND_BASE_HT]   = theMcConfig.GetRebondLoopHt() - theMcConfig.GetRebondBaseHt();
	anData[REBOND_TIME]      = theMcConfig.GetRebondTime();
	anData[REBOND_FORCE]     = theMcConfig.GetRebondForce();	//v3.0.T407 add
	anData[REBOND_SPANLEN_X] = round(pstWireInfo->stRebondPos.dx - pstWireInfo->stBondPos[1].dx);
	anData[REBOND_SPANLEN_Y] = round(pstWireInfo->stRebondPos.dy - pstWireInfo->stBondPos[1].dy);

#if _SERVO_TYPE == 2	//v3.1.T428 add
	anData[BOND1_TRANSIT_TIME]  = theMcConfig.GetBondTransitTime(0);
	anData[BOND1_FILTER_TIME]   = theMcConfig.GetBondFilterTime(0);
	anData[BOND2_TRANSIT_TIME]  = theMcConfig.GetBondTransitTime(1);
	anData[BOND2_FILTER_TIME]   = theMcConfig.GetBondFilterTime(1);
	anData[RLS1_TRANSIT_TIME]   = theMcConfig.GetRlsTransitTime(0);
	anData[RLS1_FILTER_TIME]    = theMcConfig.GetRlsFilterTime(0);
	anData[RLS2_TRANSIT_TIME]   = theMcConfig.GetRlsTransitTime(1);
	anData[RLS2_FILTER_TIME]    = theMcConfig.GetRlsFilterTime(1);
	anData[FORCE1_GAIN_THRES]   = theMcConfig.GetForceGainThres(0);
	anData[FORCE2_GAIN_THRES]   = theMcConfig.GetForceGainThres(1);
#endif

	lStatus = theServo.SendIntDataToServo(anData, MAX_INT_VARIABLE, m_nBondBuffer);
    if (lStatus != FOK) return lStatus;
    //Sleep(1);

    if (pstWireInfo->bFastLoop == false)
    {
        lStatus = theServo.SendRealDataToServo(pstWireInfo->adRevDistHtMotion,
			pstWireInfo->nRevDistHtPoint, m_nBondBuffer, SERVO_VAR_REV_DIST_HT);
        if (lStatus != FOK) return lStatus;
    }
    //Sleep(1);

	//v3.1.T326 add
	if (pstWireInfo->bPreKinkEnable && (pstWireInfo->nPreKinkDistHtPoint > 0))
	{
		lStatus = theServo.SendRealDataToServo(pstWireInfo->adPreKinkDistHtMotion,
			pstWireInfo->nPreKinkDistHtPoint, m_nBondBuffer, SERVO_VAR_PRE_KINK_DIST_HT);
		if (lStatus != FOK) return lStatus;
	}

	if (pstWireInfo->bKink2Enable)
    {
        lStatus = theServo.SendRealDataToServo(pstWireInfo->adKink2DistHtMotion,
			pstWireInfo->nKink2DistHtPoint, m_nBondBuffer, SERVO_VAR_KINK2_DIST_HT);
        if (lStatus != FOK) return lStatus;
        //Sleep(1);
    }

    if (pstWireInfo->bKink3Enable)
    {
        lStatus = theServo.SendRealDataToServo(pstWireInfo->adKink3DistHtMotion,
			pstWireInfo->nKink3DistHtPoint, m_nBondBuffer, SERVO_VAR_KINK3_DIST_HT);
        if (lStatus != FOK) return lStatus;
        //Sleep(1);
    }

    if (pstWireInfo->bTwistEnable)
    {
        lStatus = theServo.SendRealDataToServo(pstWireInfo->adTwistRevDistZMotion,
			pstWireInfo->nTwistRevDistZPoint, m_nBondBuffer, SERVO_VAR_TWIST_REV_DIST_Z);
        if (lStatus != FOK) return lStatus;
        //Sleep(1);

        lStatus = theServo.SendRealDataToServo(pstWireInfo->adTwistFwdDistZMotion,
			pstWireInfo->nTwistFwdDistZPoint, m_nBondBuffer, SERVO_VAR_TWIST_FWD_DIST_Z);
        if (lStatus != FOK) return lStatus;
        //Sleep(1);
    }

	double adBhTraj[500];
	short nPretrig;
	if ((pstWireInfo->nLoopTopPremove > 0) && (pstWireInfo->nWclLoopPeakClosePretrig < 0))
		nPretrig = pstWireInfo->nLoopTopPremove * 4 + abs(pstWireInfo->nWclLoopPeakClosePretrig) * 2 + 6;
	else
		nPretrig = pstWireInfo->nLoopTopPremove * 4 + 6;
	for (short i = nPretrig; i < pstWireInfo->nTrajPoint; i++)
		adBhTraj[i - nPretrig] = pstWireInfo->adTrajPos[pstWireInfo->nTrajPoint * 2 + i];
	if (theMcConfig.IsLoopTopPremove())	//v3.1.T565 add
		lStatus = theServo.SendRealDataToServo(pstWireInfo->adTrajPosZ,
			pstWireInfo->nTrajPointZ, m_nBondBuffer, SERVO_VAR_TRAJ_HT);
	else
		lStatus = theServo.SendRealDataToServo(adBhTraj,
			pstWireInfo->nTrajPoint - nPretrig, m_nBondBuffer, SERVO_VAR_TRAJ_HT);
    if (lStatus != FOK) return lStatus;

	if (theMcConfig.GetAcsBufferType() == 1)	//v1.6.T23 add
	{
		theServo.SendRealDataToServo(&m_adMZ1[0][0], 300, m_nBondBuffer, SERVO_VAR_MZ1);

		//v1.6.T146 add
		CalcTail(pstWireInfo->chFabType);
		theServo.SendRealDataToServo(m_adTail, 100, m_nBondBuffer, SERVO_VAR_TAIL);

		//v1.6.T137 add
		FORCE_SWITCH_PARA* pstForceSwitch = theForceSwitch.GetRcd();
		double dForceCompensate[30][3];
		for (short i = 0; i < 30; i++)
		{
			for (short j = 0; j < 3; j++)
			{
				if (j == 0)
					dForceCompensate[i][j] = theMcData.Calc3PtBondForceDac(-pstForceSwitch->iForceTime[i][j], theMcData.GetFlatLevel()) * dForceFactor;
				else
					dForceCompensate[i][j] = pstForceSwitch->iForceTime[i][j];
			}
		}
		theServo.SendRealDataToServo(&dForceCompensate[0][0], 3,
									m_nBondBuffer, SERVO_VAR_FORCE_COMPENSATE);
	}

	return FOK;
}

int CBond::SendBallInfoToServo(ST_BALL_INFO *pstBallInfo, bool bStartBond, int nWireNum)
{
    int anData[MAX_INT_VARIABLE];
    int lStatus = FOK;

	if (bStartBond)
		anData[START_BOND] = 1;
	else
		anData[START_BOND] = 0;

	anData[BOND_POS1_X] = round(pstBallInfo->stBondPos[0].dx);
    anData[BOND_POS1_Y] = round(pstBallInfo->stBondPos[0].dy);

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_VLED: case MHS_VLED_ELITE:
    {
        if (thePgm.IsCopperSequence())
            anData[HEAD_TIME] = pstBallInfo->nSrchHtTime + 4;
        else
            anData[HEAD_TIME] = pstBallInfo->nSrchHtTime;
		break;
    }
	default:	//v3.0.T254 add
#if _SERVO_TYPE == 2	//v3.1.T270 add
		anData[HEAD_TIME] = pstBallInfo->nSrchHtTime;
#else
		anData[HEAD_TIME] = pstBallInfo->nSrchHtTime - 4;
#endif
	}

	anData[SRCH_LVL1] = pstBallInfo->nSrchLvl1;
	anData[SRCH_LVL2] = pstBallInfo->nSrchLvl2;		//v3.1.T262 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
	anData[SRCH_SPEED1] = pstBallInfo->nSrchSpeed1;
#else
	if ((theMcConfig.GetAcsBufferType() == 1)
		|| (theMcConfig.GetSrchSpeedStep() == 1))	//v3.0.T23 add
		anData[SRCH_SPEED1] = -pstBallInfo->nSrchSpeed1 * 500;	//v1.6.T18 add
	else
		anData[SRCH_SPEED1] = -pstBallInfo->nSrchSpeed1 * 2500;	//5000;
#endif
    anData[SRCH_TOL1] = pstBallInfo->nSrchTol1;

	double dForceFactor = 1000.0;	//v3.1.T293 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
	dForceFactor = 1.0;
#endif
    if (m_bDryRun)
    {
        anData[PRE_IMPACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nSrchLvl1) * dForceFactor);
        anData[CTACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nSrchLvl1) * dForceFactor);
    }
    else
    {
        anData[PRE_IMPACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nPreCtactForce[0], pstBallInfo->nCtactLvl1) * dForceFactor);
        anData[CTACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nCtactForce[0], pstBallInfo->nCtactLvl1) * dForceFactor);
    }

    anData[PRE_IMPACT_TIME1] = pstBallInfo->nPreCtactTime[0];
    anData[CTACT_TIME1] = pstBallInfo->nCtactTime[0];

    if (m_bDryRun)
        anData[BASE_FORCE1] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nSrchLvl1) * dForceFactor);
    else
        anData[BASE_FORCE1] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nBaseForce[0], pstBallInfo->nCtactLvl1) * dForceFactor);

    anData[BASE_TIME1]   = pstBallInfo->nBaseTime[0];
    
    if (m_bDryRun)
        anData[RLS_FORCE1] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nSrchLvl1) * dForceFactor);
    else
        anData[RLS_FORCE1] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nReleaseForce[0], pstBallInfo->nCtactLvl1) * dForceFactor);
    
    anData[RLS_TIME1] = pstBallInfo->nReleaseTime[0];
    anData[REV_HT] = pstBallInfo->nBaseHt;
    anData[REV_HT2] = 0;	//v3.1.T350 add
	anData[REV_DIST2_X] = 0;	//v3.1.T575 add
	anData[REV_DIST2_Y] = 0;	//v3.1.T575 add
#if _SERVO_TYPE == 2
    anData[REV_DIST_HT] = 0;	//v3.1.T555 add
#endif

	if (pstBallInfo->chBumpShapeMode == BUMP_SHAPE_STANDARD)
	{
		anData[KINK2_ENABLE]  = 0;
		anData[KINK2_HT]      = 0;
		anData[KINK2_DIST_X]  = 0;
		anData[KINK2_DIST_Y]  = 0;
#if _SERVO_TYPE == 2
		anData[KINK2_DIST_HT] = 0;	//v3.1.T538 add
#endif
		anData[KINK2_HT2]     = 0;	//v3.1.T341 add
		anData[KINK2_DIST2_X] = 0;	//v3.1.T375 add
		anData[KINK2_DIST2_Y] = 0;	//v3.1.T375 add
		anData[TRAJ_DIST_Y]   = 0;
		anData[REV_DIST_X]    = round(pstBallInfo->stBondPos[1].dx - pstBallInfo->stBondPos[0].dx);
		anData[REV_DIST_Y]    = round(pstBallInfo->stBondPos[1].dy - pstBallInfo->stBondPos[0].dy);
		anData[TRAJ_DIST_X]   = pstBallInfo->nSpanHt;
	}
	else if (pstBallInfo->chBumpShapeMode == BUMP_SHAPE_FAST)
	{
		anData[KINK2_ENABLE]  = 0;
		anData[KINK2_HT]      = 0;
		anData[KINK2_DIST_X]  = 0;
		anData[KINK2_DIST_Y]  = 0;
#if _SERVO_TYPE == 2
		anData[KINK2_DIST_HT] = 0;	//v3.1.T538 add
#endif
		anData[KINK2_HT2]     = 0;	//v3.1.T341 add
		anData[KINK2_DIST2_X] = 0;	//v3.1.T375 add
		anData[KINK2_DIST2_Y] = 0;	//v3.1.T375 add
		anData[TRAJ_DIST_Y]   = 0;
		anData[REV_DIST_X]    = round(pstBallInfo->stScrubPos.dx - pstBallInfo->stBondPos[0].dx);
		anData[REV_DIST_Y]    = round(pstBallInfo->stScrubPos.dy - pstBallInfo->stBondPos[0].dy);
		anData[TRAJ_DIST_X]   = pstBallInfo->nSpanHt;
	}
	else
	{
		anData[KINK2_ENABLE]  = 1;
		anData[KINK2_HT]      = pstBallInfo->nFlexHt;
		anData[KINK2_DIST_X]  = round(pstBallInfo->stFlexPos.dx - pstBallInfo->stBondPos[0].dx);
		anData[KINK2_DIST_Y]  = round(pstBallInfo->stFlexPos.dy - pstBallInfo->stBondPos[0].dy);
#if _SERVO_TYPE == 2
		anData[KINK2_DIST_HT] = 0;	//v3.1.T538 add
#endif
		anData[KINK2_HT2]     = 0;	//v3.1.T341 add
		anData[KINK2_DIST2_X] = 0;	//v3.1.T375 add
		anData[KINK2_DIST2_Y] = 0;	//v3.1.T375 add
		anData[TRAJ_DIST_Y]   = pstBallInfo->nFlexSpanHt;
		anData[REV_DIST_X]    = round(pstBallInfo->stBondPos[1].dx - pstBallInfo->stFlexPos.dx);
		anData[REV_DIST_Y]    = round(pstBallInfo->stBondPos[1].dy - pstBallInfo->stFlexPos.dy);
		anData[TRAJ_DIST_X]   = pstBallInfo->nSpanHt;
	}
    anData[SLOPE_LENGTH] = pstBallInfo->nBaseHt2;	//v3.1.T330 edit
#if _SERVO_TYPE == 2	//v3.1.T460 add
	if (theMcConfig.GetBumpBaseHt2() > 0)
		anData[SRCH_SPEED2] = 0;
	else
		anData[SRCH_SPEED2] = pstBallInfo->nSrchSpeed2;
#else
	if (theMcConfig.GetBumpBaseHt2() > 0)	//v3.1.T345 edit, v3.1.T347 edit
		anData[SRCH_SPEED2] = 0;	//v3.1.T330 edit
	else if ((theMcConfig.GetAcsBufferType() == 1)
		|| (theMcConfig.GetSrchSpeedStep() == 1))	//v3.0.T23 add
		anData[SRCH_SPEED2] = -pstBallInfo->nSrchSpeed2 * 500;	//v1.6.T18 add
	else
		anData[SRCH_SPEED2] = -pstBallInfo->nSrchSpeed2 * 2500;
#endif
    anData[SRCH_TOL2] = pstBallInfo->nSrchTol2;

    if (m_bDryRun)
    {
        anData[PRE_IMPACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nCtactLvl2) * dForceFactor);
        anData[CTACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nCtactLvl2) * dForceFactor);
    }
    else
    {
        anData[PRE_IMPACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nPreCtactForce[1], pstBallInfo->nCtactLvl2) * dForceFactor);
        anData[CTACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nCtactForce[1], pstBallInfo->nCtactLvl2) * dForceFactor);
    }

    anData[PRE_IMPACT_TIME2] = pstBallInfo->nPreCtactTime[1];
	anData[CTACT_TIME2] = pstBallInfo->nCtactTime[1];

    if (m_bDryRun)
        anData[BASE_FORCE2] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nCtactLvl2) * dForceFactor);
    else
        anData[BASE_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nBaseForce[1], pstBallInfo->nCtactLvl2) * dForceFactor);
    
    anData[BASE_TIME2]   = pstBallInfo->nBaseTime[1];
    
    if (m_bDryRun)
        anData[RLS_FORCE2] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nCtactLvl2) * dForceFactor);
    else
        anData[RLS_FORCE2] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->nReleaseForce[1], pstBallInfo->nCtactLvl2) * dForceFactor);

    anData[RLS_TIME2]    = pstBallInfo->nReleaseTime[1];
    anData[TRAJ_POINT]   = pstBallInfo->nMotionSpeed;
    anData[TAIL_HEIGHT]  = pstBallInfo->nTailHt;
    anData[RESET_LVL]    = pstBallInfo->nResetLvl;
    
    if (theDebug.IsDebugWaveform()
		|| theMcConfig.IsDebugModeWaveform()	//v3.1.T462 add
		|| theDebug.IsDebugWireClamp()
		|| theDebug.IsDebugSpWaveform())
    {
        if (theDebug.IsDebugSpWaveform())
            anData[SCOPE_ENABLE] = 2;
        else
        {
            if (m_nWaveformCaptureCount == 0)
                anData[SCOPE_ENABLE] = 1;
            else
                anData[SCOPE_ENABLE] = 0;

            m_nWaveformCaptureCount++;
            if (m_nWaveformCaptureCount > theMcConfig.GetWaveformCaptureCount())	//v3.0.T468 edit
                m_nWaveformCaptureCount = 0;
        }
    }
    else
        anData[SCOPE_ENABLE] = 0;

    anData[FIRST_MOTION_PROFILE_X] = pstBallInfo->nFirstMotionProfileX;
    
    if (theDebug.IsDebugStepMotion())
        anData[MOTION_STEP_DEBUG] = 1;
    else
        anData[MOTION_STEP_DEBUG] = 0;

    anData[NEXT_WIRE_EXIST]  = pstBallInfo->nNextWireExist;
    anData[SPARK_DELAY]      = pstBallInfo->nSparkDelay;
    anData[BOND_SHIFT_SPEED] = 100;
    anData[BOND_SHIFT1_X]    = round(pstBallInfo->stBondPos[0].dx);
    anData[BOND_SHIFT1_Y]    = round(pstBallInfo->stBondPos[0].dy);
    anData[BOND_SHIFT2_X]    = round(pstBallInfo->stScrubPos.dx);
    anData[BOND_SHIFT2_Y]    = round(pstBallInfo->stScrubPos.dy);
    anData[PULL_POS_X]       = round(pstBallInfo->stBondPos[1].dx);
    anData[PULL_POS_Y]       = round(pstBallInfo->stBondPos[1].dy);
    anData[TAIL_CREEP_X]     = round(pstBallInfo->stScrubPos.dx);
    anData[TAIL_CREEP_Y]     = round(pstBallInfo->stScrubPos.dy);
    anData[SRCH_DELAY1]      = pstBallInfo->nSrchDelay[0];
    anData[SRCH_DELAY2]      = pstBallInfo->nSrchDelay[1];
    anData[WCL_TAIL_CLOSE_PRETRIG]    = pstBallInfo->nWclTailClosePretrig;
    anData[WCL_PEAK_CLOSE_PRETRIG]    = pstBallInfo->nWclLoopPeakClosePretrig;
    anData[WCL_1ST_BOND_OPEN_PRETRIG] = pstBallInfo->nWcl1stBondOpenPretrig;
    anData[WCL_2ND_BOND_OPEN_PRETRIG] = pstBallInfo->nWcl2ndBondOpenPretrig;

    anData[SCRUB2_ENABLE]   = 0;
    anData[SCRUB2_SPEED]    = 100;
    anData[SCRUB2_CYCLE]    = 1;
    anData[SCRUB2_FORCE]    = 0;
    anData[SCRUB2_TIME]     = 0;
    anData[SCRUB2_POS_X]    = round(pstBallInfo->stBondPos[1].dx);
    anData[SCRUB2_POS_Y]    = round(pstBallInfo->stBondPos[1].dy);
    anData[SCRUB2_OFFSET_X] = 0;
    anData[SCRUB2_OFFSET_Y] = 0;
    anData[SCRUB2_AMP_X]    = 0;
    anData[SCRUB2_AMP_Y]    = 0;

    anData[BOND_SHIFT1_ENABLE]    = 0;
    if (m_bDryRun)
    {
        anData[SRCH_LMT1] = -20000;
        anData[SRCH_LMT2] = -20000;
    }
    else
    {
        anData[SRCH_LMT1] = pstBallInfo->nCtactLvl1 - 500;
        anData[SRCH_LMT2] = pstBallInfo->nCtactLvl2 - 500;
    }
	//v1.6.T16 add
	anData[INT_WIRE_NUM]  = ((nWireNum - 1) % thePgm.GetUnitTotalWire()) + 1;	//v1.6.T24 edit
	anData[BUMP_TYPE] = pstBallInfo->nBumpType;

	anData[BALL_BOND_ENABLE]      = 1;
    anData[LOOP_TOP_MODE]         = 0;
    anData[ABOVE_LOOP_TOP_POS_X]  = 0;
    anData[ABOVE_LOOP_TOP_POS_Y]  = 0;
    anData[ABOVE_LOOP_TOP_HEIGHT] = 0;
    anData[REV_HT_PREMOVE]    = 0;
    anData[REV_DIST_PREMOVE]  = 0;
    anData[KINK2_HT_PREMOVE]   = 0;
    anData[KINK2_DIST_PREMOVE] = 0;
    anData[LOOP_TOP_PREMOVE]   = 0;
    anData[REV_DIST_HT_POINT]  = pstBallInfo->nSpanDistZPoint;
    anData[KINK2_DIST_HT_POINT] = 0;
	anData[PRE_KINK_DIST_HT_POINT] = 0;	//v3.1.T326 add
	anData[ARC_REV_DELAY] = 0;			//v3.1.T407 add
	anData[ARC_PRE_KINK_DELAY] = 0;		//v3.1.T328 add
	anData[ARC_KINK2_DELAY] = 0;		//v3.1.T337 add
	anData[ARC_KINK3_DELAY] = 0;		//v3.1.T337 add
	anData[ARC_SPAN_DELAY] = pstBallInfo->nArcSpanDelay;	//v3.1.T332 add
	anData[BUMP_SPAN_LEN_DELAY] = pstBallInfo->nSpanLenDelay;	//v3.1.T384 add

	//v3.1.T227 add
    anData[PRE_KINK_HT_PREMOVE]   = 0;
    anData[PRE_KINK_DIST_PREMOVE] = 0;

	anData[PRE_KINK_ENABLE]  = 0;
    anData[PRE_KINK_HT]      = 0;
    anData[PRE_KINK_DIST_X]  = 0;
    anData[PRE_KINK_DIST_Y]  = 0;
    anData[PRE_KINK_DIST_HT] = 0;
    anData[PRE_KINK_HT2]     = 0;	//v3.1.T342 add
	anData[PRE_KINK_DIST2_X] = 0;	//v3.1.T575 add
	anData[PRE_KINK_DIST2_Y] = 0;	//v3.1.T575 add

	anData[KINK3_ENABLE]  = 0;
	anData[KINK3_HT]      = 0;
	anData[KINK3_DIST_X]  = 0;
	anData[KINK3_DIST_Y]  = 0;
#if _SERVO_TYPE == 2
	anData[KINK3_DIST_HT] = 0;	//v3.1.T538 add
#endif
	anData[KINK3_HT2]     = 0;	//v3.1.T341 add
	anData[KINK3_DIST2_X] = 0;	//v3.1.T375 add
	anData[KINK3_DIST2_Y] = 0;	//v3.1.T375 add
	anData[KINK3_HT_PREMOVE]   = 0;
	anData[KINK3_DIST_PREMOVE] = 0;
	anData[KINK3_DIST_HT_POINT] = 0;

    anData[TWIST_ENABLE] = 0;
    anData[TWIST_HT] = 0;
    anData[TWIST_REV_DIST_X] = 0;
    anData[TWIST_REV_DIST_Y] = 0;
    anData[REV_HT4_PREMOVE] = 0;
    anData[TWIST_REV_DIST_Z_POINT] = 0;

    anData[TWIST_FWD_DIST_X] = 0;
    anData[TWIST_FWD_DIST_Y] = 0;
    anData[TWIST_FWD_DIST_PREMOVE] = 0;
    anData[TWIST_FWD_DIST_Z_POINT] = 0;

    if (thePgm.IsCopperSequence())
        anData[COPPER_SEQUENCE] = 1;
    else
        anData[COPPER_SEQUENCE] = 0;

    MACHINE_DATA* pstMcData = theMcData.GetRcd();
    anData[DAMPGAIN] = pstMcData->nDampGain;
    anData[SEARCH_MODE] = pstMcData->nSearchMode;
    anData[RCD_SAMPLE] = pstMcData->chRcdSample;
    anData[RCD_TOL] = pstMcData->chRcdTol;

    if (pstBallInfo->b1stBondScrubEnable == true)
        anData[SCRUB1_ENABLE] = 1;
    else
        anData[SCRUB1_ENABLE] = 0;
    anData[SCRUB1_SPEED] = pstBallInfo->n1stScrubSpeed;
    anData[SCRUB1_CYCLE] = pstBallInfo->n1stScrubCycle;
    if (m_bDryRun)
        anData[SCRUB1_FORCE] = round(theMcData.Calc3PtBondForceDac(DRYRUN_FORCE, pstBallInfo->nCtactLvl1) * dForceFactor);
    else
        anData[SCRUB1_FORCE] = round(theMcData.Calc3PtBondForceDac(-pstBallInfo->n1stScrubForce, pstBallInfo->nCtactLvl1) * dForceFactor);
    anData[SCRUB1_TIME] = pstBallInfo->n1stScrubTime;
    anData[SCRUB1_AMP_X] = round(pstBallInfo->st1stScrubAmp.dx);
    anData[SCRUB1_AMP_Y] = round(pstBallInfo->st1stScrubAmp.dy);

    anData[DAMPGAIN2] = pstMcData->nDampGain2;
    anData[FAB_TEST] = 0;

    anData[TAIL_BREAK_DIST]  = pstBallInfo->nTailBreakDist;
    anData[TAIL_BREAK_SPEED] = pstBallInfo->nTailBreakSpeed * 2500;

    short nCtrlBlkX, nCtrlBlkY;
    theServo.GetTableCtrlBlock(pstBallInfo->stBondPos[0], &nCtrlBlkX, &nCtrlBlkY);

    anData[TABLE_X_CTRL_BLOCK] = nCtrlBlkX;
    anData[TABLE_Y_CTRL_BLOCK] = nCtrlBlkY;
    
    anData[FIRST_MOTION_PROFILE_Y] = pstBallInfo->nFirstMotionProfileY;

    anData[PRE_IMPACT_SHIFT_X] = round(pstBallInfo->stBondPos[1].dx);
    anData[PRE_IMPACT_SHIFT_Y] = round(pstBallInfo->stBondPos[1].dy);

	//v3.1.T34 delete
	//if (pstBallInfo->chFabType == 0)
	//	anData[FAB_TYPE] = FAB_TYPE_1;
	//else
	//	anData[FAB_TYPE] = FAB_TYPE_2;
	anData[FAB_TYPE] = pstBallInfo->chFabType;	//v3.1.T34 add

    anData[SCRUB1_Y_DELAY] = pstBallInfo->n1stScrubYDelay;

    if (pstBallInfo->bTailBondEnable == true)
        anData[TAIL_BOND_ENABLE] = 1;
    else
        anData[TAIL_BOND_ENABLE] = 0;

    anData[REV_HT_SPEED]         = pstBallInfo->nMotionSpeed;
    anData[REV_DIST_SPEED]       = pstBallInfo->nMotionSpeed;
    anData[PRE_KINK_HT_SPEED]    = pstBallInfo->nMotionSpeed;
    anData[PRE_KINK_DIST_SPEED]  = pstBallInfo->nMotionSpeed;
    anData[KINK2_HT_SPEED]       = 100;
    anData[KINK2_DIST_SPEED]     = 100;
    anData[KINK3_HT_SPEED]       = 100;
    anData[KINK3_DIST_SPEED]     = 100;
    anData[TWIST_HT_SPEED]       = 100;
    anData[TWIST_REV_DIST_SPEED] = 100;
    anData[TWIST_FWD_DIST_SPEED] = 100;
    anData[TRAJ_SPEED]           = 100;
    anData[PULL_SPEED]           = 100;

    anData[FAST_LOOP] = 0;

	//v1.6.T135 add
	if (theMcConfig.GetAcsBufferType() == 1)
	{
		FORCE_SWITCH_PARA* pstForceSwitch = theForceSwitch.GetRcd();
		//anData[FORCE_SWITCH]  = pstForceSwitch->iForceSwitch;   //v1.6.P136sc edit, v3.1.T341 delete
		//anData[IMPACT_SPEED1] = pstForceSwitch->nSpeed1;	//v3.1.T341 delete
		//anData[IMPACT_SPEED2] = pstForceSwitch->nSpeed2;	//v3.1.T341 delete
		//anData[IMPACT_SPEED3] = pstForceSwitch->nSpeed3;	//v3.1.T337 delete
		//anData[IMPACT_MODE1]  = pstForceSwitch->nMode1;	//v3.1.T337 delete
		//anData[IMPACT_MODE2]  = pstForceSwitch->nMode2;	//v3.1.T332 delete
		//anData[IMPACT_MODE3]  = pstForceSwitch->nMode3;	//v3.1.T328 delete
	}

	//v1.6.T254 add, v3.1.T315 delete
	//anData[SOFTSRCH1_ENABLE]        = (int)theMcConfig.IsSoftSrchEnable(0);
	//anData[SOFTSRCH1_SPEED]         = theMcConfig.GetSoftSrchSpeed(0);
	//anData[SOFTSRCH1_POSERR]        = theMcConfig.GetSoftSrchPosErr(0);
	//anData[SOFTSRCH1_POSERR_SERIES] = theMcConfig.GetSoftSrchPosErrSeries(0);
	//anData[SOFTSRCH2_ENABLE]        = (int)theMcConfig.IsSoftSrchEnable(1);
	//anData[SOFTSRCH2_SPEED]         = theMcConfig.GetSoftSrchSpeed(1);
	//anData[SOFTSRCH2_POSERR]        = theMcConfig.GetSoftSrchPosErr(1);
	//anData[SOFTSRCH2_POSERR_SERIES] = theMcConfig.GetSoftSrchPosErrSeries(1);

	//v3.1.T315 add
	anData[RH_POS_GAIN]       = 0;
	anData[RH_VEL_GAIN]       = 0;
	anData[RH_VEL_INTEGRATOR] = 0;
	anData[RH_AFF]            = 0;
	anData[RD_POS_GAIN]       = 0;
	anData[RD_VEL_GAIN]       = 0;
	anData[RD_VEL_INTEGRATOR] = 0;
	anData[RD_AFF]            = 0;
	if (theMcConfig.IsControlGroupEnable())
	{
		short nLoopMode = theWire.GetRcd(nWireNum)->nLoopMode;
		if (nLoopMode > 0)	//v3.1.T316 add
		{
			anData[RH_POS_GAIN]       = theMcConfig.GetPosGainRH(nLoopMode);
			anData[RH_VEL_GAIN]       = theMcConfig.GetVelGainRH(nLoopMode);
			anData[RH_VEL_INTEGRATOR] = theMcConfig.GetVelIntegratorRH(nLoopMode);
			anData[RH_AFF]            = theMcConfig.GetAffRH(nLoopMode);
			anData[RD_POS_GAIN]       = theMcConfig.GetPosGainRD(nLoopMode);
			anData[RD_VEL_GAIN]       = theMcConfig.GetVelGainRD(nLoopMode);
			anData[RD_VEL_INTEGRATOR] = theMcConfig.GetVelIntegratorRD(nLoopMode);
			anData[RD_AFF]            = theMcConfig.GetAffRD(nLoopMode);
		}
	}

	anData[TAIL_DELAY] = theMcConfig.GetTailDelay();	//v3.0.T307 add

	lStatus = theServo.SendIntDataToServo(anData, MAX_INT_VARIABLE, m_nBondBuffer);
    if (lStatus != FOK) return lStatus;
    //Sleep(1);

    lStatus = theServo.SendRealDataToServo(pstBallInfo->adSpanDistZMotion,
			pstBallInfo->nSpanDistZPoint, m_nBondBuffer, SERVO_VAR_REV_DIST_HT);
    if (lStatus != FOK) return lStatus;
    //Sleep(1);

    return lStatus;
}

// Trigger servo to bonding.
int CBond::TriggerBonding(short nBumpSeq)	//v3.1.T94 edit
{
	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		CString str;
		str.Format(_T("TriggerBonding_%d"), m_nBondBuffer);
		theDebug.AddInfo(str);
	}

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_TriggerBonding_Buf"), m_nBondBuffer);
	if (theServo.TriggerBuffer(m_nBondBuffer) != FOK)
	{
		SetErrNo(5);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerBonding_Err"), BOND_SERVO_ERROR);	//v3.1.T406 add
		return BOND_SERVO_ERROR;
	}

    StartStickDetect(nBumpSeq);
    
    if (m_nBondBuffer == 0)
        m_nBondBuffer = 1;
    else
        m_nBondBuffer = 0;

    if (m_nBondType == BONDTYPE_RESTART)
        m_nBondType = BONDTYPE_NORMAL;
    
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_TriggerBonding_Err"), FOK);	//v3.1.T406 edit
    return FOK;
}

int CBond::CheckBondResult(short nBumpSeq, bool bNextWireExist)	//v3.1.T506 edit
{
    if (theMcConfig.IsBypassAll()) return FOK;

	int lServoStatus = CheckBondProgress(6);
	theApp.SetRedLamp(true);

	if (theUSG.CheckWrtStatus() == -1)
	{
		theUSG.ResetProfileBuffer();
		if (InitUsgProfile() == BOND_WIRE_LENGTH_ERROR)
			return BOND_WIRE_LENGTH_ERROR;
	}

    if (lServoStatus == FOK)
    {
		ST_BUMP_BALL stBumpBall;
		const WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);
		if ((nBumpSeq == BUMPSEQ_BALL)
			|| (nBumpSeq == BUMPSEQ_BALL2))	//v3.1.T103 add
		{
			switch (pstWire->nBumpType)	//v3.1.T103 add
			{
			case BUMP_BOS:
				theBallPara.GetBbosBall(&stBumpBall);
				break;
			case BUMP_SOB:
				theBallPara.GetBsobBall(&stBumpBall);
				break;
			case BUMP_BSB:	//v3.1.T103 add
				if (nBumpSeq == BUMPSEQ_BALL)	//BSOB
					theBallPara.GetBsobBall(&stBumpBall);
				else	//BBOS
					theBallPara.GetBbosBall(&stBumpBall);
				break;
			}
		}

        BOOL bCheckNsop = m_bCheckNsop && pstWire->bCheckNsop 
						&& (nBumpSeq == BUMPSEQ_WIRE);

		BOOL bCheckNsol;
        if (nBumpSeq == BUMPSEQ_WIRE)
            bCheckNsol = m_bCheckNsol && pstWire->bCheckNsop;
        else
            bCheckNsol = m_bCheckNsol && pstWire->bCheckNsop
							&& stBumpBall.bStickDetect;

        if (bCheckNsop || bCheckNsol)
        {
			BOOL bTailBreak = FALSE;
            if (nBumpSeq == BUMPSEQ_WIRE)
            {
				switch (pstWire->nBumpType)	//v3.1.T88 add
				{
				case BUMP_SOB:
				case BUMP_BSB:	//v3.1.T103 add
				{
				    ST_BUMP_WIRE stBumpWire;
					theBallPara.GetBsobWire(&stBumpWire);
					bTailBreak = stBumpWire.bTailBreakEnable;
					break;
				}
				default:
					bTailBreak = theBondPara.GetRcd()->bTailBreakEnable;
				}
            }
            else
                bTailBreak = stBumpBall.bTailBreakEnable;

			MACHINE_DATA* pstMcData = theMcData.GetRcd();
            lServoStatus = theBSD.CheckStickResult(
							bCheckNsop, bCheckNsol, bTailBreak,
							pstMcData->nNonStickSample,
							pstMcData->nTailBreakNonStickSample,
							pstMcData->nTailShortSample,
							pstMcData->bStickCoarseDetect,
							pstMcData->nStickCoarseDetectSamples,
							&m_nBondStickSample);
        }

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("BsdCount"), theBSDDebug.GetFalseAlmCnter());

        if (m_bDryRun && (lServoStatus != BOND_BSD_COMMUNICATION_ERROR))
            lServoStatus = FOK;

        if (m_bDryRun == false)
        {
            short nBondUnit = theMHS.GetCurBondUnit();
			short nCtactBufferIndex = thePgm.CalcCtactBufferIndex(m_nCurWire,
										m_nCurRow, m_nCurCol, nBondUnit);
            double dContact1 = 0.0, dContact2 = 0.0;
			int nCtactStatus;

			if ((pstWire->nBumpType == BUMP_SOB)
				|| (pstWire->nBumpType == BUMP_BSB))	//v3.1.T103 add
            {
                if (theDebug.IsDebugContactLevel())
                {
                    if (m_nCurWire == 1)
                        theDebug.AddInfo(_T("Unit"), nBondUnit);
                }

				switch (nBumpSeq)	//v3.1.T94 add
				{
				case BUMPSEQ_BALL:
				{
					if (m_nBondBuffer == 1)
						theServo.ReadMemoryReal(MEMVAR_BUFFER0_CTACT1, &dContact1);
					else
						theServo.ReadMemoryReal(MEMVAR_BUFFER1_CTACT1, &dContact1);

					//v3.0.T21 edit
					nCtactStatus = theCtact.AddRcd(nCtactBufferIndex, SECOND_BOND, round(dContact1));
					if (theMcConfig.IsCtactLowErr(SECOND_BOND)
						&& (nCtactStatus == 2))
						lServoStatus = BONDERR_2ND_CTACT_LOW;
					else if (theMcConfig.IsCtactHighErr(SECOND_BOND)
						&& (nCtactStatus == 3))
						lServoStatus = BONDERR_2ND_CTACT_HIGH;
					break;
				}
				case BUMPSEQ_WIRE:
				{
					if (m_nBondBuffer == 1)
						theServo.ReadMemoryReal(MEMVAR_BUFFER0_CTACT1, &dContact1);
					else
						theServo.ReadMemoryReal(MEMVAR_BUFFER1_CTACT1, &dContact1);

					//v3.0.T21 edit
					nCtactStatus = theCtact.AddRcd(nCtactBufferIndex, FIRST_BOND, round(dContact1));
					if (theMcConfig.IsCtactLowErr(FIRST_BOND)
						&& (nCtactStatus == 2))
						lServoStatus = BONDERR_1ST_CTACT_LOW;
					else if (theMcConfig.IsCtactHighErr(FIRST_BOND)
						&& (nCtactStatus == 3))
						lServoStatus = BONDERR_1ST_CTACT_HIGH;
					break;
				}
				}
            }
            else if (nBumpSeq == BUMPSEQ_WIRE)
            {
                if (m_nBondBuffer == 1)
                {
                    theServo.ReadMemoryReal(MEMVAR_BUFFER0_CTACT1, &dContact1);
                    theServo.ReadMemoryReal(MEMVAR_BUFFER0_CTACT2, &dContact2);
                }
                else
                {
                    theServo.ReadMemoryReal(MEMVAR_BUFFER1_CTACT1, &dContact1);
                    theServo.ReadMemoryReal(MEMVAR_BUFFER1_CTACT2, &dContact2);
                }

                if (theDebug.IsDebugContactLevel())
                {
                    if (m_nCurWire == 1)
                        theDebug.AddInfo(_T("Unit"), nBondUnit);
                }

				//v3.0.T21 edit
				nCtactStatus = theCtact.AddRcd(nCtactBufferIndex, FIRST_BOND, round(dContact1));
				if (theMcConfig.IsCtactLowErr(FIRST_BOND)
					&& (nCtactStatus == 2))
					lServoStatus = BONDERR_1ST_CTACT_LOW;
				else if (theMcConfig.IsCtactHighErr(FIRST_BOND)
					&& (nCtactStatus == 3))
					lServoStatus = BONDERR_1ST_CTACT_HIGH;
				if (lServoStatus != FOK)
					theCtact.AddRcd(nCtactBufferIndex, SECOND_BOND, round(dContact2));
				else
				{
					//v3.0.T21 edit
					nCtactStatus = theCtact.AddRcd(nCtactBufferIndex, SECOND_BOND, round(dContact2));
					if (theMcConfig.IsCtactLowErr(SECOND_BOND)
						&& (nCtactStatus == 2))
						lServoStatus = BONDERR_2ND_CTACT_LOW;
					else if (theMcConfig.IsCtactHighErr(SECOND_BOND)
						&& (nCtactStatus == 3))
						lServoStatus = BONDERR_2ND_CTACT_HIGH;
				}
            }
        }
    }

    if (lServoStatus == FOK)
		lServoStatus = CheckBondProgress(7, 1);
    else
		CheckBondProgress(7, 2);

	//if (lServoStatus == FOK)	//v3.0.T385 delete
		theServo.ResetBondProgress();

	if (theDebug.IsDebugBondTiming()
		|| (theDebug.IsDebugWireCycleTime()
			&& (theMcConfig.GetWireCycleTime() < 2)))
	{
		CString sInfo;
		sInfo.Format(_T("W%d_%.3fms"), m_nCurWire,
						m_WireCycleTimer.GetTime_x1ms());
		theDebug.AddInfo(sInfo);
	}
	theApp.SetRedLamp(false);
    
	if ((m_nBondMode != BONDMODE_AUTO)	//v3.1.T483 edit
		&& !bNextWireExist)	//v3.1.T506 add
		Sleep(50);	//v3.1.T470 add

    if (thePgm.IsCopperSequence() == false)
    {
        if ((lServoStatus == FOK) && (m_bDryRun == false) && m_bCheckFab)
        {
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_IsSparkSuccess_1"));	//v3.1.T469 add
			if (theMcConfig.IsEfoErrIO())	//v3.1.T434 add
			{
				if (theServo.GetInput(0) == 0)	//efo fail
				{
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("IoErr"));	//v3.1.T471 add
					//if (theEFO.IsSparkSuccess() == false)	//v3.1.T500 delete
					{
						lServoStatus = BOND_NEFO_FAIL;
						//ShowDebugMsg(DEBUG_BOND_TIMING, _T("SparkFail"));	//v3.1.T469 add, v3.1.T500 delete
					}
				}
			}
			else
			{
				if (theEFO.IsSparkSuccess() == false)	//v1.6.T338 add
				{
					lServoStatus = BOND_NEFO_FAIL;
					ShowDebugMsg(DEBUG_BOND_TIMING, _T("SparkFail"));	//v3.1.T469 add
				}
			}
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_IsSparkSuccess_1"));	//v3.1.T469 add
        }
    }

    if ((lServoStatus == FOK) && (nBumpSeq == BUMPSEQ_WIRE))
	{
		if (thePgm.IsWireFeedCheck())
		{
			if (theBSD.CheckWireFeedStatus(thePgm.GetWireFeedTimeout()) == FNOK)
				lServoStatus = BOND_NO_WIRE_FEED_ERROR;
		}
	}

    return lServoStatus;
}

// Inform stop auto bonding
void CBond::InformAutoBondStop(void)
{
	//v3.1.T461 add
	CBusy bs(&m_bBusy);
	if (m_bRequestStopBond || m_bStopBondReady) return;
	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		CString str;
		str.Format(_T("StopAutoBond_%d"), m_nCurWire);
		theDebug.AddInfo(str);
	}

	//m_nBondMode = BONDMODE_STOP;
	m_bStopBondReady = false;	//v3.1.T461 add
    m_bRequestStopBond = true;
}

EN_BOND_MODE CBond::GetBondMode(void)
{
	return m_nBondMode;
}

void CBond::SetBondMode(EN_BOND_MODE nMode)
{
	m_nBondMode = nMode;
}

void CBond::MoveBondHeadToFireLevel(void)
{
	if (theMcConfig.IsBhMoveWait())	//v3.1.T459 add
		theServo.MoveAxisZ(theMcData.GetRcd()->lResetLevel, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
	else
		theServo.MoveAxisZ(theMcData.GetRcd()->lResetLevel, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
}

void CBond::MoveBondHeadToRelaxPosition(void)
{
	if (theMcConfig.IsBhMoveWait())	//v3.1.T459 add
		theServo.MoveAxisZ(theMcData.GetRcd()->lRelaxLevel, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
	else
		theServo.MoveAxisZ(theMcData.GetRcd()->lRelaxLevel, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
}

void CBond::LoadBondData(void)
{
    ST_BOND_DATA stBondData;
    theDataMgr.CopyFromBuffer((char *)&stBondData, sizeof(ST_BOND_DATA));
    m_nCurWire			= stBondData.nCurWire;
    m_nRestartWire		= stBondData.nRestartWire;
    m_nCurRow			= stBondData.nCurRow;
    m_nCurCol			= stBondData.nCurCol;
    m_bCheckNsop		= stBondData.bCheckNsop;
    m_bCheckNsol		= stBondData.bCheckNsol;
	m_bCheckFab			= stBondData.bCheckFab;
    m_bDryRun			= stBondData.bDryRun;
    m_bBondNoVision		= stBondData.bBondNoVision;
    m_bAutoIndex		= stBondData.bAutoIndex;
    m_bCompleteBond		= stBondData.bCompleteBond;
    m_nCurTrack			= stBondData.nCurTrack;
    m_bIll				= stBondData.bIll;
    m_nEndWire			= theWire.GetTotalWireNum(); 
    m_bAutoDefineBond	= false;
    m_bDryRun			= false;
}

void CBond::SaveBondData(void)
{
    ST_BOND_DATA stBondData;
    stBondData.nCurWire			= m_nCurWire;
    stBondData.nRestartWire		= m_nRestartWire;
    stBondData.nCurRow			= m_nCurRow;
    stBondData.nCurCol			= m_nCurCol;
    stBondData.bCheckNsop		= m_bCheckNsop;
    stBondData.bCheckNsol		= m_bCheckNsol;
    stBondData.bCheckFab		= m_bCheckFab;
    stBondData.bDryRun			= m_bDryRun;
    stBondData.bBondNoVision	= m_bBondNoVision;
    stBondData.bAutoIndex		= m_bAutoIndex;
    stBondData.bCompleteBond	= m_bCompleteBond;
    stBondData.nCurTrack		= m_nCurTrack;
    stBondData.bIll				= m_bIll;
	//stBondData.nEndWire		= m_nEndWire;
    theDataMgr.CopyToBuffer((char *)&stBondData, sizeof(ST_BOND_DATA));
}

void CBond::ResetBondData(void)		//v1.5.T135 edit
{
    m_nCurWire		= 1;
    m_nRestartWire	= 1;
    m_nBondedWire	= 0;
    m_nCurRow		= 1;
    m_nCurCol		= 1;
    m_bCheckNsop	= true;
    m_bCheckNsol	= true;
    m_bCheckFab		= true;
    m_bDryRun		= false;
    m_bBondNoVision	= false;
    m_bAutoIndex	= true;
    m_bCompleteBond	= false;
    m_bIll			= false;
    m_nCurMatRow	= 1;
    m_nCurMatCol	= 1;

    if (thePgm.GetTrackType() == OUTER_TRACK)
        m_nCurTrack = OUTER_TRACK;
    else
        m_nCurTrack = INNER_TRACK;

	m_nEndWire = theWire.GetTotalWireNum();
    m_bAutoDefineBond = false;

    SaveBondData();
}

// Prepare bonding before enter auto bond menu
int CBond::PrepareBonding(void)
{
    int lStatus = FNOK;

    if (m_nCurWire > thePgm.GetTotalWire())
        m_nCurWire = thePgm.GetTotalWire();

    //ResetMatrixPrStatus(m_nCurWire);
    //SetBondObjectSequenceMode(BUMPOPT_NORMAL);

	theMHS.CloseWindowClamp();

	if ((theMHS.GetCurBondUnit() > 0) || (m_bAutoIndex == false))
	{
		lStatus = thePgm.IsMultiPr() ? PerformMultiPrAlignment(false) : PerformVisionAlignment();
		lStatus = (lStatus == QUIT_AUTO_BOND ? FNOK : FOK);

        theVisionSys.EnableVideo(true);
        theScanKeyThread.EnableJoyStick();
        theVisionSys.ClearDrawing();
		theVisionDisplay.EnableDisplay(true); 
    	
        //MoveToWirePoint(m_nCurWire, FIRST_BOND);
	}

    SendTableSpeedProfileToServo();    
    Sleep_x1ms(30);
    SendBondHeadSpeedProfileToServo();
    Sleep_x1ms(30);
    theServo.SendServoCtrlProfileToServo();
    Sleep_x1ms(30);

	return lStatus;
}

//v3.1.T50 clean
void CBond::MoveToWirePoint(short nWireNum, short nBondPt, bool bSlowSpeed, bool bUpdateLight)
{
	if (nWireNum > theWire.GetTotalWireNum()) return;

	if (bUpdateLight)
	{
		const WIRE_RECORD *pstWire = theWire.GetRcd(nWireNum);
		const REF_RECORD *pstRef = theRef.GetRcd(pstWire->nRefNum[nBondPt]);
		theVisionSys.UpdateLightingParameter(
				pstRef->stManuAlignLightPara[0].nCoaxRedLight,
				pstRef->stManuAlignLightPara[0].bSideRedLight,
				pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
				pstRef->stManuAlignLightPara[0].bSideBlueLight);
	}

	TBL_POS astBondPt[2];
	theWire.CalcBondPoint(nWireNum, &astBondPt[0], &astBondPt[1]);
	theServo.MoveTable2(astBondPt[nBondPt], MTN_ABS, MTN_WAIT, bSlowSpeed);
}

int CBond::DieAheadPR(void)
{
    if (((thePgm.GetDeviceSeq() != PGM_DIE_AHEAD)
		&& (thePgm.GetDeviceSeq() != PGM_DIE_LEAD_AHEAD)
		&& (thePgm.GetDeviceSeq() != PGM_MAT_DIE_AHEAD))
		|| m_bBondNoVision)
	{
		return FOK;
	}

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_DieAheadPR"));
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("beg_DieAheadPR"));

    int nStatus = FOK;
    TBL_POS stPrPos, stVisionOffset, stCurTblPos;
    MATRIX_RECORD stAheadMatrix;
    PR_SRCH_RESULT stPrSrchResult;
    ST_WIRE_INFO stCurWireInfo;
    CString str;

	PGM_RECORD *pstPgm = thePgm.GetRcd();

    short nUnitTotalWire = thePgm.GetUnitTotalWire();
	if (nUnitTotalWire == 0) //throw "w.temp.43" ;
	{
		AfxMessageBox(_T("temp.43, nUnitTotalWire == 0"));
		return FOK;
	}
    short nUnitNum = (m_nCurWire - 1) / nUnitTotalWire + 1;
	short nCol, nRow;
	thePgm.GetRowCol(nUnitNum, &nRow, &nCol);

    if (nCol > thePgm.GetDeviceColNum() - pstPgm->nPrAheadCol)
	{
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("LastColumn"));
		goto QUIT;
	}

	short nAheadUnit = thePgm.GetUnitNum(nRow, nCol + pstPgm->nPrAheadCol);

	//v1.6.T350 add
	short nUnitWireNum = (m_nCurWire - 1) % nUnitTotalWire + 1;
	short nAheadWireNum;
	if (((pstPgm->nPrAheadCol % 2) == 1)		//look ahead odd col
		&& (theMcConfig.GetDeviceSeq() == 1))
		nAheadWireNum = (nAheadUnit * nUnitTotalWire) - nUnitWireNum + 1;
	//v3.0.T15 add
	else if (((pstPgm->nPrAheadCol % 2) == 1)	//look ahead odd col
		&& (theMcConfig.GetDeviceSeq() == 2))
	{
		if (nUnitWireNum == 1) nUnitWireNum = 2;
		else if (nUnitWireNum == 2) nUnitWireNum = 1;
		else if (nUnitWireNum == 4) nUnitWireNum = 5;
		else if (nUnitWireNum == 5) nUnitWireNum = 4;
		nAheadWireNum = (nAheadUnit * nUnitTotalWire) - nUnitWireNum + 1;
	}
	else	//look ahead even col
		nAheadWireNum = ((nAheadUnit - 1) * nUnitTotalWire) + nUnitWireNum;
    const WIRE_RECORD *pstWire = theWire.GetRcd(nAheadWireNum);
	const REF_RECORD *pstRef = theRef.GetRcd(pstWire->nRefNum[0]);
    theMatrix.GetRcd(&stAheadMatrix, pstWire->nRefNum[0]);
    
	//v3.1.T578 add
	if (theMHS.GetCurBondUnit() == theTopTrack.m_stIndexerVar.nTotalUnits)
	{
		if (nCol > thePgm.GetDeviceColNum() - pstPgm->nPrAheadCol
					- theMcConfig.GetSkipLastCol())
		{
			stAheadMatrix.nVisionStatus = VISION_SKIP_DIE;
			theMatrix.SetRcd(&stAheadMatrix, pstWire->nRefNum[0]);
			goto QUIT;
		}
	}

    if (stAheadMatrix.nVisionStatus != VISION_NOT_DONE)
	{
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("VisionDone"));
		goto QUIT;
	}

    nStatus = CalcWireInfo(m_nCurWire, stCurTblPos, &stCurWireInfo, str, false);
    if (nStatus != FOK)
	{
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("CalcWireInfo_Err"), nStatus);
		goto QUIT;
	}

    stPrPos = stCurWireInfo.stBondPos[0];
    if (abs(theBondPara.GetRcd()->nBondShift1) > 0)
        stPrPos.dx -= round((double)theBondPara.GetRcd()->nBondShift1 / theServo.GetResolXY());

    //theVisionSys.SetSearchSize(pstRef->nVisionSrchSizeX[0], pstRef->nVisionSrchSizeY[0]);
	theVisionSys.UpdateLightingParameter(
				pstRef->stVisionLightPara[0].nCoaxRedLight,
				pstRef->stVisionLightPara[0].bSideRedLight, 
				pstRef->stVisionLightPara[0].nCoaxBlueLight,
				pstRef->stVisionLightPara[0].bSideBlueLight);
	theVisionSys.ChangeLensMag(pstRef->nVisionAlignMag[0]);

	//v1.6.T259 add
    short nSearchRange, nPrPassScore;
	if (theMcConfig.GetDieAheadPR() > 0)
	{
		theVisionSys.EnableVideo(false);
		theVisionSys.SetPassScore(
			(short)pstRef->ulVisionAlignPatternID[0], pstPgm->nAutoSkipScore);
		if (pstRef->nRefType == REF_TYPE_LEAD)
		{
			nSearchRange = pstPgm->nLeadSearchRange;
			nPrPassScore = pstPgm->nLeadPrPassScore;
		}
		else
		{
			nSearchRange = pstPgm->nDieSearchRange;
			nPrPassScore = pstPgm->nPrPassScore;
		}
	}

	nStatus = CheckBondProgress(2);
    if (nStatus != FOK)
	{
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("CheckBondProgress(2)_Err"), nStatus);
		goto QUIT;
	}

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("beg_GrabImage"));
	if (theMcConfig.GetDieAheadPR() == 0)	//v1.6.T259 add
		theVisionSys.EnableVideo(false);
    theVisionSys.GrabImage();
    if (pstRef->nVisionAlgorithm[0] == 1)
        theVisionSys.UpdateBinaryThreshold(pstRef->stVisionLightPara[0].nImageThreshold);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("end_GrabImage"));

	if (theMcConfig.GetDieAheadPR() == 0)	//v1.6.T259 add
	{
	    if (pstRef->nRefType == REF_TYPE_LEAD)
		{
			nSearchRange = pstPgm->nLeadSearchRange;
	        nPrPassScore = pstPgm->nLeadPrPassScore;
		}
	    else
		{
			nSearchRange = pstPgm->nDieSearchRange;
	        nPrPassScore = pstPgm->nPrPassScore;
		}

		theVisionSys.SetPassScore(
			(short)pstRef->ulVisionAlignPatternID[0], pstPgm->nAutoSkipScore);
	}
	int nVisionError = theVisionSys.SearchVisionPattern(
						(short)pstRef->ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stPrSrchResult);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("PrStatus"), nVisionError);
	if (nVisionError == FOK)
	{
		if (theMcConfig.GetRandomSkip() > 0)
		{
			if ((rand() % theMcConfig.GetRandomSkip()) > 10)
			{
				nVisionError = FNOK;
				ShowDebugMsg(DEBUG_VISION_TIMING, _T("RandomSkip"));
			}
		}
	}
	else
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("AutoSkip"));

	if (nVisionError == FOK)
	{
        theVisionSys.VisionOffsetToTablePos(pstRef->nVisionAlignMag[0], &stPrSrchResult, &stVisionOffset);
                
        if (pstPgm->bDiePrMultiSrch == false)
        {
            stAheadMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
            stAheadMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
            stAheadMatrix.stVisionAlignPos[1].dx = stAheadMatrix.stVisionAlignPos[0].dx + pstRef->stVisionAlignPos[1].dx - pstRef->stVisionAlignPos[0].dx;
            stAheadMatrix.stVisionAlignPos[1].dy = stAheadMatrix.stVisionAlignPos[0].dy + pstRef->stVisionAlignPos[1].dy - pstRef->stVisionAlignPos[0].dy;

            if (pstRef->nLocateMode == REF_LOCATE_SINGLE_POINT)
            {
                stAheadMatrix.dCosAln = cos(stPrSrchResult.dAngle);
                stAheadMatrix.dSinAln = sin(stPrSrchResult.dAngle);
            }
            stAheadMatrix.nVisionStatus = VISION_SUCCESS;
            theMatrix.CalcMatrix(&stAheadMatrix, pstWire->nRefNum[0], VISION_PR_ALIGN);
            
			ShowDebugMsg(DEBUG_VISION_TIMING, _T("Update Matrix"), pstWire->nRefNum[0]);
        }
        else
        {
            stAheadMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
            stAheadMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;

			theVisionSys.SetPassScore(
				(short)pstRef->ulVisionAlignPatternID[1], pstPgm->nAutoSkipScore);
            nVisionError = theVisionSys.SearchVisionPattern(
							(short)pstRef->ulVisionAlignPatternID[1],
							nSearchRange, nPrPassScore, &stPrSrchResult);
            if (nVisionError == FOK)
            {
                theVisionSys.VisionOffsetToTablePos(pstRef->nVisionAlignMag[0], &stPrSrchResult, &stVisionOffset);
                stAheadMatrix.stVisionAlignPos[1].dx = stPrPos.dx + stVisionOffset.dx;
                stAheadMatrix.stVisionAlignPos[1].dy = stPrPos.dy + stVisionOffset.dy;

				double dRefAlignDist = Calc2PtDist(pstRef->stVisionAlignPos[0], pstRef->stVisionAlignPos[1]);
				double dMatrixAlignDist = Calc2PtDist(stAheadMatrix.stVisionAlignPos[0], stAheadMatrix.stVisionAlignPos[1]);
				double dAlignTol;
				if (pstRef->nRefType == REF_TYPE_LEAD)
					dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
				else
					dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

				if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					stAheadMatrix.nVisionStatus = VISION_FAILURE;
					theMatrix.SetRcd(&stAheadMatrix, pstWire->nRefNum[0]);
				}
				else
				{
					stAheadMatrix.nVisionStatus = VISION_SUCCESS;
					theMatrix.CalcMatrix(&stAheadMatrix, pstWire->nRefNum[0], VISION_PR_ALIGN);
				}

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("UpdateMatrix"), pstWire->nRefNum[0]);
            }
			else if (thePgm.IsAutoSkipUnit())
			{
				SetSkipUnit(pstWire->nRefNum[0], VISION_FAILURE);	//V1.1.W265 edit
			}
			else
			{
				stAheadMatrix.nVisionStatus = VISION_FAILURE;
				theMatrix.SetRcd(&stAheadMatrix, pstWire->nRefNum[0]);
			}
        }
    }
    else if (thePgm.IsAutoSkipUnit())
//			|| (pstPgm->nManualSkip == 1)) //V1.1.W268 edit, v1.6.T242 delete
	{
		SetSkipUnit(pstWire->nRefNum[0], VISION_FAILURE);	//V1.1.W265 edit
	}
	else if (thePgm.IsAutoSkipDie())	//v1.6.T242 edit
	{
		stAheadMatrix.nVisionStatus = VISION_FAILURE;
		theMatrix.SetRcd(&stAheadMatrix, pstWire->nRefNum[0]);
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("SkipRef"), pstWire->nRefNum[0]);
	}

	if ((thePgm.GetDeviceSeq() != PGM_DIE_LEAD_AHEAD) || m_bBondNoVision)
	{
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("NoVision"));
		goto QUIT;
	}

	pstRef = theRef.GetRcd(pstWire->nRefNum[1]);
	theVisionSys.UpdateLightingParameter(
				pstRef->stVisionLightPara[0].nCoaxRedLight, 
				pstRef->stVisionLightPara[0].bSideRedLight, 
				pstRef->stVisionLightPara[0].nCoaxBlueLight,
				pstRef->stVisionLightPara[0].bSideBlueLight);
	theVisionSys.ChangeLensMag(pstRef->nVisionAlignMag[0]);

QUIT:
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_DieAheadPR_Err"), nStatus);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("end_DieAheadPR_Err"), nStatus);
    return nStatus;
}

int CBond::CheckBondProgress(short nProgress, short nIndex, bool bShowDebugMsg)
{
	if (theMcConfig.IsBypassAll()) return FOK;

	//v3.0.T445 add
	if (bShowDebugMsg)
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_Progress_Index"), nProgress, nIndex);

	int nBondProgress = 0, nTemp;
    long lTimeOut = 5000;
    if (theDebug.IsDebugStepMotion()) lTimeOut = 3600000;	//v3.1.T482 edit - 1 hour
	CHTimer timer;
	do
	{
        if (theServo.GetBondProgress(&nTemp) == FOK)
			nBondProgress = nTemp;

		ProcessKeyboard();	//v2.0.T38 add
    } while ((nBondProgress < nProgress)
				&& (timer.GetTime_x1ms() < lTimeOut)
				&& !(m_bRequestStopBond && theDebug.IsDebugStepMotion()));	//v3.1.T482 add

	if (bShowDebugMsg)
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_Progress"), nProgress, nBondProgress);
	if (nBondProgress < nProgress)
	{
		SetErrBondProgress(nProgress, nBondProgress);
		m_sTemp.Format(_T("%s_BondTimeout_%d_%d"),
			CTime::GetCurrentTime().Format(_T("%Y%m%d_%H%M%S")),
			nProgress, nBondProgress);
		theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		return BOND_TIMEOUT;
	}

	return FOK;
}

// Check if PR alignment has done
// return value
// 0: Align not done
// 1: Align done
// 2: Wire number out of range
short CBond::CheckAheadAlignDone(short* pnWireNum, bool bForward)
{
    if (*pnWireNum > thePgm.GetTotalWire()) return 2;

    short i;
    WIRE_RECORD* pstWire;
    MATRIX_RECORD *pstMatrix1, *pstMatrix2;

    if ((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_SINGLE)
		|| (thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
    {
        pstWire = theWire.GetRcd(*pnWireNum);
        pstMatrix1 = theMatrix.GetRcd(pstWire->nRefNum[0]);

        if ((pstMatrix1->nVisionStatus == VISION_NOT_DONE)
			|| (pstMatrix1->nVisionStatus == VISION_FAILURE))
            return 0;

        if (pstMatrix1->nVisionStatus == VISION_SKIP_DIE)
		{
            if (bForward == true)
            {
                for (i = *pnWireNum + 1; i <= thePgm.GetTotalWire(); i++)
                {
                    pstWire = theWire.GetRcd(i);
                    pstMatrix1 = theMatrix.GetRcd(pstWire->nRefNum[0]);
                    if (pstMatrix1->nVisionStatus != VISION_SKIP_DIE)
                        break;
                }
    
                *pnWireNum = i;
                if (*pnWireNum > thePgm.GetTotalWire())
                {
                    *pnWireNum = thePgm.GetTotalWire();
                    return 2;
                }
            }
            else
            {
                for (i = *pnWireNum - 1; i > 0; i--)
                {
                    pstWire = theWire.GetRcd(i);
                    pstMatrix1 = theMatrix.GetRcd(pstWire->nRefNum[0]);
                    if (pstMatrix1->nVisionStatus != VISION_SKIP_DIE)
                        break;
                }

                *pnWireNum = i;
                if (*pnWireNum < 1)
                {
                    *pnWireNum = 1;
                    return 2;
                }
            }

            if (pstMatrix1->nVisionStatus == VISION_SUCCESS)
                return 1;
            else
                return 0;
        }
    }
    else if ((thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
    {
        pstWire = theWire.GetRcd(*pnWireNum);
        pstMatrix1 = theMatrix.GetRcd(pstWire->nRefNum[0]);
        pstMatrix2 = theMatrix.GetRcd(pstWire->nRefNum[1]);

        if ((pstMatrix1->nVisionStatus == VISION_NOT_DONE)
			|| (pstMatrix1->nVisionStatus == VISION_FAILURE)
			|| (pstMatrix2->nVisionStatus == VISION_NOT_DONE)
			|| (pstMatrix2->nVisionStatus == VISION_FAILURE))
            return 0;

        if ((pstMatrix1->nVisionStatus == VISION_SKIP_DIE)
			|| (pstMatrix2->nVisionStatus == VISION_SKIP_DIE))
        {
            if (bForward == true)
            {
                for (i = *pnWireNum + 1; i <= thePgm.GetTotalWire(); i++)
                {
                    pstWire = theWire.GetRcd(i);
                    pstMatrix1 = theMatrix.GetRcd(pstWire->nRefNum[0]);
                    pstMatrix2 = theMatrix.GetRcd(pstWire->nRefNum[1]);
                    if ((pstMatrix1->nVisionStatus != VISION_SKIP_DIE) &&
                        (pstMatrix2->nVisionStatus != VISION_SKIP_DIE))
                        break;
                }
    
                *pnWireNum = i;
                if (*pnWireNum > thePgm.GetTotalWire())
                {
                    *pnWireNum = thePgm.GetTotalWire();
                    return 2;
                }
            }
            else
            {
                for (i = *pnWireNum - 1; i > 0; i--)
                {
                    pstWire = theWire.GetRcd(i);
                    pstMatrix1 = theMatrix.GetRcd(pstWire->nRefNum[0]);
                    pstMatrix2 = theMatrix.GetRcd(pstWire->nRefNum[1]);
                    if ((pstMatrix1->nVisionStatus != VISION_SKIP_DIE) &&
                        (pstMatrix2->nVisionStatus != VISION_SKIP_DIE))
                        break;
                }

                *pnWireNum = i;
                if (*pnWireNum < 1)
                {
                    *pnWireNum = 1;
                    return 2;
                }
            }

            if (pstMatrix1->nVisionStatus == VISION_SUCCESS)
                return 1;
            else
                return 0;
        }
    }

    return 1;
}

int CBond::SendTableSpeedProfileToServo(void)
{
    double dSpeedProfile[18];
    SERVO_SPEED_PROFILE stServoSpeedProfile;

	short i;
    for (i = 0; i < 6; i++)
    {
        theServo.GetSpeedProfile(MOTOR_X, i, &stServoSpeedProfile);
        dSpeedProfile[i * 3 + 0] = stServoSpeedProfile.nMaxVel / theServo.GetResolXY() * 1E3;
        dSpeedProfile[i * 3 + 1] = stServoSpeedProfile.nMaxAcc / theServo.GetResolXY() * 1E6;
        dSpeedProfile[i * 3 + 2] = stServoSpeedProfile.lJerk / theServo.GetResolXY() * 1E6;
    }
    int nStatus = theServo.SendRealDataToServo(dSpeedProfile, 3, ACSC_NONE, SERVO_VAR_SPEED_X);
    if (nStatus != FOK) return nStatus;
    
    for (i = 0; i < 6; i++)
    {
        theServo.GetSpeedProfile(MOTOR_Y, i, &stServoSpeedProfile);
        dSpeedProfile[i * 3 + 0] = stServoSpeedProfile.nMaxVel / theServo.GetResolXY() * 1E3;
        dSpeedProfile[i * 3 + 1] = stServoSpeedProfile.nMaxAcc / theServo.GetResolXY() * 1E6;
        dSpeedProfile[i * 3 + 2] = stServoSpeedProfile.lJerk / theServo.GetResolXY() * 1E6;
    }
    nStatus = theServo.SendRealDataToServo(dSpeedProfile, 3, ACSC_NONE, SERVO_VAR_SPEED_Y);
    if (nStatus != FOK) return nStatus;

    return FOK;
}

int CBond::SendBondHeadSpeedProfileToServo(void)
{
	double dSpeedProfile[15];
	SERVO_SPEED_PROFILE stServoSpeedProfile;

	for (short i = 0; i < 5; i++)
	{
		theServo.GetSpeedProfile(MOTOR_Z, i + 1, &stServoSpeedProfile);
		dSpeedProfile[i * 3 + 0] = stServoSpeedProfile.nMaxVel / theServo.GetResolZ() * 1E3;
		dSpeedProfile[i * 3 + 1] = stServoSpeedProfile.nMaxAcc / theServo.GetResolZ() * 1E6;
		dSpeedProfile[i * 3 + 2] = stServoSpeedProfile.lJerk / theServo.GetResolZ() * 1E6;
	}

	int nStatus = theServo.SendRealDataToServo(dSpeedProfile, 3, ACSC_NONE, SERVO_VAR_SPEED_Z);
	if (nStatus != FOK) return nStatus;

	return FOK;
}

// Get current bonding track
short CBond::GetCurrentTrack(void)
{
    return m_nCurTrack;
}

void CBond::Init(void)
{
    theSparkStateRelay.Initialize(NEFO_STATE_RELAY);

	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		m_N2h2Port.Initialize(theStepperIO.m_Output.m_N2h2Port.nPort);
		m_AirTensioner.Initialize(theStepperIO.m_Output.m_AirTensioner.nPort);
		m_PanelLight.Initialize(theStepperIO.m_Output.m_PanelLight.nPort);
		m_ThermalSensorX.Initialize(theStepperIO.m_Input.m_ThermalSensorX.nPort);
		m_ThermalSensorY.Initialize(theStepperIO.m_Input.m_ThermalSensorY.nPort);
	}
	else
	{
		m_N2h2Port.Initialize(STEP_OUTPUT_N2H2_PORT);
		m_AirTensioner.Initialize(STEP_OUTPUT_AIR_TENSIONER);
		m_PanelLight.Initialize(OUTPUT_PANEL_LIGHT_PORT);
		m_ThermalSensorX.Initialize(TBL_THERMAL_PROTECT_X);
		m_ThermalSensorY.Initialize(TBL_THERMAL_PROTECT_Y);
	}

	if (!theMcConfig.IsBypassMHS())		//v3.1.T172 add
	{
		m_N2h2Port.SetRelayState(FALSE, true);
    	m_PanelLight.SetRelayState(FALSE);
	}

    int iErrorStatus = theBSD.Init();
    if (!theMcConfig.IsBypassAll())
    {
        if (iErrorStatus != BSD_COMM_OK)
        {
		    CString str;
            switch (iErrorStatus)
            {
            case BSD_COMM_NETWORK_ERROR:
                str = theMsg.GetMsg(MSG_BSD_NETWORK_ERR);
                break;
            case BSD_COMM_SOCKET_ERROR:
                str = theMsg.GetMsg(MSG_BSD_SOCKET_ERR);
                break;
            case BSD_COMM_CONNECT_ERROR:
                str = theMsg.GetMsg(MSG_BSD_COMM_ERR);
                break;
            case BSD_COMM_VERSION_ERROR:
                str = theMsg.GetMsg(MSG_BSD_VERSION_ERR);
                break;
            case BSD_COMM_EFO_INIT_ERROR:
                str = theMsg.GetMsg(MSG_EFO_INIT_ERR);
                break;
            default:
                break;
            }
            
		    CStringList sList;
            sList.AddTail(str);
            theMsg.ShowMsg(ERROR_MSG, sList);
        }
    }

    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
	{
		theTowerLight.Init();
		theTowerLight.EnableBlinkRedLight(false);
		theTowerLight.EnableIdleLight();

		//theServo.ClearAllBuffer();		//v3.0.T91 delete
		theServo.DownloadBufferProgram();	//v3.0.T91 rename

		theVisionDisplay.SetDisplayMode(VISION_DISPLAY_NORMAL_MODE);
		theVisionDisplay.EnableDisplay(true);
		theVisionDisplay.SetDisplayWire(m_nCurWire);
		theVisionDisplay.SetDisplayBondPt(FIRST_BOND);
		Sleep(10);

		SendTableSpeedProfileToServo();    
		Sleep(30);

		SendBondHeadSpeedProfileToServo();
		Sleep(30);

		theServo.SendServoCtrlProfileToServo();
		Sleep(30);
	}

	ResetMinMaxRef();	//V1.5.W145 add
}

void CBond::StartStickDetect(short nBumpSeq)	//v3.1.T94 edit
{
    if (!theMcConfig.IsBypassAll())
    {
        //if (nBumpSeq == BUMPSEQ_BALL)
        //	theBSD.StartStickDetect(1);
        //else
            theBSD.StartStickDetect(0);
    }

    //if (m_nStickDetectBlock == 0)
    //    m_nStickDetectBlock = 1;
    //else
    //    m_nStickDetectBlock = 0;
}

int CBond::UpdateBondTipOffset(short nBondedWireNum)
{
    TBL_POS st1stBondPt, st2ndBondPt, stUpdateBtos;
	REF_RECORD *pstRef;		//v3.1.T99 edit
	WIRE_RECORD *pstWire;	//v3.1.T99 edit
	short i, nUnitTotalDie, nUnitDieNum;

    if (nBondedWireNum <= theWire.GetTotalWireNum())
    {
		theWire.CalcBondPoint(nBondedWireNum, &st1stBondPt, &st2ndBondPt);
		theServo.MoveTable2(st1stBondPt);	//v3.1.T52 edit
		theVisionSys.EnableVideo(true);

        CPositionAdjustDlg Dlg(BTOS_DLG);
		Dlg.DoModal();
		if (Dlg.m_bSelectCancel) return FOK;
			
        theServo.GetTablePosition(&st2ndBondPt);

        if (!theMcConfig.IsBypassAll())
        {
			if (thePgm.GetMultiBtos() != BTOS_DIR_DISABLE)
			{
				bool bCtactValid = false;
				int nWireCtactLvl;
				short nCtactBufferIndex = thePgm.CalcCtactBufferIndex(nBondedWireNum,
											m_nCurRow, m_nCurCol,
											theMHS.GetCurBondUnit());

				pstWire = theWire.GetRcd(nBondedWireNum);
				pstRef = theRef.GetRcd(pstWire->nRefNum[0]);
				
				bCtactValid = theCtact.GetCtactLvl(nCtactBufferIndex, FIRST_BOND, &nWireCtactLvl);
				
				stUpdateBtos.dx = pstRef->stBtos.dx + (st1stBondPt.dx - st2ndBondPt.dx);
				stUpdateBtos.dy = pstRef->stBtos.dy + (st1stBondPt.dy - st2ndBondPt.dy);

				if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
					|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
				{
					nUnitTotalDie = thePgm.GetUnitTotalDie();

					if (nUnitTotalDie == 1)
						nUnitDieNum = 1;
					else
					    nUnitDieNum = pstWire->nRefNum[0] % nUnitTotalDie;

					if (pstWire->nRefNum[0] > 0)
					{
						if (nUnitDieNum == 1)
						{
							pstRef = theRef.GetRcd(0);
							pstRef->stBtos.dx += (st1stBondPt.dx - st2ndBondPt.dx);
							pstRef->stBtos.dy += (st1stBondPt.dy - st2ndBondPt.dy);
						}

						for (i = 1; i < theRef.GetTotalRefNum(); i++)
						{
							if (nUnitTotalDie == 1)
							{
								pstRef = theRef.GetRcd(i);
								pstRef->stBtos.dx = stUpdateBtos.dx;
								pstRef->stBtos.dy = stUpdateBtos.dy;
								if (bCtactValid)
									pstRef->lCtactLvl = nWireCtactLvl;
							}
							else if ((i % nUnitTotalDie) == nUnitDieNum)
							{
								//nRefNum = m_nCurrentRefNum + i * thePgm.GetUnitTotalDie();
								pstRef = theRef.GetRcd(i);
								pstRef->stBtos.dx = stUpdateBtos.dx;
								pstRef->stBtos.dy = stUpdateBtos.dy;
								if (bCtactValid)
									pstRef->lCtactLvl = nWireCtactLvl;
							}
						}
					}
				}
				else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
				{
					nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
					nUnitDieNum = pstWire->nRefNum[0] % nUnitTotalDie;

					for (i = 0; i < theRef.GetTotalRefNum(); i++)
					{
						if ((i % nUnitTotalDie) == nUnitDieNum)
						{
							pstRef = theRef.GetRcd(i);
							pstRef->stBtos.dx = stUpdateBtos.dx;
							pstRef->stBtos.dy = stUpdateBtos.dy;
							if (bCtactValid)
								pstRef->lCtactLvl = nWireCtactLvl;
						}
						else if (i % nUnitTotalDie == 0)
						{
							if (nUnitDieNum == 1)
							{
								pstRef = theRef.GetRcd(i);
								pstRef->stBtos.dx += (st1stBondPt.dx - st2ndBondPt.dx);
								pstRef->stBtos.dy += (st1stBondPt.dy - st2ndBondPt.dy);
							}
						}
					}
				}
				else
				{
					pstRef = theRef.GetRcd(pstWire->nRefNum[0]);
					pstRef->stBtos.dx = stUpdateBtos.dx;
					pstRef->stBtos.dy = stUpdateBtos.dy;
					if (bCtactValid)
						pstRef->lCtactLvl = nWireCtactLvl;

					if (pstWire->nRefNum[0] == 1)
					{
						pstRef = theRef.GetRcd(0);
						pstRef->stBtos.dx += (st1stBondPt.dx - st2ndBondPt.dx);
						pstRef->stBtos.dy += (st1stBondPt.dy - st2ndBondPt.dy);
					}
				}

			    MACHINE_DATA *pstMcData = theMcData.GetRcd();
				pstMcData->stBondTipOs.dx = stUpdateBtos.dx;
				pstMcData->stBondTipOs.dy = stUpdateBtos.dy;
			}
			else
			{
			    MACHINE_DATA *pstMcData = theMcData.GetRcd();
				pstMcData->stBondTipOs.dx += (st1stBondPt.dx - st2ndBondPt.dx);
				pstMcData->stBondTipOs.dy += (st1stBondPt.dy - st2ndBondPt.dy);

				theRef.UpdateAllBtos(pstMcData->stBondTipOs);
		    }
          
        
//***********************Support lead BTOS Adjustment*****************************************************

			if (m_b2ndPointBtosEnable && (thePgm.GetMultiBtos() != BTOS_DIR_DISABLE))
			{
				m_b2ndPointBtosEnable = false;

				pstRef = theRef.GetRcd(0);

				theWire.CalcBondPoint(nBondedWireNum, &st1stBondPt, &st2ndBondPt);
				st1stBondPt.dx = st2ndBondPt.dx;
				st1stBondPt.dy = st2ndBondPt.dy;
				theServo.MoveTable2(st1stBondPt);	//v3.1.T52 edit

				theVisionSys.UpdateLightingParameter(
					pstRef->stManuAlignLightPara[0].nCoaxRedLight,
					pstRef->stManuAlignLightPara[0].bSideRedLight,
					pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
					pstRef->stManuAlignLightPara[0].bSideBlueLight);

				theVisionSys.EnableVideo(true);

				CPositionAdjustDlg dlg;
				dlg.DoModal();
				if (dlg.m_bSelectCancel) return FOK;

				theServo.GetTablePosition(&st2ndBondPt);
			
				stUpdateBtos.dx = pstRef->stBtos.dx + (st1stBondPt.dx - st2ndBondPt.dx);
				stUpdateBtos.dy = pstRef->stBtos.dy + (st1stBondPt.dy - st2ndBondPt.dy);

				if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
				{
					nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
					nUnitDieNum = 0;

					for (i = 0; i < theRef.GetTotalRefNum(); i++)
					{
						if ((i % nUnitTotalDie) == nUnitDieNum)
						{
							pstRef = theRef.GetRcd(i);
							pstRef->stBtos.dx = stUpdateBtos.dx;
							pstRef->stBtos.dy = stUpdateBtos.dy;
						}
					}
				}
				else
				{
					pstRef = theRef.GetRcd(0);
					pstRef->stBtos.dx = stUpdateBtos.dx;
					pstRef->stBtos.dy = stUpdateBtos.dy;
				}
			}
		}
	}

    return FOK;
}

int CBond::Set2ndPointBtosEnable(bool bEnable)
{
	m_b2ndPointBtosEnable = bEnable;

	return 0;
}

// Help message call back function
int CBond::HelpMsgCallBackFunction(short nCallBackFunctionId)
{
	switch (nCallBackFunctionId)
	{
	//case MSG_CALLBACK_ONLOAD_EMPTY:
	//    theVledOnload.ResetOnloadEmptyFlag();
	//    break;
	//case MSG_CALLBACK_OFFLOAD_FORK_FULL:
	//    break;
	case MSG_CALLBACK_AUTOSAMPLING_ONOK:
	{
		double dDieVolt;
		double dLeadVolt;
		theBSDDebug.CaculateAvePara(&dDieVolt, &dLeadVolt);
		if ((round(dDieVolt) > 1) && (round(dLeadVolt) > 1))
		{
			MACHINE_DATA *pstMcData = theMcData.GetRcd();
			pstMcData->fDieShortThreshold = (float)dDieVolt;
			pstMcData->fLeadShortThreshold = (float)dLeadVolt;

			theBSD.SetStickPara((BYTE)pstMcData->fDieShortThreshold,
								(BYTE)pstMcData->fLeadShortThreshold,
								pstMcData->nNonStickSample,
								pstMcData->nTailShortSample);
		}

		theBSDDebug.ResetSampling();
		theBSDDebug.SetAutoSamplingFlag(false);
		theBSDDebug.ResetFalseAlmcnter();
		break;
	}
	case MSG_CALLBACK_DOUBLE_LF_HANDLE:
	{
		theVledTrack.SearchHome();
		Sleep(10);
		theVledTrack.CheckHomeState();
		break;
	}
	case MSG_CALLBACK_OFFLOAD_JAM:
	{
		theTopTrack.m_EjectSensor.UpdateSensorState();                     
		if (theTopTrack.m_EjectSensor.GetSensorState() == FALSE)
		{
			theTopTrack.MoveIndexerToLogicZero();
			Sleep(10);
			theTopTrack.WaitMoveIndexer();

			TOP_TRACK_DATA stTrackData;
			theTopTrack.GetRcd(&stTrackData);
			theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot()
							+ 1 + stTrackData.iOutputMagazineSlotSequence);
			theTopOffloader.WaitMoveMagHolder();
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

// Perform lead ahead pattern recognition
int CBond::PerformLeadAheadPR(void)
{
    int lErrorStatus, lVisionError, lServoStatus;
    short nUnitTotalWire, nCol, nRow, nUnitNum, nAheadWireNum, nSearchRange, nPrPassScore, nAheadSubUnit;
    TBL_POS stPrPos, stVisionOffset, stCurTblPos;
    WIRE_RECORD stWireRcd;
    REF_RECORD stAheadRef;
    MATRIX_RECORD stAheadMatrix;
    PR_SRCH_RESULT stPrSrchResult;
    ST_WIRE_INFO stCurWireInfo;
    CString str;

    PGM_RECORD *pstPgm = thePgm.GetRcd();

    if ((thePgm.GetDeviceSeq() != PGM_DIE_LEAD_AHEAD)
		|| m_bBondNoVision)
    {
        return FOK;
    }

    nUnitTotalWire = thePgm.GetUnitTotalWire();

    nUnitNum = (m_nCurWire - 1) / nUnitTotalWire + 1;
    thePgm.GetRowCol(nUnitNum, &nRow, &nCol);

    if (nCol > pstPgm->nDeviceColNum - pstPgm->nPrAheadCol)
        return FOK;
    
	nAheadSubUnit = thePgm.GetUnitNum(nRow, nCol + pstPgm->nPrAheadCol);
    nAheadWireNum = m_nCurWire + (nAheadSubUnit - nUnitNum) * nUnitTotalWire;
    
    theWire.GetRcd(&stWireRcd, nAheadWireNum);
    theRef.GetRcd(&stAheadRef, stWireRcd.nRefNum[1]);
    theMatrix.GetRcd(&stAheadMatrix, stWireRcd.nRefNum[1]);

    if (stAheadMatrix.nVisionStatus != VISION_NOT_DONE)
        return FOK;

    lErrorStatus = CalcWireInfo(m_nCurWire, stCurTblPos, &stCurWireInfo, str, false);
    if (lErrorStatus != FOK)
	{
		if(lErrorStatus==BOND_WIRE_LENGTH_ERROR)return lErrorStatus;	//V1.1.W254 add
        return FOK;		//140923 Tony why??
	}

    stPrPos = stCurWireInfo.stBondPos[1];

    //theVisionSys.SetSearchSize(stAheadRef.nVisionSrchSizeX[0], stAheadRef.nVisionSrchSizeY[0]);
    theVisionSys.UpdateLightingParameter(stAheadRef.stVisionLightPara[0].nCoaxRedLight, 
        stAheadRef.stVisionLightPara[0].bSideRedLight, 
        stAheadRef.stVisionLightPara[0].nCoaxBlueLight,
        stAheadRef.stVisionLightPara[0].bSideBlueLight);
    theVisionSys.ChangeLensMag(stAheadRef.nVisionAlignMag[0]);

    lServoStatus = CheckBondProgress(5);
    if (lServoStatus != FOK) return lServoStatus;

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("beg_GrabImage"));

    theVisionSys.EnableVideo(false);
    theVisionSys.GrabImage();
    if (stAheadRef.nVisionAlgorithm[0] == 1)
        theVisionSys.UpdateBinaryThreshold(stAheadRef.stVisionLightPara[0].nImageThreshold);

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("end_GrabImage"));

    if (stAheadRef.nRefType == REF_TYPE_LEAD)
    {
        nSearchRange = pstPgm->nLeadSearchRange;
        nPrPassScore = pstPgm->nLeadPrPassScore;
    }
    else
    {
        nSearchRange = pstPgm->nDieSearchRange;
        nPrPassScore = pstPgm->nPrPassScore;
    }

	theVisionSys.SetPassScore(
					(short)stAheadRef.ulVisionAlignPatternID[0],
					pstPgm->nAutoSkipScore);

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stAheadRef.nRefNum);
	ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stAheadRef.nRefNum);
    lVisionError = theVisionSys.SearchVisionPattern(
					(short)stAheadRef.ulVisionAlignPatternID[0],
					nSearchRange, nPrPassScore, &stPrSrchResult);
	ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), lVisionError);
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), lVisionError);

    if (lVisionError == FOK)
    {
        theVisionSys.VisionOffsetToTablePos(stAheadRef.nVisionAlignMag[0], &stPrSrchResult, &stVisionOffset);
                
        stAheadMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
        stAheadMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
        stAheadMatrix.stVisionAlignPos[1].dx = stAheadMatrix.stVisionAlignPos[0].dx + stAheadRef.stVisionAlignPos[1].dx - stAheadRef.stVisionAlignPos[0].dx;
        stAheadMatrix.stVisionAlignPos[1].dy = stAheadMatrix.stVisionAlignPos[0].dy + stAheadRef.stVisionAlignPos[1].dy - stAheadRef.stVisionAlignPos[0].dy;

        if (stAheadRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
        {
            stAheadMatrix.dCosAln = cos(stPrSrchResult.dAngle);
            stAheadMatrix.dSinAln = sin(stPrSrchResult.dAngle);
        }
        
        stAheadMatrix.nVisionStatus = VISION_SUCCESS;
        theMatrix.CalcMatrix(&stAheadMatrix, stWireRcd.nRefNum[1], VISION_PR_ALIGN);

		ShowDebugMsg(DEBUG_VISION_TIMING, _T("Update Matrix"), stWireRcd.nRefNum[1]);
    }
    else
    {
        stAheadMatrix.nVisionStatus = VISION_FAILURE;
        theMatrix.SetRcd(&stAheadMatrix, stWireRcd.nRefNum[1]);
    }
    
    theRef.GetRcd(&stAheadRef, stWireRcd.nRefNum[0]);

    theVisionSys.UpdateLightingParameter(
				stAheadRef.stVisionLightPara[0].nCoaxRedLight, 
			    stAheadRef.stVisionLightPara[0].bSideRedLight, 
		        stAheadRef.stVisionLightPara[0].nCoaxBlueLight,
				stAheadRef.stVisionLightPara[0].bSideBlueLight);
    theVisionSys.ChangeLensMag(stAheadRef.nVisionAlignMag[0]);

    return FOK;
}

//v3.1.T443 edit - get reference number which need perform PR
void CBond::GetPerformPrRef(short nWireNum, short *pnStartRef, short *pnEndRef)
{
	short nStartRef, nLastRef;

	if (nWireNum == 1)
		nStartRef = 0;
	else
	{
		const WIRE_RECORD *pstWire = theWire.GetRcd(nWireNum);
		if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
		{
			if ((pstWire->nRefNum[0] == 0) || (pstWire->nRefNum[1] == 0))
			    nStartRef = max(pstWire->nRefNum[0], pstWire->nRefNum[1]);	//v3.1.T481 add
			else
				nStartRef = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);

			const MATRIX_RECORD *pstMat = theMatrix.GetRcd(0);
			if ((pstMat->nVisionStatus == VISION_NOT_DONE) ||
				(pstMat->nVisionStatus == VISION_FAILURE))
				nStartRef = 0;
		}
		else
			nStartRef = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);
	}
	nLastRef = theRef.GetTotalRefNum() - 1;

	short nUnitTotalWire, nUnitTotalDie, nUnitNum, nRow, nCol;
	PGM_RECORD *pstPgm = thePgm.GetRcd();
	if ((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie();
		nUnitTotalWire = thePgm.GetUnitTotalWire();

		if (pstPgm->nBondSeqOrientation == SEQUENCE_VERTICAL)
		{
			if (nUnitTotalWire == 0)
			{
				AfxMessageBox(_T("temp.44, nUnitTotalWire = 0"));
				return;
			}
			nUnitNum = (nWireNum - 1) / nUnitTotalWire + 1;
			thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
			nLastRef = (pstPgm->nPrAheadCol + nCol - 1) * pstPgm->nDeviceRowNum * nUnitTotalDie;
		}
		else
		{
			if (nUnitTotalWire == 0)
			{
				AfxMessageBox(_T("temp.45, nUnitTotalWire == 0"));
				return;
			}
			if ((nUnitTotalWire == 1)
				|| ((nUnitTotalWire > 1)
					&& (nWireNum % nUnitTotalWire == 1)))
				nLastRef = ((nWireNum - 1) / nUnitTotalWire + pstPgm->nPrAheadCol) * nUnitTotalDie;
			else
				nLastRef = ((nWireNum - 1) / nUnitTotalWire + pstPgm->nPrAheadCol) * nUnitTotalDie + 1;
		}
	}
	else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
		nUnitTotalWire = thePgm.GetUnitTotalWire();

		if (pstPgm->nBondSeqOrientation == SEQUENCE_VERTICAL)
		{
			if (nUnitTotalWire == 0)
			{
				AfxMessageBox(_T("temp.46, nUnitTotalWire == 0"));
				return;
			}
			nUnitNum = (nWireNum - 1) / nUnitTotalWire + 1;
			thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
			nLastRef = (pstPgm->nPrAheadCol + nCol - 1) * pstPgm->nDeviceRowNum * nUnitTotalDie;
		}
		else
		{
			if (nUnitTotalWire == 0)
			{
				AfxMessageBox(_T("temp.43, nUnitTotalWire == 0"));
				return;
			}
			if ((nUnitTotalWire == 1) || ((nUnitTotalWire > 1) && (nWireNum % nUnitTotalWire == 1)))
				nLastRef = ((nWireNum - 1) / nUnitTotalWire + pstPgm->nPrAheadCol - 1) * nUnitTotalDie + 1;
			else
				nLastRef = ((nWireNum - 1) / nUnitTotalWire + pstPgm->nPrAheadCol) * nUnitTotalDie + 1;
		}
	}

	if (nLastRef > (theRef.GetTotalRefNum() - 1))
		nLastRef = theRef.GetTotalRefNum() - 1;

	*pnStartRef = nStartRef;
	*pnEndRef   = nLastRef;
}

// Reset matrix Pr status with wire number
void CBond::ResetMatrixPrStatus(short nWireNum, bool bResetSkip)
{
    short i, nStartRef, nLastRef, nUnitTotalWire, nBkupVisionStatus;
    TBL_POS stStepRepeatOffset;

	if (m_nBondedWire == 0) ResetMinMaxRef();	//V1.5.W145 add

    if (nWireNum > 1)
        bResetSkip = false;

    if (nWireNum == 1)
        nStartRef = 0;
    else
    {
        if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
        {
            nUnitTotalWire = thePgm.GetUnitTotalWire();
            if ((nUnitTotalWire > 1) && ((nWireNum % nUnitTotalWire) != 1))
            {
				if (nUnitTotalWire == 0)
				{
					AfxMessageBox(_T("temp.48, nUnitTotalWire == 0"));
					return;
				}
                nWireNum = ((nWireNum - 1) / nUnitTotalWire + 1) * nUnitTotalWire + 1;
                if (nWireNum > thePgm.GetTotalWire())
                    nWireNum = thePgm.GetTotalWire();
            }
        }

		WIRE_RECORD stWireRcd;
        theWire.GetRcd(&stWireRcd, nWireNum);

        if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
        {
            if ((stWireRcd.nRefNum[0] == 0) || (stWireRcd.nRefNum[1] == 0))
                nStartRef = max(stWireRcd.nRefNum[0], stWireRcd.nRefNum[1]);
            else
                nStartRef = min(stWireRcd.nRefNum[0], stWireRcd.nRefNum[1]);
        }
        else
        {
            nStartRef = min(stWireRcd.nRefNum[0], stWireRcd.nRefNum[1]);
        }
    }

    nLastRef  = theRef.GetTotalRefNum() - 1;

    if (thePgm.GetDeviceSeq() == PGM_MATRIX)
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, m_nCurRow, m_nCurCol, &stStepRepeatOffset);
    else
        thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Reset Matrix"), nStartRef, nLastRef);

	REF_RECORD stRefRcd;
    MATRIX_RECORD stMatrixRcd;

    for (i = nStartRef; i <= nLastRef; i++)
    {
        theRef.GetRcd(&stRefRcd, i);
        theMatrix.GetRcd(&stMatrixRcd, i);

        nBkupVisionStatus = stMatrixRcd.nVisionStatus;

        stMatrixRcd.nVisionStatus = VISION_SUCCESS;
        stMatrixRcd.stVisionAlignPos[0].dx = stRefRcd.stVisionAlignPos[0].dx + stStepRepeatOffset.dx;
        stMatrixRcd.stVisionAlignPos[0].dy = stRefRcd.stVisionAlignPos[0].dy + stStepRepeatOffset.dy;
		stMatrixRcd.stVisionAlignPos[1].dx = stRefRcd.stVisionAlignPos[1].dx - stStepRepeatOffset.dx;
        stMatrixRcd.stVisionAlignPos[1].dy = stRefRcd.stVisionAlignPos[1].dy - stStepRepeatOffset.dy;

        theMatrix.CalcMatrix(&stMatrixRcd, i, VISION_PR_ALIGN);

        if (m_bBondNoVision) continue;
		if ((thePgm.IsLeadPrEnable() == false)
			&& (stRefRcd.nRefType != REF_TYPE_DIE)) continue;

		if ((nBkupVisionStatus == VISION_SKIP_DIE) && (bResetSkip == false))
			theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
		else if(i<m_nMinRef||i>m_nMaxRef)	//V1.5.W145 edit
			theMatrix.SetVisionStatus(i, VISION_NOT_DONE);
    }

	if (m_bBondNoVision == false)
		theMatrix.SetVisionStatus(0, thePgm.IsLeadPrEnable() ? VISION_NOT_DONE : VISION_SUCCESS);
}

void CBond::ResetSingleMatrixPr(short nRow, short nCol, short nWire)
{
	if (nWire < 1) return;

	m_nCurMatRow = m_nCurRow = nRow;
	m_nCurMatCol = m_nCurCol = nCol;

	m_nCurWire = m_nRestartWire = m_nEndWire = nWire;

	for(nWire=1; nWire<theRef.GetTotalRefNum(); nWire++)
		theMatrix.SetVisionStatus(nWire, VISION_SUCCESS);

	WIRE_RECORD stWireRcd;
	theWire.GetRcd(&stWireRcd, m_nCurWire);

	theMatrix.SetVisionStatus(stWireRcd.nRefNum[0], VISION_NOT_DONE);
	theMatrix.SetVisionStatus(stWireRcd.nRefNum[1], VISION_NOT_DONE);

	theMatrix.SetVisionStatus(0, thePgm.IsLeadPrEnable() ? VISION_NOT_DONE : VISION_SUCCESS);
}

void CBond::RedoPrAlignment(short nRwRow, short nRwCol, short nRwWire)
{
	m_bRedoPR = TRUE;	//V1.1.W250 add

	short nUnit, nUnitTotalWire, nWire;
	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_TOP_LED:
	{
		const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
		if ((pstDevice->nHalfIndexOn > 0)
			&& (theTopTrack.GetCurBondUnit() == pstDevice->nHalfIndexOn))
	    {
			nUnitTotalWire = thePgm.GetUnitTotalWire();
			nUnit = thePgm.GetTotalUnit() / 2 + 1;
			nWire = nUnitTotalWire * (nUnit - 1) + 1;
			ResetMatrixPrStatus(m_nRestartWire >= nWire ? m_nRestartWire : nWire, false);
		}
		else
			ResetMatrixPrStatus(m_nRestartWire, false);
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
		if ((pstDevice->nHalfIndexOn > 0)
			&& (theTEIndex.GetCurBondUnit() == pstDevice->nHalfIndexOn))
	    {
			nUnitTotalWire = thePgm.GetUnitTotalWire();
			nUnit = thePgm.GetTotalUnit() / 2 + 1;
			nWire = nUnitTotalWire * (nUnit - 1) + 1;
			ResetMatrixPrStatus(m_nRestartWire >= nWire ? m_nRestartWire : nWire, false);
		}
		else
			ResetMatrixPrStatus(m_nRestartWire, false);
		break;
	}
	case MHS_R2R:
	{
		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);
		if ((stTopDevicePara.nHalfIndexOn > 0)
			&& (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
	    {
			nUnitTotalWire = thePgm.GetUnitTotalWire();
			nUnit = thePgm.GetTotalUnit() / 2 + 1;
			nWire = nUnitTotalWire * (nUnit - 1) + 1;
			if (m_nRestartWire >= nWire)
                ResetMatrixPrStatus(m_nRestartWire, false);
            else
                ResetMatrixPrStatus(nWire, false);
		}
		else
			ResetMatrixPrStatus(m_nRestartWire, false);
		break;
	}
	case MHS_CU_ELITE:
	{
		CUELITE_DEVICE_RECORD stCuEliteDevicePara;
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
		if ((stCuEliteDevicePara.nHalfIndexOn > 0)
			&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
	    {
			nUnitTotalWire = thePgm.GetUnitTotalWire();
			nUnit = thePgm.GetTotalUnit() / 2 + 1;
			nWire = nUnitTotalWire * (nUnit - 1) + 1;

			if (m_nRestartWire >= nWire)
                ResetMatrixPrStatus(m_nRestartWire, false);
            else
                ResetMatrixPrStatus(nWire, false);
		}
		else
			ResetMatrixPrStatus(m_nRestartWire, false);
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	case MHS_FOXCONN:	//v3.0.T217 add
        ResetMatrixPrStatus(m_nRestartWire, false);
		break;
	}

	if (nRwWire > 0) ResetSingleMatrixPr(nRwRow, nRwCol, nRwWire);

    /*if ((lpstPgmRcd->nDeviceSequenceType == PGM_HYBRID) ||
        (lpstPgmRcd->nDeviceSequenceType == PGM_DIE_AHEAD) ||
        (lpstPgmRcd->nDeviceSequenceType == PGM_MAT_HYBRID) ||
        (lpstPgmRcd->nDeviceSequenceType == PGM_MAT_DIE_AHEAD))
    {
	    MATRIX_RECORD stMatrixRcd;
        theMatrix.GetRcd(&stMatrixRcd, 0);
        stMatrixRcd.nVisionStatus = VISION_NOT_DONE;
        theMatrix.SetRcd(&stMatrixRcd, 0);
	}*/
  
    PrepareBonding();
    MoveToWirePoint(m_nCurWire, FIRST_BOND, false);
	m_bRedoPR = FALSE;	//V1.1.W250 add
}

void CBond::SetBumpBondOption(short nOption)
{
    m_nBumpBondOption = nOption;
}

short CBond::GetBumpBondOption(void)
{
    return m_nBumpBondOption;
}

int CBond::SetPrPatternPassScore(void)
{
    long        alPatternId[100];
    short       i, j, nTotalRef, nTotalPattern = 0;
    REF_RECORD  stRefRcd;
	int k;

    PGM_RECORD *pstPgm = thePgm.GetRcd();
    nTotalRef = theRef.GetTotalRefNum();

    for (i = 0; i < nTotalRef; i++)
	{
        theRef.GetRcd(&stRefRcd, i);
        
        if (nTotalPattern == 0)
		{
            alPatternId[nTotalPattern] = stRefRcd.ulVisionAlignPatternID[0];
            nTotalPattern++;

			if (stRefRcd.nRefType == REF_TYPE_LEAD)
				theVisionSys.SetPassScore(
						(short)stRefRcd.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
			else
				theVisionSys.SetPassScore(
						(short)stRefRcd.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

            if (stRefRcd.nBakUpIndex >0) 
            {
				for(k=0; k<MAX_BAK_UP_PR_NUM; k++)
				{
					if(stRefRcd.stBakUpPr[k].bValid == true)
					{
						alPatternId[nTotalPattern] = stRefRcd.stBakUpPr[k].lPatID[0];
						nTotalPattern++;

						if (stRefRcd.nRefType == REF_TYPE_LEAD)
							theVisionSys.SetPassScore(
								(short)stRefRcd.stBakUpPr[k].lPatID[0],
								pstPgm->nAutoSkipScore);
						else
							theVisionSys.SetPassScore(
								(short)stRefRcd.stBakUpPr[k].lPatID[0],
								pstPgm->nAutoSkipScore);
					}
				}
            }

            if (stRefRcd.nLocateMode == REF_LOCATE_DUAL_POINT)
            {
                alPatternId[nTotalPattern] = stRefRcd.ulVisionAlignPatternID[1];
                nTotalPattern++;

				if (stRefRcd.nRefType == REF_TYPE_LEAD)
					theVisionSys.SetPassScore(
						(short)stRefRcd.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);
				else
					theVisionSys.SetPassScore(
						(short)stRefRcd.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);

				if (stRefRcd.nBakUpIndex >0) 
				{
					for(k=0; k<MAX_BAK_UP_PR_NUM; k++)
					{
						if(stRefRcd.stBakUpPr[k].bValid == true)
						{
							alPatternId[nTotalPattern] = stRefRcd.stBakUpPr[k].lPatID[1];
							nTotalPattern++;

							if (stRefRcd.nRefType == REF_TYPE_LEAD)
								theVisionSys.SetPassScore(
									(short)stRefRcd.stBakUpPr[k].lPatID[1],
									pstPgm->nAutoSkipScore);
							else
								theVisionSys.SetPassScore(
									(short)stRefRcd.stBakUpPr[k].lPatID[1],
									pstPgm->nAutoSkipScore);
						 }
                     }

                  }
              }
		}
        else
        {
            for (j = 0; j < nTotalPattern; j++)
            {
                if (stRefRcd.ulVisionAlignPatternID[0] == alPatternId[j])
                    break;
            }

            if (j >= nTotalPattern)
            {
                alPatternId[nTotalPattern] = stRefRcd.ulVisionAlignPatternID[0];
                nTotalPattern++;

				if (stRefRcd.nRefType == REF_TYPE_LEAD)
					theVisionSys.SetPassScore(
						(short)stRefRcd.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
				else
					theVisionSys.SetPassScore(
						(short)stRefRcd.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
            }

			if (stRefRcd.nBakUpIndex >0) 
			{
				for(k=0; k<MAX_BAK_UP_PR_NUM; k++)
				{
					if(stRefRcd.stBakUpPr[k].bValid == true)
					{  
						
						for (j = 0; j < nTotalPattern; j++)
						{
							if (stRefRcd.stBakUpPr[k].lPatID[0] == alPatternId[j])
								break;
						}

						if (j >= nTotalPattern)
						{
							alPatternId[nTotalPattern] = stRefRcd.stBakUpPr[k].lPatID[0];
							nTotalPattern++;

							if (stRefRcd.nRefType == REF_TYPE_LEAD)
								theVisionSys.SetPassScore(
									(short)stRefRcd.stBakUpPr[k].lPatID[0],
									pstPgm->nAutoSkipScore);
							else
								theVisionSys.SetPassScore(
									(short)stRefRcd.stBakUpPr[k].lPatID[0],
									pstPgm->nAutoSkipScore);
						}
					}
				}
            }

            if (stRefRcd.nLocateMode == REF_LOCATE_DUAL_POINT)
            {
                for (j = 0; j < nTotalPattern; j++)
                {
                    if (stRefRcd.ulVisionAlignPatternID[1] == alPatternId[j])
                        break;
                }

                if (j >= nTotalPattern)
                {
                    alPatternId[nTotalPattern] = stRefRcd.ulVisionAlignPatternID[1];
                    nTotalPattern++;

					if (stRefRcd.nRefType == REF_TYPE_LEAD)
						theVisionSys.SetPassScore(
							(short)stRefRcd.ulVisionAlignPatternID[1],
							pstPgm->nAutoSkipScore);
					else
						theVisionSys.SetPassScore(
							(short)stRefRcd.ulVisionAlignPatternID[1],
							pstPgm->nAutoSkipScore);
                }

				if (stRefRcd.nBakUpIndex >0) 
				{
					for(k=0; k<MAX_BAK_UP_PR_NUM; k++)
					{
						if(stRefRcd.stBakUpPr[k].bValid == true)
						{  
							for (j = 0; j < nTotalPattern; j++)
							{
								if (stRefRcd.stBakUpPr[k].lPatID[1] == alPatternId[j])
									break;
							}

							if (j >= nTotalPattern)
							{
								alPatternId[nTotalPattern] = stRefRcd.stBakUpPr[k].lPatID[1];
								nTotalPattern++;

								if (stRefRcd.nRefType == REF_TYPE_LEAD)
									theVisionSys.SetPassScore(
										(short)stRefRcd.stBakUpPr[k].lPatID[1],
										pstPgm->nAutoSkipScore);
								else
									theVisionSys.SetPassScore(
										(short)stRefRcd.stBakUpPr[k].lPatID[1],
										pstPgm->nAutoSkipScore);
							}
						}
					}
               				
				}

            }
        }
    }

	//index PR set

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	if (stPrIndexRefRcd.bIndexPrAvail == true)
		theVisionSys.SetPassScore(
				(short)stPrIndexRefRcd.ulVisionAlignPatternID,
				pstPgm->nLeadPrPassScore);

    return 0;
}

int CBond::RetryPR(TBL_POS* pstPrPos, ULONG ulPattern, short nPrAlgorithm, short nBinaryThreshold, short nPrPassScore, PR_SRCH_RESULT* pstSrchResult)
{
    short i, lVisionError;
    double dHighMagFactX, dHighMagFactY, dLowMagFactX, dLowMagFactY, dFactCos, dFactSin;
    TBL_POS stCurPos;
    const PGM_RECORD *pstPgm = thePgm.GetRcd();

	theVisionSys.GetVisionFactor(&dHighMagFactX, &dHighMagFactY, &dLowMagFactX, &dLowMagFactY, &dFactCos, &dFactSin);

    for (i = 0; i < 4; i++) //Retry 4 times
    {
        switch (i)
        {
        case 0:
            {
                stCurPos.dx = round(pstPrPos->dx + dLowMagFactX * PR_RETRY_PIXEL_X);
                stCurPos.dy = round(pstPrPos->dy + dLowMagFactY * PR_RETRY_PIXEL_Y);
            }
            break;
        case 1:
            {
                stCurPos.dx = round(pstPrPos->dx - dLowMagFactX * PR_RETRY_PIXEL_X);
                stCurPos.dy = round(pstPrPos->dy + dLowMagFactY * PR_RETRY_PIXEL_Y);
            }
            break;
        case 2:
            {
                stCurPos.dx = round(pstPrPos->dx - dLowMagFactX * PR_RETRY_PIXEL_X);
                stCurPos.dy = round(pstPrPos->dy - dLowMagFactY * PR_RETRY_PIXEL_Y);
            }
            break;
        case 3:
            {
                stCurPos.dx = round(pstPrPos->dx + dLowMagFactX * PR_RETRY_PIXEL_X);
                stCurPos.dy = round(pstPrPos->dy - dLowMagFactY * PR_RETRY_PIXEL_Y);
            }
            break;
        default:
            break;
        }

		theServo.MoveTable2(stCurPos, MTN_ABS, MTN_WAIT, MTN_FAST);	//v3.1.T52 edit

        if (pstPgm->nVisionMoveDelay < 10)
            Sleep(10);
        else
            Sleep(pstPgm->nVisionMoveDelay);

        theVisionSys.GrabImage();
        if (nPrAlgorithm == 1)
            theVisionSys.UpdateBinaryThreshold(nBinaryThreshold);

		theVisionSys.SetPassScore((short)ulPattern, pstPgm->nAutoSkipScore);
        lVisionError = theVisionSys.SearchVisionPattern(
				(short)ulPattern, 7, nPrPassScore, pstSrchResult);
        if (lVisionError == FOK)
        {
            *pstPrPos = stCurPos;
            break;
        }
    }
    
    return lVisionError;
}

// Power compensation function
int CBond::PowerCompensation(short* pnPower)
{
    if (*pnPower > 0)
    {
        MACHINE_DATA* pstMcData = theMcData.GetRcd();

        short nPower = *pnPower;
        *pnPower = round((double)nPower * (double)pstMcData->nPowerSlope / 100.0 + (double)pstMcData->nPowerOffset * 2.55);
		if (*pnPower < 0) *pnPower = 0;
		if (*pnPower > 255) *pnPower = 255;
    }

    return 0;
}

short CBond::SwapBkUpPrToMain(short nRefNum, short nBkPrId)
{
	short i, nUnitTotalDie, nUnitDieNum;
	REF_RECORD stCurRef, stWapRef;

	LIGHT_PARA  stVisionTempLightPara[2];
	short       nTempAlignSrchSizeX[2], nTempAlignSrchSizeY[2]; 
	ULONG       ulVisionTempPatternID[2];
	short       nTempVisionAlgorithm[2];

	theRef.GetRcd(&stWapRef, nRefNum);

	stVisionTempLightPara[0]  = stWapRef.stVisionLightPara[0];
	nTempAlignSrchSizeX[0]    = stWapRef.nVisionSrchSizeX[0];
	nTempAlignSrchSizeY[0]    = stWapRef.nVisionSrchSizeY[0];
	ulVisionTempPatternID[0]  = stWapRef.ulVisionAlignPatternID[0];
	nTempVisionAlgorithm[0]   = stWapRef.nVisionAlgorithm[0];

	if (stWapRef.nLocateMode == REF_LOCATE_DUAL_POINT)
	{
		stVisionTempLightPara[1]  = stWapRef.stVisionLightPara[1];
		nTempAlignSrchSizeX[1]    = stWapRef.nVisionSrchSizeX[1];
		nTempAlignSrchSizeY[1]    = stWapRef.nVisionSrchSizeY[1];
		ulVisionTempPatternID[1]  = stWapRef.ulVisionAlignPatternID[1];
		nTempVisionAlgorithm[1]   = stWapRef.nVisionAlgorithm[1];
	}

	if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie();
		nUnitDieNum = nRefNum % nUnitTotalDie;

		if (stWapRef.nRefType == REF_TYPE_DIE)
		{
			for (i = 1; i < theRef.GetTotalRefNum(); i++)
			{
				if ((i % nUnitTotalDie) == nUnitDieNum)
				{
					theRef.GetRcd(&stCurRef, i);

					stCurRef.stVisionLightPara[0]       = stWapRef.stBakUpPr[nBkPrId].stLight[0];
					stCurRef.nVisionSrchSizeX[0]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeX[0];
					stCurRef.nVisionSrchSizeY[0]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeY[0];
					stCurRef.ulVisionAlignPatternID[0]  = stWapRef.stBakUpPr[nBkPrId].lPatID[0];
					stCurRef.nVisionAlgorithm[0]        = stWapRef.stBakUpPr[nBkPrId].nAlgo[0]; 

					stCurRef.stBakUpPr[nBkPrId].stLight[0]    = stVisionTempLightPara[0];
					stCurRef.stBakUpPr[nBkPrId].nSrchSizeX[0] = nTempAlignSrchSizeX[0];
					stCurRef.stBakUpPr[nBkPrId].nSrchSizeY[0] = nTempAlignSrchSizeY[0];
					stCurRef.stBakUpPr[nBkPrId].lPatID[0]     = ulVisionTempPatternID[0];
					stCurRef.stBakUpPr[nBkPrId].nAlgo[0]      = nTempVisionAlgorithm[0]; 

					if(stWapRef.nLocateMode == REF_LOCATE_DUAL_POINT)
					{
						stCurRef.stVisionLightPara[1]       = stWapRef.stBakUpPr[nBkPrId].stLight[1];
						stCurRef.nVisionSrchSizeX[1]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeX[1];
						stCurRef.nVisionSrchSizeY[1]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeY[1];
						stCurRef.ulVisionAlignPatternID[1]  = stWapRef.stBakUpPr[nBkPrId].lPatID[1];
						stCurRef.nVisionAlgorithm[1]        = stWapRef.stBakUpPr[nBkPrId].nAlgo[1]; 

						stCurRef.stBakUpPr[nBkPrId].stLight[1]    = stVisionTempLightPara[1];
						stCurRef.stBakUpPr[nBkPrId].nSrchSizeX[1] = nTempAlignSrchSizeX[1];
						stCurRef.stBakUpPr[nBkPrId].nSrchSizeY[1] = nTempAlignSrchSizeY[1];
						stCurRef.stBakUpPr[nBkPrId].lPatID[1]     = ulVisionTempPatternID[1];
						stCurRef.stBakUpPr[nBkPrId].nAlgo[1]      = nTempVisionAlgorithm[1];
					}

					theRef.SetRcd(&stCurRef, i);
				}
			}
		}
	}
	else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
		nUnitDieNum = nRefNum % nUnitTotalDie;

		if (stWapRef.nRefType == REF_TYPE_DIE)
		{
			for (i = 1; i < theRef.GetTotalRefNum(); i++)
			{
				if ((i % nUnitTotalDie) == nUnitDieNum)
				{
					theRef.GetRcd(&stCurRef, i);

					stCurRef.stVisionLightPara[0]       = stWapRef.stBakUpPr[nBkPrId].stLight[0];
					stCurRef.nVisionSrchSizeX[0]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeX[0];
					stCurRef.nVisionSrchSizeY[0]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeY[0];
					stCurRef.ulVisionAlignPatternID[0]  = stWapRef.stBakUpPr[nBkPrId].lPatID[0];
					stCurRef.nVisionAlgorithm[0]        = stWapRef.stBakUpPr[nBkPrId].nAlgo[0]; 

					stCurRef.stBakUpPr[nBkPrId].stLight[0]    = stVisionTempLightPara[0];
					stCurRef.stBakUpPr[nBkPrId].nSrchSizeX[0] = nTempAlignSrchSizeX[0];
					stCurRef.stBakUpPr[nBkPrId].nSrchSizeY[0] = nTempAlignSrchSizeY[0];
					stCurRef.stBakUpPr[nBkPrId].lPatID[0]     = ulVisionTempPatternID[0];
					stCurRef.stBakUpPr[nBkPrId].nAlgo[0]      = nTempVisionAlgorithm[0]; 

					if(stWapRef.nLocateMode == REF_LOCATE_DUAL_POINT)
					{
						stCurRef.stVisionLightPara[1]       = stWapRef.stBakUpPr[nBkPrId].stLight[1];
						stCurRef.nVisionSrchSizeX[1]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeX[1];
						stCurRef.nVisionSrchSizeY[1]        = stWapRef.stBakUpPr[nBkPrId].nSrchSizeY[1];
						stCurRef.ulVisionAlignPatternID[1]  = stWapRef.stBakUpPr[nBkPrId].lPatID[1];
						stCurRef.nVisionAlgorithm[1]        = stWapRef.stBakUpPr[nBkPrId].nAlgo[1]; 

						stCurRef.stBakUpPr[nBkPrId].stLight[1]    = stVisionTempLightPara[1];
						stCurRef.stBakUpPr[nBkPrId].nSrchSizeX[1] = nTempAlignSrchSizeX[1];
						stCurRef.stBakUpPr[nBkPrId].nSrchSizeY[1] = nTempAlignSrchSizeY[1];
						stCurRef.stBakUpPr[nBkPrId].lPatID[1]     = ulVisionTempPatternID[1];
						stCurRef.stBakUpPr[nBkPrId].nAlgo[1]      = nTempVisionAlgorithm[1];
					}

					theRef.SetRcd(&stCurRef, i);
				}
			}
		}
	}

	return 0;
}

int CBond::ResetBkPrCounts(void)
{
	short i;

	for (i=0; i<MAX_BAK_UP_PR_NUM; i++)
	{
		BkUpPrUseTimes[i] = 0;
	}
	
	return 0;
}

int CBond::IcremBkPrCounts(short nId)
{

	BkUpPrUseTimes[nId]++;

	return 0;
}

// Perform capillary smooth
int CBond::PerformCapillarySmooth(short nSrchSpeed, short nSrchTol, short nForce, float fSmoothPower, short nAmplitude, short nCycle)
{
    long lCtactSrchHt, lStatus = FOK;
	long lStartTime, lElapseTime, lUsgTime;
    double dJerk, dMaxAcc, dMaxVel, dMoveDist, dPosError = 0.0;
    short nMoveTime, nScrubT1, nScrubYDelay, nAmp;
    DUMMY_BOND_PARA stDummyBondPara;
    SERVO_SPEED_PROFILE stServoSpeedProfile;

    theServo.SetServoSpeedProfile(MOTOR_X, 3, true);
    theServo.SetServoSpeedProfile(MOTOR_Y, 3, true);

    theBondPara.GetDummyBondPara(&stDummyBondPara);

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	theServo.MoveTable2(pstMcData->stBondTipOs, MTN_REL);	//v3.1.T52 edit
	Sleep(100);

    theServo.GetSpeedProfile(MOTOR_X, 3, &stServoSpeedProfile);
    dJerk = (double)(stServoSpeedProfile.lJerk) / theServo.GetResolXY() / 1000.0 * 10.0 / 100.0;
    dMaxAcc = (double)(stServoSpeedProfile.nMaxAcc) / theServo.GetResolXY() * 10.0 / 100.0;
    dMaxVel = (double)(stServoSpeedProfile.nMaxVel) / theServo.GetResolXY();

    dMoveDist = nAmplitude / theServo.GetResolXY();
    nAmp = round(dMoveDist);
    nMoveTime = Calc3rdOrderMotionTime(dJerk, dMaxAcc, dMaxVel, dMoveDist);
    nScrubT1 = nMoveTime;
    nScrubYDelay = nScrubT1 / 2;

    if (nAmplitude == 0)
        lUsgTime = 10 * nCycle;
    else
        lUsgTime = nMoveTime * 3 * nCycle;

	lCtactSrchHt = round((pstMcData->lResetLevel - 2000) / theServo.GetResolZ());
    theServo.MoveAxisZ(lCtactSrchHt, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
    Sleep(50);
    
    theServo.StartJogZ(MOTION_NEGATIVE, nSrchSpeed);
	Sleep(40);

	lStartTime = GetCurrentTime();
    do
    {
        lStatus = theServo.GetPositionError(MOTOR_Z, &dPosError);
        if (lStatus != FOK) break;

		lElapseTime = GetCurrentTime() - lStartTime;
		if (lElapseTime > SERVO_TIMEOUT) break;
    } while (fabs(dPosError) < nSrchTol);

    theServo.EndJogZ();

    int    lCurDac, lBkupDac;
    double dForceDac, dCtactLvl;

    theServo.GetBondHeadPosition(&dCtactLvl);

    dForceDac = (double)-nForce * pstMcData->dForceFactorKf + dCtactLvl * pstMcData->dForceFactorKa + pstMcData->dForceFactorIo;
    
    theServo.ReadBondHeadMFlags(&lBkupDac);

    lCurDac = lBkupDac | 0x0002;
    lCurDac = lCurDac & 0xFFFDFFFF;

    theServo.WriteBondHeadMFlags(lCurDac);
    theServo.WriteBondHeadDcom(dForceDac);
    theServo.WriteMemoryInteger(MEMVAR_USG_PATTERN, USG_TRIG_PATTERN_STANDBY1);

	UsgSelfDefineBond((WORD)lUsgTime * 10, (BYTE)round(fSmoothPower * 2.55));

    theServo.PerformTableScrub(nAmp, nScrubYDelay, nCycle);
    theServo.WriteMemoryInteger(MEMVAR_USG_PATTERN, USG_TRIG_PATTERN_CONTACT);
    theServo.WriteBondHeadMFlags(lBkupDac);

    dForceDac = 0.0;
    theServo.WriteBondHeadDcom(dForceDac);
    
    theServo.MoveAxisZ(500, MOTION_RELATIVE, MOTION_WITH_WAIT);
	Sleep(50);

    MoveBondHeadToFireLevel();
    Sleep(100);

	TBL_POS tbOffset;
	tbOffset.dx = -pstMcData->stBondTipOs.dx;
	tbOffset.dy = -pstMcData->stBondTipOs.dy;
	theServo.MoveTable2(tbOffset, MTN_REL);	//v3.1.T52 edit

    return 0;
}

// Perform capillary clean
int CBond::PerformCapillaryClean(short nSrchSpeed, short nSrchTol, short nForce, short nCycle)
{
    long            lCtactSrchHt, lStatus = FOK;
	long			lStartTime, lElapseTime;
    short           i;
    double          dPosError = 0.0;
    DUMMY_BOND_PARA stDummyBondPara;

    theServo.SetServoSpeedProfile(MOTOR_X, 3, true);
    theServo.SetServoSpeedProfile(MOTOR_Y, 3, true);

    theBondPara.GetDummyBondPara(&stDummyBondPara);

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	theServo.MoveTable2(pstMcData->stBondTipOs, MTN_REL);	//v3.1.T52 edit
	Sleep(200);

	lCtactSrchHt = round((pstMcData->lResetLevel - 2000) / theServo.GetResolZ());
    theServo.MoveAxisZ(lCtactSrchHt, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
    Sleep(50);
    
    theServo.StartJogZ(MOTION_NEGATIVE, nSrchSpeed);
	Sleep(40);

	lStartTime = GetCurrentTime();
    do
    {
        lStatus = theServo.GetPositionError(MOTOR_Z, &dPosError);
        if (lStatus != FOK) break;

		lElapseTime = GetCurrentTime() - lStartTime;
		if (lElapseTime > SERVO_TIMEOUT) break;
    } while (fabs(dPosError) < nSrchTol);

    theServo.EndJogZ();

    double dCtactLvl;
    theServo.GetBondHeadPosition(&dCtactLvl);

    int    lCurDac, lBkupDac;
    double dForceDac;

    dForceDac = (double)-nForce * pstMcData->dForceFactorKf + dCtactLvl * pstMcData->dForceFactorKa + pstMcData->dForceFactorIo;
    
    theServo.ReadBondHeadMFlags(&lBkupDac);

    lCurDac = lBkupDac | 0x0002;
    lCurDac = lCurDac & 0xFFFDFFFF;

    theServo.WriteBondHeadMFlags(lCurDac);
    theServo.WriteBondHeadDcom(dForceDac);

    Sleep(stDummyBondPara.nBondTime);

    theServo.WriteBondHeadMFlags(lBkupDac);

    //dForceDac = 0.0;

    theServo.WriteBondHeadDcom(0.0);
    
    for (i = 0; i < nCycle; i++)
    {
        theServo.MoveAxisZ(500, MOTION_RELATIVE, MOTION_WITH_WAIT);
		theServo.StartJogZ(MOTION_NEGATIVE, nSrchSpeed);
        Sleep(15);

        lStartTime = GetCurrentTime();
        do
        {
            lStatus = theServo.GetPositionError(MOTOR_Z, &dPosError);
            if (lStatus != FOK) break;

            lElapseTime = GetCurrentTime() - lStartTime;
            if (lElapseTime > SERVO_TIMEOUT) break;
        } while (fabs(dPosError) < nSrchTol);

        theServo.EndJogZ();

        theServo.WriteBondHeadMFlags(lCurDac);
        theServo.WriteBondHeadDcom(dForceDac);
        Sleep(stDummyBondPara.nBondTime);

        theServo.WriteBondHeadMFlags(lBkupDac);
        theServo.WriteBondHeadDcom(0.0);
    }

    theServo.MoveAxisZ(500, MOTION_RELATIVE, MOTION_WITH_WAIT);
    Sleep(50);

    MoveBondHeadToFireLevel();

    Sleep(100);

	TBL_POS tbOffset;
	tbOffset.dx = -pstMcData->stBondTipOs.dx;
	tbOffset.dy = -pstMcData->stBondTipOs.dy;
	theServo.MoveTable2(tbOffset, MTN_REL);	//v3.1.T52 edit

    return 0;
}

double CBond::GetBondTimer(void)
{
    return m_BondTimer.GetTime_x1ms();
}

double CBond::GetElapseBondTime(void)
{
    return m_dCurBondTime - m_dLastBondTime;
}

int CBond::UsgSelfDefineBond(WORD wTime, BYTE bDAC)
{
	theUSG.ResetProfileBuffer();
	InitUsgProfile();
	Sleep(5);

	ST_USG_PROFILE stUsgOscillate;		//profile 5: USG Oscillate, default 5s
	stUsgOscillate.cIndex = 5;
	stUsgOscillate.stVertex1.cLevel = bDAC;
	stUsgOscillate.stVertex1.nTime  = 0;
	stUsgOscillate.stVertex2.cLevel = bDAC;
	stUsgOscillate.stVertex2.nTime  = wTime;		//x0.1ms dynamic time, triggered by event sync
	stUsgOscillate.stVertex3.cLevel = 0;
	stUsgOscillate.stVertex3.nTime  = wTime + 1;	//dynamic time, triggered by event stop
	stUsgOscillate.stVertex4.cLevel = 0;
	stUsgOscillate.stVertex4.nTime  = wTime + 2;
	theUSG.SetSmallProfile(stUsgOscillate);
	Sleep(10);

	//profile 5 addr
	theServo.SetOutputIo(0, 5, 0);
	theServo.SetOutputIo(0, 4, 1);
	theServo.SetOutputIo(0, 7, 0);
	Sleep(5);

	theServo.SetOutputIo(0, 6, 0);
	Sleep(5);

	theServo.SetOutputIo(0, 6, 1);
	theServo.SetOutputIo(0, 5, 0);
	theServo.SetOutputIo(0, 4, 0);
	theServo.SetOutputIo(0, 7, 0);

	//reset
	//OUT0.5=0;OUT0.4=0;OUT0.7=0;OUT0.6=1;

	return 0;
}

bool CBond::IsUpdateBondData(void)
{
    return m_bUpdateBondData;
}

void CBond::SetUpdateBondData(bool bUpdate)
{
    m_bUpdateBondData = bUpdate;
}

int CBond::PrepareBondingWithoutPr(void)
{
    if (m_nCurWire > thePgm.GetTotalWire())
        m_nCurWire = thePgm.GetTotalWire();

	theMHS.CloseWindowClamp();

	if ((theMHS.GetCurBondUnit() > 0) || (m_bAutoIndex == false))
	{
        theVisionSys.EnableVideo(true);
        theScanKeyThread.EnableJoyStick();
        theVisionSys.ClearDrawing();
		theVisionDisplay.EnableDisplay(true);
	}

    SendTableSpeedProfileToServo();    
    Sleep(30);
    SendBondHeadSpeedProfileToServo();
    Sleep(30);
    theServo.SendServoCtrlProfileToServo();
    Sleep(30);

	return 0;
}

void CBond::PreFireEFO(void)
{
	//v1.6.T274 add
	m_bPreFireEfoSuccess = true;
	if (theMcConfig.IsEfoPrefire() == false) return;

	theEFO.ClearRxCount();	//v1.5.T75 edit

	//set shortest efo time
	WORD anFabTime[4][3];		//v3.1.T502 edit		
	WORD anFabCurrent[4][3];	//v3.1.T498 add, v3.1.T502 edit
	short nFabNum = (theMcConfig.GetFabType() == 1 ? 4 : 2);
	ST_EFO_PROFILE stEfoProfile;
	for (short i = 0; i < nFabNum; i++)
	{
		theEfoProfile.GetProfile(i, &stEfoProfile);
		//backup original time
		anFabTime[i][0] = stEfoProfile.nTime1;
		anFabTime[i][1] = stEfoProfile.nTime2;
		anFabTime[i][2] = stEfoProfile.nTime3;
		if (theMcConfig.GetPrefireTime() > 0)
			stEfoProfile.nTime1 = theMcConfig.GetPrefireTime();
		else
			stEfoProfile.nTime1 = 200;
		stEfoProfile.nTime2 = 0;
		stEfoProfile.nTime3 = 0;

		//v3.1.T498 add - backup original current
		anFabCurrent[i][0] = stEfoProfile.nCurrent1;
		anFabCurrent[i][1] = stEfoProfile.nCurrent2;
		anFabCurrent[i][2] = stEfoProfile.nCurrent3;
		if (theMcConfig.GetPrefireCurrent() > 0)
			stEfoProfile.nCurrent1 = theMcConfig.GetPrefireCurrent();
		stEfoProfile.nCurrent2 = 0;
		stEfoProfile.nCurrent3 = 0;

		theEfoProfile.SetProfile(i, &stEfoProfile);	//v1.5.T69 edit
	}
	theEFO.SendProfile();
	Sleep_x1ms(300);

#if _SERVO_TYPE == 2
	theServo.TrigEFO();		//v3.1.T344 add
#else
	theServo.ResetBondProgress();
	theServo.RunBuffer(3);
	Sleep_x1ms(100);
	CheckBondProgress(7, 3);	//v1.5.T113 add
#endif

	if (thePgm.IsCopperSequence())
	{
		Sleep_x1ms(100);
		m_bPreFireEfoSuccess = theEFO.IsSparkSuccess();		//v1.6.T338 add
	}
	else
		theEFO.ClearRxCount();	//ignore efo error	//v1.5.T75 edit

	//resume original efo time/current
	for (short i = 0; i < nFabNum; i++)
	{
		theEfoProfile.GetProfile(i, &stEfoProfile);
		stEfoProfile.nTime1 = anFabTime[i][0];
		stEfoProfile.nTime2 = anFabTime[i][1];
		stEfoProfile.nTime3 = anFabTime[i][2];
		//v3.1.T498 add
		stEfoProfile.nCurrent1 = anFabCurrent[i][0];
		stEfoProfile.nCurrent2 = anFabCurrent[i][1];
		stEfoProfile.nCurrent3 = anFabCurrent[i][2];
		theEfoProfile.SetProfile(i, &stEfoProfile);	//v1.5.T69 edit
	}
	theEFO.SendProfile();
	Sleep_x1ms(300);
}

bool CBond::IsAutoBonding(void)
{
	return (m_nBondMode == BONDMODE_AUTO);
}

void CBond::SetErrNo(int nErrNo)
{
	m_nErrNo = nErrNo;
}

void CBond::SetErrBondProgress(short nCheckProgress, short nReturnProgress)
{
	m_anErrBondProgress[0] = nCheckProgress;
	m_anErrBondProgress[1] = nReturnProgress;
}

int CBond::CtactSrch(short nSrchSpeed, short nSrchTol, long *plCtactLvl)
{
    if (theMcConfig.IsBypassServo())
    {
        *plCtactLvl = -2000;
        return 0;
    }

	theScanKeyThread.DisableJoyStick();
	theServo.MoveAxisZ(theMcData.GetRcd()->lResetLevel - 2000, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
	Sleep(50);	//v3.1.T425 add

    theServo.StartJogZ(MOTION_NEGATIVE, nSrchSpeed);
	//Sleep(40);
    
    double dPosErr;
	CHTimer timer;
    do
	{
        if (theServo.GetPositionError(MOTOR_Z, &dPosErr) != FOK) break;
#if _SERVO_TYPE == 2	//v3.1.T266 add
		Sleep_x1ms(theMcConfig.GetCtactSrchSampleTime());	//v3.1.T264 add
#endif
    } while ((fabs(dPosErr) < nSrchTol)
				&& (timer.GetTime_x1ms() < SERVO_TIMEOUT));
    theServo.EndJogZ();
	Sleep_x1ms(100);
    theServo.GetBondHeadEncPos(&dPosErr);
    *plCtactLvl = round(dPosErr);

	REF_RECORD stRefRcd;
    theRef.GetRcd(&stRefRcd, 0);
    DUMMY_BOND_PARA stDummyBondPara;
    theBondPara.GetDummyBondPara(&stDummyBondPara);
    theServo.ApplyForceControl(-stDummyBondPara.nBondForce, stDummyBondPara.nBondTime,
								thePgm.IsProgramValid(), &stRefRcd);
    
    MoveBondHeadToFireLevel();
	theScanKeyThread.EnableJoyStick();
    return 0;
}

void CBond::CapillaryWearOutPowerCompensate(short* pnPower)
{
    short nPowerOffset;
    double  x, x1, x2;	//v1.1.288.T9 edit

	//v1.6.T362 edit
	BOND_STATISTICS_RCD *pstStatRcd = theStat.GetRcd();

    if (pstStatRcd->lCapillaryCount <= theMcData.GetPwrCompOffsetCapiCount())
    {
        nPowerOffset = round(theMcData.GetPwrCompOs1() * 2.55);
        *pnPower += nPowerOffset;
    }
    else if (pstStatRcd->lCapillaryCount < theMcData.GetPwrCompEndCapiCount())
    {
        x  = pstStatRcd->lCapillaryCount;
        x1 = theMcData.GetPwrCompOffsetCapiCount();
        x2 = theMcData.GetPwrCompEndCapiCount();
        //y1 = stMachineData.dPwrCompOffset;	//v1.1.288.T9 delete
        
        nPowerOffset = round(theMcData.GetPwrCompOs1()	//v1.1.288.T9 edit
								* (1.0 - (x - x1) / (x2 - x1)) * 2.55);
        *pnPower += nPowerOffset;
    }

    if (*pnPower < 0)
        *pnPower = 0;
    else if (*pnPower > 255)
        *pnPower = 255;
}

//v1.1.288.T10 add
void CBond::CompensateCapiWearOutPower1(short* pnPower)
{
	*pnPower += CalcCapiWearOutPower1();
    if (*pnPower < 0)
        *pnPower = 0;
    else if (*pnPower > 255)
        *pnPower = 255;
}

//v1.1.288.T15 add
void CBond::CompensateCapiWearOutPower2(short* pnPower)
{
	*pnPower += CalcCapiWearOutPower2();
    if (*pnPower < 0)
        *pnPower = 0;
    else if (*pnPower > 255)
        *pnPower = 255;
}

//v1.1.288.T13 add
short CBond::CalcCapiWearOutPower1()
{
	//v1.6.T362 edit
	double x1, x2;
	x1 = theMcData.GetPwrCompOffsetCapiCount();
	x2 = theMcData.GetPwrCompEndCapiCount();
	if (x2 < x1) return 0;

    double x = theStat.GetRcd()->lCapillaryCount;
	short nPowerOs = 0;
    if (x > x1)
    {
		if (x < x2)
		{
			try
			{
				nPowerOs = round(theMcData.GetPwrCompOs1() * 2.55
									* (x - x1) / (x2 - x1));
			}
			catch (...)
			{
				nPowerOs = round(theMcData.GetPwrCompOs1() * 2.55);
			}
		}
		else
			nPowerOs = round(theMcData.GetPwrCompOs1() * 2.55);
    }

	return nPowerOs;
}

//v1.1.288.T15 add
short CBond::CalcCapiWearOutPower2()
{
	//v1.6.T362 edit
	double x1, x2;
	x1 = theMcData.GetPwrCompOffsetCapiCount();
	x2 = theMcData.GetPwrCompEndCapiCount();
	if (x2 < x1) return 0;

    double x = theStat.GetRcd()->lCapillaryCount;
	short nPowerOs = 0;
    if (x > x1)
    {
		if (x < x2)
		{
			try
			{
				nPowerOs = round(theMcData.GetPwrCompOs2() * 2.55
									* (x - x1) / (x2 - x1));
			}
			catch (...)
			{
				nPowerOs = round(theMcData.GetPwrCompOs2() * 2.55);
			}
		}
		else
			nPowerOs = round(theMcData.GetPwrCompOs2() * 2.55);
    }

	return nPowerOs;
}

void CBond::GetSubunitStartEndRef(short nCurRef, short* nStartRef, short* nEndRef)
{
	short nUnitTotalDie = thePgm.GetUnitTotalDie();
	//v1.6.T238 add
	if (nUnitTotalDie <= 0)
	{
		AfxMessageBox(_T("GetSubunitStartEndRef error!"));
		nUnitTotalDie = 1;
	}

	short nCurUnit = (nCurRef - 1) / nUnitTotalDie;

	*nStartRef = (nCurUnit * nUnitTotalDie) + 1;
	*nEndRef = *nStartRef + nUnitTotalDie - 1;
}

void CBond::SetSkipUnit(short nCurRef, BYTE nStatus)	//V1.1.W265 edit
{
	short nStartRef, nEndRef;
	GetSubunitStartEndRef(nCurRef, &nStartRef, &nEndRef);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("SkipUnit"), nStartRef, nEndRef);
	for (short i = nStartRef; i <= nEndRef; i++)
		theMatrix.SetVisionStatus(i, nStatus);
}

void  CBond::PrepareErrorMsg(long lVisionError, REF_RECORD *pstRef, PR_SRCH_RESULT *pstSrchResult, CStringList *psList)
{
	CString str;
	str.Format(_T("%s-%s%d"),
		theMsg.GetMsg(pstRef->nRefType == 0 ? MSG_LEAD : MSG_DIE),
		theMsg.GetMsg(MSG_REF), pstRef->nRefNum);
	str.AppendFormat(_T(" : %d"), int(pstSrchResult->dScore));

	double dAngle = pstSrchResult->dAngle * 180.0 / PI;
	if (dAngle > 180)dAngle -= 360;
	else if (dAngle <= -180)dAngle += 360;
	str.AppendFormat(_T(", %d"), (int)dAngle);

	psList->RemoveAll();
	psList->AddTail(str);
	psList->AddTail(theMsg.GetMsg(
					lVisionError == VISION_ERROR_NO_PATTERN ?
					MSG_PR_FAIL : MSG_PR_QUALITY_REJECT));
}

long CBond::TryBackupPR(BYTE nPoint, PGM_RECORD* lpstPgmRcd, REF_RECORD* lpstCurRef, REF_RECORD* lpstNextRef, TBL_POS* lpstCurTblPos, MATRIX_RECORD* lpstMatrix, short i, short j, short nLastRef,short nLastProfileX, short nLastProfileY, short nSearchRange, short nPrPassScore)
{
	double dMoveDist;
	short nMoveProfile, nTimes, msgIndex=0;
	long lMoveDist, lVisionError=FNOK;

	TBL_POS stPrPos;
	TBL_POS stVisionOffset;
	PR_SRCH_RESULT stSrchResult;

	for (nTimes = 0; nTimes < MAX_BAK_UP_PR_NUM; nTimes++)
	{
		if (lpstCurRef->stBakUpPr[nTimes].bValid != true)continue;

		theVisionSys.UpdateLightingParameter(
			lpstCurRef->stBakUpPr[nTimes].stLight[nPoint].nCoaxRedLight,
			lpstCurRef->stBakUpPr[nTimes].stLight[nPoint].bSideRedLight,
			lpstCurRef->stBakUpPr[nTimes].stLight[nPoint].nCoaxBlueLight,
			lpstCurRef->stBakUpPr[nTimes].stLight[nPoint].bSideBlueLight);

		lMoveDist = round(fabs(lpstCurTblPos->dx
			- lpstCurRef->stBakUpPr[nTimes].stAlignPos[nPoint].dx) * theServo.GetResolXY());
		nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
		if (nMoveProfile != nLastProfileX)
		{
			nLastProfileX = nMoveProfile;
			theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
			Sleep(1);
		}

		lMoveDist = round(fabs(lpstCurTblPos->dy
			- lpstCurRef->stBakUpPr[nTimes].stAlignPos[nPoint].dy) * theServo.GetResolXY());
		nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
		if (nMoveProfile != nLastProfileY)
		{
			nLastProfileY = nMoveProfile;
			theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
			Sleep(1);
		}

		theServo.FastMoveTable2(lpstCurRef->stBakUpPr[nTimes].stAlignPos[nPoint]);	//v3.1.T56 edit
		dMoveDist = Calc2PtDist(*lpstCurTblPos,
			lpstCurRef->stBakUpPr[nTimes].stAlignPos[nPoint]) * theServo.GetResolXY();
		*lpstCurTblPos = stPrPos = lpstCurRef->stBakUpPr[nTimes].stAlignPos[nPoint];

		//theVisionSys.UpdateLightingParameter(lpstCurRef->stBakUpPr[nTimes].stVisionBkupLightPara[nPoint].nCoaxRedLight, lpstCurRef->stBakUpPr[nTimes].stVisionBkupLightPara[nPoint].bSideRedLight, lpstCurRef->stBakUpPr[nTimes].stVisionBkupLightPara[nPoint].nCoaxBlueLight, lpstCurRef->stBakUpPr[nTimes].stVisionBkupLightPara[nPoint].bSideBlueLight);
		//theVisionSys.ChangeLensMag(lpstCurRef->nVisionAlignMag[nPoint]);

		theServo.WaitTableStop();

		if (dMoveDist > VISION_LONG_DIST)
			Sleep(lpstPgmRcd->nVisionLongMoveDelay);
		else
			Sleep(lpstPgmRcd->nVisionMoveDelay);

		theVisionSys.EnableVideo(false);
		theVisionSys.GrabImage();

		if(lpstCurRef->stBakUpPr[nTimes].nAlgo[nPoint] == 0)
			lVisionError = theVisionSys.SearchVisionPattern(
				(short)lpstCurRef->stBakUpPr[nTimes].lPatID[nPoint],
				nSearchRange, nPrPassScore, &stSrchResult, nTimes);

		if(lVisionError != FOK)continue;

		IcremBkPrCounts(nTimes);

		break;
	}

	if (lVisionError != FOK)return lVisionError;

	//Pre move to next point
	if (j < nLastRef && lpstPgmRcd->nVisionPreMove == 1)
	{
		theVisionSys.UpdateLightingParameter(lpstNextRef->stVisionLightPara[0].nCoaxRedLight, lpstNextRef->stVisionLightPara[0].bSideRedLight, lpstNextRef->stVisionLightPara[0].nCoaxBlueLight, lpstNextRef->stVisionLightPara[0].bSideBlueLight);

		lMoveDist = round(fabs(lpstCurTblPos->dx - lpstNextRef->stVisionAlignPos[0].dx) * theServo.GetResolXY());
		nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
		if (nMoveProfile != nLastProfileX)
		{
			nLastProfileX = nMoveProfile;
			theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
			Sleep(1);
		}

		lMoveDist = round(fabs(lpstCurTblPos->dy - lpstNextRef->stVisionAlignPos[0].dy) * theServo.GetResolXY());
		nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
		if (nMoveProfile != nLastProfileY)
		{
			nLastProfileY = nMoveProfile;
			theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
			Sleep(1);
		}

		//theServo.FastMoveTable(&lpstNextRef->stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
		theServo.FastMoveTable2(lpstNextRef->stVisionAlignPos[0]);	//v3.1.T56 edit
		dMoveDist = Calc2PtDist(*lpstCurTblPos, lpstNextRef->stVisionAlignPos[0]) * theServo.GetResolXY();
		//theVisionSys.UpdateLightingParameter(lpstNextRef->stVisionLightPara[0].nCoaxRedLight, lpstNextRef->stVisionLightPara[0].bSideRedLight, lpstNextRef->stVisionLightPara[0].nCoaxBlueLight, lpstNextRef->stVisionLightPara[0].bSideBlueLight);
		//theVisionSys.ChangeLensMag(lpstNextRef->nVisionAlignMag[0]);
	}

	theVisionSys.VisionOffsetToTablePos(lpstCurRef->nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

	lpstMatrix->nVisionStatus = VISION_SUCCESS;
	m_nAutoSkipDieCount = 0;

	lpstMatrix->stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
	lpstMatrix->stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
	lpstMatrix->stVisionAlignPos[1].dx = lpstMatrix->stVisionAlignPos[0].dx
							+ lpstCurRef->stBakUpPr[nTimes].stAlignPos[1].dx
							- lpstCurRef->stBakUpPr[nTimes].stAlignPos[0].dx;
	lpstMatrix->stVisionAlignPos[1].dy = lpstMatrix->stVisionAlignPos[0].dy
							+ lpstCurRef->stBakUpPr[nTimes].stAlignPos[1].dy
							- lpstCurRef->stBakUpPr[nTimes].stAlignPos[0].dy;

	theMatrix.CalcMatrix(lpstMatrix, i, nTimes+2);

	return lVisionError;
}

void CBond::PopWireLenZeroError(short nWireNum)		//V1.1.W254 add
{
	CString strMsg;
	CStringList strList;
	strMsg.Format(theMsg.GetMsg(MSG_WIRE_LEN_ZERO), nWireNum);
	strList.AddTail(strMsg);
	theMsg.ShowMsg(ERROR_MSG, strList);

	theLog.WriteLong(_T("Wire %d Length = 0"), nWireNum, true);
}

void CBond::SetTeaching(BYTE bTeaching)		//V1.1.W276 add
{
	m_bRedoPR=bTeaching;
}

//v1.1.288.T2 add
double CBond::CalcTransducerLenCompen(short nWireNum)
{
	double fCompStep = 0.0;
	double fCompTime = m_nIdleTime_x1s - m_BondTimer.GetTime_x1s();
	if ((theMcData.GetTransducerIdleTime() > 0)
		&& (theMcData.GetTransducerLengthComp() != 0)
		&& (fCompTime > 0.0))
	{
		fCompStep = 2.0 * theMcData.GetTransducerLengthComp() * fCompTime
				/ (theMcData.GetTransducerIdleTime() * 60.0);
		if (theDebug.IsDebugTransducer())
		{
			m_sTemp.Format(_T("W%d_%.0fstep_%.1f/%ds"),
				nWireNum, fCompStep, fCompTime, m_nIdleTime_x1s);
			theDebug.AddInfo(m_sTemp);	//v3.0.T96 add
		}
	}
	return fCompStep;
}

void CBond::ResetMinMaxRef()	//V1.5.W145 add
{
	m_nMinRef = m_nMaxRef = 0;
}

void CBond::SetUsgProfile(short nWireNum, WORD *pnTime, BYTE *pnPower, BYTE nIndex, int nSegNum)
{
	theUSG.SetProfile(pnTime, pnPower, nIndex, nSegNum);

	if (theDebug.IsDebugUsgProfile())
	{
		m_dCurBondTime = m_BondTimer.GetTime_x1ms();
		m_sTemp.Empty();
		if (theMcConfig.IsDebugInfoShowTime())
			m_sTemp.Format(_T("%.1f_"), m_dCurBondTime);
		m_sTemp.AppendFormat(_T("W%d"), nWireNum);	//v3.1.T312 edit
		if (nSegNum == USG_1ST_BOND_SEG_NUM)
			m_sTemp.Append(_T("/1"));	//1st bond
		else
			m_sTemp.Append(_T("/2"));	//2nd bond
		m_sTemp.AppendFormat(_T("/%d"), nIndex);	//v3.1.T313 add

		for (short i=0; i<nSegNum; i++)
		{
			if (*(pnTime+i) > 0)
				m_sTemp.AppendFormat(_T("_%d-%d/%d"),
							i, *(pnTime+i), *(pnPower+i));
		}
		m_dLastBondTime = m_dCurBondTime;
		theDebug.AddInfo(m_sTemp);
	}
}

void CBond::StopLfCycleTimer()
{
	m_bLfCycleStarted = false;
}

double CBond::GetLfCycleTime()
{
	return m_dLfCycleTime;
}

//v3.0.T115 add
long CBond::GetIdleTime_x1s()
{
	return round(m_IdleTimer.GetTime_x1s());
}

//v3.0.T115 add
long CBond::GetBondTime_x1s()
{
	return round(m_BondTimer.GetTime_x1s());
}

//v3.0.T123 add
void CBond::StartShowPrTimer()
{
	m_ShowPrTimer.StartTimer();
}

//v3.1.T51 clean
int CBond::MatrixAlign(bool bSlowMove, EN_VISION_ACTION nVisionAction)
{
    short i, k, nStartRef, nLastRef, nSrchRange, nPassScore;
    int nVisErr, lStopSelected;
    double dMoveDist = 0.0;
    CStringList sList;
    TBL_POS stVisionOffset, stStepRepeatOffset, stCurTblPos, stPrPos;
    REF_RECORD stCurRef, stNextRef;
    MATRIX_RECORD stMatrix1, stMatrix2;
    PR_SRCH_RESULT stSrchRes;
    short nLastProfX = 0, nLastProfY = 0, nCurProf;
    long lMoveDist, lMoveDistX, lMoveDistY;
    PGM_RECORD *pstPgm = thePgm.GetRcd();

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("MatrixAlign"));
	theVisionDisplay.EnableDisplay(false);
    if (m_bBondNoVision && (nVisionAction == VISION_ACTION_AUTO))
        return PERFORM_WIRE_BOND;

    nStartRef = 0;
    nLastRef = theRef.GetTotalRefNum() - 1;
    GetPerformPrRef(m_nCurWire, &nStartRef, &nLastRef);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Check PR"), nStartRef, nLastRef);

	if (nVisionAction == VISION_ACTION_ALL_UNIT)
	{
		nStartRef = 0;
		nLastRef = theRef.GetTotalRefNum() - 1;
	}
    else if (nVisionAction == VISION_ACTION_SINGLE_UNIT)
    {
		nStartRef = 0;
        nLastRef = thePgm.GetUnitTotalDie();
    }

    for (i = nStartRef; i <= nLastRef; i++)
    {
        theMatrix.GetRcd(&stMatrix2, i);
        if ((stMatrix2.nVisionStatus != VISION_SUCCESS) &&
            (stMatrix2.nVisionStatus != VISION_SKIP_DIE))
        {
            nStartRef = i;
            break;
        }
    }

    if (i > nLastRef) //All alignments already done
        return PERFORM_WIRE_BOND;

    for (i = nLastRef; i >= nStartRef; i--)
    {
        theMatrix.GetRcd(&stMatrix2, i);
        if ((stMatrix2.nVisionStatus == VISION_SUCCESS) ||
            (stMatrix2.nVisionStatus == VISION_SKIP_DIE))
            nLastRef -= 1;
        else
            break;
    }

    if (nLastRef < nStartRef) return PERFORM_WIRE_BOND;

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Perform PR"), nStartRef, nLastRef);
    theServo.GetTablePosition(&stCurTblPos);
	thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol,
								m_nCurRow, m_nCurCol, &stStepRepeatOffset);

	short nInc = 1;
    for (short j = nStartRef; j <= nLastRef; j++)
	{
		i = j;
		nInc = 1;

        theRef.GetRcd(&stCurRef, i);
        stCurRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
        stCurRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
        stCurRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

		for(k = 0; k < MAX_BAK_UP_PR_NUM; k++)
		{
			stCurRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
			stCurRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
			stCurRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
			stCurRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
		}

        theMatrix.GetRcd(&stMatrix1, i);
        if (stMatrix1.nVisionStatus == VISION_SKIP_DIE) continue;	//V1.1.W250 add

        if (j < nLastRef)
        {
	        theRef.GetRcd(&stNextRef, i + nInc);
            stNextRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
            stNextRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
            stNextRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

			for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
			{
				stNextRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
				stNextRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
				stNextRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
				stNextRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
			}
        }

        if (stMatrix1.nVisionStatus == VISION_SUCCESS) continue;

		theVisionSys.UpdateLightingParameter(
						stCurRef.stVisionLightPara[0].nCoaxRedLight,
						stCurRef.stVisionLightPara[0].bSideRedLight,
						stCurRef.stVisionLightPara[0].nCoaxBlueLight,
						stCurRef.stVisionLightPara[0].bSideBlueLight);

		//if ((i == nStartRef) || (pstPgm->nVisionPreMove == 0))
        {
            lMoveDistX = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
            lMoveDistY = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());

			dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
			if ((lMoveDistX > 0) || (lMoveDistY > 0))
			//if (dMoveDist > 100.0)
            {
                nLastProfX = GetTableAutoSpeedProfile(lMoveDistX);
                theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, bSlowMove);

                //lMoveDistY = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nLastProfY = GetTableAutoSpeedProfile(lMoveDistY);
                theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, bSlowMove);
                Sleep_x1ms(1);

                if (bSlowMove == true)
                {
                    nLastProfX = 4;
                    nLastProfY = 4;
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
            }

            stCurTblPos = stCurRef.stVisionAlignPos[0];
			ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
								stCurRef.nRefNum,
								stCurRef.stVisionAlignPos[0].dx,
								stCurRef.stVisionAlignPos[0].dy);
        }

		stPrPos = stCurRef.stVisionAlignPos[0];
        theVisionSys.EnableVideo(false);

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("start ref"), stCurRef.nRefNum);
		theServo.WaitTableStop();
        
		//pr srch delay
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
        if (dMoveDist > VISION_LONG_DIST)
			Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
        else if (dMoveDist > 100.0)
			Sleep_x1ms(pstPgm->nVisionMoveDelay);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

		theVisionSys.GrabImage();
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

        if (stCurRef.nVisionAlgorithm[0] == 1)
            theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

        if (((stCurRef.nRefType == REF_TYPE_LEAD) && (pstPgm->nPrAlignMode == 1)) || //Single point alignment without PR angle
            (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT))
        {
            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
				Sleep_x1ms(20);
				theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_UpdateLightPara"));

                lMoveDist = round(fabs(stCurTblPos.dx
					- stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfX)
                {
                    nLastProfX = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfY)
                {
                    nLastProfY = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
							stNextRef.nRefNum,
							stNextRef.stVisionAlignPos[0].dx,
							stNextRef.stVisionAlignPos[0].dy);
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            nVisErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), nVisErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), nVisErr);

            theMHS.SetThreadStop(false);

			BOOL bRedoPrEnable = FALSE;
			switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
			{
			case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
			case MHS_TOP_ELITE:	//v3.0.T410 add
				bRedoPrEnable = FALSE;
				break;
			case MHS_VLED: case MHS_VLED_ELITE:
                bRedoPrEnable = pstPgm->bPrRetry;
				break;
			}

            if ((nVisErr != FOK) && bRedoPrEnable) 
			{
                nVisErr = RetryPR(&stPrPos, stCurRef.ulVisionAlignPatternID[0],
								stCurRef.nVisionAlgorithm[0],
								stCurRef.stVisionLightPara[0].nImageThreshold,
								nPassScore, &stSrchRes);
                
                if (nVisErr != FOK)
                {
                    lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                    nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                    if (nCurProf != nLastProfX)
                    {
                        nLastProfX = nCurProf;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                        Sleep_x1ms(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                    nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                    if (nCurProf != nLastProfY)
                    {
                        nLastProfY = nCurProf;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                        Sleep_x1ms(1);
                    }

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITH_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0], MTN_ABS, MTN_WAIT);	//v3.1.T56 edit
					stPrPos = stCurRef.stVisionAlignPos[0];
					stCurTblPos = stCurRef.stVisionAlignPos[0];

                    theVisionSys.SetCameraExposureTime(pstPgm->nRetryExposureTime);

                    nVisErr = RetryPR(&stPrPos, stCurRef.ulVisionAlignPatternID[0],
										stCurRef.nVisionAlgorithm[0],
										stCurRef.stVisionLightPara[0].nImageThreshold,
										nPassScore, &stSrchRes);
                    
                    theVisionSys.SetCameraExposureTime(pstPgm->nExposureTime);
                }
            }

			if (nVisErr == FOK)
			{
				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrResult"),
								stCurRef.nRefNum, stSrchRes.dOffsetX,
								stSrchRes.dOffsetY, stSrchRes.dAngle);

                theVisionSys.VisionOffsetToTablePos(
					stCurRef.nVisionAlignMag[0], &stSrchRes, &stVisionOffset);

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/Offset"),
					stCurRef.nRefNum, stVisionOffset.dx, stVisionOffset.dy);

                stMatrix1.nVisionStatus = VISION_SUCCESS;
                m_nAutoSkipDieCount = 0;

				stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
				stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
				stMatrix1.stVisionAlignPos[1].dx = stMatrix1.stVisionAlignPos[0].dx + stCurRef.stVisionAlignPos[1].dx - stCurRef.stVisionAlignPos[0].dx;
				stMatrix1.stVisionAlignPos[1].dy = stMatrix1.stVisionAlignPos[0].dy + stCurRef.stVisionAlignPos[1].dy - stCurRef.stVisionAlignPos[0].dy;

				if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
				{
					stMatrix1.dCosAln = cos(stSrchRes.dAngle);
					stMatrix1.dSinAln = sin(stSrchRes.dAngle);
				}

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/MatPos"),
							stCurRef.nRefNum,
							stMatrix1.stVisionAlignPos[0].dx,
							stMatrix1.stVisionAlignPos[0].dy);

                theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
				
				//check current Die overlap
				if ((j > 0) && (stCurRef.nRefType == REF_TYPE_DIE)
					&& isCheckDieOverlap(i, thePgm.GetDeviceSeq()))
					return QUIT_AUTO_BOND;
			}

			if ((nVisErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nVisErr = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef,
								&stCurTblPos, &stMatrix1, i, j, nLastRef,
								nLastProfX, nLastProfY, nSrchRange, nPassScore);

			//PR failed, show error or skip die/unit
            if (nVisErr != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if (m_bRedoPR == FALSE)
						theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if (m_bRedoPR == FALSE)
						theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					theStat.SetMachineState(MCSTATE_STANDBY);

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nVisErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
							{
								if (pstPgm->nManualSkip == 0)
									theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
								else
									SetSkipUnit(i);
							}
						}
					}

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_OK)
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
					else
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					theServo.GetTablePosition(&stCurTblPos);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx
							- stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfX)
						{
							nLastProfX = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfY)
						{
							nLastProfY = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
		}
        else if ((stCurRef.nRefType == REF_TYPE_DIE)
				&& pstPgm->bDiePrMultiSrch) //Two point alignment with multi search
		{
            double dRefAlignDist, dMatrixAlignDist, dAlignTol;

            //Pre move to next point
            if ((j < nLastRef) && pstPgm->nVisionPreMove == 1)
            {
				theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx
					- stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfX)
                {
                    nLastProfX = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfY)
                {
                    nLastProfY = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
            }

            nSrchRange = pstPgm->nDieSearchRange;
            nPassScore = pstPgm->nPrPassScore;
			
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
            nVisErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPassScore, &stSrchRes);
            theMHS.SetThreadStop(false);
            if (nVisErr == FOK)
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stVisionOffset);
                stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

                nVisErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSrchRange, nPassScore, &stSrchRes);
                if (nVisErr == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stVisionOffset);

                    stMatrix1.nVisionStatus = VISION_SUCCESS;
                    m_nAutoSkipDieCount = 0;

                    stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                    stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

                    dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0], stCurRef.stVisionAlignPos[1]);
                    dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0], stMatrix1.stVisionAlignPos[1]);
                    if (stCurRef.nRefType == REF_TYPE_LEAD)
                        dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                    else
                        dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                    if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                        nVisErr = FNOK;
                    else
                        theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
                }
            }

			if ((nVisErr != FOK) && (stCurRef.nBakUpIndex > 0))
			{
				for (int iTryTimes = 0; iTryTimes < MAX_BAK_UP_PR_NUM; iTryTimes++)
				{
					if (stCurRef.stBakUpPr[iTryTimes].bValid)
					{
						theVisionSys.UpdateLightingParameter(
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxBlueLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dx)
							* theServo.GetResolXY());
                        nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                        if (nCurProf != nLastProfX)
                        {
                            nLastProfX = nCurProf;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dy)
							* theServo.GetResolXY());
                        nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                        if (nCurProf != nLastProfY)
                        {
                            nLastProfY = nCurProf;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                            Sleep_x1ms(1);
                        }

						theServo.FastMoveTable2(stCurRef.stBakUpPr[iTryTimes].stAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos,
							stCurRef.stBakUpPr[iTryTimes].stAlignPos[0])
							* theServo.GetResolXY();
						stPrPos = stCurRef.stBakUpPr[iTryTimes].stAlignPos[0];

						theServo.WaitTableStop();

						if (dMoveDist > VISION_LONG_DIST)
							Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
						else
							Sleep_x1ms(pstPgm->nVisionMoveDelay);

						theVisionSys.EnableVideo(false);
						theVisionSys.GrabImage();

						if (stCurRef.stBakUpPr[iTryTimes].nAlgo[0] == 0)
							nVisErr = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[0],
								nSrchRange, nPassScore, &stSrchRes, iTryTimes);

                        if (nVisErr == FOK)
                        {
                            theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stVisionOffset);
                            stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
                            stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;

							theVisionSys.UpdateLightingParameter(
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxBlueLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideBlueLight);
							theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

                            nVisErr = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[1],
								nSrchRange, nPassScore, &stSrchRes, iTryTimes);
                            if (nVisErr == FOK)
                            {
                                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stVisionOffset);
                                
                                stMatrix1.nVisionStatus = VISION_SUCCESS;
                                m_nAutoSkipDieCount = 0;

                                stMatrix1.stVisionAlignPos[1].dx = stPrPos.dx + stVisionOffset.dx;
                                stMatrix1.stVisionAlignPos[1].dy = stPrPos.dy + stVisionOffset.dy;

                                dRefAlignDist = Calc2PtDist(
											stCurRef.stBakUpPr[iTryTimes].stAlignPos[0],
											stCurRef.stBakUpPr[iTryTimes].stAlignPos[1]);
                                dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0],
																stMatrix1.stVisionAlignPos[1]);
                                if (stCurRef.nRefType == REF_TYPE_LEAD)
                                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                                else
                                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
                                    nVisErr = FNOK;
                                else
                                    theMatrix.CalcMatrix(&stMatrix1, i, iTryTimes+2);
                            }
                        }
					}

					if (nVisErr == FOK)
					{
						IcremBkPrCounts(iTryTimes);
						break;
					}
				}
				
				if (nVisErr == FOK)
				{
					//Pre move to next point
					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);

                        lMoveDist = round(fabs(stCurTblPos.dx
							- stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                        nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                        if (nCurProf != nLastProfX)
                        {
                            nLastProfX = nCurProf;
                            theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                            Sleep_x1ms(1);
                        }

                        lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                        nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                        if (nCurProf != nLastProfY)
                        {
                            nLastProfY = nCurProf;
                            theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                            Sleep_x1ms(1);
                        }

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}
                }
			}

            if (nVisErr != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					theStat.SetMachineState(MCSTATE_STANDBY);

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nCurProf = GetTableAutoSpeedProfile(lMoveDist);
					if (nCurProf != nLastProfX)
					{
						nLastProfX = nCurProf;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nCurProf = GetTableAutoSpeedProfile(lMoveDist);
					if (nCurProf != nLastProfY)
					{
						nLastProfY = nCurProf;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nVisErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					if (!thePgm.IsAutoSkipDie() && !thePgm.IsAutoSkipUnit())
					{
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}
					else
					{
						if (((thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
							|| (thePgm.GetDeviceSeq() == PGM_HYBRID)
							|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
							|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
							&& ((i == 0) || (m_nAutoSkipDieCount > 4)))
						{
							theServo.SetServoControlParameter();
							lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						}
						else
						{
							lStopSelected = SHOW_MSG_RESULT_SKIP;
							m_nAutoSkipDieCount++;
						}
					}

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
						}
					}

					theMHS.SetOxidationProtect(false);
					if (lStopSelected == 1)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix1.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
						}
						else
							SetSkipUnit(i);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx
							- stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfX)
						{
							nLastProfX = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfY)
						{
							nLastProfY = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
        }
        else if ((pstPgm->nPrAlignMode == 0) //Two point alignment
					|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD) 
					|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD))
		{
            //Pre move to next point
            if (pstPgm->nVisionPreMove == 1)
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfX)
                {
                    nLastProfX = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfY)
                {
                    nLastProfY = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPassScore = pstPgm->nPrPassScore;
            }
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
            nVisErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), nVisErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), nVisErr);
            theMHS.SetThreadStop(false);
			if ((nVisErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nVisErr = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef,
								&stCurTblPos, &stMatrix1, i, j, nLastRef,
								nLastProfX, nLastProfY, nSrchRange, nPassScore);
            if (nVisErr != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nCurProf = GetTableAutoSpeedProfile(lMoveDist);
					if (nCurProf != nLastProfX)
					{
						nLastProfX = nCurProf;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nCurProf = GetTableAutoSpeedProfile(lMoveDist);
					if (nCurProf != nLastProfY)
					{
						nLastProfY = nCurProf;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nVisErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);
					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx
							- stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfX)
						{
							nLastProfX = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfY)
						{
							nLastProfY = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
            }
            else
            {
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stVisionOffset);
                stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;
            }

            if (pstPgm->nVisionPreMove == 0)
            {
				theVisionSys.UpdateLightingParameter(
						stCurRef.stVisionLightPara[1].nCoaxRedLight,
						stCurRef.stVisionLightPara[1].bSideRedLight,
						stCurRef.stVisionLightPara[1].nCoaxBlueLight,
						stCurRef.stVisionLightPara[1].bSideBlueLight);

                lMoveDist = round(fabs(stCurTblPos.dx
					- stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfX)
                {
                    nLastProfX = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfY)
                {
                    nLastProfY = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

                dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
                stCurTblPos = stCurRef.stVisionAlignPos[1];
            }

			theServo.WaitTableStop();

			//pr srch delay
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
            if (dMoveDist > VISION_LONG_DIST)
                Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
            else
                Sleep_x1ms(pstPgm->nVisionMoveDelay);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

            theVisionSys.EnableVideo(false);
            theVisionSys.GrabImage();
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

            if (stCurRef.nVisionAlgorithm[1] == 1)
                theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

            //Pre move to next point
            if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
            {
                lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfX)
                {
                    nLastProfX = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
                    Sleep_x1ms(1);
                }

                lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
                nCurProf = GetTableAutoSpeedProfile(lMoveDist);
                if (nCurProf != nLastProfY)
                {
                    nLastProfY = nCurProf;
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
                    Sleep_x1ms(1);
                }

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveNextAlignPos1"));

                dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
                //stCurTblPos = stNextRef.stVisionAlignPos[0];
            }

            if (stCurRef.nRefType == REF_TYPE_LEAD)
            {
                nSrchRange = pstPgm->nLeadSearchRange;
                nPassScore = pstPgm->nLeadPrPassScore;
            }
            else
            {
                nSrchRange = pstPgm->nDieSearchRange;
                nPassScore = pstPgm->nPrPassScore;
            }

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
            nVisErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat2"), nVisErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat2"), nVisErr);
			if ((nVisErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nVisErr = TryBackupPR(1, pstPgm, &stCurRef, &stNextRef,
								&stCurTblPos, &stMatrix1, i, j, nLastRef,
								nLastProfX, nLastProfY, nSrchRange, nPassScore);
            if (nVisErr != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
					nCurProf = GetTableAutoSpeedProfile(lMoveDist);
					if (nCurProf != nLastProfX)
					{
						nLastProfX = nCurProf;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
					nCurProf = GetTableAutoSpeedProfile(lMoveDist);
					if (nCurProf != nLastProfY)
					{
						nLastProfY = nCurProf;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[1];

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nVisErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);
					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix1.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
						}
						else
							SetSkipUnit(i);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter();
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfX)
						{
							nLastProfX = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nCurProf = GetTableAutoSpeedProfile(lMoveDist);
						if (nCurProf != nLastProfY)
						{
							nLastProfY = nCurProf;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
            }
            else
			{
                double dRefAlignDist, dMatrixAlignDist, dAlignTol;
                theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[1], &stSrchRes, &stVisionOffset);

                stMatrix1.nVisionStatus = VISION_SUCCESS;
                stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[1].dx + stVisionOffset.dx;
                stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[1].dy + stVisionOffset.dy;

                dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0], stCurRef.stVisionAlignPos[1]);
                dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0], stMatrix1.stVisionAlignPos[1]);
                if (stCurRef.nRefType == REF_TYPE_LEAD)
                    dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
                else
                    dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

                if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					if (thePgm.IsAutoSkipDie()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nVisErr == VISION_ERROR_NO_PATTERN))
					{
						theMatrix.SetVisionStatus(stCurRef.nRefNum, VISION_SKIP_DIE);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else if (thePgm.IsAutoSkipUnit()
							&& (stCurRef.nRefType == REF_TYPE_DIE))
					{
						SetSkipUnit(i);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);
						stMatrix1.nVisionStatus = VISION_NOT_DONE;

						if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}

						sList.RemoveAll();
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							sList.AddTail(theMsg.GetMsg(MSG_LEAD_TOL_FAILURE));
						else
							sList.AddTail(theMsg.GetMsg(MSG_DIE_TOL_FAILURE));
						sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

						theMHS.SetOxidationProtect(true);
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						theMHS.SetOxidationProtect(false);
						if (lStopSelected == SHOW_MSG_RESULT_STOP)
						{
							theVisionSys.EnableVideo(true);
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableIdleLight();
							return QUIT_AUTO_BOND;
						}
						if (lStopSelected == SHOW_MSG_RESULT_SKIP)
						{
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableBondingLight();

							ProcessKeyboard();	//v2.0.T38 add
							theServo.WaitTableStop();

							if (pstPgm->nManualSkip == 0)
							{
								stMatrix1.nVisionStatus = VISION_SKIP_DIE;
								theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
							}
							else
								SetSkipUnit(i);

							if (m_nBondMode == BONDMODE_STOP)
								return QUIT_AUTO_BOND;

							theServo.SetServoControlParameter();
						}
						else
						{
							if (isManualAlignmentStop(i))
								return QUIT_AUTO_BOND;
						}

						theServo.GetTablePosition(&stCurTblPos);

						if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
							theStat.SetMachineState(MCSTATE_PRODUCTION);

						if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
						{
							lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
							nCurProf = GetTableAutoSpeedProfile(lMoveDist);
							if (nCurProf != nLastProfX)
							{
								nLastProfX = nCurProf;
								theServo.SetServoSpeedProfile(MOTOR_X, nLastProfX, false);
								Sleep_x1ms(1);
							}

							lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
							nCurProf = GetTableAutoSpeedProfile(lMoveDist);
							if (nCurProf != nLastProfY)
							{
								nLastProfY = nCurProf;
								theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfY, false);
								Sleep_x1ms(1);
							}

							//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
							theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
							dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
							stCurTblPos = stNextRef.stVisionAlignPos[0];
						}

						continue;
					}
                }
                else
                    theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
            }

			//PR failed, show error or skip die/unit
            if (nVisErr != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nVisErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
					theStat.SetMachineState(MCSTATE_STANDBY);
					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nVisErr, &stCurRef, &stSrchRes, &sList);
				}
            }
        }

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end ref"), stCurRef.nRefNum);
    }

	if (m_bIll == TRUE) thePrAlign.PerformInteliLeadLocate(true);

	short nAheadDone = CheckAheadAlignDone(&m_nCurWire);
	if (nAheadDone == 0) return PERFORM_VISION_ALIGN;
	if (nAheadDone == 2)
	{
		if (thePgm.IsAutoSkipDie())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef)
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;
				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG,sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		else if (thePgm.IsAutoSkipUnit())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef / thePgm.GetUnitTotalDie())
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;
				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG,sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		return COMPLETE_UNIT_UPDATE;
	}

	return PERFORM_WIRE_BOND;
}

//v3.1.T319 add
void CBond::CalcArcMotionZ(double dDist, double dT1, double *pdTraj, short nSkewPercent)
{
	double SST = 0.00025;
	double Vtop = dDist / dT1 / SST;
	int nT1 = (int)ceil(dT1 * nSkewPercent / 100.0);
	int nT3 = (int)(dT1 * 2) - nT1;
	double V1s = Vtop / nT1;
	double V3s = Vtop / nT3;
	double D1s = V1s * SST;
	double D3s = V3s * SST;

	double Pos = 0;
	int i, j = 0;;
	for (i = 1; i <= nT1; i++)
	{
		Pos += D1s * (i - 0.5);
		*(pdTraj + j) = Pos;
		j++;
	}
	for (i = nT3; i > 1; i--)
	{
		Pos += D3s * (i - 0.5);
		*(pdTraj + j) = Pos;
		j++;
	}
	*(pdTraj + j) = dDist;	//last pos
}

//v3.1.T455 add
int CBond::DieAheadAlign2(bool bSlowMove, EN_VISION_ACTION nVisionAction)	//V1.4.W20 add
{
	ShowDebugMsg(DEBUG_BOND_TIMING, _T("DieAheadAlign2"));
	theVisionDisplay.EnableDisplay(false);
	if (m_bBondNoVision && (nVisionAction == VISION_ACTION_AUTO))
		return PERFORM_WIRE_BOND;
	
	short i, k;
	TOP_DEVICE_RECORD *pstTopDevice; 
	CUELITE_DEVICE_RECORD stCuEliteDevicePara;
	short nLastIndexCol, nLastIndex, nUnitNum, nUnitTotalDie, nUnitBaseRef;
	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_TOP_LED: case MHS_VLED: case MHS_VLED_ELITE:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		pstTopDevice = theDevice.GetTopRcd();
		if (pstTopDevice->nHalfIndexOn == 0)
		{
			nLastIndexCol = theDevice.GetLastIndexWndColumn();
			if (nLastIndexCol < thePgm.GetDeviceColNum())
			{
				if (pstTopDevice->nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}
			}
		}
		break;
	}
	case MHS_R2R:
	{
		pstTopDevice = theDevice.GetTopRcd();
		if (pstTopDevice->nHalfIndexOn == 0)
		{
			nLastIndexCol = theDevice.GetLastIndexWndColumn();
			if (nLastIndexCol < thePgm.GetDeviceColNum())
			{
				if (pstTopDevice->nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}

				nLastIndex = pstTopDevice->nDeviceColmNum
								/ pstTopDevice->nWndUnitNum + 1;
				if (theR2RTrack.GetCurrentBondUnit() == nLastIndex)
				{
					for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
					{
						for (k = nLastIndexCol + 1; k <= thePgm.GetDeviceColNum(); k++)
						{
							nUnitNum = thePgm.GetUnitNum(i, k);
							nUnitTotalDie = thePgm.GetUnitTotalDie();

							nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
							for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
							{
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
							}
						}
					}
				}
			}
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

		if (stCuEliteDevicePara.nHalfIndexOn == 0)
		{
			nLastIndexCol = theDevice.GetLastIndexWndColumn();
			if (nLastIndexCol < thePgm.GetDeviceColNum())
			{
				if (stCuEliteDevicePara.nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.38, nWndUnitNum == 0"));
					return 0;
				}

				nLastIndex = stCuEliteDevicePara.nDeviceColmNum / stCuEliteDevicePara.nWndUnitNum + 1;
				if (theCuEliteTrack.GetCurrentBondUnit() == nLastIndex)
				{
					for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
					{
						for (k = nLastIndexCol + 1; k <= thePgm.GetDeviceColNum(); k++)
						{
							nUnitNum = thePgm.GetUnitNum(i, k);
							nUnitTotalDie = thePgm.GetUnitTotalDie();

							nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
							for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
							{
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
							}
						}
					}
				}
			}
		}
		break;
	}
	}

	short nStartRef = 0;
	short nLastRef = theRef.GetTotalRefNum() - 1;
	GetPerformPrRef(m_nCurWire, &nStartRef, &nLastRef);
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Check PR"), nStartRef, nLastRef);

	if (nVisionAction == VISION_ACTION_ALL_UNIT)
	{
		nStartRef = 0;
		nLastRef = theRef.GetTotalRefNum() - 1;
	}
	else if (nVisionAction == VISION_ACTION_SINGLE_UNIT)
	{
		nStartRef = 0;
		nLastRef = thePgm.GetUnitTotalDie();
	}
	short oneImageFindNum = 3;
	if (nLastRef < 24)
		oneImageFindNum = 1;
	short nFirstLineStart = 0;
	short nFirstLineLast = 0;
	bool isNeedAutoShot = false;
	if (oneImageFindNum == 3)
	{
		GetPerformPrRef(1, &nFirstLineStart, &nFirstLineLast);
		if(nLastRef > nFirstLineLast)
		{
			isNeedAutoShot = true;
			oneImageFindNum = 1;
		}
	}
	//for (i = nStartRef; i <= nLastRef; i++)
	//{
	//	if ((theMatrix.GetRcd(i)->nVisionStatus != VISION_SUCCESS)
	//		&& (theMatrix.GetRcd(i)->nVisionStatus != VISION_SKIP_DIE))
	//	{
	//		nStartRef = i;
	//		break;
	//	}
	//}
	if (i > nLastRef)	//all alignment done
		return PERFORM_WIRE_BOND;

	//for (i = nLastRef; i >= nStartRef; i--)
	//{
	//	if ((theMatrix.GetRcd(i)->nVisionStatus == VISION_SUCCESS)
	//		|| (theMatrix.GetRcd(i)->nVisionStatus == VISION_SKIP_DIE))
	//		nLastRef -= 1;
	//	else
	//		break;
	//}
	if (nLastRef < nStartRef) return PERFORM_WIRE_BOND;
	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Perform PR"), nStartRef, nLastRef);

	TBL_POS stCurPos, stStepRepeatOS;
	theServo.GetTablePosition(&stCurPos);
	thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOS);

	short nSrchRange, nPassScore;
	int nSrchErr, lStopSelected;
	double dMoveDist = 0.0;
	CStringList sList;
	TBL_POS stPrOS, stPrPos;
	PR_SRCH_RESULT stSrchRes;
	short nLastProfileX = 0, nLastProfileY = 0, nMoveProfile;
	long lMoveDist, lMoveDistX, lMoveDistY;
	PGM_RECORD *pstPgm = thePgm.GetRcd();
	REF_RECORD stCurRef, stNextRef;
	REF_RECORD stRef1,stRef3;
	short nBeforeI = 0, nNextI = 0;
	MATRIX_RECORD stMatrix;
	MATRIX_RECORD stMat1;
	MATRIX_RECORD stMat3;
	short nInc = 1;
	for (short j = nStartRef; j <= nLastRef; j++)
	{
		//Default Reverse
		switch (theMcConfig.GetMhsType())
		{
		case MHS_TOP_LED: case MHS_R2R:
		case MHS_VLED: case MHS_VLED_ELITE:
		case MHS_TOP_ELITE:
		{
			pstTopDevice = theDevice.GetTopRcd();
			break;
		}
		case MHS_CU_ELITE:
		{
			theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
			break;
		}
		}
		
		if (j == 0)  //lead PR
		{
			//for the high power 3rd index
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTEIndex.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theR2RTrack.GetCurrentBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn)))
			{
				WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);	//v3.1.T90 edit
				if ((pstWire->nRefNum[0] == 0) || (pstWire->nRefNum[1] == 0))
					nInc = max(pstWire->nRefNum[0], pstWire->nRefNum[1]);
				else
					nInc = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);

				i = 0;
				j = nInc - 1;
			}
			else
			{
				i = 0;
				nInc = nLastRef;
			}
		}
		else  //after lead PR
		{
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTEIndex.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theR2RTrack.GetCurrentBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (pstTopDevice->nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == pstTopDevice->nHalfIndexOn)))
			{
				i = j;
				nInc = 1;
			}
			else
			{
				if (nStartRef == 0)
				{
				   i = nLastRef - j + 1;
				   nInc = -1;
				}
				else
				{
					i = j;
					nInc = 1;
				}
			}
		}
		if(j != 0)
		{
			nNextI = i + oneImageFindNum * nInc;
			if(isNeedAutoShot && nLastRef > 2 && 
				oneImageFindNum == 1 && i <= nFirstLineLast + 1)
			{
				nNextI = i + 2 * nInc;
			}
		}
		else
		{
			if(nLastRef > 2 && oneImageFindNum == 3)
				nNextI = nLastRef - 1;
			else
				nNextI = nLastRef;
		}
		stCurRef = *theRef.GetRcd(i);
		stCurRef.stVisionAlignPos[0].dx += stStepRepeatOS.dx;
		stCurRef.stVisionAlignPos[0].dy += stStepRepeatOS.dy;
		stCurRef.stVisionAlignPos[1].dx += stStepRepeatOS.dx;
		stCurRef.stVisionAlignPos[1].dy += stStepRepeatOS.dy;
		for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
		{
			stCurRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOS.dx; 
			stCurRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOS.dy;
			stCurRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOS.dx;
			stCurRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOS.dy;
		}
		if(oneImageFindNum == 3)
		{
			if(i > 0)
			{
				stRef1 = *theRef.GetRcd(i - 1);
				stRef1.stVisionAlignPos[0].dx += stStepRepeatOS.dx;
				stRef1.stVisionAlignPos[0].dy += stStepRepeatOS.dy;
				stRef1.stVisionAlignPos[1].dx += stStepRepeatOS.dx;
				stRef1.stVisionAlignPos[1].dy += stStepRepeatOS.dy;
				for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
				{
					stRef1.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOS.dx; 
					stRef1.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOS.dy;
					stRef1.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOS.dx;
					stRef1.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOS.dy;
				}
				stRef3 = *theRef.GetRcd(i + 1);
				stRef3.stVisionAlignPos[0].dx += stStepRepeatOS.dx;
				stRef3.stVisionAlignPos[0].dy += stStepRepeatOS.dy;
				stRef3.stVisionAlignPos[1].dx += stStepRepeatOS.dx;
				stRef3.stVisionAlignPos[1].dy += stStepRepeatOS.dy;
				for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
				{
					stRef3.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOS.dx; 
					stRef3.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOS.dy;
					stRef3.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOS.dx;
					stRef3.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOS.dy;
				}
			}

		}

		stMatrix = *theMatrix.GetRcd(i);
		if(i > 0 && oneImageFindNum == 3)
		{
			stMat1 = *theMatrix.GetRcd(i - 1);
			stMat3 = *theMatrix.GetRcd(i + 1);
		}
		
		if(oneImageFindNum == 3 && j != 0)
		{
			if ((stMatrix.nVisionStatus == VISION_SKIP_DIE || stMatrix.nVisionStatus == VISION_SUCCESS)
				&& (stMat1.nVisionStatus == VISION_SKIP_DIE|| stMat1.nVisionStatus == VISION_SUCCESS)
				&& (stMat3.nVisionStatus == VISION_SKIP_DIE|| stMat3.nVisionStatus == VISION_SUCCESS)) 
			{
				j += 2;
					continue;	//V1.1.W250 add
			}
		}
		

		if (j + (oneImageFindNum - 1) < nLastRef && nNextI >= 0 && nNextI <= nLastRef)
		{
			stNextRef = *theRef.GetRcd(nNextI);
			stNextRef.stVisionAlignPos[0].dx += stStepRepeatOS.dx;
			stNextRef.stVisionAlignPos[0].dy += stStepRepeatOS.dy;
			stNextRef.stVisionAlignPos[1].dx += stStepRepeatOS.dx;
			stNextRef.stVisionAlignPos[1].dy += stStepRepeatOS.dy;

			for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
			{
				stNextRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOS.dx; 
				stNextRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOS.dy;
				stNextRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOS.dx;
				stNextRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOS.dy;
			}
			
		}

		if(oneImageFindNum == 1)
		{
			//if (stMatrix.nVisionStatus == VISION_SKIP_DIE 
			//	&& stMat1.nVisionStatus == VISION_SKIP_DIE
			//	&& stMat3.nVisionStatus == VISION_SKIP_DIE) 
			//{
			//	j += 2;
			//		continue;	//V1.1.W250 add
			//}
			if (stMatrix.nVisionStatus == VISION_SUCCESS)
			{
				if(isNeedAutoShot && j != 0 && nLastRef > 2 && 
					oneImageFindNum == 1 && i <= nFirstLineLast +1)
				{
					j++;
					isNeedAutoShot = false;
					oneImageFindNum = 3;
				}
				continue;
			}
		}
		

		theVisionSys.UpdateLightingParameter(
					stCurRef.stVisionLightPara[0].nCoaxRedLight,
					stCurRef.stVisionLightPara[0].bSideRedLight,
					stCurRef.stVisionLightPara[0].nCoaxBlueLight,
					stCurRef.stVisionLightPara[0].bSideBlueLight);

		lMoveDistX = round(fabs(stCurPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
		lMoveDistY = round(fabs(stCurPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());

		dMoveDist = Calc2PtDist(stCurPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
		if ((lMoveDistX > 0) || (lMoveDistY > 0))	//V1.1.W303 edit
		{
			nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
			theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, bSlowMove);

			nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
			theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, bSlowMove);
			Sleep_x1ms(1);

			if (bSlowMove == true)
			{
				nLastProfileX = 4;
				nLastProfileY = 4;
			}
			theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
		}

		stCurPos = stCurRef.stVisionAlignPos[0];
		ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
								stCurRef.nRefNum,
								stCurRef.stVisionAlignPos[0].dx,
								stCurRef.stVisionAlignPos[0].dy);
		stPrPos = stCurRef.stVisionAlignPos[0];
		theVisionSys.EnableVideo(false);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("start ref"), stCurRef.nRefNum);
		theServo.WaitTableStop();
	    
		//pr srch delay
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
		if (dMoveDist > VISION_LONG_DIST)
			Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
		else if (dMoveDist > 100.0)
			Sleep_x1ms(pstPgm->nVisionMoveDelay);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

		theVisionSys.GrabImage();
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

		if (stCurRef.nVisionAlgorithm[0] == 1)
			theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

		if (((stCurRef.nRefType == REF_TYPE_LEAD) && (pstPgm->nPrAlignMode == 1)) || //Single point alignment without PR angle
			(stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT))
		{
			//Pre move to next point
			if (j + (oneImageFindNum - 1) < nLastRef && nNextI >= 0 
				&& nNextI <= nLastRef && (pstPgm->nVisionPreMove == 1))
			{
				Sleep_x1ms(20);
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_UpdateLightPara"));

				lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
							stNextRef.nRefNum,
							stNextRef.stVisionAlignPos[0].dx,
							stNextRef.stVisionAlignPos[0].dy);
			}
			if(false)
			{
				return PERFORM_VISION_ALIGN;
			}
			
			for(int index = 0; index < oneImageFindNum; index++)
			{
				REF_RECORD currRef;
				MATRIX_RECORD currMatrix;
				int matrixIndex = i;
				if(index == 0 && oneImageFindNum == 3)
				{
					currRef = stRef1;
					currMatrix = stMat1;
					matrixIndex = i - 1;
				}
				else if(index == 2 && oneImageFindNum == 3)
				{
					currRef = stRef3;
					currMatrix = stMat3;
					matrixIndex = i + 1;
				}
				else
				{
					currRef = stCurRef;
					currMatrix = stMatrix;
				}
				if (currMatrix.nVisionStatus == VISION_SUCCESS) continue;
				if (currMatrix.nVisionStatus == VISION_SKIP_DIE) continue;
				if (currRef.nRefType == REF_TYPE_LEAD)
				{
					nSrchRange = pstPgm->nLeadSearchRange;
					nPassScore = pstPgm->nLeadPrPassScore;
				}
				else
				{
					nSrchRange = pstPgm->nDieSearchRange;
					nPassScore = pstPgm->nPrPassScore;
				}

			theVisionSys.SetPassScore(
						(short)currRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), currRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), currRef.nRefNum);
			if (oneImageFindNum > 1)
			{
				short nSrchStartX;
				short nSrchStartY;
				short nSrchWidth;
				short nSrchHt;
				theVisionSys.GetSrchRegion(nSrchStartX, nSrchStartY, nSrchWidth, nSrchHt, index);
				nSrchErr = theVisionSys.SearchVisionPattern(
					(short)currRef.ulVisionAlignPatternID[0],
					nSrchStartX, nSrchStartY,
					nSrchWidth, nSrchHt, nPassScore, &stSrchRes,-1,index);
			}
			else
			{
				nSrchErr = theVisionSys.SearchVisionPattern(
					(short)currRef.ulVisionAlignPatternID[0], nSrchRange,nPassScore, &stSrchRes);
			}
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), nSrchErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), nSrchErr);

			theMHS.SetThreadStop(false);

			BOOL bRedoPrEnable = FALSE;
			switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
			{
			case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				bRedoPrEnable = FALSE;
				break;
			}
			case MHS_VLED: case MHS_VLED_ELITE:
			{
				bRedoPrEnable = pstPgm->bPrRetry;
				break;
			}
			}

			if ((nSrchErr != FOK) && bRedoPrEnable) 
			{
				nSrchErr = RetryPR(
					&stPrPos, currRef.ulVisionAlignPatternID[0],
					currRef.nVisionAlgorithm[0],
					currRef.stVisionLightPara[0].nImageThreshold,
					nPassScore, &stSrchRes);
	            
				if (nSrchErr != FOK &&(oneImageFindNum == 1 || index == 2))
				{
					lMoveDist = round(fabs(stCurPos.dx - currRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurPos.dy - currRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					theServo.FastMoveTable2(currRef.stVisionAlignPos[0], MTN_ABS, MTN_WAIT);	//v3.1.T56 edit
					stPrPos = currRef.stVisionAlignPos[0];
					stCurPos = currRef.stVisionAlignPos[0];

					theVisionSys.SetCameraExposureTime(pstPgm->nRetryExposureTime);
					nSrchErr = RetryPR(
						&stPrPos, currRef.ulVisionAlignPatternID[0],
						currRef.nVisionAlgorithm[0],
						currRef.stVisionLightPara[0].nImageThreshold,
						nPassScore, &stSrchRes);
					theVisionSys.SetCameraExposureTime(pstPgm->nExposureTime);
				}
			}

			if (nSrchErr == FOK)
			{
				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrResult"),
							currRef.nRefNum, stSrchRes.dOffsetX,
							stSrchRes.dOffsetY, stSrchRes.dAngle);
				theVisionSys.VisionOffsetToTablePos(
					currRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/Offset"),
					currRef.nRefNum, stPrOS.dx, stPrOS.dy);

				m_nAutoSkipDieCount = 0;

				currMatrix.nVisionStatus = VISION_SUCCESS;
				currMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stPrOS.dx;
				currMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stPrOS.dy;
				currMatrix.stVisionAlignPos[1].dx = currMatrix.stVisionAlignPos[0].dx + currRef.stVisionAlignPos[1].dx - currRef.stVisionAlignPos[0].dx;
				currMatrix.stVisionAlignPos[1].dy = currMatrix.stVisionAlignPos[0].dy + currRef.stVisionAlignPos[1].dy - currRef.stVisionAlignPos[0].dy;
				if (currRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
				{
					currMatrix.dCosAln = cos(stSrchRes.dAngle);
					currMatrix.dSinAln = sin(stSrchRes.dAngle);
				}
				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/MatPos"),
								currRef.nRefNum,
								currMatrix.stVisionAlignPos[0].dx,
								currMatrix.stVisionAlignPos[0].dy);
				theMatrix.CalcMatrix(&currMatrix, matrixIndex, VISION_PR_ALIGN);
				
				//check current Die overlap
				if ((j > 0) && (currRef.nRefType == REF_TYPE_DIE)
					&& isCheckDieOverlap(matrixIndex, thePgm.GetDeviceSeq()))
					return QUIT_AUTO_BOND;
			}

			if ((nSrchErr != FOK) && (currRef.nBakUpIndex > 0))
				nSrchErr = TryBackupPR(0, pstPgm, &currRef, &stNextRef,
							&stCurPos, &currMatrix, matrixIndex, j, nLastRef,
							nLastProfileX, nLastProfileY, nSrchRange, nPassScore);

			//PR failed, show error or skip die/unit
			if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && currRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(matrixIndex, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if (m_bRedoPR == FALSE)
						theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (currRef.nRefType == REF_TYPE_DIE)
						&& (nSrchErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(matrixIndex);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					if (m_bRedoPR == FALSE)
						theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit
					break;		
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
					theStat.SetMachineState(MCSTATE_STANDBY);
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(nSrchErr, &currRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);

					theMHS.SetOxidationProtect(false);
	                
					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (currRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (currRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
							{
								if (pstPgm->nManualSkip == 0)
									theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
								else
									SetSkipUnit(matrixIndex);
							}
						}
					}

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_OK)
					{
						if (isManualAlignmentStop(matrixIndex))
							return QUIT_AUTO_BOND;
					}
					else
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						currMatrix.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&currMatrix, matrixIndex, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);
	                
					theServo.GetTablePosition(&stCurPos);

					if (j + (oneImageFindNum - 1) < nLastRef && nNextI >= 0 
						&& nNextI <= nLastRef && (pstPgm->nVisionPreMove == 1)
						&& (oneImageFindNum == 1 || index == 2))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}

			}
			//end for
		}
		else if ((stCurRef.nRefType == REF_TYPE_DIE) && (pstPgm->bDiePrMultiSrch == true)) //Two point alignment with multi search
		{
			double dRefAlignDist, dMatrixAlignDist, dAlignTol;

			//Pre move to next point
			if ((j < nLastRef) && pstPgm->nVisionPreMove == 1)
			{
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

				lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
			}

			nSrchRange = pstPgm->nDieSearchRange;
			nPassScore = pstPgm->nPrPassScore;
			
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
			nSrchErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSrchRange, nPassScore, &stSrchRes);
			theMHS.SetThreadStop(false);
			if (nSrchErr == FOK)
			{
				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
				stMatrix.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stPrOS.dx;
				stMatrix.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stPrOS.dy;

				nSrchErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSrchRange, nPassScore, &stSrchRes);
				if (nSrchErr == FOK)
				{
					theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);

					stMatrix.nVisionStatus = VISION_SUCCESS;
					m_nAutoSkipDieCount = 0;

					stMatrix.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[0].dx + stPrOS.dx;
					stMatrix.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[0].dy + stPrOS.dy;

					dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0],
												stCurRef.stVisionAlignPos[1]);
					dMatrixAlignDist = Calc2PtDist(stMatrix.stVisionAlignPos[0],
												stMatrix.stVisionAlignPos[1]);
					if (stCurRef.nRefType == REF_TYPE_LEAD)
						dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
					else
						dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

					if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
						nSrchErr = FNOK;
					else
						theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
				}
			}

			if (nSrchErr != FOK && stCurRef.nBakUpIndex > 0) 
			{
				int iTryTimes;
				for (iTryTimes = 0; iTryTimes < MAX_BAK_UP_PR_NUM; iTryTimes++)
				{
					if (stCurRef.stBakUpPr[iTryTimes].bValid == true)
					{
						theVisionSys.UpdateLightingParameter(
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxBlueLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurPos.dx
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dx)
							* theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurPos.dy
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dy)
							* theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						theServo.FastMoveTable2(stCurRef.stBakUpPr[iTryTimes].stAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurPos,
							stCurRef.stBakUpPr[iTryTimes].stAlignPos[0])
							* theServo.GetResolXY();
						stPrPos = stCurRef.stBakUpPr[iTryTimes].stAlignPos[0];

						theServo.WaitTableStop();

						if (dMoveDist > VISION_LONG_DIST)
							Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
						else
							Sleep_x1ms(pstPgm->nVisionMoveDelay);

						theVisionSys.EnableVideo(false);
						theVisionSys.GrabImage();

						if (stCurRef.stBakUpPr[iTryTimes].nAlgo[0] == 0)
							nSrchErr = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[0],
								nSrchRange, nPassScore, &stSrchRes, iTryTimes);

						if (nSrchErr == FOK)
						{
							theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
							stMatrix.stVisionAlignPos[0].dx = stPrPos.dx + stPrOS.dx;
							stMatrix.stVisionAlignPos[0].dy = stPrPos.dy + stPrOS.dy;

							theVisionSys.UpdateLightingParameter(
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxBlueLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideBlueLight);
							theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

							nSrchErr = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[1],
								nSrchRange, nPassScore, &stSrchRes, iTryTimes);
							if (nSrchErr == FOK)
							{
								theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
	                            
								stMatrix.nVisionStatus = VISION_SUCCESS;
								m_nAutoSkipDieCount = 0;

								stMatrix.stVisionAlignPos[1].dx = stPrPos.dx + stPrOS.dx;
								stMatrix.stVisionAlignPos[1].dy = stPrPos.dy + stPrOS.dy;

								dRefAlignDist = Calc2PtDist(
									stCurRef.stBakUpPr[iTryTimes].stAlignPos[0],
									stCurRef.stBakUpPr[iTryTimes].stAlignPos[1]);
								dMatrixAlignDist = Calc2PtDist(
									stMatrix.stVisionAlignPos[0],
									stMatrix.stVisionAlignPos[1]);
								if (stCurRef.nRefType == REF_TYPE_LEAD)
									dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
								else
									dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

								if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
									nSrchErr = FNOK;
								else
								   theMatrix.CalcMatrix(&stMatrix, i, iTryTimes+2);
							}
						}
					}

					if (nSrchErr == FOK)
					{
						IcremBkPrCounts(iTryTimes);
						break;
					}
				}
				
				if (nSrchErr == FOK)
				{
					//Pre move to next point
					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}
				}
			}

			if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);

					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nSrchErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					theStat.SetMachineState(MCSTATE_STANDBY);

					lMoveDist = round(fabs(stCurPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurPos = stCurRef.stVisionAlignPos[0];
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					if (!thePgm.IsAutoSkipDie()
						&& !thePgm.IsAutoSkipUnit())
					{
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}
					else
					{
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
						}
					}

					theMHS.SetOxidationProtect(false);
					if (lStopSelected == 1)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if(lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
						}
						else
							SetSkipUnit(i);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;
						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(
							stNextRef.stVisionLightPara[0].nCoaxRedLight,
							stNextRef.stVisionLightPara[0].bSideRedLight,
							stNextRef.stVisionLightPara[0].nCoaxBlueLight,
							stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}
					continue;
				}
			}
		}
		else if (pstPgm->nPrAlignMode == 0) //Two point alignment
		{
			//Pre move to next point
			if (pstPgm->nVisionPreMove == 1)
			{
				lMoveDist = round(fabs(stCurPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

				dMoveDist = Calc2PtDist(stCurPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
				stCurPos = stCurRef.stVisionAlignPos[1];
			}

			if (stCurRef.nRefType == REF_TYPE_LEAD)
			{
				nSrchRange = pstPgm->nLeadSearchRange;
				nPassScore = pstPgm->nLeadPrPassScore;
			}
			else
			{
				nSrchRange = pstPgm->nDieSearchRange;
				nPassScore = pstPgm->nPrPassScore;
			}
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			nSrchErr = theVisionSys.SearchVisionPattern(
					(short)stCurRef.ulVisionAlignPatternID[0],
					nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), nSrchErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), nSrchErr);
			theMHS.SetThreadStop(false);
			if ((nSrchErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nSrchErr = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef,
							&stCurPos, &stMatrix, i, j, nLastRef,
							nLastProfileX, nLastProfileY, nSrchRange, nPassScore);

			if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
					nSrchErr == VISION_ERROR_NO_PATTERN)
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}
	                
					lMoveDist = round(fabs(stCurPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurPos = stCurRef.stVisionAlignPos[0];

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						stMatrix.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;
						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
	                
					theServo.GetTablePosition(&stCurPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurPos = stNextRef.stVisionAlignPos[0];
					}
					continue;
				}
			}
			else
			{
				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchRes, &stPrOS);
				stMatrix.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stPrOS.dx;
				stMatrix.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stPrOS.dy;
			}

			if (pstPgm->nVisionPreMove == 0)
			{
				theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[1].nCoaxRedLight, stCurRef.stVisionLightPara[1].bSideRedLight, stCurRef.stVisionLightPara[1].nCoaxBlueLight, stCurRef.stVisionLightPara[1].bSideBlueLight);

				lMoveDist = round(fabs(stCurPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

				dMoveDist = Calc2PtDist(stCurPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
				stCurPos = stCurRef.stVisionAlignPos[1];
			}

			theServo.WaitTableStop();

			//pr srch delay
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
			if (dMoveDist > VISION_LONG_DIST)
				Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
			else
				Sleep_x1ms(pstPgm->nVisionMoveDelay);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

			theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

			if (stCurRef.nVisionAlgorithm[1] == 1)
				theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

			//Pre move to next point
			if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
			{
				lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveNextAlignPos1"));

				dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
			}

			if (stCurRef.nRefType == REF_TYPE_LEAD)
			{
				nSrchRange = pstPgm->nLeadSearchRange;
				nPassScore = pstPgm->nLeadPrPassScore;
			}
			else
			{
				nSrchRange = pstPgm->nDieSearchRange;
				nPassScore = pstPgm->nPrPassScore;
			}
			// test 
			
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			nSrchErr = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSrchRange, nPassScore, &stSrchRes);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat2"), nSrchErr);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat2"), nSrchErr);
			if ((nSrchErr != FOK) && (stCurRef.nBakUpIndex > 0))
				nSrchErr = TryBackupPR(1, pstPgm, &stCurRef, &stNextRef,
							&stCurPos, &stMatrix, i, j, nLastRef,
							nLastProfileX, nLastProfileY, nSrchRange, nPassScore);

			if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE) 
					&& (nSrchErr == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}

					lMoveDist = round(fabs(stCurPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
					stCurPos = stCurRef.stVisionAlignPos[1];

					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();
						ProcessKeyboard();
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
						}
						else
							SetSkipUnit(i);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;
						theServo.SetServoControlParameter();
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurPos = stNextRef.stVisionAlignPos[0];
					}
					continue;
				}
			}
			else
			{
				double dRefAlignDist, dMatrixAlignDist, dAlignTol;
				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[1], &stSrchRes, &stPrOS);

				stMatrix.nVisionStatus = VISION_SUCCESS;
				stMatrix.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[1].dx + stPrOS.dx;
				stMatrix.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[1].dy + stPrOS.dy;

				dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0],
											stCurRef.stVisionAlignPos[1]);
				dMatrixAlignDist = Calc2PtDist(stMatrix.stVisionAlignPos[0],
											stMatrix.stVisionAlignPos[1]);
				if (stCurRef.nRefType == REF_TYPE_LEAD)
					dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
				else
					dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

				if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					if (thePgm.IsAutoSkipDie() && stCurRef.nRefType == REF_TYPE_DIE && 
						nSrchErr == VISION_ERROR_NO_PATTERN)
					{
						theMatrix.SetVisionStatus(stCurRef.nRefNum, VISION_SKIP_DIE);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else if (thePgm.IsAutoSkipUnit() && stCurRef.nRefType == REF_TYPE_DIE)
					{
						SetSkipUnit(i);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);

						stMatrix.nVisionStatus = VISION_NOT_DONE;

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}

						sList.RemoveAll();
						sList.AddTail(theMsg.GetMsg(stCurRef.nRefType == REF_TYPE_LEAD ? MSG_LEAD_TOL_FAILURE : MSG_DIE_TOL_FAILURE));
						sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

						theMHS.SetOxidationProtect(true);
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						theMHS.SetOxidationProtect(false);

						if (lStopSelected == SHOW_MSG_RESULT_STOP)
						{
							theVisionSys.EnableVideo(true);
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableIdleLight();
							return QUIT_AUTO_BOND;
						}
						else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
						{
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableBondingLight();
							ProcessKeyboard();
							theServo.WaitTableStop();

							if (pstPgm->nManualSkip == 0)
							{
								stMatrix.nVisionStatus = VISION_SKIP_DIE;
								theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
							}
							else
								SetSkipUnit(i);

							if (m_nBondMode == BONDMODE_STOP)
								return QUIT_AUTO_BOND;
							theServo.SetServoControlParameter();
						}
						else
						{
							if (isManualAlignmentStop(i))
								return QUIT_AUTO_BOND;
						}

						theServo.GetTablePosition(&stCurPos);

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
							theStat.SetMachineState(MCSTATE_PRODUCTION);

						if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
						{
							lMoveDist = round(fabs(stCurPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileX)
							{
								nLastProfileX = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
								Sleep_x1ms(1);
							}

							lMoveDist = round(fabs(stCurPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileY)
							{
								nLastProfileY = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
								Sleep_x1ms(1);
							}

							theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
							dMoveDist = Calc2PtDist(stCurPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
							stCurPos = stNextRef.stVisionAlignPos[0];
						}
						continue;
					}
				}
				else
					theMatrix.CalcMatrix(&stMatrix, i, VISION_PR_ALIGN);
			}

			//PR failed, show error or skip die/unit
			if (nSrchErr != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (nSrchErr == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (nSrchErr == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
					theStat.SetMachineState(MCSTATE_STANDBY);
					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(nSrchErr, &stCurRef, &stSrchRes, &sList);
				}
			}
		}

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end ref"), stCurRef.nRefNum);
		if(oneImageFindNum == 3)
		{
			if(j == 0)
				j++;
			else
				j += 2;
		}
		else if(isNeedAutoShot && j != 0 && nLastRef > 2 && 
				oneImageFindNum == 1 && i <= nFirstLineLast +1)
		{
			j++;
			isNeedAutoShot = false;
			oneImageFindNum = 3;
		}
	}

	if (m_bIll) thePrAlign.PerformInteliLeadLocate(true);

	short nAheadAlignDone = CheckAheadAlignDone(&m_nCurWire);
	if (nAheadAlignDone == 0)
		return PERFORM_VISION_ALIGN;
	else if (nAheadAlignDone == 2)
	{
		if (thePgm.IsAutoSkipDie())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef)
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG,sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		else if (thePgm.IsAutoSkipUnit())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef / thePgm.GetUnitTotalDie())
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG,sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		return COMPLETE_UNIT_UPDATE;
	}
	return PERFORM_WIRE_BOND;
}

//v3.1.T491 add
int CBond::MatrixDieAheadAlign2(bool bSlowMove, EN_VISION_ACTION nVisionAction)
{
	short i, k, nStartRef, nLastRef, nAheadAlignDone, nSearchRange, nPrPassScore;
	int lVisionError, lStopSelected;
	double dMoveDist = 0.0;
	CStringList sList;
	TBL_POS stVisionOffset, stStepRepeatOffset, stCurTblPos, stPrPos;
	REF_RECORD stCurRef, stNextRef;
	MATRIX_RECORD stMatrix1;
	PR_SRCH_RESULT stSrchResult;
	TOP_DEVICE_RECORD stTopDevicePara; 
	CUELITE_DEVICE_RECORD stCuEliteDevicePara;
	short nLastProfileX = 0, nLastProfileY = 0, nMoveProfile;
	long lMoveDist, lMoveDistX, lMoveDistY;
	short nLastIndexWndColumn, nLastIndex, nUnitNum, nUnitTotalDie, nUnitBaseRef;
	PGM_RECORD *pstPgm = thePgm.GetRcd();

	ShowDebugMsg(DEBUG_BOND_TIMING, _T("MatrixDieAheadAlign"));

	theVisionDisplay.EnableDisplay(false);

	if (m_bBondNoVision && (nVisionAction == VISION_ACTION_AUTO))
		return PERFORM_WIRE_BOND;

	switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
	{
	case MHS_TOP_LED: case MHS_VLED: case MHS_VLED_ELITE:
	{
		theDevice.GetTopRcd(&stTopDevicePara);

		//if (stTopDevicePara.nHalfIndexOn == 0)	//V1.1.W256 delete
		{
			nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
			if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
			{
				if (stTopDevicePara.nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}

				nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
				if (theTopTrack.GetCurBondUnit() == nLastIndex)
				{
					for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
					{
						for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
						{
							nUnitNum = thePgm.GetUnitNum(i, k);
							nUnitTotalDie = thePgm.GetUnitTotalDie();

							nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
							for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
							{
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
							}
						}
					}
				}
			}
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
		if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
		{
			theDevice.GetTopRcd(&stTopDevicePara);
			if (stTopDevicePara.nWndUnitNum == 0)
			{
				AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
				return 0;
			}

			nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
			if (theTEIndex.GetCurBondUnit() == nLastIndex)
			{
				for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
				{
					for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
					{
						nUnitNum = thePgm.GetUnitNum(i, k);
						nUnitTotalDie = thePgm.GetUnitTotalDie();

						nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
						for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
						{
							theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
						}
					}
				}
			}
		}
		break;
	}
	case MHS_R2R:
	{
		theDevice.GetTopRcd(&stTopDevicePara);

		if (stTopDevicePara.nHalfIndexOn == 0)
		{
			nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
			if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
			{
				if(stTopDevicePara.nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.37, nWndUnitNum == 0"));
					return 0;
				}

				nLastIndex = stTopDevicePara.nDeviceColmNum / stTopDevicePara.nWndUnitNum + 1;
				if (theR2RTrack.GetCurrentBondUnit() == nLastIndex)
				{
					for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
					{
						for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
						{
							nUnitNum = thePgm.GetUnitNum(i, k);
							nUnitTotalDie = thePgm.GetUnitTotalDie();

							nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
							for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
							{
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
							}
						}
					}
				}
			}
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

		if (stCuEliteDevicePara.nHalfIndexOn == 0)
		{
			nLastIndexWndColumn = theDevice.GetLastIndexWndColumn();
			if (nLastIndexWndColumn < thePgm.GetDeviceColNum())
			{
				if (stCuEliteDevicePara.nWndUnitNum == 0)
				{
					AfxMessageBox(_T("temp.38, nWndUnitNum == 0"));
					return 0;
				}
				nLastIndex = stCuEliteDevicePara.nDeviceColmNum / stCuEliteDevicePara.nWndUnitNum + 1;
				if (theCuEliteTrack.GetCurrentBondUnit() == nLastIndex)
				{
					for (i = 1; i <= thePgm.GetDeviceRowNum(); i++)
					{
						for (k = nLastIndexWndColumn + 1; k <= thePgm.GetDeviceColNum(); k++)
						{
							nUnitNum = thePgm.GetUnitNum(i, k);
							nUnitTotalDie = thePgm.GetUnitTotalDie();

							nUnitBaseRef = (nUnitNum - 1) * nUnitTotalDie + 1;
							for (short r = nUnitBaseRef; r < nUnitBaseRef + nUnitTotalDie; r++)
							{
								theMatrix.SetVisionStatus(r, VISION_SKIP_DIE);
							}
						}
					}
				}
			}
		}
		break;
	}
	}

	nStartRef = 0;
	nLastRef = theRef.GetTotalRefNum() - 1;

	GetPerformPrRef(m_nCurWire, &nStartRef, &nLastRef);

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Check PR"), nStartRef, nLastRef);

	if (nVisionAction == VISION_ACTION_ALL_UNIT)
	{
		nStartRef = 0;
		nLastRef = theRef.GetTotalRefNum() - 1;
	}
	else if (nVisionAction == VISION_ACTION_SINGLE_UNIT)
	{
		nStartRef = 0;
		nLastRef = thePgm.GetUnitTotalDie();
	}

	for (i = nStartRef; i <= nLastRef; i++)
	{
		if ((theMatrix.GetRcd(i)->nVisionStatus != VISION_SUCCESS) &&
			(theMatrix.GetRcd(i)->nVisionStatus != VISION_SKIP_DIE))
		{
			nStartRef = i;
			break;
		}
	}

	if (i > nLastRef) //All alignments already done
		return PERFORM_WIRE_BOND;

	for (i = nLastRef; i >= nStartRef; i--)
	{
		if ((theMatrix.GetRcd(i)->nVisionStatus == VISION_SUCCESS) ||
			(theMatrix.GetRcd(i)->nVisionStatus == VISION_SKIP_DIE))
			nLastRef -= 1;
		else
			break;
	}

	if (nLastRef < nStartRef)
		return PERFORM_WIRE_BOND;

	ShowDebugMsg(DEBUG_VISION_TIMING, _T("Perform PR"), nStartRef, nLastRef);

	theServo.GetTablePosition(&stCurTblPos);

	thePgm.CalcStepRepeatOffset(m_nCurMatRow, m_nCurMatCol, 1, 1, &stStepRepeatOffset);

	short nInc = 1;
	for (short j = nStartRef; j <= nLastRef; j++)
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
		{
		case MHS_TOP_LED: case MHS_R2R:
		case MHS_VLED: case MHS_VLED_ELITE:
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			theDevice.GetTopRcd(&stTopDevicePara);
			break;
		}
		case MHS_CU_ELITE:
		{
			theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
			break;
		}
		}
		
		if (j == 0)  //lead PR
		{
			//for the high power 3rd index
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (stTopDevicePara.nHalfIndexOn > 0)       
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (stTopDevicePara.nHalfIndexOn > 0)       
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn)))
			{
				WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);	//v3.1.T90 edit
				if ((pstWire->nRefNum[0] == 0) || (pstWire->nRefNum[1] == 0))
					nInc = max(pstWire->nRefNum[0], pstWire->nRefNum[1]);
				else
					nInc = min(pstWire->nRefNum[0], pstWire->nRefNum[1]);

				i = 0;
				j = nInc - 1;
				//nInc=nLastRef;
			}
			else
			{
				i = 0;
				nInc = nLastRef;
			}
		}
		else  //after lead PR
		{
			if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)	//v3.0.T410 add
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_R2R)
					&& (stTopDevicePara.nHalfIndexOn > 0)
					&& (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_CU_ELITE)
					&& (stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED) 
					&& (stTopDevicePara.nHalfIndexOn > 0)       
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
				|| ((theMcConfig.GetMhsType() == MHS_VLED_ELITE)
					&& (stTopDevicePara.nHalfIndexOn > 0)       
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn)))
			{
				i = j;
				nInc = 1;
			}
			else
			{
				if (nStartRef == 0)
				{
				   i = nLastRef - j + 1;
				   nInc = -1;
				}
				else
				{
					i = j;
					nInc = 1;
				}
			}
		}

		theRef.GetRcd(&stCurRef, i);
		stCurRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
		stCurRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
		stCurRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
		stCurRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

		for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
		{
			stCurRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
			stCurRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
			stCurRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
			stCurRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
		}

		theMatrix.GetRcd(&stMatrix1, i);

		if(stMatrix1.nVisionStatus==VISION_SKIP_DIE)continue;	//V1.1.W250 add

		if (j < nLastRef)
		{
			theRef.GetRcd(&stNextRef, i + nInc);
			stNextRef.stVisionAlignPos[0].dx += stStepRepeatOffset.dx;
			stNextRef.stVisionAlignPos[0].dy += stStepRepeatOffset.dy;
			stNextRef.stVisionAlignPos[1].dx += stStepRepeatOffset.dx;
			stNextRef.stVisionAlignPos[1].dy += stStepRepeatOffset.dy;

			for (k = 0; k < MAX_BAK_UP_PR_NUM; k++)
			{
				stNextRef.stBakUpPr[k].stAlignPos[0].dx += stStepRepeatOffset.dx; 
				stNextRef.stBakUpPr[k].stAlignPos[0].dy += stStepRepeatOffset.dy;
				stNextRef.stBakUpPr[k].stAlignPos[1].dx += stStepRepeatOffset.dx;
				stNextRef.stBakUpPr[k].stAlignPos[1].dy += stStepRepeatOffset.dy;
			}
		}

		if (stMatrix1.nVisionStatus == VISION_SUCCESS)
		{
			continue;
		}

		theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[0].nCoaxRedLight, stCurRef.stVisionLightPara[0].bSideRedLight, stCurRef.stVisionLightPara[0].nCoaxBlueLight, stCurRef.stVisionLightPara[0].bSideBlueLight);

		lMoveDistX = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
		lMoveDistY = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());

		dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
		if ((lMoveDistX > 0) || (lMoveDistY > 0))
		{
			nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
			theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, bSlowMove);

			nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
			theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, bSlowMove);
			Sleep_x1ms(1);

			if (bSlowMove == true)
			{
				nLastProfileX = 4;
				nLastProfileY = 4;
			}

			//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
			theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
		}

		stCurTblPos = stCurRef.stVisionAlignPos[0];

		ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
								stCurRef.nRefNum,
								stCurRef.stVisionAlignPos[0].dx,
								stCurRef.stVisionAlignPos[0].dy);

		stPrPos = stCurRef.stVisionAlignPos[0];
		theVisionSys.EnableVideo(false);

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("start ref"), stCurRef.nRefNum);
		theServo.WaitTableStop();
	    
		//pr srch delay
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
		if (dMoveDist > VISION_LONG_DIST)
			Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
		else if (dMoveDist > 100.0)
			Sleep_x1ms(pstPgm->nVisionMoveDelay);
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

		theVisionSys.GrabImage();
		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

		if (stCurRef.nVisionAlgorithm[0] == 1)
			theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[0].nImageThreshold);

		if (((stCurRef.nRefType == REF_TYPE_LEAD) && (pstPgm->nPrAlignMode == 1)) || //Single point alignment without PR angle
			(stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT))
		{
			//Pre move to next point
			if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
			{
				Sleep_x1ms(20);
				theVisionSys.UpdateLightingParameter(
									stNextRef.stVisionLightPara[0].nCoaxRedLight,
									stNextRef.stVisionLightPara[0].bSideRedLight,
									stNextRef.stVisionLightPara[0].nCoaxBlueLight,
									stNextRef.stVisionLightPara[0].bSideBlueLight);
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_UpdateLightPara"));

				lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrPos"),
							stNextRef.nRefNum,
							stNextRef.stVisionAlignPos[0].dx,
							stNextRef.stVisionAlignPos[0].dy);
			}

			if (stCurRef.nRefType == REF_TYPE_LEAD)
			{
				nSearchRange = pstPgm->nLeadSearchRange;
				nPrPassScore = pstPgm->nLeadPrPassScore;
			}
			else
			{
				nSearchRange = pstPgm->nDieSearchRange;
				nPrPassScore = pstPgm->nPrPassScore;
			}

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), lVisionError);

			theMHS.SetThreadStop(false);

			BOOL bRedoPrEnable = FALSE;
			switch (theMcConfig.GetMhsType())	//v3.0.T151 edit
			{
			case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				bRedoPrEnable = FALSE;
				break;
			}
			case MHS_VLED: case MHS_VLED_ELITE:
			{
				bRedoPrEnable = pstPgm->bPrRetry;
				break;
			}
			}

			if ((lVisionError != FOK) && bRedoPrEnable) 
			{
				lVisionError = RetryPR(&stPrPos, stCurRef.ulVisionAlignPatternID[0], stCurRef.nVisionAlgorithm[0], stCurRef.stVisionLightPara[0].nImageThreshold, nPrPassScore, &stSrchResult);
	            
				if (lVisionError != FOK)
				{
					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITH_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0], MTN_ABS, MTN_WAIT);	//v3.1.T56 edit
					stPrPos = stCurRef.stVisionAlignPos[0];
					stCurTblPos = stCurRef.stVisionAlignPos[0];

					theVisionSys.SetCameraExposureTime(pstPgm->nRetryExposureTime);

					lVisionError = RetryPR(&stPrPos, stCurRef.ulVisionAlignPatternID[0], stCurRef.nVisionAlgorithm[0], stCurRef.stVisionLightPara[0].nImageThreshold, nPrPassScore, &stSrchResult);

					theVisionSys.SetCameraExposureTime(pstPgm->nExposureTime);
				}
			}

			if (lVisionError == FOK)
			{
				m_nSerialSkipNum=0;		//V1.1.W265 add

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/PrResult"),
					stCurRef.nRefNum, stSrchResult.dOffsetX,
					stSrchResult.dOffsetY, stSrchResult.dAngle);

				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/Offset"),
					stCurRef.nRefNum, stVisionOffset.dx, stVisionOffset.dy);

				stMatrix1.nVisionStatus = VISION_SUCCESS;
				m_nAutoSkipDieCount = 0;

				stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
				stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;
				stMatrix1.stVisionAlignPos[1].dx = stMatrix1.stVisionAlignPos[0].dx + stCurRef.stVisionAlignPos[1].dx - stCurRef.stVisionAlignPos[0].dx;
				stMatrix1.stVisionAlignPos[1].dy = stMatrix1.stVisionAlignPos[0].dy + stCurRef.stVisionAlignPos[1].dy - stCurRef.stVisionAlignPos[0].dy;

				if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
				{
					stMatrix1.dCosAln = cos(stSrchResult.dAngle);
					stMatrix1.dSinAln = sin(stSrchResult.dAngle);
				}

				ShowDebugMsg(DEBUG_VISION_TIMING, _T("Ref/MatPos"),
							stCurRef.nRefNum,
							stMatrix1.stVisionAlignPos[0].dx,
							stMatrix1.stVisionAlignPos[0].dy);

				theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
				
				//check current Die overlap
				if ((j > 0) && (stCurRef.nRefType == REF_TYPE_DIE)
					&& isCheckDieOverlap(i,thePgm.GetDeviceSeq()))
					return QUIT_AUTO_BOND;
			}

			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos, 
												&stMatrix1, i, j, nLastRef, nLastProfileX, 
												nLastProfileY, nSearchRange, nPrPassScore);

			//PR failed, show error or skip die/unit
			if (lVisionError != FOK)
			{
				if ((m_bRedoPR == FALSE) && thePgm.IsAutoSkipDie() 
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))	//V1.1.W265 edit
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					m_nSerialSkipNum++;		//V1.1.W265 add
					m_nSkipNumPerIndex++;	//V1.1.W262 add
					theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit

					if (CheckSerialSkipNumLimit(pstPgm) == FALSE)
						return QUIT_AUTO_BOND;		//V1.1.W265 add
				}
				else if ((m_bRedoPR == FALSE) && thePgm.IsAutoSkipUnit() 
							&& (stCurRef.nRefType == REF_TYPE_DIE)
							&& (lVisionError == VISION_ERROR_NO_PATTERN))	//V1.1.W265 edit
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;

					m_nSerialSkipNum++;		//V1.1.W265 add
					m_nSkipNumPerIndex++;	//V1.1.W262 add
					theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);	//V1.1.W250 edit

					if (CheckSerialSkipNumLimit(pstPgm) == FALSE)
						return QUIT_AUTO_BOND;		//V1.1.W265 add
				}
				else
				{
					if (pstPgm->nAutoSkipDieUnit == 0)		//v3.0.T323 add
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);
					}

					theStat.SetMachineState(MCSTATE_STANDBY);

					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
	                
					theServo.SetServoControlParameter();

					if ((pstPgm->nAutoSkipDieUnit == 1)
						&& (theMcConfig.GetMhsType() == MHS_FOXCONN))
						lStopSelected = SHOW_MSG_RESULT_SKIP;	//v3.0.T323 add
					else
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);

					theMHS.SetOxidationProtect(false);
	                
					if ((m_nBondMode != BONDMODE_IDLE) && (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else if (m_bRedoPR == FALSE)	//V1.1.W265 edit
							{
								if (pstPgm->nManualSkip == 0)
									theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);	//V1.1.W262 add
								else
									SetSkipUnit(i);

								m_nSerialSkipNum++;
								m_nSkipNumPerIndex++;
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);

								if (CheckSerialSkipNumLimit(pstPgm) == FALSE)
									return QUIT_AUTO_BOND;
							}
						}
					}

					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_OK)
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
					else
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}

					if ((m_nBondMode != BONDMODE_IDLE) 
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);
	                
					theServo.GetTablePosition(&stCurTblPos);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
		}
		else if ((stCurRef.nRefType == REF_TYPE_DIE) && (pstPgm->bDiePrMultiSrch == true)) //Two point alignment with multi search
		{
			double dRefAlignDist, dMatrixAlignDist, dAlignTol;

			//Pre move to next point
			if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
			{
				theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

				lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
			}

			nSearchRange = pstPgm->nDieSearchRange;
			nPrPassScore = pstPgm->nPrPassScore;
			
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);
			lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
			theMHS.SetThreadStop(false);
			if (lVisionError == FOK)
			{
				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
				stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
				stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

				lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSearchRange, nPrPassScore, &stSrchResult);
				if (lVisionError == FOK)
				{
					theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);

					stMatrix1.nVisionStatus = VISION_SUCCESS;
					m_nAutoSkipDieCount = 0;

					stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
					stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;

					dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0],
												stCurRef.stVisionAlignPos[1]);
					dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0],
												stMatrix1.stVisionAlignPos[1]);
					if (stCurRef.nRefType == REF_TYPE_LEAD)
						dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
					else
						dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

					if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
						lVisionError = FNOK;
					else
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
				}
			}

			if (lVisionError != FOK && stCurRef.nBakUpIndex > 0) 
			{
				int iTryTimes;

				for (iTryTimes = 0; iTryTimes < MAX_BAK_UP_PR_NUM; iTryTimes++)
				{
					if (stCurRef.stBakUpPr[iTryTimes].bValid == true)
					{
						theVisionSys.UpdateLightingParameter(
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideRedLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].nCoaxBlueLight,
							stCurRef.stBakUpPr[iTryTimes].stLight[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dx)
							* theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy
							- stCurRef.stBakUpPr[iTryTimes].stAlignPos[0].dy)
							* theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						theServo.FastMoveTable2(stCurRef.stBakUpPr[iTryTimes].stAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos,
							stCurRef.stBakUpPr[iTryTimes].stAlignPos[0])
							* theServo.GetResolXY();
						stPrPos = stCurRef.stBakUpPr[iTryTimes].stAlignPos[0];

						theServo.WaitTableStop();

						if (dMoveDist > VISION_LONG_DIST)
							Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
						else
							Sleep_x1ms(pstPgm->nVisionMoveDelay);

						theVisionSys.EnableVideo(false);
						theVisionSys.GrabImage();

						if (stCurRef.stBakUpPr[iTryTimes].nAlgo[0] == 0)
							lVisionError = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[0],
								nSearchRange, nPrPassScore, &stSrchResult, iTryTimes);

						if (lVisionError == FOK)
						{
							theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
							stMatrix1.stVisionAlignPos[0].dx = stPrPos.dx + stVisionOffset.dx;
							stMatrix1.stVisionAlignPos[0].dy = stPrPos.dy + stVisionOffset.dy;

							theVisionSys.UpdateLightingParameter(
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideRedLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].nCoaxBlueLight,
								stCurRef.stBakUpPr[iTryTimes].stLight[1].bSideBlueLight);
							theVisionSys.ChangeLensMag(stCurRef.nVisionAlignMag[1]);

							lVisionError = theVisionSys.SearchVisionPattern(
								(short)stCurRef.stBakUpPr[iTryTimes].lPatID[1],
								nSearchRange, nPrPassScore, &stSrchResult, iTryTimes);
							if (lVisionError == FOK)
							{
								theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
	                            
								stMatrix1.nVisionStatus = VISION_SUCCESS;
								m_nAutoSkipDieCount = 0;

								stMatrix1.stVisionAlignPos[1].dx = stPrPos.dx + stVisionOffset.dx;
								stMatrix1.stVisionAlignPos[1].dy = stPrPos.dy + stVisionOffset.dy;

								dRefAlignDist = Calc2PtDist(
											stCurRef.stBakUpPr[iTryTimes].stAlignPos[0],
											stCurRef.stBakUpPr[iTryTimes].stAlignPos[1]);
								dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0],
																stMatrix1.stVisionAlignPos[1]);
								if (stCurRef.nRefType == REF_TYPE_LEAD)
									dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
								else
									dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

								if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
									lVisionError = FNOK;
								else
								   theMatrix.CalcMatrix(&stMatrix1, i, iTryTimes+2);
							}
						}
					}

					if (lVisionError == FOK)
					{
						IcremBkPrCounts(iTryTimes);
						break;
					}
				}
				
				if (lVisionError == FOK)
				{
					//Pre move to next point
					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}
				}
			}

			if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);

					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					theStat.SetMachineState(MCSTATE_STANDBY);

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					if (!thePgm.IsAutoSkipDie() && !thePgm.IsAutoSkipUnit())
					{
						theServo.SetServoControlParameter();
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					}
					else
					{
						if ((i == 0) || (m_nAutoSkipDieCount > 4))
						{
							theServo.SetServoControlParameter();
							lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						}
						else
						{
							lStopSelected = SHOW_MSG_RESULT_SKIP;
							m_nAutoSkipDieCount++;
						}
					}

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (lStopSelected != SHOW_MSG_RESULT_SKIP)
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}
						else
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_SKIP_DIE);
						}
					}

					theMHS.SetOxidationProtect(false);
					if (lStopSelected == 1)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix1.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
						}
						else
						{
							SetSkipUnit(i);
						}

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					}

					continue;
				}
			}
		}
		else if (pstPgm->nPrAlignMode == 0) //Two point alignment
		{
			//Pre move to next point
			if (pstPgm->nVisionPreMove == 1)
			{
				lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

				dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
				stCurTblPos = stCurRef.stVisionAlignPos[1];
			}

			if (stCurRef.nRefType == REF_TYPE_LEAD)
			{
				nSearchRange = pstPgm->nLeadSearchRange;
				nPrPassScore = pstPgm->nLeadPrPassScore;
			}
			else
			{
				nSearchRange = pstPgm->nDieSearchRange;
				nPrPassScore = pstPgm->nPrPassScore;
			}
			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[0],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat1_R"), stCurRef.nRefNum);
			lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[0],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat1"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat1"), lVisionError);
			theMHS.SetThreadStop(false);
			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(0, pstPgm, &stCurRef, &stNextRef, &stCurTblPos,
											&stMatrix1, i, j, nLastRef, nLastProfileX,
											nLastProfileY, nSearchRange, nPrPassScore);

			if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}
	                
					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[0]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[0]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[0];
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);

					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
					theMHS.SetOxidationProtect(false);
					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						stMatrix1.nVisionStatus = VISION_SKIP_DIE;
						theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter(false);
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}
	                
					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						theVisionSys.UpdateLightingParameter(stNextRef.stVisionLightPara[0].nCoaxRedLight, stNextRef.stVisionLightPara[0].bSideRedLight, stNextRef.stVisionLightPara[0].nCoaxBlueLight, stNextRef.stVisionLightPara[0].bSideBlueLight);

						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
			}
			else
			{
				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[0], &stSrchResult, &stVisionOffset);
				stMatrix1.stVisionAlignPos[0].dx = stCurRef.stVisionAlignPos[0].dx + stVisionOffset.dx;
				stMatrix1.stVisionAlignPos[0].dy = stCurRef.stVisionAlignPos[0].dy + stVisionOffset.dy;
			}

			if (pstPgm->nVisionPreMove == 0)
			{
				theVisionSys.UpdateLightingParameter(stCurRef.stVisionLightPara[1].nCoaxRedLight, stCurRef.stVisionLightPara[1].bSideRedLight, stCurRef.stVisionLightPara[1].nCoaxBlueLight, stCurRef.stVisionLightPara[1].bSideBlueLight);

				lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveAlignPos2"));

				dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
				stCurTblPos = stCurRef.stVisionAlignPos[1];
			}

			theServo.WaitTableStop();

			//pr srch delay
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchDelay"));
			if (dMoveDist > VISION_LONG_DIST)
				Sleep_x1ms(pstPgm->nVisionLongMoveDelay);
			else
				Sleep_x1ms(pstPgm->nVisionMoveDelay);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchDelay"));

			theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_GrabImage"));

			if (stCurRef.nVisionAlgorithm[1] == 1)
				theVisionSys.UpdateBinaryThreshold(stCurRef.stVisionLightPara[1].nImageThreshold);

			//Pre move to next point
			if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
			{
				lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileX)
				{
					nLastProfileX = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
					Sleep_x1ms(1);
				}

				lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
				nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
				if (nMoveProfile != nLastProfileY)
				{
					nLastProfileY = nMoveProfile;
					theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
					Sleep_x1ms(1);
				}

				//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
				theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
				ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_MoveNextAlignPos1"));

				dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
			}

			if (stCurRef.nRefType == REF_TYPE_LEAD)
			{
				nSearchRange = pstPgm->nLeadSearchRange;
				nPrPassScore = pstPgm->nLeadPrPassScore;
			}
			else
			{
				nSearchRange = pstPgm->nDieSearchRange;
				nPrPassScore = pstPgm->nPrPassScore;
			}

			theVisionSys.SetPassScore(
						(short)stCurRef.ulVisionAlignPatternID[1],
						pstPgm->nAutoSkipScore);

			ShowDebugMsg(DEBUG_BOND_TIMING, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			ShowDebugMsg(TIME_STAMP_PR, _T("beg_SrchPat2_R"), stCurRef.nRefNum);
			lVisionError = theVisionSys.SearchVisionPattern(
						(short)stCurRef.ulVisionAlignPatternID[1],
						nSearchRange, nPrPassScore, &stSrchResult);
			ShowDebugMsg(TIME_STAMP_PR, _T("end_SrchPat2"), lVisionError);
			ShowDebugMsg(DEBUG_BOND_TIMING, _T("end_SrchPat2"), lVisionError);
			if ((lVisionError != FOK) && (stCurRef.nBakUpIndex > 0))
				lVisionError = TryBackupPR(1, pstPgm, &stCurRef, &stNextRef, &stCurTblPos,
												&stMatrix1, i, j, nLastRef, nLastProfileX,
												nLastProfileY, nSearchRange, nPrPassScore);

			if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
					{
						if (stCurRef.nRefType == REF_TYPE_LEAD)
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
						else
							theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
					}

					lMoveDist = round(fabs(stCurTblPos.dx - stCurRef.stVisionAlignPos[1].dx) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileX)
					{
						nLastProfileX = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
						Sleep_x1ms(1);
					}

					lMoveDist = round(fabs(stCurTblPos.dy - stCurRef.stVisionAlignPos[1].dy) * theServo.GetResolXY());
					nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
					if (nMoveProfile != nLastProfileY)
					{
						nLastProfileY = nMoveProfile;
						theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
						Sleep_x1ms(1);
					}

					//theServo.FastMoveTable(&stCurRef.stVisionAlignPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
					theServo.FastMoveTable2(stCurRef.stVisionAlignPos[1]);	//v3.1.T56 edit
					dMoveDist = Calc2PtDist(stCurTblPos, stCurRef.stVisionAlignPos[1]) * theServo.GetResolXY();
					stCurTblPos = stCurRef.stVisionAlignPos[1];
	                
					theMHS.SetOxidationProtect(true);

					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
					sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));

					theServo.SetServoControlParameter();
					lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
	                
					theMHS.SetOxidationProtect(false);
					if (lStopSelected == SHOW_MSG_RESULT_STOP)
					{
						theVisionSys.EnableVideo(true);
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableIdleLight();
						return QUIT_AUTO_BOND;
					}
					else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
					{
						theTowerLight.EnableBlinkRedLight(false);
						theTowerLight.EnableBondingLight();

						ProcessKeyboard();	//v2.0.T38 add
						theServo.WaitTableStop();

						if (pstPgm->nManualSkip == 0)
						{
							stMatrix1.nVisionStatus = VISION_SKIP_DIE;
							theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
						}
						else
						{
							SetSkipUnit(i);
						}

						if (m_nBondMode == BONDMODE_STOP)
							return QUIT_AUTO_BOND;

						theServo.SetServoControlParameter();
					}
					else
					{
						if (isManualAlignmentStop(i))
							return QUIT_AUTO_BOND;
					}

					theServo.GetTablePosition(&stCurTblPos);

					if ((m_nBondMode != BONDMODE_IDLE)
						&& (m_nBondMode != BONDMODE_STOP))
						theStat.SetMachineState(MCSTATE_PRODUCTION);

					if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
					{
						lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileX)
						{
							nLastProfileX = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
							Sleep_x1ms(1);
						}

						lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
						nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
						if (nMoveProfile != nLastProfileY)
						{
							nLastProfileY = nMoveProfile;
							theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
							Sleep_x1ms(1);
						}

						//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
						theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
						dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
						stCurTblPos = stNextRef.stVisionAlignPos[0];
					}

					continue;
				}
			}
			else
			{
				double dRefAlignDist, dMatrixAlignDist, dAlignTol;
				theVisionSys.VisionOffsetToTablePos(stCurRef.nVisionAlignMag[1], &stSrchResult, &stVisionOffset);

				stMatrix1.nVisionStatus = VISION_SUCCESS;
				stMatrix1.stVisionAlignPos[1].dx = stCurRef.stVisionAlignPos[1].dx + stVisionOffset.dx;
				stMatrix1.stVisionAlignPos[1].dy = stCurRef.stVisionAlignPos[1].dy + stVisionOffset.dy;

				dRefAlignDist = Calc2PtDist(stCurRef.stVisionAlignPos[0], stCurRef.stVisionAlignPos[1]);
				dMatrixAlignDist = Calc2PtDist(stMatrix1.stVisionAlignPos[0], stMatrix1.stVisionAlignPos[1]);
				if (stCurRef.nRefType == REF_TYPE_LEAD)
					dAlignTol = pstPgm->nLeadPrTol / theServo.GetResolXY();
				else
					dAlignTol = pstPgm->nDiePrTol / theServo.GetResolXY();

				if (fabs(dRefAlignDist - dMatrixAlignDist) > dAlignTol)
				{
					if (thePgm.IsAutoSkipDie()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
					{
						theMatrix.SetVisionStatus(stCurRef.nRefNum, VISION_SKIP_DIE);

						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else if (thePgm.IsAutoSkipUnit()
							&& (stCurRef.nRefType == REF_TYPE_DIE))
					{
						SetSkipUnit(i);
						m_nAutoSkipDieCount++;
						m_lTotalAutoSkipDieCount++;
					}
					else
					{
						theTowerLight.EnableErrorLight();
						theTowerLight.EnableBlinkRedLight(true);

						stMatrix1.nVisionStatus = VISION_NOT_DONE;

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
						{
							if (stCurRef.nRefType == REF_TYPE_LEAD)
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_LEAD_PR);
							else
								theStat.SetMachineState(MCSTATE_STANDBY, STATERR_DIE_PR);
						}

						theMHS.SetOxidationProtect(true);
						theServo.SetServoControlParameter();
						sList.RemoveAll();
						sList.AddTail(theMsg.GetMsg(stCurRef.nRefType == REF_TYPE_LEAD ? MSG_LEAD_TOL_FAILURE : MSG_DIE_TOL_FAILURE));
						sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN));
						lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);
						theMHS.SetOxidationProtect(false);

						if (lStopSelected == SHOW_MSG_RESULT_STOP)
						{
							theVisionSys.EnableVideo(true);
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableIdleLight();
							return QUIT_AUTO_BOND;
						}
						else if (lStopSelected == SHOW_MSG_RESULT_SKIP)
						{
							theTowerLight.EnableBlinkRedLight(false);
							theTowerLight.EnableBondingLight();

							ProcessKeyboard();	//v2.0.T38 add
							theServo.WaitTableStop();

							if (pstPgm->nManualSkip == 0)
							{
								stMatrix1.nVisionStatus = VISION_SKIP_DIE;
								theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
							}
							else
							{
								SetSkipUnit(i);
							}

							if (m_nBondMode == BONDMODE_STOP)
								return QUIT_AUTO_BOND;

							theServo.SetServoControlParameter();
						}
						else
						{
							if (isManualAlignmentStop(i))
								return QUIT_AUTO_BOND;
						}

						theServo.GetTablePosition(&stCurTblPos);

						if ((m_nBondMode != BONDMODE_IDLE)
							&& (m_nBondMode != BONDMODE_STOP))
							theStat.SetMachineState(MCSTATE_PRODUCTION);

						if ((j < nLastRef) && (pstPgm->nVisionPreMove == 1))
						{
							lMoveDist = round(fabs(stCurTblPos.dx - stNextRef.stVisionAlignPos[0].dx) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileX)
							{
								nLastProfileX = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
								Sleep_x1ms(1);
							}

							lMoveDist = round(fabs(stCurTblPos.dy - stNextRef.stVisionAlignPos[0].dy) * theServo.GetResolXY());
							nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
							if (nMoveProfile != nLastProfileY)
							{
								nLastProfileY = nMoveProfile;
								theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
								Sleep_x1ms(1);
							}

							//theServo.FastMoveTable(&stNextRef.stVisionAlignPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
							theServo.FastMoveTable2(stNextRef.stVisionAlignPos[0]);	//v3.1.T56 edit
							dMoveDist = Calc2PtDist(stCurTblPos, stNextRef.stVisionAlignPos[0]) * theServo.GetResolXY();
							stCurTblPos = stNextRef.stVisionAlignPos[0];
						}

						continue;
					}
				}
				else
					theMatrix.CalcMatrix(&stMatrix1, i, VISION_PR_ALIGN);
			}

			//PR failed, show error or skip die/unit
			if (lVisionError != FOK)
			{
				if (thePgm.IsAutoSkipDie()
					&& (stCurRef.nRefType == REF_TYPE_DIE)
					&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					theMatrix.SetVisionStatus(i, VISION_SKIP_DIE);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else if (thePgm.IsAutoSkipUnit()
						&& (stCurRef.nRefType == REF_TYPE_DIE)
						&& (lVisionError == VISION_ERROR_NO_PATTERN))
				{
					SetSkipUnit(i);
					m_nAutoSkipDieCount++;
					m_lTotalAutoSkipDieCount++;
				}
				else
				{
					theTowerLight.EnableErrorLight();
					theTowerLight.EnableBlinkRedLight(true);
					theStat.SetMachineState(MCSTATE_STANDBY);
					theMHS.SetOxidationProtect(true);
					PrepareErrorMsg(lVisionError, &stCurRef, &stSrchResult, &sList);
				}
			}
		}

		ShowDebugMsg(DEBUG_BOND_TIMING, _T("end ref"), stCurRef.nRefNum);
	}

	if (m_bIll) thePrAlign.PerformInteliLeadLocate(true);

	nAheadAlignDone = CheckAheadAlignDone(&m_nCurWire);
	if (nAheadAlignDone == 0)
		return PERFORM_VISION_ALIGN;
	else if (nAheadAlignDone == 2)
	{
		if (thePgm.IsAutoSkipDie())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef)
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG, sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		else if (thePgm.IsAutoSkipUnit())
		{
			if (m_lTotalAutoSkipDieCount == nLastRef / thePgm.GetUnitTotalDie())
			{
				theServo.MoveBondHeadToSafePos();
				m_lTotalAutoSkipDieCount = 0;

				sList.AddTail(theMsg.GetMsg(MSG_IS_CONTINUE_AUTO_SKIP));
				if (theMsg.ShowMsgX(ERROR_MSG, sList,0,0,1) == SHOW_MSG_RESULT_STOP)
					return QUIT_AUTO_BOND;
			}
			else
				m_lTotalAutoSkipDieCount = 0;
		}
		return COMPLETE_UNIT_UPDATE;
	}

	return PERFORM_WIRE_BOND;
}
