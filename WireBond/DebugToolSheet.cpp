// DebugToolSheet.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "DebugToolSheet.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "User.h"

// CDebugToolSheet
IMPLEMENT_DYNAMIC(CDebugToolSheet, CPropertySheet)

CDebugToolSheet::CDebugToolSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
   AddPages();
}

CDebugToolSheet::CDebugToolSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
   AddPages();
}

CDebugToolSheet::~CDebugToolSheet()
{
}

BEGIN_MESSAGE_MAP(CDebugToolSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)	//V1.4.W43 add
END_MESSAGE_MAP()

BOOL CDebugToolSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    CPropertySheet::OnInitDialog();

    CRect Rect;
    GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

    TCITEM tcItem;
    WCHAR szChar[80];

    CTabCtrl* pCtrl = GetTabControl();
    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = szChar;

	//v2.0.T68 delete
	//theMsg.GetMsg(MSG_MSG_FILE_MANAGE_TOOL, szChar);    
	//pCtrl->SetItem(0, &tcItem);

	if (theUser.GetUserLevel() > USER_SERVICE)
	{
		theMsg.GetMsg(MSG_MODULE_TEST, szChar);    
		//pCtrl->SetItem(1, &tcItem);	//v2.0.T68 delete
		pCtrl->SetItem(0, &tcItem);		//v2.0.T68 add
	}

	return FALSE;
}

HBRUSH CDebugToolSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDebugToolSheet::AddPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    //AddPage(&m_DiagnoFileTab);	//v2.0.T68 delete

    if (theUser.GetUserLevel() > USER_SERVICE)
	    AddPage(&m_MotionTimeDebugPage);
}

LRESULT CDebugToolSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.4.W43 add
{
	//v2.0.T68 delete
	//return ((m_DiagnoFileTab.SendMessage(WM_ISDIALOGBUSY) == FALSE)
	//		&& (m_MotionTimeDebugPage.SendMessage(WM_ISDIALOGBUSY) == FALSE)
	//		&& DestroyWindow()) * DIALOG_CLOSED;

	return ((m_MotionTimeDebugPage.SendMessage(WM_ISDIALOGBUSY) == FALSE)
			&& DestroyWindow()) * DIALOG_CLOSED;	//v2.0.T68 add
}

void CDebugToolSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();
	delete this;
}
