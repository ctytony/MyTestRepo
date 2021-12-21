#pragma once

#include "datastruct.h"
#include "Sensor.h"
#include "Relay.h"
#include "Reference.h"
#include "Program.h"
#include "Matrix.h"
#include "Wire.h"
#include "LoopPara.h"
#include "BondPara.h"
#include "Debug.h"

#define OPENLOOP 0 
#define VOLTAGE  1 
#define CURRENT  2 
#define POWER    3

// CBond command target

enum EN_BOND_MODE	//v3.0.T113 rename
{
	BONDMODE_IDLE,
	BONDMODE_STOP,
	BONDMODE_SINGLE,
	BONDMODE_AUTO,
	BONDMODE_CORRECT,
	BONDMODE_BTOS
};

enum EN_BOND_TYPE	//v3.0.T114 rename
{
	BONDTYPE_SINGLE,
	BONDTYPE_RESTART,
	BONDTYPE_NORMAL
};

enum BOND_STATE 
{
	QUIT_AUTO_BOND,
	START_AUTO_BOND,
	PERFORM_VISION_ALIGN,
	PERFORM_WIRE_BOND,
	COMPLETE_UNIT_UPDATE,
	AUTO_BOND_INFORM_INDEX,
	AUTO_BOND_AFTER_INDEX,
	MAKEUP_AHEAD_VISION_ALIGN,
};

//v3.1.T93 delete
//enum EN_BOND_OBJECT_TYPE	//v3.0.T187 rename
//{
//	BONDOBJTYPE_BALL,
//	BONDOBJTYPE_WIRE,
//};

enum EN_BUMP_SEQ	//v3.1.T93 add
{
	BUMPSEQ_WIRE,
	BUMPSEQ_BALL,
	BUMPSEQ_BALL2,	//v3.1.T94 add
};

//v3.0.T187 delete
//enum BOND_SEQUENCE_STAGE
//{
//	BOND_STAGE_SENT,
//	BOND_STAGE_BONDED,
//};

//v3.1.T89 delete
//enum EN_BOND_OBJECT_SEQUENCE	//v3.0.T187 rename
//{
	//BONDOBJSEQ_NORMAL,
	//BONDOBJSEQ_BALL_ONLY,
	//BONDOBJSEQ_WIRE_ONLY,
//};

enum EN_BUMP_OPTION		//v3.1.T89 add
{
	BUMPOPT_NORMAL,
	BUMPOPT_BALL_ONLY,
	BUMPOPT_WIRE_ONLY,
};

enum EN_HELP_MSG		//v3.0.T187 rename
{
	MSG_CALLBACK_NONE,
	MSG_CALLBACK_ONLOAD_EMPTY,
	MSG_CALLBACK_OFFLOAD_FORK_FULL,
	MSG_CALLBACK_AUTOSAMPLING_ONOK,
	MSG_CALLBACK_DOUBLE_LF_HANDLE,
	MSG_CALLBACK_SRCH_TIME_OUT,
	MSG_CALLBACK_OFFLOAD_JAM,
	MSG_CALLBACK_ONLOAD_MAG_BUF_EMPTY,
	MSG_CALLBACK_OFFLOAD_MAG_BUF_EMPTY,
	MSG_CALLBACK_ONLOAD_PULLY_FULL,
	MSG_CALLBACK_OFFLOAD_PULLY_FULL,
};

enum EN_VISION_ACTION	//v1.6.T243 edit
{
	VISION_ACTION_AUTO = 0,
	VISION_ACTION_ALL_UNIT,
	VISION_ACTION_SINGLE_UNIT,
};

//v3.0.T406 delete
//#define USG_DUMMY_BLK0				30
//#define USG_DUMMY_BLK1				31
//#define USG_FIRST_BOND_SEGMENT_NUM	15
//#define USG_SECOND_BOND_SEGMENT_NUM	10

//#define USG_TRIG_PATTERN_IDLE			0	//v3.0.T406 delete
//#define USG_TRIG_PATTERN_SEAT			2	//v3.0.T406 delete
#define USG_TRIG_PATTERN_STANDBY1		6
//#define USG_TRIG_PATTERN_STANDBY2		5	//v3.0.T406 delete
#define USG_TRIG_PATTERN_CONTACT		3
//#define USG_SAMPLE_RATE				2	//v3.0.T406 delete

#define USG_1ST_BOND_SEG_NUM		20		//16
#define USG_2ND_BOND_SEG_NUM		18		//14	//v3.0.T412 edit
//#define USG_MIN_RAMP_TIME	1		//0.1ms

#define CONTACT_SEARCH_HEIGHT		1000.0	//1mm
#define VISION_LONG_DIST			10000.0	//10mm
#define DRYRUN_CTACT_OFFSET			500.0	//500um
#define DRYRUN_FORCE				-5		//5gm

//#define WIRE_END					1		//v3.0.T187 delete
     
#define BOOST_DURATION_SHORT        0.3
#define BOOST_DURATION_LONG         2.0
#define BOOST_AMPLITUDE_LOW         1.5
#define BOOST_AMPLITUDE_MIDDLE      2.0
#define BOOST_AMPLITUDE_HIGH        3.0

#define PR_RETRY_PIXEL_X            64
#define PR_RETRY_PIXEL_Y            48

class CBond : public CObject
{
public:
	CBond();
	virtual ~CBond();
public:	//variables
    // 2nd bond stick detect sample
    short m_nBondStickSample;
	CRelay m_AirTensioner;
	CRelay m_N2h2Port;
    CRelay m_PanelLight;
    short m_nCurWire;
    short m_nRestartWire;
    short m_nBondedWire;
	short m_nEndWire;
    short m_nCurRow;
    short m_nCurCol;
    short m_nCurMatRow;
    short m_nCurMatCol;
	long m_lTotalAutoSkipDieCount;
public:	//functions
	bool IsCheckNsop() {return m_bCheckNsop;};
	bool IsCheckNsol() {return m_bCheckNsol;};
	bool IsCheckFab() {return m_bCheckFab;};
	bool IsDryRun() {return m_bDryRun;};
	bool IsBondNoVision() {return m_bBondNoVision;};
	bool IsAutoIndex() {return m_bAutoIndex;};
	bool IsCompleteBond() {return m_bCompleteBond;};
	bool IsIll() {return m_bIll;};
	bool IsAutoDefineBond() {return m_bAutoDefineBond;};
	void SetCheckNsop(bool bCheck) {m_bCheckNsop = bCheck;};
	void SetCheckNsol(bool bCheck) {m_bCheckNsol = bCheck;};
	void SetCheckFab(bool bCheck) {m_bCheckFab = bCheck;};
	void SetDryRun(bool bDryRun) {m_bDryRun = bDryRun;};
	void SetBondNoVision(bool bNoVision) {m_bBondNoVision = bNoVision;};
	void SetAutoIndex(bool bAutoIndex) {m_bAutoIndex = bAutoIndex;};
	void SetCompleteBond(bool bCompleteBond) {m_bCompleteBond = bCompleteBond;};
	void SetIll(bool bIll) {m_bIll = bIll;};
	void SetAutoDefineBond(bool bAutoDefineBond)
	{	m_bAutoDefineBond = bAutoDefineBond;};
	void SetPrOn1stEnterDone(bool bDone)
	{	m_bPrOn1stEnterDone = bDone;};
	void SetCurTrack(short nCurTrack) {m_nCurTrack = nCurTrack;};
	void StartShowPrTimer();	//v3.0.T123 add
	long GetIdleTime_x1s();		//v3.0.T115 add
	long GetBondTime_x1s();		//v3.0.T115 add
	double GetLfCycleTime();
	void StopLfCycleTimer();
	void ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, double dParam);	//v3.0.T329 add
	void ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, int nParam2, double dParam3);	//v1.6.T234 add
	void ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, double dParam2, double dParam3, double dParam4);	//v1.6.T234 add
	void ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, double dParam2, double dParam3);	//v1.6.T234 add
	void ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam1, int nParam2);	//v1.5.T114 add, v1.6.T234 edit
	void ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg, int nParam);	//v1.5.T57 add, v1.6.T234 edit
	void ShowDebugMsg(EN_DEBUG_MODE enDebugMode, CString sMsg);				//v1.5.T56 add, v1.6.T234 edit
    void PerformAutoBond(EN_BOND_MODE nBondMode);
    int CalcWireInfo(short nWireNum, TBL_POS stCurTblPos,
			ST_WIRE_INFO *pstWireInfo, CString &sDebugInfo, bool bDebug);
    int CalcBallInfo(short nWireNum, TBL_POS stCurPos,
			ST_BALL_INFO *pstBallInfo, CString &sDebugInfo,
			bool bDebug, EN_BUMP_SEQ nBumpSeq = BUMPSEQ_BALL);	//v3.1.T103 add
    int PerformMultiPrAlignment(bool bSlowMove = true, short nVisionAction = VISION_ACTION_AUTO);
    int PerformVisionAlignment(bool bSlowMove = true,
			EN_VISION_ACTION nVisionAction = VISION_ACTION_AUTO);
    int PerformManualAlignment(short nRefNum);
    // Inform work holder to index a new lead frame
    int AutoBondInformIndex(void);
    // Handle functions after index
    int AutoBondAfterIndex(void);
    void MoveBondHeadToFireLevel(void);	//v3.1.T459 edit
    void InformAutoBondStop(void);
    EN_BOND_MODE GetBondMode(void);
    void SetBondMode(EN_BOND_MODE nMode);
    void MoveBondHeadToRelaxPosition(void);	//v3.1.T459 edit
	void LoadBondData(void);	//v3.0.T186 rename
	void SaveBondData(void);	//v3.0.T186 rename
    void ResetBondData(void);	//v3.0.T186 rename
    // Prepare bonding before enter auto bond menu
    int PrepareBonding(void);
    void MoveToWirePoint(short nWireNum, short nBondPt, bool bSlowSpeed = true, bool bUpdateLight = true);	//v3.1.T50 clean
    int DieAheadPR(void);
    int CheckBondProgress(short nProgress, short nIndex = 0, bool bShowDebugMsg = true);	//v1.5.T113 edit
    // Check if ahead PR alignment has done
    short CheckAheadAlignDone(short* pnWireNum, bool bForward = true);
    int SendTableSpeedProfileToServo(void);
    int SendBondHeadSpeedProfileToServo(void);
    // Get current bonding track
    short GetCurrentTrack(void);
	void Init(void);
    void StartStickDetect(short nBumpSeq);	//v3.1.T94 edit
    bool WaitIndexComplete();
    int UpdateBondTipOffset(short nBondedWireNum);
    // Help message call back function
    int HelpMsgCallBackFunction(short nCallBackFunctionId);
    // Perform lead ahead pattern recognition
    int PerformLeadAheadPR(void);
    // Get reference number which need perform PR
    void GetPerformPrRef(short nWireNum, short *pnStartRef, short *pnEndRef);	//v3.1.T443 edit
    void ResetMatrixPrStatus(short nWireNum, bool bResetSkip = true);
    void RedoPrAlignment(short nRwRow, short nRwCol, short nRwWire);
    void SetBumpBondOption(short nOption);	//v3.1.T92 rename
    short GetBumpBondOption(void);			//v3.1.T92 rename
    int SetPrPatternPassScore(void);
	int InitUsgProfile();
	int OnBSDLogEnable(int nErrorType, CString szString);
    int RetryPR(TBL_POS* pstPrPos, ULONG ulPattern, short nPrAlgorithm, short nBinaryThreshold, short nPrPassScore, PR_SRCH_RESULT* pstSrchResult);
    int PowerCompensation(short* pnPower);
	short SwapBkUpPrToMain(short nRefNum, short nBkPrId) ;
	int ResetBkPrCounts(void);
	int IcremBkPrCounts(short nId);
	int Set2ndPointBtosEnable(bool bEnable);
    int PerformCapillarySmooth(short nSrchSpeed, short nSrchTol, short nForce, float fSmoothPower, short nAmplitude, short nCycle);
    int PerformCapillaryClean(short nSrchSpeed, short nSrchTol, short nForce, short nCycle);
    double GetBondTimer(void);
	int UsgSelfDefineBond(WORD wTime, BYTE bDAC);
    bool IsUpdateBondData(void);	//v3.0.T187 rename
    void SetUpdateBondData(bool bUpdate);	//v3.0.T187 rename
	int PrepareBondingWithoutPr(void);
	void PreFireEFO(void);
	bool IsAutoBonding(void);
	void SetErrNo(int nErrNo);	//to identify where error come from
	int CtactSrch(short nSrchSpeed, short nSrchTol, long *plCtactLvl);
	void CapillaryWearOutPowerCompensate(short* pnPower);
	void CompensateCapiWearOutPower1(short* pnPower);	//v1.1.288.T10 add
	void CompensateCapiWearOutPower2(short* pnPower);	//v1.1.288.T15 add
	short CalcCapiWearOutPower1();	//v1.1.288.T13 add
	short CalcCapiWearOutPower2();	//v1.1.288.T15 add
	void GetSubunitStartEndRef(short nCurRef, short* nStartRef, short* nEndRef);
	void SetSkipUnit(short nCurRef, BYTE nStatus=VISION_SKIP_DIE);	//V1.1.W265 edit
	bool isCheckDieOverlap(short nCurRec, short nSequenceType);
	bool isManualAlignmentStop(short nCurRec);
	bool isQuitManualAlign(short nCount);
	void ResetSingleMatrixPr(short nRow, short nCol, short nWire);
	void SetTeaching(BYTE bTeaching);		//V1.1.W276 add
	void ProcessKeyboard();	//V1.4.W20 add, v2.0.T34 rename
	void ResetMinMaxRef();	//V1.5.W145 add
private:	//variables
	BOOL m_bBusy;	//v3.1.T461 add
	bool m_bStopBondReady;	//v3.1.T461 add
	UINT m_nStopCount;
	CHTimer m_ShowPrTimer;	//v3.0.T123 add
	CString m_sTemp;	//v3.0.T96 add
	bool m_bLfCycleStarted;
	CHTimer m_LfCycleTimer;
	int m_nLfCycleUnit;
	int m_nLfCycleWire;

	int m_nLfCycleMatRow;
	int m_nLfCycleMatCol;

	double m_dLfCyclePrevTime;
	double m_dLfCycleTime;
	int m_nIdleTime_x1s;
	int m_nErrNo;	//assign unique no. to indicate where the error come from
	short m_anErrBondProgress[2];
	WORD m_anUsgTime1[USG_1ST_BOND_SEG_NUM];
	BYTE m_anUsgPower1[USG_1ST_BOND_SEG_NUM];
	WORD m_anUsgTime2[USG_2ND_BOND_SEG_NUM];
	BYTE m_anUsgPower2[USG_2ND_BOND_SEG_NUM];
	int m_iUSGError;
	CHTimer m_BondTimer;
	CHTimer m_WireCycleTimer;
	double m_dLastBondTime;
	double m_dCurBondTime;
	CHTimer m_IdleTimer;
	int m_iHeaterWarnSkip;
	BYTE m_anIndex[2];
	int BkUpPrUseTimes[MAX_BAK_UP_PR_NUM];
	bool m_b2ndPointBtosEnable;
	//short m_nBondObjSeq;	//v3.1.T89 delete
	short m_nBumpBondOption;	//v3.1.T89 add
	bool m_bRequestStopBond;
	CSensor m_ThermalSensorX;
	CSensor m_ThermalSensorY;
	bool m_bUpdateBondData;	//v3.0.T187 rename
	short m_nAutoSkipDieCount;
	short m_nWaveformCaptureCount;
	bool m_bPreFireEfoSuccess;
	short m_nStickDetectBlock;
	CSensor theSparkStateRelay;
	// Current working track id
	short m_nCurTrack;
	EN_BOND_MODE m_nBondMode;
	EN_BOND_TYPE m_nBondType;
	// Swap bond buffer between 0 and 1 to speed up.
	short m_nBondBuffer;
	bool m_bCheckNsop;
	bool m_bCheckNsol;
	bool m_bCheckFab;
	bool m_bDryRun;
	bool m_bBondNoVision;
	bool m_bAutoIndex;
	bool m_bCompleteBond;
	bool m_bIll;	//intelligent lead locate
	bool m_bAutoDefineBond;
	bool m_bPrOn1stEnterDone;
	TBL_POS m_stCurTblPos;
	BYTE m_bRedoPR;				//V1.1.W250 add
	BYTE m_nSkipNumPerIndex;	//V1.1.W262 add
	BYTE m_nSerialSkipNum;		//V1.1.W265 add
	short m_nMinRef, m_nMaxRef;	//V1.5.W145 add
	double m_adMZ1[3][300];		//v1.5.175.W1 add, v1.6.T184 edit
	double m_adTail[100];		//v1.6.T185 add
private:	//functions
	void SetUsgProfile(short nWireNum, WORD *pnTime, BYTE *pnPower, BYTE nIndex, int nSegNum);
    double GetElapseBondTime();
	double CalcTransducerLenCompen(short nWireNum);	//v1.1.288.T2 add
    int CalcUsgProfile(short nWireNum, short nBumpSeq);	//v3.1.T93 edit
	void SetErrBondProgress(short nCheckProgress, short nReturnProgress);	//to indicate bond progress status
    int InitAutoBond(void);
    int QuitAutoBond(void);
    int StartAutoBond(void);
    // Perform wire bonding both for single or auto
    int PerformWireBond(void);
    // Update bonding status when complete bond one unit
    int CompleteUnitUpdate(void);
    int BondOneWire(bool bStartBond);
    int BondOneBall(bool bStartBond, EN_BUMP_SEQ nBumpSeq = BUMPSEQ_BALL);	//v3.1.T101 edit
    void ReportBondError(short nError);
    int SendWireInfoToServo(ST_WIRE_INFO *pstWireInfo, bool bStartBond, int nWireNum);
    int SendBallInfoToServo(ST_BALL_INFO *pstBallInfo, bool bStartBond, int nWireNum);
    int TriggerBonding(short nBumpSeq);		//v3.1.T94 edit
    int CheckBondResult(short nBumpSeq, bool bNextWireExist);	//v3.1.T506 edit
	void PrepareErrorMsg(long lVisionError, REF_RECORD *pstRef, PR_SRCH_RESULT *pstSrchResult, CStringList *psList);
	long TryBackupPR(BYTE nPoint, PGM_RECORD* lpstPgmRcd, REF_RECORD* lpstCurRef, REF_RECORD* lpstNextRef, TBL_POS* lpstCurTblPos, MATRIX_RECORD* lpstMatrix, short i, short j, short nLastRef, short nLastProfileX, short nLastProfileY, short nSearchRange, short nPrPassScore);
	void PopWireLenZeroError(short nWireNum);		//V1.1.W254 add
	int DieAheadAlign(bool bSlowMove, EN_VISION_ACTION nVisionAction);	//V1.4.W20 add
	int DieAheadAlign2(bool bSlowMove, EN_VISION_ACTION nVisionAction);	//3.1.T455 add
	int MatrixDieAheadAlign(bool bSlowMove, EN_VISION_ACTION nVisionAction);
	int MatrixDieAheadAlign2(bool bSlowMove, EN_VISION_ACTION nVisionAction);	//v3.1.T491 add
	int MatrixAlign(bool bSlowMove, EN_VISION_ACTION nVisionAction);	//v3.0.T279 add
	BYTE CheckPerIndexSkipNumLimit();		//V1.1.W262 add
	BYTE CheckSerialSkipNumLimit(PGM_RECORD *pstPgm);	//V1.1.W265 add
	void CollectData(ST_WIRE_INFO *pstWireInfo,
						ST_LOOP_PARA *pstLoopPara,
						long lCtactLvl1, long lCtactLvl2,
						short nSrchHt2, DOUBLE *pdArray);	//v1.5.175.W1 add
	void CalcMZ1(const WIRE_RECORD *pstWire, ST_WIRE_INFO *pstWireInfo);	//v1.5.175.W1 add
	void CalcTail(char cFabType);	//v1.6.T146 add
	void CalcArcMotionZ(double dDist, double dT1, double *pdTraj, short nSkewPercent = 100);	//v3.1.T319 add
};

extern CBond theBond;
