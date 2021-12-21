/***************************************************************
MaterialHandlerThread.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "StepperControl.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledClamp.h"
#include "VledOnload.h"
#include "VledOffload.h"
#include "MachineConfig.h"
#include "TowerLight.h"
#include "Bond.h" 
#include "WireBond.h"
#include "MsgManager.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "VisionSystem.h"
#include "IniFile.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "VEClamp.h"
#include "VEIndex.h"
#include "VEOnload.h"
#include "VEOffload.h"
#include "CuIndex.h"
#include "CuClamp.h"
#include "CuOnload.h"
#include "CuOffload.h"
#include "R2RIndex.h"
#include "R2ROnload.h"
#include "R2ROffload.h"
#include "Debug.h"
#include "servo.h"
#include "DlgElevCtrl.h"
#include "CanController.h"
#include "FoxconnIndex.h"
#include "TopEliteIndex.h"

CMaterialHandlerThread theMHS;

// CMaterialHandlerThread

CMaterialHandlerThread::CMaterialHandlerThread()
: m_pMhsThread(NULL)
, m_bRequestIndex(false)
, m_bThreadActive(false)
, m_bAutoIndex(false)
, m_bBusy(false)
, m_bDryRun(false)
, m_bStopAll(false)
, m_bSensorEnable(true)
, m_bThreadStop(false)
, m_bLastLf(false)
, m_bOxidationProtect(false)
, m_lOnloadFullStartTime(0)
, m_lOffloadFullStartTime(0)
, m_lOnloadRefreshIOStartTime(0)
, m_lOffloadRefreshIOStartTime(0)
{
}

CMaterialHandlerThread::~CMaterialHandlerThread()
{
}

// Perform material handle tasks
UINT PerformMaterialHandle(LPVOID pParam)
{
    long lStartTime = GetCurrentTime(), lElapseTime;

	while (theMHS.IsThreadActive())
	{
		if (theMHS.IsThreadStop())
		{
			Sleep(100);
			continue;
		}

		switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
		{
		case MHS_TOP_LED:
		{
			if (theTopTrack.IsVacuumControl()
				&& theMcConfig.IsVacAutoCutoff()	//v3.0.T340 add
				&& !theBond.IsAutoBonding()			//v3.0.T360 add
				&& theTopClamp.IsVacuumOn()		//v3.0.T378 edit
				&& (theTopClamp.CheckVacuumStatus() == FNOK)
				&& (theTopClamp.GetVacDuration() > 3000))	//v3.0.T378 edit
			{
				theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
			}

			if (theMHS.IsAutoIndex() || theMHS.IsBusy())
			{
				theMHS.SetBusy(false);

				//debug  18K/Hour
				if (theMHS.IsDryRun())
				{
					lElapseTime = GetCurrentTime() - lStartTime;
					if (lElapseTime > 8000)
					{
						if ((theMHS.IsRequestIndex() == false))
						{
							theMHS.SetRequestIndex(true);
							lStartTime = GetCurrentTime();
						}
					}
				}

				long lDuration = thePgm.GetRcd()->lRelaySafetyDuration; 
				if ((theTopTrack.m_InputClaw.GetDuration() > lDuration)
					|| (theTopTrack.m_OutputClaw.GetDuration() > lDuration)
					|| (theTopTrack.m_Ejector.GetDuration() > lDuration)
					|| (theTopTrack.m_Injector.GetDuration() > lDuration))
				{
					theTopTrack.ResetRelayErrorState();
				}

				////offloader empty
				//theTopOffloader.m_MagDetectedSensor.UpdateSensorState();
				//if(theTopOffloader.m_MagDetectedSensor.GetSensorState()==TRUE)
				//    theTopOffloader.SetEmptyReported(false);

				//Onload
				if (theTopOnloader.IsValid())
				{
					if (theTopOnloader.GetEvent() == TOP_ONLOAD_EVENT_IDLE)
					{
						if (theTopTrack.m_bRequestOnload)
						{
							if (theTopOnloader.IsAfterInit())
							{
								theTopOnloader.SubmitOnloadEvent(TOP_ONLOAD_EVENT_FIRST);
								theTopOnloader.ResetAfterInit();
							}
							else
							{
								theTopOnloader.SubmitOnloadEvent(TOP_ONLOAD_EVENT_START);
							}
						}
						
						if (GetCurrentTime() - theMHS.m_lOnloadRefreshIOStartTime > 1000)
						{
							//theTopOnloader.UpdateAllSensor();
							theTopOnloader.m_MagDetectedSensor.UpdateSensorState();
							if (theTopOnloader.m_MagDetectedSensor.GetSensorState() == TRUE)
								theTopOnloader.SetEmpty(false);
							else
								theTopOnloader.SetEmpty(true);

							theTopOnloader.m_MagBufferPresentSensor.UpdateSensorState();
							if (theTopOnloader.m_MagBufferPresentSensor.GetSensorState() == TRUE)
								theTopOnloader.SetBufferEmpty(false);
							else
								theTopOnloader.SetBufferEmpty(true);

							theTopOnloader.m_PullyMagExistSensor.UpdateSensorState();
							if (theTopOnloader.m_PullyMagExistSensor.GetSensorState() == FALSE)
							{
								theTopOnloader.SetPusherFull(false);
								theMHS.m_lOnloadFullStartTime = GetCurrentTime();
							}
							else
							{
								if (GetCurrentTime() - theMHS.m_lOnloadFullStartTime>2000)
									theTopOnloader.SetPusherFull(true);
							}

							theMHS.m_lOnloadRefreshIOStartTime=GetCurrentTime();
						}
					}
					else if (theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
						theTopOnloader.ProcessOnloadEvent();

					if (theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
						 theMHS.SetBusy(true);
				}

				//Offload
				if (theTopOffloader.IsValid())
				{
					if (theTopOffloader.GetEvent() == TOP_OFFLOAD_EVENT_IDLE)
					{
						if (theTopTrack.IsOffloadEnable())
						{
						   theTopOffloader.SubmitOffloadEvent(TOP_OFFLOAD_EVENT_START);
						   theTopTrack.SetOffloadEnable(false); 
						}

						if (GetCurrentTime() - theMHS.m_lOffloadRefreshIOStartTime > 1000)
						{
							//theTopOffloader.UpdateAllSensor();
							theTopOffloader.m_MagDetectedSensor.UpdateSensorState();
							if (theTopOffloader.m_MagDetectedSensor.GetSensorState() == TRUE)
								theTopOffloader.SetEmpty(false);
							else
								theTopOffloader.SetEmpty(true);

							theTopOffloader.m_MagBufferPresentSensor.UpdateSensorState();
							if (theTopOffloader.m_MagBufferPresentSensor.GetSensorState() == TRUE)
								theTopOffloader.SetBufferEmpty(false);
							else
								theTopOffloader.SetBufferEmpty(true);
							theTopOffloader.m_PullyMagExistSensor.UpdateSensorState();
							if (theTopOffloader.m_PullyMagExistSensor.GetSensorState() == FALSE)
							{
								theTopOffloader.SetPusherFull(false);
								theMHS.m_lOffloadFullStartTime = GetCurrentTime();
							}
							else
							{
								if (GetCurrentTime() - theMHS.m_lOffloadFullStartTime>2000)
									theTopOffloader.SetPusherFull(true);
							}
							theMHS.m_lOffloadRefreshIOStartTime=GetCurrentTime();
						}

		    		}
					else if (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)
							theTopOffloader.ProcessOffloadEvent();

					if (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)
						theMHS.SetBusy(true);
				}
			}

			theTowerLight.BlinkYellowLight();
			theTowerLight.BlinkRedLight();
			break;
		}
		case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
		{
			if (theTEIndex.IsVacuumControl()
				&& theMcConfig.IsVacAutoCutoff()
				&& !theBond.IsAutoBonding()
				&& theTopClamp.IsVacuumOn()
				&& (theTopClamp.CheckVacuumStatus() == FNOK)
				&& (theTopClamp.GetVacDuration() > 3000))
			{
				theTopClamp.SetVacuumOn(false);
			}

			if (theMHS.IsAutoIndex() || theMHS.IsBusy())
			{
				theMHS.SetBusy(false);

				//debug  18K/Hour
				if (theMHS.IsDryRun())
				{
					lElapseTime = GetCurrentTime() - lStartTime;
					if (lElapseTime > 8000)
					{
						if ((theMHS.IsRequestIndex() == false))
						{
							theMHS.SetRequestIndex(true);
							lStartTime = GetCurrentTime();
						}
					}
				}

				long lDuration = thePgm.GetRcd()->lRelaySafetyDuration; 
				if ((theTEIndex.m_InputClaw.GetDuration() > lDuration)
					|| (theTEIndex.m_OutputClaw.GetDuration() > lDuration)
					|| (theTEIndex.m_Ejector.GetDuration() > lDuration)
					|| (theTEIndex.m_Injector.GetDuration() > lDuration))
				{
					theTEIndex.ResetRelayErrorState();
				}

				//Onload
				if (theTopOnloader.IsValid())
				{
					if (theTopOnloader.GetEvent() == TOP_ONLOAD_EVENT_IDLE)
					{
						if (theTEIndex.m_bRequestOnload)
						{
							if (theTopOnloader.IsAfterInit())
							{
								theTopOnloader.SubmitOnloadEvent(TOP_ONLOAD_EVENT_FIRST);
								theTopOnloader.ResetAfterInit();
							}
							else
								theTopOnloader.SubmitOnloadEvent(TOP_ONLOAD_EVENT_START);
						}
						
						if (GetCurrentTime() - theMHS.m_lOnloadRefreshIOStartTime > 1000)
						{
							//theTopOnloader.UpdateAllSensor();
							theTopOnloader.m_MagDetectedSensor.UpdateSensorState();
							if (theTopOnloader.m_MagDetectedSensor.GetSensorState() == TRUE)
								theTopOnloader.SetEmpty(false);
							else
								theTopOnloader.SetEmpty(true);

							theTopOnloader.m_MagBufferPresentSensor.UpdateSensorState();
							if (theTopOnloader.m_MagBufferPresentSensor.GetSensorState() == TRUE)
								theTopOnloader.SetBufferEmpty(false);
							else
								theTopOnloader.SetBufferEmpty(true);

							theTopOnloader.m_PullyMagExistSensor.UpdateSensorState();
							if (theTopOnloader.m_PullyMagExistSensor.GetSensorState() == FALSE)
							{
								theTopOnloader.SetPusherFull(false);
								theMHS.m_lOnloadFullStartTime = GetCurrentTime();
							}
							else
							{
								if (GetCurrentTime() - theMHS.m_lOnloadFullStartTime>2000)
									theTopOnloader.SetPusherFull(true);
							}

							theMHS.m_lOnloadRefreshIOStartTime=GetCurrentTime();
						}
					}
					else if (theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
						theTopOnloader.ProcessOnloadEvent();

					if (theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
						 theMHS.SetBusy(true);
				}

				//Offload
				if (theTopOffloader.IsValid())
				{
					if (theTopOffloader.GetEvent() == TOP_OFFLOAD_EVENT_IDLE)
					{
						if (theTEIndex.IsOffloadEnable())
						{
						   theTopOffloader.SubmitOffloadEvent(TOP_OFFLOAD_EVENT_START);
						   theTEIndex.SetOffloadEnable(false); 
						}

						if (GetCurrentTime() - theMHS.m_lOffloadRefreshIOStartTime > 1000)
						{
							//theTopOffloader.UpdateAllSensor();
							theTopOffloader.m_MagDetectedSensor.UpdateSensorState();
							if (theTopOffloader.m_MagDetectedSensor.GetSensorState() == TRUE)
								theTopOffloader.SetEmpty(false);
							else
								theTopOffloader.SetEmpty(true);

							theTopOffloader.m_MagBufferPresentSensor.UpdateSensorState();
							if (theTopOffloader.m_MagBufferPresentSensor.GetSensorState() == TRUE)
								theTopOffloader.SetBufferEmpty(false);
							else
								theTopOffloader.SetBufferEmpty(true);
							theTopOffloader.m_PullyMagExistSensor.UpdateSensorState();
							if (theTopOffloader.m_PullyMagExistSensor.GetSensorState() == FALSE)
							{
								theTopOffloader.SetPusherFull(false);
								theMHS.m_lOffloadFullStartTime = GetCurrentTime();
							}
							else
							{
								if (GetCurrentTime() - theMHS.m_lOffloadFullStartTime>2000)
									theTopOffloader.SetPusherFull(true);
							}
							theMHS.m_lOffloadRefreshIOStartTime=GetCurrentTime();
						}

		    		}
					else if (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)
							theTopOffloader.ProcessOffloadEvent();

					if (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE)
						theMHS.SetBusy(true);
				}
			}

			theTowerLight.BlinkYellowLight();
			theTowerLight.BlinkRedLight();
			break;
		}
		case MHS_FOXCONN:
		{
			if (theMHS.IsAutoIndex() || theMHS.IsBusy())
			{
				theMHS.SetBusy(false);

				if (theMHS.IsDryRun())
				{
					lElapseTime = GetCurrentTime() - lStartTime;
					if (lElapseTime > 8000)
					{
						if ((theMHS.IsRequestIndex() == false))
						{
							theMHS.SetRequestIndex(true);
							lStartTime = GetCurrentTime();
						}
					}
				}
			}

			theTowerLight.BlinkYellowLight();
			theTowerLight.BlinkRedLight();
			break;
		}
		case MHS_R2R:
		{
			TOP_TRACK_DATA stTrackData;
			theR2RTrack.GetTopTrackRcd(&stTrackData);
			if (stTrackData.bVacuumControl
				&& theMcConfig.IsVacAutoCutoff()	//v3.0.T340 add
				&& !theBond.IsAutoBonding()			//v3.0.T360 add
				&& theTopClamp.IsVacuumOn()		//v3.0.T378 edit
				&& (theTopClamp.CheckVacuumStatus() == FNOK)
				&& (theTopClamp.GetVacDuration() > 3000))	//v3.0.T378 edit
			{
				theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
			}

			//Offloader down
			theR2ROffloader.theStopBondSnr.UpdateSensorState();
			if (theR2ROffloader.theStopBondSnr.GetSensorState() == TRUE)
				theR2ROffloader.SetOffloadErrorCode(MHS_R2R_OFFLOAD_STOP);

			theR2ROffloader.theErrorSnr.UpdateSensorState();
			if (theR2ROffloader.theErrorSnr.GetSensorState() == TRUE)
				theR2ROffloader.SetOffloadErrorCode(MHS_R2R_OFFLOAD_ERROR);

			//Onloader down
			theR2ROnloader.theStopBondSnr.UpdateSensorState();
			if (theR2ROnloader.theStopBondSnr.GetSensorState() == TRUE)
				theR2ROnloader.SetOnloadErrorCode(MHS_R2R_ONLOAD_STOP);

			theR2ROnloader.theErrorSnr.UpdateSensorState();
			if (theR2ROnloader.theErrorSnr.GetSensorState() == TRUE)
				theR2ROnloader.SetOnloadErrorCode(MHS_R2R_ONLOAD_ERROR);

			if (theMHS.IsAutoIndex() || theMHS.IsBusy())
			{
				theMHS.SetBusy(false);

				//debug  18K/Hour
				if (theMHS.IsDryRun())
				{
					lElapseTime = GetCurrentTime() - lStartTime;
					if (lElapseTime > 8000)
					{
						if (theMHS.IsRequestIndex() == false)
						{
							theMHS.SetRequestIndex(true);
							lStartTime = GetCurrentTime();
						}
					}
				}
				//track for over heat
				if (theR2RTrack.theInputClaw.GetDuration() > RELAY_SAFTY_DURATION  ||
					theR2RTrack.theOutputClaw.GetDuration() > RELAY_SAFTY_DURATION ||
					theR2RTrack.theEjector.GetDuration() > RELAY_SAFTY_DURATION ||
					theR2RTrack.theInjector.GetDuration() > RELAY_SAFTY_DURATION)
				{
					theR2RTrack.ResetRelayErrorState();
				}
			}

			theTowerLight.BlinkYellowLight();
			theTowerLight.BlinkRedLight();
			break;
		}
		case MHS_CU_ELITE:
		{
			CUELITE_TRACK_DATA stTrackData;
			theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);
			if (stTrackData.bVacuumControl
				&& theMcConfig.IsVacAutoCutoff()	//v3.0.T340 add
				&& !theBond.IsAutoBonding()			//v3.0.T360 add
				&& (theCuEliteLFClamp.m_VacuumValve.GetRelayState() == RELAY_ON)
				&& (theCuEliteLFClamp.CheckVacuumStatus() == FNOK)
				&& (theCuEliteLFClamp.m_VacuumValve.GetDuration() > 3000))
			{
				theCuEliteLFClamp.m_VacuumValve.SetRelayState(RELAY_OFF);
			}

			if (theMHS.IsAutoIndex() || theMHS.IsBusy())
			{
				theMHS.SetBusy(false);

				//debug  18K/Hour
				if (theMHS.IsDryRun())
				{
					lElapseTime = GetCurrentTime() - lStartTime;
					if (lElapseTime > 8000)
					{
						if ((theMHS.IsRequestIndex() == false))
						{
							theMHS.SetRequestIndex(true);
							lStartTime = GetCurrentTime();
						}
					}
				}

				long lDuration = thePgm.GetRcd()->lRelaySafetyDuration;
				if ((theCuEliteTrack.theInputClaw.GetDuration() > lDuration)
					|| (theCuEliteTrack.theOutputClaw.GetDuration() > lDuration)
					|| (theCuEliteTrack.theEjector.GetDuration() > lDuration)
					|| (theCuEliteTrack.theInjector.GetDuration() > lDuration))
				{
					theCuEliteTrack.ResetRelayErrorState();
				}

				//offloader empty
				//theCuEliteOffloader.theMagDetectedSensor.UpdateSensorState();
				//if(theCuEliteOffloader.theMagDetectedSensor.GetSensorState()==TRUE)
				//    theCuEliteOffloader.SetOffloadEmptyReportedflag(false);

				//Onload
				if (theCuEliteOnloader.GetOnloadValidState() == TRUE)
				{
					if (theCuEliteOnloader.GetOnloadEvent() == CUELITE_ONLOAD_EVENT_IDLE)
					{
						if (theCuEliteTrack.m_bRequestOnloadFlag == true)
						{
							if (theCuEliteOnloader.GetOnloaderInitStatus() == TRUE)  //@25
							{
								theCuEliteOnloader.SubmitOnloadEvent(CUELITE_ONLOAD_EVENT_FIRST);
								theCuEliteOnloader.ResetOnloadInitStatus();
							}
							else
							{
								theCuEliteOnloader.SubmitOnloadEvent(CUELITE_ONLOAD_EVENT_START);
							}
						}
						if (GetCurrentTime() - theMHS.m_lOnloadRefreshIOStartTime > 1000)
						{
							//theCuEliteOnloader.UpdateAllSensor();
							theCuEliteOnloader.theMagDetectedSensor.UpdateSensorState();
							if (theCuEliteOnloader.theMagDetectedSensor.GetSensorState() == TRUE)
								theCuEliteOnloader.SetOnloadEmptyflag(false);
							else
								theCuEliteOnloader.SetOnloadEmptyflag(true);

							if (theCuEliteOnloader.theMagBufferPresentSensor.GetCurrentState() == SENSOR_ON)
								theCuEliteOnloader.SetOnloadBufferEmptyflag(false);
							else
								theCuEliteOnloader.SetOnloadBufferEmptyflag(true);

							theCuEliteOnloader.thePullyMagExistSensor.UpdateSensorState();
							if (theCuEliteOnloader.thePullyMagExistSensor.GetSensorState() == FALSE)
							{
								theCuEliteOnloader.SetOnloadPullyFullflag(false);
								theMHS.m_lOnloadFullStartTime = GetCurrentTime();
							}
							else
							{
								if (GetCurrentTime() - theMHS.m_lOnloadFullStartTime>2000)
									theCuEliteOnloader.SetOnloadPullyFullflag(true);
							}

							theMHS.m_lOnloadRefreshIOStartTime=GetCurrentTime();
						}
					}
					else if (theCuEliteOnloader.GetOnloadEvent() != CUELITE_ONLOAD_EVENT_IDLE)
						theCuEliteOnloader.ProcessOnloadEvent();

					if (theCuEliteOnloader.GetOnloadEvent() != CUELITE_ONLOAD_EVENT_IDLE)
						theMHS.SetBusy(true);
				}

				//Offload
				if (theCuEliteOffloader.GetOffloadValidState() == TRUE)
				{
					if (theCuEliteOffloader.GetOffloadEvent() == CUELITE_OFFLOAD_EVENT_IDLE)
					{
						if (theCuEliteTrack.GetOffloadEnableFlag() == true)
						{
							theCuEliteOffloader.SubmitOffloadEvent(CUELITE_OFFLOAD_EVENT_START);
							theCuEliteTrack.SetOffloadEnableFlag(false);
						}

						if (GetCurrentTime() - theMHS.m_lOffloadRefreshIOStartTime > 1000)
						{
							//theCuEliteOffloader.UpdateAllSensor();
							theCuEliteOffloader.theMagDetectedSensor.UpdateSensorState();
							if (theCuEliteOffloader.theMagDetectedSensor.GetSensorState() == TRUE)
								theCuEliteOffloader.SetOffloadEmptyflag(false);
							else
								theCuEliteOffloader.SetOffloadEmptyflag(true);

							theCuEliteOffloader.theMagBufferPresentSensor.UpdateSensorState();
							if (theCuEliteOffloader.theMagBufferPresentSensor.GetSensorState() == TRUE)
								theCuEliteOffloader.SetOffloadBufferEmptyflag(false);
							else
								theCuEliteOffloader.SetOffloadBufferEmptyflag(true);
							theCuEliteOffloader.thePullyMagExistSensor.UpdateSensorState();
							if (theCuEliteOffloader.thePullyMagExistSensor.GetSensorState() == FALSE)
							{
								theCuEliteOffloader.SetOffloadPullyFullflag(false);
								theMHS.m_lOffloadFullStartTime = GetCurrentTime();
							}
							else
							{
								if (GetCurrentTime() - theMHS.m_lOffloadFullStartTime>2000)
									theCuEliteOffloader.SetOffloadPullyFullflag(true);
							}
							theMHS.m_lOffloadRefreshIOStartTime=GetCurrentTime();
						}

					}
					else if (theCuEliteOffloader.GetOffloadEvent() != CUELITE_OFFLOAD_EVENT_IDLE)
						theCuEliteOffloader.ProcessOffloadEvent();

					if (theCuEliteOffloader.GetOffloadEvent() != CUELITE_OFFLOAD_EVENT_IDLE)
						theMHS.SetBusy(true);
				}
			}

			theTowerLight.BlinkYellowLight();
			theTowerLight.BlinkRedLight();
			break;
		}
		case MHS_VLED_ELITE:
		{
			theVEliteTrack.theOnloadTrackSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_ON);

			theVEliteOnload.theForkExistSensor.UpdateSensorState();
			if (theVEliteOnload.theForkExistSensor.GetSensorState() == SENSOR_OFF)
			{
				theVEliteOnload.ResetEmpty();
			}

			if (theMHS.IsAutoIndex() || theMHS.IsBusy() || theVEliteTrack.IsOffloadEnable())
			{
				theMHS.SetBusy(false);

				//onloader:
				if (theVEliteOnload.IsValid() && 
					(theVEliteOnload.GetErrorCode() == MHS_NO_ERROR))
				{
					if ((theVEliteOnload.GetEvent() == ONLOAD_IDLE) &&
						(!theVEliteOnload.IsEmpty()) &&
						(theMHS.IsAutoIndex() == true) &&
						(theMHS.IsLastLf() == false))
					{
						theVEliteTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_OFF);
						theVEliteOnload.theForkExistSensor.UpdateSensorState();

						if (((theVEliteTrack.theOnloadTrackSensor.GetSensorState() == SENSOR_OFF) &&
							(theVEliteTrack.thePreBondSensor.GetSensorState() == SENSOR_OFF) &&
							(theVEliteOnload.theForkExistSensor.GetSensorState() == SENSOR_ON) &&
							(theVEliteTrack.GetInjectPosState() == INJECT_POS_HOME)) ||
							theMHS.IsDryRun())
						{
							theVEliteOnload.SubmitEvent(ONLOAD_EVENT_START);
						}
					}
					else if (theVEliteOnload.GetEvent() != ONLOAD_IDLE)
					{
						theVEliteOnload.ProcessEvent();
					}

					if (theVEliteOnload.GetEvent() != ONLOAD_IDLE)
						theMHS.SetBusy(true);
				}

				//track:::Injector
				if ((theVEliteTrack.GetTrackErrorCode() == MHS_NO_ERROR) &&
					(theVEliteTrack.GetInjectErrorCode() == MHS_NO_ERROR) &&
					theVEliteTrack.IsTrackValid())
				{
					if ((theVEliteTrack.GetInjectEvent() == INJECT_EVENT_IDLE) &&
						theMHS.IsAutoIndex())
					{                    
						if (theVEliteTrack.GetInjectPosState() == INJECT_POS_HOME)
						{
							theVEliteTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_OFF);
							if ((theVEliteTrack.theOnloadTrackSensor.GetSensorState() == SENSOR_ON) &&
								(theVEliteTrack.thePreBondSensor.GetSensorState() == SENSOR_OFF))
								theVEliteTrack.SubmitInjectEvent(INJECT_EVENT_PUSH_TO_CHECK_POS);
						}
						else
						{
							if ((theVEliteTrack.theLeftClaw.GetClawTicker() == 0) &&
								theVEliteTrack.theLeftClaw.IsIndexEmpty())
							{
								theVEliteTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_OFF);

								if (theVEliteTrack.thePreBondSensor.GetSensorState() == SENSOR_OFF)
									theVEliteTrack.SubmitInjectEvent(INJECT_EVENT_PUSH_TO_INJECT_POS);
							}
						}
					}
					else if (theVEliteTrack.GetInjectEvent() != INJECT_EVENT_IDLE)
						theVEliteTrack.ProcessInjectEvent();

					if (theVEliteTrack.GetInjectEvent() != INJECT_EVENT_IDLE)
						theMHS.SetBusy(true);
				}

				//debug 18K/Hour
				if (theMHS.IsDryRun())
				{
					lElapseTime = GetCurrentTime() - lStartTime;
					if (lElapseTime > 260)
					{
						if ((theMHS.IsRequestIndex() == false))
						{
							theMHS.SetRequestIndex(true);
							lStartTime = GetCurrentTime();
						}
					}
				}

				if (theVEliteOffload.GetEvent() == VE_OFFLOAD_EVENT_IDLE)
				{
					if (theVEliteTrack.IsOffloadEnable() || theMHS.IsDryRun())
					{
						if (theVEliteTrack.theRightClaw.IsBusy() == false)
						{
							theVEliteOffload.SubmitEvent(VE_OFFLOAD_EVENT_OFFLOAD);
							theVEliteTrack.SetOffloadEnable(false);
						}
					}
				}
				else
				{
					theVEliteOffload.ProcessEvent();
				}

				if ((theVEliteOffload.GetEvent() != VE_OFFLOAD_EVENT_IDLE) && 
					(theVEliteOffload.GetErrorCode() == MHS_NO_ERROR))
				{
					theMHS.SetBusy(true);
				}

			}

			theTowerLight.BlinkRedLight();
		}
		case MHS_VLED:
		{
			theVledTrack.theOnloadTrackSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_ON);

			theVledOnload.theForkExistSensor.UpdateSensorState(); //@20
			if (theVledOnload.theForkExistSensor.GetSensorState() == SENSOR_OFF)
			{
				theVledOnload.ResetOnloadEmptyFlag();
			}

			theVledOffload.theOffloadForkExistSensor.UpdateSensorState();
			if (theVledOffload.theOffloadForkExistSensor.GetSensorState() == SENSOR_OFF)
			{
				theVledOffload.ResetForkFullFlag();
			}

			if (theMHS.IsAutoIndex() || theMHS.IsBusy() || theVledTrack.GetOffloadEnableFlag() == true) //@17
			{
				theMHS.SetBusy(false);

				//onloader:
				if ((theVledOnload.GetOnloadValidState() == TRUE) && 
					(theVledOnload.GetOnloadErrorCode() == MHS_NO_ERROR))
				{
					if ((theVledOnload.GetOnloadEvent() == ONLOAD_IDLE) &&
						(theVledOnload.GetOnloadEmptyFlag() == false) &&
						(theMHS.IsAutoIndex() == true) &&
						(theMHS.IsLastLf() == false))   //@10
					{
						theVledTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_OFF);
						theVledOnload.theForkExistSensor.UpdateSensorState();

						if (((theVledTrack.theOnloadTrackSensor.GetSensorState() == SENSOR_OFF) &&
							(theVledTrack.thePreBondSensor.GetSensorState() == SENSOR_OFF) &&
							(theVledOnload.theForkExistSensor.GetSensorState() == SENSOR_ON) &&
							(theVledTrack.GetInjectorPosState() == INJECT_POS_HOME)) ||  
							theMHS.IsDryRun())
						{
							theVledOnload.SubmitOnloadEvent(ONLOAD_EVENT_START);
						}
					}
					else if (theVledOnload.GetOnloadEvent() != ONLOAD_IDLE)
					{
						theVledOnload.ProcessOnloadEvent();
					}

					if (theVledOnload.GetOnloadEvent() != ONLOAD_IDLE)
						theMHS.SetBusy(true);
				}

				//track:::Injector
				if ((theVledTrack.GetTrackErrorCode() == MHS_NO_ERROR) &&
					(theVledTrack.GetInjectErrorCode() == MHS_NO_ERROR) &&
					(theVledTrack.GetTrackValidState() == TRUE))
				{
					if ((theVledTrack.GetInjectorEvent() == INJECT_EVENT_IDLE) &&
						theMHS.IsAutoIndex())
					{                    
						if (theVledTrack.GetInjectorPosState() == INJECT_POS_HOME)
						{
							theVledTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_OFF);
							if ((theVledTrack.theOnloadTrackSensor.GetSensorState() == SENSOR_ON) &&
								(theVledTrack.thePreBondSensor.GetSensorState() == SENSOR_OFF))
								theVledTrack.SubmitInjectorEvent(INJECT_EVENT_PUSH_TO_CHECK_POS);
						}
						else //if (theVledTrack.GetInjectorPosState() == INJECT_POS_CHECK_DOUBLE_LF) //@15
						{
							if (theVledTrack.theLeftClaw.GetClawTicker() == 0 &&
								theVledTrack.theLeftClaw.IsIndexEmpty()) //@15
							{
								theVledTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_OFF);

								if (theVledTrack.thePreBondSensor.GetSensorState() == SENSOR_OFF)
									theVledTrack.SubmitInjectorEvent(INJECT_EVENT_PUSH_TO_INJECT_POS);
							}
						}
					}
					else if (theVledTrack.GetInjectorEvent() != INJECT_EVENT_IDLE)
						theVledTrack.ProcessInjectorEvent();

					if (theVledTrack.GetInjectorEvent() != INJECT_EVENT_IDLE)
						theMHS.SetBusy(true);
				}

				//debug  18K/Hour
				if (theMHS.IsDryRun())
				{
					lElapseTime = GetCurrentTime() - lStartTime;
					if (lElapseTime > 260)
					{
						if ((theMHS.IsRequestIndex() == false))
						{
							theMHS.SetRequestIndex(true);
							lStartTime = GetCurrentTime();
						}
					}
				}

				//offload:
				if ((theVledOffload.GetOffloadValidState() == TRUE) &&
					(theVledOffload.GetOffloadErrorCode() == MHS_NO_ERROR))
				{
					if (theVledOffload.GetOffloadEvent() == FORK_OFFLOAD_EVENT_IDLE)
					{
						if (theVledTrack.GetOffloadEnableFlag() == true || theMHS.IsDryRun())
						{
							theVledOffload.theOffloadForkExistSensor.UpdateSensorState();
							if ((theVledTrack.theRightClaw.IsBusy() == false) &&
								(theVledOffload.theOffloadForkExistSensor.GetSensorState() != FALSE))
							{
								theVledOffload.SubmitOffloadEvent();
								theVledTrack.SetOffloadEnableFlag(false);
							}
						}
					}
					else
					{
						theVledOffload.theOffloadForkExistSensor.UpdateSensorState();
						if (theVledOffload.theOffloadForkExistSensor.GetSensorState() != FALSE)
						{
							theVledOffload.ProcessOffloadEvent();
						}
						//else //@20
						//    theVledOffload.m_nOffloadErrorCode = MHS_OFFLOAD_FORK_NOT_EXIST;
					}

					if (theVledOffload.GetOffloadEvent() != FORK_OFFLOAD_EVENT_IDLE && 
						theVledOffload.GetOffloadErrorCode() == MHS_NO_ERROR)
					{
						theMHS.SetBusy(true);
					}
				}
			}
			theTowerLight.BlinkRedLight();
			break;
		}
		}

		Sleep(10);
	}

    return 0;
}

UINT CMaterialHandlerThread::VEliteWin32Timing(void)
{
    SINGLE_CAP_TRACK_RCD stLfTrackRcd;

    while (theMHS.IsThreadActive())
    {
        if (theMHS.IsThreadStop() == false)
        {
            if ((theMHS.IsRequestIndex()) &&
                theVEliteTrack.IsTrackValid() &&
                (theVEliteTrack.GetTrackErrorCode() == MHS_NO_ERROR))
			{
                if ((theVEliteTrack.GetIndexEvent() == TRACK_EVENT_IDLE) &&
                    //(((theVledOffload.CheckOffloadFull() == false) &&
                    //  (theVledOffload.theOffloadForkExistSensor.GetSensorState() != FALSE)) || 
                    // (theMHS.GetMhsDryRunState() == TRUE)) &&
                    theMHS.IsAutoIndex())
                {
                    if ((theVEliteTrack.GetBondUnitNum() == 0) &&
                        (theVEliteTrack.theLeftClaw.GetClawTicker() == 0))
                    {
                        theVEliteTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_ON);

                        if (theVEliteTrack.thePreBondSensor.GetSensorState() == SENSOR_ON)
                            theVEliteTrack.SubmitIndexEvent(TRACK_EVENT_FIRST_INDEX);
                    }
                    else
                        theVEliteTrack.SubmitIndexEvent(TRACK_EVENT_INDEX);
                }
                else if ((theVEliteTrack.GetIndexEvent() != TRACK_EVENT_IDLE)
						&& (theVEliteOffload.IsKickingLF() == false)
						&& (theVEliteOffload.IsError() == false))
                {
                    theVEliteTrack.ProcessIndexEvent();

                    if (theVEliteTrack.GetIndexEvent() == TRACK_EVENT_IDLE)
                    {
                        theVEliteTrack.GetRcd(&stLfTrackRcd);

                        if (stLfTrackRcd.chBondUnitType == 0)
                        {
                            theMHS.SetRequestIndex(false);
                            if (theDebug.IsDebugWorkHolder())
                                theDebug.AddInfo(_T("BondUnit"), theVEliteTrack.GetBondUnitNum());
                        }
                        else if (stLfTrackRcd.chBondUnitType == 1)
                        {
                            if (theVEliteTrack.GetBondUnitNum() % 2 == 1)
                            {
                                theMHS.SetRequestIndex(false);
                                if (theDebug.IsDebugWorkHolder())
                                    theDebug.AddInfo(_T("BondUnit"), theVEliteTrack.GetBondUnitNum());
                            }
                        }
                        else
                        {
                            if (theVEliteTrack.GetBondUnitNum() % 2 == 0)
                            {
                                theMHS.SetRequestIndex(false);
                                if (theDebug.IsDebugWorkHolder())
                                    theDebug.AddInfo(_T("BondUnit"), theVEliteTrack.GetBondUnitNum());
                            }
                        }
                    }
                }
            }
			else if (theMHS.IsRequestIndex() == false &&
				     theVEliteTrack.GetTrackErrorCode() == MHS_NO_ERROR)
			{
				if (theVEliteTrack.IsAvoidCrash())
				{
					if (theVEliteTrack.theLeftClaw.IsBusy() == false)
					{
						theVEliteTrack.theLeftClaw.AvoidCrashRollBack();
					    theVEliteTrack.SetAvoidCrash(false);
					}
				}
			}
        }

        Sleep(2);
    }

    return 0;
}

UINT CMaterialHandlerThread::VledWin32Timing(void)
{
    SINGLE_CAP_TRACK_RCD stLfTrackRcd;

    while (theMHS.IsThreadActive())
    {
        if (theMHS.IsThreadStop() == false)
        {
            if ((theMHS.IsRequestIndex()) &&
                (theVledTrack.GetTrackValidState() == TRUE) &&
                (theVledTrack.GetTrackErrorCode() == MHS_NO_ERROR))
			{
                if ((theVledTrack.GetIndexEvent() == TRACK_EVENT_IDLE)
					&& (((theVledOffload.CheckOffloadFull() == false)
					&& (theVledOffload.theOffloadForkExistSensor.GetSensorState() != FALSE))
					|| theMHS.IsDryRun())
					&& theMHS.IsAutoIndex())
                {
                    if ((theVledTrack.GetBondUnitNum() == 0) && //@15
                        (theVledTrack.theLeftClaw.GetClawTicker() == 0))
                    {
                        theVledTrack.thePreBondSensor.UpdateSensorState(theMHS.IsDryRun(), SENSOR_ON);

                        if (theVledTrack.thePreBondSensor.GetSensorState() == SENSOR_ON)
                            theVledTrack.SubmitIndexEvent(TRACK_EVENT_FIRST_INDEX);
                    }
                    else
                        theVledTrack.SubmitIndexEvent(TRACK_EVENT_INDEX);
                }
                else if (theVledTrack.GetIndexEvent() != TRACK_EVENT_IDLE)
                {
                    theVledTrack.ProcessIndexEvent();

                    if (theVledTrack.GetIndexEvent() == TRACK_EVENT_IDLE)
                    {
                        theVledTrack.GetLfTrackRcd(&stLfTrackRcd); //@15

                        if (stLfTrackRcd.chBondUnitType == 0)
                        {
                            theMHS.SetRequestIndex(false);
                            if (theDebug.IsDebugWorkHolder())
                                theDebug.AddInfo(_T("BondUnit"), theVledTrack.GetBondUnitNum());
                        }
                        else if (stLfTrackRcd.chBondUnitType == 1)
                        {
                            if (theVledTrack.GetBondUnitNum() % 2 == 1)
                            {
                                theMHS.SetRequestIndex(false);
                                if (theDebug.IsDebugWorkHolder())
                                    theDebug.AddInfo(_T("BondUnit"), theVledTrack.GetBondUnitNum());
                            }
                        }
                        else
                        {
                            if (theVledTrack.GetBondUnitNum() % 2 == 0)
                            {
                                theMHS.SetRequestIndex(false);
                                if (theDebug.IsDebugWorkHolder())
                                    theDebug.AddInfo(_T("BondUnit"), theVledTrack.GetBondUnitNum());
                            }
                        }
                    }
                }
            }
			else if (theMHS.IsRequestIndex() == false &&
				     theVledTrack.GetTrackErrorCode() == MHS_NO_ERROR)
			{
				if (theVledTrack.GetAvoidCrashFlag() == true)
				{
					if(theVledTrack.theLeftClaw.IsBusy() == false)
					{
						theVledTrack.theLeftClaw.AvoidCrashRollBack();
					    theVledTrack.SetAvoidCrashFlag(false);
					}
				}
			}
        }

        Sleep(2);
    }

    return 0;
}

UINT VledThreadProc(LPVOID pParam)
{
    theMHS.VledWin32Timing();	
	return 0;
}

UINT VEliteThreadProc(LPVOID pParam)
{
    theMHS.VEliteWin32Timing();	
	return 0;
}

UINT FoxconnIndexThreadProc(LPVOID pParam)
{
	while (theMHS.IsThreadActive())
    {
        if (theMHS.IsThreadStop() == false) 
        {
			theMHS.m_bIndexBusy = false; 

			if (theMHS.IsRequestIndex())
			{
				if (theFoxconnTrack.GetEvent() == FC_EVENT_IDLE)
					theFoxconnTrack.SubmitIndexEvent();
				else
					theFoxconnTrack.ProcessIndexEvent();
			}
			else if (theFoxconnTrack.GetEvent() != FC_EVENT_IDLE)
				theFoxconnTrack.ProcessIndexEvent();

			if (theFoxconnTrack.GetEvent() != FC_EVENT_IDLE)
				theMHS.m_bIndexBusy = true;
		}

		Sleep(2);
	}

	return 0;
}

UINT TopIndexThreadProc(LPVOID pParam)
{
	while (theMHS.IsThreadActive())
    {
        if (theMHS.IsThreadStop() == false) 
        {
			if (theMHS.IsAutoIndex() || theMHS.m_bIndexBusy)
            {
				theMHS.m_bIndexBusy = false;

				//Indexer 
				if (theTopTrack.IsValid())
				{
					if (theMHS.IsRequestIndex())
					{
						if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
							theTopTrack.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);
						else
							theTopTrack.ProcessIndexEvent();
					}
					else if (theTopTrack.GetEvent() != INDEX_EVENT_IDLE)
						theTopTrack.ProcessIndexEvent();

					if (theTopTrack.GetEvent() != INDEX_EVENT_IDLE)
						theMHS.m_bIndexBusy = true;
				}
			}
		}

		Sleep(2);
	}

	return 0;
}

//v3.0.T419 add
UINT TopEliteIndexThreadProc(LPVOID pParam)
{
	while (theMHS.IsThreadActive())
	{
		if (!theMHS.IsThreadStop() && (theMHS.IsAutoIndex() || theMHS.m_bIndexBusy))
		{
			theMHS.m_bIndexBusy = false;

			//Indexer 
			if (theTEIndex.IsValid())
			{
				if (theMHS.IsRequestIndex())
				{
					if (theTEIndex.GetEvent() == INDEX_EVENT_IDLE)
						theTEIndex.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);
					else
						theTEIndex.ProcessIndexEvent();
				}
				else if (theTEIndex.GetEvent() != INDEX_EVENT_IDLE)
					theTEIndex.ProcessIndexEvent();

				if (theTEIndex.GetEvent() != INDEX_EVENT_IDLE)
					theMHS.m_bIndexBusy = true;
			}
		}

		Sleep_x1ms(2);
	}

	return 0;
}

UINT R2RIndexThreadProc( LPVOID pParam )
{
	while (theMHS.IsThreadActive())
    {
        if (theMHS.IsThreadStop() == false) 
        {
			  if (theMHS.IsAutoIndex() || theMHS.m_bIndexBusy)
                {
					theMHS.m_bIndexBusy = false;

					//Indexer 
					if(theR2RTrack.GetTrackValidState() == TRUE)
					{
						if ((theMHS.IsRequestIndex()))
						{
							if(theR2RTrack.GetTopTrackEvent() == INDEX_EVENT_IDLE)
								theR2RTrack.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);
							else
								theR2RTrack.ProcessIndexEvent();
							
						}

						else if(theR2RTrack.GetTopTrackEvent() != INDEX_EVENT_IDLE)
								theR2RTrack.ProcessIndexEvent();
								
						if(theR2RTrack.GetTopTrackEvent() != INDEX_EVENT_IDLE)
							theMHS.m_bIndexBusy = true;

					}

				}

		}

		Sleep(2);
	 }

	return 0;
}

UINT CuEliteIndexThreadProc(LPVOID pParam)
{
	while (theMHS.IsThreadActive())
    {
        if (theMHS.IsThreadStop() == false) 
        {
			if (theMHS.IsAutoIndex() || theMHS.m_bIndexBusy)
            {
				theMHS.m_bIndexBusy = false;

				//Indexer
				if (theCuEliteTrack.GetTrackValidState() == TRUE)
				{
					if (theMHS.IsRequestIndex())
					{
						if (theCuEliteTrack.GetCuEliteTrackEvent() == INDEX_EVENT_IDLE)
	                        //SubmitIndex Event only as INDEX_EVENT_IDLE 
							theCuEliteTrack.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);
						else
							//continue the sequence[] 
							theCuEliteTrack.ProcessIndexEvent();
					}
					else if (theCuEliteTrack.GetCuEliteTrackEvent() != INDEX_EVENT_IDLE)
						//continue the sequence[] 
						theCuEliteTrack.ProcessIndexEvent();

					if (theCuEliteTrack.GetCuEliteTrackEvent() != INDEX_EVENT_IDLE)
						theMHS.m_bIndexBusy = true;
				}
			}
		}

		Sleep(2);
	}

	return 0;
}

// Begin thread

int CMaterialHandlerThread::BeginThread(void)
{
    m_pMhsThread = AfxBeginThread(PerformMaterialHandle, (LPVOID)this);
	Sleep(10);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_FOXCONN:
		m_pIndexThread = AfxBeginThread(FoxconnIndexThreadProc, (LPVOID)this);
		break;
	case MHS_VLED:
    	m_pIndexThread = AfxBeginThread(VledThreadProc, (LPVOID)this);
		break;
	case MHS_VLED_ELITE:
    	m_pIndexThread = AfxBeginThread(VEliteThreadProc, (LPVOID)this);
		break;
	case MHS_TOP_LED:
		m_pIndexThread = AfxBeginThread(TopIndexThreadProc, (LPVOID)this);
		break;
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
		m_pIndexThread = AfxBeginThread(TopEliteIndexThreadProc, (LPVOID)this);
		break;
	case MHS_R2R:
		m_pIndexThread = AfxBeginThread(R2RIndexThreadProc, (LPVOID)this);
		break;
	case MHS_CU_ELITE:
        m_pIndexThread = AfxBeginThread(CuEliteIndexThreadProc, (LPVOID)this);
		break;
	}

    return 0;
}

void CMaterialHandlerThread::Init(void)
{
	if (theMcConfig.IsBypassMHS()) return;	//v3.1.T172 add

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
    {
	    theTopOnloader.Init();
    	theTopTrack.Init();
        theTopClamp.Init();
		theTopOffloader.Init();
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
	    theTopOnloader.Init();
    	theTEIndex.Init();
        theTopClamp.Init();
		theTopOffloader.Init();
		break;
    }
	case MHS_FOXCONN:
	{
		theFoxconnTrack.Init();	//v3.0.T175 edit
		break;
	}
	case MHS_R2R:
    {
	    theR2ROnloader.Initialize();
    	theR2RTrack.Initialize();
        theTopClamp.Init();
		theR2ROffloader.Initialize();
		break;
    }
	case MHS_CU_ELITE:
    {
        theCuEliteOnloader.Initialize();
    	theCuEliteTrack.Initialize();
        theCuEliteLFClamp.Init();
		theCuEliteOffloader.Initialize();
		break;
    }
	case MHS_VLED_ELITE:
    {
		theVEliteOnload.Init();
		theVEliteTrack.Init();
		theVEliteLfClamp.Init();
		theVEliteOffload.Init();
		break;
    }
	case MHS_VLED:
    {
		theVledOnload.Initialize();
		theVledTrack.Initialize();
		theVledLfClamp.Initialize();
		theVledOffload.Initialize();
		break;
    }
	}

    SetThreadActive(true);
}

void CMaterialHandlerThread::SaveMhsRcd(void)
{
	theDataMgr.CopyToBuffer((char *)&m_nWhVersion, sizeof(m_nWhVersion));

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		theTopClamp.SaveRcd();
		theTopTrack.SaveRcd();
		theTopOnloader.SaveRcd();
		theTopOffloader.SaveRcd();
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTopClamp.SaveRcd();
		theTEIndex.SaveRcd();
		theTopOnloader.SaveRcd();
		theTopOffloader.SaveRcd();
		break;
	}
	case MHS_R2R:
	{
		theTopClamp.SaveRcd();
		theR2RTrack.SaveTrackRcd();
		theR2ROnloader.SaveOnloadRcd();
		theR2ROffloader.SaveOffloadRcd();
		break;
	}
	case MHS_CU_ELITE:
	{
		theCuEliteLFClamp.SaveClampRcd();
		theCuEliteTrack.SaveTrackRcd();
		theCuEliteOnloader.SaveOnloadRcd();
		theCuEliteOffloader.SaveOffloadRcd();
		break;
	}
	case MHS_VLED_ELITE:
	{
		theVEliteOnload.SaveRcd();
		theVEliteTrack.SaveRcd();
		theVEliteLfClamp.SaveRcd();
		theVEliteOffload.SaveRcd();
		break;
	}
	case MHS_VLED:
	{
		theVledOnload.SaveLfOnloadRcd();
		theVledTrack.SaveLfTrackRcd();
		theVledOffload.SaveForkOffloadRcd();
		theVledLfClamp.SaveLfClampRcd();
		break;
	}
	}
}

void CMaterialHandlerThread::LoadMhsRcd(void)
{
	theDataMgr.CopyFromBuffer((char *)&m_nWhVersion, sizeof(m_nWhVersion));

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		theTopClamp.LoadRcd();
		theTopTrack.LoadRcd();
		theTopOnloader.LoadRcd();
		theTopOffloader.LoadRcd();

		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);
		theTopTrack.RefreshMeasurementData(&stTopDevicePara);

		theTopTrack.SetCurBondUnit(0);
		theTopTrack.SetTrackNeedClear(true);
		theTopTrack.m_pTargetClaw = &theTopTrack.m_InputClaw;
		m_nWhVersion = WH_VERSION_TOP;
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTopClamp.LoadRcd();
		theTEIndex.LoadRcd();
		theTopOnloader.LoadRcd();
		theTopOffloader.LoadRcd();

		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);
		theTEIndex.RefreshMeasurementData(&stTopDevicePara);

		theTEIndex.SetCurBondUnit(0);
		theTEIndex.SetTrackNeedClear(true);
		theTEIndex.m_pTargetClaw = &theTEIndex.m_InputClaw;
		m_nWhVersion = WH_VERSION_TOP;
		break;
	}
	case MHS_FOXCONN:	//v3.0.T194 add
	{
		theFoxconnTrack.LoadRcd();
		theFoxconnTrack.SetCurBondUnit(1);
		m_nWhVersion = WH_VERSION_TOP;
		break;
	}
	case MHS_R2R:
	{
		theTopClamp.LoadRcd();
		theR2RTrack.LoadTrackRcd();
		theR2ROnloader.LoadOnloadRcd();
		theR2ROffloader.LoadOffloadRcd();

		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);
		theR2RTrack.RefreshMeasurementData(&stTopDevicePara);

		theR2RTrack.SetCurrentBondUnit(0);
		theR2RTrack.SetTrackNeedClearFlag(TRUE);
		theR2RTrack.theTargetClaw = &theR2RTrack.theInputClaw;
		m_nWhVersion = WH_VERSION_R2R;
		break;
	}
	case MHS_CU_ELITE:
	{
		theCuEliteLFClamp.LoadClampRcd();
		theCuEliteTrack.LoadTrackRcd();
		theCuEliteOnloader.LoadOnloadRcd();
		theCuEliteOffloader.LoadOffloadRcd();

		CUELITE_DEVICE_RECORD stCuEliteDevicePara;
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
		theCuEliteTrack.RefreshMeasurementData(&stCuEliteDevicePara);

		theCuEliteTrack.SetCurrentBondUnit(0);
		theCuEliteTrack.SetTrackNeedClearFlag(TRUE);
		theCuEliteTrack.theTargetClaw = &theCuEliteTrack.theInputClaw;
		m_nWhVersion = WH_VERSION_CUELITE;
		break;
	}
	case MHS_VLED_ELITE:
	{
		theVEliteOnload.LoadRcd();
		theVEliteTrack.LoadRcd();
		theVEliteLfClamp.LoadRcd();
		theVEliteOffload.LoadRcd();
		m_nWhVersion = WH_VERSION_VELITE;
		break;
	}
	case MHS_VLED:
	{
		theVledOnload.LoadLfOnloadRcd();
		theVledTrack.LoadLfTrackRcd();
		theVledOffload.LoadForkOffloadRcd();
		theVledLfClamp.LoadLfClampRcd();
        m_nWhVersion = WH_VERSION_VERTICAL;
		break;
	}
	}
}

void CMaterialHandlerThread::SaveMhsPowerOnData(void)
{
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
       	TOP_MHS_POWER_ON_DATA stMhsPowerOnData;
		stMhsPowerOnData.m_stSlipOnFront     = theTopTrack.m_stSlipOnTrack;
		stMhsPowerOnData.nCurrentUnit        = theTopTrack.GetCurBondUnit();
		stMhsPowerOnData.nOnloadCurrentSlot  = theTopOnloader.GetCurSlot();
		stMhsPowerOnData.nOffloadCurrentSlot = theTopOffloader.GetCurSlot();
        theDataMgr.CopyToBuffer((char *)&stMhsPowerOnData, sizeof(TOP_MHS_POWER_ON_DATA));
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
       	TOP_MHS_POWER_ON_DATA stMhsPowerOnData;
		stMhsPowerOnData.m_stSlipOnFront     = theTEIndex.m_stSlipOnTrack;
		stMhsPowerOnData.nCurrentUnit        = theTEIndex.GetCurBondUnit();
		stMhsPowerOnData.nOnloadCurrentSlot  = theTopOnloader.GetCurSlot();
		stMhsPowerOnData.nOffloadCurrentSlot = theTopOffloader.GetCurSlot();
        theDataMgr.CopyToBuffer((char *)&stMhsPowerOnData, sizeof(TOP_MHS_POWER_ON_DATA));
		break;
	}
	case MHS_R2R:
	{
       	TOP_MHS_POWER_ON_DATA stMhsPowerOnData;
		stMhsPowerOnData.m_stSlipOnFront     = theR2RTrack.m_stSlipOnTrack;
		stMhsPowerOnData.nCurrentUnit        = theR2RTrack.GetCurrentBondUnit();
		stMhsPowerOnData.nOnloadCurrentSlot  = theR2ROnloader.GetCurrentSlotNum();
		stMhsPowerOnData.nOffloadCurrentSlot = theR2ROffloader.GetCurrentSlotNum();
        theDataMgr.CopyToBuffer((char *)&stMhsPowerOnData, sizeof(TOP_MHS_POWER_ON_DATA));
		break;
	}
	case MHS_CU_ELITE:
	{
       	CUELITE_MHS_POWER_ON_DATA stMhsPowerOnData;
		stMhsPowerOnData.m_stSlipOnFront     = theCuEliteTrack.m_stSlipOnTrack;
		stMhsPowerOnData.nCurrentUnit        = theCuEliteTrack.GetCurrentBondUnit();
		stMhsPowerOnData.nOnloadCurrentSlot  = theCuEliteOnloader.GetCurrentSlotNum();
		stMhsPowerOnData.nOffloadCurrentSlot = theCuEliteOffloader.GetCurrentSlotNum();
        theDataMgr.CopyToBuffer((char *)&stMhsPowerOnData, sizeof(CUELITE_MHS_POWER_ON_DATA));
		break;
	}
	case MHS_VLED_ELITE:
	{
		MHS_POWER_ON_DATA stMhsPowerOnData;
    	stMhsPowerOnData.nLeftClawTick      = theVEliteTrack.theLeftClaw.GetClawTicker();
		stMhsPowerOnData.nRightClawTick     = theVEliteTrack.theRightClaw.GetClawTicker();
		stMhsPowerOnData.nBondingCounter    = theVEliteTrack.m_nBondUnitNum;
		theDataMgr.CopyToBuffer((char *)&stMhsPowerOnData, sizeof(MHS_POWER_ON_DATA));
		break;
	}
	case MHS_VLED:
	{
		MHS_POWER_ON_DATA stMhsPowerOnData;
    	stMhsPowerOnData.nLeftClawTick      = theVledTrack.theLeftClaw.GetClawTicker();
		stMhsPowerOnData.nRightClawTick     = theVledTrack.theRightClaw.GetClawTicker();
		stMhsPowerOnData.nBondingCounter    = theVledTrack.m_nBondUnitNum;
		stMhsPowerOnData.nOffloadForkBuffer = theVledOffload.m_nLeadframeCount;
		stMhsPowerOnData.bBufferSwitchFlag  = theVledOffload.bBuffSwitchFlag;
		theDataMgr.CopyToBuffer((char *)&stMhsPowerOnData, sizeof(MHS_POWER_ON_DATA));
		break;
	}
	}
}

void CMaterialHandlerThread::LoadMhsPowerOnData(void)
{
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		TOP_MHS_POWER_ON_DATA stMhsPowerOnData;
        theDataMgr.CopyFromBuffer((char *)&stMhsPowerOnData, sizeof(TOP_MHS_POWER_ON_DATA));

		theTopTrack.m_stSlipOnTrack = stMhsPowerOnData.m_stSlipOnFront;
		//theTopTrack.SetCurrentBondUnit(stMhsPowerOnData.nCurrentUnit);
		theTopOnloader.SetCurSlot(stMhsPowerOnData.nOnloadCurrentSlot);
		theTopOffloader.SetCurSlot(stMhsPowerOnData.nOffloadCurrentSlot);
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		TOP_MHS_POWER_ON_DATA stMhsPowerOnData;
        theDataMgr.CopyFromBuffer((char*)&stMhsPowerOnData, sizeof(TOP_MHS_POWER_ON_DATA));

		theTEIndex.m_stSlipOnTrack = stMhsPowerOnData.m_stSlipOnFront;
		theTopOnloader.SetCurSlot(stMhsPowerOnData.nOnloadCurrentSlot);
		theTopOffloader.SetCurSlot(stMhsPowerOnData.nOffloadCurrentSlot);
		break;
	}
	case MHS_R2R:
	{
		TOP_MHS_POWER_ON_DATA stMhsPowerOnData;
        theDataMgr.CopyFromBuffer((char *)&stMhsPowerOnData, sizeof(TOP_MHS_POWER_ON_DATA));

		theR2RTrack.m_stSlipOnTrack = stMhsPowerOnData.m_stSlipOnFront;
		//theTopTrack.SetCurrentBondUnit(stMhsPowerOnData.nCurrentUnit);
		theR2ROnloader.SetCurrentSlotNum(stMhsPowerOnData.nOnloadCurrentSlot);
		theR2ROffloader.SetCurrentSlotNum(stMhsPowerOnData.nOffloadCurrentSlot);
		break;
	}
	case MHS_CU_ELITE:
	{
        CUELITE_MHS_POWER_ON_DATA stMhsPowerOnData;
        theDataMgr.CopyFromBuffer((char *)&stMhsPowerOnData, sizeof(CUELITE_MHS_POWER_ON_DATA));

		theCuEliteTrack.m_stSlipOnTrack = stMhsPowerOnData.m_stSlipOnFront;
		//theTopTrack.SetCurrentBondUnit(stMhsPowerOnData.nCurrentUnit);
		theCuEliteOnloader.SetCurrentSlotNum(stMhsPowerOnData.nOnloadCurrentSlot);
		theCuEliteOffloader.SetCurrentSlotNum(stMhsPowerOnData.nOffloadCurrentSlot);
		break;
	}
	case MHS_VLED_ELITE:
	{
		MHS_POWER_ON_DATA stMhsPowerOnData;
        theDataMgr.CopyFromBuffer((char *)&stMhsPowerOnData, sizeof(MHS_POWER_ON_DATA));
		theVEliteTrack.theLeftClaw.SetClawTicker(stMhsPowerOnData.nLeftClawTick);
		theVEliteTrack.theRightClaw.SetClawTicker(stMhsPowerOnData.nRightClawTick);
		theVEliteTrack.m_nBondUnitNum = stMhsPowerOnData.nBondingCounter;
		break;
	}
	case MHS_VLED:
	{
		MHS_POWER_ON_DATA stMhsPowerOnData;
        theDataMgr.CopyFromBuffer((char *)&stMhsPowerOnData, sizeof(MHS_POWER_ON_DATA));
		theVledTrack.theLeftClaw.SetClawTicker(stMhsPowerOnData.nLeftClawTick);
		theVledTrack.theRightClaw.SetClawTicker(stMhsPowerOnData.nRightClawTick);
		theVledTrack.m_nBondUnitNum = stMhsPowerOnData.nBondingCounter;
		theVledOffload.m_nLeadframeCount = stMhsPowerOnData.nOffloadForkBuffer;
		theVledOffload.bBuffSwitchFlag = stMhsPowerOnData.bBufferSwitchFlag;
		break;
	}
	}
}

DWORD WINAPI CMaterialHandlerThread::ThreadProc(LPVOID lpVoid)
{
	Sleep(50);
	theCuEliteOffloader.theMagPullyDcMotor.TriggerForwardMotion();
	Sleep(100);
	theCuEliteOffloader.theMagPullyDcMotor.TriggerBacktoHome();

	return NOERROR;
}

int CMaterialHandlerThread::WarmReset(void)
{
	short nMsgNum, anMsg[10];
	CHTimer timer;

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		theTopTrack.ResetRelayErrorState();
		Sleep(200);
		theTopTrack.HomeIndexer();
		Sleep(300);
		theTopTrack.WaitHomeIndexer();
	    Sleep(500);
    	theTopTrack.MoveIndexerToLogicZero();
		Sleep(500);

		theTopTrack.m_InjectSensor.UpdateSensorState();
		theTopTrack.m_EjectSensor.UpdateSensorState();
		theTopTrack.m_AlignSensor.UpdateSensorState();
		if ((theTopTrack.m_InjectSensor.GetSensorState() == SENSOR_ON)
			|| (theTopTrack.m_EjectSensor.GetSensorState() == SENSOR_ON)
			|| (theTopTrack.m_AlignSensor.GetSensorState() == SENSOR_ON))
		{ 
			theTopClamp.HomeClamp();
			Sleep(10);
			theTopClamp.WaitHomeClamp();

			nMsgNum = 1;
			anMsg[0] = MSG_REMOVE_LF_ON_TRACK;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);

			theTopClamp.PreCloseClamp();
		}

		do
		{
			theTopOnloader.m_MagDetectedSensor.UpdateSensorState();
			theTopOffloader.m_MagDetectedSensor.UpdateSensorState();
			if ((theTopOnloader.m_MagDetectedSensor.GetSensorState() == SENSOR_ON)
				|| (theTopOffloader.m_MagDetectedSensor.GetSensorState() == SENSOR_ON))
			{
				nMsgNum = 1;
				anMsg[0] = MSG_REMOVE_MAG_ON_ELEV;
				//anMsg[1] = MSG_REMOVE_MAG_BELOW_ELEV;
				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);
			}
			else
				break;
		} while (true);

		do
		{
			theTopOnloader.m_MagDropSensor.UpdateSensorState();
			theTopOffloader.m_MagDropSensor.UpdateSensorState();
			if ((theTopOnloader.m_MagDropSensor.GetSensorState() == SENSOR_ON)
				|| (theTopOffloader.m_MagDropSensor.GetSensorState() == SENSOR_ON))
			{
				nMsgNum = 1;
				//anMsg[0] = MSG_REMOVE_MAG_ON_ELEV;
				anMsg[0] = MSG_REMOVE_MAG_BELOW_ELEV;
				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);
			}
			else
				break;
		} while (true);

		//Double Check
		timer.StartTimer();
		do 
		{
			Sleep_x1ms(10);
			theTopTrack.m_InjectSensor.UpdateSensorState();
			theTopTrack.m_EjectSensor.UpdateSensorState();
			//theTopOnloader.m_MagDetectedSensor.UpdateSensorState();
			//theTopOffloader.m_MagDetectedSensor.UpdateSensorState();
		} while ((timer.GetTime_x1ms() < 1000)
				&& ((theTopTrack.m_InjectSensor.GetSensorState() == SENSOR_OFF)
					|| (theTopTrack.m_EjectSensor.GetSensorState() == SENSOR_OFF)));
					//|| (theTopOnloader.m_MagDetectedSensor.GetSensorState() == SENSOR_OFF)
					//|| (theTopOffloader.m_MagDetectedSensor.GetSensorState() == SENSOR_OFF)));

		//start home all
		theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
		theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
        Sleep(100);
        		
		theTopOnloader.m_InjectPusher.TriggerBacktoHome();
        Sleep(100);

		timer.StartTimer();
        do 
        {
			Sleep_x1ms(10);
        } while ((timer.GetTime_x1ms() < 6000)
				&& (theStepper.IsBusy(theTopOnloader.m_nMagHolderStepperId)
					|| theStepper.IsBusy(theTopOffloader.m_nMagHolderStepperId)));

		theTopOnloader.HomeMagClamper();
		//Sleep(10);
		theTopOffloader.HomeMagClamper();
		Sleep(10);
		theTopOnloader.WaitHomeMagClamper();
		//Sleep(10);
		theTopOffloader.WaitHomeMagClamper();
		Sleep(300);

        theTopOnloader.HomeMagHolder();
        Sleep(10);
        theTopOffloader.HomeMagHolder();
        Sleep(10);
        theTopOnloader.WaitHomeMagHolder();
        Sleep(10);
        theTopOffloader.WaitHomeMagHolder();
        Sleep(10);

        theTopOnloader.m_MagPullyDcMotor.TriggerForwardMotion();
        theTopOffloader.m_MagPullyDcMotor.TriggerForwardMotion();
        Sleep(100);
        
        theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
        theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
        Sleep(100);

        theTopOnloader.MoveMagHolderToSlot(1);   
        Sleep(10);

        theTopOffloader.MoveMagHolderToSlot(1);  
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			Sleep(2000);
		else
			Sleep(4000);

		theTopOnloader.WaitHomeMagHolder();
		//Sleep(50);
		theTopOffloader.WaitHomeMagHolder();
		Sleep(100);

        theTopOnloader.MoveMagClamperToClampPos();
        //Sleep(10);
        theTopOffloader.MoveMagClamperToClampPos();
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			Sleep(1000);
		theTopOnloader.WaitMoveMagClamper();
		//Sleep(10);
		theTopOffloader.WaitMoveMagClamper();
		Sleep(500);

		theTopClamp.HomeClamp();
		//Sleep(10);
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			Sleep(3000);	//debug clamp not close at startup
		theTopClamp.WaitHomeClamp();
        Sleep(800);

        //theTopClamp.CloseClamp();
        //Sleep(1000);
		if (CloseWindowClamp() == FNOK)
		{
			nMsgNum = 1;
			anMsg[0] = MSG_WINDOW_CLAMP_HOME_FAIL;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);
		}

		//v3.0.T362 add
		theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
		Sleep_x1ms(500);

		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTEIndex.ResetRelayErrorState();
		Sleep(200);
		theTEIndex.HomeIndexer();
		Sleep(300);
		theTEIndex.WaitHomeIndexer();
	    Sleep(500);
    	theTEIndex.MoveIndexerToLogicZero();
		Sleep(500);

		theTEIndex.m_InjectSensor.UpdateSensorState();
		theTEIndex.m_EjectSensor.UpdateSensorState();
		theTEIndex.m_AlignSensor.UpdateSensorState();
		if ((theTEIndex.m_InjectSensor.GetSensorState() == SENSOR_ON)
			|| (theTEIndex.m_EjectSensor.GetSensorState() == SENSOR_ON)
			|| (theTEIndex.m_AlignSensor.GetSensorState() == SENSOR_ON))
		{ 
			theTopClamp.HomeClamp();
			Sleep(10);
			theTopClamp.WaitHomeClamp();

			nMsgNum = 1;
			anMsg[0] = MSG_REMOVE_LF_ON_TRACK;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);

			theTopClamp.PreCloseClamp();
		}

		do
		{
			theTopOnloader.m_MagDetectedSensor.UpdateSensorState();
			theTopOffloader.m_MagDetectedSensor.UpdateSensorState();
			if ((theTopOnloader.m_MagDetectedSensor.GetSensorState() == SENSOR_ON)
				|| (theTopOffloader.m_MagDetectedSensor.GetSensorState() == SENSOR_ON))
			{
				nMsgNum = 1;
				anMsg[0] = MSG_REMOVE_MAG_ON_ELEV;
				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);
			}
			else
				break;
		} while (true);

		do
		{
			theTopOnloader.m_MagDropSensor.UpdateSensorState();
			theTopOffloader.m_MagDropSensor.UpdateSensorState();
			if ((theTopOnloader.m_MagDropSensor.GetSensorState() == SENSOR_ON)
				|| (theTopOffloader.m_MagDropSensor.GetSensorState() == SENSOR_ON))
			{
				nMsgNum = 1;
				anMsg[0] = MSG_REMOVE_MAG_BELOW_ELEV;
				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);
			}
			else
				break;
		} while (true);

		//Double Check
		timer.StartTimer();
		do 
		{
			Sleep_x1ms(10);
			theTEIndex.m_InjectSensor.UpdateSensorState();
			theTEIndex.m_EjectSensor.UpdateSensorState();
		} while ((timer.GetTime_x1ms() < 1000)
				&& ((theTEIndex.m_InjectSensor.GetSensorState() == SENSOR_OFF)
					|| (theTEIndex.m_EjectSensor.GetSensorState() == SENSOR_OFF)));

		//start home all
		theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
		theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
        Sleep(100);
        		
		theTopOnloader.m_InjectPusher.TriggerBacktoHome();
        Sleep(100);

		timer.StartTimer();
        do 
        {
			Sleep_x1ms(10);
        } while ((timer.GetTime_x1ms() < 6000)
				&& (theStepper.IsBusy(theTopOnloader.m_nMagHolderStepperId)
					|| theStepper.IsBusy(theTopOffloader.m_nMagHolderStepperId)));

		theTopOnloader.HomeMagClamper();
		theTopOffloader.HomeMagClamper();
		Sleep(10);
		theTopOnloader.WaitHomeMagClamper();
		theTopOffloader.WaitHomeMagClamper();
		Sleep(300);

        theTopOnloader.HomeMagHolder();
        Sleep(10);
        theTopOffloader.HomeMagHolder();
        Sleep(10);
        theTopOnloader.WaitHomeMagHolder();
        Sleep(10);
        theTopOffloader.WaitHomeMagHolder();
        Sleep(10);

        theTopOnloader.m_MagPullyDcMotor.TriggerForwardMotion();
        theTopOffloader.m_MagPullyDcMotor.TriggerForwardMotion();
        Sleep(100);
        
        theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
        theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
        Sleep(100);

        theTopOnloader.MoveMagHolderToSlot(1);   
        Sleep(10);

        theTopOffloader.MoveMagHolderToSlot(1);  
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			Sleep(2000);
		else
			Sleep(4000);

		theTopOnloader.WaitHomeMagHolder();
		theTopOffloader.WaitHomeMagHolder();
		Sleep(100);

        theTopOnloader.MoveMagClamperToClampPos();
        theTopOffloader.MoveMagClamperToClampPos();
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			Sleep(1000);
		theTopOnloader.WaitMoveMagClamper();
		theTopOffloader.WaitMoveMagClamper();
		Sleep(500);

		theTopClamp.HomeClamp();
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			Sleep(3000);	//debug clamp not close at startup
		theTopClamp.WaitHomeClamp();
        Sleep(800);

		if (CloseWindowClamp() == FNOK)
		{
			nMsgNum = 1;
			anMsg[0] = MSG_WINDOW_CLAMP_HOME_FAIL;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg);
		}

		theTopClamp.SetVacuumOn(false);
		Sleep_x1ms(500);
		break;
    }
	case MHS_FOXCONN:	//v3.0.T175 add
	{
		theFoxconnTrack.ResetRelayErrorState();
		Sleep(200);
		theFoxconnTrack.HomeIndexer();
		Sleep(200);
		theFoxconnTrack.WaitHomeIndexer();
	    Sleep(200);
		theFoxconnTrack.MoveToFirstIndexPos();	//v3.0.T194 add
		break;
    }
	case MHS_R2R:
    {
		theR2RTrack.ResetRelayErrorState();
		Sleep(200);

		theR2RTrack.SearchIndexStepHome();
		Sleep(10);

		theR2RTrack.WaitStepHomeFinish();
		Sleep(10);

		theR2RTrack.MoveToLogicZero();
		Sleep(500);

		//home---
		theTopClamp.HomeClamp();
		Sleep(100);

		theTopClamp.WaitHomeClamp();
		Sleep(300);

		theTopClamp.PreCloseClamp();
		Sleep(500);

		//Offload OR Onload down
		CStringList sList;
		theR2ROffloader.theStopBondSnr.UpdateSensorState();
        if (theR2ROffloader.theStopBondSnr.GetSensorState() == SENSOR_ON)
		{
			sList.AddTail(theMsg.GetMsg(MSG_OFFLOAD_DOWN));
			theMsg.ShowMsg(ERROR_MSG, sList);
		}
		theR2ROnloader.theStopBondSnr.UpdateSensorState();
		if (theR2ROnloader.theStopBondSnr.GetSensorState() == TRUE) //FALSE
		{
			sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_DOWN));
			theMsg.ShowMsg(ERROR_MSG, sList);
		}
		break;
    }
	case MHS_CU_ELITE:
	{
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
		{
			if (theStepperIO.EnableStepper(CANADDR_ONLOAD, true) == CAN_SEND_ERROR)
				return CAN_SEND_ERROR;
			else if (theStepperIO.EnableStepper(CANADDR_OFFLOAD, true) == CAN_SEND_ERROR)
				return CAN_SEND_ERROR;
			else if (theStepperIO.EnableStepper(CANADDR_INDEX, true) == CAN_SEND_ERROR)
				return CAN_SEND_ERROR;
			Sleep_x1ms(500);
		}
		theCuEliteTrack.ResetRelayErrorState();
		Sleep_x1ms(200);
		theCuEliteTrack.SearchIndexStepHome();
		Sleep_x1ms(300);
		theCuEliteTrack.WaitStepHomeFinish();
		Sleep_x1ms(500);
		theCuEliteTrack.MoveToLogicZero();
		Sleep_x1ms(500);

		if(theCuEliteTrack.theInjectSnr.GetCurrentState() == SENSOR_ON ||
			theCuEliteTrack.theEjectSnr.GetCurrentState() == SENSOR_ON ||
			theCuEliteTrack.theAlignSnr.GetCurrentState() == SENSOR_ON)
		{ 
			theCuEliteLFClamp.SearchHome();
			Sleep_x1ms(10);
			theCuEliteLFClamp.WaitHomeFinish();
			Sleep_x1ms(10);

			nMsgNum = 1;
			anMsg[0] = MSG_REMOVE_LF_ON_TRACK;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg, false, false, true);

			theCuEliteLFClamp.PreCloseClamp();
			Sleep_x1ms(10);
		}

		if (theCuEliteOnloader.theMagDetectedSensor.GetCurrentState() ==SENSOR_ON ||
			theCuEliteOffloader.theMagDetectedSensor.GetCurrentState() ==SENSOR_ON)
		{
			nMsgNum = 2;
			anMsg[0] = MSG_REMOVE_MAG_ON_ELEV;
			anMsg[1] = MSG_REMOVE_MAG_BELOW_ELEV;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg, false, false, true);
		}

		theCuEliteTrack.MoveFrontTrackHome();
		Sleep_x1ms(100);
		theCuEliteTrack.MoveBackTrackHome();
		Sleep_x1ms(100);

		theCuEliteLFClamp.SearchHome();
		Sleep_x1ms(10);
		theCuEliteLFClamp.WaitHomeFinish();
		Sleep_x1ms(10);

		nMsgNum = 2;
		anMsg[0] = MSG_REMOVE_TOP_PLATE_WINDOW_CLAMP;
		anMsg[1] = MSG_REMOVE_ELEV_SUPPORT_BAR;
		theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg, false, false, true);

		//Double Check
		timer.StartTimer();
		do 
		{
			if (theCuEliteTrack.theInjectSnr.GetCurrentState() == SENSOR_ON &&
				theCuEliteTrack.theEjectSnr.GetCurrentState() == SENSOR_ON &&
				theCuEliteOnloader.theMagDetectedSensor.GetCurrentState() ==SENSOR_ON &&
				theCuEliteOffloader.theMagDetectedSensor.GetCurrentState()==SENSOR_ON)
				break;

			Sleep_x1ms(2);

		} while (timer.GetTime_x1ms() < 2000);

		//home---
		theCuEliteOnloader.theMagPullyDcMotor.TriggerBacktoHome();
		theCuEliteOffloader.theMagPullyDcMotor.TriggerBacktoHome();

		Sleep_x1ms(100);

		theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();

		Sleep_x1ms(100);

		short nBusyState = 1; //@22
		long  lElapsTime, lStartTime = GetCurrentTime();

		do 
		{
			if ((theStepper.IsBusy(theCuEliteOnloader.m_ucForkHolderStepId) == false) &&
				(theStepper.IsBusy(theCuEliteOffloader.m_ucForkHolderStepId) == false))
				nBusyState = 0;

			lElapsTime = GetCurrentTime() - lStartTime; //@29
			if (lElapsTime > 6000)
				break;

		} while (nBusyState);

		theCuEliteOnloader.SearchOnloadMagLengthStepHome();
		Sleep_x1ms(10);
		theCuEliteOffloader.SearchOffloadMagLengthStepHome();
		Sleep_x1ms(10);
		theCuEliteOnloader.WaitOnloadMagLengthStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOffloader.WaitOffloadMagLengthStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOnloader.SearchOnloadMagClamperFrontStepHome();
		Sleep_x1ms(10);
		theCuEliteOnloader.SearchOnloadMagClamperBackStepHome();
		Sleep_x1ms(10);
		theCuEliteOffloader.SearchOffloadMagClamperFrontStepHome();
		Sleep_x1ms(10);
		theCuEliteOffloader.SearchOffloadMagClamperBackStepHome();
		Sleep_x1ms(10);
		theCuEliteOnloader.WaitOnloadMagClamperFrontStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOnloader.WaitOnloadMagClamperBackStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOffloader.WaitOffloadMagClamperFrontStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOffloader.WaitOffloadMagClamperBackStepHomeFinish();
		Sleep_x1ms(300);

		theCuEliteOnloader.SearchOnloadElevetorLimit();
		Sleep_x1ms(10);
		theCuEliteOffloader.SearchOffloadElevetorLimit();
		Sleep_x1ms(10);

		do 
		{
			if (theCuEliteOnloader.theFork1PresentSensor.GetCurrentState() == SENSOR_ON )
				theStepper.StopMotor(theCuEliteOnloader.m_ucForkHolderStepId,STEPPER_STOP_DEC);
			else if (theCuEliteOnloader.theFork2PresentSensor.GetCurrentState() == SENSOR_ON)
				theStepper.StopMotor(theCuEliteOnloader.m_ucForkHolderStepId,STEPPER_STOP_DEC);
			else if (theCuEliteOffloader.theFork1PresentSensor.GetCurrentState() == SENSOR_ON )
				theStepper.StopMotor(theCuEliteOffloader.m_ucForkHolderStepId,STEPPER_STOP_DEC);
			else if (theCuEliteOffloader.theFork2PresentSensor.GetCurrentState() == SENSOR_ON)
				theStepper.StopMotor(theCuEliteOffloader.m_ucForkHolderStepId,STEPPER_STOP_DEC);
		} while (theStepper.IsBusy(theCuEliteOnloader.m_ucForkHolderStepId)
				|| theStepper.IsBusy(theCuEliteOffloader.m_ucForkHolderStepId));

		if (theCuEliteOnloader.SetOnloadMagLenLimit())
		{
			nMsgNum = 1;
			anMsg[0] = MSG_ONLOAD_MAG_LEN_LIMIT_FAIL;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg, false, false, true);
		}
		
		if (theCuEliteOffloader.SetOffloadMagLenLimit())
		{
			nMsgNum = 1;
			anMsg[0] = MSG_OFFLOAD_MAG_LEN_LIMIT_FAIL;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsg, false, false, true);
		}

		Sleep_x1ms(100);

		theCuEliteOnloader.SearchOnloadElvStepHome();
		Sleep_x1ms(10);
		theCuEliteOffloader.SearchOffloadElvStepHome();
		Sleep_x1ms(10);
		theCuEliteOnloader.WaitStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOffloader.WaitStepHomeFinish();
		Sleep_x1ms(10);

		HANDLE hThread=CreateThread(NULL,0,ThreadProc,NULL,0,NULL);

		theCuEliteOnloader.theMagPullyDcMotor.TriggerForwardMotion();
		Sleep_x1ms(100);
		theCuEliteOnloader.theMagPullyDcMotor.TriggerBacktoHome();

		WaitThread(hThread,1000);		//V3.3.W180 edit

		theCuEliteOnloader.MoveTotheSlot(1);   
		Sleep_x1ms(10);

		theCuEliteOffloader.MoveTotheSlot(1);  
		Sleep_x1ms(100);

		theCuEliteOnloader.OnloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(10);
		theCuEliteOffloader.OffloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(10);
		theCuEliteOnloader.OnloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(10);
		theCuEliteOffloader.OffloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(10);
		theCuEliteOnloader.OnloadMagLengthMovetoStandyPos();
		Sleep_x1ms(10);
		theCuEliteOffloader.OffloadMagLengthMovetoStandyPos();
		Sleep_x1ms(4000);

		theCuEliteTrack.MoveFrontTrackToStandbyPos();
		Sleep_x1ms(10);
		theCuEliteTrack.MoveBackTrackToStandbyPos();
		Sleep_x1ms(500);
		theCuEliteTrack.WaitFrontTrackStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteTrack.WaitBackTrackStepHomeFinish();
		Sleep_x1ms(300);

		theCuEliteLFClamp.CloseClamp();
		Sleep_x1ms(500);
		break;
	}
	case MHS_VLED_ELITE:
	{
		theVEliteLfClamp.SearchClampHome();
		Sleep(1000);
		theVEliteLfClamp.WaitStepHomeFinish();
		Sleep(100);
		theVEliteLfClamp.OpenLfClamp();
		Sleep(100);

		//reset offload
		theVEliteOffload.m_MagKickerHomeSensor.UpdateSensorState();
		if (theVEliteOffload.m_MagKickerHomeSensor.GetSensorState() != SENSOR_ON)
			theVEliteOffload.m_MagKicker.TriggerBacktoHome();
		theVEliteOffload.SearchStepHome();
		theVEliteOffload.WaitStepHomeFinish();

		theVEliteTrack.SearchHome();
		Sleep(10);
		//theVledOffload.SearchStepHome();
		Sleep(10);
		
		theVEliteTrack.theLeftClaw.SearchHome();
		Sleep(10);
		theVEliteTrack.theRightClaw.SearchHome();
		Sleep(10);

		theVEliteTrack.CheckHomeState();
		//theVledOffload.WaitStepHomeFinish();

		theVEliteOnload.SearchStepHome();
		Sleep(10);

		theVEliteTrack.theLeftClaw.CheckHomeState();
		theVEliteTrack.theRightClaw.CheckHomeState();

		Sleep(100);
		theVEliteOnload.WaitStepHomeFinish();

		theVEliteLfClamp.CloseLfClamp();
		Sleep(10);
		break;
	}
	case MHS_VLED:
	{
		theVledOffload.theOutputClaw.SetRelayState(RELAY_OFF);
		theVledOffload.thePusherCylinder.MoveBackward();

		theVledLfClamp.SearchClampHome();
		Sleep(1000);
		theVledLfClamp.WaitStepHomeFinish();
		Sleep(100);
		theVledLfClamp.OpenLfClamp(); //@13
		Sleep(100);
		theVledTrack.SearchHome();
		Sleep(10);
		theVledOffload.SearchStepHome();
		Sleep(10);

		theVledTrack.theLeftClaw.SearchHome();
		Sleep(10);
		theVledTrack.theRightClaw.SearchHome();
		Sleep(10);

		theVledTrack.CheckHomeState();
		theVledOffload.WaitStepHomeFinish();

		theVledOnload.SearchStepHome();
		Sleep(10);

		theVledTrack.theLeftClaw.CheckHomeState();
		theVledTrack.theRightClaw.CheckHomeState();

		Sleep(100);
		theVledOnload.WaitStepHomeFinish();

		theVledLfClamp.CloseLfClamp();
		break;
	}
	}

    return 0;
}

void CMaterialHandlerThread::SetThreadActive(bool bActive)
{
    m_bThreadActive = bActive;
}

bool CMaterialHandlerThread::IsThreadActive(void)
{
    return (m_bThreadActive);
}

void CMaterialHandlerThread::SetAutoIndex(bool bAutoIndex)
{
    m_bAutoIndex = bAutoIndex;
}

bool CMaterialHandlerThread::IsAutoIndex(void)
{
    return m_bAutoIndex;
}

bool CMaterialHandlerThread::IsDryRun(void)
{
    return m_bDryRun;
}

void CMaterialHandlerThread::SetDryRun(bool bDryRun)
{
    m_bDryRun = bDryRun;
}

void CMaterialHandlerThread::SetStopAll(bool bStopAll)
{
    m_bStopAll = bStopAll;
}

bool CMaterialHandlerThread::IsStopAll(void)
{
    return m_bStopAll;
}

void CMaterialHandlerThread::SetSensorEnable(bool bSensorEnable)
{
	m_bSensorEnable = bSensorEnable;
}

void CMaterialHandlerThread::InitAutoIndex(void)
{
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		theTopClamp.PreCloseClamp();
		theTopTrack.ResetSlipRcd();
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTopClamp.PreCloseClamp();
		theTEIndex.ResetSlipRcd();
		break;
	}
	case MHS_R2R:
	{
		theTopClamp.PreCloseClamp();
		theR2RTrack.ResetSlipRcd();
		break;
	}
	case MHS_CU_ELITE:
	{
		theCuEliteLFClamp.PreCloseClamp();
		theCuEliteTrack.ResetSlipRcd();
		break;
	}
	case MHS_VLED_ELITE:
	{
		theVEliteOnload.theLfExistSensor.UpdateSensorState();
		theVEliteOnload.theForkExistSensor.UpdateSensorState();

		theVEliteTrack.theOnloadTrackSensor.UpdateSensorState();
		theVEliteTrack.thePreBondSensor.UpdateSensorState();

		theVEliteOnload.ResetEvent();
		theVEliteTrack.ResetIndexEvent();
		theVEliteOffload.ResetEvent();

		if (theVEliteTrack.theLeftClaw.GetClawTicker() == 0 &&
			theVEliteTrack.theRightClaw.GetClawTicker() == 0)
		{
			theVEliteTrack.theLeftClaw.SetIndexEmpty(true);
			theVEliteTrack.SubmitIndexEvent(TRACK_EVENT_FIRST_INDEX);
		}
		break;
	}
	case MHS_VLED:
	{
		theVledOnload.theLfExistSensor.UpdateSensorState();
		theVledOnload.theForkExistSensor.UpdateSensorState();
		theVledOffload.theOffloadForkExistSensor.UpdateSensorState();

		theVledTrack.theOnloadTrackSensor.UpdateSensorState();
		theVledTrack.thePreBondSensor.UpdateSensorState();

		theVledOnload.ResetOnloadEvent();
		theVledTrack.ResetIndexEvent();
		theVledOffload.ResetForkEvent();

		if (theVledTrack.theLeftClaw.GetClawTicker() == 0 &&
			theVledTrack.theRightClaw.GetClawTicker() == 0)
		{
			theVledTrack.theLeftClaw.SetIndexEmpty(true);
			theVledTrack.SubmitIndexEvent(TRACK_EVENT_FIRST_INDEX);
		}
		break;
	}
	}
}

int CMaterialHandlerThread::ClearTrack()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		//v3.0.T362 add
		theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
		Sleep_x1ms(500);

		CStringList sList;
		sList.AddTail(theMsg.GetMsg(MSG_REMOVE_LF_ON_TRACK_AUTOMATICALLY));
		if (theMsg.ShowMsgX(WARNING_MSG, sList, 0, 0, 1) == SHOW_MSG_RESULT_STOP)
		{
			theTopTrack.SetTrackNeedClear(false);
			return 0;
		}

		theServo.MoveBondHeadToSafePos();
		Sleep_x1ms(100);

		//open clamper
		theTopClamp.MoveClampSafePos();
	    Sleep_x1ms(100);

		if ((theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
			|| (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE))
		{
			short anMsg[1] = {MSG_WAIT_ON_OFF_LOADER_FINISH};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsg);
			return 1;
		}
	 
		//reset event
		theTopOnloader.ResetEvent();
		theTopTrack.ResetEvent();
		theTopOffloader.ResetEvent();
		Sleep_x1ms(10);

		theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theTopTrack.SubmitClearTrackEvent();
		Sleep_x1ms(100);

		CHTimer timer;
		do
		{
			theTopTrack.ProcessIndexEvent();
			Sleep_x1ms(2);
		} while ((theTopTrack.GetEvent() != INDEX_EVENT_IDLE)
					&& (timer.GetTime_x1ms() < TIMEOUT_3));

		theTopTrack.ResetSlipRcd();
		theTopTrack.ResetErrorCode(1);
	    theTopTrack.ResetErrorCode(0);
		m_bLastLf = false;

		theBond.m_nCurWire = 1;
		theBond.m_nRestartWire = 1;

		theTopTrack.SetTrackNeedClear(false);

		if (thePgm.IsProgramValid())
			thePgm.GetRowCol(1, &theBond.m_nCurRow, &theBond.m_nCurCol);
       
		//close clamper
		theTopClamp.CloseClamp();
		Sleep_x1ms(100);

		if (theTopTrack.m_EjectSensor.GetCurrentState() == TRUE)
		{
			short anMsg[1] = {MSG_OFFLOAD_OUTPUT_JAM};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsg);
		}

		//v3.0.T362 add
		theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
		Sleep_x1ms(500);

		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTopClamp.SetVacuumOn(false);
		Sleep_x1ms(500);

		CStringList sList;
		sList.AddTail(theMsg.GetMsg(MSG_REMOVE_LF_ON_TRACK_AUTOMATICALLY));
		if (theMsg.ShowMsgX(WARNING_MSG, sList, 0, 0, 1) == SHOW_MSG_RESULT_STOP)
		{
			theTEIndex.SetTrackNeedClear(false);
			return 0;
		}

		theServo.MoveBondHeadToSafePos();
		Sleep_x1ms(100);

		//open clamper
		theTopClamp.MoveClampSafePos();
	    Sleep_x1ms(100);

		if ((theTopOnloader.GetEvent() != TOP_ONLOAD_EVENT_IDLE)
			|| (theTopOffloader.GetEvent() != TOP_OFFLOAD_EVENT_IDLE))
		{
			short anMsg[1] = {MSG_WAIT_ON_OFF_LOADER_FINISH};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsg);
			return 1;
		}
	 
		//reset event
		theTopOnloader.ResetEvent();
		theTEIndex.ResetEvent();
		theTopOffloader.ResetEvent();
		Sleep_x1ms(10);

		theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theTEIndex.SubmitClearTrackEvent();
		Sleep_x1ms(100);

		CHTimer timer;
		do
		{
			theTEIndex.ProcessIndexEvent();
			Sleep_x1ms(2);
		} while ((theTEIndex.GetEvent() != INDEX_EVENT_IDLE)
					&& (timer.GetTime_x1ms() < TIMEOUT_3));

		theTEIndex.ResetSlipRcd();
		theTEIndex.ResetErrorCode(1);
	    theTEIndex.ResetErrorCode(0);
		m_bLastLf = false;

		theBond.m_nCurWire = 1;
		theBond.m_nRestartWire = 1;

		theTEIndex.SetTrackNeedClear(false);

		if (thePgm.IsProgramValid())
			thePgm.GetRowCol(1, &theBond.m_nCurRow, &theBond.m_nCurCol);
       
		//close clamper
		theTopClamp.CloseClamp();
		Sleep_x1ms(100);

		if (theTEIndex.m_EjectSensor.GetCurrentState() == TRUE)
		{
			short anMsg[1] = {MSG_OFFLOAD_OUTPUT_JAM};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsg);
		}

		theTopClamp.SetVacuumOn(false);
		Sleep_x1ms(500);
		break;
    }
	case MHS_FOXCONN:	//v3.0.T217 add
		theFoxconnTrack.HomeIndexer();
		theFoxconnTrack.WaitHomeIndexer();
		break;
	case MHS_CU_ELITE:
	{
		CStringList sList;
		sList.AddTail(theMsg.GetMsg(MSG_REMOVE_LF_ON_TRACK_AUTOMATICALLY));
		if (theMsg.ShowMsgX(WARNING_MSG, sList, 0, 0, 1) == SHOW_MSG_RESULT_STOP)
		{
			theCuEliteTrack.SetTrackNeedClearFlag(FALSE);
			return 0;
		}

		if (theCuEliteOffloader.theMagDetectedSensor.GetCurrentState() == FALSE)
		{
			short anMsg[1] = {MSG_OFFLOAD_ELEV_NO_MAG};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsg);
		}

		long lElapseTime, lLastTime;
		short nEvent = 1;

		theServo.MoveBondHeadToSafePos();
		Sleep(100);

		//open clamper
		theCuEliteLFClamp.MoveClampSafePos();
	    Sleep(100);

		if ((theCuEliteOnloader.GetOnloadEvent() != CUELITE_ONLOAD_EVENT_IDLE) ||
			(theCuEliteOffloader.GetOffloadEvent() != CUELITE_OFFLOAD_EVENT_IDLE))
		{
			short anMsg[1] = {MSG_WAIT_ON_OFF_LOADER_FINISH};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsg);
			return 1;
		}
	 
		//--------------reset event-------------------------------

		theCuEliteOnloader.ResetOnloadEvent();
		theCuEliteTrack.ResetTrackEvent();
		theCuEliteOffloader.ResetOffloadEvent();
		Sleep(10);

		theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theCuEliteTrack.SubmitClearTrackEvent();
		Sleep(100);

		lLastTime = GetCurrentTime();
		do 
		{
			theCuEliteTrack.ProcessIndexEvent();
			lElapseTime = ::GetCurrentTime() - lLastTime;
			if (lElapseTime > TIMEOUT_3) break;
			Sleep(2);
		} while (theCuEliteTrack.GetCuEliteTrackEvent() != INDEX_EVENT_IDLE);

		theCuEliteTrack.ResetSlipRcd();
		theCuEliteTrack.ResetTrackErrorCode(1);
	    theCuEliteTrack.ResetTrackErrorCode(0);
		m_bLastLf = false;

		theBond.m_nCurWire = 1;
		theBond.m_nRestartWire = 1;

		theCuEliteTrack.SetTrackNeedClearFlag(FALSE);

		if (thePgm.IsProgramValid())
			thePgm.GetRowCol(1, &theBond.m_nCurRow, &theBond.m_nCurCol);

		//close clamper
		theCuEliteLFClamp.CloseClamp();
		Sleep(100);

		if (theCuEliteTrack.theEjectSnr.GetCurrentState() == TRUE)
		{
			short anMsg[1] = {MSG_OFFLOAD_OUTPUT_JAM};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsg);
		}
		break;
    }
	case MHS_VLED_ELITE:
    {
        SetRequestIndex(false);
        SetBusy(false);

        theVEliteTrack.theLeftClaw.UpdateClawState();

        SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
        theVEliteTrack.GetRcd(&pstLfTrackRcd);
        Sleep_x1ms(10);

        theVEliteTrack.ResetIndexEvent();
        theVEliteOffload.ResetEvent();
        theVEliteLfClamp.OpenLfClamp();
        Sleep_x1ms(100);

		theVEliteLfClamp.CheckLfClampStatus(FALSE);

		theVEliteTrack.theLeftClaw.ResetAll();
		theVEliteTrack.theRightClaw.ResetAll();

		theVEliteTrack.theRightClaw.IndexSteps(pstLfTrackRcd.nLeadFrameUnits + 1);
		Sleep_x1ms(2000);

		theVEliteTrack.theLeftClaw.IndexSteps(pstLfTrackRcd.nLeadFrameUnits);
		Sleep_x1ms(2000);
		theVEliteTrack.theRightClaw.IndexSteps(pstLfTrackRcd.nLeadFrameUnits + 1);
		Sleep_x1ms(2000);

		theVEliteLfClamp.CloseLfClamp();

		theVEliteTrack.theLeftClaw.ResetAll();
		theVEliteTrack.theRightClaw.ResetAll();
		theVEliteTrack.ResetIndexEvent();
		theVEliteTrack.SetBondUnitNum(0);

		theVEliteOffload.ResetEvent();

		theBond.m_nCurWire = 1;
		theBond.m_nRestartWire = 1;
		break;
    }
	case MHS_VLED:
    {
        long lStartTime, lElapseTime;
        short nCurrentUnit, nLeftTicker;

        SetRequestIndex(false);
        SetBusy(false);

        theVledTrack.theLeftClaw.UpdateClawState();

        nCurrentUnit = theVledTrack.GetBondUnitNum();
        nLeftTicker = theVledTrack.theLeftClaw.GetClawTicker(); 
        SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
        theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
        Sleep(10);

        theVledTrack.ResetIndexEvent();
        theVledOffload.ResetForkEvent();
        theVledLfClamp.OpenLfClamp();
        Sleep(100);

		theVledLfClamp.CheckLfClampStatus(FALSE);

		theVledTrack.theLeftClaw.ResetAll();
		theVledTrack.theRightClaw.ResetAll();

		theVledTrack.theRightClaw.IndexSteps(pstLfTrackRcd.nLeadFrameUnits + 1); //@18
		Sleep(2000);

		lStartTime = GetCurrentTime();
		theVledOffload.SubmitOffloadEvent();
		do
		{
			theVledOffload.ProcessOffloadEvent();

			if ((theVledOffload.GetOffloadEvent() == FORK_OFFLOAD_EVENT_IDLE) ||
				(theVledOffload.GetOffloadErrorCode() != MHS_NO_ERROR)) //@15
				break;

			Sleep(10);

			lElapseTime = GetCurrentTime() - lStartTime;
		} while (lElapseTime < 8000);

		theVledTrack.theLeftClaw.IndexSteps(pstLfTrackRcd.nLeadFrameUnits);
		Sleep(2000);
		theVledTrack.theRightClaw.IndexSteps(pstLfTrackRcd.nLeadFrameUnits + 1); //@21
		Sleep(2000);

		theVledLfClamp.CloseLfClamp();

		theVledOffload.SubmitOffloadEvent();

		lStartTime = GetCurrentTime();
		do
		{
			theVledOffload.ProcessOffloadEvent();

			if ((theVledOffload.GetOffloadEvent() == FORK_OFFLOAD_EVENT_IDLE) ||
				(theVledOffload.GetOffloadErrorCode() != MHS_NO_ERROR)) //@15
				break;

			Sleep(10);

			lElapseTime = GetCurrentTime() - lStartTime;
		} while (lElapseTime < 8000);

		theVledTrack.theLeftClaw.ResetAll();
		theVledTrack.theRightClaw.ResetAll();
		theVledTrack.ResetIndexEvent();
		theVledTrack.SetBondUnitNum(0);

        theVledOffload.ResetForkEvent();

		theBond.m_nCurWire = 1;
		theBond.m_nRestartWire = 1;
		break;
    }
	}

    return 0;
}

int CMaterialHandlerThread::ResetWorkHolder(void)
{
    long lStartTime, lElapseTime;
    SINGLE_CAP_TRACK_RCD stLfTrackRcd;
    theVledTrack.GetLfTrackRcd(&stLfTrackRcd);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_FOXCONN:
    {
	    theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theFoxconnTrack.HomeIndexer();
		theFoxconnTrack.WaitHomeIndexer();
		break;
    }
	case MHS_TOP_LED:
    {
	    theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theTopTrack.ResetSlipRcd();
		theTopTrack.HomeIndexer();
		Sleep(10);
		theTopTrack.WaitHomeIndexer();
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
	    theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theTEIndex.ResetSlipRcd();
		theTEIndex.HomeIndexer();
		Sleep(10);
		theTEIndex.WaitHomeIndexer();
		break;
    }
	case MHS_R2R:
    {
	    theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theR2RTrack.ResetSlipRcd();
		theR2RTrack.SearchIndexStepHome();
		Sleep(10);
		theR2RTrack.WaitStepHomeFinish();
		break;
    }
	case MHS_CU_ELITE:
    {
	    theMHS.SetAutoIndex(false);
		theMHS.SetRequestIndex(false);
		theCuEliteTrack.ResetSlipRcd();   
		theCuEliteTrack.SearchIndexStepHome();
		Sleep(10);
		theCuEliteTrack.WaitStepHomeFinish();
		break;
    }
	case MHS_VLED_ELITE:
    {
        // 1.clear all the error
        ShowError();

        //2. clear all LF in the track and reset the event
        SetRequestIndex(false);
        SetBusy(false);
        Sleep(10);

        theVEliteTrack.ResetIndexEvent();
        theVEliteOffload.ResetEvent();
        theVEliteTrack.theLeftClaw.ResetAll();
        theVEliteTrack.theRightClaw.ResetAll();

        theVEliteLfClamp.OpenLfClamp();
        Sleep(200);
        theVEliteTrack.theRightClaw.IndexSteps(stLfTrackRcd.nLeadFrameUnits + 1);
        Sleep(2000);

        theVEliteTrack.theLeftClaw.IndexSteps(stLfTrackRcd.nLeadFrameUnits);
        Sleep(2000);
        theVEliteTrack.theRightClaw.IndexSteps(stLfTrackRcd.nLeadFrameUnits + 1);
        Sleep(2000);

        theVEliteLfClamp.CloseLfClamp();
        //if (lElapseTime >= 8000) return 0;

        theVEliteTrack.theLeftClaw.ResetAll();
        theVEliteTrack.theRightClaw.ResetAll();
        theVEliteTrack.SetBondUnitNum(0);
        theVEliteTrack.SetOffloadEnable(false);

		theVEliteOffload.ResetEvent();
        theVEliteOnload.ResetEvent();
        theVEliteTrack.ResetIndexEvent();

        //3. warm reset all the motor
        WarmReset();
		break;
    }
	case MHS_VLED:
    {
        // 1.clear all the error
        ShowError();

        //2. clear all LF in the track and reset the event
        SetRequestIndex(false);
        SetBusy(false);

        Sleep(10);

        theVledTrack.ResetIndexEvent();
        theVledOffload.ResetForkEvent();
        theVledTrack.theLeftClaw.ResetAll();
        theVledTrack.theRightClaw.ResetAll();

        theVledLfClamp.OpenLfClamp();
        Sleep(200);
        theVledTrack.theRightClaw.IndexSteps(stLfTrackRcd.nLeadFrameUnits + 1);
        Sleep(2000);

        lStartTime = GetCurrentTime();
        theVledOffload.SubmitOffloadEvent();
        do
		{
            theVledOffload.ProcessOffloadEvent();

            if ((theVledOffload.GetOffloadEvent() == FORK_OFFLOAD_EVENT_IDLE) ||
                (theVledOffload.GetOffloadErrorCode() != MHS_NO_ERROR)) //@15
                break;

            Sleep(10);

            lElapseTime = GetCurrentTime() - lStartTime;
        } while (lElapseTime < 8000);

        theVledTrack.theLeftClaw.IndexSteps(stLfTrackRcd.nLeadFrameUnits);
        Sleep(2000);
        theVledTrack.theRightClaw.IndexSteps(stLfTrackRcd.nLeadFrameUnits + 1); //@21
        Sleep(2000);

        theVledLfClamp.CloseLfClamp();
        if (lElapseTime >= 8000) return 0;

        theVledOffload.SubmitOffloadEvent();

        lStartTime = GetCurrentTime();
        do
		{
            theVledOffload.ProcessOffloadEvent();

            if ((theVledOffload.GetOffloadEvent() == FORK_OFFLOAD_EVENT_IDLE) ||
                (theVledOffload.GetOffloadErrorCode() != MHS_NO_ERROR)) //@15
                break;

            Sleep(10);

            lElapseTime = GetCurrentTime() - lStartTime;
        } while (lElapseTime < 8000);

        theVledTrack.theLeftClaw.ResetAll();
        theVledTrack.theRightClaw.ResetAll();
        theVledTrack.SetBondUnitNum(0); //@6
        theVledTrack.SetOffloadEnableFlag(false);  //@17

        theVledOffload.ResetForkEvent();
        theVledOnload.ResetOnloadEvent();
        theVledTrack.ResetIndexEvent();

        //3. warm reset all the motor
        WarmReset();
		break;
    }
	}

    return 0;
}

void CMaterialHandlerThread::ReportError(short* panErrorCode, bool bWorkerThread)
{
    short i = 0, nClearTrackId = 0, nCallBackId = MSG_CALLBACK_NONE;
    CString sMsg;
    CStringList sList;
    sList.RemoveAll();

    for (i = 0; i < MHS_ERROR_MAX; i++)
    {
		if (panErrorCode[i] == MHS_NO_ERROR) break;

		sMsg.Empty();
        switch (panErrorCode[i])
        {
        case MHS_CLAW_MISSING_STEPS:
            sMsg = theMsg.GetMsg(MSG_CLAW_MISSING_STEP);
            break;
        case MHS_ONLOAD_EMPTY:
            sMsg = theMsg.GetMsg(MSG_ONLOAD_FORK_EMPTY);
            nCallBackId = MSG_CALLBACK_ONLOAD_EMPTY;
            break;
        case MHS_ONLOAD_TRACK_JAM:
            sMsg = theMsg.GetMsg(MSG_ONLOAD_TRACK_JAM);
            break;
        case MHS_OFFLOAD_KICKER_JAM:
            sMsg = theMsg.GetMsg(MSG_OFFLOAD_KICKER_JAM);
            if (nClearTrackId == 0) nClearTrackId = 1;
            break;
        case MHS_OFFLOAD_PUSHER_JAM:
            sMsg = theMsg.GetMsg(MSG_OFFLOAD_PUSHER_JAM);

			switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
			{
			case MHS_TOP_LED:
			case MHS_TOP_ELITE:	//v3.0.T410 add
	        {
				nCallBackId = MSG_CALLBACK_OFFLOAD_JAM;
				theTopOffloader.SetJamNotHandled(true);
				break;
			}
			case MHS_CU_ELITE:
	        {
				nCallBackId = MSG_CALLBACK_OFFLOAD_JAM; 
				theCuEliteOffloader.ActiveOffloadJamHandleEvent();
				break;
			}
			}
            break;
        case MHS_OFFLOAD_FORK_FULL:
            sMsg = theMsg.GetMsg(MSG_OFFLOAD_FORK_FULL);
            nCallBackId = MSG_CALLBACK_OFFLOAD_FORK_FULL;
            break;
        case MHS_ONLOAD_DOUBLE_LEAD_FRAME:
            sMsg = theMsg.GetMsg(MSG_ONLOAD_TRACK_DOUBLE_LF);
			nCallBackId = MSG_CALLBACK_DOUBLE_LF_HANDLE;
            break;
        case MHS_END_ALL_LF:
			LastLeadframeHandle();
            return;

		//TOP
		case MHS_TOP_ONLOAD_NO_LF:
			sMsg = theMsg.GetEngMsg(MSG_NO_LF_PUSHED_OUT);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_NO_LF_PUSHED_OUT);
            break;
		case MHS_TOP_ONLOAD_NO_MAG:
			sMsg = theMsg.GetEngMsg(MSG_ONLOAD_ELEV_NO_MAG);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_ONLOAD_ELEV_NO_MAG);
            break;
		case MHS_TOP_OFFLOAD_NO_MAG:
			sMsg = theMsg.GetEngMsg(MSG_OFFLOAD_ELEV_NO_MAG);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_OFFLOAD_ELEV_NO_MAG);
			break;
		case MHS_TOP_SRCH_TIME_OUT:
			sMsg = theMsg.GetEngMsg(MSG_SRCH_TIME_OUT);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_SRCH_TIME_OUT);
			//theTopTrack.SetCurrentBondUnit(0);
			//nCallBackId = MSG_CALLBACK_SRCH_TIME_OUT;
            break;
		case MHS_TOP_EJECT_DIST_NOT_ENOUGH:
			sMsg = theMsg.GetEngMsg(MSG_EJECT_WRONG);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_EJECT_WRONG);
			break;
		case MHS_TOP_ONLOADER_NO_RESPONSE:
			sMsg = theMsg.GetEngMsg(MSG_ONLOAD_ELEV_NO_RESPONSE);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_ONLOAD_ELEV_NO_RESPONSE);
            break;
		case MHS_TOP_ONLOAD_MAG_BUF_EMPTY:
			sMsg = theMsg.GetEngMsg( MSG_ONLOAD_MAG_BUFFER_EMPTY);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_ONLOAD_MAG_BUFFER_EMPTY);
			nCallBackId = MSG_CALLBACK_ONLOAD_MAG_BUF_EMPTY;  
			break;
		case MHS_TOP_OFFLOAD_MAG_BUF_EMPTY:
			sMsg = theMsg.GetEngMsg(MSG_OFFLOAD_MAG_BUFFER_EMPTY);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_OFFLOAD_MAG_BUFFER_EMPTY);
			nCallBackId = MSG_CALLBACK_OFFLOAD_MAG_BUF_EMPTY;
			break;
		case MHS_TOP_ONLOAD_MAG_PULLY_FULL:
			sMsg = theMsg.GetEngMsg( MSG_ONLOAD_MAG_PULLY_FULL); 
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg( MSG_ONLOAD_MAG_PULLY_FULL);
			nCallBackId = MSG_CALLBACK_ONLOAD_PULLY_FULL;
			break;
		case MHS_TOP_OFFLOAD_MAG_PULLY_FULL:
			sMsg = theMsg.GetEngMsg( MSG_OFFLOAD_MAG_PULLY_FULL);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg( MSG_OFFLOAD_MAG_PULLY_FULL);
			nCallBackId = MSG_CALLBACK_OFFLOAD_PULLY_FULL;
			break;
		case MHS_TOP_ONLOAD_PULLY_MOTOR_JAM:
			sMsg = theMsg.GetEngMsg(MSG_ONLOAD_MAG_PULLY_JAM);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_ONLOAD_MAG_PULLY_JAM);
			break;
		case MHS_TOP_OFFLOAD_PULLY_MOTOR_JAM:
			sMsg = theMsg.GetEngMsg(MSG_OFFLOAD_MAG_PULLY_JAM);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_OFFLOAD_MAG_PULLY_JAM);
			break;
		case MHS_TOP_OFFLOAD_TIME_OUT:
			sMsg = theMsg.GetEngMsg(MSG_ONLOAD_PULLY_JAM);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_ONLOAD_PULLY_JAM);
			break;
		case MHS_TOP_BH_NOT_IN_SAFE_POS:
			theLog.WriteLine(theMsg.GetEngMsg(MSG_BH_NOT_IN_SAFE_POS), true);	//v2.0.T84 add
			sMsg = theMsg.GetMsg(MSG_BH_NOT_IN_SAFE_POS);
			break; 
        case MHS_OFFLOAD_TRACK_JAM:
			sMsg = theMsg.GetEngMsg(MSG_OFFLOAD_OUTPUT_JAM);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_OFFLOAD_OUTPUT_JAM);

			switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
			{
			case MHS_TOP_LED:
			case MHS_TOP_ELITE:	//v3.0.T410 add
	        {
				nCallBackId = MSG_CALLBACK_OFFLOAD_JAM;
				theTopOffloader.SetJamNotHandled(true);
				break;
			}
			case MHS_CU_ELITE:
	        {
				nCallBackId = MSG_CALLBACK_OFFLOAD_JAM; 
				theCuEliteOffloader.ActiveOffloadJamHandleEvent();
				break;
			}
			}
            break;
		case MHS_TOP_ONLOAD_PUSH_KICKER_WRONG:
			sMsg = theMsg.GetEngMsg(MSG_ONLOAD_PUSH_KICKER_WRONG);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_ONLOAD_PUSH_KICKER_WRONG);
			break;
		case MHS_TOP_WINDOW_CLAMPER_NOT_CLOSED:		//V1.5.W11 add
			sMsg = theMsg.GetEngMsg(MSG_WINDOW_CLAMP_NOT_CLOSE);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_WINDOW_CLAMP_NOT_CLOSE);
			break;
		case MHS_TOP_WINDOW_CLAMPER_NOT_OPENED:		//V1.5.W11 add
			sMsg = theMsg.GetEngMsg(MSG_WINDOW_CLAMP_NOT_OPEN);
			theLog.WriteLine(sMsg, true);	//v2.0.T84 add

			sMsg = theMsg.GetMsg(MSG_WINDOW_CLAMP_NOT_OPEN);
			break;

		//R2R
		case MHS_R2R_ONLOAD_STOP:
			sMsg = theMsg.GetMsg(MSG_ONLOAD_DOWN);
			break;
		case MHS_R2R_OFFLOAD_STOP:
			sMsg = theMsg.GetMsg(MSG_OFFLOAD_DOWN);
			break;
		case MHS_R2R_ONLOAD_ERROR:
			sMsg = theMsg.GetMsg(MSG_ONLOAD_ERR);
			break;
		case MHS_R2R_OFFLOAD_ERROR:
			sMsg = theMsg.GetMsg(MSG_OFFLOAD_ERR);
			break;
		
		//CU ELITE
		case MHS_CUELITE_ONLOAD_NO_LF:
			sMsg = theMsg.GetMsg(MSG_NO_LF_PUSHED_OUT);
            break;
		case MHS_CUELITE_ONLOAD_NO_MAG:
			sMsg = theMsg.GetMsg(MSG_ONLOAD_ELEV_NO_MAG);
            break;
		case MHS_CUELITE_OFFLOAD_NO_MAG:
			sMsg = theMsg.GetMsg(MSG_OFFLOAD_ELEV_NO_MAG);
			break;
		case MHS_CUELITE_SRCH_TIME_OUT:
			sMsg = theMsg.GetMsg(MSG_SRCH_TIME_OUT);
            break;
		case MHS_CUELITE_EJECT_DIST_NOT_ENOUGH:
			sMsg = theMsg.GetMsg(MSG_EJECT_WRONG);
			break;
		case MHS_CUELITE_ONLOADER_NO_RESPONSE:
			sMsg = theMsg.GetMsg(MSG_ONLOAD_ELEV_NO_RESPONSE);
            break;
		case MHS_CUELITE_ONLOAD_MAG_BUF_EMPTY:
            sMsg = theMsg.GetMsg(MSG_ONLOAD_MAG_BUFFER_EMPTY);
			nCallBackId = MSG_CALLBACK_ONLOAD_MAG_BUF_EMPTY;
			break;
		case MHS_CUELITE_OFFLOAD_MAG_BUF_EMPTY:
            sMsg = theMsg.GetMsg(MSG_OFFLOAD_MAG_BUFFER_EMPTY);
			nCallBackId = MSG_CALLBACK_OFFLOAD_MAG_BUF_EMPTY;
			break;
		case MHS_CUELITE_ONLOAD_MAG_PULLY_FULL:
			sMsg = theMsg.GetMsg( MSG_ONLOAD_MAG_PULLY_FULL); 
			nCallBackId = MSG_CALLBACK_ONLOAD_PULLY_FULL;
			break;
		case MHS_CUELITE_OFFLOAD_MAG_PULLY_FULL:
			sMsg = theMsg.GetMsg( MSG_OFFLOAD_MAG_PULLY_FULL);
			nCallBackId = MSG_CALLBACK_OFFLOAD_PULLY_FULL;
			break;
		case MHS_CUELITE_ONLOAD_PULLY_MOTOR_JAM:
			sMsg = theMsg.GetMsg(MSG_ONLOAD_MAG_PULLY_JAM);
			break;
		case MHS_CUELITE_OFFLOAD_PULLY_MOTOR_JAM:
			sMsg = theMsg.GetMsg(MSG_OFFLOAD_MAG_PULLY_JAM);
			break;
		case MHS_CUELITE_OFFLOAD_TIME_OUT:
			sMsg = theMsg.GetMsg(MSG_ONLOAD_PULLY_JAM);
			break;
		case MHS_CUELITE_BOND_HEAD_NOT_SEFTY:
			sMsg = theMsg.GetMsg(MSG_BH_NOT_IN_SAFE_POS);
			break;
		case MHS_CUELITE_INJECTOR_ERROR:
			sMsg = theMsg.GetMsg(MSG_INJECTOR_ERR);
			break;
		case MHS_CUELITE_OFFLOAD_JAM:
			sMsg = theMsg.GetMsg(MSG_OFFLOAD_TIMEOUT);
			break;
		case MHS_CUELITE_ONLOAD_PUSH_KICKER_WRONG:
			sMsg = theMsg.GetMsg(MSG_ONLOAD_PUSH_KICKER_WRONG);
			break;
		case MHS_VE_OFFLOAD_MAG_ARM_EMPTY:
			sMsg = theMsg.GetMsg(MSG_MAG_ARM_EMPTY);
			break;
		case MHS_VE_OFFLOAD_MAG_ARM_FULL:
			sMsg = theMsg.GetMsg(MSG_MAG_ARM_FULL);
			break;
		case MHS_VE_OFFLOAD_MAG_DROP_FAIL:
			sMsg = theMsg.GetMsg(MSG_MAG_DROP_FAIL);
			break;
		case MHS_VE_OFFLOAD_MAG_KICKER_FULL:
			sMsg = theMsg.GetMsg(MSG_MAG_KICKER_FULL);
			break;
		case MHS_VE_OFFLOAD_LF_OUTPUT_JAM:
			sMsg = theMsg.GetMsg(MSG_LF_OUTPUT_JAM);
			break;
        default:
            break;
        }

		if (!sMsg.IsEmpty()) sList.AddTail(sMsg);
    }

	//theMHS.SetOxidationProtect(true);

	if (!bWorkerThread) 
		//modeless, must be used on GUI
		theMsg.ShowMhsMsg(WARNING_MSG, sList, nCallBackId, nClearTrackId);
	else
		//mode , can be used on GUI or worker thread
		theMsg.ShowMsg(WARNING_MSG, sList);

	//theMHS.SetOxidationProtect(false);
}

short CMaterialHandlerThread::GetTopStopReason(void)
{
	short nMhsError = FOK, nMhsWarning = FOK; 
    short i = 0, nErrorStatus = FOK, anErrorList[MHS_ERROR_MAX] = {0};

	nErrorStatus = theTopTrack.GetErrorCode(1);
	if (nErrorStatus != FOK)
	{
		anErrorList[i] = nErrorStatus;
		i++;
		nMhsError = FNOK;

		theTopTrack.ResetErrorCode(1);
	}
    if ((nMhsError == FNOK) || (nMhsWarning == FNOK))
        theMHS.ReportError(anErrorList);
    return nMhsError;
}

short CMaterialHandlerThread::GetR2RStopReason(void)
{
	short nMhsError = FOK, nMhsWarning = FOK; 
    short i = 0, nErrorStatus = FOK, anErrorList[MHS_ERROR_MAX] = {0};

	nErrorStatus = theR2RTrack.GetTrackErrorCode(1);
	if (nErrorStatus != FOK)
	{
		anErrorList[i] = nErrorStatus;
		i++;
		nMhsError = FNOK;

		theR2RTrack.ResetTrackErrorCode(1);
	}
    if ((nMhsError == FNOK) || (nMhsWarning == FNOK))
        theMHS.ReportError(anErrorList);
    return nMhsError;
}

short CMaterialHandlerThread::GetCuEliteStopReason(void)
{
    short nMhsError = FOK, nMhsWarning = FOK; 
    short i = 0, nErrorStatus = FOK, anErrorList[MHS_ERROR_MAX] = {0};

	nErrorStatus = theCuEliteTrack.GetTrackErrorCode(1);
	if (nErrorStatus != FOK)
	{
		anErrorList[i] = nErrorStatus;
		i++;
		nMhsError = FNOK;

		theCuEliteTrack.ResetTrackErrorCode(1);
	}

	if ((nMhsError == FNOK) || (nMhsWarning == FNOK))
	   theMHS.ReportError(anErrorList);

	return nMhsError;
}

//add bool bWorkerThread
//because worker thread CANNOT display modeless dialog directly, mode dialog ok
short CMaterialHandlerThread::ShowError(bool bWorkerThread)
{
    short nMhsError = FOK, nMhsWarning = FOK;
    short i = 0, nErrorStatus = FOK, anErrorList[MHS_ERROR_MAX] = {0};

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_R2R:
	{
		nErrorStatus = theR2RTrack.GetTrackErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theR2RTrack.ResetTrackErrorCode();
		}

		nErrorStatus = theR2ROffloader.GetOffloadErrorCode();
		if ((nErrorStatus != FOK)
			&& theR2ROffloader.GetOffloadErrorReported() == false)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theR2ROffloader.ResetOffloadErrorCode();
			theR2ROffloader.SetOffloadErrorReported(true);
		}

		nErrorStatus = theR2ROnloader.GetOnloadErrorCode();
		if ((nErrorStatus != FOK)
			&& theR2ROnloader.GetOnloadErrorReported() == false)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theR2ROnloader.ResetOnloadErrorCode();
			theR2ROnloader.SetOnloadErrorReported(true);
		}
		break;
	}
	case MHS_TOP_LED:
	{
		if (theTopOnloader.IsEmpty()
			&& (theTopOnloader.IsEmptyReported() == false))
		{
			anErrorList[i] = MHS_TOP_ONLOAD_NO_MAG;
			i++;
			nMhsWarning = FNOK;
			theTopOnloader.SetEmptyReported(true);
		}
		else if (theTopOnloader.IsEmpty()
				&& theTopOnloader.IsEmptyReported())
		{
			anErrorList[i] = MHS_TOP_ONLOAD_NO_MAG;
			i++;
		}
		else
		{
			theTopOnloader.SetEmptyReported(false);
		}

		//add protection when no magzine on offloader
		if (theTopOffloader.IsEmpty()
			&& (theTopOffloader.IsEmptyReported() == false))
		{
			anErrorList[i] = MHS_TOP_OFFLOAD_NO_MAG;
			i++;
			nMhsError = FNOK;
			theTopOffloader.SetEmptyReported(true);
		}
		else if (theTopOffloader.IsEmpty()
				&& theTopOffloader.IsEmptyReported())
		{
			anErrorList[i] = MHS_TOP_OFFLOAD_NO_MAG;
			i++;
		}
		else
			theTopOffloader.SetEmptyReported(false);

		if (theTopOnloader.IsPusherFull()
			&& theTopOnloader.IsPusherFullReported() == false)
		{
			theTopOnloader.SetPusherFullReported(true);
			anErrorList[i] = MHS_TOP_ONLOAD_MAG_PULLY_FULL;
			i++;
			nMhsWarning = FNOK;
		}
		else if (theTopOnloader.IsPusherFull()
				&& theTopOnloader.IsPusherFullReported())
		{
			anErrorList[i] = MHS_TOP_ONLOAD_MAG_PULLY_FULL;
			i++;
		}
		else
			theTopOnloader.SetPusherFullReported(false);

		if (theTopOffloader.IsPusherFull()
			&& (theTopOffloader.IsPusherFullReported() == false))
		{
			theTopOffloader.SetPusherFullReported(true);
			anErrorList[i] = MHS_TOP_OFFLOAD_MAG_PULLY_FULL;
			i++;
			nMhsWarning = FNOK;
		}
		else if (theTopOffloader.IsPusherFull()
				&& theTopOffloader.IsPusherFullReported())
		{
			anErrorList[i] = MHS_TOP_OFFLOAD_MAG_PULLY_FULL;
			i++;
		}
		else
			theTopOffloader.SetPusherFullReported(false);

		if (theTopOnloader.IsBufferEmpty())
		{
			TOP_DEVICE_RECORD stTopDevicePara;
			theDevice.GetTopRcd(&stTopDevicePara);

			TOP_ONLOAD_RCD stOnloadData;
			theTopOnloader.GetRcd(&stOnloadData);

			if (theTopOnloader.GetCurSlot() <= stOnloadData.lOnloadSlotNum)
			{
				if (theTopOnloader.IsBufferEmptyReported() == false)
				{
					theTopOnloader.SetBufferEmptyReported(true);
					anErrorList[i] = MHS_TOP_ONLOAD_MAG_BUF_EMPTY;
					i++;
					nMhsWarning = FNOK;
				}
				else
				{
					anErrorList[i] = MHS_TOP_ONLOAD_MAG_BUF_EMPTY;
					i++;
				}
			}
		}
		else
			theTopOnloader.SetBufferEmptyReported(false);

		if (theTopOffloader.IsBufferEmpty())
		{
			TOP_DEVICE_RECORD  stTopDevicePara;
			theDevice.GetTopRcd(&stTopDevicePara);

			TOP_OFFLOAD_RCD  stOffloadData;
			theTopOffloader.GetRcd(&stOffloadData);

			if (theTopOffloader.GetCurSlot() <= stOffloadData.lOffloadSlotNum)
			{
				if (theTopOffloader.IsBufferEmptyReported() == false)
				{
					theTopOffloader.SetBufferEmptyReported(true);
					anErrorList[i] = MHS_TOP_OFFLOAD_MAG_BUF_EMPTY;
					i++;
					nMhsWarning = FNOK;
				}
				else
				{
					if (IsSameMhsError(anErrorList,nErrorStatus,i)==false)
					{
						anErrorList[i] = MHS_TOP_OFFLOAD_MAG_BUF_EMPTY;
						i++;
					}
				}
			}
		}
		else
			theTopOffloader.SetBufferEmptyReported(false);

		nErrorStatus = theTopTrack.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
			}
			theTopTrack.ResetErrorCode();
		}

		nErrorStatus = theTopOffloader.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
			}

			theTopOffloader.ResetErrorCode();
		}

		nErrorStatus = theTopOnloader.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
				theTopOnloader.ResetErrorCode();
			}
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		if (theTopOnloader.IsEmpty()
			&& (theTopOnloader.IsEmptyReported() == false))
		{
			anErrorList[i] = MHS_TOP_ONLOAD_NO_MAG;
			i++;
			nMhsWarning = FNOK;
			theTopOnloader.SetEmptyReported(true);
		}
		else if (theTopOnloader.IsEmpty()
				&& theTopOnloader.IsEmptyReported())
		{
			anErrorList[i] = MHS_TOP_ONLOAD_NO_MAG;
			i++;
		}
		else
			theTopOnloader.SetEmptyReported(false);

		//add protection when no magzine on offloader
		if (theTopOffloader.IsEmpty()
			&& (theTopOffloader.IsEmptyReported() == false))
		{
			anErrorList[i] = MHS_TOP_OFFLOAD_NO_MAG;
			i++;
			nMhsError = FNOK;
			theTopOffloader.SetEmptyReported(true);
		}
		else if (theTopOffloader.IsEmpty()
				&& theTopOffloader.IsEmptyReported())
		{
			anErrorList[i] = MHS_TOP_OFFLOAD_NO_MAG;
			i++;
		}
		else
			theTopOffloader.SetEmptyReported(false);

		if (theTopOnloader.IsPusherFull()
			&& theTopOnloader.IsPusherFullReported() == false)
		{
			theTopOnloader.SetPusherFullReported(true);
			anErrorList[i] = MHS_TOP_ONLOAD_MAG_PULLY_FULL;
			i++;
			nMhsWarning = FNOK;
		}
		else if (theTopOnloader.IsPusherFull()
				&& theTopOnloader.IsPusherFullReported())
		{
			anErrorList[i] = MHS_TOP_ONLOAD_MAG_PULLY_FULL;
			i++;
		}
		else
			theTopOnloader.SetPusherFullReported(false);

		if (theTopOffloader.IsPusherFull()
			&& (theTopOffloader.IsPusherFullReported() == false))
		{
			theTopOffloader.SetPusherFullReported(true);
			anErrorList[i] = MHS_TOP_OFFLOAD_MAG_PULLY_FULL;
			i++;
			nMhsWarning = FNOK;
		}
		else if (theTopOffloader.IsPusherFull()
				&& theTopOffloader.IsPusherFullReported())
		{
			anErrorList[i] = MHS_TOP_OFFLOAD_MAG_PULLY_FULL;
			i++;
		}
		else
			theTopOffloader.SetPusherFullReported(false);

		if (theTopOnloader.IsBufferEmpty())
		{
			TOP_DEVICE_RECORD stTopDevicePara;
			theDevice.GetTopRcd(&stTopDevicePara);

			TOP_ONLOAD_RCD stOnloadData;
			theTopOnloader.GetRcd(&stOnloadData);

			if (theTopOnloader.GetCurSlot() <= stOnloadData.lOnloadSlotNum)
			{
				if (theTopOnloader.IsBufferEmptyReported() == false)
				{
					theTopOnloader.SetBufferEmptyReported(true);
					anErrorList[i] = MHS_TOP_ONLOAD_MAG_BUF_EMPTY;
					i++;
					nMhsWarning = FNOK;
				}
				else
				{
					anErrorList[i] = MHS_TOP_ONLOAD_MAG_BUF_EMPTY;
					i++;
				}
			}
		}
		else
			theTopOnloader.SetBufferEmptyReported(false);

		if (theTopOffloader.IsBufferEmpty())
		{
			TOP_DEVICE_RECORD  stTopDevicePara;
			theDevice.GetTopRcd(&stTopDevicePara);

			TOP_OFFLOAD_RCD  stOffloadData;
			theTopOffloader.GetRcd(&stOffloadData);

			if (theTopOffloader.GetCurSlot() <= stOffloadData.lOffloadSlotNum)
			{
				if (theTopOffloader.IsBufferEmptyReported() == false)
				{
					theTopOffloader.SetBufferEmptyReported(true);
					anErrorList[i] = MHS_TOP_OFFLOAD_MAG_BUF_EMPTY;
					i++;
					nMhsWarning = FNOK;
				}
				else
				{
					if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
					{
						anErrorList[i] = MHS_TOP_OFFLOAD_MAG_BUF_EMPTY;
						i++;
					}
				}
			}
		}
		else
			theTopOffloader.SetBufferEmptyReported(false);

		nErrorStatus = theTEIndex.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
			}
			theTEIndex.ResetErrorCode();
		}

		nErrorStatus = theTopOffloader.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
			}

			theTopOffloader.ResetErrorCode();
		}

		nErrorStatus = theTopOnloader.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
				theTopOnloader.ResetErrorCode();
			}
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		if (theCuEliteOnloader.GetOnloadEmptyflag()
			&& theCuEliteOnloader.GetOnloadEmptyReported() == false)
		{
			anErrorList[i] = MHS_CUELITE_ONLOAD_NO_MAG;
			i++;
			nMhsWarning = FNOK;
			theCuEliteOnloader.SetOnloadEmptyReported(true);
		}
		else if (theCuEliteOnloader.GetOnloadEmptyflag()
				&& theCuEliteOnloader.GetOnloadEmptyReported())
		{
			anErrorList[i] = MHS_CUELITE_ONLOAD_NO_MAG;
			i++;
		}
		else
			theCuEliteOnloader.SetOnloadEmptyReported(false);

		//add protection when no magzine on offloader
		if (theCuEliteOffloader.GetOffloadEmptyflag()
			&& theCuEliteOffloader.GetOffloadEmptyReported() == false)
		{
			anErrorList[i] = MHS_CUELITE_OFFLOAD_NO_MAG;
			i++;
			nMhsWarning = FNOK;
			theCuEliteOffloader.SetOffloadEmptyReported(true);
		}
		else if (theCuEliteOffloader.GetOffloadEmptyflag()
				&& theCuEliteOffloader.GetOffloadEmptyReported())
		{
			anErrorList[i] = MHS_CUELITE_OFFLOAD_NO_MAG;
			i++;
		}
		else
			theCuEliteOffloader.SetOffloadEmptyReported(false);

		if (theCuEliteOnloader.GetOnloadPullyFullflag()
			&& theCuEliteOnloader.GetOnloadPullyFullReported() == false)
		{
			theCuEliteOnloader.SetOnloadPullyFullReported(true);
			anErrorList[i] = MHS_CUELITE_ONLOAD_MAG_PULLY_FULL;
			i++;
			nMhsWarning = FNOK;
		}
		else if (theCuEliteOnloader.GetOnloadPullyFullflag()
			&& theCuEliteOnloader.GetOnloadPullyFullReported())
		{
			anErrorList[i] = MHS_CUELITE_ONLOAD_MAG_PULLY_FULL;
			i++;
		}
		else
			theCuEliteOnloader.SetOnloadPullyFullReported(false);

		if (theCuEliteOffloader.GetOffloadPullyFullflag()
			&& theCuEliteOffloader.GetOffloadPullyFullReported() == false)
		{
			theCuEliteOffloader.SetOffloadPullyFullReported(true);
			anErrorList[i] = MHS_CUELITE_OFFLOAD_MAG_PULLY_FULL;
			i++;
			nMhsWarning = FNOK;
		}
		else if (theCuEliteOffloader.GetOffloadPullyFullflag()
				&& theCuEliteOffloader.GetOffloadPullyFullReported())
		{
			anErrorList[i] = MHS_CUELITE_OFFLOAD_MAG_PULLY_FULL;
			i++;
		}
		else
			theCuEliteOffloader.SetOffloadPullyFullReported(false);

		if (theCuEliteOnloader.GetOnloadBufferEmptyflag())
		{
			CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
			theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

			if (theCuEliteOnloader.GetCurrentSlotNum() <= stCuEliteDevicePara.lMagSlotNum)
			{
				if (theCuEliteOnloader.GetOnloadBufferEmptyReported()==false)
				{
					theCuEliteOnloader.SetOnloadBufferEmptyReported(true);
					anErrorList[i] = MHS_CUELITE_ONLOAD_MAG_BUF_EMPTY;
					i++;
					nMhsWarning = FNOK;
				}
				else
				{
					anErrorList[i] = MHS_CUELITE_ONLOAD_MAG_BUF_EMPTY;
					i++;
				}
			}
		}
		else
			theCuEliteOnloader.SetOnloadBufferEmptyReported(false);

		if (theCuEliteOffloader.GetOffloadBufferEmptyflag())
		{
			CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
			theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

			if (theCuEliteOffloader.GetCurrentSlotNum() <= stCuEliteDevicePara.lMagSlotNum)
			{
				if (theCuEliteOffloader.GetOffloadBufferEmptyReported() == false)
				{
					theCuEliteOffloader.SetOffloadBufferEmptyReported(true);
					anErrorList[i] = MHS_CUELITE_OFFLOAD_MAG_BUF_EMPTY;
					i++;
					nMhsWarning = FNOK;
				}
				else
				{
					if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
					{
						anErrorList[i] = MHS_CUELITE_OFFLOAD_MAG_BUF_EMPTY;
						i++;
					}
				}
			}
		}
		else
			theCuEliteOffloader.SetOffloadBufferEmptyReported(false);

		nErrorStatus = theCuEliteTrack.GetTrackErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
			}
			theCuEliteTrack.ResetTrackErrorCode();
		}

		nErrorStatus = theCuEliteOffloader.GetOffloadErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
			}

			theCuEliteOffloader.ResetOffloadErrorCode();
		}

		nErrorStatus = theCuEliteOnloader.GetOnloadErrorCode();
		if (nErrorStatus != FOK)
		{
			if (IsSameMhsError(anErrorList,nErrorStatus,i) == false)
			{
				anErrorList[i] = nErrorStatus;
				i++;
				nMhsError = FNOK;
				theCuEliteOnloader.ResetOnloadErrorCode();
			}
		}
		break;
	}
	case MHS_VLED_ELITE:
	{
        if (theVEliteOnload.IsEmpty() && (!theVEliteOnload.IsEmptyMsgReported()))
        {
			theVEliteOnload.SetEmptyMsgReported();
            anErrorList[i] = MHS_ONLOAD_EMPTY;
			i++;
			nMhsWarning = FNOK;
        }

        nErrorStatus = theVEliteOnload.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVEliteOnload.ResetErrorCode();
            theVEliteOnload.MovePickerBackward();
		}

		nErrorStatus = theVEliteTrack.GetInjectErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVEliteTrack.ResetInjectErrorCode();
            theVEliteOnload.MovePickerBackward();
		}

		nErrorStatus = theVEliteTrack.GetTrackErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVEliteTrack.ResetTrackErrorCode();
		}

		nErrorStatus = theVEliteOffload.GetErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVEliteOffload.ResetErrorCode();
		}
		break;
	}
	case MHS_VLED:
	{
        if ((theVledOnload.GetOnloadEmptyFlag())
			&& (theVledOnload.GetOnloadEmptyMsgReported() == false))
        {
			theVledOnload.SetOnloadEmptyMsgReported();
            anErrorList[i] = MHS_ONLOAD_EMPTY;
			i++;
			nMhsWarning = FNOK;
        }

		if ((theVledOffload.CheckOffloadFull())
			&& (theVledOffload.GetForkFullMsgReported() == false))
		{
			theVledOffload.SetForkFullMsgReported();
            anErrorList[i] = MHS_OFFLOAD_FORK_FULL;
			i++;
			nMhsWarning = FNOK;
        }

        nErrorStatus = theVledOnload.GetOnloadErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVledOnload.ResetOnloadErrorCode();
            theVledOnload.MovePickerBackward();
		}

		nErrorStatus = theVledTrack.GetInjectErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVledTrack.ResetInjectErrorCode();
            theVledOnload.MovePickerBackward();
		}

		nErrorStatus = theVledTrack.GetTrackErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVledTrack.ResetTrackErrorCode();
		}

		nErrorStatus = theVledOffload.GetOffloadErrorCode();
		if (nErrorStatus != FOK)
		{
			anErrorList[i] = nErrorStatus;
			i++;
			nMhsError = FNOK;
			theVledOffload.ResetOffloadErrorCode();
		}
		break;
	}
	}

    if ((nMhsError == FNOK) || (nMhsWarning == FNOK))
        theMHS.ReportError(anErrorList, bWorkerThread);

    return nMhsError;
}

void CMaterialHandlerThread::SetThreadStop(bool bStop)
{
    m_bThreadStop = bStop;
}

bool CMaterialHandlerThread::IsThreadStop()
{
    return (m_bThreadStop);
}

int CMaterialHandlerThread::LastLeadframeHandle(void)
{
    long lStartTime, lElapseTime;
    SINGLE_CAP_TRACK_RCD stLfTrackRcd;
    theVledTrack.GetLfTrackRcd(&stLfTrackRcd);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	case MHS_TOP_ELITE:	//v3.0.T410 add
    {
		theMHS.SetRequestIndex(false);
		Sleep(10);
		GetTopStopReason();
		break;
    }
	case MHS_R2R:
    {
		theMHS.SetRequestIndex(false);
		Sleep(10);
		GetR2RStopReason();
		break;
    }
	case MHS_CU_ELITE:
    {
		theMHS.SetRequestIndex(false);
		Sleep(10);
		GetCuEliteStopReason();
		break;
    }
	case MHS_VLED_ELITE:
    {
		lStartTime = GetCurrentTime();
		do 
		{
			if (theVEliteTrack.GetIndexEvent() == TRACK_EVENT_IDLE) break;
			lElapseTime = GetCurrentTime() - lStartTime;
			Sleep(10);
		} while (lElapseTime < 10000);

		Sleep(500);

        theVEliteLfClamp.OpenLfClamp();
        Sleep(500);
        theVEliteTrack.LastLeadframeHandle(stLfTrackRcd.nLeadFrameUnits);
        Sleep(2000);
        theVEliteLfClamp.CloseLfClamp();

	    theMHS.SetRequestIndex(false);
		theMHS.m_bBusy = false;
		SetAutoIndex(false);

		theVEliteTrack.ResetIndexEvent();
		theVEliteOffload.ResetEvent();
		theVEliteTrack.theLeftClaw.ResetAll();
		theVEliteTrack.theRightClaw.ResetAll();
		theVEliteTrack.SetBondUnitNum(0);

		lStartTime = GetCurrentTime();
		theVEliteOffload.SubmitEvent(VE_OFFLOAD_EVENT_OFFLOAD);
		do
		{
			theVEliteOffload.ProcessEvent();
			if (theVEliteOffload.GetEvent() == VE_OFFLOAD_EVENT_IDLE) break;
			Sleep(10);
			lElapseTime = GetCurrentTime() - lStartTime;
		} while (lElapseTime < 8000);

		theVEliteTrack.ResetTrackErrorCode();

		m_bLastLf = false;
        theVEliteOnload.MovePickerBackward();
        theVEliteTrack.theRightClaw.ResetAll();
		break;
    }
	case MHS_VLED:
    {
		//theMHS.SetRequestIndex(false);	
		lStartTime = GetCurrentTime();
		do 
		{
			if (theVledTrack.GetIndexEvent() == TRACK_EVENT_IDLE) break;
			lElapseTime = GetCurrentTime() - lStartTime;
			Sleep(10);
		} while (lElapseTime < 10000);

		Sleep(500);

        theVledLfClamp.OpenLfClamp();
        Sleep(500);
        theVledTrack.theRightClaw.IndexSteps(stLfTrackRcd.nLeadFrameUnits);
        Sleep(2000);
        theVledLfClamp.CloseLfClamp();

	    theMHS.SetRequestIndex(false);
		theMHS.SetBusy(false);
		SetAutoIndex(false);

		theVledTrack.ResetIndexEvent();
		theVledOffload.ResetForkEvent();
		theVledTrack.theLeftClaw.ResetAll();
		theVledTrack.theRightClaw.ResetAll();
		theVledTrack.SetBondUnitNum(0);

        lStartTime = GetCurrentTime();
        theVledOffload.SubmitOffloadEvent();
        do
		{
            theVledOffload.ProcessOffloadEvent();
            if (theVledOffload.GetOffloadEvent() == FORK_OFFLOAD_EVENT_IDLE) break;
            Sleep(10);
            lElapseTime = GetCurrentTime() - lStartTime;
        } while (lElapseTime < 8000);

		theVledTrack.ResetTrackErrorCode();

		m_bLastLf = false;
        theVledOnload.MovePickerBackward();
        //theVledTrack.theLeftClaw.ResetAll();
        theVledTrack.theRightClaw.ResetAll();
     
        /*theVledOffload.ResetForkEvent();
        theVledOnload.ResetOnloadEvent();
        theVledTrack.ResetIndexEvent();*/
		break;
    }
	}

    return 0;
}

void CMaterialHandlerThread::SetLastLf(bool bLastLF)
{
	m_bLastLf = bLastLF;
}

bool CMaterialHandlerThread::IsLastLf(void)
{
	return m_bLastLf;
}

bool CMaterialHandlerThread::IsOxidationProtect(void)
{
	return m_bOxidationProtect;
}

void CMaterialHandlerThread::SetOxidationProtect(bool bProtect)
{
	m_bOxidationProtect = bProtect;

	if (bProtect == false)
	{
		if (theMcConfig.GetMhsType() == MHS_VLED)
		{
			theVledLfClamp.CheckLfClampStatus(FALSE);
			if (theVledLfClamp.GetClampStatus() != SINGLE_CLAMP_CLOSE)
			{
				theVledLfClamp.CloseLfClamp();
			}
		}
	}
}

short CMaterialHandlerThread::GetCurBondUnit(void)
{
	short nBondUnit = 0;
    
    if (!theMcConfig.IsBypassAll())
    {
		switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
		{
		case MHS_TOP_LED:
            nBondUnit = theTopTrack.GetCurBondUnit();
			break;
		case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
            nBondUnit = theTEIndex.GetCurBondUnit();
			break;
		case MHS_FOXCONN:	//v3.0.T175 edit
			nBondUnit = theFoxconnTrack.GetCurBondUnit();
			break;
		case MHS_R2R:
            nBondUnit = theR2RTrack.GetCurrentBondUnit();
			break;
		case MHS_CU_ELITE:
            nBondUnit = theCuEliteTrack.GetCurrentBondUnit();
			break;
		case MHS_VLED_ELITE:
            nBondUnit = theVEliteTrack.GetBondUnitNum();
			break;
		case MHS_VLED:
            nBondUnit = theVledTrack.GetBondUnitNum();
			break;
		}
    }
    else
        nBondUnit = 1;

	return nBondUnit;
}

void CMaterialHandlerThread::ResetCurBondUnit()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
		theTopTrack.SetCurBondUnit(0);
		break;
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
		theTEIndex.SetCurBondUnit(0);
		break;
	case MHS_R2R:
		theR2RTrack.SetCurrentBondUnit(0);
		break;
	case MHS_CU_ELITE:
		theCuEliteTrack.SetCurrentBondUnit(0);
		break;
	case MHS_VLED_ELITE:
		theVEliteTrack.SetBondUnitNum(0);
		break;
	case MHS_VLED:
		theVledTrack.SetBondUnitNum(0);
		break;
	}
}

void CMaterialHandlerThread::UpdateDebugInfo(void)
{
    short nErr;

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	case MHS_CU_ELITE:
	case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		theDebug.AddInfo(_T("IndexUnit"), GetCurBondUnit());
		break;
	}
	case MHS_VLED_ELITE:
	{
		CString sTemp;
		sTemp.Format(_T("TrackState_%d_Event_%d"),
						theVEliteTrack.m_nIndexState,
						theVEliteTrack.GetIndexEvent());
		theDebug.AddInfo(sTemp);

		sTemp.Format(_T("OffloadState_%d_Event_%d_Err_%d"),
						theVEliteOffload.GetState(),
						theVEliteOffload.GetEvent(),
						theVEliteOffload.GetErrorCode());
		theDebug.AddInfo(sTemp);

		sTemp.Format(_T("LeftClawTicker_%d_BondUnit_%d"),
				theVEliteTrack.theLeftClaw.GetClawTicker(),
				theVEliteTrack.GetBondUnitNum());
		theDebug.AddInfo(sTemp);

		sTemp.Format(_T("RightClawTicker_%d_OffloadEnable_%d"),
				theVEliteTrack.theRightClaw.GetClawTicker(),
				theVEliteTrack.IsOffloadEnable());
		theDebug.AddInfo(sTemp);

		theDebug.AddInfo(_T("ClamperStatus"), theVEliteLfClamp.GetClampStatus());

		nErr = theVEliteOnload.GetErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("OnloadError"), nErr);

		nErr = theVEliteTrack.GetInjectErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("InjectError"), nErr);

		nErr = theVEliteTrack.GetTrackErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("TrackError"), nErr);

		nErr = theVEliteOffload.GetErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("OffloadError"), nErr);
		break;
	}
	case MHS_VLED:
	{
		CString sTemp;
		sTemp.Format(_T("TrackState_%d_Event_%d"),
						theVledTrack.m_nIndexState,
						theVledTrack.GetIndexEvent()); 
		theDebug.AddInfo(sTemp);

		sTemp.Format(_T("ForkOffloadState_%d_Event_%d_Err_%d"),
						theVledOffload.GetOffloadState(),
						theVledOffload.GetOffloadEvent(),
						theVledOffload.GetOffloadErrorCode());
		theDebug.AddInfo(sTemp);
	    
		sTemp.Format(_T("LeftClawTicker_%d_BondUnit_%d"),
					theVledTrack.theLeftClaw.GetClawTicker(),
					theVledTrack.GetBondUnitNum());
		theDebug.AddInfo(sTemp);

		sTemp.Format(_T("RightClawTicker_%d_OffloadEnable_%d"),
					theVledTrack.theRightClaw.GetClawTicker(),
					theVledTrack.GetOffloadEnableFlag());
		theDebug.AddInfo(sTemp);

		theDebug.AddInfo(_T("ClamperStatus"), theVledLfClamp.GetClampStatus());

		nErr = theVledOnload.GetOnloadErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("OnloadError"), nErr);

		nErr = theVledTrack.GetInjectErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("InjectError"), nErr);

		nErr = theVledTrack.GetTrackErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("TrackError"), nErr);

		nErr = theVledOffload.GetOffloadErrorCode();
		if (nErr != FOK)
			theDebug.AddInfo(_T("OffloadError"), nErr);
		break;
	}
	}
}

// Get material handler data version
short CMaterialHandlerThread::GetMhsDataVersion(void)
{
    return m_nWhVersion;
}

int CMaterialHandlerThread::IndexLeadFrame(void)
{
    short nEvent = 1;
	short nCurUnit, nCurRow, nCurCol, nUnitTotalWire;
    long lLastTime, lElapseTime;
    bool bIndexInProgress = true;

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
    {
		theTopOnloader.m_MagDetectedSensor.UpdateSensorState();
		if ((theTopOnloader.m_MagDetectedSensor.GetSensorState() == FALSE)
			&& (theTopTrack.GetCurBondUnit() == 0))
		{
			short nMsgNum, anMsgArray[10]; 
			nMsgNum = 1;
			anMsgArray[0] = MSG_ONLOAD_ELEV_NO_MAG;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
			return true;
		}
		else
		{
			if (theTopOnloader.GetErrorCode() == MHS_TOP_ONLOAD_NO_MAG)
				theTopOnloader.ResetErrorCode();
		}

		const TOP_DEVICE_RECORD *pstTopDev = theDevice.GetTopRcd();
        if (theTopTrack.IsTrackNeedClear())
            if (theMHS.ClearTrack() != FOK) return FNOK;

        if (theTopOffloader.IsJamNotHandled())
        {
			CDlgElevCtrl objElev;
			if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)) return FNOK;

			if (theTopOffloader.GetCurSlot() < pstTopDev->lMagSlotNum)
				theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot() + 1 + theTopTrack.GetRcd()->iOutputMagazineSlotSequence);
			else
				theTopOffloader.EjectOffloadMag();
        }

        if (theMHS.GetCurBondUnit() == 0)
        {
            theTopTrack.ResetErrorCode(1);
            theTopTrack.ResetErrorCode(0);

            int iPusherStatus;
            iPusherStatus = theTopOnloader.m_InjectPusher.TriggerForwardMotion();
            iPusherStatus += theTopOnloader.m_InjectPusher.TriggerBacktoHome();
            if (iPusherStatus != FOK)
            {
                short nMsgNum, anMsgArray[10]; 
                nMsgNum = 1;
                anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
                theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
                return FNOK;
            }

            //theTopTrack.m_InjectSensor.UpdateSensorState();
            //if (theTopTrack.m_InjectSensor.GetSensorState() == FALSE)
            {
				theTopOnloader.ResetAfterInit();
                theTopTrack.RequestOnload();
            }
        }

        //******************************ACTION********************************************

        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep(10);

        theServo.MoveBondHeadToSafePos();

        if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
            theTopTrack.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);
        Sleep(100);

        lLastTime = GetCurrentTime();
        while (nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theTopTrack.GetEvent();
            if (theMHS.ShowError() != FOK) break;

            if (theTopTrack.GetIndexPrManualAlign() == INDEX_PR_ENTER_MANU_ALIGN)
            {
                theTopTrack.PerformPrIndex();
                lLastTime = GetCurrentTime();
            }

            lElapseTime = GetCurrentTime() - lLastTime;
            if (lElapseTime > TIMEOUT_3) break;

            Sleep(1);
        }

        theVisionSys.EnableVideo(true);
        theMHS.SetAutoIndex(false);

        if (pstTopDev->nHalfIndexOn > 0)
        {
            if (theTopTrack.GetCurBondUnit() == pstTopDev->nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire  = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nBondedWire  = nUnitTotalWire * (nCurUnit - 1);
                }
            }
            else
            {			
                nCurUnit = 1;
                thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire  = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

            nCurUnit = 1;
            thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);
        }

        theBond.m_nCurMatRow = 1;
        theBond.m_nCurMatCol = 1;
        theBond.m_nCurRow = nCurRow;
        theBond.m_nCurCol = nCurCol;
        theBond.ResetMatrixPrStatus(1);
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
		theTopOnloader.m_MagDetectedSensor.UpdateSensorState();
		if ((theTopOnloader.m_MagDetectedSensor.GetSensorState() == FALSE)
			&& (theTEIndex.GetCurBondUnit() == 0))
		{
			short nMsgNum, anMsgArray[10]; 
			nMsgNum = 1;
			anMsgArray[0] = MSG_ONLOAD_ELEV_NO_MAG;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
			return true;
		}
		else
		{
			if (theTopOnloader.GetErrorCode() == MHS_TOP_ONLOAD_NO_MAG)
				theTopOnloader.ResetErrorCode();
		}

		const TOP_DEVICE_RECORD *pstTopDev = theDevice.GetTopRcd();
        if (theTEIndex.IsTrackNeedClear())
            if (theMHS.ClearTrack() != FOK) return FNOK;

        if (theTopOffloader.IsJamNotHandled())
        {
			CDlgElevCtrl objElev;
			if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)) return FNOK;

			if (theTopOffloader.GetCurSlot() < pstTopDev->lMagSlotNum)
				theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot() + 1 + theTopTrack.GetRcd()->iOutputMagazineSlotSequence);
			else
				theTopOffloader.EjectOffloadMag();
        }

        if (theMHS.GetCurBondUnit() == 0)
        {
            theTEIndex.ResetErrorCode(1);
            theTEIndex.ResetErrorCode(0);

            int iPusherStatus;
            iPusherStatus = theTopOnloader.m_InjectPusher.TriggerForwardMotion();
            iPusherStatus += theTopOnloader.m_InjectPusher.TriggerBacktoHome();
            if (iPusherStatus != FOK)
            {
                short nMsgNum, anMsgArray[10]; 
                nMsgNum = 1;
                anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
                theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
                return FNOK;
            }

			theTopOnloader.ResetAfterInit();
			theTEIndex.RequestOnload();
        }

        //******************************ACTION********************************************

        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep(10);

        theServo.MoveBondHeadToSafePos();

        if (theTEIndex.GetEvent() == INDEX_EVENT_IDLE)
            theTEIndex.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);
        Sleep(100);

        lLastTime = GetCurrentTime();
        while (nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theTEIndex.GetEvent();
            if (theMHS.ShowError() != FOK) break;

            if (theTEIndex.GetIndexPrManualAlign() == INDEX_PR_ENTER_MANU_ALIGN)
            {
                theTEIndex.PerformPrIndex();
                lLastTime = GetCurrentTime();
            }

            lElapseTime = GetCurrentTime() - lLastTime;
            if (lElapseTime > TIMEOUT_3) break;

            Sleep(1);
        }

        theVisionSys.EnableVideo(true);
        theMHS.SetAutoIndex(false);

        if (pstTopDev->nHalfIndexOn > 0)
        {
            if (theTEIndex.GetCurBondUnit() == pstTopDev->nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire  = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nBondedWire  = nUnitTotalWire * (nCurUnit - 1);
                }
            }
            else
            {			
                nCurUnit = 1;
                thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire  = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

            nCurUnit = 1;
            thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);
        }

        theBond.m_nCurMatRow = 1;
        theBond.m_nCurMatCol = 1;
        theBond.m_nCurRow = nCurRow;
        theBond.m_nCurCol = nCurCol;
        theBond.ResetMatrixPrStatus(1);
		break;
    }
	case MHS_R2R:
    {
        TOP_DEVICE_RECORD stTopDevicePara;
        theDevice.GetTopRcd(&stTopDevicePara);
        if (theR2RTrack.GetTrackNeedClearFlag() == TRUE)
            if (theMHS.ClearTrack() != FOK) return FNOK;

        if (theR2ROffloader.GetOffloadJamHandleFlag() == TRUE)
        {
            theR2RTrack.theEjectSnr.UpdateSensorState();
            if (theR2RTrack.theEjectSnr.GetSensorState() == FALSE)
            {
                if (theR2ROffloader.GetCurrentSlotNum()< stTopDevicePara.lMagSlotNum)
                    theR2ROffloader.MoveTotheSlot(theTopOffloader.GetCurSlot()+1);
                else
                    theR2ROffloader.EjectOffloadMag();
            }
            else
            {
                short nMsgNum, anMsgArray[10]; 
                nMsgNum = 1;
                anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;
                theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
                return FNOK;
            }
        }

        if (theMHS.GetCurBondUnit() == 0)
        {
            theR2RTrack.ResetTrackErrorCode(1);
            theR2RTrack.ResetTrackErrorCode(0);

            int iPusherStatus;
            iPusherStatus = theR2ROnloader.theInjectPusher.TriggerForwardMotion();
            iPusherStatus += theR2ROnloader.theInjectPusher.TriggerBacktoHome();
            if (iPusherStatus != FOK)
            {
                short nMsgNum, anMsgArray[10]; 
                nMsgNum = 1;
                anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
                theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
                return FNOK;
            }

            //theTopTrack.m_InjectSensor.UpdateSensorState();
            //if(theTopTrack.m_InjectSensor.GetSensorState() == FALSE)
            {
                theR2RTrack.RequestOnloadEvent();
            }
        }

        //******************************ACTION********************************************

        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep(10);

        theServo.MoveBondHeadToSafePos();

        if (theR2RTrack.GetTopTrackEvent() == INDEX_EVENT_IDLE)
            theR2RTrack.SubmitIndexEvent(INDEX_EVENT_SINGLE_INDEX);

        Sleep(100);

        lLastTime = GetCurrentTime();
        while (nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theR2RTrack.GetTopTrackEvent();
            if (theMHS.ShowError() != FOK) break;

            if (theR2RTrack.GetIndexPrManuAlignFlag() == INDEX_PR_ENTER_MANU_ALIGN)
            {
                theR2RTrack.PerformIndexPrprogress();
                lLastTime = GetCurrentTime();
            }

            lElapseTime = GetCurrentTime() - lLastTime;
            if (lElapseTime >  TIMEOUT_3) break;

            Sleep(1);
        }

        theVisionSys.EnableVideo(true);

        theMHS.SetAutoIndex(false);

        if (stTopDevicePara.nHalfIndexOn > 0)
        {
            if (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire  = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nBondedWire  = nUnitTotalWire * (nCurUnit - 1);
                }
            }
            else
            {			
                nCurUnit = 1;
                thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire  = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

            nCurUnit = 1;
            thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);
        }

        theBond.m_nCurMatRow = 1;
        theBond.m_nCurMatCol = 1;
        theBond.m_nCurRow = nCurRow;
        theBond.m_nCurCol = nCurCol;
        theBond.ResetMatrixPrStatus(1);
		break;
    }
	case MHS_CU_ELITE:
    {
		theCuEliteOnloader.theMagDetectedSensor.UpdateSensorState();
		if (theCuEliteOnloader.theMagDetectedSensor.GetSensorState()==FALSE && theCuEliteTrack.GetCurrentBondUnit() == 0)
		{
			short nMsgNum, anMsgArray[10]; 
			nMsgNum = 1;
			anMsgArray[0] = MSG_ONLOAD_ELEV_NO_MAG;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
			return true;
		}
		else
		{
			if (theCuEliteOnloader.GetOnloadErrorCode() == MHS_CUELITE_ONLOAD_NO_MAG)
				theCuEliteOnloader.ResetOnloadErrorCode();
		}

        CUELITE_DEVICE_RECORD stCuEliteDevicePara;
        theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
        if (theCuEliteTrack.GetTrackNeedClearFlag() == TRUE)
            if (theMHS.ClearTrack() != FOK) return FNOK;

        if (theCuEliteOffloader.GetOffloadJamHandleFlag() == TRUE)
        {
            theCuEliteTrack.theEjectSnr.UpdateSensorState();
            if (theCuEliteTrack.theEjectSnr.GetSensorState() == FALSE)
            {
				CUELITE_TRACK_DATA stTrackData;
				theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);
                if (theCuEliteOffloader.GetCurrentSlotNum() < stCuEliteDevicePara.lMagSlotNum)
                    //theCuEliteOffloader.MoveTotheSlot(theCuEliteOffloader.GetCurrentSlotNum()+1);
                    theCuEliteOffloader.MoveTotheSlot(theCuEliteOffloader.GetCurrentSlotNum() + 1 + stTrackData.iOutputMagazineSlotSequence);
                else
                    theCuEliteOffloader.EjectOffloadMag();
            }
            else
            {
                short nMsgNum, anMsgArray[10];
                nMsgNum = 1;
                anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;
                theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
                return FNOK;
            }
        }

        if (theMHS.GetCurBondUnit() == 0)
        {
            theCuEliteTrack.ResetTrackErrorCode(1);
            theCuEliteTrack.ResetTrackErrorCode(0);

            int iPusherStatus;
			iPusherStatus = theCuEliteOnloader.theInjectPusher.TriggerForwardMotion();
			iPusherStatus += theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
            if (iPusherStatus != FOK)
            {
                short nMsgNum, anMsgArray[10]; 
                nMsgNum = 1;
                anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
                theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
                return FNOK;
            }

            //theCuEliteTrack.theInjectSnr.UpdateSensorState();
            //if(theCuEliteTrack.theInjectSnr.GetSensorState() == FALSE)
            {
				theCuEliteOnloader.ResetOnloadInitStatus();
                theCuEliteTrack.RequestOnloadEvent();
            }
        }

        //******************************ACTION********************************************

        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep(10);

        theServo.MoveBondHeadToSafePos();

        if (theCuEliteTrack.GetCuEliteTrackEvent() == INDEX_EVENT_IDLE)
            theCuEliteTrack.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);

        Sleep(100);

        lLastTime = GetCurrentTime();
        while (nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theCuEliteTrack.GetCuEliteTrackEvent();
            //because this member function is called from worker thread, 
			//can not use modeless dialog, just a short term answer
            if (theMHS.ShowError(TRUE) != FOK) break;

            if (theCuEliteTrack.GetIndexPrManuAlignFlag() == INDEX_PR_ENTER_MANU_ALIGN)
            {
                theCuEliteTrack.PerformIndexPrprogress();
                lLastTime = GetCurrentTime();
            }

            lElapseTime = GetCurrentTime() - lLastTime;
            if (lElapseTime > TIMEOUT_3) break;

            Sleep(1);
        }

        theVisionSys.EnableVideo(true);

        theMHS.SetAutoIndex(false);

        if (stCuEliteDevicePara.nHalfIndexOn > 0)
        {
            if (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire  = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

                    nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (nCurUnit - 1) + 1;
                    theBond.m_nBondedWire  = nUnitTotalWire * (nCurUnit - 1);
                }
            }
            else
            {			
                nCurUnit = 1;
                thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire  = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

            nCurUnit = 1;
            thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);
        }

        theBond.m_nCurMatRow = 1;
        theBond.m_nCurMatCol = 1;
        theBond.m_nCurRow = nCurRow;
        theBond.m_nCurCol = nCurCol;
        theBond.ResetMatrixPrStatus(1);
		break;
    }
	case MHS_VLED_ELITE:
	{
		nCurUnit = theVEliteTrack.GetBondUnitNum();

		theMHS.SetLastLf(false);
		theMHS.SetAutoIndex(true);
		theMHS.SetDryRun(false);

		if (nCurUnit == 0)
			theVEliteTrack.theLeftClaw.SetIndexEmpty(true);

		theMHS.SetRequestIndex(true);
		Sleep(200);

		lLastTime = GetCurrentTime();
		while (bIndexInProgress)
		{
			bIndexInProgress = theMHS.IsRequestIndex();
			if (theMHS.ShowError() != FOK) break;

			lElapseTime = ::GetCurrentTime() - lLastTime;
			if (lElapseTime > 15000) break;

			Sleep_x1ms(1);
		}

		theMHS.SetAutoIndex(false);

		nCurUnit = 1;

		theBond.m_nCurMatRow = 1;
		theBond.m_nCurMatCol = 1;
		break;
	}
	case MHS_VLED:
    {
        nCurUnit = theVledTrack.GetBondUnitNum();

        theMHS.SetLastLf(false);
        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);

        if (nCurUnit == 0)
            theVledTrack.theLeftClaw.SetIndexEmpty(true);

        theMHS.SetRequestIndex(true);
        Sleep(200);

        lLastTime = GetCurrentTime();
        while (bIndexInProgress)
        {
            bIndexInProgress = theMHS.IsRequestIndex();
            if (theMHS.ShowError() != FOK) break;

            lElapseTime=::GetCurrentTime()-lLastTime;
            if (lElapseTime > 15000) break;

            Sleep_x1ms(1);
        }

        theMHS.SetAutoIndex(false);

        nCurUnit = 1;

        theBond.m_nCurMatRow = 1;
        theBond.m_nCurMatCol = 1;
		break;
    }
	}

	if (thePgm.IsProgramValid())
    {
        short nBkupUnit = nCurUnit;

        if (nCurUnit > thePgm.GetTotalUnit())
        {
            nCurUnit = nBkupUnit;
            return FNOK;
        }
        else
            thePgm.GetRowCol(nCurUnit, &nCurRow, &nCurCol);

        if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
        {
            short nUnitTotalWire = thePgm.GetTotalWire() / thePgm.GetTotalUnit();
            theBond.m_nCurWire = (nCurUnit - 1) * nUnitTotalWire + 1;
            theBond.m_nRestartWire = 1;  //theBond.m_nCurWire;
            theBond.m_nBondedWire  = theBond.m_nCurWire - 1;

			switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
			{
			case MHS_TOP_LED:
            {
                TOP_DEVICE_RECORD stTopDevicePara;
                theDevice.GetTopRcd(&stTopDevicePara);
                if ((stTopDevicePara.nHalfIndexOn > 0)
					&& (theTopTrack.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
                {
                    theBond.m_nRestartWire = theBond.m_nCurWire;  
                }
				break;
            }
			case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
            {
                TOP_DEVICE_RECORD stTopDevicePara;
                theDevice.GetTopRcd(&stTopDevicePara);
                if ((stTopDevicePara.nHalfIndexOn > 0)
					&& (theTEIndex.GetCurBondUnit() == stTopDevicePara.nHalfIndexOn))
                {
                    theBond.m_nRestartWire = theBond.m_nCurWire;  
                }
				break;
            }
			case MHS_R2R:
            {
                TOP_DEVICE_RECORD stTopDevicePara;
                theDevice.GetTopRcd(&stTopDevicePara);
                if ((stTopDevicePara.nHalfIndexOn > 0)
					&& (theR2RTrack.GetCurrentBondUnit() == stTopDevicePara.nHalfIndexOn))
                {
                    theBond.m_nRestartWire = theBond.m_nCurWire;  
                }
				break;
            }
			case MHS_CU_ELITE:
            {
                CUELITE_DEVICE_RECORD stCuEliteDevicePara;
                theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

                if ((stCuEliteDevicePara.nHalfIndexOn > 0)
					&& (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn))
                {
                    theBond.m_nRestartWire = theBond.m_nCurWire;  
                }
				break;
            }
			}
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1; //theBond.m_nCurWire;
            theBond.m_nBondedWire  = theBond.m_nCurWire - 1;
        }

        theBond.m_nCurRow = nCurRow;
        theBond.m_nCurCol = nCurCol;

        theBond.ResetMatrixPrStatus(1);
    }

    theBond.SetPrOn1stEnterDone(true);
    theBond.SetAutoDefineBond(false);
    return 0;
}

void CMaterialHandlerThread::ToggleLfClamper(void)
{
    int iReturnStatus = FOK, iBusyState = FOK; 
    long lOffset;

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_VLED:
	case MHS_VLED_ELITE:
    {
        theVledLfClamp.CheckLfClampStatus(FALSE);

        if (theVledLfClamp.GetClampStatus() == SINGLE_CLAMP_CLOSE)
            theVledLfClamp.OpenLfClamp();
        else
            theVledLfClamp.CloseLfClamp();

        do
		{
            Sleep(1);
        } while (theStepper.IsBusy(SINGLE_CAP_CLAMPER));
		break;
    }
	case MHS_CU_ELITE:
    {
        theCuEliteLFClamp.CheckClampStatus();
        if (theCuEliteLFClamp.GetClampStatus() != CUELITE_WND_CLAMP_SAFE_CLAMP)
        {
            theServo.MoveBondHeadToSafePos();
            Sleep(10);
            theCuEliteLFClamp.MoveClampSafePos();
        }
        else
        {
            INDEX_PR_REF stPrIndexRefRcd;
            theDevice.GetIndexPrRef(&stPrIndexRefRcd);

            CUELITE_TRACK_DATA  stTrackData;              
            theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);

            theCuEliteLFClamp.PreCloseClamp();

            Sleep(200);

			if (((stTrackData.nPrIndexEnable == 2)		//enable 1st unit only
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theTopTrack.IsPrIndexMask() == false)
					&& (theTopTrack.GetCurBondUnit() == 1))
				|| ((stTrackData.nPrIndexEnable == 1)	//enable for all
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theTopTrack.IsPrIndexMask() == false)))
            {
                iReturnStatus = theCuEliteTrack.CaculatePrIndexOffset(&lOffset);
                if (iReturnStatus == FOK)
                {
					if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nSrchUnit)
						theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
                    theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
                    Sleep(400);

                    theStepper.MoveDist(CUELITE_DEVICE_INDEXER, lOffset, MOTION_RELATIVE);
                    do
                    {
                        iBusyState = (int)theStepper.IsBusy(CUELITE_DEVICE_INDEXER);
                        Sleep(2);
                    } while (iBusyState != 0);

                    theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
					theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);
                }
                else
                {
                    theCuEliteTrack.PerformIndexPrprogress();

                    if(theCuEliteTrack.GetIndexPrManuAlignFlag() == CUELITE_INDEX_PR_MANU_ALIGN_OK)
                    {
						if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nSrchUnit)
							theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
                        theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
                        Sleep(400);

                        theStepper.MoveDist(CUELITE_DEVICE_INDEXER, theCuEliteTrack.m_loffsetXInUm, MOTION_RELATIVE);
                        do
                        {
                            iBusyState = (int)theStepper.IsBusy(CUELITE_DEVICE_INDEXER);
                            Sleep(2);
                        } while (iBusyState != 0);

                        theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
						theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);
                    }
                    else
                    {
                        theCuEliteTrack.SetIndexPrManuAlignFlag(CUELITE_INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
                    }
                }
            }
        }
		break;
    }
	case MHS_R2R:
    {
        theTopClamp.CheckClampStatus();
        if (theTopClamp.GetClampStatus() != WND_CLAMP_SAFE_CLAMP)
        {
            theServo.MoveBondHeadToSafePos();
            Sleep(10);
            theTopClamp.MoveClampSafePos();
        }
        else
        {
            INDEX_PR_REF stPrIndexRefRcd;
            theDevice.GetIndexPrRef(&stPrIndexRefRcd);

            TOP_TRACK_DATA  stTrackData;              
            theR2RTrack.GetTopTrackRcd(&stTrackData);

            theTopClamp.PreCloseClamp();

            Sleep(200);

			if (((stTrackData.nPrIndexEnable == 2)		//enable 1st unit only
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theR2RTrack.GetPrIndexMask() == false)
					&& (theR2RTrack.GetCurrentBondUnit() == 1))
				|| ((stTrackData.nPrIndexEnable == 1)	//enable for all
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theR2RTrack.GetPrIndexMask() == false)))
            {
                iReturnStatus = theR2RTrack.CaculatePrIndexOffset(&lOffset);
                if (iReturnStatus == FOK)
                {
                    theR2RTrack.TargetClaw(RELAY_ON);
                    Sleep(400);

                    theStepper.MoveDist(theR2RTrack.m_ucIndexerStepperId, lOffset, MOTION_RELATIVE);
                    do
                    {
                        iBusyState = (int)theStepper.IsBusy(theR2RTrack.m_ucIndexerStepperId);
                        Sleep(2);
                    } while (iBusyState != 0);

                    theR2RTrack.TargetClaw(RELAY_OFF);
                }
                else
                {
                    theR2RTrack.PerformIndexPrprogress();

                    if (theR2RTrack.GetIndexPrManuAlignFlag() == INDEX_PR_MANU_ALIGN_OK)
                    {
                        theR2RTrack.TargetClaw(RELAY_ON);
                        Sleep(400);

                        theStepper.MoveDist(theR2RTrack.m_ucIndexerStepperId, theR2RTrack.m_loffsetXInUm, MOTION_RELATIVE);
                        do
                        {
                            iBusyState = (int)theStepper.IsBusy(theR2RTrack.m_ucIndexerStepperId);
                            Sleep(2);
                        } while (iBusyState != 0);

                        theR2RTrack.TargetClaw(RELAY_OFF);
                    }
                    else
                    {
                        theR2RTrack.SetIndexPrManuAlignFlag(INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
                    }
                }
            }
        }
		break;
    }
	case MHS_TOP_LED:
    {
        theTopClamp.CheckClampStatus();
        if (theTopClamp.GetClampStatus() != WND_CLAMP_SAFE_CLAMP)
        {
            theServo.MoveBondHeadToSafePos();
            Sleep(10);
            theTopClamp.MoveClampSafePos();
        }
        else
        {
            INDEX_PR_REF stPrIndexRefRcd;
            theDevice.GetIndexPrRef(&stPrIndexRefRcd);

            TOP_TRACK_DATA *pstTopTrack = theTopTrack.GetRcd();

            theTopClamp.PreCloseClamp();

            Sleep(200);

			if (((pstTopTrack->nPrIndexEnable == 2)		//enable 1st unit only
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theTopTrack.IsPrIndexMask() == false)
					&& (theTopTrack.GetCurBondUnit() == 1))
				|| ((pstTopTrack->nPrIndexEnable == 1)	//enable for all
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theTopTrack.IsPrIndexMask() == false)))
            {
                iReturnStatus = theTopTrack.CalcPrIndexOffset(&lOffset);
                if (iReturnStatus == FOK)
                {
					if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
						theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
                    theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
                    Sleep(400);

                    theStepper.MoveDist(theTopTrack.GetIndexStepID(), lOffset, MOTION_RELATIVE);
                    do
                    {
                        iBusyState = (int)theStepper.IsBusy(theTopTrack.GetIndexStepID());
                        Sleep(2);
                    } while (iBusyState != 0);

                    theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
					theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);
                }
                else
                {
                    theTopTrack.PerformPrIndex();

                    if (theTopTrack.GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_OK)
                    {
						if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
							theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
                        theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
                        Sleep(400);

                        theStepper.MoveDist(theTopTrack.GetIndexStepID(), theTopTrack.m_lOffsetx1um, MOTION_RELATIVE);
                        do
                        {
                            iBusyState = (int)theStepper.IsBusy(theTopTrack.GetIndexStepID());
                            Sleep(2);
                        } while (iBusyState != 0);

                        theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
						theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);
                    }
                    else
                    {
                        theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
                    }
                }
            }
        }
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
        theTopClamp.CheckClampStatus();
        if (theTopClamp.GetClampStatus() != WND_CLAMP_SAFE_CLAMP)
        {
            theServo.MoveBondHeadToSafePos();
            Sleep(10);
            theTopClamp.MoveClampSafePos();
        }
        else
        {
            theTopClamp.PreCloseClamp();
            Sleep(200);

			INDEX_PR_REF stPrIndexRefRcd;
            theDevice.GetIndexPrRef(&stPrIndexRefRcd);
            ST_TE_INDEX_RCD *pstTopTrack = theTEIndex.GetRcd();
			if (((pstTopTrack->nPrIndexEnable == 2)		//enable 1st unit only
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theTEIndex.IsPrIndexMask() == false)
					&& (theTEIndex.GetCurBondUnit() == 1))
				|| ((pstTopTrack->nPrIndexEnable == 1)	//enable for all
					&& stPrIndexRefRcd.bIndexPrAvail
					&& (theTEIndex.IsPrIndexMask() == false)))
            {
                iReturnStatus = theTEIndex.CalcPrIndexOffset(&lOffset);
                if (iReturnStatus == FOK)
                {
					if (theTEIndex.GetCurBondUnit() < theTEIndex.m_stIndexVar.nSrchUnit)
						theTEIndex.m_LFHolder.SetRelayState(RELAY_ON);
                    theTEIndex.m_pTargetClaw->SetRelayState(RELAY_ON);
                    Sleep(400);

                    theStepper.MoveDist(theTEIndex.GetIndexStepID(), lOffset, MOTION_RELATIVE);
                    do
                    {
                        iBusyState = (int)theStepper.IsBusy(theTEIndex.GetIndexStepID());
                        Sleep(2);
                    } while (iBusyState != 0);

                    theTEIndex.m_pTargetClaw->SetRelayState(RELAY_OFF);
					theTEIndex.m_LFHolder.SetRelayState(RELAY_OFF);
                }
                else
                {
                    theTEIndex.PerformPrIndex();

                    if (theTEIndex.GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_OK)
                    {
						if (theTEIndex.GetCurBondUnit() < theTEIndex.m_stIndexVar.nSrchUnit)
							theTEIndex.m_LFHolder.SetRelayState(RELAY_ON);
                        theTEIndex.m_pTargetClaw->SetRelayState(RELAY_ON);
                        Sleep(400);

                        theStepper.MoveDist(theTEIndex.GetIndexStepID(), theTEIndex.m_lOffsetx1um, MOTION_RELATIVE);
                        do
                        {
                            iBusyState = (int)theStepper.IsBusy(theTEIndex.GetIndexStepID());
                            Sleep(2);
                        } while (iBusyState != 0);

                        theTEIndex.m_pTargetClaw->SetRelayState(RELAY_OFF);
						theTEIndex.m_LFHolder.SetRelayState(RELAY_OFF);
                    }
                    else
                    {
                        theTEIndex.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
                    }
                }
            }
        }
		break;
    }
	}
}

int CMaterialHandlerThread::SaveWorkHolderData(CString szWorkHolderFileName)  
{
	if (theDataMgr.AllocBufferMemory(DATA_BUF_SIZE) != 0)
		return DATA_MGR_MALLOC_MEM_FAIL; 

	theMHS.SaveMhsRcd();
	theMHS.SaveMhsPowerOnData();
	theDataMgr.SaveDataToFile(szWorkHolderFileName);
	return 0;
}

int CMaterialHandlerThread::LoadWorkHolderData(CString szWorkHolderFileName)  
{
    int nFileLength;
	int ret = theDataMgr.LoadDataFromFile(szWorkHolderFileName, &nFileLength);
    if (ret != 0) return ret;
    
    theMHS.LoadMhsRcd();
	theMHS.LoadMhsPowerOnData();

    theDataMgr.FreeBufferMemory();

	return 0;
}

int CMaterialHandlerThread::CloseWindowClamp(void)
{
	if (theMcConfig.IsBypassMHS()) return FOK;	//v3.1.T172 add

	unsigned int nTimeout = 4000;
	if (theMcConfig.IsBypassAll()) nTimeout = 0;

	long lStartTime = GetCurrentTime();
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
    {
        theTopClamp.CloseClamp();
        do
        {
            Sleep(100);
            theTopClamp.CheckClampStatus();
        } while ((theTopClamp.GetClampStatus() != WND_CLAMP_CLOSE)
					&& (GetCurrentTime() - lStartTime < nTimeout));

		if (theTopClamp.GetClampStatus() != WND_CLAMP_CLOSE)
			return FNOK;
		break;
    }
	case MHS_CU_ELITE:
    {
        theCuEliteLFClamp.CloseClamp();
        do
        {
            Sleep(100);
            theCuEliteLFClamp.CheckClampStatus();
        } while ((theCuEliteLFClamp.GetClampStatus() != CUELITE_WND_CLAMP_CLOSE)
				&& ((GetCurrentTime() - lStartTime) < nTimeout));

		if (theCuEliteLFClamp.GetClampStatus() != CUELITE_WND_CLAMP_CLOSE)
			return FNOK;
		break;
	}
	}

	return FOK;
}

int CMaterialHandlerThread::PreCloseWindowClamp(void)
{
	if (theMcConfig.IsBypassMHS()) return FOK;	//v3.1.T172 add

	unsigned long nTimeout = 4000;
	if (theMcConfig.IsBypassAll()) nTimeout = 0;

	unsigned long lStartTime = GetCurrentTime();

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
    {
		theTopClamp.CheckClampStatus();
		if (theTopClamp.GetClampStatus() == WND_CLAMP_OPEN)
		{
			theTopClamp.PreCloseClamp();
			do
			{
				Sleep(100);
				theTopClamp.CheckClampStatus();
			} while ((theTopClamp.GetClampStatus() != WND_CLAMP_PRE_CLOSE)
					&& (GetCurrentTime() - lStartTime < nTimeout));
		}
		if (theTopClamp.GetClampStatus() != WND_CLAMP_PRE_CLOSE)
			return FNOK;
		break;
    }
	case MHS_CU_ELITE:
    {
		theCuEliteLFClamp.CheckClampStatus();
		if (theCuEliteLFClamp.GetClampStatus() == WND_CLAMP_OPEN)
		{
			theCuEliteLFClamp.PreCloseClamp();
			do
			{
				Sleep(100);
				theCuEliteLFClamp.CheckClampStatus();
			} while ((theCuEliteLFClamp.GetClampStatus() != WND_CLAMP_PRE_CLOSE)
					&& (GetCurrentTime() - lStartTime < nTimeout));

			if (theCuEliteLFClamp.GetClampStatus() != WND_CLAMP_PRE_CLOSE)
				return FNOK;
		}
		break;
	}
	}

	return FOK;
}

bool CMaterialHandlerThread::IsSameMhsError(short anErrorList[], short nError, int nTotalError)
{
	for (int i = 0; i < nTotalError; i++)
	{
		if (anErrorList[i] == nError)
		{
			return true;
		}
	}
	return false;
}

void CMaterialHandlerThread::ClearLastLf(void)
{
    if (theMcConfig.GetMhsType() != MHS_VLED_ELITE) return;

	theMHS.SetRequestIndex(false);
	theMHS.SetBusy(false);

	theVEliteOffload.SubmitEvent(VE_OFFLOAD_EVENT_OFFLOAD);

	DWORD dwStartTime = GetCurrentTime();
	do
	{
		theVEliteOffload.ProcessEvent();

		if ((theVEliteOffload.GetEvent() == VE_OFFLOAD_EVENT_IDLE) ||
			theVEliteOffload.IsError())
			break;

		Sleep(10);
	} while (GetCurrentTime() - dwStartTime < 8000);
}

bool CMaterialHandlerThread::IsBusy(void)
{
	return (m_bBusy);
}

void CMaterialHandlerThread::SetBusy(bool bBusy)
{
	m_bBusy = bBusy;
}

bool CMaterialHandlerThread::IsRequestIndex(void)
{
	return (m_bRequestIndex);
}

void CMaterialHandlerThread::SetRequestIndex(bool bIndex)
{
	m_bRequestIndex = bIndex;
}

//v3.0.T58 add
void CMaterialHandlerThread::VerifyData()
{
    int nFileLen; CString sPathName;
	sPathName.Format(_T("%s%s"), _T(DIR_DATALOG), _T("WhData.dat"));
    if (theDataMgr.LoadDataFromFile(sPathName, &nFileLen) != 0)
	{
		::AfxMessageBox(_T("WhData.dat not found"));
		return;
	}

	short nWhVersion;
	theDataMgr.CopyFromBuffer((char *)&nWhVersion, sizeof(nWhVersion));

	theTopClamp.VerifyData();
	theTopTrack.VerifyData();
	theTopOnloader.VerifyData();
	theTopOffloader.VerifyData();

	//theMHS.LoadMhsPowerOnData();

    theDataMgr.FreeBufferMemory();
}
