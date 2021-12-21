// CapillarySmoothPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Bond.h"
#include "CapillarySmoothPage.h"
#include "servo.h"

// CCapillarySmoothPage dialog
IMPLEMENT_DYNAMIC(CCapillarySmoothPage, CPropertyPage)

CCapillarySmoothPage::CCapillarySmoothPage()
	: CPropertyPage(CCapillarySmoothPage::IDD)
    , m_nSmoothForce(50)
    , m_fSmoothPwr(20.0)
    , m_nSmoothAmplitude(10)
    , m_nSmoothCycle(20)
    , m_iSrchSpeed(0)
    , m_nSrchThreshold(36)
{
}

CCapillarySmoothPage::~CCapillarySmoothPage()
{
}

void CCapillarySmoothPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FORCE, m_nSmoothForce);
    DDX_Text(pDX, IDC_EDIT_POWER, m_fSmoothPwr);
    DDX_Text(pDX, IDC_EDIT_AMPLITUDE, m_nSmoothAmplitude);
    DDX_Text(pDX, IDC_EDIT_CYCLE, m_nSmoothCycle);
    DDX_CBIndex(pDX, IDC_COMBO_SRCH_SPEED, m_iSrchSpeed);
    DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_nSrchThreshold);
}

BEGIN_MESSAGE_MAP(CCapillarySmoothPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_FORCE, OnFocusForce)
    ON_EN_SETFOCUS(IDC_EDIT_POWER, OnFocusPower)
    ON_EN_SETFOCUS(IDC_EDIT_AMPLITUDE, OnFocusAmplitude)
    ON_EN_SETFOCUS(IDC_EDIT_CYCLE, OnFocusCycle)
    ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
    ON_CBN_SELCHANGE(IDC_COMBO_SRCH_SPEED, OnSrchSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_THRESHOLD, OnFocusThreshold)
    ON_BN_CLICKED(IDC_BUTTON_START_CLEAN, OnStartClean)
    ON_BN_CLICKED(IDC_BUTTON_CLEAN_NEXT, OnCleanNext)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCapillarySmoothPage message handlers
BOOL CCapillarySmoothPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_STATIC_PARA,theMsg.GetMsg(MSG_PARAMETERS));
    SetDlgItemTextW(IDC_STATIC_SRCH_SPEED,theMsg.GetMsg(MSG_SRCH_SPEED));
    SetDlgItemTextW(IDC_STATIC_SRCH_TOL,theMsg.GetMsg(MSG_SRCH_THRES));
    SetDlgItemTextW(IDC_STATIC_FORCE,theMsg.GetMsg(MSG_FORCE_GM));
    SetDlgItemTextW(IDC_STATIC_CYCLE,theMsg.GetMsg(MSG_CYCLE));
    SetDlgItemTextW(IDC_STATIC_SMOOTH,theMsg.GetMsg(MSG_SMOOTH));
    SetDlgItemTextW(IDC_STATIC_AMPLITUDE,theMsg.GetMsg(MSG_AMPLITUDE));
    SetDlgItemTextW(IDC_STATIC_POWER,theMsg.GetMsg(MSG_POWER_DAC));
    SetDlgItemTextW(IDC_BUTTON_START,theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_NEXT,theMsg.GetMsg(MSG_NEXT));
    SetDlgItemTextW(IDC_STATIC_CLEAN,theMsg.GetMsg(MSG_CLEAN));
    SetDlgItemTextW(IDC_BUTTON_START_CLEAN,theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_CLEAN_NEXT,theMsg.GetMsg(MSG_NEXT));

    const PGM_RECORD *pstPgm = thePgm.GetRcd();
    m_nSmoothForce     = pstPgm->nCapiSmoothForce;
    m_fSmoothPwr       = pstPgm->fCapiSmoothPower;
    m_nSmoothAmplitude = pstPgm->nCapiSmoothAmplitude;
    m_nSmoothCycle     = pstPgm->nCapiSmoothCycle;
    m_iSrchSpeed       = pstPgm->nCapiSmoothSpeed - 3;
    m_nSrchThreshold   = pstPgm->nCapiSmoothThreshold;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CCapillarySmoothPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCapillarySmoothPage::OnFocusForce()
{
    GetDlgItem(IDC_STATIC_PARA)->SetFocus();

    theKeyPad.GetUserKeyInData(&m_nSmoothForce, 50, 10, 300);

    UpdateData(false);
}

void CCapillarySmoothPage::OnFocusPower()
{
    GetDlgItem(IDC_STATIC_PARA)->SetFocus();

    double dSmoothPwr = m_fSmoothPwr;

    theKeyPad.GetUserKeyInData(&dSmoothPwr, 10, 0, 90);

    m_fSmoothPwr = (float)(round(dSmoothPwr * 10.0) / 10.0);

    UpdateData(false);
}

void CCapillarySmoothPage::OnFocusAmplitude()
{
    GetDlgItem(IDC_STATIC_PARA)->SetFocus();

    theKeyPad.GetUserKeyInData(&m_nSmoothAmplitude, 10, 0, 100);

    UpdateData(false);
}

void CCapillarySmoothPage::OnFocusCycle()
{
    GetDlgItem(IDC_STATIC_PARA)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nSmoothCycle, 20, 1, 300);

    UpdateData(false);
}

void CCapillarySmoothPage::OnStart()
{
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
    pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

    GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow();

    theServo.SetServoControlParameter();
    theServo.SetServoSpeedProfile(MOTOR_Z, 0);

    UpdateData(true);
}

void CCapillarySmoothPage::OnNext()
{
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
    
    theBond.PerformCapillarySmooth((m_iSrchSpeed + 3) * 2, m_nSrchThreshold, m_nSmoothForce, m_fSmoothPwr, m_nSmoothAmplitude, m_nSmoothCycle);

    GetDlgItem(IDC_BUTTON_START)->EnableWindow();
}

void CCapillarySmoothPage::OnSrchSpeed()
{
    UpdateData();
}

void CCapillarySmoothPage::OnFocusThreshold()
{
    GetDlgItem(IDC_STATIC_PARA)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nSrchThreshold, 36, 15, 100);

    UpdateData(false);
}

void CCapillarySmoothPage::OnStartClean()
{
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
    pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

    GetDlgItem(IDC_BUTTON_START_CLEAN)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_CLEAN_NEXT)->EnableWindow();

    theServo.SetServoControlParameter();
    theServo.SetServoSpeedProfile(MOTOR_Z, 0);

    UpdateData(true);
}

void CCapillarySmoothPage::OnCleanNext()
{
    GetDlgItem(IDC_BUTTON_CLEAN_NEXT)->EnableWindow(FALSE);

    theBond.PerformCapillaryClean((m_iSrchSpeed + 3) * 2, m_nSrchThreshold, m_nSmoothForce, m_nSmoothCycle);

    GetDlgItem(IDC_BUTTON_START_CLEAN)->EnableWindow();
}

void CCapillarySmoothPage::OnDestroy()
{
    PGM_RECORD *pstPgm = thePgm.GetRcd();
    pstPgm->nCapiSmoothForce     = m_nSmoothForce;
    pstPgm->fCapiSmoothPower     = m_fSmoothPwr;
    pstPgm->nCapiSmoothAmplitude = m_nSmoothAmplitude;
    pstPgm->nCapiSmoothCycle     = m_nSmoothCycle;
    pstPgm->nCapiSmoothSpeed     = m_iSrchSpeed + 3;
    pstPgm->nCapiSmoothThreshold = m_nSrchThreshold;

    CPropertyPage::OnDestroy();
}
