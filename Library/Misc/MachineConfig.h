//MachineConfig.h v1.1.T24

#pragma once

#include "DataStruct.h"
#include "MacDef.h"
#include "Motion.h"
#include "LoopPara.h"

#define WIRE_NUM 100		//v1.0.T7 add
#define WIRE_GROUP_MAX 15	//v1.0.T89 add

enum EN_MHS_TYPE	//v1.0.T129 rename
{
	MHS_NONE = -1,
    MHS_VLED = 0,
    MHS_TOP_LED,
    MHS_CU_ELITE,
	MHS_R2R,
	MHS_FOXCONN,	//v1.0.T132 add
	MHS_VLED_ELITE,
	MHS_TOP_ELITE,	//v1.0.T171 add
	MHS_MAX,
};

enum EN_STEPPER_TYPE	//v1.0.T83 edit
{
    STEP_TYPE_1240,
    STEP_TYPE_2812,
	STEP_TYPE_STEPPERIO
};

enum EN_CAMERA_TYPE		//v1.0.T83 edit
{
	CAMERA_MIL_PCI,
	CAMERA_MSZ300_USB,
	CAMERA_DAHUA_GIGE,	//v1.0.T27 add
	CAMERA_DAHENG_USB3,	//v1.0.T128 add
};

enum EN_VISION_LIBRARY	//v1.0.T83 edit
{
	VISLIB_MATROX,
	VISLIB_HALCON,
};

enum EN_TOP_ONLOAD_TYPE		//v1.0.T83 edit
{
	TOP_WITH_INJECTOR,
	TOP_WITH_PUSH_KICKER,
};

enum EN_CUELITE_ONLOAD_TYPE	//v1.0.T83 edit
{
	CUELITE_WITH_INJECTOR,
	CUELITE_WITH_PUSH_KICKER,
};

enum EN_ALIGN_SENSOR_TYPE	//v1.0.T83 edit
{
	BACK_ALIGN,
	FRONT_ALIGN,
};

struct ST_LIMIT_RANGE	//v1.0.T83 edit
{
	BYTE nHighLimit;
	BYTE nLowLimit;
};

class CMachineConfig : public CObject
{
public:
	CMachineConfig();
	virtual ~CMachineConfig();
public:		//function
	short GetSkipLastCol();								//v1.1.T24 add
	short GetGapLargeTimeHighLimit();					//v1.1.T23 add
	bool IsLoopTopPremove();							//v1.1.T22 add
	short GetSwitchForceDelay();						//v1.1.T20 add
	short GetSwitchForcePercent();						//v1.1.T18 add
	short GetPrefireCurrent();							//v1.1.T16 add
	short GetThreadWireMoveBH();						//v1.1.T15 add
	bool IsMultiPR();									//v1.1.T14 add
	bool IsGapLargeTime();								//v1.1.T13 add
	bool IsForceCaliReset();							//v1.1.T12 add
	bool IsDebugModeStartStop();						//v1.1.T10 add
	bool IsBhMoveWait();								//v1.1.T9 add
	bool IsDebugWaveformFile();							//v1.1.T8 add
	short GetUsgManualPower();							//v1.1.T7 add
	short GetEfoCurrentOffset();						//v1.1.T6 add
	short GetEfoCurrentFactor();						//v1.1.T6 add
	bool IsEfoErrIO();									//v1.1.T5 add
	short GetForceGainThres(short nBondPt);				//v1.1.T3 add
	short GetForceWndRange(short nBondPt);				//v1.1.T3 add
	short GetForceWndTime(short nBondPt);				//v1.1.T3 add
	short GetBondTransitTime(short nBondPt);			//v1.1.T3 add
	short GetBondFilterTime(short nBondPt);				//v1.1.T3 add
	short GetRlsTransitTime(short nBondPt);				//v1.1.T3 add
	short GetRlsFilterTime(short nBondPt);				//v1.1.T3 add
	short GetTwistHeight(char cLoopMode);				//v1.0.T231 add, v1.0.T234 edit
	short GetLoopShapeType();							//v1.0.T228 add
	short GetBumpBaseHt2();								//v1.0.T225 add
	bool IsControlGroupEnable();						//v1.0.T219 add
	short GetPosGainRH(short nLoopGroup);				//v1.0.T219 add
	short GetVelGainRH(short nLoopGroup);				//v1.0.T219 add
	short GetVelIntegratorRH(short nLoopGroup);			//v1.0.T219 add
	short GetAffRH(short nLoopGroup);					//v1.0.T219 add
	short GetPosGainRD(short nLoopGroup);				//v1.0.T219 add
	short GetVelGainRD(short nLoopGroup);				//v1.0.T219 add
	short GetVelIntegratorRD(short nLoopGroup);			//v1.0.T219 add
	short GetAffRD(short nLoopGroup);					//v1.0.T219 add
	bool IsPremoveGroupEnable();						//v1.0.T218 add
	short GetNeckDistPremove(short nLoopGroup);			//v1.0.T217 add
	short GetNeckHtPremove(short nLoopGroup);			//v1.0.T217 add
	bool IsBypassOnlElevNoMag();						//v1.0.T216 add
	short GetCtactSrchSampleTime();						//v1.0.T214 add
	short GetPreKinkDistPremove();						//v1.0.T211 add
	short GetPreKinkHtPremove();						//v1.0.T211 add
	short GetSafePosTol();								//v1.0.T210 add
	bool IsCheckAbnormalShutdown();						//v1.0.T208 add
	short GetEtelCmdTimeout();							//v1.0.T207 add
	short GetDebugEtel();								//v1.0.T206 add
	short GetEtelStartupDelay();						//v1.0.T206 add
	short GetNeckLAngle();								//v1.0.T205 add
	short GetPreKinkLAngle();							//v1.0.T204 add
	short GetKink2LAngle();								//v1.0.T204 add
	short GetKink3LAngle();								//v1.0.T204 add
	bool IsBypassMHS();									//v1.0.T203 add
	short GetKink3SpanLen();							//v1.0.T201 add
	short GetKink2SpanLen();							//v1.0.T200 add
	short GetMaxPatSize();								//v1.0.T198 add
	short GetFabType();									//v1.0.T197 add
	bool IsSharePR();									//v1.0.T196 add
	short GetFnkMaxPower();								//v1.0.T192 add
	short GetFnkMaxPowerTol();							//v1.0.T190 add
	short GetFnkScaleInputMin();						//v1.0.T190 add
	short GetFnkScaleInputMax();						//v1.0.T190 add
	short GetWaveformCaptureCount();					//v1.0.T188 add
	bool IsBypassServo();								//v1.0.T186 add
	short GetDryRunBondTime();							//v1.0.T184 add
	short GetFunctionKey(short nKey);					//v1.0.T181 add
	bool IsOperSetupShutDown();							//v1.0.T180 add
	bool IsOperSetupMinimizeWindow();					//v1.0.T180 add
	bool IsOperSetupMhsCali();							//v1.0.T180 add
	bool IsOperSetupServoCali();						//v1.0.T180 add
	bool IsOperSetupUsgCali();							//v1.0.T180 add
	bool IsOperSetupVisionCali();						//v1.0.T180 add
	bool IsOperRecipeBbosOffset();						//v1.0.T179 add
	bool IsOperRecipeBbosPara();						//v1.0.T179 add
	bool IsOperRecipeBsobOffset();						//v1.0.T179 add
	bool IsOperRecipeBsobPara();						//v1.0.T179 add
	bool IsOperRecipeMhsPara();							//v1.0.T179 add
	bool IsOperRecipeWireOffset();						//v1.0.T179 add
	bool IsOperRecipeLoopPara();						//v1.0.T179 add
	bool IsOperRecipeBondSetup();						//v1.0.T179 add
	short GetForceCaliSrchThres();						//v1.0.T178 add, v1.0.T187 rename
	short GetForceCaliSrchSpeed();						//v1.0.T177 add
	short GetForceSnrResetType();						//v1.0.T176 add
	bool IsForceSnrEnable();							//v1.0.T176 add
	bool IsDebugModeWaveform();							//v1.0.T175 add
	bool IsDebugModeWorkHolder();						//v1.0.T175 add
	bool IsDebugModeBondTime();							//v1.0.T174 add
	bool IsDebugModeVisionTime();						//v1.0.T174 add
	short GetCheckVacDelay();							//v1.0.T169 add
	bool IsRebondEnable();								//v1.0.T168 add
	short GetRebondLoopHt();							//v1.0.T168 add
	short GetRebondBaseHt();							//v1.0.T168 add
	short GetRebondSpanLen();							//v1.0.T168 add
	short GetRebondTime();								//v1.0.T168 add
	short GetRebondForce();								//v1.0.T168 add
	short GetRebondPower();								//v1.0.T168 add
	double GetWaveformScaleFPY();						//v1.0.T167 add
	int GetWaveformOffsetFPY();							//v1.0.T167 add
	double GetWaveformScaleRPY();						//v1.0.T167 add
	int GetWaveformOffsetRPY();							//v1.0.T167 add
	short GetWaveformTypeRPY();							//v1.0.T167 add
	short GetWaveformTypeFPY();							//v1.0.T167 add
	int GetWaveformOffsetWCL();							//v1.0.T164 add
	double GetWaveformScaleFPX();						//v1.0.T166 add
	int GetWaveformOffsetFPX();							//v1.0.T166 add
	double GetWaveformScaleRPX();						//v1.0.T166 add
	int GetWaveformOffsetRPX();							//v1.0.T166 add
	short GetWaveformTypeRPX();							//v1.0.T166 add
	short GetWaveformTypeFPX();							//v1.0.T166 add
	double GetWaveformScalePZ();						//v1.0.T159 add
	int GetWaveformOffsetPZ();							//v1.0.T159 add
	short GetLfClampPosTol();							//v1.0.T158 add
	bool IsClearTrackCheckClampOpen();					//v1.0.T157 add
	short GetFnkScaleInput();							//v1.0.T155 add
	void SetFnkScaleInput(int nInput);					//v1.0.T155 add
	short GetFnkConstMode();							//v1.0.T154 add
	short GetFnkComPort();								//v1.0.T153 add
	short GetUsgType();									//v1.0.T152 add
	bool IsVacAutoCutoff();								//v1.0.T150 add
	bool IsCheckVacuum();								//v1.0.T149 add
	bool IsTimeStampCycle();							//v1.0.T148 add
	bool IsTimeStampBond();								//v1.0.T148 add
	bool IsTimeStampPR();								//v1.0.T148 add
	bool IsTimeStampIndex();							//v1.0.T148 add
	bool IsOperRecipeManage();							//v1.0.T147 add
	bool IsOperRecipeTeach();							//v1.0.T147 add
	bool IsOperRecipeEdit();							//v1.0.T147 add
	bool IsOperRecipeBondPara();						//v1.0.T147 add
	short GetWireSpoolTrigDelay();						//v1.0.T146 add
	short GetSrchPrShowTimeOffset();					//v1.0.T145 add
	short GetWireSpoolType();							//v1.0.T144 add
	short GetWireSpoolDist();							//v1.0.T144 add
	short GetWireSpoolSpeed();							//v1.0.T144 add
	bool IsDebugCaliForce();							//v1.0.T143 add
	short GetForceCaliType();							//v1.0.T142 add, v1.0.T182 rename
	bool IsCheckInvertDie();							//v1.0.T141 add
	bool IsDryRunWcl();									//v1.0.T140 add
	short GetBondShift2(short nWireNum);				//v1.0.T139 add
	short GetTotalIndex();								//v1.0.T138 add
	long GetIndexPitch();								//v1.0.T138 add
	void SetIndexPitch(long lPitch);					//v1.0.T138 add
	long GetFirstIndexPos();							//v1.0.T138 add
	void SetFirstIndexPos(long lPos);					//v1.0.T138 add
	bool IsBypassPrLight();								//v1.0.T165 add
	bool IsBypassUSG();									//v1.0.T165 add
	bool IsBypassEFO();									//v1.0.T165 add
	short GetWclType();									//v1.0.T134 add
	short GetWclForce(short nProfile, short nStep);		//v1.0.T134 add
	short GetEfoTimeResol();							//v1.0.T133 add
	bool IsBypassAll();									//v1.0.T165 add
	bool IsBypassOffload();								//v1.0.T165 add
	bool IsBypassOnload();								//v1.0.T165 add
	bool IsBypassCamera();								//v1.0.T165 add
	bool IsDebugInfoShowTime();							//v1.0.T125 add
	short GetLogImageScore();							//v1.0.T124 add
	short GetLogImageAngle();							//v1.0.T124 add
	short GetHidePrResultTime();						//v1.0.T121 add
	short GetSrchPrRedScore();							//v1.0.T118 add
	short GetSrchPrGreenScore();						//v1.0.T118 add
	bool IsSrchPrRetry();								//v1.0.T117 add
	double GetSrchPrAngleMax();							//v1.0.T116 add
	short GetSrchPrAccuracy();							//v1.0.T115 add
	short GetSrchPrSpeed();								//v1.0.T115 add
	bool IsSrchPrCircularOverscan();					//v1.0.T115 add
	double GetSrchPrAngleAccuracy();					//v1.0.T115 add
	double GetSrchPrAngleTol();							//v1.0.T115 add
	short GetDebugInfoLen();							//v1.0.T113 add
	bool IsShowRVX();									//v1.0.T112 add
	bool IsShowFVX();									//v1.0.T112 add
	bool IsShowRPX();									//v1.0.T112 add
	bool IsShowFPX();									//v1.0.T112 add
	bool IsShowRVY();									//v1.0.T112 add
	bool IsShowFVY();									//v1.0.T112 add
	bool IsShowRPY();									//v1.0.T112 add
	bool IsShowFPY();									//v1.0.T112 add
	bool IsShowRVZ();									//v1.0.T112 add
	bool IsShowFVZ();									//v1.0.T112 add
	bool IsShowRPZ();									//v1.0.T112 add
	bool IsShowFPZ();									//v1.0.T112 add
	bool IsShowWCL();									//v1.0.T112 add
	bool IsShowMFZ();									//v1.0.T112 add
	short GetWireCycleTime();							//v1.0.T111 add
	short GetVerifyShowTrack();							//v1.0.T109 add
	short GetVerifyShowMcData();						//v1.0.T108 add
	short GetVerifyShowServoControl();					//v1.0.T107 add
	short GetVerifyShowServoSpeed();					//v1.0.T107 add
	short GetUphMode();									//v1.0.T105 add
	short GetBuf2Param(short nIdx);						//v1.0.T104 add
	short GetSrchSpeedStep();							//v1.0.T102 add
	short GetAcsXRMS(EN_MOTOR_AXIS nAxis);				//v1.0.T101 add
	bool IsRestoreMcdata();								//v1.0.T100 add
	bool IsMcdata();									//v1.0.T96 add
	short GetCtactStatSeq(short nBondPt);				//v1.0.T95 add
	bool IsCtactLowErr(short nBondPt);					//v1.0.T94 add
	bool IsCtactHighErr(short nBondPt);					//v1.0.T94 add
	short GetTailDelay();								//v1.0.T93 add
	short GetTableHomeSeq();							//v1.0.T92 add
	short GetVerifyShowPgm();							//v1.0.T91 add
	short GetVerifyShowRef();							//v1.0.T91 add
	short GetVerifyShowWire();							//v1.0.T91 add
	short GetVerifyShowBondPara();						//v1.0.T91 add
	short GetColorCursor();								//v1.0.T90 add
	short GetColorWireGroup(short nGroup);				//v1.0.T89 add
	short GetColorCurWire();							//v1.0.T89 add
	short GetEditBondPtMax();							//v1.0.T88 add
	bool IsBypassIndex();								//v1.0.T65 add
	short GetVisionLibrary();							//v1.0.T81 add
	short GetDeviceSeq();								//v1.0.T79 add
	short GetCtactTol(short nBondPt);					//v1.0.T77 add
	short GetCtactOffset(short nBondPt);				//v1.0.T77 add
	bool IsCtactGet(short nWireIdx, short nBondPt);		//v1.0.T75 add
	bool IsCtactAdd(short nWireIdx, short nBondPt);		//v1.0.T75 add
	short GetCtactSampleSize(short nBondPt);			//v1.0.T72 add
	short GetPrefireTime();								//v1.0.T71 add
	short GetSlowMoveVel();								//v1.0.T70 add
	short GetSlowMoveAcc();								//v1.0.T70 add
	short GetSlowMoveJerk();							//v1.0.T70 add
	bool IsEfoPrefire();								//v1.0.T69 add
	short GetAutoBondTimeout();							//v1.0.T67 add
	short GetEfoType();									//v1.0.T66 add
	bool IsRemoveDelaySrch(short nBondPt);				//v1.0.T78 add
	short GetDieAheadPR();								//v1.0.T64 add
	bool IsSoftSrchEnable(short nBondPt);				//v1.0.T63 add
	short GetSoftSrchPosErrSeries(short nBondPt);		//v1.0.T63 add
	short GetSoftSrchPosErr(short nBondPt);				//v1.0.T63 add
	short GetSoftSrchSpeed(short nBondPt);				//v1.0.T63 add
	bool IsRandomSeed();								//v1.0.T59 add
	short GetRandomSkip();								//v1.0.T58 add
	short GetCheckWireFeedEveryWireX();					//v1.0.T55 add
	short GetLightComPort();							//v1.0.T51 add
	short GetEfoComPort();								//v1.0.T51 add
	short GetUsgComPort();								//v1.0.T50 add
	void ToggleLanguage();								//v1.0.T49 add
	short GetHolderScrewType();							//v1.0.T48 add
	double GetWaveformScaleWCL();						//v1.0.T45 add
	short GetWaveformTypeWCL();							//v1.0.T44 add
	short GetWaveformTypeMFZ();							//v1.0.T43 add
	void SetLanguage(short nLanguage);					//v1.0.T38 add
	short GetLanguage();								//v1.0.T38 add
	int GetAlignSensorSlowSearchDist_x1um();			//v1.0.T37 add
	short GetRebootController();						//v1.0.T29 add
	short GetAcsBufferType();							//v1.0.T28 add
	bool IsCanErrResetStart();							//v1.0.T26 add
	short GetAcsTimeout();								//v1.0.T23 add
	short GetServoSampleRate();							//v1.0.T21 add
	short GetWaveformSampleRate();						//v1.0.T20 add
	short GetAcsSampleRate();							//v1.0.T19 add
	int GetAlignSensorSlowSearchSpeed();				//v1.0.T17 add
	bool IsPrIndexHalfClamp();							//v1.0.T15 add
	short GetCanCardType();								//v1.0.T14 add
	bool IsExceedTime1(short nWireNum, short nForce);	//v1.0.T13 add
	bool IsExceedTime2(short nWireNum, short nForce);	//v1.0.T13 add
	BYTE GetTime1LowLimit(short nWireNum);				//v1.0.T13 add
	BYTE GetTime1HighLimit(short nWireNum);				//v1.0.T13 add
	BYTE GetTime2LowLimit(short nWireNum);				//v1.0.T13 add
	BYTE GetTime2HighLimit(short nWireNum);				//v1.0.T13 add
	bool IsExceedForce1(short nWireNum, short nForce);	//v1.0.T12 add
	bool IsExceedForce2(short nWireNum, short nForce);	//v1.0.T12 add
	BYTE GetForce1LowLimit(short nWireNum);				//v1.0.T12 add
	BYTE GetForce1HighLimit(short nWireNum);			//v1.0.T12 add
	BYTE GetForce2LowLimit(short nWireNum);				//v1.0.T12 add
	BYTE GetForce2HighLimit(short nWireNum);			//v1.0.T12 add
	bool IsWcOpenSensorExist();							//v1.0.T11 add
	bool IsWcCloseSensorExist();						//v1.0.T11 add
	short GetCanErrRetry();								//v1.0.T10 add
	bool IsExceedPower2(short nWireNum, short nPower);	//v1.0.T9 add
	BYTE GetPower2LowLimit(short nWireNum);				//v1.0.T9 add
	BYTE GetPower2HighLimit(short nWireNum);			//v1.0.T9 add
	bool IsExceedPower1(short nWireNum, short nPower);	//v1.0.T8 add, v1.0.T9 edit
	BYTE GetPower1LowLimit(short nWireNum);				//v1.0.T7 add, v1.0.T9 edit
	BYTE GetPower1HighLimit(short nWireNum);			//v1.0.T7 add, v1.0.T9 edit
    void LoadConfig();				//v1.0.T39 edit
    void LoadAccessControl();		//v1.0.T147 add
    short GetMhsType();
	short GetCameraType();			//v1.0.T27 edit
	short GetOnloadType();
    short GetStepperType();
    short GetKeyPadType();
	short GetBondHeadType();
	short GetAlignSensorType();
	short GetMachineID();			//v1.0.T3 add
	void SetMachineID(short nID);	//v1.0.T3 add
private:	//function
	CString GetMotorString(EN_MOTOR_AXIS nAxis);	//v1.0.T101 add
	void SaveConfig();				//v1.0.T41 edit
private:	//variable
	short m_nSkipLastCol;						//v1.1.T24 add
	short m_nGapLargeTimeHighLimit;				//v1.1.T23 add
	bool m_bLoopTopPremove;						//v1.1.T22 add
	short m_nSwitchForceDelay;					//v1.1.T20 add
	short m_nSwitchForcePercent;				//v1.1.T18 add
	short m_nPrefireCurrent;					//v1.1.T16 add
	short m_nThreadWireMoveBH;					//v1.1.T15 add
	bool m_bMultiPR;							//v1.1.T14 add
	bool m_bGapLargeTime;						//v1.1.T13 add
	bool m_bForceCaliReset;						//v1.1.T12 add
	bool m_bDebugModeStartStop;					//v1.1.T10 add
	bool m_bBhMoveWait;							//v1.1.T9 add
	bool m_bDebugWaveformFile;					//v1.1.T8 add
	short m_nUsgManualPower;					//v1.1.T7 add
	short m_nEfoCurrentOffset;					//v1.1.T6 add
	short m_nEfoCurrentFactor;					//v1.1.T6 add
	bool m_bEfoErrIO;							//v1.1.T5 add
	short m_anForceGainThres[2];				//v1.1.T3 add - %
	short m_anForceWndRange[2];					//v1.1.T3 add - mN
	short m_anForceWndTime[2];					//v1.1.T3 add - ms
	short m_anBondTransitTime[2];				//v1.1.T3 add - ms
	short m_anBondFilterTime[2];				//v1.1.T3 add - ms
	short m_anRlsTransitTime[2];				//v1.1.T3 add - ms
	short m_anRlsFilterTime[2];					//v1.1.T3 add - ms
	short m_nTwistHeight[TOTAL_LOOP_MODE];		//v1.0.T231 add, v1.0.T234 edit
	short m_nLoopShapeType;						//v1.0.T228 add
	short m_nBumpBaseHt2;						//v1.0.T225 add
	bool m_bControlGroupEnable;					//v1.0.T219 add
	short m_nPosGainRH[TOTAL_LOOP_MODE];		//v1.0.T219 add
	short m_nVelGainRH[TOTAL_LOOP_MODE];		//v1.0.T219 add
	short m_nVelIntegratorRH[TOTAL_LOOP_MODE];	//v1.0.T219 add
	short m_nAffRH[TOTAL_LOOP_MODE];			//v1.0.T219 add
	short m_nPosGainRD[TOTAL_LOOP_MODE];		//v1.0.T219 add
	short m_nVelGainRD[TOTAL_LOOP_MODE];		//v1.0.T219 add
	short m_nVelIntegratorRD[TOTAL_LOOP_MODE];	//v1.0.T219 add
	short m_nAffRD[TOTAL_LOOP_MODE];			//v1.0.T219 add
	bool m_bPremoveGroupEnable;		//v1.0.T218 add
	short m_nNeckDistPremove[TOTAL_LOOP_MODE];	//v1.0.T217 add
	short m_nNeckHtPremove[TOTAL_LOOP_MODE];	//v1.0.T217 add
	bool m_bBypassOnlElevNoMag;		//v1.0.T216 add
	short m_nCtactSrchSampleTime;	//v1.0.T214 add
	short m_nPreKinkDistPremove;	//v1.0.T211 add
	short m_nPreKinkHtPremove;		//v1.0.T211 add
	short m_nSafePosTol;			//v1.0.T210 add
	bool m_bCheckAbnormalShutdown;	//v1.0.T208 add
	short m_nEtelCmdTimeout;		//v1.0.T207 add
	short m_nDebugEtel;				//v1.0.T206 add
	short m_nEtelStartupDelay;		//v1.0.T206 add
	short m_nNeckLAngle;			//v1.0.T205 add
	short m_nPreKinkLAngle;			//v1.0.T204 add
	short m_nKink2LAngle;			//v1.0.T204 add
	short m_nKink3LAngle;			//v1.0.T204 add
	bool m_bBypassMHS;				//v1.0.T203 add
	short m_nKink3SpanLen;			//v1.0.T201 add
	short m_nKink2SpanLen;			//v1.0.T200 add
	short m_nMaxPatSize;			//v1.0.T198 add
	short m_nFabType;				//v1.0.T197 add	//0:2xfab, 1:4xfab
	bool m_bSharePR;				//v1.0.T196 add
	short m_nFnkMaxPower;			//v1.0.T192 add
	short m_nFnkMaxPowerTol;		//v1.0.T190 add
	short m_nFnkScaleInputMin;		//v1.0.T190 add
	short m_nFnkScaleInputMax;		//v1.0.T190 add
	short m_nWaveformCaptureCount;	//v1.0.T188 add
	bool m_bBypassServo;			//v1.0.T186 add
	short m_nDryRunBondTime;		//v1.0.T184 add
	short m_anFunctionKey[3];		//v1.0.T181 add
	bool m_bOperSetupShutDown;		//v1.0.T180 add
	bool m_bOperSetupMinimizeWindow;	//v1.0.T180 add
	bool m_bOperSetupMhsCali;		//v1.0.T180 add
	bool m_bOperSetupServoCali;		//v1.0.T180 add
	bool m_bOperSetupUsgCali;		//v1.0.T180 add
	bool m_bOperSetupVisionCali;	//v1.0.T180 add
	bool m_bOperRecipeBbosOffset;	//v1.0.T179 add
	bool m_bOperRecipeBbosPara;		//v1.0.T179 add
	bool m_bOperRecipeBsobOffset;	//v1.0.T179 add
	bool m_bOperRecipeBsobPara;		//v1.0.T179 add
	bool m_bOperRecipeMhsPara;		//v1.0.T179 add
	bool m_bOperRecipeWireOffset;	//v1.0.T179 add
	bool m_bOperRecipeLoopPara;		//v1.0.T179 add
	bool m_bOperRecipeBondSetup;	//v1.0.T179 add
	short m_nForceCaliSrchThres;	//v1.0.T178 add, v1.0.T187 rename
	short m_nForceCaliSrchSpeed;	//v1.0.T177 add
	short m_nForceSnrResetType;		//v1.0.T176 add
	//0: ___|~~~|___|~~~|___	//start read before srch delay, reset read after bond
	//1: ___|~~~~~~~~~~~|___	//not reset read after bond 1
	//2: __|~~~~~~~~~~~~|___	//start read before jog srch 1
	bool m_bForceSnrEnable;			//v1.0.T176 add
	bool m_bDebugModeWaveform;		//v1.0.T175 add
	bool m_bDebugModeWorkHolder;	//v1.0.T175 add
	bool m_bDebugModeBondTime;		//v1.0.T174 add
	bool m_bDebugModeVisionTime;	//v1.0.T174 add
	short m_nCheckVacDelay;			//v1.0.T169 add
	bool m_bRebondEnable;			//v1.0.T168 add
	short m_nRebondLoopHt;			//v1.0.T168 add
	short m_nRebondBaseHt;			//v1.0.T168 add
	short m_nRebondSpanLen;			//v1.0.T168 add
	short m_nRebondTime;			//v1.0.T168 add
	short m_nRebondForce;			//v1.0.T168 add
	short m_nRebondPower;			//v1.0.T168 add
	double m_dWaveformScaleFPY;		//v1.0.T167 add	//0:default
	int m_nWaveformOffsetFPY;		//v1.0.T167 add
	double m_dWaveformScaleRPY;		//v1.0.T167 add	//0:default
	int m_nWaveformOffsetRPY;		//v1.0.T167 add
	short m_nWaveformTypeRPY;		//v1.0.T167 add
	short m_nWaveformTypeFPY;		//v1.0.T167 add
	int m_nWaveformOffsetWCL;		//v1.0.T164 add
	double m_dWaveformScaleFPX;		//v1.0.T166 add	//0:default
	int m_nWaveformOffsetFPX;		//v1.0.T166 add
	double m_dWaveformScaleRPX;		//v1.0.T166 add	//0:default
	int m_nWaveformOffsetRPX;		//v1.0.T166 add
	short m_nWaveformTypeRPX;		//v1.0.T166 add
	short m_nWaveformTypeFPX;		//v1.0.T166 add
	double m_dWaveformScalePZ;		//v1.0.T159 add	//0:default
	int m_nWaveformOffsetPZ;		//v1.0.T159 add
	short m_nLfClampPosTol;			//v1.0.T158 add
	bool m_bClearTrackCheckClampOpen;	//v1.0.T157 add
	short m_nFnkScaleInput;			//v1.0.T155 add
	short m_nFnkConstMode;			//v1.0.T154 add
	short m_nFnkComPort;			//v1.0.T153 add
	short m_nUsgType;				//v1.0.T152 add	//0:std, 1:fnk
	bool m_bVacAutoCutoff;			//v1.0.T150 add
	bool m_bCheckVacuum;			//v1.0.T149 add
	bool m_bTimeStampCycle;			//v1.0.T148 add
	bool m_bTimeStampBond;			//v1.0.T148 add
	bool m_bTimeStampPR;			//v1.0.T148 add
	bool m_bTimeStampIndex;			//v1.0.T148 add
	bool m_bOperRecipeManage;		//v1.0.T147 add
	bool m_bOperRecipeTeach;		//v1.0.T147 add
	bool m_bOperRecipeEdit;			//v1.0.T147 add
	bool m_bOperRecipeBondPara;		//v1.0.T147 add
	short m_nWireSpoolTrigDelay;	//v1.0.T146 add
	short m_nSrchPrShowTimeOffset;	//v1.0.T145 add
	short m_nWireSpoolType;			//v1.0.T144 add
	short m_nWireSpoolDist;			//v1.0.T144 add
	short m_nWireSpoolSpeed;		//v1.0.T144 add
	bool m_bDebugCaliForce;			//v1.0.T143 add
	short m_nForceCaliType;			//v1.0.T142 add, v1.0.T182 rename
	bool m_bCheckInvertDie;			//v1.0.T141 add
	bool m_bDryRunWcl;				//v1.0.T140 add
	short m_anBondShift2[WIRE_NUM+1];	//v1.0.T139 add
	short m_nTotalIndex;			//v1.0.T138 add
	long m_lIndexPitch;				//v1.0.T138 add
	long m_lFirstIndexPos;			//v1.0.T138 add
	bool m_bBypassPrLight;			//v1.0.T165 add
	bool m_bBypassUSG;				//v1.0.T165 add
	bool m_bBypassEFO;				//v1.0.T165 add
	short m_nWclType;				//v1.0.T134 add
	short m_anWclForce[7][3];		//v1.0.T134 add	//7 profiles, 3 steps
	short m_nEfoTimeResol;			//v1.0.T133 add
	bool m_bBypassAll;				//v1.0.T165 add
	bool m_bBypassOffload;			//v1.0.T165 add
	bool m_bBypassOnload;			//v1.0.T165 add
	bool m_bBypassCamera;			//v1.0.T165 add
	bool m_bDebugInfoShowTime;		//v1.0.T125 add
	short m_nLogImageScore;			//v1.0.T124 add
	short m_nLogImageAngle;			//v1.0.T124 add
	short m_nHidePrResultTime;		//v1.0.T121 add
	short m_nSrchPrRedScore;		//v1.0.T118 add
	short m_nSrchPrGreenScore;		//v1.0.T118 add
	bool m_bSrchPrRetry;			//v1.0.T117 add
	double m_dSrchPrAngleMax;		//v1.0.T116 add
	short m_nSrchPrAccuracy;		//v1.0.T115 add	//-1:software, 1:low, 2:medium, 3:high
	short m_nSrchPrSpeed;			//v1.0.T115 add	//-1:software, 0:very low, 1:low, 2:medium, 3:high, 4:very high
	bool m_bSrchPrCircularOverscan;	//v1.0.T115 add
	double m_dSrchPrAngleAccuracy;	//v1.0.T115 add
	double m_dSrchPrAngleTol;		//v1.0.T115 add
	short m_nDebugInfoLen;			//v1.0.T113 add
	bool m_bShowRVX;				//v1.0.T112 add
	bool m_bShowFVX;				//v1.0.T112 add
	bool m_bShowRPX;				//v1.0.T112 add
	bool m_bShowFPX;				//v1.0.T112 add
	bool m_bShowRVY;				//v1.0.T112 add
	bool m_bShowFVY;				//v1.0.T112 add
	bool m_bShowRPY;				//v1.0.T112 add
	bool m_bShowFPY;				//v1.0.T112 add
	bool m_bShowRVZ;				//v1.0.T112 add
	bool m_bShowFVZ;				//v1.0.T112 add
	bool m_bShowRPZ;				//v1.0.T112 add
	bool m_bShowFPZ;				//v1.0.T112 add
	bool m_bShowWCL;				//v1.0.T112 add
	bool m_bShowMFZ;				//v1.0.T112 add
	short m_nWireCycleTime;			//v1.0.T111 add	//0:show time & length, 1:show time, 2:show length
	short m_nVerifyShowTrack;		//v1.0.T109 add	//0:show none, 1:show all, 2:show diff
	short m_nVerifyShowMcData;		//v1.0.T108 add	//0:show none, 1:show all, 2:show diff
	short m_nVerifyShowServoControl;	//v1.0.T107 add	//0:show none, 1:show all, 2:show diff
	short m_nVerifyShowServoSpeed;		//v1.0.T107 add	//0:show none, 1:show all, 2:show diff
	short m_nUphMode;				//v1.0.T105 add
	short m_anBuf2Param[2];			//v1.0.T104 add	//0..255
	short m_nSrchSpeedStep;			//v1.0.T102 add
	short m_anAcsXRMS[4];			//v1.0.T101 add	//0..50
	bool m_bRestoreMcdata;			//v1.0.T100 add
	bool m_bMcdata;					//v1.0.T96 add
	short m_anCtactStatSeq[2];		//v1.0.T95 add
	bool m_abCtactLowErr[2];		//v1.0.T94 add
	bool m_abCtactHighErr[2];		//v1.0.T94 add
	short m_nTailDelay;				//v1.0.T93 add	//0..20 samples (x0.5ms)
	short m_nTableHomeSeq;			//v1.0.T92 add	//0:yx, 1:xy
	short m_nVerifyShowPgm;			//v1.0.T91 add	//0:show none, 1:show all, 2:show diff
	short m_nVerifyShowRef;			//v1.0.T91 add	//0:show none, 1:show all, 2:show diff
	short m_nVerifyShowWire;		//v1.0.T91 add	//0:show none, 1:show all, 2:show diff
	short m_nVerifyShowBondPara;	//v1.0.T91 add	//0:show none, 1:show all, 2:show diff
	short m_nColorCursor;			//v1.0.T90 add	//-1:default "not defined", 0..WIRE_GROUP_MAX-1
	short m_anColorWireGroup[WIRE_GROUP_MAX];	//v1.0.T89 add	//-1:default "not defined", 0..WIRE_GROUP_MAX-1
	short m_nColorCurWire;			//v1.0.T89 add	//-1:default "not defined", 0..WIRE_GROUP_MAX-1
	short m_nEditBondPtMax;			//v1.0.T88 add	//5..20(default)
	bool m_bBypassIndex;			//v1.0.T165 add
	short m_nVisionLibrary;			//v1.0.T81 add	//0:matrox, 1:halcon
	short m_nDeviceSeq;				//v1.0.T79 add	//0:default, 1:speedup
	short m_anCtactTol[2];			//v1.0.T77 add
	short m_anCtactOffset[2];		//v1.0.T77 add
	bool m_abCtactGetDefault[2];	//v1.0.T76 add
	bool m_abCtactAddDefault[2];	//v1.0.T76 add
	bool m_abCtactGetWire[CTACT_WIRE_BUFFER][2];	//v1.0.T75 add
	bool m_abCtactAddWire[CTACT_WIRE_BUFFER][2];	//v1.0.T75 add
	bool m_abCtactGet[2];			//v1.0.T74 add
	bool m_abCtactAdd[2];			//v1.0.T74 add
	short m_anCtactSampleSize[2];	//v1.0.T72 add
	short m_nPrefireTime;			//v1.0.T71 add
	short m_nSlowMoveVel;			//v1.0.T70 add
	short m_nSlowMoveAcc;			//v1.0.T70 add
	short m_nSlowMoveJerk;			//v1.0.T70 add
	bool m_bEfoPrefire;				//v1.0.T69 add
	short m_nAutoBondTimeout;		//v1.0.T67 add
	short m_nEfoType;				//v1.0.T66 add	//0:default, 1:3-profile, 2:3-prof high-current
	bool m_bRemoveDelaySrch[2];		//v1.0.T78 add
	short m_nDieAheadPR;			//v1.0.T64 add	//0:slow, 1:fast
	short m_anSoftSrchPosErrSeries[2];	//v1.0.T63 add
	short m_anSoftSrchPosErr[2];	//v1.0.T63 add
	short m_anSoftSrchSpeed[2];		//v1.0.T63 add	//unit: % of set search speed
	//array [2]: [0]=1st bond, [1]=2nd bond
	bool m_bRandomSeed;				//v1.0.T59 add
	short m_nRandomSkip;			//v1.0.T58 add
	short m_nCheckWireFeedEveryWireX;	//v1.0.T55 add
	short m_nLightComPort;			//v1.0.T51 add
	short m_nEfoComPort;			//v1.0.T51 add
	short m_nUsgComPort;			//v1.0.T50 add
	short m_nHolderScrewType;		//v1.0.T48 add	//0:lead screw 6mm, 1:ball screw 5mm, 2:4mm
	double m_dWaveformScaleWCL;		//v1.0.T45 add
	short m_nWaveformTypeWCL;		//v1.0.T44 add	//0:WCL, 1:Force, 2:BondProgress, 3:ForceSensorReset, 4:ForceSensorInput
	short m_nWaveformTypeMFZ;		//v1.0.T43 add
	short m_nLanguage;				//v1.0.T38 add	//0=english, 1=chinese
	int m_nAlignSensorSlowSearchDist_x1um;	//v1.0.T37 add	//0: not search, +/-ve: search right/left dist
	short m_nRebootController;		//v1.0.T28 add	//0=not reboot, 1=reboot, 2=reboot factory default
	//short m_nAcsBufferType;		//v1.0.T28 add, v1.0.T237 delete	//0=Tony, 1=Eric, 2=Eric2
	bool m_bCanErrResetStart;		//v1.0.T26 add
	short m_nAcsTimeout;			//v1.0.T23 add	//0=no timeout, otherwise timeout in msec
	short m_nServoSampleRate;		//v1.0.T21 add
	short m_nWaveformSampleRate;	//v1.0.T20 add	//0=2k, 1=4k, 2=5k
	short m_nAcsSampleRate;			//v1.0.T19 add	//0=2k, 1=4k, 2=5k
	bool m_bPrIndexHalfClamp;		//v1.0.T15 add
	short m_nCanCardType;			//v1.0.T14 add
	ST_LIMIT_RANGE m_astTimeRange[WIRE_NUM+1][2];		//v1.0.T83 edit
	ST_LIMIT_RANGE m_astForceRange[WIRE_NUM+1][2];		//v1.0.T83 edit
	bool m_bWcOpenSensor;			//v1.0.T11 add
	bool m_bWcCloseSensor;			//v1.0.T11 add
	short m_nCanErrRetry;			//v1.0.T10 add
	ST_LIMIT_RANGE m_astPowerRange[WIRE_NUM+1][2];		//v1.0.T83 edit
    short m_nMhsType;
	short m_nCameraType;			//v1.0.T27 edit
    short m_nStepperType;			//0: 1240, 1: 2812, 2: StepperIO card
    short m_nKeyPadType;			//0: not used, 1: before P1, 2: P1, 3: small pad
	short m_nBondHeadType;
	short m_nAlignSensorType;
	short m_nOnloadType;
    short m_nMachineID;
	int m_nAlignSensorSlowSearchSpeed;	//0: not search, other: search speed
};

extern CMachineConfig theMcConfig;
