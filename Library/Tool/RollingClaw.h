//RollingClaw.h v1.0.T6

#pragma once

#include "StepperControl.h"

// CRollingClaw command target

#define ONE_ROUND_PULSE		1600

#define CLAW_BUSY			1
#define CLAW_FREE			0

#define INDEX_ONE_STEP		0
#define CATCH_UP_ROLLING	1

#define RIGHT_CLAW			1
#define LEFT_CLAW			0

#define MODE_NORMAL			0
#define MODE_AVOID_CRASH	1

enum enSingleClawStatus 
{
    SINGLE_CLAW_HOME,
    SINGLE_CLAW_CIRCLING, 
    SINGLE_CLAW_CIRCLE_DONE, 
};

#define CLAW_LOW_SPEED   10000
#define CLAW_MID_SPEED   15000
#define CLAW_HIGH_SPEED  20000

class CRollingClaw : public CObject
{
public:
	CRollingClaw();
	virtual ~CRollingClaw();
public:	//variables
	short m_nAvoidCrashStep;
public:	//functions
	void Init(BYTE nStepperId, short nClawSide = LEFT_CLAW);
	//move claw by round
	void IndexSteps(short nSteps, BOOL bMode = MODE_NORMAL);
	void SearchHome(void);
	void CheckHomeState(void);
	void SetSpeedType(short nSpeedType);
	bool IsBusy(void);
	void StopMove(void);
    // Update the Claw Ticker and related flags state when motion finish
	int UpdateClawState();
    // Update the Claw position and monitor its state, using in the pretrigger   
	int CheckClawStatus(void);
    // Reset all the ticker, position register and flags  
	void ResetAll(void);
	void ResetClawTicker(void);
	short GetSpeedState(void);
	short GetClawTicker(void);
	bool IsIndexEmpty(void);
	void SetIndexEmpty(bool bEmpty);
	void UpdateRound(void);
    void AvoidCrashRollBack(void);
	void SetClawTicker(short nTicker);
	void GetSpeedProfile(STEP_SPEED_PROFILE *pstSpeedProfile);
	void SetSpeedProfile(STEP_SPEED_PROFILE *pstSpeedProfile);
private:	//variales
	BYTE m_nStepperId;
	STEP_SPEED_PROFILE m_stSpeedProfile;
	short m_nStepTicker;
	bool m_bIndexEmpty;
	short m_nClawStatus;
	long m_lLastPos;
	long m_lNowPos;
	short m_nClawSide;
	short m_nSpeedState;
};