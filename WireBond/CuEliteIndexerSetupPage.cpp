// CuEliteIndexerSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "CuEliteIndexerSetupPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "PopUpCuEliteStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "TeachCuEliteWorkHolderSheet.h"
#include "MachineConfig.h"
#include "VisionSystem.h"
#include "TowerLight.h"
#include "ScanKeyThread.h"
#include "CuIndex.h"
#include "CuClamp.h"
#include "CuOnload.h"
#include "R2RIndex.h"
#include "servo.h"

// CCuEliteIndexerSetupPage dialog

IMPLEMENT_DYNAMIC(CCuEliteIndexerSetupPage, CPropertyPage)

CCuEliteIndexerSetupPage::CCuEliteIndexerSetupPage()
	: CPropertyPage(CCuEliteIndexerSetupPage::IDD)
	, m_nStep(0)
	, m_nIndexId(0)
	, m_iIndexTchMode(MODE_CUELITE_MANUAL)
{
	

}

CCuEliteIndexerSetupPage::~CCuEliteIndexerSetupPage()
{
}

void CCuEliteIndexerSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_TCH_INDEX_MODE, m_iIndexTchMode);
}


BEGIN_MESSAGE_MAP(CCuEliteIndexerSetupPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, &CCuEliteIndexerSetupPage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CCuEliteIndexerSetupPage::OnBnClickedButtonNext)
//	ON_EN_SETFOCUS(IDC_EDIT_INDEX_LOGIC_ZERO, &CCuEliteIndexerSetupPage::OnEnSetfocusEditIndexLogicZero)
//	ON_EN_SETFOCUS(IDC_EDIT_INDEX_RIGHT_LIMIT, &CCuEliteIndexerSetupPage::OnEnSetfocusEditIndexRightLimit)
ON_BN_CLICKED(IDC_BUTTON_STOP_TCH, &CCuEliteIndexerSetupPage::OnBnClickedButtonStopTch)
ON_CBN_SELCHANGE(IDC_COMBO_TCH_INDEX_MODE, &CCuEliteIndexerSetupPage::OnCbnSelchangeComboTchIndexMode)
ON_LBN_DBLCLK(IDC_LIST_INDEX_ERROR_SHOW, &CCuEliteIndexerSetupPage::OnLbnDblclkListIndexErrorShow)
END_MESSAGE_MAP()


// CCuEliteIndexerSetupPage message handlers

HBRUSH CCuEliteIndexerSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CCuEliteIndexerSetupPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_NEXT);
    pWnd->EnableWindow(0);
	/*pWnd = this->GetParent()->GetDlgItem(IDOK);
    pWnd->EnableWindow(0);*/

	pWnd = GetDlgItem(IDC_BUTTON_START);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));

	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT));

	pWnd = GetDlgItem(IDC_STATIC_INDEX_TUNE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_TUNE));

	pWnd = GetDlgItem(IDC_BUTTON_STOP_TCH);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TCH_INDEX_MODE);
	pComboBox->InsertString(0, _T("Manual"));
	pComboBox->InsertString(1, _T("Auto"));

	m_iIndexTchMode = 0;

	pWnd = GetDlgItem(IDC_BUTTON_DUMMMY);
    pWnd->SetWindowPos(NULL, 1000, 1000, 100, 100, SWP_NOZORDER);

	CUELITE_DEVICE_RECORD stCuEliteDevicePara;
	theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

	CUELITE_TRACK_DATA stTrackRcd;
	theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
	short nTotalUints;
	if (stCuEliteDevicePara.nDeviceColmNum%stCuEliteDevicePara.nWndUnitNum != 0)
		nTotalUints = stCuEliteDevicePara.nDeviceColmNum/stCuEliteDevicePara.nWndUnitNum +1;
	else
		nTotalUints = stCuEliteDevicePara.nDeviceColmNum/stCuEliteDevicePara.nWndUnitNum;

	CString str;
	for (int i = 0; i < nTotalUints; i++)
	{
		str.Format(_T("Index Id: %d, error: %d"), i, stTrackRcd.lIndexError[i]);
		pListData->InsertString(i, str);
	}

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCuEliteIndexerSetupPage::OnBnClickedButtonStart()
{
	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();

	theMHS.InitAutoIndex();
	theCuEliteTrack.SetPrIndexMask(true);
	theMHS.SetLastLf(true);

	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->EnableWindow(0);

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);

	m_nStep = 0;
	//theCuEliteTrack.sestIndexTeachMode(true);

    pListBox->InsertString(0, theMsg.GetMsg(MSG_WAIT_INDEXER_HOME));

	theCuEliteTrack.ResetSlipRcd();

	theServo.MoveBondHeadToSafePos();
	Sleep(10);

	theCuEliteTrack.SearchIndexStepHome();
	theCuEliteTrack.WaitStepHomeFinish();
	Sleep(10);

	pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));

	m_nIndexId = 0;

	pListData->ResetContent();
		
	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->EnableWindow(1);

	if (psheet->m_iPageMode == WH_CUELITE_EDIT)
	{
		pWnd = psheet->GetDlgItem(IDOK);
		pWnd->EnableWindow(0);
	}
	else
	{
		CUELITE_TRACK_DATA stTrackRcd;
		theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);
		for (int i=0; i<CUELITE_MAX_INDEX_TIME; i++)
			stTrackRcd.lIndexError[i] = 0;
		theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);
	}
}

void CCuEliteIndexerSetupPage::OnBnClickedButtonNext()
{
	long lPos, lDest, lLastTime, lElapseTime;
	short nEvent =1;
	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	TBL_POS stCurTblPos;
	short nSideRedLight, nSideBlueLight;
	short nPatRcd = 0;

	theCuEliteTrack.FlagTeaching(TRUE);

	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();
	
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);

	CUELITE_TRACK_DATA stTrackRcd;
	theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);




	m_nStep++;
	//theCuEliteTrack.sestIndexTeachMode(true);

	//----- m_nStep==2 -----------------------------
	if(m_iIndexTchMode==MODE_CUELITE_AUTO && m_nStep==2)
	{
		
		    theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();	
			
			theVisionSys.LearnVisionPattern(false, 120, 120, &nPatRcd, true);

			theVisionSys.ClearPatternRcd(
				(short)stPrIndexRefRcd.ulVisionAlignPatternID);

			stPrIndexRefRcd.ulVisionAlignPatternID = nPatRcd;

			theVisionSys.SetPassScore(nPatRcd, thePgm.GetRcd()->nLeadPrPassScore);

			if (!theMcConfig.IsBypassAll())
			{
				theServo.GetTablePosition(&(stPrIndexRefRcd.stVisionAlignPos));
				stCurTblPos = stPrIndexRefRcd.stVisionAlignPos;
			}
			else
			{
				stPrIndexRefRcd.stVisionAlignPos.dx = 40000.0 / theServo.GetResolXY();
				stPrIndexRefRcd.stVisionAlignPos.dy =  8000.0 / theServo.GetResolXY();
			}

			theVisionSys.GetLightingParameter(&(stPrIndexRefRcd.stVisionLightPara.nCoaxRedLight), 
					&nSideRedLight, 
					&(stPrIndexRefRcd.stVisionLightPara.nCoaxBlueLight),
					&nSideBlueLight);
			stPrIndexRefRcd.stVisionLightPara.bSideRedLight  = (byte)nSideRedLight;
			stPrIndexRefRcd.stVisionLightPara.bSideBlueLight = (byte)nSideBlueLight;

			stPrIndexRefRcd.bIndexPrAvail = true;


			//SAVE RCD
            theDevice.SetIndexPrRef(&stPrIndexRefRcd);

            theVisionSys.EnableVideo(true);

			theCuEliteTrack.FlagTeaching(FALSE);
		
		return;
	}

    //-----------------------------------------------------


	//------------index----------------------------------------------------
    //request indexer to index
    theMHS.SetAutoIndex(true);
    theMHS.SetDryRun(false);
	
	//request onload relases leadframe as bonding unit =0
	if (theMHS.GetCurBondUnit() == 0)
	{
		int iPusherStatus;

		theCuEliteTrack.theInjectSnr.UpdateSensorState();
		if (theCuEliteTrack.theInjectSnr.GetSensorState() == FALSE)
		{
			theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
			Sleep(50);
 
			iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerForwardMotion();
			iPusherStatus+=theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
			if (iPusherStatus != FOK)
			{
				CStringList sList;
				sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_PUSH_KICKER_WRONG));
				theMsg.ShowMsg(WARNING_MSG, sList);

				theCuEliteTrack.FlagTeaching(FALSE);
				return;
			}
			theCuEliteTrack.theInjectSnr.UpdateSensorState();
			if(theCuEliteTrack.theInjectSnr.GetSensorState() == TRUE)
			{
				Sleep(100);
				theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);								
			}
		}

		//theCuEliteTrack.theInjectSnr.UpdateSensorState();
		//if(theCuEliteTrack.theInjectSnr.GetSensorState() == FALSE)
		{
			theCuEliteOnloader.ResetOnloadInitStatus();
			theCuEliteTrack.RequestOnloadEvent();
		}
	}
	Sleep(10);

	theServo.MoveBondHeadToSafePos();
	Sleep(100);

    //submit the condition
	if (theCuEliteTrack.GetCuEliteTrackEvent() == INDEX_EVENT_IDLE)
		theCuEliteTrack.SubmitIndexEvent(INDEX_EVENT_SINGLE_INDEX); //w.question:single

    //wait for indexer to be idle, then stop indexer
	Sleep(100);
	lLastTime=GetCurrentTime();
	while(nEvent != INDEX_EVENT_IDLE)
    {
		nEvent = theCuEliteTrack.GetCuEliteTrackEvent();

        //nMhsErrStatus = theMHS.ShowError(); //release later
        //if (nMhsErrStatus != FOK)
        //    break;

        lElapseTime=::GetCurrentTime()-lLastTime;
        if (lElapseTime > 20000) 
			break;

		Sleep(1);
	}
	//request indexer to stop
    theMHS.SetAutoIndex(false);

    //--------------------------------------



	//wait until the indexer INDEX_EVENT_IDLE , then calculate offset here
	//--------------- PR adjust-----------------------------------------------------
	long lOffset;
	if(m_iIndexTchMode==MODE_CUELITE_AUTO && m_nStep>2)
	{
		int iReturnStatus, iBusyState;
	        

		iReturnStatus = theCuEliteTrack.CaculatePrIndexOffset(&lOffset);
		Sleep(10);

		if(iReturnStatus == FOK)
		{
			if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nSrchUnit)
				theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
			theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
			theCuEliteLFClamp.PreCloseClamp();

			Sleep(400);
			theStepper.MoveStep(CUELITE_DEVICE_INDEXER, round((double)lOffset/theStepper.GetResol(CUELITE_DEVICE_INDEXER)), MOTION_RELATIVE);

			lLastTime = GetCurrentTime();
			do
			{
				iBusyState = (int)theStepper.IsBusy(CUELITE_DEVICE_INDEXER);
				Sleep(2);

				lElapseTime = ::GetCurrentTime() - lLastTime;
				if (lElapseTime > 20000) break;
			} while (iBusyState != 0);

			Sleep(100);

			theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
			theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);
            Sleep(stTrackRcd.lClawDelay);
			theCuEliteLFClamp.CloseClamp();
		}
		else
		{
			theServo.MoveBondHeadToSafePos();
            Sleep(10);

			theTowerLight.EnableIdleLight(); 

			theStepper.GetPos(CUELITE_DEVICE_INDEXER,&lPos);

			if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nSrchUnit)
				theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
			theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
			Sleep(100);
			theCuEliteLFClamp.MoveClampSafePos();
			//theTopClamp.PreCloseClamp();

		 	CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_DEVICE_INDEXER, CUELITE_SPEED_SMALL);

			theStepperCtrler.PresetDlgPos(DLG_POS_BOTTOM_RIGHT);
			theStepperCtrler.Preset2ndCtrlID(CUELITE_WINDOW_CLAMPER);

			theStepperCtrler.DoModal();

			//theCuEliteLFClamp.CloseClamp();
			theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
			theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);
            Sleep(stTrackRcd.lClawDelay);
			theCuEliteLFClamp.CloseClamp();

			theStepper.GetPos(CUELITE_DEVICE_INDEXER,&lDest);

			lOffset = round((double)(lDest - lPos)* theStepper.GetResol(CUELITE_DEVICE_INDEXER));
		}
	}

	else  //m_nStep =1,2
	{
		theStepper.GetPos(CUELITE_DEVICE_INDEXER,&lPos);

		if (theCuEliteTrack.GetCurrentBondUnit() <= 2)
			theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
		theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
		Sleep(100);
		theCuEliteLFClamp.PreCloseClamp();

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_DEVICE_INDEXER, CUELITE_SPEED_SMALL);

		theStepperCtrler.PresetDlgPos(DLG_POS_BOTTOM_RIGHT);
		theStepperCtrler.Preset2ndCtrlID(CUELITE_WINDOW_CLAMPER);

	    theStepperCtrler.DoModal();

		//theCuEliteLFClamp.CloseClamp();
		theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
		theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);
        Sleep(stTrackRcd.lClawDelay);
		theCuEliteLFClamp.CloseClamp();

		theStepper.GetPos(CUELITE_DEVICE_INDEXER,&lDest);


		lOffset = round((double)(lDest - lPos)* theStepper.GetResol(CUELITE_DEVICE_INDEXER));
	}
    //-----------------------------------------


    //-----------Update records from PR index--------------------------------------------   
	{
	stTrackRcd.lIndexError[m_nIndexId] = stTrackRcd.lIndexError[m_nIndexId] + (short)lOffset;

    theCuEliteTrack.TailPosUpdate(lOffset);

	CString czString;
	czString.AppendFormat(_T("Index Id: %d, error: %d. Updated!"),  m_nIndexId, stTrackRcd.lIndexError[m_nIndexId]);
	pListData->InsertString(m_nIndexId, czString);

	theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);
	m_nIndexId ++;

	}

	//--------------------------------



    //update UI 
	if(m_iIndexTchMode==MODE_CUELITE_MANUAL)
	{
		pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));
		pListBox->InsertString(1, theMsg.GetMsg(MSG_DONE_TO_QUIT));
	}
	else  //auto
	{
		if (m_nStep == 1)
		{
			theVisionSys.SetSearchSize(120, 120);

			pListBox->InsertString(0, _T("Adjust Lighting and find a patten"));
			pListBox->InsertString(1, _T("Press Next to record"));
		}
	}

    //--------------------

	//update MachineData as last unit
    //calculate nTotalUnits

	CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
	theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
	short nTotalUints;

	if( stCuEliteDevicePara.nDeviceColmNum%stCuEliteDevicePara.nWndUnitNum !=0)
	{
		nTotalUints = stCuEliteDevicePara.nDeviceColmNum/stCuEliteDevicePara.nWndUnitNum +1;

	}
	else
	{
		nTotalUints = stCuEliteDevicePara.nDeviceColmNum/stCuEliteDevicePara.nWndUnitNum;
	}


	if(theCuEliteTrack.GetCurrentBondUnit() == nTotalUints)
	{
		CWnd* pWnd= GetDlgItem(IDC_BUTTON_NEXT);
		pWnd->EnableWindow(FALSE);

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_DONE_TO_QUIT));

		//theMHS.InitAutoIndex();

		theCuEliteLFClamp.CloseClamp();

        theApp.ArchiveMachineData(ARCHIVE_SAVE);
		Sleep(100);
		CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
		theCuEliteTrack.RefreshMeasurementData(&stCuEliteDevicePara);

		theCuEliteTrack.SetPrIndexMask(false);
		theMHS.SetLastLf(false);

        //update UI
		if (psheet->m_iPageMode == WH_CUELITE_TEACH)
			psheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(1);
		else
			psheet->GetDlgItem(IDOK)->EnableWindow(1);
	}

	theCuEliteTrack.FlagTeaching(FALSE);
}


BOOL CCuEliteIndexerSetupPage::OnSetActive()
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	pListBox->InsertString(0, theMsg.GetMsg(MSG_START_TO_HOME_INDEXER));

	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();

	if (psheet->m_iPageMode == WH_CUELITE_TEACH)
	{
		CWnd* pWnd = psheet->GetDlgItem(ID_WIZNEXT);
        pWnd->EnableWindow(1);
	}
	else
	{

	    CWnd* pWnd = psheet->GetDlgItem(IDOK);
		pWnd->EnableWindow(1);

	    pWnd = GetDlgItem(IDC_BUTTON_START);
        pWnd->EnableWindow(1);


	}


	theScanKeyThread.SetJoyStickConfig(JOYSTICK_STEPPER_HORIZONTAL, CUELITE_DEVICE_INDEXER);

	return CPropertyPage::OnSetActive();
}


BOOL CCuEliteIndexerSetupPage::OnKillActive()
{

	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	return ( CPropertyPage::OnKillActive());

}


void CCuEliteIndexerSetupPage::OnBnClickedButtonStopTch()
{
	// TODO: Add your control notification handler code here
	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();

	if (psheet->m_iPageMode == WH_CUELITE_EDIT)
	{
		CWnd* pWnd = psheet->GetDlgItem(IDOK);
		pWnd->EnableWindow(1);
	}

}

void CCuEliteIndexerSetupPage::OnCbnSelchangeComboTchIndexMode()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

}

void CCuEliteIndexerSetupPage::OnLbnDblclkListIndexErrorShow()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
	int index = pListData->GetCurSel();
	long lIndexError=0;

	if (index > -1 && index < 16)
	{
		CUELITE_TRACK_DATA stTrackRcd;
		theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

		CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMMY);
		pWnd->SetFocus();

		theKeyPad.GetUserKeyInData(&lIndexError, stTrackRcd.lIndexError[index], -35000, 35000, true);

		stTrackRcd.lIndexError[index] = (short)lIndexError;

		pListData->DeleteString(index);
		CString czString;
		czString.AppendFormat(_T("Index Id: %d, error: %d. Changed!"),  index, stTrackRcd.lIndexError[index]);
		pListData->InsertString(index, czString);

		theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

		CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
		theCuEliteTrack.RefreshMeasurementData(&stCuEliteDevicePara);
	}

}