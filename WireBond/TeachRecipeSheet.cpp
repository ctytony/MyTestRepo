/***************************************************************
TeachRecipeSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "TeachRecipeSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "MachineConfig.h"
#include "..\Bond\Bond.h"

// CTeachRecipeSheet
IMPLEMENT_DYNAMIC(CTeachRecipeSheet, CPropertySheet)

CTeachRecipeSheet::CTeachRecipeSheet(UINT nIDCaption, int lRecipeMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CTeachRecipeSheet::CTeachRecipeSheet(LPCTSTR pszCaption, int lRecipeMode, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_lRecipeOperateMode = lRecipeMode;
    m_bPrAlignDone = false;
    m_pszCaption = pszCaption;
	m_pTchSequencePage = new CTchSequencePage(lRecipeMode == RECIPE_TEACH ? IDD_TCH_SEQ : IDD_EDIT_SEQ);	//V1.5.W100 add

    AddTeachRecipePages();
}

CTeachRecipeSheet::~CTeachRecipeSheet()
{
	delete m_pTchSequencePage;	//V1.5.W100 add
}

BEGIN_MESSAGE_MAP(CTeachRecipeSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)
END_MESSAGE_MAP()

void CTeachRecipeSheet::AddTeachRecipePages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	if (m_lRecipeOperateMode == RECIPE_TEACH)
	{
		AddPage(&m_TchAlignPointPage);
		AddPage(&m_TchVisionPatternPage);
		AddPage(&m_TchAdvWireLayoutPage);
		AddPage(m_pTchSequencePage);	//V1.5.W100 edit
		AddPage(&m_CtactSrchPage);
		AddPage(&m_DummyBondPage);
	}
	else if (m_lRecipeOperateMode == RECIPE_EDIT)
	{
		if (theMsg.GetMsg(MSG_ALIGNMENT) == m_pszCaption)
			AddPage(&m_TchAlignPointPage);
		else if (theMsg.GetMsg(MSG_PR_PATTERN) == m_pszCaption)
			AddPage(&m_TchVisionPatternPage);
		else if (theMsg.GetMsg(MSG_EDIT_WIRE) == m_pszCaption)
			AddPage(&m_TchAdvWireLayoutPage);
		else if (theMsg.GetMsg(MSG_BOND_SEQ) == m_pszCaption)
			AddPage(m_pTchSequencePage);	//V1.5.W100 edit
	}
	else
	{
		if (theMsg.GetMsg(MSG_CTACT_SRCH) == m_pszCaption)
			AddPage(&m_CtactSrchPage);
		else if (theMsg.GetMsg(MSG_BOND_TIP_OFFSET) == m_pszCaption)
			AddPage(&m_BondTipOffsetPage);
		else if (theMsg.GetMsg(MSG_FIRE_LEVEL) == m_pszCaption)
			AddPage(&m_FireLevelPage);
		else if (theMsg.GetMsg(MSG_DUMMY_BOND) == m_pszCaption)
			AddPage(&m_DummyBondPage);
	}
}

BOOL CTeachRecipeSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    SetFont(theApp.GetFont());

	theApp.SetDlgFont(this);	//V1.1.W139 add

    m_bModeless = FALSE;   
    m_nFlags |= WF_CONTINUEMODAL;

	if(m_psh.dwFlags&PSH_WIZARD)GetDlgItem(ID_WIZBACK)->ShowWindow(SW_HIDE);

    CPropertySheet::OnInitDialog();

    if ((m_lRecipeOperateMode == RECIPE_SETUP) || (m_lRecipeOperateMode == RECIPE_EDIT)) //@4
    {
        CTabCtrl* pCtrl = GetTabControl();
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        tcItem.pszText = (LPWSTR)m_pszCaption;
        pCtrl->SetItem(0, &tcItem);
    }

    m_bModeless = TRUE;
    m_nFlags &= ~WF_CONTINUEMODAL;
  
    SetWindowPos(NULL, TREE_DLG_LEFT, TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);
	
    CPoint pt;

    if (m_lRecipeOperateMode == RECIPE_TEACH)
    {
        pt.x = TREE_BUTTON_LEFT; // - 95;
	    pt.y = TREE_BUTTON_TOP;

        CWnd* pWnd = GetDlgItem(ID_WIZNEXT);
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
	    GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
        GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
    }

	theBond.SetTeaching(TRUE);	//V1.1.W276 add

    return TRUE;
}

BOOL CTeachRecipeSheet::DestroyWindow()
{
	((CWireBondDlg*)theApp.m_pMainWnd)->Invalidate();
	((CWireBondDlg*)theApp.m_pMainWnd)->SetDrawAreaEnable(NULL);

	theBond.SetTeaching(FALSE);	//V1.1.W276 add

    return CPropertySheet::DestroyWindow();
}

HBRUSH CTeachRecipeSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTeachRecipeSheet::PostNcDestroy()
{
    CPropertySheet::PostNcDestroy();

    delete this;
}

BYTE CTeachRecipeSheet::IsBusy()		//V1.1.W153 add
{
	if ((IsWindow(m_TchAlignPointPage.m_hWnd) == TRUE)
		&& (m_TchAlignPointPage.SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;
	if ((IsWindow(m_TchVisionPatternPage.m_hWnd) == TRUE)
		&& (m_TchVisionPatternPage.SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;
	if ((IsWindow(m_TchAdvWireLayoutPage.m_hWnd) == TRUE)
		&& (m_TchAdvWireLayoutPage.SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;
	if ((IsWindow(m_pTchSequencePage->m_hWnd) == TRUE)
		&& (m_pTchSequencePage->SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;	//V1.5.W100 edit
	if ((IsWindow(m_CtactSrchPage.m_hWnd) == TRUE)
		&& (m_CtactSrchPage.SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;
	if ((IsWindow(m_FireLevelPage.m_hWnd) == TRUE)
		&& (m_FireLevelPage.SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;
	if ((IsWindow(m_BondTipOffsetPage.m_hWnd) == TRUE)
		&& (m_BondTipOffsetPage.SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;
	if ((IsWindow(m_DummyBondPage.m_hWnd) == TRUE)
		&& (m_DummyBondPage.SendMessage(WM_ISDIALOGBUSY) == TRUE))
		return TRUE;

	return FALSE;
}

LRESULT CTeachRecipeSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W153 add
{
	return ((IsBusy() == FALSE) && DestroyWindow()) * DIALOG_CLOSED;
}
