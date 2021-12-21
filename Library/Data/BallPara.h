//BallPara.h v1.1.T8

#pragma once

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct ST_BUMP_BALL	//v1.1.T7 rename
{
    float fStandbyPower[2];
    short nCtactTime[2];
    short nCtactForce[2];
    float fCtactPower[2];
    short nBaseTime[2];
    short nBaseForce[2];
    float fBasePower[2];
    short nSrchHeight1;
    short nSrchSpeed1;
    short nSrchTol1;
    short nSrchHeight2;
    short nSrchSpeed2;
    short nSrchTol2;
    short nBondPointOffset;
    bool  bTailBreakEnable;
    char  chFabType;
    bool  b1stBondScrubEnable;
    short n1stScrubAmplitude;
    char  ch1stScrubDirection;
    short n1stScrubCycle;
    short n1stScrubSpeed;
    short n1stScrubForce;
    float f1stScrubPower;
    bool  bTailBondEnable;
    short nPreCtactTime[2];
    short nPreCtactForce[2];
    short nCtactSrchDelay1;
    bool  bStickDetect;
    short nRlsTime[2];
    short nRlsForce[2];
    short nRlsPower[2];
	short nPosOffset;		//v1.1.T4 add
	short nSpanLenDelay;	//v1.1.T4 add
	short nArcSpan;			//v1.1.T4 add
	short nArcSpanDelay;	//v1.1.T4 add
    //char  cReserve[40];
    char  cReserve[32];		//v1.1.T4 add

    //must init before use, or by default as 0
	ST_BUMP_BALL():		//v1.1.T7 rename
		fStandbyPower(),
		nCtactTime(),
		nCtactForce(),
		fCtactPower(),
		nBaseTime(),
		nBaseForce(),
		fBasePower(),
		nSrchHeight1(),
		nSrchSpeed1(),
		nSrchTol1(),
		nSrchHeight2(),
		nSrchSpeed2(),
		nSrchTol2(),
		nBondPointOffset(),
		bTailBreakEnable(),
		chFabType(),
		b1stBondScrubEnable(),
		n1stScrubAmplitude(),
		ch1stScrubDirection(),
		n1stScrubCycle(),
		n1stScrubSpeed(),
		n1stScrubForce(),
		f1stScrubPower(),
		bTailBondEnable(),
		nPreCtactTime(),
		nPreCtactForce(),
		nCtactSrchDelay1(),
		bStickDetect(),
		nRlsTime(),
		nRlsForce(),
		nRlsPower(),
		//v1.1.T4 add
		nPosOffset(),
		nSpanLenDelay(),
		nArcSpan(),
		nArcSpanDelay(){}
};

struct ST_BUMP_WIRE		//v1.1.T6 rename
{
    short nBaseTime;
    short nBaseForce;
    float fBasePower;
    short nSrchSpeed2;
    short nSrchTol2;
    short nBondPointOffset;
    bool  bTailBreakEnable;
    float fStandbyPower;
    short nPreCtactForce;
    short nPreCtactTime;
    short nCtactTime;
    short nCtactForce;
    float fCtactPower;
    short nReleaseTime;
    float fReleasePower;
    short nReleaseForce;
    short nSrchHeight;
    short nCtactSrchDelay;
    short nPreShift;
    short nBondShift;
    short nTailShift;
    short nShiftSpeed;
    char  cReserve[3];

    //must init before use, or by default as 0
	ST_BUMP_WIRE():		//v1.1.T6 rename
		nBaseTime(),
		nBaseForce(),
		fBasePower(),
		nSrchSpeed2(),
		nSrchTol2(),
		nBondPointOffset(),
		bTailBreakEnable(),
		fStandbyPower(),
		nPreCtactForce(),
		nPreCtactTime(),
		nCtactTime(),
		nCtactForce(),
		fCtactPower(),
		nReleaseTime(),
		fReleasePower(),
		nReleaseForce(),
		nSrchHeight(),
		nCtactSrchDelay(),
		nPreShift(),
		nBondShift(),
		nTailShift(),
		nShiftSpeed(){}
};

struct ST_BUMP_SHAPE	//v1.1.T8 rename
{
    short  nBaseHeight;
    short  nSpanDist;
    short  nMotionSpeed;
    short  nBallThickness;
    short  nScrubDist;
    short  nDirectionAngle;
    char   chShapeMode;
    short  nFlexLength;
    short  nFlexAngle;
    short  nFlexHeight;
    short  nCuttingPower;
    short  nCuttingPowerDelay;
    char   chRev[27];

    //must init before use, or by default as 0
	ST_BUMP_SHAPE():	//v1.1.T8 rename
		nBaseHeight(),
		nSpanDist(),
		nMotionSpeed(),
		nBallThickness(),
		nScrubDist(),
		nDirectionAngle(),
		chShapeMode(),
		nFlexLength(),
		nFlexAngle(),
		nFlexHeight(),
		nCuttingPower(),
		nCuttingPowerDelay(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CBallParaMgr command target

class CBallParaMgr : public CObject
{
public:
	CBallParaMgr();
	virtual ~CBallParaMgr();
public:	//functions
	void VerifyData();	//v1.0.T6 add
    void GetBsobBall(ST_BUMP_BALL *pstBsobBump);	//v1.1.T7 rename
    void GetBsobShape(ST_BUMP_SHAPE *pstBsobShape);	//v1.1.T8 rename
    void GetBbosBall(ST_BUMP_BALL *pstBbosBump);	//v1.1.T7 rename
    void GetBbosShape(ST_BUMP_SHAPE *pstBbosShape);	//v1.1.T8 rename
    void GetBsobWire(ST_BUMP_WIRE *pstBsobWire);	//v1.1.T6 rename
    void GetBbosWire(ST_BUMP_WIRE *pstBbosWire);	//v1.1.T6 rename
    void SetBsobBall(ST_BUMP_BALL *pstBsobBump);	//v1.1.T7 rename
    void SetBsobShape(ST_BUMP_SHAPE *pstBsobShape);	//v1.1.T8 rename
    void SetBbosBall(ST_BUMP_BALL *pstBbosBump);	//v1.1.T7 rename
    void SetBbosShape(ST_BUMP_SHAPE *pstBbosShape);	//v1.1.T8 rename
    void SetBsobWire(ST_BUMP_WIRE *pstBsobWire);	//v1.1.T6 rename
    void SetBbosWire(ST_BUMP_WIRE *pstBbosWire);	//v1.1.T6 rename
    void SaveRcd(void);
    void LoadRcd(void);
private:	//variables
    ST_BUMP_BALL m_stBsobBump;	//v1.1.T1 rename
    ST_BUMP_BALL m_stBbosBump;	//v1.1.T1 rename
    //Bsob wire 2nd bond parameter
    ST_BUMP_WIRE m_stBsobWire;	//v1.1.T1 rename
    //Bbos wire 2nd bond parameter
    ST_BUMP_WIRE m_stBbosWire;	//v1.1.T1 rename
    ST_BUMP_SHAPE m_stBsobShape;	//v1.1.T1 rename
    ST_BUMP_SHAPE m_stBbosShape;	//v1.1.T1 rename
private:	//functions
	//v1.1.T3 rename
    void GetDefaultBsobBumpPara(ST_BUMP_BALL *pstBsobBump);
    void GetDefaultBsobShapePara(ST_BUMP_SHAPE *pstBsobShape);
    void GetDefaultBbosBumpPara(ST_BUMP_BALL *pstBbosBump);
    void GetDefaultBbosShapePara(ST_BUMP_SHAPE *pstBbosShape);
    void GetDefaultBsobWirePara(ST_BUMP_WIRE *pstBsobWire);
    void GetDefaultBbosWirePara(ST_BUMP_WIRE *pstBbosWire);
};

extern CBallParaMgr theBallPara;
