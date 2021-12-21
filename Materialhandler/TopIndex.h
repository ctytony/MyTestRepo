//TopIndex.h v3.0.T418

#pragma once

#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"
#include "Relay.h"
#include "DeviceData.h"

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct TOP_TRACK_DATA
{
	short			lDelayType;
	unsigned long	lHeatingDelay;
	byte			nPrIndexEnable;					//bool bPrIndexEnable;
	long			lLimitTolerance;
	long			lSrchBackLen;
	long			lLfStandbyLen;
	long			nIndexSpeedCoeff;				//1,2,3,4,5,6,7
	long			lIndexError[TOP_MAX_INDEX_TIME];
	short			nEjectSpeedCoeff;				//1,2,3,4,5
	long			l1stSlipHeatDelay;
	long			lPrIndexTolerance;
	long			lIndexPosOffset;
	long			lOpenClampDelay;
	long			lIndexPrDelay;
	unsigned long	lInitDelay;
    bool			bVacuumControl;
	unsigned long	lVacuumDelay;
	unsigned long	l1stUnitHeatDelay;
	short			sPRIndexScore;
	int				iOutputMagazineSlotSequence;
	BYTE			nNextStepPusherIn;
	BYTE			nHeatDelayUnit;
	BYTE			nLFWaitPos;
	BYTE			bFlattenLF;						//V1.4.W35 add
	WORD			wFlattenDuration;				//V1.4.W35 add
	//char			chReserve[157];					//159 -> 157
	//char			chReserve[158];					//157 -> 158
	char			chReserve[155];					//158 -> 155

    //must init before use, or by default as 0
	TOP_TRACK_DATA():
		lDelayType(),
		lHeatingDelay(),
		nPrIndexEnable(),
		lLimitTolerance(),
		lSrchBackLen(),
		lLfStandbyLen(),
		nIndexSpeedCoeff(),
		lIndexError(),
		nEjectSpeedCoeff(),
		l1stSlipHeatDelay(),
		lPrIndexTolerance(),
		lIndexPosOffset(),
		lOpenClampDelay(),
		lIndexPrDelay(),
		lInitDelay(),
		bVacuumControl(),
		lVacuumDelay(),
		l1stUnitHeatDelay(),
		sPRIndexScore(),
		iOutputMagazineSlotSequence(),
		nNextStepPusherIn(),
		nHeatDelayUnit(),
		nLFWaitPos(),
		bFlattenLF(),				//V1.4.W35 add
		wFlattenDuration(1000){}	//V1.4.W35 add
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CTopTrack command target

enum EN_INDEX_SPEED
{
    SRCH_SPEED_PROFILE = 0, 
	SLOW_FORWARD_PROFILE,          
	FORWARD_PROFILE,
	INJECT_SPEED_PROFILE,
	BACKWARD_PROFILE,
	INDEX_SPEED_PROFILE,
	EJECT_SPEED_PROFILE,
	SLOW_SRCH_PROFILE,		//V1.5.W116 add
};

#define TOTAL_STEPS				6
#define ELEVATOR_TIME_OUT		22000

//Mech Distance in um
#define LOCATION_SENSOR_CALI_DIST	101000		//A	//104124->101000
#define CENTER_TO_LEFT_TRACK_EDGE	240060		//B
#define CENTRE_TO_LEFT_CLAW			100000		//C
#define CENTRE_TO_RIGHT_CLAW		100000		//D
#define CENTRE_TO_EJECTOR			184500		//E
#define DIAMETER_OF_GRIPPER			10000		//F
#define LEN_OF_EJECTOR				43000		//G

//center of the heat block is the logic zero position

/*                               |<---A---->|
                                 X
	                  Left Claw  |    Center of HB        Right Claw     Ejector
	|||------------------||-----------[-----|-----]-----------||---------<=======------|||
	                     |<-------C-------->|<-------D------->|
	 |<-----------------B------------------>|<------------E------------->|<--G->|
*/

#define DIST_TRACK_EDGE_TO_MAG		24000   //um
#define INJECT_TO_CLAW_LEN			84000
#define INJECTOR_EXTENDER_LEN		46000
#define LEN_OF_INJECTOR				74000
#define INJECTOR_DROP_BACK			4000
#define EJECT_BACKWARD_STEPS_AFTER_EJECT	25000
 
//  Medium Variables:
struct TOP_INDEXER_VARIABLE
{
	short      nTotalUnits;
	long       lLfTotalLen;
	long       lCatchUpLen;
	long       lSrchStartPos;
	long       l1stIndexLen;
	long       lNorIndexLen;
	long       lLeftClawIndexLimitPos;
	long       lRightClawIndexLimitPos;
	long       lRightLimitPos;
	long       lLeftLimitPos;
	long       lEjectStartPos;
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
	short      nCatchUpUnit;			//default total unit > 3 enable
};

struct ST_DELAY_STATE	//v3.0.T40 rename
{
	char			nNextState;
	unsigned long	lDelayDuration;
	long			lStartTime;
	long			lMoveDist;
	BOOL			bMoveMode;		//relative/absolute
};

enum EN_INDEX_EVENT
{
    INDEX_EVENT_IDLE,
	INDEX_EVENT_AUTO_INDEX,
	INDEX_EVENT_SINGLE_INDEX,
	INDEX_EVENT_STANDBY_NEXT_LF,
	INITIAL_INDEX_EVENT,
	INDEX_NEW_SLIP_EVENT,
	INDEX_UNIT_STANDBY_EVENT,
	INDEX_UNIT_1ST_UNIT_EVENT,
	INDEX_WITH_NOTHING_MORE,
	INDEX_EVENT_CLEAR_TRACK_FIRST,
	INDEX_EVENT_CLEAR_TRACK_NORMAL,
	INDEX_EVENT_CLEAR_LAST_UNIT,
	INDEX_EVENT_CLEAR_OUTPUT_ONLY,
};

enum EN_INDEX_STATE
{
    INDEX_STATE_IDLE,						//0
	DEVICE_INJECT,							//1
	DEVICE_WAIT_ELEVATOR,					//2
	DEVICE_INJECT_CHECK_ONLOADER,			//3
	DEVICE_INJECT_START,					//4
	DEVICE_INJECT_WAIT_AT_START,			//5
	DEVICE_INJECT_WAIT_SENSOR,				//6
	DEVICE_INJECT_WAIT_INJECT_DEST,			//7
	DEVICE_INJECT_WAIT_LOOSING_TENSE,		//8
	DEVICE_INJECT_WAIT_INJECT_DEST2,		//9
	DEVICE_COMPEN,							//10
	DEVICE_COMPEN_WAIT_AT_START,			//11
	DEVICE_COMPEN_WAIT_DESTINATION,			//12
	DEVICE_CATCH_UP,						//13
	DEVICE_WAIT_STANBY_POS,					//14
	DEVICE_ROUGH_SRCH,						//15
	DEVICE_ROUGH_SRCH_WAIT_AT_START,		//16
	DEVICE_ROUGH_SRCH_WAIT_ROUGH_REACH,		//17
	DEVICE_ROUGH_SRCH_WAIT_BACK_REDO,		//18
	DEVICE_ROUGH_SRCH_BACK,					//19
	DEVICE_ROUGH_WAIT_SRCH_BACK,			//20
	DEVICE_FINE_SRCH_WAIT_STANDBY,			//21
	DEVICE_FINE_WAIT_FORWARD,				//22
	DEVICE_OUTPUT,							//23
	DEVICE_OUTPUT_WAIT_AT_OUTPUT_POS,		//24
	DEVICE_OUTPUT_ONE_CIRCLE_DONE,			//25
	DEVICE_OUTPUT_WAIT_SEC_OUTPUT_END,		//26
	DEVICE_EJECTION,						//27
	DEVICE_OUTPUT_WAIT_AT_EJECT_POS,		//28
	DEVICE_OUTPUT_WAIT_EJECT_DEST,			//29
	DEVICE_OUTPUT_BACKWARD_AFTER_EJECT,		//30
	DEVICE_INDEX,							//31
	DEVICE_INDEX_WAIT_VACUUM_DELAY,			//32
	DEVICE_INDEX_WAIT_CLAW_CLAMP,			//33
	DEVICE_INDEX_WAIT_BH_SAFTY,				//34
	DEVICE_INDEX_WAIT_CLAMP_OPEN,			//35
	DEVICE_INDEX_WAIT_CLAMP_OPEN_DELAY,		//36
	DEVICE_INDEX_WAIT_FEED_IN,				//37
	DEVICE_INDEX_WAIT_PR_INDEX_DELAY,		//38
	DEVICE_INDEX_WAIT_CLAMP_PRE_CLOSE,		//39
	DEVICE_INDEX_WAIT_PR_FINE_TUNE,			//40
	DEVICE_INDEX_WAIT_MANU_ALIGN,			//41
    DEVICE_INDEX_WAIT_CLAMP_CLOSE,			//42
	DEVICE_INDEX_WAIT_HEAT_DELAY,			//43
	DEVICE_INDEX_STOP,						//44
    CLAW_STAND_BY,							//45
	CLAW_STAND_BY_WAIT_DEST_POS,			//46
	TOP_INDEX_DELAY_WAITING,				//47
	TOP_CLEAR_TRACK,						//48
	TOP_CLEAR_TRACK_START_1ST,				//49
	TOP_CLEAR_TRACK_OFFLOAD_1ST,			//50
	TOP_CLEAR_TRACK_WAIT_OFFLOAD_PROC,		//51
	TOP_CLEAR_TRACK_START_2ND,				//52
	CLAW_POSITION_RESET,					//53
	DEVICE_CHECK_CLAMP_OPEN,				//54
	DEVICE_CHECK_WAIT_CLAMP_OPEN,			//55
	DEVICE_INDEX_AFTER_FEED_IN,				//56
	DEVICE_INDEX_AFTER_FEED_IN_2,			//57	//V1.5.W79 add
	DEVICE_INDEX_WAIT_PR_INDEX_DELAY_2,		//58	//V1.5.W79 add
	DEVICE_INDEX_WAIT_CLAMP_CLOSE_2,		//59	//V1.5.W79 add
	DEVICE_INDEX_CLAMP_PRE_CLOSE,			//60	//V1.5.W79 add
	DEVICE_INDEX_WAIT_CLAMP_PRE_CLOSE_2,	//61	//V1.5.W79 add
	DEVICE_INDEX_SLOW_SRCH,					//62	//V1.5.W116 add
	DEVICE_INDEX_WAIT_SLOW_SRCH,			//63	//V1.5.W116 add
    DEVICE_INDEX_DELAY_AFTER_CLAMP_CLOSE,	//64	//V1.5.W140 add
	DEVICE_INDEX_SLOW_SRCH_BACK,			//65	//V1.5.W164 add
};

enum EN_TEST_EVENT
{
    INDEX_TEST_EVENT_IDLE,
	INDEX_TEST_SPEED_PROFILE,
};

#define MAX_DEBUG_STEP   5

enum EN_TEST_STATE
{
	INDEX_TEST_STATE_IDLE,
    INDEX_TEST_STATE_WAIT_HOME,
	INDEX_TEST_STATE_WAIT_CENTER,
	INDEX_TEST_STATE_WAIT_LEFT,
	INDEX_TEST_STATE_WAIT_LBACK,
	INDEX_TEST_STATE_WAIT_RIGHT,
	INDEX_TEST_STATE_WAIT_RBACK,
	INDEX_TEST_STATE_WAIT_CHANGE_PROFILE,
	INDEX_TEST_STATE_WAIT_WINCLAMP_OPEN,
	INDEX_TEST_STATE_WAIT_WINCLAMP_CLOSE
};
  
enum EN_CLAW_DRY_CYCLE_STATE
{
	CLAW_DRY_CYCLE_IDLE,
	CLAW_DRY_CYCLE_WAIT_CLAMP,
	CLAW_DRY_CYCLE_WAIT_OPEN,
};

enum EN_NEXT_LF_STATE
{
	NEXT_LF_NONE,
	NEXT_LF_AFTER_INJECT,
	NEXT_LF_AFTER_SRCH,
	NEXT_LF_AFTER_CATCH_UP,
};

enum EN_INDEX_PR_STATE 
{
	INDEX_PR_OK,
    INDEX_PR_ENTER_MANU_ALIGN,
	INDEX_PR_MANU_ALIGN_OK,
	INDEX_PR_MANU_ALIGN_STOP,
};

#define	CLAMP_FRONT_SENSOR	0x01		//V1.5.W17 add
#define	CLAMP_BACK_SENSOR	0x02		//V1.5.W17 add

#define INPUT_OUTPUT_CLAW_SPAN	205000	//V1.5.W82 add

class CTopTrack : public CObject
{
public:
	CTopTrack();
	virtual ~CTopTrack();
public:	//variables
	TOP_INDEXER_VARIABLE m_stIndexerVar;
	//Bonding Area Slip Property
	TOP_SLIP_VAR m_stSlipOnTrack;
	CSensor m_InjectSensor;
	CSensor m_EjectSensor;
	CSensor m_AlignSensor;
	CSensor m_EjectJamSensor;
	CRelay m_Injector;
	CRelay m_InputClaw;
	CRelay m_OutputClaw;
	CRelay m_Ejector;
    CRelay m_LFHolder;
	CRelay *m_pTargetClaw;
	short m_nTestEvent;
	short m_nTestState;
	long m_lTestDist[MAX_DEBUG_STEP];
	short m_nTestTicker;
	long m_lStartTime;
	short m_nGripperDryCycleState;
	unsigned long m_lClawInterval;
	long m_lOffsetx1um;
	bool m_bRequestOnload;
	CSensor m_TrackFrontHome, m_TrackBackHome;	//V1.4.W36 add
	BYTE m_nPRIState;		//V1.5.W123 add, V1.5.W140 edit
public:	//functions
	byte GetIndexStepID();	//v3.0.T373 add
	void VerifyData();		//v3.0.T58 add
	void Init(void);
	bool IsVacuumControl(void);
	void HomeIndexer(void);
	void WaitHomeIndexer(void);
	int GetErrorCode(short nType = 0);
	void ResetErrorCode(short nType = 0);
	void SaveRcd(void);		//v3.0.T32 rename
	void LoadRcd(void);		//v3.0.T32 rename
	TOP_TRACK_DATA *GetRcd(void);
	void GetRcd(TOP_TRACK_DATA* stTrackData);
	void SetRcd(TOP_TRACK_DATA* stTrackData);
	short GetCurBondUnit(void);
	void SetCurBondUnit(short nUnit);
	void RefreshMeasurementData(const TOP_DEVICE_RECORD *pstDevice);	//v3.0.T86 edit
	void ResetSlipRcd();
	void SubmitIndexEvent(short nEvent);
	void ProcessIndexEvent(void);
	short GetEvent(void);
	bool IsValid(void);
	void SetValid(bool bValid);
	void ChangeStepperMotionProfile(short nProfile);
	void SetOffloadEnable(bool bEnable);
	bool IsOffloadEnable(void);
	void SetRunNoLf(bool bRunNoLf);
	bool IsRunNoLf(void);
	void SubmitClearTrackEvent(void);
	void MoveIndexerToLogicZero(void);
	void ResetRelayErrorState(void);
	void SubmitDryRunEvent(short nEvent);
	void ProcessDryRunEvent(void);
	void ProcessGripperDryRunEvent(void);
	void SubmitGripperDryRunEvent(short nEvent);
	int CalcPrIndexOffset(long* loffsetXInUm);
	void PerformPrIndex(void);
	int PerformManualAlignment(CRelay *pClaw = NULL);
	short GetIndexPrManualAlign(void);
	void SetIndexPrManualAlign(short nAlign);
	bool IsPrIndexMask(void);
	void SetPrIndexMask(bool bMask);
	void RequestOnload(void);
	int MoveIndexerToWorkPos(long lRefTailPos);
	void ResetEvent(void);
	void WaitMoveIndexer(void);
	bool IsTrackNeedClear(void);
	void SetTrackNeedClear(bool bNeedClear);
	void SetTailPos(long lOffset);
	short GetNextLfState(void);
	long TransX(long lIndexerX, char cClawSide);
	long TransXRev(long lIndexerX, char cClawSide);
	void GetIndexerStepperProfile(short nProfile, STEP_SPEED_PROFILE *m_stIndexerSpeedProfile);
	void GetIndexerStepperProfile(short nProfile, ST_STEPPER_SPEED *pstIndexerSpeedProfile);
	short GetLfInjectUnit(void);
	void SetTeaching(bool bTeaching);
	BYTE InjectEnabled();
	BYTE IsClampReady();		//V1.5.W17 add
private:	//variables
    STEP_SPEED_PROFILE m_stIndexerSpeedProfile;
	ST_AXIS_PARA m_stIndexerAxisParam;
	byte m_nIndexStepID;	//v3.0.T373 rename
	short m_nErrorCode;
	TOP_TRACK_DATA m_stTopTrackRcd;
	ST_DELAY_STATE m_stDelayState;	//v3.0.T40 rename
	short m_nCurBondUnit;
	short m_nEvent;
	char m_cState;
	char m_cStateSequence[8];
	short m_nLastDebugState;
	bool m_bValid;
	bool m_bRunNoLf;
	bool m_bOffloadEnable;
	long m_lStartDelayTime;
	long m_lMoveDist;
	long l1stIndexLen;
	long m_lStandbyAfterSearch;
	short m_nIndexPrManualAlign;
	bool m_bPrIndexMask;
	long m_lInjectStartTime;
	short m_nTrackStopReason;
	bool m_bTrackNeedClear;
	short m_nNextLfState;
	bool m_bTeaching;
	CHTimer m_GripperDryRunTimer;
	BYTE m_bClampSensorEnabled;	//V1.5.W87 edit
	BYTE m_bNeedOffset;		//V1.5.W79 add
	int m_nPRIndexStatus;	//V1.5.W79 add
private:	//functions
	void LeftMoveArray(void);
	void EnterDelayState(char nNextState, long lDelayStartTime, long lDelayDuration_x1ms, long lMoveDist = 0, BOOL bMoveMode = 1);
	long LogicToPhyPos(long lLogicPos);	//v3.0.T374 edit
	CRelay* GetClaw();	//V1.5.W82 add
};

extern CTopTrack theTopTrack;
