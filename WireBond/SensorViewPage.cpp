// SensorViewPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "SensorViewPage.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "VledOffload.h"
#include "VEOffload.h"

// CSensorViewPage dialog
IMPLEMENT_DYNAMIC(CSensorViewPage, CPropertyPage)

CSensorViewPage::CSensorViewPage()
	: CPropertyPage(CSensorViewPage::IDD)
	, m_iSensorId(0)
{
}

CSensorViewPage::~CSensorViewPage()
{	
}

void CSensorViewPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_LBIndex(pDX, IDC_LIST_SENSOR, m_iSensorId);
}

BEGIN_MESSAGE_MAP(CSensorViewPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_SENSOR, OnListSensor)
	ON_BN_CLICKED(IDC_BUTTON_MAG_KICKER_ON, OnMagKickerOn)
	ON_BN_CLICKED(IDC_BUTTON_MAG_KICKER_OFF, OnMagKickerOff)
	ON_BN_CLICKED(IDC_BUTTON_LF_KICKER_ON, OnLfKickerSolOn)
	ON_BN_CLICKED(IDC_BUTTON_LF_KICKER_OFF, OnLfKickerSolOff)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_ON, OnClawOn)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_OFF, OnClawOff)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_ON, OnPusherOn)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_OFF, OnPusherOff)
	ON_WM_DESTROY()		//V1.5.W67 add
END_MESSAGE_MAP()

// CSensorViewPage message handlers
BOOL CSensorViewPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_SENSOR);

    CString szString = theMsg.GetMsg(MSG_ONLOAD);
	if (theMcConfig.GetMhsType() != MHS_VLED_ELITE)
		szString += _T("/") + theMsg.GetMsg(MSG_OFFLOAD);
    pListBox->AddString(szString);
    pListBox->AddString(theMsg.GetMsg(MSG_TRACK_FRONT));
    pListBox->AddString(theMsg.GetMsg(MSG_TRACK_BACK));
	if (theMcConfig.GetMhsType() == MHS_VLED_ELITE)
		pListBox->AddString(theMsg.GetMsg(MSG_OFFLOAD));

    SetDlgItemText(IDC_STATIC_SENSOR,theMsg.GetMsg(MSG_SENSOR));
    SetDlgItemText(IDC_STATIC_OFFLOAD,theMsg.GetMsg(MSG_OFFLOAD));
	SetDlgItemText(IDC_STATIC_CLAW,theMsg.GetMsg(theMcConfig.GetMhsType() == MHS_VLED_ELITE ? MSG_MAG_BUFFER : MSG_CLAW));
	SetDlgItemText(IDC_STATIC_PUSHER,theMsg.GetMsg(theMcConfig.GetMhsType() == MHS_VLED_ELITE ? MSG_MAG_CLAMPER : MSG_PUSHER));
    SetDlgItemText(IDC_BUTTON_CLAW_ON,theMsg.GetMsg(MSG_ON));
    SetDlgItemText(IDC_BUTTON_CLAW_OFF,theMsg.GetMsg(MSG_OFF));
    SetDlgItemText(IDC_BUTTON_PUSHER_ON,theMsg.GetMsg(MSG_ON));
    SetDlgItemText(IDC_BUTTON_PUSHER_OFF,theMsg.GetMsg(MSG_OFF));

	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		SetDlgItemText(IDC_STATIC_LF_KICKER,theMsg.GetMsg(MSG_LF_KICKER));
		SetDlgItemText(IDC_BUTTON_LF_KICKER_ON,theMsg.GetMsg(MSG_ON));
		SetDlgItemText(IDC_BUTTON_LF_KICKER_OFF,theMsg.GetMsg(MSG_OFF));
		SetDlgItemText(IDC_STATIC_MAG_KICKER,theMsg.GetMsg(MSG_MAG_KICKER));
		SetDlgItemText(IDC_BUTTON_MAG_KICKER_ON,theMsg.GetMsg(MSG_ON));
		SetDlgItemText(IDC_BUTTON_MAG_KICKER_OFF,theMsg.GetMsg(MSG_OFF));
		break;
	default:
		GetDlgItem(IDC_STATIC_LF_KICKER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LF_KICKER_ON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LF_KICKER_OFF)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAG_KICKER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAG_KICKER_ON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MAG_KICKER_OFF)->ShowWindow(SW_HIDE);
	}

	m_iSensorId = 0;

	m_SensorPage2 = new CSensorLayoutPage2();
	m_SensorPage2->Create(CSensorLayoutPage2::IDD, this);

	m_SensorPage3 = new CSensorLayoutPage3();
	m_SensorPage3->Create(CSensorLayoutPage3::IDD, this);

	m_SensorPage4 = new CSensorLayoutPage4();
	m_SensorPage4->Create(CSensorLayoutPage4::IDD, this);

	m_SensorPage = new CSensorLayoutPage();
	m_SensorPage->Create(CSensorLayoutPage::IDD, this);

	return CPropertyPage::OnInitDialog();
}

HBRUSH CSensorViewPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CSensorViewPage::OnDestroy()
{
	m_SensorPage->DestroyWindow();
	delete m_SensorPage;

	m_SensorPage2->DestroyWindow();
	delete m_SensorPage2;

	m_SensorPage3->DestroyWindow();
	delete m_SensorPage3;

	m_SensorPage4->DestroyWindow();
	delete m_SensorPage4;

	CPropertyPage::OnDestroy();
}

void CSensorViewPage::OnListSensor()
{
	UpdateData(true);

	if (m_iSensorId == 0)
		m_SensorPage->SetForegroundWindow();
	else if (m_iSensorId == 1)
		m_SensorPage2->SetForegroundWindow();
	else if (m_iSensorId == 2)
		m_SensorPage3->SetForegroundWindow();
	else if (m_iSensorId == 3)
		m_SensorPage4->SetForegroundWindow();
}

void CSensorViewPage::OnClawOn()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOffload.m_MagSupportSol.SetRelayState(RELAY_ON);
		theVEliteOffload.m_MagSupportSol2.SetRelayState(RELAY_ON);
		break;
	case MHS_VLED:
		theVledOffload.theOutputClaw.SetRelayState(RELAY_ON);
		break;
	}
}

void CSensorViewPage::OnClawOff()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOffload.m_MagSupportSol.SetRelayState(RELAY_OFF);
		theVEliteOffload.m_MagSupportSol2.SetRelayState(RELAY_OFF);
		break;
	case MHS_VLED:
		theVledOffload.theOutputClaw.SetRelayState(RELAY_OFF);
		break;
	}
}

void CSensorViewPage::OnPusherOn()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOffload.m_MagLockSol.SetRelayState(RELAY_ON);
		break;
	case MHS_VLED:
		theVledOffload.thePusherCylinder.MoveForward();
		break;
	}
}

void CSensorViewPage::OnPusherOff()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOffload.m_MagLockSol.SetRelayState(RELAY_OFF);
		break;
	case MHS_VLED:
		theVledOffload.thePusherCylinder.MoveBackward();
		break;
	}
}

void CSensorViewPage::OnLfKickerSolOn()
{
	theVEliteOffload.m_LfKickerSol.SetRelayState(RELAY_ON);
}

void CSensorViewPage::OnLfKickerSolOff()
{
	theVEliteOffload.m_LfKickerSol.SetRelayState(RELAY_OFF);
}

void CSensorViewPage::OnMagKickerOn()
{
	theVEliteOffload.m_MagKicker.TriggerForwardMotion();
}

void CSensorViewPage::OnMagKickerOff()
{
	theVEliteOffload.m_MagKicker.TriggerBacktoHome();
}
