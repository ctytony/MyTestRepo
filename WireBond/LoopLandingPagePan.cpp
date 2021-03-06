// LoopLandingPagePan.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "LoopLandingPagePan.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "KeyPadHandler.h"
#include "AdvLoopParaSheet.h"
#include "LoopPara.h"
// CLoopLandingPagePan 对话框

IMPLEMENT_DYNAMIC(CLoopLandingPagePan, CPropertyPage)

CLoopLandingPagePan::CLoopLandingPagePan()
	: CPropertyPage(CLoopLandingPagePan::IDD)
	, m_nSLaey(0)
	, m_nAboneLoopLop(0)
	, m_nCtactLolOffset(0)
	, m_nPullDistH(0)
	, m_nPullDistV(0)
	, m_nPullVTime(0)
	, m_iSLaeyUnit(0)
{

}

CLoopLandingPagePan::~CLoopLandingPagePan()
{
}

void CLoopLandingPagePan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SL_AEY, m_nSLaey);
	DDX_Text(pDX, IDC_EDIT_ABONE_LOOP_LOP, m_nAboneLoopLop);
	DDX_Text(pDX, IDC_EDIT_CTACT_LOL_OFFSET, m_nCtactLolOffset);
	DDX_Text(pDX, IDC_EDIT_PULL_DIST_H, m_nPullDistH);
	DDX_Text(pDX, IDC_EDIT_PULL_DIST_V, m_nPullDistV);
	DDX_Text(pDX, IDC_EDIT_PULL_DIST_V_TIME, m_nPullVTime);
	DDX_CBIndex(pDX, IDC_COMBO_SL_AEY_UNIT, m_iSLaeyUnit);
}


BEGIN_MESSAGE_MAP(CLoopLandingPagePan, CPropertyPage)

	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_EDIT_SL_AEY, &CLoopLandingPagePan::OnEnSetfocusEditSlAey)
	ON_EN_SETFOCUS(IDC_EDIT_ABONE_LOOP_LOP, &CLoopLandingPagePan::OnEnSetfocusEditAboneLoopLop)
	ON_EN_SETFOCUS(IDC_EDIT_CTACT_LOL_OFFSET, &CLoopLandingPagePan::OnEnSetfocusEditCtactLolOffset)
	ON_EN_SETFOCUS(IDC_EDIT_PULL_DIST_H, &CLoopLandingPagePan::OnEnSetfocusEditPullDistH)
	ON_EN_SETFOCUS(IDC_EDIT_PULL_DIST_V, &CLoopLandingPagePan::OnEnSetfocusEditPullDistV)
	ON_EN_SETFOCUS(IDC_EDIT_PULL_DIST_V_TIME, &CLoopLandingPagePan::OnEnSetfocusEditPullDistVTime)
	ON_CBN_SELCHANGE(IDC_COMBO_SL_AEY_UNIT, &CLoopLandingPagePan::OnCbnSelchangeComboSlAeyUnit)
END_MESSAGE_MAP()



HBRUSH CLoopLandingPagePan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLoopLandingPagePan::OnDestroy()
{
	CPropertyPage::OnDestroy();
}

BOOL CLoopLandingPagePan::OnInitDialog()
{
	theApp.SetDlgFont(this);	

	return CPropertyPage::OnInitDialog();
}

BOOL CLoopLandingPagePan::OnSetActive()
{
	UpdateMenuVariable();

	return CPropertyPage::OnSetActive();
}

void CLoopLandingPagePan::OnEnSetfocusEditSlAey()
{
	GetDlgItem(IDC_STATIC_SLAEY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSLaey);

	UpdateData(false);

	SaveParameter();

	
}

void CLoopLandingPagePan::OnEnSetfocusEditAboneLoopLop()
{
	GetDlgItem(IDC_STATIC_ABONE_LOOP_LOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nAboneLoopLop, 0, 0, 500);

	UpdateData(false);

	SaveParameter();
	
}

void CLoopLandingPagePan::OnEnSetfocusEditCtactLolOffset()
{
	GetDlgItem(IDC_STATIC_CTACT_LOL_OFFSET)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nCtactLolOffset, 0, -300, 300);

	UpdateData(false);

	SaveParameter();
}

void CLoopLandingPagePan::OnEnSetfocusEditPullDistH()
{
	GetDlgItem(IDC_STATIC_PULL_DIST_H)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nPullDistH, 0,-200,200);

	UpdateData(false);

	SaveParameter();

	
}

void CLoopLandingPagePan::OnEnSetfocusEditPullDistV()
{
	GetDlgItem(IDC_STATIC_PULL_DIST_V)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nPullDistV, 0,0,100);

	UpdateData(false);

	SaveParameter();
}

void CLoopLandingPagePan::OnEnSetfocusEditPullDistVTime()
{
	GetDlgItem(IDC_STATIC_PULL_DIST_V_TIME)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nPullVTime, 4,2,8);

	UpdateData(false);

	SaveParameter();
}

void CLoopLandingPagePan::OnCbnSelchangeComboSlAeyUnit()
{
	UpdateData();

	SaveParameter();

	UpdateData(false);
	
}

void CLoopLandingPagePan::UpdateMenuVariable(void)
{
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	m_nSLaey          = stLoopAcsModePara.nSLadj;
	m_nAboneLoopLop   = stLoopAcsModePara.nAboveLoopTop;
	m_nCtactLolOffset = stLoopAcsModePara.nCtactLvlOffset;
	m_nPullDistH      = stLoopAcsModePara.nPullDistH; 
	m_nPullDistV      = stLoopAcsModePara.nPullDistV;
	m_nPullVTime      = stLoopAcsModePara.nPullVTime;
	m_iSLaeyUnit      = stLoopAcsModePara.iSLadjUnit;

	int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SLAEY);//1
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SL_AEY)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_SL_AEY_UNIT)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ABONE_LOOP_LOP);//2
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_ABONE_LOOP_LOP)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_ABONE_LOOP_LOP_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_CTACT_LOL_OFFSET);//3
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_CTACT_LOL_OFFSET)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_CTACT_LOL_OFFSET_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_PULL_DIST_H);//4
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_PULL_DIST_H)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_PULL_DIST_H_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_PULL_DIST_V);//5
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_PULL_DIST_V)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_PULL_DIST_V_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_PULL_DIST_V_TIME);//6
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_PULL_DIST_V_TIME)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_PULL_DIST_V_TIME_SMPL)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	
	UpdateData(FALSE);
}

void CLoopLandingPagePan::SaveParameter(void)
{
	UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	stLoopAcsModePara.nSLadj          = m_nSLaey;
	stLoopAcsModePara.nAboveLoopTop   = m_nAboneLoopLop;
	stLoopAcsModePara.nCtactLvlOffset = m_nCtactLolOffset;
	stLoopAcsModePara.nPullDistH      = m_nPullDistH;
	stLoopAcsModePara.nPullDistV      = m_nPullDistV;
	stLoopAcsModePara.nPullVTime      = m_nPullVTime;
	stLoopAcsModePara.iSLadjUnit      = m_iSLaeyUnit;

	theLoopPara.SetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	(GetParent()->GetParent())->Invalidate();
}
