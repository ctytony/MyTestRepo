/***************************************************************
ParaLimitPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BondPara.h"
#include "ParaLimitPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "Bond.h"
#include "Program.h"
#include "User.h"
#include <memory>

// CParaLimitPage dialog

IMPLEMENT_DYNAMIC(CParaLimitPage, CPropertyPage)

CParaLimitPage::CParaLimitPage()
	: CPropertyPage(CParaLimitPage::IDD)
    , m_nStandbyPower1(0)
    , m_nStandbyPower2(50)
    , m_nCtactTime1(0)
    , m_nCtactTime2(5)
    , m_nCtactPower1(0)
    , m_nCtactPower2(50)
    , m_nCtactForce1(0)
    , m_nCtactForce2(100)
    , m_nBaseTime1(0)
    , m_nBaseTime2(20)
    , m_dBasePower1(0)
    , m_dBasePower2(50)
    , m_nBaseForce1(0)
    , m_nBaseForce2(100)
    , m_nPowerFactor1(0)
    , m_nPowerFactor2(50)
    , m_nForceFactor1(0)
    , m_nForceFactor2(100)
    , m_nPowerDelay1(0)
    , m_nPowerDelay2(5)
    , m_nSrchHeight1(0)
    , m_nSrchHeight2(100)
    , m_nSrchThresh1(0)
    , m_nSrchThresh2(100)
    , m_SelectStep(0)
	, m_nPowerH(100)
    , m_nPreCtactForceL(0)
    , m_nPreCtactForceH(0)
    , m_nPreCtactTimeL(0)
    , m_nPreCtactTimeH(0)
    , m_nSparkDelayL(0)
    , m_nSparkDelayH(0)
    , m_nTailLengthL(0)
    , m_nTailLengthH(0)
    , m_nPageSize(0)
    , m_nTailLength2L(0)
    , m_nTailLength2H(0)
    , m_nFireLevelL(0)
    , m_nFireLevelH(0)
    , m_nRlsForceL(0)
    , m_nRlsForceH(0)
	, m_nSrchSpeedL(0)
	, m_nSrchSpeedH(0)
	//v3.1.T564 add
    , m_nRlsPowerL(0)
    , m_nRlsPowerH(0)
{
}

CParaLimitPage::~CParaLimitPage()
{
}

void CParaLimitPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PREPOWER_L, m_nStandbyPower1);
	DDX_Text(pDX, IDC_EDIT_PREPOWER_U, m_nStandbyPower2);
	DDX_Text(pDX, IDC_EDIT_COMPATIME_L, m_nCtactTime1);
	DDX_Text(pDX, IDC_EDIT_COMPATIME_U, m_nCtactTime2);
	DDX_Text(pDX, IDC_EDIT_COMPOWER_L, m_nCtactPower1);
	DDX_Text(pDX, IDC_EDIT_COMPOWER_U, m_nCtactPower2);
	DDX_Text(pDX, IDC_EDIT_COMPAPRES_L, m_nCtactForce1);
	DDX_Text(pDX, IDC_EDIT_COMPAPRES_U, m_nCtactForce2);
	DDX_Text(pDX, IDC_EDIT_BONDTIME_L, m_nBaseTime1);
	DDX_Text(pDX, IDC_EDIT_BONDTIME_U, m_nBaseTime2);
	DDX_Text(pDX, IDC_EDIT_BONDPWR_L, m_dBasePower1);
	DDX_Text(pDX, IDC_EDIT_BONDPWR_U, m_dBasePower2);
	DDX_Text(pDX, IDC_EDIT_BONDPRES_L, m_nBaseForce1);
	DDX_Text(pDX, IDC_EDIT_BONDPRES_U, m_nBaseForce2);
	DDX_Text(pDX, IDC_EDIT_PWRAVEINDEX_L, m_nPowerFactor1);
	DDX_Text(pDX, IDC_EDIT_PWRAVEINDEX_U, m_nPowerFactor2);
	DDX_Text(pDX, IDC_EDIT_PRESAVE_INDEX_L, m_nForceFactor1);
	DDX_Text(pDX, IDC_EDIT_PRESAVE_INDEX_U, m_nForceFactor2);
	DDX_Text(pDX, IDC_EDIT_USGDELAY_L, m_nPowerDelay1);
	DDX_Text(pDX, IDC_EDIT_USGDELAY_U, m_nPowerDelay2);
	DDX_Text(pDX, IDC_EDIT_SEARCHHIGH_L, m_nSrchHeight1);
	DDX_Text(pDX, IDC_EDIT_SEARCHHIGH_U, m_nSrchHeight2);
	DDX_Text(pDX, IDC_EDIT_SEARCHTHRESH_L, m_nSrchThresh1);
	DDX_Text(pDX, IDC_EDIT_SEARCHTHRESH_U, m_nSrchThresh2);
	DDX_CBIndex(pDX, IDC_COMBO_LIMIT, m_SelectStep);
	DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_FORCE_LOW, m_nPreCtactForceL);
	DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_FORCE_HIGH, m_nPreCtactForceH);
	DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_TIME_LOW, m_nPreCtactTimeL);
	DDX_Text(pDX, IDC_EDIT_PRE_IMPACT_TIME_HIGH, m_nPreCtactTimeH);
	DDX_Text(pDX, IDC_EDIT_SPARK_DELAY_LOW, m_nSparkDelayL);
	DDX_Text(pDX, IDC_EDIT_SPARK_DELAY_HIGH, m_nSparkDelayH);
	DDX_Text(pDX, IDC_EDIT_TAIL_LENGTH_LOW, m_nTailLengthL);
	DDX_Text(pDX, IDC_EDIT_TAIL_LENGTH_HIGH, m_nTailLengthH);
	DDX_Text(pDX, IDC_EDIT_TAIL_LENGTH_2_LOW, m_nTailLength2L);
	DDX_Text(pDX, IDC_EDIT_TAIL_LENGTH_2_HIGH, m_nTailLength2H);
	DDX_Text(pDX, IDC_EDIT_FIRE_LEVEL_L, m_nFireLevelL);
	DDX_Text(pDX, IDC_EDIT_FIRE_LEVEL_H, m_nFireLevelH);
	DDX_Text(pDX, IDC_EDIT_POST_DAMP_FORCE_LOW, m_nRlsForceL);
	DDX_Text(pDX, IDC_EDIT_POST_DAMP_FORCE_HIGH, m_nRlsForceH);
	DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED_L, m_nSrchSpeedL);
	DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED_H, m_nSrchSpeedH);
	//v3.1.T564 add
	DDX_Text(pDX, IDC_EDIT_POST_DAMP_POWER_LOW, m_nRlsPowerL);
	DDX_Text(pDX, IDC_EDIT_POST_DAMP_POWER_HIGH, m_nRlsPowerH);
}

BEGIN_MESSAGE_MAP(CParaLimitPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_PREPOWER_L, OnFocusPrepowerL)
    ON_EN_SETFOCUS(IDC_EDIT_PREPOWER_U, OnFocusPrepowerU)
    ON_EN_SETFOCUS(IDC_EDIT_COMPATIME_L, OnFocusCompatimeL)
    ON_EN_SETFOCUS(IDC_EDIT_COMPATIME_U, OnFocusCompatimeU)
    ON_EN_SETFOCUS(IDC_EDIT_COMPOWER_L, OnFocusCompowerL)
    ON_EN_SETFOCUS(IDC_EDIT_COMPOWER_U, OnFocusCompowerU)
    ON_EN_SETFOCUS(IDC_EDIT_COMPAPRES_L, OnFocusCompapresL)
    ON_EN_SETFOCUS(IDC_EDIT_COMPAPRES_U, OnFocusCompapresU)
    ON_EN_SETFOCUS(IDC_EDIT_BONDTIME_L, OnFocusBondtimeL)
    ON_EN_SETFOCUS(IDC_EDIT_BONDTIME_U, OnFocusBondtimeU)
    ON_EN_SETFOCUS(IDC_EDIT_BONDPWR_L, OnFocusBondpwrL)
    ON_EN_SETFOCUS(IDC_EDIT_BONDPWR_U, OnFocusBondpwrU)
    ON_EN_SETFOCUS(IDC_EDIT_BONDPRES_L, OnFocusBondpresL)
    ON_EN_SETFOCUS(IDC_EDIT_BONDPRES_U, OnFocusBondpresU)
    ON_EN_SETFOCUS(IDC_EDIT_PWRAVEINDEX_L, OnFocusPwraveindexL)
    ON_EN_SETFOCUS(IDC_EDIT_PWRAVEINDEX_U, OnFocusPwraveindexU)
    ON_EN_SETFOCUS(IDC_EDIT_PRESAVE_INDEX_L, OnFocusPresaveIndexL)
    ON_EN_SETFOCUS(IDC_EDIT_PRESAVE_INDEX_U, OnFocusPresaveIndexU)
    ON_EN_SETFOCUS(IDC_EDIT_USGDELAY_L, OnFocusUsgdelayL)
    ON_EN_SETFOCUS(IDC_EDIT_USGDELAY_U, OnFocusUsgdelayU)
    ON_EN_SETFOCUS(IDC_EDIT_SEARCHHIGH_L, OnFocusSearchhighL)
    ON_EN_SETFOCUS(IDC_EDIT_SEARCHHIGH_U, OnFocusSearchhighU)
    ON_EN_SETFOCUS(IDC_EDIT_SEARCHTHRESH_L, OnFocusSearchthreshL)
    ON_EN_SETFOCUS(IDC_EDIT_SEARCHTHRESH_U, OnFocusSearchthreshU)
    ON_CBN_SELCHANGE(IDC_COMBO_LIMIT, OnLimit)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_FORCE_LOW, OnFocusPreImpactForceLow)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_FORCE_HIGH, OnFocusPreImpactForceHigh)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_TIME_LOW, OnFocusPreImpactTimeLow)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_IMPACT_TIME_HIGH, OnFocusPreImpactTimeHigh)
    ON_EN_SETFOCUS(IDC_EDIT_SPARK_DELAY_LOW, OnFocusSparkDelayLow)
    ON_EN_SETFOCUS(IDC_EDIT_SPARK_DELAY_HIGH, OnFocusSparkDelayHigh)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_LENGTH_LOW, OnFocusTailLengthLow)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_LENGTH_HIGH, OnFocusTailLengthHigh)
    ON_WM_VSCROLL()
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_LENGTH_2_LOW, OnFocusTailLength2Low)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_LENGTH_2_HIGH, OnFocusTailLength2High)
    ON_EN_SETFOCUS(IDC_EDIT_FIRE_LEVEL_L, OnFocusFireLevelL)
    ON_EN_SETFOCUS(IDC_EDIT_FIRE_LEVEL_H, OnFocusFireLevelH)
    ON_EN_SETFOCUS(IDC_EDIT_POST_DAMP_FORCE_LOW, OnFocusPostDampForceLow)
    ON_EN_SETFOCUS(IDC_EDIT_POST_DAMP_FORCE_HIGH, OnFocusPostDampForceHigh)
	//v3.1.T564 add
    ON_EN_SETFOCUS(IDC_EDIT_POST_DAMP_POWER_LOW, OnFocusPostDampPowerLow)
    ON_EN_SETFOCUS(IDC_EDIT_POST_DAMP_POWER_HIGH, OnFocusPostDampPowerHigh)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()		//V1.4.W25 add
END_MESSAGE_MAP()

void CParaLimitPage::OnDestroy()
{
    CPropertyPage::OnDestroy();

    UpdateData(TRUE);

	BOND_PARA_LIMIT* pstBondParaLimit = theBondPara.GetBondParaLimit();

    if (m_SelectStep == 1)
    {
        pstBondParaLimit->nStandbyPowerL[1]  = m_nStandbyPower1;
        pstBondParaLimit->nStandbyPowerH[1]  = m_nStandbyPower2;
        pstBondParaLimit->nCtactTimeL[1]     = m_nCtactTime1;
        pstBondParaLimit->nCtactTimeH[1]     = m_nCtactTime2;
        pstBondParaLimit->nCtactPowerL[1]    = m_nCtactPower1;
        pstBondParaLimit->nCtactPowerH[1]    = m_nCtactPower2;
        pstBondParaLimit->nCtactForceL[1]    = m_nCtactForce1;
        pstBondParaLimit->nCtactForceH[1]    = m_nCtactForce2;
        pstBondParaLimit->nBaseTimeL[1]      = m_nBaseTime1;
        pstBondParaLimit->nBaseTimeH[1]      = m_nBaseTime2;
        pstBondParaLimit->nBasePowerL[1]     = m_dBasePower1;
        pstBondParaLimit->nBasePowerH[1]     = m_dBasePower2;
        pstBondParaLimit->nBaseForceL[1]     = m_nBaseForce1;
        pstBondParaLimit->nBaseForceH[1]     = m_nBaseForce2;
        pstBondParaLimit->nPowerDelayL[1]    = m_nPowerDelay1;
        pstBondParaLimit->nPowerDelayH[1]    = m_nPowerDelay2;
        pstBondParaLimit->nSrchHeightL[1]    = m_nSrchHeight1;
        pstBondParaLimit->nSrchHeightH[1]    = m_nSrchHeight2;
        pstBondParaLimit->nSrchThreshL[1]    = m_nSrchThresh1;
        pstBondParaLimit->nSrchThreshH[1]    = m_nSrchThresh2;

        pstBondParaLimit->nPowerFactorL      = m_nPowerFactor1;
        pstBondParaLimit->nPowerFactorH      = m_nPowerFactor2;
        pstBondParaLimit->nForceFactorL      = m_nForceFactor1;
        pstBondParaLimit->nForceFactorH      = m_nForceFactor2;

        pstBondParaLimit->nPreCtactForceL[1] = m_nPreCtactForceL;
        pstBondParaLimit->nPreCtactForceH[1] = m_nPreCtactForceH;
        pstBondParaLimit->nPreCtactTimeL[1]  = m_nPreCtactTimeL;
        pstBondParaLimit->nPreCtactTimeH[1]  = m_nPreCtactTimeH;
        pstBondParaLimit->nTailLengthL       = m_nTailLengthL;
        pstBondParaLimit->nTailLengthH       = m_nTailLengthH;
        pstBondParaLimit->nSparkDelayL       = m_nSparkDelayL;
        pstBondParaLimit->nSparkDelayH       = m_nSparkDelayH;

        pstBondParaLimit->nTailLength2L      = m_nTailLength2L;
        pstBondParaLimit->nTailLength2H      = m_nTailLength2H;
        pstBondParaLimit->nFireLevelL        = m_nFireLevelL;
        pstBondParaLimit->nFireLevelH        = m_nFireLevelH;

		pstBondParaLimit->nSrchSpeedL[1]     = m_nSrchSpeedL * m_nScaleFactor;			//v1.6.T18 edit
		pstBondParaLimit->nSrchSpeedH[1]     = (m_nSrchSpeedH + 1) * m_nScaleFactor;	//v1.6.T18 edit
		//v3.1.T564 add
		pstBondParaLimit->nRlsPowerL[1]      = m_nRlsPowerL;
        pstBondParaLimit->nRlsPowerH[1]      = m_nRlsPowerH;
    }
    else if (m_SelectStep == 0)
    {
        pstBondParaLimit->nStandbyPowerL[0]  = m_nStandbyPower1;
        pstBondParaLimit->nStandbyPowerH[0]  = m_nStandbyPower2;
        pstBondParaLimit->nCtactTimeL[0]     = m_nCtactTime1;
        pstBondParaLimit->nCtactTimeH[0]     = m_nCtactTime2;
        pstBondParaLimit->nCtactPowerL[0]    = m_nCtactPower1;
        pstBondParaLimit->nCtactPowerH[0]    = m_nCtactPower2;
        pstBondParaLimit->nCtactForceL[0]    = m_nCtactForce1;
        pstBondParaLimit->nCtactForceH[0]    = m_nCtactForce2;
        pstBondParaLimit->nBaseTimeL[0]      = m_nBaseTime1;
        pstBondParaLimit->nBaseTimeH[0]      = m_nBaseTime2;
        pstBondParaLimit->nBasePowerL[0]     = m_dBasePower1;
        pstBondParaLimit->nBasePowerH[0]     = m_dBasePower2;
        pstBondParaLimit->nBaseForceL[0]     = m_nBaseForce1;
        pstBondParaLimit->nBaseForceH[0]     = m_nBaseForce2;
        pstBondParaLimit->nPowerDelayL[0]    = m_nPowerDelay1;
        pstBondParaLimit->nPowerDelayH[0]    = m_nPowerDelay2;
        pstBondParaLimit->nSrchHeightL[0]    = m_nSrchHeight1;
        pstBondParaLimit->nSrchHeightH[0]    = m_nSrchHeight2;
        pstBondParaLimit->nSrchThreshL[0]    = m_nSrchThresh1;
        pstBondParaLimit->nSrchThreshH[0]    = m_nSrchThresh2;

        pstBondParaLimit->nPreCtactForceL[0] = m_nPreCtactForceL;
        pstBondParaLimit->nPreCtactForceH[0] = m_nPreCtactForceH;
        pstBondParaLimit->nPreCtactTimeL[0]  = m_nPreCtactTimeL;
        pstBondParaLimit->nPreCtactTimeH[0]  = m_nPreCtactTimeH;

		pstBondParaLimit->nSrchSpeedL[0]     = m_nSrchSpeedL * m_nScaleFactor;			//v1.6.T18 edit
		pstBondParaLimit->nSrchSpeedH[0]     = (m_nSrchSpeedH + 1) * m_nScaleFactor;	//v1.6.T18 edit

		pstBondParaLimit->nRlsForceL[0]      = m_nRlsForceL;
        pstBondParaLimit->nRlsForceH[0]      = m_nRlsForceH;
		//v3.1.T564 add
		pstBondParaLimit->nRlsPowerL[0]      = m_nRlsPowerL;
        pstBondParaLimit->nRlsPowerH[0]      = m_nRlsPowerH;
    }

    theApp.ArchivePowerOnData(ARCHIVE_SAVE);
}

// CParaLimitPage message handlers
BOOL CParaLimitPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    ShowMenuItem();

	BOND_PARA_LIMIT* pstBondParaLimit = theBondPara.GetBondParaLimit();

    m_nStandbyPower1  = pstBondParaLimit->nStandbyPowerL[0];
    m_nStandbyPower2  = pstBondParaLimit->nStandbyPowerH[0];
    m_nCtactTime1     = pstBondParaLimit->nCtactTimeL[0];
    m_nCtactTime2     = pstBondParaLimit->nCtactTimeH[0];
    m_nCtactPower1    = pstBondParaLimit->nCtactPowerL[0];
    m_nCtactPower2    = pstBondParaLimit->nCtactPowerH[0];
    m_nCtactForce1    = pstBondParaLimit->nCtactForceL[0];
    m_nCtactForce2    = pstBondParaLimit->nCtactForceH[0];
    m_nBaseTime1      = pstBondParaLimit->nBaseTimeL[0];
    m_nBaseTime2      = pstBondParaLimit->nBaseTimeH[0];
    m_dBasePower1     = pstBondParaLimit->nBasePowerL[0];
    m_dBasePower2     = pstBondParaLimit->nBasePowerH[0];
    m_nBaseForce1     = pstBondParaLimit->nBaseForceL[0];
    m_nBaseForce2     = pstBondParaLimit->nBaseForceH[0];
    m_nPowerDelay1    = pstBondParaLimit->nPowerDelayL[0];
    m_nPowerDelay2    = pstBondParaLimit->nPowerDelayH[0];
    m_nSrchHeight1    = pstBondParaLimit->nSrchHeightL[0];
    m_nSrchHeight2    = pstBondParaLimit->nSrchHeightH[0];
    m_nSrchThresh1    = pstBondParaLimit->nSrchThreshL[0];
    m_nSrchThresh2    = pstBondParaLimit->nSrchThreshH[0];

    m_nPowerFactor1   = pstBondParaLimit->nPowerFactorL;
    m_nPowerFactor2   = pstBondParaLimit->nPowerFactorH;
    m_nForceFactor1   = pstBondParaLimit->nForceFactorL;
    m_nForceFactor2   = pstBondParaLimit->nForceFactorH;

    m_nPreCtactForceL = pstBondParaLimit->nPreCtactForceL[0];
    m_nPreCtactForceH = pstBondParaLimit->nPreCtactForceH[0];
    m_nPreCtactTimeL  = pstBondParaLimit->nPreCtactTimeL[0];
    m_nPreCtactTimeH  = pstBondParaLimit->nPreCtactTimeH[0];

	//v1.6.T18 add
	if ((theMcConfig.GetAcsBufferType() == 1)
		|| (theMcConfig.GetSrchSpeedStep() == 1))
		m_nScaleFactor = 10;	//=5/0.5
	else
		m_nScaleFactor = 2;		//=5/2.5

	m_nSrchSpeedL = pstBondParaLimit->nSrchSpeedL[0] / m_nScaleFactor;			//v1.6.T18 edit
	m_nSrchSpeedH = (pstBondParaLimit->nSrchSpeedH[0] - 1) / m_nScaleFactor;	//v1.6.T18 edit

    m_nRlsForceL = pstBondParaLimit->nRlsForceL[0];
    m_nRlsForceH = pstBondParaLimit->nRlsForceH[0];
	//v3.1.T564 add
    m_nRlsPowerL = pstBondParaLimit->nRlsPowerL[0];
    m_nRlsPowerH = pstBondParaLimit->nRlsPowerH[0];

    ((CComboBox*)GetDlgItem(IDC_COMBO_LIMIT))->AddString(theMsg.GetMsg(MSG_BALL));
    ((CComboBox*)GetDlgItem(IDC_COMBO_LIMIT))->AddString(theMsg.GetMsg(MSG_WEDGE));
    ((CComboBox*)GetDlgItem(IDC_COMBO_LIMIT))->SetCurSel(0);

	m_nPowerH = 255; //100;

    return CPropertyPage::OnInitDialog();
}

// Show menu item for ball and wedge bond
void CParaLimitPage::ShowMenuItem(void)
{
    GetDlgItem(IDC_STATIC_LOWER)->SetWindowPos(NULL, PARA_COLUMN1_LEFT + 10, 15, PARA_COLUMN1_WIDTH, PARA_COLUMN1_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_LOWER, theMsg.GetMsg(MSG_LOWER_LIMIT));

    GetDlgItem(IDC_STATIC_UPPER)->SetWindowPos(NULL, PARA_COLUMN2_LEFT + 20, 15, PARA_COLUMN1_WIDTH, PARA_COLUMN2_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_UPPER, theMsg.GetMsg(MSG_UPPER_LIMIT));

    GetDlgItem(IDC_COMBO_LIMIT)->SetWindowPos(NULL, PARA_LABEL_LEFT, 10, 90, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    
    //1
    int iRowTop = 50;
    GetDlgItem(IDC_STATIC_PREPWR)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_PREPWR, theMsg.GetMsg(MSG_STANDBY_POWER));
    GetDlgItem(IDC_EDIT_PREPOWER_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_PREPOWER_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_SERVICE)
    {
#if _SERVO_TYPE == 2	//v3.1.T432 add
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_FORCE, theMsg.GetMsg(MSG_TOUCHDOWN_FORCE));
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_LOW)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_HIGH)->ShowWindow(1);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_SERACHTHRESH)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SERACHTHRESH,theMsg.GetMsg(MSG_TOUCHDOWN_FORCE_RANGE));
        GetDlgItem(IDC_STATIC_SERACHTHRESH)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_L)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_U)->ShowWindow(1);
#endif

		//2
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
		GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
#if _SERVO_TYPE == 2	//v3.1.T432 add
		SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_TIME, theMsg.GetMsg(MSG_TOUCHDOWN_TIME));
#else
		SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_TIME, theMsg.GetMsg(MSG_PREIMPACT_TIME));
#endif
		GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME_LOW)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME_HIGH)->ShowWindow(1);

		//3
#if _SERVO_TYPE != 2	//v3.1.T432 add
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRE_IMPACT_FORCE, theMsg.GetMsg(MSG_PRE_IMPACT_FORCE));
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_LOW)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_HIGH)->ShowWindow(1);
#endif
    }
    else
    {
        GetDlgItem(IDC_STATIC_PRE_IMPACT_TIME)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME_LOW)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_TIME_HIGH)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_PRE_IMPACT_FORCE)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_LOW)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PRE_IMPACT_FORCE_HIGH)->ShowWindow(FALSE);
    }

    //4
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
    GetDlgItem(IDC_STATIC_IMPACTIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_IMPACTIME, theMsg.GetMsg(MSG_CTACT_TIME));
    GetDlgItem(IDC_EDIT_COMPATIME_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_COMPATIME_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    //5
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
    GetDlgItem(IDC_STATIC_IMPACPWR)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_IMPACPWR, theMsg.GetMsg(MSG_CTACT_POWER));
    GetDlgItem(IDC_EDIT_COMPOWER_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_COMPOWER_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    //6
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
    GetDlgItem(IDC_STATIC_IMPACPRESS)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_IMPACPRESS, theMsg.GetMsg(MSG_CTACT_FORCE));
    GetDlgItem(IDC_EDIT_COMPAPRES_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_COMPAPRES_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    //7
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
    GetDlgItem(IDC_STATIC_BONDTIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BONDTIME,theMsg.GetMsg(MSG_BASE_TIME));
    GetDlgItem(IDC_EDIT_BONDTIME_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_BONDTIME_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    
    //8
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
    GetDlgItem(IDC_STATIC_BONDPWR)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BONDPWR,theMsg.GetMsg(MSG_BASE_POWER));
    GetDlgItem(IDC_EDIT_BONDPWR_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_BONDPWR_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    //9
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
    GetDlgItem(IDC_STATIC_BONDPRESS)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_BONDPRESS,theMsg.GetMsg(MSG_BASE_FORCE));
    GetDlgItem(IDC_EDIT_BONDPRES_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_BONDPRES_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    //10
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
    GetDlgItem(IDC_STATIC_USGDELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_USGDELAY,theMsg.GetMsg(MSG_POWER_DELAY));
    GetDlgItem(IDC_EDIT_USGDELAY_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_USGDELAY_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_SERVICE)
    {
        //11	
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_SEARCHHIGH)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SEARCHHIGH,theMsg.GetMsg(MSG_SRCH_HT));
        GetDlgItem(IDC_STATIC_SEARCHHIGH)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SEARCHHIGH_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SEARCHHIGH_L)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SEARCHHIGH_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SEARCHHIGH_U)->ShowWindow(1);

        //11	
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SRCH_SPEED,theMsg.GetMsg(MSG_SRCH_SPEED));
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->ShowWindow(1);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_L)->ShowWindow(1);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_H)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_H)->ShowWindow(1);

        //12
#if _SERVO_TYPE != 2	//v3.1.T432 add
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_SERACHTHRESH)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SERACHTHRESH,theMsg.GetMsg(MSG_SRCH_THRES));
        GetDlgItem(IDC_STATIC_SERACHTHRESH)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_L)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_U)->ShowWindow(1);
#endif
    }
    else
    {
        GetDlgItem(IDC_STATIC_SEARCHHIGH)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_SEARCHHIGH_L)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_SEARCHHIGH_U)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_SRCH_SPEED)->ShowWindow(FALSE);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_L)->ShowWindow(FALSE);
        GetDlgItem(IDC_COMBO_SRCH_SPEED_H)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_SERACHTHRESH)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_L)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_SEARCHTHRESH_U)->ShowWindow(FALSE);
    }

    if (m_SelectStep == 1) //@1
    {
        //13
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_PWRAVE_INDEX)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PWRAVE_INDEX,theMsg.GetMsg(MSG_POWER_EQUALIZATION));
        GetDlgItem(IDC_STATIC_PWRAVE_INDEX)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PWRAVEINDEX_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PWRAVEINDEX_L)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PWRAVEINDEX_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PWRAVEINDEX_U)->ShowWindow(1);

        //14
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_PRESSAVE_INDEX)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_PRESSAVE_INDEX,theMsg.GetMsg(MSG_FORCE_EQUALIZATION));
        GetDlgItem(IDC_STATIC_PRESSAVE_INDEX)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PRESAVE_INDEX_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRESAVE_INDEX_L)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_PRESAVE_INDEX_U)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_PRESAVE_INDEX_U)->ShowWindow(1);

        //15
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_TAIL_LENGTH)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_TAIL_LENGTH,theMsg.GetMsg(MSG_TAIL_LENGTH_COLON));
        GetDlgItem(IDC_STATIC_TAIL_LENGTH)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_LOW)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_HIGH)->ShowWindow(1);

        //16
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_TAIL_LENGTH2)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_TAIL_LENGTH2,theMsg.GetMsg(MSG_FAB2_TAIL_LENGTH));
        GetDlgItem(IDC_STATIC_TAIL_LENGTH2)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_2_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_2_LOW)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_2_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_2_HIGH)->ShowWindow(1);

        //17
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_SPARK_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_SPARK_DELAY,theMsg.GetMsg(MSG_SPARK_DELAY_COLON));
        GetDlgItem(IDC_STATIC_SPARK_DELAY)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SPARK_DELAY_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SPARK_DELAY_LOW)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_SPARK_DELAY_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_SPARK_DELAY_HIGH)->ShowWindow(1);

        //18
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
        GetDlgItem(IDC_STATIC_FIRE_LEVEL)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_FIRE_LEVEL,theMsg.GetMsg(MSG_FIRE_LEVEL));
        GetDlgItem(IDC_STATIC_FIRE_LEVEL)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_FIRE_LEVEL_L)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_FIRE_LEVEL_L)->ShowWindow(1);
        GetDlgItem(IDC_EDIT_FIRE_LEVEL_H)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        GetDlgItem(IDC_EDIT_FIRE_LEVEL_H)->ShowWindow(1);
    }
    else
    {
        GetDlgItem(IDC_STATIC_PWRAVE_INDEX)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PWRAVEINDEX_L)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PWRAVEINDEX_U)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_PRESSAVE_INDEX)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PRESAVE_INDEX_L)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PRESAVE_INDEX_U)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_TAIL_LENGTH)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_LOW)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_HIGH)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_SPARK_DELAY)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_SPARK_DELAY_LOW)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_SPARK_DELAY_HIGH)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_TAIL_LENGTH2)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_2_LOW)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_TAIL_LENGTH_2_HIGH)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_FIRE_LEVEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_FIRE_LEVEL_L)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_FIRE_LEVEL_H)->ShowWindow(FALSE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_POST_DAMP_FORCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_POST_DAMP_FORCE, theMsg.GetMsg(MSG_RELEASE_FORCE));
    GetDlgItem(IDC_STATIC_POST_DAMP_FORCE)->ShowWindow(1);
    GetDlgItem(IDC_EDIT_POST_DAMP_FORCE_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_POST_DAMP_FORCE_LOW)->ShowWindow(1);
    GetDlgItem(IDC_EDIT_POST_DAMP_FORCE_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_POST_DAMP_FORCE_HIGH)->ShowWindow(1);

	//v3.1.T564 add
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_POST_DAMP_POWER)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_POST_DAMP_POWER, theMsg.GetMsg(MSG_RELEASE_POWER));
    GetDlgItem(IDC_STATIC_POST_DAMP_POWER)->ShowWindow(1);
    GetDlgItem(IDC_EDIT_POST_DAMP_POWER_LOW)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_POST_DAMP_POWER_LOW)->ShowWindow(1);
    GetDlgItem(IDC_EDIT_POST_DAMP_POWER_HIGH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_POST_DAMP_POWER_HIGH)->ShowWindow(1);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (iRowTop > PROPERTY_PAGE_HEIGHT)
	{
		m_nPageSize = iRowTop - PROPERTY_PAGE_HEIGHT;
		EnableScrollBarCtrl(SB_VERT, TRUE);
		SetScrollRange(SB_VERT, 0, m_nPageSize);
	}
	else
		EnableScrollBarCtrl(SB_VERT, FALSE);
}

HBRUSH CParaLimitPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CParaLimitPage::OnFocusPrepowerL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nStandbyPower1, 0, 0, m_nPowerH);
    UpdateData(false);
}

void CParaLimitPage::OnFocusPrepowerU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nStandbyPower2, 0, 0, m_nPowerH);
    UpdateData(false);
}

void CParaLimitPage::OnFocusCompatimeL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactTime1, 0, 0, 100);
    UpdateData(false);
}

void CParaLimitPage::OnFocusCompatimeU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactTime2, 0, 0, 100);
    UpdateData(false);
}

void CParaLimitPage::OnFocusCompowerL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactPower1, 0, 0, m_nPowerH);
    UpdateData(false);
}

void CParaLimitPage::OnFocusCompowerU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactPower2, 0, 0, m_nPowerH);
    UpdateData(false);
}

void CParaLimitPage::OnFocusCompapresL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactForce1, 0, 0, 200);
    UpdateData(false);
}

void CParaLimitPage::OnFocusCompapresU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCtactForce2, m_nCtactForce2,
				theBondPara.GetBondParaLimit()->nCtactForceL[0],
				thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CParaLimitPage::OnFocusBondtimeL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseTime1, 0, 0, 100);
    UpdateData(false);
}

void CParaLimitPage::OnFocusBondtimeU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseTime2, 100, 0, 100);
    UpdateData(false);
}

void CParaLimitPage::OnFocusBondpwrL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_dBasePower1, 0, 0, m_nPowerH);
    UpdateData(false);
}

void CParaLimitPage::OnFocusBondpwrU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_dBasePower2, 0, 0, m_nPowerH);
    UpdateData(false);
}

void CParaLimitPage::OnFocusBondpresL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseForce1, 0, 0, 200);
    UpdateData(false);
}

void CParaLimitPage::OnFocusBondpresU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseForce2, m_nBaseForce2,
			theBondPara.GetBondParaLimit()->nBaseForceL[0],
			thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CParaLimitPage::OnFocusPwraveindexL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerFactor1);
    UpdateData(false);
}

void CParaLimitPage::OnFocusPwraveindexU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerFactor2);
    UpdateData(false);
}

void CParaLimitPage::OnFocusPresaveIndexL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nForceFactor1);
    UpdateData(false);
}

void CParaLimitPage::OnFocusPresaveIndexU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nForceFactor2);
    UpdateData(false);
}

void CParaLimitPage::OnFocusUsgdelayL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerDelay1, 0, 0, 100);
    UpdateData(false);
}

void CParaLimitPage::OnFocusUsgdelayU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPowerDelay2, 100, 0, 100);
    UpdateData(false);
}

void CParaLimitPage::OnFocusSearchhighL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchHeight1);
    UpdateData(false);
}

void CParaLimitPage::OnFocusSearchhighU()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchHeight2);
    UpdateData(false);
}

void CParaLimitPage::OnFocusSearchthreshL()
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T432 add
	theKeyPad.GetUserKeyInData(&m_nSrchThresh1, 50, 10, 100);
#else
	int nSel=((CComboBox*)GetDlgItem(IDC_COMBO_LIMIT))->GetCurSel();
	theKeyPad.GetUserKeyInData(&m_nSrchThresh1, m_nSrchThresh1, 3, nSel==0 ? 79 : 99);
#endif
	UpdateData(false);
}

void CParaLimitPage::OnFocusSearchthreshU()
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T432 add
	theKeyPad.GetUserKeyInData(&m_nSrchThresh2, 100, 10, 200);
#else
	theKeyPad.GetUserKeyInData(&m_nSrchThresh2);
#endif
	UpdateData(false);
}

void CParaLimitPage::OnLimit()
{
    UpdateData(TRUE);

	BOND_PARA_LIMIT* pstBondParaLimit = theBondPara.GetBondParaLimit();

    if (m_SelectStep == 0)
    {
        // save index 1 data
        pstBondParaLimit->nStandbyPowerL[1]  = m_nStandbyPower1;
        pstBondParaLimit->nStandbyPowerH[1]  = m_nStandbyPower2;
        pstBondParaLimit->nCtactTimeL[1]     = m_nCtactTime1;
        pstBondParaLimit->nCtactTimeH[1]     = m_nCtactTime2;
        pstBondParaLimit->nCtactPowerL[1]    = m_nCtactPower1;
        pstBondParaLimit->nCtactPowerH[1]    = m_nCtactPower2;
        pstBondParaLimit->nCtactForceL[1]    = m_nCtactForce1;
        pstBondParaLimit->nCtactForceH[1]    = m_nCtactForce2;
        pstBondParaLimit->nBaseTimeL[1]      = m_nBaseTime1;
        pstBondParaLimit->nBaseTimeH[1]      = m_nBaseTime2;
        pstBondParaLimit->nBasePowerL[1]     = m_dBasePower1;
        pstBondParaLimit->nBasePowerH[1]     = m_dBasePower2;
        pstBondParaLimit->nBaseForceL[1]     = m_nBaseForce1;
        pstBondParaLimit->nBaseForceH[1]     = m_nBaseForce2;
        pstBondParaLimit->nPowerDelayL[1]    = m_nPowerDelay1;
        pstBondParaLimit->nPowerDelayH[1]    = m_nPowerDelay2;
        pstBondParaLimit->nSrchHeightL[1]    = m_nSrchHeight1;
        pstBondParaLimit->nSrchHeightH[1]    = m_nSrchHeight2;
        pstBondParaLimit->nSrchThreshL[1]    = m_nSrchThresh1;
        pstBondParaLimit->nSrchThreshH[1]    = m_nSrchThresh2;

        pstBondParaLimit->nPowerFactorL      = m_nPowerFactor1;
        pstBondParaLimit->nPowerFactorH      = m_nPowerFactor2;
        pstBondParaLimit->nForceFactorL      = m_nForceFactor1;
        pstBondParaLimit->nForceFactorH      = m_nForceFactor2;

        pstBondParaLimit->nPreCtactForceL[1] = m_nPreCtactForceL;
        pstBondParaLimit->nPreCtactForceH[1] = m_nPreCtactForceH;
        pstBondParaLimit->nPreCtactTimeL[1]  = m_nPreCtactTimeL;
        pstBondParaLimit->nPreCtactTimeH[1]  = m_nPreCtactTimeH;
        pstBondParaLimit->nTailLengthL       = m_nTailLengthL;
        pstBondParaLimit->nTailLengthH       = m_nTailLengthH;
        pstBondParaLimit->nSparkDelayL       = m_nSparkDelayL;
        pstBondParaLimit->nSparkDelayH       = m_nSparkDelayH;
        
        pstBondParaLimit->nTailLength2L      = m_nTailLength2L;
        pstBondParaLimit->nTailLength2H      = m_nTailLength2H;
        pstBondParaLimit->nFireLevelL        = m_nFireLevelL;
        pstBondParaLimit->nFireLevelH        = m_nFireLevelH;

		pstBondParaLimit->nSrchSpeedL[1]     = m_nSrchSpeedL * m_nScaleFactor;			//v1.6.T18 edit
		pstBondParaLimit->nSrchSpeedH[1]     = (m_nSrchSpeedH + 1) * m_nScaleFactor;	//v1.6.T18 edit

        pstBondParaLimit->nRlsForceL[1]      = m_nRlsForceL;
        pstBondParaLimit->nRlsForceH[1]      = m_nRlsForceH;
		//v3.1.T564 add
        pstBondParaLimit->nRlsPowerL[1]      = m_nRlsPowerL;
        pstBondParaLimit->nRlsPowerH[1]      = m_nRlsPowerH;

        //display index 0 data
        m_nStandbyPower1  = pstBondParaLimit->nStandbyPowerL[0];
        m_nStandbyPower2  = pstBondParaLimit->nStandbyPowerH[0];
        m_nCtactTime1     = pstBondParaLimit->nCtactTimeL[0];
        m_nCtactTime2     = pstBondParaLimit->nCtactTimeH[0];
        m_nCtactPower1    = pstBondParaLimit->nCtactPowerL[0];
        m_nCtactPower2    = pstBondParaLimit->nCtactPowerH[0];
        m_nCtactForce1    = pstBondParaLimit->nCtactForceL[0];
        m_nCtactForce2    = pstBondParaLimit->nCtactForceH[0];
        m_nBaseTime1      = pstBondParaLimit->nBaseTimeL[0];
        m_nBaseTime2      = pstBondParaLimit->nBaseTimeH[0];
        m_dBasePower1     = pstBondParaLimit->nBasePowerL[0];
        m_dBasePower2     = pstBondParaLimit->nBasePowerH[0];
        m_nBaseForce1     = pstBondParaLimit->nBaseForceL[0];
        m_nBaseForce2     = pstBondParaLimit->nBaseForceH[0];
        m_nPowerDelay1    = pstBondParaLimit->nPowerDelayL[0];
        m_nPowerDelay2    = pstBondParaLimit->nPowerDelayH[0];
        m_nSrchHeight1    = pstBondParaLimit->nSrchHeightL[0];
        m_nSrchHeight2    = pstBondParaLimit->nSrchHeightH[0];
        m_nSrchThresh1    = pstBondParaLimit->nSrchThreshL[0];
        m_nSrchThresh2    = pstBondParaLimit->nSrchThreshH[0];

        m_nPreCtactForceL = pstBondParaLimit->nPreCtactForceL[0];
        m_nPreCtactForceH = pstBondParaLimit->nPreCtactForceH[0];
        m_nPreCtactTimeL  = pstBondParaLimit->nPreCtactTimeL[0];
        m_nPreCtactTimeH  = pstBondParaLimit->nPreCtactTimeH[0];

		m_nSrchSpeedL     = pstBondParaLimit->nSrchSpeedL[0] / m_nScaleFactor;			//v1.6.T18 edit
		m_nSrchSpeedH     = (pstBondParaLimit->nSrchSpeedH[0] - 1) / m_nScaleFactor;	//v1.6.T18 edit

        m_nRlsForceL      = pstBondParaLimit->nRlsForceL[0];
        m_nRlsForceH      = pstBondParaLimit->nRlsForceH[0];
		//v3.1.T564 add
        m_nRlsPowerL      = pstBondParaLimit->nRlsPowerL[0];
        m_nRlsPowerH      = pstBondParaLimit->nRlsPowerH[0];

        UpdateData(false);
    }
    else if (m_SelectStep == 1)
    {
        // save index 0 data
        pstBondParaLimit->nStandbyPowerL[0]  = m_nStandbyPower1;
        pstBondParaLimit->nStandbyPowerH[0]  = m_nStandbyPower2;
        pstBondParaLimit->nCtactTimeL[0]     = m_nCtactTime1;
        pstBondParaLimit->nCtactTimeH[0]     = m_nCtactTime2;
        pstBondParaLimit->nCtactPowerL[0]    = m_nCtactPower1;
        pstBondParaLimit->nCtactPowerH[0]    = m_nCtactPower2;
        pstBondParaLimit->nCtactForceL[0]    = m_nCtactForce1;
        pstBondParaLimit->nCtactForceH[0]    = m_nCtactForce2;
        pstBondParaLimit->nBaseTimeL[0]      = m_nBaseTime1;
        pstBondParaLimit->nBaseTimeH[0]      = m_nBaseTime2;
        pstBondParaLimit->nBasePowerL[0]     = m_dBasePower1;
        pstBondParaLimit->nBasePowerH[0]     = m_dBasePower2;
        pstBondParaLimit->nBaseForceL[0]     = m_nBaseForce1;
        pstBondParaLimit->nBaseForceH[0]     = m_nBaseForce2;
        pstBondParaLimit->nPowerDelayL[0]    = m_nPowerDelay1;
        pstBondParaLimit->nPowerDelayH[0]    = m_nPowerDelay2;
        pstBondParaLimit->nSrchHeightL[0]    = m_nSrchHeight1;
        pstBondParaLimit->nSrchHeightH[0]    = m_nSrchHeight2;
        pstBondParaLimit->nSrchThreshL[0]    = m_nSrchThresh1;
        pstBondParaLimit->nSrchThreshH[0]    = m_nSrchThresh2;

        pstBondParaLimit->nPreCtactForceL[0] = m_nPreCtactForceL;
        pstBondParaLimit->nPreCtactForceH[0] = m_nPreCtactForceH;
        pstBondParaLimit->nPreCtactTimeL[0]  = m_nPreCtactTimeL;
        pstBondParaLimit->nPreCtactTimeH[0]  = m_nPreCtactTimeH;

		pstBondParaLimit->nSrchSpeedL[0]     = m_nSrchSpeedL * m_nScaleFactor;			//v1.6.T18 edit
		pstBondParaLimit->nSrchSpeedH[0]     = (m_nSrchSpeedH + 1) * m_nScaleFactor;	//v1.6.T18 edit

        pstBondParaLimit->nRlsForceL[0]      = m_nRlsForceL;
        pstBondParaLimit->nRlsForceH[0]      = m_nRlsForceH;
		//v3.1.T564 add
        pstBondParaLimit->nRlsPowerL[0]      = m_nRlsPowerL;
        pstBondParaLimit->nRlsPowerH[0]      = m_nRlsPowerH;

        //display index 1 data
        m_nStandbyPower1  = pstBondParaLimit->nStandbyPowerL[1];
        m_nStandbyPower2  = pstBondParaLimit->nStandbyPowerH[1];
        m_nCtactTime1     = pstBondParaLimit->nCtactTimeL[1];
        m_nCtactTime2     = pstBondParaLimit->nCtactTimeH[1];
        m_nCtactPower1    = pstBondParaLimit->nCtactPowerL[1];
        m_nCtactPower2    = pstBondParaLimit->nCtactPowerH[1];
        m_nCtactForce1    = pstBondParaLimit->nCtactForceL[1];
        m_nCtactForce2    = pstBondParaLimit->nCtactForceH[1];
        m_nBaseTime1      = pstBondParaLimit->nBaseTimeL[1];
        m_nBaseTime2      = pstBondParaLimit->nBaseTimeH[1];
        m_dBasePower1     = pstBondParaLimit->nBasePowerL[1];
        m_dBasePower2     = pstBondParaLimit->nBasePowerH[1];
        m_nBaseForce1     = pstBondParaLimit->nBaseForceL[1];
        m_nBaseForce2     = pstBondParaLimit->nBaseForceH[1];
        m_nPowerDelay1    = pstBondParaLimit->nPowerDelayL[1];
        m_nPowerDelay2    = pstBondParaLimit->nPowerDelayH[1];
        m_nSrchHeight1    = pstBondParaLimit->nSrchHeightL[1];
        m_nSrchHeight2    = pstBondParaLimit->nSrchHeightH[1];
        m_nSrchThresh1    = pstBondParaLimit->nSrchThreshL[1];
        m_nSrchThresh2    = pstBondParaLimit->nSrchThreshH[1];

        m_nPowerFactor1   = pstBondParaLimit->nPowerFactorL;
        m_nPowerFactor2   = pstBondParaLimit->nPowerFactorH;
        m_nForceFactor1   = pstBondParaLimit->nForceFactorL;
        m_nForceFactor2   = pstBondParaLimit->nForceFactorH;

        m_nPreCtactForceL = pstBondParaLimit->nPreCtactForceL[1];
        m_nPreCtactForceH = pstBondParaLimit->nPreCtactForceH[1];
        m_nPreCtactTimeL  = pstBondParaLimit->nPreCtactTimeL[1];
        m_nPreCtactTimeH  = pstBondParaLimit->nPreCtactTimeH[1];
        m_nTailLengthL    = pstBondParaLimit->nTailLengthL;
        m_nTailLengthH    = pstBondParaLimit->nTailLengthH;
        m_nSparkDelayL    = pstBondParaLimit->nSparkDelayL;
        m_nSparkDelayH    = pstBondParaLimit->nSparkDelayH;

        m_nTailLength2L   = pstBondParaLimit->nTailLength2L;
        m_nTailLength2H   = pstBondParaLimit->nTailLength2H;
        m_nFireLevelL     = pstBondParaLimit->nFireLevelL;
        m_nFireLevelH     = pstBondParaLimit->nFireLevelH;

		m_nSrchSpeedL     = pstBondParaLimit->nSrchSpeedL[1] / m_nScaleFactor;			//v1.6.T18 edit
		m_nSrchSpeedH     = (pstBondParaLimit->nSrchSpeedH[1] - 1) / m_nScaleFactor;	//v1.6.T18 edit

        m_nRlsForceL      = pstBondParaLimit->nRlsForceL[1];
        m_nRlsForceH      = pstBondParaLimit->nRlsForceH[1];
		//v3.1.T564 add
        m_nRlsPowerL      = pstBondParaLimit->nRlsPowerL[1];
        m_nRlsPowerH      = pstBondParaLimit->nRlsPowerH[1];
        UpdateData(false);
    }

    ShowMenuItem();
}

void CParaLimitPage::OnFocusPreImpactForceLow()
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T432 add
	theKeyPad.GetUserKeyInData(&m_nPreCtactForceL, 20, 10, 200);
#else
	theKeyPad.GetUserKeyInData(&m_nPreCtactForceL, 0, 0, 200);
#endif
	UpdateData(false);
}

void CParaLimitPage::OnFocusPreImpactForceHigh()
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T432 add
	theKeyPad.GetUserKeyInData(&m_nPreCtactForceH, 30,
			10, thePgm.GetRcd()->nBondForceLimit);
#else
	theKeyPad.GetUserKeyInData(&m_nPreCtactForceH, m_nPreCtactForceH,
			theBondPara.GetBondParaLimit()->nPreCtactForceL[0],
			thePgm.GetRcd()->nBondForceLimit);
#endif
	UpdateData(false);
}

void CParaLimitPage::OnFocusPostDampForceLow()
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRlsForceL, 0, 0, 200);
	UpdateData(false);
}

void CParaLimitPage::OnFocusPostDampForceHigh()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nRlsForceH, m_nRlsForceH,
			theBondPara.GetBondParaLimit()->nRlsForceL[0],
			thePgm.GetRcd()->nBondForceLimit);
    UpdateData(false);
}

void CParaLimitPage::OnFocusPreImpactTimeLow()
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T432 add
	theKeyPad.GetUserKeyInData(&m_nPreCtactTimeL, 1, 1, 5);
#else
	theKeyPad.GetUserKeyInData(&m_nPreCtactTimeL, 1, 0, 5);
#endif
	UpdateData(false);
}

void CParaLimitPage::OnFocusPreImpactTimeHigh()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreCtactTimeH, 5, 1, 5);
    UpdateData(false);
}

void CParaLimitPage::OnFocusSparkDelayLow()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSparkDelayL, 2, 0, 15);
    UpdateData(false);
}

void CParaLimitPage::OnFocusSparkDelayHigh()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSparkDelayH, 10, 2, 500);
    UpdateData(false);
}

void CParaLimitPage::OnFocusTailLengthLow()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailLengthL, 100, 0, 600);
    UpdateData(false);
}

void CParaLimitPage::OnFocusTailLengthHigh()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailLengthH, 400, 100, 1600);
    UpdateData(false);
}

void CParaLimitPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    short       nOffset = 0;
    int         nMin, nMax, iCurPos = GetScrollPos(SB_VERT);
    SCROLLINFO  stScrollInfo;

    GetScrollInfo(SB_VERT, &stScrollInfo);
    nMin = stScrollInfo.nMin;
    nMax = stScrollInfo.nMax;

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

void CParaLimitPage::OnFocusTailLength2Low()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailLength2L, 100, 0, 600);
    UpdateData(false);
}

void CParaLimitPage::OnFocusTailLength2High()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailLength2H, 400, 100, 1600);
    UpdateData(false);
}

void CParaLimitPage::OnFocusFireLevelL()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nFireLevelL, 5000, 2000, 8500);
    UpdateData(false);
}

void CParaLimitPage::OnFocusFireLevelH()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nFireLevelH, 5000, 4000, 8500);
    UpdateData(false);
}

BOOL CParaLimitPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta>0 ? SB_LINEUP : SB_LINEDOWN, NULL);
	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CParaLimitPage::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
	CPropertyPage::OnMouseMove(nFlags, point);
}

//v3.1.T564 add
void CParaLimitPage::OnFocusPostDampPowerLow()
{
	GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRlsPowerL, m_nRlsPowerL, 0, 255);
	UpdateData(false);
}

//v3.1.T564 add
void CParaLimitPage::OnFocusPostDampPowerHigh()
{
    GetDlgItem(IDC_STATIC_LOWER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nRlsPowerH, m_nRlsPowerH, 0, 255);
    UpdateData(false);
}
