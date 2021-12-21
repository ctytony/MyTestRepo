// CtactSrchTestPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "CtactSrchTestPage.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "Program.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "StepperControl.h"
#include "Bond.h"
#include "MachineConfig.h"
#include "servo.h"
#include "Busy.h"

// CCtactSrchTestPage dialog
IMPLEMENT_DYNAMIC(CCtactSrchTestPage, CDialog)

CCtactSrchTestPage::CCtactSrchTestPage(CWnd* pParent /*=NULL*/)
	: CDialog(CCtactSrchTestPage::IDD, pParent)
	, m_nSrchSpeed(2)
	, m_nSrchTol(30)
	, m_szString(_T(""))
{
	m_bBusy = FALSE;	//V1.4.W39 add
}

CCtactSrchTestPage::~CCtactSrchTestPage()
{
}

void CCtactSrchTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_CTACT_SRCH_TEST_SPEED, m_nSrchSpeed);
	DDX_Text(pDX, IDC_CTACT_TOL, m_nSrchTol);
	DDX_Text(pDX, IDC_EDIT_INFO, m_szString);
}

BEGIN_MESSAGE_MAP(CCtactSrchTestPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CTACT, OnCtact)
	ON_EN_SETFOCUS(IDC_CTACT_TOL, OnFocusCtactTol)
	ON_BN_CLICKED(IDC_BUTTON_INDEX, OnIndex)
	ON_CBN_SELCHANGE(IDC_COMBO_CTACT_SRCH_TEST_SPEED, OnCtactSrchTestSpeed)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.4.W39 add
END_MESSAGE_MAP()

// CCtactSrchTestPage message handlers
BOOL CCtactSrchTestPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CRect rect;
	GetParent()->GetWindowRect(&rect);
    SetWindowPos(NULL, rect.left + TREE_DLG_LEFT, rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    SetDlgItemTextW(IDC_BUTTON_CTACT,theMsg.GetMsg(MSG_START));

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		SetDlgItemTextW(IDC_BUTTON_INDEX,theMsg.GetMsg(MSG_LF_INDEX));
		break;
	}
	case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		GetDlgItem(IDC_BUTTON_INDEX)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BUTTON_CTACT)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		CRect rc;
		GetDlgItem(IDC_BUTTON_INDEX)->GetWindowRect(&rc);
		ScreenToClient(&rc);
		rect.left = (rect.left + rc.left) / 2;
		rect.right = rect.left + rc.Width();
		GetDlgItem(IDC_BUTTON_CTACT)->MoveWindow(&rect);
		break;
	}
	}

    SetDlgItemTextW(IDC_STATIC_CTACT_PARA,theMsg.GetMsg(MSG_CTACT_PARA));
    SetDlgItemTextW(IDC_SRCH_SPEED,theMsg.GetMsg(MSG_SPEED));
    SetDlgItemTextW(IDC_SRCH_TOL,theMsg.GetMsg(MSG_THRESHOLD));

	return CDialog::OnInitDialog();
}

HBRUSH CCtactSrchTestPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCtactSrchTestPage::PostNcDestroy()
{
	CDialog::PostNcDestroy();
    delete this;
}

void CCtactSrchTestPage::OnCtact()
{
	CBusy bs(&m_bBusy);	//V1.4.W39 add

	TBL_POS tbPos;
	theServo.GetTablePosition(&tbPos);
	theServo.MoveTable2(theMcData.GetRcd()->stBondTipOs, false);	//v3.1.T197 edit
	Sleep_x1ms(200);

	long lCtactLvl;
	theBond.CtactSrch(m_nSrchSpeed + 1, m_nSrchTol, &lCtactLvl);
	theServo.MoveTable2(tbPos);	//v3.1.T197 edit

	m_szString.AppendFormat(_T("Contact Level %d\r\n"), lCtactLvl);
	UpdateData(false);
}

void CCtactSrchTestPage::OnFocusCtactTol()
{
	GetDlgItem(IDC_STATIC_CTACT_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchTol, 0, 0, 100);
    UpdateData(false);
}

void CCtactSrchTestPage::OnIndex()
{
	if (theMcConfig.GetMhsType() != MHS_VLED
		&& theMcConfig.GetMhsType() != MHS_VLED_ELITE) return;

	CBusy bs(&m_bBusy);	//V1.4.W39 add

    GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(false);

	short nCurUnit = theVledTrack.GetBondUnitNum();
	theMHS.SetAutoIndex(true);
	theMHS.SetDryRun(false);

	long lStartTime;
	if ((nCurUnit == 0) && (theVledTrack.GetIndexEvent() == TRACK_EVENT_IDLE))
	{
		theVledTrack.theOnloadTrackSensor.UpdateSensorState();
		theVledTrack.thePreBondSensor.UpdateSensorState();
		long lDist;
		theStepper.GetPos(SINGLE_CAP_INJECTOR, &lDist);

		if ((theVledTrack.theOnloadTrackSensor.GetSensorState() == SENSOR_OFF)
			&& (theVledTrack.thePreBondSensor.GetSensorState() == SENSOR_OFF)
			&& (lDist != 0))
		{
			theVledTrack.SearchHome();
			Sleep_x1ms(10);
			theVledTrack.CheckHomeState();
		}

		theVledTrack.ResetIndexEvent();

		if ((theVledTrack.theLeftClaw.GetClawTicker() == 0)
			&& (theVledTrack.theRightClaw.GetClawTicker() == 0))
		{
			theVledTrack.theLeftClaw.SetIndexEmpty(true);
			theVledTrack.SubmitIndexEvent(TRACK_EVENT_FIRST_INDEX);
		}

		lStartTime = GetCurrentTime();
		do
		{		  
			theVledTrack.ProcessIndexEvent();
			if (theVledTrack.GetIndexEvent() == TRACK_EVENT_IDLE) break;
			Sleep_x1ms(10);
		} while (GetCurrentTime() - lStartTime < 8000);

		theMHS.ShowError();
	}
	else
	{
		theMHS.SetRequestIndex(true);
		Sleep_x1ms(200);

		lStartTime = GetCurrentTime();
		while (theMHS.IsRequestIndex())
		{
			if (theMHS.ShowError() != FOK) break;
			if (::GetCurrentTime() - lStartTime > 15000) break;
			Sleep_x1ms(1);
		}
	}

    theMHS.SetAutoIndex(false);
    GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
}

void CCtactSrchTestPage::OnCtactSrchTestSpeed()
{
    UpdateData();
}

LRESULT CCtactSrchTestPage::CloseDialog(WPARAM wParam, LPARAM lParam)	//V1.4.W39 add
{
	return (m_bBusy == FALSE&&DestroyWindow()) * DIALOG_CLOSED;
}
