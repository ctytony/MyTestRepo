/***************************************************************
DiagnoFileTab.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "DiagnoFileTab.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MotAlgo_DLL.h"
#include "servo.h"
#include "Busy.h"

// CDiagnoFileTab dialog
IMPLEMENT_DYNAMIC(CDiagnoFileTab, CPropertyPage)

CDiagnoFileTab::CDiagnoFileTab()
	: CPropertyPage(CDiagnoFileTab::IDD)
	, m_bSaveControlPara(FALSE)
	, m_bLoadSpeedPara(FALSE)
	, m_bLoadControlPara(FALSE)
	, m_bTuneX(FALSE)
	, m_bTuneY(FALSE)
	, m_bTuneZ(FALSE)
{
	m_bBusy=FALSE;	//V1.4.W43 add
}

CDiagnoFileTab::~CDiagnoFileTab()
{
}

void CDiagnoFileTab::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bSaveControlPara);
	DDX_Check(pDX, IDC_CHECK2, m_bLoadSpeedPara);
	DDX_Check(pDX, IDC_CHECK4, m_bLoadControlPara);
	DDX_Check(pDX, IDC_CHECK_TUNE_X, m_bTuneX);
	DDX_Check(pDX, IDC_CHECK_TUNE_Y, m_bTuneY);
	DDX_Check(pDX, IDC_CHECK_TUNE_Z, m_bTuneZ);
}

BEGIN_MESSAGE_MAP(CDiagnoFileTab, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CONTROL, OnControl)
    ON_BN_CLICKED(IDC_BUTTON_AUTO_TUNE, OnAutoTune)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.4.W43 add
END_MESSAGE_MAP()

// CDiagnoFileTab message handlers
HBRUSH CDiagnoFileTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(RGB(109,120,127));
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CDiagnoFileTab::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	m_bTuneX=FALSE;
	m_bTuneY=FALSE;
	m_bTuneZ=FALSE;

	CString szStr;
	WORD wMajor,wMinor;

	theServo.GetAutoTuneVersion(&wMajor,&wMinor);
	szStr.Format(_T("Auto Tune Version: %d.%d"),wMajor,wMinor);
    SetDlgItemTextW(IDC_STATIC_VERSION, szStr);

    return CPropertyPage::OnInitDialog();
}

void CDiagnoFileTab::OnControl()
{
	CBusy bs(&m_bBusy);	//V1.4.W43 add

	UpdateData(true);

	if (m_bSaveControlPara == TRUE)
	{
		theServo.SaveAcsMemoryParamToFile();
	}

	if (m_bLoadSpeedPara == TRUE)
	{
		theServo.LoadSpeedParameterFromFile();
	}

	if (m_bLoadControlPara == TRUE)
	{
		theServo.LoadControlParameterFromFile();
	}
}

void CDiagnoFileTab::OnAutoTune()
{
	CBusy bs(&m_bBusy);	//V1.4.W43 add

	UpdateData(TRUE);

	UINT uiAxis = 0;
	if (m_bTuneX) uiAxis |= WB_SERVO_TUNING_BIT_TUNE_2POINTS_X;
	if (m_bTuneY) uiAxis |= WB_SERVO_TUNING_BIT_TUNE_2POINTS_Y;
	if (m_bTuneZ) uiAxis |= WB_SERVO_TUNING_BIT_TUNE_2POINTS_Z;

	theServo.AutoTuneControlParaThread(uiAxis);
}

LRESULT CDiagnoFileTab::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.4.W43 add
{
	return m_bBusy;
}
