/***************************************************************
LoopParaAdvance.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "..\GkgLib\LoopPara\LoopPara.h"
#include "LoopParaAdvance.h"
#include "LoopParaSheet.h"

// CLoopParaAdvance dialog

IMPLEMENT_DYNAMIC(CLoopParaAdvance, CPropertyPage)

CLoopParaAdvance::CLoopParaAdvance()
	: CPropertyPage(CLoopParaAdvance::IDD)
    , m_iLoopGroup(0)
    , m_nTrajFactor(0)
    , m_iTrajectoryMode(0)
    , m_iLoopTopMode(0)
    , m_nCtactLvlOffset(0) //@2
    , m_nPullDist(0)
{

}

CLoopParaAdvance::~CLoopParaAdvance()
{
}

void CLoopParaAdvance::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TRAJ_FACTOR, m_nTrajFactor);
    DDX_CBIndex(pDX, IDC_COMBO_TRAJ_MODE, m_iTrajectoryMode);
    DDX_CBIndex(pDX, IDC_COMBO_LOOP_TOP_MODE, m_iLoopTopMode);
    DDX_Text(pDX, IDC_EDIT_CTACT_OFFSET, m_nCtactLvlOffset);
    DDX_Text(pDX, IDC_EDIT_PULL_DIST, m_nPullDist);
}


BEGIN_MESSAGE_MAP(CLoopParaAdvance, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_TRAJ_FACTOR, &CLoopParaAdvance::OnEnSetfocusEditTrajFactor)
	ON_CBN_SELCHANGE(IDC_COMBO_TRAJ_MODE, &CLoopParaAdvance::OnCbnSelchangeComboTrajMode)
	ON_CBN_SELCHANGE(IDC_COMBO_LOOP_TOP_MODE, &CLoopParaAdvance::OnCbnSelchangeComboLoopTopMode)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_OFFSET, &CLoopParaAdvance::OnEnSetfocusEditCtactOffset)
    ON_EN_SETFOCUS(IDC_EDIT_PULL_DIST, &CLoopParaAdvance::OnEnSetfocusEditPullDist)
END_MESSAGE_MAP()

// CLoopParaAdvance message handlers
BOOL CLoopParaAdvance::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, 1000, 1000, 100, 100, SWP_NOZORDER);

    int iRowTop;
    CString szString;

    iRowTop = PARA_COLUMN1_TOP;

    pWnd = GetDlgItem(IDC_STATIC_TRAJ_FACTOR);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TRAJ_FACTOR, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_EDIT_TRAJ_FACTOR);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_TRAJECTORY_MODE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TRAJ_MODE, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_COMBO_TRAJ_MODE);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_LOOP_TOP_MODE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_LOOP_PARA_LOOP_TOP_MODE, szString);
    pWnd->SetWindowTextW(szString);

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOOP_TOP_MODE);
    pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pComboBox->ResetContent();
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDARD, szString);
    pComboBox->InsertString(0, szString);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MODE_A, szString);
    pComboBox->InsertString(1, szString);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MODE_B, szString);
    pComboBox->InsertString(2, szString);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_CTACT_OFFSET); //@2
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_CONTACT_LVL_OFFSET, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_EDIT_CTACT_OFFSET);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_PULL_DIST); //@2
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_LOOP_PARA_PULL_DIST, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_EDIT_PULL_DIST);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    CLoopParaSheet* pLoopParaSheet = (CLoopParaSheet*)GetParent();
    m_iLoopGroup = pLoopParaSheet->m_LoopParaGeneralPage.m_iLoopGroup;

    LOOP_MODE_PARA stLoopModePara;
    theLoopParaMgr.GetLoopModePara(theLoopParaMgr.m_nLoopModeUsed[m_iLoopGroup], &stLoopModePara);

    m_nTrajFactor     = stLoopModePara.stLoopBasePara.nTrajFactor;
    m_iTrajectoryMode = stLoopModePara.stLoopBasePara.chTrajMode;
    m_iLoopTopMode    = stLoopModePara.stLoopBasePara.chLoopTopMode;
    m_nCtactLvlOffset = stLoopModePara.stLoopBasePara.nCtactLvlOffset; //@2
    m_nPullDist         = stLoopModePara.stLoopBasePara.nPullDist;

    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CLoopParaAdvance::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CLoopParaAdvance::OnEnSetfocusEditTrajFactor()
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTrajFactor, 0, 20, 100);
    UpdateData(false);
    OnOK();
}

BOOL CLoopParaAdvance::OnSetActive()
{
    // TODO: Add your specialized code here and/or call the base class
    CLoopParaSheet* pLoopParaSheet = (CLoopParaSheet*)GetParent();
    m_iLoopGroup = pLoopParaSheet->m_LoopParaGeneralPage.m_iLoopGroup;

    LOOP_MODE_PARA stLoopModePara;
    theLoopParaMgr.GetLoopModePara(theLoopParaMgr.m_nLoopModeUsed[m_iLoopGroup], &stLoopModePara);

    m_nTrajFactor     = stLoopModePara.stLoopBasePara.nTrajFactor;
    m_iTrajectoryMode = stLoopModePara.stLoopBasePara.chTrajMode;
    m_iLoopTopMode    = stLoopModePara.stLoopBasePara.chLoopTopMode;
    m_nCtactLvlOffset = stLoopModePara.stLoopBasePara.nCtactLvlOffset; //@2
    m_nPullDist         = stLoopModePara.stLoopBasePara.nPullDist;
    
    return CPropertyPage::OnSetActive();
}

void CLoopParaAdvance::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    LOOP_MODE_PARA stLoopModePara;
    theLoopParaMgr.GetLoopModePara(theLoopParaMgr.m_nLoopModeUsed[m_iLoopGroup], &stLoopModePara);

    CPropertyPage::OnOK();

    stLoopModePara.stLoopBasePara.nTrajFactor = m_nTrajFactor;
    
    if (m_iTrajectoryMode == 0)
        stLoopModePara.stLoopBasePara.chTrajMode = 0;
    else
        stLoopModePara.stLoopBasePara.chTrajMode = 1;

    if (m_iLoopTopMode == 0)
        stLoopModePara.stLoopBasePara.chLoopTopMode = ABOVE_LOOP_TOP_DISABLE;
    else if (m_iLoopTopMode == 1)
        stLoopModePara.stLoopBasePara.chLoopTopMode = ABOVE_LOOP_TOP_NORMAL;
    else
        stLoopModePara.stLoopBasePara.chLoopTopMode = ABOVE_LOOP_TOP_FAST;

    stLoopModePara.stLoopBasePara.nCtactLvlOffset = m_nCtactLvlOffset;

    stLoopModePara.stLoopBasePara.nPullDist = m_nPullDist;

    theLoopParaMgr.StoreLoopModePara(theLoopParaMgr.m_nLoopModeUsed[m_iLoopGroup], &stLoopModePara);

    theApp.ArchivePowerOnData(ARCHIVE_SAVE);
}

void CLoopParaAdvance::OnCbnSelchangeComboTrajMode()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CLoopParaAdvance::OnCbnSelchangeComboLoopTopMode()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CLoopParaAdvance::OnEnSetfocusEditCtactOffset() //@2
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nCtactLvlOffset, 0, -500, 500);

    UpdateData(false);

    OnOK();
}

void CLoopParaAdvance::OnEnSetfocusEditPullDist()
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nPullDist);

    UpdateData(false);

    OnOK();
}
