/***************************************************************
AdvBondParaPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BondPara.h"
#include "AdvBondParaPage.h"
#include "Program.h"
#include "User.h"
#include <comutil.h>
#include "MachineConfig.h"    //v1.6.P8  add
// CAdvBondParaPage dialog
IMPLEMENT_DYNAMIC(CAdvBondParaPage, CPropertyPage)

CAdvBondParaPage::CAdvBondParaPage()
	: CPropertyPage(CAdvBondParaPage::IDD)
    , m_nSrchHeight1(0)
    , m_nSrchHeight2(0)
    , m_nSrchTol1(0)
    , m_nSrchTol2(0)
    , m_nBondShift2(0)
    , m_nPowerDelay1(0)
    , m_nPowerDelay2(0)
    , m_nTailShift(0)
    , m_nSrchDelay1(0)
    , m_nSrchDelay2(0)
    , m_nPreCtactForce1(0)
    , m_nPreCtactForce2(0)
    , m_nShiftSpeed(5)
    , m_nBondShift1(0)
    , m_iSrchSpeed1(2)
    , m_nPreCtactTime1(0)
    , m_nPreCtactTime2(0)
    , m_nPreShift2(0)
    , m_iTailBreakEnable(0)
    , m_iSrchSpeed2(0)
    , m_nSrchSpeedLowLimit1(0)
    , m_nSrchSpeedLowLimit2(0)
{
}

CAdvBondParaPage::~CAdvBondParaPage()
{
}

void CAdvBondParaPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SRCH_HT1, m_nSrchHeight1);
    DDX_Text(pDX, IDC_EDIT_SRCH_HT2, m_nSrchHeight2);
    DDX_Text(pDX, IDC_EDIT_POWER_DELAY_1, m_nPowerDelay1);
    DDX_Text(pDX, IDC_EDIT_POWER_DELAY_2, m_nPowerDelay2);
    DDX_Text(pDX, IDC_EDIT_SRCH_TOL1, m_nSrchTol1);
    DDX_Text(pDX, IDC_EDIT_SRCH_TOL2, m_nSrchTol2);
    DDX_Text(pDX, IDC_EDIT_BOND_SMOOTH, m_nBondShift2);
    DDX_Text(pDX, IDC_EDIT_TAIL_CREEP, m_nTailShift);
    DDX_Text(pDX, IDC_EDIT_SRCH_DELAY1, m_nSrchDelay1);
    DDX_Text(pDX, IDC_EDIT_SRCH_DELAY2, m_nSrchDelay2);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_FORCE1, m_nPreCtactForce1);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_FORCE2, m_nPreCtactForce2);
    DDX_Text(pDX, IDC_EDIT_SHIFT_SPEED, m_nShiftSpeed);
    DDX_Text(pDX, IDC_EDIT_1ST_BOND_SHIFT, m_nBondShift1);
    DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED1, m_iSrchSpeed1);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_TIME1, m_nPreCtactTime1);
    DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_TIME2, m_nPreCtactTime2);
    DDX_Text(pDX, IDC_EDIT_PRE_SHIFT2, m_nPreShift2);
    DDX_CBIndex(pDX, IDC_COMBO_TAIL_BREAK, m_iTailBreakEnable);
    DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED2, m_iSrchSpeed2);
}

BEGIN_MESSAGE_MAP(CAdvBondParaPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_HT1, OnFocusSrchHt1)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_HT2, OnFocusSrchHt2)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_TOL1, OnFocusSrchTol1)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_TOL2, OnFocusSrchTol2)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_SMOOTH, OnFocusBondSmooth)
    ON_EN_SETFOCUS(IDC_EDIT_POWER_DELAY_1, OnFocusPowerDelay1)
    ON_EN_SETFOCUS(IDC_EDIT_POWER_DELAY_2, OnFocusPowerDelay2)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_CREEP, OnFocusTailShift)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_DELAY1, OnFocusSrchDelay1)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_DELAY2, OnFocusSrchDelay2)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_FORCE1, OnFocusPreImpactForce1)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_FORCE2, OnFocusPreImpactForce2)
    ON_EN_SETFOCUS(IDC_EDIT_1ST_BOND_SHIFT, OnFocus1stBondShift)
    ON_EN_SETFOCUS(IDC_EDIT_SHIFT_SPEED, OnFocusShiftSpeed)
    ON_CBN_SELCHANGE(IDC_COMBO_SRCH_SPEED1, OnSrchSpeed1)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_TIME1, OnFocusPreImpactTime1)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_TIME2, OnFocusPreImpactTime2)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_SHIFT2, OnFocusPreShift2)
    ON_CBN_SELCHANGE(IDC_COMBO_SRCH_SPEED2, OnSrchSpeed2)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CAdvBondParaPage message handlers
BOOL CAdvBondParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CWnd* pWnd = GetDlgItem(IDC_BOND_PARA_1ST_BOND);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT, PARA_COLUMN1_TOP, PARA_COLUMN1_WIDTH, PARA_COLUMN1_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BALL));

    pWnd = GetDlgItem(IDC_BOND_PARA_2ND_BOND);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT, PARA_COLUMN2_TOP, PARA_COLUMN1_WIDTH, PARA_COLUMN2_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_WEDGE));
    
    int iRowTop = PARA_LABEL_TOP;

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        pWnd = GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
#if _SERVO_TYPE == 2	//v3.1.T432 add
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TOUCHDOWN_FORCE));
#else
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_IMPACT_FORCE));
#endif
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        
#if _SERVO_TYPE == 2	//v3.1.T432 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_SRCH_TOL);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TOUCHDOWN_FORCE_RANGE));
		GetDlgItem(IDC_EDIT_SRCH_TOL1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_SRCH_TOL2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
#endif

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
#if _SERVO_TYPE == 2	//v3.1.T432 add
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TOUCHDOWN_TIME));
#else
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PREIMPACT_TIME));
#endif
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    }
    else
    {
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME2)->ShowWindow(SW_HIDE);
    }

    pWnd = GetDlgItem(IDC_STATIC_POWER_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_POWER_DELAY));
    GetDlgItem(IDC_EDIT_POWER_DELAY_1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_POWER_DELAY_2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    pWnd = GetDlgItem(IDC_STATIC_SRCH_HT);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SRCH_HT));
    GetDlgItem(IDC_EDIT_SRCH_HT1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_SRCH_HT2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

#if _SERVO_TYPE != 2	//v3.1.T432 add
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	pWnd = GetDlgItem(IDC_STATIC_SRCH_TOL);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SRCH_THRES));
	GetDlgItem(IDC_EDIT_SRCH_TOL1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SRCH_TOL2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
#endif

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        if (theUser.GetUserLevel() > USER_PROCESS)
        {
            iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
            pWnd = GetDlgItem(IDC_STATIC_SRCH_DELAY);
            pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SRCH_DELAY));
            pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
            GetDlgItem(IDC_EDIT_SRCH_DELAY1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
            GetDlgItem(IDC_EDIT_SRCH_DELAY2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        }
        else
        {
            GetDlgItem(IDC_STATIC_SRCH_DELAY)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDIT_SRCH_DELAY1)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDIT_SRCH_DELAY2)->ShowWindow(SW_HIDE);
        }

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_SRCH_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SRCH_SPEED));
        GetDlgItem(IDC_COMBO_SRCH_SPEED1)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);       
        GetDlgItem(IDC_COMBO_SRCH_SPEED2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);     

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_SHIFT);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_SHIFT_2));
        GetDlgItem(IDC_EDIT_PRE_SHIFT2)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_BOND_SMOOTH);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BOND_SHIFT));
        GetDlgItem(IDC_EDIT_1ST_BOND_SHIFT)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_BOND_SMOOTH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_TAIL_CREEP);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TAIL_CREEP));
        GetDlgItem(IDC_EDIT_TAIL_CREEP)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_SHIFT_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH + PARA_COLUMN1_WIDTH + 15, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SHIFT_SPEED));
        GetDlgItem(IDC_EDIT_SHIFT_SPEED)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_TAIL_BREAK);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH + PARA_COLUMN1_WIDTH + 15, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TAIL_BREAK));
    }
    else
    {
        GetDlgItem(IDC_STATIC_SRCH_DELAY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SRCH_DELAY1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SRCH_DELAY2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_SRCH_SPEED1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_SRCH_SPEED2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_SHIFT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_SHIFT2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_BOND_SMOOTH)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_1ST_BOND_SHIFT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_BOND_SMOOTH)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TAIL_CREEP)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_TAIL_CREEP)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_SHIFT_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SHIFT_SPEED)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TAIL_BREAK)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_TAIL_BREAK)->ShowWindow(SW_HIDE);
    }

    short nLower = theBondPara.GetBondParaLimit()->nSrchSpeedL[0];
    short nUpper = theBondPara.GetBondParaLimit()->nSrchSpeedH[0];
    m_nSrchSpeedLowLimit1 = nLower;

	//v1.6.T18 add
	float fStep;
	if ((theMcConfig.GetAcsBufferType() == 1)
		|| (theMcConfig.GetSrchSpeedStep() == 1))
		fStep = 0.5;
	else
		fStep = 2.5;

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SRCH_SPEED1);
	short nSpeed;
    for (nSpeed = nLower; nSpeed <= nUpper; nSpeed++)
		pComboBox->AddString(_bstr_t(nSpeed * fStep));	//v1.6.T18 edit

    nLower = theBondPara.GetBondParaLimit()->nSrchSpeedL[1];
    nUpper = theBondPara.GetBondParaLimit()->nSrchSpeedH[1];
    m_nSrchSpeedLowLimit2 = nLower;

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SRCH_SPEED2);
    for (nSpeed = nLower; nSpeed <= nUpper; nSpeed++)
		pComboBox->AddString(_bstr_t(nSpeed * fStep));	//v1.6.T18 edit

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TAIL_BREAK);
    pComboBox->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
    pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));

    BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
    m_nPreCtactForce1	= pstBondPara->nPreCtactForce[0];
    m_nPreCtactForce2	= pstBondPara->nPreCtactForce[1];
    m_nPowerDelay1		= pstBondPara->nPowerDelay[0];
    m_nPowerDelay2		= pstBondPara->nPowerDelay[1];
    m_nSrchHeight1		= pstBondPara->nSrchHeight1;
    m_nSrchHeight2		= pstBondPara->nSrchHeight2;
    m_nSrchTol1			= pstBondPara->nSrchTol1;
    m_nSrchTol2			= pstBondPara->nSrchTol2;
    m_iSrchSpeed1		= pstBondPara->nSrchSpeed1 - m_nSrchSpeedLowLimit1;
    m_iSrchSpeed2		= pstBondPara->nSrchSpeed2 - m_nSrchSpeedLowLimit2;
    m_nBondShift2		= pstBondPara->nBondShift2;
    m_nTailShift		= pstBondPara->nTailShift;
    m_nSrchDelay1		= pstBondPara->nCtactSrchDelay[0];
#if _SERVO_TYPE == 2	//v3.1.T460 add
	m_nSrchDelay2	= pstBondPara->nCtactSrchDelay[1];
#else
	m_nSrchDelay2	= pstBondPara->nCtactSrchDelay[1] + 3;
#endif
    m_nShiftSpeed		= pstBondPara->nShiftSpeed;
    m_nBondShift1		= pstBondPara->nBondShift1;
    m_nPreCtactTime1	= pstBondPara->nPreCtactTime[0];
    m_nPreCtactTime2	= pstBondPara->nPreCtactTime[1];
    m_nPreShift2		= pstBondPara->nPreShift2;
    m_iTailBreakEnable	= pstBondPara->bTailBreakEnable;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CAdvBondParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
		pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAdvBondParaPage::OnFocusSrchHt1()
{
    long lLower = theBondPara.GetBondParaLimit()->nSrchHeightL[0];

	//v1.1.288.T17 delete
	//short nSrchDist = round(m_nSrchDelay1 * (m_iSrchSpeed1 + m_nSrchSpeedLowLimit1) * 2.5) + 20;
	//if (lLower < nSrchDist) lLower = nSrchDist;

	short nOldHeight = m_nSrchHeight1;

	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchHeight1, 150, lLower,
		theBondPara.GetBondParaLimit()->nSrchHeightH[0], true);
    UpdateData(false);

	if (nOldHeight != m_nSrchHeight1)
		theLog.WriteLong2(_T("Change Search Height 1: %d->%d"), nOldHeight, m_nSrchHeight1, true);
}

void CAdvBondParaPage::OnFocusSrchHt2()
{
    long lLower = theBondPara.GetBondParaLimit()->nSrchHeightL[1];

	//v1.1.288.T17 delete
	//short nSrchDist = round(m_nSrchDelay2 * (m_iSrchSpeed2 + m_nSrchSpeedLowLimit2) * 2.5) + 20;
	//if (lLower < nSrchDist) lLower = nSrchDist;

	short nOldHeight = m_nSrchHeight2;

	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchHeight2, 150, lLower,
			theBondPara.GetBondParaLimit()->nSrchHeightH[1], true);
    UpdateData(false);

	if (nOldHeight != m_nSrchHeight2)
		theLog.WriteLong2(_T("Change Search Height 2: %d->%d"), nOldHeight, m_nSrchHeight2, true);
}

void CAdvBondParaPage::OnFocusSrchTol1()
{
	short nOldTol = m_nSrchTol1;

	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchTol1, 0,
			theBondPara.GetBondParaLimit()->nSrchThreshL[0],
			theBondPara.GetBondParaLimit()->nSrchThreshH[0], true);
    UpdateData(false);

	if (nOldTol != m_nSrchTol1)
		theLog.WriteLong2(_T("Change Search Tol 1: %d->%d"), nOldTol, m_nSrchTol1, true);
}

void CAdvBondParaPage::OnFocusSrchTol2()
{
	short nOldTol = m_nSrchTol2;

	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();    
    theKeyPad.GetUserKeyInData(&m_nSrchTol2, 0,
			theBondPara.GetBondParaLimit()->nSrchThreshL[1],
			theBondPara.GetBondParaLimit()->nSrchThreshH[1], true);
    UpdateData(false);

	if (nOldTol != m_nSrchTol2)
		theLog.WriteLong2(_T("Change Search Tol 2: %d->%d"), nOldTol, m_nSrchTol2, true);
}

void CAdvBondParaPage::OnDestroy()
{
	UpdateData();

    BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
    pstBondPara->nPreCtactForce[0]  = m_nPreCtactForce1;
    pstBondPara->nPreCtactForce[1]  = m_nPreCtactForce2;
    pstBondPara->nPowerDelay[0]     = m_nPowerDelay1;
    pstBondPara->nPowerDelay[1]     = m_nPowerDelay2;
	pstBondPara->nSrchHeight1       = m_nSrchHeight1;
	pstBondPara->nSrchSpeed1        = m_iSrchSpeed1 + m_nSrchSpeedLowLimit1;
	pstBondPara->nSrchSpeed2        = m_iSrchSpeed2 + m_nSrchSpeedLowLimit2;
    pstBondPara->nSrchTol1          = m_nSrchTol1;
    pstBondPara->nSrchHeight2       = m_nSrchHeight2;
    pstBondPara->nSrchTol2          = m_nSrchTol2;
    pstBondPara->nBondShift2        = m_nBondShift2;
    pstBondPara->nTailShift         = m_nTailShift;
    pstBondPara->nCtactSrchDelay[0] = m_nSrchDelay1;
#if _SERVO_TYPE == 2	//v3.1.T460 add
	pstBondPara->nCtactSrchDelay[1] = m_nSrchDelay2;
#else
	pstBondPara->nCtactSrchDelay[1] = m_nSrchDelay2 - 3;
#endif
    pstBondPara->nShiftSpeed        = m_nShiftSpeed;
    pstBondPara->nBondShift1        = m_nBondShift1;
    pstBondPara->nPreCtactTime[0]   = m_nPreCtactTime1;
    pstBondPara->nPreCtactTime[1]   = m_nPreCtactTime2;
    pstBondPara->nPreShift2         = m_nPreShift2;
    pstBondPara->bTailBreakEnable   = m_iTailBreakEnable != 0;

    theApp.ArchivePowerOnData(ARCHIVE_SAVE);

	CPropertyPage::OnDestroy();
}

void CAdvBondParaPage::OnFocusBondSmooth()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondShift2, 0, -30, 30);
    UpdateData(false);
}

void CAdvBondParaPage::OnFocusPowerDelay1()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerDelay1, 0,
			theBondPara.GetBondParaLimit()->nPowerDelayL[0],
			theBondPara.GetBondParaLimit()->nPowerDelayH[0], true);
    UpdateData(false);
}

void CAdvBondParaPage::OnFocusPowerDelay2()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerDelay2, 0,
			theBondPara.GetBondParaLimit()->nPowerDelayL[1],
			theBondPara.GetBondParaLimit()->nPowerDelayH[1], true);
    UpdateData(false);
}

void CAdvBondParaPage::OnFocusTailShift()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailShift, 0, -30, 30);
    UpdateData(false);
}

void CAdvBondParaPage::OnFocusSrchDelay1()
{
	//v3.1.T295 add
	long lMin = 2;
#if _SERVO_TYPE == 2	//v3.1.T460 add
	lMin = 0;
#endif

	long lMax = 8;
	if (theMcConfig.GetAcsBufferType() == 1)
		lMax = 20;	//v1.6.P8 add
	else if (theMcConfig.GetAcsBufferType() == 2)
		lMax = 30;	//v1.6.T360 add

	//v1.1.288.T17 delete
	//if (m_iSrchSpeed1 + m_nSrchSpeedLowLimit1 != 0)
	//{
	//	lMaxSrchDelay = (long)((m_nSrchHeight1 - 20.0) / (m_iSrchSpeed1 + m_nSrchSpeedLowLimit1) / 2.5);
	//	if (lMaxSrchDelay > 8) lMaxSrchDelay = 8;
	//}

	short nOldDelay = m_nSrchDelay1;

	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSrchDelay1, 6, lMin, lMax);
	UpdateData(false);

	if (nOldDelay != m_nSrchDelay1)
		theLog.WriteLong2(_T("Change Search Delay 1: %d->%d"), nOldDelay, m_nSrchDelay1, true);
}

void CAdvBondParaPage::OnFocusSrchDelay2()
{
	//v3.1.T295 add
	long lMin = 2;
#if _SERVO_TYPE == 2	//v3.1.T460 add
	lMin = 0;
#endif

	long lMax = 8;
	if (theMcConfig.GetAcsBufferType() == 1)
		lMax = 20;	//v1.6.P8 add
	else if (theMcConfig.GetAcsBufferType() == 2)
		lMax = 30;	//v1.6.T360 add

	//v1.1.288.T17 delete
	//if (m_iSrchSpeed2 + m_nSrchSpeedLowLimit2 != 0)
	//{
	//	lMaxSrchDelay = (long)((m_nSrchHeight2 - 20.0) / (m_iSrchSpeed2 + m_nSrchSpeedLowLimit2) / 2.5);
	//	if (lMaxSrchDelay > 8) lMaxSrchDelay = 8;
	//}

	short nOldDelay = m_nSrchDelay2;

	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSrchDelay2, 6, lMin, lMax);
	UpdateData(false);

	if (nOldDelay != m_nSrchDelay2)
		theLog.WriteLong2(_T("Change Search Delay 2: %d->%d"), nOldDelay, m_nSrchDelay2, true);
}

void CAdvBondParaPage::OnFocusPreImpactForce1()
{
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T432 add
	theKeyPad.GetUserKeyInData(&m_nPreCtactForce1, 20,
				theBondPara.GetBondParaLimit()->nPreCtactForceL[0],
				theBondPara.GetBondParaLimit()->nPreCtactForceH[0]);
#else
	theKeyPad.GetUserKeyInData(&m_nPreCtactForce1, 50,
				theBondPara.GetBondParaLimit()->nPreCtactForceL[0],
				thePgm.GetRcd()->nBondForceLimit, true);
#endif
	UpdateData(false);
}

void CAdvBondParaPage::OnFocusPreImpactForce2()
{
	GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T432 add
	theKeyPad.GetUserKeyInData(&m_nPreCtactForce2, 20,
				theBondPara.GetBondParaLimit()->nPreCtactForceL[1],
				theBondPara.GetBondParaLimit()->nPreCtactForceH[1]);
#else
	theKeyPad.GetUserKeyInData(&m_nPreCtactForce2, 50,
				theBondPara.GetBondParaLimit()->nPreCtactForceL[1],
				thePgm.GetRcd()->nBondForceLimit, true);
#endif
	UpdateData(false);
}

void CAdvBondParaPage::OnFocus1stBondShift()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondShift1, 0, -30, 30);
    UpdateData(false);
}

void CAdvBondParaPage::OnFocusShiftSpeed()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nShiftSpeed, 5, 1, 50);
    UpdateData(false);
}

void CAdvBondParaPage::OnSrchSpeed1()
{
	int iBkupSpeed = m_iSrchSpeed1;

	UpdateData(true);

	if (iBkupSpeed != m_iSrchSpeed1)
		theLog.WriteLong2(_T("Change Search Speed 1: %d->%d"), iBkupSpeed, m_iSrchSpeed1, true);

	//if (m_iSrchSpeed1 + m_nSrchSpeedLowLimit1 == 0) return;	//v1.5.T1 delete

	//short nSrchDist = round(m_nSrchDelay1 * (m_iSrchSpeed1 + m_nSrchSpeedLowLimit1) * 2.5) + 20;	//v1.5.T18 delete

	//if (nSrchDist <= m_nSrchHeight1) return;	//v1.5.T1 delete

	//v1.5.T18 delete
	//m_iSrchSpeed1 = iBkupSpeed;
	//UpdateData(false);
}

void CAdvBondParaPage::OnFocusPreImpactTime1()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactTime1, 1,
			theBondPara.GetBondParaLimit()->nPreCtactTimeL[0],
			theBondPara.GetBondParaLimit()->nPreCtactTimeH[0], true);
    UpdateData(false);
}

void CAdvBondParaPage::OnFocusPreImpactTime2()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactTime2, 1,
			theBondPara.GetBondParaLimit()->nPreCtactTimeL[1],
			theBondPara.GetBondParaLimit()->nPreCtactTimeH[1], true);
    UpdateData(false);
}

void CAdvBondParaPage::OnFocusPreShift2()
{
    GetDlgItem(IDC_BOND_PARA_1ST_BOND)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreShift2, 0, -30, 30);
    UpdateData(false);
}

void CAdvBondParaPage::OnSrchSpeed2()
{
	int iBkupSpeed = m_iSrchSpeed2;

	UpdateData(true);

	if (iBkupSpeed != m_iSrchSpeed2)
		theLog.WriteLong2(_T("Change Search Speed 2: %d->%d"), iBkupSpeed, m_iSrchSpeed2, true);

	//if (m_iSrchSpeed2 + m_nSrchSpeedLowLimit2 == 0) return;	//v1.5.T1 delete

	//short nSrchDist = round(m_nSrchDelay2 * (m_iSrchSpeed2 + m_nSrchSpeedLowLimit2) * 2.5) + 20;	//v1.5.T18 delete

	//if (nSrchDist <= m_nSrchHeight2) return;	//v1.5.T1 delete

	//v1.5.T18 delete
	//m_iSrchSpeed2 = iBkupSpeed;
	//UpdateData(false);
}
