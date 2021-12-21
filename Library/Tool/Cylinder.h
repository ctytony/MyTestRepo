#pragma once

#include "Sensor.h"

// CCylinder command target

typedef enum enCylinderType 
{
    CYLINDER_ONE_STATE_VALVE,
    CYLINDER_TWO_STATE_VALVE,
};

typedef enum enCylinderState 
{
    CYLINDER_IDLE, 
    CYLINDER_FORWARD_MOTION, 
    CYLINDER_DELAY, 
    CYLINDER_END_STOP, 
    CYLINDER_BACKWARD_MOTION
};

#define CYLINDER_MOTION_TIMEOUT     2000    //2s

class CCylinder : public CObject
{
public:
	CCylinder();
	virtual ~CCylinder();
public:	//variables
    // Front sensor
    CSensor theFrontSensor;
    // Rear sensor
    CSensor theRearSensor;
public:	//functions
    // Set cylinder state
    int SetCylinderState(short nState);
    // Trigger cylinder forward motion
    int TriggerForwardMotion(void);
    // Trigger cylinder backward motin
    int TriggerBackwardMotion(void);
    // Monitor cylinder state
    int MonitorCylinderState(void);
    // Initialize cylinder
    int Initialize(short nForwardValveId, short nBackwardValveId, short nCylinderType, short nFrontSensorId, short nRearSensorId);
    // Get cylinder state
    short GetCylinderState(void);
    // Configure cylinder parameters
    int ConfigCylinderParameters(short nMotionTime, short nDelayTime = 0);
    // Get cylinder parameters
    int GetCylinderParameters(short* pnMotionTime, short* pnDelayTime);
    // Cylinder move forward
    int MoveForward(void);
    // Cylinder move backward
    int MoveBackward(void);
private:	//variables
    // Cylinder type
    short m_nCylinderType;
    // Cylinder state
    short m_nCylinderState;
    // Forward valve id
    short m_nForwardValveId;
    // Backward valve id
    short m_nBackwardValveId;
    // Start time for counting elapse time
    long m_lStageStartTime;
    // Cylinder forward and backward motion time
    short m_nMotionTime;
    // Cylinder delay time between forward and backward motion
    short m_nDelayTime;
};
