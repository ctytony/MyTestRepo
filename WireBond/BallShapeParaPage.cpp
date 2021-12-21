/***************************************************************
BallShapeParaPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BallPara.h"
#include "BallParaSheet.h"
#include "BallShapeParaPage.h"

// CBallShapeParaPage dialog
IMPLEMENT_DYNAMIC(CBallShapeParaPage, CPropertyPage)

CBallShapeParaPage::CBallShapeParaPage()
	: CPropertyPage(CBallShapeParaPage::IDD)
    , m_iBallType(1)
    , m_nBaseHt(0)
    , m_nSpanLen(0)
    , m_nMotionSpeed(50)
    , m_nScrubDist(0)
    , m_nBallThickness(0)
    , m_nDirAngle(0)
    , m_iBumpShapeMode(0)
    , m_nFlexLen(0)
    , m_nFlexAngle(0)
    , m_nFlexHt(0)
    , m_nCutPower(0)
    , m_nCutPowerDelay(0)
{
}

CBallShapeParaPage::~CBallShapeParaPage()
{
}

void CBallShapeParaPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_BASE_HEIGHT, m_nBaseHt);
    DDX_Text(pDX, IDC_EDIT_SPAN_LENGTH, m_nSpanLen);
    DDX_Text(pDX, IDC_EDIT_MOTION_SPEED, m_nMotionSpeed);
    DDX_Text(pDX, IDC_EDIT_SCRUB_DIST, m_nScrubDist);
    DDX_Text(pDX, IDC_EDIT_BALL_THICKNESS, m_nBallThickness);
    DDX_Text(pDX, IDC_EDIT_DIRECTION_ANGLE, m_nDirAngle);
    DDX_CBIndex(pDX, IDC_COMBO_BUMP_SHAPE_MODE, m_iBumpShapeMode);
    DDX_Text(pDX, IDC_EDIT_FLEX_LENGTH, m_nFlexLen);
    DDX_Text(pDX, IDC_EDIT_FLEX_ANGLE, m_nFlexAngle);
    DDX_Text(pDX, IDC_EDIT_FLEX_HT, m_nFlexHt);
    DDX_Text(pDX, IDC_EDIT_CUTTING_PWR, m_nCutPower);
    DDX_Text(pDX, IDC_EDIT_CUTTING_PWR_DELAY, m_nCutPowerDelay);
}

BEGIN_MESSAGE_MAP(CBallShapeParaPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_BASE_HEIGHT, OnFocusBaseHeight)
    ON_EN_SETFOCUS(IDC_EDIT_SPAN_LENGTH, OnFocusSpanLength)
    ON_EN_SETFOCUS(IDC_EDIT_MOTION_SPEED, OnFocusMotionSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_SCRUB_DIST, OnFocusScrubDist)
    ON_EN_SETFOCUS(IDC_EDIT_BALL_THICKNESS, OnFocusBallThickness)
    ON_EN_SETFOCUS(IDC_EDIT_DIRECTION_ANGLE, OnFocusDirectionAngle)
    ON_CBN_SELCHANGE(IDC_COMBO_BUMP_SHAPE_MODE, OnBumpShapeMode)
    ON_EN_SETFOCUS(IDC_EDIT_FLEX_LENGTH, OnFocusFlexLength)
    ON_EN_SETFOCUS(IDC_EDIT_FLEX_ANGLE, OnFocusFlexAngle)
    ON_EN_SETFOCUS(IDC_EDIT_FLEX_HT, OnFocusFlexHt)
    ON_EN_SETFOCUS(IDC_EDIT_CUTTING_PWR, OnFocusCuttingPwr)
    ON_EN_SETFOCUS(IDC_EDIT_CUTTING_PWR_DELAY, OnFocusCuttingPwrDelay)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CBallShapeParaPage message handlers
BOOL CBallShapeParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
	m_iBallType = ((CBallParaSheet*)GetParent())->GetBumpBallType();

	int iRowTop = PARA_COLUMN1_TOP;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_BASE_HEIGHT, theMsg.GetMsg(MSG_BBOS_BASE_HT));
	else
		SetDlgItemTextW(IDC_STATIC_BASE_HEIGHT, theMsg.GetMsg(MSG_BASE_HT));
	GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_BASE_HEIGHT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_SPAN_LENGTH,theMsg.GetMsg(MSG_BBOS_SPAN_LEN));
	else
		SetDlgItemTextW(IDC_STATIC_SPAN_LENGTH,theMsg.GetMsg(MSG_SPAN_LEN));
	GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SPAN_LENGTH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_BALL_THICKNESS, theMsg.GetMsg(MSG_BBOS_BALL_THICKNESS));
	else
		SetDlgItemTextW(IDC_STATIC_BALL_THICKNESS, theMsg.GetMsg(MSG_BALL_THICKNESS));
	GetDlgItem(IDC_STATIC_BALL_THICKNESS)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_BALL_THICKNESS)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_SCRUB_DIST, theMsg.GetMsg(MSG_BBOS_SCRUB_DIST));
	else
		SetDlgItemTextW(IDC_STATIC_SCRUB_DIST, theMsg.GetMsg(MSG_SCRUB_DIST));
	GetDlgItem(IDC_STATIC_SCRUB_DIST)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_SCRUB_DIST)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_CUTTING_PWR, theMsg.GetMsg(MSG_BBOS_CUT_POWER));
	else
		SetDlgItemTextW(IDC_STATIC_CUTTING_PWR, theMsg.GetMsg(MSG_CUTTING_POWER));
	GetDlgItem(IDC_STATIC_CUTTING_PWR)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_CUTTING_PWR)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_CUTTING_PWR_DELAY, theMsg.GetMsg(MSG_BBOS_CUT_POWER_DELAY));
	else
		SetDlgItemTextW(IDC_STATIC_CUTTING_PWR_DELAY, theMsg.GetMsg(MSG_CUTTING_POWER_DELAY));
	GetDlgItem(IDC_STATIC_CUTTING_PWR_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_CUTTING_PWR_DELAY)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_MOTION_SPEED, theMsg.GetMsg(MSG_BBOS_MOTION_SPEED));
	else
		SetDlgItemTextW(IDC_STATIC_MOTION_SPEED, theMsg.GetMsg(MSG_MOTION_SPEED));
	GetDlgItem(IDC_STATIC_MOTION_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_MOTION_SPEED)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_DIRECTION_ANGLE, theMsg.GetMsg(MSG_BBOS_DIRECTION_ANGLE));
	else
		SetDlgItemTextW(IDC_STATIC_DIRECTION_ANGLE, theMsg.GetMsg(MSG_DIRECTION_ANGLE));
	GetDlgItem(IDC_STATIC_DIRECTION_ANGLE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_DIRECTION_ANGLE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	if (m_iBallType == 0)
		SetDlgItemTextW(IDC_STATIC_BUMP_SHAPE_MODE, theMsg.GetMsg(MSG_BBOS_BUMP_SHAPE_MODE));
	else
		SetDlgItemTextW(IDC_STATIC_BUMP_SHAPE_MODE, theMsg.GetMsg(MSG_BUMP_SHAPE_MODE));
	GetDlgItem(IDC_STATIC_BUMP_SHAPE_MODE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BUMP_SHAPE_MODE);
	pComboBox->AddString(theMsg.GetMsg(MSG_STANDARD));
	pComboBox->AddString(theMsg.GetMsg(MSG_FLEX));
	pComboBox->AddString(theMsg.GetMsg(MSG_FAST));
	pComboBox->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	SetDlgItemTextW(IDC_STATIC_FLEX_LENGTH, theMsg.GetMsg(MSG_REV_MOTION));
	GetDlgItem(IDC_STATIC_FLEX_LENGTH)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FLEX_LENGTH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	SetDlgItemTextW(IDC_STATIC_FLEX_ANGLE, theMsg.GetMsg(MSG_REV_ANGLE));
	GetDlgItem(IDC_STATIC_FLEX_ANGLE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FLEX_ANGLE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	SetDlgItemTextW(IDC_STATIC_FLEX_HEIGHT,theMsg.GetMsg(MSG_LOOP_HT));
	GetDlgItem(IDC_STATIC_FLEX_HEIGHT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FLEX_HEIGHT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	UpdateMenuVariable();
	return CPropertyPage::OnInitDialog();
}

HBRUSH CBallShapeParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBallShapeParaPage::OnFocusBaseHeight()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBaseHt, 40, 10, 400, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusSpanLength()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSpanLen, 20, -600, 600, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusMotionSpeed()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nMotionSpeed, 50, 10, 100, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusScrubDist()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nScrubDist, 10, -100, 100, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusBallThickness()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBallThickness, 20, 10, 300, true);
    UpdateData(false);
}

void CBallShapeParaPage::UpdateMenuVariable(void)
{
	ST_BUMP_SHAPE stBumpShape;
	if (m_iBallType == 0)
		theBallPara.GetBbosShape(&stBumpShape);
	else
		theBallPara.GetBsobShape(&stBumpShape);
	m_nBaseHt        = stBumpShape.nBaseHeight;
	m_nSpanLen       = stBumpShape.nSpanDist;
	m_nMotionSpeed   = stBumpShape.nMotionSpeed;
	m_nScrubDist     = stBumpShape.nScrubDist;
	m_nBallThickness = stBumpShape.nBallThickness;
	m_nDirAngle      = stBumpShape.nDirectionAngle;
	m_iBumpShapeMode = stBumpShape.chShapeMode;
	m_nFlexLen       = stBumpShape.nFlexLength;
	m_nFlexAngle     = stBumpShape.nFlexAngle;
	m_nFlexHt        = stBumpShape.nFlexHeight;
	m_nCutPower      = stBumpShape.nCuttingPower;
	m_nCutPowerDelay = stBumpShape.nCuttingPowerDelay;

	BYTE bShow = (m_iBumpShapeMode == 1);
	GetDlgItem(IDC_STATIC_FLEX_LENGTH)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_FLEX_LENGTH)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_FLEX_ANGLE)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_FLEX_ANGLE)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_FLEX_HEIGHT)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_FLEX_HT)->ShowWindow(bShow);
	UpdateData(false);
}

void CBallShapeParaPage::OnDestroy(void)
{
	UpdateData();

	ST_BUMP_SHAPE stBumpShape;
	if (m_iBallType == 0)
		theBallPara.GetBbosShape(&stBumpShape);
	else
		theBallPara.GetBsobShape(&stBumpShape);
	stBumpShape.nBaseHeight        = m_nBaseHt;
	stBumpShape.nSpanDist          = m_nSpanLen;
	stBumpShape.nMotionSpeed       = m_nMotionSpeed;
	stBumpShape.nScrubDist         = m_nScrubDist;
	stBumpShape.nBallThickness     = m_nBallThickness;
	stBumpShape.nDirectionAngle    = m_nDirAngle;
	stBumpShape.chShapeMode        = m_iBumpShapeMode;
	stBumpShape.nFlexLength        = m_nFlexLen;
	stBumpShape.nFlexAngle         = m_nFlexAngle;
	stBumpShape.nFlexHeight        = m_nFlexHt;
	stBumpShape.nCuttingPower      = m_nCutPower;
	stBumpShape.nCuttingPowerDelay = m_nCutPowerDelay;
	if (m_iBallType == 0)
		theBallPara.SetBbosShape(&stBumpShape);
	else
		theBallPara.SetBsobShape(&stBumpShape);

	CPropertyPage::OnDestroy();
}

void CBallShapeParaPage::OnFocusDirectionAngle()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nDirAngle, 0, -135, 135, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnBumpShapeMode()
{
	UpdateData();
	BYTE bShow = (m_iBumpShapeMode == 1);
	GetDlgItem(IDC_STATIC_FLEX_LENGTH)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_FLEX_LENGTH)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_FLEX_ANGLE)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_FLEX_ANGLE)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_FLEX_HEIGHT)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_FLEX_HT)->ShowWindow(bShow);
}

void CBallShapeParaPage::OnFocusFlexLength()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nFlexLen, 10, 10, 500, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusFlexAngle()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nFlexAngle, 0, -50, 50, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusFlexHt()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nFlexHt, 30, 10, 2000, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusCuttingPwr()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCutPower, 10, 0, 250, true);
    UpdateData(false);
}

void CBallShapeParaPage::OnFocusCuttingPwrDelay()
{
    GetDlgItem(IDC_STATIC_BASE_HEIGHT)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCutPowerDelay, 0, 0, 100, true);
    UpdateData(false);
}
