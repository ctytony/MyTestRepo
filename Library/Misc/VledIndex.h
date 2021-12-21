//VledIndex.h v1.0.T6

#pragma once

#include "DataStruct.h" 
#include "..\Library\Tool\Sensor.h"
#include "..\Library\Tool\RollingClaw.h"
#include "..\Library\Tool\TrackSensor.h"
#include "VLED.h"	//v3.3.T9 add

// CSingleCapTrack command target

class CSingleCapTrack : public CObject
{
public:
	CSingleCapTrack();
	virtual ~CSingleCapTrack();
public:	//variables
	CRollingClaw theLeftClaw;
    CRollingClaw theRightClaw;
	CSensor theDbCheckSensor;
	CSensor theCapExistSensor;
	CTrackSensor theOnloadTrackSensor;
	CSensor thePreBondSensor;
	CSensor theOffloadTrackSensor;
	short m_nIndexState;
    // Bond area unit number: 0, no lf;
	short m_nBondUnitNum;
public:	//functions
	int Initialize(void);
	int LoadLfTrackRcd(void);
	int SaveLfTrackRcd(void);
	int UpdateLfTrackRcd(SINGLE_CAP_TRACK_RCD* pstLfTrackRcd);
	int GetLfTrackRcd(SINGLE_CAP_TRACK_RCD* pstLfTrackRcd);
	int SubmitInjectorEvent(short nEvent);
	int ProcessInjectorEvent(void);
	int SubmitIndexEvent(short nEvent);
	int ProcessIndexEvent(void);
	int CheckHomeState(void);
	int SearchHome(void);
    //Check a fixed relationship between Right Claw, Left Claw and Bonding Item
	int CheckStepsCorrect(short nLeft, short nRight);
	int GetInjectorEvent(void);
    // Get injector position state
    short GetInjectorPosState(void);
    // Set injector position state
    int SetInjectorPosState(short nPosState);
    // Get injector error code
    short GetInjectErrorCode(void);
    // Reset inject error code
    int ResetInjectErrorCode(void);
    // Get current bond area unit number
    short GetBondUnitNum(void);
    // Set current bond area unit number
    int SetBondUnitNum(short nUnitNum);
    // Get Index Event State
	int GetIndexEvent(void);
    // Get Index Cycle time during bonding one unit
	int GetIndexCycleTime(void);
    // Reset all the index and injector event
	int ResetIndexEvent(void);
    // Get Track Valid State 
	int GetTrackValidState(void);
    // Set Track Valid during Dry Run 
	int SetTrackValid(BOOL m_bTrackValid);
	// Get track error code
	short GetTrackErrorCode(void);
    // Reset track error code
    short ResetTrackErrorCode(void);
    // Rest Bond Counter to 1 
	int ResetBondingCounter(void);
	bool GetOffloadEnableFlag(void);
	int SetOffloadEnableFlag(bool bflag);
	bool GetAvoidCrashFlag(void);
	int SetAvoidCrashFlag(bool nFlag);
private:	//variables
	SINGLE_CAP_TRACK_RCD m_stTrackRecord;
	short m_nTrackErrorCode;
	STEP_SPEED_PROFILE m_stInjectSpeedProfile;
	// Injector error code
    short m_nInjectErrorCode;
    // Injector interval too short times
    short m_nFrequentInjectTimes;
    // Start time of inject event
    long m_lInjectEventStartTime;
	BOOL m_bTrackValidFlag;
	BYTE  m_nInjectorId;
	short m_nInjectEvent;
	short m_nInjectState;
	short m_nIndexEvent;
	short m_nLeftSteps;
	short m_nRightSteps;
	bool m_bCatchUpFlag;
	short m_nLastTrackDebugState;
	long m_lRollerDelayStart;
	long m_lClampDelayStartTime;
    //a time variable to save time
	long m_lTimingCheckTime;
	// Injector position state
    short m_nInjectPosState;
    // the Time during Index 1 unit
	long m_lCycleTime;
	short nErrorMsgOffset;
	short m_nCapMissOffset;
	bool m_bOffloadEnable;
	bool m_bAvoidCrashOn;
};

extern CSingleCapTrack theVledTrack;	//v3.0.T10 edit
