//CuOnload.h v1.0.T17

#pragma once

#include "DcMotor.h"
#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"

#define MAG_CLAMPER_OFF     0
#define MAG_CLAMPER_ON      1
#define ONLOAD_TIME_OUT     20000   //8s

// CCuEliteOnload command target

typedef enum enCuEliteOnloadEvent 
{
	CUELITE_ONLOAD_EVENT_IDLE, 
	CUELITE_ONLOAD_EVENT_START,
	CUELITE_ONLOAD_EVENT_INIT,
	CUELITE_ONLOAD_EVENT_NORMAL,
	CUELITE_ONLOAD_EVENT_LAST,
	CUELITE_ONLOAD_EVENT_FIRST,
};

typedef enum enCuEliteOnloadState
{
	CUELITE_ONLOAD_STATE_IDLE,
	CUELITE_ONLOAD_STATE_WAIT_TO_1ST_SLOT,
	CUELITE_ONLOAD_STATE_WAIT_PUSHER,
	CUELITE_ONLOAD_STATE_WAIT_NEXT_SLOT_POSITION,
	CUELITE_ONLOAD_STATE_WAIT_CYLINDER_CLAMP,//==TOTAL SLOT
	CUELITE_ONLOAD_STATE_WAIT_TO_BOTTOM,
	CUELITE_ONLOAD_STATE_WAIT_PULLY_PUSHING,
	CUELITE_ONLOAD_STATE_WAIT_PULLY_TO_LIMIT,
	CUELITE_ONLOAD_STATE_WAIT_PULLY_TO_HOME,
	CUELITE_ONLOAD_STATE_WAIT_LIFTING,
	CUELITE_ONLOAD_STATE_WAIT_CYLINDER_RELEASE,
	CUELITE_ONLOAD_WAIT_INJECT_FINISH,
	CUELITE_ONLOAD_STATE_ONLOAD_END, 
};

typedef enum enCuEliteOnloadDryRunState
{
	CUELITE_ONLOAD_DRYRUN_IDLE,
	CUELITE_ONLOAD_DRYRUN_TESTSTART,
	CUELITE_ONLOAD_DRYRUN_ALL_HOME,
	CUELITE_ONLOAD_DRYRUN_WAIT_ALL_HOME,
	CUELITE_ONLOAD_DRYRUN_WAIT_TO_1ST_SLOT,
	CUELITE_ONLOAD_DRYRUN_WAIT_PUSHER,
	CUELITE_ONLOAD_DRYRUN_WAIT_NEXT_SLOT_POSITION,
	CUELITE_ONLOAD_DRYRUN_WAIT_CLAMPER_NOMAL,
	CUELITE_ONLOAD_DRYRUN_WAIT_CLAMPER_CLAMP,
	CUELITE_ONLOAD_DRYRUN_WAIT_TO_BOTTOM,
	CUELITE_ONLOAD_DRYRUN_WAIT_PULLY_PUSHING,
	CUELITE_ONLOAD_DRYRUN_WAIT_LIFTING,
	CUELITE_ONLOAD_DRYRUN_WAIT_CLAMPER_RELEASE,
	CUELITE_ONLOAD_DRYRUN_INJECT_FINISH,
	CUELITE_ONLOAD_DRYRUN_END, 
};

class CCuEliteOnload : public CObject
{
public:

	CCuEliteOnload();

	virtual ~CCuEliteOnload();

	void Initialize(void);
	void SubmitOnloadEvent(short nEvent);
	void SubmitOnloaderDryRunEvent(short nEvent); //v2.20.Q1 add
	void ProcessOnloadEvent(void);
	void ProcessOnloaderDryRunEvent(void); //v2.20.Q1 add

	CDcMotor theInjectPusher;
	CDcMotor theMagPullyDcMotor;

	//CCylinder theMagClampCylinder;    // v2.18.1.S6 delete

	CSensor theMagDetectedSensor;
	CSensor thePullyMagExistSensor;
	CSensor theMagDropSensor;               // v2.18.1.S6 add
	CSensor theForkRemoveDetectedSensor;    // v2.18.1.S6 add
	CSensor theFork1PresentSensor;
	CSensor theFork2PresentSensor;
	CSensor theMagBufferPresentSensor;

	CSensor theMagPullyHome;      //v2.20.Q1 add
	CSensor theMagPullyLimit;     //v2.20.Q1 add
	//CSensor theInjectSensor;    //v2.20.Q8 delete  //v2.20.Q1 add

	// Step motor id of onload holder
	byte m_ucForkHolderStepId;

	// Step motor id of onload clamper Front
	byte m_ucMagClamperFrontStepId;

	// Step motor id of onload clamper Back
	byte m_ucMagClamperBackStepId;

	// Step motor id of onload Length
	byte m_ucMagLengthStepId;

	// Step motor id of Onload Pully
	byte m_ucMagPullyStepId;                    //v2.20.Q1 add

	short m_nTestState; //v2.20.Q1 add

	CHTimer m_DelayTimer;
	long m_lOnloadMagLenLimit;

	short GetOnloadEvent(void);

	void SearchOnloadElvStepHome(void);
	void WaitStepHomeFinish(void);

	void SearchOnloadMagClamperFrontStepHome(void);
	void WaitOnloadMagClamperFrontStepHomeFinish(void);

	short GetOnloadErrorCode(void);

	void ResetOnloadErrorCode(void);
	void SetCurrentSlotNum(short nSlot);

	short GetCurrentSlotNum(void);
	// Set onload valid flag
	void SetOnloadValid(BOOL bOnloadValid);
	// Get onload valid flag
	BOOL GetOnloadValidState(void);

	void MoveTotheSlot(short nSlotIndex);
	void SaveOnloadRcd(void);
	void LoadOnloadRcd(void);
	void GetCuEliteOnloadRcd(CUELITE_ONLOAD_RCD* stOnloadRcd);
	void UpdateCuEliteOnloadRcd(CUELITE_ONLOAD_RCD* stOnloadRcd);
	void ResetOnloadEvent(void);

	void CaculateOffsetToBase(long* lOffsetToBase, short nCurrentSlot);

	void ResetOffsetRecord(void);
	void SetOnloadReadyFlag(bool bFlag);

	bool GetOnloadReadyFlag(void);

	void EjectOnloadMag(void);

	bool GetOnloadEmptyflag(void);
	void SetOnloadEmptyflag(bool bflag);

	bool GetOnloadEmptyReported(void);
	void SetOnloadEmptyReported(bool bflag);

	bool GetOnloadBufferEmptyflag(void);
	void SetOnloadBufferEmptyflag(bool bflag);

	bool GetOnloadBufferEmptyReported(void);
	void SetOnloadBufferEmptyReported(bool bflag);

	bool GetOnloadDrogflag(void);
	void SetOnloadDrogflag(bool bflag);

	bool GetOnloadDrogReported(void);
	void SetOnloadDrogReported(bool bflag);

	bool GetOnloadPullyFullflag(void);
	void SetOnloadPullyFullflag(bool bflag);

	bool GetOnloadPullyFullReported(void);
	void SetOnloadPullyFullReported(bool bflag);

	//	int ResetOnloadMagBuffFullFlag(void);    // v2.18.1.S6  delete
	//	bool GetOnloadMagBuffFullFlag(void);
	//	bool m_bOnloadFullMsgReported;
	//private:
	//	bool m_bOnloadFullMsg;
	//public:
	//	int SetOnloadMagBuffFullFlag(bool bFlag);

	// Relative move to last LF of magazine.
	void RelativeMoveToLastSlot(void);

	void ResetOnloadInitStatus(void);
	BOOL GetOnloaderInitStatus(void);
	void moveMagClamperStepForward();
	void moveMagClamperStepBackward();
	void OnloadMagClamperFrontMovetoStandyPos();
	void OnloadMagClamperFrontStop();
	void OnloadMagClamperBackMovetoStandyPos();
	void OnloadMagClamperBackStop();
	void OnloadMagLengthMovetoStandyPos();
	void OnloadMagLengthStop();

	void SetFirstIndex(bool bFirstSuccess, bool bAutoIndex);	//V3.3.W170 add

	bool m_bIsTriggerClamperPositon; //v2.20.Q1 add
	void moveMagClamperOpen();//v2.20.Q1 add
	int UpdateAllSensor();//v2.20.Q1 add
	int UpdateRelayState();//v2.20.Q1 add
	int GetSensorState(ST_IO_CONFIG Sensor);//v2.20.Q1 add
	void WaitMagClamperMoveFinish();//v2.20.Q1 add
	void WaitMagHoledrMoveFinish();//v2.20.Q1 add
	void SearchOnloadMagClamperBackStepHome(void);
	void WaitOnloadMagClamperBackStepHomeFinish(void);
	void SearchOnloadMagLengthStepHome(void);
	void WaitOnloadMagLengthStepHomeFinish(void);
	void SearchOnloadElevetorLimit(void);
	int SetOnloadMagLenLimit();

private:

	bool m_bOnloadEmptyflag;
	bool m_bOnloadEmptyReported;
	bool m_bOnloadBufferEmptyflag;
	bool m_bOnloadBufferEmptyReported;
	bool m_bOnloadDrogflag;
	bool m_bOnloadDrogReported;
	bool m_bOnloadPullyFullflag;
	bool m_bOnloadPullyFullReported;

	short m_nOnloadEvent;
	short m_nOnloadErrorCode;
	short m_nOnloadState;
	short m_nCurrentSlot;   // =0:no magzine; =1 first slot ; =20 20th slot
	short m_nLastOnloadDebugState;

	BOOL m_bOnloadValid;

	// Onload holder step speed profile
	STEP_SPEED_PROFILE m_stForkHolderSpeedProfile;
	ST_AXIS_PARA m_stForkHolderProfile;   //v2.20.Q1 add

	// Onload clamper step speed profile
	STEP_SPEED_PROFILE m_stMagClamperFrontStepSpeedProfile;
	ST_AXIS_PARA m_stMagClamperFrontProfile; //v2.20.Q1 add

	// Onload clamper step speed profile
	STEP_SPEED_PROFILE m_stMagClamperBackStepSpeedProfile;
	ST_AXIS_PARA m_stMagClamperBackProfile; //v2.20.Q1 add

	// Onload clamper step speed profile
	STEP_SPEED_PROFILE m_stMagLengthStepSpeedProfile;
	ST_AXIS_PARA m_stMagLengthProfile; //v2.20.Q1 add

	// Onload Pully step speed Profile
	ST_AXIS_PARA m_stMagPullyProfile;//v2.20.Q1 add

	CUELITE_ONLOAD_RCD m_stOnloadRcd;

	long m_lStandyPos;
	long m_lSlotHeight;

	bool m_bOnloadReady;

	short m_nOnloaderRetryTimes;

	BOOL m_bOnloadAfterInit;

	bool m_bFirstSuccess, m_bAutoIndex;		//V3.3.W170 add
};

extern CCuEliteOnload theCuEliteOnloader;	//v3.3.T161 add
