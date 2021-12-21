//VledClamp.h v1.0.T3

#pragma once

// CSingleLfClamp command target

#include "DataStruct.h"

typedef enum enSingleLfClampStatus 
{
    SINGLE_CLAMP_HOME,
    SINGLE_CLAMP_OPEN, 
    SINGLE_CLAMP_CLOSE, 
    SINGLE_CLAMP_OPENING, 
    SINGLE_CLAMP_CLOSING,
};

class CSingleLfClamp : public CObject
{
public:
	CSingleLfClamp();
	virtual ~CSingleLfClamp();
private:	//variables
    // Lead frame clamp status, open or clamp
    char m_bClampStatus;
    // Step motor id of current lead frame
    byte m_bLfClampStepId;
    // Lead frame clamp step speed profile
    STEP_SPEED_PROFILE m_stStepSpeedProfile;
    // Lead frame clamp record
    SINGLE_CAP_CLAMP_RCD m_stLeadFrameClampRcd;
public:	//functions
	int Initialize();
	int OpenLfClamp(void);
	int CloseLfClamp(void);
	int CheckLfClampStatus(BOOL bPretrigEnable);
	int GetLfClampRcd(SINGLE_CAP_CLAMP_RCD* pstLfClampRcd);
	int UpdateLfClampRcd(SINGLE_CAP_CLAMP_RCD* pstLfClampRcd);
    int SaveLfClampRcd(void);
	int LoadLfClampRcd(void);
	short GetClampStatus(void);
	int SearchClampHome();
	int WaitStepHomeFinish();
    // Get clamp stepper speed profile
    int GetStepperSpeedProfile(STEP_SPEED_PROFILE* pstSpeedProfile);
    // Store speed profile of clamp stepper
    int StoreStepperSpeedProfile(STEP_SPEED_PROFILE* pstSpeedProfile);
	int StopMotion(void);
};

extern CSingleLfClamp theVledLfClamp;
