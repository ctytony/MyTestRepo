// LoopLandingPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
#include "LoopLandingPage.h"
#include "User.h"

// CLoopLandingPage dialog
IMPLEMENT_DYNAMIC(CLoopLandingPage, CPropertyPage)

CLoopLandingPage::CLoopLandingPage()
	: CPropertyPage(CLoopLandingPage::IDD)
    , m_nTrajFactor(0)
    , m_nTrajMode(0)
    , m_nLoopTopMode(0)
    , m_nCtactLvlOffset(0)
    , m_nPullDist(0)
    , m_nTrajSpeed(0)
    , m_nPullSpeed(0)
{
}

CLoopLandingPage::~CLoopLandingPage()
{
}

void CLoopLandingPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TRAJ_FACTOR, m_nTrajFactor);
    DDX_CBIndex(pDX, IDC_COMBO_TRAJ_MODE, m_nTrajMode);
    DDX_CBIndex(pDX, IDC_COMBO_LOOP_TOP_MODE, m_nLoopTopMode);
    DDX_Text(pDX, IDC_EDIT_CTACT_OFFSET, m_nCtactLvlOffset);
    DDX_Text(pDX, IDC_EDIT_PULL_DIST, m_nPullDist);
    DDX_Text(pDX, IDC_EDIT_TRAJECTORY_SPEED, m_nTrajSpeed);
    DDX_Text(pDX, IDC_EDIT_PULL_SPEED, m_nPullSpeed);
}

BEGIN_MESSAGE_MAP(CLoopLandingPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_TRAJ_FACTOR, OnFocusTrajFactor)
    ON_CBN_SELCHANGE(IDC_COMBO_TRAJ_MODE, OnTrajMode)
    ON_CBN_SELCHANGE(IDC_COMBO_LOOP_TOP_MODE, OnLoopTopMode)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_OFFSET, OnFocusCtactOffset)
    ON_EN_SETFOCUS(IDC_EDIT_PULL_DIST, OnFocusPullDist)
    ON_EN_SETFOCUS(IDC_EDIT_TRAJECTORY_SPEED, OnFocusTrajectorySpeed)
    ON_EN_SETFOCUS(IDC_EDIT_PULL_SPEED, OnFocusPullSpeed)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLoopLandingPage message handlers
HBRUSH CLoopLandingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }
    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CLoopLandingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
    return CPropertyPage::OnInitDialog();
}

// Update menu variable
void CLoopLandingPage::UpdateMenuVariable(void)
{
    short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    m_nTrajFactor     = stLoopPara.stLoopBase.nTrajFactor;
    m_nTrajMode       = stLoopPara.stLoopBase.cTrajMode;
    m_nLoopTopMode    = stLoopPara.stLoopBase.cLoopTopMode;
    m_nCtactLvlOffset = stLoopPara.stLoopBase.nCtactLvlOffset;
    m_nPullDist       = stLoopPara.stLoopBase.nPullDist;
    m_nTrajSpeed      = stLoopPara.stLoopAdv.nTrajSpeed;
    m_nPullSpeed      = stLoopPara.stLoopAdv.nPullSpeed;

    CWnd* pWnd;

    int iRowTop = PARA_COLUMN1_TOP;

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        pWnd = GetDlgItem(IDC_STATIC_TRAJ_FACTOR);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TRAJ_FACTOR));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_TRAJ_FACTOR);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_STATIC_TRAJECTORY_MODE);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TRAJ_MODE));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_COMBO_TRAJ_MODE);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

		if (theUser.GetUserLevel() > USER_SERVICE)
		{
			iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

			pWnd = GetDlgItem(IDC_STATIC_TRAJECTORY_SPEED);
			pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
			pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TRAJ_SPEED));
			pWnd->ShowWindow(SW_SHOWNORMAL);

			pWnd = GetDlgItem(IDC_EDIT_TRAJECTORY_SPEED);
			pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
			pWnd->ShowWindow(SW_SHOWNORMAL);
		}

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_STATIC_LOOP_TOP_MODE);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LOOP_TOP_MODE));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOOP_TOP_MODE);
        pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pComboBox->ResetContent();
        pComboBox->AddString(theMsg.GetMsg(MSG_STANDARD));
		//v3.1.T288 delete
        //pComboBox->AddString(theMsg.GetMsg(MSG_MODE_A));
        //pComboBox->AddString(theMsg.GetMsg(MSG_MODE_B));
		//v3.1.T288 add
        pComboBox->AddString(theMsg.GetMsg(MSG_ABOVE_LOOP));
        pComboBox->AddString(theMsg.GetMsg(MSG_FAST_LOOP2));
        pComboBox->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_STATIC_CTACT_OFFSET);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CTACT_LEVEL_OFFSET));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_CTACT_OFFSET);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    }
    else
    {
        GetDlgItem(IDC_STATIC_TRAJ_FACTOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_TRAJ_FACTOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TRAJECTORY_MODE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_TRAJ_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TRAJECTORY_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TRAJECTORY_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_LOOP_TOP_MODE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_LOOP_TOP_MODE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_CTACT_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CTACT_OFFSET)->ShowWindow(SW_HIDE);
    }

    pWnd = GetDlgItem(IDC_STATIC_PULL_DIST);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PULL_DIST));

    GetDlgItem(IDC_EDIT_PULL_DIST)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_SERVICE)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_STATIC_PULL_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PULL_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_PULL_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
        GetDlgItem(IDC_STATIC_PULL_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PULL_SPEED)->ShowWindow(SW_HIDE);
    }

    UpdateData(FALSE);
}

void CLoopLandingPage::SaveParameter()	//V1.1.W246 add
{
    UpdateData();

    short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    stLoopPara.stLoopBase.nTrajFactor     = m_nTrajFactor;
    stLoopPara.stLoopBase.cTrajMode       = (char)m_nTrajMode;
    stLoopPara.stLoopBase.cLoopTopMode    = (char)m_nLoopTopMode;
    stLoopPara.stLoopBase.nCtactLvlOffset = m_nCtactLvlOffset;
    stLoopPara.stLoopBase.nPullDist       = m_nPullDist;
    stLoopPara.stLoopAdv.nTrajSpeed       = m_nTrajSpeed;
    stLoopPara.stLoopAdv.nPullSpeed       = m_nPullSpeed;
    theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);

    (GetParent()->GetParent())->Invalidate();
}

void CLoopLandingPage::OnDestroy()
{
	SaveParameter();
    CPropertyPage::OnDestroy();
}

void CLoopLandingPage::OnFocusTrajFactor()
{
    GetDlgItem(IDC_STATIC_TRAJ_FACTOR)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nTrajFactor, 0, 20, 100);	//v3.1.T88 delete
	theKeyPad.GetUserKeyInData(&m_nTrajFactor, 0, 20, 200);		//v3.1.T88 add
    UpdateData(false);
    SaveParameter();
}

void CLoopLandingPage::OnTrajMode()
{
	UpdateData(true);
    SaveParameter();
}

void CLoopLandingPage::OnLoopTopMode()
{
	UpdateData(true);
    SaveParameter();
}

void CLoopLandingPage::OnFocusCtactOffset()
{
	GetDlgItem(IDC_STATIC_TRAJ_FACTOR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactLvlOffset, 0, -500, 500);
	UpdateData(false);
	SaveParameter();
}

void CLoopLandingPage::OnFocusPullDist()
{
	GetDlgItem(IDC_STATIC_TRAJ_FACTOR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPullDist);
	UpdateData(false);
	SaveParameter();
}

BOOL CLoopLandingPage::OnSetActive()
{
	UpdateMenuVariable();
	return CPropertyPage::OnSetActive();
}

void CLoopLandingPage::OnFocusTrajectorySpeed()
{
	GetDlgItem(IDC_STATIC_TRAJ_FACTOR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTrajSpeed, 100, 50, 100);
	UpdateData(false);
	SaveParameter();
}

void CLoopLandingPage::OnFocusPullSpeed()
{
	GetDlgItem(IDC_STATIC_TRAJ_FACTOR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPullSpeed, 50, 10, 100);
	UpdateData(false);
	SaveParameter();
}
