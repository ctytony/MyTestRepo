#pragma once
#include "afx.h"
#include "DataStruct.h"

class CStepper :
	public CObject
{
public:
	CStepper(void);
	~CStepper(void);

	int Initialize(byte nMotorId);
private:
	byte m_nStepperId;
public:
	int MoveMotorSteps(long lMoveDist, short nMotionMode);
	int CheckMotionStop(void);
	int StopStep(void);
public:
	int CheckStepBusy(void);
	int SearchStepHome(void);
	int GetStepHomeSensor(BOOL* bHomeBit, BOOL* bNLimitSensor, BOOL* bPLimitSensor);
	int ServoOnServo(void);
	int GetPraticalPos(long* lpValue);
	int GetTheoreticalPos(long* lpValue);
	int ServoOffServo(void);
	int WaitStepHomeFinish(void);
	// //v6.1.2.J3 Add
	int WaitStepPosFinish(void);
};
