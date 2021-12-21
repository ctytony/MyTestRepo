// TopTestingPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "TopTestingPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "MachineConfig.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "servo.h"
#include "Busy.h"

// CTopTestingPage dialog
IMPLEMENT_DYNAMIC(CTopTestingPage, CPropertyPage)

CTopTestingPage::CTopTestingPage()
	: CPropertyPage(CTopTestingPage::IDD)
	, m_iDryCycleSpeedId(1)
	, m_iSpeedProfileNum(0)
	, m_lStartVel(0)
	, m_lDriveVel(0)
	, m_lMaxVel(0)
	, m_lAcc(0)
	, m_lJerk(0)
	, m_lJeckDec(0)
{
	m_bBusy=FALSE;	//V1.4.W23 add
}

CTopTestingPage::~CTopTestingPage()
{
}

void CTopTestingPage::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(CTopTestingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_EVENT_RESET, OnEventReset)
	ON_CBN_SELCHANGE(IDC_COMBO_DRY_RUN_CLAW, OnDryRunClaw)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_DRY_RUN, OnClawDryRun)
	ON_BN_CLICKED(IDC_BUTTON_CLAW_STOP, OnClawStop)
	ON_BN_CLICKED(IDC_BUTTON_START_SPEED_TEST, OnStartSpeedTest)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED_PROFILE, OnSpeedProfile)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SPEED_TEST, OnStopSpeedTest)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_DISABLE, OnEventDisable)
	ON_EN_SETFOCUS(IDC_EDIT_START_VEL, OnFocusStartVel)
	ON_EN_SETFOCUS(IDC_EDIT_VEL, OnFocusVel)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_VEL, OnFocusMaxVel)
	ON_EN_SETFOCUS(IDD_EDIT_ACC_, OnFocusAcc)
	ON_EN_SETFOCUS(IDC_EDIT_JERK_, OnFocusJerk)
	ON_EN_SETFOCUS(IDC_EDIT_JERK_DEC, OnFocusJerkDec)
    ON_BN_CLICKED(IDC_BUTTON_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_BUTTON_START_ONLOADER_TEST, OnStartOnloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_START_OFFLOADER_TEST, OnStartOffloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_ONLOADER_TEST, OnStopOnloaderTest)
	ON_BN_CLICKED(IDC_BUTTON_STOP_OFFLOADER_TEST, OnStopOffloaderTest)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.4.W23 add
	ON_WM_DESTROY()		//V1.4.W23 add
END_MESSAGE_MAP()

// CTopTestingPage message handlers
BOOL CTopTestingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    bool bStepperOn = false;
    if (theStepper.CheckPowerStatus(&bStepperOn) != FOK)
        bStepperOn = false;

    SetDlgItemTextW(IDC_BUTTON_EVENT_RESET,theMsg.GetMsg(MSG_INITIALIZE));
	GetDlgItem(IDC_BUTTON_EVENT_RESET)->EnableWindow(bStepperOn);

    SetDlgItemTextW(IDC_BUTTON_EVENT_DISABLE,theMsg.GetMsg(MSG_DISABLE_2));
	GetDlgItem(IDC_BUTTON_EVENT_DISABLE)->EnableWindow(bStepperOn);

    SetDlgItemTextW(IDC_BUTTON_ENABLE,theMsg.GetMsg(MSG_ENABLE_2));
	GetDlgItem(IDC_BUTTON_ENABLE)->EnableWindow(!bStepperOn);

    SetDlgItemTextW(IDC_STATIC_DRY_RUN,theMsg.GetMsg(MSG_STEPPER));
    SetDlgItemTextW(IDC_STATIC_SPEED_TEST,theMsg.GetMsg(MSG_INDEXER));
    SetDlgItemTextW(IDC_BUTTON_START_SPEED_TEST,theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_STOP_SPEED_TEST,theMsg.GetMsg(MSG_STOP));
    SetDlgItemTextW(IDC_STATIC_SOLINOID,theMsg.GetMsg(MSG_SOLENOID_TEST));
    SetDlgItemTextW(IDC_BUTTON_CLAW_DRY_RUN,theMsg.GetMsg(MSG_DRY_RUN));
    SetDlgItemTextW(IDC_BUTTON_CLAW_STOP,theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_STATIC_DRY_ONLOADER,theMsg.GetMsg(MSG_INPUT));
	SetDlgItemTextW(IDC_STATIC_DRY_OFFLOADER,theMsg.GetMsg(MSG_OUTPUT));
	SetDlgItemTextW(IDC_BUTTON_START_ONLOADER_TEST,theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_BUTTON_STOP_ONLOADER_TEST,theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_BUTTON_START_OFFLOADER_TEST,theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_BUTTON_STOP_OFFLOADER_TEST,theMsg.GetMsg(MSG_STOP));

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DRY_RUN_CLAW);
    pComboBox->AddString(theMsg.GetMsg(MSG_SLOW));
    pComboBox->AddString(theMsg.GetMsg(MSG_MEDIUM));
    pComboBox->AddString(theMsg.GetMsg(MSG_FAST));

    m_iDryCycleSpeedId=1;
    theTopTrack.m_lClawInterval = 800;  //ms

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED_PROFILE);
    pComboBox->AddString(_T("0"));
    pComboBox->AddString(_T("1"));
    pComboBox->AddString(_T("2"));
    pComboBox->AddString(_T("3"));
    pComboBox->AddString(_T("4"));
    pComboBox->AddString(_T("5"));
    pComboBox->AddString(_T("6"));

    m_iSpeedProfileNum = 0;

    STEP_SPEED_PROFILE stIndexerSpeedProfile;
    theTopTrack.GetIndexerStepperProfile(0, &stIndexerSpeedProfile);
    m_lStartVel = stIndexerSpeedProfile.lStartVelocity;
    m_lDriveVel = stIndexerSpeedProfile.lDriveVelocity;
    m_lMaxVel   = stIndexerSpeedProfile.lMaxDriveVelocity;
    m_lAcc      = stIndexerSpeedProfile.lAcc;

    if (theMcConfig.GetStepperType() == STEP_TYPE_1240)
    {
        m_lJerk = stIndexerSpeedProfile.lAccRatio;
    }
    else
    {
        m_lJerk = m_lDriveVel/3;
        m_lJeckDec = m_lJerk;
    }

    return CPropertyPage::OnInitDialog();
}

HBRUSH CTopTestingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTopTestingPage::OnDryRunClaw()
{
	UpdateData(true);

	switch(m_iDryCycleSpeedId)
	{
	case 0:
		theTopTrack.m_lClawInterval = 1200;
		break;
	case 1:
		theTopTrack.m_lClawInterval= 800;
		break;
	case 2:
		theTopTrack.m_lClawInterval = 400;
		break;
	default:
		break;
	}
}

UINT CTopTestingPage::TopGripperDryRun_ThreadProc(LPVOID pParam)
{
	CTopTestingPage* lpThis=(CTopTestingPage*)pParam;	//V1.4.W23 add
	CBusy bs(&lpThis->m_bBusy);							//V1.4.W23 add

	do
	{
		theTopTrack.ProcessGripperDryRunEvent();
		Sleep_x1ms(10);
	} while (theTopTrack.m_nGripperDryCycleState != CLAW_DRY_CYCLE_IDLE);

	return 0;
}

void CTopTestingPage::OnClawDryRun()
{
	GetDlgItem(IDC_BUTTON_CLAW_DRY_RUN)->EnableWindow(FALSE);
	theTopTrack.SubmitGripperDryRunEvent(0);
	AfxBeginThread(TopGripperDryRun_ThreadProc, (LPVOID)this);
	Sleep_x1ms(10);
}

void CTopTestingPage::OnClawStop()
{
    theTopTrack.m_nGripperDryCycleState = CLAW_DRY_CYCLE_IDLE;
    Sleep_x1ms(100);

	theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);
	theTopTrack.m_Injector.SetRelayState(RELAY_OFF);
    theTopTrack.m_InputClaw.SetRelayState(RELAY_OFF);
    theTopTrack.m_OutputClaw.SetRelayState(RELAY_OFF);
    theTopTrack.m_Ejector.SetRelayState(RELAY_OFF);

	GetDlgItem(IDC_BUTTON_CLAW_DRY_RUN)->EnableWindow(TRUE);
}

UINT CTopTestingPage::TopIndexTest_ThreadProc(LPVOID pParam)
{
	CTopTestingPage* lpThis=(CTopTestingPage*)pParam;	//V1.4.W23 add
	CBusy bs(&lpThis->m_bBusy);							//V1.4.W23 add

	do
	{
		theTopTrack.ProcessDryRunEvent();
		Sleep_x1ms(10);
	} while (theTopTrack.m_nTestState != INDEX_TEST_STATE_IDLE);

	return 0;
}

void CTopTestingPage::OnStartSpeedTest()
{
	UpdateData(TRUE);

	STEP_SPEED_PROFILE stIndexerSpeedProfile;

	theTopTrack.GetIndexerStepperProfile(m_iSpeedProfileNum, &stIndexerSpeedProfile);

	stIndexerSpeedProfile.lAcc =m_lAcc;
	stIndexerSpeedProfile.lAccRatio =m_lJerk;
	stIndexerSpeedProfile.lDriveVelocity =m_lDriveVel;
	stIndexerSpeedProfile.lMaxDriveVelocity =m_lMaxVel;
	stIndexerSpeedProfile.lStartVelocity =m_lStartVel;

	theStepper.SetSpeedParameter(theTopTrack.GetIndexStepID(), &stIndexerSpeedProfile);
	theTopTrack.SubmitDryRunEvent(INDEX_TEST_SPEED_PROFILE);
    AfxBeginThread(TopIndexTest_ThreadProc, (LPVOID)this);
	Sleep_x1ms(10);

	GetDlgItem(IDC_BUTTON_START_SPEED_TEST)->EnableWindow(FALSE);
}

void CTopTestingPage::OnSpeedProfile()
{
	UpdateData(true);

	STEP_SPEED_PROFILE stIndexerSpeedProfile;

	theTopTrack.GetIndexerStepperProfile(m_iSpeedProfileNum, &stIndexerSpeedProfile);

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

void CTopTestingPage::OnStopSpeedTest()
{
	theTopTrack.m_nTestState = INDEX_TEST_STATE_IDLE;
	theTopTrack.m_nTestEvent = INDEX_TEST_EVENT_IDLE;
	GetDlgItem(IDC_BUTTON_START_SPEED_TEST)->EnableWindow(TRUE);
}

LRESULT CTopTestingPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.4.W23 add
{
	return m_bBusy;
}

void CTopTestingPage::OnDestroy()
{
	theTopTrack.SetRunNoLf(false);

	CPropertyPage::OnDestroy();
}

void CTopTestingPage::OnFocusStartVel()
{
	GetDlgItem(IDC_BUTTON_CLAW_STOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lStartVel);

	UpdateData(false);
}

void CTopTestingPage::OnFocusVel()
{
	GetDlgItem(IDC_BUTTON_CLAW_STOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lDriveVel);

	UpdateData(false);
}

void CTopTestingPage::OnFocusMaxVel()
{
	GetDlgItem(IDC_BUTTON_CLAW_STOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lMaxVel);

	UpdateData(false);
}

void CTopTestingPage::OnFocusAcc()
{
	GetDlgItem(IDC_BUTTON_CLAW_STOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lAcc);

	UpdateData(false);
}

void CTopTestingPage::OnFocusJerk()
{
	GetDlgItem(IDC_BUTTON_CLAW_STOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lJerk);

	UpdateData(false);
}

void CTopTestingPage::OnFocusJerkDec()
{
	GetDlgItem(IDC_BUTTON_CLAW_STOP)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lJeckDec);

	UpdateData(false);
}

void CTopTestingPage::OnEnable()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

    GetDlgItem(IDC_BUTTON_ENABLE)->EnableWindow(FALSE);

    theStepper.PowerOn();

    Sleep_x1ms(1000);

    GetDlgItem(IDC_BUTTON_EVENT_DISABLE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_EVENT_RESET)->EnableWindow(TRUE);
}

void CTopTestingPage::OnEventDisable()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

    GetDlgItem(IDC_BUTTON_EVENT_DISABLE)->EnableWindow(FALSE);

	theStepper.StopMotor(theTopOnloader.m_nMagClamperStepperId);
	theStepper.StopMotor(theTopOffloader.m_nMagClamperStepperId);
    theStepper.StopMotor(theTopOnloader.m_nMagHolderStepperId);
	theStepper.StopMotor(theTopOffloader.m_nMagHolderStepperId);
	theStepper.StopMotor(theTopTrack.GetIndexStepID());
	theStepper.StopMotor(theTopClamp.GetStepID());

    theStepper.PowerOff();

    GetDlgItem(IDC_BUTTON_ENABLE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_EVENT_RESET)->EnableWindow(FALSE);
}

void CTopTestingPage::OnEventReset()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

    GetDlgItem(IDC_BUTTON_EVENT_RESET)->EnableWindow(FALSE);

	theServo.MoveBondHeadToSafePos();
	Sleep_x1ms(10);

	theMHS.SetAutoIndex(false);
	theMHS.SetRequestIndex(false);

	theTopTrack.ResetErrorCode(1);
	theTopTrack.ResetErrorCode(0);

	theMHS.SetLastLf(false);

	theTopTrack.ResetSlipRcd();
	theTopOnloader.ResetEvent();
	theTopOffloader.ResetEvent();

	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		theStepperIO.ResetStepCard();
		theTopOnloader.Init();
		theTopOffloader.Init();
		theTopClamp.Init();
		theTopTrack.Init();
		theStepperIO.EnableStepper(CANADDR_ONLOAD, true);
		theStepperIO.EnableStepper(CANADDR_OFFLOAD, true);
		theStepperIO.EnableStepper(CANADDR_INDEX, true);
		//Sleep_x1ms(500);
	}

	theTopTrack.HomeIndexer();
    Sleep_x1ms(10);
	theTopClamp.HomeClamp();

	theTopTrack.WaitHomeIndexer();
	Sleep_x1ms(100);
	theTopClamp.WaitHomeClamp();

	Sleep_x1ms(500);
	theTopClamp.PreCloseClamp();

	theTopTrack.MoveIndexerToLogicZero();
	Sleep_x1ms(800);

	theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
	theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();

	theTopOnloader.HomeMagHolder();
	Sleep_x1ms(10);
	theTopOffloader.HomeMagHolder();
	Sleep_x1ms(2000);
	theTopOnloader.HomeMagClamper();
	Sleep_x1ms(10);
	theTopOffloader.HomeMagClamper();
	Sleep_x1ms(10);
	theTopOnloader.WaitHomeMagHolder();
	Sleep_x1ms(10);
	theTopOffloader.WaitHomeMagHolder();
	Sleep_x1ms(1000);
	theTopOnloader.WaitHomeMagClamper();
	Sleep_x1ms(10);
	theTopOffloader.WaitHomeMagClamper();
	Sleep_x1ms(10);
	theTopOnloader.MoveMagHolderToSlot(1);
	Sleep_x1ms(10);
	theTopOffloader.MoveMagHolderToSlot(1); 
	Sleep_x1ms(100);
	theTopOnloader.MoveMagClamperToClampPos();
	Sleep_x1ms(10);
	theTopOffloader.MoveMagClamperToClampPos();
	Sleep_x1ms(2300);

    GetDlgItem(IDC_BUTTON_EVENT_RESET)->EnableWindow(TRUE);
}

UINT CTopTestingPage::TopOnloaderTest_ThreadProc(LPVOID pParam)
{
	CTopTestingPage* lpThis=(CTopTestingPage*)pParam;	//V1.4.W23 add
	CBusy bs(&lpThis->m_bBusy);							//V1.4.W23 add

	do
	{
		theTopOnloader.ProcessDryRunEvent();
		Sleep_x1ms(10);
	} while (! lpThis->m_bTopOnloaderTestStop);

	return 0;
}

UINT CTopTestingPage::TopOffloaderTest_ThreadProc(LPVOID pParam)
{
	CTopTestingPage* lpThis=(CTopTestingPage*)pParam;	//V1.4.W23 add
	CBusy bs(&lpThis->m_bBusy);							//V1.4.W23 add

	do
	{
		theTopOffloader.ProcessDryRunEvent();
		Sleep_x1ms(10);
	} while (! lpThis->m_bTopOffloaderTestStop);

	return 0;
}

void CTopTestingPage::OnStartOnloaderTest()
{
	theTopOnloader.SubmitDryRunEvent(TOP_ONLOAD_DRYRUN_TESTSTART);
	m_bTopOnloaderTestStop = false;
	AfxBeginThread(TopOnloaderTest_ThreadProc, (LPVOID)this);
	Sleep_x1ms(10);

	GetDlgItem(IDC_BUTTON_START_ONLOADER_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_ONLOADER_TEST)->EnableWindow(TRUE);
}

void CTopTestingPage::OnStartOffloaderTest()
{
	theTopOffloader.SubmitDryRunEvent(TOP_OFFLOAD_DRYRUN_TESTSTART);
	m_bTopOffloaderTestStop = false;
	AfxBeginThread(TopOffloaderTest_ThreadProc, (LPVOID)this);
	Sleep_x1ms(10);

	GetDlgItem(IDC_BUTTON_START_OFFLOADER_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_OFFLOADER_TEST)->EnableWindow(TRUE);
}

void CTopTestingPage::OnStopOnloaderTest()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	theTopOnloader.m_InjectPusher.ResetDcMotorError();
	theTopOnloader.m_MagPullyDcMotor.ResetDcMotorError();
	theTopOnloader.m_nTestState = TOP_ONLOAD_DRYRUN_IDLE;
	m_bTopOnloaderTestStop = true;
	Sleep_x1ms(1000);
	GetDlgItem(IDC_BUTTON_STOP_ONLOADER_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START_ONLOADER_TEST)->EnableWindow(TRUE);
}

void CTopTestingPage::OnStopOffloaderTest()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	theTopOffloader.m_MagPullyDcMotor.ResetDcMotorError();
	theTopOffloader.m_nTestState = TOP_OFFLOAD_DRYRUN_IDLE;
	m_bTopOffloaderTestStop = true;
	Sleep_x1ms(1000);
	GetDlgItem(IDC_BUTTON_STOP_OFFLOADER_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START_OFFLOADER_TEST)->EnableWindow(TRUE);
}
