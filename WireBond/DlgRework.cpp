// DlgRework.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "DlgRework.h"
#include <memory>
#include "MsgDef.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"

// CDlgRework dialog
CDlgRework::CDlgRework(CWnd* pParent, CFont* pFont)
	: CDialog(CDlgRework::IDD, pParent)
{
	m_pFont = pFont;
	m_nReworkRow = m_nReworkCol = m_nReworkWire = 0;
}

CDlgRework::~CDlgRework()
{
}

BEGIN_MESSAGE_MAP(CDlgRework, CDialog)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_ROW, OnFocusRow)
	ON_EN_SETFOCUS(IDC_EDIT_COLUMN, OnFocusColumn)
	ON_EN_SETFOCUS(IDC_EDIT_WIRE_NUM, OnFocusWireNum)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnReset)
END_MESSAGE_MAP()

// CDlgRework message handlers
BOOL CDlgRework::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this, m_pFont);	//V1.1.W139 add

	SetDlgItemText(IDC_STATIC_ROW, theMsg.GetMsg(MSG_ROW));
	SetDlgItemText(IDC_STATIC_COLUMN, theMsg.GetMsg(MSG_COLUMN));
	SetDlgItemText(IDC_STATIC_WIRE_NUM, theMsg.GetMsg(MSG_WIRE_NUM));
	SetDlgItemText(IDC_BUTTON_RESET, theMsg.GetMsg(MSG_RESET));
	SetDlgItemText(IDOK, theMsg.GetMsg(MSG_DONE));
	SetDlgItemText(IDCANCEL, theMsg.GetMsg(MSG_CANCEL));

	SetDlgItemInt(IDC_EDIT_ROW, m_nReworkRow);
	SetDlgItemInt(IDC_EDIT_COLUMN, m_nReworkCol);
	SetDlgItemInt(IDC_EDIT_WIRE_NUM, m_nReworkWire);

	return FALSE;
}

HBRUSH CDlgRework::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDlgRework::OnFocusRow()
{
	GetDlgItem(IDC_STATIC_ROW)->SetFocus();
	if (theKeyPad.GetUserKeyInData(&m_nReworkRow, m_nReworkRow, 1, thePgm.GetDeviceRowNum()) == CANCEL_SELECTED) return;
	SetDlgItemInt(IDC_EDIT_ROW, m_nReworkRow);
}

void CDlgRework::OnFocusColumn()
{
	GetDlgItem(IDC_STATIC_ROW)->SetFocus();
	if (theKeyPad.GetUserKeyInData(&m_nReworkCol, m_nReworkCol, 1, thePgm.GetMatrixCol()) == CANCEL_SELECTED) return;
	SetDlgItemInt(IDC_EDIT_COLUMN, m_nReworkCol);
}

void CDlgRework::OnFocusWireNum()
{
	GetDlgItem(IDC_STATIC_ROW)->SetFocus();
	if (theKeyPad.GetUserKeyInData(&m_nReworkWire, m_nReworkWire, 1, thePgm.GetUnitTotalWire()) == CANCEL_SELECTED) return;
	SetDlgItemInt(IDC_EDIT_WIRE_NUM, m_nReworkWire);
}

void CDlgRework::OnReset()
{
	m_nReworkRow = m_nReworkCol = m_nReworkWire = 0;
	SetDlgItemInt(IDC_EDIT_ROW, m_nReworkRow);
	SetDlgItemInt(IDC_EDIT_COLUMN, m_nReworkCol);
	SetDlgItemInt(IDC_EDIT_WIRE_NUM, m_nReworkWire);
}
