//FoxconnIndex.h

#pragma once

#include "Sensor.h"
#include "DataStruct.h"
#include "StepperControl.h"
#include "TrackSensor.h"
#include "Relay.h"
#include "DeviceData.h"

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct FC_TRACK_DATA
{
	short			lDelayType;
	unsigned long	lHeatingDelay;
	byte			nPrIndexEnable;					//bool bPrIndexEnable;
	long			lLimitTolerance;
	long			lSrchBackLen;
	long			lLfStandbyLen;
	long			nIndexSpeedCoeff;				//1,2,3,4,5,6,7
	long			lIndexError[TOP_MAX_INDEX_TIME];
	short			nEjectSpeedCoeff;				//1,2,3,4,5
	long			l1stSlipHeatDelay;
	long			lPrIndexTolerance;
	long			lIndexPosOffset;
	long			lOpenClampDelay;
	long			lIndexPrDelay;
	unsigned long	lInitDelay;
    bool			bVacuumControl;
	unsigned long	lVacuumDelay;
	unsigned long	l1stUnitHeatDelay;
	short			sPRIndexScore;
	int				iOutputMagazineSlotSequence;
	BYTE			nNextStepPusherIn;
	BYTE			nHeatDelayUnit;
	BYTE			nLFWaitPos;
	BYTE			bFlattenLF;
	WORD			wFlattenDuration;
	//char			chReserve[157];					//159 -> 157
	//char			chReserve[158];					//157 -> 158
	char			chReserve[155];					//158 -> 155

    //must init before use, or by default as 0
	FC_TRACK_DATA():
		lDelayType(),
		lHeatingDelay(),
		nPrIndexEnable(),
		lLimitTolerance(),
		lSrchBackLen(),
		lLfStandbyLen(),
		nIndexSpeedCoeff(),
		lIndexError(),
		nEjectSpeedCoeff(),
		l1stSlipHeatDelay(),
		lPrIndexTolerance(),
		lIndexPosOffset(),
		lOpenClampDelay(),
		lIndexPrDelay(),
		lInitDelay(),
		bVacuumControl(),
		lVacuumDelay(),
		l1stUnitHeatDelay(),
		sPRIndexScore(),
		iOutputMagazineSlotSequence(),
		nNextStepPusherIn(),
		nHeatDelayUnit(),
		nLFWaitPos(),
		bFlattenLF(),
		wFlattenDuration(1000){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CFoxconnTrack command target

enum EN_FOXCONN_SPEED
{
	FC_SPEED_SRCH = 0, 
	FC_SPEED_SLOW_FORWARD,          
	FC_SPEED_FORWARD,
	FC_SPEED_INJECT,
	FC_SPEED_BACKWARD,
	FC_SPEED_INDEX,
	FC_SPEED_EJECT,
	FC_SPEED_SLOW_SRCH,
};

enum EN_FOXCONN_EVENT
{
	FC_EVENT_IDLE,
	FC_EVENT_INDEX,
};

enum EN_FOXCONN_STATE
{
    FC_STATE_IDLE,
	FC_STATE_INDEX,
	FC_STATE_WAIT_INDEX,
};

class CFoxconnTrack : public CObject
{
public:
	CFoxconnTrack();
	virtual ~CFoxconnTrack();
public:	//functions
	void MoveToIndexPos(short nIndex);	//v3.0.T196 add
	void WaitMoveIndex(void);			//v3.0.T196 add
	void MoveIndexRight(long lDist);
	void MoveIndexLeft(long lDist);
	double GetIndexResol();
	long GetIndexPos();
	byte GetStepperID() {return m_nStepperID;};
	void Init(void);
	bool IsVacuumControl(void);
	void HomeIndexer(void);
	void MoveToFirstIndexPos(void);	//v3.0.T194 add
	void WaitHomeIndexer(void);
	void SaveRcd(void);
	void LoadRcd(void);
	FC_TRACK_DATA *GetRcd(void);
	void SetRcd(FC_TRACK_DATA* stTrackData);
	short GetCurBondUnit(void);
	void SetCurBondUnit(short nUnit);
	void ProcessIndexEvent(void);
	void SubmitIndexEvent();
	short GetEvent(void);
	void ResetRelayErrorState(void);
	void ResetEvent(void);
	void WaitMoveIndexer(void);
	void GetSpeedProfile(short nProfile, ST_STEPPER_SPEED *pstSpeedProfile);
	void SetSpeedProfile(short nProfile);
private:	//variable
	byte m_nStepperID;
	ST_AXIS_PARA m_stAxisParam;
	short m_nErrorCode;
	FC_TRACK_DATA m_stTrackRcd;
	short m_nCurBondUnit;
	short m_nEvent;
	char m_cState;
    CSensor m_VacuumSensor;
    CRelay m_VacuumValve;
};

extern CFoxconnTrack theFoxconnTrack;
