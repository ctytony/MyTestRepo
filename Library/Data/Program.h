//Program.h v1.1.T20

#pragma once

#include "VisionSystem.h"

//#define PGM_VERSION	96		//v1.0.T4 add
//#define PGM_VERSION	97		//v1.0.T13 add	//for LoopPara eric buffer new param
//#define PGM_VERSION	98		//v1.0.T14 add	//for inc no. of z speed block
//#define PGM_VERSION	99		//v1.0.T27 add	//for force switch
//#define PGM_VERSION	100		//v1.0.T33 add	//for 3-profile EFO
//#define PGM_VERSION	101		//v1.0.T68 add	//for individual srch ht 1/2
//#define PGM_VERSION	102		//v1.0.T71 add	//add bbos offset in wire
//#define PGM_VERSION	103		//v1.0.T72 add	//add efo profile tail len
//#define PGM_VERSION	104		//v1.1.T9 add	//add WIRE_RECORD.cBsob/BbosShapeBumpShapeMode
//#define PGM_VERSION	105		//v1.1.T11 add	//add WIRE_RECORD.cBbosFabType
//#define PGM_VERSION	106		//v1.1.T12 add	//increase WIRE_RECORD size
//#define PGM_VERSION	107		//v1.1.T14 add	//increase ST_LOOP_BASE_PARA size
//#define PGM_VERSION	108		//v1.1.T16 add	//add BOND_BALL_PARA_RCD.nPosOffset/nSpanLenDelay/nArcSpan/nArcSpanDelay
//#define PGM_VERSION	109		//v1.1.T19 add	//add BOND_PARA_LIMIT.RlsPowerL/H
#define PGM_VERSION		110		//v1.1.T20 add	//add ST_LOOP_BASE_PARA.nRev/PreKinkSpanLen2

#define MAX_INDEX_TIMES			20
#define MAX_MATRIX_PER_INDEX	50

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct PGM_RECORD
{
	short       nVersion;					//Program version for load/save bond recipe
    bool        bPgmValid;
    short       nTotalWireNum;
    short       nTotalRefNum;
    short       nTailLengthUm;
    short       nDeviceRowNum;
    short       nDeviceColNum;
    short       nDeviceSeq;					//v1.0.T44 edit
    TBL_POS     stUnitRowOffset;
    TBL_POS     stUnitColOffset;
    short       nBondSeqOrientation;		//Horizontal, Vertical	//v1.0.T55 rename
    short       nTrackType;					//Dual, single
    TBL_POS     stTrackOffset;
    short       nPrAheadCol;				//Ahead column for PR
    short       nPrAlignMode;				//PR alignment mode, 0: Normal with 2 point alignment
											//1: Single point alignment without PR angle
											//2: Single point alignment with PR angle
    short       nVisionMoveDelay;			//Motion settle delay for PR
    short       nVisionLongMoveDelay;		//Long motion settle delay for PR
    short       nVisionPreMove;				//Pre move after grab image
    //short       nVisionDebugMode;			//v1.0.T9 delete
	//short		nReserve;					//v1.0.T9 add, v1.0.T17 delete
	short		nRecipeVersion;				//v1.0.T17 add
    short       nSparkDelay;
    short       nPrPassScore;
    short       nImageTransferTime;			//Image convert time for inverse Y image
	byte		nAutoSkipDieUnit;			//0: not skip, 1: skip die, 2: skip unit
    short       nDieSearchRange;			//Die search range
    short       nLeadSearchRange;			//Lead search range
    short       nDiePrTol;					//Die PR Align Tolerance
    short       nLeadPrTol;					//Lead PR Align Tolerance
    short       nLeadPrPassScore;			//Lead PR pass score
    bool        bCopperSequence;
    bool        bLeadPrEnable;				//Lead PR enable flag
    short       nExposureTime;				//Camera exposure time in us
    bool        bConstWireLength;			//Constant wire length feature
	long        lOxiPrevtTimeOut;
    short       nRetryExposureTime;
    bool        bPrRetry;
	wchar_t     cPgmName[16];
    bool        bFabTest;
    short       nPrRotateAngle;				//PR rotate angle range
	bool        bMultiBtos;
	bool        bN2H2Enable;
	short       nN2H2StableTime;
	short       nMultiBtos;
	bool        bBkPrAutoSwap;
    short       nFab2TailLengthUm;
    bool        bDiePrMultiSrch;
	short       nRedGain;
	short       nContrast;
	short       nBrightness;
	short       nPrFilterId;
    short       nCapiSmoothSpeed;
    short       nCapiSmoothThreshold;
    short       nCapiSmoothForce;
    short       nCapiSmoothCycle;
    short       nCapiSmoothAmplitude;
    float       fCapiSmoothPower;
    bool        bTailMonitor;
    short       nMatrixRow;					//Used for matrix-hybrid
    short       nMatrixColumn;				//Used for matrix-hybrid
    TBL_POS     stMatrixRowOffset;			//Used for matrix-hybrid
    TBL_POS     stMatrixColOffset;			//Used for matrix-hybrid
    short       nInteliPadLocateDelay;
    bool        bReserve;					//bFastLoop;
    short       nLeadTipOffset;
    short       nVllDelay;
    short       nLeadCapiOffset;
	LIGHT_PARA  stVllLight;
    char        chUsgOutputLvl;
    bool        bCapillaryAutoPlug;
	short       nCheckWireFeedTimeout;
	bool        bWireFeedCheck;
    short       nAGC;						//camera AGC control
	short       nAutoSkipScore;
	short       nManualSkip;
	short       nBondForceLimit;
	long        lRelaySafetyDuration;
	short		nHeadTimeOffset;
    short       nPrIndexSearchRange;
	BYTE		bAutoPadCentering;
	BYTE		nGapFill;
	BYTE		bBinarize;
	BYTE		bCircleFit;
	BYTE		bTry2xSearchRange;
	short		nDieOverlapTol;
	BYTE		nSerialSkipNum;				//v1.0.T4 add
	BYTE		nSkipNumPerIndex;			//v1.0.T4 add

	char		  cReserve[1];		//v1.0.T4 3 -> 1

    //must init before use, or by default as 0
	PGM_RECORD():
	    nVersion(),
	    bPgmValid(),
	    nTotalWireNum(),
	    nTotalRefNum(),
	    nTailLengthUm(),
	    nDeviceRowNum(),
	    nDeviceColNum(),
	    nDeviceSeq(),			//v1.0.T44 edit
	    stUnitRowOffset(),
	    stUnitColOffset(),
	    nBondSeqOrientation(),	//v1.0.T55 rename
	    nTrackType(),
	    stTrackOffset(),
	    nPrAheadCol(),
	    nPrAlignMode(),
	    nVisionMoveDelay(),
	    nVisionLongMoveDelay(),
	    nVisionPreMove(),
	    //nVisionDebugMode(),	//v1.0.T9 delete
		//nReserve(),			//v1.0.T9 add, v1.0.T17 delete
		nRecipeVersion(),		//v1.0.T17 add
	    nSparkDelay(),
	    nPrPassScore(),
	    nImageTransferTime(),
	    nAutoSkipDieUnit(),
	    nDieSearchRange(),
	    nLeadSearchRange(),
	    nDiePrTol(),
	    nLeadPrTol(),
	    nLeadPrPassScore(),
	    bCopperSequence(),
	    bLeadPrEnable(),
	    nExposureTime(),
	    bConstWireLength(),
	    lOxiPrevtTimeOut(),
	    nRetryExposureTime(),
	    bPrRetry(),
	    cPgmName(),
	    bFabTest(),
	    nPrRotateAngle(),
	    bMultiBtos(),
	    bN2H2Enable(),
	    nN2H2StableTime(),
	    nMultiBtos(),
	    bBkPrAutoSwap(),
	    nFab2TailLengthUm(),
	    bDiePrMultiSrch(),
	    nRedGain(),
	    nContrast(),
	    nBrightness(),
	    nPrFilterId(),
	    nCapiSmoothSpeed(),
	    nCapiSmoothThreshold(),
	    nCapiSmoothForce(),
	    nCapiSmoothCycle(),
	    nCapiSmoothAmplitude(),
	    fCapiSmoothPower(),
	    bTailMonitor(),
	    nMatrixRow(),
	    nMatrixColumn(),
	    stMatrixRowOffset(),
	    stMatrixColOffset(),
	    nInteliPadLocateDelay(),
	    bReserve(),
	    nLeadTipOffset(),
	    nVllDelay(),
	    nLeadCapiOffset(),
	    stVllLight(),
	    chUsgOutputLvl(),
	    bCapillaryAutoPlug(),
	    nCheckWireFeedTimeout(),
	    bWireFeedCheck(),
		nAGC(),
		nAutoSkipScore(),
		nManualSkip(),
		nBondForceLimit(),
		lRelaySafetyDuration(),
		nHeadTimeOffset(),
		nPrIndexSearchRange(),
		bAutoPadCentering(),
		nGapFill(3),
		bBinarize(),
		bCircleFit(),
		bTry2xSearchRange(),
		nDieOverlapTol(),
		nSerialSkipNum(5),				//v1.0.T4 add
		nSkipNumPerIndex(15){}			//v1.0.T4 add
};

struct INDEX_MATRIX_SCHEDULE
{
    bool abIndexMatrixSchedule[MAX_INDEX_TIMES][MAX_MATRIX_PER_INDEX];
    char cReserve[200];

    //must init before use, or by default as 0
	INDEX_MATRIX_SCHEDULE():
		abIndexMatrixSchedule(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

enum BTOS_DIR
{
	BTOS_DIR_DISABLE,
	BTOS_DIR_ENABLE_X,
	BTOS_DIR_ENABLE_Y
};

// CProgram command target

class CProgram : public CObject
{
public:
	CProgram();
	virtual ~CProgram();
public:	//function
	int VerifyData(void);				//v1.0.T56 add
    // nRow, nCol, nUnitNum: Index from 1, 2, ...
	short GetUnitNum(short nRow, short nCol);	//v1.0.T50 add
	short GetTotalUnit(void);			//v1.0.T46 add
	bool IsEvenCol(short nWireNum);		//v1.0.T45 add
	short GetN2H2StableTime(void);		//v1.0.T25 add
	short GetMultiBtos(void);			//v1.0.T24 add
	short GetLeadPrPassScore(void);		//v1.0.T24 add
	BYTE GetSkipNumPerIndex(void);		//v1.0.T23 add
	bool IsN2H2Enable(void);			//v1.0.T22 add
	short GetBondSeqOrientation(void);	//v1.0.T21 add, v1.0.T55 rename
	short GetTailLength_x1um(void);		//v1.0.T20 add
	bool IsCapillaryAutoPlug(void);		//v1.0.T19 add
	TBL_POS GetUnitColOffset(void);		//v1.0.T12 add
	TBL_POS GetUnitRowOffset(void);		//v1.0.T12 add
	TBL_POS GetMatrixColOffset(void);	//v1.0.T11 add
	TBL_POS GetMatrixRowOffset(void);	//v1.0.T11 add
	short GetMatrixCol(void);		//v1.0.T11 add
	short GetMatrixRow(void);		//v1.0.T11 add
	short GetTrackType(void);		//v1.0.T10 add
	short GetAutoSkipScore(void);	//v1.0.T8 add
	short GetDeviceRowNum(void);	//v1.0.T7 add
	short GetDeviceColNum(void);	//v1.0.T7 add
	short GetDeviceSeq(void);		//v1.0.T69 rename
	bool IsLeadPrEnable(void);
	short GetLeadSearchRange(void);
	short GetPrIndexSearchRange(void);
    int CreateDummyProgram(TBL_POS m_stLeftTopPos, TBL_POS m_stRightBottomPos, int iProgramType, int iWirePitch, int iWireLength, CVisionSystem* pVisionSystem);
    int ClearProgram(void);
    bool IsProgramValid(void);
    void CalcProgramWorkArea(double* pdLeftLmt, double* pdTopLmt, double* pdRightLmt, double* pdBottomLmt);
    void SaveRcd(void);
    void LoadRcd(void);
    short GetTotalWire(void);	//v1.0.T69 rename
    short GetTotalRef(void);	//v1.0.T69 rename
    void SetVersion(void);		//v1.0.T69 rename
    void GetRcd(PGM_RECORD* pstRcd);
    PGM_RECORD* GetRcd();	//v1.0.T30 add
    void SetRcd(PGM_RECORD* pstRcd);
    int CreateProgram(void);
    void SetTotalWire(short nWireNum);	//v1.0.T69 rename
    void SetTotalRef(short nRefNum);	//v1.0.T69 rename
    void CalcUnitOffset(short nRow, short nCol, TBL_POS *pstOffset);	//v1.0.T41 add, v1.0.T47 edit
    int GetUnitTotalDie(void);	//v1.0.T49 rename
    
    // Get the row and col of one unit;
    // pnRow, pnCol, nUnitNum: Index from 1, 2, ...
	void GetRowCol(short nUnitNum, short *pnRow, short *pnCol);	//v1.0.T36 add

    // Get matrix row and col of one matrix
    // pnMatRow, pnMatCol, nMatNum: Index from 1, 2, ...
	void GetMatRowCol(short nMatNum, short *pnMatRow, short *pnMatCol);	//v1.0.T35 add

    // Get matrix no. from matrix row and col
    // nMatRow, nMatCol, nMatNum: Index from 1, 2, ...
	short GetMatNum(short nMatRow, short nMatCol);	//v1.0.T36 add, v1.0.T53 edit

    // Calculate the step and repeat offset
    // The offset include unit within one matrix and matrix itself.
    void CalcStepRepeatOffset(short nMatRow, short nMatCol, short nRow, short nCol, TBL_POS* pstOffset);

	int GetUnitWireNum(int nWireNum);	//v1.0.T66 add
	int GetUnitTotalWire(void);			//v1.0.T48 edit
    short GetVersion(void);				//v1.0.T69 rename
    void ResetDeviceSeqToSingle(void);	//v1.0.T69 rename
    bool GetIndexMatrixSchedule(short nIndex, short nMatrix);
    void SetIndexMatrixSchedule(short nIndex, short nMatrix, bool bState);
    short GetNextMatrix(short nIndex, short nCurMatrix, short* pnNextMatrix);
    short GetPrevMatrix(short nIndex, short nCurMatrix, short* pnPrevMatrix);
	bool IsCopperSequence(void);
	void SetCopperSequence(bool bEnable);
	bool IsFabTest(void);
	short GetWireFeedTimeout(void);
	bool IsWireFeedCheck(void);
	bool IsMultiPr(void);
	bool IsAutoSkipDie(void);
	bool IsAutoSkipUnit(void);
	short CalcCtactBufferIndex(short nCurWire, short nCurRow, short nCurCol, short nCurBondUnit);	//v1.0.T64 rename
private:	//variable
    PGM_RECORD m_stProgram;
    INDEX_MATRIX_SCHEDULE m_stIndexMatrixSchedule;
};

extern CProgram thePgm;	//v1.0.T69 rename