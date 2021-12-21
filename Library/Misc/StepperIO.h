//StepperIO.h v1.2.T41

#ifndef _STEPCARDCONTROL_H
#define _STEPCARDCONTROL_H

#pragma once

#include "HTimer.h"
#include "ControlCAN.h"
#include "CriticalSection.h"
#include "CanController.h"	//v1.2.T22 add

// CStepperIO

#define CARD_AXIS_NUM	5	//v1.2.T32 rename
#define TOTAL_AXIS_NUM	14	//v1.2.T32 rename
#define NEGATIVE		0
#define POSITIVE		1

#define INPUT_NUM		16
#define OUTPUT_NUM		16

#define STEP_MOTOR_TIMEOUT	15000

#define MAG_PULL_DISTANCE	160000

#define NCLOSE	0
#define NOPEN	1

const char UPDATELIGHT		= 0;
const char MOVEABS			= 1;
const char MOVEREL			= 2;
const char SETVEL			= 3;
const char SETACC			= 4;
const char SETDCC			= 5;
const char SETSTOPDCC		= 6;
const char STOPMOVE			= 7;
const char SETPOSITION		= 8;
const char SETOUTPUT		= 9;
const char SETSOFTLMTPOS	= 10;
const char SETSOFTLMTNEG	= 11;
const char HOME				= 12;
const char HOMEOFFSET		= 13;
const char HOMEDIR			= 14;
const char HOMEBACK			= 15;
const char HOMEFASTVEL		= 16;
const char HOMESLOWVEL		= 17;
const char HOMEEDGE			= 18;
const char LMTSENSE			= 19;
const char GETSTATUS		= 20;
const char GETPOSITION		= 21;
const char GETVEL			= 22;
const char GETINPUT			= 23;
const char ENABLELIMINT		= 24;
const char GETINPUTBIT		= 26;
const char SETOUTPUTBIT		= 27;
const char SEARCH			= 28;
const char GETSEARCHPOS		= 29;
const char SEARCHEDGE		= 30;
const char ENABLESTEPPER	= 31;
const BYTE FW_GETVERSION	= 255;	//V1.0.W17 add

enum EN_CAN_ADDRESS		//v1.2.T25 edit
{
	CANADDR_ONLOAD = 0,
	CANADDR_OFFLOAD,
	CANADDR_INDEX,
	CANADDR_IO
};

enum EN_INJECT_STEP		//v1.2.T25 edit
{
	DC_MOTOR_IDLE = 0, 
	DC_MOTOR_MOVE_TO_LIMIT,
	DC_MOTOR_MOVE_TO_HOME,
};

enum EN_MAG_PULL_MODE	//v1.2.T25 edit
{
	TRIGGER_TO_HOME = 0, 
	TRIGGER_TO_LIMIT,
	TRIGGER_TO_CIRCLE
};

enum EN_STEPPER	//v1.2.T25 edit
{
	ONLOAD_MAG_CLAMP_FRONT_EX      = 0,   //Axis 1	//card 1
	ONLOAD_HOLDER_EX               = 1,	  //Axis 2
	ONLOAD_PULLY_EX                = 2,   //Axis 3
	ONLOAD_MAG_CLAMP_BACK_EX       = 3,   //Axis 4
	ONLOAD_MAG_LEN_EX              = 4,   //Axis 5

	OFFLOAD_MAG_CLAMP_FRONT_EX     = 5,	  //Axis 6	//card 2
	OFFLOAD_HOLDER_EX              = 6,   //Axis 7
	OFFLOAD_PULLY_EX               = 7,	  //Axis 8
	OFFLOAD_MAG_CLAMP_BACK_EX      = 8,   //Axis 9
	OFFLOAD_MAG_LEN_EX             = 9,   //Axis 10

	DEVICE_INDEXER_EX              = 10,  //Axis 11	//card 3
	WINDOW_CLAMP_EX                = 11,  //Axis 12
	TRACK_FRONT_EX                 = 12,  //Axis 13
	TRACK_BACK_EX                  = 13,  //Axis 14
};

struct ST_AXIS_PARA		//v1.2.T33 rename
{
	long Pulses;
	double ScrewPitch;
	double Scale;
	long Acc;
	long Dcc;
	long Vel;
	long StopDcc;
	long FastHomeVel;
	long SlowHomeVel;
	long SlowHomeDistance;
	long HomeOffset;
	short LimitSignal;
	short HomeSignal;
	short HomeDirection;
};

struct ST_STEPPER_SPEED	//v1.0.T39 rename
{
	long  Acc;                
	long  Dcc;
	long  Vel;
	long  StopDcc;
};

struct ST_IO_CONFIG	//v1.0.T39 rename
{
	short nPort;	//v1.2.T29 rename
	short nSignal;	//v1.2.T29 rename
};

class CInputList
{
public:
	CInputList();
	~CInputList(){};
public:
	//v1.2.T29 rename
	ST_IO_CONFIG m_OnloadClamperFrontHome;
	ST_IO_CONFIG m_OnloadHolderHome;
	ST_IO_CONFIG m_OnloadHolderLimit;
	ST_IO_CONFIG m_InjectPusherHome;
	ST_IO_CONFIG m_InjectPusherLimit;
	ST_IO_CONFIG m_OnloadPullyHome;
	ST_IO_CONFIG m_OnloadPullyLimit;
	ST_IO_CONFIG m_OnloadMagExistSensor;		//middle
	ST_IO_CONFIG m_OnloadPullyMagExistSensor;
	ST_IO_CONFIG m_OnloadMagDropSensor;		//bottom
	ST_IO_CONFIG m_OnloadMagBufExistSensor;	//top
	ST_IO_CONFIG m_InjectSensor;
	ST_IO_CONFIG m_AlignSensor;
	ST_IO_CONFIG m_OnloadClamperBackHome;
	ST_IO_CONFIG m_OnloadMagLengthHome;

	//v1.2.T29 rename
	ST_IO_CONFIG m_OffloadClamperFrontHome;
	ST_IO_CONFIG m_OffloadHolderHome;
	ST_IO_CONFIG m_OffloadHolderLimit;
	ST_IO_CONFIG m_OffloadPullyHome;
	ST_IO_CONFIG m_OffloadPullyLimit;
	ST_IO_CONFIG m_OffloadMagExistSensor;
	ST_IO_CONFIG m_OffloadPullyMagExistSensor;
	ST_IO_CONFIG m_OffloadMagDropSensor;
	ST_IO_CONFIG m_OffloadMagBufExistSensor;
	ST_IO_CONFIG m_EjectSensor;
	ST_IO_CONFIG m_OffloadClamperBackHome;
	ST_IO_CONFIG m_OffloadMagLengthHome;

	//v1.2.T29 rename
	ST_IO_CONFIG m_IndexerHome;
	ST_IO_CONFIG m_IndexerLimit;
	ST_IO_CONFIG m_WindowClampHome;
	ST_IO_CONFIG m_EjectJamSensor;
	ST_IO_CONFIG m_VacuumSensor;	//v1.2.T34 rename
	ST_IO_CONFIG m_TrackFrontHome;
	ST_IO_CONFIG m_TrackBackHome;
	ST_IO_CONFIG m_TrackLimit;

	//v1.2.T29 rename
	ST_IO_CONFIG m_JoyStickPosX;
	ST_IO_CONFIG m_JoyStickNegX;
	ST_IO_CONFIG m_JoyStickPosY;
	ST_IO_CONFIG m_JoyStickNegY;

	ST_IO_CONFIG m_HeaterDetector;
	ST_IO_CONFIG m_ThermalSensorX;
	ST_IO_CONFIG m_ThermalSensorY;
	ST_IO_CONFIG m_Emergency;
};

class COutputList
{
public:
	COutputList();
	~COutputList(){};
public:
	ST_IO_CONFIG m_InjectPusherDir;
	ST_IO_CONFIG m_InjectPusherPower;
	ST_IO_CONFIG m_LFHoler;
	ST_IO_CONFIG m_InjectClaw;
	ST_IO_CONFIG m_InputClaw;
	ST_IO_CONFIG m_OutputClaw;
	ST_IO_CONFIG m_EjectClaw;
	ST_IO_CONFIG* m_TargetClaw;
	ST_IO_CONFIG m_Vacuum;

	ST_IO_CONFIG m_AllStepCardRst;
	ST_IO_CONFIG m_GreenLight;
	ST_IO_CONFIG m_RedLight;
	ST_IO_CONFIG m_YellowLight;
	ST_IO_CONFIG m_PanelLight;
	ST_IO_CONFIG m_StepCardPowerCtrl;
	ST_IO_CONFIG m_N2h2Port;
	ST_IO_CONFIG m_AirTensioner;
	ST_IO_CONFIG m_OnloadCardRst;
	ST_IO_CONFIG m_OffloadCardRst;
	ST_IO_CONFIG m_IndexCardRst;
};

class CStepperIO : public CObject
{
public:
	CStepperIO();
	virtual ~CStepperIO();
public:	//variable
	CInputList m_Input;		//v1.2.T29 rename
	COutputList m_Output;	//v1.2.T29 rename
	CHTimer m_Timer;
	ST_AXIS_PARA m_astAxisPara[TOTAL_AXIS_NUM];	//v1.2.T33 rename
public:	//function
	int SetAxisPara(WORD nAxis, ST_AXIS_PARA stAxisPara);	//v1.2.T34 edit
	int InjectPush();
	bool ResetStepCard();
	void ResetMagPullyState();
	int GetInjectPushState();
	void ResetInjectPushState();
	bool GetPortState(ST_IO_CONFIG Input);
	bool GetPortStateByPort(short port,bool signal = NOPEN);
	bool GetPortStateByPortEx(short port,bool signal);
	bool GetOutputPortState(ST_IO_CONFIG Output);
	void OpenPortByPort(short port, short signal = NOPEN);
	void ClosePortByPort(short port, short signal = NOPEN);
	void OpenPort(ST_IO_CONFIG Output);
	void ClosePort(ST_IO_CONFIG Output);
	bool InitStepCard();
	void InitInputPort();
	void InitOutputPort();
	void CloseAllOutput();
	int EnableStepper(char cAddr, bool bEnable);	//v1.2.T28 edit
	int SetAllAxisPara();	//v1.2.T34 rename
	void SetMotorSpeedProfile(unsigned short axis_index, ST_STEPPER_SPEED axis_speedsetting);
	int CheckStepBusyState(unsigned short usAxis);
	int MoveStep(unsigned short usAxis, long lStep, short nMotionMode);
	int MoveDist(unsigned short usAxis, long lDistMicron, short nMotionMode);
	int MoveStepSearch(unsigned short usAxis, long lMoveStep);
	int CheckStepMotionStatus(unsigned short usAxis, long* plDestinationDist);
	int CheckStepMotionSearchDis(unsigned short usAxis, long* plDestinationDist);
	double GetResolution(unsigned short usAxis);
	int SearchStepHome(unsigned short usAxis);
	int StopStep(unsigned short usAxis);
	int ReadMotorStopReason(unsigned short usAxis, BOOL* bitHome, BOOL* bitNLimit, BOOL* bitPLimit);
	int GetStepperHomeSensor(unsigned short usAxis, BOOL* bHomeBit,BOOL* bNLimitBit,BOOL* bPLimitBit);
	int GetHomeSensor(unsigned short usAxis, BOOL* bHomeBit);
	int StartJogStepper(unsigned short usAxis, short nMotionDir);
	void SetJogMoveStep(long lMoveStep);
	long GetJogMoveStep(unsigned short usAxis);
	int SetSection(unsigned short usAxis, long lAcc, long lDcc);
	int SetOutputPort(short nOutputId, short nState);
	void PowerOn(void);
	void PowerOff(void);
	bool IsPowerOn();
	int UpdateInputState(char cAddr);	//v1.2.T27 edit
	int UpdateOutputState(char cAddr);	//v1.2.T27 edit
	CString GetVersion(char nAxis);		//V1.0.W17 add
	void SetWireSpoolPara(char cCardAddr, char cCardAxis, char cCommand, long lData);	//v1.2.L30 add
private:	//variable
	CCriticalSection m_CS;	//v1.2.T7 add
	HANDLE m_hDevice;
	DWORD m_lDeviceType;	//v1.0.T21 add
	bool m_abAxisMoving[TOTAL_AXIS_NUM];	//v1.0.T36 rename
	long m_alAxisPos[TOTAL_AXIS_NUM];		//v1.0.T36 rename
	long m_lSearchPos;
	long m_lJogMoveStep;
	int m_nInjectPushStep;
	int m_anMagPullStep[2];	//v1.0.T37 rename
	int m_nMotionMoveStep;
	bool m_bPowerOn;
	long m_lLightingData;
	long m_alInput[4];		//v1.0.T37 rename
	long m_alOutput[4];		//v1.0.T37 rename
	UINT m_nVersion;		//V1.0.W17 add
private:	//function
	void SetLightPara(char cCoaxRed, char cSideRed, char cCoaxBlue, char cSideBlue);	//v1.0.T38 edit
	int MagPully(WORD nAddr, int nMode);	//v1.0.T38 edit
	VCI_CAN_OBJ m_astRxBuf[50];	//v1.2.T19 add
	void ToCardAddrAxis(char cAxisID, char *pcAddr, char *pcAxis);	//v1.2.T14 add
	BOOL InitCan();
	int SendCanData(const char cAxisID, const char cCommand, long lData = 0);
	int SendData(char cCardAddr, char cCardAxis, char cCommand, long lData = 0, bool bShowErr = true);	//v1.2.T2 add, v1.2.T18 edit
	int ProcessData(char cTxAddr, char cTxAxis, VCI_CAN_OBJ stRxData);	//v1.2.T21 edit
	int SendPortData(const char addr, const char axisID, const char command, long data = 0);
	int MoveStepEx(unsigned short usAxis, long lMoveStep, short nMotionMode);
	void InitStepParameter();	//v1.2.T5
};

extern CStepperIO theStepperIO;

#endif
