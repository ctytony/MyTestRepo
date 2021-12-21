//VEClamp.h v1.0.T2

#pragma once

// CVEliteLfClamp command target

#include "DataStruct.h"

typedef enum enVEliteLfClampState
{
    VE_STATE_CLAMP_HOME,
    VE_STATE_CLAMP_OPEN, 
    VE_STATE_CLAMP_CLOSE, 
    VE_STATE_CLAMP_OPENING, 
    VE_STATE_CLAMP_CLOSING,
};

class CVEliteLfClamp : public CObject
{
public:
	CVEliteLfClamp();
	virtual ~CVEliteLfClamp();

public:	//functions
	void Init();
	int OpenLfClamp(void);
	int CloseLfClamp(void);
	int CheckLfClampStatus(BOOL bPretrigEnable);
	void GetRcd(SINGLE_CAP_CLAMP_RCD* pstLfClampRcd);
	void SetRcd(SINGLE_CAP_CLAMP_RCD* pstLfClampRcd);
    void SaveRcd(void);
	void LoadRcd(void);
	short GetClampStatus(void);
	int SearchClampHome();
	int WaitStepHomeFinish();
    // Get clamp stepper speed profile
    int GetStepperSpeedProfile(STEP_SPEED_PROFILE* pstSpeedProfile);
    // Store speed profile of clamp stepper
    int StoreStepperSpeedProfile(STEP_SPEED_PROFILE* pstSpeedProfile);
	int StopMotion(void);
private:	//variables
    // Lead frame clamp status, open or clamp
    char m_cClampState;
    // Step motor id of current lead frame
    byte m_nLfClampStepId;
    // Lead frame clamp step speed profile
    STEP_SPEED_PROFILE m_stStepSpeedProfile;
    // Lead frame clamp record
    SINGLE_CAP_CLAMP_RCD m_stLeadFrameClampRcd;
};

extern CVEliteLfClamp theVEliteLfClamp;		//v3.3.T54 add
