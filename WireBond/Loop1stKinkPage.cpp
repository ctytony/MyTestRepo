// Loop1stKinkPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
#include "Loop1stKinkPage.h"
#include "User.h"

// CLoop1stKinkPage dialog

IMPLEMENT_DYNAMIC(CLoop1stKinkPage, CPropertyPage)

CLoop1stKinkPage::CLoop1stKinkPage()
	: CPropertyPage(CLoop1stKinkPage::IDD)
	, m_nLoopBaseMode(0)
	, m_nRevHt(0)
	, m_nRevDist(0)
	, m_nRevAngle(0)
	, m_nPreKinkEnable(0)
	, m_nPreKinkHt(0)
	, m_nPreKinkDist(0)
	, m_nPreKinkAngle(0)
	, m_nWireFeedSpeed(0)
	, m_nWireBentSpeed(0)
	, m_nPreKinkWireFeedSpeed(0)
	, m_nPreKinkWireBentSpeed(0)
	, m_nPageSize(0)
	//v3.1.T407 add
	, m_nRevHtFactor2(0)
	, m_nRevArcFactor(100)
	, m_nRevArcDelay(0)
	, m_nPreKinkHtFactor2(0)
	, m_nPreKinkArcFactor(100)
	, m_nPreKinkArcDelay(0)
	//v3.1.T575 add
	, m_nRevSpanLen2(0)
	, m_nPreKinkSpanLen2(0)
{
}

CLoop1stKinkPage::~CLoop1stKinkPage()
{
}

void CLoop1stKinkPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_LOOP_BASE_MODE, m_nLoopBaseMode);
	DDX_Text(pDX, IDC_EDIT_NECK_HEIGHT, m_nRevHt);
	DDX_Text(pDX, IDC_EDIT_REV_DIST, m_nRevDist);
	DDX_Text(pDX, IDC_EDIT_REV_ANGLE, m_nRevAngle);
	DDX_CBIndex(pDX, IDC_COMBO_PRE_KINK_ENABLE, m_nPreKinkEnable);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_HT, m_nPreKinkHt);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_DIST, m_nPreKinkDist);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_ANGLE, m_nPreKinkAngle);
	DDX_Text(pDX, IDC_EDIT_KINK1_WIRE_FEED_SPEED, m_nWireFeedSpeed);
	DDX_Text(pDX, IDC_EDIT_KINK1_WIRE_BENT_SPEED, m_nWireBentSpeed);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_WIRE_FEED_SPEED, m_nPreKinkWireFeedSpeed);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_WIRE_BENT_SPEED, m_nPreKinkWireBentSpeed);
	//v3.1.T407 add
	DDX_Text(pDX, IDC_EDIT_REV_HTFACTOR2, m_nRevHtFactor2);
	DDX_Text(pDX, IDC_EDIT_REV_ARC_FACTOR, m_nRevArcFactor);
	DDX_Text(pDX, IDC_EDIT_REV_ARC_DELAY, m_nRevArcDelay);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_HTFACTOR2, m_nPreKinkHtFactor2);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_ARC_FACTOR, m_nPreKinkArcFactor);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_ARC_DELAY, m_nPreKinkArcDelay);
	//v3.1.T575 add
	DDX_Text(pDX, IDC_EDIT_REV_SPANLEN2, m_nRevSpanLen2);
	DDX_Text(pDX, IDC_EDIT_PRE_KINK_SPANLEN2, m_nPreKinkSpanLen2);
}

BEGIN_MESSAGE_MAP(CLoop1stKinkPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO_LOOP_BASE_MODE, OnLoopBaseMode)
    ON_EN_SETFOCUS(IDC_EDIT_NECK_HEIGHT, OnFocusNeckHeight)
    ON_EN_SETFOCUS(IDC_EDIT_REV_DIST, OnFocusRevDist)
    ON_EN_SETFOCUS(IDC_EDIT_REV_ANGLE, OnFocusRevAngle)
    ON_CBN_SELCHANGE(IDC_COMBO_PRE_KINK_ENABLE, OnPreKinkEnable)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_HT, OnFocusPreKinkHt)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_DIST, OnFocusPreKinkDist)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_ANGLE, OnFocusPreKinkAngle)
    ON_EN_SETFOCUS(IDC_EDIT_KINK1_WIRE_FEED_SPEED, OnFocusKink1WireFeedSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_KINK1_WIRE_BENT_SPEED, OnFocusKink1WireBentSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_WIRE_FEED_SPEED, OnFocusPreKinkWireFeedSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_WIRE_BENT_SPEED, OnFocusPreKinkWireBentSpeed)
	//v3.1.T407 add
    ON_EN_SETFOCUS(IDC_EDIT_REV_HTFACTOR2, OnFocusRevHtFactor2)
    ON_EN_SETFOCUS(IDC_EDIT_REV_ARC_FACTOR, OnFocusRevArcFactor)
    ON_EN_SETFOCUS(IDC_EDIT_REV_ARC_DELAY, OnFocusRevArcDelay)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_HTFACTOR2, OnFocusPreKinkHtFactor2)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_ARC_FACTOR, OnFocusPreKinkArcFactor)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_ARC_DELAY, OnFocusPreKinkArcDelay)
	//v3.1.T575 add
    ON_EN_SETFOCUS(IDC_EDIT_REV_SPANLEN2, OnFocusRevSpanLen2)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_SPANLEN2, OnFocusPreKinkSpanLen2)

    ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLoop1stKinkPage message handlers
HBRUSH CLoop1stKinkPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }
    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CLoop1stKinkPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_LOOP_BASE_MODE,theMsg.GetMsg(MSG_LOOP_BASE_MODE));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOOP_BASE_MODE);
	pComboBox->AddString(theMsg.GetMsg(MSG_STANDARD));
	pComboBox->AddString(theMsg.GetMsg(MSG_MODE_A));
	pComboBox->AddString(theMsg.GetMsg(MSG_MODE_B));
    UpdateMenuVariable(); //v3.1.L370 add
    return CPropertyPage::OnInitDialog();
}

//v3.1.L370 add
void CLoop1stKinkPage::DisableScroll()  
{
	int nCurPos = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, 0);
	ScrollWindow(0, nCurPos);
	SetScrollRange(SB_VERT, 0, 0);
}

// Update loop menu variable
void CLoop1stKinkPage::UpdateMenuVariable(void)
{
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    m_nLoopBaseMode         = stLoopPara.stLoopBase.nLoopBaseMode;
    m_nRevHt                = stLoopPara.stLoopBase.nRevHt;
    m_nRevDist              = stLoopPara.stLoopBase.nRevDist;
    m_nRevAngle             = stLoopPara.stLoopBase.nRevAngle;
    m_nPreKinkEnable        = stLoopPara.stLoopBase.bPreKinkEnable;
    m_nPreKinkHt            = stLoopPara.stLoopBase.nPreKinkHt;
    m_nPreKinkDist          = stLoopPara.stLoopBase.nPreKinkDist;
    m_nPreKinkAngle         = stLoopPara.stLoopBase.nPreKinkAngle;
    m_nWireFeedSpeed        = stLoopPara.stLoopAdv.nWireFeedSpeed;
    m_nWireBentSpeed        = stLoopPara.stLoopAdv.nWireBentSpeed;
    m_nPreKinkWireFeedSpeed = stLoopPara.stLoopAdv.nPreKinkWireFeedSpeed;
    m_nPreKinkWireBentSpeed = stLoopPara.stLoopAdv.nPreKinkWireBentSpeed;
	//v3.1.T407 add
	m_nRevHtFactor2         = stLoopPara.stLoopBase.nRevHtFactor2;
	m_nRevArcFactor         = stLoopPara.stLoopBase.nRevArcFactor;
	m_nRevArcDelay          = stLoopPara.stLoopBase.nRevArcDelay;
	m_nPreKinkHtFactor2     = stLoopPara.stLoopBase.nPreKinkHtFactor2;
	m_nPreKinkArcFactor     = stLoopPara.stLoopBase.nPreKinkArcFactor;
	m_nPreKinkArcDelay      = stLoopPara.stLoopBase.nPreKinkArcDelay;
	//v3.1.T575 add
	m_nRevSpanLen2          = stLoopPara.stLoopBase.nRevSpanLen2;
	m_nPreKinkSpanLen2      = stLoopPara.stLoopBase.nPreKinkSpanLen2;

	//v3.1.L370 add
	int iCurPos = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, 0);
	ScrollWindow(0, iCurPos);

    CWnd* pWnd;
    int iRowTop = PARA_COLUMN1_TOP;

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
		GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetWindowPos(NULL, PARA_LABEL_LEFT, PARA_COLUMN1_TOP, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_COMBO_LOOP_BASE_MODE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_COMBO_LOOP_BASE_MODE)->SetWindowPos(NULL, LOOP_PARA_LEFT, PARA_COLUMN1_TOP, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    }
    else
    {
		GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_LOOP_BASE_MODE)->ShowWindow(SW_HIDE);
    }

    if ((m_nLoopBaseMode == 2) && (theUser.GetUserLevel() > USER_OPERATOR))
    {
		//v3.1.L370 add
		int m_nOtherModePageSize = 180;
		SetScrollRange(SB_VERT, 0, m_nOtherModePageSize);

        pWnd = GetDlgItem(IDC_STATIC_NECK_HEIGHT);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NECK_HT));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_NECK_HEIGHT);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    }
    else
    {
		GetDlgItem(IDC_STATIC_NECK_HEIGHT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_NECK_HEIGHT)->ShowWindow(SW_HIDE);
    }

    pWnd = GetDlgItem(IDC_STATIC_REV_DIST);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_DIST));
    GetDlgItem(IDC_EDIT_REV_DIST)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    pWnd = GetDlgItem(IDC_STATIC_REV_ANGLE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_ANGLE));
    GetDlgItem(IDC_EDIT_REV_ANGLE)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_SERVICE)
    {
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_KINK1_WIRE_FEED_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK1_WIRE_FEED_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_KINK1_WIRE_FEED_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_KINK1_WIRE_BENT_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK1_WIRE_BENT_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_KINK1_WIRE_BENT_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T575 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_REV_SPANLEN2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_SPANLEN2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_REV_SPANLEN2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_REV_HTFACTOR2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_HTFACTOR2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_REV_HTFACTOR2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_REV_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_ARC_FACTOR));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_REV_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_REV_ARC_DELAY);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_ARC_DELAY));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_REV_ARC_DELAY);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);
	}
    else
    {  
		GetDlgItem(IDC_STATIC_KINK1_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK1_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KINK1_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK1_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
		//v3.1.T407 add
		GetDlgItem(IDC_STATIC_REV_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_REV_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_REV_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_REV_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_REV_ARC_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_REV_ARC_DELAY)->ShowWindow(SW_HIDE);
    }

	if (theUser.GetUserLevel() > USER_OPERATOR)
    {
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_COMBO_PRE_KINK_ENABLE);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
		GetDlgItem(IDC_STATIC_PRE_KINK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_PRE_KINK_ENABLE)->ShowWindow(SW_HIDE);
    }

    if ((theUser.GetUserLevel() > USER_OPERATOR) && (m_nPreKinkEnable == TRUE))
    {
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_HT);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_HT));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_HT);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_DIST);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_DIST));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_DIST);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_ANGLE);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_ANGLE));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_ANGLE);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_WIRE_FEED_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_WIRE_FEED_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_WIRE_FEED_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_WIRE_BENT_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_WIRE_BENT_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_WIRE_BENT_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T575 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_SPANLEN2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_SPANLEN2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_SPANLEN2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_HTFACTOR2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_HTFACTOR2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_HTFACTOR2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_ARC_FACTOR));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_ARC_DELAY);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_ARC_DELAY));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_ARC_DELAY);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);
	}
    else   
	{
		GetDlgItem(IDC_STATIC_PRE_KINK_HT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_HT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_KINK_DIST)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_DIST)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_KINK_ANGLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_ANGLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_KINK_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_KINK_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
		//v3.1.T407 add
		GetDlgItem(IDC_STATIC_PRE_KINK_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PRE_KINK_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PRE_KINK_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PRE_KINK_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PRE_KINK_ARC_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PRE_KINK_ARC_DELAY)->ShowWindow(SW_HIDE);
	}

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	/* v3.1.L370 delete
    if (iRowTop > LOOP_PARA_PAGE_HEIGHT)
		m_nPageSize = iRowTop - LOOP_PARA_PAGE_HEIGHT;
    else
		m_nPageSize = 0;

	SetScrollPos(SB_VERT, 0);
	SetScrollRange(SB_VERT, 0, m_nPageSize);
	*/

	//v3.1.L370 add
	if (iRowTop > LOOP_PARA_PAGE_HEIGHT)
	{
		m_nPageSize = iRowTop - LOOP_PARA_PAGE_HEIGHT;
		EnableScrollBarCtrl(SB_VERT, TRUE);
		SetScrollRange(SB_VERT, 0, m_nPageSize);
	}
	else
		DisableScroll();
	UpdateData(FALSE);
}

void CLoop1stKinkPage::OnLoopBaseMode()
{
    UpdateData(true);
    SaveParameter();
    UpdateMenuVariable();
    UpdateData(false);
}

void CLoop1stKinkPage::SaveParameter()	//V1.1.W246 add
{
    UpdateData();

    short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    stLoopPara.stLoopBase.nLoopBaseMode        = m_nLoopBaseMode;
    stLoopPara.stLoopBase.nRevHt               = m_nRevHt;
    stLoopPara.stLoopBase.nRevDist             = m_nRevDist;
    stLoopPara.stLoopBase.nRevAngle            = m_nRevAngle;
    stLoopPara.stLoopBase.bPreKinkEnable       = m_nPreKinkEnable!=0;
    stLoopPara.stLoopBase.nPreKinkHt           = m_nPreKinkHt;
    stLoopPara.stLoopBase.nPreKinkDist         = m_nPreKinkDist;
    stLoopPara.stLoopBase.nPreKinkAngle        = m_nPreKinkAngle;
    stLoopPara.stLoopAdv.nWireFeedSpeed        = m_nWireFeedSpeed;
    stLoopPara.stLoopAdv.nWireBentSpeed        = m_nWireBentSpeed;
    stLoopPara.stLoopAdv.nPreKinkWireFeedSpeed = m_nPreKinkWireFeedSpeed;
    stLoopPara.stLoopAdv.nPreKinkWireBentSpeed = m_nPreKinkWireBentSpeed;
	//v3.1.T407 add
	stLoopPara.stLoopBase.nRevHtFactor2        = m_nRevHtFactor2;
	stLoopPara.stLoopBase.nRevArcFactor        = m_nRevArcFactor;
	stLoopPara.stLoopBase.nRevArcDelay         = m_nRevArcDelay;
	stLoopPara.stLoopBase.nPreKinkHtFactor2    = m_nPreKinkHtFactor2;
	stLoopPara.stLoopBase.nPreKinkArcFactor    = m_nPreKinkArcFactor;
	stLoopPara.stLoopBase.nPreKinkArcDelay     = m_nPreKinkArcDelay;
	//v3.1.T575 add
	stLoopPara.stLoopBase.nRevSpanLen2         = m_nRevSpanLen2;
	stLoopPara.stLoopBase.nPreKinkSpanLen2     = m_nPreKinkSpanLen2;

	theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);

    (GetParent()->GetParent())->Invalidate();
}

void CLoop1stKinkPage::OnDestroy()
{
	SaveParameter();
    CPropertyPage::OnDestroy();
}

void CLoop1stKinkPage::OnFocusNeckHeight()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevHt, -1000, 1, 1000);
	UpdateData(false);
	SaveParameter();
}

void CLoop1stKinkPage::OnFocusRevDist()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nRevDist, 65, -100, 100);	//v3.1.T72 delete
	theKeyPad.GetUserKeyInData(&m_nRevDist, 65, -100, 500);		//v3.1.T72 add
	UpdateData(false);
	SaveParameter();
}

void CLoop1stKinkPage::OnFocusRevAngle()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nRevAngle,5,-50,50);	//v3.1.T77 delete
	theKeyPad.GetUserKeyInData(&m_nRevAngle, 5, -80, 80);	//v3.1.T77 add
	UpdateData(false);
	SaveParameter();
}

void CLoop1stKinkPage::OnPreKinkEnable()
{
	UpdateData();
	SaveParameter();
	UpdateMenuVariable();
	UpdateData(false);
}

void CLoop1stKinkPage::OnFocusPreKinkHt()
{
    GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreKinkHt, 60, -400, 400);
    UpdateData(false);
    SaveParameter();
}

void CLoop1stKinkPage::OnFocusPreKinkDist()
{
    GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreKinkDist, 50, -2000, 2000);
    UpdateData(false);
    SaveParameter();
}

void CLoop1stKinkPage::OnFocusPreKinkAngle()
{
    GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreKinkAngle, 20, -80, 80);
    UpdateData(false);
    SaveParameter();
}

BOOL CLoop1stKinkPage::OnSetActive()
{
    //UpdateMenuVariable();	//v3.1.T370 delete
    return CPropertyPage::OnSetActive();
}

void CLoop1stKinkPage::OnFocusKink1WireFeedSpeed()
{
    GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWireFeedSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

void CLoop1stKinkPage::OnFocusKink1WireBentSpeed()
{
    GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWireBentSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

void CLoop1stKinkPage::OnFocusPreKinkWireFeedSpeed()
{
    GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreKinkWireFeedSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

void CLoop1stKinkPage::OnFocusPreKinkWireBentSpeed()
{
    GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreKinkWireBentSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

void CLoop1stKinkPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SCROLLINFO stScrollInfo;
    if (GetScrollInfo(SB_VERT, &stScrollInfo) == FALSE) return;

    short nOffset = 0;
    int nMin = stScrollInfo.nMin;
    int nMax = stScrollInfo.nMax;
    int iCurPos = GetScrollPos(SB_VERT);

    switch(nSBCode)
    {
    case SB_LINEDOWN:
	{
		if (iCurPos == nMax) break;
		nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		if (iCurPos + nOffset > nMax)
			nOffset = nMax - iCurPos;
		break;
	}
    case SB_LINEUP:
	{
		if (iCurPos == nMin) break;
		nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE);
		if (iCurPos + nOffset < nMin)
			nOffset = nMin - iCurPos;
		break;
	}
    case SB_PAGEDOWN:
	{
		if (iCurPos == nMax) break;
		nOffset = m_nPageSize - iCurPos;
		break;
	}
    case SB_PAGEUP:
	{
		if (iCurPos == nMin) break;
		nOffset = -iCurPos;
		break;
	}
	case SB_THUMBTRACK:		//V1.1.W246 add
		nOffset=nPos-iCurPos;
		break;
    default:
        break;
    }

    SetScrollPos(SB_VERT, iCurPos + nOffset);
    ScrollWindow(0, -nOffset);

    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CLoop1stKinkPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)	//V1.1.W246 add
{
	PostMessage(WM_VSCROLL, zDelta>0 ? SB_LINEUP : SB_LINEDOWN, NULL);
	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CLoop1stKinkPage::OnMouseMove(UINT nFlags, CPoint point)	//V1.1.W246 add
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	CPropertyPage::OnMouseMove(nFlags, point);
}

//v3.1.T407 add
void CLoop1stKinkPage::OnFocusRevHtFactor2()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevHtFactor2, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop1stKinkPage::OnFocusRevArcFactor()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevArcFactor, 0, -20, 180);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop1stKinkPage::OnFocusRevArcDelay()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevArcDelay, 0, 0, 50);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop1stKinkPage::OnFocusPreKinkHtFactor2()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPreKinkHtFactor2, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop1stKinkPage::OnFocusPreKinkArcFactor()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPreKinkArcFactor, 0, -20, 180);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop1stKinkPage::OnFocusPreKinkArcDelay()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPreKinkArcDelay, 0, 0, 50);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T575 add
void CLoop1stKinkPage::OnFocusRevSpanLen2()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevSpanLen2);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T575 add
void CLoop1stKinkPage::OnFocusPreKinkSpanLen2()
{
	GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPreKinkSpanLen2);
	UpdateData(false);
	SaveParameter();
}
