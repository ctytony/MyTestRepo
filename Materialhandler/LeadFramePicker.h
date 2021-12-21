#pragma once

// CLeadFramePicker command target
typedef enum enLfPickerState {LF_PICKER_IDLE, LF_PICKER_FORWARD_MOTION, LF_PICKER_DELAY, LF_PICKER_END_STOP, LF_PICKER_BACKWARD_MOTION};

class CLeadFramePicker : public CObject
{
public:
	CLeadFramePicker();
	virtual ~CLeadFramePicker();
public:
    // Forward valve status
    short m_nForwardValveOn;
    // Backward valve status
    short m_nBackwardValveOn;
    // Forward cylinder sensor status, 0: not sensed, 1: sensed
    short m_nForwardCylinderSensorOn;
    // Backward cylinder sensor status, 0: not sensed, 1: sensed
    short m_nBackwardCylinderSensorOn;
    // Lead frame buffer sensor status, 0: Empty, 1: have buffer
    short m_nLeadFrameBufferSensorOn;
    // Set state of forward cylinder valve
    int SetForwardValveState(short nState);
    // Set state of backward cylinder valve
    int SetBackwardValveState(short nState);
    // Get sensor status of forward cylinder
    short GetForwardCylinderSensorStatus(void);
    // Get sensor status of backward cylinder
    short GetBackwardCylinderSensorStatus(void);
    // Get sensor status of lead frame buffer
    short GetLeadFrameBufferSensorStatus(void);
    // Status of lead frame picker
    short m_nPickerStatus;
    // Pick delay for magnet
    short m_nPickDelay;
    // Start time of picker delay
    long m_lPickerDelayStartTime;
private:
    // Forward valve start time
    long m_lForwardValveStartTime;
    // Backward valve start time
    long m_lBackwardValveStartTime;
};


