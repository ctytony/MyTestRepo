/***************************************************************
TopEliteIndex.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "TopEliteIndex.h"
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

CTopEliteIndex theTEIndex;

// CTopEliteIndex
short gnInc;

CTopEliteIndex::CTopEliteIndex()
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
, m_nNextLfState(TE_NEXTLF_NONE)
{
	//in ms
	m_stDelayState.lDelay_x1ms = 0;
	m_stDelayState.lStartTime  = GetCurrentTime();
	m_stDelayState.cNextState  = TE_STATE_IDLE;
	m_stDelayState.cMoveMode   = 1;
	m_stDelayState.lMoveDist   = 0;

	ZeroMemory(m_stIndexRcd.lIndexError, sizeof(m_stIndexRcd.lIndexError[TOP_MAX_INDEX_TIME]));

	m_stIndexRcd.lLimitTolerance = 5000;
	m_stIndexRcd.lHeatingDelay = 0;
	m_stIndexRcd.nIndexSpeedCoeff = 4;
	m_stIndexRcd.nEjectSpeedCoeff = 2;
	m_stIndexRcd.lLfStandbyLen = 0;
	m_stIndexRcd.lSrchBackLen = 15000;
	m_stIndexRcd.l1stSlipHeatDelay = 0;		//ms
	m_stIndexRcd.nPrIndexEnable = false;
	m_stIndexRcd.lPrIndexTolerance = 0;
	m_stIndexRcd.lIndexPosOffset = 0;
	m_stIndexRcd.lOpenClampDelay = 0;
	m_stIndexRcd.lIndexPrDelay = 0;
    m_stIndexRcd.bVacuumControl = true;
	m_stIndexRcd.l1stUnitHeatDelay = 0;

	l1stIndexLen = m_stIndexVar.l1stIndexLen;
	m_stIndexRcd.nNextStepPusherIn = 2;
	m_bTeaching = false;
	m_bClampSensorEnabled = FALSE;
	m_nPRIState = 0;
}

CTopEliteIndex::~CTopEliteIndex()
{
}

// CTopEliteIndex member functions

void CTopEliteIndex::Init(void)
{
	m_nIndexStepID = DEVICE_INDEXER_EX;

	// Indexer stepper
	ST_AXIS_PARA stAxisPara;
	stAxisPara.Pulses = 1600;    
	stAxisPara.ScrewPitch = 25000;
	stAxisPara.Scale = stAxisPara.ScrewPitch / stAxisPara.Pulses;
	stAxisPara.Acc = 8000000;
	stAxisPara.Dcc = 8000000;
	stAxisPara.Vel = 300000;
	stAxisPara.StopDcc = 80000000;
	stAxisPara.FastHomeVel = 80000;
	stAxisPara.SlowHomeVel = 60000;
	stAxisPara.SlowHomeDistance = 10000;
	stAxisPara.HomeOffset = 2000;
	stAxisPara.LimitSignal = 1;
	stAxisPara.HomeSignal = 0;
	stAxisPara.HomeDirection = 0;
	theStepper.SetSpeedParameter(m_nIndexStepID, stAxisPara);
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
	m_TrackFrontHome.Initialize(theStepperIO.m_Input.m_TrackFrontHome.nPort);
	m_TrackBackHome.Initialize(theStepperIO.m_Input.m_TrackBackHome.nPort);

	m_bClampSensorEnabled = theMcConfig.IsWcCloseSensorExist()
							|| theMcConfig.IsWcOpenSensorExist();	//V1.5.W17
}

void CTopEliteIndex::HomeIndexer(void)
{
	//theStepper.DisableScurveProfile(m_nIndexStepID, &m_stIndexerSpeedProfile);
	Sleep_x1ms(10);
	theStepper.SearchHome(m_nIndexStepID);
}

void CTopEliteIndex::WaitHomeIndexer(void)
{
	CHTimer timer;
    do
	{
        Sleep_x1ms(10);
    } while ((timer.GetTime_x1ms() < STEP_MOTOR_MOTION_TIMEOUT)
				&& (theStepper.IsHome(m_nIndexStepID) == false));
}

int CTopEliteIndex::GetErrorCode(short nType)
{
	return (nType == 0 ? m_nErrorCode : m_nTrackStopReason);
}

void CTopEliteIndex::ResetErrorCode(short nType)
{
	if (nType == 0)
		m_nErrorCode = 0;
	else
		m_nTrackStopReason = 0;
}

bool CTopEliteIndex::IsValid(void)
{
	return m_bValid;
}

void CTopEliteIndex::SetRunNoLf(bool bRunNoLf)
{
	m_bRunNoLf = bRunNoLf;
}

bool CTopEliteIndex::IsRunNoLf(void)
{
	return m_bRunNoLf;
}

short CTopEliteIndex::GetIndexPrManualAlign(void)
{
	return m_nIndexPrManualAlign;
}

void CTopEliteIndex::SetIndexPrManualAlign(short nAlign)
{
	m_nIndexPrManualAlign = nAlign;
}

void CTopEliteIndex::ResetRelayErrorState(void)
{
	m_Injector.SetRelayState(RELAY_OFF);
	m_InputClaw.SetRelayState(RELAY_OFF);
	m_OutputClaw.SetRelayState(RELAY_OFF);
	m_Ejector.SetRelayState(RELAY_OFF);
	m_LFHolder.SetRelayState(RELAY_OFF);
}

void CTopEliteIndex::MoveIndexerToLogicZero(void)
{
	SetStepSpeed(TE_SPEED_SLOW_FORWARD);

	long lMoveDist = theMcData.GetZeroOffset() <= 0 ? 50000 : theMcData.GetZeroOffset();
	theStepper.MoveDist(m_nIndexStepID, lMoveDist);
}

void CTopEliteIndex::WaitMoveIndexer(void)
{
	long lPos;
	long lMoveDist = theMcData.GetZeroOffset() <= 0 ? 50000 : theMcData.GetZeroOffset();

	CHTimer timer;
	do
	{
		lPos = theStepper.GetPos(m_nIndexStepID);
		if (round((double)lMoveDist / theStepper.GetResol(m_nIndexStepID)) == lPos)
			break;

		Sleep_x1ms(10);
	} while (timer.GetTime_x1ms() < 5000);
}

bool CTopEliteIndex::IsPrIndexMask(void)
{
	return m_bPrIndexMask;
}

void CTopEliteIndex::SetPrIndexMask(bool bMask)
{
	m_bPrIndexMask = bMask;
}

void CTopEliteIndex::SetValid(bool bValid)
{
	m_bValid = bValid;
}

void CTopEliteIndex::SaveRcd(void)
{
	theDataMgr.CopyToBuffer((char *)&m_stIndexRcd, sizeof(m_stIndexRcd));
}

void CTopEliteIndex::LoadRcd(void)
{
	theDataMgr.CopyFromBuffer((char *)&m_stIndexRcd, sizeof(m_stIndexRcd));
}

void CTopEliteIndex::SetRcd(ST_TE_INDEX_RCD *pstTopIndex)
{
	m_stIndexRcd = *pstTopIndex;
}

short CTopEliteIndex::GetCurBondUnit(void)
{
	return m_nCurBondUnit;
}

void CTopEliteIndex::SetCurBondUnit(short nUnit)
{
	m_nCurBondUnit = nUnit;
}

short CTopEliteIndex::GetEvent(void)
{
	return m_nEvent;
}

void CTopEliteIndex::SetOffloadEnable(bool bEnable)
{
	m_bOffloadEnable = bEnable;
}

bool CTopEliteIndex::IsOffloadEnable(void)
{
	return m_bOffloadEnable;
}

long CTopEliteIndex::LogicToPhyPos(long lLogicPos)
{
	return (lLogicPos + theMcData.GetZeroOffset());
}

//always call this function when one continuous process finished
void CTopEliteIndex::LeftMoveArray(void)
{
	*(INT64*)m_acStateSeq /= 256;
}

// use this function only when reset and clear track in condition 
void CTopEliteIndex::ResetSlipRcd()
{
	m_stSlipOnTrack.bSlipSrchDone       = false;
	m_stSlipOnTrack.lSlipHeaderPos      = -CENTER_TO_LEFT_EDGE;
	m_stSlipOnTrack.lSlipTailPos        = -CENTER_TO_LEFT_EDGE;
	m_stSlipOnTrack.lSlipOutputTailPos  = CENTER_TO_LEFT_EDGE;
	m_stSlipOnTrack.bCompenDone         = false;

	SetCurBondUnit(0);

	m_cState = TE_STATE_IDLE;
	m_nEvent = TE_EVENT_IDLE;

	m_nNextLfState = TE_NEXTLF_NONE;

	m_stDelayState.lDelay_x1ms = 0;
	m_stDelayState.lStartTime  = GetCurrentTime();
	m_stDelayState.cNextState  = TE_STATE_IDLE;
	m_stDelayState.cMoveMode   = 1;
	m_stDelayState.lMoveDist   = 0;
}

void CTopEliteIndex::ResetEvent(void)
{
	m_cState = TE_STATE_IDLE;
	m_nEvent = TE_EVENT_IDLE;
}

void CTopEliteIndex::RefreshMeasurementData(const TOP_DEVICE_RECORD *pstDevice)
{
	if (pstDevice->nWndUnitNum == 0)
	{
		AfxMessageBox(_T("temp.40, nWndUnitNum == 0"));
		return;
	}

	//device related:
	if (pstDevice->nDeviceColmNum % pstDevice->nWndUnitNum != 0)
		m_stIndexVar.nTotalUnits = pstDevice->nDeviceColmNum
									/ pstDevice->nWndUnitNum + 1;
	else
		m_stIndexVar.nTotalUnits = pstDevice->nDeviceColmNum
									/ pstDevice->nWndUnitNum;

	m_stIndexVar.lLfTotalLen = pstDevice->lDeviceTotalLen;
	m_stIndexVar.lNorIndexLen = pstDevice->lDeviceColmPitch
									* pstDevice->nWndUnitNum;

	//Index profile calculate:

	switch (pstDevice->nDeviceId)
	{
	case ID_DIC_0603:
	{
		m_stIndexVar.nInjectUnit  = 1;
		m_stIndexVar.nSrchUnit    = m_stIndexVar.nTotalUnits - 6;	//12
		m_stIndexVar.nCatchUpUnit = m_stIndexVar.nTotalUnits - 5;	//13
		m_stIndexVar.nCompEnable  = TRUE;
		break;
	}
	case ID_PCB_LONG:
	{
		m_stIndexVar.nInjectUnit  = m_stIndexVar.nTotalUnits - 2;
		m_stIndexVar.nSrchUnit    = m_stIndexVar.nTotalUnits - 1;    
		m_stIndexVar.nCatchUpUnit = 0;
		m_stIndexVar.nCompEnable  = TRUE;
		break;
	}
	case ID_10W_HIGHPWR:
	{
		m_stIndexVar.nCompEnable  = FALSE;
		m_stIndexVar.nInjectUnit  = 1;
		m_stIndexVar.nSrchUnit    = 2;
		m_stIndexVar.nCatchUpUnit = 0;
		break;
	}
	case ID_100W_HIGHPWR:
	{
		m_stIndexVar.nCompEnable  = FALSE;
		m_stIndexVar.nInjectUnit  = 0;
		m_stIndexVar.nSrchUnit    = 0;
		m_stIndexVar.nCatchUpUnit = 0;
		break;
	}
	case ID_SOP16:
	{
		m_stIndexVar.nInjectUnit  = m_stIndexVar.nTotalUnits - 2;
		m_stIndexVar.nSrchUnit    = m_stIndexVar.nTotalUnits - 1;    
		m_stIndexVar.nCatchUpUnit = 0;
		m_stIndexVar.nCompEnable  = TRUE;
		break;
	}
	case ID_SOP7_1C:
	{
		m_stIndexVar.nInjectUnit  = m_stIndexVar.nTotalUnits - 5;
		m_stIndexVar.nSrchUnit    = m_stIndexVar.nTotalUnits - 4;
		m_stIndexVar.nCatchUpUnit = m_stIndexVar.nTotalUnits - 3;
		m_stIndexVar.nCompEnable  = TRUE;
		break;
	}
	case ID_SOP7_2C:
	{
		m_stIndexVar.nInjectUnit  = m_stIndexVar.nTotalUnits - 3;
		m_stIndexVar.nSrchUnit    = m_stIndexVar.nTotalUnits - 2;
		m_stIndexVar.nCatchUpUnit = 0;
		m_stIndexVar.nCompEnable  = FALSE;
		break;
	}
	case ID_CEDAR:
	{
		m_stIndexVar.nInjectUnit  = 1;
		m_stIndexVar.nSrchUnit    = 2;
		m_stIndexVar.nCatchUpUnit = 0;
		m_stIndexVar.nCompEnable  = FALSE;
		break;
	}
	default:
	{
		if (m_stIndexVar.nTotalUnits <= 3)
		{
			m_stIndexVar.nCompEnable  = TRUE;
			m_stIndexVar.nInjectUnit  = 1;
			m_stIndexVar.nSrchUnit    = 2;
			m_stIndexVar.nCatchUpUnit = 0;
		}
		else
		{
			m_stIndexVar.nInjectUnit = 1;
			for (int i=1; i<m_stIndexVar.nTotalUnits-2; i++)
			{
				if (m_stIndexVar.lNorIndexLen * (m_stIndexVar.nTotalUnits + 0.5 - i) < CENTER_TO_ALIGN_SENSOR)
				{
					if (i < m_stIndexVar.nTotalUnits - 2)
					{
						m_stIndexVar.nInjectUnit = i;
						break;
					}
				}
				else
					m_stIndexVar.nInjectUnit = 1;
			}

			m_stIndexVar.nSrchUnit    = m_stIndexVar.nTotalUnits-2;   //2nd last 1
			m_stIndexVar.nCatchUpUnit = m_stIndexVar.nTotalUnits-1;   //1st last 1
			m_stIndexVar.nCompEnable  = TRUE;
		}
		break;
	}
	}

	//left/right limit position based on the calibration
	m_stIndexVar.lRightLimitPos = theMcData.GetIndexerLimit() - theMcData.GetZeroOffset() - m_stIndexRcd.lLimitTolerance;
    m_stIndexVar.lLeftLimitPos  = -theMcData.GetZeroOffset();

//Length-------------------------------------------------------------------------------
	//(tailedge + ClampPos) or (headerEdge - ClampPos)
	
	//the center of the slip unit is aligning with the heat block
	//m_stIndexVar.l1stIndexLen  =round((double) CENTER_TO_ALIGN_SENSOR -m_stIndexRcd.lLfStandbyLen + (pstTopDevicePara->lDeviceEdgeLen + pstTopDevicePara->lDeviceColmPitch *(pstTopDevicePara->nWndUnitNum - 0.5))/2);

	if (m_stIndexVar.nCatchUpUnit == 0)
		m_stIndexVar.l1stIndexLen = m_stIndexRcd.lLfStandbyLen + (pstDevice->lDeviceColmPitch * pstDevice->nWndUnitNum) + pstDevice->lDeviceEdgeLen * 2;
	else
		m_stIndexVar.l1stIndexLen = m_stIndexRcd.lLfStandbyLen + (pstDevice->lDeviceColmPitch * pstDevice->nWndUnitNum) * (m_stIndexVar.nTotalUnits - m_stIndexVar.nCatchUpUnit + 1) + pstDevice->lDeviceEdgeLen * 2;

	m_stIndexVar.lCatchUpLen =  m_stIndexVar.lNorIndexLen + m_stIndexRcd.lIndexError[m_nCurBondUnit];

	//Action Limit
	m_stIndexVar.lEjectStartPos = m_stIndexVar.lLeftLimitPos;
	m_stIndexVar.lSrchStartPos  = m_stIndexVar.lLeftLimitPos;
	
	// Injection Profile
	m_stIndexVar.lInjectStart[0]  = m_stIndexVar.lLeftLimitPos + 5000 ;
	m_stIndexVar.lInjectStart[1]  = m_stIndexVar.lLeftLimitPos;
	m_stIndexVar.lInjectStart[2]  = m_stIndexVar.lLeftLimitPos;
	m_stIndexVar.lInjectEnd[0] = m_stIndexVar.lInjectStart[0] + 22000;
	m_stIndexVar.lInjectEnd[1] = m_stIndexVar.lInjectStart[1] + 30000;
	m_stIndexVar.lInjectEnd[2] = m_stIndexVar.lInjectStart[2] + 30000;
}

void CTopEliteIndex::SubmitClearTrackEvent(void)
{
	memset(m_acStateSeq, 0, sizeof(m_acStateSeq));
	char cClear[TOTAL_STEP] = {TE_STATE_CLEAR_TRACK, TE_STATE_CLEAR_TRACK_START_1ST, TE_STATE_CLAW_POS_RESET, TE_STATE_CLEAR_TRACK_START_2ND, TE_STATE_CLAW_POS_RESET, TE_STATE_IDLE};

	if (m_nCurBondUnit == 0)
	{
		if (m_AlignSensor.GetCurrentState() == TRUE)
		{
			m_stSlipOnTrack.lSlipHeaderPos = -CENTER_TO_ALIGN_SENSOR;
		}

		m_stSlipOnTrack.lSlipTailPos = -CENTER_TO_ALIGN_SENSOR;
		memcpy(m_acStateSeq, cClear, TOTAL_STEP);
		m_nEvent = TE_EVENT_CLEAR_TRACK_FIRST;
		m_cState = m_acStateSeq[0];
	}
	else
	{
		memcpy(m_acStateSeq, cClear, TOTAL_STEP);
		m_nEvent = TE_EVENT_CLEAR_TRACK_NORMAL;
		m_cState = m_acStateSeq[0];
	}

	LeftMoveArray();
}

void CTopEliteIndex::SubmitIndexEvent(short nEvent)		//verified 2019.11.12
{
	memset(m_acStateSeq, TE_STATE_IDLE, sizeof(m_acStateSeq));
	m_nEvent = TE_EVENT_IDLE;

	char acIndex[TOTAL_STEP]  = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
									TE_STATE_CLAW_STANDBY, TE_STATE_IDLE,
									TE_STATE_IDLE, TE_STATE_IDLE};

	char acInitIndex[TOTAL_STEP] = {TE_STATE_INJECT, TE_STATE_ROUGH_SRCH,
									TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
									TE_STATE_CLAW_STANDBY, TE_STATE_IDLE };

	char acNewSlip[TOTAL_STEP]   = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
									TE_STATE_OUTPUT, TE_STATE_CLAW_STANDBY,
									TE_STATE_IDLE,TE_STATE_IDLE};

    // Inject, compens(multiple), srch, standby (maximum 4 steps before index)
	char acIndexInject[TOTAL_STEP] = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
									TE_STATE_INJECT, TE_STATE_CLAW_STANDBY,
									TE_STATE_IDLE, TE_STATE_IDLE};

	char acIndexInjectCompen[TOTAL_STEP] = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
											TE_STATE_INJECT, TE_STATE_COMPEN,
											TE_STATE_CLAW_STANDBY, TE_STATE_IDLE};

	char acCompen[TOTAL_STEP] = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
									TE_STATE_COMPEN, TE_STATE_CLAW_STANDBY,
									TE_STATE_IDLE, TE_STATE_IDLE};

	char acLfLocating[TOTAL_STEP] = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
									TE_STATE_ROUGH_SRCH, TE_STATE_CLAW_STANDBY,
									TE_STATE_IDLE, TE_STATE_IDLE};

	char acCatchUp[TOTAL_STEP] = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
									TE_STATE_CLAW_STANDBY, TE_STATE_CATCH_UP,
									TE_STATE_CLAW_STANDBY, TE_STATE_IDLE};

	char acNewSlipInject[TOTAL_STEP] = {TE_STATE_CLAW_STANDBY, TE_STATE_INDEX,
										TE_STATE_OUTPUT, TE_STATE_INJECT,
										TE_STATE_ROUGH_SRCH,TE_STATE_CLAW_STANDBY};

	if (nEvent == TE_EVENT_STANDBY_NEXT_LF)
	{
		m_nEvent = TE_EVENT_STANDBY_NEXT_LF;

		short i = 0;
		if ((m_nCurBondUnit > m_stIndexVar.nInjectUnit) && (m_nNextLfState < TE_NEXTLF_AFTER_SRCH))
		{
			m_acStateSeq[i++] = TE_STATE_INJECT;
            if (m_stIndexVar.nTotalUnits <= 3)
              	m_acStateSeq[i++] = TE_STATE_COMPEN;
		}

		if ((m_nCurBondUnit > m_stIndexVar.nSrchUnit)
			&& (m_nNextLfState < TE_NEXTLF_AFTER_SRCH))
		{
			m_acStateSeq[i++] = TE_STATE_ROUGH_SRCH;
		}

		if ((m_stIndexVar.nCatchUpUnit != 0)
			&& (m_nCurBondUnit > m_stIndexVar.nCatchUpUnit)
			&& (m_nNextLfState < TE_NEXTLF_AFTER_CATCH_UP))
		{
			m_acStateSeq[i++] = TE_STATE_CLAW_STANDBY;
			m_acStateSeq[i++] = TE_STATE_CATCH_UP;
		}
		m_acStateSeq[i] = TE_STATE_CLAW_STANDBY;
	    m_cState = m_acStateSeq[0];
	}
	//maximum steps: Inject ->compens(multiple) -> srch ->catch up @before index
    //minimum steps: Inject(with comp) -> srch @before index
	else
	{
		if (m_nCurBondUnit == 0)  // 1st index event
		{		
			m_nNextLfState = TE_NEXTLF_NONE;
			m_nEvent = TE_EVENT_INITIAL_INDEX;
			memcpy(m_acStateSeq, acInitIndex, TOTAL_STEP);
			m_cState = m_acStateSeq[0];
		}
		else if (m_nCurBondUnit == m_stIndexVar.nTotalUnits)
		{
			if (m_stIndexVar.nTotalUnits > 3)
			{
				memcpy(m_acStateSeq, acNewSlip, TOTAL_STEP);

                if (m_stIndexVar.nCatchUpUnit != 0)
                {
				    if (m_nNextLfState < TE_NEXTLF_AFTER_CATCH_UP)
					    theMHS.SetLastLf(true);
                }
                else
                {
                    if (m_nNextLfState < TE_NEXTLF_AFTER_SRCH)
					    theMHS.SetLastLf(true);
                }
			}
			else  // =3 or 2 index times(no compen and catch up)
			{
				if (m_stIndexVar.nTotalUnits <= 2)
					memcpy(m_acStateSeq, acNewSlipInject, TOTAL_STEP);
				else 
					memcpy(m_acStateSeq, acNewSlip, TOTAL_STEP);

				if (m_nNextLfState != TE_NEXTLF_AFTER_SRCH)
					theMHS.SetLastLf(true);
			}

			m_cState = m_acStateSeq[0];
			m_nEvent = TE_EVENT_NEW_SLIP;
		}
//************************************flexible profile*************************
		else if (m_nCurBondUnit == m_stIndexVar.nInjectUnit)
		{
			m_nEvent = TE_EVENT_1ST_UNIT;
			if ((theMHS.IsLastLf() == true)
				|| (nEvent == TE_EVENT_SINGLE_INDEX))
				memcpy(m_acStateSeq, acIndex, TOTAL_STEP);
			else
				memcpy(m_acStateSeq, acIndexInjectCompen, TOTAL_STEP);
			m_cState = m_acStateSeq[0];
		}
		else if (m_nCurBondUnit == m_stIndexVar.nSrchUnit)
		{
			m_nEvent = TE_EVENT_UNIT_STANDBY;
			if (m_nNextLfState == TE_NEXTLF_AFTER_SRCH)
				memcpy(m_acStateSeq, acLfLocating, TOTAL_STEP);
			else
				memcpy(m_acStateSeq, acIndex, TOTAL_STEP);
			m_cState = m_acStateSeq[0];
		}
		else if (m_nCurBondUnit == m_stIndexVar.nCatchUpUnit)
		{
			m_nEvent = TE_EVENT_UNIT_STANDBY;
			if (m_nNextLfState == TE_NEXTLF_AFTER_SRCH)
				memcpy(m_acStateSeq, acCatchUp, TOTAL_STEP);
			else
				memcpy(m_acStateSeq, acIndex, TOTAL_STEP);
			m_cState = m_acStateSeq[0];
		}
		else
		{
			m_nEvent = TE_EVENT_NOTHING_MORE;
			memcpy(m_acStateSeq, acIndex, TOTAL_STEP);
			m_cState = m_acStateSeq[0];
		}
	}

	LeftMoveArray();
}

void CTopEliteIndex::ProcessIndexEvent(void)
{
	switch(m_cState)
	{
	case TE_STATE_IDLE:		//verified 2019.11.11
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-Idle"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_nEvent = TE_EVENT_IDLE;
		break;
	}
//1~5  Start Injection of new device
    case TE_STATE_INJECT:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-Inject"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_cState = TE_STATE_WAIT_ELEV;	
		break;
	}
	case TE_STATE_WAIT_ELEV:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitElev"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_lInjectStartTime = GetCurrentTime();
		m_cState = TE_STATE_INJECT_CHECK_ONLOAD;
		break;
	}
	case TE_STATE_INJECT_CHECK_ONLOAD:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectCheckOnload"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theTEIndex.m_bRequestOnload == false)
		{
			m_stIndexVar.m_iInjectTimes = 0;
			m_cState = TE_STATE_INJECT_START;
		}
		else
		{
			if (GetCurrentTime() - m_lInjectStartTime > ELEV_TIMEOUT)  //22s
			{
                // Elevator onload retry
				m_stIndexVar.m_iOnloadRetryTimes = 0;
				m_nErrorCode = MHS_TOP_ONLOADER_NO_RESPONSE;
				//theMHS.SetLastLf(true);
				m_cState = m_acStateSeq[0];    
    		    LeftMoveArray();
			}
		}
		break;
	}
	case TE_STATE_INJECT_START:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (m_InjectSensor.GetCurrentState() == FALSE) //No Lead Frame or other error
		{
			if (m_nEvent == TE_EVENT_INITIAL_INDEX)  
			{
				m_nErrorCode = MHS_TOP_ONLOAD_NO_LF;
				do 
				{
					LeftMoveArray();
					m_cState = m_acStateSeq[0];
				} while (m_acStateSeq[0] != TE_STATE_IDLE);
			}
			else
			{
				m_nTrackStopReason = MHS_TOP_ONLOAD_NO_LF;
				theMHS.SetLastLf(true);
				do 
				{
					LeftMoveArray();
					m_cState = m_acStateSeq[0];
				} while (m_acStateSeq[0] != TE_STATE_CLAW_STANDBY && m_acStateSeq[0] != TE_STATE_IDLE);
			}
		}
		else
        {
			SetStepSpeed(TE_SPEED_INJECT);
			m_stIndexVar.m_lInjectStartPos = m_stIndexVar.lLeftLimitPos ;
			theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(m_stIndexVar.m_lInjectStartPos));	//v3.0.T374 edit
			m_cState = TE_STATE_INJECT_WAIT_AT_START;
        }
		break;
	}
	case TE_STATE_INJECT_WAIT_AT_START:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectWaitStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

        if (theStepper.IsBusy(m_nIndexStepID)) break;

		// clamp left claw
		m_Injector.SetRelayState(RELAY_ON);

		// Lead frame holder Open
		m_LFHolder.SetRelayState(RELAY_ON);

		SetStepSpeed(TE_SPEED_SLOW_FORWARD);
		if (theMcConfig.GetOnloadType() == TOP_WITH_PUSH_KICKER)
		m_stIndexVar.m_lInjectEndPos = m_stIndexVar.m_lInjectStartPos + 50000;

		EnterDelayState(TE_STATE_INJECT_WAIT_SENSOR, GetCurrentTime(), SMALL_RELAY_DELAY,
			round((double)LogicToPhyPos(m_stIndexVar.m_lInjectEndPos) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit

		break;
	}
	case TE_STATE_INJECT_WAIT_SENSOR:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectWaitSensor"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		m_Injector.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_cState = TE_STATE_INJECT_WAIT_INJECT_DEST2;
		break;
	}
	case TE_STATE_INJECT_WAIT_INJECT_DEST2:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-InjectWaitDest2"), m_cState);
			m_nLastDebugState = m_cState;
		}

        if (theStepper.IsBusy(m_nIndexStepID)) break;

		//OPEN INJECTOR, LF holder close
		m_Injector.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);

		if (m_nEvent != TE_EVENT_INITIAL_INDEX)
			m_nNextLfState = TE_NEXTLF_AFTER_SRCH;

		EnterDelayState(m_acStateSeq[0], RELAY_DELAY, 0, 0); 
		LeftMoveArray();
        break;
	}
//6~8 --28  Compensate to let standby device catch up with bonding one
	case TE_STATE_COMPEN: 
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevComp"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		SetStepSpeed(TE_SPEED_BACKWARD);
		theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(m_stIndexVar.lSrchStartPos));
		m_cState = TE_STATE_COMPEN_WAIT_AT_START;
		break;
	}
	case TE_STATE_COMPEN_WAIT_AT_START:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-CompWaitStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		SetStepSpeed(TE_SPEED_SLOW_FORWARD);
		m_InputClaw.SetRelayState(RELAY_ON);
		m_LFHolder.SetRelayState(RELAY_ON);
		EnterDelayState(TE_STATE_COMPEN_WAIT_DEST, RELAY_ACTION_DELAY,
			round(m_stIndexRcd.nLFWaitPos * 1000 / theStepper.GetResol(m_nIndexStepID)), 0);
		break;
	}
	case TE_STATE_COMPEN_WAIT_DEST:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-CompWaitDest"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			m_InputClaw.SetRelayState(RELAY_OFF);
			m_LFHolder.SetRelayState(RELAY_OFF);
			m_stSlipOnTrack.bCompenDone = true;

		    EnterDelayState(m_acStateSeq[0], RELAY_DELAY, 0, 0);   
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

					EnterDelayState(m_acStateSeq[0], RELAY_DELAY, 0, 0);   

					LeftMoveArray();
				}
			}
		}
		break;
	}
//**********************************Catch Up after Align****************************************************
	case TE_STATE_CATCH_UP:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-CatchUp"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		SetStepSpeed(TE_SPEED_SLOW_FORWARD);
		m_InputClaw.SetRelayState(RELAY_ON);
		m_LFHolder.SetRelayState(RELAY_ON);

		//catch-up update
		m_stIndexVar.lCatchUpLen = m_stIndexVar.lNorIndexLen + m_stIndexRcd.lIndexError[m_nCurBondUnit];
		EnterDelayState(TE_STATE_WAIT_STANDBY_POS, RELAY_ACTION_DELAY,
			round((double)m_stIndexVar.lCatchUpLen / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
		break;
	}
	case TE_STATE_WAIT_STANDBY_POS: 
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitStandbyPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		if (m_nEvent != TE_EVENT_INITIAL_INDEX)
			m_nNextLfState = TE_NEXTLF_AFTER_CATCH_UP;

		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_stSlipOnTrack.lSlipHeaderPos = m_stSlipOnTrack.lSlipHeaderPos + m_stIndexVar.lCatchUpLen;

		SetStepSpeed(TE_SPEED_INDEX);
		EnterDelayState(m_acStateSeq[0], RELAY_DELAY, 0, 0);   
		LeftMoveArray();
		break;
	}
//**********************************SEARCH LOCATION SENSOR***********************************************************
//9~12  Srch the location Sensor and ready for index: 
	case TE_STATE_ROUGH_SRCH:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrch"), m_cState);
			m_nLastDebugState = m_cState;
		}

		SetStepSpeed(TE_SPEED_BACKWARD);
		if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
			theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(m_stIndexVar.lSrchStartPos));	//v3.0.T374 edit
		else
			MoveIndexerToLogicZero();

        m_cState = TE_STATE_ROUGH_SRCH_WAIT_AT_START;
		break;
	}
	case TE_STATE_ROUGH_SRCH_WAIT_AT_START:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchWaitStart"), m_cState);
			m_nLastDebugState = m_cState;
		}

		long lAbsPos;
		if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
			lAbsPos = LogicToPhyPos(m_stIndexVar.lSrchStartPos);
		else
			lAbsPos = LogicToPhyPos(0);
        lAbsPos = round((double)lAbsPos / theStepper.GetResol(m_nIndexStepID));
        if (theStepper.GetPos(m_nIndexStepID, FALSE) == lAbsPos)
        {
            //clamp left claw
            SetStepSpeed(TE_SPEED_SRCH);

            if (theTEIndex.IsRunNoLf() == false)
            {
				if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
				{
					m_InputClaw.SetRelayState(RELAY_ON);
					m_LFHolder.SetRelayState(RELAY_ON);
					theStepper.MoveStepSearch(m_nIndexStepID, 1);

					EnterDelayState(TE_STATE_ROUGH_SRCH_WAIT_ROUGH_REACH, RELAY_ACTION_DELAY,
						round(90000 / theStepperIO.GetResolution(m_nIndexStepID)), 0);	//v3.0.T372 add
				}
				else
				{
					theStepper.MoveStepSearch(m_nIndexStepID, 1);
					EnterDelayState(TE_STATE_ROUGH_SRCH_WAIT_ROUGH_REACH, GetCurrentTime(), RELAY_ACTION_DELAY, 0);
				}
            }
            else
            {
                m_InputClaw.SetRelayState(RELAY_ON);
				m_LFHolder.SetRelayState(RELAY_ON);
                EnterDelayState(TE_STATE_ROUGH_SRCH_WAIT_ROUGH_REACH, RELAY_ACTION_DELAY,
					round((double)m_stIndexRcd.lSrchBackLen / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
            }
        }
        break;
	}
	case TE_STATE_ROUGH_SRCH_WAIT_ROUGH_REACH:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchWaitReach"), m_cState);
			m_nLastDebugState = m_cState;
		}

        //stop, start srch back  RECHECK_SENSOR_BACK_DIST
        if (theTEIndex.IsRunNoLf() == false)
        {
            if (m_AlignSensor.GetCurrentState() == TRUE)
            {
                theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);

				int m_nSrchDist = abs(theMcConfig.GetAlignSensorSlowSearchDist_x1um());

				if ((theMcConfig.GetAlignSensorSlowSearchSpeed() == 0)
					|| (theMcConfig.GetAlignSensorSlowSearchDist_x1um() == 0))
					EnterDelayState(TE_STATE_FINE_SRCH_WAIT_STANDBY, 100, 0, 0);
				else if (theMcConfig.GetAlignSensorSlowSearchSpeed() > 0)
					EnterDelayState(TE_STATE_INDEX_SLOW_SRCH, 100, m_nSrchDist, 0);
				else
				{
					SetStepSpeed(TE_SPEED_SLOW_SRCH);
					EnterDelayState(TE_STATE_INDEX_SLOW_SRCH_BACK, 100, -m_nSrchDist, 0);  //v1.6.P180 edit
				}
            }
            else
            {
                //error handling:
                if (theStepper.IsBusy(m_nIndexStepID) == false)
                {
                    m_InputClaw.SetRelayState(RELAY_OFF);
					m_LFHolder.SetRelayState(RELAY_OFF);
                    if (m_nEvent == TE_EVENT_INITIAL_INDEX)  
                    {
                        m_nErrorCode = MHS_TOP_SRCH_TIME_OUT;
                        m_cState = TE_STATE_INDEX_STOP;

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
                        m_cState = m_acStateSeq[0];    
                        LeftMoveArray(); 
                    }
                }
            }
        }
        else
        {
            if (theStepper.IsBusy(m_nIndexStepID) == false)
            {
                EnterDelayState(TE_STATE_ROUGH_SRCH_WAIT_BACK_REDO, RELAY_DELAY,
					-round((double)m_stIndexRcd.lSrchBackLen / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
            }
        }
        break;
	}
	case TE_STATE_ROUGH_SRCH_WAIT_BACK_REDO:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchWaitRedo"), m_cState);
			m_nLastDebugState = m_cState;
		}

		//check pos, srch again
		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_stSlipOnTrack.lSlipHeaderPos = -CENTER_TO_ALIGN_SENSOR - m_stIndexRcd.lSrchBackLen;
		EnterDelayState(TE_STATE_ROUGH_SRCH_BACK, RELAY_DELAY, 0, 0);
		break;
	}
// second time srch start here--------->>>>
	case TE_STATE_ROUGH_SRCH_BACK:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughSrchBack"), m_cState);
			m_nLastDebugState = m_cState;
		}

		SetStepSpeed(TE_SPEED_BACKWARD);
		theStepper.MoveStep(m_nIndexStepID, LogicToPhyPos(m_stIndexVar.lSrchStartPos));
		m_cState = TE_STATE_ROUGH_WAIT_SRCH_BACK; 
		break;
	}
	case TE_STATE_ROUGH_WAIT_SRCH_BACK:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-RoughWaitSrchBack"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_InputClaw.SetRelayState(RELAY_ON);
		m_LFHolder.SetRelayState(RELAY_ON);
		SetStepSpeed(TE_SPEED_SRCH);

		long lPos;
		if (theTEIndex.IsRunNoLf() == false)
			lPos = round((double)90000 / theStepper.GetResol(m_nIndexStepID));
		else
			lPos = round((double)m_stIndexRcd.lSrchBackLen / theStepper.GetResol(m_nIndexStepID));
		EnterDelayState(TE_STATE_FINE_SRCH_WAIT_STANDBY, RELAY_DELAY, lPos, 0);
        break;
	}
//15~16 -----------for fine search--------------------------
	case TE_STATE_FINE_SRCH_WAIT_STANDBY:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-FineSrchWaitStandby"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		SetStepSpeed(TE_SPEED_SLOW_FORWARD); 
		m_stSlipOnTrack.lSlipHeaderPos = -CENTER_TO_ALIGN_SENSOR;

		long pos, searchpos;
		theStepperIO.CheckStepMotionSearchDis(m_nIndexStepID, &searchpos);
		pos = theStepper.GetPos(m_nIndexStepID);
		theStepper.MoveStepSearch(m_nIndexStepID, 0);

		if (theMcConfig.GetAlignSensorType() == BACK_ALIGN)
		{
			if (pos > 0)
			{
				TOP_DEVICE_RECORD stDeviceRcd;
				theDevice.GetTopRcd(&stDeviceRcd);
				if (m_stIndexVar.nCatchUpUnit == 0)
					m_lStandbyAfterSearch = round(CENTER_TO_ALIGN_SENSOR - (m_stIndexRcd.lLfStandbyLen + m_stIndexVar.lNorIndexLen * 0.5 + stDeviceRcd.lDeviceEdgeLen * 2) + m_stIndexRcd.lIndexError[0]);
				else
					m_lStandbyAfterSearch = round(CENTER_TO_ALIGN_SENSOR - (m_stIndexRcd.lLfStandbyLen + m_stIndexVar.lNorIndexLen * 1.5 + stDeviceRcd.lDeviceEdgeLen * 2) + m_stIndexRcd.lIndexError[0]);
				
				EnterDelayState(TE_STATE_FINE_WAIT_FORWARD, 0,
					round((double)m_lStandbyAfterSearch / theStepper.GetResol(m_nIndexStepID)), 0);	//v3.0.T372 add
			}
			else
			{
				m_nTrackStopReason = (theMHS.IsLastLf() == true ? MHS_TOP_ONLOAD_NO_LF : MHS_TOP_SRCH_TIME_OUT);

				theMHS.SetLastLf(true);
				m_cState = m_acStateSeq[0];    
				LeftMoveArray(); 
			}
		}
		else
		{
			m_InputClaw.SetRelayState(RELAY_ON);
			m_LFHolder.SetRelayState(RELAY_ON);
			long lPos = round((double)m_stIndexRcd.lLfStandbyLen / theStepper.GetResol(m_nIndexStepID));
			long lAbsPos = round((double)(LogicToPhyPos(0) - 8000) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
			EnterDelayState(TE_STATE_FINE_WAIT_FORWARD, RELAY_ACTION_DELAY , lPos + (lAbsPos - searchpos) + pos, 1);
		}
		l1stIndexLen = m_stIndexVar.l1stIndexLen;
		
		break;
	}
	case TE_STATE_INDEX_SLOW_SRCH:
	{
		if(theStepper.IsBusy(m_nIndexStepID))break;

		SetStepSpeed(TE_SPEED_SLOW_SRCH);
		theStepper.MoveStep(m_nIndexStepID,800,MOTION_RELATIVE);

		m_cState = TE_STATE_INDEX_WAIT_SLOW_SRCH;

		break;
	}
	case TE_STATE_INDEX_WAIT_SLOW_SRCH:
	{
		if(m_AlignSensor.GetCurrentState()==SENSOR_OFF)break;

		theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);
		EnterDelayState(TE_STATE_FINE_SRCH_WAIT_STANDBY, 100, 0, 0);

		break;
	}
	case TE_STATE_INDEX_SLOW_SRCH_BACK:
	{
		if(m_AlignSensor.GetCurrentState()==SENSOR_ON)
		{
            if (!theStepper.IsBusy(m_nIndexStepID))
            {
				CStringList sList;
				sList.AddTail(theMsg.GetMsg(MSG_SLOW_SRCH_FAIL));
                theMsg.ShowMsg(ERROR_MSG, sList);
            }
			break;
		}

		theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);
		EnterDelayState(TE_STATE_FINE_SRCH_WAIT_STANDBY, 100, 0, 0);

		break;
	}
	case TE_STATE_FINE_WAIT_FORWARD:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-FineWaitForward"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		if (m_nEvent == TE_EVENT_INITIAL_INDEX)  // 1st Slip Heat Delay
		{
			m_stSlipOnTrack.lSlipHeaderPos = -CENTER_TO_ALIGN_SENSOR + m_lStandbyAfterSearch;
			m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipHeaderPos - m_stIndexVar.lLfTotalLen;
			m_stSlipOnTrack.lSlipTailPos += 8000;

			EnterDelayState(m_acStateSeq[0], GetCurrentTime(), 800 + m_stIndexRcd.l1stSlipHeatDelay,
				round((double)LogicToPhyPos(m_stIndexVar.lLeftLimitPos) / theStepper.GetResol(m_nIndexStepID)));
		}
		else
		{
			m_stSlipOnTrack.lSlipHeaderPos = -CENTER_TO_ALIGN_SENSOR + m_lStandbyAfterSearch;
			EnterDelayState(m_acStateSeq[0], RELAY_DELAY, 0, 0);
			m_nNextLfState = TE_NEXTLF_AFTER_SRCH;
		}

		LeftMoveArray();
        break;
	}
//-------------w.index--------------------------//
// 24~ 29 : Index Procedure
	case TE_STATE_INDEX:	//verified 2019.11.14
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevIndex"), m_cState);
			m_nLastDebugState = m_cState;
		}

		//EnterDelayState(TE_STATE_INDEX_WAIT_BH_SAFETY, 0, 0, 0);
		theTopClamp.MoveClampSafePos();
		m_cState = TE_STATE_INDEX_WAIT_BH_SAFETY;
		break;
	}
	case TE_STATE_INDEX_WAIT_BH_SAFETY:	//verified 2019.11.14
    {
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitBhSafety"), m_cState);
			m_nLastDebugState = m_cState;
		}

	    TBL_POS pstPos;
		theServo.GetTablePosition(&pstPos);
	    theTopOffloader.SetEmptyReported(false);
		//v3.1.T220 delete
		//if ((fabs(pstTblPos.dx - theMcData.GetXYStandbyPos().dx) < 2.0) &&
		//	(fabs(pstTblPos.dy - theMcData.GetXYStandbyPos().dy) < 2.0))
		//v3.1.T220 add
		if ((fabs(pstPos.dx - theMcData.GetXYStandbyPos().dx) < theMcConfig.GetSafePosTol())
			&& (fabs(pstPos.dy - theMcData.GetXYStandbyPos().dy) < theMcConfig.GetSafePosTol()))
	    {
		    if (m_stIndexRcd.bVacuumControl == true)
		    {
				theTopClamp.SetVacuumOn(false);
		        m_cState = TE_STATE_INDEX_WAIT_VACUUM_DELAY;
		    }
		    else
		        m_cState = TE_STATE_INDEX_WAIT_CLAW_CLAMP;
	    }
	    else
	    {
		    m_nErrorCode = MHS_TOP_BH_NOT_IN_SAFE_POS;
		    m_cState = TE_STATE_INDEX_STOP;
	    }
		break;
    }
    case TE_STATE_INDEX_WAIT_VACUUM_DELAY:	//verified 2019.11.14
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitVacDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		EnterDelayState(TE_STATE_INDEX_WAIT_CLAW_CLAMP, m_stIndexRcd.lVacuumDelay, 0, 0); 
		break;
	}
	case TE_STATE_INDEX_WAIT_CLAW_CLAMP:	//verified 2019.11.14
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClawClamp"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.MoveClampSafePos();

		if (m_nEvent == TE_EVENT_INITIAL_INDEX)
		{
			m_stIndexVar.lIndexLenAftPR = l1stIndexLen;
			m_pTargetClaw->SetRelayState(RELAY_ON);

			if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);
		}
		else if (m_nEvent == TE_EVENT_NEW_SLIP)
		{
			if (m_stIndexVar.nCatchUpUnit != 0)
				m_stIndexVar.lIndexLenAftPR = l1stIndexLen - m_stIndexVar.lCatchUpLen;
			else
				m_stIndexVar.lIndexLenAftPR = l1stIndexLen;

			m_InputClaw.SetRelayState(RELAY_ON);
			m_OutputClaw.SetRelayState(RELAY_ON);
			m_LFHolder.SetRelayState(RELAY_ON);
		}
		else
		{
			TOP_DEVICE_RECORD stDeviceRcd;
			theDevice.GetTopRcd(&stDeviceRcd);

			if ((m_nEvent == TE_EVENT_UNIT_STANDBY)
				&& (stDeviceRcd.nHalfIndexOn > 0)
				&& (m_nCurBondUnit == (m_stIndexVar.nTotalUnits - 1)))
				m_stIndexVar.lIndexLenAftPR = m_stIndexVar.lNorIndexLen / 2
										+ m_stIndexRcd.lIndexError[m_nCurBondUnit];
			else
				m_stIndexVar.lIndexLenAftPR = m_stIndexVar.lNorIndexLen
										+ m_stIndexRcd.lIndexError[m_nCurBondUnit];

			if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);
			m_pTargetClaw->SetRelayState(RELAY_ON);
		}

		m_cState = TE_STATE_INDEX_WAIT_CLAMP_OPEN;
		break;
	}
	case TE_STATE_INDEX_WAIT_CLAMP_OPEN:	//verified 2019.11.14
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d_%d-WaitClampOpen"), m_cState, m_nEvent);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.CheckClampStatus();
		if (theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
		{
			if (m_nEvent != TE_EVENT_INITIAL_INDEX)
				EnterDelayState(TE_STATE_INDEX_WAIT_CLAMP_OPEN_DELAY, m_stIndexRcd.lOpenClampDelay, 0, 0);
			else
				m_cState = TE_STATE_INDEX_WAIT_CLAMP_OPEN_DELAY;
		}
		break;
	}
	case  TE_STATE_INDEX_WAIT_CLAMP_OPEN_DELAY:	//verified 2019.11.14
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClampOpenDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if ((m_bClampSensorEnabled == TRUE)
			&& (m_TrackBackHome.GetCurrentState() == SENSOR_ON))
		{
			m_nErrorCode = MHS_TOP_WINDOW_CLAMPER_NOT_OPENED;
			m_cState = TE_STATE_INDEX_STOP;
			break;
		}

		m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
			+ round((double)m_stIndexVar.lIndexLenAftPR / theStepper.GetResol(m_nIndexStepID));	//v3.0.T372 edit, v3.0.T376 edit
		EnterDelayState(TE_STATE_INDEX_WAIT_FEED_IN, 0, m_lMoveDist);
		break;
	}
	case TE_STATE_INDEX_WAIT_FEED_IN:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitFeedIn"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;
		if (m_lMoveDist != theStepper.GetPos(m_nIndexStepID)) break;

		if (m_stIndexRcd.bFlattenLF)
		{
			theTopClamp.CloseClamp();
			EnterDelayState(theMcConfig.IsPrIndexHalfClamp() ? TE_STATE_INDEX_AFTER_FEED_IN : TE_STATE_INDEX_AFTER_FEED_IN_2, m_stIndexRcd.wFlattenDuration, 0, 0);
		}
		else
		{
			m_cState = theMcConfig.IsPrIndexHalfClamp() ? TE_STATE_INDEX_AFTER_FEED_IN : TE_STATE_INDEX_AFTER_FEED_IN_2;
		}

		break;
	}
	case TE_STATE_INDEX_AFTER_FEED_IN:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-AfterFeedIn"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.PreCloseClamp();
		m_cState = TE_STATE_INDEX_WAIT_PR_INDEX_DELAY;

        if (m_nEvent != TE_EVENT_NEW_SLIP)break;

        m_OutputClaw.SetRelayState(RELAY_OFF);

        if ((theMHS.IsLastLf() == false)
			&& (m_stIndexVar.nTotalUnits  <= 3))
        {
            RequestOnload();
        }

        if (theMHS.IsLastLf())
        {
            BOOL bNextLfReady = 1;

            if (m_stIndexVar.nTotalUnits > 3)
            {
                if (m_nNextLfState != TE_NEXTLF_AFTER_CATCH_UP)
                    bNextLfReady = TE_NEXTLF_NONE;
            }
            else
            {
                if (m_nNextLfState != TE_NEXTLF_AFTER_SRCH)
                    bNextLfReady = TE_NEXTLF_NONE;
            }

            if ((m_AlignSensor.GetCurrentState() == FALSE) && (bNextLfReady == TE_NEXTLF_NONE))
            {
                m_nCurBondUnit = 0;
                m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexVar.lIndexLenAftPR;
                m_nErrorCode = MHS_END_ALL_LF;
                m_cState = TE_STATE_INDEX_STOP;
            }
            else
            {
                m_nNextLfState = TE_NEXTLF_NONE;
            }
        }
        else
        {
            m_nNextLfState = TE_NEXTLF_NONE;
        }

        break;
	}
	case TE_STATE_INDEX_AFTER_FEED_IN_2:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-AfterFeedIn2"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.CloseClamp();
		m_cState = TE_STATE_INDEX_WAIT_PR_INDEX_DELAY_2;

        if (m_nEvent != TE_EVENT_NEW_SLIP)break;

        m_OutputClaw.SetRelayState(RELAY_OFF);

        if ((theMHS.IsLastLf() == false)
			&& (m_stIndexVar.nTotalUnits  <= 3))
        {
            RequestOnload();
        }

        if (theMHS.IsLastLf())
        {
            BOOL bNextLfReady = 1;

            if (m_stIndexVar.nTotalUnits > 3)
            {
                if (m_nNextLfState != TE_NEXTLF_AFTER_CATCH_UP)
                    bNextLfReady = TE_NEXTLF_NONE;
            }
            else
            {
                if (m_nNextLfState != TE_NEXTLF_AFTER_SRCH)
                    bNextLfReady = TE_NEXTLF_NONE;
            }

            if ((m_AlignSensor.GetCurrentState() == FALSE) && (bNextLfReady == TE_NEXTLF_NONE))
            {
                m_nCurBondUnit = 0;
                m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexVar.lIndexLenAftPR;
                m_nErrorCode = MHS_END_ALL_LF;
                m_cState = TE_STATE_INDEX_STOP;
            }
            else
                m_nNextLfState = TE_NEXTLF_NONE;
        }
        else
            m_nNextLfState = TE_NEXTLF_NONE;

        break;
	}
	case  TE_STATE_INDEX_WAIT_PR_INDEX_DELAY:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitPrIndexDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		EnterDelayState(TE_STATE_INDEX_WAIT_CLAMP_PRE_CLOSE, m_stIndexRcd.lIndexPrDelay, 0, 0);

		m_nCurBondUnit++;
		if (m_nCurBondUnit > m_stIndexVar.nTotalUnits)
			m_nCurBondUnit = 1;

       break;
	}
	case  TE_STATE_INDEX_WAIT_PR_INDEX_DELAY_2:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitPrIndexDelay2"), m_cState);
			m_nLastDebugState = m_cState;
		}

		EnterDelayState(TE_STATE_INDEX_WAIT_CLAMP_CLOSE_2, m_stIndexRcd.lIndexPrDelay, 0, 0);

		m_nCurBondUnit++;
		if (m_nCurBondUnit > m_stIndexVar.nTotalUnits)
			m_nCurBondUnit = 1;

       break;
	}
	case TE_STATE_INDEX_WAIT_CLAMP_PRE_CLOSE:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitClampPreClose"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (!m_stIndexRcd.bVacuumControl)
		//vacuum enabled not check pre-close resolved hang but reason unknown
		//vacuum disabled remain checking
		{
			theTopClamp.CheckClampStatus();
			if (theTopClamp.GetClampStatus() != WND_CLAMP_PRE_CLOSE) break;
		}

		INDEX_PR_REF stPrIndexRefRcd;
		theDevice.GetIndexPrRef(&stPrIndexRefRcd);

		if (((m_stIndexRcd.nPrIndexEnable == 2)	//enable 1st unit only
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)
				&& (GetCurBondUnit() == 1))
			|| ((m_stIndexRcd.nPrIndexEnable == 1)	//enable for all
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)))
		{
			m_nPRIState = 1;

			long lPrOffset;
			int iReturnStatus = CalcPrIndexOffset(&lPrOffset);
			if (abs(lPrOffset) > m_stIndexRcd.lPrIndexTolerance)
			{
				if (iReturnStatus == FOK)
				{
					SetIndexPrManualAlign(TE_INDEXPR_OK);
					m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
							+ round((double)lPrOffset / theStepper.GetResol(m_nIndexStepID));	//v3.0.T372 edit, v3.0.T376 edit
					EnterDelayState(TE_STATE_INDEX_WAIT_PR_FINE_TUNE, 0, m_lMoveDist);
					m_stIndexVar.lIndexLenAftPR = m_stIndexVar.lIndexLenAftPR + lPrOffset;
				}
				else 
				{
					m_nPRIState = 0;
					SetIndexPrManualAlign(TE_INDEXPR_ENTER_MANUAL_ALIGN);
					m_cState = TE_STATE_INDEX_WAIT_MANUAL_ALIGN;
				}
			}
			else
				m_cState = TE_STATE_INDEX_WAIT_PR_FINE_TUNE;
		}
		else
			m_cState = TE_STATE_INDEX_WAIT_PR_FINE_TUNE;

		theServo.WaitTableStop();
		theServo.MoveTable2(stPrIndexRefRcd.stVisionAlignPos, true, false, false);	//v3.1.T210 edit
        
        break;
	}
	case TE_STATE_INDEX_WAIT_CLAMP_CLOSE_2:
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

		if (((m_stIndexRcd.nPrIndexEnable == 2)	//enable 1st unit only
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)
				&& (GetCurBondUnit() == 1))
			|| ((m_stIndexRcd.nPrIndexEnable == 1)	//enable for all
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (m_bPrIndexMask == false)))
		{
			m_nPRIState = 1;

			long lPrOffset;
			m_nPRIndexStatus = CalcPrIndexOffset(&lPrOffset);
			if (abs(lPrOffset) > m_stIndexRcd.lPrIndexTolerance)
			{
				m_bNeedOffset = TRUE;

				if (m_nPRIndexStatus == FOK)
				{
					m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
							+ round(lPrOffset / theStepper.GetResol(m_nIndexStepID));

					m_stIndexVar.lIndexLenAftPR = m_stIndexVar.lIndexLenAftPR + lPrOffset;
				}
				else
					m_nPRIState = 0;

				EnterDelayState(TE_STATE_INDEX_CLAMP_PRE_CLOSE, 50, 0, 0);
			}
			else
				m_cState = TE_STATE_INDEX_WAIT_PR_FINE_TUNE;
		}
		else
			m_cState = TE_STATE_INDEX_WAIT_PR_FINE_TUNE;

		theServo.WaitTableStop();
		theServo.MoveTable2(stPrIndexRefRcd.stVisionAlignPos, true, false, false);	//v3.1.T210 edit
        
        break;
	}
	case TE_STATE_INDEX_CLAMP_PRE_CLOSE:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClampPreClose"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.PreCloseClamp();
		EnterDelayState(TE_STATE_INDEX_WAIT_CLAMP_PRE_CLOSE_2, 50, 0, 0);

		break;
	}
	case TE_STATE_INDEX_WAIT_CLAMP_PRE_CLOSE_2:
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
				SetIndexPrManualAlign(TE_INDEXPR_OK);
				theStepper.MoveStep(m_nIndexStepID, m_lMoveDist);
				m_cState = TE_STATE_INDEX_WAIT_PR_FINE_TUNE;
			}
			else
			{
				SetIndexPrManualAlign(TE_INDEXPR_ENTER_MANUAL_ALIGN);
				m_cState = TE_STATE_INDEX_WAIT_MANUAL_ALIGN;
			}
		}
		else
		{
			m_cState = TE_STATE_INDEX_WAIT_PR_FINE_TUNE;
		}

		break;
	}
	case TE_STATE_INDEX_WAIT_MANUAL_ALIGN:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitManualAlign"), m_cState);
			m_nLastDebugState = m_cState;
		}

        if (GetIndexPrManualAlign() == TE_INDEXPR_MANUAL_ALIGN_STOP)
        {
            m_cState = TE_STATE_INDEX_WAIT_PR_FINE_TUNE;
        }
        else if (GetIndexPrManualAlign() == TE_INDEXPR_MANUAL_ALIGN_OK)
        {
            m_pTargetClaw->SetRelayState(RELAY_ON);
            if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);

			m_lMoveDist = theStepper.GetPos(m_nIndexStepID)
					+ round((double)m_lOffsetx1um / theStepper.GetResol(m_nIndexStepID));	//v3.0.T372 edit, v3.0.T376 edit
				EnterDelayState(TE_STATE_INDEX_WAIT_PR_FINE_TUNE, RELAY_ACTION_DELAY, m_lMoveDist);
			m_stIndexVar.lIndexLenAftPR = m_stIndexVar.lIndexLenAftPR + m_lOffsetx1um;
        }
        break;
	}
	case TE_STATE_INDEX_WAIT_PR_FINE_TUNE:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitPrFineTune"), m_cState);
			m_nLastDebugState = m_cState;
		}

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

		if ((m_nEvent == TE_EVENT_NEW_SLIP)
			|| (m_nEvent == TE_EVENT_INITIAL_INDEX))
		{
			//Head Position transfer to tail position
			//Head Position Reset.
			if (m_nEvent == TE_EVENT_NEW_SLIP)
				m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexVar.lIndexLenAftPR;

			m_stSlipOnTrack.lSlipHeaderPos = m_stSlipOnTrack.lSlipHeaderPos + m_stIndexVar.lIndexLenAftPR;
			m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipHeaderPos - m_stIndexVar.lLfTotalLen;
			m_stSlipOnTrack.lSlipTailPos += 8000;

			//reset header
			m_stSlipOnTrack.lSlipHeaderPos = -CENTER_TO_LEFT_EDGE;
		}
		else
		{
			//Only Update Tail Position
			m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipTailPos + m_stIndexVar.lIndexLenAftPR;
		}
		m_cState = TE_STATE_INDEX_WAIT_CLAMP_CLOSE;
        break;
	}
	case TE_STATE_INDEX_WAIT_CLAMP_CLOSE:
	{
		if (m_cState != m_nLastDebugState)
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

			if (m_stIndexVar.nTotalUnits >= 3)
			{
				if ((m_nCurBondUnit == m_stIndexVar.nInjectUnit)
					&& (theMHS.IsLastLf() == false))
				{
					RequestOnload();
				}
			}

			m_lStartDelayTime = GetCurrentTime();

			if (m_nPRIState==1&&theDebug.IsDebugIndexAccuracy())
				EnterDelayState(TE_STATE_INDEX_DELAY_AFTER_CLAMP_CLOSE, 100, 0, 0);
			else
				m_cState = TE_STATE_INDEX_WAIT_HEAT_DELAY;
		}
		break;
	}
	case TE_STATE_INDEX_DELAY_AFTER_CLAMP_CLOSE:
	{
		m_nPRIState = 2;
		long lPrOffset = 0;
		CalcPrIndexOffset(&lPrOffset);
		m_nPRIState = 0;
		m_cState = TE_STATE_INDEX_WAIT_HEAT_DELAY;
		break;
	}
	case TE_STATE_INDEX_WAIT_HEAT_DELAY:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitHeatDelay"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (m_nEvent == TE_EVENT_INITIAL_INDEX)
        {
			if (GetCurrentTime() - m_lStartDelayTime  > (m_bTeaching ? 1000 : m_stIndexRcd.lInitDelay))	//V1.5.W64 edit
            {
                theMHS.SetRequestIndex(false);
                m_cState = m_acStateSeq[0];    
                LeftMoveArray();
            }
        }
        else
        {
            if (m_nEvent == TE_EVENT_NEW_SLIP)
            {
                if (GetCurrentTime() - m_lStartDelayTime > m_stIndexRcd.l1stUnitHeatDelay)
                {
                    theMHS.SetRequestIndex(false);
                    m_cState = m_acStateSeq[0];    
                    LeftMoveArray();
                }
            }
            else
            {
                if (GetCurrentTime() - m_lStartDelayTime > m_stIndexRcd.lHeatingDelay)
                {
                    theMHS.SetRequestIndex(false);
                    m_cState = m_acStateSeq[0];    
                    LeftMoveArray();
                }
            }
        }
        break;
	}
	case TE_STATE_INDEX_STOP:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-IndexStop"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theMHS.SetRequestIndex(false);
		m_InputClaw.SetRelayState(RELAY_OFF);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_OutputClaw.SetRelayState(RELAY_OFF);
		m_cState = m_acStateSeq[0];    
		LeftMoveArray();
		break;
	}
//output
// 17~21 ---------Output and Eject the front slip---------- 
//output loop:
	case TE_STATE_OUTPUT:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevOutput"), m_cState);
			m_nLastDebugState = m_cState;
		}

		SetStepSpeed(TE_SPEED_BACKWARD);

        if (theDebug.IsDebugWorkHolder())
            theDebug.AddInfo(_T("SlipTailPos"), m_stSlipOnTrack.lSlipOutputTailPos);

		if (MoveIndexerToWorkPos(m_stSlipOnTrack.lSlipOutputTailPos))
			m_cState = TE_STATE_EJECT;
		else
			m_cState = TE_STATE_OUTPUT_WAIT_AT_OUTPUT_POS;

		m_lStartTime = GetCurrentTime();
		break;
	}
	case TE_STATE_OUTPUT_WAIT_AT_OUTPUT_POS:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitOutputPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		if (theTopOffloader.GetEvent() == TOP_OFFLOAD_EVENT_IDLE)
		{
			long lOriginPos = theStepper.GetPos(m_nIndexStepID);
			m_pTargetClaw->SetRelayState(TRUE);
			if (m_pTargetClaw == &m_InputClaw)
				m_LFHolder.SetRelayState(RELAY_ON);
			long lPos = round((double)LogicToPhyPos(m_stIndexVar.lRightLimitPos) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
			m_lMoveDist = round((double)(lPos - lOriginPos) * theStepper.GetResol(m_nIndexStepID));
			EnterDelayState(TE_STATE_OUTPUT_ONE_CIRCLE_DONE, RELAY_ACTION_DELAY, lPos);
		}
		else
		{
			if (GetCurrentTime() - m_lStartTime > ELEV_TIMEOUT)
			{
				m_cState = TE_STATE_CLAW_STANDBY;
				m_nErrorCode = MSG_OFFLOAD_TIMEOUT;
			}
		}
        break;
	}
	case TE_STATE_OUTPUT_ONE_CIRCLE_DONE:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-OneCircleDone"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipOutputTailPos + m_lMoveDist;
		m_pTargetClaw->SetRelayState(FALSE);
		if (m_pTargetClaw == &m_InputClaw)
			m_LFHolder.SetRelayState(RELAY_OFF);
		m_cState = TE_STATE_OUTPUT;
        break;
	}
//Ejection Start:
	case TE_STATE_EJECT:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-DevEject"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		SetStepSpeed(TE_SPEED_BACKWARD);
		EnterDelayState(TE_STATE_OUTPUT_WAIT_AT_EJECT_POS, GetCurrentTime(), 0,
			round((double)LogicToPhyPos(m_stIndexVar.lEjectStartPos) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit
		break;
	}
	case TE_STATE_OUTPUT_WAIT_AT_EJECT_POS:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitEjectPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		long lPos = round((double)LogicToPhyPos(m_stIndexVar.lEjectStartPos) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
		if (lPos != theStepper.GetPos(m_nIndexStepID, FALSE)) break;

		//clamp Ejector
		SetStepSpeed(TE_SPEED_EJECT);
		m_Ejector.SetRelayState(TRUE);
		EnterDelayState(TE_STATE_OUTPUT_WAIT_EJECT_DEST, GetCurrentTime(), SMALL_RELAY_DELAY,
			round((double)LogicToPhyPos(m_stIndexVar.lRightLimitPos) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit
    	break;
	}
	case TE_STATE_OUTPUT_WAIT_EJECT_DEST:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-WaitEjectDest"), m_cState);
			m_nLastDebugState = m_cState;
		}

		long lPos = round((double)LogicToPhyPos(m_stIndexVar.lRightLimitPos) / theStepper.GetResol(m_nIndexStepID));	//v3.0.T374 edit
        if (theStepper.GetPos(m_nIndexStepID) == lPos)
        {
			EnterDelayState(TE_STATE_OUTPUT_BACKWARD_AFTER_EJECT, GetCurrentTime(), 30,
				round((double)LogicToPhyPos(m_stIndexVar.lRightLimitPos - BACKWARD_AFTER_EJECT) / theStepper.GetResol(m_nIndexStepID)));	//v3.0.T374 edit
        }
        else if (m_EjectJamSensor.GetCurrentState() == FALSE)
        {
            theStepper.StopMotor(m_nIndexStepID, STEPPER_STOP_DEC);
            m_Ejector.SetRelayState(FALSE);
            m_cState = m_acStateSeq[0];   
            m_nErrorCode = MHS_OFFLOAD_PUSHER_JAM;

			while (theStepper.IsBusy(m_nIndexStepID))
			{
				Sleep_x1ms(10);
			}

			EnterDelayState(TE_STATE_OUTPUT_BACKWARD_AFTER_EJECT, 0,
				round((double)-10000 / theStepper.GetResol(m_nIndexStepID)), 0);

            LeftMoveArray();
        }
        break;
	}
	case TE_STATE_OUTPUT_BACKWARD_AFTER_EJECT:
	{
		if (m_cState != m_nLastDebugState)
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
				m_cState = m_acStateSeq[0];
			} while (m_acStateSeq[0] != TE_STATE_IDLE);
			break;
		}

		if (m_nEvent == TE_EVENT_CLEAR_LAST_UNIT)
			m_nCurBondUnit = 0;

		m_cState = m_acStateSeq[0];
		LeftMoveArray();

		if (theMHS.IsLastLf() && (m_nCurBondUnit == 0))
		{
			theTEIndex.ResetSlipRcd();
			theTEIndex.ResetRelayErrorState();
			theTEIndex.MoveIndexerToLogicZero();

			theMHS.SetLastLf(false);
			theMHS.SetBusy(true);
		}
		else
		{
			if ((m_stIndexVar.nTotalUnits > 3) && (m_stIndexVar.nInjectUnit == 1))
			{
				if ((theMHS.IsLastLf() == false)
					&& (m_nEvent == TE_EVENT_NEW_SLIP))
				{
					RequestOnload();
				}
			}
		}
		break;
	}
//*****************************************Standby Index Position*******************************************
	case TE_STATE_CLAW_STANDBY:	//verified 2019.11.12
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClawStandby"), m_cState);
			m_nLastDebugState = m_cState;
		}

		MoveIndexerToWorkPos(m_stSlipOnTrack.lSlipTailPos);
		m_cState = TE_STATE_CLAW_STANDBY_WAIT_DEST_POS;
		break;
	}
	case TE_STATE_CLAW_STANDBY_WAIT_DEST_POS:	//verified 2019.11.12
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClawWaitDestPos"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		SetStepSpeed(TE_SPEED_INDEX);
		m_cState = m_acStateSeq[0]; 
		LeftMoveArray();
		break;
	}
    case TE_STATE_WAIT_DELAY_MOVE:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-IndexDelayWaiting"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (GetCurrentTime() - m_stDelayState.lStartTime < m_stDelayState.lDelay_x1ms) break;
		if (theStepper.IsBusy(m_nIndexStepID)) break;

		if ((m_stDelayState.cMoveMode == 0) && (m_stDelayState.lMoveDist != 0))
			theStepper.MoveStep(m_nIndexStepID, m_stDelayState.lMoveDist, MOTION_RELATIVE);
		else if (m_stDelayState.cMoveMode == 1)
			theStepper.MoveStep(m_nIndexStepID, m_stDelayState.lMoveDist);

		m_cState = m_stDelayState.cNextState;

        break;
	}
//*****************************FOR CLEAR TRACK*****************************************
	case TE_STATE_CLEAR_TRACK:	//verified 2019.11.11
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrack"), m_cState);
			m_nLastDebugState = m_cState;
		}

		theTopClamp.CheckClampStatus();
		if (theTopClamp.GetClampStatus() != WND_CLAMP_SAFE_CLAMP) break;

		SetStepSpeed(TE_SPEED_INDEX);
		m_cState = m_acStateSeq[0]; 
		LeftMoveArray();
        break;
	}
	case TE_STATE_CLEAR_TRACK_START_1ST:	//verified 2019.11.11
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackStart1st"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipTailPos;
		m_cState = TE_STATE_OUTPUT;
		break;
	}
	case TE_STATE_CLEAR_TRACK_START_2ND:	//verified 2019.11.11
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackStart2nd"), m_cState);
			m_nLastDebugState = m_cState;
		}

		CUELITE_DEVICE_RECORD stDeviceRcd;
		theDevice.GetCuEliteRcd(&stDeviceRcd);
        if (m_stSlipOnTrack.lSlipHeaderPos >= -CENTER_TO_ALIGN_SENSOR)
        {
            m_stSlipOnTrack.lSlipOutputTailPos = m_stSlipOnTrack.lSlipHeaderPos - stDeviceRcd.lDeviceTotalLen;
            m_cState = TE_STATE_OUTPUT;
        }
		else
            m_cState = TE_STATE_IDLE;
        break;
	}
	case TE_STATE_CLAW_POS_RESET:	//verified 2019.11.11
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClawPosReset"), m_cState);
			m_nLastDebugState = m_cState;
		}

		m_InputClaw.SetRelayState(FALSE);
        m_LFHolder.SetRelayState(RELAY_OFF);
		m_OutputClaw.SetRelayState(FALSE);
		theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(0));
		m_cState = TE_STATE_CLEAR_TRACK_OFFLOAD_1ST;
		break;
	}
	case TE_STATE_CLEAR_TRACK_OFFLOAD_1ST:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackOffload1st"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theStepper.IsBusy(m_nIndexStepID)) break;

		theTopOffloader.SubmitOffloadEvent(TOP_OFFLOAD_EVENT_START);
		m_cState = TE_STATE_CLEAR_TRACK_WAIT_OFFLOAD_PROC;
		break;
	}
	case TE_STATE_CLEAR_TRACK_WAIT_OFFLOAD_PROC:
	{
		if (m_cState != m_nLastDebugState)
		{
			theBond.ShowDebugMsg(DEBUG_WORK_HOLDER, _T("%d-ClearTrackWaitOffload"), m_cState);
			m_nLastDebugState = m_cState;
		}

		if (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)
			theTopOffloader.ProcessOffloadEvent();
		else
		{
			theTEIndex.SetOffloadEnable(false);
			m_cState = m_acStateSeq[0]; 
			LeftMoveArray();
		}
		break;
	}
	default:
		break;
	}
}

//transfer indexer coordinate to globe coordinate
long CTopEliteIndex::TransX(long lIndexerX, char cClawSide)
{
	return lIndexerX + (cClawSide == LEFT_CLAW ? -CENTER_TO_LEFT_CLAW : CENTER_TO_RIGHT_CLAW);
}

//transfer globe coordinate to indexer coordinate 
long CTopEliteIndex::TransXRev(long lIndexerX, char cClawSide)
{
	return lIndexerX + (cClawSide == LEFT_CLAW ? CENTER_TO_LEFT_CLAW : -CENTER_TO_RIGHT_CLAW);
}

int CTopEliteIndex::MoveIndexerToWorkPos(long lRefTailPos)
{
	long lLfHeadPos;
	long lPos;

	TOP_DEVICE_RECORD stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);

	SetStepSpeed(TE_SPEED_FORWARD);

	m_stIndexVar.lLeftClawIndexLimitPos = TransX(m_stIndexVar.lRightLimitPos, LEFT_CLAW) - (m_stIndexVar.lNorIndexLen + m_stIndexRcd.lIndexError[m_nCurBondUnit]);
	m_stIndexVar.lRightClawIndexLimitPos = TransX(m_stIndexVar.lLeftLimitPos, RIGHT_CLAW);  

	if (lRefTailPos + m_stIndexRcd.lIndexPosOffset <= m_stIndexVar.lLeftClawIndexLimitPos)
	{
		//transfer to indexer coordinate
		if (lRefTailPos <= TransX(m_stIndexVar.lLeftLimitPos, LEFT_CLAW))
			lPos = m_stIndexVar.lLeftLimitPos + 10000;
		else
			lPos = TransXRev(lRefTailPos + m_stIndexRcd.lIndexPosOffset + 5000, LEFT_CLAW);

		m_pTargetClaw = &m_InputClaw;
	}
	else //right claw 
	{
        if (lRefTailPos + m_stIndexRcd.lIndexPosOffset + 2000 + stDeviceRcd.lDeviceEdgeLen > TransX(m_stIndexVar.lRightLimitPos, RIGHT_CLAW))
		{
			return 1;
		}

		lLfHeadPos = lRefTailPos + stDeviceRcd.lDeviceTotalLen;
		if (lLfHeadPos - m_stIndexRcd.lIndexPosOffset < 28000)
		{
			lPos = TransXRev(lRefTailPos + 5000, LEFT_CLAW);	//5mm offset default
			m_pTargetClaw = &m_InputClaw;
		}
		else
		{
			if (lRefTailPos + m_stIndexRcd.lIndexPosOffset <= 28000)
			{
				lPos = m_stIndexVar.lLeftLimitPos + 4000;
				m_pTargetClaw = &m_OutputClaw;
			}
			else
			{
				lPos = TransXRev(lRefTailPos + m_stIndexRcd.lIndexPosOffset, RIGHT_CLAW); 
				m_pTargetClaw = &m_OutputClaw;
			}
		}
	}

	theStepper.MoveDist(m_nIndexStepID, LogicToPhyPos(lPos));
	return 0;
}

void CTopEliteIndex::EnterDelayState(char cNextState, long lDelay_x1ms, long lMoveDist, char cMoveMode)
{
	//delay -> move -> goto next state
	m_stDelayState.lDelay_x1ms = lDelay_x1ms;
	m_stDelayState.lStartTime  = GetCurrentTime();
	m_stDelayState.cNextState  = cNextState;
	m_stDelayState.cMoveMode   = cMoveMode;
	m_stDelayState.lMoveDist   = lMoveDist;
	m_cState = TE_STATE_WAIT_DELAY_MOVE;
}

void CTopEliteIndex::SetStepSpeed(short nProfile)
{
	ST_STEPPER_SPEED stStepSpeed;
	GetStepSpeed(nProfile, &stStepSpeed);
	theStepperIO.SetMotorSpeedProfile(m_nIndexStepID, stStepSpeed);
}

void CTopEliteIndex::GetStepSpeed(short nProfile, ST_STEPPER_SPEED *pstStepSpeed)
{
	switch (nProfile)
	{
	case TE_SPEED_SRCH:
	{
		pstStepSpeed->Acc = 6000000;
		pstStepSpeed->Dcc = 6000000;
		pstStepSpeed->Vel = 150000;
		pstStepSpeed->StopDcc = 20000000;
		break;
	}
	case TE_SPEED_SLOW_FORWARD:
	{
		pstStepSpeed->Acc = 2000000 + 1500000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Dcc = 2000000 + 1500000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Vel = 30000 + 60000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->StopDcc = 80000000;
		break;
	}
	case TE_SPEED_FORWARD:
	{
		pstStepSpeed->Acc = 2000000 + 1700000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Dcc = 2000000 + 1700000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Vel = 30000 + 67000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->StopDcc = 80000000;
		break;
	}
	case TE_SPEED_INJECT:
	{
		pstStepSpeed->Acc = 2000000 + 1500000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Dcc = 2000000 + 1500000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Vel = 30000 + 60000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->StopDcc = 80000000;
		break;
	}
	case TE_SPEED_BACKWARD:
	{
		pstStepSpeed->Acc = 2000000 + 1700000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Dcc = 2000000 + 1700000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Vel = 30000 + 67000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->StopDcc = 80000000;
		break;
	}
	case TE_SPEED_INDEX:
	{
		pstStepSpeed->Acc = 2000000 + 2000000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Dcc = 2000000 + 2000000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->Vel = 30000 + 85000 * m_stIndexRcd.nIndexSpeedCoeff;
		pstStepSpeed->StopDcc = 80000000;
		break;
	}
	case TE_SPEED_EJECT:
	{
		pstStepSpeed->Acc = 2000000 + 1200000 * m_stIndexRcd.nEjectSpeedCoeff;
		pstStepSpeed->Dcc = 2000000 + 1200000 * m_stIndexRcd.nEjectSpeedCoeff;
		pstStepSpeed->Vel = 30000 + 40000 * m_stIndexRcd.nEjectSpeedCoeff;
		pstStepSpeed->StopDcc = 80000000;
		break;
	}
	case TE_SPEED_SLOW_SRCH:
	{
		int nSpeed = abs(theMcConfig.GetAlignSensorSlowSearchSpeed());
		pstStepSpeed->Acc = 60000 * nSpeed;
		pstStepSpeed->Dcc = 60000 * nSpeed;
		pstStepSpeed->Vel = 1500 * nSpeed;
		pstStepSpeed->StopDcc = 200000 * nSpeed;
		break;
	}
	default:
		break;
	}
}

int CTopEliteIndex::CalcPrIndexOffset(long* loffsetXInUm)   
{
	PR_SRCH_RESULT stSrchResult[2];

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);
	if (stPrIndexRefRcd.bIndexPrAvail == false) return FNOK;

	*loffsetXInUm = 10000;
	theVisionSys.SetDrawWire(false);

	theServo.WaitTableStop();

	theServo.MoveTable2(stPrIndexRefRcd.stVisionAlignPos, true, false, false);	//v3.1.T210 edit
	theVisionSys.UpdateLightingParameter(
			stPrIndexRefRcd.stVisionLightPara.nCoaxRedLight,
			stPrIndexRefRcd.stVisionLightPara.bSideRedLight,
			stPrIndexRefRcd.stVisionLightPara.nCoaxBlueLight,
			stPrIndexRefRcd.stVisionLightPara.bSideBlueLight);
	theVisionSys.ChangeLensMag(stPrIndexRefRcd.nVisionAlignMag);

	theServo.WaitTableStop();
	theVisionSys.EnableVideo(false);
	theVisionSys.GrabImage();

	short nPrPassScore = theTEIndex.GetRcd()->sPRIndexScore;

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

		if (m_nPRIState&&theDebug.IsDebugIndexAccuracy())
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

int CTopEliteIndex::PerformManualAlignment(CRelay *pClaw)
{
	if (GetCurBondUnit() < m_stIndexVar.nSrchUnit)
		m_LFHolder.SetRelayState(RELAY_ON);

	if (pClaw == NULL)
		m_pTargetClaw->SetRelayState(RELAY_ON);
	else
		pClaw->SetRelayState(RELAY_ON);

	CPopUpTopStepperCtr topStepperCtrl(theTEIndex.m_nIndexStepID);
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

CRelay* CTopEliteIndex::GetClaw()
{
	//Input claw pos
	long lClawPos = 0;
	theStepper.GetPos(m_nIndexStepID, &lClawPos);
	lClawPos = round(lClawPos * theStepper.GetResol(m_nIndexStepID))
					- theMcData.GetZeroOffset() - IN_OUT_CLAW_SPAN / 2;

	//LF pos
	TOP_DEVICE_RECORD stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);
	long lHeadPos = round((m_nCurBondUnit - 0.5) * stDeviceRcd.nWndUnitNum
					* stDeviceRcd.lDeviceColmPitch) + stDeviceRcd.lDeviceEdgeLen;
	long lTailPos = lHeadPos - stDeviceRcd.lDeviceTotalLen;
	if (lClawPos > lTailPos && lClawPos < lHeadPos) return &m_InputClaw;

	//Output claw pos
	lClawPos += IN_OUT_CLAW_SPAN;
	if (lClawPos > lTailPos && lClawPos < lHeadPos) return &m_OutputClaw;

	return NULL;
}

void CTopEliteIndex::PerformPrIndex(void)
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
		theTEIndex.SetIndexPrManualAlign(TE_INDEXPR_MANUAL_ALIGN_STOP);
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
		lStopSelected = theTEIndex.PerformManualAlignment(GetClaw());
		if (bJoyStickValid == false)
			theScanKeyThread.DisableJoyStick(); 

		if (lStopSelected != SHOW_MSG_RESULT_OK)
		{
			theTEIndex.SetIndexPrManualAlign(TE_INDEXPR_MANUAL_ALIGN_STOP);
		}
		else
		{
			theTEIndex.SetIndexPrManualAlign(TE_INDEXPR_MANUAL_ALIGN_OK);
			theTowerLight.EnableBlinkRedLight(false);
			theTowerLight.EnableBondingLight();
		}
	}
	else
	{
		theTEIndex.SetIndexPrManualAlign(TE_INDEXPR_MANUAL_ALIGN_OK);
		m_lOffsetx1um = 0;

		theTowerLight.EnableBlinkRedLight(false);
		theTowerLight.EnableBondingLight();
	}
}

void CTopEliteIndex::RequestOnload(void)
{
	if (m_InjectSensor.GetCurrentState() == SENSOR_OFF)
		theTEIndex.m_bRequestOnload = true;
}

short CTopEliteIndex::GetNextLfState(void)
{
	return m_nNextLfState;
}

//************************************Debug*****************************************************
void CTopEliteIndex::SubmitDryRunEvent(short nEvent)
{
	m_nTestTicker = 0;
	m_lTestDist[0] = 10000;		//um
	m_lTestDist[1] = 20000;		//um
	m_lTestDist[2] = 30000;		//um
	m_lTestDist[3] = 40000;		//um
	m_lTestDist[4] = 55000;		//um
	
	//SetStepSpeed(0);

	m_nTestEvent = nEvent;

	if (m_nTestEvent == TE_TEST_EVENT_IDLE)
		m_nTestState = TE_TEST_STATE_IDLE;
	else
	{
		theServo.MoveBondHeadToSafePos();
		Sleep_x1ms(1000);

		HomeIndexer();
		Sleep_x1ms(1500);
	    m_nTestState = TE_TEST_STATE_WAIT_HOME;
	}
}

void CTopEliteIndex::ProcessDryRunEvent(void)
{
	switch(m_nTestState)
	{
	case TE_TEST_STATE_WAIT_HOME:
	{
		if (theStepper.IsHome(m_nIndexStepID) == false) break;

		theStepper.MoveDist(m_nIndexStepID, theMcData.GetRcd()->lZeroOffset);
		m_nTestState = TE_TEST_STATE_WAIT_CENTER;
		break;
	}
	case TE_TEST_STATE_WAIT_CENTER:
	{
		if (theStepper.IsBusy(m_nIndexStepID)) break;
		if (theStepper.IsBusy(theTopClamp.GetStepID())) break;

		theStepper.MoveDist(m_nIndexStepID, -m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
		m_nTestState = TE_TEST_STATE_WAIT_LEFT;
		break;
	}
	case TE_TEST_STATE_WAIT_LEFT:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			theStepper.MoveDist(m_nIndexStepID, m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
			m_nTestState = TE_TEST_STATE_WAIT_LBACK;
		}
		break;
	}
	case TE_TEST_STATE_WAIT_LBACK:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			theStepper.MoveDist(m_nIndexStepID, m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
			m_nTestState = TE_TEST_STATE_WAIT_RIGHT;
		}
		break;
	}
	case TE_TEST_STATE_WAIT_RIGHT:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			theStepper.MoveDist(m_nIndexStepID, -m_lTestDist[m_nTestTicker], MOTION_RELATIVE);
			m_nTestState = TE_TEST_STATE_WAIT_RBACK;
		}
		break;
	}
	case TE_TEST_STATE_WAIT_RBACK:
	{
		if (theStepper.IsBusy(m_nIndexStepID) == false)
		{
			if (m_nTestTicker == MAX_DEBUG_STEP - 1)
				gnInc = -1;
			else if (m_nTestTicker == 0)
				gnInc = 1;

			m_nTestTicker = m_nTestTicker + gnInc;

			m_lStartTime = GetCurrentTime();
		    m_nTestState =TE_TEST_STATE_WAIT_CHANGE_PROFILE;
		}
		break;
	}
	case TE_TEST_STATE_WAIT_CHANGE_PROFILE:
	{
		if (GetCurrentTime() - m_lStartTime < 300) break;

		m_nTestState = TE_TEST_STATE_WAIT_CLAMP_OPEN;
		m_lStartTime = GetCurrentTime();
		theTopClamp.MoveClampSafePos();
		break;
	}
	case TE_TEST_STATE_WAIT_CLAMP_OPEN:
	{
		if (GetCurrentTime() - m_lStartTime < 300) break;

		m_nTestState = TE_TEST_STATE_WAIT_CLAMP_CLOSE;
		theTopClamp.CloseClamp();
		m_lStartTime = GetCurrentTime();
		break;
	}
	case TE_TEST_STATE_WAIT_CLAMP_CLOSE:
	{
		if (GetCurrentTime() - m_lStartTime < 500) break;

		m_nTestState = TE_TEST_STATE_WAIT_CENTER;
		break;
	}
	default:
		break;
	}
}

void CTopEliteIndex::ProcessGripperDryRunEvent(void)
{
    switch (m_nGripperDryCycleState)
    {
    case TE_CLAW_DRYRUN_WAIT_CLAMP:
	{
		if (m_GripperDryRunTimer.GetTime_x1ms() < m_lClawInterval) break;

		m_Injector.SetRelayState(RELAY_ON);
		m_InputClaw.SetRelayState(TRUE);
		m_LFHolder.SetRelayState(RELAY_ON);
		m_OutputClaw.SetRelayState(TRUE);
		m_Ejector.SetRelayState(TRUE);

		m_GripperDryRunTimer.StartTimer();
		m_nGripperDryCycleState = TE_CLAW_DRYRUN_WAIT_OPEN;
        break;
	}
    case TE_CLAW_DRYRUN_WAIT_OPEN:
	{
		if (m_GripperDryRunTimer.GetTime_x1ms() < m_lClawInterval) break;

		m_Injector.SetRelayState(RELAY_OFF);
		m_InputClaw.SetRelayState(FALSE);
		m_LFHolder.SetRelayState(RELAY_OFF);
		m_OutputClaw.SetRelayState(FALSE);
		m_Ejector.SetRelayState(FALSE);

		m_GripperDryRunTimer.StartTimer();
		m_nGripperDryCycleState = TE_CLAW_DRYRUN_WAIT_CLAMP;
        break;
	}
	default:
        break;
    }
}

void CTopEliteIndex::SubmitGripperDryRunEvent(short nEvent)
{
	m_GripperDryRunTimer.StartTimer();
	m_nGripperDryCycleState = TE_CLAW_DRYRUN_WAIT_CLAMP;
}

bool CTopEliteIndex::IsTrackNeedClear(void)
{
	return m_bTrackNeedClear;
}

void CTopEliteIndex::SetTrackNeedClear(bool bNeedClear)
{
	m_bTrackNeedClear = bNeedClear;
}

void CTopEliteIndex::SetTailPos(long lOffset)
{
	m_stSlipOnTrack.lSlipTailPos = m_stSlipOnTrack.lSlipTailPos + lOffset;
}

short CTopEliteIndex::GetLfInjectUnit(void)
{
	return m_stIndexVar.nInjectUnit;
}

void CTopEliteIndex::SetTeaching(bool bTeaching)
{
	m_bTeaching = bTeaching;
}

BYTE CTopEliteIndex::InjectEnabled()
{
	return ((m_stIndexVar.nTotalUnits - m_nCurBondUnit <= m_stIndexRcd.nNextStepPusherIn)
				&& (m_InjectSensor.GetCurrentState() == SENSOR_OFF));
}

bool CTopEliteIndex::IsVacuumControl()
{
	return (m_stIndexRcd.bVacuumControl);
}

BYTE CTopEliteIndex::IsClampReady()
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

ST_TE_INDEX_RCD *CTopEliteIndex::GetRcd(void)
{
	return &m_stIndexRcd;
}

void CTopEliteIndex::VerifyData(void)
{
	ST_TE_INDEX_RCD stIndexRcd;
	theDataMgr.CopyFromBuffer((char *)&stIndexRcd, sizeof(ST_TE_INDEX_RCD));

	short nShow = theMcConfig.GetVerifyShowTrack();
	if (nShow == 0) return;

	theLog.SetLogMode(LOG_VERIFY_RECIPE);

	CString sMsg; bool bDiff;
	theLog.WriteLine(_T("\n[WH TRACK]"));
	//1st LF heat delay
	sMsg.Format(_T("HeatDelay(1st LF)= %d"), m_stIndexRcd.lInitDelay);
	bDiff = false;
	if (stIndexRcd.lInitDelay != m_stIndexRcd.lInitDelay)
	{
		bDiff = true;
		sMsg.AppendFormat(_T(" (%d)"), stIndexRcd.lInitDelay);
	}
	if ((nShow==1) || ((nShow==2) && bDiff)) theLog.WriteLine(sMsg);
	//1st unit heat delay
	sMsg.Format(_T("HeatDelay(1st Unit)= %d"), m_stIndexRcd.l1stUnitHeatDelay);
	bDiff = false;
	if (stIndexRcd.l1stUnitHeatDelay != m_stIndexRcd.l1stUnitHeatDelay)
	{
		bDiff = true;
		sMsg.AppendFormat(_T(" (%d)"), stIndexRcd.l1stUnitHeatDelay);
	}
	if ((nShow==1) || ((nShow==2) && bDiff)) theLog.WriteLine(sMsg);
	//heat delay
	sMsg.Format(_T("HeatDelay= %d"), m_stIndexRcd.lHeatingDelay);
	bDiff = false;
	if (stIndexRcd.lHeatingDelay != m_stIndexRcd.lHeatingDelay)
	{
		bDiff = true;
		sMsg.AppendFormat(_T(" (%d)"), stIndexRcd.lHeatingDelay);
	}
	if ((nShow==1) || ((nShow==2) && bDiff)) theLog.WriteLine(sMsg);

	theLog.SetLogMode(LOG_MACHINE_DATA);
}

byte CTopEliteIndex::GetIndexStepID(void)
{
	return m_nIndexStepID;
}