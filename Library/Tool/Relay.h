//Relay.h v1.0.T5

#pragma once

#define SMALL_RELAY_DELAY           100		//ms
#define RELAY_ACTION_DELAY          125		//ms
#define RELAY_DELAY                 120		//ms
#define RELAY_SAFTY_DURATION        30000	//ms

class CRelay : public CObject
{
public:
	CRelay();
	virtual ~CRelay();
public:	//functions
	long GetDuration(void);
    int SetRelayState(short nState, bool bStepIO = false);
    int Initialize(short nRelayPortAddressId);
    short GetRelayState(void);
private:	//variables
    short m_nState;
    short m_nPortAddr;
	long m_lRegBeginTime;
	bool m_b1stEnter;
};
