// AfpToolSheet.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "AfpToolSheet.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"
// CAfpToolSheet

IMPLEMENT_DYNAMIC(CAfpToolSheet, CPropertySheet)

CAfpToolSheet::CAfpToolSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddAfpToolPages();
}

CAfpToolSheet::CAfpToolSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddAfpToolPages();
}

CAfpToolSheet::~CAfpToolSheet()
{
}

BEGIN_MESSAGE_MAP(CAfpToolSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.5.W67 add
END_MESSAGE_MAP()

// CAfpToolSheet message handlers
BOOL CAfpToolSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add
   
	CPropertySheet::OnInitDialog();

	WCHAR szChar[80];
    CTabCtrl* pCtrl = GetTabControl();

    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = szChar;

    theMsg.GetMsg(MSG_ENCODER, szChar);
    pCtrl->SetItem(0, &tcItem);
    theMsg.GetMsg(MSG_AUTO_TUNE, szChar);
    pCtrl->SetItem(1, &tcItem);
	theMsg.GetMsg(MSG_IO, szChar);
    pCtrl->SetItem(2, &tcItem);

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    return GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
}

void CAfpToolSheet::AddAfpToolPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	AddPage(&theServoAutoTunePage);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_VLED: case MHS_VLED_ELITE:
        AddPage(&theSensorViewPage);
		break;
	case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		AddPage(&theTopSensorViewPage);
		break;
	}
}

void CAfpToolSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();

	delete this;
}

HBRUSH CAfpToolSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CAfpToolSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.5.W67 add
{
	return (theServoAutoTunePage.SendMessage(WM_ISDIALOGBUSY)==FALSE&&DestroyWindow())*DIALOG_CLOSED;
}
