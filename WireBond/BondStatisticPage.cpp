/***************************************************************
CBondStatisticPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "BondStatistics.h"
#include "BondStatisticPage.h"
#include "Bond.h"
#include "AutoBondSheet.h"
#include "User.h"
#include "DlgMCID.h"
#include <comutil.h>

// CBondStatisticPage dialog
IMPLEMENT_DYNAMIC(CBondStatisticPage, CPropertyPage)

CBondStatisticPage::CBondStatisticPage()
	: CPropertyPage(CBondStatisticPage::IDD)
    , m_StartDateTime(COleDateTime::GetCurrentTime())
    , m_lUph(0)
    , m_lOutput(0)
    , m_szProductTime(_T(""))
    , m_szStandbyTime(_T(""))
    , m_szMTBATime(_T(""))
    , m_szMTBFTime(_T(""))
    , m_lBondedWire(0)
    , m_nAssistCount(0)
    , m_nFailureCount(0)
    , m_lCapillaryCount(0)
    , m_nNsopCount(0)
    , m_nNsolCount(0)
    , m_nEfoOpenCount(0)
    , m_nShortTailCount(0)
    , m_nLeadPrErrorCount(0)
    , m_nDiePrErrorCount(0)
    , m_nSkipDieCount(0)
    , m_nMhsCount(0)
{
	m_pTextFont = NULL;
}

CBondStatisticPage::~CBondStatisticPage()
{
	delete m_pTextFont;
}

void CBondStatisticPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_START_TIME, m_StartDateTime);
    DDX_Text(pDX, IDC_UPH, m_lUph);
    DDX_Text(pDX, IDC_OUTPUT, m_lOutput);
    DDX_Text(pDX, IDC_PRODUCT_TIME, m_szProductTime);
    DDX_Text(pDX, IDC_STANDBY_TIME, m_szStandbyTime);
    DDX_Text(pDX, IDC_MTBA, m_szMTBATime);
    DDX_Text(pDX, IDC_MTBF, m_szMTBFTime);
    DDX_Text(pDX, IDC_WIRE, m_lBondedWire);
    DDX_Text(pDX, IDC_ASSIST, m_nAssistCount);
    DDX_Text(pDX, IDC_FAILURE, m_nFailureCount);
    DDX_Text(pDX, IDC_CAPILLARY, m_lCapillaryCount);
    DDX_Text(pDX, IDC_NSOP, m_nNsopCount);
    DDX_Text(pDX, IDC_NSOL, m_nNsolCount);
    DDX_Text(pDX, IDC_EFO_OPEN, m_nEfoOpenCount);
    DDX_Text(pDX, IDC_SHORT_TAIL, m_nShortTailCount);
    DDX_Text(pDX, IDC_LEAD_PR, m_nLeadPrErrorCount);
    DDX_Text(pDX, IDC_DIE_PR, m_nDiePrErrorCount);
    DDX_Text(pDX, IDC_SKIP_DIE, m_nSkipDieCount);
    DDX_Text(pDX, IDC_MHS, m_nMhsCount);
    DDX_Text(pDX, IDC_WIRE_FEED, m_nWireFeedCount);	//V1.1.W251 add
    DDX_Text(pDX, IDC_LF_TIME, m_sLfCycleTime);
}

BEGIN_MESSAGE_MAP(CBondStatisticPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_RESET, OnReset)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnStop)
    ON_BN_CLICKED(IDC_BUTTON_AUTO_BOND, OnAutoBond)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CBondStatisticPage message handlers
BOOL CBondStatisticPage::OnInitDialog()
{
    m_pTextFont = new CFont();
    m_pTextFont->CreateFontW(23, 0, 0, 0, 500, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, _T("Arial")); //@1

	theApp.SetDlgFont(this,m_pTextFont);	//V1.1.W139 add

    SetDlgItemTextW(IDC_BUTTON_AUTO_BOND,theMsg.GetMsg(MSG_AUTO_BOND));
    SetDlgItemTextW(IDC_BUTTON_STOP,theMsg.GetMsg(MSG_STOP_BOND));
    SetDlgItemTextW(IDC_BUTTON_RESET,theMsg.GetMsg(MSG_RESET));
    SetDlgItemTextW(IDC_STATIC_STATISTICS,theMsg.GetMsg(MSG_MC_NUM)+_bstr_t(theMcConfig.GetMachineID()).GetBSTR()+_T(" (")+theApp.GetSoftwareVersion()+_T(")"));	//V1.1.W258 edit
    SetDlgItemTextW(IDC_STATIC_START_TIME,theMsg.GetMsg(MSG_START_TIME));
    SetDlgItemTextW(IDC_STATIC_OUTPUT,theMsg.GetMsg(MSG_OUTPUT_COLON));
    SetDlgItemTextW(IDC_STATIC_PRODUCTION,theMsg.GetMsg(MSG_PRODUCTION));
    SetDlgItemTextW(IDC_STATIC_STANDBY,theMsg.GetMsg(MSG_STANDBY));
    SetDlgItemTextW(IDC_STATIC_ASSIST,theMsg.GetMsg(MSG_ASSIST));
    SetDlgItemTextW(IDC_STATIC_FAILURE,theMsg.GetMsg(MSG_FAILURE));
    SetDlgItemTextW(IDC_STATIC_WIRE,theMsg.GetMsg(MSG_WIRE));
    SetDlgItemTextW(IDC_STATIC_CAPILLARY,theMsg.GetMsg(MSG_TOOL_USAGE));
    SetDlgItemTextW(IDC_STATIC_EFO_OPEN,theMsg.GetMsg(MSG_EFO_OPEN));
    SetDlgItemTextW(IDC_STATIC_SHORT_TAIL,theMsg.GetMsg(MSG_SHORT_TAIL));
    SetDlgItemTextW(IDC_STATIC_ASSISTANCE,theMsg.GetMsg(MSG_ASSISTANCE));

	{	//V1.1.W250 add
		short nID = 0;
		if (thePgm.GetRcd()->nAutoSkipDieUnit == 1)
			nID = MSG_SKIP_DIE;
		else if (thePgm.GetRcd()->nAutoSkipDieUnit == 2)
			nID = MSG_SKIP_UNIT;
		else if (thePgm.GetRcd()->nManualSkip == 0)
			nID = MSG_SKIP_DIE;
		else if (thePgm.GetRcd()->nManualSkip == 1)
			nID = MSG_SKIP_UNIT;

		SetDlgItemText(IDC_STATIC_SKIP_DIE_UNIT, theMsg.GetMsg(nID));
	}

    UpdateStatDisplay();
    SetTimer(1, 5000, NULL);

    return CPropertyPage::OnInitDialog();
}

HBRUSH CBondStatisticPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBondStatisticPage::OnReset()
{
	short nMsg = MSG_CONFIRM_TO_RESET;
	if ((short)theMsg.ShowMsg(WARNING_MSG, 1, &nMsg, true, false) == SHOW_MSG_RESULT_STOP)
		return;	//V1.1.W261 add

    theStat.ResetRcd();
    UpdateStatDisplay();
}

void CBondStatisticPage::UpdateStatDisplay(void)
{
	CString sFormat("%H:%M:%S");

	BOND_STATISTICS_RCD *pstStatRcd = theStat.GetRcd();
    m_StartDateTime     = COleDateTime(pstStatRcd->stStatsStartTime);
    m_lOutput           = pstStatRcd->lOutputUnit;
    m_lBondedWire       = pstStatRcd->lBondedWire;
    m_lCapillaryCount   = pstStatRcd->lCapillaryCount;
    m_nNsopCount        = pstStatRcd->nNsopCount;
    m_nNsolCount        = pstStatRcd->nNsolCount;
    m_nEfoOpenCount     = pstStatRcd->nEfoOpenCount;
    m_nShortTailCount   = pstStatRcd->nShortTailCount;
    m_nLeadPrErrorCount = pstStatRcd->nLeadPrErrorCount;
    m_nDiePrErrorCount  = pstStatRcd->nDiePrErrorCount;
    m_nSkipDieCount     = pstStatRcd->nSkipDieCount;
    m_nMhsCount         = pstStatRcd->nMhsCount;
	m_nWireFeedCount	= pstStatRcd->nWireFeedCount;	//V1.1.W251 add
	m_lUph              = pstStatRcd->tProductionTime > 0 ? theStat.GetNetUph() : 0;
    m_szProductTime     = CTime(pstStatRcd->tProductionTime).FormatGmt(sFormat);
    m_szStandbyTime     = CTime(pstStatRcd->tStandbyTime).FormatGmt(sFormat);
    m_nAssistCount      = pstStatRcd->nAssistCount;

	time_t tt = (m_nAssistCount > 0 ? pstStatRcd->tProductionTime / m_nAssistCount : pstStatRcd->tProductionTime);
    m_szMTBATime        = CTime(tt).FormatGmt(sFormat);
    m_nFailureCount     = pstStatRcd->nFailureCount;
	tt = (m_nFailureCount > 0 ? pstStatRcd->tProductionTime / m_nFailureCount : pstStatRcd->tProductionTime);
    m_szMTBFTime        = CTime(tt).FormatGmt(sFormat);

	if (theDebug.IsDebugLfCycleTime())
		m_sLfCycleTime.Format(_T("%.3fs"), theBond.GetLfCycleTime());

    UpdateData(false);
}

BOOL CBondStatisticPage::OnSetActive()
{
	UpdateStatDisplay();
    return CPropertyPage::OnSetActive();
}

void CBondStatisticPage::OnTimer(UINT_PTR nIDEvent)
{
    UpdateStatDisplay();
    CPropertyPage::OnTimer(nIDEvent);
}

void CBondStatisticPage::OnStop()
{
	UpdateStatDisplay();
    CAutoBondSheet* pAutoBondSheet = (CAutoBondSheet*)GetParent();
    pAutoBondSheet->m_AutoBondPage.StopAutoBond();
}

void CBondStatisticPage::OnAutoBond()
{
	UpdateStatDisplay();
	if (theBond.IsAutoBonding()) return;	//v1.6.T315 add
	CAutoBondSheet* pAutoBondSheet = (CAutoBondSheet*)GetParent();
    pAutoBondSheet->m_AutoBondPage.StartAutoBond();
}

void CBondStatisticPage::OnDestroy()	//V1.1.W148 add
{
	KillTimer(1);
	CPropertyPage::OnDestroy();
}

void CBondStatisticPage::PopMCIDDlg(CPoint& pt)	//V1.1.W286 add
{
	if (CRect(23,13,266,29).PtInRect(pt) == FALSE) return;
	if (theUser.GetUserLevel() < USER_ENGINEER) return;

	CAutoBondSheet* pAutoBondSheet = (CAutoBondSheet*)GetParent();
	if (pAutoBondSheet->m_AutoBondPage.GetDlgItem(IDC_AUTO_BOND)->IsWindowEnabled() == FALSE)
		return;

	CDlgMCID(this).DoModal();
	SetDlgItemTextW(IDC_STATIC_STATISTICS, theMsg.GetMsg(MSG_MC_NUM) + _bstr_t(theMcConfig.GetMachineID()).GetBSTR() + _T(" (") + theApp.GetSoftwareVersion() + _T(")"));
}

void CBondStatisticPage::OnLButtonDblClk(UINT nFlags, CPoint pt)	//V1.1.W286 edit
{
	PopMCIDDlg(pt);
	CPropertyPage::OnLButtonDblClk(nFlags, pt);
}
