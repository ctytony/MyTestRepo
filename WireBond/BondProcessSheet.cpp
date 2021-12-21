/***************************************************************
BondProcessSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "BondProcessSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "User.h"

// CBondProcessSheet
IMPLEMENT_DYNAMIC(CBondProcessSheet, CPropertySheet)

CBondProcessSheet::CBondProcessSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddBondProcessPages();
}

CBondProcessSheet::CBondProcessSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    AddBondProcessPages();
}

CBondProcessSheet::~CBondProcessSheet()
{
}

BEGIN_MESSAGE_MAP(CBondProcessSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.4.W25 add
END_MESSAGE_MAP()

// Add bond process pages
void CBondProcessSheet::AddBondProcessPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
    m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    AddPage(&m_BondSafetyPage);

    if (theUser.GetUserLevel() == USER_PROCESS)
    {
        //AddPage(&m_ParaLimitPage);
        AddPage(&m_BondConfigPage);
        AddPage(&m_BondFeaturePage);
    }
    else if (theUser.GetUserLevel() > USER_PROCESS)
    {
        AddPage(&m_UsgConfigPage);
        AddPage(&m_LoopConfigPage);
        AddPage(&m_BondConfigPage);
        AddPage(&m_BondFeaturePage);
		AddPage(&m_ParaLimitPage);
    }
}

BOOL CBondProcessSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    CPropertySheet::OnInitDialog();

    CTabCtrl* pCtrl = GetTabControl();

    TCITEM tcItem;
    WCHAR szChar[80];
    tcItem.mask = TCIF_TEXT;

    theMsg.GetMsg(MSG_BOND_SAFETY, szChar);    
    tcItem.pszText = szChar;
    pCtrl->SetItem(0, &tcItem);

    if (theUser.GetUserLevel() == USER_PROCESS)
    {
        theMsg.GetMsg(MSG_BOND, szChar);
        pCtrl->SetItem(1, &tcItem);

        theMsg.GetMsg(MSG_FEATURE, szChar);
        pCtrl->SetItem(2, &tcItem);
    }
    else if (theUser.GetUserLevel() > USER_PROCESS)
    {
        theMsg.GetMsg(MSG_USG_CONFIG, szChar);    
        pCtrl->SetItem(1, &tcItem);

        theMsg.GetMsg(MSG_LOOP, szChar);    
        pCtrl->SetItem(2, &tcItem);

        theMsg.GetMsg(MSG_BOND, szChar);
        pCtrl->SetItem(3, &tcItem);

        theMsg.GetMsg(MSG_FEATURE, szChar);
        pCtrl->SetItem(4, &tcItem);

		theMsg.GetMsg(MSG_PARA_RANGE, szChar);
		pCtrl->SetItem(5, &tcItem);
    }

    CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	return GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
}

HBRUSH CBondProcessSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CBondProcessSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.4.W25 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}

void CBondProcessSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();

	((CWireBondDlg*)(theApp.m_pMainWnd))->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.GetRcd()->bCapillaryAutoPlug);
	delete this;
}
