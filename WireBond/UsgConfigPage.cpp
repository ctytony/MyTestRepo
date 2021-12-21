/***************************************************************
UsgConfigPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BondPara.h"
#include "UsgConfigPage.h"

// CUsgConfigPage dialog
IMPLEMENT_DYNAMIC(CUsgConfigPage, CPropertyPage)

CUsgConfigPage::CUsgConfigPage()
	: CPropertyPage(CUsgConfigPage::IDD)
    , m_nImpactRampTime1(0)
    , m_nImpactRampTime2(0)
    , m_nBaseRampTime1(0)
    , m_nBaseRampTime2(0)
    , m_nReleaseRampTime1(0)
    , m_nReleaseRampTime2(0)
    , m_nFeedPower(0)
    , m_nFeedPowerTime(0)
    , m_iBoostAmplitude(0)
    , m_iBoostDuration(0)
    , m_iBoostMode(0)
    , m_nSeatPower(0)
    , m_nSeatTime(0)
	, m_nTailPower(0)
    , m_nTailTime(0)
    , m_iImpactBoostMode(0)
    , m_iImpactBoostAmp(0)
    , m_iImpactBoostDuration(0)
{
}

CUsgConfigPage::~CUsgConfigPage()
{
}

void CUsgConfigPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_IMPACT_1, m_nImpactRampTime1);
    DDX_Text(pDX, IDC_EDIT_IMPACT_2, m_nImpactRampTime2);
    DDX_Text(pDX, IDC_EDIT_BOND_1, m_nBaseRampTime1);
    DDX_Text(pDX, IDC_EDIT_BOND_2, m_nBaseRampTime2);
    DDX_Text(pDX, IDC_EDIT_RELEASE_1, m_nReleaseRampTime1);
    DDX_Text(pDX, IDC_EDIT_RELEASE_2, m_nReleaseRampTime2);
    DDX_CBIndex(pDX, IDC_COMBO_AMPLITUDE, m_iBoostAmplitude);
    DDX_CBIndex(pDX, IDC_COMBO_DURATION, m_iBoostDuration);
    DDX_CBIndex(pDX, IDC_COMBO3, m_iBoostMode);
    DDX_Text(pDX, IDC_EDIT_FEED_POWER, m_nFeedPower);
    DDX_Text(pDX, IDC_EDIT_FEED_POWER_TIME, m_nFeedPowerTime);
    DDX_Text(pDX, IDC_EDIT_SEAT_POWER, m_nSeatPower);
    DDX_Text(pDX, IDC_EDIT_SEAT_POWER_TIME, m_nSeatTime);
    DDX_Text(pDX, IDC_EDIT_TAIL_POWER, m_nTailPower);
    DDX_Text(pDX, IDC_EDIT_TAIL_POWER_TIME, m_nTailTime);
    DDX_CBIndex(pDX, IDC_COMBO_IMPACT_BOOST_MODE, m_iImpactBoostMode);
    DDX_CBIndex(pDX, IDC_COMBO_IMPACT_AMPLITUDE, m_iImpactBoostAmp);
    DDX_CBIndex(pDX, IDC_COMBO_IMPACT_DURATION, m_iImpactBoostDuration);
}

BEGIN_MESSAGE_MAP(CUsgConfigPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_IMPACT_1, OnFocusImpact1)
    ON_EN_SETFOCUS(IDC_EDIT_IMPACT_2, OnFocusImpact2)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_1, OnFocusBond1)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_2, OnFocusBond2)
    ON_EN_SETFOCUS(IDC_EDIT_RELEASE_1, OnFocusRelease1)
    ON_EN_SETFOCUS(IDC_EDIT_RELEASE_2, OnFocusRelease2)
    ON_EN_SETFOCUS(IDC_EDIT_FEED_POWER, OnFocusFeedPower)
    ON_EN_SETFOCUS(IDC_EDIT_FEED_POWER_TIME, OnFocusFeedPowerTime)
    ON_EN_SETFOCUS(IDC_EDIT_SEAT_POWER, OnFocusSeatPower)
	ON_EN_SETFOCUS(IDC_EDIT_SEAT_POWER_TIME, OnFocusSeatPowerTime)
	ON_EN_SETFOCUS(IDC_EDIT_TAIL_POWER, OnFocusTailPower)
	ON_EN_SETFOCUS(IDC_EDIT_TAIL_POWER_TIME, OnFocusTailPowerTime)
    ON_CBN_SELCHANGE(IDC_COMBO_IMPACT_BOOST_MODE, OnImpactBoostMode)
    ON_CBN_SELCHANGE(IDC_COMBO_IMPACT_AMPLITUDE, OnImpactAmplitude)
    ON_CBN_SELCHANGE(IDC_COMBO_IMPACT_DURATION, OnImpactDuration)
    ON_CBN_SELCHANGE(IDC_COMBO_BOOST_MODE, OnBoostMode)
    ON_CBN_SELCHANGE(IDC_COMBO_AMPLITUDE, OnAmplitude)
    ON_CBN_SELCHANGE(IDC_COMBO_DURATION, OnDuration)
	ON_WM_DESTROY()		//V1.4.W25 add
END_MESSAGE_MAP()

// CUsgConfigPage message handlers
BOOL CUsgConfigPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_BALL_BOND, theMsg.GetMsg(MSG_BALL));
	SetDlgItemTextW(IDC_STATIC_WEDGE, theMsg.GetMsg(MSG_WEDGE));
	SetDlgItemTextW(IDC_STATIC_RAMP_TIME, theMsg.GetMsg(MSG_USG_RAMP_TIME));
	SetDlgItemTextW(IDC_STATIC_IMPACT, theMsg.GetMsg(MSG_CTACT_POWER_PERCENT));
	SetDlgItemTextW(IDC_STATIC_BOND, theMsg.GetMsg(MSG_BOND_POWER_PERCENT));
	SetDlgItemTextW(IDC_STATIC_RELEASE, theMsg.GetMsg(MSG_RELEASE_POWER_PERCENT));
	SetDlgItemTextW(IDC_STATIC_FEED_POWER, theMsg.GetMsg(MSG_POWER_2));
	SetDlgItemTextW(IDC_STATIC_POWER, theMsg.GetMsg(MSG_POWER_2));
	SetDlgItemTextW(IDC_STATIC_TIME, theMsg.GetMsg(MSG_TIME_2));
	SetDlgItemTextW(IDC_STATIC_SEAT, theMsg.GetMsg(MSG_POWER_SEAT));
	SetDlgItemTextW(IDC_STATIC_FEED, theMsg.GetMsg(MSG_POWER_FEED));
	SetDlgItemTextW(IDC_STATIC_TAIL, theMsg.GetMsg(MSG_POWER_TAIL));
	SetDlgItemTextW(IDC_STATIC_BOOST, theMsg.GetMsg(MSG_1ST_BOND_BOOST));
	SetDlgItemTextW(IDC_STATIC_BOOST_MODE, theMsg.GetMsg(MSG_BOOST_MODE));
	SetDlgItemTextW(IDC_STATIC_AMPLITUDE, theMsg.GetMsg(MSG_BOOST_AMPLITUDE));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_IMPACT_AMPLITUDE);
	pComboBox->AddString(theMsg.GetMsg(MSG_LOW));
	pComboBox->AddString(theMsg.GetMsg(MSG_MIDDLE));
	pComboBox->AddString(theMsg.GetMsg(MSG_HIGH));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_AMPLITUDE);
	pComboBox->AddString(theMsg.GetMsg(MSG_LOW));
	pComboBox->AddString(theMsg.GetMsg(MSG_MIDDLE));
	pComboBox->AddString(theMsg.GetMsg(MSG_HIGH));

	SetDlgItemTextW(IDC_STATIC_DURATION,theMsg.GetMsg(MSG_BOOST_DURATION));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_IMPACT_DURATION);
	pComboBox->AddString(theMsg.GetMsg(MSG_SHORT));
	pComboBox->AddString(theMsg.GetMsg(MSG_LONG));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DURATION);
	pComboBox->AddString(theMsg.GetMsg(MSG_SHORT));
	pComboBox->AddString(theMsg.GetMsg(MSG_LONG));

	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();
	m_nImpactRampTime1     = pstBondPara->nCtactPowerRamp[0];
	m_nImpactRampTime2     = pstBondPara->nCtactPowerRamp[1];
	m_nBaseRampTime1       = pstBondPara->nBasePowerRamp[0];
	m_nBaseRampTime2       = pstBondPara->nBasePowerRamp[1];
	m_nReleaseRampTime1    = pstBondPara->nReleasePowerRamp[0];
	m_nReleaseRampTime2    = pstBondPara->nReleasePowerRamp[1];
	m_iBoostMode           = pstBondPara->ch1stBondBoostEnable;
	m_iBoostAmplitude      = pstBondPara->chBoostAmplitude1;
	m_iBoostDuration       = pstBondPara->chBoostDuration1;
	m_iImpactBoostMode     = pstBondPara->ch1stBondImpactBoostEnable;
	m_iImpactBoostAmp      = pstBondPara->chImpactBoostAmplitude1;
	m_iImpactBoostDuration = pstBondPara->chImpactBoostDuration1;

	USG_STATIC_PARA_RCD stUsgPara; 
	theBondPara.GetUSGParaRcd(&stUsgPara);
	m_nFeedPower     = stUsgPara.nFeedPower;
	m_nFeedPowerTime = stUsgPara.cFeedPowerTime;
	m_nSeatPower     = stUsgPara.nSeatPower; 
	m_nSeatTime      = stUsgPara.cSeatPowerTime;
	m_nTailPower     = stUsgPara.nTailPower; 
	m_nTailTime      = stUsgPara.cTailPowerTime;

	return CPropertyPage::OnInitDialog();
}

HBRUSH CUsgConfigPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CUsgConfigPage::OnDestroy()
{
	CPropertyPage::OnDestroy();

	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();
	pstBondPara->nCtactPowerRamp[0]         = m_nImpactRampTime1;
	pstBondPara->nCtactPowerRamp[1]         = m_nImpactRampTime2;
	pstBondPara->nBasePowerRamp[0]          = m_nBaseRampTime1;
	pstBondPara->nBasePowerRamp[1]          = m_nBaseRampTime2;
	pstBondPara->nReleasePowerRamp[0]       = m_nReleaseRampTime1;
	pstBondPara->nReleasePowerRamp[1]       = m_nReleaseRampTime2;
	pstBondPara->ch1stBondBoostEnable       = (char)m_iBoostMode;
	pstBondPara->chBoostAmplitude1          = (char)m_iBoostAmplitude;
	pstBondPara->chBoostDuration1           = (char)m_iBoostDuration;
	pstBondPara->ch1stBondImpactBoostEnable = (char)m_iImpactBoostMode;
	pstBondPara->chImpactBoostAmplitude1    = (char)m_iImpactBoostAmp;
	pstBondPara->chImpactBoostDuration1     = (char)m_iImpactBoostDuration;

	USG_STATIC_PARA_RCD stUsgPara;
	theBondPara.GetUSGParaRcd(&stUsgPara);
	stUsgPara.nFeedPower     = m_nFeedPower;
	stUsgPara.cFeedPowerTime = (char)m_nFeedPowerTime;
	stUsgPara.nSeatPower     = m_nSeatPower;
	stUsgPara.cSeatPowerTime = (char)m_nSeatTime;
	stUsgPara.nTailPower     = m_nTailPower;
	stUsgPara.cTailPowerTime = (char)m_nTailTime;
	theBondPara.SetUSGParaRcd(&stUsgPara);

	theApp.ArchivePowerOnData(ARCHIVE_SAVE);  
}

void CUsgConfigPage::OnFocusImpact1()
{
    GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nImpactRampTime1, 0, 0, 100);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusImpact2()
{
    GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nImpactRampTime2, 0, 0, 100);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusBond1()
{
    GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseRampTime1, 0, 0, 100);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusBond2()
{
    GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseRampTime2, 0, 0, 100);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusRelease1()
{
    GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleaseRampTime1, 0, 0, 100);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusRelease2()
{
    GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nReleaseRampTime2, 0, 0, 100);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusFeedPower()
{
	GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nFeedPower, 0, 0, 35);	//v3.1.T414 delete
	theKeyPad.GetUserKeyInData(&m_nFeedPower, 0, 0, 50);	//v3.1.T414 add
	UpdateData(false);
}

void CUsgConfigPage::OnFocusFeedPowerTime()
{
	GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nFeedPowerTime, 0, 0, 5);	//v3.1.T414 delete
	//theKeyPad.GetUserKeyInData(&m_nFeedPowerTime, 0, 0, 30);	//v3.1.T414 add, v3.1.T418 delete
	theKeyPad.GetUserKeyInData(&m_nFeedPowerTime, 0, 0, 100);	//v3.1.T418 add
	UpdateData(false);
}

void CUsgConfigPage::OnFocusSeatPower()
{
	GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSeatPower, 0, 0, 100);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusSeatPowerTime()
{
	GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSeatTime, 0, 0, 5);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusTailPower()
{
	GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailPower, 0, 0, 250);
    UpdateData(false);
}

void CUsgConfigPage::OnFocusTailPowerTime()
{
	GetDlgItem(IDC_STATIC_RAMP_TIME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailTime, 0, 0, 15);
    UpdateData(false);
}

void CUsgConfigPage::OnImpactBoostMode()
{
    UpdateData();
}

void CUsgConfigPage::OnImpactAmplitude()
{
    UpdateData();
}

void CUsgConfigPage::OnImpactDuration()
{
    UpdateData();
}

void CUsgConfigPage::OnBoostMode()
{
    UpdateData();
}

void CUsgConfigPage::OnAmplitude()
{
    UpdateData();
}

void CUsgConfigPage::OnDuration()
{
    UpdateData();
}
