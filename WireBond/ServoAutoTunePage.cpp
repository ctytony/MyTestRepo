/***************************************************************
ServoAutoTunePage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "ServoAutoTunePage.h"
#include "ServoTune.h"
#include "Debug.h"
#include "Busy.h"

// CServoAutoTunePage dialog
IMPLEMENT_DYNAMIC(CServoAutoTunePage, CPropertyPage)

CServoAutoTunePage::CServoAutoTunePage()
	: CPropertyPage(CServoAutoTunePage::IDD)
{
	m_bBusy=FALSE;	//V1.5.W67 add
}

CServoAutoTunePage::~CServoAutoTunePage()
{
}

void CServoAutoTunePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServoAutoTunePage, CPropertyPage)
	ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_TUNE, OnTune)
    ON_BN_CLICKED(IDC_BUTTON_XY_VERIFY, OnXyVerify)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnStop)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.5.W67 add
END_MESSAGE_MAP()

// CServoAutoTunePage message handlers
BOOL CServoAutoTunePage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_BUTTON_TUNE,theMsg.GetMsg(MSG_VERIFY));
    SetDlgItemTextW(IDC_BUTTON_STOP,theMsg.GetMsg(MSG_STOP));
    SetDlgItemTextW(IDC_BUTTON_XY_VERIFY,theMsg.GetMsg(MSG_VERIFY));

    return CPropertyPage::OnInitDialog();
}

HBRUSH CServoAutoTunePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CServoAutoTunePage::OnTune()
{
	CBusy bs(&m_bBusy);	//V1.5.W67 add

	GetDlgItem(IDC_BUTTON_TUNE)->EnableWindow(FALSE);

    theServoTune.InformStopTuning(false);
    theServoTune.VerifyBondHeadPerformance(theDebug.GetDebugMode());
}

void CServoAutoTunePage::OnXyVerify()
{
	CBusy bs(&m_bBusy);	//V1.5.W67 add

    theServoTune.VerifyTablePerformance();
}

void CServoAutoTunePage::OnStop()
{
	CBusy bs(&m_bBusy);	//V1.5.W67 add

    theServoTune.InformStopTuning(true);
    //Sleep_x1ms(200);

	GetDlgItem(IDC_BUTTON_TUNE)->EnableWindow(TRUE);
}

LRESULT CServoAutoTunePage::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.5.W67 add
{
	return m_bBusy;
}
