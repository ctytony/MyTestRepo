//DcMotor.h v1.0.T5

#pragma once

#include "Sensor.h"

// CCylinder command target

enum enDcMotorType 
{
    DC_MOTOR_TWO_DIRECTION,
	DC_MOROR_ONE_DIRECTION
};

enum enDcMotorState 
{
    DC_MOTOR_STATE_IDLE, 
    DC_MOTOR_FORWARD_MOTION, 
	DC_MOTOR_FORWARD_POWER_ON,
	DC_MOTOR_WAIT_SENSOR_LIMIT,
    DC_MOTOR_BACKWARD_DIR_SET,
	DC_MOTOR_BACKWARD_MOTION,
	DC_MOTOR_WAIT_SENSOR_HOME,
    DC_MOTOR_END_STOP,
};

enum enDcMotorEvent 
{
    DC_MOTOR_EVENT_IDLE, 
    DC_MOTOR_EVENT_START,
	DC_MOTOR_EVENT_TO_LIMIT,
	DC_MOTOR_EVENT_TO_HOME,
};

// CDcMotor command target

#define DC_MOTOR_TIMEOUT     4000    //2s

class CDcMotor : public CObject
{
public:
	CDcMotor();
	virtual ~CDcMotor();
public:	//variables
    CSensor theHomeSensor;
    CSensor theLimitSensor;
public:	//functions
	int Initialize(short nPowerId, short nDirId, short nHomeSensorId, short nLimitSensorId, short nHomeSensorSignal = NOPEN, short nLimitSensorSignal = NOPEN);//v2.20.Q1 edit
	int TriggerForwardMotion(void);
	int TriggerBacktoHome(void);
	short GetDcMotorState(void);
	int SubmitPusherMotion(short nEvent);
	int ProcessPusherMotion(void);
	short GetDcMotorEvent(void);
	short GetDcMotorError(void);
	int ResetDcMotorError(void);
	void StopMotion();
private:	//variables
	short m_nMotorType;
	short m_nPowerId;
	short m_nDirId;
	short m_nState;				//v1.0.T4 edit
	short m_nEvent;				//v1.0.T4 edit
	long m_lDcStartTime;
	short m_nError;				//v1.0.T4 edit
};
