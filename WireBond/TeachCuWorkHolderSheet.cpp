// TeachWorkHolderSheet.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "TeachCuWorkHolderSheet.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"

// CTeachCuWorkHolderSheet

IMPLEMENT_DYNAMIC(CTeachCuWorkHolderSheet, CPropertySheet)

CTeachCuWorkHolderSheet::CTeachCuWorkHolderSheet(UINT nIDCaption, int lPageMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	, m_iPageMode(0)
{

	m_iPageMode = lPageMode;

	AddTchWorkHoldPages();

}

CTeachCuWorkHolderSheet::CTeachCuWorkHolderSheet(LPCTSTR pszCaption, int lPageMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	m_iPageMode = lPageMode;

	AddTchWorkHoldPages();

}

CTeachCuWorkHolderSheet::~CTeachCuWorkHolderSheet()
{
}


BEGIN_MESSAGE_MAP(CTeachCuWorkHolderSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTeachCuWorkHolderSheet message handlers

BOOL CTeachCuWorkHolderSheet::OnInitDialog()
{

	((CWireBondDlg*)(this->GetParent()))->InhibitSystemButton();

    ModifyStyle(WS_CAPTION, 0);

    CWnd* pWnd = GetDlgItem(IDHELP);
    if (pWnd)
    {
        pWnd->ShowWindow(SW_HIDE);
    }

    pWnd = GetDlgItem(ID_WIZBACK);
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

    m_bModeless = FALSE;   
    m_nFlags |= WF_CONTINUEMODAL;



	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here

	 if (m_iPageMode == WH_CUTEACH)
    {

	    WCHAR   szChar[100];
        CTabCtrl* pCtrl = GetTabControl();
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        theMsgMgr.GetMsgString(MSG_MENU_ITEM_DEVICE_PARA, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(0, &tcItem);
        theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(1, &tcItem);
        theMsgMgr.GetMsgString(MSG_MENU_ITEM_INDEXER_SETUP_PAGE ,szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(2, &tcItem);
		 theMsgMgr.GetMsgString(MSG_MENU_ITEM_INDEX_SET_UP, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(3, &tcItem);
	 }
	 else
	 {
		  WCHAR   szChar[100];
        CTabCtrl* pCtrl = GetTabControl();
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_INDEX_SET_UP, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(0, &tcItem);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_INDEXER_SETUP_PAGE, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(1, &tcItem);
        theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(2, &tcItem);
        
	
	 }
	

    m_bModeless = TRUE;
    m_nFlags &= ~WF_CONTINUEMODAL;
  
    CRect Rect;
	(this->GetParent())->GetWindowRect(&Rect);
    
    this->SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);
	
    CPoint pt;
    CString szString;	
	
    //if (m_lRecipeOperateMode != RECIPE_SETUP)
    if (m_iPageMode == WH_CUTEACH) //@2
    {
        pt.x = TREE_BUTTON_LEFT; // - 95;
	    pt.y = TREE_BUTTON_TOP;

        pWnd = GetDlgItem(ID_WIZNEXT);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
        theMsgMgr.GetMsgString(MSG_MENU_ITEM_NEXT, szString);
        pWnd->SetWindowTextW(szString);

        pt.x += TREE_BUTTON_WIDTH;

        pWnd = GetDlgItem(IDCANCEL);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
        theMsgMgr.GetMsgString(MSG_DIALOG_BUTTON_CANCEL, szString);
        pWnd->SetWindowTextW(szString);

        pt.x += TREE_BUTTON_WIDTH;

        pWnd = GetDlgItem(ID_WIZFINISH);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
        theMsgMgr.GetMsgString(MSG_MENU_ITEM_FINISH, szString);
        pWnd->SetWindowTextW(szString);
        //pWnd->EnableWindow(0);
    }
    else
    {
	    pt.x = TREE_DONE_BUTTON_LEFT;
	    pt.y = TREE_DONE_BUTTON_TOP;

	    pWnd = GetDlgItem(IDOK);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_DONE_BUTTON_WIDTH, TREE_DONE_BUTTON_HEIGHT, SWP_NOZORDER);
	    pWnd->ShowWindow(SW_SHOW);
	    pWnd->EnableWindow(true);

        theMsgMgr.GetMsgString(MSG_DIALOG_BUTTON_DONE, szString);
        pWnd->SetWindowTextW(szString);

        pWnd = GetDlgItem(IDCANCEL);
	    if (pWnd)
	    {
            theMsgMgr.GetMsgString(MSG_DIALOG_BUTTON_CANCEL, szString);
            pWnd->SetWindowTextW(szString);
		    pWnd->ShowWindow(SW_HIDE);
	    }
    }


	return bResult;
}

int CTeachCuWorkHolderSheet::AddTchWorkHoldPages(void)
{

	
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button
   
    if (m_iPageMode == WH_CUTEACH)
    {
        AddPage(&m_CuELiteDeviceParaPage);
        AddPage(&m_CuELiteLFParaPage);
    }
    else
    {
        AddPage(&m_CuELiteDeviceParaPage);
        AddPage(&m_CuELiteLFParaPage);
    }

   
	return 0;
}



void CTeachCuWorkHolderSheet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	 CPropertySheet::PostNcDestroy();

     delete this;
}

HBRUSH CTeachCuWorkHolderSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CTeachCuWorkHolderSheet::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	((CWireBondDlg*)(this->GetParent()))->EnableSystemButton();

	((CWireBondDlg*)(this->GetParent()))->Invalidate();

	return CPropertySheet::DestroyWindow();
}


