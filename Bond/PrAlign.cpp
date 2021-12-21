/***************************************************************
PrAlign.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "Math.h"
#include "MathCalc.h"
#include "MsgDef.h"
#include "ScanKeyThread.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "Wire.h"
#include "VisionSystem.h"
#include "Bond.h"
#include "PrAlign.h"
#include "servo.h"

// CPrAlign
CPrAlign thePrAlign;

CPrAlign::CPrAlign()
{
}

CPrAlign::~CPrAlign()
{
}


// Update manual alignment point for edit 

int CPrAlign::UpdateManualAlignPoint(void)
{
    short i, nRefNum, nTotalRef, nTotalWire, nUnitTotalRef, nIncRefNum, nHybRefNum;
    TBL_POS stStepRepeatOffset, stAlignOffset;
    REF_RECORD stRefRcd;
    MATRIX_RECORD stMatrixRcd;
    WIRE_RECORD stWireRcd;

    nTotalWire = thePgm.GetTotalWire();

    if (thePgm.GetDeviceSeq() == PGM_MATRIX)
        thePgm.CalcStepRepeatOffset(theBond.m_nCurMatRow,
										theBond.m_nCurMatCol,
										theBond.m_nCurRow,
										theBond.m_nCurCol,
										&stStepRepeatOffset);
    else
        thePgm.CalcStepRepeatOffset(theBond.m_nCurMatRow,
										theBond.m_nCurMatCol,
										1, 1, &stStepRepeatOffset);

    nTotalRef = thePgm.GetTotalRef();

	switch (thePgm.GetDeviceSeq())
	{
	case PGM_MATRIX: case PGM_SINGLE:
        nUnitTotalRef = nTotalRef;
		break;
	case PGM_HYBRID: case PGM_DIE_AHEAD: case PGM_DIE_LEAD_AHEAD:
	case PGM_MAT_HYBRID: case PGM_MAT_DIE_AHEAD:
        nUnitTotalRef = thePgm.GetUnitTotalDie() + 1;
		break;
	}

    for (nRefNum = 0; nRefNum < nUnitTotalRef; nRefNum++)
    {
        theMatrix.GetRcd(&stMatrixRcd, nRefNum);

        if (stMatrixRcd.nVisionStatus != VISION_SUCCESS)
            continue;

        stAlignOffset.dx = stMatrixRcd.stUpdatedCenterPos.dx - (stMatrixRcd.stOriginalCenterPos.dx + stStepRepeatOffset.dx);
        stAlignOffset.dy = stMatrixRcd.stUpdatedCenterPos.dy - (stMatrixRcd.stOriginalCenterPos.dy + stStepRepeatOffset.dy);

        stMatrixRcd.stOriginalCenterPos = stMatrixRcd.stUpdatedCenterPos;
        theMatrix.SetRcd(&stMatrixRcd, nRefNum);

		switch (thePgm.GetDeviceSeq())
		{
		case PGM_MATRIX: case PGM_SINGLE:
            nIncRefNum = nTotalRef;
			break;
		case PGM_HYBRID: case PGM_DIE_AHEAD: case PGM_MAT_HYBRID:
		case PGM_MAT_DIE_AHEAD:
        {
            if (nRefNum == 0)
                nIncRefNum = nTotalRef;
            else
                nIncRefNum = thePgm.GetUnitTotalDie();
			break;
        }
		case PGM_DIE_LEAD_AHEAD:
            nIncRefNum = thePgm.GetUnitTotalDie() + 1;
			break;
        }
        
        nHybRefNum = nRefNum;

        while (nHybRefNum < nTotalRef)
        {
            theRef.GetRcd(&stRefRcd, nHybRefNum);

            stRefRcd.stManuAlignPos[0].dx += stAlignOffset.dx;
            stRefRcd.stManuAlignPos[0].dy += stAlignOffset.dy;

            stRefRcd.stManuAlignPos[1].dx += stAlignOffset.dx;
            stRefRcd.stManuAlignPos[1].dy += stAlignOffset.dy;

            stRefRcd.stVisionAlignPos[0].dx += stAlignOffset.dx;
            stRefRcd.stVisionAlignPos[0].dy += stAlignOffset.dy;

            stRefRcd.stVisionAlignPos[1].dx += stAlignOffset.dx;
            stRefRcd.stVisionAlignPos[1].dy += stAlignOffset.dy;

			if (stRefRcd.nBakUpIndex > MAX_BAK_UP_PR_NUM)
				stRefRcd.nBakUpIndex = MAX_BAK_UP_PR_NUM;
            for (i = 0; i < stRefRcd.nBakUpIndex; i++)
            {
                stRefRcd.stBakUpPr[i].stAlignPos[0].dx += stAlignOffset.dx;
                stRefRcd.stBakUpPr[i].stAlignPos[0].dy += stAlignOffset.dy;
                stRefRcd.stBakUpPr[i].stAlignPos[1].dx += stAlignOffset.dx;
                stRefRcd.stBakUpPr[i].stAlignPos[1].dy += stAlignOffset.dy;
            }

            theRef.SetRcd(&stRefRcd, nHybRefNum);

            for (i = 1; i <= nTotalWire; i++)
            {
                theWire.GetRcd(&stWireRcd, i);
                if (stWireRcd.nRefNum[0] == nHybRefNum)
                {
                    stWireRcd.stTchPos[0].dx += stAlignOffset.dx;
                    stWireRcd.stTchPos[0].dy += stAlignOffset.dy;
                }

                if (stWireRcd.nRefNum[1] == nHybRefNum)
                {
                    stWireRcd.stTchPos[1].dx += stAlignOffset.dx;
                    stWireRcd.stTchPos[1].dy += stAlignOffset.dy;
                }
                theWire.SetRcd(&stWireRcd, i);
            }

            nHybRefNum += nIncRefNum;
        }
    }

    return 0;
}

// Perform inteligent lead locate with or without PR
int CPrAlign::PerformInteliLeadLocate(bool bWithPR)
{
    short   nWireNum, nNextWireNum, nTotalWire, nMoveProfile;
    short   nLastProfileX = 0, nLastProfileY = 0, nAng, nLeadTipOffset;
    int     iStatus = FOK, iIllStatus = FOK, lStopSelected = SHOW_MSG_RESULT_OK;
    long    lMoveDistX, lMoveDistY, lMoveDist, lVisionError = FOK;
    double  dCos, dSin, dWireLength, dLeadCenterX, dLeadCenterY;
    WIRE_RECORD stWireRcd;
    PR_SRCH_RESULT stSrchResult;
    TBL_POS stCurTblPos, stCurTchPos, stNextTchPos, stVisionOffset;
	TBL_POS st1stBondPt, st2ndBondPt, stLeadTipOffset;
    CStringList sList;

	PGM_RECORD *pstPgm = thePgm.GetRcd();

    nTotalWire = theWire.GetTotalWireNum();

    theScanKeyThread.DisableJoyStick();

    theServo.GetTablePosition(&stCurTblPos);

    stLeadTipOffset.dx = pstPgm->nLeadTipOffset / theServo.GetResolXY();
    stLeadTipOffset.dy = 0.0;

    theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stLeadTipOffset, &stLeadTipOffset);
    nLeadTipOffset = round(sqrt(stLeadTipOffset.dx * stLeadTipOffset.dx + stLeadTipOffset.dy * stLeadTipOffset.dy));

    theVisionSys.UpdateLightingParameter(
								pstPgm->stVllLight.nCoaxRedLight,
								pstPgm->stVllLight.bSideRedLight,
								pstPgm->stVllLight.nCoaxBlueLight,
								pstPgm->stVllLight.bSideBlueLight);

    iStatus = GetNextILLWire(0, FIRST_BOND, &nWireNum);
    if (iStatus == FOK)
    {
        while (nWireNum <= nTotalWire)
        {
            theWire.GetRcd(&stWireRcd, nWireNum);
            theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

            stCurTchPos = st1stBondPt;

            if (stWireRcd.nRefNum[FIRST_BOND] == 0)
            {
                lMoveDistX = round(fabs(stCurTblPos.dx - stCurTchPos.dx) * theServo.GetResolXY());
                lMoveDistY = round(fabs(stCurTblPos.dy - stCurTchPos.dy) * theServo.GetResolXY());

                if ((lMoveDistX > 0) || (lMoveDistY > 0))
                {
                    nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);

                    nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep(1);

                    theServo.FastMoveTable(&stCurTchPos, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
    
                    stCurTblPos = stCurTchPos;
                }

                theServo.WaitTableStop();
                Sleep(pstPgm->nVllDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();
                dWireLength = Calc2PtDist(st2ndBondPt, stCurTchPos);
		        if (dWireLength == 0) //throw "w.temp.53" ;
				{
					AfxMessageBox(_T("temp.53, dWireLength == 0"));
					return 0;
				}
                dCos = (stCurTchPos.dx - st2ndBondPt.dx) / dWireLength;
                dSin = (stCurTchPos.dy - st2ndBondPt.dy) / dWireLength;

                if (dSin > 0)
                    nAng = 360 - round(acos(dCos) * 180.0 / PI);
                else
                    nAng = round(acos(dCos) * 180.0 / PI);

                iStatus = GetNextILLWire(nWireNum, FIRST_BOND, &nNextWireNum);
                if (iStatus == FOK)
                {
                    theWire.CalcBondPoint(nNextWireNum, &st1stBondPt, &st2ndBondPt);

                    stNextTchPos = st1stBondPt;

                    lMoveDist = round(fabs(stCurTblPos.dx - stNextTchPos.dx) * theServo.GetResolXY()); //@82
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextTchPos.dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&stNextTchPos, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    stCurTblPos = stNextTchPos;
                }

                iIllStatus = theVisionSys.LocateLeadCenter(0, 0, nAng,
					pstPgm->stVllLight.nImageThreshold,
                    nLeadTipOffset, &dLeadCenterX, &dLeadCenterY);
                
                if (iIllStatus == FOK)
                {
                    stSrchResult.dOffsetX = dLeadCenterX;
                    stSrchResult.dOffsetY = dLeadCenterY;
                    stSrchResult.dAngle   = 0.0;
                    stSrchResult.dScore   = 100.0;

                    theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stVisionOffset);

                    stCurTchPos.dx += stVisionOffset.dx;
                    stCurTchPos.dy += stVisionOffset.dy;

                    stCurTchPos.dx += (double)stWireRcd.n1stPtLeadOffsetX / theServo.GetResolXY();
                    stCurTchPos.dy += (double)stWireRcd.n1stPtLeadOffsetY / theServo.GetResolXY();

                    if (bWithPR == true)
                        theMatrix.ReverseTransformPos(stWireRcd.nRefNum[0], &stCurTchPos);

                    RoundPosition(&stCurTchPos);

                    stWireRcd.stTchPos[0] = stCurTchPos;
                }
                else
                {
                    theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

                    stCurTchPos = st1stBondPt;

                    lMoveDist = round(fabs(stCurTblPos.dx - stCurTchPos.dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stCurTchPos.dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&stCurTchPos, MOTION_ABSOLUTE, MOTION_WITH_WAIT); //@2
                    stCurTblPos = stCurTchPos;

                    sList.RemoveAll();
                    sList.AddTail(theMsg.GetMsg(MSG_LEAD_LOCATE_FAILURE));
                    sList.AddTail(theMsg.GetMsg(MSG_MANUAL_LOCATE_LEAD)); 

                    theServo.SetServoControlParameter(); //@2

                    theVisionSys.EnableVideo(true);
                    theVisionDisplay.UpdateVisionDisplay(true);
                    theScanKeyThread.EnableJoyStick();
                    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true, true);
                    theScanKeyThread.DisableJoyStick();

                    if (lStopSelected == SHOW_MSG_RESULT_OK)
                    {
                        if (!theMcConfig.IsBypassAll())
                        {
                            theServo.GetTablePosition(&stCurTchPos);
                            if (bWithPR == true)
                                theMatrix.ReverseTransformPos(stWireRcd.nRefNum[0], &stCurTchPos);
                            stWireRcd.stTchPos[0] = stCurTchPos;
                        }

                        theServo.SetServoControlParameter(false); //@2
                    }
                    else if (lStopSelected == SHOW_MSG_RESULT_STOP)
                    {
                        break;
                    }
                }

                theWire.SetRcd(&stWireRcd, nWireNum);

                if (bWithPR == true)
                    theWire.UpdateEditedWirePoint(stWireRcd.nWireNum);

                if (iStatus == FOK)
                {
                    nWireNum = nNextWireNum;
                }
                else
                    break;
            }
        }
    }

    theServo.GetTablePosition(&stCurTblPos);

    iStatus = GetNextILLWire(0, SECOND_BOND, &nWireNum);
    if (iStatus == FOK)
    {
        while (nWireNum <= nTotalWire)
        {
            theWire.GetRcd(&stWireRcd, nWireNum);
            theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

            stCurTchPos = st2ndBondPt;

            if (stWireRcd.nRefNum[SECOND_BOND] == 0)
            {
                lMoveDistX = round(fabs(stCurTblPos.dx - stCurTchPos.dx) * theServo.GetResolXY());
                lMoveDistY = round(fabs(stCurTblPos.dy - stCurTchPos.dy) * theServo.GetResolXY());

                if ((lMoveDistX > 0) || (lMoveDistY > 0))
                {
                    nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);

                    nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep(1);

                    theServo.FastMoveTable(&stCurTchPos, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
    
                    stCurTblPos = stCurTchPos;
                }

                theServo.WaitTableStop();
                Sleep(pstPgm->nVllDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();
		        if(dWireLength ==0) //throw "w.temp.54" ;
				{
					AfxMessageBox(_T("temp.54, dWireLength == 0"));
					return 0;
				}
                dWireLength = Calc2PtDist(st1stBondPt, stCurTchPos);
                dCos = (stCurTchPos.dx - st1stBondPt.dx) / dWireLength;
                dSin = (stCurTchPos.dy - st1stBondPt.dy) / dWireLength;

                if (dSin < 0)
                    nAng = 360 - round(acos(dCos) * 180.0 / PI);
                else
                    nAng = round(acos(dCos) * 180.0 / PI);

                iStatus = GetNextILLWire(nWireNum, SECOND_BOND, &nNextWireNum);
                if (iStatus == FOK)
                {
                    theWire.CalcBondPoint(nNextWireNum, &st1stBondPt, &st2ndBondPt);

                    stNextTchPos = st2ndBondPt;

                    lMoveDist = round(fabs(stCurTblPos.dx - stNextTchPos.dx) * theServo.GetResolXY()); //@82
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextTchPos.dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&stNextTchPos, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    stCurTblPos = stNextTchPos;
                }

                iIllStatus = theVisionSys.LocateLeadCenter(0, 0,
					nAng, pstPgm->stVllLight.nImageThreshold,
                    nLeadTipOffset, &dLeadCenterX, &dLeadCenterY);

                if (iIllStatus == FOK)
                {
                    stSrchResult.dOffsetX = dLeadCenterX;
                    stSrchResult.dOffsetY = dLeadCenterY;
                    stSrchResult.dAngle   = 0.0;
                    stSrchResult.dScore   = 100.0;

                    theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stVisionOffset);

                    stCurTchPos.dx += stVisionOffset.dx;
                    stCurTchPos.dy += stVisionOffset.dy;

                    stCurTchPos.dx += (double)stWireRcd.n2ndPtLeadOffsetX / theServo.GetResolXY();
                    stCurTchPos.dy += (double)stWireRcd.n2ndPtLeadOffsetY / theServo.GetResolXY();

                    if (bWithPR == true)
                        theMatrix.ReverseTransformPos(stWireRcd.nRefNum[1], &stCurTchPos);

                    RoundPosition(&stCurTchPos);

                    stWireRcd.stTchPos[1] = stCurTchPos;
                }
                else
                {
                    theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

                    stCurTchPos = st2ndBondPt;

                    lMoveDist = round(fabs(stCurTblPos.dx - stCurTchPos.dx) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stCurTchPos.dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&stCurTchPos, MOTION_ABSOLUTE, MOTION_WITH_WAIT); //@2
                    stCurTblPos = stCurTchPos;

                    sList.RemoveAll();
                    sList.AddTail(theMsg.GetMsg(MSG_LEAD_LOCATE_FAILURE));
                    sList.AddTail(theMsg.GetMsg(MSG_MANUAL_LOCATE_LEAD)); 

                    theServo.SetServoControlParameter(); //@2

                    theVisionSys.EnableVideo(true);
                    theVisionDisplay.UpdateVisionDisplay(true);
                    theScanKeyThread.EnableJoyStick();
                    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true, true);
                    theScanKeyThread.DisableJoyStick();

                    stWireRcd.stTchPos[1] = stCurTchPos;

                    if (lStopSelected == SHOW_MSG_RESULT_OK)
                    {
                        if (!theMcConfig.IsBypassAll())
                        {
                            theServo.GetTablePosition(&stCurTchPos);
                            if (bWithPR == true)
                                theMatrix.ReverseTransformPos(stWireRcd.nRefNum[1], &stCurTchPos);
                            stWireRcd.stTchPos[1] = stCurTchPos;
                        }
                        
                        theServo.SetServoControlParameter(false); //@2
                    }
                    else if (lStopSelected == SHOW_MSG_RESULT_STOP)
                    {
                        break;
                    }
                }

                theWire.SetRcd(&stWireRcd, nWireNum);

                if (bWithPR == true)
                    theWire.UpdateEditedWirePoint(stWireRcd.nWireNum);

                if (iStatus == FOK)
                {
                    nWireNum = nNextWireNum;
                }
                else
                    break;
            }
        }
    }

    //theServo.SetServoControlParameter(true); //@2
    theScanKeyThread.EnableJoyStick();

    theVisionSys.EnableVideo(true);

    return 0;
}

// Get next wire number for ILL
int CPrAlign::GetNextILLWire(short nCurWireNum, short nBondPt, short* pnNextWireNum) //@1
{
    short nNextWireNum, nTotalWire;
    WIRE_RECORD stWireRcd;

    nTotalWire = theWire.GetTotalWireNum();

    if (nCurWireNum >= nTotalWire) return FNOK;

    for (nNextWireNum = nCurWireNum + 1; nNextWireNum <= nTotalWire; nNextWireNum++)
    {
        theWire.GetRcd(&stWireRcd, nNextWireNum);
        if (stWireRcd.nRefNum[nBondPt] == 0)
        {
            if (((nBondPt == FIRST_BOND) && (stWireRcd.b1stPtILL == true)) ||
                ((nBondPt == SECOND_BOND) && (stWireRcd.b2ndPtILL == true)))
            {
                break;
            }
        }
    }

    if (nNextWireNum <= nTotalWire)
    {
        *pnNextWireNum = nNextWireNum;
        return FOK;
    }
    else
        return FNOK;
}
