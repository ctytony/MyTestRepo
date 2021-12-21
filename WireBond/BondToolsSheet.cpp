// BondToolsSheet.cpp : implementation file

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "BondToolsSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"

// CBondToolsSheet
IMPLEMENT_DYNAMIC(CBondToolsSheet, CPropertySheet)

CBondToolsSheet::CBondToolsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddBondToolsPages();
}

CBondToolsSheet::CBondToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    AddBondToolsPages();
}

CBondToolsSheet::~CBondToolsSheet()
{
}

BEGIN_MESSAGE_MAP(CBondToolsSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.4.W22 add
END_MESSAGE_MAP()

// Add bond tool pages
void CBondToolsSheet::AddBondToolsPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    AddPage(&m_CapiSmoothPage);
}

BOOL CBondToolsSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    CPropertySheet::OnInitDialog();

    CTabCtrl* pCtrl = GetTabControl();

    WCHAR szChar[80];
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
    theMsg.GetMsg(MSG_CAP_SMOOTH, szChar);    
    tcItem.pszText = szChar;
    pCtrl->SetItem(0, &tcItem);

    CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	return GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
}

HBRUSH CBondToolsSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CBondToolsSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.4.W22 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}

void CBondToolsSheet::PostNcDestroy()		//V1.4.W22 add
{
	CPropertySheet::PostNcDestroy();

	delete this;
}
