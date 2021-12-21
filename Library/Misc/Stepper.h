//Stepper.h v1.0.T7

#pragma once

#include "Motion.h"

enum enStepAssign 
{
    BACK_LF_CLAMP_STEPPER		= 1,	//X_Axis
    FRONT_LF_CLAMP_STEPPER		= 2,	//Y_Axis
    ONLOAD_PUSHER_STEPPER		= 3,	//Z_Axis
    MAG_INDEX_STEPPER			= 4,	//U_Axis
    MAG_ELEVATOR_STEPPER		= 5,	//X_Axis
    WIRE_SPOOL_STEPPER			= 6,	//Y_Axis
};

enum enVledEliteStepper
{
	VE_STEPPER_ONLOAD_INJECTOR			= 1,	//X
	VE_STEPPER_2						= 2,	//Y
	VE_STEPPER_3						= 3,	//Z
	VE_STEPPER_ONLOAD_PICKER			= 4,	//U
	VE_STEPPER_OFFLOAD_KICKER			= 5,	//X
	VE_STEPPER_OFFLOAD_MAG_Y			= 6,	//Y
	VE_STEPPER_OFFLOAD_MAG_Z			= 7,	//Z
	VE_STEPPER_8						= 8,	//U
	VE_STEPPER_INDEX_LEFT_CLAW			= 9,	//X
	VE_STEPPER_INDEX_CLAMPER			= 10,	//Y
	VE_STEPPER_INDEX_RIGHT_CLAW			= 11,	//Z
	VE_STEPPER_12						= 12,	//U
};

enum enSingleStepAssign 
{
	SINGLE_CAP_INJECTOR			= 1,	//X_Axis
	SINGLE_CAP_LEFT_CLAW		= 2,	//Y_Axis
	SINGLE_CAP_RIGHT_CLAW		= 3,	//Z_Axis
	SINGLE_CAP_OFFLOAD_KICKER	= 4,	//U_Axis //s curve: setting: 6
	SINGLE_CAP_ONLOAD_PICKER	= 5,	//X_Axis
	SINGLE_CAP_CLAMPER			= 6,	//Y_Axis //s curve: setting: 2
};

enum enTopStepAssign  //need to check motor assignment
{
    TOP_ONLOAD_HOLDER			= 1,	//X_Axis
	TOP_ONLOAD_MAG_CLAMPER		= 2,	//Y_Axis
    TOP_WINDOW_CLAMPER			= 3,	//Z_Axis 
    TOP_DEVICE_INDEXER			= 4,	//U_Axis
	TOP_OFFLOAD_HOLDER			= 5,	//X_Axis //card2
	TOP_OFFLOAD_MAG_CLAMPER		= 6,	//Y_Axis
};

enum enStepOutputIoBitId
{
    STEP_OUTPUT_N2H2_PORT,
    //STEP_OUTPUT_PANEL_LIGHT_PORT,	// move to digital I/O
    //STEP_OUTPUT_TRACK_LF_HOLDER,
	STEP_OUTPUT_AIR_TENSIONER,
};

enum enCuEliteStepAssign
{
//								= 0,	//servo motor
    CUELITE_ONLOAD_ELEVATOR		= 1,	//X_Axis
    CUELITE_ONLOAD_XAXIS		= 2,	//Y_Axis
    CUELITE_ONLOAD_YONE			= 3,	//Z_Axis
    CUELITE_ONLOAD_YTWO			= 4,	//U_Axis

    CUELITE_OFFLOAD_ELEVATOR	= 5,	//X_Axis // card 2
    CUELITE_OFFLOAD_XAXIS		= 6,	//Y_Axis
    CUELITE_OFFLOAD_YONE		= 7,	//Z_Axis
    CUELITE_OFFLOAD_YTWO		= 8,	//U_Axis

    CUELITE_FRONT_RAIL          = 9,    //X_Axis //card 3
    CUELITE_WINDOW_CLAMPER      = 10,   //Y_Axis
    CUELITE_DEVICE_INDEXER      = 11,   //Z_Axis
    CUELITE_REAR_RAIL           = 12,   //U_Axis

    CUELITE_ONLOAD_YONE_YTWO         = 100,		//for some special fuction use only, NOT for motor class
    CUELITE_OFFLOAD_YONE_YTWO        = 101, 
    CUELITE_ONLOAD_YONE_YTWO_WIDTH   = 102,		//for some special fuction use only, NOT for motor class
    CUELITE_OFFLOAD_YONE_YTWO_WIDTH  = 103,
    CUELITE_FRONT_REAR               = 104,		//for some special fuction use only, NOT for motor class
};

struct STEP_SPEED_PROFILE
{
    byte  bTCurveAcc;           //T curve acceleration
    long  lStartVelocity;       //Start velocity
    long  lDriveVelocity;       //Const velocity
    long  lMaxDriveVelocity;    //
    long  lAcc;                 //Acceleration
    long  lAccRatio;            //Jerk
    
    unsigned long   ulHomeType;
    unsigned long   ulHomeCoarseSrchDir;
    unsigned long   ulHomeCoarseSrchSpeed;
    unsigned long   ulHomeFineSrchDir;
    unsigned long   ulHomeFineSrchSpeed;
    long            lHomeOffset;
};

struct STEPPER_SPEED_SETTING
{
    byte  bTCurveAcc;           //T curve acceleration
    long  lStartVelocity;       //Start velocity
    long  lDriveVelocity;       //Const velocity
    long  lMaxDriveVelocity;    //
    long  lAcc;                 //Acceleration
    long  lAccRatio;            //Jerk
};

//very important, software limit, don't change
#define CUELITE_ONLOAD_ELEVATOR_MAX		2400000		//2425000 0.1um 64000 count from as 0, physically
#define CUELITE_ONLOAD_XAXIS_MAX		1750000		//0.1um 140000 count from as 0, physically
#define CUELITE_ONLOAD_YONE_MAX			225000		//0.1um 28000 count from as 0, physically
#define CUELITE_ONLOAD_YTWO_MAX			225000		//0.1um 32000 count from as 0, physically

#define CUELITE_OFFLOAD_ELEVATOR_MAX	2400000		//2425000 0.1um 64000 count from as 0, physically
#define CUELITE_OFFLOAD_XAXIS_MAX		1750000		//0.1um 140000 count from as 0, physically
#define CUELITE_OFFLOAD_YONE_MAX		225000		//0.1um 28000 count from as 0, physically
#define CUELITE_OFFLOAD_YTWO_MAX		225000		//0.1um 32000 count from as 0, physically

#define CUELITE_FRONT_RAIL_MAX			243750		//0.1um 13000 count from as 0, physically
#define CUELITE_WINDOW_CLAMPER_MAX		10000		//0.1   1000 count from as 0, physically
#define CUELITE_DEVICE_INDEXER_MAX		130000		//um    8000 count from as 0, physically
#define CUELITE_REAR_RAIL_MAX			206250		//0.1um 11000 count from as 0, physically

#define STEP_CONTROLLER_BOARD_A         0 
#define STEP_CONTROLLER_BOARD_B         1
#define STEP_CONTROLLER_BOARD_C         2

#define STEP_MOTOR_MOTION_TIMEOUT       20000
#define STEP_MOTOR_CMD_RANGE            800000

#define STEPPER_STOP_FAST               0x00
#define STEPPER_STOP_DEC                0x0F

#define MAX_STEPPER_AXIS                15

class CStepController : public CObject
{
public:
	CStepController();
	virtual ~CStepController();
public:	//functions
    int Init();
    int ReportStepError(int lErrorStatus);
    int DisableStepper();
    int SetStepSpeedParameter(byte nMotor, STEP_SPEED_PROFILE* pstStepSpeedProfile, bool bDelay = false);	//v1.0.0.T17 edit
    int MoveStep(byte nMotor, long lStep, short nMotionMode);
    int MoveDist(byte nMotor, long lDistMicron, short nMotionMode);
    int CheckStepMotionStatus(byte nMotor, long* plDestinationDist, BOOL bCallBackFree = TRUE);
    double GetStepResolution(byte nMotor);
    int GetStepMotorAxis(byte nMotor, byte* pbBoardId, byte* pbMotorAxis);
    int SearchStepHome(byte nMotor);
    int StopStep(byte nMotor, byte nMode = 0x00);
    int CheckStepHomeStatus(byte nMotor, unsigned long* pulStatus);
    int CheckStepBusyState(byte nMotor);
	int SetHomeLogic(byte nMotor, unsigned short nValue);
	int ServoOnServoMotor(byte nMotor);
	void SavePosToReg(byte nMotor,long lPowerOnPos);
	void ReadMotorStopReason(byte nMotor, BOOL* bitHome, BOOL* bitNLimit, BOOL* bitPLimit);
	void SetMotorSpeedProfile(byte nMotor,STEPPER_SPEED_SETTING *pstSpeedProfile);
	void GetStepperHomeSensor(byte nMotor, BOOL* bHomeBit,BOOL* bNLimitBit,BOOL* bPLimitBit);
    void GetStepReg(DWORD *pulReg);
	void StartJogStepper(byte bStepperId, short nMotionDir);
	void SetJogStep(long lStep);		//how long jog move per step
	int DisableScurveProfile(byte nMotor, STEP_SPEED_PROFILE* pstStepSpeedProfile);
	void GetHomeSensor(BYTE nStepId, BOOL* bHome);
	void GetLimitSensor(BYTE nStepId, BOOL* bPlimit, BOOL* bNlimit);
    void SetOutputPort(short nOutputId, short nState);
	int PowerOn(void);
	int PowerOff(void);
    int CheckStepperPowerStatus(bool *pbStepperOn);
    long GetAxisOffset(byte nMotor) {return m_lAxisOffset[nMotor - 1];};
    void SetAxisOffset(byte nMotor, long lOffset) {m_lAxisOffset[nMotor - 1] = lOffset;};
private:	//variables
	short m_nHomeDir[MAX_STEPPER_AXIS];
	long m_lAxisOffset[MAX_STEPPER_AXIS];	//machine offset for each axis
	long m_lJogStep;
};

extern CStepController theStepController;
