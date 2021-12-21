/***************************************************************
ServoGeneralPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "Bond.h"
#include "ServoGeneralPage.h"
#include "MachineConfig.h"
#include "MaterialHandlerThread.h"
#include "servo.h"
#include "User.h"
#include <memory>

// CServoGeneralPage dialog
IMPLEMENT_DYNAMIC(CServoGeneralPage, CPropertyPage)

CServoGeneralPage::CServoGeneralPage()
	: CPropertyPage(CServoGeneralPage::IDD)
    , m_nSearchMode(0)
    , m_nDampGain(0)
    , m_nDampGain2(0)
{
}

CServoGeneralPage::~CServoGeneralPage()
{
}

void CServoGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCH_MODE, m_nSearchMode);
	DDX_Text(pDX, IDC_EDIT_DAMP_GAIN, m_nDampGain);
	DDX_Text(pDX, IDC_EDIT_DAMP_GAIN2, m_nDampGain2);
}

BEGIN_MESSAGE_MAP(CServoGeneralPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_X_ENABLE, OnXEnable)
    ON_BN_CLICKED(IDC_BUTTON_X_DISABLE, OnXDisable)
    ON_BN_CLICKED(IDC_BUTTON_X_HOME, OnXHome)
    ON_BN_CLICKED(IDC_BUTTON_Y_ENABLE, OnYEnable)
    ON_BN_CLICKED(IDC_BUTTON_Y_DISABLE, OnYDisable)
    ON_BN_CLICKED(IDC_BUTTON_Y_HOME, OnYHome)
    ON_BN_CLICKED(IDC_BUTTON_Z_ENABLE, OnZEnable)
    ON_BN_CLICKED(IDC_BUTTON_Z_DISABLE, OnZDisable)
    ON_BN_CLICKED(IDC_BUTTON_Z_HOME, OnZHome)
    ON_BN_CLICKED(IDC_BUTTON_INITIALIZE, OnInitialize)
    ON_BN_CLICKED(IDC_BUTTON_RECOVER, OnRecover)
    ON_EN_SETFOCUS(IDC_EDIT_SEARCH_MODE, OnFocusSearchMode)
    ON_EN_SETFOCUS(IDC_EDIT_DAMP_GAIN, OnFocusDampGain)
    ON_EN_SETFOCUS(IDC_EDIT_DAMP_GAIN2, OnFocusDampGain2)
END_MESSAGE_MAP()

// CServoGeneralPage message handlers

BOOL CServoGeneralPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
    m_nSearchMode = pstMcData->nSearchMode;
    m_nDampGain   = pstMcData->nDampGain;
    m_nDampGain2  = pstMcData->nDampGain2;

    SetDlgItemTextW(IDC_BUTTON_X_HOME, theMsg.GetMsg(MSG_HOME));
    SetDlgItemTextW(IDC_BUTTON_X_ENABLE, theMsg.GetMsg(MSG_ENABLE_2));
    SetDlgItemTextW(IDC_BUTTON_X_DISABLE, theMsg.GetMsg(MSG_DISABLE_2));
    SetDlgItemTextW(IDC_BUTTON_Y_HOME, theMsg.GetMsg(MSG_HOME));
    SetDlgItemTextW(IDC_BUTTON_Y_ENABLE, theMsg.GetMsg(MSG_ENABLE_2));
    SetDlgItemTextW(IDC_BUTTON_Y_DISABLE, theMsg.GetMsg(MSG_DISABLE_2));
    SetDlgItemTextW(IDC_BUTTON_Z_HOME, theMsg.GetMsg(MSG_HOME));
    SetDlgItemTextW(IDC_BUTTON_Z_ENABLE, theMsg.GetMsg(MSG_ENABLE_2));
    SetDlgItemTextW(IDC_BUTTON_Z_DISABLE, theMsg.GetMsg(MSG_DISABLE_2));
    SetDlgItemTextW(IDC_STATIC_MOTORS, theMsg.GetMsg(MSG_MOTOR));
    SetDlgItemTextW(IDC_BUTTON_INITIALIZE, theMsg.GetMsg(MSG_INITIALIZE));
    SetDlgItemTextW(IDC_BUTTON_RECOVER, theMsg.GetMsg(MSG_RECOVER));
    SetDlgItemTextW(IDC_STATIC_SYSTEM, theMsg.GetMsg(MSG_SYSTEM));

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
		BYTE bEnable=theUser.GetUserLevel()>USER_SERVICE;

		GetDlgItem(IDC_EDIT_SEARCH_MODE)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_DAMP_GAIN)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_DAMP_GAIN2)->EnableWindow(bEnable);
    }
    else
    {
	    GetDlgItem(IDC_STATIC_CTACT_SRCH)->ShowWindow(SW_HIDE);
	    GetDlgItem(IDC_STATIC_SRCH_MODE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_DAMP_GAIN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_RCD_SAMPLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SEARCH_MODE)->ShowWindow(SW_HIDE);
	    GetDlgItem(IDC_EDIT_DAMP_GAIN)->ShowWindow(SW_HIDE);
	    GetDlgItem(IDC_EDIT_DAMP_GAIN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RCD_SAMPLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RCD_TOL)->ShowWindow(SW_HIDE);
    }

	if ((pstMcData->chRcdSample < 0) || (pstMcData->chRcdSample > 1))
		pstMcData->chRcdSample = 1;
	SetDlgItemInt(IDC_EDIT_RCD_SAMPLE, pstMcData->chRcdSample);

	pstMcData->chRcdTol = 1;
	SetDlgItemInt(IDC_EDIT_RCD_TOL, pstMcData->chRcdTol);

    return CPropertyPage::OnInitDialog();
}

HBRUSH CServoGeneralPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CServoGeneralPage::OnXEnable()
{
    theServo.EnableMotor(MOTOR_X);
}

void CServoGeneralPage::OnXDisable()
{
    theServo.DisableMotor(MOTOR_X);
}

void CServoGeneralPage::OnXHome()
{
	theMHS.CloseWindowClamp();
    theServo.EnableMotor(MOTOR_X);
    theServo.SearchHome(MOTOR_X);
}

void CServoGeneralPage::OnYEnable()
{
    theServo.EnableMotor(MOTOR_Y);
}

void CServoGeneralPage::OnYDisable()
{
    theServo.DisableMotor(MOTOR_Y);
}

void CServoGeneralPage::OnYHome()
{
	theMHS.CloseWindowClamp();
    theServo.EnableMotor(MOTOR_X);
    theServo.SearchHome(MOTOR_Y);
}

void CServoGeneralPage::OnZEnable()
{
     theServo.EnableMotor(MOTOR_Z);
}

void CServoGeneralPage::OnZDisable()
{
    theServo.DisableMotor(MOTOR_Z);
}

void CServoGeneralPage::OnZHome()
{
	theMHS.CloseWindowClamp();

	TBL_POS stTblPos;
	if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
		|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
		|| (theMcConfig.GetMhsType() == MHS_R2R)
		|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
	{
		stTblPos.dx = -6250;
		stTblPos.dy = 62170;   
	}
	else
	{
		stTblPos.dx = 41000; //10000;
		stTblPos.dy = 20000;  
	}
    theServo.MoveTable2(stTblPos);	//v3.1.T204 edit

    theServo.EnableMotor(MOTOR_Z);
    Sleep_x1ms(20);
    theServo.SearchLimit(MOTOR_Z);
    Sleep_x1ms(20);
    theServo.SearchBondHeadHomeFromLimit();
    Sleep_x1ms(20);
    theBond.MoveBondHeadToFireLevel();
}

void CServoGeneralPage::OnInitialize()
{
	theMHS.CloseWindowClamp();

	if (theServo.ServoPreparePowerOn() == FOK)
	{
        theServo.PowerOnServo();
        theBond.MoveBondHeadToFireLevel();
	}
}

void CServoGeneralPage::OnRecover()
{
    theServo.RecoverServo();
    theBond.MoveBondHeadToFireLevel();
}

void CServoGeneralPage::OnFocusSearchMode()
{
    GetDlgItem(IDC_BUTTON_X_HOME)->SetFocus();
	if (theMcConfig.GetAcsBufferType() == 1)
		theKeyPad.GetUserKeyInData(&m_nSearchMode, 1, 0, 1000);		//v1.6.P8 add
	else
		theKeyPad.GetUserKeyInData(&m_nSearchMode, 1, 1, 2);
    UpdateData(false);

	theMcData.GetRcd()->nSearchMode = m_nSearchMode;
}

void CServoGeneralPage::OnFocusDampGain()
{
    GetDlgItem(IDC_BUTTON_X_HOME)->SetFocus();
#if _SERVO_TYPE == 1	//v3.0.T485 add
	if (theMcConfig.GetAcsBufferType() == 1)
		theKeyPad.GetUserKeyInData(&m_nDampGain, 0, 0, 1000);	//v1.6.P8 add
	else
		theKeyPad.GetUserKeyInData(&m_nDampGain, 0, 0, 100);
#else if _SERVO_TYPE == 0	//v3.1.T230 edit
	if (theMcConfig.GetAcsBufferType() == 1)
		theKeyPad.GetUserKeyInData(&m_nDampGain, 0, 0, 1000);	//v1.6.P8 add
	else
		theKeyPad.GetUserKeyInData(&m_nDampGain, 0, 0, 255);	//v1.1.288.T12 add
#endif
    UpdateData(false);

	theMcData.GetRcd()->nDampGain = m_nDampGain;
}

void CServoGeneralPage::OnFocusDampGain2()
{
    GetDlgItem(IDC_BUTTON_X_HOME)->SetFocus();
#if _SERVO_TYPE == 1	//v3.0.T485 add
	if (theMcConfig.GetAcsBufferType() == 1)
		theKeyPad.GetUserKeyInData(&m_nDampGain2, 0, 0, 1000);	//v1.6.P8 add
	else
		theKeyPad.GetUserKeyInData(&m_nDampGain2, 0, 0, 100);
#else if _SERVO_TYPE == 0	//v3.1.T230 edit
	if (theMcConfig.GetAcsBufferType() == 1)
		theKeyPad.GetUserKeyInData(&m_nDampGain2, 0, 0, 1000);	//v1.6.P8 add
	else
		theKeyPad.GetUserKeyInData(&m_nDampGain2, 0, 0, 255);	//v1.1.288.T12 add
#endif
    UpdateData(false);

	theMcData.GetRcd()->nDampGain2 = m_nDampGain2;
}
