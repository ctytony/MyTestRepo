// Loop1stKinkPagePan.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "Loop1stKinkPagePan.h"
#include "KeyPadHandler.h"
#include "AdvLoopParaSheet.h"
#include "LoopPara.h"
#include "MenuConfig.h"
// CLoop1stKinkPagePan 对话框

IMPLEMENT_DYNAMIC(CLoop1stKinkPagePan, CPropertyPage)

CLoop1stKinkPagePan::CLoop1stKinkPagePan()
	: CPropertyPage(CLoop1stKinkPagePan::IDD)
	, m_iRDadjUnit(0)
	, m_iNeckHephtUnit(0)
	, m_nNeckAngle(0)
	, m_nRHadj(0)
	, m_nNeckHephtadj(0)
{

}

CLoop1stKinkPagePan::~CLoop1stKinkPagePan()
{
}

void CLoop1stKinkPagePan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_RH_ADJ, m_iRDadjUnit);
	DDX_CBIndex(pDX, IDC_COMBO_NECK_HEPHT, m_iNeckHephtUnit);
	DDX_Text(pDX, IDC_EDIT_NECK_ANGLE, m_nNeckAngle);
	DDX_Text(pDX, IDC_EDIT_RH_ADY, m_nRHadj);
	DDX_Text(pDX, IDC_EDIT_NECK_HEPHT_ADJ, m_nNeckHephtadj);
}


BEGIN_MESSAGE_MAP(CLoop1stKinkPagePan, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_RH_ADJ, &CLoop1stKinkPagePan::OnCbnSelchangeComboRhAdj)
	ON_CBN_SELCHANGE(IDC_COMBO_NECK_HEPHT, &CLoop1stKinkPagePan::OnCbnSelchangeComboNeckHepht)
	ON_EN_SETFOCUS(IDC_EDIT_NECK_ANGLE, &CLoop1stKinkPagePan::OnEnSetfocusEditNeckAngle)
	ON_EN_SETFOCUS(IDC_EDIT_RH_ADY, &CLoop1stKinkPagePan::OnEnSetfocusEditRhAdy)
	ON_EN_SETFOCUS(IDC_EDIT_NECK_HEPHT_ADJ, &CLoop1stKinkPagePan::OnEnSetfocusEditNeckHephtAdj)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()

END_MESSAGE_MAP()


// CLoop1stKinkPagePan 消息处理程序

void CLoop1stKinkPagePan::OnCbnSelchangeComboRhAdj()
{
	UpdateData(true);

	SaveParameter();

	UpdateData(false);
}

void CLoop1stKinkPagePan::OnCbnSelchangeComboNeckHepht()
{
	UpdateData();

	SaveParameter();

	UpdateData(false);
	
}

void CLoop1stKinkPagePan::OnEnSetfocusEditNeckAngle()
{
	GetDlgItem(IDC_STATIC_NECK_ANGLE)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nNeckAngle, 35, 5, 75);
    
	UpdateData(false);

	SaveParameter();

}

void CLoop1stKinkPagePan::OnEnSetfocusEditRhAdy()
{
	GetDlgItem(IDC_STATIC_RH_ADJ)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nRHadj);

	UpdateData(false);

	SaveParameter();
}

void CLoop1stKinkPagePan::OnEnSetfocusEditNeckHephtAdj()
{
	GetDlgItem(IDC_STATIC_NECK_HEPHT_ADJ)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nNeckHephtadj);

	UpdateData(false);

	SaveParameter();
}

HBRUSH CLoop1stKinkPagePan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLoop1stKinkPagePan::OnDestroy()
{

	CPropertyPage::OnDestroy();	
}


BOOL CLoop1stKinkPagePan::OnSetActive()
{
	UpdateMenuVariable();

	return CPropertyPage::OnSetActive();

}

BOOL CLoop1stKinkPagePan::OnInitDialog()
{

	theApp.SetDlgFont(this);	


	return CPropertyPage::OnInitDialog();
}

void CLoop1stKinkPagePan::UpdateMenuVariable(void)
{
	
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

    theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	m_iNeckHephtUnit    = stLoopAcsModePara.iNeckHtUnit;
	m_nNeckAngle        = stLoopAcsModePara.nNeckAngle;
	m_nRHadj            = stLoopAcsModePara.nRHadj;
	m_iRDadjUnit        = stLoopAcsModePara.iRDadjUnit;
	m_nNeckHephtadj     = stLoopAcsModePara.nNeckHtAdj;

	int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_NECK_ANGLE);//1
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_NECK_ANGLE)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_NECK_ANGLE_DEG)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_RH_ADJ);//2
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_RH_ADY)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_RH_ADJ)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_NECK_HEPHT_ADJ);//3
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_NECK_HEPHT_ADJ)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_NECK_HEPHT)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	UpdateData(FALSE);
}

void CLoop1stKinkPagePan::SaveParameter(void)
{
	UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	stLoopAcsModePara.iNeckHtUnit    = m_iNeckHephtUnit;
	stLoopAcsModePara.nNeckAngle     = m_nNeckAngle;
	stLoopAcsModePara.nRHadj         = m_nRHadj;
	stLoopAcsModePara.iRDadjUnit     = m_iRDadjUnit;
	stLoopAcsModePara.nNeckHtAdj     = m_nNeckHephtadj;

	theLoopPara.SetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	(GetParent()->GetParent())->Invalidate();
}
