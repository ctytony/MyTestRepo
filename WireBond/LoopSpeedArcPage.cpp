// LoopSpeedArcPage.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "LoopSpeedArcPage.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "KeyPadHandler.h"
#include "AdvLoopParaSheet.h"
#include "LoopPara.h"
// CLoopSpeedArcPage 对话框

IMPLEMENT_DYNAMIC(CLoopSpeedArcPage, CPropertyPage)

CLoopSpeedArcPage::CLoopSpeedArcPage()
	: CPropertyPage(CLoopSpeedArcPage::IDD)
	, m_nSpdbEeuH(0)
	, m_nSpdBeuDist(0)
	, m_nSpdHL(0)
	, m_nSpdRD2(0)
	, m_nSpdSL(0)
	, m_nSpdTraj(0)
	, m_nArcRD(0)
	, m_nArcRD2(0)
	, m_nArcBeu(0)
	, m_nArcLpVop(0)
	, m_nArcVaj(0)
	, m_nPageSize(0)
	, m_nSpdRH(0)
	, m_nSpdRD(0)
{

}

CLoopSpeedArcPage::~CLoopSpeedArcPage()
{
}

void CLoopSpeedArcPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPD_BEU_H, m_nSpdbEeuH);
	DDX_Text(pDX, IDC_EDIT_SPD_BEU_DIST, m_nSpdBeuDist);
	DDX_Text(pDX, IDC_EDIT_SPD_HL, m_nSpdHL);
	DDX_Text(pDX, IDC_EDIT_SPD_RD2, m_nSpdRD2);
	DDX_Text(pDX, IDC_EDIT_SPD_SL, m_nSpdSL);
	DDX_Text(pDX, IDC_EDIT_SPD_TRAJ, m_nSpdTraj);
	DDX_Text(pDX, IDC_EDIT_ARC_RD, m_nArcRD);
	DDX_Text(pDX, IDC_EDIT_ARC_RD2, m_nArcRD2);
	DDX_Text(pDX, IDC_EDIT_ARC_BEU, m_nArcBeu);
	DDX_Text(pDX, IDC_EDIT_ARC_LP_VOP, m_nArcLpVop);
	DDX_Text(pDX, IDC_EDIT_ARC_VAJ, m_nArcVaj);
	DDX_Text(pDX, IDC_EDIT_SPD_RH, m_nSpdRH);
	DDX_Text(pDX, IDC_EDIT_SPD_RD, m_nSpdRD);
}


BEGIN_MESSAGE_MAP(CLoopSpeedArcPage, CPropertyPage)
	
	ON_EN_SETFOCUS(IDC_EDIT_SPD_RH, &CLoopSpeedArcPage::OnEnSetfocusEditSpdRh)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_RD, &CLoopSpeedArcPage::OnEnSetfocusEditSpdRd)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_BEU_H, &CLoopSpeedArcPage::OnEnSetfocusEditSpdBeuH)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_BEU_DIST, &CLoopSpeedArcPage::OnEnSetfocusEditSpdBeuDist)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_HL, &CLoopSpeedArcPage::OnEnSetfocusEditSpdHl)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_RD2, &CLoopSpeedArcPage::OnEnSetfocusEditSpdRd2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SL, &CLoopSpeedArcPage::OnEnSetfocusEditSpdSl)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_TRAJ, &CLoopSpeedArcPage::OnEnSetfocusEditSpdTraj)
	ON_EN_SETFOCUS(IDC_EDIT_ARC_RD, &CLoopSpeedArcPage::OnEnSetfocusEditArcRd)
	ON_EN_SETFOCUS(IDC_EDIT_ARC_RD2, &CLoopSpeedArcPage::OnEnSetfocusEditArcRd2)
	ON_EN_SETFOCUS(IDC_EDIT_ARC_BEU, &CLoopSpeedArcPage::OnEnSetfocusEditArcBeu)
	ON_EN_SETFOCUS(IDC_EDIT_ARC_LP_VOP, &CLoopSpeedArcPage::OnEnSetfocusEditArcLpVop)
	ON_EN_SETFOCUS(IDC_EDIT_ARC_VAJ, &CLoopSpeedArcPage::OnEnSetfocusEditArcVaj)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


void CLoopSpeedArcPage::OnEnSetfocusEditSpdRh()
{
	GetDlgItem(IDC_STATIC_SPD_TH)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdRH, 100, 20, 200);

	UpdateData(false);

	SaveParameter();

}

void CLoopSpeedArcPage::OnEnSetfocusEditSpdRd()
{
	GetDlgItem(IDC_STATIC_SPD_RD)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdRD, 100, 20, 200);

	UpdateData(false);

	SaveParameter();
	
}

void CLoopSpeedArcPage::OnEnSetfocusEditSpdBeuH()
{
	GetDlgItem(IDC_STATIC_SPD_BEU_H)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdbEeuH, 100, 20, 200);

	UpdateData(false);

	SaveParameter();
}

void CLoopSpeedArcPage::OnEnSetfocusEditSpdBeuDist()
{
	GetDlgItem(IDC_STATIC_SPD_BEU_DIST)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdBeuDist, 100, 20, 200);

	UpdateData(false);

	SaveParameter();
}

void CLoopSpeedArcPage::OnEnSetfocusEditSpdHl()
{
	GetDlgItem(IDC_STATIC_SPD_HL)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdHL, 100, 20, 200);

	UpdateData(false);

	SaveParameter();

	
}

void CLoopSpeedArcPage::OnEnSetfocusEditSpdRd2()
{
	GetDlgItem(IDC_STATIC_SPD_RD2)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdRD2, 100, 20, 200);

	UpdateData(false);

	SaveParameter();
	
}

void CLoopSpeedArcPage::OnEnSetfocusEditSpdSl()
{
	GetDlgItem(IDC_STATIC_SPD_SL)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdSL, 100, 20, 200);

	UpdateData(false);

	SaveParameter();
	
}

void CLoopSpeedArcPage::OnEnSetfocusEditSpdTraj()
{
	GetDlgItem(IDC_STATIC_SPD_TRAJ)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nSpdTraj, 100, 20, 200);

	UpdateData(false);

	SaveParameter();

}

void CLoopSpeedArcPage::OnEnSetfocusEditArcRd()
{
	GetDlgItem(IDC_STATIC_ARC_RD)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nArcRD, 200, 20, 500);

	UpdateData(false);

	SaveParameter();

}

void CLoopSpeedArcPage::OnEnSetfocusEditArcRd2()
{
	GetDlgItem(IDC_STATIC_ARC_RD2)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nArcRD2, 200, 20, 500);

	UpdateData(false);

	SaveParameter();
	
}

void CLoopSpeedArcPage::OnEnSetfocusEditArcBeu()
{
	GetDlgItem(IDC_STATIC_ARC_BEU)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nArcBeu, 200, 20, 500);

	UpdateData(false);

	SaveParameter();
	
}

void CLoopSpeedArcPage::OnEnSetfocusEditArcLpVop()
{
	GetDlgItem(IDC_STATIC_ARC_LP_VOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nArcLpVop, 200, 20, 500);

	UpdateData(false);

	SaveParameter();
}

void CLoopSpeedArcPage::OnEnSetfocusEditArcVaj()
{
	GetDlgItem(IDC_STATIC_ARC_VAJ)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nArcVaj, 200, 20, 500);

	UpdateData(false);

	SaveParameter();
}

BOOL CLoopSpeedArcPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	

	return CPropertyPage::OnInitDialog();
}

HBRUSH CLoopSpeedArcPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLoopSpeedArcPage::OnDestroy()
{
	CPropertyPage::OnDestroy();
}

BOOL CLoopSpeedArcPage::OnSetActive()
{
	UpdateMenuVariable();

	return CPropertyPage::OnSetActive();
}

void CLoopSpeedArcPage::UpdateMenuVariable()
{
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	m_nSpdbEeuH   = stLoopAcsModePara.nSpdBelleHt;
	m_nSpdBeuDist = stLoopAcsModePara.nSpdBelleDist;
	m_nSpdHL      = stLoopAcsModePara.nSpdHL;
	m_nSpdRD2     = stLoopAcsModePara.nSpdRD2; 
	m_nSpdSL      = stLoopAcsModePara.nSpdSL;
	m_nSpdTraj    = stLoopAcsModePara.nSpdTraj;
	m_nArcRD      = stLoopAcsModePara.nArcRD;
	m_nArcRD2     = stLoopAcsModePara.nArcRD2;
	m_nArcBeu     = stLoopAcsModePara.nArcBelle;
	m_nArcLpVop   = stLoopAcsModePara.nArcLoopTop;
	m_nArcVaj     = stLoopAcsModePara.nArcTraj;
	m_nSpdRH      = stLoopAcsModePara.nSpdRH; 
	m_nSpdRD      = stLoopAcsModePara.nSpdRD;

	int iRowTop = PARA_COLUMN1_TOP;                          //v1.6.P190sc edit

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SPD_TH);//1
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_RH)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_TH_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPD_RD);//2
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_RD)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_RD_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPD_BEU_H);//3
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_BEU_H)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_BEU_H_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPD_BEU_DIST);//4
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_BEU_DIST)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_BEU_DIST_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPD_HL);//5
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_HL)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_HL_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPD_RD2);//6
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_RD2)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_RD2_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPD_SL);//7
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_SL)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_SL_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPD_TRAJ);//8
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPD_TRAJ)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_SPD_TRAJ_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ARC_RD);//9
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_ARC_RD)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_ARC_RD_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ARC_RD2);//10
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_ARC_RD2)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_ARC_RD2_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);


	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ARC_BEU);//11
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_ARC_BEU)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_ARC_BEU_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);


	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ARC_LP_VOP);//12
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_ARC_LP_VOP)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_ARC_LP_VOP_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_ARC_VAJ);//13
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH-40, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_ARC_VAJ)->SetWindowPos(NULL, LOOP_PARA_LEFT-40, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_ARC_VAJ_PERCEN)->SetWindowPos(NULL, 380, iRowTop, 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);




	if (iRowTop > LOOP_PARA_PAGE_HEIGHT)m_nPageSize = iRowTop - LOOP_PARA_PAGE_HEIGHT+50;
	else m_nPageSize=0;

	SetScrollPos(SB_VERT, 0);
	SetScrollRange(SB_VERT, 0, m_nPageSize);

	UpdateData(FALSE);
}

void CLoopSpeedArcPage::SaveParameter()
{
    UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	LOOP_MODE_ACS_PARA stLoopAcsModePara;

	theLoopPara.GetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	stLoopAcsModePara.nSpdBelleHt   = m_nSpdbEeuH;
	stLoopAcsModePara.nSpdBelleDist = m_nSpdBeuDist;
	stLoopAcsModePara.nSpdHL        = m_nSpdHL;
	stLoopAcsModePara.nSpdRD2       = m_nSpdRD2;
	stLoopAcsModePara.nSpdSL        = m_nSpdSL;
	stLoopAcsModePara.nSpdTraj      = m_nSpdTraj;
	stLoopAcsModePara.nArcRD        = m_nArcRD;
	stLoopAcsModePara.nArcRD2       = m_nArcRD2;
	stLoopAcsModePara.nArcBelle     = m_nArcBeu;
	stLoopAcsModePara.nArcLoopTop   = m_nArcLpVop;
	stLoopAcsModePara.nArcTraj      = m_nArcVaj;
	stLoopAcsModePara.nSpdRH        = m_nSpdRH;
	stLoopAcsModePara.nSpdRD        = m_nSpdRD;

	theLoopPara.SetLoopAcsModePara(theLoopPara.m_nLoopAcsModeUsed[nLoopGroup], &stLoopAcsModePara);

	(GetParent()->GetParent())->Invalidate();
}

void CLoopSpeedArcPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	SCROLLINFO  stScrollInfo;

	if(GetScrollInfo(SB_VERT, &stScrollInfo)==FALSE)return;

	short nOffset = 0;

	int nMin = stScrollInfo.nMin;
	int nMax = stScrollInfo.nMax;
	int iCurPos = GetScrollPos(SB_VERT);

	switch(nSBCode)
	{
	case SB_LINEDOWN:
		{
			if (iCurPos == nMax)
				break;

			nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
			if (iCurPos + nOffset > nMax)
				nOffset = nMax - iCurPos;
		}
		break;

	case SB_LINEUP:
		{
			if (iCurPos == nMin)
				break;

			nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE);
			if (iCurPos + nOffset < nMin)
				nOffset = nMin - iCurPos;
		}
		break;

	case SB_PAGEDOWN:
		{
			if (iCurPos == nMax)
				break;

			nOffset = m_nPageSize - iCurPos;
		}
		break;

	case SB_PAGEUP:
		{
			if (iCurPos == nMin)
				break;

			nOffset = -iCurPos;
		}
		break;

	case SB_THUMBTRACK:		

		nOffset=nPos-iCurPos;

		break;

	default:
		break;
	}

	SetScrollPos(SB_VERT, iCurPos + nOffset);
	ScrollWindow(0, -nOffset);

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLoopSpeedArcPage::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_SPD_TH)->SetFocus();

	CPropertyPage::OnMouseMove(nFlags, point);
}

BOOL CLoopSpeedArcPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta>0 ? SB_LINEUP : SB_LINEDOWN, NULL);

	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}
