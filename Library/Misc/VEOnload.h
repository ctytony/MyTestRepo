//VEOnload.h v1.0.T4

#pragma once

#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"	//v2.18.6.T41 add, v2.20.Q1 edit
#include "Relay.h"
#include "VLED.h"		//v3.3.T9 add

// CVEliteOnload command target
#define DROP_LF_POS 0
//#define LF_STANDBY_POS -700	//v3.3.w18 delete
#define VE_LF_STANDBY_POS 700	//v3.3.w18 add

#define WAIT_ONLOAD_TRACK_SENSOR_TIME   1500	//1500ms
#define ONLOAD_PICKER_RETRY_TIME        2		//@2
#define PICK_SHAKE_LEN                  2000

#define STEPPER_LIMIT_POS               80000 

#define BACK_FORK                       2
#define PICKER_SENSOR_ON                0
#define PICKER_SENSOR_OFF               1 

class CVEliteOnload : public CObject
{
public:
	CVEliteOnload();
	virtual ~CVEliteOnload();

public:	//variables
	// Lead frame exist sensor
    CSensor theLfExistSensor;
	//CSensor theLimitPosSensor;	//v3.3.T20 delete
	CSensor thePickerHoldPosSensor;
	CSensor theDropLfPosSensor;
	CSensor theForkExistSensor;
public:	//functions
    short GetEvent(void);
    void SubmitEvent(short nEvent);
    void ProcessEvent(void);
    // Onload lead frame
    int OnloadLeadFrame();
    void SetValid(bool bValid);
    bool IsValid(void);
    // Search stepper home
    int SearchStepHome(void);
    // Stop stepper motion
    int StopStepMotion(void);
    // Wait stepper home finish
    int WaitStepHomeFinish(void);
    // Wait step motion complete
    int WaitStepMotionComplete(void);
    bool IsEmpty(void);
    bool IsEmptyMsgReported(void);
    void SetEmptyMsgReported(void);
    void ResetEmpty(void);
    void ResetEvent(void);
    short GetErrorCode(void);
    void ResetErrorCode(void);
    short GetWarningCode(void);
	void MovePickerForward(void);
	void MovePickerBackward(void);
    void SetPickerHighSpeed(bool bHighSpeed);
	void Init(void);
    void GetRcd(LEADFRAME_ONLOAD_RCD* pstRcd);
    void SetRcd(LEADFRAME_ONLOAD_RCD* pstRcd);
    void LoadRcd(void);
    void SaveRcd(void);
private:	//variables
    // Start time of picker push to avoid lead frame tile
    long m_lPickerPushStartTime;
    // Start time of droping LF to avoid drop outside
	long m_lPickerDropStartTime;
    short m_nState;
    short m_nLastDebugState;
    bool m_bEmpty;
    bool m_bEmptyMsgReported;
    // Step motor id of onload pusher
    byte m_bPusherStepId;
    // Onload pusher step speed profile
    STEP_SPEED_PROFILE m_stPickerSpeedProfile;
	long m_lDist;
	long m_lMove;
    //Onload interval too short times
    short m_nFrequentOnloadTimes;
    long m_lEventStartTime;
    short m_nEvent;
    short m_nErrorCode;
    short m_nWarningCode;
    // Onload  track pick state 
    bool m_nTrackPickState;
    // Lead frame onload record
    LEADFRAME_ONLOAD_RCD m_stRcd;
    bool m_bValid;
};

extern CVEliteOnload theVEliteOnload;	//v3.3.T54 add
