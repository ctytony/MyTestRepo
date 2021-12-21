// BondSafetyPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BondSafetyPage.h"
#include "MathCalc.h"
#include "User.h"

// CBondSafetyPage dialog
IMPLEMENT_DYNAMIC(CBondSafetyPage, CPropertyPage)

CBondSafetyPage::CBondSafetyPage()
	: CPropertyPage(CBondSafetyPage::IDD)
    , m_lCapillaryWearOutLimit(0)
	, m_lOpenClamperDelay(0)
	, m_lRelaySafetyDuration(0)
	, m_nImpLowerLimit(0)
	, m_nImpUpperLimit(0)
	, m_nBondForceLimit(0)
{
}

CBondSafetyPage::~CBondSafetyPage()
{
}

void CBondSafetyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CAPILLARY_LIMIT, m_lCapillaryWearOutLimit);
	DDX_Text(pDX, IDC_EDIT_CLAMPER_OPEN_TIME, m_lOpenClamperDelay);
	DDX_Text(pDX, IDC_EDIT_RELAY_SAFETY_DURATION, m_lRelaySafetyDuration);
	DDX_Text(pDX, IDC_EDIT_IMP_LOWER_LIMIT, m_nImpLowerLimit);
	DDX_Text(pDX, IDC_EDIT_IMP_UPPER_LIMIT, m_nImpUpperLimit);
	DDX_Text(pDX, IDC_EDIT_BONDFORCE_LIMIT, m_nBondForceLimit);
}

BEGIN_MESSAGE_MAP(CBondSafetyPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_CAPILLARY_LIMIT, OnFocusCapillaryLimit)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMPER_OPEN_TIME, OnFocusClamperOpenTime)
	ON_EN_SETFOCUS(IDC_EDIT_RELAY_SAFTY_DURATION, OnFocusRelaySafetyDuration)
	ON_EN_SETFOCUS(IDC_EDIT_IMP_LOWER_LIMIT, OnFocusImpLowerLimit)
	ON_EN_SETFOCUS(IDC_EDIT_IMP_UPPER_LIMIT, OnFocusImpUpperLimit)
	ON_EN_SETFOCUS(IDC_EDIT_BONDFORCE_LIMIT, OnFocusBondForceLimit)
	ON_WM_DESTROY()		//V1.4.W25 add
END_MESSAGE_MAP()

// CBondSafetyPage message handlers
BOOL CBondSafetyPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_CAPILLARY_LIMIT,
					theMsg.GetMsg(MSG_CAP_WEAR_OUT_LIMIT));
	SetDlgItemTextW(IDC_STATIC_OPEN_CLAMP_DELAY,
					theMsg.GetMsg(MSG_THERMAL_PROTECT_TIME));
	SetDlgItemTextW(IDC_STATIC_IMP_LIMIT,
					theMsg.GetMsg(MSG_IMPEDANCE));
	SetDlgItemTextW(IDC_STATIC_IMP_LOWER,
					theMsg.GetMsg(MSG_LOWER_LIMIT));
	SetDlgItemTextW(IDC_STATIC_IMP_UPPER,
					theMsg.GetMsg(MSG_UPPER_LIMIT));
	SetDlgItemTextW(IDC_STATIC_BONDFORCE_LIMIT,
					theMsg.GetMsg(MSG_BOND_FORCE_LIMIT));
	SetDlgItemTextW(IDC_STATIC_RELAY_SAFETY_DURATION,
					theMsg.GetMsg(MSG_RELAY_SAFETY_DURATION));	//V1.1.W149 add

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	m_lCapillaryWearOutLimit = pstMcData->lCapillaryLimit;
	m_nImpLowerLimit         = pstMcData->nImpLowerLimit;
	m_nImpUpperLimit         = pstMcData->nImpUpperLimit;

	m_lOpenClamperDelay    = round(thePgm.GetRcd()->lOxiPrevtTimeOut / 1000.0);
	m_nBondForceLimit      = thePgm.GetRcd()->nBondForceLimit;
	m_lRelaySafetyDuration = round(thePgm.GetRcd()->lRelaySafetyDuration / 1000.0);

	GetDlgItem(IDC_EDIT_BONDFORCE_LIMIT)->EnableWindow(theUser.GetUserLevel() >= USER_ENGINEER);

	return CPropertyPage::OnInitDialog();
}

HBRUSH CBondSafetyPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBondSafetyPage::OnFocusCapillaryLimit()
{
	GetDlgItem(IDC_STATIC_CAPILLARY_LIMIT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lCapillaryWearOutLimit);
	UpdateData(false);
}

void CBondSafetyPage::OnDestroy()
{
	UpdateData();

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	pstMcData->lCapillaryLimit = m_lCapillaryWearOutLimit;
	pstMcData->nImpLowerLimit  = m_nImpLowerLimit;
	pstMcData->nImpUpperLimit  = m_nImpUpperLimit;

	PGM_RECORD *pstPgm = thePgm.GetRcd();
	pstPgm->lOxiPrevtTimeOut = m_lOpenClamperDelay * 1000;
	pstPgm->nBondForceLimit = m_nBondForceLimit;
	if (m_lRelaySafetyDuration > 120)
		m_lRelaySafetyDuration = 120;	//maximum 2 minutes
	pstPgm->lRelaySafetyDuration = m_lRelaySafetyDuration * 1000;

	CPropertyPage::OnDestroy();
}

void CBondSafetyPage::OnFocusClamperOpenTime()
{
	GetDlgItem(IDC_STATIC_CAPILLARY_LIMIT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lOpenClamperDelay);
	UpdateData(false);
}

void CBondSafetyPage::OnFocusBondForceLimit()
{
	GetDlgItem(IDC_STATIC_CAPILLARY_LIMIT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBondForceLimit, m_nBondForceLimit, 0, 1000);
	UpdateData(false);
}

void CBondSafetyPage::OnFocusImpLowerLimit()
{
	GetDlgItem(IDC_STATIC_CAPILLARY_LIMIT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nImpLowerLimit);
	UpdateData(false);
}

void CBondSafetyPage::OnFocusImpUpperLimit()
{
	GetDlgItem(IDC_STATIC_CAPILLARY_LIMIT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nImpUpperLimit);
	UpdateData(false);
}

//v3.3.D148
void CBondSafetyPage::OnFocusRelaySafetyDuration()
{
	GetDlgItem(IDC_STATIC_CAPILLARY_LIMIT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lRelaySafetyDuration, m_lRelaySafetyDuration, 0, 120);
	UpdateData(false);
}
