// CfgBondPage.cpp : implementation file
//

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "BondPara.h"
#include "CfgBondPage.h"
#include "User.h"

// CCfgBondPage dialog

IMPLEMENT_DYNAMIC(CCfgBondPage, CPropertyPage)

CCfgBondPage::CCfgBondPage()
	: CPropertyPage(CCfgBondPage::IDD)
    , m_nPowerOffset1(0)
    , m_nPowerOffset2(0)
    , m_nForceOffset1(0)
    , m_nForceOffset2(0)
    , m_nTimeOffset1(0)
    , m_nTimeOffset2(0)
    , m_nPowerCompSlope(0)
    , m_nPowerCompOs(0)
    , m_nTailBreakDist(50)
    , m_iTailBreakSpeed(2)
    , m_lCapiWearOutCount1(0)
    , m_lCapiWearOutCount2(0)
    , m_dCapiWearOutPowerOs1(0.0)
    , m_dCapiWearOutPowerOs2(0.0)
{
}

CCfgBondPage::~CCfgBondPage()
{
}

void CCfgBondPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_POWER_OS_1, m_nPowerOffset1);
    DDX_Text(pDX, IDC_EDIT_POWER_OS_2, m_nPowerOffset2);
    DDX_Text(pDX, IDC_EDIT_FORCE_OS_1, m_nForceOffset1);
    DDX_Text(pDX, IDC_EDIT_FORCE_OS_2, m_nForceOffset2);
    DDX_Text(pDX, IDC_EDIT_TIME_OS_1, m_nTimeOffset1);
    DDX_Text(pDX, IDC_EDIT_TIME_OS_2, m_nTimeOffset2);
    DDX_Text(pDX, IDC_EDIT_PWR_SLOPE, m_nPowerCompSlope);
    DDX_Text(pDX, IDC_EDIT_PWR_OFFSET, m_nPowerCompOs);
    DDX_Text(pDX, IDC_EDIT_TAIL_BREAK_DIST, m_nTailBreakDist);
    DDX_CBIndex(pDX, IDC_COMBO_TAIL_BREAK_SPEED, m_iTailBreakSpeed);
    DDX_Text(pDX, IDC_EDIT_T1, m_lCapiWearOutCount1);
    DDX_Text(pDX, IDC_EDIT_T2, m_lCapiWearOutCount2);
    DDX_Text(pDX, IDC_EDIT_CAPI_POWER_OS1, m_dCapiWearOutPowerOs1);
    DDX_Text(pDX, IDC_EDIT_CAPI_POWER_OS2, m_dCapiWearOutPowerOs2);
}

BEGIN_MESSAGE_MAP(CCfgBondPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_POWER_OS_1, OnFocusPowerOs1)
    ON_EN_SETFOCUS(IDC_EDIT_POWER_OS_2, OnFocusPowerOs2)
    ON_EN_SETFOCUS(IDC_EDIT_FORCE_OS_1, OnFocusForceOs1)
    ON_EN_SETFOCUS(IDC_EDIT_FORCE_OS_2, OnFocusForceOs2)
    ON_EN_SETFOCUS(IDC_EDIT_TIME_OS_1, OnFocusTimeOs1)
    ON_EN_SETFOCUS(IDC_EDIT_TIME_OS_2, OnFocusTimeOs2)
    ON_EN_SETFOCUS(IDC_EDIT_PWR_SLOPE, OnFocusPwrSlope)
    ON_EN_SETFOCUS(IDC_EDIT_PWR_OFFSET, OnFocusPwrOffset)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_BREAK_DIST, OnFocusTailBreakDist)
    ON_EN_SETFOCUS(IDC_EDIT_T1, OnFocusT1)
    ON_EN_SETFOCUS(IDC_EDIT_T2, OnFocusT2)
    ON_EN_SETFOCUS(IDC_EDIT_CAPI_POWER_OS1, OnFocusCapiPowerOs1)
    ON_EN_SETFOCUS(IDC_EDIT_CAPI_POWER_OS2, OnFocusCapiPowerOs2)
	ON_WM_DESTROY()		//V1.4.W25 add
END_MESSAGE_MAP()

// CCfgBondPage message handlers
BOOL CCfgBondPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        SetDlgItemTextW(IDC_STATIC_CRT_BOND_OFFSET,theMsg.GetMsg(MSG_CORRECT_BOND_OFFSET));
        SetDlgItemTextW(IDC_STATIC_BALL_BOND,theMsg.GetMsg(MSG_BALL));
        SetDlgItemTextW(IDC_STATIC_WEDGE_BOND,theMsg.GetMsg(MSG_WEDGE));
        SetDlgItemTextW(IDC_STATIC_POWER,theMsg.GetMsg(MSG_BASE_POWER));
        SetDlgItemTextW(IDC_STATIC_FORCE,theMsg.GetMsg(MSG_BASE_FORCE));
        SetDlgItemTextW(IDC_STATIC_TIME,theMsg.GetMsg(MSG_BASE_TIME));
        SetDlgItemTextW(IDC_STATIC_POWER_COMPENSATION,theMsg.GetMsg(MSG_POWER_COMPENSATION));
        SetDlgItemTextW(IDC_STATIC_FACTOR,theMsg.GetMsg(MSG_FACTORS_COLON));

        GetDlgItem(IDC_EDIT_PWR_SLOPE)->EnableWindow(theUser.GetUserLevel() > USER_SERVICE);
        GetDlgItem(IDC_EDIT_PWR_OFFSET)->EnableWindow(theUser.GetUserLevel() > USER_SERVICE);
    }
    else
    {
        GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_BALL_BOND)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_WEDGE_BOND)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_POWER)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_POWER_OS_1)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_POWER_OS_2)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_FORCE)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_FORCE_OS_1)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_FORCE_OS_2)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_TIME)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_TIME_OS_1)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_TIME_OS_2)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_POWER_COMPENSATION)->ShowWindow(false);
        GetDlgItem(IDC_STATIC_FACTOR)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_PWR_SLOPE)->ShowWindow(false);
        GetDlgItem(IDC_EDIT_PWR_OFFSET)->ShowWindow(false);
    }

    SetDlgItemTextW(IDC_STATIC_TAIL_BREAK,theMsg.GetMsg(MSG_TAIL_BREAK));
    SetDlgItemTextW(IDC_STATIC_TAIL_BREAK_DIST,theMsg.GetMsg(MSG_DISTANCE));
    SetDlgItemTextW(IDC_STATIC_TAIL_BREAK_SPEED,theMsg.GetMsg(MSG_SPEED));
    SetDlgItemTextW(IDC_STATIC_CAPI_PWR_COMP,theMsg.GetMsg(MSG_CAP_WEAR_OUT_POWER_COMPENSATE));
    SetDlgItemTextW(IDC_STATIC_TIME_TICK,theMsg.GetMsg(MSG_START_END_COUNT));
    SetDlgItemTextW(IDC_STATIC_POWER_OS,theMsg.GetMsg(MSG_POWER_COMPENSATE));

    BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
    m_nPowerOffset1   = round(pstBondPara->fCrtBondPowerOffset1 * 2.55);
    m_nPowerOffset2   = round(pstBondPara->fCrtBondPowerOffset2 * 2.55);
    m_nForceOffset1   = pstBondPara->nCrtBondForceOffset1;
    m_nForceOffset2   = pstBondPara->nCrtBondForceOffset2;
    m_nTimeOffset1    = pstBondPara->nCrtBondTimeOffset1;
    m_nTimeOffset2    = pstBondPara->nCrtBondTimeOffset2;
    m_nTailBreakDist  = pstBondPara->nTailBreakDist;
    m_iTailBreakSpeed = pstBondPara->nTailBreakSpeed - 1;	//v3.1.T366 edit

    MACHINE_DATA* pstMcData = theMcData.GetRcd();
    m_nPowerCompSlope      = pstMcData->nPowerSlope;
    m_nPowerCompOs         = pstMcData->nPowerOffset;
    m_lCapiWearOutCount1   = pstMcData->lPwrCompOffsetCapiCount;
    m_lCapiWearOutCount2   = pstMcData->lPwrCompEndCapiCount;
    m_dCapiWearOutPowerOs1 = pstMcData->fPwrCompOs1;	//v1.1.288.T15 edit
    m_dCapiWearOutPowerOs2 = pstMcData->fPwrCompOs2;	//v1.1.288.T15 add

    return CPropertyPage::OnInitDialog();
}

HBRUSH CCfgBondPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }
    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCfgBondPage::OnDestroy()
{
    UpdateData();

    BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
    pstBondPara->fCrtBondPowerOffset1 = (float)m_nPowerOffset1 / (float)2.55;
    pstBondPara->fCrtBondPowerOffset2 = (float)m_nPowerOffset2 / (float)2.55;
    pstBondPara->nCrtBondForceOffset1 = m_nForceOffset1;
    pstBondPara->nCrtBondForceOffset2 = m_nForceOffset2;
    pstBondPara->nCrtBondTimeOffset1  = m_nTimeOffset1;
    pstBondPara->nCrtBondTimeOffset2  = m_nTimeOffset2;
    pstBondPara->nTailBreakDist       = m_nTailBreakDist;
    pstBondPara->nTailBreakSpeed      = m_iTailBreakSpeed + 1;	//v3.1.T366 edit

    MACHINE_DATA* pstMcData = theMcData.GetRcd();
    pstMcData->nPowerSlope             = m_nPowerCompSlope;
    pstMcData->nPowerOffset            = m_nPowerCompOs;
    pstMcData->lPwrCompOffsetCapiCount = m_lCapiWearOutCount1;
    pstMcData->lPwrCompEndCapiCount    = m_lCapiWearOutCount2;
    pstMcData->fPwrCompOs1             = (float)m_dCapiWearOutPowerOs1;	//v1.1.288.T15 edit
    pstMcData->fPwrCompOs2             = (float)m_dCapiWearOutPowerOs2;	//v1.1.288.T15 add

    CPropertyPage::OnDestroy();
}

void CCfgBondPage::OnFocusPowerOs1()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerOffset1, 5, 0, 10);
    UpdateData(false);
}

void CCfgBondPage::OnFocusPowerOs2()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerOffset2, 5, 0, 10);
    UpdateData(false);
}

void CCfgBondPage::OnFocusForceOs1()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nForceOffset1, 0, 0, 50);
    UpdateData(false);
}

void CCfgBondPage::OnFocusForceOs2()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nForceOffset2, 0, 0, 50);
    UpdateData(false);
}

void CCfgBondPage::OnFocusTimeOs1()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTimeOffset1, 0, 0, 20);
    UpdateData(false);
}

void CCfgBondPage::OnFocusTimeOs2()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTimeOffset2, 0, 0, 20);
    UpdateData(false);
}

void CCfgBondPage::OnFocusPwrSlope()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerCompSlope, 100, 80, 150);
    UpdateData(false);
}

void CCfgBondPage::OnFocusPwrOffset()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerCompOs, 0, -10, 10);
    UpdateData(false);
}

void CCfgBondPage::OnFocusTailBreakDist()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailBreakDist, 50, 10, 300);
    UpdateData(false);
}

void CCfgBondPage::OnFocusT1()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lCapiWearOutCount1);
    UpdateData(false);
}

void CCfgBondPage::OnFocusT2()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lCapiWearOutCount2);
    UpdateData(false);
}

//v1.1.288.T15 edit
void CCfgBondPage::OnFocusCapiPowerOs1()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_dCapiWearOutPowerOs1, -10, -50, 50);
    UpdateData(false);
}

//v1.1.288.T15 add
void CCfgBondPage::OnFocusCapiPowerOs2()
{
    GetDlgItem(IDC_STATIC_CRT_BOND_OFFSET)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_dCapiWearOutPowerOs2, -10, -50, 50);
    UpdateData(false);
}
