//ServoETEL.h v1.0.T5

#pragma once

#include "DataStruct.h"
#include "Motion.h"
#include "ServoController.h"

class CServoETEL
{
public:
	CServoETEL(void);
	~CServoETEL(void);
public:	//functions
	int GetInput(short bit);
	void TrigEFO();
	void VerifyControlData();
	void VerifySpeedData();
	void JogSpeedUp(void);			//set double jog speed
	void JogSpeedDown(void);		//set half jog speed
	void JogSpeedSlowest(void);		//set slowest jog speed
	int GetTableMechLimit(double *pdPosvLmtX, double *pdNegvLmtX,
							double *pdPosvLmtY, double *pdNegvLmtY);
	int SetTableKillDec(void);		//set motion kill deceleration of table
	void ClearAllBuffer(void);
	void PowerOnServo();
	int ServoPreparePowerOn(void);
	int InitWireClamp(void);
	int GetBondHeadPosition(double *pdPos);	//read bond head command position
	double GetResolXY(void);
	double GetResolZ(void);
	void LoadControlParameterFromFile(void);
	void LoadSpeedParameterFromFile(void);
	int UpdateWireClampProfile(short nDebugMode, bool bDryRun = false);
	void SaveAcsMemoryParamToFile(void);
	int AutoTuneControlParaThread(UINT uiAxis);
	int GetAutoTuneVersion(unsigned short *usVerMajor, unsigned short *usVerMinor);
	int ForceCaliSrchCtact(short nJogSpeed);
	int ReadForceSensor();
	void EnableForceSensor(bool bEnable);
	int SearchTableLimit(short nMotor, short nDir);
	int DisableTableLimit(void);
	int EnableTableLimit(void);
	int SetPosCompFactor(double dPosCompKp, double dPosCompOs);		//set position compensation factor to servo
	int PositionCompen(long lCtactLvl, double *pdForceFactorKa, double *pdForceFactorOs);
	void GetControlProfile(short nMotor, short nProfileNum, SERVO_CONTROL_PROFILE *pstControlProfile);
	void SetControlProfile(short nMotor, short nProfileNum, SERVO_CONTROL_PROFILE *pstControlProfile);
	int RecoverServo(void);
	int SearchBondHeadHomeFromLimit();		//search from upper limit position
	int DisableMotor(short nMotor);
	int EnableMotor(short nMotor);
	CString GetVersion(void);
	int SearchHome(short nMotor);
	int SearchLimit(short nMotor, short nDir = MOTION_POSITIVE);
	int GetServoScopeData(double *pdWaveformData);
	int GetSpScopeData(int* piSpWave);
	int GetWireClampProfile(short nProfileIndex, ST_WCL_PROFILE *pstWclProfile);
	int SetWireClampProfile(short nProfileIndex, ST_WCL_PROFILE *pstWclProfile);
	int SetWireClampIdleCurrent(double dCurrent);
	void SaveControlParameter(void);
	void LoadControlParameter(int nFileLength);
	void SaveWireClampProfile(void);
	void LoadWireClampProfile(void);
	void SaveMotionParameter(void);
	void LoadMotionParameter(void);
	int ApplyForceControl(short nForce, short nTime, bool bPgmValid, REF_RECORD *pstRef);
	int GetBondHeadEncPos(double *pdPos);
	int PerformTableScrub(short nAmplitude, short nYDelay, short nCycle);
	int WriteBondHeadDcom(double dDcom);
	int ReadBondHeadMFlags(int *pnMFlags);
	int WriteBondHeadMFlags(int nMFlags);
	int GetPositionError(short nMotor, double *pdPosErr);
	void DownloadBufferProgram(void);
	int GetBondProgress(int *pnBondProgress);
	int ReadMemoryInteger(EN_MEMORY_VAR nMemVar, int *pnValue);
	int WriteMemoryInteger(EN_MEMORY_VAR nMemVar, int nValue);
	int ReadMemoryReal(EN_MEMORY_VAR nMemVar, double *pdValue);
	int TriggerBuffer(short nBuffer);
	int GetTableCtrlBlock(TBL_POS stDesPos, short* pnCtrlBlockX, short* pnCtrlBlockY);
	int FastMoveTable2(TBL_POS stPos, bool bAbsolute = true, bool bWait = false);
	int StartServoTimerDebug(short nDebugMode);
	int StopServoTimerDebug(short nDebugMode);
	double GetServoSampleRate();
	int GetSpeedProfile(short nMotor, short nProfileNum, double *pdJerk, double *pdMaxAcc, double *pdMaxVel, short nSampleRateType = NORMAL_SAMPLE_RATE);
	void GetSpeedProfile(short nMotor, short nProfileNum, SERVO_SPEED_PROFILE *pstSpeedProfile);
	void SetSpeedProfile(short nMotor, short nProfileNum, SERVO_SPEED_PROFILE *pstSpeedProfile);
	int EndJogX(void);
	int EndJogY(void);
	int EndJogZ(void);
	int TriggerWireClampProfile(short nWclProfile);
	int SetOutputIo(short nPort, short nBit, short nValue);
	void MoveBondHeadToSafePos(void);
	int StopBuffer(short nBuffer);
	int SendServoCtrlProfileToServo(void);
	int DownloadBufferToServo(short nBuffer, char* pcProgram, short nSize);
	int ClearBuffer(short nBuffer);
	int FastMoveTable(TBL_POS *pstPos, short nMotionMode, short nWait);
	int SetServoSpeedProfile(short nMotor, short nProfileNum, bool bSlowSpeed = false);
	int SetServoControlParameter(bool bIdle = true);
	int Init();
private:	//variables
	ST_SOFT_POS_LMT m_stPosLmt;
	bool m_bJoggingX;
	bool m_bJoggingY;
	bool m_bJoggingZ;
	int m_nJogSpeed;	//table Jog speed encoder/second
	ST_WCL_PROFILE m_astWclProfile[WCL_MAX_PROFILE];
	SERVO_CONTROL_PARAMETER m_stControlParam;
	SERVO_MOTION_PARAMETER m_stMotionParam;
	short m_nMsgType;
	short m_nMsgNum;
	short m_nMsgIndex;
	bool m_bStopValid;
	bool m_bSkipValid;
	CStringList m_sList;
	HANDLE m_hMemCmd, m_hMemData, m_hMemBondProgress;
	void *m_pMemCmd, *m_pMemData, *m_pMemBondProgress;
	HANDLE m_hMemRestartBond, m_hMemStartBond0, m_hMemStartBond1;
	void *m_pMemRestartBond, *m_pMemStartBond0, *m_pMemStartBond1;
	HANDLE m_hMemBuf0Ctact1, m_hMemBuf0Ctact2;
	HANDLE m_hMemBuf1Ctact1, m_hMemBuf1Ctact2;
	void *m_pMemBuf0Ctact1, *m_pMemBuf0Ctact2;
	void *m_pMemBuf1Ctact1, *m_pMemBuf1Ctact2;
	HANDLE m_hMemServoIntData0, m_hMemServoIntData1;
	void *m_pMemServoIntData0, *m_pMemServoIntData1;
	HANDLE m_hMemSoftPosLmt;
	void *m_pMemSoftPosLmt;
	HANDLE m_hMemScopeData;	//v1.0.T5 add
	void *m_pMemScopeData;	//v1.0.T5 add
	int m_anCmd[10];
	int m_nCmdSize;
private:	//functions
	void InitOutput(void);
	bool IsMotorXYZ(short nMotor);
	int MoveAxisX(long lDist, short nMotionMode, short nWait);
	int MoveAxisY(long lDist, short nMotionMode, short nWait);
	void SetVel(short nMotor, double dVel_ms);
	void SetAcc(short nMotor, double dAcc_ms2);
	void SetJerk(short nMotor, int nTime_ms);
	int GetMotorAxis(short nMotor);
	int SendCommand(int nCmd, bool bWaitAck = true, int nTimeout = 3000);
	int WaitCmdAck(int nTimeout);
	int WaitCmdAck(double *pdWaitTime_x1ms, int nTimeout);
protected:
	int TriggerRestartWire(bool bPara);
	int SendIntDataToServo(int *pnData, short nDataLen, short nBuffer);
	int SendRealDataToServo(double *pdData, short nDataLen, short nBuffer, short nServoVar);
	int SyncMotion(short nMotor);
    int CheckServoError(short nCheckBufMode);
	int DisableServoController(void);
	int RunBuffer(short nBuffer);
    int ResetBondProgress(void);
    int GetTablePosition(TBL_POS *pstPos);	//v1.0.T2 edit
	int StartJogX(short nJogDirection);
	int StartJogY(short nJogDirection);
	int StartJogZ(short nJogDirection, short nJogSpeed);
	int MoveAxisZ(long lDist, short nMotionMode, short nWait);
	int MoveTable2(TBL_POS stDist, bool bAbsolute = true,
					bool bWait = true, bool bSlowSpeed = true);
};
