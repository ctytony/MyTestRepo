// Loop3rdKinkPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
#include "Loop3rdKinkPage.h"
#include "User.h"

// CLoop3rdKinkPage dialog
IMPLEMENT_DYNAMIC(CLoop3rdKinkPage, CPropertyPage)

CLoop3rdKinkPage::CLoop3rdKinkPage()
	: CPropertyPage(CLoop3rdKinkPage::IDD)
	, m_nSpanLen(0)
	, m_nSpanLenUnit(0)
	, m_nHtFactor(0)
	, m_nSmooth(0)
	, m_nWireFeedSpeed(0)
	, m_nWireBentSpeed(0)
	//v3.1.T407 add
	, m_nHtFactor2(0)
	, m_nArcFactor(100)
	, m_nArcDelay(0)
	, m_nHeight(0)
	, m_nSpanLen2(0)
{
}

CLoop3rdKinkPage::~CLoop3rdKinkPage()
{
}

void CLoop3rdKinkPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPAN2_LENGTH, m_nSpanLen);
	DDX_CBIndex(pDX, IDC_COMBO_SPAN2_LENGTH_UNIT, m_nSpanLenUnit);
	DDX_Text(pDX, IDC_EDIT_KINK3_HT_FACTOR, m_nHtFactor);
	DDX_Text(pDX, IDC_EDIT_KINK3_SMOOTH, m_nSmooth);
	DDX_Text(pDX, IDC_EDIT_KINK3_WIRE_FEED_SPEED, m_nWireFeedSpeed);
	DDX_Text(pDX, IDC_EDIT_KINK3_WIRE_BENT_SPEED, m_nWireBentSpeed);
	//v3.1.T407 add
	DDX_Text(pDX, IDC_EDIT_KINK3_HTFACTOR2, m_nHtFactor2);
	DDX_Text(pDX, IDC_EDIT_KINK3_ARC_FACTOR, m_nArcFactor);
	DDX_Text(pDX, IDC_EDIT_KINK3_ARC_DELAY, m_nArcDelay);
	DDX_Text(pDX, IDC_EDIT_KINK3_HT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_KINK3_SPANLEN2, m_nSpanLen2);
}

BEGIN_MESSAGE_MAP(CLoop3rdKinkPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_SPAN2_LENGTH, OnFocusSpan2Length)
	ON_CBN_SELCHANGE(IDC_COMBO_SPAN2_LENGTH_UNIT, OnSpan2LengthUnit)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_HT_FACTOR, OnFocusKink3HtFactor)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_SMOOTH, OnFocusKink3Smooth)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_WIRE_FEED_SPEED, OnFocusKink3WireFeedSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_WIRE_BENT_SPEED, OnFocusKink3WireBentSpeed)
	//v3.1.T407 add
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_HTFACTOR2, OnFocusKink3HtFactor2)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_ARC_FACTOR, OnFocusKink3ArcFactor)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_ARC_DELAY, OnFocusKink3ArcDelay)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_HT, OnFocusKink3Height)
	ON_EN_SETFOCUS(IDC_EDIT_KINK3_SPANLEN2, OnFocusKink3SpanLen2)

	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLoop3rdKinkPage message handlers
HBRUSH CLoop3rdKinkPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CLoop3rdKinkPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
    return CPropertyPage::OnInitDialog();
}

// Update loop menu variable
void CLoop3rdKinkPage::UpdateMenuVariable(void)
{
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
	m_nSpanLen       = stLoopPara.stLoopBase.nKink3SpanLen;
	m_nSpanLenUnit   = stLoopPara.stLoopBase.cKink3SpanLenUnit;
	m_nHtFactor      = stLoopPara.stLoopBase.nKink3HtFactor;
	m_nSmooth        = stLoopPara.stLoopBase.nKink3Smooth;
	//m_nKink3WireFeedSpeed = stLoopPara.stLoopAdv.nKink4WireFeedSpeed;	//v3.1.T157 delete
	m_nWireFeedSpeed = stLoopPara.stLoopAdv.nWireFeedSpeed3;	//v3.1.T157 add
	m_nWireBentSpeed = stLoopPara.stLoopAdv.nWireBentSpeed3;
	//v3.1.T407 add
	m_nHtFactor2     = stLoopPara.stLoopBase.nKink3HtFactor2;
	m_nArcFactor     = stLoopPara.stLoopBase.nKink3ArcFactor;
	m_nArcDelay      = stLoopPara.stLoopBase.nKink3ArcDelay;
	m_nHeight        = stLoopPara.stLoopBase.nKink3Ht;
	m_nSpanLen2      = stLoopPara.stLoopBase.nKink3SpanLen2;

	int iRowTop = PARA_COLUMN1_TOP;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_SPAN_LEN));
	GetDlgItem(IDC_EDIT_SPAN2_LENGTH)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	pWnd = GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN_UNIT);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_SPAN_LEN_UNIT));
	GetDlgItem(IDC_COMBO_SPAN2_LENGTH_UNIT)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	pWnd = GetDlgItem(IDC_STATIC_KINK3_HT_FACTOR);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_HT_FACTOR));
	GetDlgItem(IDC_EDIT_KINK3_HT_FACTOR)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	//iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_KINK3_SMOOTH);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_SMOOTH));
	pWnd->ShowWindow(SW_HIDE);

	pWnd = GetDlgItem(IDC_EDIT_KINK3_SMOOTH);
	pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->ShowWindow(SW_HIDE);

	if (theUser.GetUserLevel() > USER_SERVICE)
	{
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK3_WIRE_FEED_SPEED);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_WIRE_FEED_SPEED));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK3_WIRE_FEED_SPEED);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK3_WIRE_BENT_SPEED);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_WIRE_BENT_SPEED));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK3_WIRE_BENT_SPEED);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK3_HT);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_HT));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK3_HT);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK3_SPANLEN2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_SPANLEN2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK3_SPANLEN2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK3_HTFACTOR2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_HTFACTOR2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK3_HTFACTOR2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK3_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_ARC_FACTOR));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK3_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK3_ARC_DELAY);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK3_ARC_DELAY));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK3_ARC_DELAY);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		GetDlgItem(IDC_STATIC_KINK3_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK3_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KINK3_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK3_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
		//v3.1.T407 add
		GetDlgItem(IDC_STATIC_KINK3_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK3_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KINK3_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK3_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KINK3_ARC_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK3_ARC_DELAY)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);
}

void CLoop3rdKinkPage::SaveParameter()	//V1.1.W246 add
{
	UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
	stLoopPara.stLoopBase.nKink3SpanLen     = m_nSpanLen;
	stLoopPara.stLoopBase.cKink3SpanLenUnit = m_nSpanLenUnit;
	stLoopPara.stLoopBase.nKink3HtFactor    = m_nHtFactor;
	stLoopPara.stLoopBase.nKink3Smooth      = m_nSmooth;
	//stLoopPara.stLoopAdv.nKink4WireFeedSpeed = m_nKink3WireFeedSpeed;	//v3.1.T157 delete
	stLoopPara.stLoopAdv.nWireFeedSpeed3    = m_nWireFeedSpeed;	//v3.1.T157 add
	stLoopPara.stLoopAdv.nWireBentSpeed3    = m_nWireBentSpeed;
	//v3.1.T407 add
	stLoopPara.stLoopBase.nKink3HtFactor2   = m_nHtFactor2;
	stLoopPara.stLoopBase.nKink3ArcFactor   = m_nArcFactor;
	stLoopPara.stLoopBase.nKink3ArcDelay    = m_nArcDelay;
	stLoopPara.stLoopBase.nKink3Ht          = m_nHeight;
	stLoopPara.stLoopBase.nKink3SpanLen2    = m_nSpanLen2;
	theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);

	(GetParent()->GetParent())->Invalidate();
}

void CLoop3rdKinkPage::OnDestroy()
{
	SaveParameter();
    CPropertyPage::OnDestroy();
}

void CLoop3rdKinkPage::OnFocusSpan2Length()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSpanLen);
    UpdateData(false);
    SaveParameter();
}

void CLoop3rdKinkPage::OnSpan2LengthUnit()
{
    UpdateData(TRUE);
    if (m_nSpanLenUnit == 0)
        m_nSpanLen = 30;
    else if (m_nSpanLenUnit == 1)
        m_nSpanLen = 200;
    UpdateData(FALSE);
    SaveParameter();
}

void CLoop3rdKinkPage::OnFocusKink3HtFactor()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nHtFactor);
    UpdateData(false);
    SaveParameter();
}

void CLoop3rdKinkPage::OnFocusKink3Smooth()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSmooth);
    UpdateData(false);
    SaveParameter();
}

BOOL CLoop3rdKinkPage::OnSetActive()
{
    UpdateMenuVariable();
    return CPropertyPage::OnSetActive();
}

void CLoop3rdKinkPage::OnFocusKink3WireFeedSpeed()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWireFeedSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

void CLoop3rdKinkPage::OnFocusKink3WireBentSpeed()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWireBentSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

//v3.1.T407 add
void CLoop3rdKinkPage::OnFocusKink3HtFactor2()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nHtFactor2, 0, -200, 200);
    UpdateData(false);
    SaveParameter();
}

//v3.1.T407 add
void CLoop3rdKinkPage::OnFocusKink3ArcFactor()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nArcFactor, 100, 20, 180);
    UpdateData(false);
    SaveParameter();
}

//v3.1.T407 add
void CLoop3rdKinkPage::OnFocusKink3ArcDelay()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nArcDelay, 0, 0, 50);
    UpdateData(false);
    SaveParameter();
}

//v3.1.T407 add
void CLoop3rdKinkPage::OnFocusKink3Height()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nHeight);
    UpdateData(false);
    SaveParameter();
}

//v3.1.T407 add
void CLoop3rdKinkPage::OnFocusKink3SpanLen2()
{
    GetDlgItem(IDC_STATIC_KINK3_SPAN_LEN)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSpanLen2);
    UpdateData(false);
    SaveParameter();
}
