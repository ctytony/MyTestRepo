//Debug.h v1.0.T19

#pragma once

enum EN_DEBUG_MODE	//v1.0.T7 edit, v1.0.T8 edit
{
    DEBUG_NONE,
    DEBUG_BOND_TIMING,
    DEBUG_VISION_TIMING,
    DEBUG_SERVO_TIMING,
    DEBUG_WAVEFORM,
    DEBUG_STEP_MOTION,
    DEBUG_TRANSDUCER,		//v1.0.T2 edit
    DEBUG_WORK_HOLDER,
    DEBUG_WIRE_CLAMP,		//v1.0.T6 edit
	DEBUG_SP_WAVEFORM,
    DEBUG_CTACT_LVL,
	DEBUG_LOG_TRACE,
	DEBUG_IO,
	DEBUG_INDEX_ACCURACY,
	DEBUG_LOG_IMAGE,		//v1.0.T17 add
	DEBUG_LEAD_LOCATE,		//v1.0.T18 add
	DEBUG_SRCH_PATTERN,		//v1.0.T16 add
	DEBUG_WIRE_FEED,		//v1.0.T9 add
	DEBUG_USG_PROFILE,		//v1.0.T9 add
	DEBUG_WIRE_CYCLE_TIME,	//v1.0.T10 add
	DEBUG_LF_CYCLE_TIME,	//v1.0.T11 add
	TIME_STAMP_CYCLE,		//v1.0.T19 add
	TIME_STAMP_PR,			//v1.0.T19 add
	TIME_STAMP_BOND,		//v1.0.T19 add
	TIME_STAMP_INDEX,		//v1.0.T19 add
};

class CDebug
{
public:
	CDebug(void);
	~CDebug(void);
public:	//function
	CString GetDebugInfo();				//v1.0.T15 add
	void AddInfo(CString sInfo, long lValue);	//v1.0.T13 add
	void ClearInfo();					//v1.0.T12 add
	void AddInfo(CString sInfo);		//v1.0.T12 add
	bool IsDebugLfCycleTime(void);		//v1.0.T11 add
	bool IsDebugWireCycleTime(void);	//v1.0.T10 add
	bool IsDebugUsgProfile(void);		//v1.0.T9 add
	bool IsDebugWireFeed(void);			//v1.0.T9 add
	bool IsDebugSrchPattern(void);		//v1.0.T16 add
	bool IsDebugLogImage(void);			//v1.0.T17 add
	bool IsDebugLeadLocate(void);		//v1.0.T18 add
	bool IsDebugBondTiming(void);		//v1.0.T2 add
	bool IsDebugVisionTiming(void);		//v1.0.T2 add
	bool IsDebugServoTiming(void);		//v1.0.T2 add
	bool IsDebugWaveform(void);			//v1.0.T2 add
	bool IsDebugStepMotion(void);		//v1.0.T2 add
	bool IsDebugWorkHolder(void);		//v1.0.T2 add
	bool IsDebugWireClamp(void);		//v1.0.T2 add
	bool IsDebugSpWaveform(void);		//v1.0.T2 add
	bool IsDebugContactLevel(void);		//v1.0.T2 add
	bool IsDebugLogTrace(void);			//v1.0.T2 add
	bool IsDebugIO(void);				//v1.0.T2 add
	bool IsDebugIndexAccuracy(void);	//v1.0.T2 add
	bool IsDebugTransducer(void);		//v1.0.T2 add
	short GetDebugMode(void);
	void SetDebugMode(short nDebugMode);
private:	//variable
	CString m_sDebugInfo;	//v1.0.T15 add
	CString m_sTemp;		//v1.0.T13 add
	short m_nDebugMode;
private:	//function
	void Remove1stLine();	//v1.0.T12 add
};

extern CDebug theDebug;
