/***************************************************************
CBondHeadCalibSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "BondHeadCalibSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"

// CBondHeadCalibSheet
IMPLEMENT_DYNAMIC(CBondHeadCalibSheet, CPropertySheet)

CBondHeadCalibSheet::CBondHeadCalibSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hwndPage = NULL;	//V1.1.W145 add
}

CBondHeadCalibSheet::CBondHeadCalibSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_pszCaption = pszCaption;
    AddPropertyPages();
}

CBondHeadCalibSheet::~CBondHeadCalibSheet()
{
}

BEGIN_MESSAGE_MAP(CBondHeadCalibSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W145 add
END_MESSAGE_MAP()

// Add all property pages
void CBondHeadCalibSheet::AddPropertyPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	if (theMsg.GetMsg(MSG_CALI_POSITION) == m_pszCaption)
		AddPage(&m_PositionCalibPage);
	else if (theMsg.GetMsg(MSG_CALI_FORCE) == m_pszCaption)
		AddPage(&m_ForceCalibPage);
	else if (theMsg.GetMsg(MSG_WIRE_CLAMP) == m_pszCaption)
		AddPage(&m_WclCalibPage);

	m_hwndPage = GetPage(0)->m_hWnd;
}

BOOL CBondHeadCalibSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);
    SetFont(theApp.GetFont());
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CPropertySheet::OnInitDialog();

    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = (LPWSTR)m_pszCaption;
    GetTabControl()->SetItem(0, &tcItem);

    CRect Rect;
	GetParent()->GetWindowRect(&Rect);
   
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

    return FALSE;
}

HBRUSH CBondHeadCalibSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
		return theApp.m_DlgBkBrush;

	return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBondHeadCalibSheet::PostNcDestroy()		//V1.1.W145 add
{
	CPropertySheet::PostNcDestroy();
	delete this;
}

LRESULT CBondHeadCalibSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W145 add
{
	return (((m_hwndPage == NULL)
			|| ::SendMessage(m_hwndPage,WM_ISDIALOGBUSY,0,0) == FALSE)
			&& DestroyWindow()) * DIALOG_CLOSED;
}
