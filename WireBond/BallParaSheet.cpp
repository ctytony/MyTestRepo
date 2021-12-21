/***************************************************************
BallParaSheet.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "BallParaSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"

// CBallParaSheet
IMPLEMENT_DYNAMIC(CBallParaSheet, CPropertySheet)

CBallParaSheet::CBallParaSheet(UINT nIDCaption, char chBumpBallType, CWnd* pParentWnd, UINT iSelectPage) //@1
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
    , m_chBumpBallType(0)
{
    m_chBumpBallType = chBumpBallType; //@1
    AddBallParaPages();
}

CBallParaSheet::CBallParaSheet(LPCTSTR pszCaption, char chBumpBallType, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_chBumpBallType = chBumpBallType; //@1
    AddBallParaPages();
}

CBallParaSheet::~CBallParaSheet()
{
}

BEGIN_MESSAGE_MAP(CBallParaSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W244 add
END_MESSAGE_MAP()

// Add ball parameter pages
void CBallParaSheet::AddBallParaPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

    AddPage(&m_BallBondParaPage);
    AddPage(&m_BallShapeParaPage);
	AddPage(&m_BumpWireParaPage);
}

BOOL CBallParaSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    CPropertySheet::OnInitDialog();

    WCHAR   szChar[80];
    CTabCtrl* pCtrl = GetTabControl();
    TCITEM tcItem;
    tcItem.mask = TCIF_TEXT;
    tcItem.pszText = szChar;

    theMsg.GetMsg(MSG_BUMP_BALL_PARA, szChar);	//v3.1.T358 rename
    pCtrl->SetItem(0, &tcItem);

    theMsg.GetMsg(MSG_BALL_SHAPE, szChar);    
    pCtrl->SetItem(1, &tcItem);
    
    theMsg.GetMsg(MSG_BUMP_WIRE, szChar);
    pCtrl->SetItem(2, &tcItem);

    CRect Rect;
	GetParent()->GetWindowRect(&Rect);

    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

    return FALSE;
}

HBRUSH CBallParaSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

// Get current bump ball type
char CBallParaSheet::GetBumpBallType(void) //@1
{
    return m_chBumpBallType;
}

void CBallParaSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();

	delete this;
}

LRESULT CBallParaSheet::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W244 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}
