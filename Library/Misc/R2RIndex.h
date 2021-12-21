//R2RIndex.h v1.1.T12

#pragma once

#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"
#include "Relay.h"
#include "TopIndex.h"

// CR2RTrack command target

class CR2RTrack : public CObject
{
public:
	CR2RTrack();
	virtual ~CR2RTrack();
public:
	int Initialize(void);

	TOP_INDEXER_VARIABLE m_stIndexerVar;

	// Lead frame indexer stepper ID
	byte m_ucIndexerStepperId;
	
private:
    // Indexer stepper speed profile
    STEP_SPEED_PROFILE m_stIndexerSpeedProfile;
	ST_AXIS_PARA m_stIndexerProfile;
	short m_nTrackErrorCode;
	TOP_TRACK_DATA m_stTopTrackRcd;
	ST_DELAY_STATE stTimeDelayPara;	//v1.0.T18 rename
	short m_nCurrentBondingUnit;
	short m_nIndexEvent;
	char m_nIndexState;
	char StateSeqence[6];
	short m_nLastIndexDebugState;
	BOOL m_bTrackValid;
	BOOL m_bRunWithNoLf;
	bool m_bOffloadEnableFlag;
	long m_lStartDelayTime;
	long m_lOriginPos; 
	long m_lMoveDist;
	long l1stIndexLen;//v2.20.Q1 add
	char m_nLastState; //v2.20.w1
public:
	//Bonding Area Slip Property
	TOP_SLIP_VAR m_stSlipOnTrack;
	CSensor  theInjectSnr;
	CSensor  theEjectSnr;
	CSensor  theAlignSnr;
	CSensor  theEjectJamSnr;
	CRelay   theInjector;
	CRelay   theInputClaw;
	CRelay   theOutputClaw;
	CRelay   theEjector;
    CRelay   theLFHolder;
	CRelay   *theTargetClaw;
	int SearchIndexStepHome(void);
	int WaitStepHomeFinish(void);
	int GetTrackErrorCode(short nType = 0);
	int ResetTrackErrorCode(short nType = 0);
	int SaveTrackRcd(void);
	int LoadTrackRcd(void);
	int GetTopTrackRcd(TOP_TRACK_DATA* stTrackData);
	int UpdateTopTrackRcd(TOP_TRACK_DATA* stTrackData);
	short GetCurrentBondUnit(void);
	int SetCurrentBondUnit(short nUnit);
	int RefreshMeasurementData(TOP_DEVICE_RECORD* pstTopDevicePara);
	int TransLogicToPhysicPos(long plLogicPos, long* plPhysicPos);
	int ResetSlipRcd();
	int SubmitIndexEvent(short nEvent);
	int ProcessIndexEvent(void);
	bool GetBondingEnableFlag(void);
	short GetTopTrackEvent(void);
	// Get Track Valid State 
	int GetTrackValidState(void);
    // Set Track Valid during Dry Run 
	int SetTrackValid(BOOL bTrackValid);
	int ChangeStepperMotionProfile(short nProfile);
	int SetOffloadEnableFlag(bool bFlag);
	bool GetOffloadEnableFlag(void);
	int SetAutoRunWithNoLf(BOOL bFlag);
	BOOL GetAutoRunWithNoLf(void);
	int SubmitClearTrackEvent(void);
	int MoveToLogicZero(void);
	int ResetRelayErrorState(void);
private:
	int LeftMoveArray(void);
	int CopyArray(char* cOriginal,short nTotalSteps);
	int EnterDelayState(char nNextStateAfterDelay, long DelayStartTime, long lDelayDurationInMs , long lMoveDist = 0, BOOL bDrMode =1 );


//**********FOR DEBUG AND DRY RUN****************************************
public:
	int SubmitIndexerDryRunEvent(short nEvent);
	int ProcessIndexerDryRun(void);
	short m_nTestEvent;
	short m_nTestState;
	long m_lTestDist[MAX_DEBUG_STEP];
	short m_TestTicker;
	short m_nIcrem;
	long m_lStartTime;
	long m_lRelayStartTime;
	int ProcessTopGripperDryRun(void);
	int SubmitGripperDryRun(short nEvent);
	short m_nGripperDryCycleState;
	unsigned long m_lClawInterval;


//************************************************************************

	
//********************PR INDEX********************************************
	
private:
	short m_bIndexPrManuAlignFlag;
	bool m_bPrIndexMask;
public:
	int CaculatePrIndexOffset(long* loffsetXInUm);
	int PerformIndexPrprogress(void);
	int PerformManualAlignment();
	short GetIndexPrManuAlignFlag(void);
	int SetIndexPrManuAlignFlag(short nFlag);
	long m_loffsetXInUm;
	bool GetPrIndexMask(void);
	int SetPrIndexMask(bool bMask);

//************************************************************************	
	bool m_bRequestOnloadFlag;
	int RequestOnloadEvent(void);
private:
	long m_lInjectStartTime;
	short m_nTrackStopReason;
public:
	int MoveIndexerToWorkPos(long lRefTailPos);
	int ResetTrackEvent(void);
	short CheckAtLogicZeroPos(void);
private:
	BOOL m_bTrackNeedClear;
public:
	BOOL GetTrackNeedClearFlag(void);
	int SetTrackNeedClearFlag(BOOL bFlag);
	int TailPosUpdate(long loffset);
private:
	short m_nNextLfState;
public:
	short GetNextLfState(void);
	long TransX(long lIndexerX, char cClawSide);
	long TransXRev(long lIndexerX, char cClawSide);
	int GetTopIndexerStepperProfile(short nProfile, STEP_SPEED_PROFILE *m_stIndexerSpeedProfile);
	int GetTopIndexerStepperProfile(short nProfile, ST_STEPPER_SPEED *pstIndexerSpeedProfile);
	short GetLfInjectUnit(void);
    //v2.20.w1 
    int TargetClaw(short nOnOff);
	int ErrorCode(short nMsg, char cWillState, char cWasState);
};

extern CR2RTrack theR2RTrack;
