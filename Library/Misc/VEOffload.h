//VEOffload.h v1.0.T4

#pragma once

#include "Cylinder.h"
#include "Relay.h"
#include "Sensor.h"
#include "DcMotor.h"
#include "DataStruct.h"

#pragma pack(push, 1)
#pragma warning( disable : 4351 )
typedef struct VE_OFFLOAD_RCD
{
	//LF kicker data
	int nLfKickDist;
	//Mag indexer data
	int nMagSupportPosY;
	int nMagSupportPosZ;
	int nMagDropPosZ;
	int nMag1stSlotPosY;
	int nMag1stSlotPosZ;
	int nMagKickerPosY;
	int nMagKickerPosZ;
	short nPitchWidth;
	short nMagSlotNo;
	int nMagWidth;
	char cReserve[100];

	VE_OFFLOAD_RCD():
		nLfKickDist(),
		nMagSupportPosY(),
		nMagSupportPosZ(),
		nMagDropPosZ(),
		nMag1stSlotPosY(),
		nMag1stSlotPosZ(),
		nMagKickerPosY(),
		nMagKickerPosZ(),
		nPitchWidth(),
		nMagSlotNo(),
		nMagWidth(){}
} _VE_OFFLOAD_RCD;
#pragma warning( default : 4351 )
#pragma pack(pop)

typedef enum enVEliteOffloadEvent
{
    VE_OFFLOAD_EVENT_IDLE,
    VE_OFFLOAD_EVENT_OFFLOAD,
    VE_OFFLOAD_EVENT_LOAD_MAG,
    VE_OFFLOAD_EVENT_MAG_MOVE_PITCH_FORWARD,
    VE_OFFLOAD_EVENT_MAG_MOVE_PITCH_BACKWARD,
    VE_OFFLOAD_EVENT_EJECT_MAG,
};

typedef enum enVEliteOffloadState
{
    VE_OFFLOAD_STATE_IDLE,
    VE_OFFLOAD_STATE_START,

	VE_OFFLOAD_STATE_LFKICKER_MOVETO_HOME,
	VE_OFFLOAD_STATE_LFKICKER_WAIT_HOME,

	VE_OFFLOAD_STATE_LFKICKER_KICK_LF,
	VE_OFFLOAD_STATE_LFKICKER_KICK_LF_DELAY,
	VE_OFFLOAD_STATE_LFKICKER_KICK_LF_DELAY_2,
	VE_OFFLOAD_STATE_LFKICKER_WAIT_KICK_LF,

	VE_OFFLOAD_STATE_MAG_MOVETO_SUPPORT,
	VE_OFFLOAD_STATE_MAG_WAIT_SUPPORT,

	VE_OFFLOAD_STATE_MAG_MOVETO_DROP,
	VE_OFFLOAD_STATE_MAG_WAIT_DROP,

	VE_OFFLOAD_STATE_MAG_MOVETO_FIRST_SLOT_Y,
	VE_OFFLOAD_STATE_MAG_WAIT_FIRST_SLOT_Y,

	VE_OFFLOAD_STATE_MAG_MOVETO_FIRST_SLOT_Z,
	VE_OFFLOAD_STATE_MAG_WAIT_FIRST_SLOT_Z,

	VE_OFFLOAD_STATE_MAG_MOVE_PITCH_FORWARD,
	VE_OFFLOAD_STATE_MAG_WAIT_PITCH_FORWARD,

	VE_OFFLOAD_STATE_MAG_MOVE_PITCH_BACKWARD,
	VE_OFFLOAD_STATE_MAG_WAIT_PITCH_BACKWARD,

	VE_OFFLOAD_STATE_MAG_MOVETO_KICKER_Y,
	VE_OFFLOAD_STATE_MAG_WAIT_KICKER_Y,

	VE_OFFLOAD_STATE_MAG_MOVETO_KICKER_Z,
	VE_OFFLOAD_STATE_MAG_WAIT_KICKER_Z,

	VE_OFFLOAD_STATE_MAG_MOVETO_HOME_Y,
	VE_OFFLOAD_STATE_MAG_WAIT_HOME_Y,

	VE_OFFLOAD_STATE_MAG_MOVETO_HOME_Z,
	VE_OFFLOAD_STATE_MAG_WAIT_HOME_Z,

	VE_OFFLOAD_STATE_OPEN_SUPPORT,
	VE_OFFLOAD_STATE_WAIT_OPEN_SUPPORT,

	VE_OFFLOAD_STATE_CLOSE_SUPPORT,
	VE_OFFLOAD_STATE_WAIT_CLOSE_SUPPORT,

	VE_OFFLOAD_STATE_MAGKICKER_KICK_MAG,
	VE_OFFLOAD_STATE_MAGKICKER_WAIT_KICK_MAG,

	VE_OFFLOAD_STATE_MAGKICKER_MOVETO_HOME,
	VE_OFFLOAD_STATE_MAGKICKER_WAIT_HOME,

    VE_OFFLOAD_STATE_STOP,
};

class CVEliteOffload : public CObject
{
public:
	CVEliteOffload();
	virtual ~CVEliteOffload();

public:	//variables
	CSensor m_MagKickerHomeSensor;
	CSensor m_MagKickerLimitSensor;
	CSensor m_LfKickerJamSensor;
	CSensor m_LfOutputJamSensor;
	CSensor m_MagArmPresenceSensor;
	CSensor m_MagBufDropSensor;
	CSensor m_MagBufPresenceSensor;
	CSensor m_MagFullSensor;
	CSensor m_MagKickerPresenceSensor;
	CDcMotor m_MagKicker;
	CRelay m_MagSupportSol;
	CRelay m_MagSupportSol2;
	CRelay m_LfKickerSol;
	CRelay m_MagLockSol;
public:	//functions
	void OffloadLeadframe(void);
	bool IsError(void);
	bool IsKickingLF(void);
	void SearchStepHome(void);
	void WaitStepHomeFinish(void);
    void Init();
    void SubmitEvent(short nEvent);
    void ProcessEvent(void);
    void GetRcd(VE_OFFLOAD_RCD* pstRcd);
    void SetRcd(VE_OFFLOAD_RCD* pstRcd);
    void LoadRcd(void);
    void SaveRcd(void);
	short GetEvent();
	short GetState(void);
	short GetErrorCode(void);
	void ResetErrorCode(void);
	void ResetEvent(void);
private:	//variables
	bool m_bFirstLF;
	bool m_bKickingLF;
	short m_nErrorCode;
	short m_nEvent;
	short m_nState;
	short m_nCurMagSlot;
	DWORD m_dwStartTime;
    VE_OFFLOAD_RCD m_stRcd;
private:	//functions
	void WaitStepHomeFinish(BYTE nStepper);
	bool IsFirstLF(void);
	void SetKickingLF(bool bKicking);
	void SetErrorCode(short nErrorCode);
};

extern CVEliteOffload theVEliteOffload;
