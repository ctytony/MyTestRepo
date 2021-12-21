//TowerLight v1.0.T6

#pragma once

#include "Relay.h"

#define TOWER_LIGHT_BLINK_INTERVAL      1000    //1s
#define TOWER_LIGHT_RED_BLINK_INTERVAL  1000    //1s

// CTowerLight command target

class CTowerLight : public CObject
{
public:
	CTowerLight();
	virtual ~CTowerLight();
public:	//functions
    void Init(void);	//v1.0.T6 edit
    int EnableErrorLight(void);
    int EnableIdleLight(void);
    int EnableBondingLight(void);
    int EnableWarningLight(bool bState);
    int BlinkYellowLight(void);
	int BlinkRedLight(void);
	int EnableBlinkRedLight(bool bState);
	void RedLight(bool bOn, short nDebugMode);
private:	//variables
    CRelay m_GreenLight;	//v1.0.T5 add
    CRelay m_RedLight;		//v1.0.T5 add
    CRelay m_YellowLight;	//v1.0.T5 add
    bool m_bYellowBlinkEnable;
    long m_lYellowBlinkStartTime;
	BOOL m_bRedBlinkingState;
	long  m_lRedBlinkStartTime;
};

extern CTowerLight theTowerLight;