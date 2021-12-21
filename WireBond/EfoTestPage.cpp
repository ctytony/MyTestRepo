/***************************************************************
EfoTestPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "EfoTestPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "Bond.h" 
#include "EfoControl.h"
#include "SerialComIO.h"
#include "MachineConfig.h"
#include "ScanKeyThread.h"
#include "User.h"
#include "servo.h"
#include "Busy.h"

// CEfoTestPage dialog
IMPLEMENT_DYNAMIC(CEfoTestPage, CPropertyPage)

CEfoTestPage::CEfoTestPage()
	: CPropertyPage(CEfoTestPage::IDD)
	, m_bStopFiring(false)
	, m_lCycleCount(0)
	, m_nCycleTime(60)	//v3.0.T472 edit
	, m_lTotalCycle(0)
	, m_sMsg(_T(""))
	, m_bDualFab(FALSE)
    , m_nFailCount(0)
{
	m_bBusy=FALSE;
}

CEfoTestPage::~CEfoTestPage()
{
}

void CEfoTestPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_BURN_CYCLE, m_nCycleTime);
    DDX_Text(pDX, IDC_EDIT_BURN_TIMES, m_lTotalCycle);
    DDX_Text(pDX, IDC_TXT_BURN_COUNTS, m_lCycleCount);
    DDX_Text(pDX, IDC_EDIT_DEBUG_DISPLAY, m_sMsg);
    DDX_Check(pDX, IDC_CHECK_DUAL_FAB_ENABLE, m_bDualFab);
    DDX_Text(pDX, IDC_STATIC_FAILURE_TIEMS, m_nFailCount);
}

BEGIN_MESSAGE_MAP(CEfoTestPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START_BURN, OnStartBurn)
	ON_BN_CLICKED(IDC_BUTTON_STOP_BURN, OnStopBurn)
	ON_EN_SETFOCUS(IDC_EDIT_BURN_CYCLE, OnFocusBurnCycle)
	ON_EN_SETFOCUS(IDC_EDIT_BURN_TIMES, OnFocusBurnTimes)
	ON_BN_CLICKED(IDC_BUTTON_DRY_RUN_MODE, OnDryRunMode)
	ON_BN_CLICKED(IDC_BUTTON_TRIGG_MODE, OnTriggMode)
	ON_BN_CLICKED(IDC_BUTTON_GET_VERSION, OnGetVersion)
	ON_BN_CLICKED(IDC_BUTTON_GET_PROFILE, OnGetProfile)
	ON_BN_CLICKED(IDC_CHECK_DUAL_FAB_ENABLE, OnDualFabEnable)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)		//V1.4.W26 add
END_MESSAGE_MAP()

// CEfoTestPage message handlers
HBRUSH CEfoTestPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CEfoTestPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_EFO_BURN_IN,theMsg.GetMsg(MSG_ETORCH_BURN_IN));
	SetDlgItemTextW(IDC_STATIC_BURN_IN_CYCLE,theMsg.GetMsg(MSG_CYCLE_TIME));
	SetDlgItemTextW(IDC_STATIC_BURN_IN_TIMES,theMsg.GetMsg(MSG_TOTAL_CYCLE));
	SetDlgItemTextW(IDC_STATIC_FAIL_TIMES,theMsg.GetMsg(MSG_FAILURE));
	SetDlgItemTextW(IDC_BUTTON_START_BURN,theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_BUTTON_STOP_BURN,theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_STATIC_BURN_COUNTS,theMsg.GetMsg(MSG_CYCLE_NUM));

	if (theUser.GetUserLevel() < USER_ENGINEER)
	{
		GetDlgItem(IDC_STATIC_EFO_DEBUG)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DRY_RUN_MODE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TRIGG_MODE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GET_VERSION)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GET_PROFILE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEBUG_DISPLAY)->ShowWindow(FALSE);
	}
	
	m_nCycleTime = 60;	//v3.0.T472 edit
	m_lTotalCycle = 100000;

	BYTE bRevData[255];
	memset(bRevData, 0, 255);
	theSerialCommIO.Digital_IO_Rev(bRevData, 255);

	return CPropertyPage::OnInitDialog();
}

void CEfoTestPage::OnStartBurn()
{
	CBusy bs(&m_bBusy);		//V1.4.W26 add

	GetDlgItem(IDC_BUTTON_START_BURN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_BURN)->EnableWindow();
	UpdateData(TRUE);

    theScanKeyThread.StopThread(); 

	theServo.SetOutputIo(0, 0, 0);
	theServo.SetOutputIo(0, 1, 0);
	theServo.SetOutputIo(0, 2, 1);
	Sleep_x1ms(10);

    m_lCycleCount = 0;
    m_nFailCount = 0;
	short nFabState = 0;
	CHTimer timer;
	do
	{
		if (m_bDualFab)
		{
			nFabState = !nFabState;
			theServo.SetOutputIo(0, 3, nFabState);
		}

		//cycle time delay
		timer.StartTimer();
		do
		{
			theBond.ProcessKeyboard();
			ProcessMessage();
		} while (!m_bStopFiring && (timer.GetTime_x1ms() < m_nCycleTime));

		//fire efo
#if _SERVO_TYPE == 2
		theServo.TrigEFO();		//v3.1.T344 add
#else
		theServo.ResetBondProgress();
		theServo.RunBuffer(3);
		Sleep_x1ms(10);
		theBond.CheckBondProgress(7, false);
		Sleep_x1ms(10);
		theServo.StopBuffer(3);
#endif
		if (theEFO.IsSparkSuccess() == false) m_nFailCount++;	//v1.6.T338 add
		m_lCycleCount++;
		UpdateData(FALSE);

	} while (!m_bStopFiring && (m_lCycleCount < m_lTotalCycle));

    theScanKeyThread.BeginThread();
	m_bStopFiring = false;
	theServo.SetOutputIo(0, 3, 0);

	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_START_BURN)->EnableWindow();
	GetDlgItem(IDC_BUTTON_STOP_BURN)->EnableWindow(FALSE);
}

void CEfoTestPage::OnStopBurn()
{
	m_bStopFiring = true;
	UpdateData(FALSE);
}

void CEfoTestPage::OnFocusBurnCycle()
{
	GetDlgItem(IDC_STATIC_EFO_BURN_IN)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCycleTime, 60, 30, 500);	//v3.0.T472 edit
	UpdateData(FALSE);
}

void CEfoTestPage::OnFocusBurnTimes()
{
	GetDlgItem(IDC_STATIC_EFO_BURN_IN)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lTotalCycle, 1000, 10, 1000000);
	UpdateData(FALSE);
}

void CEfoTestPage::OnDryRunMode()
{
	m_sMsg.Empty();
	if (theEFO.SetBurnInMode(1) == -1)
		m_sMsg.AppendFormat(_T("Start Burn-In com error\r\n"));
	else
		m_sMsg.AppendFormat(_T("Start Burn-In\r\n"));
	UpdateData(FALSE);
}

void CEfoTestPage::OnTriggMode()
{
	m_sMsg.Empty();
	if (theEFO.SetBurnInMode(0) == -1)
		m_sMsg.AppendFormat(_T("Stop Burn-In com error\r\n"));
	else
		m_sMsg.AppendFormat(_T("Stop Burn-In\r\n"));
	UpdateData(FALSE);
}

void CEfoTestPage::OnGetVersion()
{
	m_sMsg.Empty();
	BYTE nVersion = theEFO.GetVersion();
	if (nVersion == 255)
		m_sMsg.AppendFormat(_T("Get Version com error\r\n"));
	else
		m_sMsg.AppendFormat(_T("EFO version: %d\r\n"), nVersion);
	UpdateData(FALSE);
}

void CEfoTestPage::OnGetProfile()
{
	m_sMsg.Empty();
	if (theEFO.RequestProfile() != FOK)
		m_sMsg.AppendFormat(_T("Get Profile com error\r\n"));
	else
	{
		m_sMsg.AppendFormat(_T("Get Profile [I,T]\r\n"));
		m_sMsg.AppendFormat(_T("FAB 1 = [%d, %d]\r\n"), theEFO.m_acRxBuf[0], theEFO.m_acRxBuf[1]);
		m_sMsg.AppendFormat(_T("FAB 2 = [%d, %d]\r\n"), theEFO.m_acRxBuf[2], theEFO.m_acRxBuf[3]);
	}
	UpdateData(FALSE);
}

void CEfoTestPage::OnDualFabEnable()
{
	UpdateData(TRUE);
}

LRESULT CEfoTestPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.4.W26 add
{
	return m_bBusy;
}
