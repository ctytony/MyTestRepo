/***************************************************************
CfgAccPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "CfgWclProfilePage.h"
#include "Bond.h"
#include "Debug.h"
#include "servo.h"

// CCfgWclProfilePage dialog
IMPLEMENT_DYNAMIC(CCfgWclProfilePage, CPropertyPage)

CCfgWclProfilePage::CCfgWclProfilePage()
	: CPropertyPage(CCfgWclProfilePage::IDD)
	, m_nThreadWireOpenForce(100)
	, m_nThreadWireCloseForce(110)
	, m_nHoldWireCloseForce(50)
	, m_lProfileType(1)
	, m_nInterimForce(10)
	, m_nInterimRampTime(2)
	, m_nInterimHoldTime(2)
	, m_nHoldForce(80)
	, m_nHoldRampTime(2)
	, m_nStep2Force(0)
	, m_nStep2RampTime(0)
	, m_nStep2HoldTime(0)
{
}

CCfgWclProfilePage::~CCfgWclProfilePage()
{
}

void CCfgWclProfilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPEN, m_nThreadWireOpenForce);
	DDX_Text(pDX, IDC_EDIT_CLOSE, m_nThreadWireCloseForce);
	DDX_Text(pDX, IDC_EDIT_HOLD, m_nHoldWireCloseForce);
	DDX_CBIndex(pDX, IDC_COMBO_PROFILE, m_lProfileType);
	DDX_Text(pDX, IDC_EDIT_INTERIM_FORCE, m_nInterimForce);
	DDX_Text(pDX, IDC_EDIT_INTERIM_RAMP_TIME, m_nInterimRampTime);
	DDX_Text(pDX, IDC_EDIT_INTERIM_HOLD_TIME, m_nInterimHoldTime);
	DDX_Text(pDX, IDC_EDIT_HOLD_FORCE, m_nHoldForce);
	DDX_Text(pDX, IDC_EDIT_HOLD_RAMP_TIME, m_nHoldRampTime);
	DDX_Text(pDX, IDC_EDIT_STEP2_FORCE, m_nStep2Force);
	DDX_Text(pDX, IDC_EDIT_STEP2_RAMP_TIME, m_nStep2RampTime);
	DDX_Text(pDX, IDC_EDIT_STEP2_HOLD_TIME, m_nStep2HoldTime);
}

BEGIN_MESSAGE_MAP(CCfgWclProfilePage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_PROFILE, OnProfile)
	ON_EN_CHANGE(IDC_EDIT_INTERIM_FORCE, OnEditInterimForce)
	ON_EN_CHANGE(IDC_EDIT_INTERIM_RAMP_TIME, OnEditInterimRampTime)
	ON_EN_CHANGE(IDC_EDIT_INTERIM_HOLD_TIME, OnEditInterimHoldTime)
	ON_EN_CHANGE(IDC_EDIT_HOLD_FORCE, OnEditHoldForce)
	ON_EN_CHANGE(IDC_EDIT_HOLD_RAMP_TIME, OnEditHoldRampTime)
	ON_EN_SETFOCUS(IDC_EDIT_OPEN, OnFocusOpen)
	ON_EN_SETFOCUS(IDC_EDIT_CLOSE, OnFocusClose)
	ON_EN_SETFOCUS(IDC_EDIT_HOLD, OnFocusHold)
	ON_EN_SETFOCUS(IDC_EDIT_INTERIM_FORCE, OnFocusInterimForce)
	ON_EN_SETFOCUS(IDC_EDIT_INTERIM_RAMP_TIME, OnFocusInterimRampTime)
	ON_EN_SETFOCUS(IDC_EDIT_INTERIM_HOLD_TIME, OnFocusInterimHoldTime)
	ON_EN_SETFOCUS(IDC_EDIT_HOLD_FORCE, OnFocusHoldForce)
	ON_EN_SETFOCUS(IDC_EDIT_HOLD_RAMP_TIME, OnFocusHoldRampTime)
	ON_EN_SETFOCUS(IDC_EDIT_STEP2_FORCE, OnFocusStep2Force)
	ON_EN_SETFOCUS(IDC_EDIT_STEP2_RAMP_TIME, OnFocusStep2RampTime)
	ON_EN_SETFOCUS(IDC_EDIT_STEP2_HOLD_TIME, OnFocusStep2HoldTime)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCfgWclProfilePage message handlers
BOOL CCfgWclProfilePage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	ST_WCL_PROFILE stWclProfile;

	theServo.GetWireClampProfile(WCL_THREAD_WIRE_OPEN, &stWclProfile);
	m_nThreadWireOpenForce = stWclProfile.nHoldForce;

	theServo.GetWireClampProfile(WCL_THREAD_WIRE_CLOSE, &stWclProfile);
	m_nThreadWireCloseForce = abs(stWclProfile.nHoldForce);

	theServo.GetWireClampProfile(WCL_HOLD_WIRE_CLOSE, &stWclProfile);
	m_nHoldWireCloseForce = abs(stWclProfile.nHoldForce);

	theServo.GetWireClampProfile(m_lProfileType, &stWclProfile);
	m_nInterimForce    = abs(stWclProfile.nInterimForce);
	m_nInterimRampTime = stWclProfile.nInterimRampTime;
	m_nInterimHoldTime = stWclProfile.nInterimHoldTime;
	m_nHoldForce       = abs(stWclProfile.nHoldForce);
	m_nHoldRampTime    = stWclProfile.nHoldRamptime;
	m_nStep2RampTime   = stWclProfile.nStep2RampTime;
	m_nStep2HoldTime   = stWclProfile.nStep2HoldTime;
	m_nStep2Force      = abs(stWclProfile.nStep2Force);

	SetDlgItemTextW(IDC_STATIC_GENERAL, theMsg.GetMsg(MSG_GENERAL_PROFILE));
	SetDlgItemTextW(IDC_STATIC_THREAD_WIRE_OPEN, theMsg.GetMsg(MSG_THREAD_WIRE_OPEN_FORCE));
	SetDlgItemTextW(IDC_STATIC_THREAD_WIRE_CLOSE, theMsg.GetMsg(MSG_THREAD_WIRE_CLOSE_FORCE));
	SetDlgItemTextW(IDC_STATIC_HOLD_WIRE_CLOSE, theMsg.GetMsg(MSG_HOLD_WIRE_CLOSE_FORCE));
	SetDlgItemTextW(IDC_STATIC_BOND_PROFILE, theMsg.GetMsg(MSG_BOND_PROFILE));
	SetDlgItemTextW(IDC_STATIC_PROFILE_TYPE, theMsg.GetMsg(MSG_PROFILE_TYPE));
	SetDlgItemTextW(IDC_STATIC_1ST_STEP_FORCE, theMsg.GetMsg(MSG_1ST_STEP_FORCE));
	SetDlgItemTextW(IDC_STATIC_1ST_STEP_RAMP_TIME, theMsg.GetMsg(MSG_1ST_STEP_RAMP_TIME));
	SetDlgItemTextW(IDC_STATIC_1ST_STEP_HOLD_TIME, theMsg.GetMsg(MSG_1ST_STEP_HOLD_TIME));
	SetDlgItemTextW(IDC_STATIC_2ND_STEP_FORCE, theMsg.GetMsg(MSG_2ND_STEP_FORCE));
	SetDlgItemTextW(IDC_STATIC_2ND_STEP_RAMP_TIME, theMsg.GetMsg(MSG_2ND_STEP_RAMP_TIME));
	SetDlgItemTextW(IDC_STATIC_2ND_STEP_HOLD_TIME, theMsg.GetMsg(MSG_2ND_STEP_HOLD_TIME));
	SetDlgItemTextW(IDC_STATIC_CLAMP_FORCE, theMsg.GetMsg(MSG_CLAMP_FORCE));
	SetDlgItemTextW(IDC_STATIC_CLAMP_RAMP_TIME, theMsg.GetMsg(MSG_CLAMP_RAMP_TIME));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PROFILE);
	pComboBox->AddString(theMsg.GetMsg(MSG_BALL_SIT_OPEN_CLOSE));
	pComboBox->AddString(theMsg.GetMsg(MSG_SRCH_HT_OPEN));
	pComboBox->AddString(theMsg.GetMsg(MSG_PEAK_HT_CLOSE));
	pComboBox->AddString(theMsg.GetMsg(MSG_TAIL_HT_CLOSE));

	return CPropertyPage::OnInitDialog();
}

HBRUSH CCfgWclProfilePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}
	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCfgWclProfilePage::OnDestroy()
{
	UpdateData(true);

	ST_WCL_PROFILE stWclProfile;
	theServo.GetWireClampProfile(WCL_THREAD_WIRE_OPEN, &stWclProfile);
	stWclProfile.nStep2Force = m_nThreadWireOpenForce;
	stWclProfile.nHoldForce  = m_nThreadWireOpenForce;
	theServo.SetWireClampProfile(WCL_THREAD_WIRE_OPEN, &stWclProfile);

	theServo.GetWireClampProfile(WCL_THREAD_WIRE_CLOSE, &stWclProfile);
#if _SERVO_TYPE == 2	//v3.1.T353 add
	stWclProfile.nStep2Force = m_nThreadWireCloseForce;
	stWclProfile.nHoldForce  = m_nThreadWireCloseForce;
#else
	if (theMcConfig.GetWclType() == 1)	//v3.1.T496 add
	{
		stWclProfile.nStep2Force = m_nThreadWireCloseForce;
		stWclProfile.nHoldForce  = m_nThreadWireCloseForce;
	}
	else
	{
		stWclProfile.nStep2Force = -m_nThreadWireCloseForce;
		stWclProfile.nHoldForce  = -m_nThreadWireCloseForce;
	}
#endif
	theServo.SetWireClampProfile(WCL_THREAD_WIRE_CLOSE, &stWclProfile);

	theServo.GetWireClampProfile(WCL_HOLD_WIRE_CLOSE, &stWclProfile);
#if _SERVO_TYPE == 2	//v3.1.T353 add
	stWclProfile.nInterimForce = m_nHoldWireCloseForce;
	stWclProfile.nStep2Force   = m_nHoldWireCloseForce;
	stWclProfile.nHoldForce    = m_nHoldWireCloseForce;
#else
	if (theMcConfig.GetWclType() == 1)	//v3.1.T496 add
	{
		stWclProfile.nInterimForce = m_nHoldWireCloseForce;
		stWclProfile.nStep2Force   = m_nHoldWireCloseForce;
		stWclProfile.nHoldForce    = m_nHoldWireCloseForce;
	}
	else
	{
		stWclProfile.nInterimForce = -m_nHoldWireCloseForce;
		stWclProfile.nStep2Force   = -m_nHoldWireCloseForce;
		stWclProfile.nHoldForce    = -m_nHoldWireCloseForce;
	}
#endif
	theServo.SetWireClampProfile(WCL_HOLD_WIRE_CLOSE, &stWclProfile);
	theServo.UpdateWireClampProfile(theDebug.GetDebugMode(), theBond.IsDryRun());
	CPropertyPage::OnDestroy();
}

void CCfgWclProfilePage::OnProfile()
{
	UpdateData(true);
	ST_WCL_PROFILE stWclProfile;
	theServo.GetWireClampProfile(m_lProfileType, &stWclProfile);
	m_nInterimForce    = abs(stWclProfile.nInterimForce);
	m_nInterimRampTime = stWclProfile.nInterimRampTime;
	m_nInterimHoldTime = stWclProfile.nInterimHoldTime;
	m_nHoldForce       = abs(stWclProfile.nHoldForce);
	m_nHoldRampTime    = stWclProfile.nHoldRamptime;
	m_nStep2RampTime   = stWclProfile.nStep2RampTime;
	m_nStep2HoldTime   = stWclProfile.nStep2HoldTime;
	m_nStep2Force      = abs(stWclProfile.nStep2Force);
	UpdateData(false);
}

void CCfgWclProfilePage::OnEditInterimForce()
{
    UpdateWclProfile();
}

void CCfgWclProfilePage::OnEditInterimRampTime()
{
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnEditInterimHoldTime()
{
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnEditHoldForce()
{
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnEditHoldRampTime()
{
	UpdateWclProfile();
}

void CCfgWclProfilePage::UpdateWclProfile(void)
{
	UpdateData(true);

	ST_WCL_PROFILE stWclProfile;
	theServo.GetWireClampProfile(m_lProfileType, &stWclProfile);
	if (((m_lProfileType == WCL_PEAK_HEIGHT_CLOSE)
		|| (m_lProfileType == WCL_TAIL_HEIGHT_CLOSE))
		&& (theMcConfig.GetWclType() == 0))	//v3.1.T496 add
		stWclProfile.nInterimForce = -m_nInterimForce;
	else
		stWclProfile.nInterimForce = m_nInterimForce;
#if _SERVO_TYPE == 2	//v3.1.T398 add
	stWclProfile.nInterimForce = m_nInterimForce;
#endif
	stWclProfile.nInterimRampTime = m_nInterimRampTime;
	stWclProfile.nInterimHoldTime = m_nInterimHoldTime;

	stWclProfile.nStep2RampTime = m_nStep2RampTime;
	stWclProfile.nStep2HoldTime = m_nStep2HoldTime;
	if ((m_lProfileType == WCL_SEARCH_HEIGHT_OPEN)
		|| (theMcConfig.GetWclType() == 1))	//v3.1.T496 add
	{
		stWclProfile.nStep2Force = m_nStep2Force;
		stWclProfile.nHoldForce  = m_nHoldForce;
	}
	else
	{
		stWclProfile.nStep2Force = -m_nStep2Force;
		stWclProfile.nHoldForce  = -m_nHoldForce;
	}
#if _SERVO_TYPE == 2	//v3.1.T398 add
	stWclProfile.nStep2Force = m_nStep2Force;
	stWclProfile.nHoldForce  = m_nHoldForce;
#endif
	stWclProfile.nHoldRamptime  = m_nHoldRampTime;
	theServo.SetWireClampProfile(m_lProfileType, &stWclProfile);
}

void CCfgWclProfilePage::OnFocusOpen()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nThreadWireOpenForce);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusClose()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nThreadWireCloseForce);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusHold()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHoldWireCloseForce);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusInterimForce()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nInterimForce);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusInterimRampTime()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nInterimRampTime);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusInterimHoldTime()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nInterimHoldTime);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusHoldForce()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHoldForce);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusHoldRampTime()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHoldRampTime);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusStep2Force()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nStep2Force);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusStep2RampTime()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nStep2RampTime);
	UpdateData(false);
	UpdateWclProfile();
}

void CCfgWclProfilePage::OnFocusStep2HoldTime()
{
	GetDlgItem(IDC_STATIC_GENERAL)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nStep2HoldTime);
	UpdateData(false);
	UpdateWclProfile();
}
