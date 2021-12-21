//motion.h v3.1.T575

#pragma once

#define _SERVO_TYPE 1	//0:ACS PCI, 1:ACS SC, 2:ETEL

//v3.1.T30 add
#define MTN_ABS		true
#define MTN_REL		false
#define MTN_WAIT	true
#define MTN_NO_WAIT	false
#define MTN_SLOW	true
#define MTN_FAST	false

enum EN_MOTOR_AXIS	//v3.1.T97 add
{
	MOTOR_X,
	MOTOR_Y,
	MOTOR_Z,
	MOTOR_WCL
};

enum EN_MOTION_DIRECTION	//v3.0.T496 rename
{
	MOTION_NEGATIVE = -1,
	MOTION_POSITIVE = 1
};

enum EN_MOTION_MODE			//v3.0.T496 rename
{
	MOTION_ABSOLUTE,
	MOTION_RELATIVE
};

enum EN_MOTION_SYNC			//v3.0.T496 rename
{
	MOTION_WITH_WAIT,
	MOTION_WITHOUT_WAIT
};

enum EN_MEMORY_VAR	//v3.0.T492 rename
{
    MEMVAR_START_BOND_0,		//Trigger buffer 0 start bonding
    MEMVAR_BOND_PROGRESS,		//Report bond progress to PC
    MEMVAR_VISION_STATUS,		//Inform controller vision status
    MEMVAR_USG_PATTERN,			//USG trigger pattern from controller
    MEMVAR_START_BOND_1,		//Trigger buffer 1 start bonding
    MEMVAR_BOND_STATUS,			//Inform controller bond error status
    MEMVAR_RESTART_BOND,		//Trigger restart bonding
    MEMVAR_BUFFER0_CTACT1,		//Buffer0 1st bond contact level
    MEMVAR_BUFFER0_CTACT2,		//Buffer0 2nd bond contact level
    MEMVAR_BUFFER1_CTACT1,		//Buffer1 1st bond contact level
    MEMVAR_BUFFER1_CTACT2,		//Buffer1 2nd bond contact level
};

enum EN_SERVO_INT_VAR	//v1.6.T141 add, v3.0.T492 rename
{
	BOND_POS1_X,				//0		//v3.1.T277 rename
	BOND_POS1_Y,				//1		//v3.1.T277 rename
	HEAD_TIME,					//2
	SRCH_LVL1,					//3
	SRCH_SPEED1,				//4
	SRCH_TOL1,					//5
	CTACT_FORCE1,				//6
	CTACT_TIME1,				//7
	BASE_FORCE1,				//8
	BASE_TIME1,					//9
	RLS_FORCE1,					//10
	RLS_TIME1,					//11
	REV_HT,						//12	//v3.1.T282 rename
	REV_DIST_X,					//13
	REV_DIST_Y,					//14
	TRAJ_DIST_X,				//15
	KINK2_ENABLE,				//16
	KINK2_HT,					//17	//v3.1.T282 rename
	KINK2_DIST_X,				//18	//v3.1.T282 rename
	KINK2_DIST_Y,				//19	//v3.1.T282 rename
	TRAJ_DIST_Y,				//20
	SLOPE_LENGTH,				//21
	SRCH_SPEED2,				//22
	SRCH_TOL2,					//23
	CTACT_FORCE2,				//24
	CTACT_TIME2,				//25
	BASE_FORCE2,				//26
	BASE_TIME2,					//27
	RLS_FORCE2,					//28
	RLS_TIME2,					//29
	TRAJ_POINT,					//30
	TAIL_HEIGHT,				//31
	RESET_LVL,					//32
	SCOPE_ENABLE,				//33
	FIRST_MOTION_PROFILE_X,		//34
	MOTION_STEP_DEBUG,			//35
	WCL_TAIL_CLOSE_PRETRIG,		//36	//v3.1.T282 rename
	NEXT_WIRE_EXIST,			//37
	SPARK_DELAY,				//38
	BOND_SHIFT2_X,				//39	//v3.1.T273 rename
	BOND_SHIFT2_Y,				//40	//v3.1.T273 rename
	PULL_POS_X,					//41
	PULL_POS_Y,					//42
	TAIL_CREEP_X,				//43
	TAIL_CREEP_Y,				//44
	SRCH_DELAY1,				//45
	SRCH_DELAY2,				//46
	WCL_PEAK_CLOSE_PRETRIG,		//47	//v3.1.T282 rename
	PRE_IMPACT_FORCE1,			//48
	PRE_IMPACT_FORCE2,			//49
	WCL_1ST_BOND_OPEN_PRETRIG,	//50
	WCL_2ND_BOND_OPEN_PRETRIG,	//51
	BOND_SHIFT_SPEED,			//52
	BOND_SHIFT1_X,				//53	//v3.1.T274 rename
	BOND_SHIFT1_Y,				//54	//v3.1.T274 rename
	SCRUB2_ENABLE,				//55	//v3.1.T276 rename
	SCRUB2_SPEED,				//56	//v3.1.T276 rename
	SCRUB2_CYCLE,				//57	//v3.1.T276 rename
	SCRUB2_FORCE,				//58	//v3.1.T276 rename
	SCRUB2_TIME,				//59	//v3.1.T276 rename
	SCRUB2_POS_X,				//60	//v3.1.T276 rename
	SCRUB2_POS_Y,				//61	//v3.1.T276 rename
	SCRUB2_OFFSET_X,			//62	//v3.1.T276 rename
	SCRUB2_OFFSET_Y,			//63	//v3.1.T276 rename
	SCRUB2_AMP_X,				//64	//v3.1.T276 rename
	SCRUB2_AMP_Y,				//65	//v3.1.T276 rename
	BOND_SHIFT1_ENABLE,			//66	//v3.1.T274 rename
	SRCH_LMT1,					//67
	SRCH_LMT2,					//68
	BALL_BOND_ENABLE,			//69
	LOOP_TOP_MODE,				//70
	ABOVE_LOOP_TOP_POS_X,		//71
	ABOVE_LOOP_TOP_POS_Y,		//72
	ABOVE_LOOP_TOP_HEIGHT,		//73
	TABLE_X_CTRL_BLOCK,			//74
	TABLE_Y_CTRL_BLOCK,			//75
	REV_HT_PREMOVE,				//76	//v3.1.T544 rename
	REV_DIST_PREMOVE,			//77	//v3.1.T544 rename
	KINK2_HT_PREMOVE,			//78	//v3.1.T283 rename
	KINK2_DIST_PREMOVE,			//79	//v3.1.T283 rename
	LOOP_TOP_PREMOVE,			//80	//v3.1.T226 rename
	REV_DIST_HT_POINT,			//81	//v3.1.T544 rename
	KINK2_DIST_HT_POINT,		//82	//v3.1.T544 rename
	COPPER_SEQUENCE,			//83
	DAMPGAIN,					//84
	SEARCH_MODE,				//85
	SCRUB1_ENABLE,				//86	//v3.1.T276 rename
	SCRUB1_SPEED,				//87	//v3.1.T276 rename
	SCRUB1_CYCLE,				//88	//v3.1.T276 rename
	SCRUB1_FORCE,				//89	//v3.1.T276 rename
	SCRUB1_TIME,				//90	//v3.1.T276 rename
	SCRUB1_AMP_X,				//91	//v3.1.T276 rename
	SCRUB1_AMP_Y,				//92	//v3.1.T276 rename
	FIRST_MOTION_PROFILE_Y,		//93
	DAMPGAIN2,					//94
	FAB_TEST,					//95
	TAIL_BREAK_DIST,			//96
	TAIL_BREAK_SPEED,			//97
	PRE_IMPACT_TIME1,			//98
	PRE_IMPACT_TIME2,			//99
	PRE_IMPACT_SHIFT_X,			//100
	PRE_IMPACT_SHIFT_Y,			//101
	FAB_TYPE,					//102
	SCRUB1_Y_DELAY,				//103	//v3.1.T276 rename
	TAIL_BOND_ENABLE,			//104
	PRE_KINK_HT,				//105
	PRE_KINK_DIST_X,			//106
	PRE_KINK_DIST_Y,			//107
	PRE_KINK_DIST_HT,			//108
	PRE_KINK_ENABLE,			//109
	KINK3_ENABLE,				//110
	KINK3_HT,					//111	//v3.1.T282 rename
	KINK3_DIST_X,				//112	//v3.1.T282 rename
	KINK3_DIST_Y,				//113	//v3.1.T282 rename
	KINK3_DIST_HT_POINT,		//114	//v3.1.T544 rename
	KINK3_HT_PREMOVE,			//115	//v3.1.T282 rename
	KINK3_DIST_PREMOVE,			//116	//v3.1.T282 rename
	TWIST_ENABLE,				//117
	TWIST_HT,					//118	//v3.1.T282 rename
	TWIST_REV_DIST_X,			//119
	TWIST_REV_DIST_Y,			//120
	TWIST_REV_DIST_Z_POINT,		//121
	TWIST_FWD_DIST_X,			//122
	TWIST_FWD_DIST_Y,			//123
	TWIST_FWD_DIST_Z_POINT,		//124
	REV_HT4_PREMOVE,			//125	//v3.1.T229 rename
	TWIST_FWD_DIST_PREMOVE,		//126	//v3.1.T226 rename
	REV_HT_SPEED,				//127	//v3.1.T282 rename
	REV_DIST_SPEED,				//128	//v3.1.T282 rename
	PRE_KINK_HT_SPEED,			//129	//v3.1.T282 rename
	PRE_KINK_DIST_SPEED,		//130	//v3.1.T282 rename
	KINK2_HT_SPEED,				//131	//v3.1.T282 rename
	KINK2_DIST_SPEED,			//132	//v3.1.T282 rename
	KINK3_HT_SPEED,				//133	//v3.1.T282 rename
	KINK3_DIST_SPEED,			//134	//v3.1.T282 rename
	TWIST_HT_SPEED,				//135	//v3.1.T282 rename
	TWIST_REV_DIST_SPEED,		//136	//v3.1.T282 rename
	TWIST_FWD_DIST_SPEED,		//137	//v3.1.T282 rename
	TRAJ_SPEED,					//138
	PULL_SPEED,					//139
	FAST_LOOP,					//140
	START_BOND,					//141
	RCD_SAMPLE,					//142
	RCD_TOL,					//143
	INT_WIRE_NUM,				//144
	BUMP_TYPE,					//145	//v3.1.T98 rename
	//FORCE_SWITCH,				//146	//v3.1.T341 delete
	PRE_KINK_HT2,				//146	//v3.1.T341 add
	//IMPACT_SPEED1,			//147	//v3.1.T341 delete
	KINK2_HT2,					//147	//v3.1.T341 add
	//IMPACT_SPEED2,			//148	//v3.1.T341 delete
	KINK3_HT2,					//148	//v3.1.T341 add
	//IMPACT_SPEED3,			//149	//v3.1.T337 delete
	ARC_KINK2_DELAY,			//149	//v3.1.T337 add
	//IMPACT_MODE1,				//150	//v3.1.T337 delete
	ARC_KINK3_DELAY,			//150	//v3.1.T337 add
	//IMPACT_MODE2,				//151	//v3.1.T332 delete
	ARC_SPAN_DELAY,				//151	//v3.1.T332 add
	//IMPACT_MODE3,				//152	//v3.1.T328 delete
	ARC_PRE_KINK_DELAY,			//152	//v3.1.T328 add
	SRCH_HT1,					//153
	SRCH_HT2,					//154

	//v1.6.T254 add, v3.1.T315 delete
	//SOFTSRCH1_ENABLE,			//155
	//SOFTSRCH1_SPEED,			//156
	//SOFTSRCH1_POSERR,			//157
	//SOFTSRCH1_POSERR_SERIES,	//158
	//SOFTSRCH2_ENABLE,			//159
	//SOFTSRCH2_SPEED,			//160
	//SOFTSRCH2_POSERR,			//161
	//SOFTSRCH2_POSERR_SERIES,	//162

	//v3.1.T315 add
	RH_POS_GAIN,				//155
	RH_VEL_GAIN,				//156
	RH_VEL_INTEGRATOR,			//157
	RH_AFF,						//158
	RD_POS_GAIN,				//159
	RD_VEL_GAIN,				//160
	RD_VEL_INTEGRATOR,			//161
	RD_AFF,						//162

	TAIL_DELAY,					//163
	//BUF2_PARAM1,				//164	//v3.0.T24 add, v3.1.T318 delete
	TRAJ_FACTOR,				//164	//v3.1.T318 add
	//BUF2_PARAM2,				//165	//v3.0.T24 add, v3.1.T326 delete
	PRE_KINK_DIST_HT_POINT,		//165	//v3.1.T326 add, v3.1.T544 rename
	REBOND_LOOP_HT,				//166	//v3.0.T400 add
	REBOND_BASE_HT,				//167	//v3.0.T400 add
	REBOND_TIME,				//168	//v3.0.T400 add
	REBOND_SPANLEN_X,			//169	//v3.0.T403 add
	REBOND_SPANLEN_Y,			//170	//v3.0.T403 add
	REBOND_FORCE,				//171	//v3.0.T407 add
	PRE_KINK_HT_PREMOVE,		//172	//v3.1.T227 add
	PRE_KINK_DIST_PREMOVE,		//173	//v3.1.T227 add
	SRCH_LVL2,					//174	//v3.1.T260 add
	REV_HT2,					//175	//v3.1.T350 add
	KINK2_DIST2_X,				//176	//v3.1.T375 add
	KINK2_DIST2_Y,				//177	//v3.1.T375 add
	KINK3_DIST2_X,				//178	//v3.1.T375 add
	KINK3_DIST2_Y,				//179	//v3.1.T375 add
	BUMP_SPAN_LEN_DELAY,		//180	//v3.1.T384 add
	ARC_REV_DELAY,				//181	//v3.1.T407 add
	REV_DIST2_X,				//182	//v3.1.T575 add
	REV_DIST2_Y,				//183	//v3.1.T575 add
	PRE_KINK_DIST2_X,			//184	//v3.1.T575 add
	PRE_KINK_DIST2_Y,			//185	//v3.1.T575 add
#if _SERVO_TYPE == 2	//v3.1.T428 add
	BOND1_TRANSIT_TIME,			//186
	BOND1_FILTER_TIME,			//187
	BOND2_TRANSIT_TIME,			//188
	BOND2_FILTER_TIME,			//189
	RLS1_TRANSIT_TIME,			//190
	RLS1_FILTER_TIME,			//191
	RLS2_TRANSIT_TIME,			//192
	RLS2_FILTER_TIME,			//193
	FORCE1_GAIN_THRES,			//194
	FORCE2_GAIN_THRES,			//195
	KINK2_DIST_HT,				//196	//v3.1.T538 add
	KINK3_DIST_HT,				//197	//v3.1.T538 add
	REV_DIST_HT,				//198	//v3.1.T547 add
#endif
	MAX_INT_VARIABLE
};

/*v3.1.T552 delete
struct ST_SERVO_INT_VAR	//v3.0.T491 add, v3.0.T492 rename
{
	int nBondPos1X;					//0
	int nBondPos1Y;					//1
	int nHeadTime;					//2
	int nSrchLvl1;					//3
	int nSrchSpeed1;				//4
	int nSrchTol1;					//5
	int nCtactForce1;				//6
	int nCtactTime1;				//7
	int nBaseForce1;				//8
	int nBaseTime1;					//9
	int nRlsForce1;					//10
	int nRlsTime1;					//11
	int nRevHt;						//12
	int nRevDistX;					//13
	int nRevDistY;					//14
	int nTrajDistX;					//15
	int nKink2Enable;				//16
	int nRevHt2;					//17
	int nRevDist2X;					//18
	int nRevDist2Y;					//19
	int nTrajDistY;					//20
	int nSlopeLen;					//21
	int nSrchSpeed2;				//22
	int nSrchTol2;					//23
	int nCtactForce2;				//24
	int nCtactTime2;				//25
	int nBaseForce2;				//26
	int nBaseTime2;					//27
	int nRlsForce2;					//28
	int nRlsTime2;					//29
	int nTrajPt;					//30
	int nTailHt;					//31
	int nResetLvl;					//32
	int nScopeEnable;				//33
	int n1stMotionProfileX;			//34
	int nMotionStepDebug;			//35
	int nTailWclClosePreTrig;		//36
	int nNextWireExist;				//37
	int nSparkDelay;				//38
	int nBondShiftX;				//39
	int nBondShiftY;				//40
	int nPullPosX;					//41
	int nPullPosY;					//42
	int nTailCreepX;				//43
	int nTailCreepY;				//44
	int nSrchDelay1;				//45
	int nSrchDelay2;				//46
	int nPeakWclClosePreTrig;		//47
	int nPreImpactForce1;			//48
	int nPreImpactForce2;			//49
	int nWcl1stBondOpenPreTrig;		//50
	int nWcl2ndBondOpenPreTrig;		//51
	int nBondShiftSpeed;			//52
	int n1stBondShiftX;				//53
	int n1stBondShiftY;				//54
	int n2ndScrubEnable;			//55
	int n2ndScrubSpeed;				//56
	int n2ndScrubCycle;				//57
	int n2ndScrubForce;				//58
	int n2ndScrubTime;				//59
	int n2ndScrubPosX;				//60
	int n2ndScrubPosY;				//61
	int n2ndScrubOffsetX;			//62
	int n2ndScrubOffsetY;			//63
	int n2ndScrubAmpX;				//64
	int n2ndScrubAmpY;				//65
	int n1stBondShiftEnable;		//66
	int nSrchLimit1;				//67
	int nSrchLimit2;				//68
	int nBallBondEnable;			//69
	int nLoopTopMode;				//70
	int nAboveLoopTopPosX;			//71
	int nAboveLoopTopPosY;			//72
	int nAboveLoopTopHt;			//73
	int nCtrlBlkX;					//74
	int nCtrlBlkY;					//75
	int nNeckHtPreTrig;				//76
	int nNeckDistPreTrig;			//77
	int nRevHt2PreTrig;				//78
	int nRevDist2PreTrig;			//79
	int nLoopTopPreTrig;			//80
	int nNeckDistZPt;				//81
	int nRevDist2ZPt;				//82
	int nCopperSeq;					//83
	int nDampGain;					//84
	int nSrchMode;					//85
	int n1stScrubEnable;			//86
	int n1stScrubSpeed;				//87
	int n1stScrubCycle;				//88
	int n1stScrubForce;				//89
	int n1stScrubTime;				//90
	int n1stScrubAmpX;				//91
	int n1stScrubAmpY;				//92
	int n1stMotionProfileY;			//93
	int nDampGain2;					//94
	int nFabTest;					//95
	int nTailBreakDist;				//96
	int nTailBreakSpeed;			//97
	int nPreImpactTime1;			//98
	int nPreImpactTime2;			//99
	int nPreImpactShiftX;			//100
	int nPreImpactShiftY;			//101
	int nFabType;					//102
	int n1stScrubYDelay;			//103
	int nTailBondEnable;			//104
	int nPreKinkHt;					//105
	int nPreKinkDistX;				//106
	int nPreKinkDistY;				//107
	int nPreKinkDistHt;				//108
	int nPreKinkEnable;				//109
	int nKink3Enable;				//110
	int nRevHt3;					//111
	int nRevDist3X;					//112
	int nRevDist3Y;					//113
	int nRevDist3ZPt;				//114
	int nRevHt3PreTrig;				//115
	int nRevDist3PreTrig;			//116
	int nTwistEnable;				//117
	int nRevHt4;					//118
	int nTwistRevDistX;				//119
	int nTwistRevDistY;				//120
	int nTwistFwdRevDistZPt;		//121
	int nTwistFwdDistX;				//122
	int nTwistFwdDistY;				//123
	int nTwistFwdDistZPt;			//124
	int nRevHt4PreTrig;				//125
	int nTwistFwdDistPreTrig;		//126
	int nRevHt1Speed;				//127
	int nRevDist1Speed;				//128
	int nPreKinkRevHtSpeed;			//129
	int nPreKinkRevDistSpeed;		//130
	int nRevHt2Speed;				//131
	int nRevDist2Speed;				//132
	int nRevHt3Speed;				//133
	int nRevDist3Speed;				//134
	int nRevHt4Speed;				//135
	int nRevTwistSpeed;				//136
	int nFwdTwistSpeed;				//137
	int nTrajSpeed;					//138
	int nPullSpeed;					//139
	int nFastLoop;					//140
	int nStartBond;					//141
	int nRcdSample;					//142
	int nRcdTol;					//143
	int nIntWireNum;				//144
	//int nIntBallType;				//145	//v3.1.T96 delete
	int nBumpType;					//145	//v3.1.T96 add
	int nForceSwitch;				//146
	int nImpactSpeed1;				//147
	int nImpactSpeed2;				//148
	int nImpactSpeed3;				//149
	int nImpactMode1;				//150
	int nImpactMode2;				//151
	int nImpactmode3;				//152
	int nSrchHt1;					//153
	int nSrchHt2;					//154
	int nSoftSrchEnable1;			//155
	int nSoftSrchSpeed1;			//156
	int nSoftSrchPosErr1;			//157
	int nSoftSrchPosErrSeries1;		//158
	int nSoftSrchEnable2;			//159
	int nSoftSrchSpeed2;			//160
	int nSoftSrchPosErr2;			//161
	int nSoftSrchPosErrSeries2;		//162
	int nTailDelay;					//163
	int nBuf2Para1;					//164
	int nBuf2Para2;					//165
	int nRebondLoopHt;				//166
	int nRebondBaseHt;				//167
	int nRebondTime;				//168
	int nRebondSpanLenX;			//169
	int nRebondSpanLenY;			//170
	int RebondForce;				//171
};
*/

enum EN_SERVO_VAR	//v3.0.T495 add, v3.1.T238 rename
{								//etel implementation
    SERVO_VAR_TRAJ,				//not found
    SERVO_VAR_FIRST_MOTION,		//not found
    SERVO_VAR_SRCH_HT,			//not found
    SERVO_VAR_RESET,			//not found
    SERVO_VAR_SPEED_X,			//done
    SERVO_VAR_SPEED_Z,			//done
    SERVO_VAR_CONTROL_Z,		//done
    SERVO_VAR_CONTROL_X,		//done
    SERVO_VAR_CONTROL_Y,		//done
    SERVO_VAR_REV_DIST_HT,		//done	//v3.1.T553 rename
    SERVO_VAR_KINK2_DIST_HT,	//done	//v3.1.T553 rename
    SERVO_VAR_TRAJ_HT,			//done	//v3.1.T553 rename
    SERVO_VAR_SPEED_Y,			//done
    SERVO_VAR_KINK3_DIST_HT,	//done	//v3.1.T553 rename
    SERVO_VAR_TWIST_REV_DIST_Z,	//done
    SERVO_VAR_TWIST_FWD_DIST_Z,	//done
    SERVO_VAR_MZ1,				//for eric buffer
	SERVO_VAR_FORCE_COMPENSATE,	//for eric buffer
	SERVO_VAR_TAIL,				//for eric buffer
    SERVO_VAR_PRE_KINK_DIST_HT,	//v3.1.T326 add, v3.1.T553 rename
    SERVO_VAR_MAX,
};

enum EN_WCL_ACTION	//v3.0.T495 add
{
    WCL_BALL_SIT_OPEN_CLOSE, 
    WCL_SEARCH_HEIGHT_OPEN, 
    WCL_PEAK_HEIGHT_CLOSE, 
    WCL_TAIL_HEIGHT_CLOSE,
    WCL_THREAD_WIRE_OPEN,
    WCL_THREAD_WIRE_CLOSE,
    WCL_HOLD_WIRE_CLOSE,
    WCL_MAX_PROFILE
};

struct ST_WCL_PROFILE	//v3.0.T499 add
{
    short nProfileType;
    short nInterimForce;
    short nInterimRampTime;
    short nInterimHoldTime;
    short nHoldForce;
    short nHoldRamptime;
    short nStep2Force;
    short nStep2RampTime;
    short nStep2HoldTime;
};

//v3.1.T310 add
struct ST_SOFT_POS_LMT
{
	int nMinLmt[3];
	int nMaxLmt[3];
};