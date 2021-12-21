// VisionParaSheet.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "VisionParaSheet.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "User.h"

// CVisionParaSheet
IMPLEMENT_DYNAMIC(CVisionParaSheet, CPropertySheet)

CVisionParaSheet::CVisionParaSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddBondParaPages();
}

CVisionParaSheet::CVisionParaSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    AddBondParaPages();
}

CVisionParaSheet::~CVisionParaSheet()
{
}

BEGIN_MESSAGE_MAP(CVisionParaSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)
END_MESSAGE_MAP()

// CVisionParaSheet message handlers
BOOL CVisionParaSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    BOOL bResult = CPropertySheet::OnInitDialog();

    WCHAR szChar[80];
    CTabCtrl* pCtrl = GetTabControl();

    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = szChar;

    theMsg.GetMsg(MSG_VISION, szChar);
    pCtrl->SetItem(0, &tcItem);
    theMsg.GetMsg(MSG_ADVANCED_VISION, szChar);
    pCtrl->SetItem(1, &tcItem);

	if (theUser.GetUserLevel() > USER_PROCESS)
	{
		theMsg.GetMsg(MSG_LIGHT_PARA_2, szChar);
		pCtrl->SetItem(2, &tcItem);
	}

    CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

    return bResult;
}

HBRUSH CVisionParaSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CVisionParaSheet::AddBondParaPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    AddPage(&m_VisionCfgPage);
    AddPage(&m_AdvVisionCfgPage);
	if (theUser.GetUserLevel() > USER_PROCESS) 
	{
        AddPage(&m_LightCfgPage);
	}
}

LRESULT CVisionParaSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W259 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}

void CVisionParaSheet::PostNcDestroy()		//V1.1.W259 add
{
	CPropertySheet::PostNcDestroy();

	delete this;
}
