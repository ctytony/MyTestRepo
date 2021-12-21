// SwapWireDlg.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "SwapWireDlg.h"

// CSwapWireDlg dialog

IMPLEMENT_DYNAMIC(CSwapWireDlg, CDialog)

CSwapWireDlg::CSwapWireDlg(short nDefaultWire, CWnd* pParent /*=NULL*/)
	: CDialog(CSwapWireDlg::IDD, pParent)
    , m_nFirstWireNum(nDefaultWire)
    , m_nSecondWireNum(nDefaultWire)
{

}

CSwapWireDlg::~CSwapWireDlg()
{
}

void CSwapWireDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FIRST_WIRE, m_nFirstWireNum);
    DDX_Text(pDX, IDC_EDIT_SECOND_WIRE, m_nSecondWireNum);
}


BEGIN_MESSAGE_MAP(CSwapWireDlg, CDialog)
    ON_EN_SETFOCUS(IDC_EDIT_FIRST_WIRE, &CSwapWireDlg::OnEnSetfocusEditFirstWire)
    ON_EN_SETFOCUS(IDC_EDIT_SECOND_WIRE, &CSwapWireDlg::OnEnSetfocusEditSecondWire)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSwapWireDlg message handlers

void CSwapWireDlg::OnEnSetfocusEditFirstWire()
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nFirstWireNum);

    UpdateData(false);
}

void CSwapWireDlg::OnEnSetfocusEditSecondWire()
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nSecondWireNum);

    UpdateData(false);
}

BOOL CSwapWireDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SWAP_WIRE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SWAP_WIRE));

	pWnd = GetDlgItem(IDC_STATIC_FIRST_WIRE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FIRST_WIRE));

	pWnd = GetDlgItem(IDC_STATIC_SECOND_WIRE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SECOND_WIRE));

	pWnd = GetDlgItem(IDOK);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSwapWireDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    CDialog::OnOK();
}

HBRUSH CSwapWireDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		 pDC->SetBkColor(RGB(0, 192, 20));
        return theApp.m_DlgBkBrushA;
	}

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

// Get swap wire number
int CSwapWireDlg::GetSwapWireNumber(short* pn1stSwapWire, short* pn2ndSwapWire)
{
    *pn1stSwapWire = m_nFirstWireNum;
    *pn2ndSwapWire = m_nSecondWireNum;

    return 0;
}

void CSwapWireDlg::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialog::OnCancel();
}
