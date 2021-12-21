//TopEliteIndex.h

#pragma once

#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"
#include "Relay.h"
#include "DeviceData.h"

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct ST_TE_INDEX_RCD
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
	BYTE			bFlattenLF;
	WORD			wFlattenDuration;
	//char			chReserve[157];					//159 -> 157
	//char			chReserve[158];					//157 -> 158
	char			chReserve[155];					//158 -> 155

    //must init before use, or by default as 0
	ST_TE_INDEX_RCD():
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
		bFlattenLF(),
		wFlattenDuration(1000){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CTopEliteIndex command target

enum EN_TE_INDEX_SPEED	//v3.0.T414 edit
{
    TE_SPEED_SRCH = 0, 
	TE_SPEED_SLOW_FORWARD,          
	TE_SPEED_FORWARD,
	TE_SPEED_INJECT,
	TE_SPEED_BACKWARD,
	TE_SPEED_INDEX,
	TE_SPEED_EJECT,
	TE_SPEED_SLOW_SRCH,
};

#define MAX_DEBUG_STEP		5
#define	CLAMP_FRONT_SENSOR	0x01
#define	CLAMP_BACK_SENSOR	0x02
#define IN_OUT_CLAW_SPAN	205000

#define TOTAL_STEP			6
#define ELEV_TIMEOUT		22000

//Mech Distance in um
#define CENTER_TO_ALIGN_SENSOR		101000		//A
#define CENTER_TO_LEFT_EDGE			240060		//B
#define CENTER_TO_LEFT_CLAW			100000		//C
#define CENTER_TO_RIGHT_CLAW		100000		//D
#define BACKWARD_AFTER_EJECT		25000

//center of the heat block is the logic zero position

/*                               |<---A---->|
                                 X
	                  Left Claw  |    Center of HB        Right Claw     Ejector
	|||------------------||-----------[-----|-----]-----------||---------<=======------|||
	                     |<-------C-------->|<-------D------->|
	 |<-----------------B------------------>|<------------E------------->|<--G->|
*/

//  Medium Variables:
struct ST_TE_INDEX_VAR
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

struct ST_TE_DELAY_STATE	//v3.0.T414 edit
{
	char cNextState;
	DWORD lDelay_x1ms;
	long lStartTime;
	long lMoveDist;
	char cMoveMode;		//0:relative, 1:absolute
};

enum EN_TE_INDEX_EVENT	//v3.0.T414 rename
{
    TE_EVENT_IDLE,
	TE_EVENT_AUTO_INDEX,
	TE_EVENT_SINGLE_INDEX,
	TE_EVENT_STANDBY_NEXT_LF,
	TE_EVENT_INITIAL_INDEX,
	TE_EVENT_NEW_SLIP,
	TE_EVENT_UNIT_STANDBY,
	TE_EVENT_1ST_UNIT,
	TE_EVENT_NOTHING_MORE,
	TE_EVENT_CLEAR_TRACK_FIRST,
	TE_EVENT_CLEAR_TRACK_NORMAL,
	TE_EVENT_CLEAR_LAST_UNIT,
	TE_EVENT_CLEAR_OUTPUT_ONLY,
};

enum EN_TE_INDEX_STATE	//v3.0.T414 rename
{
    TE_STATE_IDLE,							//0
	TE_STATE_INJECT,						//1
	TE_STATE_WAIT_ELEV,						//2
	TE_STATE_INJECT_CHECK_ONLOAD,			//3
	TE_STATE_INJECT_START,					//4
	TE_STATE_INJECT_WAIT_AT_START,			//5
	TE_STATE_INJECT_WAIT_SENSOR,			//6
	TE_STATE_INJECT_WAIT_INJECT_DEST,		//7
	TE_STATE_INJECT_WAIT_LOOSING_TENSE,		//8
	TE_STATE_INJECT_WAIT_INJECT_DEST2,		//9
	TE_STATE_COMPEN,						//10
	TE_STATE_COMPEN_WAIT_AT_START,			//11
	TE_STATE_COMPEN_WAIT_DEST,				//12
	TE_STATE_CATCH_UP,						//13
	TE_STATE_WAIT_STANDBY_POS,				//14
	TE_STATE_ROUGH_SRCH,					//15
	TE_STATE_ROUGH_SRCH_WAIT_AT_START,		//16
	TE_STATE_ROUGH_SRCH_WAIT_ROUGH_REACH,	//17
	TE_STATE_ROUGH_SRCH_WAIT_BACK_REDO,		//18
	TE_STATE_ROUGH_SRCH_BACK,				//19
	TE_STATE_ROUGH_WAIT_SRCH_BACK,			//20
	TE_STATE_FINE_SRCH_WAIT_STANDBY,		//21
	TE_STATE_FINE_WAIT_FORWARD,				//22
	TE_STATE_OUTPUT,						//23
	TE_STATE_OUTPUT_WAIT_AT_OUTPUT_POS,		//24
	TE_STATE_OUTPUT_ONE_CIRCLE_DONE,		//25
	TE_STATE_OUTPUT_WAIT_SEC_OUTPUT_END,	//26
	TE_STATE_EJECT,							//27
	TE_STATE_OUTPUT_WAIT_AT_EJECT_POS,		//28
	TE_STATE_OUTPUT_WAIT_EJECT_DEST,		//29
	TE_STATE_OUTPUT_BACKWARD_AFTER_EJECT,	//30
	TE_STATE_INDEX,							//31
	TE_STATE_INDEX_WAIT_VACUUM_DELAY,		//32
	TE_STATE_INDEX_WAIT_CLAW_CLAMP,			//33
	TE_STATE_INDEX_WAIT_BH_SAFETY,			//34
	TE_STATE_INDEX_WAIT_CLAMP_OPEN,			//35
	TE_STATE_INDEX_WAIT_CLAMP_OPEN_DELAY,	//36
	TE_STATE_INDEX_WAIT_FEED_IN,			//37
	TE_STATE_INDEX_WAIT_PR_INDEX_DELAY,		//38
	TE_STATE_INDEX_WAIT_CLAMP_PRE_CLOSE,	//39
	TE_STATE_INDEX_WAIT_PR_FINE_TUNE,		//40
	TE_STATE_INDEX_WAIT_MANUAL_ALIGN,		//41
    TE_STATE_INDEX_WAIT_CLAMP_CLOSE,		//42
	TE_STATE_INDEX_WAIT_HEAT_DELAY,			//43
	TE_STATE_INDEX_STOP,					//44
    TE_STATE_CLAW_STANDBY,					//45
	TE_STATE_CLAW_STANDBY_WAIT_DEST_POS,	//46
	TE_STATE_WAIT_DELAY_MOVE,				//47
	TE_STATE_CLEAR_TRACK,					//48
	TE_STATE_CLEAR_TRACK_START_1ST,			//49
	TE_STATE_CLEAR_TRACK_OFFLOAD_1ST,		//50
	TE_STATE_CLEAR_TRACK_WAIT_OFFLOAD_PROC,	//51
	TE_STATE_CLEAR_TRACK_START_2ND,			//52
	TE_STATE_CLAW_POS_RESET,				//53
	TE_STATE_CHECK_CLAMP_OPEN,				//54
	TE_STATE_CHECK_WAIT_CLAMP_OPEN,			//55
	TE_STATE_INDEX_AFTER_FEED_IN,			//56
	TE_STATE_INDEX_AFTER_FEED_IN_2,			//57
	TE_STATE_INDEX_WAIT_PR_INDEX_DELAY_2,	//58
	TE_STATE_INDEX_WAIT_CLAMP_CLOSE_2,		//59
	TE_STATE_INDEX_CLAMP_PRE_CLOSE,			//60
	TE_STATE_INDEX_WAIT_CLAMP_PRE_CLOSE_2,	//61
	TE_STATE_INDEX_SLOW_SRCH,				//62
	TE_STATE_INDEX_WAIT_SLOW_SRCH,			//63
    TE_STATE_INDEX_DELAY_AFTER_CLAMP_CLOSE,	//64
	TE_STATE_INDEX_SLOW_SRCH_BACK,			//65
};

enum EN_TE_TEST_EVENT	//v3.0.T414 rename
{
    TE_TEST_EVENT_IDLE,
	TE_TEST_EVENT_SPEED_PROFILE,
};

enum EN_TE_TEST_STATE	//v3.0.T414 rename
{
	TE_TEST_STATE_IDLE,
    TE_TEST_STATE_WAIT_HOME,
	TE_TEST_STATE_WAIT_CENTER,
	TE_TEST_STATE_WAIT_LEFT,
	TE_TEST_STATE_WAIT_LBACK,
	TE_TEST_STATE_WAIT_RIGHT,
	TE_TEST_STATE_WAIT_RBACK,
	TE_TEST_STATE_WAIT_CHANGE_PROFILE,
	TE_TEST_STATE_WAIT_CLAMP_OPEN,
	TE_TEST_STATE_WAIT_CLAMP_CLOSE
};
  
enum EN_TE_CLAW_DRYRUN_STATE	//v3.0.T414 rename
{
	TE_CLAW_DRYRUN_IDLE,
	TE_CLAW_DRYRUN_WAIT_CLAMP,
	TE_CLAW_DRYRUN_WAIT_OPEN,
};

enum EN_TE_NEXTLF_STATE	//v3.0.T414 rename
{
	TE_NEXTLF_NONE,
	TE_NEXTLF_AFTER_INJECT,
	TE_NEXTLF_AFTER_SRCH,
	TE_NEXTLF_AFTER_CATCH_UP,
};

enum EN_TE_INDEXPR_STATE	//v3.0.T414 rename
{
	TE_INDEXPR_OK,
    TE_INDEXPR_ENTER_MANUAL_ALIGN,
	TE_INDEXPR_MANUAL_ALIGN_OK,
	TE_INDEXPR_MANUAL_ALIGN_STOP,
};

class CTopEliteIndex : public CObject
{
public:
	CTopEliteIndex();
	virtual ~CTopEliteIndex();
public:	//variables
	ST_TE_INDEX_VAR m_stIndexVar;
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
	CSensor m_TrackFrontHome, m_TrackBackHome;
	BYTE m_nPRIState;
public:	//functions
	byte GetIndexStepID();
	void VerifyData();
	void Init(void);
	bool IsVacuumControl(void);
	void HomeIndexer(void);
	void WaitHomeIndexer(void);
	int GetErrorCode(short nType = 0);
	void ResetErrorCode(short nType = 0);
	void SaveRcd(void);
	void LoadRcd(void);
	ST_TE_INDEX_RCD *GetRcd(void);
	void SetRcd(ST_TE_INDEX_RCD *pstTopIndex);
	short GetCurBondUnit(void);
	void SetCurBondUnit(short nUnit);
	void RefreshMeasurementData(const TOP_DEVICE_RECORD *pstDevice);
	void ResetSlipRcd();
	void SubmitIndexEvent(short nEvent);
	void ProcessIndexEvent(void);
	short GetEvent(void);
	bool IsValid(void);
	void SetValid(bool bValid);
	void SetStepSpeed(short nProfile);
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
	void GetStepSpeed(short nProfile, ST_STEPPER_SPEED *pstStepSpeed);
	short GetLfInjectUnit(void);
	void SetTeaching(bool bTeaching);
	BYTE InjectEnabled();
	BYTE IsClampReady();
private:	//variables
	byte m_nIndexStepID;
	short m_nErrorCode;
	ST_TE_INDEX_RCD m_stIndexRcd;
	ST_TE_DELAY_STATE m_stDelayState;
	short m_nCurBondUnit;
	short m_nEvent;
	char m_cState;
	char m_acStateSeq[8];
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
	BYTE m_bClampSensorEnabled;
	BYTE m_bNeedOffset;
	int m_nPRIndexStatus;
private:	//functions
	void LeftMoveArray(void);
	void EnterDelayState(char cNextState, long lDelay_x1ms, long lMoveDist = 0, char cMoveMode = 1);
	long LogicToPhyPos(long lLogicPos);
	CRelay *GetClaw();
};

extern CTopEliteIndex theTEIndex;
