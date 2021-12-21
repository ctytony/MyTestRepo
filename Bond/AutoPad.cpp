/***************************************************************
AutoPad.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "Math.h"
#include "MathCalc.h"
#include "MsgDef.h"
#include "AutoPadMgr.h"
#include "VisionSystem.h"
#include "Wire.h"
#include "AutoPad.h"
#include "Bond.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "Matrix.h"
#include "ScanKeyThread.h"
#include "servo.h"

CAutoPad theAutoPad;

// CAutoPad

CAutoPad::CAutoPad()
{
}

CAutoPad::~CAutoPad()
{
}


// Perform auto pad without PR during teach bond point

int CAutoPad::PerformAutoPadWithoutPR(void)
{
    short   nWireNum, nNextWireNum, nTotalWire, nMoveProfile, nLastProfileX = 0, nLastProfileY = 0;
    int     iStatus = FOK, lStopSelected = SHOW_MSG_RESULT_OK;
    long    lMoveDistX, lMoveDistY, lMoveDist, lVisionError = FOK;
    double  dMoveDist;
    WIRE_RECORD stWireRcd, stNextWireRcd;
    TBL_POS     stCurTblPos, stSrchRangeEnc, stSrchRangePr;
    AUTO_PAD_TYPE stPadRcd;
    PR_SRCH_RESULT  stSrchResult;
    CStringList sList;

	const PGM_RECORD *pstPgm = thePgm.GetRcd();

    nTotalWire = theWire.GetTotalWireNum();

    theServo.SetServoControlParameter(false);

    theScanKeyThread.DisableJoyStick();

    theServo.GetTablePosition(&stCurTblPos);

    iStatus = GetNextAutoPadWire(0, FIRST_BOND, &nWireNum);
    if (iStatus == FOK)
    {
        while (nWireNum <= nTotalWire)
        {
            theWire.GetRcd(&stWireRcd, nWireNum);
            if (stWireRcd.chPadType[0] > 0)
            {
                lMoveDistX = round(fabs(stCurTblPos.dx - stWireRcd.stTchPos[0].dx) * theServo.GetResolXY());
                lMoveDistY = round(fabs(stCurTblPos.dy - stWireRcd.stTchPos[0].dy) * theServo.GetResolXY());

                if ((lMoveDistX > 0) || (lMoveDistY > 0))
                {
                    nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);

                    nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep(1);

                    theServo.FastMoveTable(&stWireRcd.stTchPos[0], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
                    dMoveDist = Calc2PtDist(stCurTblPos, stWireRcd.stTchPos[0]) * theServo.GetResolXY();
    
                    stCurTblPos = stWireRcd.stTchPos[0];
                }

                theAutoPadMgr.GetRcd(stWireRcd.chPadType[0] - 1, &stPadRcd);

                theVisionSys.UpdateLightingParameter(stPadRcd.stPrLightPara.nCoaxRedLight,
                    stPadRcd.stPrLightPara.bSideRedLight,
                    stPadRcd.stPrLightPara.nCoaxBlueLight,
                    stPadRcd.stPrLightPara.bSideBlueLight);

                theServo.WaitTableStop();

                Sleep(pstPgm->nInteliPadLocateDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();

                iStatus = GetNextAutoPadWire(nWireNum, FIRST_BOND, &nNextWireNum);
                if (iStatus == FOK)
                {
                    theWire.GetRcd(&stNextWireRcd, nNextWireNum);

                    lMoveDist = round(fabs(stCurTblPos.dx - stNextWireRcd.stTchPos[0].dx) * theServo.GetResolXY()); //@82
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextWireRcd.stTchPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&(stNextWireRcd.stTchPos[0]), MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    dMoveDist = Calc2PtDist(stCurTblPos, stNextWireRcd.stTchPos[0]) * theServo.GetResolXY();
                    stCurTblPos = stNextWireRcd.stTchPos[0];
                }

                stSrchRangeEnc.dx = stPadRcd.nPadWidth * 1.5;
                stSrchRangeEnc.dy = stPadRcd.nPadHeight * 1.5;

                theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stSrchRangeEnc, &stSrchRangePr);

                lVisionError = theVisionSys.SearchVisionPattern(
									(short)stPadRcd.ulPrPatternID,
									round(stSrchRangePr.dx),
									round(stSrchRangePr.dy),
									pstPgm->nPrPassScore,
									&stSrchResult);

                if (lVisionError == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stSrchRangeEnc);

                    stWireRcd.stTchPos[0].dx += stSrchRangeEnc.dx;
                    stWireRcd.stTchPos[0].dy += stSrchRangeEnc.dy;
                }
                else
                {
                    lMoveDist = round(fabs(stCurTblPos.dx - stWireRcd.stTchPos[0].dx) * theServo.GetResolXY()); //@82
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stWireRcd.stTchPos[0].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&(stWireRcd.stTchPos[0]), MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    dMoveDist   = Calc2PtDist(stCurTblPos, stWireRcd.stTchPos[0]) * theServo.GetResolXY();
                    stCurTblPos = stWireRcd.stTchPos[0];

                    sList.RemoveAll();
                    sList.AddTail(theMsg.GetMsg(MSG_PAD_LOCATE_FAIL));
                    sList.AddTail(theMsg.GetMsg(MSG_MANUAL_LOCATE_PAD)); 

                    theVisionSys.EnableVideo(true);
                    theScanKeyThread.EnableJoyStick();
                    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true, true);
                    theScanKeyThread.DisableJoyStick();

                    if (lStopSelected == SHOW_MSG_RESULT_OK)
                    {
                        if (!theMcConfig.IsBypassAll())
                            theServo.GetTablePosition(&stWireRcd.stTchPos[0]);
                    }
                    else if (lStopSelected == SHOW_MSG_RESULT_STOP)
                    {
                        break;
                    }
                }

                theWire.SetRcd(&stWireRcd, nWireNum);

                if (iStatus == FOK)
                    nWireNum = nNextWireNum;
                else
                    break;
            }
        }
    }

    iStatus = GetNextAutoPadWire(0, SECOND_BOND, &nWireNum);
    if (iStatus == FOK)
    {
        while (nWireNum <= nTotalWire)
        {
            theWire.GetRcd(&stWireRcd, nWireNum);
            if (stWireRcd.chPadType[1] > 0)
            {
                lMoveDistX = round(fabs(stCurTblPos.dx - stWireRcd.stTchPos[1].dx) * theServo.GetResolXY());
                lMoveDistY = round(fabs(stCurTblPos.dy - stWireRcd.stTchPos[1].dy) * theServo.GetResolXY());

                if ((lMoveDistX > 0) || (lMoveDistY > 0))
                {
                    nLastProfileX = GetTableAutoSpeedProfile(lMoveDistX);
                    theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);

                    nLastProfileY = GetTableAutoSpeedProfile(lMoveDistY);
                    theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                    Sleep(1);

                    theServo.FastMoveTable(&stWireRcd.stTchPos[1], MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT);
                    dMoveDist = Calc2PtDist(stCurTblPos, stWireRcd.stTchPos[1]) * theServo.GetResolXY();

                    stCurTblPos = stWireRcd.stTchPos[1];
                }

                theAutoPadMgr.GetRcd(stWireRcd.chPadType[1] - 1, &stPadRcd);

                theVisionSys.UpdateLightingParameter(stPadRcd.stPrLightPara.nCoaxRedLight,
                    stPadRcd.stPrLightPara.bSideRedLight,
                    stPadRcd.stPrLightPara.nCoaxBlueLight,
                    stPadRcd.stPrLightPara.bSideBlueLight);

                theServo.WaitTableStop();

                Sleep(pstPgm->nInteliPadLocateDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();

                iStatus = GetNextAutoPadWire(nWireNum, SECOND_BOND, &nNextWireNum);
                if (iStatus == FOK)
                {
                    theWire.GetRcd(&stNextWireRcd, nNextWireNum);

                    lMoveDist = round(fabs(stCurTblPos.dx - stNextWireRcd.stTchPos[1].dx) * theServo.GetResolXY()); //@82
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stNextWireRcd.stTchPos[1].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&(stNextWireRcd.stTchPos[1]), MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    dMoveDist = Calc2PtDist(stCurTblPos, stNextWireRcd.stTchPos[1]) * theServo.GetResolXY();
                    stCurTblPos = stNextWireRcd.stTchPos[1];
                }

                stSrchRangeEnc.dx = stPadRcd.nPadWidth * 1.5;
                stSrchRangeEnc.dy = stPadRcd.nPadHeight * 1.5;

                theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stSrchRangeEnc, &stSrchRangePr);

                lVisionError = theVisionSys.SearchVisionPattern(
									(short)stPadRcd.ulPrPatternID,
									round(stSrchRangePr.dx),
									round(stSrchRangePr.dy),
									pstPgm->nPrPassScore,
									&stSrchResult);

                if (lVisionError == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stSrchRangeEnc);

                    stWireRcd.stTchPos[1].dx += stSrchRangeEnc.dx;
                    stWireRcd.stTchPos[1].dy += stSrchRangeEnc.dy;
                }
                else
                {
                    lMoveDist = round(fabs(stCurTblPos.dx - stWireRcd.stTchPos[1].dx) * theServo.GetResolXY()); //@82
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileX)
                    {
                        nLastProfileX = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_X, nLastProfileX, false);
                        Sleep(1);
                    }

                    lMoveDist = round(fabs(stCurTblPos.dy - stWireRcd.stTchPos[1].dy) * theServo.GetResolXY());
                    nMoveProfile = GetTableAutoSpeedProfile(lMoveDist);
                    if (nMoveProfile != nLastProfileY)
                    {
                        nLastProfileY = nMoveProfile;
                        theServo.SetServoSpeedProfile(MOTOR_Y, nLastProfileY, false);
                        Sleep(1);
                    }

                    theServo.FastMoveTable(&(stWireRcd.stTchPos[1]), MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    dMoveDist = Calc2PtDist(stCurTblPos, stWireRcd.stTchPos[1]) * theServo.GetResolXY();
                    stCurTblPos = stWireRcd.stTchPos[1];

                    sList.RemoveAll();
                    sList.AddTail(theMsg.GetMsg(MSG_PAD_LOCATE_FAIL));
                    sList.AddTail(theMsg.GetMsg(MSG_MANUAL_LOCATE_PAD)); 

                    theVisionSys.EnableVideo(true);
                    theScanKeyThread.EnableJoyStick();
                    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true, true);
                    theScanKeyThread.DisableJoyStick();

                    if (lStopSelected == SHOW_MSG_RESULT_OK)
                    {
                        if (!theMcConfig.IsBypassAll())
                            theServo.GetTablePosition(&stWireRcd.stTchPos[0]);
                    }
                    else if (lStopSelected == SHOW_MSG_RESULT_STOP)
                    {
                        break;
                    }
                }

                theWire.SetRcd(&stWireRcd, nWireNum);

                if (iStatus == FOK)
                    nWireNum = nNextWireNum;
                else
                    break;
            }
        }
    }

    theServo.SetServoControlParameter(true);
    theScanKeyThread.EnableJoyStick();

    theVisionSys.EnableVideo(true);

    return 0;
}

// Get wire number of next auto pad
int CAutoPad::GetNextAutoPadWire(short nCurWireNum, short nBondPt, short* pnNextWireNum)
{
    short nNextWireNum, nTotalWire;
    WIRE_RECORD stWireRcd;

    nTotalWire = theWire.GetTotalWireNum();

    if (nCurWireNum >= nTotalWire) return FNOK;

    for (nNextWireNum = nCurWireNum + 1; nNextWireNum <= nTotalWire; nNextWireNum++)
    {
        theWire.GetRcd(&stWireRcd, nNextWireNum);
        if (stWireRcd.chPadType[nBondPt] > 0)
            break;
    }

    if (nNextWireNum <= nTotalWire)
    {
        *pnNextWireNum = nNextWireNum;
        return FOK;
    }
    else
        return FNOK;
}

// Perform auto pad with PR
int CAutoPad::PerformAutoPadWithPR(void)
{
    short   nWireNum, nNextWireNum, nTotalWire, nMoveProfile, nLastProfileX = 0, nLastProfileY = 0;
    int     iStatus = FOK, lStopSelected = SHOW_MSG_RESULT_OK;
    long    lMoveDistX, lMoveDistY, lMoveDist, lVisionError = FOK;
    double  dMoveDist;
    WIRE_RECORD stWireRcd, stNextWireRcd;
    TBL_POS     stCurTblPos, stSrchRangeEnc, stSrchRangePr, st1stBondPt, st2ndBondPt, stCurTchPos, stNextTchPos;
    AUTO_PAD_TYPE stPadRcd;
    PR_SRCH_RESULT stSrchResult;
    CStringList sList;

	const PGM_RECORD *pstPgm = thePgm.GetRcd();

	switch (thePgm.GetDeviceSeq())
	{
	case PGM_SINGLE: case PGM_MATRIX:
        nTotalWire = theWire.GetTotalWireNum();
		break;
	default:
        nTotalWire = thePgm.GetUnitTotalWire();
	}

    theServo.SetServoControlParameter(false);

    theScanKeyThread.DisableJoyStick();

    theServo.GetTablePosition(&stCurTblPos);

    iStatus = GetNextAutoPadWire(0, FIRST_BOND, &nWireNum);
    if (iStatus == FOK)
    {
        while (nWireNum <= nTotalWire)
        {
            theWire.GetRcd(&stWireRcd, nWireNum);
            theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

            stCurTchPos = st1stBondPt;

            if (stWireRcd.chPadType[0] > 0)
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
                    dMoveDist = Calc2PtDist(stCurTblPos, stCurTchPos) * theServo.GetResolXY();

                    stCurTblPos = stCurTchPos;
                }

                theAutoPadMgr.GetRcd(stWireRcd.chPadType[0] - 1, &stPadRcd);

                theVisionSys.UpdateLightingParameter(stPadRcd.stPrLightPara.nCoaxRedLight,
                    stPadRcd.stPrLightPara.bSideRedLight,
                    stPadRcd.stPrLightPara.nCoaxBlueLight,
                    stPadRcd.stPrLightPara.bSideBlueLight);

                theServo.WaitTableStop();

                Sleep(pstPgm->nInteliPadLocateDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();

                iStatus = GetNextAutoPadWire(nWireNum, FIRST_BOND, &nNextWireNum);
                if (iStatus == FOK)
                {
                    theWire.GetRcd(&stNextWireRcd, nNextWireNum);
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
                    dMoveDist = Calc2PtDist(stCurTblPos, stNextTchPos) * theServo.GetResolXY();
                    stCurTblPos = stNextTchPos;
                }

                stSrchRangeEnc.dx = stPadRcd.nPadWidth * 1.5;
                stSrchRangeEnc.dy = stPadRcd.nPadHeight * 1.5;

                theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stSrchRangeEnc, &stSrchRangePr);

                lVisionError = theVisionSys.SearchVisionPattern(
									(short)stPadRcd.ulPrPatternID,
									round(stSrchRangePr.dx),
									round(stSrchRangePr.dy),
									pstPgm->nPrPassScore,
									&stSrchResult);

                if (lVisionError == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stSrchRangeEnc);

                    stCurTchPos.dx += stSrchRangeEnc.dx;
                    stCurTchPos.dy += stSrchRangeEnc.dy;

                    theMatrix.ReverseTransformPos(stWireRcd.nRefNum[0], &stCurTchPos);
                    stWireRcd.stTchPos[0] = stCurTchPos;
                }
                else
                {
                    theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

                    stCurTchPos = st1stBondPt;

                    lMoveDist = round(fabs(stCurTblPos.dx - stCurTchPos.dx) * theServo.GetResolXY()); //@82
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

                    theServo.FastMoveTable(&stCurTchPos, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    dMoveDist = Calc2PtDist(stCurTblPos, stCurTchPos) * theServo.GetResolXY();
                    stCurTblPos = stCurTchPos;

                    sList.RemoveAll();
                    sList.AddTail(theMsg.GetMsg(MSG_PAD_LOCATE_FAIL));
                    sList.AddTail(theMsg.GetMsg(MSG_MANUAL_LOCATE_PAD)); 

                    theVisionSys.EnableVideo(true);
                    theScanKeyThread.EnableJoyStick();
                    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true, true);
                    theScanKeyThread.DisableJoyStick();

                    if (lStopSelected == SHOW_MSG_RESULT_OK)
                    {
                        if (!theMcConfig.IsBypassAll())
                        {
                            theServo.GetTablePosition(&stCurTchPos);
                            theMatrix.ReverseTransformPos(stWireRcd.nRefNum[0], &stCurTchPos);
                            stWireRcd.stTchPos[0] = stCurTchPos;
                        }
                    }
                    else if (lStopSelected == SHOW_MSG_RESULT_STOP)
                    {
                        break;
                    }
                }

                theWire.SetRcd(&stWireRcd, nWireNum);
                theWire.UpdateEditedWirePoint(stWireRcd.nWireNum);

                if (iStatus == FOK)
                    nWireNum = nNextWireNum;
                else
                    break;
            }
        }
    }

    iStatus = GetNextAutoPadWire(0, SECOND_BOND, &nWireNum);
    if (iStatus == FOK)
    {
        while (nWireNum <= nTotalWire)
        {
            theWire.GetRcd(&stWireRcd, nWireNum);
            theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

            stCurTchPos = st2ndBondPt;

            if (stWireRcd.chPadType[1] > 0)
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
                    dMoveDist = Calc2PtDist(stCurTblPos, stCurTchPos) * theServo.GetResolXY();

                    stCurTblPos = stCurTchPos;
                }

                theAutoPadMgr.GetRcd(stWireRcd.chPadType[1] - 1, &stPadRcd);

                theVisionSys.UpdateLightingParameter(stPadRcd.stPrLightPara.nCoaxRedLight,
                    stPadRcd.stPrLightPara.bSideRedLight,
                    stPadRcd.stPrLightPara.nCoaxBlueLight,
                    stPadRcd.stPrLightPara.bSideBlueLight);

                theServo.WaitTableStop();

                Sleep(pstPgm->nInteliPadLocateDelay);

                theVisionSys.EnableVideo(false);
                theVisionSys.GrabImage();

                iStatus = GetNextAutoPadWire(nWireNum, SECOND_BOND, &nNextWireNum);
                if (iStatus == FOK)
                {
                    theWire.GetRcd(&stNextWireRcd, nNextWireNum);
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
                    dMoveDist = Calc2PtDist(stCurTblPos, stNextTchPos) * theServo.GetResolXY();
                    stCurTblPos = stNextTchPos;
                }

                stSrchRangeEnc.dx = stPadRcd.nPadWidth * 1.5;
                stSrchRangeEnc.dy = stPadRcd.nPadHeight * 1.5;

                theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stSrchRangeEnc, &stSrchRangePr);

                lVisionError = theVisionSys.SearchVisionPattern(
									(short)stPadRcd.ulPrPatternID,
									round(stSrchRangePr.dx),
									round(stSrchRangePr.dy),
									pstPgm->nPrPassScore,
									&stSrchResult);

                if (lVisionError == FOK)
                {
                    theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stSrchRangeEnc);

                    stCurTchPos.dx += stSrchRangeEnc.dx;
                    stCurTchPos.dy += stSrchRangeEnc.dy;

                    theMatrix.ReverseTransformPos(stWireRcd.nRefNum[1], &stCurTchPos);
                    stWireRcd.stTchPos[1] = stCurTchPos;
                }
                else
                {
                    theWire.CalcBondPoint(nWireNum, &st1stBondPt, &st2ndBondPt);

                    stCurTchPos = st2ndBondPt;

                    lMoveDist = round(fabs(stCurTblPos.dx - stCurTchPos.dx) * theServo.GetResolXY()); //@82
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

                    theServo.FastMoveTable(&stCurTchPos, MOTION_ABSOLUTE, MOTION_WITHOUT_WAIT); //@82
                    dMoveDist = Calc2PtDist(stCurTblPos, stCurTchPos) * theServo.GetResolXY();
                    stCurTblPos = stCurTchPos;

                    sList.RemoveAll();
                    sList.AddTail(theMsg.GetMsg(MSG_PAD_LOCATE_FAIL));
                    sList.AddTail(theMsg.GetMsg(MSG_MANUAL_LOCATE_PAD)); 

                    theVisionSys.EnableVideo(true);
                    theScanKeyThread.EnableJoyStick();
                    lStopSelected = theMsg.ShowMsgS(HELP_MSG, sList, true, true);
                    theScanKeyThread.DisableJoyStick();

                    if (lStopSelected == SHOW_MSG_RESULT_OK)
                    {
                        if (!theMcConfig.IsBypassAll())
                        {
                            theServo.GetTablePosition(&stCurTchPos);
                            theMatrix.ReverseTransformPos(stWireRcd.nRefNum[1], &stCurTchPos);
                            stWireRcd.stTchPos[1] = stCurTchPos;
                        }
                    }
                    else if (lStopSelected == SHOW_MSG_RESULT_STOP)
                    {
                        break;
                    }
                }

                theWire.SetRcd(&stWireRcd, nWireNum);
                theWire.UpdateEditedWirePoint(stWireRcd.nWireNum);

                if (iStatus == FOK)
                    nWireNum = nNextWireNum;
                else
                    break;
            }
        }
    }

    theServo.SetServoControlParameter(true);
    theScanKeyThread.EnableJoyStick();

    theVisionSys.EnableVideo(true);

    return 0;
}
