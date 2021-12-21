//VledOffload.h v1.0.T5

#pragma once

#include "Cylinder.h"
#include "Sensor.h"
#include "DataStruct.h"
#include "VledOnload.h"		//v3.3.T157 add

#pragma pack(push, 1)
#pragma warning( disable : 4351 )
typedef struct FORK_OFFLOAD_RCD
{
    short   nPusherDely;
    short   nForkCapacity;
	long    lKickerStandbyPos;
	long    lKickerEndPos;
	short   nKickerSpeed;
	short   nForkSwitchBuffer;
    long    lKickerEndPos2;
    char    chRev[24];

    //v2.19.0.w2 add - must init before use, or by default as 0
	FORK_OFFLOAD_RCD():
		nPusherDely(),
		nForkCapacity(),
		lKickerStandbyPos(),
		lKickerEndPos(),
		nKickerSpeed(),
		nForkSwitchBuffer(),
		lKickerEndPos2(){}
} _FORK_OFFLOAD_RCD;
#pragma warning( default : 4351 )
#pragma pack(pop)

typedef enum enForkOffloadEvent
{
    FORK_OFFLOAD_EVENT_IDLE,
    FORK_OFFLOAD_EVENT_OFFLOAD,
};

typedef enum enForkOffloadState
{
    FORK_OFFLOAD_STATE_IDLE,
	FORK_OFFLOAD_STATE_POS_STANDBY,
    FORK_OFFLOAD_STATE_LF_FEED_IN_DELAY,
	OFFLOAD_KICKER_STATE_WAIT_CLAW_CLOSE,
    FORK_OFFLOAD_STATE_WAIT_LF_KICK_OUT,
    FORK_OFFLOAD_STATE_WAIT_PUSH_ROTARY,
	OFFLOAD_KICKER_STATE_WAIT_CLAW_OPEN,
    OFFLOAD_KICKER_STATE_WAIT_KICKER_BACKWARD,
    FORK_OFFLOAD_STATE_WAIT_OFFLOAD_FINISH,
    FORK_OFFLOAD_STATE_STOP_OFFLOAD,
};

// CForkOffload command target

class CForkOffload : public CObject
{
public:
	CForkOffload();
	virtual ~CForkOffload();

public:	//variables
    CRelay theOutputClaw;
    CCylinder thePusherCylinder;
	CSensor theOffloadForkExistSensor;
    short m_nOffloadErrorCode;
    // Lead frame count on offload fork
    short m_nLeadframeCount;
	int SearchStepHome(void);
	int WaitStepHomeFinish(void);
	int OffloadLeadframe(void);
	int SwitchOutputSteps(void);
	BOOL bBuffSwitchFlag;
public:	//functions
    int Initialize();
    short GetOffloadEvent(void);
    int SubmitOffloadEvent(void);
    int ProcessOffloadEvent(void);
    int GetForkOffloadRcd(FORK_OFFLOAD_RCD* pstForkOffloadRcd);
    int UpdateForkOffloadRcd(FORK_OFFLOAD_RCD* pstForkOffloadRcd);
    int LoadForkOffloadRcd(void);
    int SaveForkOffloadRcd(void);
    BOOL GetOffloadValidState(void);
    int SetForkOffloadValid(BOOL bValid);
    int ResetForkEvent(void);
    // Check if offload full
    bool CheckOffloadFull(void);
    // Get onload empty msg reported status
    bool GetForkFullMsgReported(void);
    // Get onload empty msg reported status
    int SetForkFullMsgReported(void);
    // Reset onload empty flag
    int ResetForkFullFlag(void);
    short GetOffloadErrorCode(void);
    short ResetOffloadErrorCode(void);
    short GetOffloadWarningCode(void);
    short GetOffloadState(void);
private:	//variables
    byte m_nKickerId;
    short m_nOffloadEvent;
    short m_nOffloadState;
    short m_nLastOffloadDebugState;
    // Start time of LF out delay
    long  m_lWaitLfOutStartTime;
    short m_nOffloadWarningCode;
    FORK_OFFLOAD_RCD stForkOffloadRcd;
	STEP_SPEED_PROFILE m_stKickerSpeedProfile;
    BOOL m_bOffloadValid;
    // Offload fork full message reported flag
    bool m_bForkFullMsgReported;
	long m_lClawDelayTime;
};

extern CForkOffload theVledOffload;
