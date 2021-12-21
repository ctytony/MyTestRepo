//VledOnload.h v1.0.T3

#pragma once

#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"	//v2.18.6.T41 add, v2.20.Q1 edit
#include "Relay.h"
#include "VLED.h"		//v3.3.T9 add

// CLfOnload command target
#define DROP_LF_POS			0
#define LF_STANDBY_POS		-700

//v3.3.T9 delete
//typedef enum enOnloadType
//{
//    ONLOADER_NORMAL_LF,
//    ONLOADER_COPPER_LF,
//};

//v3.3.T9 delete
//typedef enum enOnloadEvent 
//{
//    ONLOAD_IDLE, 
//    ONLOAD_EVENT_START,
//};

//v3.3.T9 delete
//typedef enum enOnloadState
//{
//    ONLOAD_STATE_IDLE,
//	ONLOAD_STATE_WAIT_STEP_ABOVE_TRACK,
//	ONLOAD_STATE_WAIT_LF_DROP_DOWN,
//    ONLOAD_STATE_WAIT_PICKER_FORWARD,
//    ONLOAD_STATE_WAIT_BACK_DELAY,
//    ONLOAD_STATE_WAIT_PICKER_BACK,
//	ONLOAD_STATE_WAIT_DROP_LF_DELAY,
//	ONLOAD_STATE_ONLOAD_END, //@1
//};

//v3.3.T9 delete
//typedef enum enOnloadPickState
//{
//    ONLOAD_PICK_SUCCESS,
//    ONLOAD_PICK_PENDING,
//};

#define WAIT_ONLOAD_TRACK_SENSOR_TIME   1500	//1500ms
#define ONLOAD_PICKER_RETRY_TIME        2		//@2
#define PICK_SHAKE_LEN                  2000
#define STEPPER_LIMIT_POS               80000 
#define BACK_FORK                       2
#define PICKER_SENSOR_ON                0
#define PICKER_SENSOR_OFF               1 

class CLfOnload : public CObject
{
public:
	CLfOnload();
	virtual ~CLfOnload();
    
	// Lead frame exist sensor
    CSensor theLfExistSensor;
    
	CSensor theLimitPosSensor;

	CSensor thePickerHoldPosSensor;

	CSensor theDropLfPosSensor;

	CSensor theForkExistSensor;

	
   
    // Initialize of onloading system
	int Initialize(void);
private:
    // Event of onload
    short m_nOnloadEvent;
    // Onload error code //@1
    short m_nOnloadErrorCode;
    // Onload warning code
    short m_nOnloadWarningCode;
    // Onload  track pick state 
    bool m_nTrackPickState;
   
public:
    // Get event of onload
    short GetOnloadEvent(void);
    // Submit an event to onload
    int SubmitOnloadEvent(short nEvent);
    // Process event of onload
    int ProcessOnloadEvent(void);
    // Onload lead frame
    int OnloadLeadFrame();
private:
    // Lead frame onload record
    LEADFRAME_ONLOAD_RCD stLeadFrameOnloadRcd;
public:
    // Get lead frame onload record
    int GetLfOnloadRcd(LEADFRAME_ONLOAD_RCD* pstLfOnloadRcd);
    // Update lead frame onload record
    int UpdateLfOnloadRcd(LEADFRAME_ONLOAD_RCD* pstLfOnloadRcd);
    // Load lead frame onload record from data buffer
    int LoadLfOnloadRcd(void);
    // Save lead frame onload record to data buffer
    int SaveLfOnloadRcd(void);
private:
    // Onload valid flag
    BOOL m_bOnloadValid;
public:
    // Set onload valid flag
    int SetOnloadValid(BOOL bOnloadValid);
    // Get onload valid flag
    BOOL GetOnloadValidState(void);
private:
    // Wait onload track sensor flag
    //bool m_bWaitTrackSensor;
    // Start time to wait on load inner track sensor
    //long m_lTrackWaitStartTime;
    // Inner track pick LF failure times @2
    //long m_lTrackPickFailTimes;
    //Onload interval too short times
    short m_nFrequentOnloadTimes;
    // Start time of onload event
    long m_lOnloadEventStartTime;
public:
    // Update wait onload track sensor state
    //int UpdateWaitSensorState(void);
    // Get wait sensor state
    //bool GetWaitSensorState();
private:
    // Step motor id of onload pusher
    byte m_bPusherStepId;
    // Onload pusher step speed profile
    STEP_SPEED_PROFILE m_stPickerSpeedProfile;

	long m_lDist;

	long m_lMove;
public:
    // Search stepper home
    int SearchStepHome(void);
    // Stop stepper motion
    int StopStepMotion(void);
    // Wait stepper home finish
    int WaitStepHomeFinish(void);
    // Wait step motion complete
    int WaitStepMotionComplete(void);

private:
    // Onload state
    short m_nOnloadState;
    // Last state for debug
    short m_nLastOnloadDebugState;
    // Onload buffer empty flag
    bool m_bOnloadEmpty;
    // Onload buffer empty message reported
    bool m_bOnloadEmptyMsgReported;
public:
    // Get onload empty flag
    bool GetOnloadEmptyFlag(void);
    // Get onload empty msg reported status
    bool GetOnloadEmptyMsgReported(void);
    // Get onload empty msg reported status
    int SetOnloadEmptyMsgReported(void);
    // Reset onload empty flag
    int ResetOnloadEmptyFlag(void);
private:
    // Start time of picker push to avoid lead frame tile
    long m_lPickerPushStartTime;
    // Start time of droping LF to avoid drop outside
	long m_lPickerDropStartTime;
public:
    // Reset onload event
    int ResetOnloadEvent(void);
    // Get onload error code //@1
    short GetOnloadErrorCode(void);
    // Reset onload error code
    short ResetOnloadErrorCode(void);
    // Get onload warning code
    short GetOnloadWarningCode(void);
	//Move picker forward
	int MovePickerForward(void);
    //Move picker backward
	int MovePickerBackward(void);
    // Change picker speed
    int ChangePickerSpeed(bool bHighSpeed);
};

extern CLfOnload theVledOnload;
