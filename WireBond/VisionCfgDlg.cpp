/***************************************************************
VisionCfgDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Bond.h"
#include "VisionCfgDlg.h"
#include "VisionSystem.h"
#include "MachineConfig.h"
#include "User.h"
#include "BondStatistics.h"	//V1.1.W250 add

// CVisionCfgDlg dialog

IMPLEMENT_DYNAMIC(CVisionCfgDlg, CPropertyPage)

CVisionCfgDlg::CVisionCfgDlg()
	: CPropertyPage(CVisionCfgDlg::IDD)
    , m_nShortDistSrchDelay(0)
    , m_nLongDistSrchDelay(0)
    , m_lSinglePr(0)
    , m_lPreMoveEnable(1)
    , m_nPrPassScore(60)
    , m_nImageTransferTime(0)
    , m_iAutoSkipEnable(0)
    , m_iDieSearchRange(0)
    , m_iLeadSearchRange(0)
    , m_iPrIndexSearchRange(0)
    , m_nDieAlignTol(0)
    , m_nLeadAlignTol(0)
    , m_nLeadPrPassScore(0)
    , m_iLeadPrEnable(0)
    , m_nExposureTime(0)
    , m_RetryExposureTime(0)
	, m_nAutoSkipScore(0)
	, m_iManualSkip(0)
{
}

CVisionCfgDlg::~CVisionCfgDlg()
{
}

void CVisionCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHORT_DELAY, m_nShortDistSrchDelay);
	DDX_Text(pDX, IDC_EDIT_LONG_DELAY, m_nLongDistSrchDelay);
	DDX_CBIndex(pDX, IDC_COMBO_SINGLE_PR, m_lSinglePr);
	DDX_Text(pDX, IDC_EDIT_PR_SCORE, m_nPrPassScore);
	DDX_CBIndex(pDX, IDC_COMBO_AUTO_SKIP, m_iAutoSkipEnable);
	DDX_CBIndex(pDX, IDC_COMBO_DIE_RANGE, m_iDieSearchRange);
	DDX_CBIndex(pDX, IDC_COMBO_LEAD_RANGE, m_iLeadSearchRange);
	DDX_CBIndex(pDX, IDC_COMBO_PRINDEX_LEAD_RANGE, m_iPrIndexSearchRange);
	DDX_Text(pDX, IDC_EDIT_DIE_TOL, m_nDieAlignTol);
	DDX_Text(pDX, IDC_EDIT_LEAD_TOL, m_nLeadAlignTol);
	DDX_Text(pDX, IDC_EDIT_LEAD_PR_SCORE, m_nLeadPrPassScore);
	DDX_CBIndex(pDX, IDC_COMBO_LEAD_PR, m_iLeadPrEnable);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE_TIME, m_nExposureTime);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE_TIME2, m_RetryExposureTime);
	DDX_Text(pDX, IDC_EDIT_AUTO_SKIP_SCORE, m_nAutoSkipScore);
	DDX_CBIndex(pDX, IDC_COMBO_MANUAL_SKIP, m_iManualSkip);
}

BEGIN_MESSAGE_MAP(CVisionCfgDlg, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_SHORT_DELAY, OnFocusShortDelay)
    ON_EN_SETFOCUS(IDC_EDIT_LONG_DELAY, OnFocusLongDelay)
    ON_EN_SETFOCUS(IDC_EDIT_PR_SCORE, OnFocusPrScore)
    ON_CBN_SELCHANGE(IDC_COMBO_DIE_RANGE, OnDieRange)
    ON_CBN_SELCHANGE(IDC_COMBO_LEAD_RANGE, OnLeadRange)
    ON_CBN_SELCHANGE(IDC_COMBO_PRINDEX_LEAD_RANGE, OnPrIndexingLeadRange)
    ON_EN_SETFOCUS(IDC_EDIT_DIE_TOL, OnFocusDieTol)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_TOL, OnFocusLeadTol)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_PR_SCORE, OnFocusLeadPrScore)
	ON_CBN_SELCHANGE(IDC_COMBO_SINGLE_PR, OnSinglePr)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTO_SKIP, OnAutoSkip)
    ON_CBN_SELCHANGE(IDC_COMBO_LEAD_PR, OnLeadPr)
    ON_EN_SETFOCUS(IDC_EDIT_EXPOSURE_TIME, OnFocusExposureTime)
    ON_EN_SETFOCUS(IDC_EDIT_EXPOSURE_TIME2, OnFocusExposureTime2)
	ON_EN_CHANGE(IDC_EDIT_EXPOSURE_TIME, OnEditExposureTime)
	ON_EN_SETFOCUS(IDC_EDIT_AUTO_SKIP_SCORE, OnFocusAutoSkipScore)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()		//V1.1.W259 add
END_MESSAGE_MAP()

// CVisionCfgDlg message handlers
BOOL CVisionCfgDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    const PGM_RECORD *pstPgm = thePgm.GetRcd();

    m_lSinglePr           = pstPgm->nPrAlignMode;
    m_nShortDistSrchDelay = pstPgm->nVisionMoveDelay;
    m_nLongDistSrchDelay  = pstPgm->nVisionLongMoveDelay;
    m_lPreMoveEnable      = pstPgm->nVisionPreMove;
    m_nPrPassScore        = pstPgm->nPrPassScore;
    m_nImageTransferTime  = pstPgm->nImageTransferTime;
    m_iAutoSkipEnable     = pstPgm->nAutoSkipDieUnit;
    m_iDieSearchRange     = pstPgm->nDieSearchRange - 1;
    m_iLeadSearchRange    = pstPgm->nLeadSearchRange - 1;
    m_iPrIndexSearchRange = pstPgm->nPrIndexSearchRange - 1;
    m_nDieAlignTol        = pstPgm->nDiePrTol;
    m_nLeadAlignTol       = pstPgm->nLeadPrTol;
    m_nLeadPrPassScore    = pstPgm->nLeadPrPassScore;
    m_iLeadPrEnable       = pstPgm->bLeadPrEnable;
    m_nExposureTime       = pstPgm->nExposureTime;
    m_RetryExposureTime   = pstPgm->nRetryExposureTime;
	m_nAutoSkipScore      = pstPgm->nAutoSkipScore;
	m_iManualSkip         = pstPgm->nManualSkip;

    SetDlgItemTextW(IDC_STATIC_MOTION_DELAY, theMsg.GetMsg(MSG_MOTION_DELAY));
    SetDlgItemTextW(IDC_STATIC_SHORT_DIST, theMsg.GetMsg(MSG_SHORT_DIST));
    SetDlgItemTextW(IDC_STATIC_LONG_DIST, theMsg.GetMsg(MSG_LONG_DIST));
    SetDlgItemTextW(IDC_STATIC_SRCH_RANGE, theMsg.GetMsg(MSG_PR_SRCH_RANGE));
    SetDlgItemTextW(IDC_STATIC_DIE, theMsg.GetMsg(MSG_DIE));
    SetDlgItemTextW(IDC_STATIC_LEAD, theMsg.GetMsg(MSG_LEAD));
    SetDlgItemTextW(IDC_STATIC_ALIGN_TOL, theMsg.GetMsg(MSG_VISION_ALIGN_TOL));
    SetDlgItemTextW(IDC_STATIC_DIE_TOL, theMsg.GetMsg(MSG_DIE));
    SetDlgItemTextW(IDC_STATIC_LEAD_TOL, theMsg.GetMsg(MSG_LEAD));
    SetDlgItemTextW(IDC_STATIC_SRCH_MODE, theMsg.GetMsg(MSG_SRCH_MODE));
    SetDlgItemTextW(IDC_STATIC_PR_SCORE, theMsg.GetMsg(MSG_PR_PASS_PERCENT));
    SetDlgItemTextW(IDC_STATIC_LEAD_PR, theMsg.GetMsg(MSG_LEAD_PR));

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LEAD_PR);
    pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
    pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));

    SetDlgItemTextW(IDC_STATIC_SINGLE_PR,theMsg.GetMsg(MSG_SINGLE_PR));
	GetDlgItem(IDC_STATIC_SINGLE_PR)->ShowWindow(theUser.GetUserLevel() > USER_PROCESS);

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SINGLE_PR);
    pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
    pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));
	pComboBox->ShowWindow(theUser.GetUserLevel() > USER_PROCESS);

	SetDlgItemTextW(IDC_STATIC_MANUAL_SKIP, theMsg.GetMsg(MSG_MANUAL_SKIP));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MANUAL_SKIP);
	pComboBox->AddString(theMsg.GetMsg(MSG_DIE));
	pComboBox->AddString(theMsg.GetMsg(MSG_UNIT));

    SetDlgItemTextW(IDC_STATIC_AUTO_SKIP, theMsg.GetMsg(MSG_AUTO_SKIP_DIE_UNIT));
    
    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_AUTO_SKIP);
    pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
    pComboBox->AddString(theMsg.GetMsg(MSG_DIE));
    pComboBox->AddString(theMsg.GetMsg(MSG_UNIT));

	SetDlgItemTextW(IDC_STATIC_AUTO_SKIP_SCORE, theMsg.GetMsg(MSG_AUTO_SKIP_SCORE));
    SetDlgItemTextW(IDC_STATIC_PRINDEX_LEAD, theMsg.GetMsg(MSG_INDEX));
	SetDlgItemText(IDC_STATIC_DOUBLE_SEARCH_RANGE, theMsg.GetMsg(MSG_SRCH_RANGE_RETRY));

    pComboBox = (CComboBox*)GetDlgItem(IDC_CB_DOUBLE_SEARCH_RANGE);
    pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
    pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));
	pComboBox->SetCurSel(pstPgm->bTry2xSearchRange);

    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        SetDlgItemTextW(IDC_STATIC_EXPOSURE_TIME, theMsg.GetMsg(MSG_EXPOSURE_TIME_COLON));
        SetDlgItemTextW(IDC_STATIC_DEBUG, theMsg.GetMsg(MSG_DEBUG_COLON));
    }
    else
    {
        GetDlgItem(IDC_STATIC_EXPOSURE_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_EXPOSURE_TIME)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_EXPOSURE_TIME2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_DEBUG)->ShowWindow(SW_HIDE);

        pComboBox->ShowWindow(SW_HIDE);
    }

	SetScrollRange(SB_VERT, 0, PROPERTY_PAGE_HEIGHT/2);

    return CPropertyPage::OnInitDialog();
}

HBRUSH CVisionCfgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CVisionCfgDlg::OnFocusShortDelay()
{
    GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nShortDistSrchDelay, 0, 0, 1000);
    UpdateData(false);
}

void CVisionCfgDlg::OnFocusLongDelay()
{
    GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nLongDistSrchDelay, 0, 0, 1000);
    UpdateData(false);
}

void CVisionCfgDlg::OnFocusPrScore()
{
	short nOldScore = m_nPrPassScore;	//v1.5.T32 add

	GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPrPassScore, 70, 0, 100);
    UpdateData(FALSE);

	//v1.5.T32 add
	if (nOldScore != m_nPrPassScore)
		theLog.WriteLong2(_T("Change Die PR Score: %d->%d"), nOldScore, m_nPrPassScore, true);
}

void CVisionCfgDlg::OnDieRange()
{
    UpdateData();
	//v3.0.T321 edit
	short nWidth;
	short nHeight;
	if (theMcConfig.GetMhsType() == MHS_FOXCONN)
	{
		nWidth  = FOXCONN_VISION_SEARCH_WIDTH * (m_iDieSearchRange + 1);
		nHeight = FOXCONN_VISION_SEARCH_HEIGHT * (m_iDieSearchRange + 1);
	}
	else
	{
		nWidth  = VISION_SEARCH_BASE_WIDTH * (m_iDieSearchRange + 1);
		nHeight = VISION_SEARCH_BASE_HEIGHT * (m_iDieSearchRange + 1);
	}
    theVisionSys.SetSearchSize(nWidth, nHeight);
}

void CVisionCfgDlg::OnLeadRange()
{
    UpdateData();
    short nWidth  = VISION_SEARCH_BASE_WIDTH * (m_iLeadSearchRange + 1);
    short nHeight = VISION_SEARCH_BASE_HEIGHT * (m_iLeadSearchRange + 1);
    theVisionSys.SetSearchSize(nWidth, nHeight);
}

void CVisionCfgDlg::OnPrIndexingLeadRange()
{
    UpdateData();
    short nWidth  = VISION_SEARCH_BASE_WIDTH * (m_iPrIndexSearchRange + 1);
    short nHeight = VISION_SEARCH_BASE_HEIGHT * (m_iPrIndexSearchRange + 1);
    theVisionSys.SetSearchSize(nWidth, nHeight);
}

void CVisionCfgDlg::OnFocusDieTol()
{
    GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nDieAlignTol, 50, 0, 100);
    UpdateData(false);
}

void CVisionCfgDlg::OnFocusLeadTol()
{
    GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nLeadAlignTol, 100, 40, 500);
    UpdateData(false);
}

void CVisionCfgDlg::OnFocusLeadPrScore()
{
	short nOldScore = m_nLeadPrPassScore;	//v1.5.T32 add

	GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nLeadPrPassScore, 40, 40, 100);
	UpdateData(FALSE);

	//v1.5.T32 add
	if (nOldScore != m_nLeadPrPassScore)
		theLog.WriteLong2(_T("Change Lead PR Score: %d->%d"), nOldScore, m_nLeadPrPassScore, true);
}

void CVisionCfgDlg::OnDestroy()		//V1.1.W259 edit
{
    UpdateData(true);

    PGM_RECORD *pstPgm = thePgm.GetRcd();
    pstPgm->nPrAlignMode         = m_lSinglePr;
    pstPgm->nVisionMoveDelay     = m_nShortDistSrchDelay;
    pstPgm->nVisionLongMoveDelay = m_nLongDistSrchDelay;
    pstPgm->nVisionPreMove       = m_lPreMoveEnable;
    pstPgm->nPrPassScore         = m_nPrPassScore;
    pstPgm->nLeadPrPassScore     = m_nLeadPrPassScore;
    pstPgm->nImageTransferTime   = m_nImageTransferTime;
	pstPgm->bTry2xSearchRange    = (BYTE)((CComboBox*)GetDlgItem(IDC_CB_DOUBLE_SEARCH_RANGE))->GetCurSel();
	pstPgm->nAutoSkipDieUnit     = m_iAutoSkipEnable;
    pstPgm->nDieSearchRange      = m_iDieSearchRange + 1;
    pstPgm->nLeadSearchRange     = m_iLeadSearchRange + 1;
    pstPgm->nPrIndexSearchRange  = m_iPrIndexSearchRange + 1;
    pstPgm->nDiePrTol            = m_nDieAlignTol;
    pstPgm->nLeadPrTol           = m_nLeadAlignTol;
    pstPgm->bLeadPrEnable        = m_iLeadPrEnable!=0;;
    pstPgm->nExposureTime        = m_nExposureTime;
    pstPgm->nRetryExposureTime   = m_RetryExposureTime;
	pstPgm->nAutoSkipScore       = m_nAutoSkipScore;
	pstPgm->nManualSkip          = m_iManualSkip;

    theBond.SetPrPatternPassScore();
    theVisionSys.SavePatternRcd();	//v3.0.T70 edit

    CPropertyPage::OnDestroy();		//V1.1.W259 edit
}

void CVisionCfgDlg::OnSinglePr()
{
	UpdateData(true);
}

void CVisionCfgDlg::OnDebug()
{
	UpdateData(true);
}

void CVisionCfgDlg::OnAutoSkip()	//V1.1.W250 edit
{
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AUTO_SKIP))->GetCurSel();
	if (nSel == m_iAutoSkipEnable) return;

	CStringList sList;
	sList.AddTail(theMsg.GetMsg(MSG_RESET_SKIP_STAT));
	sList.AddTail(theMsg.GetMsg(MSG_YES_TO_PROCEED));
	nSel = theMsg.ShowMsgX(WARNING_MSG, sList, 0, 0, 1) != SHOW_MSG_RESULT_STOP;
	UpdateData(nSel);
	if (nSel) theStat.ResetRcd(STATERR_SKIP_DIE);
}

void CVisionCfgDlg::OnLeadPr()
{
	UpdateData(true);
}

void CVisionCfgDlg::OnFocusExposureTime()
{
    GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nExposureTime, 1000, 100, 8000);
    UpdateData(false);

	OnEditExposureTime();
}

void CVisionCfgDlg::OnFocusExposureTime2()
{
    GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();    
    theKeyPad.GetUserKeyInData(&m_RetryExposureTime, 2000, 100, 8000);
    UpdateData(FALSE);
}

void CVisionCfgDlg::OnEditExposureTime()
{
	UpdateData(TRUE);
	theVisionSys.SetCameraExposureTime(m_nExposureTime);
}

void CVisionCfgDlg::OnFocusAutoSkipScore()
{
	//UpdateData(true);	//v1.5.T32 delete
	short nOldScore = m_nAutoSkipScore;	//v1.5.T32 add

	GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nAutoSkipScore, 50, 0, 100);
	UpdateData(FALSE);

	//v1.5.T32 add
	if (nOldScore != m_nAutoSkipScore)
		theLog.WriteLong2(_T("Change Skip PR Score: %d->%d"), nOldScore, m_nAutoSkipScore, true);
}

void CVisionCfgDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	short nOffset = 0;
	int nCurPos = GetScrollPos(SB_VERT);

	SCROLLINFO stScrollInfo;
	GetScrollInfo(SB_VERT, &stScrollInfo);

    switch(nSBCode)
    {
    case SB_LINEDOWN:
		if (nCurPos == stScrollInfo.nMax) break;
		nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		if (nCurPos + nOffset > stScrollInfo.nMax)
			nOffset = stScrollInfo.nMax - nCurPos;
        break;
    case SB_LINEUP:
		if (nCurPos == stScrollInfo.nMin) break;
		nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE);
		if (nCurPos + nOffset < stScrollInfo.nMin)
			nOffset = stScrollInfo.nMin - nCurPos;
        break;
    case SB_PAGEDOWN:
		if (nCurPos == stScrollInfo.nMax) break;
		nOffset = PROPERTY_PAGE_HEIGHT/2 - nCurPos;
		break;
    case SB_PAGEUP:
		if (nCurPos == stScrollInfo.nMin) break;
		nOffset = -nCurPos;
		break;
	case SB_THUMBTRACK:
		nOffset = nPos - nCurPos;
        break;
    }

	SetScrollPos(SB_VERT, nCurPos + nOffset);
	ScrollWindow(0, -nOffset);

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CVisionCfgDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, NULL);
	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CVisionCfgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_MOTION_DELAY)->SetFocus();
	CPropertyPage::OnMouseMove(nFlags, point);
}
