// BallScrubPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BallPara.h"
#include "BallScrubPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"

// CBallScrubPage dialog

IMPLEMENT_DYNAMIC(CBallScrubPage, CPropertyPage)

CBallScrubPage::CBallScrubPage()
	: CPropertyPage(CBallScrubPage::IDD)
	, m_iBallScrubEnable(0)
	, m_nBallScrubCycle(0)
	, m_nBallScrubAmplitude(0)
	, m_iBallScrubDirection(0)
	, m_nBallScrubSpeed(0)
	, m_nBallScrubForce(0)
	, m_nBallScrubPower(0)
{

}

CBallScrubPage::~CBallScrubPage()
{
}

void CBallScrubPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_BALL_SCRUB, m_iBallScrubEnable);
	DDX_Text(pDX, IDC_EDIT_BALL_SCRUB_CYCLE, m_nBallScrubCycle);
	DDX_Text(pDX, IDC_EDIT_BALL_SCRUB_AMP, m_nBallScrubAmplitude);
	DDX_CBIndex(pDX, IDC_COMBO_BALL_SCRUB_DIR, m_iBallScrubDirection);
	DDX_Text(pDX, IDC_EDIT_BALL_SCRUB_SPEED, m_nBallScrubSpeed);
	DDX_Text(pDX, IDC_EDIT_BALL_SCRUB_FORCE, m_nBallScrubForce);
	DDX_Text(pDX, IDC_EDIT_BALL_SCRUB_POWER, m_nBallScrubPower);
}


BEGIN_MESSAGE_MAP(CBallScrubPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_BALL_SCRUB, &CBallScrubPage::OnCbnSelchangeComboBallScrub)
	ON_CBN_SELCHANGE(IDC_COMBO_BALL_SCRUB_DIR, &CBallScrubPage::OnCbnSelchangeComboBallScrubDir)
	ON_EN_SETFOCUS(IDC_EDIT_BALL_SCRUB_CYCLE, &CBallScrubPage::OnEnSetfocusEditBallScrubCycle)
	ON_EN_SETFOCUS(IDC_EDIT_BALL_SCRUB_AMP, &CBallScrubPage::OnEnSetfocusEditBallScrubAmp)
	ON_EN_SETFOCUS(IDC_EDIT_BALL_SCRUB_SPEED, &CBallScrubPage::OnEnSetfocusEditBallScrubSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_BALL_SCRUB_FORCE, &CBallScrubPage::OnEnSetfocusEditBallScrubForce)
	ON_EN_SETFOCUS(IDC_EDIT_BALL_SCRUB_POWER, &CBallScrubPage::OnEnSetfocusEditBallScrubPower)
END_MESSAGE_MAP()


// CBallScrubPage message handlers

HBRUSH CBallScrubPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CBallScrubPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetWindowPos(NULL, 1000, 1000, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	pWnd = GetDlgItem(IDC_STATIC_BALL_SCRUB);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PARAMETERS));

	int iRowTop = 50;
	
	pWnd = GetDlgItem(IDC_STATIC_BALL_SCRUB_ENABLE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BALL_SCRUB));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BALL_SCRUB);
    pComboBox->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pComboBox->ResetContent();
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_DISABLE));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_ENABLE));

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_CYCLE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CYCLE));

    pWnd = GetDlgItem(IDC_EDIT_BALL_SCRUB_CYCLE);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	
	pWnd = GetDlgItem(IDC_STATIC_AMPLITUDE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_AMPLITUDE));

    pWnd = GetDlgItem(IDC_EDIT_BALL_SCRUB_AMP);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	
	pWnd = GetDlgItem(IDC_STATIC_DIRECTION);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DIRECTION));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BALL_SCRUB_DIR);
    pComboBox->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pComboBox->ResetContent();
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_VERTICAL));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_HORIZONTAL));
	pComboBox->InsertString(2, theMsg.GetMsg(MSG_CIRCLE));

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_SPEED);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SPEED_PERCENT));

    pWnd = GetDlgItem(IDC_EDIT_BALL_SCRUB_SPEED);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	
	pWnd = GetDlgItem(IDC_STATIC_BALL_SCRUB_FORCE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FORCE_GM));

    pWnd = GetDlgItem(IDC_EDIT_BALL_SCRUB_FORCE);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	
	pWnd = GetDlgItem(IDC_STATIC_BALL_SCRUB_POWER);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_POWER_DAC));

    pWnd = GetDlgItem(IDC_EDIT_BALL_SCRUB_POWER);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	
	ST_BUMP_BALL stBumpBall;
	theBallPara.GetBsobBall(&stBumpBall);
	m_iBallScrubEnable    = stBumpBall.b1stBondScrubEnable;
	m_nBallScrubCycle     = stBumpBall.n1stScrubCycle;
	m_nBallScrubAmplitude = stBumpBall.n1stScrubAmplitude;
	m_iBallScrubDirection = stBumpBall.ch1stScrubDirection;
	m_nBallScrubSpeed     = stBumpBall.n1stScrubSpeed;
	m_nBallScrubForce     = stBumpBall.n1stScrubForce;
	m_nBallScrubPower     = round(stBumpBall.f1stScrubPower * 2.55);

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE un


}

void CBallScrubPage::OnCbnSelchangeComboBallScrub()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CBallScrubPage::OnCbnSelchangeComboBallScrubDir()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CBallScrubPage::OnEnSetfocusEditBallScrubCycle()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBallScrubCycle,1,0,5);

	UpdateData(false);
}

void CBallScrubPage::OnEnSetfocusEditBallScrubAmp()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBallScrubAmplitude,2,0,50);

	UpdateData(false);
}

void CBallScrubPage::OnEnSetfocusEditBallScrubSpeed()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBallScrubSpeed,10,5,350);

	UpdateData(false);
}

void CBallScrubPage::OnEnSetfocusEditBallScrubForce()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBallScrubForce,50,0,200);

	UpdateData(false);
}

void CBallScrubPage::OnEnSetfocusEditBallScrubPower()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nBallScrubPower, 10);

	UpdateData(false);
}

void CBallScrubPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	ST_BUMP_BALL stBumpBall;
	theBallPara.GetBsobBall(&stBumpBall);
	if (m_iBallScrubEnable == TRUE)
		stBumpBall.b1stBondScrubEnable = true;
	else
		stBumpBall.b1stBondScrubEnable = false;
	stBumpBall.n1stScrubCycle      = m_nBallScrubCycle;
	stBumpBall.n1stScrubAmplitude  = m_nBallScrubAmplitude;
	stBumpBall.ch1stScrubDirection  = m_iBallScrubDirection;
	stBumpBall.n1stScrubSpeed = m_nBallScrubSpeed;
	stBumpBall.n1stScrubForce = m_nBallScrubForce;
	stBumpBall.f1stScrubPower = (float)m_nBallScrubPower / (float)2.55;
	theBallPara.SetBsobBall(&stBumpBall);

	CPropertyPage::OnOK();
}
