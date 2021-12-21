#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "SingleTestingPage.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledOffload.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "Bond.h"
#include "VEOnload.h"
#include "VEOffload.h"
#include "VEIndex.h"
#include "Debug.h"

// CSingleTestingPage dialog

IMPLEMENT_DYNAMIC(CSingleTestingPage, CPropertyPage)

CSingleTestingPage::CSingleTestingPage()
	: CPropertyPage(CSingleTestingPage::IDD)
	, m_bOnloadEnable(FALSE)
	, m_bTrackEnable(FALSE)
	, m_bForkOffloadEnable(FALSE)
	, m_bSensorEnable(FALSE)
	, m_lIndexCycleTime(0)
	, m_iUph(0)
{

}

CSingleTestingPage::~CSingleTestingPage()
{
}

void CSingleTestingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ONLOAD, m_bOnloadEnable);
	DDX_Check(pDX, IDC_CHECK_TRACK, m_bTrackEnable);
	DDX_Check(pDX, IDC_CHECK_OFFLOAD, m_bForkOffloadEnable);
	DDX_Check(pDX, IDC_CHECK_SENSOR_VALID, m_bSensorEnable);
	DDX_Text(pDX, IDC_EDIT_INDEX_CYCLE, m_lIndexCycleTime);
}

BEGIN_MESSAGE_MAP(CSingleTestingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ONLOAD, &CSingleTestingPage::OnBnClickedCheckOnload)
	ON_BN_CLICKED(IDC_CHECK_TRACK, &CSingleTestingPage::OnBnClickedCheckTrack)
	ON_BN_CLICKED(IDC_CHECK_OFFLOAD, &CSingleTestingPage::OnBnClickedCheckOffload)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CSingleTestingPage::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CSingleTestingPage::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_CHECK_SENSOR_VALID, &CSingleTestingPage::OnBnClickedCheckSensorValid)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TEST, &CSingleTestingPage::OnBnClickedButtonStopTest)
	ON_BN_CLICKED(IDC_BUTTON_ONLOAD_TEST, &CSingleTestingPage::OnBnClickedButtonOnloadTest)
	ON_BN_CLICKED(IDC_BUTTON_OFFLOAD_TEST, &CSingleTestingPage::OnBnClickedButtonOffloadTest)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSingleTestingPage message handlers
BOOL CSingleTestingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_CHECK_ONLOAD);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INPUT));

    pWnd = GetDlgItem(IDC_CHECK_TRACK);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TRACK));

    pWnd = GetDlgItem(IDC_CHECK_OFFLOAD);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OUTPUT));

	pWnd = GetDlgItem(IDC_BUTTON_INIT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INITIALIZE));

	pWnd = GetDlgItem(IDC_BUTTON_RUN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DRY_RUN));

    pWnd = GetDlgItem(IDC_BUTTON_STOP_TEST);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

	pWnd = GetDlgItem(IDC_CHECK_SENSOR_VALID);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RUN_WITH_SENSORS));

	pWnd = GetDlgItem(IDC_STATIC_MODULE_TEST);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_MODULE_TEST));

	pWnd = GetDlgItem(IDC_BUTTON_ONLOAD_TEST);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INPUT));

	pWnd = GetDlgItem(IDC_BUTTON_OFFLOAD_TEST);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OUTPUT));

	pWnd = GetDlgItem(IDC_STATIC_STATISTIC);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STATISTICS));

	pWnd = GetDlgItem(IDC_STATIC_DRY_RUN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DRY_RUN));

	pWnd = GetDlgItem(IDC_STATIC_INDEX_TIME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_CYCLE_TIME));

	m_bOnloadEnable = TRUE;
	m_bTrackEnable = TRUE;
	m_bForkOffloadEnable = TRUE;
	m_bSensorEnable = TRUE;
	
	m_lIndexCycleTime = theVledTrack.GetIndexCycleTime();

	double dUph;

	if (m_lIndexCycleTime == 0)
    	m_iUph = 0;
	else
	   m_iUph = round((double)(3600 * 1000 / m_lIndexCycleTime));

	dUph = (double)m_iUph / 1000.0;
	pWnd = GetDlgItem(IDC_EDIT_INDEX_UPH);
	CString str;
	str.Format(_T("%0.1f K"), dUph);
	pWnd->SetWindowTextW(str);

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSingleTestingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CSingleTestingPage::OnBnClickedCheckOnload()
{
	UpdateData(true);
    theVledOnload.SetOnloadValid(m_bOnloadEnable);
}

void CSingleTestingPage::OnBnClickedCheckTrack()
{
	UpdateData(true);
    theVledTrack.SetTrackValid(m_bTrackEnable);
}

void CSingleTestingPage::OnBnClickedCheckOffload()
{
	UpdateData(true);    
    theVledOffload.SetForkOffloadValid(m_bForkOffloadEnable);
}

void CSingleTestingPage::OnBnClickedCheckSensorValid()
{
	UpdateData(true);	
	theMHS.SetSensorEnable(m_bSensorEnable == TRUE ? true : false);
}

void CSingleTestingPage::OnBnClickedButtonInit()
{
	theMHS.WarmReset();
	theMHS.ShowError();
}

void CSingleTestingPage::OnBnClickedButtonRun()
{
    if (theDebug.IsDebugWorkHolder()) theDebug.ClearInfo();	//v3.0.T96 edit

    theMHS.InitAutoIndex();
    theMHS.SetDryRun(true);
    theMHS.SetAutoIndex(true);

	SetTimer(1, 1000, NULL);

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_RUN);
    pWnd->EnableWindow(FALSE);

    pWnd = GetDlgItem(IDC_BUTTON_STOP_TEST);
    pWnd->EnableWindow(TRUE);
}

void CSingleTestingPage::OnBnClickedButtonStopTest()
{
	theMHS.SetDryRun(false);
    theMHS.SetAutoIndex(false);
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteTrack.SetOffloadEnable(false);
		break;
	case MHS_VLED:
		theVledTrack.SetOffloadEnableFlag(false);
		break;
	}

	KillTimer(1);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_RUN);
    pWnd->EnableWindow(TRUE);
    
    pWnd = GetDlgItem(IDC_BUTTON_STOP_TEST);
    pWnd->EnableWindow(FALSE);
}

void CSingleTestingPage::OnBnClickedButtonOnloadTest()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOnload.OnloadLeadFrame();
		break;
	case MHS_VLED:
		theVledOnload.OnloadLeadFrame();
		break;
	}
}

void CSingleTestingPage::OnBnClickedButtonOffloadTest()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOffload.OffloadLeadframe();
		break;
	case MHS_VLED:
		theVledOffload.OffloadLeadframe();
		break;
	}
}

void CSingleTestingPage::OnOK()
{
	theVledOnload.SetOnloadValid(TRUE);
	theVledTrack.SetTrackValid(TRUE);
	theVledOffload.SetForkOffloadValid(TRUE);
	theMHS.SetSensorEnable(true);

	CPropertyPage::OnOK();
}

void CSingleTestingPage::OnTimer(UINT_PTR nIDEvent)
{
	m_lIndexCycleTime = theVledTrack.GetIndexCycleTime();

	if (m_lIndexCycleTime == 0)
		m_iUph = 0;
	else
		m_iUph = round((double)(3600 * 1000 / m_lIndexCycleTime));

	double dUph = (double)m_iUph / 1000.0;

	CString szString;
	CWnd* pWnd = GetDlgItem(IDC_EDIT_INDEX_UPH);
	szString.Format(_T("%0.1f K"), dUph);
	pWnd->SetWindowTextW(szString);

	UpdateData(false);

	CPropertyPage::OnTimer(nIDEvent);
}
