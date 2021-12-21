// AdvLoopParaSheet.cpp : implementation file
//

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
#include "MachineConfig.h"
// CAdvLoopParaSheet
IMPLEMENT_DYNAMIC(CAdvLoopParaSheet, CPropertySheet)

CAdvLoopParaSheet::CAdvLoopParaSheet(UINT nIDCaption, int iLoopGroup, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
    , m_nCurrentGroup(iLoopGroup)
{
    AddLoopParaPages();
}

CAdvLoopParaSheet::CAdvLoopParaSheet(LPCTSTR pszCaption, int iLoopGroup, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
    , m_nCurrentGroup(iLoopGroup)
{
    AddLoopParaPages();
}

CAdvLoopParaSheet::~CAdvLoopParaSheet()
{
}

BEGIN_MESSAGE_MAP(CAdvLoopParaSheet, CPropertySheet)
    ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// Add loop parameter pages
void CAdvLoopParaSheet::AddLoopParaPages(void)
{
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;      // Lose the Help button

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[m_nCurrentGroup], &stLoopPara);

	if (theMcConfig.GetAcsBufferType() == 1)  //V1.5.175.P6 add
	{
		AddPage(&GeneralPagePan);  
		AddPage(&Kink1PagePan); 
	}
	else
	{
		AddPage(&GeneralPage);  
		AddPage(&Kink1Page);    
	}

	if (stLoopPara.stLoopBase.nLoopProfile >= LOOP_PROFILE_BGA2)
	{
		if (theMcConfig.GetAcsBufferType() == 1)
			AddPage(&Kink2PagePan);
		else
			AddPage(&Kink2Page);        
	}

	if (stLoopPara.stLoopBase.nLoopProfile >= LOOP_PROFILE_BGA3)
	{
		if (theMcConfig.GetAcsBufferType() == 1)
			AddPage(&Kink3PagePan);
		else
			AddPage(&Kink3Page);
	}

	if (stLoopPara.stLoopBase.nLoopProfile >= LOOP_PROFILE_BGA4)
		AddPage(&LastKinkPage);

	if (theMcConfig.GetAcsBufferType() == 1)  //V1.5.175.P6 add
	{
		AddPage(&LandingPagePan); 
		AddPage(&SpeedArcPage);
	}
	else
		AddPage(&LandingPage);
}

HBRUSH CAdvLoopParaSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CAdvLoopParaSheet::OnInitDialog()
{
    ModifyStyle(WS_CAPTION, 0);

    // Change all Dialog and children Fonts
    SetFont(theApp.GetFont()); //property sheet

	theApp.SetDlgFont(this);	//V1.1.W139 add

    CPropertySheet::OnInitDialog();

	//SetWindowPos(NULL, TREE_DLG_LEFT, 260, TREE_DLG_WIDTH, 506, SWP_NOZORDER);	//v3.1.L370 delete
	SetWindowPos(NULL, TREE_DLG_LEFT, 330, TREE_DLG_WIDTH, 506, SWP_NOZORDER);		//v3.1.L370 add

    short i = 0;
    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[m_nCurrentGroup], &stLoopPara);
    CTabCtrl* pCtrl = GetTabControl();

    TCITEM tcItem;
    WCHAR szChar[80];

    tcItem.pszText = szChar;
    tcItem.mask = TCIF_TEXT;
    
    theMsg.GetMsg(MSG_GENERAL, szChar);
    pCtrl->SetItem(i++, &tcItem);

    theMsg.GetMsg(MSG_1ST_KINK, szChar);
    pCtrl->SetItem(i++, &tcItem);

    if (stLoopPara.stLoopBase.nLoopProfile >= LOOP_PROFILE_BGA2)
    {
        theMsg.GetMsg(MSG_2ND_KINK, szChar);
        pCtrl->SetItem(i++, &tcItem);
    }

    if (stLoopPara.stLoopBase.nLoopProfile >= LOOP_PROFILE_BGA3)
    {
        theMsg.GetMsg(MSG_3RD_KINK, szChar);
        pCtrl->SetItem(i++, &tcItem);
    }

    if (stLoopPara.stLoopBase.nLoopProfile >= LOOP_PROFILE_BGA4)
    {
        theMsg.GetMsg(MSG_LAST_KINK, szChar);
        pCtrl->SetItem(i++, &tcItem);
    }

    theMsg.GetMsg(MSG_LANDING, szChar);    
    pCtrl->SetItem(i, &tcItem);

    return FALSE;
}

void CAdvLoopParaSheet::PostNcDestroy()
{
    CPropertySheet::PostNcDestroy();

    delete this;
}

// Get current loop group of menu display
short CAdvLoopParaSheet::GetLoopGroup(void)
{
    return m_nCurrentGroup;
}
