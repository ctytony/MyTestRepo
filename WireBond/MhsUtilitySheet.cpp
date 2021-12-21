// MhsUtilitySheet.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MhsUtilitySheet.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"

// CMhsUtilitySheet
IMPLEMENT_DYNAMIC(CMhsUtilitySheet, CPropertySheet)

CMhsUtilitySheet::CMhsUtilitySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPropertyPages();
}

CMhsUtilitySheet::CMhsUtilitySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPropertyPages();
}

CMhsUtilitySheet::~CMhsUtilitySheet()
{
}

BEGIN_MESSAGE_MAP(CMhsUtilitySheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)	//V1.4.W23 add
END_MESSAGE_MAP()

// Add all property pages
void CMhsUtilitySheet::AddPropertyPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		AddPage(&m_TopIndexerDebugPage);
		AddPage(&m_TopStepperPage);
		AddPage(&m_TopSensorPage);
		AddPage(&m_TopTestPage);
		break;
	}
	case MHS_FOXCONN:
	{
		AddPage(&m_TopIndexerDebugPage);
		break;
	}
	case MHS_R2R:
	{
		AddPage(&m_R2RIndexerDebugPage);
		AddPage(&m_R2RSensorPage);
		AddPage(&m_R2RTestPage);
		break;
	}
	case MHS_CU_ELITE:
	{
		AddPage(&theCuEliteIndexerDebugPage);
		AddPage(&theCuEliteStepperPage);
		AddPage(&theCuEliteSensorPage);
		AddPage(&theCuEliteTestPage);
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		AddPage(&theSingleTestingPage);
		AddPage(&theSensorViewPage);
		AddPage(&theSingleStepperPage);
		break;
	}
	}
}

// CMhsUtilitySheet message handlers
BOOL CMhsUtilitySheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);
    
    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet
	theApp.SetDlgFont(this);	//V1.1.W139 add
    CPropertySheet::OnInitDialog();

    TCITEM tcItem;
    WCHAR szChar[80];
	tcItem.pszText =szChar;
    tcItem.mask = TCIF_TEXT;

    CTabCtrl* pCtrl = GetTabControl();
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED: case MHS_CU_ELITE:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		theMsg.GetMsg(MSG_INDEXER, szChar);
		pCtrl->SetItem(0, &tcItem);

		theMsg.GetMsg(MSG_ELEVATOR, szChar);
		pCtrl->SetItem(1, &tcItem);

		theMsg.GetMsg(MSG_SENSOR_AND_CYLINDER, szChar);
		pCtrl->SetItem(2, &tcItem);

		theMsg.GetMsg(MSG_TEST, szChar);
		pCtrl->SetItem(3, &tcItem);
		break;
	}
	case MHS_FOXCONN:
	{
		theMsg.GetMsg(MSG_INDEXER, szChar);
		pCtrl->SetItem(0, &tcItem);
		break;
	}
	case MHS_R2R:
	{
		theMsg.GetMsg(MSG_INDEXER, szChar);
		pCtrl->SetItem(0, &tcItem);

		theMsg.GetMsg(MSG_SENSOR_AND_CYLINDER, szChar);
		pCtrl->SetItem(1, &tcItem);

		theMsg.GetMsg(MSG_TEST, szChar);
		pCtrl->SetItem(2, &tcItem);
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		theMsg.GetMsg(MSG_GENERAL, szChar);
		pCtrl->SetItem(0, &tcItem);

		theMsg.GetMsg(MSG_SENSOR_AND_CYLINDER, szChar);
		pCtrl->SetItem(1, &tcItem);

		theMsg.GetMsg(MSG_STEPPER, szChar);
		pCtrl->SetItem(2, &tcItem);
		break;
	}
	}
	
	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	return GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
}

HBRUSH CMhsUtilitySheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CMhsUtilitySheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.4.W23 add
{
	return ((m_TopIndexerDebugPage.SendMessage(WM_ISDIALOGBUSY) == FALSE)
			&& (m_TopTestPage.SendMessage(WM_ISDIALOGBUSY) == FALSE)
			&& (m_TopStepperPage.SendMessage(WM_ISDIALOGBUSY) == FALSE)
			&& DestroyWindow()) * DIALOG_CLOSED;
}

void CMhsUtilitySheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();
	delete this;
}
