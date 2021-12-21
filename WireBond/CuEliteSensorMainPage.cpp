/*****************************************************************
TopSensorMainPage.cpp : implementation file
*****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "CuEliteSensorMainPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "CuIndex.h"
#include "CuClamp.h"

// CCuEliteSensorMainPage dialog

IMPLEMENT_DYNAMIC(CCuEliteSensorMainPage, CPropertyPage)

CCuEliteSensorMainPage::CCuEliteSensorMainPage()
	: CPropertyPage(CCuEliteSensorMainPage::IDD)
	, m_iSensorId(0)
{
}

CCuEliteSensorMainPage::~CCuEliteSensorMainPage()
{
}

void CCuEliteSensorMainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_LBIndex(pDX, IDC_LIST_SENSOR, m_iSensorId);
}

BEGIN_MESSAGE_MAP(CCuEliteSensorMainPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_SENSOR, &CCuEliteSensorMainPage::OnLbnSelchangeListSensor)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_CLAW_ON, &CCuEliteSensorMainPage::OnBnClickedButtonLeftClawOn)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_CLAW_OFF, &CCuEliteSensorMainPage::OnBnClickedButtonLeftClawOff)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_CLAW_ON, &CCuEliteSensorMainPage::OnBnClickedButtonRightClawOn)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_CLAW_OFF, &CCuEliteSensorMainPage::OnBnClickedButtonRightClawOff)
	ON_BN_CLICKED(IDC_BUTTON_EJECTOR_ON, &CCuEliteSensorMainPage::OnBnClickedButtonEjectorOn)
	ON_BN_CLICKED(IDC_BUTTON_EJECTOR_OFF, &CCuEliteSensorMainPage::OnBnClickedButtonEjectorOff)
	ON_BN_CLICKED(IDC_BUTTON_INJECTOR_ON, &CCuEliteSensorMainPage::OnBnClickedButtonInjectorOn)
	ON_BN_CLICKED(IDC_BUTTON_INJECTOR_OFF, &CCuEliteSensorMainPage::OnBnClickedButtonInjectorOff)
    ON_BN_CLICKED(IDC_BUTTON_VACUUM_ON, OnVacuumOn)
    ON_BN_CLICKED(IDC_BUTTON_VACUUM_OFF, OnVacuumOff)
    ON_BN_CLICKED(IDC_BUTTON_LF_HOLDER_ON, &CCuEliteSensorMainPage::OnBnClickedButtonLfHolderOn) //@2
    ON_BN_CLICKED(IDC_BUTTON_LF_HOLDER_OFF, &CCuEliteSensorMainPage::OnBnClickedButtonLfHolderOff)
END_MESSAGE_MAP()

// CCuEliteSensorMainPage message handlers
BOOL CCuEliteSensorMainPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SENSOR);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SENSOR));

	pWnd = GetDlgItem(IDC_STATIC_CYLINDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CYLINDER));

    int iRowTop = 200;

  /*  iRowTop += TREE_BUTTON_HEIGHT;*/

    pWnd = GetDlgItem(IDC_STATIC_LF_HOLDER); //@2
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop+5, PARA_LABEL_WIDTH-20, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LF_HOLDER));

	pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_ON);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT-20, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ON));

	pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT-10, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFF));

    iRowTop += TREE_BUTTON_HEIGHT;

    pWnd = GetDlgItem(IDC_STATIC_PULLER);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop+5, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INJECTOR));

	pWnd = GetDlgItem(IDC_BUTTON_INJECTOR_ON);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT-20, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ON));

	pWnd = GetDlgItem(IDC_BUTTON_INJECTOR_OFF);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT-10, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFF));

    iRowTop += TREE_BUTTON_HEIGHT;

    pWnd = GetDlgItem(IDC_STATIC_LEFT_CLAW);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop+5, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LEFT_CLAW_COLON));

	pWnd = GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT-20, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ON));

	pWnd = GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT-10, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFF));

    iRowTop += TREE_BUTTON_HEIGHT;

    pWnd = GetDlgItem(IDC_STATIC_RIGHT_CLAW);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop+5, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RIGHT_CLAW_COLON));

	pWnd = GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT-20, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ON));

	pWnd = GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT-10, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFF));

    iRowTop += TREE_BUTTON_HEIGHT;

    pWnd = GetDlgItem(IDC_STATIC_KICKER);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop+5, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_EJECTOR));

	pWnd = GetDlgItem(IDC_BUTTON_EJECTOR_ON);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT-20, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ON));

	pWnd = GetDlgItem(IDC_BUTTON_EJECTOR_OFF);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT-10, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFF));

    iRowTop += TREE_BUTTON_HEIGHT;

    pWnd = GetDlgItem(IDC_STATIC_VACUUM);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop+5, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_VACUUM));

	pWnd = GetDlgItem(IDC_BUTTON_VACUUM_ON);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT-20, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ON));

	pWnd = GetDlgItem(IDC_BUTTON_VACUUM_OFF);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT-10, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFF));

    m_iSensorId = 0;
	pWnd = GetDlgItem(IDC_LIST_SENSOR);
	((CListBox*)pWnd)->ResetContent();
	((CListBox*)pWnd)->InsertString(0, theMsg.GetMsg(MSG_ONLOAD)
							+ _T("/") + theMsg.GetMsg(MSG_OFFLOAD));
	((CListBox*)pWnd)->InsertString(1, theMsg.GetMsg(MSG_TRACK_FRONT));

	return FALSE;
}

BOOL CCuEliteSensorMainPage::OnSetActive( )
{
	if(CPropertyPage::OnSetActive()==FALSE)return TRUE;

	theCuEliteFrontView = new CSnsrCuELiteFrontLayout();
	theCuEliteFrontView->Create(CSnsrCuELiteFrontLayout::IDD,this);

	theCuEliteOnOffLoaderView = new CSnsrCuEliteOnOffLoaderLayout();
	theCuEliteOnOffLoaderView->Create(CSnsrCuEliteOnOffLoaderLayout::IDD,this);

	// UI update output ejector
	BYTE bEnable=theCuEliteTrack.theEjector.GetRelayState() == RELAY_ON;
	GetDlgItem(IDC_BUTTON_EJECTOR_ON)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_EJECTOR_OFF)->EnableWindow(bEnable);

	 // UI update input injector
	bEnable=theCuEliteTrack.theInjector.GetRelayState() == RELAY_ON;
	GetDlgItem(IDC_BUTTON_INJECTOR_ON)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_INJECTOR_OFF)->EnableWindow(bEnable);

	 // UI update input claw
	bEnable=theCuEliteTrack.theInputClaw.GetRelayState() == RELAY_ON;
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF)->EnableWindow(bEnable);

	 // UI update output claw
	bEnable=theCuEliteTrack.theOutputClaw.GetRelayState() == RELAY_ON;
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF)->EnableWindow(bEnable);

	 // UI update output claw
	bEnable=theCuEliteTrack.theOutputClaw.GetRelayState() == RELAY_ON;
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF)->EnableWindow(bEnable);

	 // UI update slip holder
	bEnable=theCuEliteTrack.theLFHolder.GetRelayState() == RELAY_ON;
	GetDlgItem(IDC_BUTTON_LF_HOLDER_ON)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF)->EnableWindow(bEnable);

	CUELITE_TRACK_DATA* lpstTrackRcd=new CUELITE_TRACK_DATA;
	theCuEliteTrack.GetCuEliteTrackRcd(lpstTrackRcd);
	bEnable=lpstTrackRcd->bVacuumControl;
	delete lpstTrackRcd;

	if(bEnable==TRUE)bEnable=theCuEliteLFClamp.m_VacuumValve.GetRelayState() == RELAY_ON;

	// UI disable
	GetDlgItem(IDC_BUTTON_VACUUM_ON)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BUTTON_VACUUM_OFF)->EnableWindow(bEnable);

	return TRUE;
}

HBRUSH CCuEliteSensorMainPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCuEliteSensorMainPage::OnLbnSelchangeListSensor()
{
    UpdateData(true);

	if(m_iSensorId==0)theCuEliteOnOffLoaderView->SetForegroundWindow();
	else if(m_iSensorId==1)theCuEliteFrontView->SetForegroundWindow();
}

void CCuEliteSensorMainPage::OnBnClickedButtonLeftClawOn()
{
	theCuEliteTrack.theInputClaw.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF)->EnableWindow(TRUE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonLeftClawOff()
{
	theCuEliteTrack.theInputClaw.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF)->EnableWindow(FALSE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonRightClawOn()
{
	theCuEliteTrack.theOutputClaw.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF)->EnableWindow(TRUE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonRightClawOff()
{
	theCuEliteTrack.theOutputClaw.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF)->EnableWindow(FALSE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonEjectorOn()
{
	theCuEliteTrack.theEjector.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_EJECTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EJECTOR_OFF)->EnableWindow(TRUE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonEjectorOff()
{
	theCuEliteTrack.theEjector.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_EJECTOR_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_EJECTOR_OFF)->EnableWindow(FALSE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonInjectorOn()
{
	theCuEliteTrack.theInjector.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_INJECTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_INJECTOR_OFF)->EnableWindow(TRUE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonInjectorOff()
{
	theCuEliteTrack.theInjector.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_INJECTOR_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_INJECTOR_OFF)->EnableWindow(FALSE);
}

void CCuEliteSensorMainPage::OnVacuumOn()
{
    theCuEliteLFClamp.m_VacuumValve.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_VACUUM_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VACUUM_OFF)->EnableWindow(TRUE);
}

void CCuEliteSensorMainPage::OnVacuumOff()
{
    theCuEliteLFClamp.m_VacuumValve.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_VACUUM_ON)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_VACUUM_OFF)->EnableWindow(FALSE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonLfHolderOn()
{
    theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);

	GetDlgItem(IDC_BUTTON_LF_HOLDER_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF)->EnableWindow(TRUE);
}

void CCuEliteSensorMainPage::OnBnClickedButtonLfHolderOff()
{
    theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LF_HOLDER_ON)->EnableWindow(TRUE);
}

BOOL CCuEliteSensorMainPage::OnKillActive()
{
	if( CPropertyPage::OnKillActive()==FALSE)return FALSE;

	theCuEliteOnOffLoaderView->DestroyWindow();
	delete theCuEliteOnOffLoaderView;

	theCuEliteFrontView->DestroyWindow();
	delete theCuEliteFrontView;

	theCuEliteTrack.theInjector.SetRelayState(RELAY_OFF);
	theCuEliteTrack.theInputClaw.SetRelayState(RELAY_OFF);
	theCuEliteTrack.theOutputClaw.SetRelayState(RELAY_OFF);
	theCuEliteTrack.theEjector.SetRelayState(RELAY_OFF);

	return TRUE;
}
