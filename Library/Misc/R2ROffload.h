//R2ROffload.h v1.0.T18

#pragma once

#include "DcMotor.h"
#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"   //v2.20.Q1 edit	//v2.18.6.T41 add
#include "TrackSensor.h"
#include "TopOffload.h"

class CR2ROffload : public CObject
{
public:

	CR2ROffload();

	virtual ~CR2ROffload();

	int Initialize(void);

	int SubmitOffloadEvent(short nEvent);

	int SubmitOffloaderDryRunEvent(short nEvent); //v2.20.Q1 add

	int ProcessOffloadEvent(void);

	int ProcessOffloaderDryRunEvent(void); //v2.20.Q1 add

	

	CDcMotor theMagPullyDcMotor;

//CCylinder theMagClampCylinder;		//v2.18.1.S6 delete

	CSensor theMagDetectedSensor;

	CSensor thePullyMagExistSensor;
    CSensor theMagDropSensor;               // v2.18.1.S6 add
    CSensor theForkRemoveDetectedSensor;    // v2.18.1.S6 add
    CSensor theFork1PresentSensor;			//	v2.18.1.S6 add
    CSensor theFork2PresentSensor;			// v2.18.1.S6 add
    CSensor theMagBufferPresentSensor;	    // v2.18.1.S6 add

	CSensor theEjectSensor;       //v2.20.Q1 add
	CSensor theMagPullyHome;      //v2.20.Q1 add
	CSensor theMagPullyLimit;     //v2.20.Q1 add

    CSensor theStopBondSnr; //v2.20.w2
    CSensor theErrorSnr; //v2.20.w2

	// Step motor id of offload holder
	byte m_ucForkHolderStepId;

	// Step motor id of offload clamper
	byte m_ucMagClamperStepId;

	// Step motor id of Offload Pully
	byte m_ucMagPullyStepId;                    //v2.20.Q1 add

	int SetOffloadErrorCode(short nErrorCode); //v2.20.w1

private:

	short m_nOffloadEvent;

	short m_nOffloadState;

	short m_nOffloadErrorCode;
	
	BOOL m_bOffloadValid;

	short m_nCurrentSlot;

	// Offload holder step speed profile
	STEP_SPEED_PROFILE m_stForkHolderSpeedProfile;
	ST_AXIS_PARA m_stForkHolderProfile;   //v2.20.Q1 add

	
	// Offload clamper step speed profile
	STEP_SPEED_PROFILE m_stMagClamperStepSpeedProfile;
	ST_AXIS_PARA m_stMagClamperProfile; //v2.20.Q1 add

	
	// Offload Pully step speed Profile
	ST_AXIS_PARA m_stMagPullyProfile;//v2.20.Q1 add

	short m_nLastOffloadDebugState;

	TOP_OFFLOAD_RCD m_stOffloadRcd;

public:
	short GetOffloadEvent(void);

	int SearchOffloadElvStepHome(void);

	int WaitStepHomeFinish(void);
    int SearchOffloadMagClamperStepHome(void);
    int WaitOffloadMagClamperStepHomeFinish(void);

	short GetOffloadErrorCode(void);

	int ResetOffloadErrorCode(void);
	   // Get offload valid state
    BOOL GetOffloadValidState(void);
    // Set fork offload valid flag
    int SetForkOffloadValid(BOOL bValid);

	int SetCurrentSlotNum(short nSlot);

	short GetCurrentSlotNum(void);


private:
	long m_lStandyPos;
	long m_lSlotHeight;
public:
	int MoveTotheSlot(short nSlotIndex);

	int SaveOffloadRcd(void);

	int LoadOffloadRcd(void);

	int GetTopOffloadRcd(TOP_OFFLOAD_RCD* stOffloadRcd);

	int UpdateTopOffloadRcd(TOP_OFFLOAD_RCD* stOffloadRcd);

	int ResetOffloadEvent(void);

	int CaculateOffsetToBase(long* lOffsetToBase, short nCurrentSlot);
	int ResetOffsetRecord(void);
private:
	long m_lOffloadStartTime;
public:
	int EjectOffloadMag(void);

	short m_nTestState;//v2.20.Q1 add


//v2.18.1.S6 delete
//	int ResetOffloadMagBuffFullFlag(void);
//	bool GetOffloadMagBuffFullFlag(void);
//	bool m_bOffloadFullMsgReported;
//private:
//	bool m_bOffloadFullMsg;
public:
	int SetOffloadMagBuffFullFlag(bool bFlag);
	// Relative move to last slot of magazine
	int RelativeMoveToLastSlot(void);
private:
	BOOL m_bOffloadJamNotHandled;
public:
	int ActiveOffloadJamHandleEvent(void);
	BOOL GetOffloadJamHandleFlag(void);
private:
	bool m_bOffloadEmptyflag;
public:
	bool GetOffloadEmptyflag(void);
	void SetOffloadEmptyflag(bool bflag);
private:
	bool m_bOffloadEmptyReported;
public:
	bool GetOffloadEmptyReported(void);
	void SetOffloadEmptyReported(bool bflag);
private:
	bool m_bOffloadBufferEmptyflag;
public:
	bool GetOffloadBufferEmptyflag(void);
	void SetOffloadBufferEmptyflag(bool bflag);
private:
	bool m_bOffloadBufferEmptyReported;
public:
	bool GetOffloadBufferEmptyReported(void);
	void SetOffloadBufferEmptyReported(bool bflag);
private:
	bool m_bOffloadErrorflag;
public:
	bool GetOffloadErrorflag(void);
	void SetOffloadErrorflag(bool bflag);
private:
	bool m_bOffloadErrorReported;
public:
	bool GetOffloadErrorReported(void);
	void SetOffloadErrorReported(bool bflag);
private:
	bool m_bOffloadPullyFullflag;
public:
	bool GetOffloadPullyFullflag(void);
	void SetOffloadPullyFullflag(bool bflag);
private:
	bool m_bOffloadPullyFullReported;
public:
	bool GetOffloadPullyFullReported(void);
	void SetOffloadPullyFullReported(bool bflag);


	void moveMagClamperStepForward();	// v2.18.1.S6 add
    void moveMagClamperStepBackward();	// v2.18.1.S6 add
    void OffLoadMagClamperStepperMovetoClampPosition();  //v2.18.2.S5 add
    void OffLoadMagClamperStepperStop();  //v2.18.2.S5 add
    void OffLoadMagClamperMovetoClampPosition();  //v2.18.2.S7 add

	bool m_bIsTriggerClamperPositon; //v2.20.Q1 add
	void moveMagClamperOpen();//v2.20.Q1 add
	int UpdateAllSensor();//v2.20.Q1 add
	int UpdateRelayState();//v2.20.Q1 add
	int GetSensorState(ST_IO_CONFIG Sensor);//v2.20.Q1 add
	int WaitMagClamperMoveFinish();//v2.20.Q1 add
	int WaitMagHoledrMoveFinish();//v2.20.Q1 add
	void OffLoadAllHome();//v2.20.Q5 add
	CHTimer m_DelayTimer;
};

extern CR2ROffload theR2ROffloader;		//v3.3.T160 add
