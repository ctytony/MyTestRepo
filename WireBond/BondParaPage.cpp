/***************************************************************
BondParaPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Bond.h"
#include "BondParaPage.h"
#include "User.h"

// CBondParaPage dialog

IMPLEMENT_DYNAMIC(CBondParaPage, CPropertyPage)

CBondParaPage::CBondParaPage()
	: CPropertyPage(CBondParaPage::IDD)
    , m_nCtactTime1(2)
    , m_nCtactTime2(1)
    , m_nCtactPower1(60)
    , m_nCtactPower2(60)
    , m_nCtactForce1(50)
    , m_nCtactForce2(50)
    , m_nBaseTime1(8)
    , m_nBaseTime2(6)
    , m_nBasePower1(60)
    , m_nBasePower2(60)
    , m_nBaseForce1(50)
    , m_nBaseForce2(50)
    , m_nRlsTime1(1)
    , m_nRlsTime2(1)
    , m_nRlsPower1(0)
    , m_nRlsPower2(0)
    , m_nRlsForce1(20)
    , m_nRlsForce2(0)
    , m_nPowerFactor(20)
    , m_nForceFactor(10)
    , m_nStandbyPower1(0)
    , m_nStandbyPower2(0)
{
}

CBondParaPage::~CBondParaPage()
{
}

void CBondParaPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PARA_CTACT_TIME1, m_nCtactTime1);
    DDX_Text(pDX, IDC_PARA_CTACT_TIME2, m_nCtactTime2);
    DDX_Text(pDX, IDC_PARA_CTACT_POWER1, m_nCtactPower1);
    DDX_Text(pDX, IDC_PARA_CTACT_POWER2, m_nCtactPower2);
    DDX_Text(pDX, IDC_PARA_CTACT_FORCE1, m_nCtactForce1);
    DDX_Text(pDX, IDC_PARA_CTACT_FORCE2, m_nCtactForce2);
    DDX_Text(pDX, IDC_PARA_BASE_TIME1, m_nBaseTime1);
    DDX_Text(pDX, IDC_PARA_BASE_TIME2, m_nBaseTime2);
    DDX_Text(pDX, IDC_PARA_BASE_POWER1, m_nBasePower1);
    DDX_Text(pDX, IDC_PARA_BASE_POWER2, m_nBasePower2);
    DDX_Text(pDX, IDC_PARA_BASE_FORCE1, m_nBaseForce1);
    DDX_Text(pDX, IDC_PARA_BASE_FORCE2, m_nBaseForce2);
    DDX_Text(pDX, IDC_PARA_RELEASE_TIME1, m_nRlsTime1);
    DDX_Text(pDX, IDC_PARA_RELEASE_TIME2, m_nRlsTime2);
    DDX_Text(pDX, IDC_PARA_RELEASE_POWER1, m_nRlsPower1);
    DDX_Text(pDX, IDC_PARA_RELEASE_POWER2, m_nRlsPower2);
    DDX_Text(pDX, IDC_PARA_RELEASE_FORCE1, m_nRlsForce1);
    DDX_Text(pDX, IDC_PARA_RELEASE_FORCE2, m_nRlsForce2);
    DDX_Text(pDX, IDC_PARA_POWER_FACTOR, m_nPowerFactor);
    DDX_Text(pDX, IDC_PARA_FORCE_FACTOR, m_nForceFactor);
    DDX_Text(pDX, IDC_PARA_STDBY_POWER1, m_nStandbyPower1);
    DDX_Text(pDX, IDC_PARA_STDBY_POWER2, m_nStandbyPower2);
}

BEGIN_MESSAGE_MAP(CBondParaPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_PARA_STDBY_POWER1, OnFocusParaStandbyPower1)
    ON_EN_SETFOCUS(IDC_PARA_STDBY_POWER2, OnFocusParaStandbyPower2)
    ON_EN_SETFOCUS(IDC_PARA_CTACT_TIME1, OnFocusParaCtactTime1)
    ON_EN_SETFOCUS(IDC_PARA_CTACT_TIME2, OnFocusParaCtactTime2)
    ON_EN_SETFOCUS(IDC_PARA_CTACT_POWER1, OnFocusParaCtactPower1)
    ON_EN_SETFOCUS(IDC_PARA_CTACT_POWER2, OnFocusParaCtactPower2)
    ON_EN_SETFOCUS(IDC_PARA_CTACT_FORCE1, OnFocusParaCtactForce1)
    ON_EN_SETFOCUS(IDC_PARA_CTACT_FORCE2, OnFocusParaCtactForce2)
    ON_EN_SETFOCUS(IDC_PARA_BASE_TIME1, OnFocusParaBaseTime1)
    ON_EN_SETFOCUS(IDC_PARA_BASE_TIME2, OnFocusParaBaseTime2)
    ON_EN_SETFOCUS(IDC_PARA_BASE_POWER1, OnFocusParaBasePower1)
    ON_EN_SETFOCUS(IDC_PARA_BASE_POWER2, OnFocusParaBasePower2)
    ON_EN_SETFOCUS(IDC_PARA_BASE_FORCE1, OnFocusParaBaseForce1)
    ON_EN_SETFOCUS(IDC_PARA_BASE_FORCE2, OnFocusParaBaseForce2)
    ON_EN_SETFOCUS(IDC_PARA_RELEASE_TIME1, OnFocusParaRlsTime1)
    ON_EN_SETFOCUS(IDC_PARA_RELEASE_TIME2, OnFocusParaRlsTime2)
    ON_EN_SETFOCUS(IDC_PARA_RELEASE_POWER1, OnFocusParaRlsPower1)
    ON_EN_SETFOCUS(IDC_PARA_RELEASE_POWER2, OnFocusParaRlsPower2)
    ON_EN_SETFOCUS(IDC_PARA_RELEASE_FORCE1, OnFocusParaRlsForce1)
    ON_EN_SETFOCUS(IDC_PARA_RELEASE_FORCE2, OnFocusParaRlsForce2)
    ON_EN_SETFOCUS(IDC_PARA_POWER_FACTOR, OnFocusParaPowerFactor)
    ON_EN_SETFOCUS(IDC_PARA_FORCE_FACTOR, OnFocusParaForceFactor)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CBondParaPage message handlers
BOOL CBondParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CWnd* pWnd = GetDlgItem(IDC_BOND_PARA_1ST_BOND);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT, PARA_COLUMN1_TOP, PARA_COLUMN1_WIDTH, PARA_COLUMN1_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BALL));

    pWnd = GetDlgItem(IDC_BOND_PARA_2ND_BOND);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT, PARA_COLUMN2_TOP, PARA_COLUMN1_WIDTH, PARA_COLUMN2_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_WEDGE));
    
    int iRowTop = PARA_LABEL_TOP;

    pWnd = GetDlgItem(IDC_STANDBY_POWER);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STANDBY_POWER));

    GetDlgItem(IDC_PARA_STDBY_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_PARA_STDBY_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_CTACT_TIME);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CTACT_TIME));

    GetDlgItem(IDC_PARA_CTACT_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_PARA_CTACT_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_CTACT_POWER);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CTACT_POWER));

    GetDlgItem(IDC_PARA_CTACT_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_PARA_CTACT_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_CTACT_FORCE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CTACT_FORCE));

    GetDlgItem(IDC_PARA_CTACT_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_PARA_CTACT_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_BASE_TIME);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BASE_TIME));

    GetDlgItem(IDC_PARA_BASE_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_PARA_BASE_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_BASE_POWER);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BASE_POWER));

    GetDlgItem(IDC_PARA_BASE_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_PARA_BASE_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_BASE_FORCE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BASE_FORCE));

    GetDlgItem(IDC_PARA_BASE_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_PARA_BASE_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_RELEASE_TIME);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RELEASE_TIME));

        GetDlgItem(IDC_PARA_RELEASE_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_PARA_RELEASE_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_RELEASE_POWER);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RELEASE_POWER));

        GetDlgItem(IDC_PARA_RELEASE_POWER1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_PARA_RELEASE_POWER2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_RELEASE_FORCE);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RELEASE_FORCE));

        GetDlgItem(IDC_PARA_RELEASE_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_PARA_RELEASE_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    }
    else
    {
        GetDlgItem(IDC_RELEASE_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_PARA_RELEASE_TIME1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_PARA_RELEASE_TIME2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RELEASE_POWER)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_PARA_RELEASE_POWER1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_PARA_RELEASE_POWER2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_RELEASE_FORCE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_PARA_RELEASE_FORCE1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_PARA_RELEASE_FORCE2)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_POWER_FACTOR);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH + 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_POWER_EQUALIZATION));

    GetDlgItem(IDC_PARA_POWER_FACTOR)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_FORCE_FACTOR);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH + 50, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FORCE_EQUALIZATION));

    GetDlgItem(IDC_PARA_FORCE_FACTOR)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
    m_nStandbyPower1 = round(pstBondPara->fStandbyPower[0] * 2.55);
    m_nStandbyPower2 = round(pstBondPara->fStandbyPower[1] * 2.55);
    m_nCtactTime1    = pstBondPara->nCtactTime[0];
    m_nCtactTime2    = pstBondPara->nCtactTime[1];
    m_nCtactPower1   = round(pstBondPara->fCtactPower[0] * 2.55);
    m_nCtactPower2   = round(pstBondPara->fCtactPower[1] * 2.55);
    m_nCtactForce1   = pstBondPara->nCtactForce[0];
    m_nCtactForce2   = pstBondPara->nCtactForce[1];
    m_nBaseTime1     = pstBondPara->nBaseTime[0];
    m_nBaseTime2     = pstBondPara->nBaseTime[1];
    m_nBasePower1    = round(pstBondPara->fBasePower[0] * 2.55);
    m_nBasePower2    = round(pstBondPara->fBasePower[1] * 2.55);
    m_nBaseForce1    = pstBondPara->nBaseForce[0];
    m_nBaseForce2    = pstBondPara->nBaseForce[1];
    m_nRlsTime1      = pstBondPara->nReleaseTime[0];
    m_nRlsTime2      = pstBondPara->nReleaseTime[1];
    m_nRlsPower1     = round(pstBondPara->fReleasePower[0] * 2.55);
    m_nRlsPower2     = round(pstBondPara->fReleasePower[1] * 2.55);
    m_nRlsForce1     = pstBondPara->nReleaseForce[0];
    m_nRlsForce2     = pstBondPara->nReleaseForce[1];
    m_nPowerFactor   = round(pstBondPara->fPowerFactor * 2.55);
    m_nForceFactor   = pstBondPara->nForceFactor;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CBondParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBondParaPage::OnFocusParaStandbyPower1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nStandbyPower1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nStandbyPower1, 0,
			theBondPara.GetBondParaLimit()->nStandbyPowerL[0],
			theBondPara.GetBondParaLimit()->nStandbyPowerH[0], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nStandbyPower1)
		theLog.WriteLong2(_T("Change Standby Power 1: %d->%d"), nOldPower, m_nStandbyPower1, true);
}

void CBondParaPage::OnFocusParaStandbyPower2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nStandbyPower2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nStandbyPower2, 0,
			theBondPara.GetBondParaLimit()->nStandbyPowerL[1],
			theBondPara.GetBondParaLimit()->nStandbyPowerH[1], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nStandbyPower2)
		theLog.WriteLong2(_T("Change Standby Power 2: %d->%d"), nOldPower, m_nStandbyPower2, true);
}

void CBondParaPage::OnFocusParaCtactTime1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldTime = m_nCtactTime1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactTime1, 1,
				theBondPara.GetBondParaLimit()->nCtactTimeL[0],
				theBondPara.GetBondParaLimit()->nCtactTimeH[0], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldTime != m_nCtactTime1)
		theLog.WriteLong2(_T("Change Contact Time 1: %d->%d"), nOldTime, m_nCtactTime1, true);
}

void CBondParaPage::OnFocusParaCtactTime2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldTime = m_nCtactTime2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactTime2, 1,
				theBondPara.GetBondParaLimit()->nCtactTimeL[1],
				theBondPara.GetBondParaLimit()->nCtactTimeH[1], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldTime != m_nCtactTime2)
		theLog.WriteLong2(_T("Change Contact Time 2: %d->%d"), nOldTime, m_nCtactTime2, true);
}

void CBondParaPage::OnFocusParaCtactPower1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nCtactPower1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactPower1, 0,
			theBondPara.GetBondParaLimit()->nCtactPowerL[0],
			theBondPara.GetBondParaLimit()->nCtactPowerH[0], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nCtactPower1)
		theLog.WriteLong2(_T("Change Contact Power 1: %d->%d"), nOldPower, m_nCtactPower1, true);
}

void CBondParaPage::OnFocusParaCtactPower2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nCtactPower2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactPower2, 0,
			theBondPara.GetBondParaLimit()->nCtactPowerL[1],
			theBondPara.GetBondParaLimit()->nCtactPowerH[1], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nCtactPower2)
		theLog.WriteLong2(_T("Change Contact Power 2: %d->%d"), nOldPower, m_nCtactPower2, true);
}

void CBondParaPage::OnFocusParaCtactForce1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldForce = m_nCtactForce1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactForce1, 10,
				theBondPara.GetBondParaLimit()->nCtactForceL[0],
				thePgm.GetRcd()->nBondForceLimit, true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldForce != m_nCtactForce1)
		theLog.WriteLong2(_T("Change Contact Force 1: %d->%d"), nOldForce, m_nCtactForce1, true);
}

void CBondParaPage::OnFocusParaCtactForce2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO)return;

	short nOldForce = m_nCtactForce2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactForce2, m_nCtactForce2,
				theBondPara.GetBondParaLimit()->nCtactForceL[1],
				thePgm.GetRcd()->nBondForceLimit, true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldForce != m_nCtactForce2)
		theLog.WriteLong2(_T("Change Contact Force 2: %d->%d"), nOldForce, m_nCtactForce2, true);
}

void CBondParaPage::OnFocusParaBaseTime1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldTime = m_nBaseTime1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBaseTime1, 8,
				theBondPara.GetBondParaLimit()->nBaseTimeL[0],
				theBondPara.GetBondParaLimit()->nBaseTimeH[0], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldTime != m_nBaseTime1)
		theLog.WriteLong2(_T("Change Base Time 1: %d->%d"), nOldTime, m_nBaseTime1, true);
}

void CBondParaPage::OnFocusParaBaseTime2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldTime = m_nBaseTime2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBaseTime2, m_nBaseTime2,
				theBondPara.GetBondParaLimit()->nBaseTimeL[1],
				theBondPara.GetBondParaLimit()->nBaseTimeH[1], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldTime != m_nBaseTime2)
		theLog.WriteLong2(_T("Change Base Time 2: %d->%d"), nOldTime, m_nBaseTime2, true);
}

void CBondParaPage::OnFocusParaBasePower1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nBasePower1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBasePower1, 15,
				theBondPara.GetBondParaLimit()->nBasePowerL[0],
				theBondPara.GetBondParaLimit()->nBasePowerH[0], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nBasePower1)
		theLog.WriteLong2(_T("Change Base Power 1: %d->%d"), nOldPower, m_nBasePower1, true);
}

void CBondParaPage::OnFocusParaBasePower2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nBasePower2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBasePower2, m_nBasePower2,
			theBondPara.GetBondParaLimit()->nBasePowerL[1],
			theBondPara.GetBondParaLimit()->nBasePowerH[1], true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nBasePower2)
		theLog.WriteLong2(_T("Change Base Power 2: %d->%d"), nOldPower, m_nBasePower2, true);
}

void CBondParaPage::OnFocusParaBaseForce1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldForce = m_nBaseForce1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBaseForce1, 30,
				theBondPara.GetBondParaLimit()->nBaseForceL[0],
				thePgm.GetRcd()->nBondForceLimit, true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldForce != m_nBaseForce1)
		theLog.WriteLong2(_T("Change Base Force 1: %d->%d"), nOldForce, m_nBaseForce1, true);
}

void CBondParaPage::OnFocusParaBaseForce2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldForce = m_nBaseForce2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBaseForce2, m_nBaseForce2,
				theBondPara.GetBondParaLimit()->nBaseForceL[1],
				thePgm.GetRcd()->nBondForceLimit, true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldForce != m_nBaseForce2)
		theLog.WriteLong2(_T("Change Base Force 2: %d->%d"), nOldForce, m_nBaseForce2, true);
}

void CBondParaPage::OnFocusParaPowerFactor()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldFactor = m_nPowerFactor;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPowerFactor, m_nPowerFactor,
				theBondPara.GetBondParaLimit()->nPowerFactorL,
				theBondPara.GetBondParaLimit()->nPowerFactorH, true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldFactor != m_nPowerFactor)
		theLog.WriteLong2(_T("Change Power Factor: %d->%d"), nOldFactor, m_nPowerFactor, true);
}

void CBondParaPage::OnFocusParaForceFactor()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldFactor = m_nForceFactor;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nForceFactor, m_nForceFactor,
				theBondPara.GetBondParaLimit()->nForceFactorL,
				theBondPara.GetBondParaLimit()->nForceFactorH, true);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldFactor != m_nForceFactor)
		theLog.WriteLong2(_T("Change Force Factor: %d->%d"), nOldFactor, m_nForceFactor, true);
}

void CBondParaPage::OnFocusParaRlsTime1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldTime = m_nRlsTime1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRlsTime1, 1, 0, 50);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldTime != m_nRlsTime1)
		theLog.WriteLong2(_T("Change Release Time 1: %d->%d"), nOldTime, m_nRlsTime1, true);
}

void CBondParaPage::OnFocusParaRlsTime2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldTime = m_nRlsTime2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRlsTime2, 1, 0, 50);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldTime != m_nRlsTime2)
		theLog.WriteLong2(_T("Change Release Time 2: %d->%d"), nOldTime, m_nRlsTime2, true);
}

void CBondParaPage::OnFocusParaRlsPower1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nRlsPower1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nRlsPower1, 0, 0, 100);	//v3.1.T564 delete
	//v3.1.T564 add
	theKeyPad.GetUserKeyInData(&m_nRlsPower1, m_nRlsPower1,
		theBondPara.GetBondParaLimit()->nRlsPowerL[0],
		theBondPara.GetBondParaLimit()->nRlsPowerH[0]);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nRlsPower1)
		theLog.WriteLong2(_T("Change Release Power 1: %d->%d"), nOldPower, m_nRlsPower1, true);
}

void CBondParaPage::OnFocusParaRlsPower2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldPower = m_nRlsPower2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nRlsPower2, 0, 0, 100);	//v3.1.T564 delete
	//v3.1.T564 add
	theKeyPad.GetUserKeyInData(&m_nRlsPower2, m_nRlsPower2,
		theBondPara.GetBondParaLimit()->nRlsPowerL[1],
		theBondPara.GetBondParaLimit()->nRlsPowerH[1]);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldPower != m_nRlsPower2)
		theLog.WriteLong2(_T("Change Release Power 2: %d->%d"), nOldPower, m_nRlsPower2, true);
}

void CBondParaPage::OnFocusParaRlsForce1()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldForce = m_nRlsForce1;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRlsForce1, m_nRlsForce1,
							0, thePgm.GetRcd()->nBondForceLimit);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldForce != m_nRlsForce1)
		theLog.WriteLong2(_T("Change Release Force 1: %d->%d"), nOldForce, m_nRlsForce1, true);
}

void CBondParaPage::OnFocusParaRlsForce2()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	short nOldForce = m_nRlsForce2;
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRlsForce2, m_nRlsForce2,
						0, thePgm.GetRcd()->nBondForceLimit);
	UpdateData(false);
	SetBondPara();	//v3.0.T365 add

	if (nOldForce != m_nRlsForce2)
		theLog.WriteLong2(_T("Change Release Force 2: %d->%d"), nOldForce, m_nRlsForce2, true);
}

void CBondParaPage::OnDestroy()	//V1.1.W151 add
{
	UpdateData();
	SetBondPara();	//v3.0.T365 add
    theApp.ArchivePowerOnData(ARCHIVE_SAVE);
	CPropertyPage::OnDestroy();
}

//v3.0.T365 add
void CBondParaPage::SetBondPara()
{
	BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
    pstBondPara->fStandbyPower[0] = (float)m_nStandbyPower1 / (float)2.55;
    pstBondPara->fStandbyPower[1] = (float)m_nStandbyPower2 / (float)2.55;
    pstBondPara->nCtactTime[0]    = m_nCtactTime1;
    pstBondPara->nCtactTime[1]    = m_nCtactTime2;
    pstBondPara->fCtactPower[0]   = (float)m_nCtactPower1 / (float)2.55;
    pstBondPara->fCtactPower[1]   = (float)m_nCtactPower2 / (float)2.55;
    pstBondPara->nCtactForce[0]   = m_nCtactForce1;
    pstBondPara->nCtactForce[1]   = m_nCtactForce2;
    pstBondPara->nBaseTime[0]     = m_nBaseTime1;
    pstBondPara->nBaseTime[1]     = m_nBaseTime2;
    pstBondPara->fBasePower[0]    = (float)m_nBasePower1 / (float)2.55;
    pstBondPara->fBasePower[1]    = (float)m_nBasePower2 / (float)2.55;
    pstBondPara->nBaseForce[0]    = m_nBaseForce1;
    pstBondPara->nBaseForce[1]    = m_nBaseForce2;
    pstBondPara->nReleaseTime[0]  = m_nRlsTime1;
    pstBondPara->nReleaseTime[1]  = m_nRlsTime2;
    pstBondPara->fReleasePower[0] = (float)m_nRlsPower1 / (float)2.55;
    pstBondPara->fReleasePower[1] = (float)m_nRlsPower2 / (float)2.55;
    pstBondPara->nReleaseForce[0] = m_nRlsForce1;
    pstBondPara->nReleaseForce[1] = m_nRlsForce2;
    pstBondPara->fPowerFactor     = (float)m_nPowerFactor / (float)2.55;
    pstBondPara->nForceFactor     = m_nForceFactor;
}
