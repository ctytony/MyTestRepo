// TopIndexerSetupPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "TopIndexerSetupPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "PopUpTopStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "TeachWorkHolderSheet.h"
#include "MachineConfig.h"
#include "VisionSystem.h"
#include "TowerLight.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "R2RIndex.h"
#include "servo.h"
#include "Busy.h"
#include "ControlState.h"

// CTopIndexerSetupPage dialog
IMPLEMENT_DYNAMIC(CTopIndexerSetupPage, CPropertyPage)

CTopIndexerSetupPage::CTopIndexerSetupPage()
	: CPropertyPage(CTopIndexerSetupPage::IDD)
	, m_nStep(0)
	, m_nIndexID(0)
	, m_nWndUnitNum(0)	//v3.0.T86 add
	, m_nTeachMode(TCHMODE_MANUAL)
{
}

CTopIndexerSetupPage::~CTopIndexerSetupPage()
{
}

void CTopIndexerSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_TCH_INDEX_MODE, m_nTeachMode);
	DDX_Text(pDX, IDC_EDIT_CLAMP_COL, m_nWndUnitNum);	//v3.0.T86 add
}

BEGIN_MESSAGE_MAP(CTopIndexerSetupPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TCH, OnStop)
	ON_CBN_SELCHANGE(IDC_COMBO_TCH_INDEX_MODE, OnIndexMode)
	ON_LBN_DBLCLK(IDC_LIST_INDEX_ERROR_SHOW, OnIndexErrorShow)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W158 add
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_COL, OnFocusClampCol)	//v3.0.T86 add
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CTopIndexerSetupPage::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CTopIndexerSetupPage message handlers
HBRUSH CTopIndexerSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CTopIndexerSetupPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);

	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));
	SetDlgItemTextW(IDC_STATIC_INDEX_TUNE, theMsg.GetMsg(MSG_INDEX_TUNE));
	SetDlgItemTextW(IDC_BUTTON_STOP_TCH, theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_STATIC_CLAMP_COL, theMsg.GetMsg(MSG_CLAMP_COL));	//v3.0.T86 add

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TCH_INDEX_MODE);
	pComboBox->AddString(_T("Manual"));
	pComboBox->AddString(_T("Auto"));

	m_nTeachMode = 0;

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->AddString(theMsg.GetMsg(MSG_START_TO_HOME_INDEXER));

	short nTotalUnit;
	const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
	m_nWndUnitNum = pstDevice->nWndUnitNum;	//v3.0.T86 add
	if (m_nWndUnitNum > 0)
	{
		nTotalUnit = pstDevice->nDeviceColmNum / m_nWndUnitNum;
		if (pstDevice->nDeviceColmNum % m_nWndUnitNum != 0) nTotalUnit++;
	}

	CString sMsg;
	pListBox = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
	for (int i=0; i<nTotalUnit; i++)
	{
		sMsg.Format(_T("Index Id: %d, error: %d"),
						i, theTopTrack.GetRcd()->lIndexError[i]);
		pListBox->AddString(sMsg);
	}

	return CPropertyPage::OnInitDialog();
}

void CTopIndexerSetupPage::OnStart()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	if (!theMcConfig.IsBypassIndex())
	{
		theMHS.InitAutoIndex();
		theTopTrack.SetPrIndexMask(true);
		theMHS.SetLastLf(true);
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);

	m_nStep = 0;

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_WAIT_INDEXER_HOME));

	if (!theMcConfig.IsBypassIndex())
	{
		theTopTrack.ResetSlipRcd();
		theServo.MoveBondHeadToSafePos();
		Sleep_x1ms(10);
		theTopTrack.HomeIndexer();
		theTopTrack.WaitHomeIndexer();
		Sleep_x1ms(10);
	}
	else
		Sleep_x1ms(1000);

	pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));

	m_nIndexID = 0;

	pListBox = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
	pListBox->ResetContent();
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow();

	CTeachWorkHolderSheet *pSheet = (CTeachWorkHolderSheet*)GetParent();
	if (pSheet->m_iPageMode == WH_EDIT)
		pSheet->GetDlgItem(IDOK)->EnableWindow(false);
	else
	{
		for (int i = 0; i < TOP_MAX_INDEX_TIME; i++)
			theTopTrack.GetRcd()->lIndexError[i] = 0;
	}
}

void CTopIndexerSetupPage::OnNext()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	CTeachWorkHolderSheet *pSheet = (CTeachWorkHolderSheet*)GetParent();
	CControlState cs(GetDlgItem(IDC_BUTTON_NEXT)->m_hWnd, FALSE, TRUE);	//V1.1.W158 add
	cs.AddControl(pSheet->GetDlgItem(IDCANCEL)->m_hWnd, FALSE, TRUE);	//V1.1.W158 add

	theTopTrack.SetTeaching(true);
	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();

	m_nStep++;

	if ((m_nTeachMode == TCHMODE_AUTO) && (m_nStep == 2))
	{
	    theVisionSys.EnableVideo(false);
		theVisionSys.GrabImage();

		short nPatRcd = 0;
		theVisionSys.LearnVisionPattern(false, 120, 120, &nPatRcd, true);

		INDEX_PR_REF stPrRef;
		theDevice.GetIndexPrRef(&stPrRef);
		theVisionSys.ClearPatternRcd((short)stPrRef.ulVisionAlignPatternID);
		stPrRef.ulVisionAlignPatternID = nPatRcd;

		theVisionSys.SetPassScore(nPatRcd, thePgm.GetRcd()->nLeadPrPassScore);

		stPrRef.stVisionAlignPos.dx = 40000 / theServo.GetResolXY();
		stPrRef.stVisionAlignPos.dy = 8000 / theServo.GetResolXY();
		if (!theMcConfig.IsBypassServo())
			theServo.GetTablePosition(&stPrRef.stVisionAlignPos);

		short nSideRedLight, nSideBlueLight;
		theVisionSys.GetLightingParameter(
				&stPrRef.stVisionLightPara.nCoaxRedLight, &nSideRedLight,
				&stPrRef.stVisionLightPara.nCoaxBlueLight, &nSideBlueLight);
		stPrRef.stVisionLightPara.bSideRedLight = (byte)nSideRedLight;
		stPrRef.stVisionLightPara.bSideBlueLight = (byte)nSideBlueLight;
		stPrRef.bIndexPrAvail = true;
        theDevice.SetIndexPrRef(&stPrRef);

        theVisionSys.EnableVideo(true);
		theTopTrack.SetTeaching(false);
		return;
	}

    theMHS.SetAutoIndex(true);
    theMHS.SetDryRun(false);
		
	if (!theMcConfig.IsBypassIndex() && (theMHS.GetCurBondUnit() == 0))
	{
		int iPusherStatus = theTopOnloader.m_InjectPusher.TriggerForwardMotion();
		iPusherStatus += theTopOnloader.m_InjectPusher.TriggerBacktoHome();
		if (iPusherStatus != FOK)
		{
			short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
			theTopTrack.SetTeaching(false);
			return;
		}

		theTopOnloader.ResetAfterInit();
		theTopTrack.RequestOnload();
		Sleep_x1ms(10);
	}

	theServo.MoveBondHeadToSafePos();
    Sleep_x1ms(100);

	if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
	{
		theTopTrack.SubmitIndexEvent(INDEX_EVENT_SINGLE_INDEX);
		Sleep_x1ms(100);
	}

	CHTimer timer;
	if (!theMcConfig.IsBypassIndex())
	{
		do
		{
			ProcessMessage();
			if (theMHS.ShowError() != FOK) return;
		} while ((theTopTrack.GetEvent() != INDEX_EVENT_IDLE)
					&& (timer.GetTime_x1ms() < 10000));
	}

	theMHS.SetAutoIndex(false);

	long lPos, lDest, lOffset = 0;
	if ((m_nTeachMode == TCHMODE_AUTO) && (m_nStep > 2))
	{
		Sleep_x1ms(10);

		if (theTopTrack.CalcPrIndexOffset(&lOffset) == FOK)
		{
			if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
				theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
			theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
			theTopClamp.PreCloseClamp();
		   	Sleep_x1ms(400);
			theStepper.MoveStep(theTopTrack.GetIndexStepID(),
				round(lOffset/theStepper.GetResol(theTopTrack.GetIndexStepID())), MOTION_RELATIVE);

			timer.StartTimer();
			do
			{
				ProcessMessage();
			} while (theStepper.IsBusy(theTopTrack.GetIndexStepID())
						&& (timer.GetTime_x1ms() < 10000));
			Sleep_x1ms(100);

			theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
			theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);
		}
		else
		{
			theServo.MoveBondHeadToSafePos();
            Sleep_x1ms(10);

			theTowerLight.EnableBlinkRedLight(false);
			theTowerLight.EnableIdleLight(); 

			theStepper.GetPos(theTopTrack.GetIndexStepID(), &lPos);

			if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
				theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
			theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
			Sleep_x1ms(100);
			theTopClamp.MoveClampSafePos();

	 		CPopUpTopStepperCtr dlg(theTopTrack.GetIndexStepID(), SPEED_SMALL);
			dlg.PresetDlgPos(DLG_POS_BOTTOM_RIGHT);
			dlg.Preset2ndCtrlID(TOP_WINDOW_CLAMPER);
			dlg.DoModal();

			theTopClamp.CloseClamp();
			theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
			theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);

			theStepper.GetPos(theTopTrack.GetIndexStepID(), &lDest);
			lOffset = round((lDest - lPos) * theStepper.GetResol(theTopTrack.GetIndexStepID()));
		}
	}
	else
	{
		theStepper.GetPos(theTopTrack.GetIndexStepID(), &lPos);

		if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
			theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);

		theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
		Sleep_x1ms(100);
		theTopClamp.PreCloseClamp();

	 	CPopUpTopStepperCtr dlg(theTopTrack.GetIndexStepID(), SPEED_SMALL);
		dlg.PresetDlgPos(DLG_POS_BOTTOM_RIGHT);
		dlg.Preset2ndCtrlID(TOP_WINDOW_CLAMPER);
        dlg.DoModal();

		theTopClamp.CloseClamp();
		theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
		theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);

		theStepper.GetPos(theTopTrack.GetIndexStepID(),&lDest);
		lOffset = round((lDest - lPos) * theStepper.GetResol(theTopTrack.GetIndexStepID()));
	}

	TOP_TRACK_DATA stTrackRcd;
	theTopTrack.GetRcd(&stTrackRcd);
	stTrackRcd.lIndexError[m_nIndexID] = stTrackRcd.lIndexError[m_nIndexID] + lOffset;

    theTopTrack.SetTailPos(lOffset);

	CString str;
	str.Format(_T("IndexID: %d, Error: %d. Updated!"),
				m_nIndexID, stTrackRcd.lIndexError[m_nIndexID]);
	CListBox *pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
	pListData->InsertString(m_nIndexID, str);

    theTopTrack.SetRcd(&stTrackRcd);
	m_nIndexID ++;

	if (m_nTeachMode == TCHMODE_MANUAL)
    {
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_QUIT));
	}
	else if (m_nStep == 1) //auto
	{
		//DISPLAY MSG
		theVisionSys.SetSearchSize(120, 120);
		pListBox->AddString(_T("Adjust Lighting and find a patten"));
		pListBox->AddString(_T("Press Next to record"));
	}

	//v3.0.T86 edit
	short nTotalUnit = 0;
	const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
	if (pstDevice->nWndUnitNum != 0)
	{
		nTotalUnit = pstDevice->nDeviceColmNum / pstDevice->nWndUnitNum;
		if (pstDevice->nDeviceColmNum % pstDevice->nWndUnitNum != 0) nTotalUnit++;
	}

	if (theTopTrack.GetCurBondUnit() == nTotalUnit)
	{
		cs.UpdateFinalState(GetDlgItem(IDC_BUTTON_NEXT)->m_hWnd, FALSE);

		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_QUIT));

		theTopClamp.CloseClamp();

        theApp.ArchiveMachineData(ARCHIVE_SAVE);
		Sleep_x1ms(100);

		theTopTrack.RefreshMeasurementData(pstDevice);
		theApp.ArchivePowerOnData(ARCHIVE_SAVE);	//v3.0.T86 add

		theTopTrack.SetPrIndexMask(false);
		theMHS.SetLastLf(false);

		pSheet->GetDlgItem(pSheet->m_iPageMode == WH_TEACH ? ID_WIZNEXT : IDOK)->EnableWindow();
	}

	theTopTrack.SetTeaching(false);
}

BOOL CTopIndexerSetupPage::OnSetActive()
{
	CTeachWorkHolderSheet *pSheet = (CTeachWorkHolderSheet*)GetParent();
	if (pSheet->m_iPageMode == WH_TEACH)
		pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(false);
	return CPropertyPage::OnSetActive();
}

void CTopIndexerSetupPage::OnStop()
{
	CTeachWorkHolderSheet *pSheet = (CTeachWorkHolderSheet*)GetParent();
	if (pSheet->m_iPageMode == WH_EDIT)
		pSheet->GetDlgItem(IDOK)->EnableWindow();
}

void CTopIndexerSetupPage::OnIndexMode()
{
	UpdateData(true);
}

void CTopIndexerSetupPage::OnIndexErrorShow()
{
	CListBox *pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
	int index = pListData->GetCurSel();
	if ((index < 0) || (index > 15)) return;

	CBusy bs(&m_bBusy);		//V1.1.W158 add

	GetDlgItem(IDC_STATIC_TCH_INDEX_MODE)->SetFocus();

	TOP_TRACK_DATA stTrackRcd;
	theTopTrack.GetRcd(&stTrackRcd);
	if (theKeyPad.GetUserKeyInData(&stTrackRcd.lIndexError[index],
		stTrackRcd.lIndexError[index], -35000, 35000, true) == CANCEL_SELECTED)
		return;
	theTopTrack.SetRcd(&stTrackRcd);

	pListData->DeleteString(index);
	CString str;
	str.Format(_T("Index Id: %d, error: %d. Changed!"),
					index, stTrackRcd.lIndexError[index]);
	pListData->InsertString(index, str);

	//v3.0.T86 edit
	const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
	theTopTrack.RefreshMeasurementData(pstDevice);
}

LRESULT CTopIndexerSetupPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W158 add
{
	return (m_bBusy == TRUE);
}

//v3.0.T86 add
void CTopIndexerSetupPage::OnFocusClampCol()
{
	CBusy bs(&m_bBusy);
	GetDlgItem(IDC_STATIC_CLAMP_COL)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWndUnitNum);
    UpdateData(false);
	theDevice.GetTopRcd()->nWndUnitNum = m_nWndUnitNum;
}

void CTopIndexerSetupPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_RECIPE, 0);	//v3.0.T457 add
}
