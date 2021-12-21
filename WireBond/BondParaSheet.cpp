/***************************************************************
BondParaSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "BondParaSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "User.h"

// CBondParaSheet
IMPLEMENT_DYNAMIC(CBondParaSheet, CPropertySheet)

CBondParaSheet::CBondParaSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddBondParaPages();
}

CBondParaSheet::CBondParaSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    AddBondParaPages();
}

CBondParaSheet::~CBondParaSheet()
{
}

BEGIN_MESSAGE_MAP(CBondParaSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W158 add
END_MESSAGE_MAP()

// Add bond parameter pages
void CBondParaSheet::AddBondParaPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    AddPage(&m_BondParaPage);
    AddPage(&m_AdvBondParaPage);
    
    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        AddPage(&m_ScrubParaPage);
        AddPage(&m_SparkParaPage);
    }
}

BOOL CBondParaSheet::OnInitDialog()
{
	ModifyStyle(WS_CAPTION, 0);

	// Change all Dialog and children Fonts
	SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

	CPropertySheet::OnInitDialog();

	WCHAR szChar[80];

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = szChar;

	CTabCtrl* pCtrl = GetTabControl();

	theMsg.GetMsg(MSG_BOND, szChar);    
	pCtrl->SetItem(0, &tcItem);

	theMsg.GetMsg(MSG_ADVANCE_BOND, szChar);    
	pCtrl->SetItem(1, &tcItem);

	if (theUser.GetUserLevel() > USER_OPERATOR)
	{
		theMsg.GetMsg(MSG_SCRUB, szChar);    
		pCtrl->SetItem(2, &tcItem);

		theMsg.GetMsg(MSG_SPARK, szChar);    
		pCtrl->SetItem(3, &tcItem);
	}

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	return FALSE;
}

HBRUSH CBondParaSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		return theApp.m_DlgBkBrush;
	}

	return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBondParaSheet::PostNcDestroy()	//V1.1.W159 add
{
	CPropertySheet::PostNcDestroy();

	delete this;
}

LRESULT CBondParaSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W159 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}
