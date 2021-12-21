// Loop3rdKinkPagePan.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "Loop3rdKinkPagePan.h"
#include "MenuConfig.h"
#include "KeyPadHandler.h"
#include "MenuConfig.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"

// CLoop3rdKinkPagePan 对话框

IMPLEMENT_DYNAMIC(CLoop3rdKinkPagePan, CPropertyPage)

CLoop3rdKinkPagePan::CLoop3rdKinkPagePan()
	: CPropertyPage(CLoop3rdKinkPagePan::IDD)
	, m_nBelleHT(0)
	, m_nBelleDist(0)
	, m_nBelleAngle(0)
	, m_iBelleAfxe(0)
{

}

CLoop3rdKinkPagePan::~CLoop3rdKinkPagePan()
{
}

void CLoop3rdKinkPagePan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BELLE_HT, m_nBelleHT);
	DDX_Text(pDX, IDC_EDIT_SPAN2_LENGTH2, m_nBelleDist);
	DDX_Text(pDX, IDC_EDIT_BELLE_ANGLE, m_nBelleAngle);
	DDX_CBIndex(pDX, IDC_COMBO_BELLE_AFXE, m_iBelleAfxe);
}


BEGIN_MESSAGE_MAP(CLoop3rdKinkPagePan, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_EDIT_BELLE_HT, &CLoop3rdKinkPagePan::OnEnSetfocusEditBelleHt)
	ON_EN_SETFOCUS(IDC_EDIT_BELLE_DIST, &CLoop3rdKinkPagePan::OnEnSetfocusEditBelleDist)
	ON_EN_SETFOCUS(IDC_EDIT_BELLE_ANGLE, &CLoop3rdKinkPagePan::OnEnSetfocusEditBelleAngle)
	ON_CBN_SELCHANGE(IDC_COMBO_BELLE_AFXE, &CLoop3rdKinkPagePan::OnCbnSelchangeComboBelleAfxe)
END_MESSAGE_MAP()


// CLoop3rdKinkPagePan 消息处理程序

HBRUSH CLoop3rdKinkPagePan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLoop3rdKinkPagePan::OnDestroy()
{
	CPropertyPage::OnDestroy();	
}

BOOL CLoop3rdKinkPagePan::OnSetActive()
{
	UpdateMenuVariable();

	return CPropertyPage::OnSetActive();
}

BOOL CLoop3rdKinkPagePan::OnInitDialog()
{
	theApp.SetDlgFont(this);	


	return CPropertyPage::OnInitDialog();
}



void CLoop3rdKinkPagePan::UpdateMenuVariable(void)
{
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	m_nBelleHT    = stLoopAcsModePara.nBelleHt;
	m_nBelleDist  = stLoopAcsModePara.nBelleDist;
	m_nBelleAngle = stLoopAcsModePara.nBelleAngle;
	m_iBelleAfxe  = stLoopAcsModePara.iBelleAfterKink2; 

	int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_BELLE_HT);//1
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_BELLE_HT)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_BELLE_HT_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_BELLE_DIST);//2
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_BELLE_DIST)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_BELLE_DIST_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_BELLE_ANGLE);//3
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_BELLE_ANGLE)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_BELLE_ANGLE_DEG)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_BELLE_AFXE);//4
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_BELLE_AFXE)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

     UpdateData(FALSE);
}

void CLoop3rdKinkPagePan::SaveParameter(void)
{
	UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	stLoopAcsModePara.nBelleHt         = m_nBelleHT;
	stLoopAcsModePara.nBelleDist       = m_nBelleDist;
	stLoopAcsModePara.nBelleAngle      = m_nBelleAngle;
	stLoopAcsModePara.iBelleAfterKink2 = m_iBelleAfxe;

	theLoopPara.SetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	(GetParent()->GetParent())->Invalidate();
}



void CLoop3rdKinkPagePan::OnEnSetfocusEditBelleHt()
{
	GetDlgItem(IDC_STATIC_BELLE_HT)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBelleHT, 150, 0, 300);

	UpdateData(false);

	SaveParameter();
}

void CLoop3rdKinkPagePan::OnEnSetfocusEditBelleDist()
{
	GetDlgItem(IDC_STATIC_BELLE_DIST)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBelleDist, 150, 0, 300);

	UpdateData(false);

	SaveParameter();

}

void CLoop3rdKinkPagePan::OnEnSetfocusEditBelleAngle()
{
	GetDlgItem(IDC_STATIC_BELLE_ANGLE)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBelleAngle, 30, 0,60);

	UpdateData(false);

	SaveParameter();
}

void CLoop3rdKinkPagePan::OnCbnSelchangeComboBelleAfxe()
{
	UpdateData(true);

	SaveParameter();

	UpdateData(false);
}
