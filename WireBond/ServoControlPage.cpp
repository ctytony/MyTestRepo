/***************************************************************
ServoControlPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "KeyPadHandler.h"
#include "ServoControlPage.h"
#include "servo.h"

// CServoControlPage dialog

IMPLEMENT_DYNAMIC(CServoControlPage, CPropertyPage)

CServoControlPage::CServoControlPage()
	: CPropertyPage(CServoControlPage::IDD)
    , m_nCtrlProfileZ(0)
    , m_lVelGain(0)
    , m_lVelIntegrator(0)
    , m_lPosGain(0)
    , m_lAccFeedForward(0)
    , m_lVelGainX(0)
    , m_lVelGainY(0)
    , m_lVelIntegratorX(0)
    , m_lVelIntegratorY(0)
    , m_lPosGainX(0)
    , m_lPosGainY(0)
    , m_lAccFeedForwardX(0)
    , m_lAccFeedForwardY(0)
    , m_nCtrlProfileX(0)
    , m_nCtrlProfileY(0)
{
}

CServoControlPage::~CServoControlPage()
{
}

void CServoControlPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_SETTING_NUM, m_nCtrlProfileZ);
    DDX_Text(pDX, IDC_EDIT_VEL_GAIN, m_lVelGain);
    DDX_Text(pDX, IDC_EDIT_VEL_INTEGRATOR, m_lVelIntegrator);
    DDX_Text(pDX, IDC_EDIT_POS_GAIN, m_lPosGain);
    DDX_Text(pDX, IDC_EDIT_FFC, m_lAccFeedForward);
    DDX_Text(pDX, IDC_EDIT_X_VEL_GAIN, m_lVelGainX);
    DDX_Text(pDX, IDC_EDIT_Y_VEL_GAIN, m_lVelGainY);
    DDX_Text(pDX, IDC_EDIT_X_VEL_INTEGRATOR, m_lVelIntegratorX);
    DDX_Text(pDX, IDC_EDIT_Y_VEL_INTEGRATOR, m_lVelIntegratorY);
    DDX_Text(pDX, IDC_EDIT_X_POS_GAIN, m_lPosGainX);
    DDX_Text(pDX, IDC_EDIT_Y_POS_GAIN, m_lPosGainY);
    DDX_Text(pDX, IDC_EDIT_X_FFC, m_lAccFeedForwardX);
    DDX_Text(pDX, IDC_EDIT_Y_FFC, m_lAccFeedForwardY);
    DDX_CBIndex(pDX, IDC_COMBO_BLOCK_NUM_X, m_nCtrlProfileX);
    DDX_CBIndex(pDX, IDC_COMBO_BLOCK_NUM_Y, m_nCtrlProfileY);
}

BEGIN_MESSAGE_MAP(CServoControlPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO_SETTING_NUM, OnSettingNum)
    ON_EN_SETFOCUS(IDC_EDIT_VEL_GAIN, OnFocusVelGain)
    ON_EN_SETFOCUS(IDC_EDIT_VEL_INTEGRATOR, OnFocusVelIntegrator)
    ON_EN_SETFOCUS(IDC_EDIT_POS_GAIN, OnFocusPosGain)
    ON_EN_SETFOCUS(IDC_EDIT_FFC, OnFocusFfc)
    ON_EN_SETFOCUS(IDC_EDIT_X_VEL_GAIN, OnFocusXVelGain)
    ON_EN_SETFOCUS(IDC_EDIT_Y_VEL_GAIN, OnFocusYVelGain)
    ON_EN_SETFOCUS(IDC_EDIT_X_VEL_INTEGRATOR, OnFocusXVelIntegrator)
    ON_EN_SETFOCUS(IDC_EDIT_Y_VEL_INTEGRATOR, OnFocusYVelIntegrator)
    ON_EN_SETFOCUS(IDC_EDIT_X_POS_GAIN, OnFocusXPosGain)
    ON_EN_SETFOCUS(IDC_EDIT_Y_POS_GAIN, OnFocusYPosGain)
    ON_EN_SETFOCUS(IDC_EDIT_X_FFC, OnFocusXFfc)
    ON_EN_SETFOCUS(IDC_EDIT_Y_FFC, OnFocusYFfc)
    ON_CBN_SELCHANGE(IDC_COMBO_BLOCK_NUM_X, OnBlockNumX)
    ON_CBN_SELCHANGE(IDC_COMBO_BLOCK_NUM_Y, OnBlockNumY)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CServoControlPage message handlers
BOOL CServoControlPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    m_nCtrlProfileZ = 0;

    SERVO_CONTROL_PROFILE stCtrlProfile;
    theServo.GetControlProfile(MOTOR_Z, m_nCtrlProfileZ, &stCtrlProfile);
    m_lVelGain        = stCtrlProfile.lVelGain;
    m_lVelIntegrator  = stCtrlProfile.lVelIntegrator;
    m_lPosGain        = stCtrlProfile.lPosGain;
    m_lAccFeedForward = stCtrlProfile.lAccFeedFwd;
    
    m_nCtrlProfileX = 0;
    m_nCtrlProfileY = 0;

    theServo.GetControlProfile(MOTOR_X, 0, &stCtrlProfile);
    m_lVelGainX        = stCtrlProfile.lVelGain;
    m_lVelIntegratorX  = stCtrlProfile.lVelIntegrator;
    m_lPosGainX        = stCtrlProfile.lPosGain;
    m_lAccFeedForwardX = stCtrlProfile.lAccFeedFwd;

    theServo.GetControlProfile(MOTOR_Y, 0, &stCtrlProfile);
    m_lVelGainY        = stCtrlProfile.lVelGain;
    m_lVelIntegratorY  = stCtrlProfile.lVelIntegrator;
    m_lPosGainY        = stCtrlProfile.lPosGain;
    m_lAccFeedForwardY = stCtrlProfile.lAccFeedFwd;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CServoControlPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CServoControlPage::OnSettingNum()
{
    SERVO_CONTROL_PROFILE stCtrlProfile;
    theServo.GetControlProfile(MOTOR_Z, m_nCtrlProfileZ, &stCtrlProfile);
    stCtrlProfile.lVelGain       = m_lVelGain;
    stCtrlProfile.lVelIntegrator = m_lVelIntegrator;
    stCtrlProfile.lPosGain       = m_lPosGain;
    stCtrlProfile.lAccFeedFwd    = m_lAccFeedForward;
    theServo.SetControlProfile(MOTOR_Z, m_nCtrlProfileZ, &stCtrlProfile);
    UpdateData();

    theServo.GetControlProfile(MOTOR_Z, m_nCtrlProfileZ, &stCtrlProfile);
    m_lVelGain        = stCtrlProfile.lVelGain;
    m_lVelIntegrator  = stCtrlProfile.lVelIntegrator;
    m_lPosGain        = stCtrlProfile.lPosGain;
    m_lAccFeedForward = stCtrlProfile.lAccFeedFwd;
    UpdateData(false);
}

void CServoControlPage::OnFocusVelGain()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lVelGain);
    UpdateData(false);
}

void CServoControlPage::OnFocusVelIntegrator()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lVelIntegrator);
    UpdateData(false);
}

void CServoControlPage::OnFocusPosGain()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lPosGain);
    UpdateData(false);
}

void CServoControlPage::OnFocusFfc()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lAccFeedForward);
    UpdateData(false);
}

void CServoControlPage::OnDestroy()
{
    SERVO_CONTROL_PROFILE stCtrlProfile;
    theServo.GetControlProfile(MOTOR_Z, m_nCtrlProfileZ, &stCtrlProfile);
    stCtrlProfile.lVelGain       = m_lVelGain;
    stCtrlProfile.lVelIntegrator = m_lVelIntegrator;
    stCtrlProfile.lPosGain       = m_lPosGain;
    stCtrlProfile.lAccFeedFwd    = m_lAccFeedForward;
    theServo.SetControlProfile(MOTOR_Z, m_nCtrlProfileZ, &stCtrlProfile);

    theServo.GetControlProfile(MOTOR_X, m_nCtrlProfileX, &stCtrlProfile);
    stCtrlProfile.lVelGain       = m_lVelGainX;
    stCtrlProfile.lVelIntegrator = m_lVelIntegratorX;
    stCtrlProfile.lPosGain       = m_lPosGainX;
    stCtrlProfile.lAccFeedFwd    = m_lAccFeedForwardX;
    theServo.SetControlProfile(MOTOR_X, m_nCtrlProfileX, &stCtrlProfile);

    theServo.GetControlProfile(MOTOR_Y, m_nCtrlProfileY, &stCtrlProfile);
    stCtrlProfile.lVelGain       = m_lVelGainY;
    stCtrlProfile.lVelIntegrator = m_lVelIntegratorY;
    stCtrlProfile.lPosGain       = m_lPosGainY;
    stCtrlProfile.lAccFeedFwd    = m_lAccFeedForwardY;
    theServo.SetControlProfile(MOTOR_Y, m_nCtrlProfileY, &stCtrlProfile);

    theServo.SetServoControlParameter();

    CPropertyPage::OnDestroy();
}

void CServoControlPage::OnFocusXVelGain()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lVelGainX);
    UpdateData(false);
}

void CServoControlPage::OnFocusYVelGain()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lVelGainY);
    UpdateData(false);
}

void CServoControlPage::OnFocusXVelIntegrator()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lVelIntegratorX);
    UpdateData(false);
}

void CServoControlPage::OnFocusYVelIntegrator()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lVelIntegratorY);
    UpdateData(false);
}

void CServoControlPage::OnFocusXPosGain()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lPosGainX);
    UpdateData(false);
}

void CServoControlPage::OnFocusYPosGain()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lPosGainY);
    UpdateData(false);
}

void CServoControlPage::OnFocusXFfc()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lAccFeedForwardX);
    UpdateData(false);
}

void CServoControlPage::OnFocusYFfc()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lAccFeedForwardY);
    UpdateData(false);
}

void CServoControlPage::OnBlockNumX()
{
    SERVO_CONTROL_PROFILE stCtrlProfile;
    theServo.GetControlProfile(MOTOR_X, m_nCtrlProfileX, &stCtrlProfile);
    stCtrlProfile.lVelGain       = m_lVelGainX;
    stCtrlProfile.lVelIntegrator = m_lVelIntegratorX;
    stCtrlProfile.lPosGain       = m_lPosGainX;
    stCtrlProfile.lAccFeedFwd    = m_lAccFeedForwardX;
    theServo.SetControlProfile(MOTOR_X, m_nCtrlProfileX, &stCtrlProfile);
    UpdateData();

    theServo.GetControlProfile(MOTOR_X, m_nCtrlProfileX, &stCtrlProfile);
    m_lVelGainX   = stCtrlProfile.lVelGain;
    m_lVelIntegratorX  = stCtrlProfile.lVelIntegrator;
    m_lPosGainX        = stCtrlProfile.lPosGain;
    m_lAccFeedForwardX = stCtrlProfile.lAccFeedFwd;
    UpdateData(false);
}

void CServoControlPage::OnBlockNumY()
{
    SERVO_CONTROL_PROFILE stCtrlProfile;
    theServo.GetControlProfile(MOTOR_Y, m_nCtrlProfileY, &stCtrlProfile);
    stCtrlProfile.lVelGain       = m_lVelGainY;
    stCtrlProfile.lVelIntegrator = m_lVelIntegratorY;
    stCtrlProfile.lPosGain       = m_lPosGainY;
    stCtrlProfile.lAccFeedFwd    = m_lAccFeedForwardY;
    theServo.SetControlProfile(MOTOR_Y, m_nCtrlProfileY, &stCtrlProfile);
    UpdateData();

    theServo.GetControlProfile(MOTOR_Y, m_nCtrlProfileY, &stCtrlProfile);
    m_lVelGainY       = stCtrlProfile.lVelGain;
    m_lVelIntegratorY      = stCtrlProfile.lVelIntegrator;
    m_lPosGainY            = stCtrlProfile.lPosGain;
    m_lAccFeedForwardY     = stCtrlProfile.lAccFeedFwd;
    UpdateData(false);
}
