// TeachWorkHolderSheet.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "CuEliteWorkHolderSetupPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"

// CCuEliteWorkHolderSetupSheet

IMPLEMENT_DYNAMIC(CCuEliteWorkHolderSetupSheet, CPropertySheet)

CCuEliteWorkHolderSetupSheet::CCuEliteWorkHolderSetupSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

	AddTchWorkHoldPages();

}

CCuEliteWorkHolderSetupSheet::CCuEliteWorkHolderSetupSheet(LPCTSTR pszCaption,  CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{


	AddTchWorkHoldPages();

}

CCuEliteWorkHolderSetupSheet::~CCuEliteWorkHolderSetupSheet()
{
}


BEGIN_MESSAGE_MAP(CCuEliteWorkHolderSetupSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCuEliteWorkHolderSetupSheet message handlers

BOOL CCuEliteWorkHolderSetupSheet::OnInitDialog()
{
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
    SetFont(theApp.m_pCurrentFont); //property sheet

    CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        //all children including property pages
        //providing all pages were activated in OnInitDialog (MUST!)
        pChildWnd->SetFont(theApp.m_pCurrentFont); 
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

     BOOL bResult = CPropertySheet::OnInitDialog();
    // TODO:  Add your specialized code here
	WCHAR   szChar[100];
    CTabCtrl* pCtrl = GetTabControl();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;

	theMsgMgr.GetMsgString(MSG_MENU_ITEM_WH_SET_UP, szChar);    
    tcItem.pszText = szChar;
    pCtrl->SetItem(0, &tcItem);

    theMsgMgr.GetMsgString(MSG_MENU_ITEM_DEVICE_OFFSET, szChar);    
    tcItem.pszText = szChar;
    pCtrl->SetItem(1, &tcItem);
	//v2.18.5.w4 add{
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MACHINE_OFFSET, szChar);    
    tcItem.pszText = szChar;
    pCtrl->SetItem(2, &tcItem);
	//}
  
    CRect Rect;
	(this->GetParent())->GetWindowRect(&Rect);
   
    this->SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	CPoint pt;
	pt.x = TREE_DONE_BUTTON_LEFT;
	pt.y = TREE_DONE_BUTTON_TOP;

	pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_DONE_BUTTON_WIDTH, TREE_DONE_BUTTON_HEIGHT, SWP_NOZORDER);
	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(true);

    CString szString;
    theMsgMgr.GetMsgString(MSG_DIALOG_BUTTON_DONE, szString);
    pWnd->SetWindowTextW(szString);

	return bResult;
}

int CCuEliteWorkHolderSetupSheet::AddTchWorkHoldPages(void)
{
    HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;   // Lose the Help button

    AddPage(&m_CuEliteDeviceOffsetPage);

    AddPage(&m_CuEliteWhSetUpPage);
	//v2.18.5.w4 add{
    AddPage(&m_CuEliteMachineOffsetPage);
    //}
	return 0;
}



HBRUSH CCuEliteWorkHolderSetupSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	  if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }
    
    HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CCuEliteWorkHolderSetupSheet::OnDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	((CWireBondDlg*)(this->GetParent()))->EnableSystemButton();

	 CPropertySheet::OnDestroy();
}


void CCuEliteWorkHolderSetupSheet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	 CPropertySheet::PostNcDestroy();

    delete this;
}



