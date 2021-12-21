// Loop2ndKinkPagePan.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "Loop2ndKinkPagePan.h"
#include "KeyPadHandler.h"
#include "MenuConfig.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
// CLoop2ndKinkPagePan 对话框

IMPLEMENT_DYNAMIC(CLoop2ndKinkPagePan, CPropertyPage)

CLoop2ndKinkPagePan::CLoop2ndKinkPagePan()
	: CPropertyPage(CLoop2ndKinkPagePan::IDD)
	, m_nFLengh(0)
	, m_iFLenghUnit(0)
	, m_nHLadj(0)
	, m_iHLadjUnit(0)
	, m_nFLadj(0)
	, m_iFLadjUnit(0)
	, m_nAngComp(0)
	, m_nKintHt(0)
{

}

CLoop2ndKinkPagePan::~CLoop2ndKinkPagePan()
{
}

void CLoop2ndKinkPagePan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FL_ENGH, m_nFLengh);
	DDX_CBIndex(pDX, IDC_COMBO_FL_ENGH_UNIT, m_iFLenghUnit);
	DDX_Text(pDX, IDC_EDIT_HL_ADJ, m_nHLadj);
	DDX_CBIndex(pDX, IDC_COMBO_HL_ADJ_UNIT, m_iHLadjUnit);
	DDX_Text(pDX, IDC_EDIT_FL_ADJ, m_nFLadj);
	DDX_CBIndex(pDX, IDC_COMBO_FL_ADJ_UNIT, m_iFLadjUnit);
	DDX_Text(pDX, IDC_EDIT_ANG_COMP, m_nAngComp);
	DDX_Text(pDX, IDC_EDIT_KINT_HT, m_nKintHt);
}


BEGIN_MESSAGE_MAP(CLoop2ndKinkPagePan, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_EDIT_FL_ENGH, &CLoop2ndKinkPagePan::OnEnSetfocusEditFlEngh)
	ON_CBN_SELCHANGE(IDC_COMBO_FL_ENGH_UNIT, &CLoop2ndKinkPagePan::OnCbnSelchangeComboFlEnghUnit)
	ON_EN_SETFOCUS(IDC_EDIT_HL_ADJ, &CLoop2ndKinkPagePan::OnEnSetfocusEditHlAdj)
	ON_CBN_SELCHANGE(IDC_COMBO_HL_ADJ_UNIT, &CLoop2ndKinkPagePan::OnCbnSelchangeComboHlAdjUnit)
	ON_EN_SETFOCUS(IDC_EDIT_FL_ADJ, &CLoop2ndKinkPagePan::OnEnSetfocusEditFlAdj)
	ON_CBN_SELCHANGE(IDC_COMBO_FL_ADJ_UNIT, &CLoop2ndKinkPagePan::OnCbnSelchangeComboFlAdjUnit)
	ON_EN_SETFOCUS(IDC_EDIT_ANG_COMP, &CLoop2ndKinkPagePan::OnEnSetfocusEditAngComp)
	ON_EN_SETFOCUS(IDC_EDIT_FL_ENGH, &CLoop2ndKinkPagePan::OnEnSetfocusEditFlEngh)
	ON_EN_SETFOCUS(IDC_EDIT_KINT_HT, &CLoop2ndKinkPagePan::OnEnSetfocusEditKintHt)
END_MESSAGE_MAP()


// CLoop2ndKinkPagePan 消息处理程序

HBRUSH CLoop2ndKinkPagePan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLoop2ndKinkPagePan::OnDestroy()
{

	CPropertyPage::OnDestroy();	

}

BOOL CLoop2ndKinkPagePan::OnSetActive()
{
	UpdateMenuVariable();

	return CPropertyPage::OnSetActive();
}

BOOL CLoop2ndKinkPagePan::OnInitDialog()
{

	theApp.SetDlgFont(this);	


	return CPropertyPage::OnInitDialog();
}

void CLoop2ndKinkPagePan::UpdateMenuVariable(void)
{
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	m_nFLengh     = stLoopAcsModePara.nFlatLength;
	m_iFLenghUnit = stLoopAcsModePara.iFlatLengthUnit;
	m_nHLadj      = stLoopAcsModePara.nHLadj ;
	m_iHLadjUnit  = stLoopAcsModePara.iHLadjUnit; 
	m_nFLadj      = stLoopAcsModePara.nFLadj ;
	m_iFLadjUnit  = stLoopAcsModePara.iFLadjUnit;
	m_nAngComp    = stLoopAcsModePara.nAngleComp;
	m_nKintHt     = stLoopAcsModePara.nKinkHt;

	int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_FL_ENGH);//1
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FL_ENGH)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FL_ENGH_UNIT)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_HL_ADJ);//2
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HL_ADJ)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_HL_ADJ_UNIT)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FL_ADJ);//3
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FL_ADJ)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FL_ADJ_UNIT)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ANG_COMP);//4
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_ANG_COMP)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_ANG_COMP_DEG)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_KINT_HT);//5
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_KINT_HT)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_KINT_HT_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	UpdateData(FALSE);

}

void CLoop2ndKinkPagePan::SaveParameter(void)
{
	UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	stLoopAcsModePara.nFlatLength     = m_nFLengh;
	stLoopAcsModePara.iFlatLengthUnit = m_iFLenghUnit;
	stLoopAcsModePara.nHLadj          = m_nHLadj;
	stLoopAcsModePara.iHLadjUnit      = m_iHLadjUnit;
	stLoopAcsModePara.nFLadj          = m_nFLadj;
	stLoopAcsModePara.iFLadjUnit      = m_iFLadjUnit;
    stLoopAcsModePara.nAngleComp      = m_nAngComp;
	stLoopAcsModePara.nKinkHt         = m_nKintHt;

	theLoopPara.SetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	(GetParent()->GetParent())->Invalidate();
}


void CLoop2ndKinkPagePan::OnCbnSelchangeComboFlEnghUnit()
{
	UpdateData(true);

	SaveParameter();

	UpdateData(false);

}

void CLoop2ndKinkPagePan::OnEnSetfocusEditHlAdj()
{
	GetDlgItem(IDC_STATIC_HL_ADJ)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nHLadj);

	UpdateData(false);

	SaveParameter();

}

void CLoop2ndKinkPagePan::OnCbnSelchangeComboHlAdjUnit()
{
	UpdateData(true);

	SaveParameter();

	UpdateData(false);

}

void CLoop2ndKinkPagePan::OnEnSetfocusEditFlAdj()
{
	GetDlgItem(IDC_STATIC_FL_ADJ)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nFLadj);

	UpdateData(false);

	SaveParameter();

}

void CLoop2ndKinkPagePan::OnCbnSelchangeComboFlAdjUnit()
{
	UpdateData(true);

	SaveParameter();

	UpdateData(false);

}

void CLoop2ndKinkPagePan::OnEnSetfocusEditAngComp()
{
	GetDlgItem(IDC_STATIC_ANG_COMP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nAngComp, 0, -45, 45);

	UpdateData(false);

	SaveParameter();
}

void CLoop2ndKinkPagePan::OnEnSetfocusEditFlEngh()
{
	GetDlgItem(IDC_STATIC_FL_ENGH)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nFLengh);

	UpdateData(false);

	SaveParameter();
	
}

void CLoop2ndKinkPagePan::OnEnSetfocusEditKintHt()
{
	GetDlgItem(IDC_STATIC_KINT_HT)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nKintHt, 0, -200, 200);

	UpdateData(false);

	SaveParameter();
}
