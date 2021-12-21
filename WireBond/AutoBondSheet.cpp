/***************************************************************
AutoBondSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "AutoBondSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"
#include "Bond.h"

// CAutoBondSheet

IMPLEMENT_DYNAMIC(CAutoBondSheet, CPropertySheet)

CAutoBondSheet::CAutoBondSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddAutoBondPages();
}

CAutoBondSheet::CAutoBondSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    AddAutoBondPages();
}

CAutoBondSheet::~CAutoBondSheet()
{
}

BEGIN_MESSAGE_MAP(CAutoBondSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)	//V1.1.W148 add
END_MESSAGE_MAP()

// Add pages for auto bond sheet

void CAutoBondSheet::AddAutoBondPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    AddPage(&m_AutoBondPage);
    AddPage(&m_BondStatisticPage);
    AddPage(&m_BondEditPage);
    AddPage(&m_BondParaPage);
    AddPage(&m_LoopParaGeneralPage);
}

BOOL CAutoBondSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    SetFont(theApp.GetFont());
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CPropertySheet::OnInitDialog();

    GetDlgItem(IDOK)->EnableWindow();
    GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

	TCHAR szChar[80];
    CTabCtrl* pCtrl = GetTabControl();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
	//bond
    theMsg.GetMsg(MSG_BOND, szChar);
    tcItem.pszText = szChar;
    pCtrl->SetItem(0, &tcItem);
	//statistic
    theMsg.GetMsg(MSG_STATISTICS, szChar);
    tcItem.pszText = szChar;
    pCtrl->SetItem(1, &tcItem);
	//backup lighting
    theMsg.GetMsg(MSG_BACKUP_LIGHTING, szChar);
    tcItem.pszText = szChar;
    pCtrl->SetItem(2, &tcItem);
	//bond parameter
    theMsg.GetMsg(MSG_BOND_PARA, szChar);
    tcItem.pszText = szChar;
    pCtrl->SetItem(3, &tcItem);
	//loop parameter
    theMsg.GetMsg(MSG_LOOP_PARA, szChar);
    tcItem.pszText = szChar;
    pCtrl->SetItem(4, &tcItem);

    CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    SetTimer(1, 200, NULL);

	m_TabCtrl.Install(this);	//V1.1.W169 add

    return FALSE;
}

HBRUSH CAutoBondSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAutoBondSheet::EnablePage(BYTE nPage, BYTE bEnable)	//V1.1.W169 add
{
	m_TabCtrl.EnableItem(nPage,bEnable);
}

void CAutoBondSheet::PostNcDestroy()
{
    CPropertySheet::PostNcDestroy();
    delete this;
}

void CAutoBondSheet::OnTimer(UINT_PTR nIDEvent)
{
	if (theMcConfig.GetKeyPadType() > 0)	//v2.0.T39 edit
	{
		if (theBond.GetBondMode() == BONDMODE_IDLE)
		{
			if (theKeyPad.IsKeyDown(KEY_BOND) == SENSOR_ON)
			{
				theKeyPad.SetKeyDown(KEY_BOND, false);
				m_AutoBondPage.StartAutoBond();
			}

			//v2.0.T39 add
			if ((theMcConfig.GetKeyPadType() == 3)
				&& (theKeyPad.IsKeyDown(KEY_OUT_MAG_HOME) == SENSOR_ON))	//v3.0.T480 edit
			{
				theKeyPad.SetKeyDown(KEY_OUT_MAG_HOME, false);	//v3.0.T480 edit
				m_AutoBondPage.StartSingleBond();
			}
		}

		if (theBond.GetBondMode() == BONDMODE_AUTO)
		{
			if (theKeyPad.IsKeyDown(KEY_STOP) == SENSOR_ON)
			{
				theKeyPad.SetKeyDown(KEY_STOP, false);
				m_AutoBondPage.StopAutoBond();
			}
		}
	}

	CPropertySheet::OnTimer(nIDEvent);
}

LRESULT CAutoBondSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W148 add
{
	return (GetDlgItem(IDOK)->IsWindowEnabled() == FALSE ? FALSE : DestroyWindow()) * DIALOG_CLOSED;
}
