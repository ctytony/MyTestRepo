// BondTipOffsetPage.cpp : implementation file
//

#include "stdafx.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "BondTipOffsetPage.h"


// CBondTipOffsetPage dialog

IMPLEMENT_DYNAMIC(CBondTipOffsetPage, CPropertyPage)

CBondTipOffsetPage::CBondTipOffsetPage()
	: CPropertyPage(CBondTipOffsetPage::IDD)
{

}

CBondTipOffsetPage::~CBondTipOffsetPage()
{
}

void CBondTipOffsetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBondTipOffsetPage, CPropertyPage)
END_MESSAGE_MAP()


// CBondTipOffsetPage message handlers
BOOL CBondTipOffsetPage::OnInitDialog()
{
    int iRowTop;
    CString szString;

    CFont* m_pTextFont = (((this->GetParent())->GetParent())->GetFont());
    CWnd* pChildWnd = GetWindow(GW_CHILD);

    while (pChildWnd)
    {
        pChildWnd->SetFont(m_pTextFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

    iRowTop = BTOS_COLUMN1_TOP;

    CWnd* pWnd = GetDlgItem(IDC_BTOS_X);
    pWnd->SetWindowPos(NULL, BTOS_COLUMN1_LEFT + 40, iRowTop, BTOS_COLUMN1_WIDTH - 40, BTOS_COLUMN1_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_BTOS_Y);
    pWnd->SetWindowPos(NULL, BTOS_COLUMN2_LEFT + 40, iRowTop, BTOS_COLUMN2_WIDTH - 40, BTOS_COLUMN2_HEIGHT, SWP_NOZORDER);

    iRowTop += BTOS_COLUMN1_HEIGHT + BTOS_LINE_SPACE;

    pWnd = GetDlgItem(IDC_CUR_BTOS);
    pWnd->SetWindowPos(NULL, BTOS_LABEL_LEFT, iRowTop, BTOS_LABEL_WIDTH, BTOS_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_BTOS_CURRENT, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_BTOS_CUR_X);
    pWnd->SetWindowPos(NULL, BTOS_COLUMN1_LEFT, iRowTop, BTOS_COLUMN1_WIDTH, BTOS_COLUMN1_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_BTOS_CUR_Y);
    pWnd->SetWindowPos(NULL, BTOS_COLUMN2_LEFT, iRowTop, BTOS_COLUMN2_WIDTH, BTOS_COLUMN2_HEIGHT, SWP_NOZORDER);

    iRowTop += BTOS_COLUMN1_HEIGHT + BTOS_LINE_SPACE;

    pWnd = GetDlgItem(IDC_NEW_BTOS);
    pWnd->SetWindowPos(NULL, BTOS_LABEL_LEFT, iRowTop, BTOS_LABEL_WIDTH, BTOS_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_BTOS_NEW, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_BTOS_NEW_X);
    pWnd->SetWindowPos(NULL, BTOS_COLUMN1_LEFT, iRowTop, BTOS_COLUMN1_WIDTH, BTOS_COLUMN1_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_BTOS_NEW_Y);
    pWnd->SetWindowPos(NULL, BTOS_COLUMN2_LEFT, iRowTop, BTOS_COLUMN2_WIDTH, BTOS_COLUMN2_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_SRCH_SPEED);
    pWnd->SetWindowPos(NULL, CTACT_PARA_LABEL1_LEFT, CTACT_PARA_TOP, CTACT_PARA_LABEL_WIDTH, CTACT_PARA_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_BTOS_SPEED);
    pWnd->SetWindowPos(NULL, CTACT_PARA_COLUMN1_LEFT, CTACT_PARA_TOP, CTACT_PARA_COLUMN_WIDTH, CTACT_PARA_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_SRCH_TOL);
    pWnd->SetWindowPos(NULL, CTACT_PARA_LABEL2_LEFT, CTACT_PARA_TOP, CTACT_PARA_LABEL_WIDTH, CTACT_PARA_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDC_BTOS_TOL);
    pWnd->SetWindowPos(NULL, CTACT_PARA_COLUMN2_LEFT, CTACT_PARA_TOP, CTACT_PARA_COLUMN_WIDTH, CTACT_PARA_HEIGHT, SWP_NOZORDER);
    
    pWnd = GetDlgItem(IDC_LIST_INFO);
    pWnd->SetWindowPos(NULL, CTACT_INFO_LEFT, CTACT_INFO_TOP, CTACT_INFO_WIDTH, CTACT_INFO_HEIGHT, SWP_NOZORDER);

    CPropertyPage::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
