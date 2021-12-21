//LoopPara.h v1.0.T35

#pragma once

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct ST_LOOP_BASE_PARA	//v1.0.T17 rename
{
    short nKinkNum;
    short nLoopHt;				//v1.0.T21 rename
    short nRevDist;
    short nRevAngle;
    short nLoopHtCorrect;
    short nKink2SpanLen;		//v1.0.T28 rename
    short nKink2SpanLenUnit;	//v1.0.T28 rename	//0: %; 1: um
    short nKink2HtFactor;		//v1.0.T28 rename
    short nPullDist;
    short nLoopBaseMode;		//0: Standard; 1: Mode A
    short nSrchHt2; 			//v1.0.T21 rename
    short nSrchSpeed2;
    short nRevHt;				//v1.0.T21 rename
    short nTrajFactor;
    char  cTrajMode;			//v1.0.T21 rename
    char  cLoopTopMode;			//v1.0.T21 rename
    short nCtactLvlOffset;
    short nLAngle;
    bool  bPreKinkEnable;
    short nPreKinkHt;			//v1.0.T22 rename
    short nPreKinkDist;
    short nPreKinkAngle;
    short nKink2Smooth;			//v1.0.T28 rename
    short nKink3SpanLen;		//v1.0.T28 rename
    char  cKink3SpanLenUnit;	//v1.0.T28 rename	//0: %; 1: um
    short nKink3HtFactor;		//v1.0.T28 rename
    short nKink3Smooth;			//v1.0.T28 rename
    short nTwistRatio;
    short nCtactAngleCorrect;
    short nSlopeLen;			//v1.0.T21 rename
    short nLoopProfile;
    char  cSlopeLenUnit;		//v1.0.T21 rename	//0: %; 1: um
	short nTwistFwdHtOffset;	//v1.0.T3 add
	short nRevHtFactor2;		//v1.0.T30 add
	short nRevArcFactor;		//v1.0.T30 add
	short nRevArcDelay;			//v1.0.T30 add
	short nPreKinkHtFactor2;	//v1.0.T30 add
	short nPreKinkArcFactor;	//v1.0.T30 add
	short nPreKinkArcDelay;		//v1.0.T30 add
	short nKink2HtFactor2;		//v1.0.T30 add
	short nKink2ArcFactor;		//v1.0.T30 add
	short nKink2ArcDelay;		//v1.0.T30 add
	short nKink2Ht;				//v1.0.T30 add
	short nKink2SpanLen2;		//v1.0.T30 add
	short nKink3HtFactor2;		//v1.0.T30 add
	short nKink3ArcFactor;		//v1.0.T30 add
	short nKink3ArcDelay;		//v1.0.T30 add
	short nKink3Ht;				//v1.0.T30 add
	short nKink3SpanLen2;		//v1.0.T30 add
	short nRevSpanLen2;			//v1.0.T35 add
	short nPreKinkSpanLen2;		//v1.0.T35 add
	//char  cReserve[5];		//v1.0.T3 add
	//char  cReserve[100];		//v1.0.T30 add
	char  cReserve[96];			//v1.0.T35 add

    //must init before use, or by default as 0
	ST_LOOP_BASE_PARA():
		nKinkNum(),
		nLoopHt(),				//v1.0.T21 rename
		nRevDist(),
		nRevAngle(),
		nLoopHtCorrect(),
		nKink2SpanLen(),		//v1.0.T28 rename
		nKink2SpanLenUnit(),	//v1.0.T28 rename
		nKink2HtFactor(),		//v1.0.T28 rename
		nPullDist(),
		nLoopBaseMode(),
		nSrchHt2(),				//v1.0.T21 rename
		nSrchSpeed2(),
		nRevHt(),				//v1.0.T21 rename
		nTrajFactor(),
		cTrajMode(),			//v1.0.T21 rename
		cLoopTopMode(),			//v1.0.T21 rename
		nCtactLvlOffset(),
		nLAngle(),
		bPreKinkEnable(),
		nPreKinkHt(),			//v1.0.T22 rename
		nPreKinkDist(),
		nPreKinkAngle(),
		nKink2Smooth(),			//v1.0.T28 rename
		nKink3SpanLen(),		//v1.0.T28 rename
		cKink3SpanLenUnit(),	//v1.0.T28 rename
		nKink3HtFactor(),		//v1.0.T28 rename
		nKink3Smooth(),			//v1.0.T28 rename
		nTwistRatio(),
		nCtactAngleCorrect(),
		nSlopeLen(),			//v1.0.T21 rename
		nLoopProfile(),
		cSlopeLenUnit(),		//v1.0.T21 rename
		nTwistFwdHtOffset(),
		nRevHtFactor2(),		//v1.0.T30 add
		nRevArcFactor(),		//v1.0.T30 add
		nRevArcDelay(),			//v1.0.T30 add
		nPreKinkHtFactor2(),	//v1.0.T30 add
		nPreKinkArcFactor(),	//v1.0.T30 add
		nPreKinkArcDelay(),		//v1.0.T30 add
		nKink2HtFactor2(),		//v1.0.T30 add
		nKink2ArcFactor(),		//v1.0.T30 add
		nKink2ArcDelay(),		//v1.0.T30 add
		nKink2Ht(),				//v1.0.T30 add
		nKink2SpanLen2(),		//v1.0.T30 add
		nKink3HtFactor2(),		//v1.0.T30 add
		nKink3ArcFactor(),		//v1.0.T30 add
		nKink3ArcDelay(),		//v1.0.T30 add
		nKink3Ht(),				//v1.0.T30 add
		nKink3SpanLen2(),		//v1.0.T30 add
		nRevSpanLen2(),			//v1.0.T35 add
		nPreKinkSpanLen2(){}	//v1.0.T35 add
};

struct ST_LOOP_ADVANCE_PARA	//v1.0.T17 rename
{
    short nWireFeedSpeed;		//v1.0.T23 rename
    short nWireBentSpeed;		//v1.0.T23 rename
    short nPreKinkWireFeedSpeed;
    short nPreKinkWireBentSpeed;
    short nWireFeedSpeed2;		//v1.0.T23 rename
    short nWireBentSpeed2;		//v1.0.T23 rename
    short nWireFeedSpeed3;		//v1.0.T23 rename
    short nWireBentSpeed3;		//v1.0.T23 rename
    short nTwistWireFeedSpeed;	//v1.0.T23 rename
    short nTwistRevBentSpeed;
    short nTwistFwdBentSpeed;
    short nTrajSpeed;			//v1.0.T22 rename
    short nPullSpeed;
    char  chRev[180];

    //must init before use, or by default as 0
	ST_LOOP_ADVANCE_PARA():
		nWireFeedSpeed(),		//v1.0.T23 rename
		nWireBentSpeed(),		//v1.0.T23 rename
		nPreKinkWireFeedSpeed(),
		nPreKinkWireBentSpeed(),
		nWireFeedSpeed2(),		//v1.0.T23 rename
		nWireBentSpeed2(),		//v1.0.T23 rename
		nWireFeedSpeed3(),		//v1.0.T23 rename
		nWireBentSpeed3(),		//v1.0.T23 rename
		nTwistWireFeedSpeed(),	//v1.0.T23 rename
		nTwistRevBentSpeed(),
		nTwistFwdBentSpeed(),
		nTrajSpeed(),			//v1.0.T22 rename
		nPullSpeed(){}
};

struct LOOP_MODE_ACS_PARA    // v1.0.P3 add
{
	//General                   
	short nLoopHeight1;
	short nLoopCorrect;
	short nAlpha;
	short nXyDelay;			//v1.0.T12 edit
	//kink1
	int iNeckHtUnit;		//v1.0.T12 edit
	short nNeckAngle;
	short nRHadj;
	int iRDadjUnit;
	short nNeckHtAdj;		//v1.0.T12 edit
	//Landing
	short nSLadj;			//v1.0.T12 edit
	short nAboveLoopTop;	//v1.0.T13 edit
	short nCtactLvlOffset;	//v1.0.T13 edit
	short nPullDistH;
	short nPullDistV;
	short nPullVTime;
	int iSLadjUnit;			//v1.0.T12 edit
	//S and A
	short nSpdBelleHt;		//v1.0.T13 edit
	short nSpdBelleDist;	//v1.0.T13 edit
	short nSpdHL;
	short nSpdRD2;
	short nSpdSL;
	short nSpdTraj;
	short nArcRD;
	short nArcRD2;
	short nArcBelle;		//v1.0.T13 edit
	short nArcLoopTop;		//v1.0.T13 edit
	short nArcTraj;			//v1.0.T13 edit
	short nSpdRH;
	short nSpdRD;
    //kink2
	short nFlatLength;		//v1.0.T13 edit
	int   iFlatLengthUnit;	//v1.0.T13 edit
	short nHLadj;
	int   iHLadjUnit;
	short nFLadj;
	int   iFLadjUnit;
	short nAngleComp;		//v1.0.T13 edit
    //Belle kink
	short nBelleHt;			//v1.0.T13 edit
	short nBelleDist;
	short nBelleAngle;
	int   iBelleAfterKink2;	//v1.0.T13 edit

	short nKinkHt;			//v1.0.T13 edit
    char  chRev[168];

	LOOP_MODE_ACS_PARA():
	    nLoopHeight1(),    
		nLoopCorrect(),
		nAlpha(),
		nXyDelay(),
		iNeckHtUnit(),
		nNeckAngle(),
		nRHadj(),
		iRDadjUnit(),
		nNeckHtAdj(),
		nSLadj(),
		nAboveLoopTop(),
		nCtactLvlOffset(),
		nPullDistH(),
		nPullDistV(),
		nPullVTime(),
		iSLadjUnit(),
		nSpdBelleHt(),
		nSpdBelleDist(),
		nSpdHL(),
		nSpdRD2(),
		nSpdSL(),
		nSpdTraj(),
		nArcRD(),
		nArcRD2(),
		nArcBelle(),
		nArcLoopTop(),
		nArcTraj(),
		nSpdRH(),
		nSpdRD(),
		nFlatLength(),
		iFlatLengthUnit(),
		nHLadj(),
		iHLadjUnit(),
		nFLadj(),
		iFLadjUnit(),
		nAngleComp(),
		nBelleHt(),
		nBelleDist(),
		nBelleAngle(),
		iBelleAfterKink2(),
		nKinkHt(){}
};

struct ST_LOOP_PARA	//v1.0.T17 rename
{
    ST_LOOP_BASE_PARA stLoopBase;	//v1.0.T19 rename
    ST_LOOP_ADVANCE_PARA stLoopAdv;	//v1.0.T19 rename

    //must init before use, or by default as 0
	ST_LOOP_PARA():
		stLoopBase(),
		stLoopAdv(){}
};

struct ST_LOOP_CONFIG_PARA	//v1.0.T17 rename
{
	short nNeckHtPremove;		//v1.0.T25 rename
	short nNeckDistPremove;		//v1.0.T25 rename
	short nKink2HtPremove;		//v1.0.T27 rename
	short nKink2DistPremove;	//v1.0.T27 rename
	short nLoopTopPremove;		//v1.0.T25 rename
	short nKink3HtPremove;		//v1.0.T27 rename
	short nKink3DistPremove;	//v1.0.T27 rename
	short nTwistHtPremove;		//v1.0.T27 rename
	short nTwistFwdDistPremove;	//v1.0.T25 rename
	char chRev[142];

	//must init before use, or by default as 0
	ST_LOOP_CONFIG_PARA():
		nNeckHtPremove(),
		nNeckDistPremove(),
		nKink2HtPremove(),
		nKink2DistPremove(),
		nLoopTopPremove(),
		nKink3HtPremove(),
		nKink3DistPremove(),
		nTwistHtPremove(),
		nTwistFwdDistPremove(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

#define TOTAL_LOOP_MODE			12
#define LOOP_STD_WIRE_LENGTH	2800    //um

enum EN_ABOVE_LOOP_TOP_MODE	//v1.0.T17 rename
{
    ABOVE_LOOP_TOP_DISABLE,
    ABOVE_LOOP_TOP_NORMAL,
    ABOVE_LOOP_TOP_FAST,
};

enum EN_LOOP_PROFILE	//v1.0.T17 rename
{
    LOOP_PROFILE_BGA1,
    LOOP_PROFILE_VLED,
    LOOP_PROFILE_BGA2,
    LOOP_PROFILE_BGA3,
    LOOP_PROFILE_BGA4,
};

// CLoopParaMgr command target

class CLoopParaMgr : public CObject
{
public:
	CLoopParaMgr();
	virtual ~CLoopParaMgr();
public:	//variables
    short m_nTotalLoopModeUsed;
    short m_nLoopModeUsed[TOTAL_LOOP_MODE];
	short m_nLoopAcsModeUsed[TOTAL_LOOP_MODE];	//v1.0.P3 add, v1.0.T9 edit
public:	//functions
    void GetDefaultRcd(ST_LOOP_PARA *pstLoopModePara, short nKinkNum);
    void GetRcd(short nLoopMode, ST_LOOP_PARA *pstLoopModePara);
    void SetRcd(short nLoopMode, ST_LOOP_PARA *pstLoopModePara);
    void GetConfigRcd(ST_LOOP_CONFIG_PARA *pstLoopConfig);
    void SetConfigRcd(ST_LOOP_CONFIG_PARA *pstLoopConfig);
    void CountProgramLoopMode(void);
    void SaveRcd(void);
    void LoadRcd(void);
    short GetLoopGroupsMaxSrchSpeed2(void);
    void ConvertLoopProfile(short nLoopGroup, short nLoopProfile, short nCtactDifference);
private:	//variables
    ST_LOOP_PARA m_astLoopPara[TOTAL_LOOP_MODE];
	LOOP_MODE_ACS_PARA m_LoopAcsModeArray[TOTAL_LOOP_MODE];	//v1.0.P3 add, v1.0.T9 edit
    ST_LOOP_CONFIG_PARA m_stLoopConfig;
public:
	void VerifyData();	//v1.0.T15 add
	void GetDefaultLoopAcsModePara(LOOP_MODE_ACS_PARA* pstLoopAcsModePara);	//v1.0.P3 add
	void GetLoopAcsModePara(short nLoopAcsMode, LOOP_MODE_ACS_PARA* pstLoopAcsModePara);	//v1.0.P3 add
	void SetLoopAcsModePara(short nLoopAcsMode, LOOP_MODE_ACS_PARA* pstLoopAcsModePara);	//v1.0.P3 add
};

extern CLoopParaMgr theLoopPara;	//v1.0.T10 edit
