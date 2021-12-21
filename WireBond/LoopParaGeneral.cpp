/***************************************************************
LoopParaGeneral.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Bond.h"
#include "LoopParaGeneral.h"
#include "User.h"

// CLoopParaGeneral dialog

IMPLEMENT_DYNAMIC(CLoopParaGeneral, CPropertyPage)

CLoopParaGeneral::CLoopParaGeneral()
	: CPropertyPage(CLoopParaGeneral::IDD)
    , m_nKinkNum(0)
    , m_nLoopHt(150)
    , m_nRevDist(60)
    , m_nRevAngle(0)
    , m_nLoopHtCorrect(0)
    , m_nKink2SpanLen(20)
    , m_nKink2HtFactor(50)
    , m_nLAngle(0)
    , m_nLoopBaseMode(0)
    , m_nLoopGroup(0)
    , m_nKink2SpanLenUnit(0)
    , m_nRevHt(0)
    , m_nPreKinkEnable(0)
    , m_nPreKinkHt(0)
    , m_nPreKinkDist(0)
    , m_nPreKinkAngle(0)
{
}

CLoopParaGeneral::~CLoopParaGeneral()
{
}

void CLoopParaGeneral::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_KINK_NUM, m_nKinkNum);
    DDX_Text(pDX, IDC_EDIT_LOOP_HT, m_nLoopHt);
    DDX_Text(pDX, IDC_EDIT_REV_DIST, m_nRevDist);
    DDX_Text(pDX, IDC_EDIT_REV_ANGLE, m_nRevAngle);
    DDX_Text(pDX, IDC_EDIT_LOOPHT_COR, m_nLoopHtCorrect);
    DDX_Text(pDX, IDC_EDIT_SPAN_LENGTH, m_nKink2SpanLen);
    DDX_Text(pDX, IDC_EDIT_KINK2HT_FACTOR, m_nKink2HtFactor);
    DDX_Text(pDX, IDC_EDIT_L_ANGLE, m_nLAngle);
    DDX_CBIndex(pDX, IDC_COMBO_LOOP_BASE_MODE, m_nLoopBaseMode);
    DDX_CBIndex(pDX, IDC_COMBO_LOOP_GROUP, m_nLoopGroup);
    DDX_CBIndex(pDX, IDC_COMBO_SPAN_LENGTH_UNIT, m_nKink2SpanLenUnit);
    DDX_Text(pDX, IDC_EDIT_NECK_HEIGHT, m_nRevHt);
    DDX_CBIndex(pDX, IDC_COMBO_PRE_KINK_ENABLE, m_nPreKinkEnable);
    DDX_Text(pDX, IDC_EDIT_PRE_KINK_HT, m_nPreKinkHt);
    DDX_Text(pDX, IDC_EDIT_PRE_KINK_DIST, m_nPreKinkDist);
    DDX_Text(pDX, IDC_EDIT_PRE_KINK_ANGLE, m_nPreKinkAngle);
}

BEGIN_MESSAGE_MAP(CLoopParaGeneral, CPropertyPage)
    ON_CBN_SELCHANGE(IDC_COMBO_KINK_NUM, OnKinkNum)
    ON_CBN_SELCHANGE(IDC_COMBO_LOOP_GROUP, OnLoopGroup)
    ON_CBN_SELCHANGE(IDC_COMBO_SPAN_LENGTH_UNIT, OnSpanLengthUnit)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_LOOP_HT, OnFocusLoopHt)
    ON_EN_SETFOCUS(IDC_EDIT_REV_DIST, OnFocusRevDist)
    ON_EN_SETFOCUS(IDC_EDIT_REV_ANGLE, OnFocusRevAngle)
    ON_EN_SETFOCUS(IDC_EDIT_LOOPHT_COR, OnFocusLoophtCor)
    ON_EN_SETFOCUS(IDC_EDIT_SPAN_LENGTH, OnFocusSpanLength)
    ON_EN_SETFOCUS(IDC_EDIT_KINK2HT_FACTOR, OnFocusKink2htFactor)
    ON_EN_SETFOCUS(IDC_EDIT_L_ANGLE, OnFocusLAngle)
    ON_CBN_SELCHANGE(IDC_COMBO_LOOP_BASE_MODE, OnLoopBaseMode)
    ON_EN_SETFOCUS(IDC_EDIT_NECK_HEIGHT, OnFocusNeckHeight)
    ON_CBN_SELCHANGE(IDC_COMBO_PRE_KINK_ENABLE, OnPreKinkEnable)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_HT, OnFocusPreKinkHt)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_DIST, OnFocusPreKinkDist)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_KINK_ANGLE, OnFocusPreKinkAngle)
    ON_WM_VSCROLL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLoopParaGeneral message handlers
BOOL CLoopParaGeneral::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOOP_GROUP);

    WCHAR wcBuffer[8]; 
    for (int i = 0; i < theLoopPara.m_nTotalLoopModeUsed; i++)
    {
        _itow_s(theLoopPara.m_nLoopModeUsed[i], wcBuffer, 8, 10);
        pComBox->AddString(wcBuffer);
    }

    m_nLoopGroup = 0;
	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[m_nLoopGroup], &stLoopPara);
	if ((stLoopPara.stLoopBase.nKinkNum > 0)
		&& (stLoopPara.stLoopBase.nKinkNum < 4))
		m_nKinkNum = stLoopPara.stLoopBase.nKinkNum - 1;
    m_nLoopHt           = stLoopPara.stLoopBase.nLoopHt;
    m_nRevDist          = stLoopPara.stLoopBase.nRevDist;
    m_nRevAngle         = stLoopPara.stLoopBase.nRevAngle;
    m_nLoopHtCorrect    = stLoopPara.stLoopBase.nLoopHtCorrect;
    m_nKink2SpanLen     = stLoopPara.stLoopBase.nKink2SpanLen;
    m_nKink2SpanLenUnit = stLoopPara.stLoopBase.nKink2SpanLenUnit;
    m_nKink2HtFactor    = stLoopPara.stLoopBase.nKink2HtFactor;
    m_nLAngle           = stLoopPara.stLoopBase.nLAngle;
    m_nLoopBaseMode     = stLoopPara.stLoopBase.nLoopBaseMode;
    m_nRevHt            = stLoopPara.stLoopBase.nRevHt;
    m_nPreKinkEnable    = stLoopPara.stLoopBase.bPreKinkEnable;
    m_nPreKinkHt        = stLoopPara.stLoopBase.nPreKinkHt;
    m_nPreKinkDist      = stLoopPara.stLoopBase.nPreKinkDist;
    m_nPreKinkAngle     = stLoopPara.stLoopBase.nPreKinkAngle;

    UpdateLoopParaMenu();
    
    return CPropertyPage::OnInitDialog();
}

void CLoopParaGeneral::UpdateLoopParaMenu(void)
{
    SetWindowTextW(theMsg.GetMsg(MSG_LOOP_GROUP));

    int iRowTop = PARA_COLUMN1_TOP;
    CWnd* pWnd = GetDlgItem(IDC_STATIC_LOOP_GROUP);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LOOP_GROUP));
    GetDlgItem(IDC_COMBO_LOOP_GROUP)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_KINK_NUM);
    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_KINK_NUM);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LOOP_TYPE));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pComboBox->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
        GetDlgItem(IDC_STATIC_KINK_NUM)->ShowWindow(SW_HIDE);
        pComboBox->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    pWnd = GetDlgItem(IDC_STATIC_LOOP_HT);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LOOP_HEIGHT));
    GetDlgItem(IDC_EDIT_LOOP_HT)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    pWnd = GetDlgItem(IDC_STATIC_LOOPHT_COR);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LOOP_HT_CORRECT));
    GetDlgItem(IDC_EDIT_LOOPHT_COR)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    pWnd = GetDlgItem(IDC_STATIC_L_ANGLE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_L_ANGLE));
    pWnd->ShowWindow(SW_SHOWNORMAL);
    pWnd = GetDlgItem(IDC_EDIT_L_ANGLE);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->ShowWindow(SW_SHOWNORMAL);

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOOP_BASE_MODE);
	pComboBox->ResetContent();	//v3.1.T361 add
    pComboBox->InsertString(0, theMsg.GetMsg(MSG_STANDARD));
    pComboBox->InsertString(1, theMsg.GetMsg(MSG_MODE_A));
    pComboBox->InsertString(2, theMsg.GetMsg(MSG_MODE_B));
    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_LOOP_BASE_MODE);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LOOP_BASE_MODE));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pComboBox->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
        GetDlgItem(IDC_STATIC_LOOP_BASE_MODE)->ShowWindow(SW_HIDE);
        pComboBox->ShowWindow(SW_HIDE);
    }
    
    if ((m_nLoopBaseMode == 2) && (theUser.GetUserLevel() > USER_OPERATOR))
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_NECK_HEIGHT);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NECK_HT));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pWnd = GetDlgItem(IDC_EDIT_NECK_HEIGHT);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
        GetDlgItem(IDC_STATIC_NECK_HEIGHT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_NECK_HEIGHT)->ShowWindow(SW_HIDE);
    }
    
    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    pWnd = GetDlgItem(IDC_STATIC_REV_DIST);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_DIST));
    GetDlgItem(IDC_EDIT_REV_DIST)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    pWnd = GetDlgItem(IDC_STATIC_REV_ANGLE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REV_ANGLE));
    GetDlgItem(IDC_EDIT_REV_ANGLE)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    if (theUser.GetUserLevel() > USER_OPERATOR)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pWnd = GetDlgItem(IDC_COMBO_PRE_KINK_ENABLE);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
        GetDlgItem(IDC_STATIC_PRE_KINK)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_PRE_KINK_ENABLE)->ShowWindow(SW_HIDE);
    }

    if ((theUser.GetUserLevel() > USER_OPERATOR) && (m_nPreKinkEnable == TRUE))
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_HT);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_HT));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_HT);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_DIST);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_DIST));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_DIST);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_PRE_KINK_ANGLE);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRE_KINK_ANGLE));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pWnd = GetDlgItem(IDC_EDIT_PRE_KINK_ANGLE);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
        GetDlgItem(IDC_STATIC_PRE_KINK_HT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_HT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_KINK_DIST)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_DIST)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_PRE_KINK_ANGLE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PRE_KINK_ANGLE)->ShowWindow(SW_HIDE);
    }

    if (m_nKinkNum > 0)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_SPAN_LENGTH);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_SPAN_LEN));	//v3.1.T145 rename
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pWnd = GetDlgItem(IDC_EDIT_SPAN_LENGTH);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        if (theUser.GetUserLevel() > USER_OPERATOR)
        {
            iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
            pWnd = GetDlgItem(IDC_STATIC_SPAN_LENGTH_UNIT);
            pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
            pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SPAN_LENGTH_UNIT));
            pWnd->ShowWindow(SW_SHOWNORMAL);
            pWnd = GetDlgItem(IDC_COMBO_SPAN_LENGTH_UNIT);
            pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
            pWnd->ShowWindow(SW_SHOWNORMAL);
        }
        else
        {
            GetDlgItem(IDC_STATIC_SPAN_LENGTH_UNIT)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_COMBO_SPAN_LENGTH_UNIT)->ShowWindow(SW_HIDE);
        }

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
        pWnd = GetDlgItem(IDC_STATIC_KINK2HT_FACTOR);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK2_HT_FACTOR));
        pWnd->ShowWindow(SW_SHOWNORMAL);
        pWnd = GetDlgItem(IDC_EDIT_KINK2HT_FACTOR);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
        GetDlgItem(IDC_STATIC_SPAN_LENGTH)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SPAN_LENGTH)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_SPAN_LENGTH_UNIT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_SPAN_LENGTH_UNIT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_KINK2HT_FACTOR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_KINK2HT_FACTOR)->ShowWindow(SW_HIDE);
    }

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    if (iRowTop > PROPERTY_PAGE_HEIGHT)
    {
        m_nPageSize = iRowTop - PROPERTY_PAGE_HEIGHT;
        EnableScrollBarCtrl(SB_VERT, TRUE);
        SetScrollRange(SB_VERT, 0, m_nPageSize);
    }
    else
        EnableScrollBarCtrl(SB_VERT, FALSE);
}

void CLoopParaGeneral::OnKinkNum()
{
	if (theBond.GetBondMode() != BONDMODE_IDLE) return;

	UpdateData(TRUE);

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetDefaultRcd(&stLoopPara, m_nKinkNum + 1);
	m_nLoopHt           = stLoopPara.stLoopBase.nLoopHt;
	m_nRevDist          = stLoopPara.stLoopBase.nRevDist;
	m_nRevAngle         = stLoopPara.stLoopBase.nRevAngle;
	m_nLoopHtCorrect    = stLoopPara.stLoopBase.nLoopHtCorrect;
	m_nKink2SpanLen     = stLoopPara.stLoopBase.nKink2SpanLen;
	m_nKink2SpanLenUnit = stLoopPara.stLoopBase.nKink2SpanLenUnit;
	m_nKink2HtFactor    = stLoopPara.stLoopBase.nKink2HtFactor;
	m_nLAngle           = stLoopPara.stLoopBase.nLAngle;
	m_nLoopBaseMode     = stLoopPara.stLoopBase.nLoopBaseMode;
	m_nRevHt            = stLoopPara.stLoopBase.nRevHt;

	UpdateLoopParaMenu();
	UpdateData(FALSE);

	theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[m_nLoopGroup], &stLoopPara);
}

void CLoopParaGeneral::OnLoopGroup()
{
	int nOriLoopGroup = m_nLoopGroup;
	UpdateData(TRUE);

	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nOriLoopGroup], &stLoopPara);
	if ((m_nKinkNum > -1) && (m_nKinkNum < 3))
		stLoopPara.stLoopBase.nKinkNum = m_nKinkNum + 1;
	stLoopPara.stLoopBase.nLoopHt           = m_nLoopHt;
	stLoopPara.stLoopBase.nRevDist          = m_nRevDist;
	stLoopPara.stLoopBase.nRevAngle         = m_nRevAngle;
	stLoopPara.stLoopBase.nLoopHtCorrect    = m_nLoopHtCorrect;
	stLoopPara.stLoopBase.nKink2SpanLen     = m_nKink2SpanLen;
	stLoopPara.stLoopBase.nKink2SpanLenUnit = m_nKink2SpanLenUnit;
	stLoopPara.stLoopBase.nKink2HtFactor    = m_nKink2HtFactor;
	stLoopPara.stLoopBase.nLAngle           = m_nLAngle;
	stLoopPara.stLoopBase.nLoopBaseMode     = m_nLoopBaseMode;
	stLoopPara.stLoopBase.nRevHt            = m_nRevHt;
	//v3.1.T361 add
	stLoopPara.stLoopBase.bPreKinkEnable    = (m_nPreKinkEnable != 0);
	stLoopPara.stLoopBase.nPreKinkHt        = m_nPreKinkHt;
	stLoopPara.stLoopBase.nPreKinkDist      = m_nPreKinkDist;
	stLoopPara.stLoopBase.nPreKinkAngle     = m_nPreKinkAngle;
	theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[nOriLoopGroup], &stLoopPara);

	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[m_nLoopGroup], &stLoopPara);
	if ((stLoopPara.stLoopBase.nKinkNum > 0)
		&& (stLoopPara.stLoopBase.nKinkNum < 4))
		m_nKinkNum = stLoopPara.stLoopBase.nKinkNum - 1;
	m_nLoopHt           = stLoopPara.stLoopBase.nLoopHt;
	m_nRevDist          = stLoopPara.stLoopBase.nRevDist;
	m_nRevAngle         = stLoopPara.stLoopBase.nRevAngle;
	m_nLoopHtCorrect    = stLoopPara.stLoopBase.nLoopHtCorrect;
	m_nKink2SpanLen     = stLoopPara.stLoopBase.nKink2SpanLen;
	m_nKink2SpanLenUnit = stLoopPara.stLoopBase.nKink2SpanLenUnit;
	m_nKink2HtFactor    = stLoopPara.stLoopBase.nKink2HtFactor;
	m_nLAngle           = stLoopPara.stLoopBase.nLAngle;
	m_nLoopBaseMode     = stLoopPara.stLoopBase.nLoopBaseMode;
	m_nRevHt            = stLoopPara.stLoopBase.nRevHt;
	//v3.1.T361 add
	m_nPreKinkEnable    = stLoopPara.stLoopBase.bPreKinkEnable;
	m_nPreKinkHt        = stLoopPara.stLoopBase.nPreKinkHt;
	m_nPreKinkDist      = stLoopPara.stLoopBase.nPreKinkDist;
	m_nPreKinkAngle     = stLoopPara.stLoopBase.nPreKinkAngle;

	UpdateLoopParaMenu();
	UpdateData(FALSE);
}

void CLoopParaGeneral::OnSpanLengthUnit()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	UpdateData(TRUE);
	if (m_nKink2SpanLenUnit == 0)
		m_nKink2SpanLen = 30;
	else if (m_nKink2SpanLenUnit == 1)
		m_nKink2SpanLen = 200;
	UpdateData(FALSE);
}

HBRUSH CLoopParaGeneral::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLoopParaGeneral::OnFocusLoopHt()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;
	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nLoopHt);
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnFocusRevDist()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;
	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevDist, 65, -100, 100);
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnFocusRevAngle()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;
	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevAngle, 5, -50, 50);
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnFocusLoophtCor()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;
	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nLoopHtCorrect, 0, -200, 200);
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnFocusSpanLength()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;
	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nKink2SpanLen);
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnFocusKink2htFactor()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;
	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nKink2HtFactor, 70, -100, 100);
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnFocusLAngle()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;
	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nLAngle, 0, -45, 45);	//v3.0.T242 delete
	theKeyPad.GetUserKeyInData(&m_nLAngle, 0, -90, 90);		//v3.0.T242 add
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnLoopBaseMode()
{
	if (theBond.GetBondMode() != BONDMODE_IDLE) return;

	UpdateData(true);
	UpdateLoopParaMenu();
	UpdateData(false);
}

void CLoopParaGeneral::OnFocusNeckHeight()
{
	if (theBond.GetBondMode() == BONDMODE_AUTO) return;

	GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRevHt, 0, 1, 1000);
	UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnPreKinkEnable()
{
    UpdateData();
    UpdateLoopParaMenu();
    UpdateData(false);
}

void CLoopParaGeneral::OnFocusPreKinkHt()
{
    GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreKinkHt, 60, -400, 400);
    UpdateData(false);
}

void CLoopParaGeneral::OnFocusPreKinkDist()
{
    GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPreKinkDist, 50, -2000, 2000);
    UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnFocusPreKinkAngle()
{
    GetDlgItem(IDC_STATIC_LOOP_GROUP)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPreKinkAngle, 20, -80, 80);
    UpdateData(false);
	SetLoopPara();	//v3.0.T365 add
}

void CLoopParaGeneral::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    short nOffset = 0;
    int nMin, nMax, iCurPos = GetScrollPos(SB_VERT);
    SCROLLINFO stScrollInfo;

    GetScrollInfo(SB_VERT, &stScrollInfo);
    nMin = stScrollInfo.nMin;
    nMax = stScrollInfo.nMax;

    switch(nSBCode)
    {
    case SB_LINEDOWN:
	{
		if (iCurPos == nMax) break;
		nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		if (iCurPos + nOffset > nMax)
			nOffset = nMax - iCurPos;
		break;
	}
    case SB_LINEUP:
	{
		if (iCurPos == nMin) break;
		nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE);
		if (iCurPos + nOffset < nMin)
			nOffset = nMin - iCurPos;
		break;
	}
    case SB_PAGEDOWN:
	{
		if (iCurPos == nMax) break;
		nOffset = m_nPageSize - iCurPos;
		break;
	}
    case SB_PAGEUP:
	{
		if (iCurPos == nMin) break;
		nOffset = -iCurPos;
		break;
	}
    default:
        break;
    }

    SetScrollPos(SB_VERT, iCurPos + nOffset);
    ScrollWindow(0, -nOffset);

    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLoopParaGeneral::OnDestroy()	//V1.1.W151 add
{
	SetLoopPara();
	theApp.ArchivePowerOnData(ARCHIVE_SAVE);
	CPropertyPage::OnDestroy();
}

//v3.0.T365 add
void CLoopParaGeneral::SetLoopPara()
{
	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[m_nLoopGroup], &stLoopPara);
	if ((m_nKinkNum > -1) && (m_nKinkNum < 3))
		stLoopPara.stLoopBase.nKinkNum = m_nKinkNum + 1;
	stLoopPara.stLoopBase.nLoopHt           = m_nLoopHt;
	stLoopPara.stLoopBase.nRevDist          = m_nRevDist;
	stLoopPara.stLoopBase.nRevAngle         = m_nRevAngle;
	stLoopPara.stLoopBase.nLoopHtCorrect    = m_nLoopHtCorrect;
	stLoopPara.stLoopBase.nKink2SpanLen     = m_nKink2SpanLen;
	stLoopPara.stLoopBase.nKink2SpanLenUnit = m_nKink2SpanLenUnit;
	stLoopPara.stLoopBase.nKink2HtFactor    = m_nKink2HtFactor;
	stLoopPara.stLoopBase.nLAngle           = m_nLAngle;
	stLoopPara.stLoopBase.nLoopBaseMode     = m_nLoopBaseMode;
	stLoopPara.stLoopBase.nRevHt            = m_nRevHt;
	stLoopPara.stLoopBase.bPreKinkEnable    = (m_nPreKinkEnable != 0);
	stLoopPara.stLoopBase.nPreKinkHt        = m_nPreKinkHt;
	stLoopPara.stLoopBase.nPreKinkDist      = m_nPreKinkDist;
	stLoopPara.stLoopBase.nPreKinkAngle     = m_nPreKinkAngle;
	theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[m_nLoopGroup], &stLoopPara);
}
