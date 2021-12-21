/***************************************************************
BumpWireParaPage.cpp : implementation file
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
#include "BumpWireParaPage.h"
#include "Program.h"
#include "User.h"
#include "MachineConfig.h"

// CBumpWireParaPage dialog

IMPLEMENT_DYNAMIC(CBumpWireParaPage, CPropertyPage)

CBumpWireParaPage::CBumpWireParaPage()
	: CPropertyPage(CBumpWireParaPage::IDD)
    , m_iBallType(1)
    , m_nBaseTime(0)
    , m_nBasePower(0)
    , m_nBaseForce(0)
    , m_nBondPointOffset(0)
    , m_iSearchSpeed(0)
    , m_iTailBreakEnable(0)
    , m_nSrchTol(0)
    , m_nStandbyPower(0)
    , m_nPreCtactTime(0)
    , m_nPreCtactForce(0)
    , m_nCtactTime(0)
    , m_nCtactPower(0)
    , m_nCtactForce(0)
    , m_nReleaseTime(0)
    , m_nReleasePower(0)
    , m_nReleaseForce(0)
    , m_nSrchHeight(0)
    , m_nSrchDelay(0)
    , m_nPageSize(0)
    , m_nPreShift(0)
    , m_nBondShift(0)
    , m_nTailShift(0)
    , m_nShiftSpeed(0)
{
}

CBumpWireParaPage::~CBumpWireParaPage()
{
}

void CBumpWireParaPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_BASE_TIME, m_nBaseTime);
    DDX_Text(pDX, IDC_EDIT_BOND_POWER, m_nBasePower);
    DDX_Text(pDX, IDC_EDIT_BASE_FORCE, m_nBaseForce);
    DDX_Text(pDX, IDC_EDIT_BP_OFFSET, m_nBondPointOffset);
    DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED, m_iSearchSpeed);
    DDX_CBIndex(pDX, IDC_COMBO_TAIL_BREAK, m_iTailBreakEnable);
    DDX_Text(pDX, IDC_EDIT_SRCH_TOL, m_nSrchTol);
    DDX_Text(pDX, IDC_EDIT_STANDBY_POWER, m_nStandbyPower);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_TIME, m_nPreCtactTime);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_FORCE, m_nPreCtactForce);
    DDX_Text(pDX, IDC_EDIT_IMPACT_TIME, m_nCtactTime);
    DDX_Text(pDX, IDC_EDIT_IMPACT_POWER, m_nCtactPower);
    DDX_Text(pDX, IDC_EDIT_IMPACT_FORCE, m_nCtactForce);
    DDX_Text(pDX, IDC_EDIT_RELEASE_TIME, m_nReleaseTime);
    DDX_Text(pDX, IDC_EDIT_RELEASE_POWER, m_nReleasePower);
    DDX_Text(pDX, IDC_EDIT_RELEASE_FORCE, m_nReleaseForce);
    DDX_Text(pDX, IDC_EDIT_SRCH_HEIGHT, m_nSrchHeight);
    DDX_Text(pDX, IDC_EDIT_SRCH_DELAY, m_nSrchDelay);
    DDX_Text(pDX, IDC_EDIT_PRE_SHIFT, m_nPreShift);
    DDX_Text(pDX, IDC_EDIT_BOND_SHIFT, m_nBondShift);
    DDX_Text(pDX, IDC_EDIT_TAIL_SHIFT, m_nTailShift);
    DDX_Text(pDX, IDC_EDIT_SHIFT_SPEED, m_nShiftSpeed);
}

BEGIN_MESSAGE_MAP(CBumpWireParaPage, CPropertyPage)
    ON_EN_SETFOCUS(IDC_EDIT_BASE_TIME, OnFocusBaseTime)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_POWER, OnFocusBondPower)
    ON_EN_SETFOCUS(IDC_EDIT_BASE_FORCE, OnFocusBaseForce)
    ON_EN_SETFOCUS(IDC_EDIT_BP_OFFSET, OnFocusBpOffset)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_TOL, OnFocusSrchTol)
    ON_WM_VSCROLL()
    ON_EN_SETFOCUS(IDC_EDIT_STANDBY_POWER, OnFocusStandbyPower)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_TIME, OnFocusPreImpactTime)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_FORCE, OnFocusPreImpactForce)
    ON_EN_SETFOCUS(IDC_EDIT_IMPACT_TIME, OnFocusImpactTime)
    ON_EN_SETFOCUS(IDC_EDIT_IMPACT_POWER, OnFocusImpactPower)
    ON_EN_SETFOCUS(IDC_EDIT_IMPACT_FORCE, OnFocusImpactForce)
    ON_EN_SETFOCUS(IDC_EDIT_RELEASE_TIME, OnFocusReleaseTime)
    ON_EN_SETFOCUS(IDC_EDIT_RELEASE_POWER, OnFocusReleasePower)
    ON_EN_SETFOCUS(IDC_EDIT_RELEASE_FORCE, OnFocusReleaseForce)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_HEIGHT, OnFocusSrchHeight)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_DELAY, OnFocusSrchDelay)
    ON_CBN_SELCHANGE(IDC_COMBO_SRCH_SPEED, OnSrchSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_SHIFT, OnFocusPreShift)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_SHIFT, OnFocusBondShift)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_SHIFT, OnFocusTailShift)
    ON_EN_SETFOCUS(IDC_EDIT_SHIFT_SPEED, OnFocusShiftSpeed)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CBumpWireParaPage message handlers
BOOL CBumpWireParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
    m_iBallType = ((CBallParaSheet*)GetParent())->GetBumpBallType();
    UpdateMenuVariable();
    return CPropertyPage::OnInitDialog();
}

HBRUSH CBumpWireParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBumpWireParaPage::OnFocusBaseTime()
{
	GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseTime, m_nBaseTime,
				theBondPara.GetBondParaLimit()->nBaseTimeL[1],
				theBondPara.GetBondParaLimit()->nBaseTimeH[1]);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusBondPower()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBasePower, m_nBasePower,
				theBondPara.GetBondParaLimit()->nBasePowerL[1],
				theBondPara.GetBondParaLimit()->nBasePowerH[1]);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusBaseForce()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseForce, m_nBaseForce,
				theBondPara.GetBondParaLimit()->nBaseForceL[1],
				thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusBpOffset()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondPointOffset, 0, -100, 100);
    UpdateData(false);
}

void CBumpWireParaPage::UpdateMenuVariable(void)
{
    int iRowTop = PARA_COLUMN1_TOP;
    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_TIME,theMsg.GetMsg(MSG_PREIMPACT_TIME));
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    }
    else
    {
        GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME)->ShowWindow(SW_HIDE);
    }

    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_STANDBY_POWER,theMsg.GetMsg(MSG_STANDBY_POWER));
    GetDlgItem(IDC_EDIT_STANDBY_POWER)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_FORCE,theMsg.GetMsg(MSG_PRE_IMPACT_FORCE));
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
		GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_IMPACT_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_IMPACT_TIME,theMsg.GetMsg(MSG_CTACT_TIME));
    GetDlgItem(IDC_EDIT_IMPACT_TIME)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_IMPACT_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_IMPACT_POWER,theMsg.GetMsg(MSG_CTACT_POWER));
    GetDlgItem(IDC_EDIT_IMPACT_POWER)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_IMPACT_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_IMPACT_FORCE,theMsg.GetMsg(MSG_CTACT_FORCE));
    GetDlgItem(IDC_EDIT_IMPACT_FORCE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_BASE_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BASE_TIME,theMsg.GetMsg(MSG_BASE_TIME));
    GetDlgItem(IDC_EDIT_BASE_TIME)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_BASE_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BASE_POWER,theMsg.GetMsg(MSG_BASE_POWER));
    GetDlgItem(IDC_EDIT_BOND_POWER)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_BASE_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BASE_FORCE,theMsg.GetMsg(MSG_BASE_FORCE));
    GetDlgItem(IDC_EDIT_BASE_FORCE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_RELEASE_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_RELEASE_TIME,theMsg.GetMsg(MSG_RELEASE_TIME));
        GetDlgItem(IDC_EDIT_RELEASE_TIME)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_RELEASE_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_RELEASE_POWER,theMsg.GetMsg(MSG_RELEASE_POWER));
        GetDlgItem(IDC_EDIT_RELEASE_POWER)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_RELEASE_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_RELEASE_FORCE,theMsg.GetMsg(MSG_RELEASE_FORCE));
        GetDlgItem(IDC_EDIT_RELEASE_FORCE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
        GetDlgItem(IDC_STATIC_RELEASE_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RELEASE_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RELEASE_POWER)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RELEASE_POWER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RELEASE_FORCE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_RELEASE_FORCE)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_SRCH_HEIGHT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_SRCH_HEIGHT,theMsg.GetMsg(MSG_SRCH_HT));
    GetDlgItem(IDC_EDIT_SRCH_HEIGHT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SRCH_SPEED,theMsg.GetMsg(MSG_SRCH_SPEED));
        GetDlgItem(IDC_COMBO_SRCH_SPEED)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_SRCH_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SRCH_DELAY,theMsg.GetMsg(MSG_SRCH_DELAY));
        GetDlgItem(IDC_EDIT_SRCH_DELAY)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_SRCH_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SRCH_DELAY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SRCH_DELAY)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_SRCH_TOL)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_SRCH_TOL,theMsg.GetMsg(MSG_SRCH_THRES));
    GetDlgItem(IDC_EDIT_SRCH_TOL)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_PRE_SHIFT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRE_SHIFT,theMsg.GetMsg(MSG_PRE_SHIFT_2));
        GetDlgItem(IDC_EDIT_PRE_SHIFT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_BOND_SHIFT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_BOND_SHIFT,theMsg.GetMsg(MSG_BOND_SHIFT));
        GetDlgItem(IDC_EDIT_BOND_SHIFT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_TAIL_SHIFT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_TAIL_SHIFT,theMsg.GetMsg(MSG_TAIL_CREEP));
        GetDlgItem(IDC_EDIT_TAIL_SHIFT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_SHIFT_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SHIFT_SPEED,theMsg.GetMsg(MSG_SHIFT_SPEED));
        GetDlgItem(IDC_EDIT_SHIFT_SPEED)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
        GetDlgItem(IDC_STATIC_PRE_SHIFT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_SHIFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BOND_SHIFT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_BOND_SHIFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_SHIFT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_TAIL_SHIFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SHIFT_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SHIFT_SPEED)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_BP_OFFSET)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BP_OFFSET,theMsg.GetMsg(MSG_2ND_BOND_OFFSET));
    GetDlgItem(IDC_EDIT_BP_OFFSET)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        GetDlgItem(IDC_STATIC_TAIL_BREAK)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_TAIL_BREAK,theMsg.GetMsg(MSG_TAIL_BREAK));

		CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TAIL_BREAK);
        pComboBox->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
        pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));
    }
    else
    {
        GetDlgItem(IDC_STATIC_TAIL_BREAK)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_TAIL_BREAK)->ShowWindow(SW_HIDE);
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

	ST_BUMP_WIRE stBumpWire;
    if (m_iBallType == 0)
		theBallPara.GetBbosWire(&stBumpWire);
    else
		theBallPara.GetBsobWire(&stBumpWire);
    m_nBaseTime        = stBumpWire.nBaseTime;
    m_nBasePower       = round(stBumpWire.fBasePower * 2.55);
    m_nBaseForce       = stBumpWire.nBaseForce;
    m_iSearchSpeed     = stBumpWire.nSrchSpeed2 - 2;
    m_nSrchTol         = stBumpWire.nSrchTol2;
    m_nStandbyPower    = round(stBumpWire.fStandbyPower * 2.55);
    m_nPreCtactTime    = stBumpWire.nPreCtactTime;
    m_nPreCtactForce   = stBumpWire.nPreCtactForce;
    m_nCtactTime       = stBumpWire.nCtactTime;
    m_nCtactPower      = round(stBumpWire.fCtactPower * 2.55);
    m_nCtactForce      = stBumpWire.nCtactForce;
    m_nReleaseTime     = stBumpWire.nReleaseTime;
    m_nReleasePower    = round(stBumpWire.fReleasePower * 2.55);
    m_nReleaseForce    = stBumpWire.nReleaseForce;
    m_nSrchHeight      = stBumpWire.nSrchHeight;
#if _SERVO_TYPE == 2	//v3.1.T460 add
	m_nSrchDelay       = stBumpWire.nCtactSrchDelay;
#else
	m_nSrchDelay       = stBumpWire.nCtactSrchDelay + 3;
#endif
    m_nPreShift        = stBumpWire.nPreShift;
    m_nBondShift       = stBumpWire.nBondShift;
    m_nTailShift       = stBumpWire.nTailShift;
    m_nShiftSpeed      = stBumpWire.nShiftSpeed;
    m_nBondPointOffset = stBumpWire.nBondPointOffset;
    m_iTailBreakEnable = stBumpWire.bTailBreakEnable;

    UpdateData(false);
}

void CBumpWireParaPage::OnDestroy()
{
	UpdateData();

    ST_BUMP_WIRE stBumpWire;
    if (m_iBallType == 0)
		theBallPara.GetBbosWire(&stBumpWire);
    else
		theBallPara.GetBsobWire(&stBumpWire);
    stBumpWire.nBaseTime        = m_nBaseTime;
    stBumpWire.fBasePower       = (float)m_nBasePower / (float)2.55;
    stBumpWire.nBaseForce       = m_nBaseForce;
    stBumpWire.nSrchSpeed2      = m_iSearchSpeed + 2;
    stBumpWire.nSrchTol2        = m_nSrchTol;
    stBumpWire.fStandbyPower    = (float)m_nStandbyPower / (float)2.55;
    stBumpWire.nPreCtactTime    = m_nPreCtactTime;
    stBumpWire.nPreCtactForce   = m_nPreCtactForce;
    stBumpWire.nCtactTime       = m_nCtactTime;
    stBumpWire.fCtactPower      = (float)m_nCtactPower / (float)2.55;
    stBumpWire.nCtactForce      = m_nCtactForce;
    stBumpWire.nReleaseTime     = m_nReleaseTime;
    stBumpWire.fReleasePower    = (float)m_nReleasePower / (float)2.55;
    stBumpWire.nReleaseForce    = m_nReleaseForce;
    stBumpWire.nSrchHeight      = m_nSrchHeight;
#if _SERVO_TYPE == 2	//v3.1.T460 add
	stBumpWire.nCtactSrchDelay  = m_nSrchDelay;
#else
	stBumpWire.nCtactSrchDelay  = m_nSrchDelay - 3;
#endif
    stBumpWire.nPreShift        = m_nPreShift;
    stBumpWire.nBondShift       = m_nBondShift;
    stBumpWire.nTailShift       = m_nTailShift;
    stBumpWire.nShiftSpeed      = m_nShiftSpeed;
    stBumpWire.nBondPointOffset = m_nBondPointOffset;
    stBumpWire.bTailBreakEnable = m_iTailBreakEnable != 0;
    if (m_iBallType == 0)
		theBallPara.SetBbosWire(&stBumpWire);
    else
		theBallPara.SetBsobWire(&stBumpWire);

    CPropertyPage::OnDestroy();
}

void CBumpWireParaPage::OnFocusSrchTol()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchTol, 12, 5, 80);
    UpdateData(false);
}

void CBumpWireParaPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
        if (iCurPos == nMax) break;

        nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        if (iCurPos + nOffset > nMax)
            nOffset = nMax - iCurPos;
        break;
    case SB_LINEUP:
        if (iCurPos == nMin) break;

        nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE);
        if (iCurPos + nOffset < nMin)
            nOffset = nMin - iCurPos;
        break;
    case SB_PAGEDOWN:
        if (iCurPos == nMax) break;
        nOffset = m_nPageSize - iCurPos;
        break;
    case SB_PAGEUP:
        if (iCurPos == nMin) break;
        nOffset = -iCurPos;
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

void CBumpWireParaPage::OnFocusStandbyPower()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nStandbyPower, m_nStandbyPower,
				theBondPara.GetBondParaLimit()->nStandbyPowerL[1],
				theBondPara.GetBondParaLimit()->nStandbyPowerH[1]);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusPreImpactTime()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactTime, m_nPreCtactTime,
				theBondPara.GetBondParaLimit()->nPreCtactTimeL[1],
				theBondPara.GetBondParaLimit()->nPreCtactTimeH[1]);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusPreImpactForce()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactForce, m_nPreCtactForce,
				theBondPara.GetBondParaLimit()->nPreCtactForceL[1],
				thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusImpactTime()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactTime, m_nCtactTime,
				theBondPara.GetBondParaLimit()->nCtactTimeL[1],
				theBondPara.GetBondParaLimit()->nCtactTimeH[1]);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusImpactPower()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactPower, m_nCtactPower,
				theBondPara.GetBondParaLimit()->nCtactPowerL[1],
				theBondPara.GetBondParaLimit()->nCtactPowerH[1]);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusImpactForce()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactForce, m_nCtactForce,
				theBondPara.GetBondParaLimit()->nCtactForceL[1],
				thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusReleaseTime()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleaseTime, m_nReleaseTime, 0, 40);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusReleasePower()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleasePower, m_nReleasePower, 0, 200);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusReleaseForce()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleaseForce, m_nReleaseForce,
				theBondPara.GetBondParaLimit()->nCtactForceL[1],
				thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusSrchHeight()
{
    long lLowLmt = theBondPara.GetBondParaLimit()->nSrchHeightL[1];
    short nSrchDist = round(m_nSrchDelay * (m_iSearchSpeed + 2.0) * 2.5);
    if (lLowLmt < nSrchDist) lLowLmt = nSrchDist;

    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchHeight, 150, lLowLmt,
			theBondPara.GetBondParaLimit()->nSrchHeightH[1], true);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusSrchDelay()
{
	GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
	//v3.1.T392 delete
	//short nMaxSrchDelay = (short)(m_nSrchHeight / (m_iSearchSpeed + 2.0) / 2.5);
	//if (nMaxSrchDelay > 8) nMaxSrchDelay = 8;
	//theKeyPad.GetUserKeyInData(&m_nSrchDelay, 6, 2, nMaxSrchDelay);
	theKeyPad.GetUserKeyInData(&m_nSrchDelay, 6, 2, 15);	//v3.1.T392 add
	UpdateData(false);
}

void CBumpWireParaPage::OnSrchSpeed()
{
    int iBkupSpeed = m_iSearchSpeed;
    UpdateData(true);

    short nSrchDist = round(m_nSrchDelay * (m_iSearchSpeed + 2.0) * 2.5);
    if (nSrchDist > m_nSrchHeight)
    {
        m_iSearchSpeed = iBkupSpeed;
        UpdateData(false);
    }
}

void CBumpWireParaPage::OnFocusPreShift()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreShift, 0, -30, 30);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusBondShift()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondShift, 0, -30, 30);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusTailShift()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailShift, 0, -30, 30);
    UpdateData(false);
}

void CBumpWireParaPage::OnFocusShiftSpeed()
{
    GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nShiftSpeed, 10, 1, 50);
    UpdateData(false);
}

BOOL CBumpWireParaPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, NULL);
	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CBumpWireParaPage::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_STANDBY_POWER)->SetFocus();
	CPropertyPage::OnMouseMove(nFlags, point);
}
