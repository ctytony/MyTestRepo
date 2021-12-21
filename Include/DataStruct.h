//DataStruct.h v3.1.T575

#pragma once

//v3.0.T314 add
#define WCL_OPEN_TIME				6000	//6000ms
#define WCL_CALIB_FORCE1			-40		//-50     //gm
#define WCL_CALIB_FORCE2			-75		//-100    //gm
#define BH_CALIB_FORCE1				-50		//gm
#define BH_CALIB_FORCE2				-100	//gm
#define BH_CALIB_FORCE3				-200	//gm
#define BH_FORCE_CALIB_SRCH_TOL		20		//v3.1.T532 edit - 40

//v1.6.T346 add
#define ALGORITHM_BLOB_PLUS_EDGE	0
#define ALGORITHM_EDGE_PLUS_BLOB	1
#define ALGORITHM_BLOB				2
#define ALGORITHM_EDGE				3

//v1.6.T304 add
#define CTACT_WIRE_BUFFER	500		//contact stat wire buffer size

#define DIR_WBDATA			"D:\\WBDATA\\"		//v1.6.T114 edit
#define DIR_WBDATA_VISION	"D:\\WBDATA\\VISION\\"
#define DIR_RECIPE			"D:\\RecipeDir\\"	//v1.6.T114 edit
//v2.0.T111 add
#define DIR_BACKUP			"D:\\WBDATA\\BACKUP\\"
#define DIR_BACKUP_VISION	"D:\\WBDATA\\BACKUP\\VISION\\"
//v2.0.T113 add
#define DIR_MCDATA			"D:\\MCDATA\\"
#define DIR_MCDATA_BACKUP	"D:\\MCDATA\\BACKUP\\"
//v3.0.T171 add
#define DIR_DATALOG			"D:\\DATALOG\\"
#define DIR_DATALOG_VISION	"D:\\DATALOG\\VISION\\"
#define DIR_DATALOG_IMAGE	"D:\\DATALOG\\IMAGE\\"

#define	WM_REFRESH					WM_USER + 100
#define	WM_CLOSEDIALOG				WM_USER + 101	//V1.1.W142 add
#define	WM_ISDIALOGBUSY				WM_USER + 102	//V1.1.W145 add
#define	WM_SAVESETTING				WM_USER + 103	//V1.5.W117 add
#define	DIALOG_CLOSED				0xff			//V1.1.W144 add

#define SYNC_WAITING_TIME			500		//50ms

#define TOP_MAX_INDEX_TIME			8
#define TOP_MAX_MAG_SLOT			60		//30

#define CUELITE_MAX_INDEX_TIME		16		//8->16
#define CUELITE_MAX_MAG_SLOT		60 

#define MAX_BAK_UP_PR_NUM			4

#define	MAG_LEN_LIMIT				939000
#define	MAG_WIDTH_LIMIT				264600
#define	FRONT_TRACK_LIMIT			84200
#define	BACK_TRACK_LIMIT			72400

#define ID_CUELITE  _T("ID_CUELITE")     //machine OFFSET calibration file name
#define WHS_FILE_PATH  _T("D:\\wbdata\\LeadFrame\\")  // _T("D:\\WbData\\WHS\\") //work holder data file 

#pragma pack(push, 1)
#pragma warning(disable : 4351)

enum EN_LF_CLAMP_STATE	//v3.1.T386 rename
{
    WND_CLAMP_OPEN, 
    WND_CLAMP_CLOSE, 
	WND_CLAMP_SAFE_CLAMP,
	WND_CLAMP_PRE_CLOSE,
	WND_CLAMP_OPENING, 
    WND_CLAMP_CLOSING,
	WND_CLAMP_MOVING,
	WND_CLAMP_PRE_CLOSING,
	WND_CLAMP_SAFE_OPENING	
};

struct TBL_POS
{
    double dx;
    double dy;

    //must init before use, or by default as 0
	TBL_POS(double dX = 0, double dY = 0):
	    dx(dX),
		dy(dY){}
};

struct LIGHT_PARA
{
    short nCoaxRedLight;
    byte  bSideRedLight;
    byte  bSideBlueLight;
    short nCoaxBlueLight;
    short nImageThreshold;

    //must init before use, or by default as 0
	LIGHT_PARA():
	    nCoaxRedLight(),
	    bSideRedLight(),
	    bSideBlueLight(),
	    nCoaxBlueLight(),
		nImageThreshold(){}
};

struct BACK_UP_PR
{
	bool        bValid;
	short       nAlgo[2];
	ULONG       lPatID[2];
	short       nSrchSizeX[2];
	short       nSrchSizeY[2];
	LIGHT_PARA  stLight[2];
	TBL_POS     stAlignPos[2];
	double      dAlignSin;
	double      dAlignCos;

    //must init before use, or by default as 0
	BACK_UP_PR():
	    bValid(),
	    nAlgo(),
	    lPatID(),
	    nSrchSizeX(),
	    nSrchSizeY(),
	    stLight(),
	    stAlignPos(),
	    dAlignSin(),
		dAlignCos(){}
};

struct ST_BOND_DATA		//v3.0.T186 rename
{
    short nCurWire;
    short nRestartWire;
    short nCurRow;
    short nCurCol;
    bool bCheckNsop;
    bool bCheckNsol;
    bool bCheckFab;
    bool bDryRun;
    bool bBondNoVision;
    bool bAutoIndex;
    bool bCompleteBond;
    short nCurTrack;
	short nEndWire;
    short nCurMatRow;	//for matrix-hybrid
    short nCurMatCol;	//for matrix-hybrid
    bool bIll;			//ILL
    char chReserve[10];

    //must init before use, or by default as 0
	ST_BOND_DATA():
		nCurWire(),
		nRestartWire(),
		nCurRow(),
		nCurCol(),
		bCheckNsop(),
		bCheckNsol(),
		bCheckFab(),
		bDryRun(),
		bBondNoVision(),
		bAutoIndex(),
		bCompleteBond(),
		nCurTrack(),
		nEndWire(),
		nCurMatRow(),
		nCurMatCol(),
		bIll(){}
};

struct LOOP_PROFILE_DISP_PARA
{
    TBL_POS stBondPos[2];
    TBL_POS stKink1;
    bool bKink2Enable;
    TBL_POS stKink2;
    bool bKink3Enable;
    TBL_POS stKink3;
    bool bKink4Enable;
    TBL_POS stKink4;

    //must init before use, or by default as 0
	LOOP_PROFILE_DISP_PARA():
		stBondPos(),
		stKink1(),
		bKink2Enable(),
		stKink2(),
		bKink3Enable(),
		stKink3(),
		bKink4Enable(),
		stKink4(){}
};

struct ST_WIRE_INFO
{
	short nSrchSpeed1;
	short nSrchSpeed2;
	short nSrchTol1;
	short nSrchTol2;
	short nStandbyPower[2];
	short nCtactTime[2];
	short nCtactPower[2];
	short nCtactForce[2];
	short nPowerDelay[2];
	short nBaseTime[2];
	short nBasePower[2];
	short nBaseForce[2];
	short nReleaseTime[2];
	short nReleasePower[2];
	short nReleaseForce[2];
	bool  bPreKinkEnable;
	bool  bKink2Enable;
	bool  bKink3Enable;
	bool  bTwistEnable;		//v3.1.T284 rename
	char  cLoopTopMode;		//v3.1.T279 rename
	int   nCtactLvl1;		//v3.1.T373 edit
	int   nCtactLvl2;		//v3.1.T373 edit
	int   nSrchLvl1;		//v1.6.T150 edit, v3.1.T373 edit
	int   nRevHt;			//v3.1.T284 rename, v3.1.T373 edit
	int   nRevDistHt;		//v3.1.T284 rename, v3.1.T373 edit
	int   nPreKinkHt;		//v3.1.T284 rename, v3.1.T373 edit
	int   nPreKinkDistHt;	//v3.1.T284 rename, v3.1.T373 edit
	int   nKink2Ht;			//v3.1.T284 rename, v3.1.T373 edit
	int   nKink2DistHt;		//v3.1.T284 rename, v3.1.T373 edit
	int   nKink3Ht;			//v3.1.T284 rename, v3.1.T373 edit
	int   nKink3DistHt;		//v3.1.T284 rename, v3.1.T373 edit
	int   nTwistHt;			//v3.1.T284 rename, v3.1.T373 edit
	int   nTwistRevDistHt;	//v3.1.T284 rename, v3.1.T373 edit
	int   nTwistFwdDistHt;	//v3.1.T284 rename, v3.1.T373 edit
	int   nSlopeLen;		//v3.1.T373 edit
	int   nAboveLoopTopHt;	//v3.1.T373 edit
	//long  lTrajHeight;	//v3.1.T373 delete
	int   nTailHt;			//v3.1.T373 edit
	int   nResetLvl;		//v3.1.T373 edit
	TBL_POS stBondShiftPos1;	//v3.1.T275 rename
	TBL_POS stBondPos[2];
	TBL_POS stRevPos;		//v3.1.T547 rename
	TBL_POS stPreKinkPos;
	TBL_POS stKink2Pos;
	TBL_POS stKink3Pos;
	TBL_POS stTwistRevKinkPos;
	TBL_POS stTwistFwdKinkPos;
	TBL_POS stLoopTopPos;
	TBL_POS stAboveLoopTopPos;
	TBL_POS stTrajEndPos;
	TBL_POS stPullPos;
	TBL_POS stPreShiftPos;
	TBL_POS stBondShiftPos2;	//v3.1.T275 rename
	TBL_POS stTailShiftPos;
	short   nTrajPoint;
	double  adTrajPos[1500];
	short   nFirstMotionProfileX;
	short   nFirstMotionProfileY;
	short   nSrchHtTime;
	short   nNextWireExist;
	short   nSparkDelay;
	short   nWclTailClosePretrig;
	short   nSrchDelay[2];
	short   nWclLoopPeakClosePretrig;
	short   nPreCtactForce[2];
	short   nPreCtactTime[2];
	short   nWcl1stBondOpenPretrig;
	short   nWcl2ndBondOpenPretrig;
	short   nWclLoopPeakCloseTime;
	bool    b2ndBondScrubEnable;
	TBL_POS st2ndScrubPos;
	TBL_POS st2ndScrubOffset2;
	TBL_POS st2ndScrubAmp;
	short   n2ndScrubCycle;
	short   n2ndScrubForce;
	short   n2ndScrubPower;
	short   n2ndScrubOffsetTime;
	short   n2ndScrubTime;
	short   n2ndScrubSpeed;
	bool    bBondShift1Enable;	//v3.1.T275 rename
	short   nShiftSpeed;
	short   nBumpType;	//v3.1.T96 edit
	short   nCtactPowerRamp[2];
	short   nBasePowerRamp[2];
	short   nReleasePowerRamp[2];
	bool    bScopeEnable;
	short   nFeedPower;
	short   nFeedPowerTime;
	short   nRevHtPremove;			//v3.1.T543 rename
	short   nRevDistPremove;		//v3.1.T543 rename
	short   nKink2HtPremove;		//v3.1.T229 rename, v3.1.T394 rename
	short   nKink2DistPremove;		//v3.1.T229 rename, v3.1.T394 rename
	short   nKink3HtPremove;		//v3.1.T229 rename, v3.1.T394 rename
	short   nKink3DistPremove;		//v3.1.T229 rename, v3.1.T394 rename
	short   nTwistHtPremove;		//v3.1.T229 rename, v3.1.T394 rename
	short   nTwistFwdDistPremove;	//v3.1.T229 rename
	short   nLoopTopPremove;		//v3.1.T229 rename
	short   nRevDistHtPoint;		//v3.1.T542 rename
	double  adRevDistHtMotion[500];	//v3.1.T542 rename
	short	nArcRevDelay;
	short	nArcPreKinkDelay;			//v3.1.T328 add
	short   nPreKinkDistHtPoint;		//v3.1.T326 add, v3.1.T541 rename
	double  adPreKinkDistHtMotion[500];	//v3.1.T326 add, v3.1.T541 rename
	short	nArcKink2Delay;				//v3.1.T337 add
	short   nKink2DistHtPoint;			//v3.1.T539 rename
	double  adKink2DistHtMotion[500];	//v3.1.T539 rename
	short	nArcKink3Delay;				//v3.1.T337 add
	short   nKink3DistHtPoint;			//v3.1.T540 rename
	double  adKink3DistHtMotion[500];	//v3.1.T540 rename
	short   nTwistRevDistZPoint;
	double  adTwistRevDistZMotion[500];
	short   nTwistFwdDistZPoint;
	double  adTwistFwdDistZMotion[500];
	short   n1stCtactBoostPower;
	short   n1stCtactBoostTime;
	short   n1stBaseBoostPower;
	short   n1stBaseBoostTime;
	bool    b1stBondScrubEnable;
	TBL_POS st1stScrubAmp;
	short   n1stScrubCycle;
	short   n1stScrubForce;
	short   n1stScrubPower;
	short   n1stScrubTime;
	short   n1stScrubSpeed;
	short   n1stScrubYDelay;
	short   nTailBreakDist;
	short   nTailBreakSpeed;
	char    chFabType;
	short   nKink1WireFeedSpeed;
	short   nKink1WireBentSpeed;
	short   nPreKinkWireFeedSpeed;
	short   nPreKinkWireBentSpeed;
	short   nKink2WireFeedSpeed;
	short   nKink2WireBentSpeed;
	short   nKink3WireFeedSpeed;
	short   nKink3WireBentSpeed;
	short   nKink4WireFeedSpeed;
	short   nTwistRevBentSpeed;
	short   nTwistFwdBentSpeed;
	short   nTrajectorySpeed;
	short   nPullSpeed;
	bool    bFastLoop;
	int     nSrchHt1;		//v1.6.T151 add
	int     nSrchHt2;		//v1.6.T151 add
	TBL_POS stRebondPos;	//v3.0.T403 add
	int		nSrchLvl2;		//v3.1.T260 add, v3.1.T373 edit
	short	nTrajFactor;	//v3.1.T318 add - unit percent
	int		nPreKinkHt2;	//v3.1.T341 add, v3.1.T373 edit
	int		nKink2Ht2;		//v3.1.T341 add, v3.1.T373 edit
	int		nKink3Ht2;		//v3.1.T341 add, v3.1.T373 edit
	int		nRevHt2;		//v3.1.T350 add, v3.1.T373 edit
	TBL_POS stKink2Dist2;	//v3.1.T375 add
	TBL_POS stKink3Dist2;	//v3.1.T375 add
	//v3.1.T565 add
	short   nTrajPointZ;
	double  adTrajPosZ[500];
	TBL_POS stRevDist2;		//v3.1.T575 add
	TBL_POS stPreKinkDist2;	//v3.1.T575 add

	//must init before use, or by default as 0
	ST_WIRE_INFO():
		nSrchSpeed1(),
		nSrchSpeed2(),
		nSrchTol1(),
		nSrchTol2(),
		nStandbyPower(),
		nCtactTime(),
		nCtactPower(),
		nCtactForce(),
		nPowerDelay(),
		nBaseTime(),
		nBasePower(),
		nBaseForce(),
		nReleaseTime(),
		nReleasePower(),
		nReleaseForce(),
		bPreKinkEnable(),
		bKink2Enable(),
		bKink3Enable(),
		bTwistEnable(),		//v3.1.T284 rename
		cLoopTopMode(),		//v3.1.T279 rename
		nCtactLvl1(),		//v3.1.T373 edit
		nCtactLvl2(),		//v3.1.T373 edit
		nSrchLvl1(),		//v1.6.T150 edit, v3.1.T373 edit
		nRevHt(),			//v3.1.T284 rename, v3.1.T373 edit
		nRevDistHt(),		//v3.1.T284 rename, v3.1.T373 edit
		nPreKinkHt(),		//v3.1.T284 rename, v3.1.T373 edit
		nPreKinkDistHt(),	//v3.1.T284 rename, v3.1.T373 edit
		nKink2Ht(),			//v3.1.T284 rename, v3.1.T373 edit
		nKink2DistHt(),		//v3.1.T284 rename, v3.1.T373 edit
		nKink3Ht(),			//v3.1.T284 rename, v3.1.T373 edit
		nKink3DistHt(),		//v3.1.T284 rename, v3.1.T373 edit
		nTwistHt(),			//v3.1.T284 rename, v3.1.T373 edit
		nTwistRevDistHt(),	//v3.1.T284 rename, v3.1.T373 edit
		nTwistFwdDistHt(),	//v3.1.T284 rename, v3.1.T373 edit
		nSlopeLen(),		//v3.1.T373 edit
		nAboveLoopTopHt(),	//v3.1.T373 edit
		//lTrajHeight(),	//v3.1.T373 delete
		nTailHt(),			//v3.1.T373 edit
		nResetLvl(),		//v3.1.T373 edit
		stBondShiftPos1(),	//v3.1.T275 rename
		stBondPos(),
		stRevPos(),			//v3.1.T547 rename
		stPreKinkPos(),
		stKink2Pos(),
		stKink3Pos(),
		stTwistRevKinkPos(),
		stTwistFwdKinkPos(),
		stLoopTopPos(),
		stAboveLoopTopPos(),
		stTrajEndPos(),
		stPullPos(),
		stPreShiftPos(),
		stBondShiftPos2(),	//v3.1.T275 rename
		stTailShiftPos(),
		nTrajPoint(),
		adTrajPos(),
		nFirstMotionProfileX(),
		nFirstMotionProfileY(),
		nSrchHtTime(),
		nNextWireExist(),
		nSparkDelay(),
		nWclTailClosePretrig(),
		nSrchDelay(),
		nWclLoopPeakClosePretrig(),
		nPreCtactForce(),
		nPreCtactTime(),
		nWcl1stBondOpenPretrig(),
		nWcl2ndBondOpenPretrig(),
		nWclLoopPeakCloseTime(),
		b2ndBondScrubEnable(),
		st2ndScrubPos(),
		st2ndScrubOffset2(),
		st2ndScrubAmp(),
		n2ndScrubCycle(),
		n2ndScrubForce(),
		n2ndScrubPower(),
		n2ndScrubOffsetTime(),
		n2ndScrubTime(),
		n2ndScrubSpeed(),
		bBondShift1Enable(),	//v3.1.T275 rename
		nShiftSpeed(),
		nBumpType(),	//v3.1.T96 edit
		nCtactPowerRamp(),
		nBasePowerRamp(),
		nReleasePowerRamp(),
		bScopeEnable(),
		nFeedPower(),
		nFeedPowerTime(),
		nRevHtPremove(),			//v3.1.T543 rename
		nRevDistPremove(),			//v3.1.T543 rename
		nKink2HtPremove(),			//v3.1.T394 rename
		nKink2DistPremove(),		//v3.1.T394 rename
		nKink3HtPremove(),			//v3.1.T394 rename
		nKink3DistPremove(),		//v3.1.T394 rename
		nTwistHtPremove(),			//v3.1.T394 rename
		nTwistFwdDistPremove(),
		nLoopTopPremove(),
		nRevDistHtPoint(),			//v3.1.T542 rename
		adRevDistHtMotion(),		//v3.1.T542 rename
		nArcRevDelay(),
		nArcPreKinkDelay(),			//v3.1.T328 add
		nPreKinkDistHtPoint(),		//v3.1.T326 add, v3.1.T541 rename
		adPreKinkDistHtMotion(),	//v3.1.T326 add, v3.1.T541 rename
		nArcKink2Delay(),			//v3.1.T337 add
		nKink2DistHtPoint(),		//v3.1.T539 rename
		adKink2DistHtMotion(),		//v3.1.T539 rename
		nArcKink3Delay(),			//v3.1.T337 add
		nKink3DistHtPoint(),		//v3.1.T540 rename
		adKink3DistHtMotion(),		//v3.1.T540 rename
		nTwistRevDistZPoint(),
		adTwistRevDistZMotion(),
		nTwistFwdDistZPoint(),
		adTwistFwdDistZMotion(),
		n1stCtactBoostPower(),
		n1stCtactBoostTime(),
		n1stBaseBoostPower(),
		n1stBaseBoostTime(),
		b1stBondScrubEnable(),
		st1stScrubAmp(),
		n1stScrubCycle(),
		n1stScrubForce(),
		n1stScrubPower(),
		n1stScrubTime(),
		n1stScrubSpeed(),
		n1stScrubYDelay(),
		nTailBreakDist(),
		nTailBreakSpeed(),
		chFabType(),
		nKink1WireFeedSpeed(),
		nKink1WireBentSpeed(),
		nPreKinkWireFeedSpeed(),
		nPreKinkWireBentSpeed(),
		nKink2WireFeedSpeed(),
		nKink2WireBentSpeed(),
		nKink3WireFeedSpeed(),
		nKink3WireBentSpeed(),
		nKink4WireFeedSpeed(),
		nTwistRevBentSpeed(),
		nTwistFwdBentSpeed(),
		nTrajectorySpeed(),
		nPullSpeed(),
		bFastLoop(),
		nSrchHt1(),			//v1.6.T151 add
		nSrchHt2(),			//v1.6.T151 add
		stRebondPos(),		//v3.0.T403 add
		nSrchLvl2(),		//v3.1.T260 add, v3.1.T373 edit
		nTrajFactor(),		//v3.1.T318 add
		nPreKinkHt2(),		//v3.1.T341 add, v3.1.T373 edit
		nKink2Ht2(),		//v3.1.T341 add, v3.1.T373 edit
		nKink3Ht2(),		//v3.1.T341 add, v3.1.T373 edit
		nRevHt2(),			//v3.1.T350 add, v3.1.T373 edit
		stKink2Dist2(),		//v3.1.T375 add
		stKink3Dist2(),		//v3.1.T375 add
		//v3.1.T565 add
		nTrajPointZ(),
		adTrajPosZ(),
		stRevDist2(),		//v3.1.T575 add
		stPreKinkDist2(){}	//v3.1.T575 add
};

struct ST_BALL_INFO
{
    short nSrchSpeed1;
    short nSrchSpeed2;
    short nSrchTol1;
    short nSrchTol2;
    short nStandbyPower[2];
    short nCtactTime[2];
    short nCtactPower[2];
    short nCtactForce[2];
    short nPowerDelay[2];
    short nBaseTime[2];
    short nBasePower[2];
    short nBaseForce[2];
    short nReleaseTime[2];
    short nReleasePower[2];
    short nReleaseForce[2];
    int   nCtactLvl1;		//v3.1.T373 edit
    int   nCtactLvl2;		//v3.1.T373 edit
    int   nSrchLvl1;		//v3.1.T261 rename, v3.1.T373 edit
    short nSrchHtTime;
    int   nBaseHt;			//v3.1.T373 edit
    int   nSpanHt;			//v3.1.T373 edit
    int   nBaseHt2;			//v3.1.T330 rename, v3.1.T373 edit
    int   nFlexSpanHt;		//v3.1.T373 edit
    int   nFlexHt;			//v3.1.T373 edit
    short nMotionSpeed;
    int   nTailHt;			//v3.1.T373 edit
    int   nResetLvl;		//v3.1.T373 edit
    TBL_POS stBondPos[2];
    TBL_POS stFlexPos;
    TBL_POS stScrubPos;
    short   nFirstMotionProfileX;
    short   nFirstMotionProfileY;
    short   nNextWireExist;
    short   nSparkDelay;
    short   nWclTailClosePretrig;
    short   nSrchDelay[2];
    short   nWclLoopPeakClosePretrig;
    short   nPreCtactForce[2];
    short   nPreCtactTime[2];
    short   nWcl1stBondOpenPretrig;
    short   nWcl2ndBondOpenPretrig;
    short   nWclLoopPeakCloseTime;
    short   nBumpType;	//v3.1.T96 edit
    short   nCtactPowerRamp[2];
    short   nBasePowerRamp[2];
    short   nReleasePowerRamp[2];
    bool    bScopeEnable;
    short   nTailBreakDist;
    short   nTailBreakSpeed;
    char    chFabType;
    bool    b1stBondScrubEnable;
    TBL_POS st1stScrubAmp;
    short   n1stScrubCycle;
    short   n1stScrubForce;
    short   n1stScrubPower;
    short   n1stScrubTime;
    short   n1stScrubSpeed;
    short   n1stScrubYDelay;
    bool    bTailBondEnable;
    char    chBumpShapeMode;
    short   nBumpCutPower;
    short   nBumpCutPowerDelay;
    short   nBumpCutPowerTime;
    short   nSpanDistZPoint;
    double  adSpanDistZMotion[500];
	int		nSrchLvl2;		//v3.1.T262 add, v3.1.T373 edit
	short	nArcSpanDelay;	//v3.1.T332 add
	short	nSpanLenDelay;	//v3.1.T384 add

    //must init before use, or by default as 0
	ST_BALL_INFO():
		nSrchSpeed1(),
		nSrchSpeed2(),
		nSrchTol1(),
		nSrchTol2(),
		nStandbyPower(),
		nCtactTime(),
		nCtactPower(),
		nCtactForce(),
		nPowerDelay(),
		nBaseTime(),
		nBasePower(),
		nBaseForce(),
		nReleaseTime(),
		nReleasePower(),
		nReleaseForce(),
		nCtactLvl1(),		//v3.1.T373 edit
		nCtactLvl2(),		//v3.1.T373 edit
		nSrchLvl1(),		//v3.1.T261 rename, v3.1.T373 edit
		nSrchHtTime(),
		nBaseHt(),			//v3.1.T373 edit
		nSpanHt(),			//v3.1.T373 edit
		nBaseHt2(),			//v3.1.T330 rename, v3.1.T373 edit
		nFlexSpanHt(),		//v3.1.T373 edit
		nFlexHt(),			//v3.1.T373 edit
		nMotionSpeed(),
		nTailHt(),			//v3.1.T373 edit
		nResetLvl(),		//v3.1.T373 edit
		stBondPos(),
		stFlexPos(),
		stScrubPos(),
		nFirstMotionProfileX(),
		nFirstMotionProfileY(),
		nNextWireExist(),
		nSparkDelay(),
		nWclTailClosePretrig(),
		nSrchDelay(),
		nWclLoopPeakClosePretrig(),
		nPreCtactForce(),
		nPreCtactTime(),
		nWcl1stBondOpenPretrig(),
		nWcl2ndBondOpenPretrig(),
		nWclLoopPeakCloseTime(),
		nBumpType(),	//v3.1.T96 edit
		nCtactPowerRamp(),
		nBasePowerRamp(),
		nReleasePowerRamp(),
		bScopeEnable(),
		nTailBreakDist(),
		nTailBreakSpeed(),
		chFabType(),
		b1stBondScrubEnable(),
		st1stScrubAmp(),
		n1stScrubCycle(),
		n1stScrubForce(),
		n1stScrubPower(),
		n1stScrubTime(),
		n1stScrubSpeed(),
		n1stScrubYDelay(),
		bTailBondEnable(),
		chBumpShapeMode(),
		nBumpCutPower(),
		nBumpCutPowerDelay(),
		nBumpCutPowerTime(),
		nSpanDistZPoint(),
		adSpanDistZMotion(),
		nSrchLvl2(),		//v3.1.T262 add, v3.1.T373 edit
		nArcSpanDelay(),	//v3.1.T332 add
		nSpanLenDelay(){}	//v3.1.T384 add
};

struct PR_SRCH_RESULT
{
    double dOffsetX;
    double dOffsetY;
    double dScore;
    double dScale;
    double dAngle;

    //must init before use, or by default as 0
	PR_SRCH_RESULT():
		dOffsetX(),
		dOffsetY(),
		dScore(),
		dScale(),
		dAngle(){}
};

struct MHS_POWER_ON_DATA
{
    short nRightClawTick;
    short nLeftClawTick;
	short nBondingCounter;
	short nOffloadForkBuffer;
	BOOL bBufferSwitchFlag; 
    char  chReserve[194];

    //must init before use, or by default as 0
	MHS_POWER_ON_DATA():
		nRightClawTick(),
		nLeftClawTick(),
		nBondingCounter(),
		nOffloadForkBuffer(),
		bBufferSwitchFlag(){}
};

struct LEADFRAME_ONLOAD_RCD
{
    short   nPickerDelayTime;
    short   nPushStepSpeed;
    short   nDropLfDelayTime;
	long    lOvershootLen;
	short   bOnloaderType;
   	char    chRev[26];
  
    //must init before use, or by default as 0
	LEADFRAME_ONLOAD_RCD():
		nPickerDelayTime(),
		nPushStepSpeed(),
		nDropLfDelayTime(),
		lOvershootLen(),
		bOnloaderType(){}
};

struct SINGLE_CAP_TRACK_RCD
{
	short   nLeadFrameUnits;
	long    lDbLfCheckPos;
    long    lInjectPos;
    short   nInitUnitNum;
	short   nIndexSteps;
	short   nOutputSteps;
	long    lDbCheckSpeed;
	short   nEndBondOffset;
	short   nPreheatTime;
	long    lClawCatchUpSpeed;
	long    lClawIndexSpeed;
	short   nInjectBackDelay;
	short   nClawActionRate;
    char    chBondUnitType;
    short   nAvoidCrashPulse;
	short   nNormalLFPreheatTime;
	short   nAvoidCrashCompen;
    char    chReserve[37];

    //must init before use, or by default as 0
	SINGLE_CAP_TRACK_RCD():
		nLeadFrameUnits(),
		lDbLfCheckPos(),
		lInjectPos(),
		nInitUnitNum(),
		nIndexSteps(),
		nOutputSteps(),
		lDbCheckSpeed(),
		nEndBondOffset(),
		nPreheatTime(),
		lClawCatchUpSpeed(),
		lClawIndexSpeed(),
		nInjectBackDelay(),
		nClawActionRate(),
		chBondUnitType(),
		nAvoidCrashPulse(),
		nNormalLFPreheatTime(),
		nAvoidCrashCompen(){}
};

struct SINGLE_CAP_CLAMP_RCD
{
    long    lOpenPosition;
    long    lClosePosition;
	short   nOpenPretrigRate;
	short   nClampPretrigRate;
	char    chRev[32];

    //must init before use, or by default as 0
	SINGLE_CAP_CLAMP_RCD():
		lOpenPosition(),
		lClosePosition(),
		nOpenPretrigRate(),
		nClampPretrigRate(){}
};

struct TOP_SLIP_VAR            //Power on dat
{
	bool      bSlipSrchDone;     
	long      lSlipHeaderPos;
	long      lSlipTailPos;
	long      lSlipOutputTailPos;
	bool      bCompenDone;	

    //must init before use, or by default as 0
	TOP_SLIP_VAR():
		bSlipSrchDone(),
		lSlipHeaderPos(),
		lSlipTailPos(),
		lSlipOutputTailPos(),
		bCompenDone(){}
};

struct TOP_MHS_POWER_ON_DATA
{
	TOP_SLIP_VAR  m_stSlipOnFront;
	short         nCurrentUnit;
	short         nOnloadCurrentSlot;
	short         nOffloadCurrentSlot;
	bool          bOnloadReady;
	char       chReserve[122];

    //must init before use, or by default as 0
	TOP_MHS_POWER_ON_DATA():
		m_stSlipOnFront(),
		nCurrentUnit(),
		nOnloadCurrentSlot(),
		nOffloadCurrentSlot(),
		bOnloadReady(){}
};

//-----------------------------CUELITE_MHS--------------------------------

struct CUELITE_TRACK_DATA
{
	short      lDelayType;
	unsigned long lHeatingDelay;
	byte       nPrIndexEnable;
	long       lLimitTolerance;			//distance to push slip into output magazine
	long       lSrchBackLen;
	long       lLfStandbyLen;			//distance in between slips
    long       nIndexSpeedCoeff;		//1,2,3,4,5,6,7
	short      lIndexError[CUELITE_MAX_INDEX_TIME];
	short      nEjectSpeedCoeff;		// 1,2,3,4,5
	long       l1stSlipHeatDelay;
	long       lPrIndexTolerance;
	long       lIndexPosOffset;
	long       lOpenClampDelay;
	long       lIndexPrDelay;
	unsigned long       lInitDelay;
    bool                bVacuumControl;
	unsigned long       lVacuumDelay;
	unsigned long       l1stUnitHeatDelay;
	int        iLFTrackRange;
	long       lLFFrontTrackStandby;		//1um
	long       lLFBackTrackStandby;			//1um
	long       lWindowClamperOverHeatTime;	//ms
	short      sPRIndexScore;
	BYTE       nNextStepPusherIn;
	int		   iOutputMagazineSlotSequence;
	long       lClawDelay;

	//this is the BLOCK size, reserve for future
	//MUST reduce this size if add any variable 
	//and variable must be added in sequence
	char       chReserve[140];  //[167]-->[151]->[149]->[148]->[144]->[140]

    //must init before use, or by default as 0
	CUELITE_TRACK_DATA():
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
		iLFTrackRange(),
		lLFFrontTrackStandby(),
		lLFBackTrackStandby(),		
		lWindowClamperOverHeatTime(),
		sPRIndexScore(),
		nNextStepPusherIn(),
		iOutputMagazineSlotSequence(),
		lClawDelay(){}
};

struct CUELITE_ONLOAD_RCD
{
	long        lSlotOffset[CUELITE_MAX_MAG_SLOT];
	long        lSlotPitch;
	long        lSwapMagPitch;
	long        lOnload1stSlotLevel;				//190000
	long        lOnLoadMagLengthStandby;	        //1um      
	long        lOnLoadMagClamperFrontStandby;      //1um
	long        lOnLoadMagClamperBackStandby;       //1um
	long        lOnLoadMagClamperDepth;             //1um	Front_Track relative depth from Standby postion
	long        lOnLoadMagLenLimit1;                //1um
	long        lOnLoadMagLenLimit2;                //1um
	long        lOnLoadMagLenLimit3;                //1um

	//this is the BLOCK size, reserve for future
	//MUST reduce this size if add any variable 
	// and varibable must be added be added in sequence
	char        chReserve[50];

	//must init before use, or by default as 0
	CUELITE_ONLOAD_RCD():
	lSlotOffset(),
		lOnLoadMagLengthStandby(),
		lOnLoadMagClamperFrontStandby(),
		lOnLoadMagClamperBackStandby(),
		lOnLoadMagClamperDepth(),
		lOnLoadMagLenLimit1(),
		lOnLoadMagLenLimit2(),
		lOnLoadMagLenLimit3(),
		lSlotPitch(),		
		lSwapMagPitch(),
		lOnload1stSlotLevel(){}
};

struct CUELITE_OFFLOAD_RCD
{
	long        lSlotOffset[CUELITE_MAX_MAG_SLOT];	//60
	long        lSlotPitch;
	long        lSwapMagPitch;
	long        lOffload1stSlotLevel;				//190000
	long        lOffLoadMagLengthStandby;	        //1um      
	long        lOffLoadMagClamperFrontStandby;     //1um
	long        lOffLoadMagClamperBackStandby;      //1um
	long        lOffLoadMagClamperDepth;            //1um    Front_Track relative depth from Standby postion
	long        lOffLoadMagLenLimit1;               //1um
	long        lOffLoadMagLenLimit2;               //1um
	long        lOffLoadMagLenLimit3;               //1um

	//this is the BLOCK size, reserve for future
	//MUST reduce this size if add any variable 
	// and varibable must be added be added in sequence
	char        chReserve[50];

	//must init before use, or by default as 0
	CUELITE_OFFLOAD_RCD():
	lSlotOffset(),
		lOffLoadMagLengthStandby(),
		lOffLoadMagClamperFrontStandby(),
		lOffLoadMagClamperBackStandby(),
		lOffLoadMagClamperDepth(),
		lOffLoadMagLenLimit1(),
		lOffLoadMagLenLimit2(),
		lOffLoadMagLenLimit3(),
		lSlotPitch(),		
		lSwapMagPitch(),
		lOffload1stSlotLevel(){}
};

struct CUELITE_SLIP_VAR            //Power on dat
{
	bool      bSlipSrchDone;     
	long      lSlipHeaderPos;
	long      lSlipTailPos;
	long      lSlipOutputTailPos;
	long      lNewSlipHeaderPos;
	bool      bCompenDone;

    //must init before use, or by default as 0
	CUELITE_SLIP_VAR():
		bSlipSrchDone(),
		lSlipHeaderPos(),
		lSlipTailPos(),
		lSlipOutputTailPos(),
		lNewSlipHeaderPos(),		
		bCompenDone(){}
};

struct CUELITE_STEPPER_CLAMP_PARA
{
	long        lWindClampSafeClosePos;
	long        lWindClampPreClosePos;
    long        lWindClampClosePos;
	long        lDriveVelocity;	            //velocity of window clamper

	//this is the BLOCK size, reserve for future
	//MUST reduce this size if add any variable 
	// and varibable must be added be added in sequence
	char        chReserve[84];

    //must init before use, or by default as 0
	CUELITE_STEPPER_CLAMP_PARA():
		lWindClampSafeClosePos(),
		lWindClampPreClosePos(),
		lWindClampClosePos(),
		lDriveVelocity(){}
};

struct CUELITE_MHS_POWER_ON_DATA
{
	CUELITE_SLIP_VAR  m_stSlipOnFront;
	short             nCurrentUnit;
	short             nOnloadCurrentSlot;
	short             nOffloadCurrentSlot;
	bool              bOnloadReady;
	char              chReserve[122];

    //must init before use, or by default as 0
	CUELITE_MHS_POWER_ON_DATA():
		m_stSlipOnFront(),
		nCurrentUnit(),
		nOnloadCurrentSlot(),
		nOffloadCurrentSlot(),
		bOnloadReady(){}
};

#pragma warning(default : 4351)
#pragma pack(pop)
