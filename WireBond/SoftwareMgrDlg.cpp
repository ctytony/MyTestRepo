/***************************************************************
SoftwareMgrDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "SoftwareMgrDlg.h"
#include "VisionSystem.h"
#include "UsgControl.h"
#include "BsdControl.h"
#include "EfoControl.h"
#include "Bond.h"
#include "MachineConfig.h"
#include "Register.h" 	//using partner's library for machine registration
#include <fstream>
#include "User.h"
#include "servo.h"
#include <comutil.h>

// CSoftwareMgrDlg dialog
IMPLEMENT_DYNAMIC(CSoftwareMgrDlg, CDialog)

CSoftwareMgrDlg::CSoftwareMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftwareMgrDlg::IDD, pParent)
	, m_szUsgVersion(_T("xx"))
	, m_szBsdVersion(_T("xx"))
	, m_szServoVersion(_T("xx"))
	, m_szEfoVersion(_T("xx"))
{
}

CSoftwareMgrDlg::~CSoftwareMgrDlg()
{
}

void CSoftwareMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_USG_VER, m_szUsgVersion);
	DDX_Text(pDX, IDC_STATIC_BSD_VER, m_szBsdVersion);
	DDX_Text(pDX, IDC_STATIC_SERVO_VER, m_szServoVersion);
	DDX_Text(pDX, IDC_STATIC_EFO_VER, m_szEfoVersion);
}

BEGIN_MESSAGE_MAP(CSoftwareMgrDlg, CDialog)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W163 add
END_MESSAGE_MAP()

// CSoftwareMgrDlg message handlers
BOOL CSoftwareMgrDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	//USG
	//m_szUsgVersion.Format(_T("2.%d"), theUSG.GetVersion());		//v3.1.T579 delete
	m_szUsgVersion.Format(_T("v%.1f"), theUSG.GetVersion() / 10.0);	//v3.1.T579 add

	//BSD
	BYTE Ver = 1;
	theBSD.GetVersion(&Ver);
	m_szBsdVersion.Format(_T("v3.%d"), Ver);

	//EFO
	m_szEfoVersion.Format(_T("v%.1f"), theEFO.GetVersion() / 10.0);

	//servo
	m_szServoVersion.Format(_T("%s"), theServo.GetVersion());

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    SetDlgItemTextW(IDC_STATIC_VERSION, theMsg.GetMsg(MSG_SOFTWARE_VERSION));
    SetDlgItemTextW(IDC_STATIC_BUILD_TIME, theMsg.GetMsg(MSG_DATE_TIME));
    SetDlgItemTextW(IDC_STATIC_VISION_VERSION, theMsg.GetMsg(MSG_PR_VERSION));
    SetDlgItemTextW(IDC_STATIC_CURRENT_VERSION, theApp.GetSoftwareVersion());
    SetDlgItemTextW(IDC_STATIC_VISION_VER, theVisionSys.GetVersion());

	LPTSTR lpszAppFileName = new TCHAR[_MAX_PATH];
	GetModuleFileName(theApp.m_hInstance, lpszAppFileName, _MAX_PATH);

    CFileStatus status;
    if (CFile::GetStatus(lpszAppFileName, status))
        SetDlgItemTextW(IDC_STATIC_CUR_BUILD_TIME,status.m_mtime.Format("%Y/%m/%d %H:%M:%S"));

	delete lpszAppFileName;

	SetDlgItemTextW(IDC_STATIC_MACHINE_ID,_bstr_t(theMcConfig.GetMachineID()).GetBSTR());	//V1.4.W19 add

	if (theUser.GetUserLevel() > USER_OPERATOR)
    {
		if (!theMcConfig.IsBypassAll() && (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO))	//V1.4.W37 add
		{
			SetDlgItemTextW(IDC_STATIC_ONLOAD,
					theStepperIO.GetVersion(CANADDR_ONLOAD * CARD_AXIS_NUM));	//V1.5.W23 add
			SetDlgItemTextW(IDC_STATIC_PERIOD_VALIDITY,
					theStepperIO.GetVersion(CANADDR_INDEX * CARD_AXIS_NUM));	//V1.4.W37 add
			SetDlgItemTextW(IDC_STATIC_OFFLOAD,
					theStepperIO.GetVersion(CANADDR_OFFLOAD * CARD_AXIS_NUM));	//V1.5.W23 add
			SetDlgItemTextW(IDC_STATIC_IO,
					theStepperIO.GetVersion(CANADDR_IO * CARD_AXIS_NUM));		//V1.5.W81 add
		}
    }
    else
    {
		GetDlgItem(IDC_STATIC_ONLOAD_1)->ShowWindow(SW_HIDE);			//V1.5.W165 add
		GetDlgItem(IDC_STATIC_ONLOAD)->ShowWindow(SW_HIDE);				//V1.5.W165 add
		GetDlgItem(IDC_STATIC_PERIOD_OF_VALIDITY)->ShowWindow(SW_HIDE);	//V1.4.W19 add
		GetDlgItem(IDC_STATIC_PERIOD_VALIDITY)->ShowWindow(SW_HIDE);	//V1.4.W19 add
		GetDlgItem(IDC_STATIC_OFFLOAD_1)->ShowWindow(SW_HIDE);			//V1.5.W165 add
		GetDlgItem(IDC_STATIC_OFFLOAD)->ShowWindow(SW_HIDE);			//V1.5.W165 add
		GetDlgItem(IDC_STATIC_IDLE)->ShowWindow(SW_HIDE);				//V1.5.W165 add
		GetDlgItem(IDC_STATIC_IO)->ShowWindow(SW_HIDE);					//V1.5.W81 add
    }

	UpdateData(false);

    return CDialog::OnInitDialog();
}

HBRUSH CSoftwareMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CSoftwareMgrDlg::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W163 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}

void CSoftwareMgrDlg::PostNcDestroy()		//V1.1.W163 add
{
	CDialog::PostNcDestroy();
	delete this;
}
