// TopTestingPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "CuEliteTestingPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "MachineConfig.h"
#include "CuIndex.h"
#include "CuOnload.h"
#include "CuOffload.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "servo.h"

// CCuEliteTestingPage dialog

IMPLEMENT_DYNAMIC(CCuEliteTestingPage, CPropertyPage)

CCuEliteTestingPage::CCuEliteTestingPage()
	: CPropertyPage(CCuEliteTestingPage::IDD)
	, m_iDryCycleSpeedId(1)
	, m_iDryRunSpeedID(0)
	, m_lMoveStep(500)
	, m_iMotorID(0)
{

}

CCuEliteTestingPage::~CCuEliteTestingPage()
{
}

void CCuEliteTestingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_DRY_RUN_CLAW, m_iDryCycleSpeedId);
	DDX_CBIndex(pDX, IDC_COMBO_DRY_RUN_SPEED, m_iDryRunSpeedID);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_lMoveStep);
	DDX_CBIndex(pDX, IDC_COMBO_MOTOR2, m_iMotorID);
}


BEGIN_MESSAGE_MAP(CCuEliteTestingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_EVENT_RESET, &CCuEliteTestingPage::OnBnClickedButtonEventReset)
	ON_CBN_SELCHANGE(IDC_COMBO_DRY_RUN_CLAW, &CCuEliteTestingPage::OnCbnSelchangeComboDryRunClaw)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_DRY_RUN, &CCuEliteTestingPage::OnBnClickedButtonClawDryRun)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_STOP, &CCuEliteTestingPage::OnBnClickedButtonClawStop)
	ON_BN_CLICKED(IDC_BUTTON_START_SPEED_TEST, &CCuEliteTestingPage::OnBnClickedButtonStartSpeedTest)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_PROFILE, &CCuEliteTestingPage::OnCbnSelchangeComboSpeedProfile)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SPEED_TEST, &CCuEliteTestingPage::OnBnClickedButtonStopSpeedTest)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_DISABLE, &CCuEliteTestingPage::OnBnClickedButtonEventDisable)
    ON_BN_CLICKED(IDC_BUTTON_ENABLE, &CCuEliteTestingPage::OnBnClickedButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_START_ONLOADER_TEST, &CCuEliteTestingPage::OnBnClickedButtonStartOnloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_ONLOADER_TEST, &CCuEliteTestingPage::OnBnClickedButtonStopOnloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_START_OFFLOADER_TEST, &CCuEliteTestingPage::OnBnClickedButtonStartOffloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_OFFLOADER_TEST, &CCuEliteTestingPage::OnBnClickedButtonStopOffloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_BACKWARD_, &CCuEliteTestingPage::OnBnClickedButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON_FORWARD_, &CCuEliteTestingPage::OnBnClickedButtonForward)
	ON_BN_CLICKED(IDC_BUTTON_HOME_STEP, &CCuEliteTestingPage::OnBnClickedButtonHomeStep)
	ON_BN_CLICKED(IDC_BUTTON_STAND_BY, &CCuEliteTestingPage::OnBnClickedButtonStandBy)
END_MESSAGE_MAP()


// CCuEliteTestingPage message handlers

BOOL CCuEliteTestingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CString str;

    CWnd* pWnd = GetDlgItem(IDC_STATIC_DRY_RUN);
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

	pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_BACKWARD_);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BACKWARD));

	pWnd = GetDlgItem(IDC_BUTTON_FORWARD_);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FORWARD));

	pWnd = GetDlgItem(IDC_BUTTON_STAND_BY);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STANDBY_POS));

    //*********Gripper Dry Cycle Testing************************************

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DRY_RUN_CLAW);
    pComboBox->InsertString(0, theMsg.GetMsg(MSG_SLOW));
    pComboBox->InsertString(1, theMsg.GetMsg(MSG_MEDIUM));
    pComboBox->InsertString(2, theMsg.GetMsg(MSG_FAST));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DRY_RUN_SPEED);
	pComboBox->InsertString(0, _T("Search"));
	pComboBox->InsertString(1, _T("Slow Forward"));
	pComboBox->InsertString(2, _T("Forward"));
	pComboBox->InsertString(3, _T("Inject"));
	pComboBox->InsertString(4, _T("Backward"));
	pComboBox->InsertString(5, _T("Index"));
	pComboBox->InsertString(6, _T("Eject"));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTOR2);
	str = theMsg.GetMsg(MSG_INPUT);
	pComboBox->InsertString(0, str + theMsg.GetMsg(MSG_ELEVATOR));
	pComboBox->InsertString(1, str + theMsg.GetMsg(MSG_SIDE_CLAMPER));
	pComboBox->InsertString(2, str + theMsg.GetMsg(MSG_FRONT_CLAMPER));
	pComboBox->InsertString(3, str + theMsg.GetMsg(MSG_REAR_CLAMPER));

	str = theMsg.GetMsg(MSG_OUTPUT);
	pComboBox->InsertString(4, str + theMsg.GetMsg(MSG_ELEVATOR));
	pComboBox->InsertString(5, str + theMsg.GetMsg(MSG_SIDE_CLAMPER));
	pComboBox->InsertString(6, str + theMsg.GetMsg(MSG_FRONT_CLAMPER));
	pComboBox->InsertString(7, str + theMsg.GetMsg(MSG_REAR_CLAMPER));

	pComboBox->InsertString(8, theMsg.GetMsg(MSG_FRONT_RAIL));
	pComboBox->InsertString(9, theMsg.GetMsg(MSG_REAR_RAIL));
	pComboBox->InsertString(10, theMsg.GetMsg(MSG_INDEX_STEPPER));

    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCuEliteTestingPage::OnSetActive( )
{
	if (CPropertyPage::OnSetActive())
	{
		bool bStepperOn = false;
		int  iStatus = FOK;

		iStatus = theStepper.CheckPowerStatus(&bStepperOn);
		if (iStatus != FOK)
			bStepperOn = false;

		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_BUTTON_EVENT_RESET);
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

		m_iDryCycleSpeedId=1;
		theCuEliteTrack.m_lClawInterval = 800;  //ms

		UpdateData(false);
	}
	return true;
}

HBRUSH CCuEliteTestingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


UINT CuElite_DryRun_ThreadProc( LPVOID pParam)
{
	CCuEliteTestingPage* pObject = (CCuEliteTestingPage *)pParam;
	return pObject->DryRunProcess(); 	

}

//void CCuEliteTestingPage::OnBnClickedButtonDryRun()
//{
//	UpdateData(TRUE);
//
//	bStopDryRun =false;
//
//	m_pDryWinThread = AfxBeginThread(DryRun_ThreadProc, (LPVOID)this);
//
//}

int  CCuEliteTestingPage::DryRunProcess()
{	
   return 0;
}

//*************************Gripper Dry Cycle************************** 
void CCuEliteTestingPage::OnCbnSelchangeComboDryRunClaw()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	switch(m_iDryCycleSpeedId)
	{
	case 0:

		theCuEliteTrack.m_lClawInterval = 1200;

		break;
	case 1:

		theCuEliteTrack.m_lClawInterval= 800;

		break;

	case 2:
		theCuEliteTrack.m_lClawInterval = 400;

		break;

	default:
		break;

	}


}


UINT CuEliteGripperDryRun_ThreadProc( LPVOID pParam )
{
	
	do
   {
	   theCuEliteTrack.ProcessCuEliteGripperDryRun();
	   Sleep(10);
	  	  	   
	}
	while(theCuEliteTrack.m_nGripperDryCycleState != CLAW_DRY_CYCLE_IDLE);
    

	return 0;
}



void CCuEliteTestingPage::OnBnClickedButtonClawDryRun()
{
	// TODO: Add your control notification handler code here

	 theCuEliteTrack.SubmitGripperDryRun(0);

	 m_pWinThread = AfxBeginThread(CuEliteGripperDryRun_ThreadProc, (LPVOID)this);

	 Sleep(10);
}

void CCuEliteTestingPage::OnBnClickedButtonClawStop()
{
	// TODO: Add your control notification handler code here
   
    theCuEliteTrack.m_nGripperDryCycleState = CLAW_DRY_CYCLE_IDLE;
    Sleep(100);

    theCuEliteTrack.theInputClaw.SetRelayState(RELAY_OFF);
    theCuEliteTrack.theOutputClaw.SetRelayState(RELAY_OFF);
    theCuEliteTrack.theEjector.SetRelayState(RELAY_OFF);
}

//----------------Speed test--------------------------------------------------


UINT CuEliteIndexTest_ThreadProc( LPVOID pParam )
{

	do
   {

	   theCuEliteTrack.ProcessIndexerDryRun();

	   Sleep(10);
	}
	while(theCuEliteTrack.m_nTestState != INDEX_TEST_STATE_IDLE);
    

	return 0;
}


void CCuEliteTestingPage::OnBnClickedButtonStartSpeedTest()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	STEP_SPEED_PROFILE stIndexerSpeedProfile;

	theCuEliteTrack.GetCuEliteIndexerStepperProfile(m_iDryRunSpeedID, &stIndexerSpeedProfile);

	theStepper.SetSpeedParameter(theCuEliteTrack.m_ucIndexerStepperId, &stIndexerSpeedProfile);

	theCuEliteTrack.SubmitIndexerDryRunEvent(INDEX_TEST_SPEED_PROFILE);

    m_pWinThreadSpeed = AfxBeginThread(CuEliteIndexTest_ThreadProc, (LPVOID)this);

	Sleep(10);

}

void CCuEliteTestingPage::OnCbnSelchangeComboSpeedProfile()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);


	UpdateData(FALSE);
	
}

void CCuEliteTestingPage::OnBnClickedButtonStopSpeedTest()
{
	// TODO: Add your control notification handler code here
	theCuEliteTrack.m_nTestState = INDEX_TEST_STATE_IDLE;
	theCuEliteTrack.m_nTestEvent = INDEX_TEST_EVENT_IDLE;

}

//void CCuEliteTestingPage::OnBnClickedCheckWithoutLf()
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(true);
//
//	theCuEliteTrack.SetAutoRunWithNoLf(m_bWithoutLf);
//
//	
//}

void CCuEliteTestingPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	theCuEliteTrack.SetAutoRunWithNoLf(FALSE);

	/*theCuEliteOffloader.SetForkOffloadValid(TRUE);
	theCuEliteOnloader.SetOnloadValid(TRUE);*/

	CPropertyPage::OnOK();
}


void CCuEliteTestingPage::OnBnClickedButtonEnable()
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

void CCuEliteTestingPage::OnBnClickedButtonEventDisable()
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_EVENT_DISABLE);
    pWnd->EnableWindow(FALSE);

    theStepper.StopMotor(CUELITE_ONLOAD_ELEVATOR);
	theStepper.StopMotor(CUELITE_OFFLOAD_ELEVATOR);
	theStepper.StopMotor(CUELITE_DEVICE_INDEXER);
	theStepper.StopMotor(CUELITE_WINDOW_CLAMPER);

    theStepper.PowerOff();

    pWnd = GetDlgItem(IDC_BUTTON_ENABLE);
    pWnd->EnableWindow(TRUE);

    pWnd = GetDlgItem(IDC_BUTTON_EVENT_RESET);
    pWnd->EnableWindow(FALSE);
}

void CCuEliteTestingPage::OnBnClickedButtonEventReset()
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_EVENT_RESET);
    pWnd->EnableWindow(FALSE);

	theServo.MoveBondHeadToSafePos();
	Sleep(10);

	theMHS.SetAutoIndex(false);
	theMHS.SetRequestIndex(false);

	theCuEliteTrack.ResetTrackErrorCode(1);
	theCuEliteTrack.ResetTrackErrorCode(0);

	theMHS.SetLastLf(false);

	theCuEliteTrack.ResetSlipRcd();
	theCuEliteOnloader.ResetOnloadEvent();
	theCuEliteOffloader.ResetOffloadEvent();

	theMHS.WarmReset();
	Sleep(100);
    
    pWnd->EnableWindow(TRUE);
}

//----------------Onloader/Offloader test-----------------------

UINT CuEliteOnloaderTest_ThreadProc( LPVOID pParam )
{
	do
	{
		theCuEliteOnloader.ProcessOnloaderDryRunEvent();

		Sleep(10);
	}
	while(theCuEliteOnloader.m_nTestState != INDEX_TEST_STATE_IDLE);

	return 0;
}

UINT CuEliteOffloaderTest_ThreadProc( LPVOID pParam )
{
	do
	{
		theCuEliteOffloader.ProcessOffloaderDryRunEvent();

		Sleep(10);
	}
	while(theCuEliteOffloader.m_nTestState != INDEX_TEST_STATE_IDLE);

	return 0;
}


void CCuEliteTestingPage::OnBnClickedButtonStartOnloaderTest()
{
	// TODO: Add your control notification handler code here
	theCuEliteOnloader.SubmitOnloaderDryRunEvent(TOP_ONLOAD_DRYRUN_TESTSTART);

	m_pWinThreadOnloader = AfxBeginThread(CuEliteOnloaderTest_ThreadProc, (LPVOID)this);

	Sleep(10);

	GetDlgItem(IDC_BUTTON_START_ONLOADER_TEST)->EnableWindow(FALSE);
}

void CCuEliteTestingPage::OnBnClickedButtonStopOnloaderTest()
{
	// TODO: Add your control notification handler code here
	theCuEliteOnloader.theInjectPusher.ResetDcMotorError();
	theCuEliteOnloader.theMagPullyDcMotor.ResetDcMotorError();
	theCuEliteOnloader.m_nTestState = TOP_ONLOAD_DRYRUN_IDLE;
	GetDlgItem(IDC_BUTTON_START_ONLOADER_TEST)->EnableWindow(TRUE);
}

void CCuEliteTestingPage::OnBnClickedButtonStartOffloaderTest()
{
	// TODO: Add your control notification handler code here
	theCuEliteOffloader.SubmitOffloaderDryRunEvent(TOP_OFFLOAD_DRYRUN_TESTSTART);

	m_pWinThreadOffloader = AfxBeginThread(CuEliteOffloaderTest_ThreadProc, (LPVOID)this);

	Sleep(10);

	GetDlgItem(IDC_BUTTON_START_OFFLOADER_TEST)->EnableWindow(FALSE);
}

void CCuEliteTestingPage::OnBnClickedButtonStopOffloaderTest()
{
	// TODO: Add your control notification handler code here
	theCuEliteOffloader.theMagPullyDcMotor.ResetDcMotorError();
	theCuEliteOffloader.m_nTestState = TOP_OFFLOAD_DRYRUN_IDLE;
	GetDlgItem(IDC_BUTTON_START_OFFLOADER_TEST)->EnableWindow(TRUE);
}

void CCuEliteTestingPage::OnBnClickedButtonBackward()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	switch (m_iMotorID)
	{
	case 0:
		m_ucStepperID = theCuEliteOnloader.m_ucForkHolderStepId;
		break;
	case 1:
		m_ucStepperID = theCuEliteOnloader.m_ucMagLengthStepId;
		break;
	case 2:
		m_ucStepperID = theCuEliteOnloader.m_ucMagClamperFrontStepId;
		break;
	case 3:
		m_ucStepperID = theCuEliteOnloader.m_ucMagClamperBackStepId;
		break;
	case 4:
		m_ucStepperID = theCuEliteOffloader.m_ucForkHolderStepId;
		break;
	case 5:
		m_ucStepperID = theCuEliteOffloader.m_ucMagLengthStepId;
		break;
	case 6:
		m_ucStepperID = theCuEliteOffloader.m_ucMagClamperFrontStepId;
		break;
	case 7:
		m_ucStepperID = theCuEliteOffloader.m_ucMagClamperBackStepId;
		break;
	case 8:
		m_ucStepperID = theCuEliteTrack.m_ucFrontTrackStepperId;
		break;
	case 9:
		m_ucStepperID = theCuEliteTrack.m_ucBackTrackStepperId;
		break;
	case 10:
		m_ucStepperID = theCuEliteTrack.m_ucIndexerStepperId;
		break;
	}

	theStepper.MoveStep(m_ucStepperID, 0 - m_lMoveStep, MOTION_RELATIVE);
}

void CCuEliteTestingPage::OnBnClickedButtonForward()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	switch (m_iMotorID)
	{
	case 0:
		m_ucStepperID = theCuEliteOnloader.m_ucForkHolderStepId;
		break;
	case 1:
		m_ucStepperID = theCuEliteOnloader.m_ucMagLengthStepId;
		break;
	case 2:
		m_ucStepperID = theCuEliteOnloader.m_ucMagClamperFrontStepId;
		break;
	case 3:
		m_ucStepperID = theCuEliteOnloader.m_ucMagClamperBackStepId;
		break;
	case 4:
		m_ucStepperID = theCuEliteOffloader.m_ucForkHolderStepId;
		break;
	case 5:
		m_ucStepperID = theCuEliteOffloader.m_ucMagLengthStepId;
		break;
	case 6:
		m_ucStepperID = theCuEliteOffloader.m_ucMagClamperFrontStepId;
		break;
	case 7:
		m_ucStepperID = theCuEliteOffloader.m_ucMagClamperBackStepId;
		break;
	case 8:
		m_ucStepperID = theCuEliteTrack.m_ucFrontTrackStepperId;
		break;
	case 9:
		m_ucStepperID = theCuEliteTrack.m_ucBackTrackStepperId;
		break;
	case 10:
		m_ucStepperID = theCuEliteTrack.m_ucIndexerStepperId;
		break;
	}

	theStepper.MoveStep(m_ucStepperID, m_lMoveStep, MOTION_RELATIVE);
}

void CCuEliteTestingPage::OnBnClickedButtonHomeStep()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	switch (m_iMotorID)
	{
	case 0:
		m_ucStepperID = theCuEliteOnloader.m_ucForkHolderStepId;
		break;
	case 1:
		m_ucStepperID = theCuEliteOnloader.m_ucMagLengthStepId;
		break;
	case 2:
		m_ucStepperID = theCuEliteOnloader.m_ucMagClamperFrontStepId;
		break;
	case 3:
		m_ucStepperID = theCuEliteOnloader.m_ucMagClamperBackStepId;
		break;
	case 4:
		m_ucStepperID = theCuEliteOffloader.m_ucForkHolderStepId;
		break;
	case 5:
		m_ucStepperID = theCuEliteOffloader.m_ucMagLengthStepId;
		break;
	case 6:
		m_ucStepperID = theCuEliteOffloader.m_ucMagClamperFrontStepId;
		break;
	case 7:
		m_ucStepperID = theCuEliteOffloader.m_ucMagClamperBackStepId;
		break;
	case 8:
		m_ucStepperID = theCuEliteTrack.m_ucFrontTrackStepperId;
		break;
	case 9:
		m_ucStepperID = theCuEliteTrack.m_ucBackTrackStepperId;
		break;
	case 10:
		m_ucStepperID = theCuEliteTrack.m_ucIndexerStepperId;
		break;
	}

	theStepper.SearchHome(m_ucStepperID);
}

void CCuEliteTestingPage::OnBnClickedButtonStandBy()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	switch (m_iMotorID)
	{
	case 0:
		theCuEliteOnloader.MoveTotheSlot(1);
		break;
	case 1:
		theCuEliteOnloader.OnloadMagLengthMovetoStandyPos();
		break;
	case 2:
		theCuEliteOnloader.OnloadMagClamperFrontMovetoStandyPos();
		break;
	case 3:
		theCuEliteOnloader.OnloadMagClamperBackMovetoStandyPos();
		break;
	case 4:
		theCuEliteOffloader.MoveTotheSlot(1);
		break;
	case 5:
		theCuEliteOffloader.OffloadMagLengthMovetoStandyPos();
		break;
	case 6:
		theCuEliteOffloader.OffloadMagClamperFrontMovetoStandyPos();
		break;
	case 7:
		theCuEliteOffloader.OffloadMagClamperBackMovetoStandyPos();
		break;
	case 8:
		theCuEliteTrack.MoveFrontTrackToStandbyPos();
		break;
	case 9:
		theCuEliteTrack.MoveBackTrackToStandbyPos();
		break;
	case 10:
		theCuEliteTrack.MoveToLogicZero();
		break;
	}

}
