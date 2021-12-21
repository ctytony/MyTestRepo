/***************************************************************
CMaterialHandleConfigSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MhsConfigSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"

// CMaterialHandleConfigSheet
IMPLEMENT_DYNAMIC(CMaterialHandleConfigSheet, CPropertySheet)

CMaterialHandleConfigSheet::CMaterialHandleConfigSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage),m_pPropPage(NULL)
{
    AddPropertyPages();
}

CMaterialHandleConfigSheet::CMaterialHandleConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),m_pPropPage(NULL)
{
    AddPropertyPages();
}

CMaterialHandleConfigSheet::~CMaterialHandleConfigSheet()
{
}

BEGIN_MESSAGE_MAP(CMaterialHandleConfigSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)
	ON_MESSAGE(WM_SAVESETTING,SaveSetting)	//V1.5.W117 add
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// Add all property pages
void CMaterialHandleConfigSheet::AddPropertyPages(void)
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
		//AddPage(&m_TopClampPage);
		AddPage(&m_TopSettingPage);
		//v3.0.T428 delete
        //AddPage(&m_TopStepperPage);
	    //AddPage(&m_TopSensorPage);
		break;
	}
	case MHS_FOXCONN:
	{
		AddPage(&m_TopSettingPage);
		break;
	}
	case MHS_R2R:
	{
		AddPage(&m_R2RParaSettingPage);
	    AddPage(&m_R2RSensorPage);
		break;
	}
	case MHS_CU_ELITE:
    {
		AddPage(&m_CuEliteParaSettingPage);
		AddPage(&m_CuEliteStepperPage);
		AddPage(&m_CuEliteSensorPage);
		break;
    }
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		if (theMcConfig.GetMhsType() == MHS_VLED_ELITE)
			m_pPropPage = new CVledElitePage();
		else
			m_pPropPage = new CSingleCapTrackPage();

		AddPage(m_pPropPage);
		AddPage(&m_MhsSettingPage);
	    AddPage(&theSensorViewPage);
        AddPage(&theSingleStepperPage);
		break;
	}
	}
}

BOOL CMaterialHandleConfigSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);
    SetFont(theApp.GetFont()); //property sheet
	theApp.SetDlgFont(this);	//V1.1.W139 add
    CPropertySheet::OnInitDialog();

    TCITEM tcItem;
    WCHAR szChar[80];
    tcItem.mask = TCIF_TEXT;
	tcItem.pszText = szChar;

    CTabCtrl *pCtrl = GetTabControl();

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED: case MHS_CU_ELITE:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		theMsg.GetMsg(MSG_SETTING_2, szChar);
		pCtrl->SetItem(0, &tcItem);

		theMsg.GetMsg(MSG_ELEVATOR, szChar);
		pCtrl->SetItem(1, &tcItem);

		theMsg.GetMsg(MSG_SENSOR_AND_CYLINDER, szChar);
		pCtrl->SetItem(2, &tcItem);
		break;
	}
	case MHS_FOXCONN:
	{
		theMsg.GetMsg(MSG_SETTING_2, szChar);
		pCtrl->SetItem(0, &tcItem);
		break;
	}
	case MHS_R2R:
	{
		theMsg.GetMsg(MSG_SETTING_2, szChar);
		pCtrl->SetItem(0, &tcItem);

		theMsg.GetMsg(MSG_SENSOR_AND_CYLINDER, szChar);
		pCtrl->SetItem(1, &tcItem);
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		theMsg.GetMsg(MSG_STEPPER, szChar);
		pCtrl->SetItem(0, &tcItem);
		
        theMsg.GetMsg(MSG_SETTING, szChar);
		pCtrl->SetItem(1, &tcItem);
		
        theMsg.GetMsg(MSG_SENSOR_AND_CYLINDER, szChar);
		pCtrl->SetItem(2, &tcItem);
		
        theMsg.GetMsg(MSG_STEPPER, szChar);
		pCtrl->SetItem(3, &tcItem);
		break;
	}
	}

    CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

    return FALSE;
}

HBRUSH CMaterialHandleConfigSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CMaterialHandleConfigSheet::CloseDialog(WPARAM wParam, LPARAM lParam)	//V1.1.W248 add
{
	return (m_TopStepperPage.SendMessage(WM_ISDIALOGBUSY) == FALSE&&DestroyWindow()) * DIALOG_CLOSED;
}

LRESULT CMaterialHandleConfigSheet::SaveSetting(WPARAM wParam, LPARAM lParam)	//V1.5.W117 add
{
	return m_TopSettingPage.PostMessageW(WM_SAVESETTING);
}

void CMaterialHandleConfigSheet::OnDestroy()
{
	CPropertySheet::OnDestroy();
	if (m_pPropPage) delete m_pPropPage;
}

void CMaterialHandleConfigSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();
    delete this;
}
