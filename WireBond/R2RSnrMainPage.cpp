#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "R2RSnrMainPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TopClamp.h"
#include "R2RIndex.h"

// CR2RSnrMainPage dialog

IMPLEMENT_DYNAMIC(CR2RSnrMainPage, CPropertyPage)

CR2RSnrMainPage::CR2RSnrMainPage()
	: CPropertyPage(CR2RSnrMainPage::IDD)
	, m_iSensorId(0)
{

}

CR2RSnrMainPage::~CR2RSnrMainPage()
{
}

void CR2RSnrMainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_LBIndex(pDX, IDC_LIST_SENSOR, m_iSensorId);
}


BEGIN_MESSAGE_MAP(CR2RSnrMainPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_SENSOR, &CR2RSnrMainPage::OnLbnSelchangeListSensor)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_CLAW_ON, &CR2RSnrMainPage::OnBnClickedButtonLeftClawOn)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_CLAW_OFF, &CR2RSnrMainPage::OnBnClickedButtonLeftClawOff)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_CLAW_ON, &CR2RSnrMainPage::OnBnClickedButtonRightClawOn)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_CLAW_OFF, &CR2RSnrMainPage::OnBnClickedButtonRightClawOff)
	ON_BN_CLICKED(IDC_BUTTON_EJECTOR_ON, &CR2RSnrMainPage::OnBnClickedButtonEjectorOn)
	ON_BN_CLICKED(IDC_BUTTON_EJECTOR_OFF, &CR2RSnrMainPage::OnBnClickedButtonEjectorOff)
	ON_BN_CLICKED(IDC_BUTTON_INJECTOR_ON, &CR2RSnrMainPage::OnBnClickedButtonInjectorOn)
	ON_BN_CLICKED(IDC_BUTTON_INJECTOR_OFF, &CR2RSnrMainPage::OnBnClickedButtonInjectorOff)
    ON_BN_CLICKED(IDC_BUTTON_LF_HOLDER_ON, &CR2RSnrMainPage::OnBnClickedButtonLfHolderOn) //@2
    ON_BN_CLICKED(IDC_BUTTON_LF_HOLDER_OFF, &CR2RSnrMainPage::OnBnClickedButtonLfHolderOff)
	ON_BN_CLICKED(IDC_BUTTON_VACUUM_ON_TOP, &CR2RSnrMainPage::OnBnClickedButtonVacuumOnTop)
	ON_BN_CLICKED(IDC_BUTTON_VACUUM_OFF_TOP, &CR2RSnrMainPage::OnBnClickedButtonVacuumOffTop)
END_MESSAGE_MAP()

// CR2RSnrMainPage message handlers
BOOL CR2RSnrMainPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SENSOR);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SENSOR));

	pWnd = GetDlgItem(IDC_STATIC_CYLINDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CYLINDER));

    int iRowTop = 180;

    pWnd = GetDlgItem(IDC_STATIC_LF_HOLDER); //@2
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop+5, PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
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
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PULLER));

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
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KICKER));

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

	pWnd = GetDlgItem(IDC_BUTTON_VACUUM_ON_TOP);
    pWnd->SetWindowPos(NULL, PARA_COLUMN1_LEFT-20, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ON));
	TOP_TRACK_DATA stTrackRcd;
	theR2RTrack.GetTopTrackRcd(&stTrackRcd);
	if (stTrackRcd.bVacuumControl == false)
		pWnd->EnableWindow(false);

	pWnd = GetDlgItem(IDC_BUTTON_VACUUM_OFF_TOP);
    pWnd->SetWindowPos(NULL, PARA_COLUMN2_LEFT-10, iRowTop, TREE_BUTTON_WIDTH - 5, TREE_BUTTON_HEIGHT - 5, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFF));
	if (stTrackRcd.bVacuumControl == false)
		pWnd->EnableWindow(false);

    m_iSensorId = 0;
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_SENSOR);
	pListBox->ResetContent();
	pListBox->InsertString(0, theMsg.GetMsg(MSG_MATERIAL_HANDLER));

    theR2RFrontView = new CSnsrR2RFrontLayout();
	theR2RFrontView->Create(CSnsrR2RFrontLayout::IDD,this);

   // EnableButtons(false);
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CR2RSnrMainPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	// TODO:  Change any attributes of the DC here
	   HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CR2RSnrMainPage::OnLbnSelchangeListSensor()
{
	// TODO: Add your control notification handler code here
    UpdateData(true);

	if (m_iSensorId == 0)
	{
		theR2RFrontView->SetForegroundWindow();
	}
}

//void CR2RSnrMainPage::OnBnClickedButtonOnloadAirOn()
//{
//	// TODO: Add your control notification handler code here
//	//pOnload->theMagClampCylinder.TriggerForwardMotion();
//}
//
//void CR2RSnrMainPage::OnBnClickedButtonOnloadAirOff()
//{
//	// TODO: Add your control notification handler code here
//	//pOnload->theMagClampCylinder.TriggerBackwardMotion();
//}
//
//void CR2RSnrMainPage::OnBnClickedButtonOffloadAirOn()
//{
//	// TODO: Add your control notification handler code here
//	//pOffload->theMagClampCylinder.TriggerForwardMotion();
//
//}
//
//void CR2RSnrMainPage::OnBnClickedButtonOffloadAirOff()
//{
//	// TODO: Add your control notification handler code here
//	//pOffload->theMagClampCylinder.TriggerBackwardMotion();
//
//}

void CR2RSnrMainPage::OnBnClickedButtonLeftClawOn()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.theInputClaw.SetRelayState(RELAY_ON);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON);
	pWnd->EnableWindow(FALSE);
	pWnd =GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF);
	pWnd->EnableWindow(TRUE);

}

void CR2RSnrMainPage::OnBnClickedButtonLeftClawOff()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.theInputClaw.SetRelayState(RELAY_OFF);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON);
	pWnd->EnableWindow(TRUE);
	pWnd =GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF);
	pWnd->EnableWindow(FALSE);

}

void CR2RSnrMainPage::OnBnClickedButtonRightClawOn()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.theOutputClaw.SetRelayState(RELAY_ON);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON);
	pWnd->EnableWindow(FALSE);
	pWnd =GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF);
	pWnd->EnableWindow(TRUE);

}

void CR2RSnrMainPage::OnBnClickedButtonRightClawOff()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.theOutputClaw.SetRelayState(RELAY_OFF);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON);
	pWnd->EnableWindow(TRUE);
	pWnd =GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF);
	pWnd->EnableWindow(FALSE);

}

void CR2RSnrMainPage::OnBnClickedButtonEjectorOn()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.theEjector.SetRelayState(RELAY_ON);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_EJECTOR_ON);
	pWnd->EnableWindow(FALSE);
	pWnd =GetDlgItem(IDC_BUTTON_EJECTOR_OFF);
	pWnd->EnableWindow(TRUE);

}

void CR2RSnrMainPage::OnBnClickedButtonEjectorOff()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.theEjector.SetRelayState(RELAY_OFF);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_EJECTOR_ON);
	pWnd->EnableWindow(TRUE);
	pWnd =GetDlgItem(IDC_BUTTON_EJECTOR_OFF);
	pWnd->EnableWindow(FALSE);

}


void CR2RSnrMainPage::OnBnClickedButtonInjectorOn()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.theInjector.SetRelayState(RELAY_ON);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_INJECTOR_ON);
	pWnd->EnableWindow(FALSE);
	pWnd =GetDlgItem(IDC_BUTTON_INJECTOR_OFF);
	pWnd->EnableWindow(TRUE);
}

void CR2RSnrMainPage::OnBnClickedButtonInjectorOff()
{
	// TODO: Add your control notification handler code here
		theR2RTrack.theInjector.SetRelayState(RELAY_OFF);

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_INJECTOR_ON);
	pWnd->EnableWindow(TRUE);
	pWnd =GetDlgItem(IDC_BUTTON_INJECTOR_OFF);
	pWnd->EnableWindow(FALSE);
}


void CR2RSnrMainPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	
	theR2RFrontView->DestroyWindow();
	delete theR2RFrontView;


	theR2RTrack.theInjector.SetRelayState(RELAY_OFF);
	theR2RTrack.theInputClaw.SetRelayState(RELAY_OFF);
	theR2RTrack.theOutputClaw.SetRelayState(RELAY_OFF);
	theR2RTrack.theEjector.SetRelayState(RELAY_OFF);


	CPropertyPage::OnOK();
}

void CR2RSnrMainPage::OnBnClickedButtonLfHolderOn()
{
    // TODO: Add your control notification handler code here
    theR2RTrack.theLFHolder.SetRelayState(RELAY_ON);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_ON);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF);
	pWnd->EnableWindow(TRUE);
}

void CR2RSnrMainPage::OnBnClickedButtonLfHolderOff()
{
    // TODO: Add your control notification handler code here
    theR2RTrack.theLFHolder.SetRelayState(RELAY_OFF);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_ON);
	pWnd->EnableWindow(TRUE);
}

void CR2RSnrMainPage::EnableButtons(bool bEnable)
{
    CWnd* pWnd = GetDlgItem(IDC_STATIC_LF_HOLDER);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_ON);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_LF_HOLDER_OFF);
    pWnd->EnableWindow(bEnable);

    pWnd = GetDlgItem(IDC_STATIC_PULLER);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_INJECTOR_ON);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_INJECTOR_OFF);
    pWnd->EnableWindow(bEnable);

    pWnd = GetDlgItem(IDC_STATIC_LEFT_CLAW);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_LEFT_CLAW_ON);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_LEFT_CLAW_OFF);
    pWnd->EnableWindow(bEnable);

    pWnd = GetDlgItem(IDC_STATIC_RIGHT_CLAW);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_RIGHT_CLAW_ON);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_RIGHT_CLAW_OFF);
    pWnd->EnableWindow(bEnable);

    pWnd = GetDlgItem(IDC_STATIC_KICKER);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_EJECTOR_ON);
    pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_BUTTON_EJECTOR_OFF);
    pWnd->EnableWindow(bEnable);
}

void CR2RSnrMainPage::OnBnClickedButtonVacuumOnTop()
{
	theTopClamp.SetVacuumOn(true);	//v3.0.T378 edit

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_VACUUM_ON_TOP); 
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_VACUUM_OFF_TOP);       
	pWnd->EnableWindow(TRUE);
}

void CR2RSnrMainPage::OnBnClickedButtonVacuumOffTop()
{
	theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_VACUUM_ON_TOP);  
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_BUTTON_VACUUM_OFF_TOP);         
	pWnd->EnableWindow(FALSE);
}
