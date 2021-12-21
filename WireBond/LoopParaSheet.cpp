/***************************************************************
LoopParaSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "..\GkgLib\LoopPara\LoopPara.h"
#include "LoopParaSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "..\GkgLib\User\User.h"

// CLoopParaSheet

IMPLEMENT_DYNAMIC(CLoopParaSheet, CPropertySheet)

CLoopParaSheet::CLoopParaSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddLoopParaPages();
}

CLoopParaSheet::CLoopParaSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    AddLoopParaPages();
}

CLoopParaSheet::~CLoopParaSheet()
{
}


BEGIN_MESSAGE_MAP(CLoopParaSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// Add loop parameter pages to loop parameter sheet

int CLoopParaSheet::AddLoopParaPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    AddPage(&m_LoopParaGeneralPage);

    if (theUser.GetUserLevel() > USER_OPERATOR) //@1
    {
        AddPage(&m_LoopParaAdvancePage);
    }

    return 0;
}

BOOL CLoopParaSheet::OnInitDialog()
{
    // TODO:  Add your specialized code here

    theLoopParaMgr.CountProgramLoopMode();

    ((CWireBondDlg*)(this->GetParent()))->InhibitSystemButton();
    
    ModifyStyle(WS_CAPTION, 0);

    CWnd* pWnd = GetDlgItem(IDHELP);
	if (pWnd)
	{
		pWnd->ShowWindow(SW_HIDE);
	}

    pWnd = GetDlgItem(IDCANCEL);
	if (pWnd)
	{
		pWnd->ShowWindow(SW_HIDE);
	}

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    BOOL bResult = CPropertySheet::OnInitDialog();

    WCHAR   szChar[100];
    CTabCtrl* pCtrl = GetTabControl();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_GENERAL, szChar);    
    tcItem.pszText = szChar;
    pCtrl->SetItem(0, &tcItem);

    if (theUser.GetUserLevel() > USER_OPERATOR) //@1
    {
        theMsgMgr.GetMsgString(MSG_MENU_ITEM_ADVANCE_LOOP, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(1, &tcItem);
    }

    CRect Rect;
	(this->GetParent())->GetWindowRect(&Rect);

    this->SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	CPoint pt;
	pt.x = TREE_DONE_BUTTON_LEFT;
	pt.y = TREE_DONE_BUTTON_TOP;

	pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_DONE_BUTTON_WIDTH, TREE_DONE_BUTTON_HEIGHT, SWP_NOZORDER);
	
	CString szString;
    theMsgMgr.GetMsgString(MSG_DIALOG_BUTTON_DONE, szString);
    pWnd->SetWindowTextW(szString);

    return bResult;
}

HBRUSH CLoopParaSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(RGB(0, 128, 128));
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}
