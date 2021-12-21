/***************************************************************
FoxconnIndex.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "FoxconnIndex.h"
#include "MacDef.h"
#include "MathCalc.h"
#include "StepperControl.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "Reference.h"
#include "MaterialHandlerThread.h"
#include "VisionSystem.h"
#include "TowerLight.h"
#include "MSGDEF.H"
#include "ScanKeyThread.h"
#include "MachineConfig.h"
#include "Bond.h"
#include "DataManager.h"
#include "PopUpTopStepperCtr.h"
#include "servo.h"
#include "Debug.h"

CFoxconnTrack theFoxconnTrack;

// CFoxconnTrack

CFoxconnTrack::CFoxconnTrack()
: m_nErrorCode(0)
, m_nCurBondUnit(1)
, m_nEvent(0)
, m_cState(0)
{
	//Indexer stepper
	m_stAxisParam.Pulses = 1600;    
	m_stAxisParam.ScrewPitch = 25000;
	m_stAxisParam.Scale = m_stAxisParam.ScrewPitch / m_stAxisParam.Pulses;
	m_stAxisParam.Acc = 8000000;
	m_stAxisParam.Dcc = 8000000;
	m_stAxisParam.Vel = 300000;
	m_stAxisParam.StopDcc = 80000000;
	m_stAxisParam.FastHomeVel = 80000;
	m_stAxisParam.SlowHomeVel = 60000;
	m_stAxisParam.SlowHomeDistance = 10000;
	m_stAxisParam.HomeOffset = 2000;
	m_stAxisParam.LimitSignal = 1;
	m_stAxisParam.HomeSignal = 0;
	m_stAxisParam.HomeDirection = 0;

	ZeroMemory(m_stTrackRcd.lIndexError, sizeof(m_stTrackRcd.lIndexError[TOP_MAX_INDEX_TIME]));

	m_stTrackRcd.lLimitTolerance = 5000;
	m_stTrackRcd.lHeatingDelay = 0;
	m_stTrackRcd.nIndexSpeedCoeff = 4;
	m_stTrackRcd.nEjectSpeedCoeff = 2;
	m_stTrackRcd.lLfStandbyLen = 0;
	m_stTrackRcd.lSrchBackLen = 15000;
	m_stTrackRcd.l1stSlipHeatDelay = 0;		//ms
	m_stTrackRcd.nPrIndexEnable = false;
	m_stTrackRcd.lPrIndexTolerance = 0;
	m_stTrackRcd.lIndexPosOffset = 0;
	m_stTrackRcd.lOpenClampDelay = 0;
	m_stTrackRcd.lIndexPrDelay = 0;
    m_stTrackRcd.bVacuumControl = true;
	m_stTrackRcd.l1stUnitHeatDelay = 0;
	m_stTrackRcd.nNextStepPusherIn = 2;
}

CFoxconnTrack::~CFoxconnTrack()
{
}

// CFoxconnTrack member functions

void CFoxconnTrack::Init(void)
{
	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		m_nStepperID = DEVICE_INDEXER_EX;
		theStepper.SetSpeedParameter(m_nStepperID, m_stAxisParam);
		Sleep_x1ms(10);

		m_VacuumValve.Initialize(theStepperIO.m_Output.m_Vacuum.nPort);
		m_VacuumSensor.Initialize(theStepperIO.m_Input.m_VacuumSensor.nPort);
	}
}

//v3.0.T194 add
void CFoxconnTrack::MoveToFirstIndexPos(void)
{
	MoveToIndexPos(1);
	WaitMoveIndex();
}

void CFoxconnTrack::HomeIndexer(void)
{
	theStepper.SearchHome(m_nStepperID);
}

void CFoxconnTrack::WaitHomeIndexer(void)
{
	CHTimer timer;
    do
	{
        Sleep_x1ms(10);
    } while ((timer.GetTime_x1ms() < STEP_MOTOR_MOTION_TIMEOUT)
				&& (theStepper.IsHome(m_nStepperID) == false));
}

void CFoxconnTrack::ResetRelayErrorState(void)
{
	m_VacuumValve.SetRelayState(RELAY_OFF);
}

void CFoxconnTrack::WaitMoveIndexer(void)
{
	long lMoveDist = theMcData.GetZeroOffset() <= 0 ? 50000 : theMcData.GetZeroOffset();
	long lPos; CHTimer timer;
	do
	{
		Sleep_x1ms(10);
	    theStepper.GetPos(m_nStepperID, &lPos);
	} while ((round(lMoveDist / theStepper.GetResol(m_nStepperID)) != lPos)
			&& (timer.GetTime_x1ms() < 5000));
}

void CFoxconnTrack::SaveRcd(void)
{
	theDataMgr.CopyToBuffer((char *)&m_stTrackRcd, sizeof(m_stTrackRcd));
}

void CFoxconnTrack::LoadRcd(void)
{
	theDataMgr.CopyFromBuffer((char *)&m_stTrackRcd, sizeof(m_stTrackRcd));
	m_stTrackRcd.nIndexSpeedCoeff = 0;	//v3.0.T194 add
}

void CFoxconnTrack::SetRcd(FC_TRACK_DATA* stTrackData)
{
	m_stTrackRcd = *stTrackData;
}

short CFoxconnTrack::GetCurBondUnit(void)
{
	return m_nCurBondUnit;
}

void CFoxconnTrack::SetCurBondUnit(short nUnit)
{
	m_nCurBondUnit = nUnit;
}

short CFoxconnTrack::GetEvent(void)
{
	return m_nEvent;
}

void CFoxconnTrack::ResetEvent(void)
{
	m_cState = FC_STATE_IDLE;
	m_nEvent = FC_EVENT_IDLE;
}

void CFoxconnTrack::ProcessIndexEvent(void)
{
	switch (m_cState)
	{
	case FC_STATE_IDLE:
	{
		m_nEvent = FC_EVENT_IDLE;
		break;
	}
	case FC_STATE_INDEX:
	{
		m_nCurBondUnit++;
		if (m_nCurBondUnit > theMcConfig.GetTotalIndex())
			m_nCurBondUnit = 1;
		MoveToIndexPos(m_nCurBondUnit);
		m_cState = FC_STATE_WAIT_INDEX;
		break;
	}
	case FC_STATE_WAIT_INDEX:
	{
		if (theStepper.IsBusy(m_nStepperID)) break;
		theMHS.SetRequestIndex(false);
		m_cState = FC_STATE_IDLE;
		break;
	}
	default:
		break;
	}
}

void CFoxconnTrack::SetSpeedProfile(short nProfile)
{
	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		ST_STEPPER_SPEED stSpeedProfile;
		GetSpeedProfile(nProfile, &stSpeedProfile);
		theStepperIO.SetMotorSpeedProfile(m_nStepperID, stSpeedProfile);
	}
}

void CFoxconnTrack::GetSpeedProfile(short nProfile, ST_STEPPER_SPEED *pstSpeedProfile)
{
	switch(nProfile)
	{
	case FC_SPEED_SRCH:
	{
		pstSpeedProfile->Acc = 6000000;
		pstSpeedProfile->Dcc = 6000000;
		pstSpeedProfile->Vel = 150000;
		pstSpeedProfile->StopDcc = 20000000;
		break;
	}
	case FC_SPEED_SLOW_FORWARD:
	{
		pstSpeedProfile->Acc = 2000000 + 1500000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Dcc = 2000000 + 1500000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Vel = 30000 + 60000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->StopDcc = 80000000;
		break;
	}
	case FC_SPEED_FORWARD:
	{
		pstSpeedProfile->Acc = 2000000 + 1700000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Dcc = 2000000 + 1700000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Vel = 30000 + 67000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->StopDcc = 80000000;
		break;
	}
	case FC_SPEED_INJECT:
	{
		pstSpeedProfile->Acc = 2000000 + 1500000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Dcc = 2000000 + 1500000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Vel = 30000 + 60000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->StopDcc = 80000000;
		break;
	}
	case FC_SPEED_BACKWARD:
	{
		pstSpeedProfile->Acc = 2000000 + 1700000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Dcc = 2000000 + 1700000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Vel = 30000 + 67000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->StopDcc = 80000000;
		break;
	}
	case FC_SPEED_INDEX:
	{
		pstSpeedProfile->Acc = 2000000 + 2000000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Dcc = 2000000 + 2000000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->Vel = 30000 + 85000 * m_stTrackRcd.nIndexSpeedCoeff;
		pstSpeedProfile->StopDcc = 80000000;
		break;
	}
	case FC_SPEED_EJECT:
	{
		pstSpeedProfile->Acc = 2000000 + 1200000 * m_stTrackRcd.nEjectSpeedCoeff;
		pstSpeedProfile->Dcc = 2000000 + 1200000 * m_stTrackRcd.nEjectSpeedCoeff;
		pstSpeedProfile->Vel = 30000 + 40000 * m_stTrackRcd.nEjectSpeedCoeff;
		pstSpeedProfile->StopDcc = 80000000;
		break;
	}
	case FC_SPEED_SLOW_SRCH:
	{
		int nSpeed = abs(theMcConfig.GetAlignSensorSlowSearchSpeed());
		pstSpeedProfile->Acc = 60000 * nSpeed;
		pstSpeedProfile->Dcc = 60000 * nSpeed;
		pstSpeedProfile->Vel = 1500 * nSpeed;
		pstSpeedProfile->StopDcc = 200000 * nSpeed;
		break;
	}
	default:
		break;
	}
}

bool CFoxconnTrack::IsVacuumControl()
{
	return (m_stTrackRcd.bVacuumControl);
}

FC_TRACK_DATA *CFoxconnTrack::GetRcd(void)
{
	return &m_stTrackRcd;
}

double CFoxconnTrack::GetIndexResol(void)
{
	return (theStepper.GetResol(m_nStepperID) / 5.0);
}

long CFoxconnTrack::GetIndexPos(void)
{
	long lStep;
	theStepper.GetPos(m_nStepperID, &lStep);
	return round(lStep * theStepper.GetResol(m_nStepperID) / 5.0);
}

void CFoxconnTrack::MoveIndexRight(long lDist)
{
	SetSpeedProfile(FC_SPEED_SLOW_FORWARD);
	theStepper.MoveStep(m_nStepperID, round(lDist / theStepper.GetResol(m_nStepperID) * 5.0), MOTION_RELATIVE);
	while (theStepper.IsBusy(m_nStepperID)) {Sleep_x1ms(10);};
}

void CFoxconnTrack::MoveIndexLeft(long lDist)
{
	SetSpeedProfile(FC_SPEED_SLOW_FORWARD);
	theStepper.MoveStep(m_nStepperID, round(-lDist / theStepper.GetResol(m_nStepperID) * 5.0), MOTION_RELATIVE);
	while (theStepper.IsBusy(m_nStepperID)) {Sleep_x1ms(10);};
}

void CFoxconnTrack::SubmitIndexEvent()
{
	m_nEvent = FC_EVENT_INDEX;
	m_cState = FC_STATE_INDEX;
}

//v3.0.T195 add
void CFoxconnTrack::MoveToIndexPos(short nIndex)
{
	if (nIndex < 1) return;
	SetSpeedProfile(FC_SPEED_SLOW_FORWARD);
	//v3.0.T300 delete
	//long lPos = theMcConfig.GetFirstIndexPos() + (theMcConfig.GetIndexPitch() * (nIndex - 1));
	//theStepper.MoveStep(m_nStepperID, round(lPos / theStepper.GetResol(m_nStepperID) * 5.0));
	//v3.0.T300 add
	if (nIndex == 1)
		theStepper.MoveStep(m_nStepperID, round(theMcConfig.GetFirstIndexPos()
						/ theStepper.GetResol(m_nStepperID) * 5.0));
	else
		theStepper.MoveStep(m_nStepperID, round(theMcConfig.GetIndexPitch()
						/ theStepper.GetResol(m_nStepperID) * 5.0), MOTION_RELATIVE);
}

//v3.0.T195 add
void CFoxconnTrack::WaitMoveIndex()
{
	CHTimer timer;
    do
	{
        Sleep_x1ms(10);
    } while (theStepper.IsBusy(m_nStepperID)
				&& (timer.GetTime_x1ms() < STEP_MOTOR_MOTION_TIMEOUT));
}
