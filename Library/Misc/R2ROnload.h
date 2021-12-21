//R2ROnload.h v1.0.T18

#pragma once

#include "DcMotor.h"
#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"		//v2.18.6.T41 add //v2.20.Q1 edit
#include "TrackSensor.h"
#include "TopOnload.h"

class CR2ROnload : public CObject
{
public:
	CR2ROnload();

	virtual ~CR2ROnload();

	int Initialize(void);

	int SubmitOnloadEvent(short nEvent);

	int SubmitOnloaderDryRunEvent(short nEvent); //v2.20.Q1 add

	int ProcessOnloadEvent(void);

	int ProcessOnloaderDryRunEvent(void); //v2.20.Q1 add

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
	CSensor theInjectSensor;      //v2.20.Q1 add

    CSensor theStopBondSnr; //v2.20.w2
    CSensor theErrorSnr; //v2.20.w2


	// Step motor id of onload holder
	byte m_ucForkHolderStepId;

	// Step motor id of onload clamper
	byte m_ucMagClamperStepId;

	// Step motor id of Onload Pully
	byte m_ucMagPullyStepId;                    //v2.20.Q1 add

	int SetOnloadErrorCode(short nErrorCode); //v2.20.w1

	
private:

	short m_nOnloadEvent;

	short m_nOnloadErrorCode;

	short m_nOnloadState;

	BOOL m_bOnloadValid;

	short m_nCurrentSlot;   // =0:no magzine; =1 first slot ; =20 20th slot

	short m_nLastOnloadDebugState;

	
    // Onload holder step speed profile
    STEP_SPEED_PROFILE m_stForkHolderSpeedProfile;
	ST_AXIS_PARA m_stForkHolderProfile;   //v2.20.Q1 add

    
    // Onload clamper step speed profile
    STEP_SPEED_PROFILE m_stMagClamperStepSpeedProfile;
	ST_AXIS_PARA m_stMagClamperProfile; //v2.20.Q1 add

	
	// Onload Pully step speed Profile
	ST_AXIS_PARA m_stMagPullyProfile;//v2.20.Q1 add

	TOP_ONLOAD_RCD m_stOnloadRcd;



public:

	short GetOnloadEvent(void);

	int SearchOnloadElvStepHome(void);

	int WaitStepHomeFinish(void);
    int SearchOnloadMagClamperStepHome(void);
    int WaitOnloadMagClamperStepHomeFinish(void);

	short GetOnloadErrorCode(void);

	int ResetOnloadErrorCode(void);

	
private:
	long m_lStandyPos;

	long m_lSlotHeight;

	bool m_bOnloadReady;


public:
	int SetCurrentSlotNum(short nSlot);

	short GetCurrentSlotNum(void);
	 // Set onload valid flag
    int SetOnloadValid(BOOL bOnloadValid);
    // Get onload valid flag
    BOOL GetOnloadValidState(void);

	int MoveTotheSlot(short nSlotIndex);

	int SaveOnloadRcd(void);

	int LoadOnloadRcd(void);

	int GetTopOnloadRcd(TOP_ONLOAD_RCD* stOnloadRcd);

	int UpdateTopOnloadRcd(TOP_ONLOAD_RCD* stOnloadRcd);

	int ResetOnloadEvent(void);

	int CaculateOffsetToBase(long* lOffsetToBase, short nCurrentSlot);

	int ResetOffsetRecord(void);

	int SetOnloadReadyFlag(bool bFlag);

	bool GetOnloadReadyFlag(void);

	int EjectOnloadMag(void);

	short m_nTestState; //v2.20.Q1 add

	private:
		bool m_bOnloadEmptyflag;
public:
	bool GetOnloadEmptyflag(void);
	void SetOnloadEmptyflag(bool bflag);
private:
	bool m_bOnloadEmptyReported;
public:
	bool GetOnloadEmptyReported(void);
	void SetOnloadEmptyReported(bool bflag);
private:
	bool m_bOnloadBufferEmptyflag;
public:
	bool GetOnloadBufferEmptyflag(void);
	void SetOnloadBufferEmptyflag(bool bflag);
private:
	bool m_bOnloadBufferEmptyReported;
public:
	bool GetOnloadBufferEmptyReported(void);
	void SetOnloadBufferEmptyReported(bool bflag);
private:
	bool m_bOnloadErrorflag;
public:
	bool GetOnloadErrorflag(void);
	void SetOnloadErrorflag(bool bflag);
private:
	bool m_bOnloadErrorReported;
public:
	bool GetOnloadErrorReported(void);
	void SetOnloadErrorReported(bool bflag);
private:
	bool m_bOnloadPullyFullflag;
public:
	bool GetOnloadPullyFullflag(void);
	void SetOnloadPullyFullflag(bool bflag);
private:
	bool m_bOnloadPullyFullReported;
public:
	bool GetOnloadPullyFullReported(void);
	void SetOnloadPullyFullReported(bool bflag);

//	int ResetOnloadMagBuffFullFlag(void);    // v2.18.1.S6  delete
//	bool GetOnloadMagBuffFullFlag(void);
//	bool m_bOnloadFullMsgReported;
//private:
//	bool m_bOnloadFullMsg;
//public:
//	int SetOnloadMagBuffFullFlag(bool bFlag);
private:
	short m_nOnloaderRetryTimes;
public:
    // Relative move to last LF of magazine.
    int RelativeMoveToLastSlot(void);
private:
	BOOL m_bOnloadAfterInit;
public:
	int ResetOnloadInitStatus(void);
	BOOL GetOnloaderInitStatus(void);
    void moveMagClamperStepForward();
    void moveMagClamperStepBackward();
    void OnLoadMagClamperStepperMovetoClampPosition();  //v2.18.2.S5 add
    void OnLoadMagClamperStepperStop();  //v2.18.2.S5 add
    void OnLoadMagClamperMovetoClampPosition();  //v2.18.2.S7 add

	bool m_bIsTriggerClamperPositon; //v2.20.Q1 add
	void moveMagClamperOpen();//v2.20.Q1 add
	int UpdateAllSensor();//v2.20.Q1 add
	int UpdateRelayState();//v2.20.Q1 add
	int GetSensorState(ST_IO_CONFIG Sensor);//v2.20.Q1 add
	int WaitMagClamperMoveFinish();//v2.20.Q1 add
	int WaitMagHoledrMoveFinish();//v2.20.Q1 add
	void OnLoadAllHome();//v2.20.Q5 add
	CHTimer m_DelayTimer;
};

extern CR2ROnload theR2ROnloader;	//v3.3.T160 add
