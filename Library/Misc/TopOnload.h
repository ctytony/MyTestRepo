//TopOnload.h v1.0.T24

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

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct TOP_ONLOAD_RCD
{
	long        lSlotOffset[TOP_MAX_MAG_SLOT];
	long        lSlotPitch;
	long        lSwapMagPitch;
	long        lOnload1stSlotLevel;	//190000
	long        lOnloadSlotNum;
	char        chReserve[24];			//28->24

    //must init before use, or by default as 0
	TOP_ONLOAD_RCD():
		lSlotOffset(),
		lSlotPitch(),
		lSwapMagPitch(),
		lOnload1stSlotLevel(),
		lOnloadSlotNum(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CTopOnload command target

//enum enTopOnloadEvent		//v1.0.T13 delete
enum EN_TOP_ONLOAD_EVENT	//v1.0.T13 add
{
    TOP_ONLOAD_EVENT_IDLE, 
    TOP_ONLOAD_EVENT_START,
	TOP_ONLOAD_EVENT_INIT,
	TOP_ONLOAD_EVENT_NORMAL,
	TOP_ONLOAD_EVENT_LAST,
	TOP_ONLOAD_EVENT_FIRST,
};

//enum enTopOnloadState		//v1.0.T13 delete
enum EN_TOP_ONLOAD_STATE	//v1.0.T13 add
{
    TOP_ONLOAD_STATE_IDLE,
	TOP_ONLOAD_STATE_WAIT_TO_1ST_SLOT,
	TOP_ONLOAD_STATE_WAIT_PUSHER,
	TOP_ONLOAD_STATE_WAIT_NEXT_SLOT_POSITION,
	TOP_ONLOAD_STATE_WAIT_CYLINDER_CLAMP,	//==TOTAL SLOT
	TOP_ONLOAD_STATE_WAIT_TO_BOTTOM,
    TOP_ONLOAD_STATE_WAIT_PULLY_PUSHING,
	TOP_ONLOAD_STATE_WAIT_PULLY_TO_LIMIT,
	TOP_ONLOAD_STATE_WAIT_PULLY_TO_HOME,
	TOP_ONLOAD_STATE_WAIT_LIFTING,
	TOP_ONLOAD_STATE_WAIT_CYLINDER_RELEASE,
    TOP_ONLOAD_STATE_WAIT_INJECT_FINISH,
	TOP_ONLOAD_STATE_END, 
};

//enum enTopOnloadDryRunState	//v1.0.T13 delete
enum EN_TOP_ONLOAD_DRYRUN_STATE	//v1.0.T13 add
{
	TOP_ONLOAD_DRYRUN_IDLE,
	TOP_ONLOAD_DRYRUN_TESTSTART,
	TOP_ONLOAD_DRYRUN_ALL_HOME,
	TOP_ONLOAD_DRYRUN_WAIT_ALL_HOME,
	TOP_ONLOAD_DRYRUN_WAIT_TO_1ST_SLOT,
	TOP_ONLOAD_DRYRUN_WAIT_PUSHER,
	TOP_ONLOAD_DRYRUN_WAIT_NEXT_SLOT_POSITION,
	TOP_ONLOAD_DRYRUN_WAIT_CLAMPER_NOMAL,
	TOP_ONLOAD_DRYRUN_WAIT_CLAMPER_CLAMP,
	TOP_ONLOAD_DRYRUN_WAIT_TO_BOTTOM,
	TOP_ONLOAD_DRYRUN_WAIT_PULLY_PUSHING,
	TOP_ONLOAD_DRYRUN_WAIT_LIFTING,
	TOP_ONLOAD_DRYRUN_WAIT_CLAMPER_RELEASE,
	TOP_ONLOAD_DRYRUN_INJECT_FINISH,
	TOP_ONLOAD_DRYRUN_END, 
};

class CTopOnload : public CObject
{
public:
	CTopOnload();
	virtual ~CTopOnload();
public:	//variables
	CDcMotor m_InjectPusher;
	CDcMotor m_MagPullyDcMotor;
	CSensor m_MagDetectedSensor;
	CSensor m_PullyMagExistSensor;
    CSensor m_MagDropSensor;
    CSensor m_ForkRemoveDetectedSensor;
    CSensor m_Fork1PresentSensor;
    CSensor m_Fork2PresentSensor;
    CSensor m_MagBufferPresentSensor;
	byte m_nMagHolderStepperId;		//Z axis
	byte m_nMagClamperStepperId;	//Y axis
	short m_nTestState;
	CHTimer m_DelayTimer;
	bool m_bClampPos;
public:	//functions
	void VerifyData();	//v1.0.T15 add
	void ResetAfterInit(void);
	bool IsAfterInit(void);
    void MoveMagClamperForward();
    void MoveMagClamperBackward();
    void SearchMoveMagClamperToClampPos();
    void StopMagClamper();
    void MoveMagClamperToClampPos();
	void SetFirstIndex(bool bFirstSuccess, bool bAutoIndex);
	void OpenMagClamper();
	int UpdateAllSensor();
	int UpdateRelayState();
	int GetSensorState(ST_IO_CONFIG Sensor);
	void WaitMoveMagClamper();
	void WaitMoveMagHolder();
	void HomeAll();
	bool IsPusherFullReported(void);
	void SetPusherFullReported(bool bReported);
	bool IsDropReported(void);
	void SetDropReported(bool bReported);
	bool IsDrop(void);
	void SetDrop(bool bDrop);
	bool IsPusherFull(void);
	void SetPusherFull(bool bFull);
	bool IsBufferEmptyReported(void);
	void SetBufferEmptyReported(bool bReported);
	bool IsBufferEmpty(void);
	void SetBufferEmpty(bool bEmpty);
	bool IsEmptyReported(void);
	void SetEmptyReported(bool bReported);
	bool IsEmpty(void);
	void SetEmpty(bool bEmpty);
	void SetCurSlot(short nSlot);
	short GetCurSlot(void);
    void SetValid(bool bValid);
    bool IsValid(void);
	void MoveMagHolderToSlot(short nSlot);
	void SaveRcd(void);
	void LoadRcd(void);
	void GetRcd(TOP_ONLOAD_RCD *stRcd);
	void SetRcd(TOP_ONLOAD_RCD *stRcd);
	void ResetEvent(void);
	void ResetSlotOffset(void);
	int EjectOnloadMag(void);
	void Init(void);
	void SubmitOnloadEvent(short nEvent);
	void SubmitDryRunEvent(short nEvent);
	void ProcessOnloadEvent(void);
	void ProcessDryRunEvent(void);
	short GetEvent(void);
	void HomeMagHolder(void);
	void WaitHomeMagHolder(void);
    void HomeMagClamper(void);
    void WaitHomeMagClamper(void);
	short GetErrorCode(void);
	void ResetErrorCode(void);
private:	//variables
	CSensor m_MagPusherHome;
	CSensor m_MagPusherLimit;
	byte m_nMagPusherStepperId;
	short m_nEvent;
	short m_nErrorCode;
	short m_nState;
	bool m_bValid;
	short m_nCurSlot;	//0: no magzine, 1: first slot, 20: 20th slot
	short m_nLastDebugState;
    STEP_SPEED_PROFILE m_stMagHolderSpeedProfile;
	ST_AXIS_PARA m_stMagHolderAxisParam;
    STEP_SPEED_PROFILE m_stMagClamperSpeedProfile;
	ST_AXIS_PARA m_stMagClamperAxisParam;
	ST_AXIS_PARA m_stMagPusherAxisParam;
	TOP_ONLOAD_RCD m_stOnloadRcd;
	long m_lStandyPos;
	long m_lSlotHeight;
	bool m_bEmpty;
	bool m_bEmptyReported;
	bool m_bBufferEmpty;
	bool m_bBufferEmptyReported;
	bool m_bDrop;
	bool m_bDropReported;
	bool m_bPusherFull;
	bool m_bPusherFullReported;
	short m_nRetryTime;
	bool m_bAfterInit;
	bool m_bFirstSuccess;
	bool m_bAutoIndex;
private:	//functions
    void MoveToLastSlot(void);
	long CalcOffsetToBase(short nCurSlot);
};

extern CTopOnload theTopOnloader;
