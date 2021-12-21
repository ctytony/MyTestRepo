// TeachWorkHolderSheet.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "TeachWorkHolderSheet.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"

// CTeachWorkHolderSheet
IMPLEMENT_DYNAMIC(CTeachWorkHolderSheet, CPropertySheet)

CTeachWorkHolderSheet::CTeachWorkHolderSheet(UINT nIDCaption, int lPageMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	, m_iPageMode(0)
{
	m_iPageMode = lPageMode;

	AddTchWorkHoldPages();
}

CTeachWorkHolderSheet::CTeachWorkHolderSheet(LPCTSTR pszCaption, int lPageMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_iPageMode = lPageMode;

	AddTchWorkHoldPages();
}

CTeachWorkHolderSheet::~CTeachWorkHolderSheet()
{
}

BEGIN_MESSAGE_MAP(CTeachWorkHolderSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W158 add
END_MESSAGE_MAP()

// CTeachWorkHolderSheet message handlers
BOOL CTeachWorkHolderSheet::OnInitDialog()
{
	ModifyStyle(WS_CAPTION, 0);

	if(m_psh.dwFlags&PSH_WIZARD)GetDlgItem(ID_WIZBACK)->ShowWindow(SW_HIDE);

	SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

	m_bModeless = FALSE;   
	m_nFlags |= WF_CONTINUEMODAL;

	CPropertySheet::OnInitDialog();

	WCHAR szChar[80];

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;   
	tcItem.pszText = szChar;

	CTabCtrl* pCtrl = GetTabControl();

	if (m_iPageMode == WH_TEACH)
	{
		theMsg.GetMsg(MSG_DEVICE, szChar);
		pCtrl->SetItem(0, &tcItem);
		theMsg.GetMsg(MSG_ELEVATOR, szChar); 
		pCtrl->SetItem(1, &tcItem);
		theMsg.GetMsg(MSG_INDEXER ,szChar);
		pCtrl->SetItem(2, &tcItem);
		 theMsg.GetMsg(MSG_PR_INDEX_2, szChar);
		pCtrl->SetItem(3, &tcItem);
	}
	else
	{
		theMsg.GetMsg(MSG_PR_INDEX_2, szChar);
		pCtrl->SetItem(0, &tcItem);
		theMsg.GetMsg(MSG_INDEXER, szChar);
		pCtrl->SetItem(1, &tcItem);
		theMsg.GetMsg(MSG_ELEVATOR, szChar);
		pCtrl->SetItem(2, &tcItem);
	}

	m_bModeless = TRUE;
	m_nFlags &= ~WF_CONTINUEMODAL;

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);

	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	CPoint pt;

	if (m_iPageMode == WH_TEACH) //@2
	{
		pt.x = TREE_BUTTON_LEFT; // - 95;
		pt.y = TREE_BUTTON_TOP;

		GetDlgItem(ID_WIZNEXT)->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(ID_WIZNEXT,theMsg.GetMsg(MSG_NEXT));

		pt.x += TREE_BUTTON_WIDTH;

		GetDlgItem(IDCANCEL)->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDCANCEL,theMsg.GetMsg(MSG_CANCEL));

		pt.x += TREE_BUTTON_WIDTH;

		GetDlgItem(ID_WIZFINISH)->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(ID_WIZFINISH,theMsg.GetMsg(MSG_FINISH));
	}
	else
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	}

	return FALSE;
}

void CTeachWorkHolderSheet::AddTchWorkHoldPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	if (m_iPageMode == WH_TEACH)
	{
		AddPage(&m_TopDeviceParaPage);
		AddPage(&theElevPartSetUpPage);
		AddPage(&theIndexerPartSetUpPage);
		AddPage(&m_TopTchPrIndexPage);
	}
	else
	{
		AddPage(&m_TopTchPrIndexPage);
		AddPage(&theIndexerPartSetUpPage);
		AddPage(&theElevPartSetUpPage);
	}
}

void CTeachWorkHolderSheet::PostNcDestroy()
{
	 CPropertySheet::PostNcDestroy();

	 delete this;
}

HBRUSH CTeachWorkHolderSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		return theApp.m_DlgBkBrush;
	}

	return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CTeachWorkHolderSheet::DestroyWindow()
{
	GetParent()->Invalidate();

	return CPropertySheet::DestroyWindow();
}

BYTE CTeachWorkHolderSheet::IsBusy()		//V1.1.W158 add
{
	if(IsWindow(m_TopDeviceParaPage.m_hWnd)==TRUE&&m_TopDeviceParaPage.SendMessage(WM_ISDIALOGBUSY)==TRUE)return TRUE;
	if(IsWindow(theElevPartSetUpPage.m_hWnd)==TRUE&&theElevPartSetUpPage.SendMessage(WM_ISDIALOGBUSY)==TRUE)return TRUE;
	if(IsWindow(theIndexerPartSetUpPage.m_hWnd)==TRUE&&theIndexerPartSetUpPage.SendMessage(WM_ISDIALOGBUSY)==TRUE)return TRUE;
	if(IsWindow(m_TopTchPrIndexPage.m_hWnd)==TRUE&&m_TopTchPrIndexPage.SendMessage(WM_ISDIALOGBUSY)==TRUE)return TRUE;

	return GetDlgItem(m_iPageMode == WH_TEACH ? ID_WIZFINISH : IDOK)->IsWindowEnabled()==FALSE;
}

LRESULT CTeachWorkHolderSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W158 add
{
	return (IsBusy()==FALSE&&DestroyWindow())*DIALOG_CLOSED;
}
