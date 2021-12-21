// Loop2ndKinkPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
#include "Loop2ndKinkPage.h"
#include "User.h"

// CLoop2ndKinkPage dialog
IMPLEMENT_DYNAMIC(CLoop2ndKinkPage, CPropertyPage)

CLoop2ndKinkPage::CLoop2ndKinkPage()
	: CPropertyPage(CLoop2ndKinkPage::IDD)
    , m_nSpanLen(0)
    , m_nSpanLenUnit(0)
    , m_nHtFactor(0)
    , m_nWireFeedSpeed(0)
    , m_nWireBentSpeed(0)
	//v3.1.T407 add
	, m_nHtFactor2(0)
	, m_nArcFactor(100)
	, m_nArcDelay(0)
	, m_nHeight(50)
	, m_nSpanLen2(0)
{
}

CLoop2ndKinkPage::~CLoop2ndKinkPage()
{
}

void CLoop2ndKinkPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPAN_LENGTH, m_nSpanLen);
	DDX_CBIndex(pDX, IDC_COMBO_SPAN_LENGTH_UNIT, m_nSpanLenUnit);
	DDX_Text(pDX, IDC_EDIT_KINK2HT_FACTOR, m_nHtFactor);
	DDX_Text(pDX, IDC_EDIT_KINK2_WIRE_FEED_SPEED, m_nWireFeedSpeed);
	DDX_Text(pDX, IDC_EDIT_KINK2_WIRE_BENT_SPEED, m_nWireBentSpeed);
	//v3.1.T407 add
	DDX_Text(pDX, IDC_EDIT_KINK2_HTFACTOR2, m_nHtFactor2);
	DDX_Text(pDX, IDC_EDIT_KINK2_ARC_FACTOR, m_nArcFactor);
	DDX_Text(pDX, IDC_EDIT_KINK2_ARC_DELAY, m_nArcDelay);
	DDX_Text(pDX, IDC_EDIT_KINK2_HT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_KINK2_SPANLEN2, m_nSpanLen2);
}

BEGIN_MESSAGE_MAP(CLoop2ndKinkPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_SPAN_LENGTH, OnFocusSpanLength)
	ON_CBN_SELCHANGE(IDC_COMBO_SPAN_LENGTH_UNIT, OnSpanLengthUnit)
	ON_EN_SETFOCUS(IDC_EDIT_KINK2HT_FACTOR, OnFocusKink2htFactor)
	ON_EN_SETFOCUS(IDC_EDIT_KINK2_WIRE_FEED_SPEED, OnFocusKink2WireFeedSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_KINK2_WIRE_BENT_SPEED, OnFocusKink2WireBentSpeed)
	//v3.1.T407 add
	ON_EN_SETFOCUS(IDC_EDIT_KINK2_HTFACTOR2, OnFocusKink2HtFactor2)
	ON_EN_SETFOCUS(IDC_EDIT_KINK2_ARC_FACTOR, OnFocusKink2ArcFactor)
	ON_EN_SETFOCUS(IDC_EDIT_KINK2_ARC_DELAY, OnFocusKink2ArcDelay)
	ON_EN_SETFOCUS(IDC_EDIT_KINK2_HT, OnFocusKink2Height)
	ON_EN_SETFOCUS(IDC_EDIT_KINK2_SPANLEN2, OnFocusKink2SpanLen2)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLoop2ndKinkPage message handlers
HBRUSH CLoop2ndKinkPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CLoop2ndKinkPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
    return CPropertyPage::OnInitDialog();
}

// UpdateMenuVariable
void CLoop2ndKinkPage::UpdateMenuVariable(void)
{
	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
	m_nSpanLen       = stLoopPara.stLoopBase.nKink2SpanLen;
	m_nSpanLenUnit   = stLoopPara.stLoopBase.nKink2SpanLenUnit;
	m_nHtFactor      = stLoopPara.stLoopBase.nKink2HtFactor;
	m_nWireFeedSpeed = stLoopPara.stLoopAdv.nWireFeedSpeed2;
	m_nWireBentSpeed = stLoopPara.stLoopAdv.nWireBentSpeed2;
	//v3.1.T407 add
	m_nHtFactor2     = stLoopPara.stLoopBase.nKink2HtFactor2;
	m_nArcFactor     = stLoopPara.stLoopBase.nKink2ArcFactor;
	m_nArcDelay      = stLoopPara.stLoopBase.nKink2ArcDelay;
	m_nHeight        = stLoopPara.stLoopBase.nKink2Ht;
	m_nSpanLen2      = stLoopPara.stLoopBase.nKink2SpanLen2;

	int iRowTop = PARA_COLUMN1_TOP;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SPAN_LENGTH);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_SPAN_LEN));	//v3.1.T145 rename
	GetDlgItem(IDC_EDIT_SPAN_LENGTH)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	pWnd = GetDlgItem(IDC_STATIC_SPAN_LENGTH_UNIT);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SPAN_LENGTH_UNIT));
	GetDlgItem(IDC_COMBO_SPAN_LENGTH_UNIT)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	pWnd = GetDlgItem(IDC_STATIC_KINK2HT_FACTOR);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_HT_FACTOR));
	GetDlgItem(IDC_EDIT_KINK2HT_FACTOR)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	if (theUser.GetUserLevel() > USER_SERVICE)
	{
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK2_WIRE_FEED_SPEED);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_WIRE_FEED_SPEED));
		pWnd->ShowWindow(SW_SHOWNORMAL);
	    
		pWnd = GetDlgItem(IDC_EDIT_KINK2_WIRE_FEED_SPEED);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK2_WIRE_BENT_SPEED);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_WIRE_BENT_SPEED));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK2_WIRE_BENT_SPEED);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK2_HT);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_HT));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK2_HT);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK2_SPANLEN2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_SPANLEN2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK2_SPANLEN2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK2_HTFACTOR2);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_HTFACTOR2));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK2_HTFACTOR2);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK2_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_ARC_FACTOR));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK2_ARC_FACTOR);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);

		//v3.1.T407 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		pWnd = GetDlgItem(IDC_STATIC_KINK2_ARC_DELAY);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_ARC_DELAY));
		pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_EDIT_KINK2_ARC_DELAY);
		pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		GetDlgItem(IDC_STATIC_KINK2_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK2_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KINK2_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK2_WIRE_BENT_SPEED)->ShowWindow(SW_HIDE);
		//v3.1.T407 add
		GetDlgItem(IDC_STATIC_KINK2_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK2_HTFACTOR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KINK2_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK2_ARC_FACTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KINK2_ARC_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK2_ARC_DELAY)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);
}

void CLoop2ndKinkPage::SaveParameter()	//V1.1.W246 add
{
	UpdateData();

	short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
	stLoopPara.stLoopBase.nKink2SpanLen     = m_nSpanLen;
	stLoopPara.stLoopBase.nKink2SpanLenUnit = m_nSpanLenUnit;
	stLoopPara.stLoopBase.nKink2HtFactor    = m_nHtFactor;
	stLoopPara.stLoopAdv.nWireFeedSpeed2    = m_nWireFeedSpeed;
	stLoopPara.stLoopAdv.nWireBentSpeed2    = m_nWireBentSpeed;
	//v3.1.T407 add
	stLoopPara.stLoopBase.nKink2HtFactor2   = m_nHtFactor2;
	stLoopPara.stLoopBase.nKink2ArcFactor   = m_nArcFactor;
	stLoopPara.stLoopBase.nKink2ArcDelay    = m_nArcDelay;
	stLoopPara.stLoopBase.nKink2Ht          = m_nHeight;
	stLoopPara.stLoopBase.nKink2SpanLen2    = m_nSpanLen2;

	theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);

	(GetParent()->GetParent())->Invalidate();
}

void CLoop2ndKinkPage::OnDestroy()
{
	SaveParameter();
    CPropertyPage::OnDestroy();
}

void CLoop2ndKinkPage::OnFocusSpanLength()
{
    GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSpanLen);
    UpdateData(false);
    SaveParameter();
}

void CLoop2ndKinkPage::OnSpanLengthUnit()
{
    UpdateData(TRUE);
    if (m_nSpanLenUnit == 0)
        m_nSpanLen = 30;
    else if (m_nSpanLenUnit == 1)
        m_nSpanLen = 200;
    UpdateData(FALSE);
    SaveParameter();
}

void CLoop2ndKinkPage::OnFocusKink2htFactor()
{
    GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nHtFactor, 50, -100, 100);
    UpdateData(false);
    SaveParameter();
}

BOOL CLoop2ndKinkPage::OnSetActive()
{
    UpdateMenuVariable();
    return CPropertyPage::OnSetActive();
}

void CLoop2ndKinkPage::OnFocusKink2WireFeedSpeed()
{
    GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWireFeedSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

void CLoop2ndKinkPage::OnFocusKink2WireBentSpeed()
{
    GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWireBentSpeed, 100, -100, 100);
    UpdateData(false);
    SaveParameter();
}

//v3.1.T407 add
void CLoop2ndKinkPage::OnFocusKink2HtFactor2()
{
	GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHtFactor2, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop2ndKinkPage::OnFocusKink2ArcFactor()
{
	GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nArcFactor, 100, 20, 180);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop2ndKinkPage::OnFocusKink2ArcDelay()
{
	GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nArcDelay, 0, 0, 50);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop2ndKinkPage::OnFocusKink2Height()
{
	GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHeight);
	UpdateData(false);
	SaveParameter();
}

//v3.1.T407 add
void CLoop2ndKinkPage::OnFocusKink2SpanLen2()
{
	GetDlgItem(IDC_STATIC_SPAN_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSpanLen2);
	UpdateData(false);
	SaveParameter();
}
