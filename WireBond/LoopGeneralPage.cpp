// LoopGeneralPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
#include "LoopGeneralPage.h"

// CLoopGeneralPage dialog
IMPLEMENT_DYNAMIC(CLoopGeneralPage, CPropertyPage)

CLoopGeneralPage::CLoopGeneralPage()
	: CPropertyPage(CLoopGeneralPage::IDD)
    , m_nLoopHeight(0)
    , m_nLoopHtCorrect(0)
	, m_nLAngle(0)
{
}

CLoopGeneralPage::~CLoopGeneralPage()
{
}

void CLoopGeneralPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_LOOP_HT, m_nLoopHeight);
    DDX_Text(pDX, IDC_EDIT_LOOP_HT_CORRECT, m_nLoopHtCorrect);
    DDX_Text(pDX, IDC_EDIT_L_ANGLE, m_nLAngle);
}

BEGIN_MESSAGE_MAP(CLoopGeneralPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_LOOP_HT, OnFocusLoopHt)
    ON_EN_SETFOCUS(IDC_EDIT_LOOP_HT_CORRECT, OnFocusLoopHtCorrect)
    ON_EN_SETFOCUS(IDC_EDIT_L_ANGLE, OnFocusLAngle)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLoopGeneralPage message handlers
HBRUSH CLoopGeneralPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CLoopGeneralPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_STATIC_LOOP_HT, theMsg.GetMsg(MSG_LOOP_HEIGHT));
    SetDlgItemTextW(IDC_STATIC_LOOP_HT_CORRECT, theMsg.GetMsg(MSG_LOOP_HT_CORRECT));
    SetDlgItemTextW(IDC_STATIC_L_ANGLE, theMsg.GetMsg(MSG_L_ANGLE));

    int iRowTop = PARA_COLUMN1_TOP;
	GetDlgItem(IDC_STATIC_LOOP_HT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_LOOP_HT)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	GetDlgItem(IDC_STATIC_LOOP_HT_CORRECT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_LOOP_HT_CORRECT)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	GetDlgItem(IDC_STATIC_L_ANGLE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_L_ANGLE)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	UpdateMenuVariable();

    return CPropertyPage::OnInitDialog();
}

// Update loop menu variable
void CLoopGeneralPage::UpdateMenuVariable(void)
{
    short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    m_nLoopHeight    = stLoopPara.stLoopBase.nLoopHt;
    m_nLoopHtCorrect = stLoopPara.stLoopBase.nLoopHtCorrect;
    m_nLAngle        = stLoopPara.stLoopBase.nLAngle;

    UpdateData(FALSE);
}

void CLoopGeneralPage::SaveParameter()	//V1.1.W246 add
{
    UpdateData();

    short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    stLoopPara.stLoopBase.nLoopHt        = m_nLoopHeight;
    stLoopPara.stLoopBase.nLoopHtCorrect = m_nLoopHtCorrect;
	stLoopPara.stLoopBase.nLAngle        = m_nLAngle;
    theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);

    (GetParent()->GetParent())->Invalidate();
}

void CLoopGeneralPage::OnDestroy()
{
	SaveParameter();
    CPropertyPage::OnDestroy();
}

void CLoopGeneralPage::OnFocusLoopHt()
{
    GetDlgItem(IDC_STATIC_LOOP_HT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nLoopHeight);
    UpdateData(false);
    SaveParameter();
}

void CLoopGeneralPage::OnFocusLoopHtCorrect()
{
	GetDlgItem(IDC_STATIC_LOOP_HT)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nLoopHtCorrect, 0, -200, 200);	//v3.1.T365 delete
	theKeyPad.GetUserKeyInData(&m_nLoopHtCorrect, 0, -200, 400);	//v3.1.T365 add
	UpdateData(false);
	SaveParameter();
}

BOOL CLoopGeneralPage::OnSetActive()
{
    UpdateMenuVariable();
    return CPropertyPage::OnSetActive();
}

void CLoopGeneralPage::OnFocusLAngle()
{
    GetDlgItem(IDC_STATIC_L_ANGLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nLAngle);
    UpdateData(false);
    SaveParameter();
}
