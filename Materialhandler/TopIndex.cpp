/***************************************************************
TopTrack.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "TopIndex.h"
#include "TopClamp.h"
#include "TopOffload.h"
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
#include "RollingClaw.h"
#include "DigitalIO.h"
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

CTopTrack theTopTrack;

// CTopTrack
short nInc;

CTopTrack::CTopTrack()
: m_nErrorCode(0)
, m_nCurBondUnit(0)
, m_nEvent(0)
, m_cState(0)
, m_nLastDebugState(0)
, m_bValid(true)
, m_nTestEvent(0)
, m_nTestState(0)
, m_nGripperDryCycleState(0)
, m_lClawInterval(0)
, m_bOffloadEnable(false)
, m_bRunNoLf(false)
, m_bPrIndexMask(false)
, m_nIndexPrManualAlign(FALSE)
, m_lOffsetx1um(0)
, m_bRequestOnload(false)
, m_lInjectStartTime(0)
, m_nTrackStopReason(0)
, m_bTrackNeedClear(false)
, m_nNextLfState(NEXT_LF_NONE)
{
	// Indexer stepper
	//**********************************for STEP_TYPE_STEPPERIO
	m_stIndexerAxisParam.Pulses = 1600;    
	m_stIndexerAxisParam.ScrewPitch = 25000;
	m_stIndexerAxisParam.Scale = m_stIndexerAxisParam.ScrewPitch / m_stIndexerAxisParam.Pulses;
	m_stIndexerAxisParam.Acc = 8000000;
	m_stIndexerAxisParam.Dcc = 8000000;
	m_stIndexerAxisParam.Vel = 300000;
	m_stIndexerAxisParam.StopDcc = 80000000;
	m_stIndexerAxisParam.FastHomeVel = 80000;
	m_stIndexerAxisParam.SlowHomeVel = 60000;
	m_stIndexerAxisParam.SlowHomeDistance = 10000;
	m_stIndexerAxisParam.HomeOffset = 2000;
	m_stIndexerAxisParam.LimitSignal = 1;
	m_stIndexerAxisParam.HomeSignal = 0;
	m_stIndexerAxisParam.HomeDirection = 0;
	//**********************************for STEP_TYPE_STEPPERIO

	m_stIndexerSpeedProfile.bTCurveAcc         = 0x06;
	m_stIndexerSpeedProfile.lStartVelocity     = 1000;
	m_stIndexerSpeedProfile.lDriveVelocity     = 6000;   
	m_stIndexerSpeedProfile.lMaxDriveVelocity  = 8000;
	m_stIndexerSpeedProfile.lAcc               = 350000;
	m_stIndexerSpeedProfile.lAccRatio          = 1000000;

	m_stIndexerSpeedProfile.ulHomeType            = 6;
	m_stIndexerSpeedProfile.ulHomeCoarseSrchDir   = 1;
	m_stIndexerSpeedProfile.ulHomeCoarseSrchSpeed = 4000;
	m_stIndexerSpeedProfile.ulHomeFineSrchDir     = 0;
	m_stIndexerSpeedProfile.ulHomeFineSrchSpeed   = 0;
	m_stIndexerSpeedProfile.lHomeOffset = 0;

	//in ms
	m_stDelayState.lDelayDuration = 0;
	m_stDelayState.lStartTime = GetCurrentTime();
	m_stDelayState.nNextState = INDEX_STATE_IDLE;
	m_stDelayState.bMoveMode = 1;
	m_stDelayState.lMoveDist = 0;

	ZeroMemory(m_stTopTrackRcd.lIndexError, sizeof(m_stTopTrackRcd.lIndexError[TOP_MAX_INDEX_TIME]));

	m_stTopTrackRcd.lLimitTolerance = 5000;
	m_stTopTrackRcd.lHeatingDelay = 0;
	m_stTopTrackRcd.nIndexSpeedCoeff = 4;
	m_stTopTrackRcd.nEjectSpeedCoeff = 2;
	m_stTopTrackRcd.lLfStandbyLen = 0;
	m_stTopTrackRcd.lSrchBackLen = 15000;
	m_stTopTrackRcd.l1stSlipHeatDelay = 0;		//ms
	m_stTopTrackRcd.nPrIndexEnable = false;
	m_stTopTrackRcd.lPrIndexTolerance = 0;
	m_stTopTrackRcd.lIndexPosOffset = 0;
	m_stTopTrackRcd.lOpenClampDelay = 0;
	m_stTopTrackRcd.lIndexPrDelay = 0;
    m_stTopTrackRcd.bVacuumControl = true;
	m_stTopTrackRcd.l1stUnitHeatDelay = 0;

	l1stIndexLen = m_stIndexerVar.l1stIndexLen;

	m_stTopTrackRcd.nNextStepPusherIn = 2;
	m_bTeaching = false;

	m_bClampSensorEnabled=FALSE;	//V1.5.W87 edit

	m_nPRIState=0;		//V1.5.W123 add
}

CTopTrack::~CTopTrack()
{
}

// CTopTrack member functions

void CTopTrack::Init(void)
{
	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		m_nIndexStepID = DEVICE_INDEXER_EX;
		theStepper.SetSpeedParameter(m_nIndexStepID, m_stIndexerAxisParam);
		Sleep_x1ms(10);

		m_LFHolder.Initialize(theStepperIO.m_Output.m_LFHoler.nPort);
		m_InputClaw.Initialize(theStepperIO.m_Output.m_InputClaw.nPort);
		m_OutputClaw.Initialize(theStepperIO.m_Output.m_OutputClaw.nPort);
		m_Ejector.Initialize(theStepperIO.m_Output.m_EjectClaw.nPort);
		m_Injector.Initialize(theStepperIO.m_Output.m_InjectClaw.nPort);
		m_AlignSensor.Initialize(theStepperIO.m_Input.m_AlignSensor.nPort);
		m_EjectJamSensor.Initialize(theStepperIO.m_Input.m_EjectJamSensor.nPort);
		m_InjectSensor.Initialize(theStepperIO.m_Input.m_InjectSensor.nPort);
		m_EjectSensor.Initialize(theStepperIO.m_Input.m_EjectSensor.nPort);
		m_TrackFrontHome.Initialize(theStepperIO.m_Input.m_TrackFrontHome.nPort);	//V1.4.W37 add
		m_TrackBackHome.Initialize(theStepperIO.m_Input.m_TrackBackHome.nPort);	//V1.4.W37 add
	}
	else
	{
		m_nIndexStepID = TOP_DEVICE_INDEXER;
		theStepper.SetSpeedParameter(m_nIndexStepID, &m_stIndexerSpeedProfile);
		Sleep_x1ms(10);

		m_InjectSensor.Initialize(TOP_TRACK_INJECT_SENSOR);
		m_EjectSensor.Initialize(TOP_TRACK_EJECT_SENSOR);
		m_AlignSensor.Initialize(TOP_TRACK_ALIGHN_SENSOR);
		m_EjectJamSensor.Initialize(TOP_TRACK_EJECT_JAM_SENSOR);
		Sleep_x1ms(10);

		m_InputClaw.Initialize(TOP_RELAY_LEFT_GRIPPER);
		m_OutputClaw.Initialize(TOP_RELAY_RIGHT_GRIPPER);
		m_Ejector.Initialize(TOP_REALY_EJECTOR);
		m_Injector.Initialize(TOP_REALY_INJECTOR);
		m_LFHolder.Initialize(OUTPUT_TRACK_LF_HOLDER);
	}

	m_bClampSensorEnabled=theMcConfig.IsWcCloseSensorExist()||theMcConfig.IsWcOpenSensorExist();	//V1.5.W17
}

void CTopTrack::HomeIndexer(void)
{
	theStepper.DisableScurveProfile(m_nIndexStepID, &m_stIndexerSpeedProfile);
	Sleep_x1ms(10);
	theStepper.SearchHome(m_nIndexStepID);
}

void CTopTrack::WaitHomeIndexer(void)
{
	CHTimer timer;
    do
	{
        Sleep_x1ms(10);
    } while ((timer.GetTime_x1ms() < STEP_MOTOR_MOTION_TIMEOUT)
				&& (theStepper.IsHome(m_nIndexStepID) == false));
}

int CTopTrack::GetErrorCode(short nType)
{
	return (nType == 0 ? m_nErrorCode : m_nTrackStopReason);
}

void CTopTrack::ResetErrorCode(short nType)
{
	if (nType == 0)
		m_nErrorCode = 0;
	else
		m_nTrackStopReason = 0;
}

bool CTopTrack::IsValid(void)
{
	return m_bValid;
}

void CTopTrack::SetRunNoLf(bool bRunNoLf)
{
	m_bRunNoLf = bRunNoLf;
}

bool CTopTrack::IsRunNoLf(void)
{
	return m_bRunNoLf;
}

short CTopTrack::GetIndexPrManualAlign(void)
{
	return m_nIndexPrManualAlign;
}

void CTopTrack::SetIndexPrManualAlign(short nAlign)
{
	m_nIndexPrManualAlign = nAlign;
}

void CTopTrack::ResetRelayErrorState(void)
{
	m_Injector.SetRelayState(RELAY_OFF);
	m_InputClaw.SetRelayState(RELAY_OFF);
	m_OutputClaw.SetRelayState(RELAY_OFF);
	m_Ejector.SetRelayState(RELAY_OFF);
	m_LFHolder.SetRelayState(RELAY_OFF);
}

void CTopTrack::MoveIndexerToLogicZero(void)
{
	ChangeStepperMotionProfile(SLOW_FORWARD_PROFILE);

	long lMoveDist = theMcData.GetZeroOffset() <= 0 ? 50000 : theMcData.GetZeroOffset();
	theStepper.MoveDist(m_nIndexStepID, lMoveDist);
}

void CTopTrack::WaitMoveIndexer(void)
{
	long lPos;
	long lMoveDist = theMcData.GetZeroOffset() <= 0 ? 50000 : theMcData.GetZeroOffset();

	CHTimer timer;
	do
	{
		lPos = theStepper.GetPos(m_nIndexStepID);	//v3.0.T376 edit
		if (round((double)lMoveDist / theStepper.GetResol(m_nIndexStepID)) == lPos)
			break;

		Sleep_x1ms(10);
	} while (timer.GetTime_x1ms() < 5000);
}

bool CTopTrack::IsPrIndexMask(void)
{
	return m_bPrIndexMask;
}

void CTopTrack::SetPrIndexMask(bool bMask)
{
	m_bPrIndexMask = bMask;
}

void CTopTrack::SetValid(bool bValid)
{
	m_bValid = bValid;
}

void CTopTrack::SaveRcd(void)	//v3.0.T32 rename
{
	theDataMgr.CopyToBuffer((char *)&m_stTopTrackRcd, sizeof(m_stTopTrackRcd));
}

void CTopTrack::LoadRcd(void)	//v3.0.T32 rename
{
	theDataMgr.CopyFromBuffer((char *)&m_stTopTrackRcd, sizeof(m_stTopTrackRcd));
}

void CTopTrack::GetRcd(TOP_TRACK_DATA* stTrackData)
{
	*stTrackData = m_stTopTrackRcd ;
}

void CTopTrack::SetRcd(TOP_TRACK_DATA* stTrackData)
{
	m_stTopTrackRcd = *stTrackData;
}

short CTopTrack::GetCurBondUnit(void)
{
	return m_nCurBondUnit;
}

void CTopTrack::SetCurBondUnit(short nUnit)
{
	m_nCurBondUnit = nUnit;
}

short CTopTrack::GetEvent(void)
{
	return m_nEvent;
}

void CTopTrack::SetOffloadEnable(bool bEnable)
{
	m_bOffloadEnable = bEnable;
}

bool CTopTrack::IsOffloadEnable(void)
{
	return m_bOffloadEnable;
}

//v3.0.T374 edit
long CTopTrack::LogicToPhyPos(long lLogicPos)
{
	return (lLogicPos + theMcData.GetZeroOffset());
}

//always call this function when one continuous process finished
void CTopTrack::LeftMoveArray(void)
{
	*(INT64*)m_cStateSequence /= 256;
}

// use this function only when reset and clear track in condition 
void CTopTrack::ResetSlipRcd()
{
	m_stSlipOnTrack.bSlipSrchDone       = false;
	m_stSlipOnTrack.lSlipHeaderPos      = -CENTER_TO_LEFT_TRACK_EDGE;
	m_stSlipOnTrack.lSlipTailPos        = -CENTER_TO_LEFT_TRACK_EDGE;
	m_stSlipOnTrack.lSlipOutputTailPos  = CENTER_TO_LEFT_TRACK_EDGE;
	m_stSlipOnTrack.bCompenDone         = false;

	SetCurBondUnit(0);

	m_cState = INDEX_STATE_IDLE;
	m_nEvent = INDEX_EVENT_IDLE;

	m_nNextLfState = NEXT_LF_NONE;

	m_stDelayState.lDelayDuration = 0;
	m_stDelayState.lStartTime     = GetCurrentTime();
	m_stDelayState.nNextState     = INDEX_STATE_IDLE;
	m_stDelayState.bMoveMode      = 1;
	m_stDelayState.lMoveDist      = 0;
}

void CTopTrack::ResetEvent(void)
{
	m_cState = INDEX_STATE_IDLE;
	m_nEvent = INDEX_EVENT_IDLE;
}

//v3.0.T86 edit
void CTopTrack::RefreshMeasurementData(const TOP_DEVICE_RECORD *pstDevice)
{
	if (theMcConfig.IsBypassIndex()) return;
	if (pstDevice->nWndUnitNum == 0)
	{
		AfxMessageBox(_T("temp.40, nWndUnitNum == 0"));
		return;
	}
	//device related:
	if (pstDevice->nDeviceColmNum % pstDevice->nWndUnitNum != 0)
		m_stIndexerVar.nTotalUnits = pstDevice->nDeviceColmNum
									/ pstDevice->nWndUnitNum + 1;
	else
		m_stIndexerVar.nTotalUnits = pstDevice->nDeviceColmNum
									/ pstDevice->nWndUnitNum;

	m_stIndexerVar.lLfTotalLen = pstDevice->lDeviceTotalLen;
	m_stIndexerVar.lNorIndexLen = pstDevice->lDeviceColmPitch
									* pstDevice->nWndUnitNum;

	//Index profile calculate:

	switch (pstDevice->nDeviceId)
	{
	case ID_DIC_0603:
	{
		m_stIndexerVar.nInjectUnit  = 1;
		m_stIndexerVar.nSrchUnit    = m_stIndexerVar.nTotalUnits - 6;	//12
		m_stIndexerVar.nCatchUpUnit = m_stIndexerVar.nTotalUnits - 5;	//13
		m_stIndexerVar.nCompEnable  = TRUE;
		break;
	}
	case ID_PCB_LONG:
	{
		m_stIndexerVar.nInjectUnit  = m_stIndexerVar.nTotalUnits - 2;
		m_stIndexerVar.nSrchUnit    = m_stIndexerVar.nTotalUnits - 1;    
		m_stIndexerVar.nCatchUpUnit = 0;
		m_stIndexerVar.nCompEnable  = TRUE;
		break;
	}
	case ID_10W_HIGHPWR:
	{
		m_stIndexerVar.nCompEnable  = FALSE;
		m_stIndexerVar.nInjectUnit  = 1;
		m_stIndexerVar.nSrchUnit    = 2;
		m_stIndexerVar.nCatchUpUnit = 0;
		break;
	}
	case ID_100W_HIGHPWR:
	{
		m_stIndexerVar.nCompEnable  = FALSE;
		m_stIndexerVar.nInjectUnit  = 0;
		m_stIndexerVar.nSrchUnit    = 0;
		m_stIndexerVar.nCatchUpUnit = 0;
		break;
	}
	case ID_SOP16:
	{
		m_stIndexerVar.nInjectUnit  = m_stIndexerVar.nTotalUnits - 2;
		m_stIndexerVar.nSrchUnit    = m_stIndexerVar.nTotalUnits - 1;    
		m_stIndexerVar.nCatchUpUnit = 0;
		m_stIndexerVar.nCompEnable  = TRUE;
		break;
	}
	case ID_SOP7_1C:
	{
		m_stIndexerVar.nInjectUnit  = m_stIndexerVar.nTotalUnits - 5;
		m_stIndexerVar.nSrchUnit    = m_stIndexerVar.nTotalUnits - 4;
		m_stIndexerVar.nCatchUpUnit = m_stIndexerVar.nTotalUnits - 3;
		m_stIndexerVar.nCompEnable  = TRUE;
		break;
	}
	case ID_SOP7_2C:
	{
		m_stIndexerVar.nInjectUnit  = m_stIndexerVar.nTotalUnits - 3;
		m_stIndexerVar.nSrchUnit    = m_stIndexerVar.nTotalUnits - 2;
		m_stIndexerVar.nCatchUpUnit = 0;
		m_stIndexerVar.nCompEnable  = FALSE;
		break;
	}
	case ID_CEDAR:
	{
		m_stIndexerVar.nInjectUnit  = 1;
		m_stIndexerVar.nSrchUnit    = 2;
		m_stIndexerVar.nCatchUpUnit = 0;
		m_stIndexerVar.nCompEnable  = FALSE;
		break;
	}
	default:
	{
		if (m_stIndexerVar.nTotalUnits <= 3)
		{
			m_stIndexerVar.nCompEnable  = TRUE;
			m_stIndexerVar.nInjectUnit  = 1;
			m_stIndexerVar.nSrchUnit    = 2;
			m_stIndexerVar.nCatchUpUnit = 0;
		}
		else
		{
			m_stIndexerVar.nInjectUnit = 1;
			for (int i=1; i<m_stIndexerVar.nTotalUnits-2; i++)
			{
				if (m_stIndexerVar.lNorIndexLen * (m_stIndexerVar.nTotalUnits + 0.5 - i) < LOCATION_SENSOR_CALI_DIST)
				{
					if (i < m_stIndexerVar.nTotalUnits - 2)
					{
						m_stIndexerVar.nInjectUnit = i;
						break;
					}
				}
				else
					m_stIndexerVar.nInjectUnit = 1;
			}

			m_stIndexerVar.nSrchUnit    = m_stIndexerVar.nTotalUnits-2;   //2nd last 1
			m_stIndexerVar.nCatchUpUnit = m_stIndexerVar.nTotalUnits-1;   //1st last 1
			m_stIndexerVar.nCompEnable  = TRUE;
		}
		break;
	}
	}

	//left/right limit position based on the calibration
	m_stIndexerVar.lRightLimitPos = theMcData.GetIndexerLimit() - theMcData.GetZeroOffset() - m_stTopTrackRcd.lLimitTolerance;
    m_stIndexerVar.lLeftLimitPos  = -theMcData.GetZeroOffset();

//Length-------------------------------------------------------------------------------
	//(tailedge + ClampPos) or (headerEdge - ClampPos)
	
	//the center of the slip unit is aligning with the heat block
	//m_stIndexerVar.l1stIndexLen  =round((double) LOCATION_SENSOR_CALI_DIST -m_stTopTrackRcd.lLfStandbyLen + (pstTopDevicePara->lDeviceEdgeLen + pstTopDevicePara->lDeviceColmPitch *(pstTopDevicePara->nWndUnitNum - 0.5))/2);

	if (m_stIndexerVar.nCatchUpUnit == 0)
		m_stIndexerVar.l1stIndexLen = m_stTopTrackRcd.lLfStandbyLen + (pstDevice->lDeviceColmPitch * pstDevice->nWndUnitNum) + pstDevice->lDeviceEdgeLen * 2;
	else
		m_stIndexerVar.l1stIndexLen = m_stTopTrackRcd.lLfStandbyLen + (pstDevice->lDeviceColmPitch * pstDevice->nWndUnitNum) * (m_stIndexerVar.nTotalUnits - m_stIndexerVar.nCatchUpUnit + 1) + pstDevice->lDeviceEdgeLen * 2;

	/*	
	if(pstTopDevicePara->nDeviceId == ID_DIC_0603)
		m_stIndexerVar.lCatchUpLen   =round((double) 4 * m_stIndexerVar.lNorIndexLen);
	else*/

		m_stIndexerVar.lCatchUpLen =  m_stIndexerVar.lNorIndexLen + m_stTopTrackRcd.lIndexError[m_nCurBondUnit];

//Action Limit
	
	//m_stIndexerVar.lLeftClawIndexLimitPos = round((double)m_stIndexerVar.lRightLimitPos - m_stIndexerVar.lNorIndexLen -CENTRE_TO_LEFT_CLAW- m_stTopTrackRcd.lIndexPosOffset);

	//m_stIndexerVar.lRightClawIndexLimitPos = m_stIndexerVar.lLeftLimitPos + 5000 + m_stTopTrackRcd.lIndexPosOffset;
	
	m_stIndexerVar.lEjectStartPos = m_stIndexerVar.lLeftLimitPos;

	//m_stIndexerVar.lClawOutputPos = m_stIndexerVar.lLeftLimitPos + DIAMETER_OF_GRIPPER; 

	m_stIndexerVar.lSrchStartPos  = m_stIndexerVar.lLeftLimitPos;
	
// Injection Profile
	m_stIndexerVar.lInjectStart[0]  = m_stIndexerVar.lLeftLimitPos + 5000 ;
	m_stIndexerVar.lInjectStart[1]  = m_stIndexerVar.lLeftLimitPos;
	m_stIndexerVar.lInjectStart[2]  = m_stIndexerVar.lLeftLimitPos;
	m_stIndexerVar.lInjectEnd[0] = m_stIndexerVar.lInjectStart[0] + 22000;
	m_stIndexerVar.lInjectEnd[1] = m_stIndexerVar.lInjectStart[1] + 30000;
	m_stIndexerVar.lInjectEnd[2] = m_stIndexerVar.lInjectStart[2] + 30000;
}

void CTopTrack::SubmitClearTrackEvent(void)
{
	memset(m_cStateSequence, 0, sizeof(m_cStateSequence));
	char cClear[TOTAL_STEPS] = {TOP_CLEAR_TRACK, TOP_CLEAR_TRACK_START_1ST, CLAW_POSITION_RESET, TOP_CLEAR_TRACK_START_2ND, CLAW_POSITION_RESET, INDEX_STATE_IDLE};

	if (m_nCurBondUnit == 0)
	{
		if(m_AlignSensor.GetCurrentState() == TRUE)
		{
			m_stSlipOnTrack.lSlipHeaderPos = -LOCATION_SENSOR_CALI_DIST;
		}

		m_stSlipOnTrack.lSlipTailPos = -LOCATION_SENSOR_CALI_DIST;
		memcpy(m_cStateSequence, cClear, TOTAL_STEPS);
		m_nEvent = INDEX_EVENT_CLEAR_TRACK_FIRST;
		m_cState = m_cStateSequence[0];
	}
	else
	{
		memcpy(m_cStateSequence, cClear, TOTAL_STEPS);
		m_nEvent = INDEX_EVENT_CLEAR_TRACK_NORMAL;
		m_cState = m_cStateSequence[0];
	}

	LeftMoveArray();
}

void CTopTrack::SubmitIndexEvent(short nEvent)
{
	memset(m_cStateSequence, INDEX_STATE_IDLE, sizeof(m_cStateSequence));
	m_nEvent = INDEX_EVENT_IDLE;

	char cIndexNor[TOTAL_STEPS]  = {CLAW_STAND_BY, DEVICE_INDEX, CLAW_STAND_BY, INDEX_STATE_IDLE, INDEX_STATE_IDLE, INDEX_STATE_IDLE};
	char cInitIndex[TOTAL_STEPS] = {DEVICE_INJECT,DEVICE_ROUGH_SRCH,CLAW_STAND_BY, DEVICE_INDEX ,CLAW_STAND_BY ,INDEX_STATE_IDLE };
	char cNewSlip[TOTAL_STEPS]   = {CLAW_STAND_BY, DEVICE_INDEX, DEVICE_OUTPUT, CLAW_STAND_BY, INDEX_STATE_IDLE,INDEX_STATE_IDLE};

    // Inject, compens(multiple), srch, standby (maximum 4 steps before index)
	char cIndexInject[TOTAL_STEPS] = {CLAW_STAND_BY, DEVICE_INDEX, DEVICE_INJECT, CLAW_STAND_BY, INDEX_STATE_IDLE, INDEX_STATE_IDLE};
	char cIndexInjectWithComp[TOTAL_STEPS] = {CLAW_STAND_BY, DEVICE_INDEX, DEVICE_INJECT, DEVICE_COMPEN, CLAW_STAND_BY, INDEX_STATE_IDLE};
    char cNorCompen[TOTAL_STEPS] = {CLAW_STAND_BY, DEVICE_INDEX, DEVICE_COMPEN, CLAW_STAND_BY, INDEX_STATE_IDLE, INDEX_STATE_IDLE};
	char cLfLocating[TOTAL_STEPS] = {CLAW_STAND_BY, DEVICE_INDEX, DEVICE_ROUGH_SRCH, CLAW_STAND_BY, INDEX_STATE_IDLE, INDEX_STATE_IDLE};
	char cCatchUp[TOTAL_STEPS] = {CLAW_STAND_BY, DEVICE_INDEX, CLAW_STAND_BY, DEVICE_CATCH_UP, CLAW_STAND_BY, INDEX_STATE_IDLE};

	char cNewSlipwithInject[TOTAL_STEPS] = {CLAW_STAND_BY, DEVICE_INDEX, DEVICE_OUTPUT, DEVICE_INJECT, DEVICE_ROUGH_SRCH,CLAW_STAND_BY};

	if (nEvent == INDEX_EVENT_STANDBY_NEXT_LF)
	{
		m_nEvent = INDEX_EVENT_STANDBY_NEXT_LF;

		short i = 0;
		if (m_nCurBondUnit > m_stIndexerVar.nInjectUnit && m_nNextLfState < NEXT_LF_AFTER_INJECT)
		{
			m_cStateSequence[i++] = DEVICE_INJECT;
            if (m_stIndexerVar.nTotalUnits <= 3)
            {
              	m_cStateSequence[i++] = DEVICE_COMPEN;
            }
		}

		if ((m_nCurBondUnit > m_stIndexerVar.nSrchUnit)
			&& (m_nNextLfState < NEXT_LF_AFTER_SRCH))
		{
			m_cStateSequence[i++] = DEVICE_ROUGH_SRCH;
		}

		if ((m_stIndexerVar.nCatchUpUnit != 0)
			&& (m_nCurBondUnit > m_stIndexerVar.nCatchUpUnit)
			&& (m_nNextLfState < NEXT_LF_AFTER_CATCH_UP))
		{
			m_cStateSequence[i++] = CLAW_STAND_BY;
			m_cStateSequence[i++] = DEVICE_CATCH_UP;
		}
		m_cStateSequence[i] = CLAW_STAND_BY;
	    m_cState = m_cStateSequence[0];
	}

	//maximum steps: Inject ->compens(multiple) -> srch ->catch up @before index
    //minimum steps: Inject(with comp) -> srch @before index

	else
	{
		if (m_nCurBondUnit == 0)  // 1st index event
		{		
			m_nNextLfState = NEXT_LF_NONE;
			m_nEvent = INITIAL_INDEX_EVENT;
			memcpy(m_cStateSequence, cInitIndex, TOTAL_STEPS);
			m_cState = m_cStateSequence[0];
		}
		else if (m_nCurBondUnit == m_stIndexerVar.nTotalUnits)
		{
			if (m_stIndexerVar.nTotalUnits > 3)
			{
				memcpy(m_cStateSequence, cNewSlip, TOTAL_STEPS);

                if (m_stIndexerVar.nCatchUpUnit != 0)
                {
				    if (m_nNextLfState < NEXT_LF_AFTER_CATCH_UP)
					    theMHS.SetLastLf(true);
                }
                else
                {
                    if (m_nNextLfState < NEXT_LF_AFTER_SRCH)
					    theMHS.SetLastLf(true);
                }
			}
			else  // =3 or 2 index times(no compen and catch up)
			{
				if (m_stIndexerVar.nTotalUnits <= 2)
				{
					memcpy(m_cStateSequence, cNewSlipwithInject, TOTAL_STEPS);
				}
				else 
				{
					memcpy(m_cStateSequence, cNewSlip, TOTAL_STEPS);
				}
				if (m_nNextLfState != NEXT_LF_AFTER_SRCH)
					theMHS.SetLastLf(true);
			}

			m_cState = m_cStateSequence[0];
			m_nEvent = INDEX_NEW_SLIP_EVENT;
		}
//************************************flexible profile*************************
		else if (m_nCurBondUnit == m_stIndexerVar.nInjectUnit)
		{
			m_nEvent = INDEX_UNIT_1ST_UNIT_EVENT;

			if ((theMHS.IsLastLf() == true)
				|| (nEvent == INDEX_EVENT_SINGLE_INDEX))
			{
				memcpy(m_cStateSequence, cIndexNor, TOTAL_STEPS);
			}
			else
			{
				memcpy(m_cStateSequence, cIndexInjectWithComp, TOTAL_STEPS);
			}

			m_cState = m_cStateSequence[0];
		}
		else if (m_nCurBondUnit == m_stIndexerVar.nSrchUnit)
		{
			m_nEvent = INDEX_UNIT_STANDBY_EVENT;

			if (m_nNextLfState == NEXT_LF_AFTER_INJECT)
				memcpy(m_cStateSequence, cLfLocating, TOTAL_STEPS);
			else
				memcpy(m_cStateSequence, cIndexNor, TOTAL_STEPS);

			m_cState = m_cStateSequence[0];
		}
		else if (m_nCurBondUnit == m_stIndexerVar.nCatchUpUnit)
		{
			m_nEvent = INDEX_UNIT_STANDBY_EVENT;

			if (m_nNextLfState == NEXT_LF_AFTER_SRCH)
				memcpy(m_cStateSequence, cCatchUp, TOTAL_STEPS);
			else
				memcpy(m_cStateSequence, cIndexNor, TOTAL_STEPS);

			m_cState = m_cStateSequence[0];
		}
		else
		{
			m_nEvent = INDEX_WITH_NOTHING_MORE;
			memcpy(m_cStateSequence, cIndexNor, TOTAL_STEPS);
			m_cState = m_cStateSequence[0];
		}
	}

	LeftMoveArray();
}

void CTopTrack::ProcessIndexEvent(void)
{
	//v3.0.T370 delete
	//if (theDebug.IsDebugLogTrace())
	//{
	//	if (m_cState != m_nLastDebugState)
	//		m_nLastDebugState = m_cState;
	//}
	//else if (theDebug.IsDebugWorkHolder())
	//{
	//	if (m_cState != m_nLastDebugState)
	//	{
	//		theDebug.AddInfo(_T("TrackState"), m_cState);
	//		m_nLastDebugState = m_cState;
	//	}
	//}

	switch(m_cState)
	{
	case INDEX_STATE_IDLE:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-Idle"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_nEvent = INDEX_EVENT_IDLE;
		break;
	}
//1~5  Start Injection of new device
    case DEVICE_INJECT:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-Inject"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_cState = DEVICE_WAIT_ELEVATOR;	
		break;
	}
	case DEVICE_WAIT_ELEVATOR:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitElev"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_lInjectStartTime = GetCurrentTime();
		m_cState = DEVICE_INJECT_CHECK_ONLOADER;
		break;
	}
	case DEVICE_INJECT_CHECK_ONLOADER:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectCheckOnload"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theTopTrack.m_bRequestOnload == false)
		{
			m_stIndexerVar.m_iInjectTimes = 0;
			m_cState = DEVICE_INJECT_START;
		}
		else
		{
			if (GetCurrentTime() - m_lInjectStartTime > ELEVATOR_TIME_OUT)  //22s
			{
                // Elevator onload retry
				m_stIndexerVar.m_iOnloadRetryTimes = 0;
				m_nErrorCode = MHS_TOP_ONLOADER_NO_RESPONSE;
				//theMHS.SetLastLf(true);
				m_cState = m_cStateSequence[0];    
    		    LeftMoveArray();
			}
		}
		break;
	}
	case DEVICE_INJECT_START:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (m_InjectSensor.GetCurrentState() == FALSE) //No Lead Frame or other error
		{
			if (m_nEvent == INITIAL_INDEX_EVENT)  
			{
				m_nErrorCode = MHS_TOP_ONLOAD_NO_LF;
				do 
				{
					LeftMoveArray();
					m_cState = m_cStateSequence[0];
				} while (m_cStateSequence[0] != INDEX_STATE_IDLE);
			}
			else
			{
				m_nTrackStopReason = MHS_TOP_ONLOAD_NO_LF;
				theMHS.SetLastLf(true);
				do 
				{
					LeftMoveArray();
					m_cState = m_cStateSequence[0];
				} while (m_cStateSequence[0] != CLAW_STAND_BY && m_cStateSequence[0] != INDEX_STATE_IDLE);
			}
		}
		else
        {
			ChangeStepperMotionProfile(INJECT_SPEED_PROFILE);
			m_stIndexerVar.m_lInjectStartPos = m_stIndexerVar.lLeftLimitPos ;
			theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(m_stIndexerVar.m_lInjectStartPos));	//v3.0.T374 edit
			m_cState = DEVICE_INJECT_WAIT_AT_START;
        }
		break;
	}
	case DEVICE_INJECT_WAIT_AT_START:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectWaitStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(false);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

        if (theStepper.IsBusy(m_nIndexStepID)) break;

		// clamp left claw
		m_Injector.SetRelayState(RELAY_ON);

		// Lead frame holder Open
		m_LFHolder.SetRelayState(RELAY_ON);

		ChangeStepperMotionProfile(SLOW_FORWARD_PROFILE);
		if (theMcConfig.GetOnloadType() == TOP_WITH_PUSH_KICKER)
		m_stIndexerVar.m_lInjectEndPos = m_stIndexerVar.m_lInjectStartPos + 50000;

		EnterDelayState(DEVICE_INJECT_WAIT_SENSOR, GetCurrentTime(), SMALL_RELAY_DELAY,
			round((double)LogicToPhyPos(m_stIndexerVar.m_lInjectEndPos) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit

		break;
	}
	case DEVICE_INJECT_WAIT_SENSOR:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectWaitSensor"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		m_Injector.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_cState = DEVICE_INJECT_WAIT_INJECT_DEST2;
		break;
	}
	case DEVICE_INJECT_WAIT_INJECT_DEST2:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectWaitDest2"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

        if (theStepper.IsBusy(m_nIndexStepID)) break;

		//OPEN INJECTOR, LF holder close
		m_Injector.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);

		if (m_nEvent != INITIAL_INDEX_EVENT)
			m_nNextLfState = NEXT_LF_AFTER_INJECT;

		EnterDelayState(m_cStateSequence[0], GetCurrentTime(), RELAY_DELAY, 0, 0); 
		LeftMoveArray();
        break;
	}
//6~8 --28  Compensate to let standby device catch up with bonding one
	case DEVICE_COMPEN: 
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevComp"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		ChangeStepperMotionProfile(BACKWARD_PROFILE);
		theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(m_stIndexerVar.lSrchStartPos));	//v3.0.T374 edit
		m_cState = DEVICE_COMPEN_WAIT_AT_START;
		break;
	}
	case DEVICE_COMPEN_WAIT_AT_START:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-CompWaitStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(false);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		ChangeStepperMotionProfile(SLOW_FORWARD_PROFILE);
		m_InputClaw.SetRelayState(RELAY_ON);
		m_LFHolder.SetRelayState(RELAY_ON);
		EnterDelayState(DEVICE_COMPEN_WAIT_DESTINATION, GetCurrentTime(), RELAY_ACTION_DELAY,
			round(m_stTopTrackRcd.nLFWaitPos*1000 / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
		break;
	}
	case DEVICE_COMPEN_WAIT_DESTINATION:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-CompWaitDest"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			m_InputClaw.SetRelayState(RELAY_OFF);
			m_LFHolder.SetRelayState(RELAY_OFF);
			m_stSlipOnTrack.bCompenDone = true;

		    EnterDelayState(m_cStateSequence[0], GetCurrentTime(), RELAY_DELAY, 0, 0);   
			LeftMoveArray();
		}
		else
		{
			if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
			{
				if (m_AlignSensor.GetCurrentState() == TRUE)
				{
					m_InputClaw.SetRelayState(RELAY_OFF);
					m_LFHolder.SetRelayState(RELAY_OFF);
					m_stSlipOnTrack.bCompenDone = true;

					EnterDelayState(m_cStateSequence[0], GetCurrentTime(), RELAY_DELAY, 0, 0);   

					LeftMoveArray();
				}
			}
		}
		break;
	}
//**********************************Catch Up after Align****************************************************
	case DEVICE_CATCH_UP:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-CatchUp"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(false);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		ChangeStepperMotionProfile(SLOW_FORWARD_PROFILE);
		m_InputClaw.SetRelayState(RELAY_ON);
		m_LFHolder.SetRelayState(RELAY_ON);

		//catch-up update
		m_stIndexerVar.lCatchUpLen = m_stIndexerVar.lNorIndexLen + m_stTopTrackRcd.lIndexError[m_nCurBondUnit];
		EnterDelayState(DEVICE_WAIT_STANBY_POS, GetCurrentTime(), RELAY_ACTION_DELAY,
			round((double)m_stIndexerVar.lCatchUpLen / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
		break;
	}
	case DEVICE_WAIT_STANBY_POS: 
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitStandbyPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		if (m_nEvent != INITIAL_INDEX_EVENT)
			m_nNextLfState = NEXT_LF_AFTER_CATCH_UP;

		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_stSlipOnTrack.lSlipHeaderPos = m_stSlipOnTrack.lSlipHeaderPos + m_stIndexerVar.lCatchUpLen;

		ChangeStepperMotionProfile(INDEX_SPEED_PROFILE);
		EnterDelayState(m_cStateSequence[0], GetCurrentTime(), RELAY_DELAY, 0, 0);   
		LeftMoveArray();
		break;
	}
//**********************************SEARCH LOCATION SENSOR***********************************************************
//9~12  Srch the location Sensor and ready for index: 
	case DEVICE_ROUGH_SRCH:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrch"), m_cState);
			m_nLastDebugState = m_cState;
		}

		ChangeStepperMotionProfile(BACKWARD_PROFILE);
		if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
			theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(m_stIndexerVar.lSrchStartPos));	//v3.0.T374 edit
		else
			MoveIndexerToLogicZero();

        m_cState = DEVICE_ROUGH_SRCH_WAIT_AT_START;
		break;
	}
	case DEVICE_ROUGH_SRCH_WAIT_AT_START:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchWaitStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(false);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		long lAbsPos;	//v3.0.T370 add
		if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
			lAbsPos = LogicToPhyPos(m_stIndexerVar.lSrchStartPos);	//v3.0.T374 edit
		else
			lAbsPos = LogicToPhyPos(0);
        lAbsPos = round((double)lAbsPos / theStepper.GetResol(m_nIndexStepID));
        if (theStepper.GetPos(m_nIndexStepID, FALSE) == lAbsPos)	//v3.0.T376 edit
        {
            //clamp left claw
            ChangeStepperMotionProfile(SRCH_SPEED_PROFILE);

            if (theTopTrack.IsRunNoLf() == false)
            {
				if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
				{
					m_InputClaw.SetRelayState(RELAY_ON);
					m_LFHolder.SetRelayState(RELAY_ON);
					theStepper.MoveStepSearch(m_nIndexStepID, 1);

					EnterDelayState(DEVICE_ROUGH_SRCH_WAIT_ROUGH_REACH, GetCurrentTime(), RELAY_ACTION_DELAY,
						round((theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO ? 90000 : 8000) / theStepperIO.GetResolution(m_nIndexStepID)), 0);	//v3.0.T372 add
				}
				else
				{
					theStepper.MoveStepSearch(m_nIndexStepID, 1);
					EnterDelayState(DEVICE_ROUGH_SRCH_WAIT_ROUGH_REACH, GetCurrentTime(), RELAY_ACTION_DELAY, 0);
				}
            }
            else
            {
                m_InputClaw.SetRelayState(RELAY_ON);
				m_LFHolder.SetRelayState(RELAY_ON);
                EnterDelayState(DEVICE_ROUGH_SRCH_WAIT_ROUGH_REACH, GetCurrentTime(), RELAY_ACTION_DELAY,
					round((double)m_stTopTrackRcd.lSrchBackLen / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
            }
        }
        break;
	}
	case DEVICE_ROUGH_SRCH_WAIT_ROUGH_REACH:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchWaitReach"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

        //stop, start srch back  RECHECK_SENSOR_BACK_DIST
        if (theTopTrack.IsRunNoLf() == false)
        {
            if (m_AlignSensor.GetCurrentState() == TRUE)
            {
                theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);

				int m_nSrchDist = abs(theMcConfig.GetAlignSensorSlowSearchDist_x1um());  //v1.6.P32 add, v1.6.P153sc edit

				if ((theMcConfig.GetAlignSensorSlowSearchSpeed() == 0)
					|| (theMcConfig.GetAlignSensorSlowSearchDist_x1um() == 0))	//V1.5.W116 edit, v1.6.P32 edit
					EnterDelayState(DEVICE_FINE_SRCH_WAIT_STANDBY, GetCurrentTime(), 100, 0, 0);
				else if (theMcConfig.GetAlignSensorSlowSearchSpeed() > 0)		//V1.5.W164 edit  
					EnterDelayState(DEVICE_INDEX_SLOW_SRCH, GetCurrentTime(), 100, m_nSrchDist, 0); //v1.6.P32 edit, v1.6.P153sc edit
				else	//V1.5.W164 add
				{
					ChangeStepperMotionProfile(SLOW_SRCH_PROFILE);
					EnterDelayState(DEVICE_INDEX_SLOW_SRCH_BACK, GetCurrentTime(), 100, -m_nSrchDist, 0);  //v1.6.P180 edit
				}
            }
            else
            {
                //error handling:
                if (theStepper.IsBusy(m_nIndexStepID) == false)
                {
                    m_InputClaw.SetRelayState(RELAY_OFF);
					m_LFHolder.SetRelayState(RELAY_OFF);
                    if (m_nEvent == INITIAL_INDEX_EVENT)  
                    {
                        m_nErrorCode = MHS_TOP_SRCH_TIME_OUT;
                        m_cState = DEVICE_INDEX_STOP;

                        LeftMoveArray(); 
                        LeftMoveArray(); 
                    }
                    else
                    {
                        if (theMHS.IsLastLf() == true)
                        {
                            m_nTrackStopReason = MHS_TOP_ONLOAD_NO_LF;
                        }
                        else
                        {
                            m_nTrackStopReason = MHS_TOP_SRCH_TIME_OUT;
                        }

                        theMHS.SetLastLf(true);
                        m_cState = m_cStateSequence[0];    
                        LeftMoveArray(); 
                    }
                }
            }
        }
        else
        {
            if (theStepper.IsBusy(m_nIndexStepID) == false)
            {
                EnterDelayState(DEVICE_ROUGH_SRCH_WAIT_BACK_REDO, GetCurrentTime(), RELAY_DELAY,
					-round((double)m_stTopTrackRcd.lSrchBackLen / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
            }
        }
        break;
	}
	case DEVICE_ROUGH_SRCH_WAIT_BACK_REDO:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchWaitRedo"), m_cState);
			m_nLastDebugState = m_cState;
		}

		//check pos, srch again
		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_stSlipOnTrack.lSlipHeaderPos = -LOCATION_SENSOR_CALI_DIST - m_stTopTrackRcd.lSrchBackLen;
		EnterDelayState(DEVICE_ROUGH_SRCH_BACK, GetCurrentTime(), RELAY_DELAY, 0, 0);
		break;
	}
// second time srch start here--------->>>>
	case DEVICE_ROUGH_SRCH_BACK:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchBack"), m_cState);
			m_nLastDebugState = m_cState;
		}

		ChangeStepperMotionProfile(BACKWARD_PROFILE);
		theStepper.MoveStep(m_nIndexStepID, LogicToPhyPos(m_stIndexerVar.lSrchStartPos));	//v3.0.T374 edit
		m_cState = DEVICE_ROUGH_WAIT_SRCH_BACK; 
		break;
	}
	case DEVICE_ROUGH_WAIT_SRCH_BACK:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughWaitSrchBack"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_InputClaw.SetRelayState(RELAY_ON);
		m_LFHolder.SetRelayState(RELAY_ON);
		ChangeStepperMotionProfile(SRCH_SPEED_PROFILE);

		long lPos;	//v3.0.T370 add
		if (theTopTrack.IsRunNoLf() == false)
			lPos = round((double)90000 / theStepper.GetResol(m_nIndexStepID));
		else
			lPos = round((double)m_stTopTrackRcd.lSrchBackLen / theStepper.GetResol(m_nIndexStepID));
		EnterDelayState(DEVICE_FINE_SRCH_WAIT_STANDBY, GetCurrentTime(), RELAY_DELAY, lPos, 0);
        break;
	}
//15~16 -----------for fine search--------------------------
	case DEVICE_FINE_SRCH_WAIT_STANDBY:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-FineSrchWaitStandby"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(false);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		ChangeStepperMotionProfile(SLOW_FORWARD_PROFILE); 
		m_stSlipOnTrack.lSlipHeaderPos = -LOCATION_SENSOR_CALI_DIST;

		long pos, searchpos;
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
		{
			theStepperIO.CheckStepMotionSearchDis(m_nIndexStepID, &searchpos);
			pos = theStepper.GetPos(m_nIndexStepID);	//v3.0.T376 edit
			theStepper.MoveStepSearch(m_nIndexStepID, 0);

			if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
			{
				if (pos > 0)
				{
					TOP_DEVICE_RECORD stDeviceRcd;
					theDevice.GetTopRcd(&stDeviceRcd);
					if (m_stIndexerVar.nCatchUpUnit == 0)
						m_lStandbyAfterSearch = round(LOCATION_SENSOR_CALI_DIST - (m_stTopTrackRcd.lLfStandbyLen + m_stIndexerVar.lNorIndexLen * 0.5 + stDeviceRcd.lDeviceEdgeLen * 2) + m_stTopTrackRcd.lIndexError[0]);
					else
						m_lStandbyAfterSearch = round(LOCATION_SENSOR_CALI_DIST - (m_stTopTrackRcd.lLfStandbyLen + m_stIndexerVar.lNorIndexLen * 1.5 + stDeviceRcd.lDeviceEdgeLen * 2) + m_stTopTrackRcd.lIndexError[0]);
					
					EnterDelayState(DEVICE_FINE_WAIT_FORWARD, GetCurrentTime(), 0,
						round((double)m_lStandbyAfterSearch / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
				}
				else
				{
					m_nTrackStopReason = (theMHS.IsLastLf() == true ? MHS_TOP_ONLOAD_NO_LF : MHS_TOP_SRCH_TIME_OUT);

					theMHS.SetLastLf(true);
					m_cState = m_cStateSequence[0];    
					LeftMoveArray(); 
				}
			}
			else
			{
				m_InputClaw.SetRelayState(RELAY_ON);
				m_LFHolder.SetRelayState(RELAY_ON);
				long lPos = round((double)m_stTopTrackRcd.lLfStandbyLen / theStepper.GetResol(m_nIndexStepID));
				long lAbsPos = round((double)(LogicToPhyPos(0) - 8000) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
				EnterDelayState(DEVICE_FINE_WAIT_FORWARD, GetCurrentTime(), RELAY_ACTION_DELAY , lPos + (lAbsPos - searchpos) + pos, 1);
			}
		}
		else
		{
			if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
			{
				TOP_DEVICE_RECORD stDeviceRcd;
				theDevice.GetTopRcd(&stDeviceRcd);
				if (m_stIndexerVar.nCatchUpUnit == 0)
					m_lStandbyAfterSearch = round(LOCATION_SENSOR_CALI_DIST - (m_stTopTrackRcd.lLfStandbyLen + m_stIndexerVar.lNorIndexLen * 0.5 + stDeviceRcd.lDeviceEdgeLen * 2) + m_stTopTrackRcd.lIndexError[0]);
				else
					m_lStandbyAfterSearch = round(LOCATION_SENSOR_CALI_DIST - (m_stTopTrackRcd.lLfStandbyLen + m_stIndexerVar.lNorIndexLen * 1.5 + stDeviceRcd.lDeviceEdgeLen * 2) + m_stTopTrackRcd.lIndexError[0]);

				EnterDelayState(DEVICE_FINE_WAIT_FORWARD, GetCurrentTime(), 0,
					round((double)m_lStandbyAfterSearch / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
			}
			else
			{
				m_InputClaw.SetRelayState(RELAY_ON);
				m_LFHolder.SetRelayState(RELAY_ON);
				pos = theStepper.GetPos(m_nIndexStepID, FALSE);	//v3.0.T376 edit
				long lPos = round((double)m_stTopTrackRcd.lLfStandbyLen / theStepper.GetResol(m_nIndexStepID));
				long lAbsPos = round((double)(LogicToPhyPos(0) - 8000) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
				EnterDelayState(DEVICE_FINE_WAIT_FORWARD, GetCurrentTime(), RELAY_ACTION_DELAY, lPos + lAbsPos, 1);
			}
		}
		l1stIndexLen = m_stIndexerVar.l1stIndexLen;
		
		break;
	}
	case DEVICE_INDEX_SLOW_SRCH:	//V1.5.W116 add
	{
		if(theStepper.IsBusy(m_nIndexStepID))break;

		ChangeStepperMotionProfile(SLOW_SRCH_PROFILE);
		theStepper.MoveStep(m_nIndexStepID,800,MOTION_RELATIVE);

		m_cState = DEVICE_INDEX_WAIT_SLOW_SRCH;

		break;
	}
	case DEVICE_INDEX_WAIT_SLOW_SRCH:	//V1.5.W116 add
	{
		if(m_AlignSensor.GetCurrentState()==SENSOR_OFF)break;

		theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);
		EnterDelayState(DEVICE_FINE_SRCH_WAIT_STANDBY, GetCurrentTime(), 100, 0, 0);

		break;
	}
	case DEVICE_INDEX_SLOW_SRCH_BACK:	//V1.5.W164 add
	{
		if(m_AlignSensor.GetCurrentState()==SENSOR_ON)
		{
            if (!theStepper.IsBusy(m_nIndexStepID))          //v1.6.P31  add
            {
				CStringList sList;
				sList.AddTail(theMsg.GetMsg(MSG_SLOW_SRCH_FAIL));
                theMsg.ShowMsg(ERROR_MSG, sList);
            }
			break;
		}

		theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);
		EnterDelayState(DEVICE_FINE_SRCH_WAIT_STANDBY, GetCurrentTime(), 100, 0, 0);

		break;
	}
	case DEVICE_FINE_WAIT_FORWARD:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-FineWaitForward"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		if (m_nEvent == INITIAL_INDEX_EVENT)  // 1st Slip Heat Delay
		{
			m_stSlipOnTrack.lSlipHeaderPos = -LOCATION_SENSOR_CALI_DIST + m_lStandbyAfterSearch;
			m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipHeaderPos - m_stIndexerVar.lLfTotalLen;
			m_stSlipOnTrack.lSlipTailPos += 8000;	//v1.1.288.T18 add

			EnterDelayState(m_cStateSequence[0], GetCurrentTime(), 800 + m_stTopTrackRcd.l1stSlipHeatDelay,
				round((double)LogicToPhyPos(m_stIndexerVar.lLeftLimitPos) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit
		}
		else
		{
			m_stSlipOnTrack.lSlipHeaderPos = -LOCATION_SENSOR_CALI_DIST + m_lStandbyAfterSearch;
			EnterDelayState(m_cStateSequence[0], GetCurrentTime(), RELAY_DELAY, 0, 0);
			m_nNextLfState = NEXT_LF_AFTER_SRCH;
		}

		LeftMoveArray();
        break;
	}
//-------------w.index--------------------------//
// 24~ 29 : Index Procedure
	case DEVICE_INDEX:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevIndex"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(false);
		EnterDelayState(DEVICE_INDEX_WAIT_BH_SAFTY,GetCurrentTime(), 0, 0, 0);
        break;
	}
	case DEVICE_INDEX_WAIT_BH_SAFTY:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitBhSafety"), m_cState);
			m_nLastDebugState = m_cState;
		}

		TBL_POS stPos;
		theServo.GetTablePosition(&stPos);
		theTopOffloader.SetEmptyReported(false);
		//v3.1.T220 delete
		//if ((fabs(stPos.dx - theMcData.GetXYStandbyPos().dx) < 2.0)
		//	&& (fabs(stPos.dy - theMcData.GetXYStandbyPos().dy) < 2.0))
		//v3.1.T220 add
		if ((fabs(stPos.dx - theMcData.GetXYStandbyPos().dx) < theMcConfig.GetSafePosTol())
			&& (fabs(stPos.dy - theMcData.GetXYStandbyPos().dy) < theMcConfig.GetSafePosTol()))
		{
			if (m_stTopTrackRcd.bVacuumControl == true)
			{
				theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
				m_cState = DEVICE_INDEX_WAIT_VACUUM_DELAY;
			}
			else
				m_cState = DEVICE_INDEX_WAIT_CLAW_CLAMP;
		}
		else
		{
			m_nErrorCode = MHS_TOP_BH_NOT_IN_SAFE_POS;
			m_cState = DEVICE_INDEX_STOP;
		}
		break;
	}
    case DEVICE_INDEX_WAIT_VACUUM_DELAY:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitVacDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		EnterDelayState(DEVICE_INDEX_WAIT_CLAW_CLAMP, GetCurrentTime(), m_stTopTrackRcd.lVacuumDelay, 0, 0); 
		break;
	}
	case DEVICE_INDEX_WAIT_CLAW_CLAMP:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClawClamp"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		theTopClamp.MoveClampSafePos();

		if (m_nEvent == INITIAL_INDEX_EVENT)
		{
			m_stIndexerVar.lIndexLenAftPR = l1stIndexLen;
			m_pTargetClaw->SetRelayState(RELAY_ON);

			if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);
		}
		else if (m_nEvent == INDEX_NEW_SLIP_EVENT)
		{
			if (m_stIndexerVar.nCatchUpUnit != 0)
			{
				m_stIndexerVar.lIndexLenAftPR = l1stIndexLen - m_stIndexerVar.lCatchUpLen;
			}
			else
			{
				m_stIndexerVar.lIndexLenAftPR = l1stIndexLen;
			}

			m_InputClaw.SetRelayState(RELAY_ON);
			m_OutputClaw.SetRelayState(RELAY_ON);
			m_LFHolder.SetRelayState(RELAY_ON);
		}
		else
		{
			TOP_DEVICE_RECORD stDeviceRcd;
			theDevice.GetTopRcd(&stDeviceRcd);

			if ((m_nEvent == INDEX_UNIT_STANDBY_EVENT)
				&& (stDeviceRcd.nHalfIndexOn > 0)
				&& (m_nCurBondUnit == (m_stIndexerVar.nTotalUnits - 1)))
			{
				m_stIndexerVar.lIndexLenAftPR = m_stIndexerVar.lNorIndexLen / 2 + m_stTopTrackRcd.lIndexError[m_nCurBondUnit];
			}
			else
			{
				m_stIndexerVar.lIndexLenAftPR = m_stIndexerVar.lNorIndexLen + m_stTopTrackRcd.lIndexError[m_nCurBondUnit];
			}

			if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);
			m_pTargetClaw->SetRelayState(RELAY_ON);
		}

		m_cState = DEVICE_INDEX_WAIT_CLAMP_OPEN;
		break;
	}
	case DEVICE_INDEX_WAIT_CLAMP_OPEN:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d_%d-WaitClampOpen"), m_cState, m_nEvent);
			m_nLastDebugState = m_cState;
		}

		if (theMcConfig.GetStepperType() != STEP_TYPE_STEPPERIO)
		{
			if (theStepper.IsBusy(theTopClamp.GetStepID()))
				break;
		}

		theTopClamp.CheckClampStatus();
		if (theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
		{
			if (m_nEvent != INITIAL_INDEX_EVENT)
				EnterDelayState(DEVICE_INDEX_WAIT_CLAMP_OPEN_DELAY, GetCurrentTime(), m_stTopTrackRcd.lOpenClampDelay, 0, 0);
			else
				m_cState = DEVICE_INDEX_WAIT_CLAMP_OPEN_DELAY;
		}
		break;
	}
	case  DEVICE_INDEX_WAIT_CLAMP_OPEN_DELAY:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClampOpenDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if(m_bClampSensorEnabled==TRUE&&m_TrackBackHome.GetCurrentState()==SENSOR_ON)	//V1.5.W17 edit
		{
			m_nErrorCode = MHS_TOP_WINDOW_CLAMPER_NOT_OPENED;
			m_cState = DEVICE_INDEX_STOP;

			break;
		}

		theApp.SetRedLamp(false);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
			+ round((double)m_stIndexerVar.lIndexLenAftPR / theStepper.GetResol(m_nIndexStepID));	//v3.0.T372 edit, v3.0.T376 edit
		EnterDelayState(DEVICE_INDEX_WAIT_FEED_IN, GetCurrentTime(), 0, m_lMoveDist);
		break;
	}
	case DEVICE_INDEX_WAIT_FEED_IN:		//V1.4.W35 edit, V1.5.W8 edit
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitFeedIn"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		if (m_lMoveDist != theStepper.GetPos(m_nIndexStepID)) break;	//v3.0.T376 edit

		if (m_stTopTrackRcd.bFlattenLF)
		{
			theTopClamp.CloseClamp();
			EnterDelayState(theMcConfig.IsPrIndexHalfClamp() ? DEVICE_INDEX_AFTER_FEED_IN : DEVICE_INDEX_AFTER_FEED_IN_2, GetCurrentTime(), m_stTopTrackRcd.wFlattenDuration, 0, 0);
		}
		else
		{
			m_cState = theMcConfig.IsPrIndexHalfClamp() ? DEVICE_INDEX_AFTER_FEED_IN : DEVICE_INDEX_AFTER_FEED_IN_2;
		}

		break;
	}
	case DEVICE_INDEX_AFTER_FEED_IN:		//V1.4.W35 add
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-AfterFeedIn"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.PreCloseClamp();
		m_cState = DEVICE_INDEX_WAIT_PR_INDEX_DELAY;

        if (m_nEvent != INDEX_NEW_SLIP_EVENT)break;

        m_OutputClaw.SetRelayState(RELAY_OFF);

        if ((theMHS.IsLastLf() == false)
			&& (m_stIndexerVar.nTotalUnits  <= 3))
        {
            RequestOnload();
        }

        if (theMHS.IsLastLf())
        {
            BOOL bNextLfReady = 1;

            if (m_stIndexerVar.nTotalUnits > 3)
            {
                if (m_nNextLfState != NEXT_LF_AFTER_CATCH_UP)
                    bNextLfReady = NEXT_LF_NONE;
            }
            else
            {
                if (m_nNextLfState != NEXT_LF_AFTER_SRCH)
                    bNextLfReady = NEXT_LF_NONE;
            }

            if ((m_AlignSensor.GetCurrentState() == FALSE) && (bNextLfReady == NEXT_LF_NONE))
            {
                m_nCurBondUnit = 0;
                m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexerVar.lIndexLenAftPR;
                m_nErrorCode = MHS_END_ALL_LF;
                m_cState = DEVICE_INDEX_STOP;
            }
            else
            {
                m_nNextLfState = NEXT_LF_NONE;
            }
        }
        else
        {
            m_nNextLfState = NEXT_LF_NONE;
        }

        break;
	}
	case DEVICE_INDEX_AFTER_FEED_IN_2:		//V1.4.W71 add
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-AfterFeedIn2"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.CloseClamp();
		m_cState = DEVICE_INDEX_WAIT_PR_INDEX_DELAY_2;

        if (m_nEvent != INDEX_NEW_SLIP_EVENT)break;

        m_OutputClaw.SetRelayState(RELAY_OFF);

        if ((theMHS.IsLastLf() == false)
			&& (m_stIndexerVar.nTotalUnits  <= 3))
        {
            RequestOnload();
        }

        if (theMHS.IsLastLf())
        {
            BOOL bNextLfReady = 1;

            if (m_stIndexerVar.nTotalUnits > 3)
            {
                if (m_nNextLfState != NEXT_LF_AFTER_CATCH_UP)
                    bNextLfReady = NEXT_LF_NONE;
            }
            else
            {
                if (m_nNextLfState != NEXT_LF_AFTER_SRCH)
                    bNextLfReady = NEXT_LF_NONE;
            }

            if ((m_AlignSensor.GetCurrentState() == FALSE) && (bNextLfReady == NEXT_LF_NONE))
            {
                m_nCurBondUnit = 0;
                m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexerVar.lIndexLenAftPR;
                m_nErrorCode = MHS_END_ALL_LF;
                m_cState = DEVICE_INDEX_STOP;
            }
            else
            {
                m_nNextLfState = NEXT_LF_NONE;
            }
        }
        else
        {
            m_nNextLfState = NEXT_LF_NONE;
        }

        break;
	}
	case  DEVICE_INDEX_WAIT_PR_INDEX_DELAY:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitPrIndexDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		EnterDelayState(DEVICE_INDEX_WAIT_CLAMP_PRE_CLOSE, GetCurrentTime(), m_stTopTrackRcd.lIndexPrDelay, 0, 0);

		m_nCurBondUnit++;
		if (m_nCurBondUnit > m_stIndexerVar.nTotalUnits)
			m_nCurBondUnit = 1;

       break;
	}
	case  DEVICE_INDEX_WAIT_PR_INDEX_DELAY_2:	//V1.5.W79 add
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitPrIndexDelay2"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(true);
		if (theDebug.IsDebugIO()) Sleep_x1ms(2);

		EnterDelayState(DEVICE_INDEX_WAIT_CLAMP_CLOSE_2, GetCurrentTime(), m_stTopTrackRcd.lIndexPrDelay, 0, 0);

		m_nCurBondUnit++;
		if (m_nCurBondUnit > m_stIndexerVar.nTotalUnits)
			m_nCurBondUnit = 1;

       break;
	}
	case DEVICE_INDEX_WAIT_CLAMP_PRE_CLOSE:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClampPreClose"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (!m_stTopTrackRcd.bVacuumControl)	//v3.0.T337 add
		//vacuum enabled not check pre-close resolved hang but reason unknown
		//vacuum disabled remain checking
		{
			theTopClamp.CheckClampStatus();
			if (theTopClamp.GetClampStatus() != WND_CLAMP_PRE_CLOSE) break;
		}

		INDEX_PR_REF stPrIndexRefRcd;
		theDevice.GetIndexPrRef(&stPrIndexRefRcd);

		if (((m_stTopTrackRcd.nPrIndexEnable == 2)	//enable 1st unit only
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)
				&& (GetCurBondUnit() == 1))
			|| ((m_stTopTrackRcd.nPrIndexEnable == 1)	//enable for all
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)))
		{
			m_nPRIState = 1;		//V1.5.W123 add

			long lPrOffset;	//v3.0.T371 add
			int iReturnStatus = CalcPrIndexOffset(&lPrOffset);
			if (abs(lPrOffset) > m_stTopTrackRcd.lPrIndexTolerance)
			{
				if (iReturnStatus == FOK)
				{
					SetIndexPrManualAlign(INDEX_PR_OK);
					if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
					{
						m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
							+ round((double)lPrOffset / theStepper.GetResol(m_nIndexStepID));	//v3.0.T372 edit, v3.0.T376 edit
						EnterDelayState(DEVICE_INDEX_WAIT_PR_FINE_TUNE, GetCurrentTime(), 0, m_lMoveDist);
					}
					else
					{
						m_lMoveDist = round((double)lPrOffset / theStepper.GetResol(m_nIndexStepID));
						EnterDelayState(DEVICE_INDEX_WAIT_PR_FINE_TUNE, GetCurrentTime(), 0, m_lMoveDist, 0);
					}
					m_stIndexerVar.lIndexLenAftPR = m_stIndexerVar.lIndexLenAftPR + lPrOffset;
				}
				else 
				{
					m_nPRIState = 0;		//V1.5.W124 add
					SetIndexPrManualAlign(INDEX_PR_ENTER_MANU_ALIGN);
					m_cState = DEVICE_INDEX_WAIT_MANU_ALIGN;
				}
			}
			else
				m_cState = DEVICE_INDEX_WAIT_PR_FINE_TUNE;
		}
		else
			m_cState = DEVICE_INDEX_WAIT_PR_FINE_TUNE;

		theServo.WaitTableStop();
		theServo.MoveTable2(stPrIndexRefRcd.stVisionAlignPos, true, false, false);	//v3.1.T208 edit
        
        break;
	}
	case DEVICE_INDEX_WAIT_CLAMP_CLOSE_2:	//V1.5.W79 add
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClampClose"), m_cState);
			m_nLastDebugState = m_cState;
		}

        theTopClamp.CheckClampStatus();
        if (theTopClamp.GetClampStatus() != WND_CLAMP_CLOSE) break;
        
		INDEX_PR_REF stPrIndexRefRcd;
		theDevice.GetIndexPrRef(&stPrIndexRefRcd);

		m_bNeedOffset = FALSE;

		if (((m_stTopTrackRcd.nPrIndexEnable == 2)	//enable 1st unit only
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)
				&& (GetCurBondUnit() == 1))
			|| ((m_stTopTrackRcd.nPrIndexEnable == 1)	//enable for all
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)))
		{
			m_nPRIState = 1;		//V1.5.W140 add

			long lPrOffset;	//v3.0.T371 add
			m_nPRIndexStatus = CalcPrIndexOffset(&lPrOffset);
			if (abs(lPrOffset) > m_stTopTrackRcd.lPrIndexTolerance)
			{
				m_bNeedOffset = TRUE;

				if (m_nPRIndexStatus == FOK)
				{
					if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
						m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
							+ round(lPrOffset / theStepper.GetResol(m_nIndexStepID));	//v3.0.T372 edit, v3.0.T376 edit
					else
						m_lMoveDist = round(lPrOffset / theStepper.GetResol(m_nIndexStepID));

					m_stIndexerVar.lIndexLenAftPR = m_stIndexerVar.lIndexLenAftPR + lPrOffset;
				}
				else
					m_nPRIState = 0;		//V1.5.W140 add

				EnterDelayState(DEVICE_INDEX_CLAMP_PRE_CLOSE, GetCurrentTime(), 50, 0, 0);
			}
			else
				m_cState = DEVICE_INDEX_WAIT_PR_FINE_TUNE;
		}
		else
			m_cState = DEVICE_INDEX_WAIT_PR_FINE_TUNE;

		theServo.WaitTableStop();
		theServo.MoveTable2(stPrIndexRefRcd.stVisionAlignPos, true, false, false);	//v3.1.T208 edit
        
        break;
	}
	case DEVICE_INDEX_CLAMP_PRE_CLOSE:	//V1.5.W79 add
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClampPreClose"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.PreCloseClamp();
		EnterDelayState(DEVICE_INDEX_WAIT_CLAMP_PRE_CLOSE_2, GetCurrentTime(), 50, 0, 0);

		break;
	}
	case DEVICE_INDEX_WAIT_CLAMP_PRE_CLOSE_2:	//V1.5.W79 add
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClampPreClose2"), m_cState);
			m_nLastDebugState = m_cState;
		}

        theTopClamp.CheckClampStatus();
        if (theTopClamp.GetClampStatus() != WND_CLAMP_PRE_CLOSE) break;

		if (m_bNeedOffset==TRUE)
		{
			if (m_nPRIndexStatus == FOK)
			{
				SetIndexPrManualAlign(INDEX_PR_OK);

				if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
				{
					//EnterDelayState(DEVICE_INDEX_WAIT_PR_FINE_TUNE, GetCurrentTime(), 0, m_lMoveDist);
					theStepper.MoveStep(m_nIndexStepID, m_lMoveDist);
				}
				else
				{
					//EnterDelayState(DEVICE_INDEX_WAIT_PR_FINE_TUNE, GetCurrentTime(), 0, m_lMoveDist, 0);
					theStepper.MoveStep(m_nIndexStepID, m_lMoveDist, MOTION_RELATIVE);
				}

				m_cState=DEVICE_INDEX_WAIT_PR_FINE_TUNE;
			}
			else
			{
				SetIndexPrManualAlign(INDEX_PR_ENTER_MANU_ALIGN);
				m_cState = DEVICE_INDEX_WAIT_MANU_ALIGN;
			}
		}
		else
		{
			m_cState = DEVICE_INDEX_WAIT_PR_FINE_TUNE;
		}

		break;
	}
	case DEVICE_INDEX_WAIT_MANU_ALIGN:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitManualAlign"), m_cState);
			m_nLastDebugState = m_cState;
		}

        if (GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_STOP)
        {
            m_cState = DEVICE_INDEX_WAIT_PR_FINE_TUNE;
        }
        else if (GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_OK)
        {
            m_pTargetClaw->SetRelayState(RELAY_ON);
            if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);

			if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			{
				m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
					+ round((double)m_lOffsetx1um / theStepper.GetResol(m_nIndexStepID));	//v3.0.T372 edit, v3.0.T376 edit
				EnterDelayState(DEVICE_INDEX_WAIT_PR_FINE_TUNE, GetCurrentTime(), RELAY_ACTION_DELAY, m_lMoveDist);
			}
			else
			{
				m_lMoveDist = round((double)m_lOffsetx1um / theStepper.GetResol(m_nIndexStepID));
				EnterDelayState(DEVICE_INDEX_WAIT_PR_FINE_TUNE, GetCurrentTime(), RELAY_ACTION_DELAY, m_lMoveDist, 0);
			}
			m_stIndexerVar.lIndexLenAftPR = m_stIndexerVar.lIndexLenAftPR + m_lOffsetx1um;
        }
        break;
	}
	case DEVICE_INDEX_WAIT_PR_FINE_TUNE:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitPrFineTune"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theApp.SetRedLamp(false);
		if (theStepper.IsBusy(m_nIndexStepID)) break;
		if (theStepper.IsBusy(theTopClamp.GetStepID())) break;

		m_InputClaw.SetRelayState(RELAY_OFF);
		m_OutputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		Sleep_x1ms(20);

		if (m_bTeaching == false)
		{
			theTopClamp.CloseClamp();
		}

		if ((m_nEvent == INDEX_NEW_SLIP_EVENT)
			|| (m_nEvent == INITIAL_INDEX_EVENT))
		{
			//Head Position transfer to tail position
			//Head Position Reset.
			if (m_nEvent == INDEX_NEW_SLIP_EVENT)
				m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexerVar.lIndexLenAftPR;

			m_stSlipOnTrack.lSlipHeaderPos = m_stSlipOnTrack.lSlipHeaderPos + m_stIndexerVar.lIndexLenAftPR;
			m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipHeaderPos - m_stIndexerVar.lLfTotalLen;
			m_stSlipOnTrack.lSlipTailPos += 8000;	//v1.1.288.T18 add

			//reset header
			m_stSlipOnTrack.lSlipHeaderPos = -CENTER_TO_LEFT_TRACK_EDGE;
		}
		else
		{
			//Only Update Tail Position
			m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexerVar.lIndexLenAftPR;
		}
		m_cState = DEVICE_INDEX_WAIT_CLAMP_CLOSE;
        break;
	}
	case DEVICE_INDEX_WAIT_CLAMP_CLOSE:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClampClose"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.CheckClampStatus();
		if (m_bTeaching || (theTopClamp.GetClampStatus() == WND_CLAMP_CLOSE))
		{
			m_InputClaw.SetRelayState(RELAY_OFF);
			m_OutputClaw.SetRelayState(RELAY_OFF);
            m_LFHolder.SetRelayState(RELAY_OFF);

			if (m_stIndexerVar.nTotalUnits >= 3)
			{
				if ((m_nCurBondUnit == m_stIndexerVar.nInjectUnit)
					&& (theMHS.IsLastLf() == false))
				{
					RequestOnload();
				}
			}

			m_lStartDelayTime = GetCurrentTime();

			if (m_nPRIState==1&&theDebug.IsDebugIndexAccuracy())	//V1.5.W123 add, V1.5.W140 edit
				EnterDelayState(DEVICE_INDEX_DELAY_AFTER_CLAMP_CLOSE, GetCurrentTime(), 100, 0, 0);
			else
				m_cState = DEVICE_INDEX_WAIT_HEAT_DELAY;
		}
		break;
	}
	case DEVICE_INDEX_DELAY_AFTER_CLAMP_CLOSE:	//V1.5.W140 add
	{
		m_nPRIState = 2;
		long lPrOffset = 0;
		CalcPrIndexOffset(&lPrOffset);
		m_nPRIState = 0;
		m_cState = DEVICE_INDEX_WAIT_HEAT_DELAY;
		break;
	}
	case DEVICE_INDEX_WAIT_HEAT_DELAY:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitHeatDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (m_nEvent == INITIAL_INDEX_EVENT)
        {
			if (GetCurrentTime() - m_lStartDelayTime  > (m_bTeaching ? 1000 : m_stTopTrackRcd.lInitDelay))	//V1.5.W64 edit
            {
                theMHS.SetRequestIndex(false);
                m_cState = m_cStateSequence[0];    
                LeftMoveArray();
            }
        }
        else
        {
            if (m_nEvent == INDEX_NEW_SLIP_EVENT)
            {
                if (GetCurrentTime() - m_lStartDelayTime > m_stTopTrackRcd.l1stUnitHeatDelay)
                {
                    theMHS.SetRequestIndex(false);
                    m_cState = m_cStateSequence[0];    
                    LeftMoveArray();
                }
            }
            else
            {
                if (GetCurrentTime() - m_lStartDelayTime > m_stTopTrackRcd.lHeatingDelay)
                {
                    theMHS.SetRequestIndex(false);
                    m_cState = m_cStateSequence[0];    
                    LeftMoveArray();
                }
            }
        }
        break;
	}
	case DEVICE_INDEX_STOP:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-IndexStop"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theMHS.SetRequestIndex(false);
		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_OutputClaw.SetRelayState(RELAY_OFF);
		m_cState = m_cStateSequence[0];    
		LeftMoveArray();
		break;
	}
//output
// 17~21 ---------Output and Eject the front slip---------- 
//output loop:
	case DEVICE_OUTPUT:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevOutput"), m_cState);
			m_nLastDebugState = m_cState;
		}

		ChangeStepperMotionProfile(BACKWARD_PROFILE);

        if (theDebug.IsDebugWorkHolder())
            theDebug.AddInfo(_T("SlipTailPos"), m_stSlipOnTrack.lSlipOutputTailPos);

		if (MoveIndexerToWorkPos(m_stSlipOnTrack.lSlipOutputTailPos))
			m_cState =DEVICE_EJECTION;
		else
			m_cState = DEVICE_OUTPUT_WAIT_AT_OUTPUT_POS;

		m_lStartTime = GetCurrentTime();
		break;
	}
	case DEVICE_OUTPUT_WAIT_AT_OUTPUT_POS:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitOutputPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		if (theTopOffloader.GetEvent() == TOP_OFFLOAD_EVENT_IDLE)
		{
			long lOriginPos = theStepper.GetPos(m_nIndexStepID);	//v3.0.T376 edit
			m_pTargetClaw->SetRelayState(TRUE);
			if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);
			long lPos = round((double)LogicToPhyPos(m_stIndexerVar.lRightLimitPos) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
			m_lMoveDist = round((double)(lPos - lOriginPos) * theStepper.GetResol(m_nIndexStepID));
			EnterDelayState(DEVICE_OUTPUT_ONE_CIRCLE_DONE, GetCurrentTime(), RELAY_ACTION_DELAY, lPos);
		}
		else
		{
			if (GetCurrentTime() - m_lStartTime > ELEVATOR_TIME_OUT)
			{
				m_cState = CLAW_STAND_BY;
				m_nErrorCode = MSG_OFFLOAD_TIMEOUT;
			}
		}
        break;
	}
	case DEVICE_OUTPUT_ONE_CIRCLE_DONE:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-OneCircleDone"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipOutputTailPos + m_lMoveDist;
		m_pTargetClaw->SetRelayState(FALSE);
		if (m_pTargetClaw == &m_InputClaw)
			m_LFHolder.SetRelayState(RELAY_OFF);
		m_cState = DEVICE_OUTPUT;
        break;
	}
//Ejection Start:
	case DEVICE_EJECTION:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevEject"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		ChangeStepperMotionProfile(BACKWARD_PROFILE);
		EnterDelayState(DEVICE_OUTPUT_WAIT_AT_EJECT_POS, GetCurrentTime(), 0,
			round((double)LogicToPhyPos(m_stIndexerVar.lEjectStartPos) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit
		break;
	}
	case DEVICE_OUTPUT_WAIT_AT_EJECT_POS:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitEjectPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		long lPos = round((double)LogicToPhyPos(m_stIndexerVar.lEjectStartPos) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
		if (lPos != theStepper.GetPos(m_nIndexStepID, FALSE)) break;	//v3.0.T376 edit

		//clamp Ejector
		ChangeStepperMotionProfile(EJECT_SPEED_PROFILE);
		m_Ejector.SetRelayState(TRUE);
		EnterDelayState(DEVICE_OUTPUT_WAIT_EJECT_DEST, GetCurrentTime(), SMALL_RELAY_DELAY,
			round((double)LogicToPhyPos(m_stIndexerVar.lRightLimitPos) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit
    	break;
	}
	case DEVICE_OUTPUT_WAIT_EJECT_DEST:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitEjectDest"), m_cState);
			m_nLastDebugState = m_cState;
		}

		long lPos = round((double)LogicToPhyPos(m_stIndexerVar.lRightLimitPos) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
        if (theStepper.GetPos(m_nIndexStepID) == lPos)	//v3.0.T376 edit
        {
			EnterDelayState(DEVICE_OUTPUT_BACKWARD_AFTER_EJECT, GetCurrentTime(), 30,
				round((double)LogicToPhyPos(m_stIndexerVar.lRightLimitPos - EJECT_BACKWARD_STEPS_AFTER_EJECT) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit
        }
        else if (m_EjectJamSensor.GetCurrentState() == FALSE)
        {
            theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);
            m_Ejector.SetRelayState(FALSE);
            m_cState = m_cStateSequence[0];   
            m_nErrorCode = MHS_OFFLOAD_PUSHER_JAM;

			while (theStepper.IsBusy(m_nIndexStepID))
			{
				Sleep_x1ms(10);
			}

			EnterDelayState(DEVICE_OUTPUT_BACKWARD_AFTER_EJECT, GetCurrentTime(), 0,
				round((double)-10000 / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add

            LeftMoveArray();
        }
        break;
	}
	case DEVICE_OUTPUT_BACKWARD_AFTER_EJECT:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-BackwardAfterEject"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		//release ejector
		m_Ejector.SetRelayState(FALSE);

		if (m_EjectSensor.GetCurrentState() == FALSE)
			m_bOffloadEnable = true;
		else
		{
			m_nErrorCode =  MHS_OFFLOAD_TRACK_JAM;
			do 
			{
				LeftMoveArray();
				m_cState = m_cStateSequence[0];
			} while (m_cStateSequence[0] != INDEX_STATE_IDLE);
			break;
		}

		if (m_nEvent == INDEX_EVENT_CLEAR_LAST_UNIT)
			m_nCurBondUnit = 0;

		m_cState = m_cStateSequence[0];   
		LeftMoveArray();

		if (theMHS.IsLastLf() && (m_nCurBondUnit == 0))
		{
			theTopTrack.ResetSlipRcd();
			theTopTrack.ResetRelayErrorState();
			theTopTrack.MoveIndexerToLogicZero();

			theMHS.SetLastLf(false);
			theMHS.SetBusy(true);
		}
		else
		{
			if ((m_stIndexerVar.nTotalUnits > 3) && (m_stIndexerVar.nInjectUnit == 1))
			{
				if ((theMHS.IsLastLf() == false)
					&& (m_nEvent == INDEX_NEW_SLIP_EVENT))
				{
					RequestOnload();
				}
			}
		}
		break;
	}
//*****************************************Standby Index Position*******************************************
	case CLAW_STAND_BY:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClawStandby"), m_cState);
			m_nLastDebugState = m_cState;
		}

		MoveIndexerToWorkPos(m_stSlipOnTrack.lSlipTailPos);
		m_cState = CLAW_STAND_BY_WAIT_DEST_POS;
		break;
	}
	case CLAW_STAND_BY_WAIT_DEST_POS:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClawWaitDestPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		ChangeStepperMotionProfile(INDEX_SPEED_PROFILE);
		m_cState = m_cStateSequence[0]; 
		LeftMoveArray();
		break;
	}
//******************************FOR DELAY***********************************************
    case TOP_INDEX_DELAY_WAITING:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-IndexDelayWaiting"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (GetCurrentTime() - m_stDelayState.lStartTime < m_stDelayState.lDelayDuration) break;
		if (theStepper.IsBusy(m_nIndexStepID)) break;

		if ((m_stDelayState.bMoveMode == 0) && (m_stDelayState.lMoveDist != 0))
			theStepper.MoveStep(m_nIndexStepID, m_stDelayState.lMoveDist, MOTION_RELATIVE);
		else if (m_stDelayState.bMoveMode == 1)
			theStepper.MoveStep(m_nIndexStepID, m_stDelayState.lMoveDist);

		m_cState = m_stDelayState.nNextState;

        break;
	}
//*****************************FOR CLEAR TRACK*****************************************
	case TOP_CLEAR_TRACK:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrack"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theMcConfig.IsClearTrackCheckClampOpen())	//v3.0.T358 add
		{
			theTopClamp.CheckClampStatus();
			if (theTopClamp.GetClampStatus() != WND_CLAMP_SAFE_CLAMP) break;
		}

		ChangeStepperMotionProfile(INDEX_SPEED_PROFILE);
		m_cState = m_cStateSequence[0]; 
		LeftMoveArray();
        break;
	}
	case TOP_CLEAR_TRACK_START_1ST:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackStart1st"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos;
		m_cState = DEVICE_OUTPUT;
		break;
	}
	case TOP_CLEAR_TRACK_START_2ND:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackStart2nd"), m_cState);
			m_nLastDebugState = m_cState;
		}

		TOP_DEVICE_RECORD stDeviceRcd;
		theDevice.GetTopRcd(&stDeviceRcd);
        if (m_stSlipOnTrack.lSlipHeaderPos >= -LOCATION_SENSOR_CALI_DIST)
        {
            m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipHeaderPos - stDeviceRcd.lDeviceTotalLen;
            m_cState=DEVICE_OUTPUT;
        }
        else
        {
            m_cState = INDEX_STATE_IDLE;
        }
        break;
	}
	case CLAW_POSITION_RESET:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClawPosReset"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_InputClaw.SetRelayState(FALSE);
        m_LFHolder.SetRelayState(RELAY_OFF);
		m_OutputClaw.SetRelayState(FALSE);
		theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(0));	//v3.0.T374 edit
		m_cState = TOP_CLEAR_TRACK_OFFLOAD_1ST;
		break;
	}
	case TOP_CLEAR_TRACK_OFFLOAD_1ST:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackOffload1st"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		theTopOffloader.SubmitOffloadEvent(TOP_OFFLOAD_EVENT_START);
		m_cState = TOP_CLEAR_TRACK_WAIT_OFFLOAD_PROC;
		break;
	}
	case TOP_CLEAR_TRACK_WAIT_OFFLOAD_PROC:
	{
		if (m_cState != m_nLastDebugState)	//v1.5.T58 add
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackWaitOffload"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)
			theTopOffloader.ProcessOffloadEvent();
		else
		{
			theTopTrack.SetOffloadEnable(false);
			m_cState = m_cStateSequence[0]; 
			LeftMoveArray();
		}
		break;
	}
	default:
		break;
	}
}

//transfer indexer coordinate to globe coordinate
long CTopTrack::TransX(long lIndexerX, char cClawSide)
{
	return lIndexerX + (cClawSide == LEFT_CLAW ? -CENTRE_TO_LEFT_CLAW : CENTRE_TO_RIGHT_CLAW);
}

//transfer globe coordinate to indexer coordinate 
long CTopTrack::TransXRev(long lIndexerX, char cClawSide)
{
	return lIndexerX + (cClawSide == LEFT_CLAW ? CENTRE_TO_LEFT_CLAW : -CENTRE_TO_RIGHT_CLAW);
}

int CTopTrack::MoveIndexerToWorkPos(long lRefTailPos)
{
	long lLfHeadPos;
	long lPos;

	TOP_DEVICE_RECORD stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);

	ChangeStepperMotionProfile(FORWARD_PROFILE);

	m_stIndexerVar.lLeftClawIndexLimitPos = TransX(m_stIndexerVar.lRightLimitPos, LEFT_CLAW) - (m_stIndexerVar.lNorIndexLen + m_stTopTrackRcd.lIndexError[m_nCurBondUnit]);
	m_stIndexerVar.lRightClawIndexLimitPos = TransX(m_stIndexerVar.lLeftLimitPos, RIGHT_CLAW);  

	if (lRefTailPos + m_stTopTrackRcd.lIndexPosOffset <= m_stIndexerVar.lLeftClawIndexLimitPos)
	{
		//transfer to indexer coordinate
		if (lRefTailPos <= TransX(m_stIndexerVar.lLeftLimitPos, LEFT_CLAW))
			lPos = m_stIndexerVar.lLeftLimitPos + 10000;
		else
			lPos = TransXRev(lRefTailPos + m_stTopTrackRcd.lIndexPosOffset + 5000, LEFT_CLAW);

		//lPos= m_stIndexerVar.lRightLimitPos- (m_stIndexerVar.lNorIndexLen+m_stTopTrackRcd.lIndexError[m_nCurBondUnit])-3000; //-3000 for safety

		m_pTargetClaw = &m_InputClaw;
	}
	else //right claw 
	{
		//ejection required.
		//+2000 =>tolerance
        if (lRefTailPos + m_stTopTrackRcd.lIndexPosOffset + 2000 + stDeviceRcd.lDeviceEdgeLen > TransX(m_stIndexerVar.lRightLimitPos, RIGHT_CLAW))
		{
			return 1;
		}

		lLfHeadPos = lRefTailPos + stDeviceRcd.lDeviceTotalLen;
		//if (lLfheadPos - m_stTopTrackRcd.lIndexPosOffset < m_stIndexerVar.lRightClawIndexLimitPos)
		if (lLfHeadPos - m_stTopTrackRcd.lIndexPosOffset < 28000)
		{
			lPos = TransXRev(lRefTailPos + 5000, LEFT_CLAW);	//5mm offset default
			m_pTargetClaw = &m_InputClaw;
		}
		else
		{
			//if (lRefTailPos + m_stTopTrackRcd.lIndexPosOffset <= m_stIndexerVar.lRightClawIndexLimitPos)
			if (lRefTailPos + m_stTopTrackRcd.lIndexPosOffset <= 28000)
			{
				//lPos = m_stIndexerVar.lLeftLimitPos + 10000;
				lPos = m_stIndexerVar.lLeftLimitPos + 4000;
				m_pTargetClaw = &m_OutputClaw;
			}
			else
			{
				lPos = TransXRev(lRefTailPos + m_stTopTrackRcd.lIndexPosOffset, RIGHT_CLAW); 
				m_pTargetClaw = &m_OutputClaw;
			}
		}
	}

	theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(lPos));	//v3.0.T374 edit
	return 0;
}

void CTopTrack::EnterDelayState(char nNextState, long lDelayStartTime, long lDelayDuration_x1ms, long lMoveDist, BOOL bMoveMode)
{	//delay -> move -> goto next state
	m_stDelayState.lDelayDuration = lDelayDuration_x1ms;
	m_stDelayState.lStartTime = lDelayStartTime;
	m_stDelayState.nNextState = nNextState;
	m_stDelayState.bMoveMode = bMoveMode;
	m_stDelayState.lMoveDist = lMoveDist;

	m_cState = TOP_INDEX_DELAY_WAITING;
}

void CTopTrack::ChangeStepperMotionProfile(short nProfile)
{
	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		ST_STEPPER_SPEED stIndexerProfile;
		GetIndexerStepperProfile(nProfile, &stIndexerProfile);
		theStepperIO.SetMotorSpeedProfile(m_nIndexStepID, stIndexerProfile);
	}
	else
	{
		STEP_SPEED_PROFILE stIndexerSpeedProfile;
		GetIndexerStepperProfile(nProfile, &stIndexerSpeedProfile);
		theStepper.SetSpeedParameter(m_nIndexStepID, &stIndexerSpeedProfile);
	}
}

void CTopTrack::GetIndexerStepperProfile(short nProfile, STEP_SPEED_PROFILE *pstIndexerSpeedProfile)
{
	switch(nProfile)
	{
	case SRCH_SPEED_PROFILE:
		{
			m_stIndexerSpeedProfile.bTCurveAcc         = 0x06;
			m_stIndexerSpeedProfile.lStartVelocity     = 100;
			m_stIndexerSpeedProfile.lDriveVelocity     = 6000;   //max ~ 28000
			m_stIndexerSpeedProfile.lMaxDriveVelocity  = 80000;
			m_stIndexerSpeedProfile.lAcc               = 500000;
			m_stIndexerSpeedProfile.lAccRatio          = 156250000;
			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
			{	
				m_stIndexerSpeedProfile.lAcc           = 50000;
				m_stIndexerSpeedProfile.lAccRatio      = 100000;
			}
		}
		break;

	case SLOW_FORWARD_PROFILE:
		{
			m_stIndexerSpeedProfile.bTCurveAcc         = 0x06;
			m_stIndexerSpeedProfile.lStartVelocity     = 100;
			m_stIndexerSpeedProfile.lDriveVelocity     = 14000;   //max ~ 28000
			m_stIndexerSpeedProfile.lMaxDriveVelocity  = 80000;
			m_stIndexerSpeedProfile.lAcc               = 700000;
			m_stIndexerSpeedProfile.lAccRatio          = 78125000;
			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
			{
				m_stIndexerSpeedProfile.lDriveVelocity = 6000;
				m_stIndexerSpeedProfile.lAcc           = 50000;
				m_stIndexerSpeedProfile.lAccRatio      = 100000;
			}
			//theStepper.SetSpeedParameter(m_ucIndexerStepperID, &m_stIndexerSpeedProfile);
		}
		break;

	case FORWARD_PROFILE:
		{
			m_stIndexerSpeedProfile.bTCurveAcc         = 0x06;
			m_stIndexerSpeedProfile.lStartVelocity     = 100;
			m_stIndexerSpeedProfile.lDriveVelocity     = 25000;   //max ~ 28000
			m_stIndexerSpeedProfile.lMaxDriveVelocity  = 40000;
			m_stIndexerSpeedProfile.lAcc               = 350000;
			m_stIndexerSpeedProfile.lAccRatio          = 78125000;
			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
			{
				m_stIndexerSpeedProfile.lDriveVelocity = 6000;
				m_stIndexerSpeedProfile.lAcc           = 50000;
				m_stIndexerSpeedProfile.lAccRatio      = 100000;
			}
			//theStepper.SetSpeedParameter(m_ucIndexerStepperID, &m_stIndexerSpeedProfile);
		}

		break;

	case INJECT_SPEED_PROFILE:
		{
			m_stIndexerSpeedProfile.bTCurveAcc         = 0x06;
			m_stIndexerSpeedProfile.lStartVelocity     = 100;
			m_stIndexerSpeedProfile.lDriveVelocity     = 18000;   //max ~ 28000
			m_stIndexerSpeedProfile.lMaxDriveVelocity  = 40000;
			m_stIndexerSpeedProfile.lAcc               = 350000;
			m_stIndexerSpeedProfile.lAccRatio          = 156250000;

			//theStepper.SetSpeedParameter(m_ucIndexerStepperID, &m_stIndexerSpeedProfile);
			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
			{
				m_stIndexerSpeedProfile.lDriveVelocity = 6000;
				m_stIndexerSpeedProfile.lAcc           = 50000;
				m_stIndexerSpeedProfile.lAccRatio      = 100000;
			}
		}

		break;

	case BACKWARD_PROFILE:
		{
			m_stIndexerSpeedProfile.bTCurveAcc         = 0x06;
			m_stIndexerSpeedProfile.lStartVelocity     = 100;
			m_stIndexerSpeedProfile.lDriveVelocity     = 20000;   //max ~ 28000

			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
				m_stIndexerSpeedProfile.lDriveVelocity = 5000 * m_stTopTrackRcd.nIndexSpeedCoeff;   
			m_stIndexerSpeedProfile.lMaxDriveVelocity  = 40000;
			m_stIndexerSpeedProfile.lAcc               = 350000;
			m_stIndexerSpeedProfile.lAccRatio          = 156250000;

			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
			{	
				m_stIndexerSpeedProfile.lAcc           = 50000;
				m_stIndexerSpeedProfile.lAccRatio      = 100000;
			}
			//theStepper.SetSpeedParameter(m_ucIndexerStepperID, &m_stIndexerSpeedProfile);
		}

		break;

	case INDEX_SPEED_PROFILE:
		{
			m_stIndexerSpeedProfile.bTCurveAcc     = 0x06;
			m_stIndexerSpeedProfile.lStartVelocity = 100;
			m_stIndexerSpeedProfile.lDriveVelocity = 10000 * m_stTopTrackRcd.nIndexSpeedCoeff;   //max ~ 80000

			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
				m_stIndexerSpeedProfile.lDriveVelocity = 3000 * m_stTopTrackRcd.nIndexSpeedCoeff;   //max ~ 80000

			m_stIndexerSpeedProfile.lMaxDriveVelocity = 80000;

			//m_stIndexerSpeedProfile.lAcc = 350000;
			m_stIndexerSpeedProfile.lAcc = 90000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			m_stIndexerSpeedProfile.lAccRatio = 156250000;
			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
			{	
				m_stIndexerSpeedProfile.lAcc               = 50000;
				m_stIndexerSpeedProfile.lAccRatio          = 100000;
			}
			//theStepper.SetSpeedParameter(m_ucIndexerStepperID, &m_stIndexerSpeedProfile);
		}
		break;

	case EJECT_SPEED_PROFILE:
		{
			m_stIndexerSpeedProfile.bTCurveAcc         = 0x06;
			m_stIndexerSpeedProfile.lStartVelocity     = 1000;
			m_stIndexerSpeedProfile.lDriveVelocity     = 4000+ 4000* m_stTopTrackRcd.nEjectSpeedCoeff;   //
			m_stIndexerSpeedProfile.lMaxDriveVelocity  = 40000;
			m_stIndexerSpeedProfile.lAcc               = 350000;
			m_stIndexerSpeedProfile.lAccRatio          = 78125000;
			if (m_stTopTrackRcd.nIndexSpeedCoeff == 1 || m_stTopTrackRcd.nIndexSpeedCoeff == 2)
			{	
				m_stIndexerSpeedProfile.lAcc           = 50000;
				m_stIndexerSpeedProfile.lAccRatio      = 100000;
			}
		}
		break;

	case SLOW_SRCH_PROFILE:		//V1.5.W116 add, W1.5.W142 edit
		{
			m_stIndexerSpeedProfile.bTCurveAcc         = 0x01;
			m_stIndexerSpeedProfile.lStartVelocity     = 10;
			m_stIndexerSpeedProfile.lDriveVelocity     = 600;
			m_stIndexerSpeedProfile.lMaxDriveVelocity  = 8000;
			m_stIndexerSpeedProfile.lAcc               = 50000;
			m_stIndexerSpeedProfile.lAccRatio          = 15625000;
		}
		break;

	default:
		break;
	}

	*pstIndexerSpeedProfile = m_stIndexerSpeedProfile;
}

void CTopTrack::GetIndexerStepperProfile(short nProfile, ST_STEPPER_SPEED *pstIndexerSpeedProfile)
{
	switch(nProfile)
	{
	case SRCH_SPEED_PROFILE:
		{
			pstIndexerSpeedProfile->Acc = 6000000;
			pstIndexerSpeedProfile->Dcc = 6000000;
			pstIndexerSpeedProfile->Vel = 150000;
			pstIndexerSpeedProfile->StopDcc = 20000000;
		}
		break;
	case SLOW_FORWARD_PROFILE:
		{
			pstIndexerSpeedProfile->Acc = 2000000 + 1500000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Dcc = 2000000 + 1500000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Vel = 30000 + 60000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->StopDcc = 80000000;
		}
		break;
	case FORWARD_PROFILE:
		{
			pstIndexerSpeedProfile->Acc = 2000000 + 1700000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Dcc = 2000000 + 1700000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Vel = 30000 + 67000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->StopDcc = 80000000;
		}
		break;
	case INJECT_SPEED_PROFILE:
		{
			pstIndexerSpeedProfile->Acc = 2000000 + 1500000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Dcc = 2000000 + 1500000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Vel = 30000 + 60000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->StopDcc = 80000000;
		}
		break;
	case BACKWARD_PROFILE:
		{
			pstIndexerSpeedProfile->Acc = 2000000 + 1700000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Dcc = 2000000 + 1700000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Vel = 30000 + 67000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->StopDcc = 80000000;

		}
		break;
	case INDEX_SPEED_PROFILE:
		{
			pstIndexerSpeedProfile->Acc = 2000000 + 2000000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Dcc = 2000000 + 2000000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->Vel = 30000 + 85000 * m_stTopTrackRcd.nIndexSpeedCoeff;
			pstIndexerSpeedProfile->StopDcc = 80000000;

		}
		break;
	case EJECT_SPEED_PROFILE:
		{
			pstIndexerSpeedProfile->Acc = 2000000 + 1200000 * m_stTopTrackRcd.nEjectSpeedCoeff;
			pstIndexerSpeedProfile->Dcc = 2000000 + 1200000 * m_stTopTrackRcd.nEjectSpeedCoeff;
			pstIndexerSpeedProfile->Vel = 30000 + 40000 * m_stTopTrackRcd.nEjectSpeedCoeff;
			pstIndexerSpeedProfile->StopDcc = 80000000;
		}
		break;
	case SLOW_SRCH_PROFILE:		//V1.5.W116 add, V1.5.W164 edit
		{
			int nSpeed=abs(theMcConfig.GetAlignSensorSlowSearchSpeed());

			pstIndexerSpeedProfile->Acc = 60000*nSpeed;
			pstIndexerSpeedProfile->Dcc = 60000*nSpeed;
			pstIndexerSpeedProfile->Vel = 1500*nSpeed;
			pstIndexerSpeedProfile->StopDcc = 200000*nSpeed;
		}
		break;
	default:
		break;
	}
}

int CTopTrack::CalcPrIndexOffset(long* loffsetXInUm)   
{
	PR_SRCH_RESULT stSrchResult[2];

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);
	if (stPrIndexRefRcd.bIndexPrAvail == false) return FNOK;

	*loffsetXInUm = 10000;
	theVisionSys.SetDrawWire(false);

	theServo.WaitTableStop();

	theServo.MoveTable2(stPrIndexRefRcd.stVisionAlignPos, true, false, false);	//v3.1.T208 edit
	theVisionSys.UpdateLightingParameter(
			stPrIndexRefRcd.stVisionLightPara.nCoaxRedLight,
			stPrIndexRefRcd.stVisionLightPara.bSideRedLight,
			stPrIndexRefRcd.stVisionLightPara.nCoaxBlueLight,
			stPrIndexRefRcd.stVisionLightPara.bSideBlueLight);
	theVisionSys.ChangeLensMag(stPrIndexRefRcd.nVisionAlignMag);

	theServo.WaitTableStop();
	theVisionSys.EnableVideo(false);
	theVisionSys.GrabImage();

	TOP_TRACK_DATA stTrackData;
	theTopTrack.GetRcd(&stTrackData);
	short nPrPassScore = stTrackData.sPRIndexScore;

	theVisionSys.SetPassScore(
				(short)stPrIndexRefRcd.ulVisionAlignPatternID,
				nPrPassScore);

	long lVisionError = theVisionSys.SearchVisionPattern(
					(short)stPrIndexRefRcd.ulVisionAlignPatternID,
					thePgm.GetLeadSearchRange(),
					nPrPassScore, &stSrchResult[0]);
	if (lVisionError == FOK)
	{
		TBL_POS stVisionOffset;

		theVisionSys.VisionOffsetToTablePos(stPrIndexRefRcd.nVisionAlignMag, &stSrchResult[0], &stVisionOffset);
		*loffsetXInUm = round((double)theServo.GetResolXY() * stVisionOffset.dx ); 
		theVisionSys.EnableVideo(true);

		if (m_nPRIState&&theDebug.IsDebugIndexAccuracy())	//V1.5.W123 edit
		{
			CString sInfo;
			sInfo.Format(m_nPRIState == 1 ? _T("OffsetBefore_%dum") : _T("After_%dum"), *loffsetXInUm);
			theDebug.AddInfo(sInfo);
		}
	}
	else
	{
		theVisionSys.EnableVideo(true);
		theTowerLight.EnableErrorLight();
		theTowerLight.EnableBlinkRedLight(true);
		m_pTargetClaw->SetRelayState(RELAY_OFF);
	}

	return lVisionError;
}

int CTopTrack::PerformManualAlignment(CRelay *pClaw)
{
	if (GetCurBondUnit() < m_stIndexerVar.nSrchUnit)
		m_LFHolder.SetRelayState(RELAY_ON);

	if (pClaw == NULL)
		m_pTargetClaw->SetRelayState(RELAY_ON);
	else
		pClaw->SetRelayState(RELAY_ON);

	CPopUpTopStepperCtr topStepperCtrl(theTopTrack.m_nIndexStepID);
	//before pop up dialog box, the correct parameter need to be passed in
	topStepperCtrl.Preset2ndCtrlID(TOP_WINDOW_CLAMPER);
	int lStopSelected = topStepperCtrl.DoModal() == IDOK ? SHOW_MSG_RESULT_OK : SHOW_MSG_RESULT_STOP;

	m_lOffsetx1um = 0;

	m_LFHolder.SetRelayState(RELAY_OFF);
	if (pClaw == NULL)
		m_pTargetClaw->SetRelayState(RELAY_OFF);
	else
		pClaw->SetRelayState(RELAY_OFF);

	return lStopSelected;
}

CRelay* CTopTrack::GetClaw()	//V1.5.W82 add
{
	long lClawPos=0;
	TOP_DEVICE_RECORD stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);

	//Input claw pos
	theStepper.GetPos(m_nIndexStepID, &lClawPos);
	lClawPos=round(lClawPos*theStepper.GetResol(m_nIndexStepID))-theMcData.GetZeroOffset()-INPUT_OUTPUT_CLAW_SPAN/2;

	//LF pos
	long lHeadPos=round((m_nCurBondUnit-0.5)*stDeviceRcd.nWndUnitNum*stDeviceRcd.lDeviceColmPitch)+stDeviceRcd.lDeviceEdgeLen;
	long lTailPos=lHeadPos-stDeviceRcd.lDeviceTotalLen;

	if(lClawPos>lTailPos&&lClawPos<lHeadPos)return &m_InputClaw;

	//Output claw pos
	lClawPos+=INPUT_OUTPUT_CLAW_SPAN;
	if(lClawPos>lTailPos&&lClawPos<lHeadPos)return &m_OutputClaw;

	return NULL;
}

void CTopTrack::PerformPrIndex(void)
{
	theMHS.SetOxidationProtect(true);
	m_InputClaw.SetRelayState(SENSOR_OFF);
	m_OutputClaw.SetRelayState(SENSOR_OFF);

	CStringList sList;
	sList.AddTail(_T("Index PR No Pattern"));
	sList.AddTail(theMsg.GetMsg(MSG_DO_MANUAL_ALIGN)); 
	long lStopSelected = theMsg.ShowMsg(ERROR_MSG, sList, true, true);

	theMHS.SetOxidationProtect(false);

	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableIdleLight();

	if (lStopSelected == SHOW_MSG_RESULT_STOP)
	{
		theVisionSys.EnableVideo(true);
		theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_STOP);
		theTowerLight.EnableBlinkRedLight(false);
		theTowerLight.EnableIdleLight();
	}
	else if (lStopSelected == SHOW_MSG_RESULT_OK)
	{
		theVisionSys.EnableVideo(true);
		theTowerLight.EnableBlinkRedLight(false);
		theTowerLight.EnableIdleLight();

		bool bJoyStickValid = theScanKeyThread.IsJoyStickValid();

		theScanKeyThread.EnableJoyStick();
		lStopSelected = theTopTrack.PerformManualAlignment(GetClaw());	//V1.5.W82 edit
		if (bJoyStickValid == false)
			theScanKeyThread.DisableJoyStick(); 

		if (lStopSelected != SHOW_MSG_RESULT_OK)
		{
			theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_STOP);
		}
		else
		{
			theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK);
			theTowerLight.EnableBlinkRedLight(false);
			theTowerLight.EnableBondingLight();
		}
	}
	else
	{
		theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK);
		m_lOffsetx1um = 0;

		theTowerLight.EnableBlinkRedLight(false);
		theTowerLight.EnableBondingLight();
	}
}

void CTopTrack::RequestOnload(void)
{
	if (m_InjectSensor.GetCurrentState() == SENSOR_OFF)
		theTopTrack.m_bRequestOnload = true;
}

short CTopTrack::GetNextLfState(void)
{
	return m_nNextLfState;
}

//************************************Debug*****************************************************
void CTopTrack::SubmitDryRunEvent(short nEvent)
{
	m_nTestTicker = 0;
	m_lTestDist[0] = 10000;		//um
	m_lTestDist[1] = 20000;		//um
	m_lTestDist[2] = 30000;		//um
	m_lTestDist[3] = 40000;		//um
	m_lTestDist[4] = 55000;		//um
	
	//ChangeStepperMotionProfile(0);

	m_nTestEvent = nEvent;

	if (m_nTestEvent == INDEX_TEST_EVENT_IDLE)
		m_nTestState = INDEX_TEST_STATE_IDLE;
	else
	{
		theServo.MoveBondHeadToSafePos();
		Sleep_x1ms(1000);

		HomeIndexer();
		Sleep_x1ms(1500);
	    m_nTestState = INDEX_TEST_STATE_WAIT_HOME;
	}
}

void CTopTrack::ProcessDryRunEvent(void)
{
	switch(m_nTestState)
	{
	case INDEX_TEST_STATE_WAIT_HOME:
	{
		if (theStepper.IsHome(m_nIndexStepID) == false) break;

		theStepper.MoveDist(m_nIndexStepID, theMcData.GetRcd()->lZeroOffset);
		m_nTestState = INDEX_TEST_STATE_WAIT_CENTER;
		break;
	}
	case INDEX_TEST_STATE_WAIT_CENTER:
	{
		if (theStepper.IsBusy(m_nIndexStepID)) break;
		if (theStepper.IsBusy(theTopClamp.GetStepID())) break;

		theStepper.MoveDist(m_nIndexStepID, -m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
		m_nTestState = INDEX_TEST_STATE_WAIT_LEFT;
		break;
	}
	case INDEX_TEST_STATE_WAIT_LEFT:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			theStepper.MoveDist(m_nIndexStepID, m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
			m_nTestState = INDEX_TEST_STATE_WAIT_LBACK;
		}
		break;
	}
	case INDEX_TEST_STATE_WAIT_LBACK:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			theStepper.MoveDist(m_nIndexStepID, m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
			m_nTestState = INDEX_TEST_STATE_WAIT_RIGHT;
		}
		break;
	}
	case INDEX_TEST_STATE_WAIT_RIGHT:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			theStepper.MoveDist(m_nIndexStepID, -m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
			m_nTestState = INDEX_TEST_STATE_WAIT_RBACK;
		}
		break;
	}
	case INDEX_TEST_STATE_WAIT_RBACK:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			if (m_nTestTicker == MAX_DEBUG_STEP - 1)
				nInc = -1;
			else if (m_nTestTicker == 0)
				nInc = 1;

			m_nTestTicker = m_nTestTicker + nInc;

			m_lStartTime = GetCurrentTime();
		    m_nTestState =INDEX_TEST_STATE_WAIT_CHANGE_PROFILE;
		}
		break;
	}
	case INDEX_TEST_STATE_WAIT_CHANGE_PROFILE:
	{
		if (GetCurrentTime() - m_lStartTime < 300) break;

		m_nTestState = INDEX_TEST_STATE_WAIT_WINCLAMP_OPEN;
		m_lStartTime = GetCurrentTime();
		theTopClamp.MoveClampSafePos();
		break;
	}
	case INDEX_TEST_STATE_WAIT_WINCLAMP_OPEN:
	{
		if (GetCurrentTime() - m_lStartTime < 300) break;

		m_nTestState = INDEX_TEST_STATE_WAIT_WINCLAMP_CLOSE;
		theTopClamp.CloseClamp();
		m_lStartTime = GetCurrentTime();
		break;
	}
	case INDEX_TEST_STATE_WAIT_WINCLAMP_CLOSE:
	{
		if (GetCurrentTime() - m_lStartTime < 500) break;

		m_nTestState = INDEX_TEST_STATE_WAIT_CENTER;
		break;
	}
	default:
		break;
	}
}

void CTopTrack::ProcessGripperDryRunEvent(void)
{
    switch (m_nGripperDryCycleState)
    {
    case CLAW_DRY_CYCLE_WAIT_CLAMP:
	{
		if (m_GripperDryRunTimer.GetTime_x1ms() < m_lClawInterval) break;

		m_Injector.SetRelayState(RELAY_ON);
		m_InputClaw.SetRelayState(TRUE);
		m_LFHolder.SetRelayState(RELAY_ON);
		m_OutputClaw.SetRelayState(TRUE);
		m_Ejector.SetRelayState(TRUE);

		m_GripperDryRunTimer.StartTimer();
		m_nGripperDryCycleState = CLAW_DRY_CYCLE_WAIT_OPEN;
        break;
	}
    case CLAW_DRY_CYCLE_WAIT_OPEN:
	{
		if (m_GripperDryRunTimer.GetTime_x1ms() < m_lClawInterval) break;

		m_Injector.SetRelayState(RELAY_OFF);
		m_InputClaw.SetRelayState(FALSE);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_OutputClaw.SetRelayState(FALSE);
		m_Ejector.SetRelayState(FALSE);

		m_GripperDryRunTimer.StartTimer();
		m_nGripperDryCycleState = CLAW_DRY_CYCLE_WAIT_CLAMP;
        break;
	}
	default:
        break;
    }
}

void CTopTrack::SubmitGripperDryRunEvent(short nEvent)
{
	m_GripperDryRunTimer.StartTimer();
	m_nGripperDryCycleState = CLAW_DRY_CYCLE_WAIT_CLAMP;
}

bool CTopTrack::IsTrackNeedClear(void)
{
	return m_bTrackNeedClear;
}

void CTopTrack::SetTrackNeedClear(bool bNeedClear)
{
	m_bTrackNeedClear = bNeedClear;
}

void CTopTrack::SetTailPos(long lOffset)
{
	m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipTailPos + lOffset;
}

short CTopTrack::GetLfInjectUnit(void)
{
	return m_stIndexerVar.nInjectUnit;
}

void CTopTrack::SetTeaching(bool bTeaching)
{
	m_bTeaching = bTeaching;
}

BYTE CTopTrack::InjectEnabled()
{
	return ((m_stIndexerVar.nTotalUnits - m_nCurBondUnit <= m_stTopTrackRcd.nNextStepPusherIn)
				&& (m_InjectSensor.GetCurrentState() == SENSOR_OFF));
}

bool CTopTrack::IsVacuumControl()
{
	return (m_stTopTrackRcd.bVacuumControl);
}

BYTE CTopTrack::IsClampReady()		//V1.5.W17 add
{
	if (m_bClampSensorEnabled == FALSE) return TRUE;
	if (m_TrackFrontHome.GetCurrentState() == SENSOR_ON) return TRUE;

	theTowerLight.EnableErrorLight();
	theTowerLight.EnableBlinkRedLight(true);

	short nMsg = MSG_WINDOW_CLAMP_NOT_CLOSE;
	theMsg.ShowMsg(ERROR_MSG, 1, &nMsg);

	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableBondingLight();

	return FALSE;
}

TOP_TRACK_DATA *CTopTrack::GetRcd(void)
{
	return &m_stTopTrackRcd;
}

//v3.0.T58 add
void CTopTrack::VerifyData(void)
{
	TOP_TRACK_DATA stTrackRcd;
	theDataMgr.CopyFromBuffer((char *)&stTrackRcd, sizeof(TOP_TRACK_DATA));

	short nShow = theMcConfig.GetVerifyShowTrack();
	if (nShow == 0) return;

	theLog.SetLogMode(LOG_VERIFY_RECIPE);

	CString sMsg; bool bDiff;
	theLog.WriteLine(_T("\n[WH TRACK]"));
	//1st LF heat delay
	sMsg.Format(_T("HeatDelay(1st LF)= %d"), m_stTopTrackRcd.lInitDelay);
	bDiff = false;
	if (stTrackRcd.lInitDelay != m_stTopTrackRcd.lInitDelay)
	{
		bDiff = true;
		sMsg.AppendFormat(_T(" (%d)"), stTrackRcd.lInitDelay);
	}
	if ((nShow==1) || ((nShow==2) && bDiff)) theLog.WriteLine(sMsg);
	//1st unit heat delay
	sMsg.Format(_T("HeatDelay(1st Unit)= %d"), m_stTopTrackRcd.l1stUnitHeatDelay);
	bDiff = false;
	if (stTrackRcd.l1stUnitHeatDelay != m_stTopTrackRcd.l1stUnitHeatDelay)
	{
		bDiff = true;
		sMsg.AppendFormat(_T(" (%d)"), stTrackRcd.l1stUnitHeatDelay);
	}
	if ((nShow==1) || ((nShow==2) && bDiff)) theLog.WriteLine(sMsg);
	//heat delay
	sMsg.Format(_T("HeatDelay= %d"), m_stTopTrackRcd.lHeatingDelay);
	bDiff = false;
	if (stTrackRcd.lHeatingDelay != m_stTopTrackRcd.lHeatingDelay)
	{
		bDiff = true;
		sMsg.AppendFormat(_T(" (%d)"), stTrackRcd.lHeatingDelay);
	}
	if ((nShow==1) || ((nShow==2) && bDiff)) theLog.WriteLine(sMsg);

	theLog.SetLogMode(LOG_MACHINE_DATA);
}

//v3.0.T373 add
byte CTopTrack::GetIndexStepID(void)
{
	return m_nIndexStepID;
}