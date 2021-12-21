//StepCardControl.h
/************************************************************************/
/*
文件功能：步进控制卡类头文件
编 译 器：VS2008
开发语言：C++
版    本：Version 1.0
作    者：赵飞麒
实现文件：StepCardControl.cpp
修改时间：2014.9.1
Copyright (C) 2000, ZhaoFeiqi, all rights reserved.
*/                                                 
/************************************************************************/

#ifndef _STEPCARDCONTROL_H
#define _STEPCARDCONTROL_H

#pragma once

#include "HTimer.h"
#ifdef _KANDE_CAN
#include "KANDE_CAN.h"
#else
#include "ControlCAN.h"
#endif
// CStepCardControl 命令目标

extern bool PeekAndPump();

#define FASTLOCK                  CRITICAL_SECTION
#define FastLockInit(x)           InitializeCriticalSection(x)
#define FastLockAcquire(x)        EnterCriticalSection(x)
#define FastLockTryAcquire(x)     TryEnterCriticalSection(x)
#define FastLockRelease(x)        LeaveCriticalSection(x)
#define FastLockClose(x)          DeleteCriticalSection(x)

template<class T> class CScopedLock
{
private:
	T* m_pLock;
public:
	CScopedLock(T* rhs)
	{
		m_pLock = rhs;
		m_pLock->Acquire();
	};
	~CScopedLock()
	{
		if(m_pLock != NULL)
		{
			m_pLock->Release();
		}
	};
	CScopedLock& operator=(CScopedLock const&);
};

class CFastLock
{
private:
	FASTLOCK m_Lock;
public:
	CFastLock();
	~CFastLock();
	void Acquire();
	void Release();
	void Release(UINT uType)
	{
		//Do nothing
	};
};

#define AXIS_OF_CARD      3
#define AXIS_NUM      8
#define NEGATIVE 0
#define POSITIVE 1

#define INPUT_NUM   16
#define OUTPUT_NUM   8

#define STEP_MOTOR_TIMEOUT       15000

#define MAG_PULL_DISTANCE   200000

#define NCLOSE 0
#define NOPEN 1

const char MOVEABS=1;
const char MOVEREL=2;
const char SETVEL=3;
const char SETACC=4;
const char SETDCC=5;
const char SETSTOPDCC=6;
const char STOPMOVE=7;
const char SETPOSITION=8;
const char SETOUTPUT=9;
const char SETSOFTLMTPOS=10;
const char SETSOFTLMTNEG=11;
const char HOME=12;
const char HOMEOFFSET=13;
const char HOMEDIR=14;
const char HOMEBACK=15;
const char HOMEFASTVEL=16;
const char HOMESLOWVEL=17;
const char HOMEEDGE=18;
const char LMTSENSE=19;
const char GETSTATUS=20;
const char GETPOSITION=21;
const char GETVEL=22;
const char GETINPUT=23;
const char ENABLELIMINT=24;
const char GETINPUTBIT=26;
const char SETOUTPUTBIT=27;
const char SEARCH=28;
const char GETSEARCHPOS=29;
const char SEARCHEDGE=30;

enum CAN_ACTION_ERROR
{
	CAN_ACTION_NORMAL,
	SEND_ERROR,
	REC_TIMEOUT,
};
enum CARD_ADDRESS
{
	ONLOADER_CARD_ADDRESS = 0,
	OFFLOADER_CARD_ADDRESS,
	INDEXER_CARD_ADDRESS,
	IO_CARD_ADDRESS

};

typedef enum enInjectStep
{
	DC_MOTOR_IDLE = 0, 
	DC_MOTOR_MOVE_TO_LIMIT,
	DC_MOTOR_MOVE_TO_HOME,
};

typedef enum MagPullMode
{
	TRIGGER_TO_HOME = 0, 
	TRIGGER_TO_LIMIT,
	TRIGGER_TO_CIRCLE
};

typedef enum KeJie_StepCard_Assign
{
	TOP_ONLOAD_MAG_CLAMPPER_EX         = 0,     //1_Axis
	TOP_ONLOAD_HOLDER_EX               = 1,	 //2_Axis
	TOP_ONLOAD_PULLY_EX                = 2,     //3_Axis
	TOP_OFFLOAD_MAG_CLAMPPER_EX        = 3,	 //3_Axis
	TOP_OFFLOAD_HOLDER_EX              = 4,     //2_Axis //card2 
	TOP_OFFLOAD_PULLY_EX               = 5,	 //1_Axis
	TOP_DEVICE_INDEXER_EX              = 6,     //1_Axis //card3
	TOP_WINDOW_CLAMPER_EX              = 7,     //2_Axis 
};


struct AxisParameter                       //电机参数
{
	long Pulses;
	double ScrewPitch;
	double m_Scale;
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

struct STEPPER_CARD_SPEED_SETTING
{
	long  Acc;                
	long  Dcc;
	long  Vel;
	long  StopDcc;
};

struct IO_CONFIG
{
	short m_port;   //端口号
	short m_signal;  //开关信号
};

class CInputList
{
public:
	CInputList();
	~CInputList(){};
public:
	IO_CONFIG m_OnLoaderMagClamperHome;
	IO_CONFIG m_OnLoaderMagHolderHome;
	IO_CONFIG m_OnLoaderMagHolderLimit;
	IO_CONFIG m_InjectPusherHome;
	IO_CONFIG m_InjectPusherLimit;
	IO_CONFIG m_OnLoaderMagPullyHome;
	IO_CONFIG m_OnLoaderMagPullyLimit;
	IO_CONFIG m_OnLoaderMagDetectedSensor; //中
	IO_CONFIG m_OnLoaderPullyMagExistSensor;//料盒存在
	IO_CONFIG m_OnLoaderMagDropSensor;//下
	IO_CONFIG m_OnLoaderMagBufferPresentSensor;//上
	IO_CONFIG m_InjectSensor;
	IO_CONFIG m_AlignSensor;

	IO_CONFIG m_OffLoaderMagClamperHome;
	IO_CONFIG m_OffLoaderMagHolderHome;
	IO_CONFIG m_OffLoaderMagHolderLimit;
	IO_CONFIG m_OffLoaderMagPullyHome;
	IO_CONFIG m_OffLoaderMagPullyLimit;
	IO_CONFIG m_OffLoaderMagDetectedSensor;
	IO_CONFIG m_OffLoaderPullyMagExistSensor;
	IO_CONFIG m_OffLoaderMagDropSensor;
	IO_CONFIG m_OffLoaderMagBufferPresentSensor;
	IO_CONFIG m_EjectSensor;

	IO_CONFIG m_IndexerHome;
	IO_CONFIG m_IndexerLimit;
	IO_CONFIG m_WindowClampHome;
	IO_CONFIG m_EjectJamSensor;
	IO_CONFIG m_VaccumSnr;

	IO_CONFIG m_JoyStickPos_X;
	IO_CONFIG m_JoyStickNeg_X;
	IO_CONFIG m_JoyStickPos_Y;
	IO_CONFIG m_JoyStickNeg_Y;

	IO_CONFIG m_HeaterDetector;
	IO_CONFIG m_ThermalSensorX;
	IO_CONFIG m_ThermalSensorY;




};

class COutputList
{
public:
	COutputList();
	~COutputList(){};
	
public:
	IO_CONFIG m_InjectPusherDir;
	IO_CONFIG m_InjectPusherPower;
	IO_CONFIG m_LFHoler;
	IO_CONFIG m_InjectClaw;
	IO_CONFIG m_InputClaw;
	IO_CONFIG m_OutputClaw;
	IO_CONFIG m_EjectClaw;
	IO_CONFIG* m_TargetClaw;
	IO_CONFIG m_Vacuum;

	IO_CONFIG m_GreenLight;
	IO_CONFIG m_RedLight;
	IO_CONFIG m_YellowLight;
	IO_CONFIG m_PanelLight;
	IO_CONFIG m_StepCardPowerCtrl;
	IO_CONFIG m_N2h2Port;
	IO_CONFIG m_AirTensioner;



};

class CStepCardControl : public CObject
{
public:
	CStepCardControl();
	virtual ~CStepCardControl();
public:
	CInputList m_InputList;
	COutputList m_OutputList;
	CHTimer m_Timer;
	AxisParameter m_StepParameter[AXIS_NUM];
	static CFastLock m_sFastLock;

private:
	HANDLE m_hDevice;
	HANDLE m_hEvent;
	BOOL InitializeCan();
	HANDLE OpenDevice(DWORD DeviceType,DWORD instance,DWORD Error);
	bool m_bIsAxisMoving[AXIS_NUM];
	bool m_bSendAndRecFinish;
	long m_SearchPos;
	long m_AxisPos[AXIS_NUM];
	long m_lJogMoveStep;
	char m_Addr;
	char m_AxisID;
	int m_iSendAndRecCount;
	int m_InjectPushStep;
	int m_MagezinePullStep[2];
	int m_MotionMoveStep;
	CHTimer m_Timer_Can;
	
	

	unsigned short m_InputValue[4];
	unsigned short m_OutputValue[4];

	int SendCanData(const char addr, const char axisID, const char command, long data=0);
	void Process_RecData(VCI_CAN_OBJ RecData);
	int DisableStepController(short bOperationMode);
	int SendPortData(const char addr, const char axisID, const char command, long data=0);
	DWORD VCI_ReceiveEx(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive, int WaitTime= -1 );


public:

	void SetSerialAddress(unsigned short add=0);
	int SetStepSpeedParameter(unsigned short axis_index, AxisParameter axisParameter);
	int InjectPush();

	int MagezinePull(unsigned short addr, int mode);
	void ResetMagPullyState();
	int GetInjectPushState();
	void ResetInjectPushState();
	bool GetPortState(IO_CONFIG Input);
	bool GetPortStateByPort(short port,bool signal=NOPEN);
	bool GetOutputPortState(IO_CONFIG Output);
	void OpenPortByPort(short port, short signal=NOPEN);
	void ClosePortByPort(short port, short signal=NOPEN);
	void OpenPort(IO_CONFIG Output);
	void ClosePort(IO_CONFIG Output);
	bool InitStepCard();
	void InitInputPort();
	void InitOutputPort();
	void CloseAllOutput();

	float m_Time;
	
	
	
public:
	int SetAllStepSpeedParameter();
	int SetMotorSpeedProfile(unsigned short axis_index, STEPPER_CARD_SPEED_SETTING axis_speedsetting);
	void InitStepParameter();
	// Check if step busy
	int CheckStepBusyState(unsigned short usAxis);
	// Move step motor
	int MoveMotorSteps(unsigned short usAxis, long lMoveDist, short nMotionMode = MOVEABS);
	int MoveMotorStepsEx(unsigned short usAxis, long lMoveDist, short nMotionMode );
	// Check step motion status
	int CheckStepMotionStatus(unsigned short usAxis, long* plDestinationDist);
	// Get the resolution of step motor
	int CheckStepMotionSearchDis(unsigned short usAxis, long* plDestinationDist);

	double GetStepResolution(unsigned short usAxis);
	// Search stepper home
	int SearchStepHome(unsigned short usAxis);
	// Stop stepper 
	int StopStep(unsigned short usAxis);
	
	int ReadMotorStopReason(unsigned short usAxis, BOOL* bitHome, BOOL* bitNLimit, BOOL* bitPLimit);

//	int SetMotorSpeedProfile(byte bMotor,STEPPER_SPEED_SETTING *pstSpeedProfile);

	int GetStepperHomeSensor(unsigned short usAxis, BOOL* bHomeBit,BOOL* bNLimitBit,BOOL* bPLimitBit);

	int GetHomeSensor(unsigned short usAxis, BOOL* bHomeBit);

	int StartJogStepper(unsigned short usAxis, short nMotionDir);

	void SetJogMoveStep(long lMoveStep);

	long GetJogMoveStep(unsigned short usAxis);

	int StopAllStep(unsigned short addr);

public:
	int SetSection(unsigned short usAxis, long lAcc, long lDcc);
	// Set output port state @3
	int SetOutputPort(short nOutputId, short nState);
	int PowerOnStepper(void);
	int PowerOffStepper(void);
	int CheckStepperPowerStatus(bool *pbStepperOn); //@5

public:
	int UpdateInputState(char addr);
	int UpdateOutputState(char addr);

};

extern CStepCardControl m_StepCardControl;

#endif

