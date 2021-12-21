// BondFromToUnitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Wire.h"
#include "BondFromToUnitDlg.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "Bond.h"

// CBondFromToUnitDlg dialog

IMPLEMENT_DYNAMIC(CBondFromToUnitDlg, CDialog)

CBondFromToUnitDlg::CBondFromToUnitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBondFromToUnitDlg::IDD, pParent)
	, m_nStartWire(0)
	, m_nEndWire(0)
{

}

CBondFromToUnitDlg::~CBondFromToUnitDlg()
{
}

void CBondFromToUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_START_ID, m_nStartWire);
	DDX_Text(pDX, IDC_EDIT_END_ID, m_nEndWire);
}

BEGIN_MESSAGE_MAP(CBondFromToUnitDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_START_ID, &CBondFromToUnitDlg::OnEnSetfocusEditStartId)
	ON_EN_SETFOCUS(IDC_EDIT_END_ID, &CBondFromToUnitDlg::OnEnSetfocusEditEndId)
END_MESSAGE_MAP()

// CBondFromToUnitDlg message handlers

BOOL CBondFromToUnitDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	GetDlgItem(IDC_STATIC_SELF_DEFINE_BOND)->SetWindowTextW(
					theMsg.GetMsg(MSG_INPUT_START_END_WIRE));
	GetDlgItem(IDC_STATIC_FROM_UNIT_NUM)->SetWindowTextW(
					theMsg.GetMsg(MSG_START_WIRE));
	GetDlgItem(IDC_STATIC_TO_UNIT_NUM)->SetWindowTextW(
					theMsg.GetMsg(MSG_END_WIRE));

	m_nStartWire = theBond.m_nRestartWire;
	m_nEndWire = theWire.GetTotalWireNum();

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBondFromToUnitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		 pDC->SetBkColor(RGB(0, 192, 20));
        return theApp.m_DlgBkBrushA;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CBondFromToUnitDlg::OnOK()
{
	UpdateData(TRUE);
	theBond.m_nRestartWire = m_nStartWire;
	theBond.m_nEndWire = m_nEndWire;
	DestroyWindow();
}

void CBondFromToUnitDlg::OnEnSetfocusEditStartId()
{
	CWnd* pWnd = GetDlgItem(IDOK);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nStartWire,
								theBond.m_nCurWire, 1,
								theWire.GetTotalWireNum());
	UpdateData(false);
}

void CBondFromToUnitDlg::OnEnSetfocusEditEndId()
{
	CWnd* pWnd = GetDlgItem(IDOK);
	pWnd->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nEndWire,
								theWire.GetTotalWireNum(), 1,
								theWire.GetTotalWireNum());
	UpdateData(false);
}

void CBondFromToUnitDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}
