//ServoController.h v1.1.T136sc

#pragma once

#include "DataStruct.h"
#include "Reference.h"
#include "Motion.h"

#define SYS_TBL_X_POSITIVE_LIMIT	180000
#define SYS_TBL_X_NEGTIVE_LIMIT		-180000
#define SYS_TBL_Y_POSITIVE_LIMIT	80000
#define SYS_TBL_Y_NEGTIVE_LIMIT		-80000

enum SPEED_PROFILE
{
    SPEED_PROFILE_0,		//High level application 
    SPEED_PROFILE_1,		//Search height 
    SPEED_PROFILE_2,		//Looping
    SPEED_PROFILE_3,		//Trajectory
    SPEED_PROFILE_4,		//Tail
    SPEED_PROFILE_5,		//Fire level
};

enum BOND_HEAD_CTRL_BLK
{
    BH_CTRL_IDLE,			//0
    BH_CTRL_1ST_CONTACT,	//1
    BH_CTRL_1ST_SRCH_HT,	//2
    BH_CTRL_RH,				//3
    BH_CTRL_LOOP_TOP,       //4
    BH_CTRL_TRAJ,           //5
    BH_CTRL_TAIL,           //6
    BH_CTRL_RESET,          //7
    BH_CTRL_2ND_CONTACT,    //8
    BH_CTRL_RD,             //9
    BH_CTRL_RH2,            //10
    BH_CTRL_RD2,            //11
    BH_CTRL_RH3,			//12
    BH_CTRL_RD3,			//13
    BH_CTRL_RH4,			//14
    BH_CTRL_RTWS,			//15
    BH_CTRL_FTWS,			//16
};

#define SERVO_SCOPE_DATA_LENGTH			1000
#define SERVO_SP_SCOPE_DATA_LENGTH		25000

enum SERVO_SCOPE_DATA
{
    SCOPE_REF_POS_X,
    SCOPE_FB_POS_X,
    SCOPE_REF_POS_Y,
    SCOPE_FB_POS_Y,
    SCOPE_REF_POS_Z,
    SCOPE_FB_POS_Z,
    SCOPE_DCOM_WCL,
    SCOPE_MFLAG_Z,
    SCOPE_DCOM_Z,
    SCOPE_REF_VEL_X,
    SCOPE_FB_VEL_X,
    SCOPE_REF_VEL_Y,
    SCOPE_FB_VEL_Y,
    SCOPE_REF_VEL_Z,
    SCOPE_FB_VEL_Z,
    MAX_SCOPE_DATA
};

enum CHECK_BUFFER_MODE
{
    CHECK_BUFFER_MODE_NONE,
    CHECK_BUFFER_MODE_ONE,
    CHECK_BUFFER_MODE_TWO,
};

#define WCL_OPEN_CURRENT_LMT		30.0
#define WCL_CLOSE_CURRENT_LMT		-30.0

#define BH_POS_COMP_KP_LOW_LMT		-0.6	//0.02 //0.04
#define BH_POS_COMP_KP_UP_LMT		0.6		//0.3 //0.08
#define BH_POS_COMP_OS_LOW_LMT		-2000	//-800
#define BH_POS_COMP_OS_UP_LMT		250		//120

enum SAMPLE_RATE_MODE
{
	NORMAL_SAMPLE_RATE,
	LOW_SAMPLE_RATE,
	HIGH_SAMPLE_RATE
};

#define SERVO_SAMPLE_RATE_LOW		1000.0	//1K
#define SERVO_SAMPLE_RATE_HIGH		10000.0	//10K

#define TABLE_LONG_DIST				10000.0	//5mm
#define SERVO_TIMEOUT				50000	//500ms
#define SERVO_DEBUG_TIMER			60000
#define BH_CTACT_SRCH_BASE_SPEED	1000

#define ACS_ERROR					0

enum SERVO_CMD_CODE
{
    SERVO_CMD_NULL = 0,
    SERVO_CMD_X_MOVE,
    SERVO_CMD_Y_MOVE,
    SERVO_CMD_RUN_PROGRAM,
    SERVO_CMD_STOP_PROGRAM,
    SERVO_CMD_READ_X_POS,
    SERVO_CMD_READ_Y_POS,
    SERVO_CMD_SEND_INTEGER,
    SERVO_CMD_SEND_REAL,
    SERVO_CMD_GET_ERROR,
    SERVO_CMD_READ_DPRAM,
    SERVO_CMD_WRITE_DPRAM,
};

#define X_MOTOR_SPEED_PROFILE_NUM     6
#define Y_MOTOR_SPEED_PROFILE_NUM     6
#define Z_MOTOR_SPEED_PROFILE_NUM     12	//v1.1.T30 add

#define X_MOTOR_CONTROL_PROFILE_NUM   12
#define Y_MOTOR_CONTROL_PROFILE_NUM   12
#define Z_MOTOR_CONTROL_PROFILE_NUM   20

struct SERVO_SPEED_PROFILE
{
    short nMaxVel;
    short nMaxAcc;
    long  lJerk;
};

struct SERVO_MOTION_PARAMETER
{
    SERVO_SPEED_PROFILE stSpeedProfileX[X_MOTOR_SPEED_PROFILE_NUM];
    SERVO_SPEED_PROFILE stSpeedProfileY[Y_MOTOR_SPEED_PROFILE_NUM];
    SERVO_SPEED_PROFILE stSpeedProfileZ[Z_MOTOR_SPEED_PROFILE_NUM];
};

struct SERVO_CONTROL_PROFILE
{
    long lVelGain;
    long lVelIntegrator;
    long lPosGain;
    long lAccFeedFwd;
};

struct SERVO_CONTROL_PARAMETER_DUMMY
{
    SERVO_CONTROL_PROFILE stControlProfileX[12];
    SERVO_CONTROL_PROFILE stControlProfileY[12];
    SERVO_CONTROL_PROFILE stControlProfileZ[9];
};

struct SERVO_CONTROL_PARAMETER
{
    SERVO_CONTROL_PROFILE stControlProfileX[X_MOTOR_CONTROL_PROFILE_NUM];
    SERVO_CONTROL_PROFILE stControlProfileY[Y_MOTOR_CONTROL_PROFILE_NUM];
    SERVO_CONTROL_PROFILE stControlProfileZ[Z_MOTOR_CONTROL_PROFILE_NUM];
};

// CServoController command target

class CServoController : public CObject
{
public:
	CServoController();
	virtual ~CServoController();
public:	//functions
	int GetInput(short bit);		//v1.1.T133 add
	int ReadForceSensor();			//v1.1.T104 add
	int ForceCaliSrchCtact(short nJogSpeed);	//v1.1.T102 add
	void EnableForceSensor(bool bEnable);		//v1.1.T101 add
	void InitConfig();				//v1.1.T89 add
	void VerifySpeedData();			//v1.1.T86 add
	void VerifyControlData();		//v1.1.T86 add
	double GetServoSampleRate();	//v1.1.T3 add
	int Init();
	void JogSpeedUp(void);			//set double jog speed
	void JogSpeedDown(void);		//set half jog speed
	void JogSpeedSlowest(void);		//set slowest jog speed
	double GetResolXY(void);		//v1.1.T71 add
	double GetResolZ(void);			//v1.1.T71 add
	//double GetTableResolution(void);		//v1.1.T118 delete
	//double GetBondHeadResolution(void);	//v1.1.T118 delete
	int GetTableMechLimit(double *pdPosLmtX, double *pdNegLmtX, double *pdPosLmtY, double *pdNegLmtY);
	int StopBuffer(short nBuffer);
	int CompileBuffer(short nBuffer);
	int TriggerBuffer(short nBuffer);	//trigger bond buffer via DPRAM
	int GetSpeedProfile(short nMotor, short nProfileNum, double *pdJerk, double *pdMaxAcc, double *pdMaxVel, short nSampleRateType = NORMAL_SAMPLE_RATE);
	int GetBondProgress(int *plBondProgress);
	int GetPositionError(short nMotor, double* pdPosError);
	int GetBondHeadPosition(double* pdPosition);	//read bond head command position
	int ReadMemoryInteger(EN_MEMORY_VAR nMemVar, int *plReadResult);
	int WriteMemoryInteger(EN_MEMORY_VAR nMemVar, int lReadResult);
	int ReadMemoryReal(EN_MEMORY_VAR nMemVar, double *pdReadResult);
	int FastMoveTable(TBL_POS* pstTblPos, short nMotionMode, short nWait);
	int FastMoveTable2(TBL_POS stPos, bool bAbsolute = true, bool bWait = false);	//v1.1.T114 add
	void GetSpeedProfile(short nMotor, short nProfileNum, SERVO_SPEED_PROFILE* pstSpeedProfile);
	void SetSpeedProfile(short nMotor, short nProfileNum, SERVO_SPEED_PROFILE* pstSpeedProfile);
	void SaveMotionParameter(void);
	void LoadMotionParameter(void);
	int SetServoSpeedProfile(short nMotor, short nProfileNum, bool bSlowSpeed = false);
	int InitWireClampProfile(void);
	int GetWireClampProfile(short nProfileIndex, ST_WCL_PROFILE *pstWclProfile);
	int SetWireClampProfile(short nProfileIndex, ST_WCL_PROFILE *pstWclProfile);
	void SaveWireClampProfile(void);
	void LoadWireClampProfile(void);
	int InitWireClamp(void);
	int UpdateWireClampProfile(short nDebugMode, bool bDryRun = false);	//v1.1.T97 edit
	int TriggerWireClampProfile(short nWclProfile);
	int ApplyForceControl(short nForce, short nTime, bool bProgramValid, REF_RECORD *pstRefRcd);
	int MoveAxisX(long lDistance, short nMotionMode, short nWait);
	int MoveAxisY(long lDistance, short nMotionMode, short nWait);
	int EndJogX(void);
	int EndJogY(void);
	int EndJogZ(void);
	int StartServoTimerDebug(short nDebugMode);
	int StopServoTimerDebug(short nDebugMode);
	int RegisterServoCmd(short nCmdCode, short nDataLength = 0, short nDebugMode = 0);
	int EnableMotor(short nMotor);
	int GetMotorAxis(short nMotor);
	int DisableMotor(short nMotor);
	int SearchHome(short nMotor);
	int SearchLimit(short nMotor, short nDirection = MOTION_POSITIVE);
	int SearchBondHeadHomeFromLimit();		//search from upper limit position
	void PowerOnServo();
	int RecoverServo(void);
	int PositionCompen(long lCtactLvl, double *pdForceFactorKa, double *pdForceFactorOs);	//v1.1.T103 rename
	int GetBondHeadEncPos(double *pdEncPos);	//v1.1.T103 rename
	int ReadBondHeadMFlags(int* plMFlags);	//read bond head motion flags
	int WriteBondHeadMFlags(int lMFlags);	//write bond head motion flags
	int WriteBondHeadDcom(double dDcom);
	int SetOutputIo(short nPort, short nBit, short nValue);
	int CheckServoFunctionError(void);
	int GetServoScopeData(double* pdWaveformData);
	int GetSpScopeData(int* piSpWave);
	void SaveControlParameter(void);
	void LoadControlParameter(int nFileLength);
	void GetControlProfile(short nMotor, short nProfileNum, SERVO_CONTROL_PROFILE* pstControlProfile);
	void SetControlProfile(short nMotor, short nProfileNum, SERVO_CONTROL_PROFILE* pstControlProfile);
	int DownloadBufferToServo(short nBuffer, char* pcProgram, short nSize);	//download buffer to servo
	int ClearBuffer(short nBuffer);
	int SetServoControlParameter(bool bIdle = true);
	// Get table servo control block from estimate position
	int GetTableCtrlBlock(TBL_POS stDesPos, short* pnCtrlBlockX, short* pnCtrlBlockY);
	void SaveAcsMemoryParamToFile(void);
	void LoadControlParameterFromFile(void);
	void LoadSpeedParameterFromFile(void);
	int SetPosCompFactor(double dPosCompKp, double dPosCompOs);		//set position compensation factor to servo
	int SetMotorZJerkFFC(int iGain, int iFactor);		//set bond head Jerk FFC to servo
	int DisableTableLimit(void);
	int EnableTableLimit(void);
	int SetBondHeadLowLimit(long lLowLimit);
	int SearchTableLimit(short nMotor, short nDirection);
	int SetWireClampIdleCurrent(double dCurrent);
	void MoveBondHeadToSafePos(void);
	int ServoPreparePowerOn(void);
	int PerformTableScrub(short nAmplitude, short nYDelay, short nCycle);
	int GetAutoTuneVersion(unsigned short *usVerMajor, unsigned short *usVerMinor);
	int AutoTuneControlParaThread(UINT uiAxis);
	int SetTableKillDec(void);		//set motion kill deceleration of table

	void GetErrorMessage(short nMsgType, short nMsgNum, short *pnMsgIndex, bool bStopValid, bool bSkipValid);
	void GetErrorMessage(short nMsgType, CStringList& szStringList, bool bStopValid, bool bSkipValid);	

	void DownloadBufferProgram(void);	//v1.1.T15 add, v1.1.T90 rename
	void ReDownloadBufferProgram(void);	//v1.1.T90 add
	void ClearAllBuffer(void);
	int SendServoCtrlProfileToServo(void);
	CString GetVersion(void);
protected:	//functions
	int TriggerRestartWire(bool bParam);	//v1.0.T14 edit, v1.1.T122 edit
    int SendRealDataToServo(double *padRealData, short nDataLength, short nBuffer, short nServoVar);
    int SendIntDataToServo(int *palIntData, short nDataLength, short nBuffer);
    int CheckServoError(short nCheckBufMode);
	int StartJogX(short nJogDirection);
	int StartJogY(short nJogDirection);
	int StartJogZ(short nJogDirection, short nJogSpeed);
	int MoveAxisZ(long lDistance, short nMotionMode, short nWait);
    int MoveTable(TBL_POS* pstTblPos, short nMotionMode, short nWait, bool bSlowSpeed = true);
    int MoveTable2(TBL_POS stPos, bool bAbsolute = true,
					bool bWait = true, bool bSlowSpeed = true);	//v1.1.T113 add
    int GetTablePosition(TBL_POS* pstTblPos, byte nWait = MOTION_WITH_WAIT);
    int ResetBondProgress(void);
	int RunBuffer(short nBuffer);
	int DisableServoController(void);	//disable all servo controllers for power off
	int SyncMotion(short nMotor);
private:	//variables
	short m_nAcsTimeout;			//v1.1.T8 add
	double m_dWaveformSampleTime;	//v1.1.T6 add
	double m_dServoSampleTime;		//v1.1.T3 add
	bool m_bJoggingX;
	bool m_bJoggingY;
	bool m_bJoggingZ;
	HANDLE m_hComm;
	TBL_POS m_TablePos;
	double m_dBondHeadPos;
	double m_dJogSpeed;		//table Jog speed encoder/second
	SERVO_MOTION_PARAMETER m_stMotionParam;
	SERVO_CONTROL_PARAMETER m_stControlParam;
	ST_WCL_PROFILE m_astWclProfile[WCL_MAX_PROFILE];
	int m_nServoTimeIndex;							//time index for servo timing debug
	long m_alDebugTimeTicker[SERVO_DEBUG_TIMER];	//time ticker of servo debug
	short m_anDebugCmdCode[SERVO_DEBUG_TIMER];		//command code for servo debug
	short m_anDebugSendData[SERVO_DEBUG_TIMER];		//send time for servo debug
	short m_nMsgType;
	short m_nMsgNum;
	short m_nMsgIndex;
	bool m_bMsgStopValid;
	bool m_bMsgSkipValid;
	CStringList m_sMsgList;
private:	//functions
	double GetTableResolution(void);	//v1.1.T119 add
	double GetBondHeadResolution(void);	//v1.1.T119 add
	int GetAnalogInput(int nPort);	//v1.1.T104 add
	void SetAnalogOutput(int nPort, int nValue);	//v1.1.T101 add
	void DownloadNewBuffer0(void);	//v1.1.T59 edit
	void DownloadNewBuffer1(void);	//v1.1.T59 edit
	void DownloadNewBuffer2(void);	//v1.1.T59 edit
	void DownloadNewBuffer7(void);	//v1.1.T59 edit
	void DownloadNewBuffer8(void);	//v1.1.T59 edit
	void DownloadNewBuffer9(void);	//v1.1.T59 edit
	void DownloadBuffer0(void);		//v1.1.T59 edit
	void DownloadBuffer1(void);		//v1.1.T59 edit
	void DownloadBuffer2(void);		//v1.1.T59 edit
	void DownloadBuffer3(void);		//v1.1.T59 edit
	void DownloadBuffer4(void);		//v1.1.T59 edit
	int SetOutputBit(short bit);
	int ClearOutputBit(short bit);
	void InitOutput(void);
    void SetErrorMessage(short nMsgType, short nMsgNum, short *pnMsgIndex, bool bStopValid = false, bool bSkipValid = false);
    void SetErrorMessage(short nMsgType, CStringList& szStringList, bool bStopValid = false, bool bSkipValid = false);
	bool IsMotorXYZ(short nMotor);
	int GetMemoryAddr(EN_MEMORY_VAR nMemVar);
	int ResetCtactDetect(void);
};
