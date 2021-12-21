/***************************************************************
ScrubParaPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BondPara.h"
#include "ScrubParaPage.h"

// CScrubParaPage dialog
IMPLEMENT_DYNAMIC(CScrubParaPage, CPropertyPage)

CScrubParaPage::CScrubParaPage()
	: CPropertyPage(CScrubParaPage::IDD)
    , m_i2ndBondScrubEnable(0)
    , m_nScrubPosOffset1(0)
    , m_nScrubPosOffset2(0)
    , m_n2ndScrubCycle(0)
    , m_n2ndScrubAmplitude(0)
    , m_i2ndScrubDirection(0)
    , m_n2ndScrubSpeed(0)
    , m_n2ndScrubForce(0)
    , m_n2ndScrubPower(0) //@1
    , m_i1stBondScrubEnable(0)
    , m_n1stScrubCycle(0)
    , m_n1stScrubAmplitude(0)
    , m_i1stScrubDirection(0)
    , m_n1stScrubSpeed(0)
    , m_n1stScrubForce(0)
    , m_n1stScrubPower(0)
{
}

CScrubParaPage::~CScrubParaPage()
{
}

void CScrubParaPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_2ND_SCRUB, m_i2ndBondScrubEnable);
    DDX_Text(pDX, IDC_EDIT_SCRUB_POS_OFFSET1, m_nScrubPosOffset1);
    DDX_Text(pDX, IDC_EDIT_SCRUB_POS_OFFSET2, m_nScrubPosOffset2);
    DDX_Text(pDX, IDC_EDIT_SCRUB_CYCLE, m_n2ndScrubCycle);
    DDX_Text(pDX, IDC_EDIT_AMPLITUDE, m_n2ndScrubAmplitude);
    DDX_CBIndex(pDX, IDC_COMBO_SCRUB_DIRECTION, m_i2ndScrubDirection);
    DDX_Text(pDX, IDC_EDIT_SCRUB_SPEED, m_n2ndScrubSpeed);
    DDX_Text(pDX, IDC_EDIT_2ND_SCRUB_FORCE, m_n2ndScrubForce);
    DDX_Text(pDX, IDC_EDIT_2ND_SCRUB_POWER, m_n2ndScrubPower);
    DDX_CBIndex(pDX, IDC_COMBO_1ST_SCRUB, m_i1stBondScrubEnable); //@1
    DDX_Text(pDX, IDC_EDIT_1ST_SCRUB_CYCLE, m_n1stScrubCycle);
    DDX_Text(pDX, IDC_EDIT_1ST_SCRUB_AMP, m_n1stScrubAmplitude);
    DDX_CBIndex(pDX, IDC_COMBO_1ST_SCRUB_DIR, m_i1stScrubDirection);
    DDX_Text(pDX, IDC_EDIT_1ST_SCRUB_SPEED, m_n1stScrubSpeed);
    DDX_Text(pDX, IDC_EDIT_1ST_SCRUB_FORCE, m_n1stScrubForce);
    DDX_Text(pDX, IDC_EDIT_1ST_SCRUB_POWER, m_n1stScrubPower);
}

BEGIN_MESSAGE_MAP(CScrubParaPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_SCRUB_POS_OFFSET1, OnFocusScrubPosOffset1)
    ON_EN_SETFOCUS(IDC_EDIT_SCRUB_POS_OFFSET2, OnFocusScrubPosOffset2)
    ON_EN_SETFOCUS(IDC_EDIT_SCRUB_CYCLE, OnFocusScrubCycle)
    ON_EN_SETFOCUS(IDC_EDIT_AMPLITUDE, OnFocusAmplitude)
    ON_EN_SETFOCUS(IDC_EDIT_SCRUB_SPEED, OnFocusScrubSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_2ND_SCRUB_FORCE, OnFocus2ndScrubForce)
    ON_EN_SETFOCUS(IDC_EDIT_2ND_SCRUB_POWER, OnFocus2ndScrubPower)
	ON_CBN_SELCHANGE(IDC_COMBO_2ND_SCRUB, On2ndScrub)
    ON_CBN_SELCHANGE(IDC_COMBO_1ST_SCRUB, On1stScrub) //@1
    ON_EN_SETFOCUS(IDC_EDIT_1ST_SCRUB_CYCLE, OnFocus1stScrubCycle)
    ON_EN_SETFOCUS(IDC_EDIT_1ST_SCRUB_AMP, OnFocus1stScrubAmp)
    ON_CBN_SELCHANGE(IDC_COMBO_1ST_SCRUB_DIR, On1stScrubDir)
    ON_EN_SETFOCUS(IDC_EDIT_1ST_SCRUB_SPEED, OnFocus1stScrubSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_1ST_SCRUB_FORCE, OnFocus1stScrubForce)
    ON_EN_SETFOCUS(IDC_EDIT_1ST_SCRUB_POWER, OnFocus1stScrubPower)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CScrubParaPage message handlers
BOOL CScrubParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_STATIC_BALL,theMsg.GetMsg(MSG_BALL));
    SetDlgItemTextW(IDC_STATIC_WEDEG,theMsg.GetMsg(MSG_WEDGE));
    SetDlgItemTextW(IDC_STATIC_2ND_SCRUB_ENABLE,theMsg.GetMsg(MSG_SCRUB_GROUP));
    SetDlgItemTextW(IDC_STATIC_SCRUB_POS_OFFSET1,theMsg.GetMsg(MSG_SCRUB_POS_OFFSET1));
    SetDlgItemTextW(IDC_STATIC_SCRUB_POS_OFFSET2,theMsg.GetMsg(MSG_SCRUB_POS_OFFSET2));
    SetDlgItemTextW(IDC_STATIC_CYCLE,theMsg.GetMsg(MSG_CYCLE));
    SetDlgItemTextW(IDC_STATIC_AMPLITUDE,theMsg.GetMsg(MSG_AMPLITUDE));
    SetDlgItemTextW(IDC_STATIC_DIRECTION,theMsg.GetMsg(MSG_DIRECTION));

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_1ST_SCRUB_DIR);
	pComboBox->AddString(theMsg.GetMsg(MSG_VERTICAL));
	pComboBox->AddString(theMsg.GetMsg(MSG_HORIZONTAL));
	pComboBox->AddString(theMsg.GetMsg(MSG_CIRCLE));

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SCRUB_DIRECTION);
    pComboBox->AddString(theMsg.GetMsg(MSG_VERTICAL));
	pComboBox->AddString(theMsg.GetMsg(MSG_HORIZONTAL));

    SetDlgItemTextW(IDC_STATIC_SPEED,theMsg.GetMsg(MSG_SPEED_PERCENT));
    SetDlgItemTextW(IDC_STATIC_2ND_SCRUB_FORCE,theMsg.GetMsg(MSG_FORCE_GM));
    SetDlgItemTextW(IDC_STATIC_2ND_SCRUB_POWER,theMsg.GetMsg(MSG_POWER_DAC));

    SCRUB_PARA_RCD stScrubRcd;

    theBondPara.GetBondScrubRcd(FIRST_BOND, m_i1stBondScrubEnable, &stScrubRcd);
    m_n1stScrubCycle      = stScrubRcd.nScrubCycle;
    m_n1stScrubAmplitude  = stScrubRcd.nScrubAmplitude;
    m_i1stScrubDirection  = stScrubRcd.chScrubDirection;
    m_n1stScrubSpeed      = stScrubRcd.nScrubSpeed;
    m_n1stScrubForce      = stScrubRcd.nScrubForce;
    m_n1stScrubPower      = round(stScrubRcd.fScrubPower * 2.55);

    theBondPara.GetBondScrubRcd(SECOND_BOND, m_i2ndBondScrubEnable, &stScrubRcd);
    m_nScrubPosOffset1    = stScrubRcd.nScrubPosOffset1;
    m_nScrubPosOffset2    = stScrubRcd.nScrubPosOffset2;
    m_n2ndScrubCycle      = stScrubRcd.nScrubCycle;
    m_n2ndScrubAmplitude  = stScrubRcd.nScrubAmplitude;
    m_i2ndScrubDirection  = stScrubRcd.chScrubDirection;
    m_n2ndScrubSpeed      = stScrubRcd.nScrubSpeed;
    m_n2ndScrubForce      = stScrubRcd.nScrubForce;
    m_n2ndScrubPower      = round(stScrubRcd.fScrubPower * 2.55);

    return CPropertyPage::OnInitDialog();
}

HBRUSH CScrubParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CScrubParaPage::OnDestroy()
{
	UpdateData();

    SCRUB_PARA_RCD stScrubRcd; //@4

    theBondPara.GetBondScrubRcd(FIRST_BOND, m_i1stBondScrubEnable, &stScrubRcd);
    stScrubRcd.nScrubCycle      = m_n1stScrubCycle;
    stScrubRcd.nScrubAmplitude  = m_n1stScrubAmplitude;
    stScrubRcd.chScrubDirection = m_i1stScrubDirection;
    stScrubRcd.nScrubSpeed      = m_n1stScrubSpeed;
    stScrubRcd.nScrubForce      = m_n1stScrubForce;
    stScrubRcd.fScrubPower      = (float)m_n1stScrubPower / (float)2.55;
    theBondPara.SetBondScrubRcd(FIRST_BOND, m_i1stBondScrubEnable, &stScrubRcd);

    theBondPara.GetBondScrubRcd(SECOND_BOND, m_i2ndBondScrubEnable, &stScrubRcd);
    stScrubRcd.nScrubPosOffset1 = m_nScrubPosOffset1;
    stScrubRcd.nScrubPosOffset2 = m_nScrubPosOffset2;
    stScrubRcd.nScrubCycle      = m_n2ndScrubCycle;
    stScrubRcd.nScrubAmplitude  = m_n2ndScrubAmplitude;
    stScrubRcd.chScrubDirection = m_i2ndScrubDirection;
    stScrubRcd.nScrubSpeed      = m_n2ndScrubSpeed;
    stScrubRcd.nScrubForce      = m_n2ndScrubForce;
    stScrubRcd.fScrubPower      = (float)m_n2ndScrubPower / (float)2.55;
    theBondPara.SetBondScrubRcd(SECOND_BOND, m_i2ndBondScrubEnable, &stScrubRcd);

	CPropertyPage::OnDestroy();
}

void CScrubParaPage::OnFocusScrubPosOffset1()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nScrubPosOffset1, 0, -20, 20); //@2

    UpdateData(false);
}

void CScrubParaPage::OnFocusScrubPosOffset2()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nScrubPosOffset2, 0, -20, 20); //@2

    UpdateData(false);
}

void CScrubParaPage::OnFocusScrubCycle()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_n2ndScrubCycle, 1, 0, 3); //@2

    UpdateData(false);
}

void CScrubParaPage::OnFocusAmplitude()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_n2ndScrubAmplitude, 2, 0, 20);

    UpdateData(false);
}

void CScrubParaPage::OnFocusScrubSpeed()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_n2ndScrubSpeed, 10, 5, 50);

    UpdateData(false);
}

void CScrubParaPage::OnFocus2ndScrubForce()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_n2ndScrubForce, 50, 0, 200);

    UpdateData(false);
}

void CScrubParaPage::OnFocus2ndScrubPower()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_n2ndScrubPower, 10, 0, 200);

    UpdateData(false);
}

void CScrubParaPage::On2ndScrub()
{
    SCRUB_PARA_RCD stScrubRcd;

    theBondPara.GetBondScrubRcd(SECOND_BOND, m_i2ndBondScrubEnable, &stScrubRcd);
    stScrubRcd.nScrubPosOffset1 = m_nScrubPosOffset1;
    stScrubRcd.nScrubPosOffset2 = m_nScrubPosOffset2;
    stScrubRcd.nScrubCycle      = m_n2ndScrubCycle;
    stScrubRcd.nScrubAmplitude  = m_n2ndScrubAmplitude;
    stScrubRcd.chScrubDirection = m_i2ndScrubDirection;
    stScrubRcd.nScrubSpeed      = m_n2ndScrubSpeed;
    stScrubRcd.nScrubForce      = m_n2ndScrubForce;
    stScrubRcd.fScrubPower      = (float)m_n2ndScrubPower / (float)2.55;
    theBondPara.SetBondScrubRcd(SECOND_BOND, m_i2ndBondScrubEnable, &stScrubRcd);

    UpdateData(true);

    theBondPara.GetBondScrubRcd(SECOND_BOND, m_i2ndBondScrubEnable, &stScrubRcd); //@4
    m_nScrubPosOffset1    = stScrubRcd.nScrubPosOffset1;
    m_nScrubPosOffset2    = stScrubRcd.nScrubPosOffset2;
    m_n2ndScrubCycle      = stScrubRcd.nScrubCycle;
    m_n2ndScrubAmplitude  = stScrubRcd.nScrubAmplitude;
    m_i2ndScrubDirection  = stScrubRcd.chScrubDirection;
    m_n2ndScrubSpeed      = stScrubRcd.nScrubSpeed;
    m_n2ndScrubForce      = stScrubRcd.nScrubForce;
    m_n2ndScrubPower      = round(stScrubRcd.fScrubPower * 2.55);

    UpdateData(FALSE);
}

void CScrubParaPage::On1stScrub()
{
    SCRUB_PARA_RCD stScrubRcd;

    theBondPara.GetBondScrubRcd(FIRST_BOND, m_i1stBondScrubEnable, &stScrubRcd); //@4
    stScrubRcd.nScrubCycle      = m_n1stScrubCycle;
    stScrubRcd.nScrubAmplitude  = m_n1stScrubAmplitude;
    stScrubRcd.chScrubDirection = m_i1stScrubDirection;
    stScrubRcd.nScrubSpeed      = m_n1stScrubSpeed;
    stScrubRcd.nScrubForce      = m_n1stScrubForce;
    stScrubRcd.fScrubPower      = (float)m_n1stScrubPower / (float)2.55;
    theBondPara.SetBondScrubRcd(FIRST_BOND, m_i1stBondScrubEnable, &stScrubRcd);

	UpdateData(true);

    theBondPara.GetBondScrubRcd(FIRST_BOND, m_i1stBondScrubEnable, &stScrubRcd);
    m_n1stScrubCycle      = stScrubRcd.nScrubCycle;
    m_n1stScrubAmplitude  = stScrubRcd.nScrubAmplitude;
    m_i1stScrubDirection  = stScrubRcd.chScrubDirection;
    m_n1stScrubSpeed      = stScrubRcd.nScrubSpeed;
    m_n1stScrubForce      = stScrubRcd.nScrubForce;
    m_n1stScrubPower      = round(stScrubRcd.fScrubPower * 2.55);

    UpdateData(FALSE);
}

void CScrubParaPage::OnFocus1stScrubCycle()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();

    theKeyPad.GetUserKeyInData(&m_n1stScrubCycle, 1, 0, 5); //@2

    UpdateData(false);
}

void CScrubParaPage::OnFocus1stScrubAmp()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();

    theKeyPad.GetUserKeyInData(&m_n1stScrubAmplitude, 2, 0, 20);

    UpdateData(false);
}

void CScrubParaPage::On1stScrubDir()
{
	UpdateData(true);
}

void CScrubParaPage::OnFocus1stScrubSpeed()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();

    theKeyPad.GetUserKeyInData(&m_n1stScrubSpeed, 10, 5, 200);

    UpdateData(false);
}

void CScrubParaPage::OnFocus1stScrubForce()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_n1stScrubForce, 20, 0, 200);

    UpdateData(false);
}

void CScrubParaPage::OnFocus1stScrubPower()
{
    GetDlgItem(IDC_STATIC_BALL)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_n1stScrubPower, 10, 0, 200);
    UpdateData(false);
}
