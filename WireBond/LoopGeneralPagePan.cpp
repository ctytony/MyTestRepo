// LoopGeneralPagePan.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "LoopGeneralPagePan.h"
#include "KeyPadHandler.h"
#include "WireBond.h"
#include "AdvLoopParaSheet.h"
#include "LoopPara.h"
#include "MenuConfig.h"
// CLoopGeneralPagePan 对话框

IMPLEMENT_DYNAMIC(CLoopGeneralPagePan, CPropertyPage)

CLoopGeneralPagePan::CLoopGeneralPagePan()
	: CPropertyPage(CLoopGeneralPagePan::IDD)
	, m_nLoopHeight1(0)
	, m_nLoopCorrect(0)
	, m_nAlpha(0)
	, m_nXydelay(0)
{

}

CLoopGeneralPagePan::~CLoopGeneralPagePan()
{
}

void CLoopGeneralPagePan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOOP_HEIGHT, m_nLoopHeight1);
	DDX_Text(pDX, IDC_EDIT_LOOP_COMPENSATION, m_nLoopCorrect);
	DDX_Text(pDX, IDC_EDIT_LOOP_ALPHA, m_nAlpha);
	DDX_Text(pDX, IDC_EDIT_LOOP_XY_DELAY, m_nXydelay);
}


BEGIN_MESSAGE_MAP(CLoopGeneralPagePan, CPropertyPage)
	ON_EN_SETFOCUS(IDC_EDIT_LOOP_HEIGHT, &CLoopGeneralPagePan::OnEnSetfocusEditLoopHeight)
	ON_EN_SETFOCUS(IDC_EDIT_LOOP_COMPENSATION, &CLoopGeneralPagePan::OnEnSetfocusEditLoopCompensation)
	ON_EN_SETFOCUS(IDC_EDIT_LOOP_ALPHA, &CLoopGeneralPagePan::OnEnSetfocusEditLoopAlpha)
	ON_EN_SETFOCUS(IDC_EDIT_LOOP_XY_DELAY, &CLoopGeneralPagePan::OnEnSetfocusEditLoopXyDelay)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CLoopGeneralPagePan 消息处理程序

void CLoopGeneralPagePan::OnEnSetfocusEditLoopHeight()
{
	GetDlgItem(IDC_STATIC_LOOP_HEIGHT)->SetFocus();
 
	theKeyPad.GetUserKeyInData(&m_nLoopHeight1,120,30,500);

	UpdateData(false);

	SaveParameter();
}

void CLoopGeneralPagePan::OnEnSetfocusEditLoopCompensation()
{
	GetDlgItem(IDC_STATIC_LOOP_COMPENSATION)->SetFocus();
    
	theKeyPad.GetUserKeyInData(&m_nLoopCorrect,0,-300,300);

	UpdateData(false);

	SaveParameter();
	
}

void CLoopGeneralPagePan::OnEnSetfocusEditLoopAlpha()
{
	GetDlgItem(IDC_STATIC_ALPHA)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nAlpha,70,0,200);

	UpdateData(false);

	SaveParameter();
}

void CLoopGeneralPagePan::OnEnSetfocusEditLoopXyDelay()
{
	GetDlgItem(IDC_STATIC_XY_DELAY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nXydelay,0,0,8);

	UpdateData(false);

	SaveParameter();
}

void CLoopGeneralPagePan::OnDestroy()
{
	CPropertyPage::OnDestroy();
}

HBRUSH CLoopGeneralPagePan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CLoopGeneralPagePan::OnInitDialog()
{
	theApp.SetDlgFont(this);	

	UpdateMenuVariable();

	return CPropertyPage::OnInitDialog();
}

void CLoopGeneralPagePan::UpdateMenuVariable(void)
{

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	m_nLoopHeight1 = stLoopAcsModePara.nLoopHeight1;
	m_nLoopCorrect = stLoopAcsModePara.nLoopCorrect;
	m_nAlpha       = stLoopAcsModePara.nAlpha;
	m_nXydelay     = stLoopAcsModePara.nXyDelay;

	int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LOOP_HEIGHT);//1
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_LOOP_HEIGHT)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_LOOP_HEIGHT_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_LOOP_COMPENSATION);//2
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_LOOP_COMPENSATION)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_LOOP_COMPENSATION_UM)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ALPHA);//3
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_LOOP_ALPHA)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_STATIC_ALPHA_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_XY_DELAY);//4
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_LOOP_XY_DELAY)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_XY_DELAY_SMPL)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);


	UpdateData(FALSE);
}


BOOL CLoopGeneralPagePan::OnSetActive()
{

	UpdateMenuVariable();

	return CPropertyPage::OnSetActive();
}

void CLoopGeneralPagePan::SaveParameter()
{

	UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	stLoopAcsModePara.nLoopHeight1 = m_nLoopHeight1;
	stLoopAcsModePara.nLoopCorrect = m_nLoopCorrect;
	stLoopAcsModePara.nAlpha       = m_nAlpha;
	stLoopAcsModePara.nXyDelay     = m_nXydelay;

    theLoopPara.SetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	(GetParent()->GetParent())->Invalidate();
}

