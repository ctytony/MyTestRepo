/***************************************************************
VledElitePage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "VledElitePage.h"
#include "PopUpTopStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VEOffload.h"
#include "VledClamp.h"
#include "DigitalIO.h"
#include "VEClamp.h"
#include "VEIndex.h"
#include "Debug.h"

extern bool GetComputerClockFromReg(unsigned __int64 *frequency);

// CVledElitePage dialog
IMPLEMENT_DYNAMIC(CVledElitePage, CPropertyPage)

CVledElitePage::CVledElitePage()
	: CPropertyPage(CVledElitePage::IDD)
	, m_lDbCheckPos(0)
	, m_lInjectPos(0)
	, m_lOpenPos(0)
	, m_lClosePos(0)
{
}

CVledElitePage::~CVledElitePage()
{
}

void CVledElitePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_DB_CHECK_POS, m_lDbCheckPos);
    DDX_Text(pDX, IDC_EDIT_INJECT_POS, m_lInjectPos);
    DDX_Text(pDX, IDC_EDIT_CLAMP_OPEN_WIDTH, m_lOpenPos);
    DDX_Text(pDX, IDC_EDIT_CLAMP_CLOSE, m_lClosePos);
}

BEGIN_MESSAGE_MAP(CVledElitePage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_INJECT_HOME, OnBnClickedButtonInjectHome)
	ON_BN_CLICKED(IDC_BUTTON_INJECT_STOP, OnBnClickedButtonInjectStop)
	ON_BN_CLICKED(IDC_BUTTON_INJECT_POS1, OnBnClickedButtonInjectPos1)
	ON_BN_CLICKED(IDC_BUTTON_INJECT_POS2, OnBnClickedButtonInjectPos2)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_HOME, OnBnClickedButtonClampHome)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_OPEN, OnBnClickedButtonClampOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_CLOSE, OnBnClickedButtonClampClose)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_OPEN_WIDTH, OnEnSetfocusEditClampOpenWidth)
	ON_EN_SETFOCUS(IDC_EDIT_DB_CHECK_POS, OnEnSetfocusEditDbCheckPos)
	ON_EN_SETFOCUS(IDC_EDIT_INJECT_POS, OnEnSetfocusEditInjectPos)
	ON_EN_SETFOCUS(IDC_EDIT_LF_KO_POS, OnLfKickOutPos)
	ON_EN_SETFOCUS(IDC_EDIT_MS_POS_Y, OnMagSupportPosY)
	ON_EN_SETFOCUS(IDC_EDIT_MS_POS_Z, OnMagSupportPosZ)
	ON_EN_SETFOCUS(IDC_EDIT_MT_POS_Z, OnMagDropPosZ)
	ON_EN_SETFOCUS(IDC_EDIT_FIRST_LF_POS_Y, On1stSlotPosY)
	ON_EN_SETFOCUS(IDC_EDIT_FIRST_LF_POS_Z, On1stSlotPosZ)
	ON_EN_SETFOCUS(IDC_EDIT_MO_POS_Y, OnMagKickerPosY)
	ON_EN_SETFOCUS(IDC_EDIT_MO_POS_Z, OnMagKickerPosZ)
	ON_EN_SETFOCUS(IDC_EDIT_PITCH_WIDTH, OnPitchWidth)
	ON_EN_SETFOCUS(IDC_EDIT_PITCH_NUM, OnPitchNum)
	ON_BN_CLICKED(IDC_BUTTON_KICKE_HOME, OnKickerHome)
	ON_BN_CLICKED(IDC_BUTTON_KICKER_BK, OnKickerForward)
	ON_BN_CLICKED(IDC_BUTTON_KICKER_STOP, OnKickerStop)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_CLOSE, OnEnSetfocusEditClampClose)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_STOP, OnBnClickedButtonClampStop)
END_MESSAGE_MAP()

// CVledElitePage message handlers
BOOL CVledElitePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CFont font;
	font.CreateFont(20,0,0,0,500,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("Arial"));
	theApp.SetDlgFont(this, &font);	//V1.1.W139 add

	SetDlgItemText(IDC_STATIC_LF_CLAMP, theMsg.GetMsg(MSG_CLAMPER));
	SetDlgItemText(IDC_STATIC_OPEN_CLAMP, theMsg.GetMsg(MSG_OPEN_POS_COLON));
	SetDlgItemText(IDC_STATIC_CLOSE_CLAMP, theMsg.GetMsg(MSG_CLAMP_POS_COLON));
	SetDlgItemText(IDC_BUTTON_CLAMP_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemText(IDC_BUTTON_CLAMP_OPEN, theMsg.GetMsg(MSG_OPEN));
	SetDlgItemText(IDC_BUTTON_CLAMP_CLOSE, theMsg.GetMsg(MSG_CLOSE));
	SetDlgItemText(IDC_BUTTON_CLAMP_STOP, theMsg.GetMsg(MSG_STOP));
	SetDlgItemText(IDC_STATIC_INJECTOR, theMsg.GetMsg(MSG_INJECTOR_STEPPER));
	SetDlgItemText(IDC_STATIC_DBCHECK_POS, theMsg.GetMsg(MSG_DOUBLE_CHECK_POS));
	SetDlgItemText(IDC_STATIC_INJECTOR_POS, theMsg.GetMsg(MSG_INJECTOR_POS));
	SetDlgItemText(IDC_BUTTON_INJECT_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemText(IDC_BUTTON_INJECT_POS1, theMsg.GetMsg(MSG_CHECK_POS));
	SetDlgItemText(IDC_BUTTON_INJECT_POS2, theMsg.GetMsg(MSG_INJECT));
	SetDlgItemText(IDC_BUTTON_INJECT_STOP, theMsg.GetMsg(MSG_STOP));
	SetDlgItemText(IDC_STATIC_LF_KO_POS, theMsg.GetMsg(MSG_LF_KICK_OUT_POS));
	SetDlgItemText(IDC_STATIC_MAG_SUP_POS, theMsg.GetMsg(MSG_MAG_SUPPORT_POS));
	SetDlgItemText(IDC_STATIC_MAG_TAKE_POS, theMsg.GetMsg(MSG_MAG_DROP_POS));
	SetDlgItemText(IDC_STATIC_FIRST_LF_POS, theMsg.GetMsg(MSG_FIRST_LF_POS));
	SetDlgItemText(IDC_STATIC_MAG_OFFLOAD_POS, theMsg.GetMsg(MSG_MAG_OFFLOAD_POS));
	SetDlgItemText(IDC_STATIC_PITCH_WIDTH, theMsg.GetMsg(MSG_PITCH_WIDTH));
	SetDlgItemText(IDC_STATIC_OFFLOAD, theMsg.GetMsg(MSG_OFFLOAD));

	CWnd* pChildWnd = GetDlgItem(IDC_COMBO_ITEM);
	((CComboBox*)pChildWnd)->AddString(theMsg.GetMsg(MSG_LF_KICKER));
	((CComboBox*)pChildWnd)->AddString(theMsg.GetMsg(MSG_MAG_ARM_Y));
	((CComboBox*)pChildWnd)->AddString(theMsg.GetMsg(MSG_MAG_ARM_Z));
	((CComboBox*)pChildWnd)->AddString(theMsg.GetMsg(MSG_MAG_KICKER));
	((CComboBox*)pChildWnd)->SetCurSel(0);

	SetDlgItemText(IDC_BUTTON_KICKE_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemText(IDC_BUTTON_KICKER_BK, theMsg.GetMsg(MSG_FORWARD));
	SetDlgItemText(IDC_BUTTON_KICKER_STOP, theMsg.GetMsg(MSG_STOP));

	m_MagKickerHomeSensor.Initialize(VE_OFFLOAD_MAG_KICKER_HOME_SENSOR);
	m_MagKickerLimitSensor.Initialize(VE_OFFLOAD_MAG_KICKER_LIMIT_SENSOR);
	m_MagKicker.Initialize(VE_OFFLOAD_MAG_KICKER_ON,VE_OFFLOAD_MAG_KICKER_DIR,VE_OFFLOAD_MAG_KICKER_HOME_SENSOR,VE_OFFLOAD_MAG_KICKER_LIMIT_SENSOR);

	SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
	theVEliteTrack.GetRcd(&pstLfTrackRcd);
	m_lDbCheckPos = pstLfTrackRcd.lDbLfCheckPos;
	m_lInjectPos = pstLfTrackRcd.lInjectPos;

	SINGLE_CAP_CLAMP_RCD pstLfClampRcd;
	theVEliteLfClamp.GetRcd(&pstLfClampRcd);
	m_lOpenPos = pstLfClampRcd.lOpenPosition;
	m_lClosePos = pstLfClampRcd.lClosePosition;

	theVEliteOffload.GetRcd(&m_vor);

	SetDlgItemInt(IDC_EDIT_LF_KO_POS,m_vor.nLfKickDist);
	SetDlgItemInt(IDC_EDIT_MS_POS_Y,m_vor.nMagSupportPosY);
	SetDlgItemInt(IDC_EDIT_MS_POS_Z,m_vor.nMagSupportPosZ);
	SetDlgItemInt(IDC_EDIT_MT_POS_Z,m_vor.nMagDropPosZ);
	SetDlgItemInt(IDC_EDIT_FIRST_LF_POS_Y,m_vor.nMag1stSlotPosY);
	SetDlgItemInt(IDC_EDIT_FIRST_LF_POS_Z,m_vor.nMag1stSlotPosZ);
	SetDlgItemInt(IDC_EDIT_MO_POS_Y,m_vor.nMagKickerPosY);
	SetDlgItemInt(IDC_EDIT_MO_POS_Z,m_vor.nMagKickerPosZ);
	SetDlgItemInt(IDC_EDIT_PITCH_WIDTH,m_vor.nPitchWidth);
	SetDlgItemInt(IDC_EDIT_PITCH_NUM,m_vor.nMagSlotNo);

	UpdateData(FALSE);

	return FALSE;
}

HBRUSH CVledElitePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CVledElitePage::OnOK()
{
	UpdateData(true);

	SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
	theVEliteTrack.GetRcd(&pstLfTrackRcd);
	pstLfTrackRcd.lDbLfCheckPos = m_lDbCheckPos;
	pstLfTrackRcd.lInjectPos = m_lInjectPos ;
	theVEliteTrack.SetRcd(&pstLfTrackRcd);

	SINGLE_CAP_CLAMP_RCD pstLfClampRcd;
	theVEliteLfClamp.GetRcd(&pstLfClampRcd);
	pstLfClampRcd.lOpenPosition = m_lOpenPos;
	pstLfClampRcd.lClosePosition = m_lClosePos;
	theVEliteLfClamp.SetRcd(&pstLfClampRcd);

	theVEliteOffload.SetRcd(&m_vor);

	CPropertyPage::OnOK();
}

void CVledElitePage::OnBnClickedButtonInjectHome()
{
	theStepper.SearchHome(SINGLE_CAP_INJECTOR);
	Sleep(10);
    theVledTrack.CheckHomeState();
}

void CVledElitePage::OnBnClickedButtonInjectStop()
{
	theStepper.StopMotor(SINGLE_CAP_INJECTOR);
}

void CVledElitePage::OnBnClickedButtonInjectPos1()
{
    long lPos, lDesPos, lElapseTime, lStartTime;

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

void CVledElitePage::OnBnClickedButtonInjectPos2()
{
    long lPos, lDesPos, lElapseTime, lStartTime;

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

void CVledElitePage::OnBnClickedButtonClampHome()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteLfClamp.SearchClampHome();
		Sleep(10);
		theVEliteLfClamp.WaitStepHomeFinish();
		break;
	case MHS_VLED:
		theVledLfClamp.SearchClampHome();
		Sleep(10);
		theVledLfClamp.WaitStepHomeFinish();
		break;
	}
}

void CVledElitePage::OnBnClickedButtonClampOpen()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteLfClamp.OpenLfClamp();
		break;
	case MHS_VLED:
		theVledLfClamp.OpenLfClamp();
		break;
	}

    if (theDebug.IsDebugWorkHolder())
    {
		int nSensorState;
		do
		{
			Sleep(100);
			switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
			{
			case MHS_VLED_ELITE:
				nSensorState = (int)theStepper.IsBusy(VE_STEPPER_INDEX_CLAMPER);
				break;
			case MHS_VLED:
				nSensorState = (int)theStepper.IsBusy(SINGLE_CAP_CLAMPER);
				break;
			}
		} while (nSensorState != 0);
    }
}

void CVledElitePage::OnBnClickedButtonClampClose()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteLfClamp.CloseLfClamp();
		break;
	case MHS_VLED:
		theVledLfClamp.CloseLfClamp();
		break;
	}

    if (theDebug.IsDebugWorkHolder())
    {
		int nSensorState;
		do
		{
			Sleep(100);
			switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
			{
			case MHS_VLED_ELITE:
				nSensorState = (int)theStepper.IsBusy(VE_STEPPER_INDEX_CLAMPER);
				break;
			case MHS_VLED:
				nSensorState = (int)theStepper.IsBusy(SINGLE_CAP_CLAMPER);
				break;
			}
		} while (nSensorState != 0);
    }
}

void CVledElitePage::OnEnSetfocusEditDbCheckPos()
{
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

void CVledElitePage::OnEnSetfocusEditInjectPos()
{
	UpdateData(true);

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lInjectPos,8750,3000,10000);

    UpdateData(false);
    
    SINGLE_CAP_TRACK_RCD   pstLfTrackRcd;
	theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
   
	pstLfTrackRcd.lInjectPos    =m_lInjectPos ;
    theVledTrack.UpdateLfTrackRcd(&pstLfTrackRcd);
}

void CVledElitePage::OnLfKickOutPos()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if (theKeyPad.GetUserKeyInData(&m_vor.nLfKickDist, 6800, 0, 8000) == CANCEL_SELECTED) return;

	SetDlgItemInt(IDC_EDIT_LF_KO_POS,m_vor.nLfKickDist);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(0);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_KICKER,m_vor.nLfKickDist);
}

void CVledElitePage::OnMagSupportPosY()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMagSupportPosY,950,0,1000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_MS_POS_Y,m_vor.nMagSupportPosY);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(1);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Y,m_vor.nMagSupportPosY);
}

void CVledElitePage::OnMagSupportPosZ()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMagSupportPosZ,39550,38000,42000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_MS_POS_Z,m_vor.nMagSupportPosZ);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(2);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Z,m_vor.nMagSupportPosZ);
}

void CVledElitePage::OnMagDropPosZ()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMagDropPosZ,27300,10000,30000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_MT_POS_Z,m_vor.nMagDropPosZ);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(2);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Z,m_vor.nMagDropPosZ);
}

void CVledElitePage::On1stSlotPosY()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMag1stSlotPosY,1300,500,2000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_FIRST_LF_POS_Y,m_vor.nMag1stSlotPosY);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(1);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Y,m_vor.nMag1stSlotPosY);
}

void CVledElitePage::On1stSlotPosZ()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMag1stSlotPosZ,12500,5000,20000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_FIRST_LF_POS_Z,m_vor.nMag1stSlotPosZ);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(2);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Z,m_vor.nMag1stSlotPosZ);
}

void CVledElitePage::OnMagKickerPosY()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMagKickerPosY,35000,10000,36000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_MO_POS_Y,m_vor.nMagKickerPosY);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(1);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Y,m_vor.nMagKickerPosY);
}

void CVledElitePage::OnMagKickerPosZ()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMagKickerPosZ,0,0,1000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_MO_POS_Z,m_vor.nMagKickerPosZ);
	theVEliteOffload.SetRcd(&m_vor);

	((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->SetCurSel(2);

	theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Z,m_vor.nMagKickerPosZ);
}

void CVledElitePage::OnPitchWidth()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nPitchWidth,1070,500,2000)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_PITCH_WIDTH,m_vor.nPitchWidth);
	theVEliteOffload.SetRcd(&m_vor);
}

void CVledElitePage::OnPitchNum()
{
	GetDlgItem(IDC_STATIC_CLOSE_CLAMP)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_vor.nMagSlotNo,25,2,50)==CANCEL_SELECTED)return;

	SetDlgItemInt(IDC_EDIT_PITCH_NUM,m_vor.nMagSlotNo);
	theVEliteOffload.SetRcd(&m_vor);
}

void CVledElitePage::OnKickerHome()
{
	int nSel=((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->GetCurSel();

	if(!nSel)theStepper.SearchHome(VE_STEPPER_OFFLOAD_KICKER);
	else if(nSel==1)theStepper.SearchHome(VE_STEPPER_OFFLOAD_MAG_Y);
	else if(nSel==2)theStepper.SearchHome(VE_STEPPER_OFFLOAD_MAG_Z);
	else if(nSel==3)
	{
		m_MagKickerHomeSensor.UpdateSensorState();
		if (m_MagKickerHomeSensor.GetSensorState() != SENSOR_ON)
			m_MagKicker.TriggerBacktoHome();
	}

	Sleep(10);
}

DWORD WINAPI CVledElitePage::StopProc(LPVOID lpVoid)
{
	CVledElitePage* lpThis=(CVledElitePage*)lpVoid;

	Sleep(100);
	lpThis->m_MagKicker.StopMotion();

	return NOERROR;
}

void CVledElitePage::OnKickerForward()
{
	int nSel=((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->GetCurSel();

	if(!nSel)theStepper.MoveStep(VE_STEPPER_OFFLOAD_KICKER,1000,MOTION_RELATIVE);
	else if(nSel==1)theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Y,3000,MOTION_RELATIVE);
	else if(nSel==2)theStepper.MoveStep(VE_STEPPER_OFFLOAD_MAG_Z,3000,MOTION_RELATIVE);
	else if(nSel==3)
	{
		m_MagKickerLimitSensor.UpdateSensorState();
		if (m_MagKickerLimitSensor.GetSensorState() == SENSOR_ON) return;

		HANDLE hTread=CreateThread(NULL,0,StopProc,this,0,NULL);
		m_MagKicker.TriggerForwardMotion();

		WaitThread(hTread,200);		//V3.3.W180 edit
	}
}

void CVledElitePage::OnKickerStop()
{
	int nSel=((CComboBox*)GetDlgItem(IDC_COMBO_ITEM))->GetCurSel();

	if(!nSel)theStepper.StopMotor(VE_STEPPER_OFFLOAD_KICKER);
	else if(nSel==1)theStepper.StopMotor(VE_STEPPER_OFFLOAD_MAG_Y);
	else if(nSel==2)theStepper.StopMotor(VE_STEPPER_OFFLOAD_MAG_Z);
	else if(nSel==3)m_MagKicker.StopMotion();
}

void CVledElitePage::OnEnSetfocusEditClampClose()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lClosePos,0,0,2048);

    UpdateData(false);

    SINGLE_CAP_CLAMP_RCD pstLfClampRcd; //@1
	theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
    pstLfClampRcd.lClosePosition=m_lClosePos;
    theVledLfClamp.UpdateLfClampRcd(&pstLfClampRcd);
}

void CVledElitePage::OnEnSetfocusEditClampOpenWidth()
{
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_INJECT_STOP);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lOpenPos,2048,1000,4096);

    UpdateData(false);
    
	SINGLE_CAP_CLAMP_RCD pstLfClampRcd;
	theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
    pstLfClampRcd.lOpenPosition=m_lOpenPos;
    theVledLfClamp.UpdateLfClampRcd(&pstLfClampRcd);
}

void CVledElitePage::OnBnClickedButtonClampStop()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteLfClamp.StopMotion();
		break;
	case MHS_VLED:
		theVledLfClamp.StopMotion();
		break;
	}
}
