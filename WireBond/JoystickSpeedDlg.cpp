// JoystickSpeedDlg.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "JoystickSpeedDlg.h"
#include "servo.h"

// CJoystickSpeedDlg dialog

IMPLEMENT_DYNAMIC(CJoystickSpeedDlg, CDialog)

CJoystickSpeedDlg::CJoystickSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJoystickSpeedDlg::IDD, pParent)
{

}

CJoystickSpeedDlg::~CJoystickSpeedDlg()
{
}

void CJoystickSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJoystickSpeedDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_INCREASE, &CJoystickSpeedDlg::OnBnClickedButtonIncrease)
    ON_BN_CLICKED(IDC_BUTTON_DECREASE, &CJoystickSpeedDlg::OnBnClickedButtonDecrease)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CJoystickSpeedDlg message handlers

BOOL CJoystickSpeedDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetWindowPos(NULL, 500,400, 370,240, SWP_NOZORDER);

	CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, 130, 160, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

	pWnd = GetDlgItem(IDC_BUTTON_INCREASE);
    pWnd->SetWindowPos(NULL, 72, 85, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INCREASE));

    pWnd = GetDlgItem(IDC_BUTTON_DECREASE);
    pWnd->SetWindowPos(NULL, 177, 85, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DECREASE));

    pWnd = GetDlgItem(IDC_STATIC_JOG_SPEED);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TBL_JOG_SPEED));

    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CJoystickSpeedDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}
void CJoystickSpeedDlg::OnBnClickedButtonIncrease()
{
    theServo.JogSpeedUp();
}

void CJoystickSpeedDlg::OnBnClickedButtonDecrease()
{
	theServo.JogSpeedDown();
}

LRESULT CJoystickSpeedDlg::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}
