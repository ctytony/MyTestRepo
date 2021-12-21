//VEIndex.h v1.0.T6

#pragma once

#include "DataStruct.h" 
#include "Sensor.h"
#include "RollingClaw.h"
#include "TrackSensor.h"
#include "VLED.h"

// CVEliteTrack command target

class CVEliteTrack : public CObject
{
public:
	CVEliteTrack();
	virtual ~CVEliteTrack();

public:	//variables
	CRollingClaw theLeftClaw;
    CRollingClaw theRightClaw;
	CSensor theDbCheckSensor;
	CSensor theCapExistSensor;
	CTrackSensor theOnloadTrackSensor;
	CSensor thePreBondSensor;
	CSensor theOffloadTrackSensor;
	short m_nIndexState;
public:	//functions
	bool IsAvoidCrash(void);
	void SetAvoidCrash(bool bAvoidCrash);
	short GetInjectEvent(void);
    short GetInjectPosState(void);
    void SetInjectPosState(short nPosState);
    short GetInjectErrorCode(void);
    void ResetInjectErrorCode(void);
	short m_nBondUnitNum;	//0 = no lf
    // Get current bond area unit number
    short GetBondUnitNum(void);
    // Set current bond area unit number
    void SetBondUnitNum(short nUnitNum);
	short GetIndexEvent(void);
    // Get Index Cycle time during bonding one unit
	int GetIndexCycleTime(void);
	void ResetIndexEvent(void);
	bool IsTrackValid(void);
	void SetTrackValid(bool bTrackValid);
	short GetTrackErrorCode(void);
    void ResetTrackErrorCode(void);
	void ResetBondUnitNum(void);
	bool IsOffloadEnable(void);
	void SetOffloadEnable(bool bflag);
	void SubmitInjectEvent(short nEvent);
	void ProcessInjectEvent(void);
	void SubmitIndexEvent(short nEvent);
	void ProcessIndexEvent(void);
	int CheckHomeState(void);
	int SearchHome(void);
    //Check a fixed relationship between Right Claw, Left Claw and Bonding Item
	int CheckStepsCorrect(short nLeft, short nRight);
	void Init(void);
	void LoadRcd(void);
	void SaveRcd(void);
	void SetRcd(SINGLE_CAP_TRACK_RCD* pstRcd);
	void GetRcd(SINGLE_CAP_TRACK_RCD* pstRcd);
	void LastLeadframeHandle(short nLeadFrameUnits);
private:	//variables
	SINGLE_CAP_TRACK_RCD m_stTrackRecord;
	short m_nTrackErrorCode;
	STEP_SPEED_PROFILE m_stInjectSpeedProfile;
    short m_nInjectErrorCode;
    // Injector interval too short times
    short m_nFrequentInjectTimes;
    // Start time of inject event
    long m_lInjectEventStartTime;
	bool m_bTrackValid;
	BYTE  m_nInjectorId;
	short m_nInjectEvent;
	short m_nInjectState;
	short m_nIndexEvent;
	short m_nLeftSteps;
	short m_nRightSteps;
	bool m_bCatchUp;
	short m_nLastTrackDebugState;
	long m_lRollerDelayStart;
	long m_lClampDelayStartTime;
    //a time variable to save time
	long m_lTimingCheckTime;
	// Injector position state
    short m_nInjectPosState;
    // the Time during Index 1 unit
	long m_lCycleTime;
	short m_nErrorMsgOffset;
	short m_nCapMissOffset;
	bool m_bOffloadEnable;
	bool m_bAvoidCrash;
};

extern CVEliteTrack theVEliteTrack;
