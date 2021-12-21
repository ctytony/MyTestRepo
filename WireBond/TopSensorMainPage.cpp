/*****************************************************************
TopSensorMainPage.cpp : implementation file
*****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "TopSensorMainPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TopClamp.h"
#include "R2RIndex.h"

// CTopSensorMainPage dialog
IMPLEMENT_DYNAMIC(CTopSensorMainPage, CPropertyPage)

CTopSensorMainPage::CTopSensorMainPage()
	: CPropertyPage(CTopSensorMainPage::IDD)
	, m_iSensorId(0)
{
}

CTopSensorMainPage::~CTopSensorMainPage()
{
}

void CTopSensorMainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_LBIndex(pDX, IDC_LIST_SENSOR, m_iSensorId);
}

BEGIN_MESSAGE_MAP(CTopSensorMainPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_SENSOR, OnListSensor)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_CLAW_ON, OnLeftClawOn)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_CLAW_OFF, OnLeftClawOff)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_CLAW_ON, OnRightClawOn)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_CLAW_OFF, OnRightClawOff)
	ON_BN_CLICKED(IDC_BUTTON_EJECTOR_ON, OnEjectorOn)
	ON_BN_CLICKED(IDC_BUTTON_EJECTOR_OFF, OnEjectorOff)
	ON_BN_CLICKED(IDC_BUTTON_INJECTOR_ON, OnInjectorOn)
	ON_BN_CLICKED(IDC_BUTTON_INJECTOR_OFF, OnInjectorOff)
    ON_BN_CLICKED(IDC_BUTTON_LF_HOLDER_ON, OnLfHolderOn) //@2
    ON_BN_CLICKED(IDC_BUTTON_LF_HOLDER_OFF, OnLfHolderOff)
	ON_BN_CLICKED(IDC_BUTTON_VACUUM_ON_TOP, OnVacuumOnTop)
	ON_BN_CLICKED(IDC_BUTTON_VACUUM_OFF_TOP, OnVacuumOffTop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTopSensorMainPage message handlers
BOOL CTopSensorMainPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_STATIC_SENSOR,theMsg.GetMsg(MSG_SENSOR));
    SetDlgItemTextW(IDC_STATIC_CYLINDER,theMsg.GetMsg(MSG_CYLINDER));
    SetDlgItemTextW(IDC_STATIC_LF_HOLDER,theMsg.GetMsg(MSG_LF_HOLDER));
    SetDlgItemTextW(IDC_BUTTON_LF_HOLDER_ON,theMsg.GetMsg(MSG_ON));
    SetDlgItemTextW(IDC_BUTTON_LF_HOLDER_OFF,theMsg.GetMsg(MSG_OFF));
    SetDlgItemTextW(IDC_STATIC_PULLER,theMsg.GetMsg(MSG_PULLER));
    SetDlgItemTextW(IDC_BUTTON_INJECTOR_ON,theMsg.GetMsg(MSG_ON));
    SetDlgItemTextW(IDC_BUTTON_INJECTOR_OFF,theMsg.GetMsg(MSG_OFF));
    SetDlgItemTextW(IDC_STATIC_LEFT_CLAW,theMsg.GetMsg(MSG_LEFT_CLAW_COLON));
    SetDlgItemTextW(IDC_BUTTON_LEFT_CLAW_ON,theMsg.GetMsg(MSG_ON));
    SetDlgItemTextW(IDC_BUTTON_LEFT_CLAW_OFF,theMsg.GetMsg(MSG_OFF));
    SetDlgItemTextW(IDC_STATIC_RIGHT_CLAW,theMsg.GetMsg(MSG_RIGHT_CLAW_COLON));
    SetDlgItemTextW(IDC_BUTTON_RIGHT_CLAW_ON,theMsg.GetMsg(MSG_ON));
    SetDlgItemTextW(IDC_BUTTON_RIGHT_CLAW_OFF,theMsg.GetMsg(MSG_OFF));
    SetDlgItemTextW(IDC_STATIC_KICKER,theMsg.GetMsg(MSG_KICKER));
    SetDlgItemTextW(IDC_BUTTON_EJECTOR_ON,theMsg.GetMsg(MSG_ON));
    SetDlgItemTextW(IDC_BUTTON_EJECTOR_OFF,theMsg.GetMsg(MSG_OFF));
    SetDlgItemTextW(IDC_STATIC_VACUUM,theMsg.GetMsg(MSG_VACUUM));
    SetDlgItemTextW(IDC_BUTTON_VACUUM_ON_TOP,theMsg.GetMsg(MSG_ON));
    SetDlgItemTextW(IDC_BUTTON_VACUUM_OFF_TOP,theMsg.GetMsg(MSG_OFF));

	TOP_TRACK_DATA stTrackData;
	theTopTrack.GetRcd(&stTrackData);

	if (stTrackData.bVacuumControl == false)
	{
		GetDlgItem(IDC_BUTTON_VACUUM_ON_TOP)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_VACUUM_OFF_TOP)->EnableWindow(false);
	}

    m_iSensorId = 0;

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_SENSOR);
	pListBox->AddString(theMsg.GetMsg(MSG_ONLOAD)+_T("/")+theMsg.GetMsg(MSG_OFFLOAD));
	pListBox->AddString(theMsg.GetMsg(MSG_MATERIAL_HANDLER));

    m_pTopFrontView = new CSnsrTopFrontLayout();
	m_pTopFrontView->Create(CSnsrTopFrontLayout::IDD,this);

	m_pTopOnOffLoaderView = new CSnsrTopOnOffLoaderLayout();
	m_pTopOnOffLoaderView->Create(CSnsrTopOnOffLoaderLayout::IDD,this);

	return CPropertyPage::OnInitDialog();
}

HBRUSH CTopSensorMainPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTopSensorMainPage::OnListSensor()
{
    UpdateData(true);

	if (m_iSensorId == 0)
		m_pTopOnOffLoaderView->SetForegroundWindow();
	else if (m_iSensorId == 1)
		m_pTopFrontView->SetForegroundWindow();
}

void CTopSensorMainPage::OnLeftClawOn()
{
	theTopTrack.m_InputClaw.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF)->EnableWindow(TRUE);
}

void CTopSensorMainPage::OnLeftClawOff()
{
	theTopTrack.m_InputClaw.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF)->EnableWindow(FALSE);
}

void CTopSensorMainPage::OnRightClawOn()
{
	theTopTrack.m_OutputClaw.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF)->EnableWindow(TRUE);
}

void CTopSensorMainPage::OnRightClawOff()
{
	theTopTrack.m_OutputClaw.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF)->EnableWindow(FALSE);
}

void CTopSensorMainPage::OnEjectorOn()
{
	theTopTrack.m_Ejector.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_EJECTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EJECTOR_OFF)->EnableWindow(TRUE);
}

void CTopSensorMainPage::OnEjectorOff()
{
	theTopTrack.m_Ejector.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_EJECTOR_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_EJECTOR_OFF)->EnableWindow(FALSE);
}

void CTopSensorMainPage::OnInjectorOn()
{
	theTopTrack.m_Injector.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_INJECTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_INJECTOR_OFF)->EnableWindow(TRUE);
}

void CTopSensorMainPage::OnInjectorOff()
{
	theTopTrack.m_Injector.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_INJECTOR_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_INJECTOR_OFF)->EnableWindow(FALSE);
}

void CTopSensorMainPage::OnDestroy()
{
	m_pTopOnOffLoaderView->DestroyWindow();
	delete m_pTopOnOffLoaderView;

	m_pTopFrontView->DestroyWindow();
	delete m_pTopFrontView;

	theTopTrack.m_Injector.SetRelayState(RELAY_OFF);
	theTopTrack.m_InputClaw.SetRelayState(RELAY_OFF);
	theTopTrack.m_OutputClaw.SetRelayState(RELAY_OFF);
	theTopTrack.m_Ejector.SetRelayState(RELAY_OFF);

	CPropertyPage::OnDestroy();
}

void CTopSensorMainPage::OnLfHolderOn()
{
    theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_LF_HOLDER_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF)->EnableWindow(TRUE);
}

void CTopSensorMainPage::OnLfHolderOff()
{
    theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LF_HOLDER_ON)->EnableWindow(TRUE);
}

void CTopSensorMainPage::EnableButtons(bool bEnable)
{
    GetDlgItem(IDC_STATIC_LF_HOLDER)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LF_HOLDER_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_PULLER)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_INJECTOR_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_INJECTOR_OFF)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_LEFT_CLAW)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_RIGHT_CLAW)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_KICKER)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_EJECTOR_ON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_EJECTOR_OFF)->EnableWindow(bEnable);
}

void CTopSensorMainPage::OnVacuumOnTop()
{
	theTopClamp.SetVacuumOn(true);	//v3.0.T378 edit

	GetDlgItem(IDC_BUTTON_VACUUM_ON_TOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VACUUM_OFF_TOP)->EnableWindow(TRUE);
}

void CTopSensorMainPage::OnVacuumOffTop()
{
	theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit

	GetDlgItem(IDC_BUTTON_VACUUM_ON_TOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_VACUUM_OFF_TOP)->EnableWindow(FALSE);
}
