//CuOffload.h v1.0.T17

#pragma once

#include "DcMotor.h"
#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"

#define OFFLOAD_START_DELAY   200  //ms

// CCuEliteOffload command target

typedef enum enCuEliteOffloadEvent 
{
	CUELITE_OFFLOAD_EVENT_IDLE, 
	CUELITE_OFFLOAD_EVENT_START,
	CUELITE_OFFLOAD_EVENT_INIT,
	CUELITE_OFFLOAD_EVENT_NORMAL,
	CUELITE_OFFLOAD_EVENT_LAST,
};

typedef enum enCuEliteOffloadState
{
	CUELITE_OFFLOAD_STATE_IDLE,
	CUELITE_OFFLOAD_STATE_WAIT_START_DELAY,  // WAIT INDEXER MOVE AWAY
	CUELITE_OFFLOAD_STATE_WAIT_TO_1ST_SLOT,
	CUELITE_OFFLOAD_STATE_WAIT_NEXT_SLOT_POSITION,
	CUELITE_OFFLOAD_STATE_WAIT_CYLINDER_CLAMP,//==TOTAL SLOT
	CUELITE_OFFLOAD_STATE_WAIT_TO_BOTTOM,
	CUELITE_OFFLOAD_STATE_WAIT_PULLY_PUSHING,
	CUELITE_OFFLOAD_STATE_WAIT_CLAMPER_OPEN,
	CUELITE_OFFLOAD_STATE_WAIT_CLAMPER_CLOSE,
	CUELITE_OFFLOAD_STATE_WAIT_PULLY_TO_LIMIT,
	CUELITE_OFFLOAD_STATE_WAIT_PULLY_TO_HOME,
	CUELITE_OFFLOAD_STATE_WAIT_LIFTING,
	CUELITE_OFFLOAD_STATE_WAIT_CYLINDER_RELEASE,
	CUELITE_OFFLOAD_STATE_OFFLOAD_END, 
	CUELITE_OFFLOAD_STATE_OFFLOAD_SCUELITE,
};

//v2.20.Q1 add
typedef enum enCuEliteOffloadDryRunState
{
	CUELITE_OFFLOAD_DRYRUN_IDLE,
	CUELITE_OFFLOAD_DRYRUN_TESTSTART,
	CUELITE_OFFLOAD_DRYRUN_WAIT_ALL_HOME,
	CUELITE_OFFLOAD_DRYRUN_WAIT_TO_1ST_SLOT,
	CUELITE_OFFLOAD_DRYRUN_WAIT_PUSHER,
	CUELITE_OFFLOAD_DRYRUN_WAIT_NEXT_SLOT_POSITION,
	CUELITE_OFFLOAD_DRYRUN_WAIT_CLAMPER_NOMAL,
	CUELITE_OFFLOAD_DRYRUN_WAIT_CLAMPER_CLAMP,
	CUELITE_OFFLOAD_DRYRUN_WAIT_TO_BOTTOM,
	CUELITE_OFFLOAD_DRYRUN_WAIT_PULLY_PUSHING,
	CUELITE_OFFLOAD_DRYRUN_WAIT_LIFTING,
	CUELITE_OFFLOAD_DRYRUN_WAIT_CLAMPER_RELEASE,
	CUELITE_OFFLOAD_DRYRUN_INJECT_FINISH,
	CUELITE_OFFLOAD_DRYRUN_END, 
};

class CCuEliteOffload : public CObject
{
public:
	CCuEliteOffload();
	virtual ~CCuEliteOffload();
public:	//functions
	void Initialize(void);
	void SubmitOffloadEvent(short nEvent);
	void SubmitOffloaderDryRunEvent(short nEvent); //v2.20.Q1 add
	void ProcessOffloadEvent(void);
	void ProcessOffloaderDryRunEvent(void); //v2.20.Q1 add
	CDcMotor theMagPullyDcMotor;
	CSensor theMagDetectedSensor;
	CSensor thePullyMagExistSensor;
	CSensor theMagDropSensor;               // v2.18.1.S6 add
	CSensor theForkRemoveDetectedSensor;    // v2.18.1.S6 add
	CSensor theFork1PresentSensor;			//	v2.18.1.S6 add
	CSensor theFork2PresentSensor;			// v2.18.1.S6 add
	CSensor theMagBufferPresentSensor;	    // v2.18.1.S6 add
	CSensor theMagPullyHome;      //v2.20.Q1 add
	CSensor theMagPullyLimit;     //v2.20.Q1 add
	// Step motor id of offload holder
	byte m_ucForkHolderStepId;
	// Step motor id of offload clamper Front
	byte m_ucMagClamperFrontStepId;
	// Step motor id of offload clamper Back
	byte m_ucMagClamperBackStepId;
	// Step motor id of offload Length
	byte m_ucMagLengthStepId;
	// Step motor id of Offload Pully
	byte m_ucMagPullyStepId;                    //v2.20.Q1 add
	short GetOffloadEvent(void);
	void SearchOffloadElvStepHome(void);
	void WaitStepHomeFinish(void);
	void SearchOffloadMagClamperFrontStepHome(void);
	void WaitOffloadMagClamperFrontStepHomeFinish(void);
	short GetOffloadErrorCode(void);
	void ResetOffloadErrorCode(void);
	// Get offload valid state
	BOOL GetOffloadValidState(void);
	// Set fork offload valid flag
	void SetForkOffloadValid(BOOL bValid);
	void SetCurrentSlotNum(short nSlot);
	short GetCurrentSlotNum(void);
	void MoveTotheSlot(short nSlotIndex);
	void MoveTotheSlot(short nSlotIndex, bool bUpdate_m_nCurrentSlot);
	void SaveOffloadRcd(void);
	void LoadOffloadRcd(void);
	void GetCuEliteOffloadRcd(CUELITE_OFFLOAD_RCD* stOffloadRcd);
	void UpdateCuEliteOffloadRcd(CUELITE_OFFLOAD_RCD* stOffloadRcd);
	void ResetOffloadEvent(void);
	void CaculateOffsetToBase(long* lOffsetToBase, short nCurrentSlot);
	void ResetOffsetRecord(void);
	void EjectOffloadMag(void);
	short m_nTestState;//v2.20.Q1 add
	int SetOffloadMagBuffFullFlag(bool bFlag);
	// Relative move to last slot of magazine
	void RelativeMoveToLastSlot(void);
	void ActiveOffloadJamHandleEvent(void);
	BOOL GetOffloadJamHandleFlag(void);
	bool GetOffloadEmptyflag(void);
	void SetOffloadEmptyflag(bool bflag);
	bool GetOffloadEmptyReported(void);
	void SetOffloadEmptyReported(bool bflag);
	bool GetOffloadBufferEmptyflag(void);
	void SetOffloadBufferEmptyflag(bool bflag);
	bool GetOffloadBufferEmptyReported(void);
	void SetOffloadBufferEmptyReported(bool bflag);
	bool GetOffloadDrogflag(void);
	void SetOffloadDrogflag(bool bflag);
	bool GetOffloadDrogReported(void);
	void SetOffloadDrogReported(bool bflag);
	bool GetOffloadPullyFullflag(void);
	void SetOffloadPullyFullflag(bool bflag);
	bool GetOffloadPullyFullReported(void);
	void SetOffloadPullyFullReported(bool bflag);
	void moveMagClamperStepForward();	// v2.18.1.S6 add
	void moveMagClamperStepBackward();	// v2.18.1.S6 add
	bool m_bIsTriggerClamperPositon; //v2.20.Q1 add
	void moveMagClamperOpen();//v2.20.Q1 add
	int UpdateAllSensor();//v2.20.Q1 add
	int UpdateRelayState();//v2.20.Q1 add
	int GetSensorState(ST_IO_CONFIG Sensor);//v2.20.Q1 add
	void WaitMagClamperMoveFinish();//v2.20.Q1 add
	void WaitMagHoledrMoveFinish();//v2.20.Q1 add
	void OffLoadAllHome();//v2.20.Q5 add
	void SearchOffloadMagClamperBackStepHome(void);
	void WaitOffloadMagClamperBackStepHomeFinish(void);
	void SearchOffloadMagLengthStepHome(void);
	void WaitOffloadMagLengthStepHomeFinish(void);
	void OffloadMagClamperFrontMovetoStandyPos();
	void OffloadMagClamperFrontStop();
	void OffloadMagClamperBackMovetoStandyPos();
	void OffloadMagClamperBackStop();
	void OffloadMagLengthMovetoStandyPos();
	void OffloadMagLengthStop();
	void SearchOffloadElevetorLimit(void);
	int SetOffloadMagLenLimit();
	CHTimer m_DelayTimer;
	long m_lOffloadMagLenLimit;
private:
	long m_lStandyPos;
	long m_lSlotHeight;
	long m_lOffloadStartTime;
	short m_nOffloadEvent;
	short m_nOffloadState;
	short m_nOffloadErrorCode;
	BOOL m_bOffloadValid;
	short m_nCurrentSlot;
	bool m_bChangeMagazine; //v3.3.D185
	long m_lClawDelayTime;
	// Offload holder step speed profile
	STEP_SPEED_PROFILE m_stForkHolderSpeedProfile;
	ST_AXIS_PARA m_stForkHolderProfile;   //v2.20.Q1 add
	// Offload clamper step speed profile
	STEP_SPEED_PROFILE m_stMagClamperFrontStepSpeedProfile;
	ST_AXIS_PARA m_stMagClamperFrontProfile; //v2.20.Q1 add
	// Offload clamper step speed profile
	STEP_SPEED_PROFILE m_stMagClamperBackStepSpeedProfile;
	ST_AXIS_PARA m_stMagClamperBackProfile; //v2.20.Q1 add
	// Offload clamper step speed profile
	STEP_SPEED_PROFILE m_stMagLengthStepSpeedProfile;
	ST_AXIS_PARA m_stMagLengthProfile; //v2.20.Q1 add
	// Offload Pully step speed Profile
	ST_AXIS_PARA m_stMagPullyProfile;	//v2.20.Q1 add
	short m_nLastOffloadDebugState;
	CUELITE_OFFLOAD_RCD m_stOffloadRcd;
	BOOL m_bOffloadJamNotHandled;
	bool m_bOffloadBufferEmptyflag;
	bool m_bOffloadEmptyflag;
	bool m_bOffloadEmptyReported;
	bool m_bOffloadBufferEmptyReported;
	bool m_bOffloadDrogflag;
	bool m_bOffloadDrogReported;
	bool m_bOffloadPullyFullflag;
	bool m_bOffloadPullyFullReported;
};

extern CCuEliteOffload theCuEliteOffloader;