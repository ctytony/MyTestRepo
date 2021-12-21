#include "StdAfx.h"
#include "Stepper.h"
#include "StepController.h"
#include "macdef.h"

CStepper::CStepper(void)
: m_nStepperId(0)
{
}

CStepper::~CStepper(void)
{
}

int CStepper::Initialize(byte nMotorId)
{
	m_nStepperId = nMotorId;
	return 0;
}



int CStepper::MoveMotorSteps(long lMoveDist, short nMotionMode)
{
	long iErrorStatus;
	iErrorStatus = theStepController.MoveMotorSteps(m_nStepperId,lMoveDist,nMotionMode);
	if(iErrorStatus != FOK)
		return iErrorStatus;
	return 0;
}



int CStepper::CheckMotionStop(void)
{
	int iBusyStatus = theStepController.CheckStepBusyState(m_nStepperId);
	if (iBusyStatus == 0)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}


int CStepper::StopStep(void)
{
	long iErrorStatus = FOK;
	theStepController.StopStep(m_nStepperId,0);
	if (iErrorStatus == FOK)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

int CStepper::CheckStepBusy(void)
{
	short nBusyStatus=theStepController.CheckStepBusyState(m_nStepperId);
	if (1 == nBusyStatus)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}	


int CStepper::SearchStepHome(void)
{
	long lErrorStatus = FOK;

	lErrorStatus = theStepController.SearchStepHome(m_nStepperId);
	if (lErrorStatus != FOK)
	{
		//		ReportStepError(lErrorStatus);
		return lErrorStatus;
	} 
	return 0;
}



int CStepper::GetStepHomeSensor(BOOL* bHomeBit, BOOL* bNLimitSensor, BOOL* bPLimitSensor)
{
	theStepController.GetStepperHomeSensor(m_nStepperId,bHomeBit,bNLimitSensor,bPLimitSensor);
	return 0;
}

// Servo Axis enable Servo motor. 
int CStepper::ServoOnServo(void)
{

	int	lAxisState = theStepController.ServoOnServoMotor(m_nStepperId);

	return lAxisState;
}

int CStepper::ServoOffServo(void)
{
	int lAxisState =  theStepController.ServoOffServoMotor(m_nStepperId);
	return lAxisState;
}

int CStepper::GetPraticalPos(long* lpValue)
{
	theStepController.GetPraticalFunction(m_nStepperId,lpValue);

	return 0;
}

int CStepper::GetTheoreticalPos(long* lpValue)
{
	theStepController.GetTheoreticalFunction(m_nStepperId,lpValue);

	return 0;
}



int CStepper::WaitStepHomeFinish(void)
{	 
	long    lErrorStatus;
	unsigned long ulHomeStatus;
	long    lStartTime, lElapseTime;

	lStartTime = GetCurrentTime();

	do {

		lErrorStatus = theStepController.CheckStepHomeStatus(m_nStepperId, &ulHomeStatus);

		if (lErrorStatus != FOK)
		{
			break;
		}

		if (ulHomeStatus == 0)
			break;

		lElapseTime = GetCurrentTime() - lStartTime;

		MSG message;
		while(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}

		Sleep(10);

	} while (lElapseTime < STEP_MOTOR_MOTION_TIMEOUT);


	return 0;

}
// //v6.1.2.J3 Add
int CStepper::WaitStepPosFinish(void)
{
	int    iStatus;
	long    lStartTime, lElapseTime;

	lStartTime = GetCurrentTime();

	do {

		iStatus = theStepController.CheckStepBusyState(m_nStepperId);

		if (iStatus == 0)
			break;

		lElapseTime = GetCurrentTime() - lStartTime;

		MSG message;
		while(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}

		Sleep(10);

	} while (lElapseTime < STEP_MOTOR_MOTION_TIMEOUT);


	return 0;
}
