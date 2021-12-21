// BondObjectDlg.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "BondObjectDlg.h"

// CBondObjectDlg dialog

IMPLEMENT_DYNAMIC(CBondObjectDlg, CDialog)

CBondObjectDlg::CBondObjectDlg(char cDisplayOption, CWnd* pParent /*=NULL*/)
: CDialog(CBondObjectDlg::IDD, pParent)
, m_iBondObject(2)
, m_cDisplayOption(cDisplayOption)
{

}

CBondObjectDlg::~CBondObjectDlg()
{
}

void CBondObjectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO_ALL, m_iBondObject);
}

BEGIN_MESSAGE_MAP(CBondObjectDlg, CDialog)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CBondObjectDlg message handlers

BOOL CBondObjectDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, 125, 200, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

	pWnd = GetDlgItem(IDC_STATIC_BOND_OBJECT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BOND_OBJECT));

    pWnd = GetDlgItem(IDC_RADIO_ALL);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_WIRE_AND_BALL));
    if (m_cDisplayOption == 0)
        pWnd->ShowWindow(SW_SHOWNORMAL);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_RADIO_BALL);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BALL_ONLY));

    pWnd = GetDlgItem(IDC_RADIO_WIRE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_WIRE_ONLY));

    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBondObjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
       pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrushA;
    }

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CBondObjectDlg::SetBondObject(int iBondObject)
{
    m_iBondObject = iBondObject;
}

int CBondObjectDlg::GetBondObject(void)
{
    return m_iBondObject;
}
