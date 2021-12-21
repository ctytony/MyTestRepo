//Sensor.h v1.0.T5

#pragma once

#include "StepperIO.h"
#include "MachineConfig.h"

class CSensor : public CObject
{
public:
	CSensor();
	virtual ~CSensor();
private:	//variables
    short m_nPortAddr;
	short m_nPortSignal;
    short m_nState;
public:		//functions
	short GetCurrentState(BOOL bDryRun = FALSE, BOOL bDryRunValue = FALSE)
	{
		UpdateSensorState(bDryRun,bDryRunValue);
		return GetSensorState();
	}
    short GetSensorState(void);
    int Initialize(short nSensorPortAddressId, short nSensorPortSignal = NOPEN);
    int ResetSensorState(short nState);
	int SetDebugSensorState(short nSensorState);
    int UpdateSensorState(BOOL bDryRun = FALSE, BOOL bDryRunValue = FALSE);
	int UpdateSensorStateEx(BOOL bDryRun = FALSE, BOOL bDryRunValue = FALSE);
};


