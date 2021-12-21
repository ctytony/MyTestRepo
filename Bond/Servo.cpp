#include "StdAfx.h"
#include "Servo.h"
#include "MACDEF.h"
#include "MsgManager.h"
#include "Debug.h"
#include "..\Materialhandler\TopIndex.h"
#include "LogTrace.h"		//v1.5.T144 add
#include "..\Include\MSGDEF.H"

CServo theServo;

CServo::CServo(void)
{
}

CServo::~CServo(void)
{
}

int CServo::MoveTable(TBL_POS *pstTblPos, short nMotionMode, short nWait, bool bSlowSpeed)
{
	if (theMcConfig.IsBypassServo()) return FOK;
	if (theTopTrack.IsClampReady() == FALSE) return FNOK;	//V1.5.W17 add

	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::MoveTable2(*pstTblPos,
				nMotionMode == MOTION_ABSOLUTE ? true : false,
				nWait == MOTION_WITH_WAIT ? true : false, bSlowSpeed);
#else
	nResult = CServoController::MoveTable(pstTblPos, nMotionMode, nWait, bSlowSpeed);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.MoveTable err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_nMsgNum = 0;
		m_nMsgIndex = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		CServoController::GetErrorMessage(m_nMsgType, m_nMsgNum, &m_nMsgIndex, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_nMsgNum, &m_nMsgIndex);
	}
#endif
	return (nResult);
}

//v3.1.T24 add
int CServo::MoveTable2(TBL_POS stPos, bool bAbsolute, bool bWait, bool bSlowSpeed)
{
	if (theMcConfig.IsBypassServo()) return FOK;
	if (theTopTrack.IsClampReady() == FALSE) return FNOK;

	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::MoveTable2(stPos, bAbsolute, bWait, bSlowSpeed);
#else
	nResult = CServoController::MoveTable2(stPos, bAbsolute, bWait, bSlowSpeed);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.MoveTable2 err"), true);

		m_nMsgType = 0;
		m_nMsgNum = 0;
		m_nMsgIndex = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		CServoController::GetErrorMessage(m_nMsgType, m_nMsgNum, &m_nMsgIndex, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_nMsgNum, &m_nMsgIndex);
	}
#endif
	return (nResult);
}

int CServo::MoveAxisZ(long lDistance, short nMotionMode, short nWait)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::MoveAxisZ(lDistance, nMotionMode, nWait);
#else
	nResult = CServoController::MoveAxisZ(lDistance, nMotionMode, nWait);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.MoveAxisZ err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll(); 
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)	//V1.1.W277 add
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::StartJogX(short nJogDirection)
{
	if (theTopTrack.IsClampReady() == FALSE) return FNOK;	//V1.5.W17 add

	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::StartJogX(nJogDirection);
#else
	nResult = CServoController::StartJogX(nJogDirection);
	if (nResult != FOK)
	{   
		theLog.WriteLine(_T("CServo.StartJogX err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::StartJogY(short nJogDirection)
{
	if (theTopTrack.IsClampReady() == FALSE) return FNOK;	//V1.5.W17 add

	int nResult = FOK;
#if _SERVO_TYPE == 2
	nResult = CServoETEL::StartJogY(nJogDirection);
#else
	nResult = CServoController::StartJogY(nJogDirection);
	if (nResult != FOK)
	{   
		theLog.WriteLine(_T("CServo.StartJogY err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::StartJogZ(short nJogDirection, short nJogSpeed)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::StartJogZ(nJogDirection, nJogSpeed);
#else
	nResult = CServoController::StartJogZ(nJogDirection, nJogSpeed);
	if (nResult != FOK)
	{   
		theLog.WriteLine(_T("CServo.StartJogZ err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::GetTablePosition(TBL_POS *pstTblPos, byte nWait)
{
	if (theMcConfig.IsBypassServo()) return FOK;

	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::GetTablePosition(pstTblPos);	//v3.1.T478 edit
#else
	nResult = CServoController::GetTablePosition(pstTblPos, nWait);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.GetTablePosition err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_nMsgNum = 0;
		m_nMsgIndex = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		CServoController::GetErrorMessage(m_nMsgType, m_nMsgNum, &m_nMsgIndex, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_nMsgNum, &m_nMsgIndex);
	}
#endif
	return (nResult);
}

int CServo::ResetBondProgress(void)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::ResetBondProgress();
#else
	nResult = CServoController::ResetBondProgress();
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.ResetBondProgress err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::RunBuffer(short nBuffer)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::RunBuffer(nBuffer);
#else
	nResult = CServoController::RunBuffer(nBuffer);
	if (nResult != FOK)
	{
		CString str;
		str.Format(_T("CServo.RunBuffer_%d_Err_%d"), nBuffer, nResult);	//v3.1.T461 add
		theLog.WriteLine(str, true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)	//V1.1.W254 add
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::DisableServoController(void)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::DisableServoController();
#else
	nResult = CServoController::DisableServoController();
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.DisableServoController err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_nMsgNum = 0;
		m_nMsgIndex = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		CServoController::GetErrorMessage(m_nMsgType, m_nMsgNum, &m_nMsgIndex, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_nMsgNum, &m_nMsgIndex);
	}
#endif
	return (nResult);
}

int CServo::CheckServoError(short nCheckBufMode)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::CheckServoError(nCheckBufMode);
#else
	nResult = CServoController::CheckServoError(nCheckBufMode);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.CheckServoError err\r\n"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::SyncMotor(short nMotor)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::SyncMotion(nMotor);
#else
	nResult = CServoController::SyncMotion(nMotor);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.SyncMotor err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_nMsgNum = 0;
		m_nMsgIndex = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		CServoController::GetErrorMessage(m_nMsgType, m_nMsgNum, &m_nMsgIndex, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_nMsgNum, &m_nMsgIndex);
	}
#endif
	return (nResult);
}

int CServo::SendRealDataToServo(double *pdData, short nDataLength, short nBuffer, short nServoVar)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::SendRealDataToServo(pdData, nDataLength, nBuffer, nServoVar);
#else
	nResult = CServoController::SendRealDataToServo(pdData, nDataLength, nBuffer, nServoVar);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.SendRealDataToServo err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

int CServo::SendIntDataToServo(int *pnData, short nDataLength, short nBuffer)
{
	int nResult = FOK;
#if _SERVO_TYPE == 2	//v3.1.T80 add
	nResult = CServoETEL::SendIntDataToServo(pnData, nDataLength, nBuffer);
#else
	nResult = CServoController::SendIntDataToServo(pnData, nDataLength, nBuffer);
	if (nResult != FOK)
	{
		theLog.WriteLine(_T("CServo.SendIntDataToServo err"), true);	//v1.5.T144 edit

		m_nMsgType = 0;
		m_bStopValid = false;
		m_bSkipValid = false;
		m_sList.RemoveAll();
		CServoController::GetErrorMessage(m_nMsgType, m_sList, m_bStopValid, m_bSkipValid);
		if (m_sList.GetCount() > 0)
			theMsg.ShowMsg(m_nMsgType, m_sList);
	}
#endif
	return (nResult);
}

void CServo::WaitTableStop()
{
	SyncMotor(MOTOR_X);
	SyncMotor(MOTOR_Y);
}

//v3.1.T134 add
int CServo::TriggerRestartWire(bool bParam)
{
#if _SERVO_TYPE == 2
	return CServoETEL::TriggerRestartWire(bParam);
#else
	return CServoController::TriggerRestartWire(bParam);
#endif
}
