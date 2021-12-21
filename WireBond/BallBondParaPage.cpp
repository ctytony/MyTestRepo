/***************************************************************
BallBondParaPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BondPara.h"
#include "BallPara.h"
#include "BallParaSheet.h"
#include "BallBondParaPage.h"
#include "Program.h"
#include "User.h"

// CBallBondParaPage dialog

IMPLEMENT_DYNAMIC(CBallBondParaPage, CPropertyPage)

CBallBondParaPage::CBallBondParaPage()
	: CPropertyPage(CBallBondParaPage::IDD)
    , m_iBallType(1)
    , m_nStandbyPower1(0)
    , m_nStandbyPower2(0)
    , m_nCtactTime1(0)
    , m_nCtactTime2(0)
    , m_nCtactPower1(0)
    , m_nCtactPower2(0)
    , m_nCtactForce1(0)
    , m_nCtactForce2(0)
    , m_nBondTime1(0)
    , m_nBondTime2(0)
    , m_nBondPower1(0)
    , m_nBondPower2(0)
    , m_nBondForce1(0)
    , m_nBondForce2(0)
    , m_nSearchHeight1(0)
    , m_nSearchTol1(0)
    , m_nSearchTol2(0)
    , m_iSearchSpeed1(0)
    , m_iSearchSpeed2(0)
    , m_iTailBreakEnable(0)
    , m_iFabType(0)
    , m_iTailBondEnable(0)
    , m_nPageSize(0)
    , m_nPreCtactTime1(0)
    , m_nPreCtactTime2(0)
    , m_nPreCtactForce1(0)
    , m_nPreCtactForce2(0)
    , m_nSrchDelay1(0)
    , m_iStickDetection(0)
    , m_nReleaseTime1(0)
    , m_nReleaseTime2(0)
    , m_nReleaseForce1(0)
    , m_nReleaseForce2(0)
    , m_nReleasePower1(0)
    , m_nReleasePower2(0)
	//v3.1.T410 add
	, m_nPosOffset(0)
	, m_nSpanLenDelay(0)
	, m_nArcSpan(0)
	, m_nArcSpanDelay(0)
{
}

CBallBondParaPage::~CBallBondParaPage()
{
}

void CBallBondParaPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PRE_POWER1, m_nStandbyPower1);
    DDX_Text(pDX, IDC_EDIT_PRE_POWER2, m_nStandbyPower2);
    DDX_Text(pDX, IDC_EDIT_CTACT_TIME1, m_nCtactTime1);
    DDX_Text(pDX, IDC_EDIT_CTACT_TIME2, m_nCtactTime2);
    DDX_Text(pDX, IDC_EDIT_CTACT_POWER1, m_nCtactPower1);
    DDX_Text(pDX, IDC_EDIT_CTACT_POWER2, m_nCtactPower2);
    DDX_Text(pDX, IDC_EDIT_CTACT_FORCE1, m_nCtactForce1);
    DDX_Text(pDX, IDC_EDIT_CTACT_FORCE2, m_nCtactForce2);
    DDX_Text(pDX, IDC_EDIT_BOND_TIME1, m_nBondTime1);
    DDX_Text(pDX, IDC_EDIT_BOND_TIME2, m_nBondTime2);
    DDX_Text(pDX, IDC_EDIT_BOND_POWER1, m_nBondPower1);
    DDX_Text(pDX, IDC_EDIT_BOND_POWER2, m_nBondPower2);
    DDX_Text(pDX, IDC_EDIT_BOND_FORCE1, m_nBondForce1);
    DDX_Text(pDX, IDC_EDIT_BOND_FORCE2, m_nBondForce2);
    DDX_Text(pDX, IDC_EDIT_SRCH_HEIGHT1, m_nSearchHeight1);
    DDX_Text(pDX, IDC_EDIT_SRCH_TOL1, m_nSearchTol1);
    DDX_Text(pDX, IDC_EDIT_SRCH_TOL2, m_nSearchTol2);
    DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED_1, m_iSearchSpeed1);
    DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED_2, m_iSearchSpeed2);
    DDX_CBIndex(pDX, IDC_COMBO_WIRE_TAIL_BREAK, m_iTailBreakEnable);
    DDX_CBIndex(pDX, IDC_COMBO_FAB_TYPE, m_iFabType);
    DDX_CBIndex(pDX, IDC_COMBO_TAIL_BOND_ENABLE, m_iTailBondEnable);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_TIME1, m_nPreCtactTime1);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_TIME2, m_nPreCtactTime2);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_FORCE1, m_nPreCtactForce1);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_FORCE2, m_nPreCtactForce2);
    DDX_Text(pDX, IDC_EDIT_SRCH_DELAY1, m_nSrchDelay1);
    DDX_CBIndex(pDX, IDC_COMBO_STICK_DETECTION, m_iStickDetection);
    DDX_Text(pDX, IDC_EDIT_RLS_TIME1, m_nReleaseTime1);
    DDX_Text(pDX, IDC_EDIT_RLS_TIME2, m_nReleaseTime2);
    DDX_Text(pDX, IDC_EDIT_RLS_FORCE1, m_nReleaseForce1);
    DDX_Text(pDX, IDC_EDIT_RLS_FORCE2, m_nReleaseForce2);
    DDX_Text(pDX, IDC_EDIT_RLS_POWER1, m_nReleasePower1);
    DDX_Text(pDX, IDC_EDIT_RLS_POWER2, m_nReleasePower2);
	//v3.1.T410 add
    DDX_Text(pDX, IDC_EDIT_POS_OFFSET, m_nPosOffset);
    DDX_Text(pDX, IDC_EDIT_SPAN_LEN_DELAY, m_nSpanLenDelay);
    DDX_Text(pDX, IDC_EDIT_ARC_SPAN, m_nArcSpan);
    DDX_Text(pDX, IDC_EDIT_ARC_SPAN_DELAY, m_nArcSpanDelay);
}

BEGIN_MESSAGE_MAP(CBallBondParaPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_PRE_POWER1, OnFocusPrePower1)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_POWER2, OnFocusPrePower2)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_TIME1, OnFocusCtactTime1)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_TIME2, OnFocusCtactTime2)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_POWER1, OnFocusCtactPower1)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_POWER2, OnFocusCtactPower2)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_FORCE1, OnFocusCtactForce1)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_FORCE2, OnFocusCtactForce2)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_TIME1, OnFocusBondTime1)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_TIME2, OnFocusBondTime2)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_POWER1, OnFocusBondPower1)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_POWER2, OnFocusBondPower2)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_FORCE1, OnFocusBondForce1)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_FORCE2, OnFocusBondForce2)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_HEIGHT1, OnFocusSrchHeight1)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_TOL1, OnFocusSrchTol1)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_TOL2, OnFocusSrchTol2)
    ON_CBN_SELCHANGE(IDC_COMBO_FAB_TYPE, OnFabType)
    ON_CBN_SELCHANGE(IDC_COMBO_TAIL_BOND_ENABLE, OnTailBondEnable)
    ON_WM_VSCROLL()
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_TIME1, OnFocusPreImpactTime1)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_TIME2, OnFocusPreImpactTime2)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_FORCE1, OnFocusPreImpactForce1)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_FORCE2, OnFocusPreImpactForce2)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_DELAY1, OnFocusSrchDelay1)
    ON_CBN_SELCHANGE(IDC_COMBO_STICK_DETECTION, OnStickDetection)
    ON_CBN_SELCHANGE(IDC_COMBO_SRCH_SPEED_1, OnSrchSpeed1)
    ON_CBN_SELCHANGE(IDC_COMBO_SRCH_SPEED_2, OnSrchSpeed2)
    ON_CBN_SELCHANGE(IDC_COMBO_WIRE_TAIL_BREAK, OnWireTailBreak)
    ON_EN_SETFOCUS(IDC_EDIT_RLS_TIME1, OnFocusRlsTime1)
    ON_EN_SETFOCUS(IDC_EDIT_RLS_TIME2, OnFocusRlsTime2)
    ON_EN_SETFOCUS(IDC_EDIT_RLS_FORCE1, OnFocusRlsForce1)
    ON_EN_SETFOCUS(IDC_EDIT_RLS_FORCE2, OnFocusRlsForce2)
    ON_EN_SETFOCUS(IDC_EDIT_RLS_POWER1, OnFocusRlsPower1)
    ON_EN_SETFOCUS(IDC_EDIT_RLS_POWER2, OnFocusRlsPower2)
	//v3.1.T410 add
    ON_EN_SETFOCUS(IDC_EDIT_POS_OFFSET, OnFocusPosOffset)
    ON_EN_SETFOCUS(IDC_EDIT_SPAN_LEN_DELAY, OnFocusSpanLenDelay)
    ON_EN_SETFOCUS(IDC_EDIT_ARC_SPAN, OnFocusArcSpan)
    ON_EN_SETFOCUS(IDC_EDIT_ARC_SPAN_DELAY, OnFocusArcSpanDelay)

	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CBallBondParaPage message handlers

BOOL CBallBondParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    int iRowTop = PARA_COLUMN1_TOP;

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        GetDlgItem(IDC_STATIC_2ND_BOND_ENABLE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_2ND_BOND_ENABLE,theMsg.GetMsg(MSG_TAIL_BOND));
        GetDlgItem(IDC_COMBO_TAIL_BOND_ENABLE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	}
	else
	{
        GetDlgItem(IDC_STATIC_2ND_BOND_ENABLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_TAIL_BOND_ENABLE)->ShowWindow(SW_HIDE);
	}

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
		GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_TIME,theMsg.GetMsg(MSG_PREIMPACT_TIME));
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	}
    else
    {
        GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME2)->ShowWindow(SW_HIDE);
    }

	GetDlgItem(IDC_STATIC_PRE_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_PRE_POWER,theMsg.GetMsg(MSG_STANDBY_POWER));
    GetDlgItem(IDC_EDIT_PRE_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_PRE_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_FORCE,theMsg.GetMsg(MSG_PRE_IMPACT_FORCE));
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
		GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE2)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_CTACT_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_CTACT_TIME,theMsg.GetMsg(MSG_CTACT_TIME));
    GetDlgItem(IDC_EDIT_CTACT_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_CTACT_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_CTACT_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_CTACT_POWER,theMsg.GetMsg(MSG_CTACT_POWER));
    GetDlgItem(IDC_EDIT_CTACT_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_CTACT_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_CTACT_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_CTACT_FORCE,theMsg.GetMsg(MSG_CTACT_FORCE));
    GetDlgItem(IDC_EDIT_CTACT_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_CTACT_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_BOND_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BOND_TIME,theMsg.GetMsg(MSG_BASE_TIME));
    GetDlgItem(IDC_EDIT_BOND_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_BOND_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_BOND_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BOND_POWER,theMsg.GetMsg(MSG_BASE_POWER));
    GetDlgItem(IDC_EDIT_BOND_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_BOND_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_BOND_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BOND_FORCE,theMsg.GetMsg(MSG_BASE_FORCE));
    GetDlgItem(IDC_EDIT_BOND_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_BOND_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_RLS_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_RLS_TIME,theMsg.GetMsg(MSG_RELEASE_TIME));
        GetDlgItem(IDC_EDIT_RLS_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_RLS_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_RLS_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_RLS_POWER,theMsg.GetMsg(MSG_RELEASE_POWER));
        GetDlgItem(IDC_EDIT_RLS_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_RLS_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_RLS_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_RLS_FORCE,theMsg.GetMsg(MSG_RELEASE_FORCE));
        GetDlgItem(IDC_EDIT_RLS_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_RLS_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
        GetDlgItem(IDC_STATIC_RLS_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RLS_TIME1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RLS_TIME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RLS_POWER)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RLS_POWER1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RLS_POWER2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RLS_FORCE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RLS_FORCE1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RLS_FORCE2)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_SRCH_HT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_SRCH_HT,theMsg.GetMsg(MSG_SRCH_HT));
    GetDlgItem(IDC_EDIT_SRCH_HEIGHT1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SRCH_SPEED,theMsg.GetMsg(MSG_SRCH_SPEED));
        GetDlgItem(IDC_COMBO_SRCH_SPEED_1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_SRCH_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SRCH_DELAY,theMsg.GetMsg(MSG_SRCH_DELAY));
        GetDlgItem(IDC_EDIT_SRCH_DELAY1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCH_DELAY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SRCH_DELAY1)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_SRCH_TOL)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_SRCH_TOL,theMsg.GetMsg(MSG_SRCH_THRES));
    GetDlgItem(IDC_EDIT_SRCH_TOL1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_SRCH_TOL2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_TAIL_BREAK)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_TAIL_BREAK,theMsg.GetMsg(MSG_TAIL_BREAK));

        CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WIRE_TAIL_BREAK);
        pComboBox->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
        pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_STICK_DETECT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_STICK_DETECT,theMsg.GetMsg(MSG_STICK_DETECTION));
        GetDlgItem(IDC_COMBO_STICK_DETECTION)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		//v3.1.T410 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_POS_OFFSET)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_POS_OFFSET, theMsg.GetMsg(MSG_POS_OFFSET));
		GetDlgItem(IDC_EDIT_POS_OFFSET)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		//v3.1.T410 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_SPAN_LEN_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_SPAN_LEN_DELAY, theMsg.GetMsg(MSG_SPAN_LEN_DELAY));
		GetDlgItem(IDC_EDIT_SPAN_LEN_DELAY)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		//v3.1.T410 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_ARC_SPAN)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_ARC_SPAN, theMsg.GetMsg(MSG_ARC_SPAN));
		GetDlgItem(IDC_EDIT_ARC_SPAN)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		//v3.1.T410 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_ARC_SPAN_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_ARC_SPAN_DELAY, theMsg.GetMsg(MSG_ARC_SPAN_DELAY));
		GetDlgItem(IDC_EDIT_ARC_SPAN_DELAY)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	}
    else
    {
        GetDlgItem(IDC_STATIC_TAIL_BREAK)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_WIRE_TAIL_BREAK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FAB_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_FAB_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STICK_DETECT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_STICK_DETECTION)->ShowWindow(SW_HIDE);
		//v3.1.T410 add
		GetDlgItem(IDC_STATIC_POS_OFFSET)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_POS_OFFSET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SPAN_LEN_DELAY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SPAN_LEN_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ARC_SPAN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ARC_SPAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ARC_SPAN_DELAY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ARC_SPAN_DELAY)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    if (iRowTop > PROPERTY_PAGE_HEIGHT)
    {
        m_nPageSize = iRowTop - PROPERTY_PAGE_HEIGHT;
        EnableScrollBarCtrl(SB_VERT, TRUE);
        SetScrollRange(SB_VERT, 0, m_nPageSize);
    }
    else
        EnableScrollBarCtrl(SB_VERT, FALSE);

    m_iBallType = ((CBallParaSheet*)GetParent())->GetBumpBallType();

    UpdateMenuVariable();
    TailBondMenuUpdate();

    return CPropertyPage::OnInitDialog();
}

HBRUSH CBallBondParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBallBondParaPage::OnDestroy()
{
    UpdateData();

    ST_BUMP_BALL stBumpBall;
    if (m_iBallType == 0)
		theBallPara.GetBbosBall(&stBumpBall);
    else
		theBallPara.GetBsobBall(&stBumpBall);
    stBumpBall.fStandbyPower[0]  = (float)m_nStandbyPower1 / (float)2.55;
    stBumpBall.fStandbyPower[1]  = (float)m_nStandbyPower2 / (float)2.55;
    stBumpBall.nCtactTime[0]     = m_nCtactTime1;
    stBumpBall.nCtactTime[1]     = m_nCtactTime2;
    stBumpBall.fCtactPower[0]    = (float)m_nCtactPower1 / (float)2.55;
    stBumpBall.fCtactPower[1]    = (float)m_nCtactPower2 / (float)2.55;
    stBumpBall.nCtactForce[0]    = m_nCtactForce1;
    stBumpBall.nCtactForce[1]    = m_nCtactForce2;
    stBumpBall.nBaseTime[0]      = m_nBondTime1;
    stBumpBall.nBaseTime[1]      = m_nBondTime2;
    stBumpBall.fBasePower[0]     = (float)m_nBondPower1 / (float)2.55;
    stBumpBall.fBasePower[1]     = (float)m_nBondPower2 / (float)2.55;
    stBumpBall.nBaseForce[0]     = m_nBondForce1;
    stBumpBall.nBaseForce[1]     = m_nBondForce2;
    stBumpBall.nSrchHeight1      = m_nSearchHeight1;
    stBumpBall.nSrchSpeed1       = m_iSearchSpeed1 + 1;
	stBumpBall.nSrchSpeed2       = m_iSearchSpeed2 == 0 ? 0 : m_iSearchSpeed2 + 1;
    stBumpBall.nSrchTol1         = m_nSearchTol1;
    stBumpBall.nSrchTol2         = m_nSearchTol2;
    stBumpBall.bTailBreakEnable  = m_iTailBreakEnable != 0;
    stBumpBall.chFabType         = m_iFabType;
    stBumpBall.bTailBondEnable   = m_iTailBondEnable != 0;
    stBumpBall.bStickDetect      = m_iStickDetection != 0;
    stBumpBall.nPreCtactTime[0]  = m_nPreCtactTime1;
    stBumpBall.nPreCtactTime[1]  = m_nPreCtactTime2;
    stBumpBall.nPreCtactForce[0] = m_nPreCtactForce1;
    stBumpBall.nPreCtactForce[1] = m_nPreCtactForce2;
    stBumpBall.nCtactSrchDelay1  = m_nSrchDelay1;
    stBumpBall.nRlsTime[0]       = m_nReleaseTime1;
    stBumpBall.nRlsTime[1]       = m_nReleaseTime2;
    stBumpBall.nRlsForce[0]      = m_nReleaseForce1;
    stBumpBall.nRlsForce[1]      = m_nReleaseForce2;
    stBumpBall.nRlsPower[0]      = m_nReleasePower1;
    stBumpBall.nRlsPower[1]      = m_nReleasePower2;
	//v3.1.T410 add
    stBumpBall.nPosOffset        = m_nPosOffset;
    stBumpBall.nSpanLenDelay     = m_nSpanLenDelay;
    stBumpBall.nArcSpan          = m_nArcSpan;
    stBumpBall.nArcSpanDelay     = m_nArcSpanDelay;
    if (m_iBallType == 0)
		theBallPara.SetBbosBall(&stBumpBall);
    else
		theBallPara.SetBsobBall(&stBumpBall);

    CPropertyPage::OnDestroy();
}

void CBallBondParaPage::OnFocusPrePower1()
{
	GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nStandbyPower1, m_nStandbyPower1,
				theBondPara.GetBondParaLimit()->nStandbyPowerL[0],
				theBondPara.GetBondParaLimit()->nStandbyPowerH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusPrePower2()
{
	GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nStandbyPower2, m_nStandbyPower2,
				theBondPara.GetBondParaLimit()->nStandbyPowerL[1],
				theBondPara.GetBondParaLimit()->nStandbyPowerH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusCtactTime1()
{
	GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactTime1, m_nCtactTime1,
				theBondPara.GetBondParaLimit()->nCtactTimeL[0],
				theBondPara.GetBondParaLimit()->nCtactTimeH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusCtactTime2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactTime2, m_nCtactTime2,
				theBondPara.GetBondParaLimit()->nCtactTimeL[1],
				theBondPara.GetBondParaLimit()->nCtactTimeH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusCtactPower1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactPower1, m_nCtactPower1,
				theBondPara.GetBondParaLimit()->nCtactPowerL[0],
				theBondPara.GetBondParaLimit()->nCtactPowerH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusCtactPower2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactPower2, m_nCtactPower2,
				theBondPara.GetBondParaLimit()->nCtactPowerL[1],
				theBondPara.GetBondParaLimit()->nCtactPowerH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusCtactForce1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactForce1, m_nCtactForce1,
				theBondPara.GetBondParaLimit()->nCtactForceL[0],
				theBondPara.GetBondParaLimit()->nCtactForceH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusCtactForce2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactForce2, m_nCtactForce2,
				theBondPara.GetBondParaLimit()->nCtactForceL[1],
				theBondPara.GetBondParaLimit()->nCtactForceH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusBondTime1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondTime1, m_nBondTime1,
				theBondPara.GetBondParaLimit()->nBaseTimeL[0],
				theBondPara.GetBondParaLimit()->nBaseTimeH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusBondTime2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondTime2, m_nBondTime2,
				theBondPara.GetBondParaLimit()->nBaseTimeL[1],
				theBondPara.GetBondParaLimit()->nBaseTimeH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusBondPower1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondPower1, m_nBondPower1,
				theBondPara.GetBondParaLimit()->nBasePowerL[0],
				theBondPara.GetBondParaLimit()->nBasePowerH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusBondPower2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondPower2, m_nBondPower2,
				theBondPara.GetBondParaLimit()->nBasePowerL[1],
				theBondPara.GetBondParaLimit()->nBasePowerH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusBondForce1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondForce1, m_nBondForce1,
				theBondPara.GetBondParaLimit()->nBaseForceL[0],
				theBondPara.GetBondParaLimit()->nBaseForceH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusBondForce2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondForce2, m_nBondForce2,
				theBondPara.GetBondParaLimit()->nBaseForceL[1],
				theBondPara.GetBondParaLimit()->nBaseForceH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusSrchHeight1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSearchHeight1, m_nSearchHeight1,
				theBondPara.GetBondParaLimit()->nSrchHeightL[0],
				theBondPara.GetBondParaLimit()->nSrchHeightH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusSrchTol1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSearchTol1, 20, 5, 100);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusSrchTol2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSearchTol2, 5, 3, 100);
    UpdateData(false);
}

void CBallBondParaPage::UpdateMenuVariable(void)
{
    ST_BUMP_BALL stBumpBall;
    if (m_iBallType == 0)
		theBallPara.GetBbosBall(&stBumpBall);
    else
		theBallPara.GetBsobBall(&stBumpBall);
    m_nStandbyPower1   = round(stBumpBall.fStandbyPower[0] * 2.55);
    m_nStandbyPower2   = round(stBumpBall.fStandbyPower[1] * 2.55);
    m_nCtactTime1      = stBumpBall.nCtactTime[0];
    m_nCtactTime2      = stBumpBall.nCtactTime[1];
    m_nCtactPower1     = round(stBumpBall.fCtactPower[0] * 2.55);
    m_nCtactPower2     = round(stBumpBall.fCtactPower[1] * 2.55);
    m_nCtactForce1     = stBumpBall.nCtactForce[0];
    m_nCtactForce2     = stBumpBall.nCtactForce[1];
    m_nBondTime1       = stBumpBall.nBaseTime[0];
    m_nBondTime2       = stBumpBall.nBaseTime[1];
    m_nBondPower1      = round(stBumpBall.fBasePower[0] * 2.55);
    m_nBondPower2      = round(stBumpBall.fBasePower[1] * 2.55);
    m_nBondForce1      = stBumpBall.nBaseForce[0];
    m_nBondForce2      = stBumpBall.nBaseForce[1];
    m_nSearchHeight1   = stBumpBall.nSrchHeight1;
    m_iSearchSpeed1    = stBumpBall.nSrchSpeed1 - 1;
	m_iSearchSpeed2    = stBumpBall.nSrchSpeed2 == 0 ? 0 : stBumpBall.nSrchSpeed2 - 1;
    m_nSearchTol1      = stBumpBall.nSrchTol1;
    m_nSearchTol2      = stBumpBall.nSrchTol2;
    m_iTailBreakEnable = stBumpBall.bTailBreakEnable;
    m_iFabType         = stBumpBall.chFabType;
    m_iTailBondEnable  = stBumpBall.bTailBondEnable;
    m_iStickDetection  = stBumpBall.bStickDetect;
    m_nPreCtactTime1   = stBumpBall.nPreCtactTime[0];
    m_nPreCtactTime2   = stBumpBall.nPreCtactTime[1];
    m_nPreCtactForce1  = stBumpBall.nPreCtactForce[0];
    m_nPreCtactForce2  = stBumpBall.nPreCtactForce[1];
    m_nSrchDelay1      = stBumpBall.nCtactSrchDelay1;
    m_nReleaseTime1    = stBumpBall.nRlsTime[0];
    m_nReleaseTime2    = stBumpBall.nRlsTime[1];
    m_nReleaseForce1   = stBumpBall.nRlsForce[0];
    m_nReleaseForce2   = stBumpBall.nRlsForce[1];
    m_nReleasePower1   = stBumpBall.nRlsPower[0];
    m_nReleasePower2   = stBumpBall.nRlsPower[1];
	//v3.1.T410 add
    m_nPosOffset       = stBumpBall.nPosOffset;
    m_nSpanLenDelay    = stBumpBall.nSpanLenDelay;
    m_nArcSpan         = stBumpBall.nArcSpan;
    m_nArcSpanDelay    = stBumpBall.nArcSpanDelay;

    UpdateData(false);
}

void CBallBondParaPage::OnFabType()
{
    UpdateData(true);
}

void CBallBondParaPage::OnTailBondEnable()
{
    UpdateData(true);
    TailBondMenuUpdate();
}

void CBallBondParaPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    short nOffset = 0;
    int nMin, nMax, iCurPos = GetScrollPos(SB_VERT);
    SCROLLINFO stScrollInfo;

    GetScrollInfo(SB_VERT, &stScrollInfo);
    nMin = stScrollInfo.nMin;
    nMax = stScrollInfo.nMax;

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
	case SB_THUMBTRACK:
		nOffset = nPos - iCurPos;
		break;
    default:
        break;
    }

    SetScrollPos(SB_VERT, iCurPos + nOffset);
    ScrollWindow(0, -nOffset);

    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBallBondParaPage::OnFocusPreImpactTime1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactTime1, m_nPreCtactTime1,
				theBondPara.GetBondParaLimit()->nPreCtactTimeL[0],
				theBondPara.GetBondParaLimit()->nPreCtactTimeH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusPreImpactTime2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactTime2, m_nPreCtactTime2,
				theBondPara.GetBondParaLimit()->nPreCtactTimeL[1],
				theBondPara.GetBondParaLimit()->nPreCtactTimeH[1]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusPreImpactForce1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactForce1, m_nPreCtactForce1,
				theBondPara.GetBondParaLimit()->nPreCtactForceL[0],
				theBondPara.GetBondParaLimit()->nPreCtactForceH[0]);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusPreImpactForce2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactForce2, m_nPreCtactForce2,
				theBondPara.GetBondParaLimit()->nPreCtactForceL[1],
				thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusSrchDelay1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchDelay1, 6, 2, 8);
    UpdateData(false);
}

void CBallBondParaPage::OnStickDetection()
{
    UpdateData(true);
}

void CBallBondParaPage::OnSrchSpeed1()
{
    UpdateData(true);
}

void CBallBondParaPage::OnSrchSpeed2()
{
    UpdateData(true);
}

void CBallBondParaPage::OnWireTailBreak()
{
    UpdateData(true);
}

void CBallBondParaPage::OnFocusRlsTime1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleaseTime1, 1, 0, 40);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusRlsTime2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleaseTime2, 1, 0, 40);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusRlsForce1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleaseForce1, 5, 0, 200);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusRlsForce2()
{
	GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nReleaseForce2, 5,
						0, thePgm.GetRcd()->nBondForceLimit);
	UpdateData(false);
}

void CBallBondParaPage::OnFocusRlsPower1()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleasePower1, 0, 0, 200);
    UpdateData(false);
}

void CBallBondParaPage::OnFocusRlsPower2()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleasePower2, 0, 0, 200);
    UpdateData(false);
}

void CBallBondParaPage::TailBondMenuUpdate(void)
{
    GetDlgItem(IDC_EDIT_PRE_POWER2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_CTACT_TIME2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_CTACT_POWER2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_CTACT_FORCE2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_BOND_TIME2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_BOND_POWER2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_BOND_FORCE2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_RLS_TIME2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_RLS_POWER2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_RLS_FORCE2)->EnableWindow(m_iTailBondEnable);
    GetDlgItem(IDC_EDIT_SRCH_TOL2)->EnableWindow(m_iTailBondEnable);
}

BOOL CBallBondParaPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, NULL);
	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CBallBondParaPage::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
	CPropertyPage::OnMouseMove(nFlags, point);
}

//v3.1.T410 add
void CBallBondParaPage::OnFocusPosOffset()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPosOffset);
    UpdateData(false);
}

//v3.1.T410 add
void CBallBondParaPage::OnFocusSpanLenDelay()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSpanLenDelay);
    UpdateData(false);
}

//v3.1.T410 add
void CBallBondParaPage::OnFocusArcSpan()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nArcSpan);
    UpdateData(false);
}

//v3.1.T410 add
void CBallBondParaPage::OnFocusArcSpanDelay()
{
    GetDlgItem(IDC_STATIC_PRE_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nArcSpanDelay);
    UpdateData(false);
}
