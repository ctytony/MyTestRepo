//BondPara.h v1.0.T25

#pragma once

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct BOND_PARA_RCD
{
    short nStandbyPower[2];
    short nCtactTime[2];
    short nCtactPowerRamp[2];
    short nCtactForce[2];
    short nBaseTime[2];
    short nBasePowerRamp[2];
    short nBaseForce[2];
    short nReleaseTime[2];
    short nReleasePowerRamp[2];
    short nReleaseForce[2];
    short nPowerFactor;
    short nForceFactor;
    short nSrchHeight1;
    short nSrchSpeed1;
    short nSrchTol1;
    short nSrchHeight2;
    short nSrchTol2;
    short nBondShift2;
    short nWclTailClosePretrig;
    short nPowerDelay[2];
    short nTailShift;
    short nCtactSrchDelay[2];
    short nWclLoopPeakClosePretrig;
    short nPreCtactForce[2];
    short nWcl1stBondOpenPretrig;
    short nWcl2ndBondOpenPretrig;
    short nWclLoopPeakCloseTime;
    short nBondShift1;
    bool  b2ndBondScrubEnable;
    short n2ndScrubPosOffset1;
    short n2ndScrubPosOffset2;
    short n2ndScrubAmplitude;
    char  ch2ndScrubDirection;
    short n2ndScrubCycle;
    short n2ndScrubSpeed;
    short n2ndScrubForce;
    short n2ndScrubPower;
    float fStandbyPower[2];
    float fCtactPower[2];
    float fBasePower[2];
    float fReleasePower[2];
    float fPowerFactor;
    float f2ndScrubPower;
    short nShiftSpeed;
    char  chFeedPower;
    char  chFeedPowerTime;
    char  ch1stBondBoostEnable;
    char  chBoostAmplitude1;
    char  chBoostDuration1;
    bool  b1stBondScrubEnable;
    short n1stScrubAmplitude;
    char  ch1stScrubDirection;
    short n1stScrubCycle;
    short n1stScrubSpeed;
    short n1stScrubForce;
    float f1stScrubPower;
    float fCrtBondPowerOffset1;
    float fCrtBondPowerOffset2;
    short nCrtBondForceOffset1;
    short nCrtBondForceOffset2;
    short nCrtBondTimeOffset1;
    short nCrtBondTimeOffset2;
    short nTailBreakDist;
    short nTailBreakSpeed;
    bool  bTailBreakEnable;
    short nPreCtactTime[2];
    short nPreShift2;
    short nSrchSpeed2;
    char  ch1stBondImpactBoostEnable;
    char  chImpactBoostAmplitude1;
    char  chImpactBoostDuration1;
    float fCrtStandbyPowerOffset[2];	//v1.0.T22 add
	char cReserve[200];
	//char  cReserve[208];

    //must init before use, or by default as 0
	BOND_PARA_RCD():
		nStandbyPower(),
		nCtactTime(),
		nCtactPowerRamp(),
		nCtactForce(),
		nBaseTime(),
		nBasePowerRamp(),
		nBaseForce(),
		nReleaseTime(),
		nReleasePowerRamp(),
		nReleaseForce(),
		nPowerFactor(),
		nForceFactor(),
		nSrchHeight1(),
		nSrchSpeed1(),
		nSrchTol1(),
		nSrchHeight2(),
		nSrchTol2(),
		nBondShift2(),
		nWclTailClosePretrig(),
		nPowerDelay(),
		nTailShift(),
		nCtactSrchDelay(),
		nWclLoopPeakClosePretrig(),
		nPreCtactForce(),
		nWcl1stBondOpenPretrig(),
		nWcl2ndBondOpenPretrig(),
		nWclLoopPeakCloseTime(),
		nBondShift1(),
		b2ndBondScrubEnable(),
		n2ndScrubPosOffset1(),
		n2ndScrubPosOffset2(),
		n2ndScrubAmplitude(),
		ch2ndScrubDirection(),
		n2ndScrubCycle(),
		n2ndScrubSpeed(),
		n2ndScrubForce(),
		n2ndScrubPower(),
		fStandbyPower(),
		fCtactPower(),
		fBasePower(),
		fReleasePower(),
		fPowerFactor(),
		f2ndScrubPower(),
		nShiftSpeed(),
		chFeedPower(),
		chFeedPowerTime(),
		ch1stBondBoostEnable(),
		chBoostAmplitude1(),
		chBoostDuration1(),
		b1stBondScrubEnable(),
		n1stScrubAmplitude(),
		ch1stScrubDirection(),
		n1stScrubCycle(),
		n1stScrubSpeed(),
		n1stScrubForce(),
		f1stScrubPower(),
		fCrtBondPowerOffset1(),
		fCrtBondPowerOffset2(),
		nCrtBondForceOffset1(),
		nCrtBondForceOffset2(),
		nCrtBondTimeOffset1(),
		nCrtBondTimeOffset2(),
		nTailBreakDist(),
		nTailBreakSpeed(),
		bTailBreakEnable(),
		nPreCtactTime(),
		nPreShift2(),
		nSrchSpeed2(),
		ch1stBondImpactBoostEnable(),
		chImpactBoostAmplitude1(),
		chImpactBoostDuration1(),
		fCrtStandbyPowerOffset(){}	//v1.0.T22 add
};

struct SCRUB_PARA_RCD
{
    short nScrubPosOffset1;
    short nScrubPosOffset2;
    short nScrubAmplitude;
    char  chScrubDirection;
    short nScrubCycle;
    short nScrubSpeed;
    short nScrubForce;
    float fScrubPower;
    char  cReserve[30];

    //must init before use, or by default as 0
	SCRUB_PARA_RCD():
		nScrubPosOffset1(),
		nScrubPosOffset2(),
		nScrubAmplitude(),
		chScrubDirection(),
		nScrubCycle(),
		nScrubSpeed(),
		nScrubForce(),
		fScrubPower(){}
};

struct USG_STATIC_PARA_RCD                
{
    short nFeedPower;
    char cFeedPowerTime;
	short nSeatPower; 
    char cSeatPowerTime;
	short nTailPower; 
    char cTailPowerTime;
	char cUSGMode;
    char  cReserve[32];

    //must init before use, or by default as 0
	USG_STATIC_PARA_RCD():
		nFeedPower(),
		cFeedPowerTime(),
		nSeatPower(),
		cSeatPowerTime(),
		nTailPower(),
		cTailPowerTime(),
		cUSGMode(){}
};

struct BOND_PARA_LIMIT
{
    short nStandbyPowerL[2];
    short nStandbyPowerH[2];
    short nCtactTimeL[2];
    short nCtactTimeH[2];
    short nCtactPowerL[2];
    short nCtactPowerH[2];
    short nCtactForceL[2];
    short nCtactForceH[2];
    short nBaseTimeL[2];
    short nBaseTimeH[2];
    short nBasePowerL[2];
    short nBasePowerH[2];
    short nBaseForceL[2];
    short nBaseForceH[2];
    short nPowerDelayL[2];
    short nPowerDelayH[2];
    short nSrchHeightL[2];
    short nSrchHeightH[2];
    short nSrchThreshL[2];
    short nSrchThreshH[2];
    short nPowerFactorL;
    short nPowerFactorH;
    short nForceFactorL;
    short nForceFactorH;
    short nPreCtactForceL[2];
    short nPreCtactForceH[2];
    short nPreCtactTimeL[2];
    short nPreCtactTimeH[2];
    short nSparkDelayL;
    short nSparkDelayH;
    short nTailLengthL;
    short nTailLengthH;
    short nTailLength2L;
    short nTailLength2H;
    short nFireLevelL;
    short nFireLevelH;
    short nSrchSpeedL[2];
    short nSrchSpeedH[2];
    short nRlsForceL[2];		//v1.0.T23 rename
    short nRlsForceH[2];		//v1.0.T23 rename
    short nRlsPowerL[2];		//v1.0.T25 add
    short nRlsPowerH[2];		//v1.0.T25 add
    //char  cReserve[72];		//reduce 2*2*2 bytes from 80
    char  cReserve[64];			//v1.0.T25 add

    //must init before use, or by default as 0
	BOND_PARA_LIMIT():
		nStandbyPowerL(),
		nStandbyPowerH(),
		nCtactTimeL(),
		nCtactTimeH(),
		nCtactPowerL(),
		nCtactPowerH(),
		nCtactForceL(),
		nCtactForceH(),
		nBaseTimeL(),
		nBaseTimeH(),
		nBasePowerL(),
		nBasePowerH(),
		nBaseForceL(),
		nBaseForceH(),
		nPowerDelayL(),
		nPowerDelayH(),
		nSrchHeightL(),
		nSrchHeightH(),
		nSrchThreshL(),
		nSrchThreshH(),
		nPowerFactorL(),
		nPowerFactorH(),
		nForceFactorL(),
		nForceFactorH(),
		nPreCtactForceL(),
		nPreCtactForceH(),
		nPreCtactTimeL(),
		nPreCtactTimeH(),
		nSparkDelayL(),
		nSparkDelayH(),
		nTailLengthL(),
		nTailLengthH(),
		nTailLength2L(),
		nTailLength2H(),
		nFireLevelL(),
		nFireLevelH(),
		nSrchSpeedL(),
		nSrchSpeedH(),
		nRlsForceL(),		//v1.0.T23 rename
		nRlsForceH(),		//v1.0.T23 rename
		nRlsPowerL(),		//v1.0.T25 add
		nRlsPowerH(){}		//v1.0.T25 add
};

struct DUMMY_BOND_PARA
{
    short nSrchSpeed;
    short nSrchTol;
    short nBondTime;
    short nBondPower;
    short nBondForce;
    float fBondPower;
    char  cReserve[16];

    //must init before use, or by default as 0
	DUMMY_BOND_PARA():
		nSrchSpeed(),
		nSrchTol(),
		nBondTime(),
		nBondPower(),
		nBondForce(),
		fBondPower(){}
};

struct DUMMY_BOND_AREA_RCD
{
    bool bBondAreaValid;
    long lCtactLvl;
    TBL_POS stLeftTopPos;
    TBL_POS stRightBottomPos;
    LIGHT_PARA stLightPara;
    char cReserve[100];

    //must init before use, or by default as 0
	DUMMY_BOND_AREA_RCD():
		bBondAreaValid(),
		lCtactLvl(),
		stLeftTopPos(),
		stRightBottomPos(),
		stLightPara(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

#define MAX_SCRUB_GROUP 7

// CBondParaMgr command target

class CBondParaMgr : public CObject
{
public:
	CBondParaMgr();
	virtual ~CBondParaMgr();
public:	//functions
	void VerifyData(void);			//v1.0.T14 add
    BOND_PARA_RCD* GetRcd(void);	//v1.0.T11 add
	void GetUSGParaRcd(USG_STATIC_PARA_RCD* pstUsgPara);	//v1.0.T7 edit
    void SetUSGParaRcd(USG_STATIC_PARA_RCD* pstUsgPara);	//v1.0.T7 edit
    void SaveRcd(void);
    void LoadRcd(void);
    void GetDummyBondPara(DUMMY_BOND_PARA* pstDummyBondPara);
    void SetDummyBondPara(DUMMY_BOND_PARA* pstDummyBondPara);
    void GetDummyBondAreaRcd(DUMMY_BOND_AREA_RCD* pstDummyBondAreaRcd);
    void SetDummyBondAreaRcd(DUMMY_BOND_AREA_RCD* pstDummyBondAreaRcd);
    void GetDefaultBondParaLimit(BOND_PARA_LIMIT* pstBondParaLimit);
    BOND_PARA_LIMIT* GetBondParaLimit(void);	//v1.0.T10 add
    void GetDefaultScrubPara(void);
    void GetBondScrubRcd(short nType, short nMode, SCRUB_PARA_RCD* pstScrubRcd);
    void SetBondScrubRcd(short nType, short nMode, SCRUB_PARA_RCD* pstScrubRcd);
private:	//variables
    BOND_PARA_RCD m_stBondPara;	//v1.0.T9 edit
    SCRUB_PARA_RCD m_ast1stScrub[MAX_SCRUB_GROUP];	//v1.0.T9 edit
    //SCRUB_PARA_RCD m_ast2ndScrub[MAX_SCRUB_GROUP];	//v1.0.T9 edit, v1.0.T14 delete
    DUMMY_BOND_PARA m_stDummyBondPara;
    DUMMY_BOND_AREA_RCD m_stDummyBondArea;	//v1.0.T7 edit
    BOND_PARA_LIMIT m_stBondParaLimit;
	USG_STATIC_PARA_RCD m_stUsgPara;	//v1.0.T7 edit
private:	//functions
    int GetDefaultBondParaRcd(BOND_PARA_RCD* pstBondParaRcd);
	int GetDefaultUsgPara(USG_STATIC_PARA_RCD* pstUsgPara);	//v1.0.T9 edit
	//v1.0.T8 add
    void GetDefaultDummyBondPara(DUMMY_BOND_PARA* pstDummyBondPara);
    void GetDefaultDummyBondArea(DUMMY_BOND_AREA_RCD* pstDummyBondArea);	//v1.0.T7 edit
};

extern CBondParaMgr theBondPara;