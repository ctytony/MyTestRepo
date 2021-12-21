//TopClamp.h v1.0.T22

#pragma once

#include "DataStruct.h"
#include "StepperControl.h"
#include "Sensor.h"
#include "Relay.h"

// CTopLfClamp command target

#define  CLAMPER_FULL_OPEN_PULSE      0
#define  CLAMPER_FULL_CLOSE_PULSE     800
#define  CLAMPER_ONE_ROUND_PULSE      1600

//v1.0.T16 add
#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct TOP_CLAMP_RCD	//v1.0.T18 rename
{
	long lWindClampSafeClosePos;
	long lWindClampPreClosePos;
    long lWindClampClosePos;
	char chReserve[88];

    //must init before use, or by default as 0
	TOP_CLAMP_RCD():
		lWindClampSafeClosePos(),
		lWindClampPreClosePos(),
		lWindClampClosePos(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

class CTopLfClamp : public CObject
{
public:
	CTopLfClamp();
	virtual ~CTopLfClamp();
public:	//functions
	long GetVacDuration();	//v1.0.T12 add
	bool IsVacuumOn();		//v1.0.T12 add
	void SetVacuumOn(bool bOn);	//v1.0.T12 add
	void VerifyData();		//v1.0.T9 add
	void Init(void);
	void CloseClamp(void);
	void CheckClampStatus(void);
	short GetClampStatus(void);
	TOP_CLAMP_RCD *GetRcd();	//v1.0.T17 add
	void LoadRcd(void);
	void SaveRcd(void);
	void HomeClamp(void);
	void WaitHomeClamp();
	void OpenClamp(void);
	void PreCloseClamp(void);
	void MoveClampSafePos(void);
    void GetSpeedProfile(STEP_SPEED_PROFILE *pstSpeedProfile);
    void SetSpeedProfile(STEP_SPEED_PROFILE *pstSpeedProfile);
	void GetAxisPara(ST_AXIS_PARA *pstAxisPara);
	void SetAxisPara(ST_AXIS_PARA *pstAxisPara);
	void LoadSpeedProfile();
	void SaveSpeedProfile();
    int CheckVacuumStatus(void);
	byte GetStepID(void);
private:	//function
	void GetRcd(TOP_CLAMP_RCD *pstTopClamp);
	void SetRcd(TOP_CLAMP_RCD *pstTopClamp);
private:	//variable
    CRelay m_VacuumValve;	//v1.0.T12 edit
	TOP_CLAMP_RCD m_stTopClamp;
    STEP_SPEED_PROFILE m_stSpeedProfile;
	ST_AXIS_PARA m_stAxisPara;
	short m_nClampStatus;
    CSensor m_VacuumSensor;
	byte m_nStepID;
};

extern CTopLfClamp theTopClamp;	//v1.0.T19 rename
