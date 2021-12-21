// R2RTestingPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "R2RTestingPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "MachineConfig.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "R2ROnload.h"
#include "R2ROffload.h"
#include "servo.h"

// CR2RTestingPage dialog

IMPLEMENT_DYNAMIC(CR2RTestingPage, CPropertyPage)

CR2RTestingPage::CR2RTestingPage()
	: CPropertyPage(CR2RTestingPage::IDD)
	, m_iDryCycleSpeedId(1)
	, m_iSpeedProfileNum(0)
	, m_lStartVel(0)
	, m_lDriveVel(0)
	, m_lMaxVel(0)
	, m_lAcc(0)
	, m_lJerk(0)
	, m_lJeckDec(0)
	, m_pWinThreadOnloader(NULL)
	, m_pWinThreadOffloader(NULL)
{

}

CR2RTestingPage::~CR2RTestingPage()
{
}

void CR2RTestingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_DRY_RUN_CLAW, m_iDryCycleSpeedId);
	DDX_CBIndex(pDX, IDC_COMBO_SPEED_PROFILE, m_iSpeedProfileNum);

	DDX_Text(pDX, IDC_EDIT_START_VEL, m_lStartVel);
	DDX_Text(pDX, IDC_EDIT_VEL, m_lDriveVel);
	DDX_Text(pDX, IDC_EDIT_MAX_VEL, m_lMaxVel);
	DDX_Text(pDX, IDD_EDIT_ACC_, m_lAcc);
	DDX_Text(pDX, IDC_EDIT_JERK_, m_lJerk);
	DDX_Text(pDX, IDC_EDIT_JERK_DEC, m_lJeckDec);
}


BEGIN_MESSAGE_MAP(CR2RTestingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_BUTTON_DRY_RUN, &CR2RTestingPage::OnBnClickedButtonDryRun)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_RESET, &CR2RTestingPage::OnBnClickedButtonEventReset)
//	ON_BN_CLICKED(IDC_BUTTON_STOP, &CR2RTestingPage::OnBnClickedButtonStop)
//	ON_BN_CLICKED(IDC_CHECK_ONLOAD, &CR2RTestingPage::OnBnClickedCheckOnload)
//	ON_BN_CLICKED(IDC_CHECK_TRACK, &CR2RTestingPage::OnBnClickedCheckTrack)
//	ON_BN_CLICKED(IDC_CHECK_OFFLOAD, &CR2RTestingPage::OnBnClickedCheckOffload)
	ON_CBN_SELCHANGE(IDC_COMBO_DRY_RUN_CLAW, &CR2RTestingPage::OnCbnSelchangeComboDryRunClaw)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_DRY_RUN, &CR2RTestingPage::OnBnClickedButtonClawDryRun)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_STOP, &CR2RTestingPage::OnBnClickedButtonClawStop)
	ON_BN_CLICKED(IDC_BUTTON_START_SPEED_TEST, &CR2RTestingPage::OnBnClickedButtonStartSpeedTest)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_PROFILE, &CR2RTestingPage::OnCbnSelchangeComboSpeedProfile)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SPEED_TEST, &CR2RTestingPage::OnBnClickedButtonStopSpeedTest)
//	ON_BN_CLICKED(IDC_CHECK_WITHOUT_LF, &CR2RTestingPage::OnBnClickedCheckWithoutLf)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_DISABLE, &CR2RTestingPage::OnBnClickedButtonEventDisable)

	ON_EN_SETFOCUS(IDC_EDIT_START_VEL, &CR2RTestingPage::OnEnSetfocusEditStartVel)
	ON_EN_SETFOCUS(IDC_EDIT_VEL, &CR2RTestingPage::OnEnSetfocusEditVel)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_VEL, &CR2RTestingPage::OnEnSetfocusEditMaxVel)
	ON_EN_SETFOCUS(IDD_EDIT_ACC_, &CR2RTestingPage::OnEnSetfocusEditAcc)
	ON_EN_SETFOCUS(IDC_EDIT_JERK_, &CR2RTestingPage::OnEnSetfocusEditJerk)
	ON_EN_SETFOCUS(IDC_EDIT_JERK_DEC, &CR2RTestingPage::OnEnSetfocusEditJerkDec)
    ON_BN_CLICKED(IDC_BUTTON_ENABLE, &CR2RTestingPage::OnBnClickedButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_START_ONLOADER_TEST, &CR2RTestingPage::OnBnClickedButtonStartOnloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_START_OFFLOADER_TEST, &CR2RTestingPage::OnBnClickedButtonStartOffloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_ONLOADER_TEST, &CR2RTestingPage::OnBnClickedButtonStopOnloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_OFFLOADER_TEST, &CR2RTestingPage::OnBnClickedButtonStopOffloaderTest)
END_MESSAGE_MAP()

// CR2RTestingPage message handlers
BOOL CR2RTestingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    bool bStepperOn = false;
    int  iStatus = FOK;

    iStatus = theStepper.CheckPowerStatus(&bStepperOn);
    if (iStatus != FOK)
        bStepperOn = false;

    CWnd*pWnd = GetDlgItem(IDC_BUTTON_EVENT_RESET);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INITIALIZE));
    if (bStepperOn == true)
        pWnd->EnableWindow(TRUE);
    else
        pWnd->EnableWindow(FALSE);

    pWnd = GetDlgItem(IDC_BUTTON_EVENT_DISABLE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DISABLE_2));
    if (bStepperOn == true)
        pWnd->EnableWindow(TRUE);
    else
        pWnd->EnableWindow(FALSE);

    pWnd = GetDlgItem(IDC_BUTTON_ENABLE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ENABLE_2));
    if (bStepperOn == true)
        pWnd->EnableWindow(FALSE);
    else
        pWnd->EnableWindow(TRUE);

    pWnd = GetDlgItem(IDC_STATIC_DRY_RUN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STEPPER));

    pWnd = GetDlgItem(IDC_STATIC_SPEED_TEST);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEXER));

    pWnd = GetDlgItem(IDC_BUTTON_START_SPEED_TEST);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));

    pWnd = GetDlgItem(IDC_BUTTON_STOP_SPEED_TEST);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

    pWnd = GetDlgItem(IDC_STATIC_SOLINOID);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SOLENOID_TEST));

    pWnd = GetDlgItem(IDC_BUTTON_CLAW_DRY_RUN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DRY_RUN));

    pWnd = GetDlgItem(IDC_BUTTON_CLAW_STOP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

//****************Onloader/Offloader Testing**************************//
	pWnd = GetDlgItem(IDC_STATIC_DRY_ONLOADER);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_INPUT));

	pWnd = GetDlgItem(IDC_STATIC_DRY_OFFLOADER);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_OUTPUT));

	pWnd = GetDlgItem(IDC_BUTTON_START_ONLOADER_TEST);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_START));

	pWnd = GetDlgItem(IDC_BUTTON_STOP_ONLOADER_TEST);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_STOP));

	pWnd = GetDlgItem(IDC_BUTTON_START_OFFLOADER_TEST);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_START));

	pWnd = GetDlgItem(IDC_BUTTON_STOP_OFFLOADER_TEST);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_STOP));

//*********Gripper Dry Cycle Testing************************************

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DRY_RUN_CLAW);
    pComboBox->InsertString(0, theMsg.GetMsg(MSG_SLOW));
    pComboBox->InsertString(1, theMsg.GetMsg(MSG_MEDIUM));
    pComboBox->InsertString(2, theMsg.GetMsg(MSG_FAST));

    m_iDryCycleSpeedId = 1;
    theR2RTrack.m_lClawInterval = 800;  //ms

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED_PROFILE);

    pComboBox->InsertString(0, _T("0"));
    pComboBox->InsertString(1, _T("1"));
    pComboBox->InsertString(2, _T("2"));
    pComboBox->InsertString(3, _T("3"));
    pComboBox->InsertString(4, _T("4"));
    pComboBox->InsertString(5, _T("5"));
    pComboBox->InsertString(6, _T("6"));

    m_iSpeedProfileNum = 0;

    STEP_SPEED_PROFILE stIndexerSpeedProfile;
    theR2RTrack.GetTopIndexerStepperProfile(0, &stIndexerSpeedProfile);

    m_lStartVel =stIndexerSpeedProfile.lStartVelocity;
    m_lDriveVel =stIndexerSpeedProfile.lDriveVelocity;
    m_lMaxVel   =stIndexerSpeedProfile.lMaxDriveVelocity;
    m_lAcc      =stIndexerSpeedProfile.lAcc;

    if(theMcConfig.GetStepperType()==STEP_TYPE_1240)
    {
        m_lJerk=stIndexerSpeedProfile.lAccRatio;
    }
    else
    {
        m_lJerk = m_lDriveVel/3;
        m_lJeckDec = m_lJerk;
    }

    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CR2RTestingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


UINT R2RDryRun_ThreadProc( LPVOID pParam)
{
	CR2RTestingPage* pObject = (CR2RTestingPage *)pParam;
	return pObject->DryRunProcess(); 	

}

//void CR2RTestingPage::OnBnClickedButtonDryRun()
//{
//	UpdateData(TRUE);
//
//	bStopDryRun =false;
//
//	m_pDryWinThread = AfxBeginThread(R2RDryRun_ThreadProc, (LPVOID)this);
//
//}

int  CR2RTestingPage::DryRunProcess()
{	
   return 0;
}

//*************************Gripper Dry Cycle************************** 
void CR2RTestingPage::OnCbnSelchangeComboDryRunClaw()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	switch(m_iDryCycleSpeedId)
	{
	case 0:

		theR2RTrack.m_lClawInterval = 1200;

		break;
	case 1:

		theR2RTrack.m_lClawInterval= 800;

		break;

	case 2:
		theR2RTrack.m_lClawInterval = 400;

		break;

	default:
		break;

	}


}


UINT R2RGripperDryRun_ThreadProc( LPVOID pParam )
{
	
	do
   {
	   theR2RTrack.ProcessTopGripperDryRun();
	   Sleep(10);
	  	  	   
	}
	while(theR2RTrack.m_nGripperDryCycleState != CLAW_DRY_CYCLE_IDLE);
    

	return 0;
}

void CR2RTestingPage::OnBnClickedButtonClawDryRun()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_CLAW_DRY_RUN)->EnableWindow(FALSE);
	theR2RTrack.SubmitGripperDryRun(0);
	m_pWinThread = AfxBeginThread(R2RGripperDryRun_ThreadProc, (LPVOID)this);
	Sleep(10);
}

void CR2RTestingPage::OnBnClickedButtonClawStop()
{
	// TODO: Add your control notification handler code here
    theR2RTrack.m_nGripperDryCycleState = CLAW_DRY_CYCLE_IDLE;
    Sleep(100);

	theR2RTrack.theLFHolder.SetRelayState(RELAY_OFF);
	theR2RTrack.theInjector.SetRelayState(RELAY_OFF);
    theR2RTrack.theInputClaw.SetRelayState(RELAY_OFF);
    theR2RTrack.theOutputClaw.SetRelayState(RELAY_OFF);
    theR2RTrack.theEjector.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_CLAW_DRY_RUN)->EnableWindow(TRUE);

}

//----------------Speed test--------------------------------------------------


UINT R2RIndexTest_ThreadProc( LPVOID pParam )
{

	do
   {

	   theR2RTrack.ProcessIndexerDryRun();

	   Sleep(10);
	}
	while(theR2RTrack.m_nTestState != INDEX_TEST_STATE_IDLE);
    

	return 0;
}



void CR2RTestingPage::OnBnClickedButtonStartSpeedTest()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	STEP_SPEED_PROFILE stIndexerSpeedProfile;

	theR2RTrack.GetTopIndexerStepperProfile(m_iSpeedProfileNum, &stIndexerSpeedProfile);

	stIndexerSpeedProfile.lAcc =m_lAcc;
	stIndexerSpeedProfile.lAccRatio =m_lJerk;
	stIndexerSpeedProfile.lDriveVelocity =m_lDriveVel;
	stIndexerSpeedProfile.lMaxDriveVelocity =m_lMaxVel;
	stIndexerSpeedProfile.lStartVelocity =m_lStartVel;

	theStepper.SetSpeedParameter(theR2RTrack.m_ucIndexerStepperId, &stIndexerSpeedProfile);

	theR2RTrack.SubmitIndexerDryRunEvent(INDEX_TEST_SPEED_PROFILE);

    m_pWinThreadSpeed = AfxBeginThread(R2RIndexTest_ThreadProc, (LPVOID)this);

	Sleep(10);

	GetDlgItem(IDC_BUTTON_START_SPEED_TEST)->EnableWindow(FALSE);

}

void CR2RTestingPage::OnCbnSelchangeComboSpeedProfile()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	STEP_SPEED_PROFILE stIndexerSpeedProfile;

	theR2RTrack.GetTopIndexerStepperProfile(m_iSpeedProfileNum, &stIndexerSpeedProfile);


	m_lStartVel =stIndexerSpeedProfile.lStartVelocity;
	m_lDriveVel =stIndexerSpeedProfile.lDriveVelocity;
	m_lMaxVel   =stIndexerSpeedProfile.lMaxDriveVelocity;
	m_lAcc      =stIndexerSpeedProfile.lAcc;


	if(theMcConfig.GetStepperType()==STEP_TYPE_1240)
	{
		m_lJerk=stIndexerSpeedProfile.lAccRatio;
		m_lJeckDec=0;

	}
	else
	{
		m_lJerk = m_lDriveVel/3;
		m_lJeckDec = m_lJerk;

	}


	UpdateData(FALSE);
	
}

void CR2RTestingPage::OnBnClickedButtonStopSpeedTest()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.m_nTestState = INDEX_TEST_STATE_IDLE;
	theR2RTrack.m_nTestEvent = INDEX_TEST_EVENT_IDLE;
	GetDlgItem(IDC_BUTTON_START_SPEED_TEST)->EnableWindow(TRUE);

}

void CR2RTestingPage::OnOK()
{
	theR2RTrack.SetAutoRunWithNoLf(FALSE);

	/*theTopOffloader.SetForkOffloadValid(TRUE);
	theTopOnloader.SetOnloadValid(TRUE);*/

	CPropertyPage::OnOK();
}

void CR2RTestingPage::OnEnSetfocusEditStartVel()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAW_STOP);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lStartVel);

	UpdateData(false);
}

void CR2RTestingPage::OnEnSetfocusEditVel()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAW_STOP);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lDriveVel);

	UpdateData(false);
}

void CR2RTestingPage::OnEnSetfocusEditMaxVel()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAW_STOP);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lMaxVel);

	UpdateData(false);
}

void CR2RTestingPage::OnEnSetfocusEditAcc()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAW_STOP);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lAcc);

	UpdateData(false);
}

void CR2RTestingPage::OnEnSetfocusEditJerk()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAW_STOP);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lJerk);

	UpdateData(false);
}

void CR2RTestingPage::OnEnSetfocusEditJerkDec()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAW_STOP);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lJeckDec);

	UpdateData(false);
}

void CR2RTestingPage::OnBnClickedButtonEnable()
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_ENABLE);
    pWnd->EnableWindow(FALSE);

    theStepper.PowerOn();

    Sleep(1000);

    pWnd = GetDlgItem(IDC_BUTTON_EVENT_DISABLE);
    pWnd->EnableWindow(TRUE);

    pWnd = GetDlgItem(IDC_BUTTON_EVENT_RESET);
    pWnd->EnableWindow(TRUE);
}

void CR2RTestingPage::OnBnClickedButtonEventDisable()
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_EVENT_DISABLE);
    pWnd->EnableWindow(FALSE);

	theStepper.StopMotor(theR2ROnloader.m_ucMagClamperStepId);
	theStepper.StopMotor(theR2ROffloader.m_ucMagClamperStepId);
    theStepper.StopMotor(theR2ROnloader.m_ucForkHolderStepId);
	theStepper.StopMotor(theR2ROffloader.m_ucForkHolderStepId);
	theStepper.StopMotor(theR2RTrack.m_ucIndexerStepperId);
	theStepper.StopMotor(theTopClamp.GetStepID());

    theStepper.PowerOff();

    pWnd = GetDlgItem(IDC_BUTTON_ENABLE);
    pWnd->EnableWindow(TRUE);

    pWnd = GetDlgItem(IDC_BUTTON_EVENT_RESET);
    pWnd->EnableWindow(FALSE);
}

void CR2RTestingPage::OnBnClickedButtonEventReset()
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_EVENT_RESET);
    pWnd->EnableWindow(FALSE);

	theServo.MoveBondHeadToSafePos();
	Sleep(10);

	theMHS.SetAutoIndex(false);
	theMHS.SetRequestIndex(false);

	theR2RTrack.ResetTrackErrorCode(1);
	theR2RTrack.ResetTrackErrorCode(0);

	theMHS.SetLastLf(false);

	theR2RTrack.ResetSlipRcd();
	theR2ROnloader.ResetOnloadEvent();
	theR2ROffloader.ResetOffloadEvent();

	theR2RTrack.SearchIndexStepHome();
    Sleep(10);
	theTopClamp.HomeClamp();

	theR2RTrack.WaitStepHomeFinish();
	Sleep(100);
	theTopClamp.WaitHomeClamp();
	Sleep(500);
	theTopClamp.PreCloseClamp();

	theR2RTrack.MoveToLogicZero();
	Sleep(800);

	theR2ROnloader.theMagPullyDcMotor.TriggerBacktoHome();
	theR2ROffloader.theMagPullyDcMotor.TriggerBacktoHome();

	theR2ROnloader.SearchOnloadElvStepHome();
	Sleep(10);
	theR2ROffloader.SearchOffloadElvStepHome();
	Sleep(10);
	theR2ROnloader.SearchOnloadMagClamperStepHome();
	Sleep(10);
	theR2ROffloader.SearchOffloadMagClamperStepHome();
	Sleep(10);
	theR2ROnloader.WaitStepHomeFinish(); //@29
	Sleep(10);
	theR2ROffloader.WaitStepHomeFinish();
	Sleep(10);
	theR2ROnloader.WaitOnloadMagClamperStepHomeFinish();
	Sleep(10);
	theR2ROffloader.WaitOffloadMagClamperStepHomeFinish();
	Sleep(10);
	theR2ROnloader.MoveTotheSlot(1);
	Sleep(10);
	theR2ROffloader.MoveTotheSlot(1); 
	Sleep(100);
	theR2ROnloader.OnLoadMagClamperMovetoClampPosition();
	Sleep(10);
	theR2ROffloader.OffLoadMagClamperMovetoClampPosition();
	Sleep(2300);


    pWnd->EnableWindow(TRUE);
}

//----------------Onloader/Offloader test-----------------------

UINT R2ROnloaderTest_ThreadProc( LPVOID pParam )
{

	do
	{

		theR2ROnloader.ProcessOnloaderDryRunEvent();

		Sleep(10);
	}
	while(theR2ROnloader.m_nTestState != INDEX_TEST_STATE_IDLE);


	return 0;
}

UINT R2ROffloaderTest_ThreadProc( LPVOID pParam )
{

	do
	{

		theR2ROffloader.ProcessOffloaderDryRunEvent();

		Sleep(10);
	}
	while(theR2ROffloader.m_nTestState != INDEX_TEST_STATE_IDLE);


	return 0;
}

void CR2RTestingPage::OnBnClickedButtonStartOnloaderTest()
{
	// TODO: 在此添加控件通知处理程序代码

	theR2ROnloader.SubmitOnloaderDryRunEvent(TOP_ONLOAD_DRYRUN_TESTSTART);

	m_pWinThreadOnloader = AfxBeginThread(R2ROnloaderTest_ThreadProc, (LPVOID)this);

	Sleep(10);

	GetDlgItem(IDC_BUTTON_START_ONLOADER_TEST)->EnableWindow(FALSE);


}

void CR2RTestingPage::OnBnClickedButtonStartOffloaderTest()
{
	// TODO: 在此添加控件通知处理程序代码

	theR2ROffloader.SubmitOffloaderDryRunEvent(TOP_OFFLOAD_DRYRUN_TESTSTART);

	m_pWinThreadOffloader = AfxBeginThread(R2ROffloaderTest_ThreadProc, (LPVOID)this);

	Sleep(10);

	GetDlgItem(IDC_BUTTON_START_OFFLOADER_TEST)->EnableWindow(FALSE);

}

void CR2RTestingPage::OnBnClickedButtonStopOnloaderTest()
{
	// TODO: 在此添加控件通知处理程序代码
	theR2ROnloader.theInjectPusher.ResetDcMotorError();
	theR2ROnloader.theMagPullyDcMotor.ResetDcMotorError();
	theR2ROnloader.m_nTestState = TOP_ONLOAD_DRYRUN_IDLE;
	GetDlgItem(IDC_BUTTON_START_ONLOADER_TEST)->EnableWindow(TRUE);

}

void CR2RTestingPage::OnBnClickedButtonStopOffloaderTest()
{
	// TODO: 在此添加控件通知处理程序代码
	theR2ROffloader.theMagPullyDcMotor.ResetDcMotorError();
	theR2ROffloader.m_nTestState = TOP_OFFLOAD_DRYRUN_IDLE;
	GetDlgItem(IDC_BUTTON_START_OFFLOADER_TEST)->EnableWindow(TRUE);
}
