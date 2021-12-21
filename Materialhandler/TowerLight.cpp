/***************************************************************
TowerLight.cpp : implementation file
History:
26 Aug, 2010	--- Zhang Dong @1
- Improve tower light control.
****************************************************************/

#include "stdafx.h"
#include "MacDef.h"
#include "DigitalIO.h"
#include "TowerLight.h"
#include "StepCardControl.h"//v2.20.Q1 add
#include "MachineConfig.h"  //v2.20.Q1 add

CTowerLight theTowerLight;

// CTowerLight

CTowerLight::CTowerLight()
: m_bYellowBlinkEnable(false)
, m_lYellowBlinkStartTime(0)
, m_lRedBlinkStartTime(0)
, m_bRedBlinkingState(0)
{
}

CTowerLight::~CTowerLight()
{
}

// Initialization

int CTowerLight::Initialize(void)
{
	if (theMachineConfig.GetStepCtrlType() == STEP_TYPE_KANDE)
	{
		theGreenLight.Initialize(m_StepCardControl.m_OutputList.m_GreenLight.m_port);
		theYellowLight.Initialize(m_StepCardControl.m_OutputList.m_YellowLight.m_port);
		theRedLight.Initialize(m_StepCardControl.m_OutputList.m_RedLight.m_port);
	}
	else
	{
		theGreenLight.Initialize(TOWER_LIGHT_GREEN);
		theRedLight.Initialize(TOWER_LIGHT_RED);
		theYellowLight.Initialize(TOWER_LIGHT_YELLOW);
	}

    return 0;
}

// Enable error light
int CTowerLight::EnableErrorLight(void)
{
    //if (theRedLight.GetRelayState() == RELAY_OFF) //@1
    {
        theRedLight.SetRelayState(RELAY_ON);
    }

    //if ((m_bYellowBlinkEnable == false) && (theYellowLight.GetRelayState() == RELAY_ON))
    if (m_bYellowBlinkEnable == false) //@1
    {
        theYellowLight.SetRelayState(RELAY_OFF);
    }

    //if (theGreenLight.GetRelayState() == RELAY_ON) //@1
    {
        theGreenLight.SetRelayState(RELAY_OFF);
    }

    return 0;
}

// Enable idle light
int CTowerLight::EnableIdleLight(void)
{
    //if (theRedLight.GetRelayState() == RELAY_ON) //@1
    {
        theRedLight.SetRelayState(RELAY_OFF);
    }

    //if ((m_bYellowBlinkEnable == false) && (theYellowLight.GetRelayState() == RELAY_OFF))
    if (m_bYellowBlinkEnable == false) //@1
    {
        theYellowLight.SetRelayState(RELAY_ON);
    }

    //if (theGreenLight.GetRelayState() == RELAY_ON) //@1
    {
        theGreenLight.SetRelayState(RELAY_OFF);
    }

    return 0;
}

// Enable bonding light
int CTowerLight::EnableBondingLight(void)
{
    //if (theRedLight.GetRelayState() == RELAY_ON) //@1
    {
        theRedLight.SetRelayState(RELAY_OFF);
    }

    //if ((m_bYellowBlinkEnable == false) && (theYellowLight.GetRelayState() == RELAY_ON))
    if (m_bYellowBlinkEnable == false) //@1
    {
        theYellowLight.SetRelayState(RELAY_OFF);
    }

    //if (theGreenLight.GetRelayState() == RELAY_OFF) //@1
    {
        theGreenLight.SetRelayState(RELAY_ON);
    }

    return 0;
}

// Enable warning light
int CTowerLight::EnableWarningLight(bool bState)
{
    if (bState == true)
    {
        if (m_bYellowBlinkEnable == false)
        {
            m_bYellowBlinkEnable = true;
            m_lYellowBlinkStartTime = GetCurrentTime();
        }
    }
    else
    {
        if (m_bYellowBlinkEnable == true)
        {
            m_bYellowBlinkEnable = false;
        }

        if ((theRedLight.GetRelayState() == RELAY_ON) ||
            (theGreenLight.GetRelayState() == RELAY_ON))
        {
            theYellowLight.SetRelayState(RELAY_OFF);
        }
        else
        {
            theYellowLight.SetRelayState(RELAY_ON);
        }
    }
    
    return 0;
}

// Blink yellow light
int CTowerLight::BlinkYellowLight(void)
{
    long lElapseTime;

    if (m_bYellowBlinkEnable == true)
    {
        lElapseTime = GetCurrentTime() - m_lYellowBlinkStartTime;

		if (lElapseTime > TOWER_LIGHT_BLINK_INTERVAL || 
			lElapseTime < -TOWER_LIGHT_BLINK_INTERVAL)
        {
            if (theYellowLight.GetRelayState() == RELAY_OFF)
                theYellowLight.SetRelayState(RELAY_ON);
            else
                theYellowLight.SetRelayState(RELAY_OFF);

            m_lYellowBlinkStartTime = GetCurrentTime();
        }
    }

    return 0;
}

// Enable warning light
int CTowerLight::EnableBlinkRedLight(bool bState)
{
	if (bState == true)
	{
		if (m_bRedBlinkingState == FALSE)
		{
			m_bRedBlinkingState = TRUE;
			m_lRedBlinkStartTime = GetCurrentTime();
		}
	}
	else
	{
		
		m_bRedBlinkingState = FALSE;
		theRedLight.SetRelayState(RELAY_OFF);
		
		if ((theRedLight.GetRelayState() == RELAY_ON) ||
			(theGreenLight.GetRelayState() == RELAY_ON))
		{
			theYellowLight.SetRelayState(RELAY_OFF);
		}
		else
		{
			theYellowLight.SetRelayState(RELAY_ON);
		}
	}

	return 0;
}

// Blink yellow light
int CTowerLight::BlinkRedLight(void)
{
	long lElapseTime;

	if (m_bRedBlinkingState == TRUE)
	{
		lElapseTime = GetCurrentTime() - m_lRedBlinkStartTime;

		if (lElapseTime > TOWER_LIGHT_RED_BLINK_INTERVAL || 
			lElapseTime < -TOWER_LIGHT_RED_BLINK_INTERVAL)
		{
			if (theRedLight.GetRelayState() == RELAY_OFF)
				theRedLight.SetRelayState(RELAY_ON);
			else
				theRedLight.SetRelayState(RELAY_OFF);

			m_lRedBlinkStartTime = GetCurrentTime();
		}
	}

	return 0;
}

//v2.18.4.T6 add
void CTowerLight::RedLight(bool bOn, short nDebugMode)
{
	if (nDebugMode == DEBUG_IO)
	{
		if (bOn)
			theRedLight.SetRelayState(RELAY_ON);
		else
			theRedLight.SetRelayState(RELAY_OFF);
	}
}
