/***************************************************************
ScanKeyThread.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "ScanKeyThread.h"
#include "DigitalIO.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "VisionSystem.h"
#include "Bond.h"
#include "StepperControl.h"
#include "MaterialHandlerThread.h"
#include "MachineConfig.h"
#include "EfoControl.h"
#include "TopClamp.h"
#include "CuClamp.h"
#include "servo.h"

CScanKeyThread theScanKeyThread;

// CScanKeyThread

CScanKeyThread::CScanKeyThread()
: m_pScanThread(NULL)
, m_bScanEnable(false)
, m_bJoyStickValid(true)
, m_nJoyStickConfig(JOYSTICK_SERVO)
, m_bStepperId(0)
{
}

CScanKeyThread::~CScanKeyThread()
{
}

//v2.0.T37 rename
UINT PerformScanKeyThread(LPVOID pParam)
{
    short nState;
    long lStartTime, lElapseTime;

    theServo.SetOutputIo(0, 0, 0);
    theServo.SetOutputIo(0, 2, 1);
    while (theScanKeyThread.m_bScanEnable)
    {
		static int refresh = 0;
		if (refresh > 300)
		{
			theScanKeyThread.m_HeaterDetector.UpdateSensorState();
			refresh = 0;
		}
		else
			refresh++;

		switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
		{
		case MHS_TOP_LED: case MHS_CU_ELITE:
		case MHS_VLED: case MHS_VLED_ELITE:
		case MHS_FOXCONN:	//v3.0.T166 add
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			if (theScanKeyThread.IsJoyStickValid() == true)
			{
				//KEY_WIRE_CLAMP
				if (theMcConfig.GetKeyPadType() == 0)
				{
					theScanKeyThread.m_WireClampButton.UpdateSensorState();
					nState = theScanKeyThread.m_WireClampButton.GetSensorState();
				}
				else
					nState = theKeyPad.IsKeyDown(KEY_WIRE_CLAMP);
				if (nState == SENSOR_ON)
				{
					theVisionSys.SetIdleLight();
					//lStartTime = GetCurrentTime();	//v3.1.T355 delete
					CHTimer timer;	//v3.1.T355 add
					theServo.TriggerWireClampProfile(WCL_THREAD_WIRE_OPEN);
					theBond.m_AirTensioner.SetRelayState(FALSE);	//v3.1.T439 add
					do
					{
						if (theMcConfig.GetKeyPadType() == 0)
						{
							theScanKeyThread.m_WireClampButton.UpdateSensorState();
							nState = theScanKeyThread.m_WireClampButton.GetSensorState();
						}
						else
							nState = theKeyPad.IsKeyDown(KEY_WIRE_CLAMP);
						//v3.1.T355 delete
						//lElapseTime = GetCurrentTime() - lStartTime;
						//if (lElapseTime > WCL_OPEN_TIME) break;
						Sleep(20);
					} while ((nState == SENSOR_ON)
								&& (timer.GetTime_x1ms() < WCL_OPEN_TIME));	//v3.1.T355 add
					theServo.TriggerWireClampProfile(WCL_THREAD_WIRE_CLOSE);
					Sleep(100);
					theServo.TriggerWireClampProfile(WCL_HOLD_WIRE_CLOSE);
				}

				//KEY_EFO_SPARK
				if (theMcConfig.GetKeyPadType() == 0)
					theDigitalIO.GetInputBitState(EFO_SPARK_BUTTON, &nState);
				else
					nState = theKeyPad.IsKeyDown(KEY_EFO_SPARK);
				if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
				{
#if _SERVO_TYPE == 2
					theServo.TrigEFO();		//v3.1.T344 add
#else
					theServo.ResetBondProgress();
					theServo.RunBuffer(3);
					Sleep(20);
					theBond.CheckBondProgress(7, 3, false);	//v1.5.T113 add, v1.5.T115 edit
#endif
					//lStartTime = GetCurrentTime();	//v3.1.T355 delete
					CHTimer timer;	//v3.1.T355 add
					do
					{
						if (theMcConfig.GetKeyPadType() == 0)
							theDigitalIO.GetInputBitState(EFO_SPARK_BUTTON, &nState);
						else
							nState = theKeyPad.IsKeyDown(KEY_EFO_SPARK);
						//v3.1.T355 delete
						//if (nState == SENSOR_OFF) break;
						//lElapseTime = GetCurrentTime() - lStartTime;
						//if (lElapseTime > WCL_OPEN_TIME) break;
						Sleep(10);
					} while ((nState == SENSOR_ON)
								&& (timer.GetTime_x1ms() < WCL_OPEN_TIME));	//v3.1.T355 add

					if (theEFO.IsSparkSuccess() == false)	//v1.6.T338 add
					{
					    CStringList sList;
						if (theEFO.GetErrorCode() == SPARK_OPEN)
							sList.AddTail(theMsg.GetMsg(MSG_SPARK_OPEN));
						else
							sList.AddTail(theMsg.GetMsg(MSG_SPARK_GAP_LARGE));
						theMsg.ShowMsg(ERROR_MSG, sList);
					}

#if _SERVO_TYPE != 2	//v3.1.T355 add
					theServo.StopBuffer(3);
					Sleep(100);
#endif
				}

				//KEY_INDEXING
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_INDEXING);

					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_INDEXING, false);
						::PostMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_INDEXING, 0);
					}
				}

				//KEY_CLEAR_TRACK
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_CLEAR_TRACK);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_CLEAR_TRACK, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_CLEAR_TRACK, 0);
					}
				}

				//KEY_IN_MAG_UP
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_IN_MAG_UP);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_IN_MAG_UP, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_IN_MAG_UP, 0);
					}
				}

				//KEY_IN_MAG_DOWN
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_IN_MAG_DOWN);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_IN_MAG_DOWN, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_IN_MAG_DOWN, 0);
					}
				}

				//KEY_IN_MAG_HOME
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_IN_MAG_HOME);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_IN_MAG_HOME, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_IN_MAG_HOME, 0);
					}
				}

				//KEY_OUT_MAG_UP
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_OUT_MAG_UP);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_OUT_MAG_UP, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_OUT_MAG_UP, 0);
					}
				}

				//KEY_OUT_MAG_DOWN
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_OUT_MAG_DOWN);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_OUT_MAG_DOWN, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_OUT_MAG_DOWN, 0);
					}
				}

				//KEY_OUT_MAG_HOME
				switch (theMcConfig.GetKeyPadType())
				{
				case 1: case 2:	//v3.0.T486 edit
					nState = theKeyPad.IsKeyDown(KEY_OUT_MAG_HOME);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_OUT_MAG_HOME, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_OUT_MAG_HOME, 0);
					}
					break;
				case 3:	//v3.0.T486 add
					nState = theKeyPad.IsKeyDown(KEY_OUT_MAG_HOME);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_OUT_MAG_HOME, 0);
					break;
				}

				//KEY_WINDOW_CLAMP
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					nState = theKeyPad.IsKeyDown(KEY_WINDOW_CLAMP);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_WINDOW_CLAMP, false);
						::PostMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_WINDOW_CLAMP, 0);
						Sleep(500);
					}
				}
			}

			//KEY_CHG_CAPILLARY
			if ((theMcConfig.GetKeyPadType() >= 1)
				&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit, v3.0.Y488 edit
			{
				nState = theKeyPad.IsKeyDown(KEY_CHG_CAPILLARY);
				if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
				{
					theKeyPad.SetKeyDown(KEY_CHG_CAPILLARY, false);
					::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_CHG_CAPILLARY, 0);
				}
			}

			//KEY_PANEL_LIGHT
			if ((theMcConfig.GetKeyPadType() >= 1)
				&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
			{
				nState = theKeyPad.IsKeyDown(KEY_PANEL_LIGHT);
				if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
				{
					theKeyPad.SetKeyDown(KEY_PANEL_LIGHT, false);
					if (theBond.m_PanelLight.GetRelayState() == RELAY_ON)
						theBond.m_PanelLight.SetRelayState(FALSE);
					else
						theBond.m_PanelLight.SetRelayState(TRUE);
				}
			}

			if (theScanKeyThread.IsJoyStickValid())
			{
				if (theMcConfig.GetKeyPadType() == 0 || theMcConfig.GetKeyPadType() == 1)
				{
					if (theScanKeyThread.m_nJoyStickConfig == JOYSTICK_SERVO)
					{       
						//jog x+
						nState = theScanKeyThread.m_JoyStickPositiveX.GetSensorState();
						theScanKeyThread.m_JoyStickPositiveX.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickPositiveX.GetSensorState())
						{
							switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
							{
							case MHS_TOP_LED:
							case MHS_TOP_ELITE:	//v3.0.T410 add
							{
								theTopClamp.CheckClampStatus();
								if ((theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
									|| (theTopClamp.GetClampStatus() == WND_CLAMP_OPEN))
								{
									theTopClamp.PreCloseClamp();
								}
								break;
							}
							case MHS_CU_ELITE:
							{
								theCuEliteLFClamp.CheckClampStatus();
								if ((theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_SAFE_CLAMP)
									|| (theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_OPEN))
								{
									theCuEliteLFClamp.PreCloseClamp();
								}
								break;
							}
							}

							nState = theScanKeyThread.m_JoyStickPositiveX.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogX(MOTION_POSITIVE);
							else
								theServo.EndJogX();
						}

						//jog x-
						nState = theScanKeyThread.m_JoyStickNegativeX.GetSensorState();
						theScanKeyThread.m_JoyStickNegativeX.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickNegativeX.GetSensorState())
						{
							nState = theScanKeyThread.m_JoyStickNegativeX.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogX(MOTION_NEGATIVE);
							else
								theServo.EndJogX();
						}

						//jog y+
						nState = theScanKeyThread.m_JoyStickPositiveY.GetSensorState();
						theScanKeyThread.m_JoyStickPositiveY.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickPositiveY.GetSensorState())
						{
							nState = theScanKeyThread.m_JoyStickPositiveY.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogY(MOTION_POSITIVE);
							else
								theServo.EndJogY();
						}

						//jog y-
						nState = theScanKeyThread.m_JoyStickNegativeY.GetSensorState();
						theScanKeyThread.m_JoyStickNegativeY.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickNegativeY.GetSensorState())
						{
							switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
							{
							case MHS_TOP_LED:
							case MHS_TOP_ELITE:	//v3.0.T410 add
							{
								theTopClamp.CheckClampStatus();
								if ((theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
									|| (theTopClamp.GetClampStatus() == WND_CLAMP_OPEN))
								{
									theTopClamp.PreCloseClamp();
								}
								break;
							}
							case MHS_CU_ELITE:
							{
								theCuEliteLFClamp.CheckClampStatus();
								if ((theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_SAFE_CLAMP)
									|| (theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_OPEN))
								{
									theCuEliteLFClamp.PreCloseClamp();
								}
								break;
							}
							}

							nState = theScanKeyThread.m_JoyStickNegativeY.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogY(MOTION_NEGATIVE);
							else
								theServo.EndJogY();
						}
					}
				}
				else if ((theMcConfig.GetKeyPadType() == 2)
						|| (theMcConfig.GetKeyPadType() == 3))	//v2.0.T35 edit
				{
					if (theScanKeyThread.m_nJoyStickConfig == JOYSTICK_SERVO)
					{
						//KEY_LEFT
						if (theScanKeyThread.m_bKeyLeftPressed != theKeyPad.IsKeyDown(KEY_LEFT))
						{
							switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
							{
							case MHS_TOP_LED: case MHS_R2R:
							case MHS_FOXCONN:	//v3.0.T166 add
							case MHS_TOP_ELITE:	//v3.0.T410 add
							{
								theTopClamp.CheckClampStatus();
								if ((theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
									|| (theTopClamp.GetClampStatus() == WND_CLAMP_OPEN))
								{
									theTopClamp.PreCloseClamp();
								}
								break;
							}
							case MHS_CU_ELITE:
							{
								theCuEliteLFClamp.CheckClampStatus();
								if ((theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_SAFE_CLAMP)
									|| (theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_OPEN))
								{
									theCuEliteLFClamp.PreCloseClamp();
								}
								break;
							}
							}
							theScanKeyThread.m_bKeyLeftPressed = theKeyPad.IsKeyDown(KEY_LEFT);

							if (theScanKeyThread.m_bKeyLeftPressed == SENSOR_ON)
								theServo.StartJogX(MOTION_POSITIVE);
							else
								theServo.EndJogX();
						}

						//KEY_RIGHT
						if (theScanKeyThread.m_bKeyRightPressed != theKeyPad.IsKeyDown(KEY_RIGHT))
						{
							switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
							{
							case MHS_TOP_LED: case MHS_R2R:
							case MHS_FOXCONN:	//v3.0.T166 add
							case MHS_TOP_ELITE:	//v3.0.T410 add
							{
								theTopClamp.CheckClampStatus();
								if ((theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
									|| (theTopClamp.GetClampStatus() == WND_CLAMP_OPEN))
								{
									theTopClamp.PreCloseClamp();
								}
								break;
							}
							case MHS_CU_ELITE:
							{
								theCuEliteLFClamp.CheckClampStatus();
								if ((theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_SAFE_CLAMP)
									|| (theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_OPEN))
								{
									theCuEliteLFClamp.PreCloseClamp();
								}
								break;
							}
							}

							theScanKeyThread.m_bKeyRightPressed = theKeyPad.IsKeyDown(KEY_RIGHT);

							if (theScanKeyThread.m_bKeyRightPressed == SENSOR_ON)
								theServo.StartJogX(MOTION_NEGATIVE);
							else
								theServo.EndJogX();
						}

						//KEY_UP
						if (theScanKeyThread.m_bKeyUpPressed != theKeyPad.IsKeyDown(KEY_UP))
						{
							switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
							{
							case MHS_TOP_LED: case MHS_R2R:
							case MHS_FOXCONN:	//v3.0.T166 add
							case MHS_TOP_ELITE:	//v3.0.T410 add
							{
								theTopClamp.CheckClampStatus();
								if ((theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
									|| (theTopClamp.GetClampStatus() == WND_CLAMP_OPEN))
								{
									theTopClamp.PreCloseClamp();
								}
								break;
							}
							case MHS_CU_ELITE:
							{
								theCuEliteLFClamp.CheckClampStatus();
								if ((theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_SAFE_CLAMP)
									|| (theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_OPEN))
								{
									theCuEliteLFClamp.PreCloseClamp();
								}
								break;
							}
							}

							theScanKeyThread.m_bKeyUpPressed = theKeyPad.IsKeyDown(KEY_UP);
							if (theScanKeyThread.m_bKeyUpPressed == SENSOR_ON)
								theServo.StartJogY(MOTION_POSITIVE);
							else
								theServo.EndJogY();
						}

						//KEY_DOWN
						if (theScanKeyThread.m_bKeyDownPressed != theKeyPad.IsKeyDown(KEY_DOWN))
						{
							switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
							{
							case MHS_TOP_LED: case MHS_R2R:
							case MHS_FOXCONN:	//v3.0.T166 add
							case MHS_TOP_ELITE:	//v3.0.T410 add
							{
								theTopClamp.CheckClampStatus();
								if ((theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP)
									|| (theTopClamp.GetClampStatus() == WND_CLAMP_OPEN))
								{
									theTopClamp.PreCloseClamp();
								}
								break;
							}
							case MHS_CU_ELITE:
							{
								theCuEliteLFClamp.CheckClampStatus();
								if ((theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_SAFE_CLAMP)
									|| (theCuEliteLFClamp.GetClampStatus() == CUELITE_WND_CLAMP_OPEN))
								{
									theCuEliteLFClamp.PreCloseClamp();
								}
								break;
							}
							}

							theScanKeyThread.m_bKeyDownPressed = theKeyPad.IsKeyDown(KEY_DOWN);
			    
							if (theScanKeyThread.m_bKeyDownPressed == SENSOR_ON)
								theServo.StartJogY(MOTION_NEGATIVE);
							else
								theServo.EndJogY();
						}
					}
				}
			}
			break;
	    }
		case MHS_R2R:
		{
			if (theScanKeyThread.IsJoyStickValid())
			{
				//wire clamp , press n hold
				if (theMcConfig.GetKeyPadType() == 0)
				{
					theScanKeyThread.m_WireClampButton.UpdateSensorState();
					nState = theScanKeyThread.m_WireClampButton.GetSensorState();
				}
				else
					nState = theKeyPad.IsKeyDown(KEY_WIRE_CLAMP);
				if (nState == SENSOR_ON)
				{
					theVisionSys.SetIdleLight();
					lStartTime = GetCurrentTime();

					theServo.TriggerWireClampProfile(WCL_THREAD_WIRE_OPEN);
					do
					{
						if (theMcConfig.GetKeyPadType() == 0)
						{
							theScanKeyThread.m_WireClampButton.UpdateSensorState();
							nState = theScanKeyThread.m_WireClampButton.GetSensorState();
						}
						else
							nState = theKeyPad.IsKeyDown(KEY_WIRE_CLAMP);

						lElapseTime = GetCurrentTime() - lStartTime;
						if (lElapseTime > WCL_OPEN_TIME) break;

						Sleep(20);
					} while (nState == SENSOR_ON);
					theServo.TriggerWireClampProfile(WCL_THREAD_WIRE_CLOSE);
					Sleep(100);

					theServo.TriggerWireClampProfile(WCL_HOLD_WIRE_CLOSE);
				}

				if (theMcConfig.GetKeyPadType() == 0)
					theDigitalIO.GetInputBitState(EFO_SPARK_BUTTON, &nState);
				else
					nState = theKeyPad.IsKeyDown(KEY_EFO_SPARK);
				if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
				{
#if _SERVO_TYPE == 2
					theServo.TrigEFO();		//v3.1.T344 add
#else
					theServo.ResetBondProgress();
					theServo.RunBuffer(3);
					Sleep(20);
					theBond.CheckBondProgress(7, 3, false);	//v1.5.T113 add, v1.5.T115 edit
#endif
					lStartTime = GetCurrentTime();
					do
					{
						if (theMcConfig.GetKeyPadType() == 0)
							theDigitalIO.GetInputBitState(EFO_SPARK_BUTTON, &nState);
						else
							nState = theKeyPad.IsKeyDown(KEY_EFO_SPARK);
						if (nState == SENSOR_OFF) break;

						lElapseTime = GetCurrentTime() - lStartTime;
						if (lElapseTime > WCL_OPEN_TIME) break;

						Sleep(10);

					} while (nState == SENSOR_ON);

					if (theEFO.IsSparkSuccess() == false)	//v1.6.T338 add
					{
					    CStringList sList;
						if (theEFO.GetErrorCode() == SPARK_OPEN)
							sList.AddTail(theMsg.GetMsg(MSG_SPARK_OPEN));
						else
							sList.AddTail(theMsg.GetMsg(MSG_SPARK_GAP_LARGE));
						theMsg.ShowMsg(ERROR_MSG, sList);
					}

					theServo.StopBuffer(3);
					Sleep(100);
				}

				if (theMcConfig.GetKeyPadType() == 1)
				{
					nState = theKeyPad.IsKeyDown(KEY_INDEXING);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_INDEXING, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_INDEXING, 0);
					}
				}

				if (theMcConfig.GetKeyPadType() == 1)
				{
					nState = theKeyPad.IsKeyDown(KEY_WINDOW_CLAMP);
					if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
					{
						theKeyPad.SetKeyDown(KEY_WINDOW_CLAMP, false);
						::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_WINDOW_CLAMP, 0);
						Sleep(500);
					}
				}
			}

			if ((theMcConfig.GetKeyPadType() >= 1)
				&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
			{
				nState = theKeyPad.IsKeyDown(KEY_CHG_CAPILLARY);
				if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
				{
					theKeyPad.SetKeyDown(KEY_CHG_CAPILLARY, false);
					::SendMessage(AfxGetMainWnd()->m_hWnd , WM_CLICK_KEYBOARDSHORTCUT , KEY_CHG_CAPILLARY, 0);
				}
			}

			if ((theMcConfig.GetKeyPadType() >= 1)
				&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
			{
				nState = theKeyPad.IsKeyDown(KEY_PANEL_LIGHT);
				if ((nState == SENSOR_ON) && !theMcConfig.IsBypassAll())
				{
					theKeyPad.SetKeyDown(KEY_PANEL_LIGHT, false);
					if (theBond.m_PanelLight.GetRelayState() == RELAY_ON)
						theBond.m_PanelLight.SetRelayState(FALSE);
					else
						theBond.m_PanelLight.SetRelayState(TRUE);
				}
			}

			// x,y joystick
			if (theScanKeyThread.IsJoyStickValid())
			{
				if (theMcConfig.GetKeyPadType() == 0 || theMcConfig.GetKeyPadType() == 1)
				{
					if (theScanKeyThread.m_nJoyStickConfig == JOYSTICK_SERVO)
					{       
						//jog x+
						nState = theScanKeyThread.m_JoyStickPositiveX.GetSensorState();
						theScanKeyThread.m_JoyStickPositiveX.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickPositiveX.GetSensorState())
						{
							theTopClamp.CheckClampStatus();
							if (theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP || 
								theTopClamp.GetClampStatus() == WND_CLAMP_OPEN)
							{
								theTopClamp.PreCloseClamp();
							}
							
							nState = theScanKeyThread.m_JoyStickPositiveX.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogX(MOTION_POSITIVE);
							else
								theServo.EndJogX();
						}

						//jog x-
						nState = theScanKeyThread.m_JoyStickNegativeX.GetSensorState();
						theScanKeyThread.m_JoyStickNegativeX.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickNegativeX.GetSensorState())
						{
							nState = theScanKeyThread.m_JoyStickNegativeX.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogX(MOTION_NEGATIVE);
							else
								theServo.EndJogX();
						}

						//jog y+
						nState = theScanKeyThread.m_JoyStickPositiveY.GetSensorState();
						theScanKeyThread.m_JoyStickPositiveY.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickPositiveY.GetSensorState())
						{
							nState = theScanKeyThread.m_JoyStickPositiveY.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogY(MOTION_POSITIVE);
							else
								theServo.EndJogY();
						}

						//jog y-
						nState = theScanKeyThread.m_JoyStickNegativeY.GetSensorState();
						theScanKeyThread.m_JoyStickNegativeY.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickNegativeY.GetSensorState())
						{
							theTopClamp.CheckClampStatus();
							if (theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP || 
								theTopClamp.GetClampStatus() == WND_CLAMP_OPEN)
							{
								theTopClamp.PreCloseClamp();
							}
							
							nState = theScanKeyThread.m_JoyStickNegativeY.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogY(MOTION_NEGATIVE);
							else
								theServo.EndJogY();
						}
					}
				}
				else
				{
					if(theScanKeyThread.m_nJoyStickConfig == JOYSTICK_SERVO)
					{       
						//jog x+
						nState = theScanKeyThread.m_JoyStickPositiveX.GetSensorState();
						theScanKeyThread.m_JoyStickPositiveX.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickPositiveX.GetSensorState())
						{
							theTopClamp.CheckClampStatus();
							if (theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP || 
								theTopClamp.GetClampStatus() == WND_CLAMP_OPEN)
							{
								theTopClamp.PreCloseClamp();
							}

							nState = theScanKeyThread.m_JoyStickPositiveX.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogX(MOTION_POSITIVE);
							else
								theServo.EndJogX();
						}

						//jog x-
						nState = theScanKeyThread.m_JoyStickNegativeX.GetSensorState();
						theScanKeyThread.m_JoyStickNegativeX.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickNegativeX.GetSensorState())
						{
							nState = theScanKeyThread.m_JoyStickNegativeX.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogX(MOTION_NEGATIVE);
							else
								theServo.EndJogX();
						}

						//jog y+
						nState = theScanKeyThread.m_JoyStickPositiveY.GetSensorState();
						theScanKeyThread.m_JoyStickPositiveY.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickPositiveY.GetSensorState())
						{
							nState = theScanKeyThread.m_JoyStickPositiveY.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogY(MOTION_POSITIVE);
							else
								theServo.EndJogY();
						}

						//jog y-
						nState = theScanKeyThread.m_JoyStickNegativeY.GetSensorState();
						theScanKeyThread.m_JoyStickNegativeY.UpdateSensorState();
						if (nState != theScanKeyThread.m_JoyStickNegativeY.GetSensorState())
						{
							theTopClamp.CheckClampStatus();
							if (theTopClamp.GetClampStatus() == WND_CLAMP_SAFE_CLAMP || 
								theTopClamp.GetClampStatus() == WND_CLAMP_OPEN)
							{
								theTopClamp.PreCloseClamp();
							}

							nState = theScanKeyThread.m_JoyStickNegativeY.GetSensorState();
							if (nState == SENSOR_ON)
								theServo.StartJogY(MOTION_NEGATIVE);
							else
								theServo.EndJogY();
						}
					}
				}
			}
			break;
		}
		}
        Sleep(20);
    }

    return 0;
}

void CScanKeyThread::BeginThread(void)
{
    m_bScanEnable = true;
    m_pScanThread = AfxBeginThread(PerformScanKeyThread, (LPVOID)this);

    theKeyPad.SetKeyProhibit(KEY_WIRE_CLAMP, false);
    theKeyPad.SetKeyProhibit(KEY_EFO_SPARK, false);
    theKeyPad.SetKeyProhibit(KEY_SPEED_UP, false);
    theKeyPad.SetKeyProhibit(KEY_SPEED_DOWN, false);
    theKeyPad.SetKeyProhibit(KEY_INDEXING, false);
    theKeyPad.SetKeyProhibit(KEY_CLEAR_TRACK, false);
    theKeyPad.SetKeyProhibit(KEY_WINDOW_CLAMP, false);
    theKeyPad.SetKeyProhibit(KEY_CHG_CAPILLARY, false);
    theKeyPad.SetKeyProhibit(KEY_USG, false);
}

void CScanKeyThread::StopThread(void)
{
    m_bScanEnable = false;

    theKeyPad.SetKeyProhibit(KEY_WIRE_CLAMP, true);
    theKeyPad.SetKeyProhibit(KEY_EFO_SPARK, true);
    theKeyPad.SetKeyProhibit(KEY_SPEED_UP, true);
    theKeyPad.SetKeyProhibit(KEY_SPEED_DOWN, true);
    theKeyPad.SetKeyProhibit(KEY_INDEXING, true);
    theKeyPad.SetKeyProhibit(KEY_CLEAR_TRACK, true);
    theKeyPad.SetKeyProhibit(KEY_WINDOW_CLAMP, true);
    theKeyPad.SetKeyProhibit(KEY_CHG_CAPILLARY, true);
    theKeyPad.SetKeyProhibit(KEY_USG, true);
}

void CScanKeyThread::Init(void)
{
	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
		m_HeaterDetector.Initialize(theStepperIO.m_Input.m_HeaterDetector.nPort);
	else
		m_HeaterDetector.Initialize(HEATER_RELAY_STATE);

    m_WireClampButton.Initialize(WIRE_CLAMP_BUTTON);
    
	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		m_JoyStickPositiveX.Initialize(theStepperIO.m_Input.m_JoyStickPosX.nPort);
		m_JoyStickNegativeX.Initialize(theStepperIO.m_Input.m_JoyStickNegX.nPort);
		m_JoyStickPositiveY.Initialize(theStepperIO.m_Input.m_JoyStickPosY.nPort);
		m_JoyStickNegativeY.Initialize(theStepperIO.m_Input.m_JoyStickNegY.nPort);
	}
	else
	{
		m_JoyStickPositiveX.Initialize(JOYSTICK_X_POSITIVE);
		m_JoyStickNegativeX.Initialize(JOYSTICK_X_NEGATIVE);
		m_JoyStickPositiveY.Initialize(JOYSTICK_Y_POSITIVE);
		m_JoyStickNegativeY.Initialize(JOYSTICK_Y_NEGATIVE);
	}

    m_JoyStickPositiveX.ResetSensorState(SENSOR_OFF);
    m_JoyStickNegativeX.ResetSensorState(SENSOR_OFF);
    m_JoyStickPositiveY.ResetSensorState(SENSOR_OFF);
    m_JoyStickNegativeY.ResetSensorState(SENSOR_OFF);
}

void CScanKeyThread::EnableJoyStick(void)
{
	m_bJoyStickValid = true;
}

void CScanKeyThread::DisableJoyStick(void)
{
	m_bJoyStickValid = false;
}

bool CScanKeyThread::IsJoyStickValid(void)
{
    return m_bJoyStickValid;
}

void CScanKeyThread::SetJoyStickConfig(short nConfig, BYTE bMotor)
{
	m_nJoyStickConfig = nConfig;
	m_bStepperId = bMotor;

    //stepper is set already, must close the stepper dialog first
	if ((nConfig == JOYSTICK_SERVO) && (m_bStepperId != 0))
	{
		CStringList sList;
		sList.AddTail(theMsg.GetMsg(MSG_CLOSE_OTHER_DIALOG));
		theMsg.ShowMsg(WARNING_MSG, sList);
	}
}

bool CScanKeyThread::IsDirectionKeyPressed(void)
{
	return (m_bKeyLeftPressed || m_bKeyRightPressed
			|| m_bKeyUpPressed || m_bKeyDownPressed);
}
