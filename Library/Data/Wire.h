//Wire.h v1.1.T26

#pragma once

#include "DataStruct.h"

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct WIRE_RECORD
{
	short       nWireNum;
	TBL_POS     stTchPos[2];			//Teach position
	short       nRefNum[2];
	short       nLoopMode;				//loop group
	bool        bVisionAhead;			//Flag if do vision ahead
	bool        bCheckNsop;				//Check nsop flag
	TBL_POS     stCorrectBondPos[2];	//Correct bond position
	float       fPowerOffset[2];
	short       nTimeOffset[2];
	short       nForceOffset[2];
	//short       nBallType;	//v1.1.T4 delete
	short       nBumpType;		//v1.1.T4 add
	short       n2ndCtactForceOffset;
	short       n1stCtactForceOffset;
	short       nCtactTimeOffset[2];
	float       fCtactPowerOffset[2];
	float       fPrePowerOffset[2];
	//float       fBumpWirePowerOffset;		//v1.1.T14 delete
	float       fReserve;	//v1.1.T14 add
	char        chPadType[2];
	bool        bConstWireLength;
	short       nWireLength;
	short       n1stBondPtPadOffsetX;
	short       n1stBondPtPadOffsetY;
	short       nWireLengthCorrect;
	char        chBondGroup;
	short       n2ndBondPtPadOffsetX;
	short       n2ndBondPtPadOffsetY;
	char        ch1stBondScrubMode;
	char        ch2ndBondScrubMode;
	char        chBumpBondScrubMode;
	bool        b1stPtILL;
	bool        b1stPtLeadCapiOffset;
	short       n1stPtLeadOffsetX;
	short       n1stPtLeadOffsetY;
	bool        b2ndPtILL;
	bool        b2ndPtLeadCapiOffset;
	short       n2ndPtLeadOffsetX;
	short       n2ndPtLeadOffsetY;
	float       fBumpPowerOffset[2];
	short       nBumpForceOffset[2];   
	short       nBumpTimeOffset[2];
	float       fBumpCtactPowerOffset[2];
	short       nBumpCtactForceOffset[2];
	short       nBumpCtactTimeOffset[2];
	float       fBump1stPreCtactPowerOffset;
	short       nBump1stPreCtactForceOffset;
	short       nBump1stPreCtactTimeOffset;
	char		cBsobFabType;	//v1.1.T12 rename
	short		nBsobShapeBaseHeightOffset;
	short		nBsobShapeSpanDistOffset;
	short		nBsobShapeScrubDistOffset;
	short		nBsobShapeBallThicknessOffset;
	short		nBsobShapeCuttingPowerOffset;
	short		nBsobShapeCuttingPowerDelayOffset;
	short		nBsobShapeMotionSpeedOffset;
	short		nBsobShapeDirectionAngleOffset;
	char		cWireFabType;	//v1.1.T12 rename
	bool		bFastLoop;
	short		nPreShiftOffset2;
	short       nPreForceOffset[2];
	short       nPreTimeOffset[2];
	BYTE        nBkPrBondPoint;
	short       nSrchHtOffset[2];		//v1.0.T22 add
	//v1.0.T24 add
	short		nBbosShapeBaseHeightOffset;
	short		nBbosShapeSpanDistOffset;
	short		nBbosShapeBallThicknessOffset;
	short		nBbosShapeScrubDistOffset;
	short		nBbosShapeCuttingPowerOffset;
	short		nBbosShapeCuttingPowerDelayOffset;
	short		nBbosShapeMotionSpeedOffset;
	short		nBbosShapeDirectionAngleOffset;
	short       nBbosBump1stPreCtactTimeOffset;
	float       fBbosBump1stPreCtactPowerOffset;
	short       nBbosBump1stPreCtactForceOffset;
	short       nBbosBumpCtactTimeOffset[2];
	float       fBbosBumpCtactPowerOffset[2];
	short       nBbosBumpCtactForceOffset[2];
	short       nBbosBumpTimeOffset[2];
	float       fBbosBumpPowerOffset[2];
	short       nBbosBumpForceOffset[2];
	char		cBsobShapeBumpShapeMode;	//v1.1.T10 add
	char		cBbosShapeBumpShapeMode;	//v1.1.T10 add
	char		cBbosFabType;				//v1.1.T12 add
	short		nBsobWirePreCtactTimeOffset;	//v1.1.T14 add
	short		nBsobWirePreCtactForceOffset;	//v1.1.T14 add
	float		fBsobWirePreCtactPowerOffset;	//v1.1.T14 add
	short		nBsobWireCtactTimeOffset;	//v1.1.T14 add
	short		nBsobWireCtactForceOffset;	//v1.1.T14 add
	float		fBsobWireCtactPowerOffset;	//v1.1.T14 add
	short		nBsobWireBondTimeOffset;	//v1.1.T14 add
	short		nBsobWireBondForceOffset;	//v1.1.T14 add
	float		fBsobWireBondPowerOffset;	//v1.1.T14 add
	short		nBsobWireRlsTimeOffset;		//v1.1.T14 add
	short		nBsobWireRlsForceOffset;	//v1.1.T14 add
	float		fBsobWireRlsPowerOffset;	//v1.1.T14 add

	//char        cReserve[68];			//v1.0.T22 delete
	//char        cReserve[64];			//v1.0.T24 delete
	//char        cReserve[8];			//v1.1.T10 delete
	//char        cReserve[6];			//v1.1.T10 add
	//char        cReserve[5];			//v1.1.T12 add
	char        cReserve[100];			//v1.1.T14 add - add 95 bytes

	//must init before use, or by default as 0
	WIRE_RECORD():
		nWireNum(),
		stTchPos(),
		nRefNum(),
		nLoopMode(),
		bVisionAhead(),
		bCheckNsop(),
		stCorrectBondPos(),
		fPowerOffset(),
		nTimeOffset(),
		nForceOffset(),
		//nBallType(),	//v1.1.T4 delete
		nBumpType(),	//v1.1.T4 add
		n2ndCtactForceOffset(),
		n1stCtactForceOffset(),
		nCtactTimeOffset(),
		fCtactPowerOffset(),
		fPrePowerOffset(),
		//fBumpWirePowerOffset(),	//v1.1.T14 delete
		fReserve(),		//v1.1.T14 add
		chPadType(),
		bConstWireLength(),
		nWireLength(),
		n1stBondPtPadOffsetX(),
		n1stBondPtPadOffsetY(),
		nWireLengthCorrect(),
		chBondGroup(),
		n2ndBondPtPadOffsetX(),
		n2ndBondPtPadOffsetY(),
		ch1stBondScrubMode(),
		ch2ndBondScrubMode(),
		chBumpBondScrubMode(),
		b1stPtILL(),
		b1stPtLeadCapiOffset(),
		n1stPtLeadOffsetX(),
		n1stPtLeadOffsetY(),
		b2ndPtILL(),
		b2ndPtLeadCapiOffset(),
		n2ndPtLeadOffsetX(),
		n2ndPtLeadOffsetY(),
		fBumpPowerOffset(),
		nBumpForceOffset(),
		nBumpTimeOffset(),
		fBumpCtactPowerOffset(),
		nBumpCtactForceOffset(),
		nBumpCtactTimeOffset(),
		fBump1stPreCtactPowerOffset(),
		nBump1stPreCtactForceOffset(),
		nBump1stPreCtactTimeOffset(),
		cBsobFabType(),		//v1.1.T12 rename
		nBsobShapeBaseHeightOffset(),
		nBsobShapeSpanDistOffset(),
		nBsobShapeScrubDistOffset(),
		nBsobShapeBallThicknessOffset(),
		nBsobShapeCuttingPowerOffset(),
		nBsobShapeCuttingPowerDelayOffset(),
		nBsobShapeMotionSpeedOffset(),
		nBsobShapeDirectionAngleOffset(),
		cWireFabType(),		//v1.1.T12 rename
		bFastLoop(),
		nPreShiftOffset2(),
		nPreForceOffset(),
		nPreTimeOffset(),
		nBkPrBondPoint(),
		nSrchHtOffset(),	//v1.0.T22 add
		//v1.0.T24 add
		nBbosShapeBaseHeightOffset(),
		nBbosShapeSpanDistOffset(),
		nBbosShapeBallThicknessOffset(),
		nBbosShapeScrubDistOffset(),
		nBbosShapeCuttingPowerOffset(),
		nBbosShapeCuttingPowerDelayOffset(),
		nBbosShapeMotionSpeedOffset(),
		nBbosShapeDirectionAngleOffset(),
		nBbosBump1stPreCtactTimeOffset(),
		fBbosBump1stPreCtactPowerOffset(),
		nBbosBump1stPreCtactForceOffset(),
		nBbosBumpCtactTimeOffset(),
		fBbosBumpCtactPowerOffset(),
		nBbosBumpCtactForceOffset(),
		nBbosBumpTimeOffset(),
		fBbosBumpPowerOffset(),
		nBbosBumpForceOffset(),
		cBsobShapeBumpShapeMode(),		//v1.1.T10 add
		cBbosShapeBumpShapeMode(),		//v1.1.T10 add
		cBbosFabType(),					//v1.1.T12 add
		nBsobWirePreCtactTimeOffset(),	//v1.1.T14 add
		nBsobWirePreCtactForceOffset(),	//v1.1.T14 add
		fBsobWirePreCtactPowerOffset(),	//v1.1.T14 add
		nBsobWireCtactTimeOffset(),		//v1.1.T14 add
		nBsobWireCtactForceOffset(),	//v1.1.T14 add
		fBsobWireCtactPowerOffset(),	//v1.1.T14 add
		nBsobWireBondTimeOffset(),		//v1.1.T14 add
		nBsobWireBondForceOffset(),		//v1.1.T14 add
		fBsobWireBondPowerOffset(),		//v1.1.T14 add
		nBsobWireRlsTimeOffset(),		//v1.1.T14 add
		nBsobWireRlsForceOffset(),		//v1.1.T14 add
		fBsobWireRlsPowerOffset(){}		//v1.1.T14 add
};
#pragma warning(default : 4351)
#pragma pack(pop)

//v1.0.T10 add
enum DUMMY_PGM_PATTERN 
{
    DUMMY_PGM_VERTICAL, 
    DUMMY_PGM_HORIZONTAL,
    DUMMY_PGM_HORIZONTAL_LEFT,	//v1.6.T277 add
	DUMMY_PGM_HORIZONTAL_RIGHT,	//v1.6.T277 add
    DUMMY_PGM_CIRCLE, 
    DUMMY_PGM_SQUARE,
    DUMMY_PGM_INCREMENTAL,
};

// CWireMgr command target

class CWireMgr : public CObject
{
public:
	CWireMgr();
	virtual ~CWireMgr();
public:	//functions
	void VerifyData(void);	//v1.0.T19 add
    short Get1stBondTaughtPts(void);	//get number of taught 1st bond points
    short Get2ndBondTaughtPts(void);	//get number of taught 2nd bond points
    void Append1stBondPoint(int iBondOn, int iPadType, TBL_POS* pstBondPt);
    void Append2ndBondPoint(int iBondOn, int iPadType, TBL_POS* pstBondPt);
    void Insert1stBondPoint(short nPointNum, int iBondOn, int iPadType, TBL_POS* pstBondPt);
    void Insert2ndBondPoint(short nPointNum, int iBondOn, int iPadType, TBL_POS* pstBondPt);
    void Delete1stBondPoint(short nPointNum);
    void Delete2ndBondPoint(short nPointNum);
    void NormalizeWireLength(void);	//normalize the wire length for whole program
    void AppendBondPoint(void);		//append taught bond point number
    void SwapWireRecord(short n1stWire, short n2ndWire);
    void GenerateWirePattern(int iPgmType, int iWirePitch, int iWireLen);	//generate wire pattern for dummy bond program
	WIRE_RECORD *GetRcd(short nWireNum);	//v1.0.T6 add
    void GetRcd(WIRE_RECORD *pstRcd, short nWireNum);
    void SetRcd(WIRE_RECORD *pstRcd, short nWireNum);
    void AddRcd(void);
    int GetTotalWireNum(void);
    void ClearAllRcd(void);
    void SaveRcd(void);
    void LoadRcd(void);
    // Calculate bond point position according to vision result
    void CalcBondPoint(short nWireNum, TBL_POS *pst1stBondPt, TBL_POS *pst2ndBondPt, bool bCorrectBond = false);
    void GenerateIncrementalWirePattern(int iWirePitch);
    // Update edited wire point to all units
    int UpdateEditedWirePoint(short nCurrentWire);
    void EditWirePoint(short nWireNum, short nBondPoint, TBL_POS* pstBondPos);
    double GetWireLength(short nWireNum);
    // Equalize force, power according to wire direction
    void EqualizeForcePower(short nWireNum, short nForceFactor, float fPowerFactor, short* pnForceCompensate, float* pfPowerCompensate);
private:	//variables
    short m_nTotalWire;			//v1.1.T15 rename
    WIRE_RECORD *m_pstWire;		//v1.1.T15 rename
    short m_nTaughtBondPt1;		//v1.1.T15 rename - no. of taught 1st bond points
    short m_nTaughtBondPt2;		//v1.1.T15 rename - no. of taught 2nd bond points
private:	//functions
	void SetDummyPgmHori(int iWirePitch, int iWireLen);		//v1.0.T9 edit
	void SetDummyPgmHoriLeft(int iWirePitch, int iWireLen);	//v1.0.T8 add, v1.0.T9 edit
	void SetDummyPgmHoriRight(int iWirePitch, int iWireLen);	//v1.0.T8 add, v1.0.T9 edit
	void SetDummyPgmVert(int iWirePitch, int iWireLen);		//v1.0.T9 edit
	void SetDummyPgmCircle(int iWirePitch, int iWireLen);	//v1.0.T9 edit
	void SetDummyPgmSquare(int iWirePitch, int iWireLen);	//v1.0.T9 edit
	void SetDefaultRcd(WIRE_RECORD* pstWireRcd);	//v1.0.T9 edit
};

extern CWireMgr theWire;
