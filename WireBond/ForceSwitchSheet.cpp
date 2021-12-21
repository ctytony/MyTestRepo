// ForceSwitchSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "ForceSwitchSheet.h"
#include "MenuConfig.h"
#include "ForceSwitch.h"

// CForceSwitchSheet

IMPLEMENT_DYNAMIC(CForceSwitchSheet, CPropertySheet)

CForceSwitchSheet::CForceSwitchSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddForceSwitchPages();
}

CForceSwitchSheet::CForceSwitchSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddForceSwitchPages();
}

CForceSwitchSheet::~CForceSwitchSheet()
{
}


BEGIN_MESSAGE_MAP(CForceSwitchSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)
END_MESSAGE_MAP()


// CForceSwitchSheet 消息处理程序

void CForceSwitchSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();

	delete this;
}

BOOL CForceSwitchSheet::OnInitDialog()
{
	ModifyStyle(WS_CAPTION, 0);

	SetFont(theApp.GetFont()); //property sheet
	theApp.SetDlgFont(this);

	CPropertySheet::OnInitDialog();

	CTabCtrl* pCtrl = GetTabControl();

	TCITEM tcItem;
	WCHAR szChar[80];
	tcItem.mask = TCIF_TEXT;

	wcscpy_s(szChar, 80, _T("Force Compensate")); 
	tcItem.pszText = szChar;
	pCtrl->SetItem(0, &tcItem);

	wcscpy_s(szChar, 80, _T("Impact Mode")); 
	tcItem.pszText = szChar;
	pCtrl->SetItem(1, &tcItem);

	wcscpy_s(szChar, 80, _T("Force Switch")); 
	tcItem.pszText = szChar;
	pCtrl->SetItem(2, &tcItem);

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);
   
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

	return false;
}

HBRUSH CForceSwitchSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		return theApp.m_DlgBkBrush;
	}

	return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CForceSwitchSheet::AddForceSwitchPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	AddPage(&m_ForceCompensatePage);
	AddPage(&m_ImpactModePage);
	AddPage(&m_ForceSwitchPage);
}

void CForceSwitchSheet::OnDestroy()
{
	CPropertySheet::OnDestroy();
}

LRESULT CForceSwitchSheet::CloseDialog(WPARAM wParam, LPARAM lParam)
{
	return DestroyWindow()*DIALOG_CLOSED;
}
