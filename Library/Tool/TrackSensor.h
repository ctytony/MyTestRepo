#pragma once

#include "Sensor.h"

#define TRACK_SENSOR_HOLDING_TIME  200

// CTrackSensor command target

class CTrackSensor : public CObject
{
public:
	CTrackSensor();
	virtual ~CTrackSensor();
private:
    CSensor m_Sensor;
    short m_nState;
    short m_nSwitchState;
    long  m_lSwitchStartTime;
public:
    int Initialize(short nSensorPortAddressId);
    void  UpdateSensorState(BOOL bDryRun = FALSE, BOOL bDryRunValue = FALSE);
    short GetSensorState(void);
	short GetCurrentState(BOOL bDryRun = FALSE, BOOL bDryRunValue = FALSE)
	{
		UpdateSensorState(bDryRun,bDryRunValue);
		return GetSensorState();
	}
	int SetDebugSensorState(short nState);
};
