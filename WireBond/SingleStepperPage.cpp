/***************************************************************
SingleStepperPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "SingleStepperPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "StepperControl.h"
#include "RollingClaw.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledOffload.h"
#include "MachineConfig.h"
#include "VEOnload.h"	
#include "VEIndex.h"
#include "Debug.h"
#include "User.h"

// CSingleStepperPage dialog

IMPLEMENT_DYNAMIC(CSingleStepperPage, CPropertyPage)

CSingleStepperPage::CSingleStepperPage()
	: CPropertyPage(CSingleStepperPage::IDD)
	, m_bStopStepper(false)
	, m_bStopRoll(false)
    , m_lClawSpeed(0)
{

}

CSingleStepperPage::~CSingleStepperPage()
{
}

void CSingleStepperPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_CLAW_SPEED, m_lClawSpeed);
}

BEGIN_MESSAGE_MAP(CSingleStepperPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_LCAW_STOP, &CSingleStepperPage::OnBnClickedButtonLcawStop)
	ON_BN_CLICKED(IDC_BUTTON_RCLAW_STOP, &CSingleStepperPage::OnBnClickedButtonRclawStop)
	ON_BN_CLICKED(IDC_BUTTON_LCLAW_RUN, &CSingleStepperPage::OnBnClickedButtonLclawRun)
	ON_BN_CLICKED(IDC_BUTTON_RCLAW_RUN, &CSingleStepperPage::OnBnClickedButtonRclawRun)
    ON_BN_CLICKED(IDC_BUTTON_PICKER_HOME, &CSingleStepperPage::OnBnClickedButtonPickerHome)
	ON_BN_CLICKED(IDC_BUTTON_PICKER_LIMIT, &CSingleStepperPage::OnBnClickedButtonPickerLimit)
    ON_BN_CLICKED(IDC_BUTTON_LEFT_CLAW_HOME, &CSingleStepperPage::OnBnClickedButtonLeftClawHome) //@1
    ON_BN_CLICKED(IDC_BUTTON_RIGHT_CLAW_HOME, &CSingleStepperPage::OnBnClickedButtonRightClawHome)
    ON_CBN_SELCHANGE(IDC_COMBO_CLAW_SPEED, &CSingleStepperPage::OnCbnSelchangeComboClawSpeed)
    ON_BN_CLICKED(IDC_BUTTON_ENABLE_STEPPER, &CSingleStepperPage::OnBnClickedButtonEnableStepper) //@5
    ON_BN_CLICKED(IDC_BUTTON_DISABLE_STEPPER, &CSingleStepperPage::OnBnClickedButtonDisableStepper)
    ON_BN_CLICKED(IDC_BUTTON_RESET, &CSingleStepperPage::OnBnClickedButtonReset)
END_MESSAGE_MAP()

// CSingleStepperPage message handlers
BOOL CSingleStepperPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_LCLAW_RUN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));
    pWnd->EnableWindow(true);

    pWnd = GetDlgItem(IDC_BUTTON_LCAW_STOP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

    pWnd = GetDlgItem(IDC_BUTTON_RCLAW_RUN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));
    pWnd->EnableWindow(true);

    pWnd = GetDlgItem(IDC_BUTTON_RCLAW_STOP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

	pWnd = GetDlgItem(IDC_STATIC_CLAWS);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAW));

	pWnd = GetDlgItem(IDC_STATIC_LF_PICKER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ONLOAD_PICKER));

	pWnd = GetDlgItem(IDC_STATIC_LEFT_CLAW);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LEFT_CLAW));

	pWnd = GetDlgItem(IDC_STATIC_RIGHT_CLAW);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RIGHT_CLAW));

	pWnd = GetDlgItem(IDC_BUTTON_LEFT_CLAW_HOME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

    pWnd = GetDlgItem(IDC_BUTTON_RIGHT_CLAW_HOME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_PICKER_HOME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_PICKER_LIMIT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LIMIT));

	pWnd = GetDlgItem(IDC_STATIC_SPEED);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SPEED));

	pWnd = GetDlgItem(IDC_STATIC_STEP_CTRL);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STEPPER_CTRL));

    bool bStepperOn = false;
    int iStatus = FOK;

    iStatus = theStepper.CheckPowerStatus(&bStepperOn);
    if (iStatus != FOK)
        bStepperOn = false;

    pWnd = GetDlgItem(IDC_BUTTON_ENABLE_STEPPER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ENABLE_2));
    if (bStepperOn == true)
        pWnd->EnableWindow(FALSE);
    else
        pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_BUTTON_DISABLE_STEPPER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DISABLE_2));
    if (bStepperOn == true)
        pWnd->EnableWindow(TRUE);
    else
        pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_RESET);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RESET));
    if (bStepperOn == true)
        pWnd->EnableWindow(TRUE);
    else
        pWnd->EnableWindow(FALSE);

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CLAW_SPEED);
    pComboBox->InsertString(0, theMsg.GetMsg(MSG_LOW));
    pComboBox->InsertString(1, theMsg.GetMsg(MSG_MIDDLE));
    pComboBox->InsertString(2, theMsg.GetMsg(MSG_HIGH));
    if (theUser.GetUserLevel() > USER_SERVICE)
        pComboBox->EnableWindow(TRUE);
    else
        pComboBox->EnableWindow(FALSE);

    STEP_SPEED_PROFILE stSpeedProfile;
    theVledTrack.theLeftClaw.GetSpeedProfile(&stSpeedProfile);
    if (stSpeedProfile.lDriveVelocity >= CLAW_HIGH_SPEED)
        m_lClawSpeed = 2;
    else if (stSpeedProfile.lDriveVelocity >= CLAW_MID_SPEED)
        m_lClawSpeed = 1;
    else
        m_lClawSpeed = 0;

    CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSingleStepperPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CSingleStepperPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(true);

	CPropertyPage::OnOK();
}

void CSingleStepperPage::OnBnClickedButtonLcawStop()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theStepper.StopMotor(SINGLE_CAP_LEFT_CLAW);
		break;
	case MHS_VLED_ELITE:
		theStepper.StopMotor(VE_STEPPER_INDEX_LEFT_CLAW);
		break;
	}
}

void CSingleStepperPage::OnBnClickedButtonRclawStop()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theStepper.StopMotor(SINGLE_CAP_RIGHT_CLAW);
		break;
	case MHS_VLED_ELITE:
		theStepper.StopMotor(VE_STEPPER_INDEX_RIGHT_CLAW);
		break;
	}
}

void CSingleStepperPage::OnBnClickedButtonLclawRun()
{
	UpdateData(true);

	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theStepper.MoveStep(SINGLE_CAP_LEFT_CLAW, ONE_ROUND_PULSE, MOTION_RELATIVE);
		do
		{
			Sleep(1);
		} while (theStepper.IsBusy(SINGLE_CAP_LEFT_CLAW));
		break;
	case MHS_VLED_ELITE:
		theStepper.MoveStep(VE_STEPPER_INDEX_LEFT_CLAW, ONE_ROUND_PULSE, MOTION_RELATIVE);
		do
		{
			Sleep(1);
		} while (theStepper.IsBusy(VE_STEPPER_INDEX_LEFT_CLAW));
		break;
	}
}

void CSingleStepperPage::OnBnClickedButtonRclawRun()
{
	UpdateData(true);

	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theStepper.MoveStep(SINGLE_CAP_RIGHT_CLAW, ONE_ROUND_PULSE, MOTION_RELATIVE);
		do
		{
			Sleep(1);
		} while (theStepper.IsBusy(SINGLE_CAP_RIGHT_CLAW));
		break;
	case MHS_VLED_ELITE:
		theStepper.MoveStep(VE_STEPPER_INDEX_RIGHT_CLAW, ONE_ROUND_PULSE, MOTION_RELATIVE);
		do
		{
			Sleep(1);
		} while (theStepper.IsBusy(VE_STEPPER_INDEX_RIGHT_CLAW));
		break;
	}
}

void CSingleStepperPage::OnBnClickedButtonPickerHome()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theVledOnload.SearchStepHome();
		Sleep(10);
		theVledOnload.WaitStepHomeFinish();
		break;
	case MHS_VLED_ELITE:
		theVEliteOnload.SearchStepHome();
		Sleep(10);
		theVEliteOnload.WaitStepHomeFinish();
		break;
	}
}

void CSingleStepperPage::OnBnClickedButtonPickerLimit()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theStepper.MoveStep(SINGLE_CAP_ONLOAD_PICKER, -STEPPER_LIMIT_POS, MOTION_RELATIVE);
		break;
	case MHS_VLED_ELITE:
		theStepper.MoveStep(VE_STEPPER_ONLOAD_PICKER, STEPPER_LIMIT_POS, MOTION_RELATIVE);
		break;
	}
}

void CSingleStepperPage::OnBnClickedButtonLeftClawHome()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theVledTrack.theLeftClaw.SearchHome();
		Sleep(20);
		theVledTrack.theLeftClaw.CheckHomeState();
		break;
	case MHS_VLED_ELITE:
		theVEliteTrack.theLeftClaw.SearchHome();
		Sleep(20);
		theVEliteTrack.theLeftClaw.CheckHomeState();
		break;
	}
}

void CSingleStepperPage::OnBnClickedButtonRightClawHome()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theVledTrack.theRightClaw.SearchHome();
		Sleep(20);
		theVledTrack.theRightClaw.CheckHomeState();
		break;
	case MHS_VLED_ELITE:
		theVEliteTrack.theRightClaw.SearchHome();
		Sleep(20);
		theVEliteTrack.theRightClaw.CheckHomeState();
		break;
	}
}

void CSingleStepperPage::OnCbnSelchangeComboClawSpeed()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    STEP_SPEED_PROFILE stSpeedProfile;

    theVledTrack.theLeftClaw.GetSpeedProfile(&stSpeedProfile);
    
    if (m_lClawSpeed == 0)
        stSpeedProfile.lDriveVelocity = CLAW_LOW_SPEED;
    else if (m_lClawSpeed == 1)
        stSpeedProfile.lDriveVelocity = CLAW_MID_SPEED;
    else
        stSpeedProfile.lDriveVelocity = CLAW_HIGH_SPEED;

    theVledTrack.theLeftClaw.SetSpeedProfile(&stSpeedProfile);

	theStepper.SetSpeedParameter(SINGLE_CAP_LEFT_CLAW, &stSpeedProfile);
	Sleep(100);
    theStepper.SetSpeedParameter(SINGLE_CAP_RIGHT_CLAW, &stSpeedProfile);
}

void CSingleStepperPage::OnBnClickedButtonEnableStepper() //@5
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_ENABLE_STEPPER); //@6
    pWnd->EnableWindow(FALSE);

    theStepper.PowerOn();
    Sleep(1000);

	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_VLED:
		theStepper.ServoOn(SINGLE_CAP_CLAMPER);
		break;
	case MHS_VLED_ELITE:
		theStepper.ServoOn(VE_STEPPER_INDEX_CLAMPER);
		break;
	}

	pWnd = GetDlgItem(IDC_BUTTON_DISABLE_STEPPER);
    pWnd->EnableWindow(TRUE);

    pWnd = GetDlgItem(IDC_BUTTON_RESET);
    pWnd->EnableWindow(TRUE);
}

void CSingleStepperPage::OnBnClickedButtonDisableStepper()
{
    // TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DISABLE_STEPPER); //@6
    pWnd->EnableWindow(FALSE);

    theStepper.PowerOff();

    pWnd = GetDlgItem(IDC_BUTTON_ENABLE_STEPPER);
    pWnd->EnableWindow(TRUE);

    pWnd = GetDlgItem(IDC_BUTTON_RESET);
    pWnd->EnableWindow(FALSE);
}

void CSingleStepperPage::OnBnClickedButtonReset()
{
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_RESET);
    pWnd->EnableWindow(FALSE);
    theMHS.ResetWorkHolder();
    pWnd->EnableWindow(TRUE);
}
