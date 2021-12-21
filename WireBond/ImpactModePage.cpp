// LmpactModePage.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "ImpactModePage.h"
#include "KeyPadHandler.h"
#include "MenuConfig.h"
#include "ForceSwitch.h"

// CImpactModePage 对话框

IMPLEMENT_DYNAMIC(CImpactModePage, CPropertyPage)

CImpactModePage::CImpactModePage()
	: CPropertyPage(CImpactModePage::IDD)
	, m_nSpeed1(0)
	, m_nSpeed2(0)
	, m_nSpeed3(0)
	, m_nMode1(0)
	, m_nMode2(0)
	, m_nMode3(0)
{

}

CImpactModePage::~CImpactModePage()
{
}

void CImpactModePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPEED1, m_nSpeed1);
	DDX_Text(pDX, IDC_EDIT_SPEED2, m_nSpeed2);
	DDX_Text(pDX, IDC_EDIT_SPEED3, m_nSpeed3);
	DDX_Text(pDX, IDC_EDIT_MODE1, m_nMode1);
	DDX_Text(pDX, IDC_EDIT_MODE2, m_nMode2);
	DDX_Text(pDX, IDC_EDIT_MODE3, m_nMode3);
}


BEGIN_MESSAGE_MAP(CImpactModePage, CPropertyPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_SPEED1, &CImpactModePage::OnEnSetfocusEditSpeed1)
	ON_EN_SETFOCUS(IDC_EDIT_SPEED2, &CImpactModePage::OnEnSetfocusEditSpeed2)
	ON_EN_SETFOCUS(IDC_EDIT_SPEED3, &CImpactModePage::OnEnSetfocusEditSpeed3)
	ON_EN_SETFOCUS(IDC_EDIT_MODE1, &CImpactModePage::OnEnSetfocusEditMode1)
	ON_EN_SETFOCUS(IDC_EDIT_MODE2, &CImpactModePage::OnEnSetfocusEditMode2)
	ON_EN_SETFOCUS(IDC_EDIT_MODE3, &CImpactModePage::OnEnSetfocusEditMode3)
END_MESSAGE_MAP()


// CImpactModePage 消息处理程序

void CImpactModePage::OnDestroy()
{
	SaveParameter();
	CPropertyPage::OnDestroy();	
}

BOOL CImpactModePage::OnInitDialog()
{
	theApp.SetDlgFont(this);	
	UpdateMenuVariable();
	return CPropertyPage::OnInitDialog();
}

BOOL CImpactModePage::OnSetActive()
{
	UpdateMenuVariable();
	return CPropertyPage::OnSetActive();
}

HBRUSH CImpactModePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CImpactModePage::SaveParameter(void)
{
	UpdateData();

	FORCE_SWITCH_PARA* pstForceSwitch = theForceSwitch.GetRcd();
	pstForceSwitch->nMode1  = m_nMode1;
	pstForceSwitch->nMode2  = m_nMode2;
	pstForceSwitch->nMode3  = m_nMode3;
	pstForceSwitch->nSpeed1 = m_nSpeed1;
	pstForceSwitch->nSpeed2 = m_nSpeed2;
	pstForceSwitch->nSpeed3 = m_nSpeed3;

	(GetParent()->GetParent())->Invalidate();
}

void CImpactModePage::UpdateMenuVariable(void)
{
	FORCE_SWITCH_PARA* pstForceSwitch = theForceSwitch.GetRcd();
	m_nSpeed1 = pstForceSwitch->nSpeed1;
	m_nSpeed2 = pstForceSwitch->nSpeed2;
	m_nSpeed3 = pstForceSwitch->nSpeed3;
	m_nMode1  = pstForceSwitch->nMode1;
	m_nMode2  = pstForceSwitch->nMode2;
	m_nMode3  = pstForceSwitch->nMode3;

	int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SPEED1);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPEED1)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPEED2);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPEED2)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPEED3);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPEED3)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_MODE1);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_MODE1)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_MODE2);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_MODE2)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_MODE3);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_MODE3)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    UpdateData(FALSE);
}

void CImpactModePage::OnEnSetfocusEditSpeed1()
{
	GetDlgItem(IDC_STATIC_SPEED1)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSpeed1, 50, 10, 150);
	UpdateData(false);
	SaveParameter();
}

void CImpactModePage::OnEnSetfocusEditSpeed2()
{
	GetDlgItem(IDC_STATIC_SPEED2)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSpeed2, 50, 10, 150);
	UpdateData(false);
	SaveParameter();
}

void CImpactModePage::OnEnSetfocusEditSpeed3()
{
	GetDlgItem(IDC_STATIC_SPEED3)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSpeed3, 50, 10, 150);
	UpdateData(false);
	SaveParameter();
}

void CImpactModePage::OnEnSetfocusEditMode1()
{
	GetDlgItem(IDC_STATIC_MODE1)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nMode1, 3, 1, 9);
	UpdateData(false);
	SaveParameter();
}

void CImpactModePage::OnEnSetfocusEditMode2()
{
	GetDlgItem(IDC_STATIC_MODE2)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nMode2, 3, 1, 9);
	UpdateData(false);
	SaveParameter();
}

void CImpactModePage::OnEnSetfocusEditMode3()
{
	GetDlgItem(IDC_STATIC_MODE3)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nMode3, 1, 1, 255);
	UpdateData(false);
	SaveParameter();
}
