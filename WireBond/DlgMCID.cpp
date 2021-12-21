// DlgMCID.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "DlgMCID.h"
#include "MsgDef.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineConfig.h"

// CDlgMCID dialog
CDlgMCID::CDlgMCID(CWnd* pParent)
	: CDialog(CDlgMCID::IDD, pParent)
{
}

CDlgMCID::~CDlgMCID()
{
}

BEGIN_MESSAGE_MAP(CDlgMCID, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT_MCID, OnFocusMcid)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDlgMCID message handlers
BOOL CDlgMCID::OnInitDialog()
{
	theApp.SetDlgFont(this);

	SetDlgItemText(IDC_STATIC_MACHINE_ID,theMsg.GetMsg(MSG_MACHINE_ID));
	SetDlgItemInt(IDC_EDIT_MCID,theMcConfig.GetMachineID());
	SetDlgItemText(IDOK,theMsg.GetMsg(MSG_DONE));

	return CDialog::OnInitDialog();
}

HBRUSH CDlgMCID::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDlgMCID::OnFocusMcid()
{
	GetDlgItem(IDC_STATIC_MACHINE_ID)->SetFocus();

	short nMachine=GetDlgItemInt(IDC_EDIT_MCID);
	if(theKeyPad.GetUserKeyInData(&nMachine,nMachine,1,30000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_MCID,nMachine);

	theMcConfig.SetMachineID(nMachine);

	PostMessage(WM_COMMAND,MAKELONG(IDOK,BN_CLICKED),(LPARAM)GetDlgItem(IDOK)->m_hWnd);	//V1.1.W266 add
}
