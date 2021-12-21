/***************************************************************
ServoTune.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MacDef.h"
#include "MathCalc.h"
#include "ServoTune.h"
#include "Bond.h"
#include "MachineData.h"
#include "MsgManager.h"
#include "Program.h"
#include "Reference.h"
#include "Wire.h"
#include "MachineConfig.h"
#include "WireBond.h"
#include "servo.h"
#include "Debug.h"

CServoTune theServoTune;

// CServoTune

CServoTune::CServoTune()
: m_bStopTuning(false) //@3
{
}

CServoTune::~CServoTune()
{
}

// Verify bond head performance

int CServoTune::VerifyBondHeadPerformance(short nDebugMode)
{
    REF_RECORD    stRefRcd;
    WIRE_RECORD   stWireRcd;
    TBL_POS       stCurPos;
    CString str;
    int iStatus = FOK;
    long lCtactSrchHt;

    if (thePgm.IsProgramValid() == false)
        return FNOK;

    if (thePgm.GetTotalWire() < 1)
        return FNOK;
    
    theBond.ResetBondData();

    theServo.SetServoControlParameter();
    theServo.SetServoSpeedProfile(MOTOR_Z, 0);

    theBond.CtactSrch(3, 46, &lCtactSrchHt);

    theWire.GetRcd(&stWireRcd, 1);

    theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[0]);
    stRefRcd.bCtactLvlValid = true;
    stRefRcd.lCtactLvl = lCtactSrchHt;
    theRef.SetRcd(&stRefRcd, stWireRcd.nRefNum[0]);

    theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[1]);
    stRefRcd.bCtactLvlValid = true;
    stRefRcd.lCtactLvl = lCtactSrchHt;
    theRef.SetRcd(&stRefRcd, stWireRcd.nRefNum[1]);

    theServo.ClearBuffer(5);
    Sleep(10);

    LPSTR lpszBuffer=new char[20000];

    lstrcpyA(lpszBuffer, "global REAL arBHSpeed(5)(3); global REAL arBHCtrl(20)(4); global REAL arXCtrl(10)(4); global REAL arYCtrl(4)(4); global REAL rWaveFormData(8)(2000); global INT iSpData(25000); REAL PAR_ADDRESS; REAL rSpeed; int lAddrDetectSw; int lAddrForceOffSw; int lAddrPreDrvCmd; int lAddrCtactDrvCmd; int lAddrLevelPE; int gnDectect;\n"); //@4
    lstrcatA(lpszBuffer, "REAL TrajZPoints(1)(500); int IntData(146); REAL arNeckDistPoints(1)(500); global int dpr 0x9C, lRestartBond; \n"); //@5
    lstrcatA(lpszBuffer, "lRestartBond = 0;\n");
    lstrcatA(lpszBuffer, "lAddrDetectSw = GETSPA(0, \"DetectSW\"); lAddrForceOffSw = GETSPA(0, \"ForceOffSw\"); lAddrPreDrvCmd = GETSPA(0, \"PreDrvCmd\"); lAddrCtactDrvCmd = GETSPA(0, \"ContactDrvCmd\"); lAddrLevelPE = GETSPA(0, \"LevelPE\"); PAR_ADDRESS = GETSPA(0,\"A_PE\");\n");
    lstrcatA(lpszBuffer, "TILL lRestartBond = 1;\n"); //@3
    lstrcatA(lpszBuffer, "while (lRestartBond = 1)\n");
    lstrcatA(lpszBuffer, "   SETSPV(SP0:DampGA, IntData(84)); gnDectect = IntData(85);\n");
    lstrcatA(lpszBuffer, "   if (IntData(33) = 1) DC rWaveFormData, 1000, 0.5, RPOS(0), FPOS(0), RPOS(1), FPOS(1), RPOS(4), FPOS(4), DCOM(5), MST(4); end; if (IntData(33) = 2) SPDC iSpData, 25000, 0.05, 0, PAR_ADDRESS, 0; end;\n");
    lstrcatA(lpszBuffer, "   A_SLVKP = arBHCtrl(2)(0); A_SLVKI = arBHCtrl(2)(1); A_SLPKP = arBHCtrl(2)(2); A_SLAFF = arBHCtrl(2)(3); A_ACC = arBHSpeed(0)(1); A_DEC = arBHSpeed(0)(1); A_JERK = arBHSpeed(0)(2); OUT0.6=0; PTP/f A, IntData(3), abs(IntData(4));\n");
    lstrcatA(lpszBuffer, "   OUT0.6=1; OUT0.5=1; OUT0.4=1; OUT0.7=1; TILL ^AST(4).#MOVE; A_SLVKP = arBHCtrl(1)(0); A_SLVKI = arBHCtrl(1)(1); A_SLPKP = arBHCtrl(1)(2); A_SLAFF = arBHCtrl(1)(3); JOG/v A, IntData(4); OUT0.6=0;\n");
    lstrcatA(lpszBuffer, "   BLOCK\n");
    lstrcatA(lpszBuffer, "   SETSP(0, lAddrLevelPE, IntData(5)); SETSP(0, lAddrPreDrvCmd, IntData(48) * 327.67 / 1000); if(IntData(98) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(48) * 327.67 / 1000); else SETSP(0, lAddrCtactDrvCmd, IntData(6) * 327.67 / 1000); end; wait IntData(45); SETSP(0, lAddrDetectSw, gnDectect); TILL MST4.#OPEN; KILL A; if(IntData(98) > 0) wait IntData(98); end;\n");
    lstrcatA(lpszBuffer, "   OUT0.6=1; OUT0.7=0; if (IntData(7) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(6) * 327.67 / 1000); wait IntData(7); end; if (IntData(9) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(8) * 327.67 / 1000); wait IntData(9); end; if (IntData(11) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(10) * 327.67 / 1000); wait IntData(11); end; SETSP(0, lAddrForceOffSw, 1);\n");
    lstrcatA(lpszBuffer, "   OUT0.5=1;OUT0.4=0;OUT0.7=0; A_SLVKP = arBHCtrl(3)(0); A_SLVKI = arBHCtrl(3)(1); A_SLPKP = arBHCtrl(3)(2); A_SLAFF = arBHCtrl(3)(3); A_ACC = arBHSpeed(1)(1); A_DEC = arBHSpeed(1)(1); A_JERK = arBHSpeed(1)(2); TILL ^MST4.#OPEN; KILL A; \n");
    lstrcatA(lpszBuffer, "   PTP/r A, IntData(12); wait 1;\n");
    lstrcatA(lpszBuffer, "   END\n");
    lstrcatA(lpszBuffer, "   TILL ^AST(4).#MOVE; OUT0.6=0; PATH/r A, 0.25; mpoint A, arNeckDistPoints, IntData(81); ends A;\n"); //@1
    lstrcatA(lpszBuffer, "   A_SLVKP = arBHCtrl(4)(0); A_SLVKI = arBHCtrl(4)(1); A_SLPKP = arBHCtrl(4)(2); A_SLAFF = arBHCtrl(4)(3); A_ACC = arBHSpeed(2)(1); A_DEC = arBHSpeed(2)(1); A_JERK = arBHSpeed(2)(2); TILL ^AST(4).#MOVE; PTP/r A, IntData(21); wait 1;\n"); //@1
    lstrcatA(lpszBuffer, "   TILL ^AST(4).#MOVE; A_SLVKP = arBHCtrl(5)(0); A_SLVKI = arBHCtrl(5)(1); A_SLPKP = arBHCtrl(5)(2); A_SLAFF = arBHCtrl(5)(3);\n");
    lstrcatA(lpszBuffer, "   OUT0.6=1;OUT0.5=1;OUT0.4=0;OUT0.7=1; PATH/r A, 0.25; mpoint A, TrajZPoints, IntData(30); ends A;\n");
    lstrcatA(lpszBuffer, "   TILL ^AST(4).#MOVE; A_SLVKP = arBHCtrl(8)(0); A_SLVKI = arBHCtrl(8)(1); A_SLPKP = arBHCtrl(8)(2); A_SLAFF = arBHCtrl(8)(3); JOG/v A, IntData(22); OUT0.6=0;\n");
    lstrcatA(lpszBuffer, "   BLOCK\n");
    lstrcatA(lpszBuffer, "   SETSPV(SP0:DampGA, IntData(94)); SETSP(0, lAddrLevelPE, IntData(23)); SETSP(0, lAddrPreDrvCmd, IntData(49) * 327.67 / 1000); if(IntData(99) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(49) * 327.67 / 1000); else SETSP(0, lAddrCtactDrvCmd, IntData(24) * 327.67 / 1000); end; wait IntData(46); SETSP(0, lAddrDetectSw, gnDectect); TILL MST4.#OPEN; KILL A; if(IntData(99) > 0) wait IntData(99); end;\n");
    lstrcatA(lpszBuffer, "   OUT0.7=0; OUT0.6=1; if (IntData(25) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(24) * 327.67 / 1000); wait IntData(25); end;\n");
    lstrcatA(lpszBuffer, "   if (IntData(27) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(26) * 327.67 / 1000); wait IntData(27); end; if (IntData(29) > 0) SETSP(0, lAddrCtactDrvCmd, IntData(28) * 327.67 / 1000); wait IntData(29); end; SETSP(0, lAddrForceOffSw, 1); OUT0.5=1;OUT0.4=1;OUT0.7=0;\n");
    lstrcatA(lpszBuffer, "   A_SLVKP = arBHCtrl(6)(0); A_SLVKI = arBHCtrl(6)(1); A_SLPKP = arBHCtrl(6)(2); A_SLAFF = arBHCtrl(6)(3); A_ACC = arBHSpeed(3)(1); A_DEC = arBHSpeed(3)(1); A_JERK = arBHSpeed(3)(2); TILL ^MST4.#OPEN; KILL A;\n");
    lstrcatA(lpszBuffer, "   PTP/r A, IntData(31);\n");
    lstrcatA(lpszBuffer, "   END\n");
    lstrcatA(lpszBuffer, "   TILL ^AST(4).#MOVE; OUT0.6=0; TILL abs(A_PE) < 1000;\n");
    lstrcatA(lpszBuffer, "   A_SLVKP = arBHCtrl(7)(0); A_SLVKI = arBHCtrl(7)(1); A_SLPKP = arBHCtrl(7)(2); A_SLAFF = arBHCtrl(7)(3); A_ACC = arBHSpeed(4)(1); A_DEC = arBHSpeed(4)(1); A_JERK = arBHSpeed(4)(2); PTP A, IntData(32);\n");
    lstrcatA(lpszBuffer, "   TILL ^AST(4).#MOVE; OUT0.5=0;OUT0.4=0;OUT0.7=0;OUT0.6=1; A_SLVKP = arBHCtrl(0)(0); A_SLVKI = arBHCtrl(0)(1); A_SLPKP = arBHCtrl(0)(2); A_SLAFF = arBHCtrl(0)(3); TILL abs(A_PE) < 1000;\n wait 20;");
    lstrcatA(lpszBuffer, "end;\n");
    lstrcatA(lpszBuffer, "STOP;\n");
    lstrcatA(lpszBuffer, "ON MST4.#OPEN; kill A; RET\n");

    iStatus = theServo.DownloadBufferToServo(5, lpszBuffer, strlen(lpszBuffer));
    if (iStatus != FOK)
        iStatus = theServo.DownloadBufferToServo(5, lpszBuffer, strlen(lpszBuffer));

	delete[] lpszBuffer;

    Sleep(100);

	theServo.RunBuffer(5);
    Sleep(10);

    theBond.SendBondHeadSpeedProfileToServo();
    Sleep(30);

    theServo.SendServoCtrlProfileToServo();
    Sleep(30);

	theBond.InitUsgProfile();

    ST_WIRE_INFO* lpstInfoRcd=new ST_WIRE_INFO;
	LPINT lpIntData=new int[MAX_INT_VARIABLE];

    theBond.CalcWireInfo(1, stCurPos, lpstInfoRcd, str, false);

	lpIntData[SRCH_LVL1] = lpstInfoRcd->nSrchLvl1;
	lpIntData[SRCH_LVL2] = lpstInfoRcd->nSrchLvl2;	//v3.1.T262 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
    lpIntData[SRCH_SPEED1] = lpstInfoRcd->nSrchSpeed1;
#else
	lpIntData[SRCH_SPEED1] = -lpstInfoRcd->nSrchSpeed1 * 2500;
#endif
    lpIntData[SRCH_TOL1] = lpstInfoRcd->nSrchTol1;

	double dForceFactor = 1000.0;	//v3.1.T293 add
#if _SERVO_TYPE == 2	//v3.1.T460 add
	dForceFactor = 1.0;
#endif
    lpIntData[PRE_IMPACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nPreCtactForce[0], lpstInfoRcd->nCtactLvl1) * dForceFactor);
    lpIntData[CTACT_FORCE1] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nCtactForce[0], lpstInfoRcd->nCtactLvl1) * dForceFactor);
    
    lpIntData[PRE_IMPACT_TIME1]   = lpstInfoRcd->nPreCtactTime[0];

    lpIntData[CTACT_TIME1]  = lpstInfoRcd->nCtactTime[0];

    lpIntData[BASE_FORCE1] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nBaseForce[0], lpstInfoRcd->nCtactLvl1) * dForceFactor);

    lpIntData[BASE_TIME1]   = lpstInfoRcd->nBaseTime[0];
    
    lpIntData[RLS_FORCE1] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nReleaseForce[0], lpstInfoRcd->nCtactLvl1) * dForceFactor);
    
    lpIntData[RLS_TIME1] = lpstInfoRcd->nReleaseTime[0];
    lpIntData[REV_HT] = lpstInfoRcd->nRevHt;
    lpIntData[REV_HT2] = 0;	//v3.1.T350 add
	lpIntData[REV_DIST2_X] = 0;	//v3.1.T575 add
	lpIntData[REV_DIST2_Y] = 0;	//v3.1.T575 add
    lpIntData[SLOPE_LENGTH] = lpstInfoRcd->nSlopeLen;
#if _SERVO_TYPE == 2	//v3.1.T460 add
    lpIntData[SRCH_SPEED2] = lpstInfoRcd->nSrchSpeed2;
#else
	lpIntData[SRCH_SPEED2] = -lpstInfoRcd->nSrchSpeed2 * 2500;	//5000;
#endif
    lpIntData[SRCH_TOL2] = lpstInfoRcd->nSrchTol2;

    lpIntData[PRE_IMPACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nPreCtactForce[1], lpstInfoRcd->nCtactLvl2) * dForceFactor);
    lpIntData[CTACT_FORCE2] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nCtactForce[1], lpstInfoRcd->nCtactLvl2) * dForceFactor);

    lpIntData[PRE_IMPACT_TIME2]   = lpstInfoRcd->nPreCtactTime[1];

    lpIntData[CTACT_TIME2]  = lpstInfoRcd->nCtactTime[1];

    lpIntData[BASE_FORCE2] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nBaseForce[1], lpstInfoRcd->nCtactLvl2) * dForceFactor);

    lpIntData[BASE_TIME2]   = lpstInfoRcd->nBaseTime[1];

    lpIntData[RLS_FORCE2] = round(theMcData.Calc3PtBondForceDac(-lpstInfoRcd->nReleaseForce[1], lpstInfoRcd->nCtactLvl2) * dForceFactor);

    lpIntData[RLS_TIME2]   = lpstInfoRcd->nReleaseTime[1];
    lpIntData[TAIL_HEIGHT] = lpstInfoRcd->nTailHt;
    lpIntData[RESET_LVL]   = lpstInfoRcd->nResetLvl;
    
    if (nDebugMode == DEBUG_SP_WAVEFORM)
        lpIntData[SCOPE_ENABLE] = 2;
    else
        lpIntData[SCOPE_ENABLE] = 1;

    lpIntData[SRCH_DELAY1] = lpstInfoRcd->nSrchDelay[0];
    lpIntData[SRCH_DELAY2] = lpstInfoRcd->nSrchDelay[1];

    lpIntData[REV_DIST_HT_POINT] = lpstInfoRcd->nRevDistHtPoint;
    lpIntData[PRE_KINK_DIST_HT_POINT] = lpstInfoRcd->nPreKinkDistHtPoint;	//v3.1.T326 add
    lpIntData[ARC_REV_DELAY] = lpstInfoRcd->nArcRevDelay;	//v3.1.T407 add
    lpIntData[ARC_PRE_KINK_DELAY] = lpstInfoRcd->nArcPreKinkDelay;	//v3.1.T328 add

    lpIntData[DAMPGAIN] = theMcData.GetRcd()->nDampGain;
    lpIntData[SEARCH_MODE] = theMcData.GetRcd()->nSearchMode;

    if ((lpstInfoRcd->nLoopTopPremove > 0) && (lpstInfoRcd->nWclLoopPeakClosePretrig < 0))
        lpIntData[TRAJ_POINT] = lpstInfoRcd->nTrajPoint - lpstInfoRcd->nLoopTopPremove * 4 -
                                  abs(lpstInfoRcd->nWclLoopPeakClosePretrig) * 4 - 9;
    else
        lpIntData[TRAJ_POINT] = lpstInfoRcd->nTrajPoint - lpstInfoRcd->nLoopTopPremove * 4 - 9;

    lpIntData[DAMPGAIN2] = theMcData.GetRcd()->nDampGain2;

    iStatus = theServo.SendIntDataToServo(lpIntData, MAX_INT_VARIABLE, 5);
	delete[] lpIntData;

    if (iStatus != FOK)
	{
		delete lpstInfoRcd;
		return iStatus;
	}
    
    Sleep(1);

    iStatus = theServo.SendRealDataToServo(lpstInfoRcd->adRevDistHtMotion,
				lpstInfoRcd->nRevDistHtPoint, 5, SERVO_VAR_REV_DIST_HT);
    if (iStatus != FOK)
    {
		delete lpstInfoRcd;
        return iStatus;
    }

    Sleep(1);

    double* lpdblBhTraj=new double[500];
    short i, nPretrig;

    nPretrig = lpstInfoRcd->nLoopTopPremove * 4 + 9;

    for (i = nPretrig; i < lpstInfoRcd->nTrajPoint; i++)
    {
        lpdblBhTraj[i - nPretrig] = lpstInfoRcd->adTrajPos[lpstInfoRcd->nTrajPoint * 2 + i];
    }

    iStatus = theServo.SendRealDataToServo(lpdblBhTraj,
				lpstInfoRcd->nTrajPoint - nPretrig, 5, SERVO_VAR_TRAJ_HT);
    if (iStatus != FOK)
    {
		delete lpstInfoRcd;
		delete lpdblBhTraj;
        return iStatus;
    }

	delete lpstInfoRcd;
	delete lpdblBhTraj;

    theServo.TriggerRestartWire(true);
    Sleep(20);

    do
    {
		Sleep_x1ms(10);

        if (theServo.CheckServoError(CHECK_BUFFER_MODE_NONE) != FOK) break;
    } while (m_bStopTuning == false);

    /*
    iStatus = acsc_WaitProgramEnd(m_hCommHandle, 5, 5000);
    if (iStatus == 0)
    {
        iStatus = acsc_GetProgramError(m_hCommHandle, 2, &lError, NULL);
        if (iStatus)
        {
            if (lError > 0)
                iStatus = FNOK;
            else
                iStatus = FOK;
        }
        else
            iStatus = FNOK;
    }
    */

    theServo.TriggerRestartWire(false);
    Sleep(500);
    
    theServo.StopBuffer(5);
    Sleep(10);

    theServo.ClearBuffer(5);
    Sleep(10);

    return 0;
}

// Verify XY table performance
int CServoTune::VerifyTablePerformance(void)
{
    REF_RECORD    stRefRcd;
    WIRE_RECORD   stWireRcd;
    TBL_POS       stCurPos;
    CString str;
    int iStatus = FOK;

    if (thePgm.IsProgramValid() == false)
        return FNOK;

    if (thePgm.GetTotalWire() < 1)
        return FNOK;

    theServo.SetServoControlParameter();
    theBond.SendTableSpeedProfileToServo();

    theWire.GetRcd(&stWireRcd, 1);

    theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[0]);
    stRefRcd.bCtactLvlValid = true;
    theRef.SetRcd(&stRefRcd, stWireRcd.nRefNum[0]);

    theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[1]);
    stRefRcd.bCtactLvlValid = true;
    theRef.SetRcd(&stRefRcd, stWireRcd.nRefNum[1]);
    
    theServo.GetTablePosition(&stCurPos);

    ST_WIRE_INFO* lpstInfoRcd = new ST_WIRE_INFO;
    theBond.CalcWireInfo(1, stCurPos, lpstInfoRcd, str, false);
    stCurPos.dx = lpstInfoRcd->stBondShiftPos1.dx - theMcData.GetRcd()->stBondTipOs.dx;
    stCurPos.dy = lpstInfoRcd->stBondShiftPos1.dy - theMcData.GetRcd()->stBondTipOs.dy;
	theServo.MoveTable2(stCurPos);	//v3.1.T209 edit
    Sleep(60);

    theServo.ClearBuffer(5);
    Sleep(10);

    LPSTR lpszBuffer = new char[20000];
    lstrcpyA(lpszBuffer, "global REAL arXSpeed(6)(3); global REAL arYSpeed(6)(3); global REAL arXCtrl(10)(4); global REAL arYCtrl(4)(4); global REAL rWaveFormData(8)(2000);\n"); //@4
    lstrcatA(lpszBuffer, "int IntData(146); global int dpr 0x9C, lRestartBond; \n"); //@5
    lstrcatA(lpszBuffer, "lRestartBond = 0;\n");
    lstrcatA(lpszBuffer, "TILL lRestartBond = 1; Y_ACC  = arXSpeed(1)(1); X_ACC = arYSpeed(0)(1); Y_DEC  = arXSpeed(1)(1); X_DEC = arYSpeed(0)(1); Y_JERK = arXSpeed(1)(2); X_JERK = arYSpeed(0)(2);\n");
    lstrcatA(lpszBuffer, "DC rWaveFormData, 1000, 0.5, RPOS(1), FPOS(1), RPOS(0), FPOS(0), RPOS(4), FPOS(4), DCOM(5), MST(4);\n");
    lstrcatA(lpszBuffer, "Y_SLVKP = arXCtrl(0)(0); Y_SLVKI = arXCtrl(0)(1); Y_SLPKP = arXCtrl(0)(2); Y_SLAFF = arXCtrl(0)(3); X_SLVKP = arYCtrl(0)(0); X_SLVKI = arYCtrl(0)(1); X_SLPKP = arYCtrl(0)(2); X_SLAFF = arYCtrl(0)(3);\n");
    lstrcatA(lpszBuffer, "PTP/m YX, IntData(0), IntData(1); wait 5;\n");
    lstrcatA(lpszBuffer, "TILL ^AST(1).#MOVE; wait 25\n");
    lstrcatA(lpszBuffer, "PTP/mr YX, IntData(2), IntData(3); wait 3;\n");
    lstrcatA(lpszBuffer, "TILL ^AST(1).#MOVE; PTP/m YX, IntData(4), IntData(5); wait 3;\n");
    lstrcatA(lpszBuffer, "TILL ^AST(1).#MOVE;\n");
    lstrcatA(lpszBuffer, "STOP\n");

    iStatus = theServo.DownloadBufferToServo(5, lpszBuffer, strlen(lpszBuffer));
    if (iStatus != FOK)
        iStatus = theServo.DownloadBufferToServo(5, lpszBuffer, strlen(lpszBuffer));

	delete[] lpszBuffer;

    Sleep(100);

	theServo.RunBuffer(5);
    Sleep(10);

    theBond.SendBondHeadSpeedProfileToServo();
    Sleep(30);

    theServo.SendServoCtrlProfileToServo();
    Sleep(30);

	LPINT lpIntData=new int[MAX_INT_VARIABLE];

    lpIntData[0] = round(lpstInfoRcd->stBondShiftPos1.dx);
    lpIntData[1] = round(lpstInfoRcd->stBondShiftPos1.dy);
    lpIntData[2] = round(lpstInfoRcd->stRevPos.dx - lpstInfoRcd->stBondPos[0].dx);
    lpIntData[3] = round(lpstInfoRcd->stRevPos.dy - lpstInfoRcd->stBondPos[0].dy);
    lpIntData[4] = round(lpstInfoRcd->stTrajEndPos.dx);
    lpIntData[5] = round(lpstInfoRcd->stTrajEndPos.dy);

	delete lpstInfoRcd;

    iStatus = theServo.SendIntDataToServo(lpIntData, 10, 5);
	delete[] lpIntData;

    if (iStatus != FOK) return iStatus;
    Sleep(10);

    theServo.TriggerRestartWire(true);
    Sleep(1000);

    theServo.StopBuffer(5);
    Sleep(10);

    theServo.ClearBuffer(5);
    Sleep(10);

    return 0;
}

// Inform stop tuning
int CServoTune::InformStopTuning(bool bStopTune) //@3
{
    m_bStopTuning = bStopTune;

    return 0;
}
