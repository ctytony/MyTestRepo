// LeadFramePicker.cpp : implementation file
//

#include "stdafx.h"
#include "MacDef.h"
#include "LeadFramePicker.h"
#include "DigitalIO.h"
#include "MaterialHandlerThread.h"

// CLeadFramePicker

CLeadFramePicker::CLeadFramePicker()
: m_nForwardValveOn(0)
, m_nBackwardValveOn(0)
, m_nForwardCylinderSensorOn(0)
, m_nBackwardCylinderSensorOn(0)
, m_nLeadFrameBufferSensorOn(0)
, m_nPickerStatus(0)
, m_nPickDelay(100)
, m_lPickerDelayStartTime(0)
, m_lForwardValveStartTime(0)
, m_lBackwardValveStartTime(0)
{
}

CLeadFramePicker::~CLeadFramePicker()
{
}


// Set state of forward cylinder valve

int CLeadFramePicker::SetForwardValveState(short nState)
{
    int lStatus = theDigitalIO.SetOutputBitState(LF_PICKER_FORWARD_VALVE, nState);
    
    m_lForwardValveStartTime = GetCurrentTime();

    return lStatus;
}

// Set state of backward cylinder valve
int CLeadFramePicker::SetBackwardValveState(short nState)
{
    int lStatus = theDigitalIO.SetOutputBitState(LF_PICKER_BACKWARD_VALVE, nState);

    m_lBackwardValveStartTime = GetCurrentTime();

    return lStatus;
}

// Get sensor status of forward cylinder
short CLeadFramePicker::GetForwardCylinderSensorStatus(void)
{
    short nState = 0;
    long  lElapseTime;
    
    if (1)
    {
        lElapseTime = GetCurrentTime() - m_lForwardValveStartTime;
        if (lElapseTime > VALVE_MOTION_TIME)
            nState = SENSOR_ON;
    }
    else
    {
        theDigitalIO.GetInputBitState(LF_PICKER_FORWARD_SENSOR, &nState);
    }
    
    m_nForwardCylinderSensorOn = nState;

    return nState;
}

// Get sensor status of backward cylinder
short CLeadFramePicker::GetBackwardCylinderSensorStatus(void)
{
    short nState = 0;
    long  lElapseTime;
    
    if (1)
    {
        lElapseTime = GetCurrentTime() - m_lBackwardValveStartTime;
        if (lElapseTime > VALVE_MOTION_TIME)
            nState = SENSOR_ON;
    }
    else
    {
        theDigitalIO.GetInputBitState(LF_PICKER_BACKWARD_SENSOR, &nState);
    }
	
    m_nBackwardCylinderSensorOn = nState;

    return nState;
}

// Get sensor status of lead frame buffer
short CLeadFramePicker::GetLeadFrameBufferSensorStatus(void)
{
    short nState = 0;

    theDigitalIO.GetInputBitState(ONLOAD_LF_EXIST_SENSOR, &nState);

    return nState;
}
