/***************************************************************
SingleCapTrackPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "SingleCapTrackPage.h"
#include "PopUpTopStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledClamp.h"
#include "VledOffload.h"
#include "Debug.h"

// CSingleCapTrackPage dialog

IMPLEMENT_DYNAMIC(CSingleCapTrackPage, CPropertyPage)

CSingleCapTrackPage::CSingleCapTrackPage()
	: CPropertyPage(CSingleCapTrackPage::IDD)
	, m_lDbCheckPos(0)
	, m_lInjectPos(0)
	, m_lOpenPos(0)
	, m_lClosePos(0)
	, m_lKickerLimit(0)	
    , m_lKickerPushPos2(0)
{

}

CSingleCapTrackPage::~CSingleCapTrackPage()
{
}

void CSingleCapTrackPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_DB_CHECK_POS, m_lDbCheckPos);
    DDX_Text(pDX, IDC_EDIT_INJECT_POS, m_lInjectPos);
    DDX_Text(pDX, IDC_EDIT_CLAMP_OPEN_WIDTH, m_lOpenPos);
    DDX_Text(pDX, IDC_EDIT_KICKER_PUSH_DIST, m_lKickerLimit);
    DDX_Text(pDX, IDC_EDIT_CLAMP_CLOSE, m_lClosePos);
    DDX_Text(pDX, IDC_EDIT_PUSH_POS2, m_lKickerPushPos2);
}


BEGIN_MESSAGE_MAP(CSingleCapTrackPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_INJECT_HOME, &CSingleCapTrackPage::OnBnClickedButtonInjectHome)
	ON_BN_CLICKED(IDC_BUTTON_INJECT_STOP, &CSingleCapTrackPage::OnBnClickedButtonInjectStop)
	ON_BN_CLICKED(IDC_BUTTON_INJECT_POS1, &CSingleCapTrackPage::OnBnClickedButtonInjectPos1)
	ON_BN_CLICKED(IDC_BUTTON_INJECT_POS2, &CSingleCapTrackPage::OnBnClickedButtonInjectPos2)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_HOME, &CSingleCapTrackPage::OnBnClickedButtonClampHome)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_OPEN, &CSingleCapTrackPage::OnBnClickedButtonClampOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_CLOSE, &CSingleCapTrackPage::OnBnClickedButtonClampClose)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_OPEN_WIDTH, &CSingleCapTrackPage::OnEnSetfocusEditClampOpenWidth)
	ON_EN_SETFOCUS(IDC_EDIT_DB_CHECK_POS, &CSingleCapTrackPage::OnEnSetfocusEditDbCheckPos)
	ON_EN_SETFOCUS(IDC_EDIT_INJECT_POS, &CSingleCapTrackPage::OnEnSetfocusEditInjectPos)
	ON_BN_CLICKED(IDC_BUTTON_KICKE_HOME, &CSingleCapTrackPage::OnBnClickedButtonKickeHome)
	ON_BN_CLICKED(IDC_BUTTON_KICKER_FORWARD, &CSingleCapTrackPage::OnBnClickedButtonKickerForward)
	ON_BN_CLICKED(IDC_BUTTON_KICKER_BK, &CSingleCapTrackPage::OnBnClickedButtonKickerBk)
	ON_BN_CLICKED(IDC_BUTTON_KICKER_STOP, &CSingleCapTrackPage::OnBnClickedButtonKickerStop)
	ON_EN_SETFOCUS(IDC_EDIT_KICKER_PUSH_DIST, &CSingleCapTrackPage::OnEnSetfocusEditKickerPushDist)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_CLOSE, &CSingleCapTrackPage::OnEnSetfocusEditClampClose)
    ON_EN_SETFOCUS(IDC_EDIT_PUSH_POS2, &CSingleCapTrackPage::OnEnSetfocusEditPushPos2)
    ON_BN_CLICKED(IDC_BUTTONKICK_FORWARD2, &CSingleCapTrackPage::OnBnClickedButtonkickForward2)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_STOP, &CSingleCapTrackPage::OnBnClickedButtonClampStop)
END_MESSAGE_MAP()

// CSingleCapTrackPage message handlers
BOOL CSingleCapTrackPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LF_CLAMP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAMPER));

	pWnd = GetDlgItem(IDC_STATIC_OPEN_CLAMP);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OPEN_POS_COLON));

	pWnd = GetDlgItem(IDC_STATIC_CLOSE_CLAMP);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAMP_POS_COLON));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_HOME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_OPEN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OPEN));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_CLOSE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLOSE));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_STOP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

	pWnd = GetDlgItem(IDC_STATIC_INJECTOR);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INJECTOR_STEPPER));

	pWnd = GetDlgItem(IDC_STATIC_DBCHECK_POS);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DOUBLE_CHECK_POS));

	pWnd = GetDlgItem(IDC_STATIC_INJECTOR_POS);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INJECTOR_POS));

	pWnd = GetDlgItem(IDC_BUTTON_INJECT_HOME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_INJECT_POS1);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CHECK_POS));

	pWnd = GetDlgItem(IDC_BUTTON_INJECT_POS2);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INJECT));

	pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

	pWnd = GetDlgItem(IDC_STATIC_KICKER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KICKER));

	pWnd = GetDlgItem(IDC_STATIC_PUSH_LIMIT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OFFLOAD_POS));

	pWnd = GetDlgItem(IDC_BUTTON_KICKE_HOME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_KICKER_FORWARD);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_POS1));

	pWnd = GetDlgItem(IDC_BUTTONKICK_FORWARD2);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_POS2));

    pWnd = GetDlgItem(IDC_BUTTON_KICKER_BK);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BACKWARD));

	pWnd = GetDlgItem(IDC_BUTTON_KICKER_STOP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

	SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
	theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
	m_lDbCheckPos = pstLfTrackRcd.lDbLfCheckPos;
	m_lInjectPos = pstLfTrackRcd.lInjectPos;

	SINGLE_CAP_CLAMP_RCD pstLfClampRcd;
	theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
	m_lOpenPos = pstLfClampRcd.lOpenPosition;
	m_lClosePos = pstLfClampRcd.lClosePosition;

	FORK_OFFLOAD_RCD pstForkOffloadRcd;
	theVledOffload.GetForkOffloadRcd(&pstForkOffloadRcd);
	m_lKickerLimit = pstForkOffloadRcd.lKickerEndPos;
	m_lKickerPushPos2 = pstForkOffloadRcd.lKickerEndPos2;

    if (theDebug.IsDebugWorkHolder()) m_Timer.StartTimer();
       
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSingleCapTrackPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CSingleCapTrackPage::OnOK()
{
	UpdateData(true);

	SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
	theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
	pstLfTrackRcd.lDbLfCheckPos = m_lDbCheckPos;
	pstLfTrackRcd.lInjectPos = m_lInjectPos ;
	theVledTrack.UpdateLfTrackRcd(&pstLfTrackRcd);

	SINGLE_CAP_CLAMP_RCD pstLfClampRcd;
	theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
	pstLfClampRcd.lOpenPosition = m_lOpenPos;
	pstLfClampRcd.lClosePosition = m_lClosePos;
	theVledLfClamp.UpdateLfClampRcd(&pstLfClampRcd);

	FORK_OFFLOAD_RCD pstForkOffloadRcd;
	theVledOffload.GetForkOffloadRcd(&pstForkOffloadRcd);
	pstForkOffloadRcd.lKickerEndPos = m_lKickerLimit;
	pstForkOffloadRcd.lKickerEndPos2 = m_lKickerPushPos2; //@2
	theVledOffload.UpdateForkOffloadRcd(&pstForkOffloadRcd);

	CPropertyPage::OnOK();
}

void CSingleCapTrackPage::OnBnClickedButtonInjectHome()
{
	// TODO: Add your control notification handler code here
	theStepper.SearchHome(SINGLE_CAP_INJECTOR);
	Sleep(10);
    theVledTrack.CheckHomeState();
}

void CSingleCapTrackPage::OnBnClickedButtonInjectStop()
{
	// TODO: Add your control notification handler code here
	theStepper.StopMotor(SINGLE_CAP_INJECTOR);
}

void CSingleCapTrackPage::OnBnClickedButtonInjectPos1()
{
	// TODO: Add your control notification handler code here
    long lPos, lDesPos, lElapseTime, lStartTime; //@3

    lDesPos = round((double)m_lDbCheckPos / theStepper.GetResol(SINGLE_CAP_INJECTOR));
	theStepper.MoveStep(SINGLE_CAP_INJECTOR, lDesPos);
    Sleep(100);

    lStartTime = GetCurrentTime();

    do {
        theStepper.GetPos(SINGLE_CAP_INJECTOR, &lPos);
        
        lElapseTime = GetCurrentTime() - lStartTime;
        if (lElapseTime > STEP_MOTOR_MOTION_TIMEOUT)
            break;

        Sleep(100);
    
    } while (lPos != lDesPos);

    CPopUpTopStepperCtr theStepperCtrler(SINGLE_CAP_INJECTOR, SPEED_FINE);
    theStepperCtrler.DoModal();

    theStepper.GetPos(SINGLE_CAP_INJECTOR, &lPos);

    m_lDbCheckPos += round((double)(lPos - lDesPos) * theStepper.GetResol(SINGLE_CAP_INJECTOR));

    UpdateData(false);

    theVledTrack.SetInjectorPosState(INJECT_POS_CHECK_DOUBLE_LF);
}

void CSingleCapTrackPage::OnBnClickedButtonInjectPos2()
{
	// TODO: Add your control notification handler code here
    long lPos, lDesPos, lElapseTime, lStartTime; //@3

    lDesPos = round((double)m_lInjectPos / theStepper.GetResol(SINGLE_CAP_INJECTOR));
	theStepper.MoveStep(SINGLE_CAP_INJECTOR, lDesPos);
    
    Sleep(100);

    lStartTime = GetCurrentTime();

    do {
        theStepper.GetPos(SINGLE_CAP_INJECTOR, &lPos);
        
        lElapseTime = GetCurrentTime() - lStartTime;
        if (lElapseTime > STEP_MOTOR_MOTION_TIMEOUT)
            break;

        Sleep(100);
    
    } while (lPos != lDesPos);

    CPopUpTopStepperCtr theStepperCtrler(SINGLE_CAP_INJECTOR, SPEED_FINE);
    theStepperCtrler.DoModal();

    theStepper.GetPos(SINGLE_CAP_INJECTOR, &lPos);

    m_lInjectPos += round((double)(lPos - lDesPos) * theStepper.GetResol(SINGLE_CAP_INJECTOR));

    UpdateData(false);

    theVledTrack.SetInjectorPosState(INJECT_POS_INJECT);
}

void CSingleCapTrackPage::OnBnClickedButtonClampHome()
{
	// TODO: Add your control notification handler code here
	theVledLfClamp.SearchClampHome();
	Sleep(10);
	theVledLfClamp.WaitStepHomeFinish();
}

void CSingleCapTrackPage::OnBnClickedButtonClampOpen()
{
    theVledLfClamp.OpenLfClamp();

    if (theDebug.IsDebugWorkHolder())
    {
       	do
		{
    		Sleep(1);
 	    } while (theStepper.IsBusy(SINGLE_CAP_CLAMPER));
    }
}

void CSingleCapTrackPage::OnBnClickedButtonClampClose()
{
    theVledLfClamp.CloseLfClamp();

    if (theDebug.IsDebugWorkHolder())
    {
       	do
		{
    		Sleep(1);
 	    } while (theStepper.IsBusy(SINGLE_CAP_CLAMPER));
    }
}

void CSingleCapTrackPage::OnEnSetfocusEditDbCheckPos()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lDbCheckPos,3200,400,5000); //@1

    UpdateData(false);
    
    SINGLE_CAP_TRACK_RCD   pstLfTrackRcd;
	theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
    pstLfTrackRcd.lDbLfCheckPos =m_lDbCheckPos;
	
    theVledTrack.UpdateLfTrackRcd(&pstLfTrackRcd);

}

void CSingleCapTrackPage::OnEnSetfocusEditInjectPos()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lInjectPos,8750,3000,10000); //@1

    UpdateData(false);
    
    SINGLE_CAP_TRACK_RCD   pstLfTrackRcd;
	theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
   
	pstLfTrackRcd.lInjectPos    =m_lInjectPos ;
    theVledTrack.UpdateLfTrackRcd(&pstLfTrackRcd);

}

void CSingleCapTrackPage::OnBnClickedButtonKickeHome()
{
	// TODO: Add your control notification handler code here
	theStepper.SearchHome(SINGLE_CAP_OFFLOAD_KICKER);
	Sleep(10);
	theVledOffload.WaitStepHomeFinish();

}

void CSingleCapTrackPage::OnBnClickedButtonKickerForward()
{
	// TODO: Add your control notification handler code here
    long lPos, lDesPos, lElapseTime, lStartTime; //@3

    lDesPos = round((double)m_lKickerLimit / theStepper.GetResol(SINGLE_CAP_OFFLOAD_KICKER));
	theStepper.MoveStep(SINGLE_CAP_OFFLOAD_KICKER, lDesPos);
    
    Sleep(100);

    lStartTime = GetCurrentTime();

    do {
        theStepper.GetPos(SINGLE_CAP_OFFLOAD_KICKER, &lPos);
        
        lElapseTime = GetCurrentTime() - lStartTime;
        if (lElapseTime > STEP_MOTOR_MOTION_TIMEOUT)
            break;

        Sleep(100);
    
    } while (lPos != lDesPos);

    CPopUpTopStepperCtr theStepperCtrler(SINGLE_CAP_OFFLOAD_KICKER, SPEED_FINE);
    theStepperCtrler.DoModal();

    theStepper.GetPos(SINGLE_CAP_OFFLOAD_KICKER, &lPos);

    m_lKickerLimit += round((double)(lPos - lDesPos) * theStepper.GetResol(SINGLE_CAP_OFFLOAD_KICKER));

    UpdateData(false);

    //theStepper.MoveStep(SINGLE_CAP_OFFLOAD_KICKER,m_lKickerLimit);
}

void CSingleCapTrackPage::OnBnClickedButtonKickerBk()
{
	// TODO: Add your control notification handler code here
	theStepper.MoveStep(SINGLE_CAP_OFFLOAD_KICKER,0);
}

void CSingleCapTrackPage::OnBnClickedButtonKickerStop()
{
	// TODO: Add your control notification handler code here
	theStepper.StopMotor(SINGLE_CAP_OFFLOAD_KICKER);
}


void CSingleCapTrackPage::OnEnSetfocusEditKickerPushDist()
{
	// TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lKickerLimit,4000,500,5000);

    UpdateData(false);

	FORK_OFFLOAD_RCD pstForkOffloadRcd; //@1
	theVledOffload.GetForkOffloadRcd(&pstForkOffloadRcd);
	pstForkOffloadRcd.lKickerEndPos=m_lKickerLimit;
	theVledOffload.UpdateForkOffloadRcd(&pstForkOffloadRcd);
}
void CSingleCapTrackPage::OnEnSetfocusEditClampClose()
{
	// TODO: Add your control notification handler code here
	 CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lClosePos,0,0,2048);

    UpdateData(false);

    SINGLE_CAP_CLAMP_RCD pstLfClampRcd; //@1
	theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
    pstLfClampRcd.lClosePosition=m_lClosePos;
    theVledLfClamp.UpdateLfClampRcd(&pstLfClampRcd);
}


void CSingleCapTrackPage::OnEnSetfocusEditClampOpenWidth()
{
	// TODO: Add your control notification handler code here
	
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lOpenPos,2048,1000,4096);

    UpdateData(false);
    
	SINGLE_CAP_CLAMP_RCD pstLfClampRcd; //@1
	theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
    pstLfClampRcd.lOpenPosition=m_lOpenPos;
    theVledLfClamp.UpdateLfClampRcd(&pstLfClampRcd);
}
void CSingleCapTrackPage::OnEnSetfocusEditPushPos2() //@2
{
    // TODO: Add your control notification handler code here
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lKickerPushPos2,4000,500,5000);

    UpdateData(false);

	FORK_OFFLOAD_RCD pstForkOffloadRcd;
	theVledOffload.GetForkOffloadRcd(&pstForkOffloadRcd);
	pstForkOffloadRcd.lKickerEndPos2 = m_lKickerPushPos2;
	theVledOffload.UpdateForkOffloadRcd(&pstForkOffloadRcd);
}

void CSingleCapTrackPage::OnBnClickedButtonkickForward2()
{
    // TODO: Add your control notification handler code here
    long lPos, lDesPos, lElapseTime, lStartTime; //@3

    lDesPos = round((double)m_lKickerPushPos2 / theStepper.GetResol(SINGLE_CAP_OFFLOAD_KICKER));
	theStepper.MoveStep(SINGLE_CAP_OFFLOAD_KICKER, lDesPos);
    
    Sleep(100);

    lStartTime = GetCurrentTime();

    do {
        theStepper.GetPos(SINGLE_CAP_OFFLOAD_KICKER, &lPos);
        
        lElapseTime = GetCurrentTime() - lStartTime;
        if (lElapseTime > STEP_MOTOR_MOTION_TIMEOUT)
            break;

        Sleep(100);
    
    } while (lPos != lDesPos);

    CPopUpTopStepperCtr theStepperCtrler(SINGLE_CAP_OFFLOAD_KICKER, SPEED_FINE);
    theStepperCtrler.DoModal();

    theStepper.GetPos(SINGLE_CAP_OFFLOAD_KICKER, &lPos);

    m_lKickerPushPos2 += round((double)(lPos - lDesPos) * theStepper.GetResol(SINGLE_CAP_OFFLOAD_KICKER));

    UpdateData(false);

    //theStepper.MoveStep(SINGLE_CAP_OFFLOAD_KICKER,m_lKickerPushPos2);
}

void CSingleCapTrackPage::OnBnClickedButtonClampStop()
{
	// TODO: Add your control notification handler code here
	theVledLfClamp.StopMotion();
}
