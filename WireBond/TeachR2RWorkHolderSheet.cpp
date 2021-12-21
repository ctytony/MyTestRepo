// TeachWorkHolderSheet.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "TeachR2RWorkHolderSheet.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"

// CTeachR2RWorkHolderSheet

IMPLEMENT_DYNAMIC(CTeachR2RWorkHolderSheet, CPropertySheet)

CTeachR2RWorkHolderSheet::CTeachR2RWorkHolderSheet(UINT nIDCaption, int lPageMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	, m_iPageMode(0)
{

	m_iPageMode = lPageMode;

	AddTchWorkHoldPages();

}

CTeachR2RWorkHolderSheet::CTeachR2RWorkHolderSheet(LPCTSTR pszCaption, int lPageMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	m_iPageMode = lPageMode;

	AddTchWorkHoldPages();

}

CTeachR2RWorkHolderSheet::~CTeachR2RWorkHolderSheet()
{
}


BEGIN_MESSAGE_MAP(CTeachR2RWorkHolderSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTeachR2RWorkHolderSheet message handlers

BOOL CTeachR2RWorkHolderSheet::OnInitDialog()
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
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    m_bModeless = FALSE;   
    m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = CPropertySheet::OnInitDialog();

	if (m_iPageMode == WH_TEACH)
    {
	    WCHAR szChar[100];
        CTabCtrl* pCtrl = GetTabControl();
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        theMsg.GetMsg(MSG_DEVICE, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(0, &tcItem);
        theMsg.GetMsg(MSG_INDEXER, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(1, &tcItem);
        theMsg.GetMsg(MSG_PR_INDEX_2 ,szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(2, &tcItem);

	 }
	 else
	 {
		WCHAR   szChar[100];
        CTabCtrl* pCtrl = GetTabControl();
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
		theMsg.GetMsg(MSG_PR_INDEX_2, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(0, &tcItem);
		theMsg.GetMsg(MSG_INDEXER, szChar);    
        tcItem.pszText = szChar;
        pCtrl->SetItem(1, &tcItem);

     }
	

    m_bModeless = TRUE;
    m_nFlags &= ~WF_CONTINUEMODAL;
  
    CRect Rect;
	(this->GetParent())->GetWindowRect(&Rect);
    
    this->SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);
	
    CPoint pt;
	
    //if (m_lRecipeOperateMode != RECIPE_SETUP)
    if (m_iPageMode == WH_TEACH)
    {
        pt.x = TREE_BUTTON_LEFT;
	    pt.y = TREE_BUTTON_TOP;

        pWnd = GetDlgItem(ID_WIZNEXT);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT));

        pt.x += TREE_BUTTON_WIDTH;

        pWnd = GetDlgItem(IDCANCEL);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CANCEL));

        pt.x += TREE_BUTTON_WIDTH;

        pWnd = GetDlgItem(ID_WIZFINISH);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FINISH));
    }
    else
    {
	    pt.x = TREE_DONE_BUTTON_LEFT;
	    pt.y = TREE_DONE_BUTTON_TOP;

	    pWnd = GetDlgItem(IDOK);
        pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_DONE_BUTTON_WIDTH, TREE_DONE_BUTTON_HEIGHT, SWP_NOZORDER);
	    pWnd->ShowWindow(SW_SHOW);
	    pWnd->EnableWindow(true);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

        pWnd = GetDlgItem(IDCANCEL);
	    if (pWnd)
	    {
            pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CANCEL));
		    pWnd->ShowWindow(SW_HIDE);
	    }
    }

	return bResult;
}

int CTeachR2RWorkHolderSheet::AddTchWorkHoldPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button
    if (m_iPageMode == WH_TEACH)
   {
        AddPage(&m_R2RDeviceage);;
	    AddPage(&m_R2RIndexerSetUpPage);
	    AddPage(&m_R2RTchPrIndexPage);

    }
    else
    {

	    AddPage(&m_R2RTchPrIndexPage);
	    AddPage(&m_R2RIndexerSetUpPage);
 
    } 

   
	return 0;
}



void CTeachR2RWorkHolderSheet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	 CPropertySheet::PostNcDestroy();

     delete this;
}

HBRUSH CTeachR2RWorkHolderSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CTeachR2RWorkHolderSheet::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	((CWireBondDlg*)(this->GetParent()))->EnableSystemButton();

	((CWireBondDlg*)(this->GetParent()))->Invalidate();

	return CPropertySheet::DestroyWindow();
}


