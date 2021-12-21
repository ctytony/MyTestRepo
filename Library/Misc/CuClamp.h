//CuClamp.h v1.0.T8

#pragma once

#include "DataStruct.h"
#include "StepperControl.h"
#include "Sensor.h"
#include "Relay.h"

#define  CUELITE_CLAMPER_FULL_CLOSE_PULSE     800

enum enCuEliteLfClampStatus 
{
    CUELITE_WND_CLAMP_OPEN, 
    CUELITE_WND_CLAMP_CLOSE, 
	CUELITE_WND_CLAMP_SAFE_CLAMP,
	CUELITE_WND_CLAMP_PRE_CLOSE,
	CUELITE_WND_CLAMP_OPENING, 
    CUELITE_WND_CLAMP_CLOSING,
	CUELITE_WND_CLAMP_MOVING,
};

class CCuEliteLfClamp : public CObject
{
public:
	CCuEliteLfClamp();
	virtual ~CCuEliteLfClamp();
public:	//variables
    CRelay m_VacuumValve;
public:	//functions
	void Init(void);
	void CloseClamp(void);
	void CheckClampStatus(void);
	short GetClampStatus(void);
	void GetClampRcd(CUELITE_STEPPER_CLAMP_PARA *pstClampParam);
	void SetClampRcd(CUELITE_STEPPER_CLAMP_PARA *pstClampParam);
	void LoadClampRcd(void);
	void SaveClampRcd(void);
	void SearchHome(void);
	void WaitHomeFinish();
	void OpenClamp(void);
	void PreCloseClamp(void);
	void MoveClampSafePos(void);
    void GetSpeedProfile(STEP_SPEED_PROFILE *pstSpeedProfile);
    void SetSpeedProfile(STEP_SPEED_PROFILE *pstSpeedProfile);
	void GetSpeedParam(ST_AXIS_PARA *pstSpeedParam);
	void SetSpeedParam(ST_AXIS_PARA *pstSpeedParam);
    int CheckVacuumStatus(void);
	byte GetStepperId(void);
private:	//variables
	CUELITE_STEPPER_CLAMP_PARA m_stClampParam;
    STEP_SPEED_PROFILE m_stSpeedProfile;
	ST_AXIS_PARA m_stSpeedParam;
	short m_nClampStatus;
    CSensor m_VacuumSensor;
	byte m_nStepperId;
private:	//functions
	void LoadSpeedProfile();
	void SaveSpeedProfile();
};

extern CCuEliteLfClamp theCuEliteLFClamp;
