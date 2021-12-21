//StepperControl.h v1.0.T14

#pragma once

#include "Stepper.h"
#include "StepperIO.h"
#include "MachineConfig.h"

// CStepperControl

class CStepperControl : public CObject
{
public:
	CStepperControl();
	virtual ~CStepperControl();
public:	//functions
	int Init();
	int ReportError(int nError);
	int DisableStepper();
	int SetSpeedParameter(byte nMotor, STEP_SPEED_PROFILE* pstStepSpeedProfile, bool bDelay = false);	//v1.0.0.T17 edit
	int SetSpeedParameter(byte nMotor, ST_AXIS_PARA stAxisPara);
	int MoveStep(byte nMotor, long lStep, short nMotionMode = MOTION_ABSOLUTE);
	int MoveDist(byte nMotor, long lDist_x1um, short nMotionMode = MOTION_ABSOLUTE);
	int MoveStepSearch(byte nMotor, long lStep);
	//int CheckMotionStatus(byte nMotor, long* plDestinationDist, BOOL bCallBackFree = TRUE);	//v1.0.T4 delete
	int GetPos(byte nMotor, long* plPos, BOOL bCallBackFree = TRUE);	//v1.0.T4 add
	long GetPos(byte nMotor, BOOL bCallBackFree = TRUE);	//v1.0.T11 add
	double GetResol(byte nMotor);	//v1.0.T9 rename
	int SearchHome(byte nMotor);
	int StopMotor(byte nMotor, byte nMode = 0x00);
	bool IsHome(byte nMotor);
	bool IsBusy(byte nMotor);
	int SetHomeLogic(byte nMotor, unsigned short nValue);
	int ServoOn(byte nMotor);
	void SavePosToReg(byte nMotor, long lPowerOnPos);
	int GetMotorStopReason(byte nMotor, BOOL* bitHome, BOOL* bitNLimit, BOOL* bitPLimit);
	int SetSpeedProfile(byte nMotor, STEPPER_SPEED_SETTING *pstSpeedProfile);
	int SetSpeedProfile(byte nMotor, ST_STEPPER_SPEED *pstSpeedProfile);
	int GetHomeSensor(byte nMotor, BOOL* bHomeBit, BOOL* bNLimitBit, BOOL* bPLimitBit);
	void GetStepReg(DWORD *pulReg);
	int StartJog(byte bStepperId, short nMotionDir);
	int SetJogStep(long lStep);		//how long jog move per step
	int DisableScurveProfile(byte nMotor, STEP_SPEED_PROFILE* pstStepSpeedProfile);
	int GetHomeSensor(BYTE nStepId, BOOL* bHome);
	int GetLimitSensor(BYTE nStepId, BOOL* bPlimit, BOOL* bNlimit);
	int SetOutputPort(short nOutputId, short nState);
	int PowerOn(void);
	int PowerOff(void);
	int CheckPowerStatus(bool *pbStepperOn);
	long GetAxisOffset(byte nMotor);
	void SetAxisOffset(byte nMotor, long lOffset);
private:	//variables
	long m_lAxisOffset[MAX_STEPPER_AXIS];	//machine offset for each axis
private:	//functions
	int CheckHomeStatus(byte nMotor, unsigned long* pulStatus);
};

extern CStepperControl theStepper;
