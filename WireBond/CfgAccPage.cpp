/***************************************************************
CfgAccPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "CfgAccPage.h"
#include "ServoConfigSheet.h"
#include "servo.h"

// CCfgAccPage dialog
IMPLEMENT_DYNAMIC(CCfgAccPage, CPropertyPage)

CCfgAccPage::CCfgAccPage()
	: CPropertyPage(CCfgAccPage::IDD)
    , m_iTblProfileNum(0)
    , m_iBondHeadProfileNum(0)
    , m_nTblMaxAcc(0)
    , m_iTblJerk(0)
    , m_nBondHeadMaxAcc(0)
    , m_iBondHeadJerk(0)
    , m_nTblMaxAccY(0)
    , m_iTblJerkY(0)
{
}

CCfgAccPage::~CCfgAccPage()
{
}

void CCfgAccPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_TBL_PROFILE, m_iTblProfileNum);
    DDX_CBIndex(pDX, IDC_COMBO_Z_PROFILE, m_iBondHeadProfileNum);
    DDX_Text(pDX, IDC_EDIT_TBL_ACC, m_nTblMaxAcc);
    DDX_Text(pDX, IDC_EDIT_TBL_JERK, m_iTblJerk);
    DDX_Text(pDX, IDC_EDIT_BH_ACC, m_nBondHeadMaxAcc);
    DDX_Text(pDX, IDC_EDIT_BH_JERK, m_iBondHeadJerk);
    DDX_Text(pDX, IDC_EDIT_Y_ACC, m_nTblMaxAccY);
    DDX_Text(pDX, IDC_EDIT_Y_JERK, m_iTblJerkY);
}

BEGIN_MESSAGE_MAP(CCfgAccPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO_TBL_PROFILE, OnTblProfile)
    ON_CBN_SELCHANGE(IDC_COMBO_Z_PROFILE, OnZProfile)
    ON_EN_SETFOCUS(IDC_EDIT_TBL_ACC, OnFocusTblAcc)
    ON_EN_SETFOCUS(IDC_EDIT_TBL_JERK, OnFocusTblJerk)
    ON_EN_SETFOCUS(IDC_EDIT_BH_ACC, OnFocusBhAcc)
    ON_EN_SETFOCUS(IDC_EDIT_BH_JERK, OnFocusBhJerk)
    ON_EN_SETFOCUS(IDC_EDIT_Y_ACC, OnFocusYAcc)
    ON_EN_SETFOCUS(IDC_EDIT_Y_JERK, OnFocusYJerk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCfgAccPage message handlers
BOOL CCfgAccPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    m_iTblProfileNum      = 0;
    m_iBondHeadProfileNum = 0;
    
    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_X, m_iTblProfileNum, &stServoSpeedProfile);
    m_nTblMaxAcc = stServoSpeedProfile.nMaxAcc;
    m_iTblJerk = stServoSpeedProfile.lJerk;
    
    theServo.GetSpeedProfile(MOTOR_Y, m_iTblProfileNum, &stServoSpeedProfile); //@2
    m_nTblMaxAccY = stServoSpeedProfile.nMaxAcc;
    m_iTblJerkY = stServoSpeedProfile.lJerk;

    theServo.GetSpeedProfile(MOTOR_Z, m_iBondHeadProfileNum, &stServoSpeedProfile);
    m_nBondHeadMaxAcc = stServoSpeedProfile.nMaxAcc;
    m_iBondHeadJerk = stServoSpeedProfile.lJerk / 1000;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CCfgAccPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCfgAccPage::OnDestroy()
{
    theServo.SetServoSpeedProfile(MOTOR_X, 3);
    theServo.SetServoSpeedProfile(MOTOR_Y, 3);
    theServo.SetServoSpeedProfile(MOTOR_Z, 0);

    theServo.SetTableKillDec(); //@4

    CPropertyPage::OnDestroy();
}

void CCfgAccPage::OnTblProfile()
{
    UpdateData(true);

    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_X, m_iTblProfileNum, &stServoSpeedProfile);
    m_nTblMaxAcc = stServoSpeedProfile.nMaxAcc;
    m_iTblJerk = stServoSpeedProfile.lJerk;

    theServo.GetSpeedProfile(MOTOR_Y, m_iTblProfileNum, &stServoSpeedProfile); //@2
    m_nTblMaxAccY = stServoSpeedProfile.nMaxAcc;
    m_iTblJerkY = stServoSpeedProfile.lJerk;

    UpdateData(false);
}

void CCfgAccPage::OnZProfile()
{
    UpdateData(true);
    
    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_Z, m_iBondHeadProfileNum, &stServoSpeedProfile);
    m_nBondHeadMaxAcc = stServoSpeedProfile.nMaxAcc;
    m_iBondHeadJerk = stServoSpeedProfile.lJerk / 1000;
    
    UpdateData(false);
}

void CCfgAccPage::OnFocusTblAcc()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nTblMaxAcc);

    UpdateData(false);

    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_X, m_iTblProfileNum, &stServoSpeedProfile);
    stServoSpeedProfile.nMaxAcc = m_nTblMaxAcc;
    theServo.SetSpeedProfile(MOTOR_X, m_iTblProfileNum, &stServoSpeedProfile);
}

void CCfgAccPage::OnFocusTblJerk()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_iTblJerk,20000, 0, 50000); //@3

    UpdateData(false);

    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_X, m_iTblProfileNum, &stServoSpeedProfile);
    stServoSpeedProfile.lJerk = m_iTblJerk;
    theServo.SetSpeedProfile(MOTOR_X, m_iTblProfileNum, &stServoSpeedProfile);
}

void CCfgAccPage::OnFocusBhAcc()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nBondHeadMaxAcc);

    UpdateData(false);

    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_Z, m_iBondHeadProfileNum, &stServoSpeedProfile);
    stServoSpeedProfile.nMaxAcc = m_nBondHeadMaxAcc;
    theServo.SetSpeedProfile(MOTOR_Z, m_iBondHeadProfileNum, &stServoSpeedProfile);
}

void CCfgAccPage::OnFocusBhJerk()
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_iBondHeadJerk, 250,0, 2500); //@1 @3

    UpdateData(false);
    
    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_Z, m_iBondHeadProfileNum, &stServoSpeedProfile);
    stServoSpeedProfile.lJerk = m_iBondHeadJerk * 1000;
    theServo.SetSpeedProfile(MOTOR_Z, m_iBondHeadProfileNum, &stServoSpeedProfile);
}

void CCfgAccPage::OnFocusYAcc() //@2
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nTblMaxAccY);

    UpdateData(false);

    SERVO_SPEED_PROFILE stServoSpeedProfile;

    theServo.GetSpeedProfile(MOTOR_Y, m_iTblProfileNum, &stServoSpeedProfile);
    stServoSpeedProfile.nMaxAcc = m_nTblMaxAccY;
    theServo.SetSpeedProfile(MOTOR_Y, m_iTblProfileNum, &stServoSpeedProfile);
}

void CCfgAccPage::OnFocusYJerk() //@2
{
    GetDlgItem(IDC_STATIC_TABLE)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_iTblJerkY, 20000,0, 50000); //@3

    UpdateData(false);

    SERVO_SPEED_PROFILE stServoSpeedProfile;
    theServo.GetSpeedProfile(MOTOR_Y, m_iTblProfileNum, &stServoSpeedProfile);
    stServoSpeedProfile.lJerk = m_iTblJerkY;
    theServo.SetSpeedProfile(MOTOR_Y, m_iTblProfileNum, &stServoSpeedProfile);
}
