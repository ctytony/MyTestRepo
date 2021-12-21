//TopOffload.h v1.0.T25

#pragma once

#include "DcMotor.h"
#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"

#define OFFLOAD_START_DELAY   200	//ms

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct TOP_OFFLOAD_RCD
{
	long        lSlotOffset[TOP_MAX_MAG_SLOT];
	long        lSlotPitch;
	long        lSwapMagPitch;
	long        lOffload1stSlotLevel;	//190000
	long        lOffloadSlotNum;
	char        chReserve[24];			//28->24

    //must init before use, or by default as 0
	TOP_OFFLOAD_RCD():
		lSlotOffset(),
		lSlotPitch(),
		lSwapMagPitch(),
		lOffload1stSlotLevel(),
		lOffloadSlotNum(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CTopOffload command target

//enum enTopOffloadEvent	//v1.0.T13 delete
enum EN_TOP_OFFLOAD_EVENT	//v1.0.T13 add
{
    TOP_OFFLOAD_EVENT_IDLE, 
    TOP_OFFLOAD_EVENT_START,
	TOP_OFFLOAD_EVENT_INIT,
	TOP_OFFLOAD_EVENT_NORMAL,
	TOP_OFFLOAD_EVENT_LAST,
};

//enum enTopOffloadState	//v1.0.T13 delete
enum EN_TOP_OFFLOAD_STATE	//v1.0.T13 add
{
    TOP_OFFLOAD_STATE_IDLE,
	TOP_OFFLOAD_STATE_WAIT_START_DELAY,		// WAIT INDEXER MOVE AWAY
	TOP_OFFLOAD_STATE_WAIT_TO_1ST_SLOT,
	TOP_OFFLOAD_STATE_WAIT_NEXT_SLOT_POSITION,
	TOP_OFFLOAD_STATE_WAIT_CYLINDER_CLAMP,	//==TOTAL SLOT
	TOP_OFFLOAD_STATE_WAIT_TO_BOTTOM,
    TOP_OFFLOAD_STATE_WAIT_PULLY_PUSHING,
	TOP_OFFLOAD_STATE_WAIT_CLAMPER_OPEN,
	TOP_OFFLOAD_STATE_WAIT_CLAMPER_CLOSE,
	TOP_OFFLOAD_STATE_WAIT_PULLY_TO_LIMIT,
	TOP_OFFLOAD_STATE_WAIT_PULLY_TO_HOME,
	TOP_OFFLOAD_STATE_WAIT_LIFTING,
	TOP_OFFLOAD_STATE_WAIT_CYLINDER_RELEASE,
	TOP_OFFLOAD_STATE_END, 
	TOP_OFFLOAD_STATE_STOP,
};

//enum enTopOffloadDryRunState		//v1.0.T13 delete
enum EN_TOP_OFFLOAD_DRYRUN_STATE	//v1.0.T13 add
{
	TOP_OFFLOAD_DRYRUN_IDLE,
	TOP_OFFLOAD_DRYRUN_TESTSTART,
	TOP_OFFLOAD_DRYRUN_WAIT_ALL_HOME,
	TOP_OFFLOAD_DRYRUN_WAIT_TO_1ST_SLOT,
	TOP_OFFLOAD_DRYRUN_WAIT_PUSHER,
	TOP_OFFLOAD_DRYRUN_WAIT_NEXT_SLOT_POSITION,
	TOP_OFFLOAD_DRYRUN_WAIT_CLAMPER_NOMAL,
	TOP_OFFLOAD_DRYRUN_WAIT_CLAMPER_CLAMP,
	TOP_OFFLOAD_DRYRUN_WAIT_TO_BOTTOM,
	TOP_OFFLOAD_DRYRUN_WAIT_PULLY_PUSHING,
	TOP_OFFLOAD_DRYRUN_WAIT_LIFTING,
	TOP_OFFLOAD_DRYRUN_WAIT_CLAMPER_RELEASE,
	TOP_OFFLOAD_DRYRUN_INJECT_FINISH,
	TOP_OFFLOAD_DRYRUN_END, 
};

class CTopOffload : public CObject
{
public:
	CTopOffload();
	virtual ~CTopOffload();
public:	//variables
	CDcMotor m_MagPullyDcMotor;
	CSensor m_MagDetectedSensor;
	CSensor m_PullyMagExistSensor;
    CSensor m_MagDropSensor;
    CSensor m_ForkRemoveDetectedSensor;
    CSensor m_Fork1PresentSensor;
    CSensor m_Fork2PresentSensor;
    CSensor m_MagBufferPresentSensor;
	byte m_nMagHolderStepperId;
	byte m_nMagClamperStepperId;
	short m_nTestState;
	CHTimer m_DelayTimer;
public:	//functions
	void VerifyData();	//v1.0.T16 add
	bool IsPusherFullReported(void);
	void SetPusherFullReported(bool bReported);
	void MoveMagClamperForward();
    void MoveMagClamperBackward();
    void SearchMoveMagClamperToClampPos();
    void StopMagClamper();
    void MoveMagClamperToClampPos();
	void OpenMagClamper();
	int UpdateAllSensor();
	int UpdateRelayState();
	int GetSensorState(ST_IO_CONFIG Sensor);
	void WaitMoveMagClamper();
	void WaitMoveMagHolder();
	void HomeAll();
	bool IsPusherFull(void);
	void SetPusherFull(bool bFull);
	bool IsDropReported(void);
	void SetDropReported(bool bReported);
	bool IsDrop(void);
	void SetDrop(bool bDrop);
	bool IsBufferEmptyReported(void);
	void SetBufferEmptyReported(bool bReported);
	bool IsBufferEmpty(void);
	void SetBufferEmpty(bool bEmpty);
	bool IsEmptyReported(void);
	void SetEmptyReported(bool bReported);
	bool IsEmpty(void);
	void SetEmpty(bool bEmpty);
	void SetJamNotHandled(bool bNotHandled);
	bool IsJamNotHandled(void);
	void EjectOffloadMag(void);
	void MoveToLastSlot(void);
	void MoveMagHolderToSlot(short nSlot);
	void SaveRcd(void);
	void LoadRcd(void);
	void GetRcd(TOP_OFFLOAD_RCD *stRcd);
	void SetRcd(TOP_OFFLOAD_RCD *stRcd);
	void ResetEvent(void);
	long CalcOffsetToBase(short nCurSlot);
	void ResetSlotOffset(void);
	short GetEvent(void);
	void HomeMagHolder(void);
	void WaitHomeMagHolder(void);
    void HomeMagClamper(void);
    void WaitHomeMagClamper(void);
	short GetErrorCode(void);
	void ResetErrorCode(void);
    bool IsValid(void);
    void SetValid(bool bValid);
	void SetCurSlot(short nSlot);
	short GetCurSlot(void);
	void Init(void);
	void SubmitOffloadEvent(short nEvent);
	void SubmitDryRunEvent(short nEvent);
	void ProcessOffloadEvent(void);
	void ProcessDryRunEvent(void);
private:	//variables
	CSensor m_MagPusherHome;
	CSensor m_MagPusherLimit;
	byte m_nMagPusherStepperId;
	bool m_bClampPos;
	short m_nEvent;
	short m_nState;
	short m_nErrorCode;
	bool m_bValid;
	short m_nCurSlot;
	STEP_SPEED_PROFILE m_stMagHolderSpeedProfile;
	ST_AXIS_PARA m_stMagHolderAxisParam;
	STEP_SPEED_PROFILE m_stMagClamperSpeedProfile;
	ST_AXIS_PARA m_stMagClamperAxisParam;
	ST_AXIS_PARA m_stMagPusherAxisParam;
	short m_nLastDebugState;
	TOP_OFFLOAD_RCD m_stOffloadRcd;
	long m_lStandbyPos;
	long m_lSlotHeight;
	bool m_bJamNotHandled;
	bool m_bEmpty;
	bool m_bEmptyReported;
	bool m_bBufferEmpty;
	bool m_bBufferEmptyReported;
	bool m_bDrop;
	bool m_bDropReported;
	bool m_bPusherFull;
	bool m_bPusherFullReported;
};

extern CTopOffload theTopOffloader;
