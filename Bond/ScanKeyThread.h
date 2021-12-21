#pragma once

#include "Sensor.h"

enum EN_JOYSTICK_CONFIG 
{
	JOYSTICK_NONE = 0,
	JOYSTICK_SERVO,
	JOYSTICK_STEPPER_HORIZONTAL,
	JOYSTICK_STEPPER_VERTICAL,	      
};

// CScanKeyThread command target

class CScanKeyThread : public CObject
{
public:
	CScanKeyThread();
	virtual ~CScanKeyThread();
public:	//variables
	bool m_bScanEnable;				//v2.0.T37 rename
	short m_nJoyStickConfig;
	bool m_bKeyLeftPressed;    
	bool m_bKeyRightPressed;
	bool m_bKeyUpPressed;
	bool m_bKeyDownPressed;
	CSensor m_HeaterDetector;		//v2.0.T37 rename
	CSensor m_WireClampButton;		//v2.0.T37 rename
	CSensor m_JoyStickPositiveX;	//v2.0.T37 rename
	CSensor m_JoyStickNegativeX;	//v2.0.T37 rename
	CSensor m_JoyStickPositiveY;	//v2.0.T37 rename
	CSensor m_JoyStickNegativeY;	//v2.0.T37 rename
public:	//functions
	void BeginThread(void);
	void EnableJoyStick(void);
	void DisableJoyStick(void);
	bool IsJoyStickValid(void);
	void SetJoyStickConfig(short nConfig, BYTE bMotor);
	void StopThread(void);
	void Init(void);
	bool IsDirectionKeyPressed(void);
private:	//variables
	CWinThread* m_pScanThread;	//v2.0.T37 rename
	bool m_bJoyStickValid;
	BYTE m_bStepperId;
};

extern CScanKeyThread theScanKeyThread;
