//CuIndex.h v1.1.T10

#pragma once

#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"
#include "Relay.h"
#include "DeviceData.h"	//v1.0.T7 add

// CCuEliteTrack command target

#define TIMEOUT_1                              10000// 20000 
#define TIMEOUT_2                              20000// 20000 
#define TIMEOUT_3                              30000// 20000 
#define TIMEOUT_4                              40000// 20000 
#define TIMEOUT_5                              50000// 20000 

typedef enum enCuEliteIndexSpeed
{
	CUELITE_SRCH_SPEED_PROFILE = 0, 
	CUELITE_SLOW_FORWARD_PROFILE,          
	CUELITE_FORWARD_PROFILE,
	CUELITE_INJECT_SPEED_PROFILE,
	CUELITE_BACKWARD_PROFILE,
	CUELITE_INDEX_SPEED_PROFILE,
	CUELITE_EJECT_SPEED_PROFILE,
};

#define CUELITE_TOTAL_STEPS                   6

#define CUELITE_ELEVATOR_TIME_OUT             22000

//Mech Distance in um
//A
#define CUELITE_LOCATION_SENSOR_CALI_DIST     104124
//B
#define CUELITE_CENTER_TO_LEFT_TRACK_EDGE     240060
//C
#define CUELITE_CENTRE_TO_LEFT_CLAW           100000
//D
#define CUELITE_CENTRE_TO_RIGHT_CLAW          100000
//E
#define CUELITE_CENTRE_TO_EJECTOR             184500 
//F
#define CUELITE_DIAMETER_OF_GRIPPER           10000
//G
#define CUELITE_LEN_OF_EJECTOR                43000

//center of the heat block is the logic zero position


/*                               |<---A---->|
X
Left Claw  |    Center of HB        Right Claw     Ejector
|||------------------||-----------[-----|-----]-----------||---------<=======------|||
|<-------C-------->|<-------D------->|
|<-----------------B------------------>|<------------E------------->|<--G->|

*/

#define CUELITE_DIST_TRACK_EDGE_TO_MAG       24000   //um
#define CUELITE_INJECT_TO_CLAW_LEN           84000

#define CUELITE_INJECTOR_EXTENDER_LEN          46000
#define CUELITE_LEN_OF_INJECTOR              74000

#define CUELITE_INJECTOR_DROP_BACK            4000

//v2.18.5.J2 Add
#define CUELITE_EJECT_BACKWARD_STEPS_AFTER_EJECT  25000  //  v2.18.5.J4 Edit



//  Medium Variables:
typedef struct CUELITE_INDEXER_VARIABLE
{
	short      nTotalUnits;        //
	long       lLfTotalLen;        //
	long       lCatchUpLen;        //
	long       lSrchStartPos;      //
	long       l1stIndexLen;       //
	long       lNorIndexLen;       //
	long       lLeftClawIndexLimitPos;    //
	long       lRightClawIndexLimitPos;   //
	long       lRightLimitPos;     //
	long       lLeftLimitPos;      //
	long       lEjectStartPos;     //
	long       lIndexLenAftPR;
	long       lInjectStart[3];
	long       lInjectEnd[3];
	int        m_iInjectTimes;
	int        m_iOnloadRetryTimes;
	long       m_lInjectEndPos;
	long       m_lInjectStartPos;

	short      nInjectUnit;
	BOOL       nCompEnable;
	short      nSrchUnit;
	short      nCatchUpUnit; //default total unit >3 enable


}_CUELITE_INDEXER_VARIABLE;


typedef struct CUELITE_INDEXER_DEALY_ST
{
	char                nNextState;        //
	unsigned long       lDelayDuration;        //
	long                lStartTime;        //
	long                lMoveDist;
	BOOL                bMoveMode;           // 0: relative   1: abs

}_CUELITE_INDEXER_DEALY_ST;


typedef enum enCuEliteIndexEvents
{
	CUELITE_INDEX_EVENT_IDLE,
	CUELITE_INDEX_EVENT_AUTO_INDEX,
	CUELITE_INDEX_EVENT_SINGLE_INDEX,
	CUELITE_INDEX_EVENT_STANDBY_NEXT_LF,
	CUELITE_INITIAL_INDEX_EVENT,
	CUELITE_INDEX_NEW_SLIP_EVENT,
	CUELITE_INDEX_UNIT_STANDBY_EVENT,
	CUELITE_INDEX_UNIT_1ST_UNIT_EVENT,
	CUELITE_INDEX_WITH_NOTHING_MORE,
	CUELITE_INDEX_EVENT_CLEAR_TRACK_FIRST,
	CUELITE_INDEX_EVENT_CLEAR_TRACK_NORMAL,
	CUELITE_INDEX_EVENT_CLEAR_LAST_UNIT,
	CUELITE_INDEX_EVENT_CLEAR_OUTPUT_ONLY,

};

typedef enum enCuEliteIndexStates	//v1.0.T25 cleanup
{
	CUELITE_INDEX_STATE_IDLE,					//0
	CUELITE_DEVICE_INJECT,						//1
	CUELITE_DEVICE_WAIT_ELEVATOR,				//2
	CUELITE_DEVICE_INJECT_CHECK_ONLOADER,		//3
	CUELITE_DEVICE_INJECT_START,				//4
	CUELITE_DEVICE_INJECT_WAIT_AT_START,		//5
	CUELITE_DEVICE_INJECT_WAIT_SENSOR,			//6
	CUELITE_DEVICE_INJECT_WAIT_INJECT_DEST,		//7
	CUELITE_DEVICE_INJECT_WAIT_LOOSING_TENSE,	//8
	CUELITE_DEVICE_INJECT_WAIT_INJECT_DEST2,	//9
	CUELITE_DEVICE_COMPEN,						//10
	CUELITE_DEVICE_COMPEN_WAIT_AT_START,		//11
	CUELITE_DEVICE_COMPEN_WAIT_DESTINATION,		//12
	CUELITE_DEVICE_CATCH_UP,					//13
	CUELITE_DEVICE_WAIT_STANBY_POS,				//14
	CUELITE_DEVICE_ROUGH_SRCH,					//15
	CUELITE_DEVICE_ROUGH_SRCH_WAIT_AT_START,	//16
	CUELITE_DEVICE_ROUGH_SRCH_WAIT_ROUGH_REACH,	//17
	CUELITE_DEVICE_ROUGH_SRCH_WAIT_BACK_REDO,	//18
	CUELITE_DEVICE_ROUGH_SRCH_BACK,				//19
	CUELITE_DEVICE_ROUGH_WAIT_SRCH_BACK,		//20
	CUELITE_DEVICE_FINE_SRCH_WAIT_STANDBY,		//21
	CUELITE_DEVICE_FINE_WAIT_FORWARD,			//22
	CUELITE_DEVICE_OUTPUT,						//23
	CUELITE_DEVICE_OUTPUT_WAIT_AT_OUPUT_POS,	//24
	CUELITE_DEVICE_OUTPUT_ONE_CIRCLE_DONE,		//25
	CUELITE_DEVICE_OUTPUT_WAIT_SEC_OUTPUT_END,	//26
	CUELITE_DEVICE_EJECTION,					//27
	CUELITE_DEVICE_OUTPUT_WAIT_AT_EJECT_POS,	//28
	CUELITE_DEVICE_OUTPUT_WAIT_EJECT_DEST,		//29
	CUELITE_DEVICE_OUTPUT_BACKWARD_AFTER_EJECT,	//30
	CUELITE_DEVICE_INDEX,						//31
	CUELITE_DEVICE_INDEX_WAIT_VACUUM_DELAY,		//32
	CUELITE_DEVICE_INDEX_WAIT_CLAW_CLAMP,		//33
	CUELITE_DEVICE_INDEX_WAIT_BH_SAFTY,			//34
	CUELITE_DEVICE_INDEX_WAIT_CLAMP_OPEN,		//35
	CUELITE_DEVICE_INDEX_WAIT_CLAMP_OPEN_DELAY,	//36
	CUELITE_DEVICE_INDEX_WAIT_FEED_IN,			//37
	CUELITE_DEVICE_INDEX_WAIT_PR_INDEX_DELAY,	//38
	CUELITE_DEVICE_INDEX_WAIT_CLAMP_PRE_CLOSE,	//39
	CUELITE_DEVICE_INDEX_WAIT_PR_FINE_TUNE,		//40
	CUELITE_DEVICE_INDEX_WAIT_MANU_ALIGN,		//41
	CUELITE_DEVICE_INDEX_WAIT_CLAMP_CLOSE,		//42
	CUELITE_DEVICE_INDEX_WAIT_HEAT_DELAY,		//43
	CUELITE_DEVICE_INDEX_STOP,					//44
	CUELITE_CLAW_STAND_BY,						//45
	CUELITE_CLAW_STAND_BY_WAIT_DEST_POS,		//46
	CUELITE_INDEX_DELAY_WAITING,				//47
	CUELITE_CLEAR_TRACK,						//48
	CUELITE_CLEAR_TRACK_START_1ST,				//49
	CUELITE_CLEAR_TRACK_OFFLOAD_1ST,			//50
	CUELITE_CLEAR_TRACK_WAIT_OFFLOAD_PROC,		//51
	CUELITE_CLEAR_TRACK_START_2ND,				//52
	CUELITE_CLAW_POSITION_RESET,				//53		
};


typedef enum enCuEliteTestEvents
{
	CUELITE_INDEX_TEST_EVENT_IDLE,
	CUELITE_INDEX_TEST_SPEED_PROFILE,
};

#define CUELITE_MAX_DEBUG_STEP   5

typedef enum enCuEliteTestStates
{
	CUELITE_INDEX_TEST_STATE_IDLE,
	CUELITE_INDEX_TEST_STATE_WAIT_HOME,
	CUELITE_INDEX_TEST_STATE_WAIT_CENTER,
	CUELITE_INDEX_TEST_STATE_WAIT_LEFT,
	CUELITE_INDEX_TEST_STATE_WAIT_LBACK,
	CUELITE_INDEX_TEST_STATE_WAIT_RIGHT,
	CUELITE_INDEX_TEST_STATE_WAIT_RBACK,
	CUELITE_INDEX_TEST_STATE_WAIT_CHANGE_PROFILE,
};

typedef enum enCuEliteClawDryCycleState
{
	CUELITE_CLAW_DRY_CYCLE_IDLE,
	CUELITE_CLAW_DRY_CYCLE_WAIT_CLAMP,
	CUELITE_CLAW_DRY_CYCLE_WAIT_OPEN,

};

typedef enum enCuEliteNextLfStatus
{
	CUELITE_NEXT_LF_NONE,
	CUELITE_NEXT_LF_AFTER_INJECT,
	CUELITE_NEXT_LF_AFTER_SRCH,
	CUELITE_NEXT_LF_AFTER_CATCH_UP,
};

typedef enum CUELITE_INDEX_PR_STATE 
{
	CUELITE_INDEX_PR_OK,
	CUELITE_INDEX_PR_ENTER_MANU_ALIGN,
	CUELITE_INDEX_PR_MANU_ALIGN_OK,
	CUELITE_INDEX_PR_MANU_ALIGN_STOP,

} _CUELITE_INDEX_PR_STATE;

class CCuEliteTrack : public CObject
{
public:

	CCuEliteTrack();

	virtual ~CCuEliteTrack();

	void Initialize(void);

	CUELITE_INDEXER_VARIABLE m_stIndexerVar;

	// Lead frame indexer stepper ID
	byte m_ucIndexerStepperId;

	// Lead frame indexer stepper ID
	byte m_ucFrontTrackStepperId;

	// Lead frame indexer stepper ID
	byte m_ucBackTrackStepperId;

	//Bonding Area Slip Property
	CUELITE_SLIP_VAR m_stSlipOnTrack;

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

	short m_nTestEvent;
	short m_nTestState;
	short m_nGripperDryCycleState;
	unsigned long m_lClawInterval;
	long m_loffsetXInUm;

	void SearchIndexStepHome(void);
	void WaitStepHomeFinish(void);
	void MoveFrontTrackHome();
	void WaitFrontTrackStepHomeFinish(void);

	void MoveBackTrackHome();
	void WaitBackTrackStepHomeFinish(void);

	int GetTrackErrorCode(short nType = 0);

	void ResetTrackErrorCode(short nType = 0);

	void SaveTrackRcd(void);
	void LoadTrackRcd(void);
	void GetCuEliteTrackRcd(CUELITE_TRACK_DATA* stTrackData);
	void UpdateCuEliteTrackRcd(CUELITE_TRACK_DATA* stTrackData);

	short GetCurrentBondUnit(void);

	void SetCurrentBondUnit(short nUnit);
	void RefreshMeasurementData(CUELITE_DEVICE_RECORD* pstCuEliteDevicePara);

	void TransLogicToPhysicPos(long plLogicPos, long* plPhysicPos);
	void ResetSlipRcd();
	void SubmitIndexEvent(short nEvent);
	void ProcessIndexEvent(void);

	bool GetBondingEnableFlag(void);

	short GetCuEliteTrackEvent(void);
	// Get Track Valid State 
	int GetTrackValidState(void);
	// Set Track Valid during Dry Run 
	void SetTrackValid(BOOL bTrackValid);

	void ChangeStepperMotionProfile(short nProfile);

	void SetOffloadEnableFlag(bool bFlag);

	bool GetOffloadEnableFlag(void);

	void SetAutoRunWithNoLf(BOOL bFlag);

	BOOL GetAutoRunWithNoLf(void);

	void SubmitClearTrackEvent(void);

	void MoveToLogicZero(void);
	void ResetRelayErrorState(void);

	void SubmitIndexerDryRunEvent(short nEvent);
	void ProcessIndexerDryRun(void);

	void ProcessCuEliteGripperDryRun(void);

	void SubmitGripperDryRun(short nEvent);

	int CaculatePrIndexOffset(long* loffsetXInUm);

	void PerformIndexPrprogress(void);

	int PerformManualAlignment();

	short GetIndexPrManuAlignFlag(void);

	void SetIndexPrManuAlignFlag(short nFlag);

	bool GetPrIndexMask(void);

	void SetPrIndexMask(bool bMask);

	//************************************************************************	
	bool m_bRequestOnloadFlag;
	void RequestOnloadEvent(void);

	int MoveIndexerToWorkPos(long lRefTailPos);

	void ResetTrackEvent(void);
	void CheckAtLogicZeroPos(void);

	BOOL GetTrackNeedClearFlag(void);
	void SetTrackNeedClearFlag(BOOL bFlag);
	void TailPosUpdate(long loffset);

	short GetNextLfState(void);
	long TransX(long lIndexerX, char cClawSide);
	long TransXRev(long lIndexerX, char cClawSide);
	void GetCuEliteIndexerStepperProfile(short nProfile, STEP_SPEED_PROFILE *m_stIndexerSpeedProfile);
	void GetCuEliteIndexerStepperProfile(short nProfile, ST_STEPPER_SPEED *pstIndexerSpeedProfile);

	short GetLfInjectUnit(void);

	void MoveFrontTrackToStandbyPos();
	void MoveBackTrackToStandbyPos();

	void FlagTeaching(BYTE bTeaching);		//V3.3.W170 add
	BYTE InjectEnabled();		//V3.3.W170 add

private:

	long  m_lTestDist[CUELITE_MAX_DEBUG_STEP];

	short  m_TestTicker;
	short m_nIcrem;
	long m_lStartTime;
	long m_lRelayStartTime;

	// Indexer stepper speed profile
	STEP_SPEED_PROFILE m_stIndexerSpeedProfile;
	ST_AXIS_PARA m_stIndexerProfile;

	STEP_SPEED_PROFILE m_stFrontTrackSpeedProfile;
	ST_AXIS_PARA m_stFrontTrackProfile;

	STEP_SPEED_PROFILE m_stBackTrackSpeedProfile;
	ST_AXIS_PARA m_stBackTrackProfile;

	short m_nTrackErrorCode;

	CUELITE_TRACK_DATA m_stCuEliteTrackRcd;

	CUELITE_INDEXER_DEALY_ST  stTimeDelayPara;

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

	long m_l1stIndexLen;//v2.20.Q1 add

	long m_lSearchPos;
	long m_lStandbyAfterSrch;

	short m_bIndexPrManuAlignFlag;

	bool m_bPrIndexMask;

	long m_lInjectStartTime;
	short m_nTrackStopReason;

	BOOL m_bTrackNeedClear;

	short m_nNextLfState;
	BYTE m_bTeaching;		//V3.3.W170 add

	void LeftMoveArray(void);
	void EnterDelayState(char nNextStateAfterDelay, long DelayStartTime, long lDelayDurationInMs , long lMoveDist = 0, BOOL bDrMode =1 );
};

extern CCuEliteTrack theCuEliteTrack;
