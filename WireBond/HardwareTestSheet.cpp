// HardwareTestSheet.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "HardwareTestSheet.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"

// CHardwareTestSheet
IMPLEMENT_DYNAMIC(CHardwareTestSheet, CPropertySheet)

CHardwareTestSheet::CHardwareTestSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages();
}

CHardwareTestSheet::CHardwareTestSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
   AddPages();
}

CHardwareTestSheet::~CHardwareTestSheet()
{
}

BEGIN_MESSAGE_MAP(CHardwareTestSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.4.W26 add
END_MESSAGE_MAP()

// CHardwareTestSheet message handlers
void CHardwareTestSheet::AddPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	AddPage(&theEfoTestPage);
}

HBRUSH CHardwareTestSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		return theApp.m_DlgBkBrush;
	}

	return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CHardwareTestSheet::OnInitDialog()
{
	ModifyStyle(WS_CAPTION, 0);

	// Change all Dialog and children Fonts
	SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

	CPropertySheet::OnInitDialog();

	WCHAR szChar[80];
	CTabCtrl* pCtrl = GetTabControl();
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	theMsg.GetMsg(MSG_EFO_TEST, szChar);    
	tcItem.pszText = szChar;
	pCtrl->SetItem(0, &tcItem);

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	return GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
}

LRESULT CHardwareTestSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.4.W26 add
{
	return (theEfoTestPage.SendMessage(WM_ISDIALOGBUSY)==FALSE&&DestroyWindow())*DIALOG_CLOSED;
}

void CHardwareTestSheet::PostNcDestroy()	//V1.4.W26 add
{
	CPropertySheet::PostNcDestroy();

	delete this;
}
