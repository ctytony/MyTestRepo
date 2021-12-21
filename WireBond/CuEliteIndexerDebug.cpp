// CuEliteIndexerDebug.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "CuEliteIndexerDebug.h"
#include "ScanKeyThread.h"
#include "MachineConfig.h"
#include "CuIndex.h"
#include "CuClamp.h"
#include "servo.h"

IMPLEMENT_DYNAMIC(CCuEliteIndexerDebug, CPropertyPage)

CCuEliteIndexerDebug::CCuEliteIndexerDebug()
	: CPropertyPage(CCuEliteIndexerDebug::IDD)
	, m_lScale(1000)
	, m_lPreclosePercent(0)
	, m_lSafeClosePercent(0)
    , m_lClampPosition(0)
    , m_lJerk(0)
    , m_StepMotorId(0)
{
	m_bStepperAddr=CUELITE_DEVICE_INDEXER;
}

CCuEliteIndexerDebug::~CCuEliteIndexerDebug()
{
}

void CCuEliteIndexerDebug::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SCALE, m_lScale);
    DDX_Text(pDX, IDC_EDIT_PRECLAMP_PERCENT, m_lPreclosePercent);
    DDX_Text(pDX, IDC_EDIT_CLAMP_PERCENT, m_lSafeClosePercent);
    DDX_Text(pDX, IDC_CLAMP_POSITION, m_lClampPosition);
    DDX_Text(pDX, IDC_EDIT2, m_lJerk);
    DDX_CBIndex(pDX, IDC_COMBO_INDEX_STEPPERS, m_StepMotorId);
}

BEGIN_MESSAGE_MAP(CCuEliteIndexerDebug, CPropertyPage)
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_HOME_STEP, &CCuEliteIndexerDebug::OnBnClickedButtonHomeStep)
	ON_EN_SETFOCUS(IDC_EDIT_SCALE, &CCuEliteIndexerDebug::OnEnSetfocusEditScale)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_HOME, &CCuEliteIndexerDebug::OnBnClickedButtonClampHome)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_CLOSE, &CCuEliteIndexerDebug::OnBnClickedButtonClampClose)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_OPEN, &CCuEliteIndexerDebug::OnBnClickedButtonClampOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_PRECLOSE, &CCuEliteIndexerDebug::OnBnClickedButtonClampPreclose)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_PERCENT, &CCuEliteIndexerDebug::OnEnSetfocusEditClampPercent)
	ON_EN_SETFOCUS(IDC_EDIT_PRECLAMP_PERCENT, &CCuEliteIndexerDebug::OnEnSetfocusEditPreclampPercent)
	ON_EN_CHANGE(IDC_EDIT_CLAMP_PERCENT, &CCuEliteIndexerDebug::OnEnChangeEditClampPercent)
	ON_EN_CHANGE(IDC_EDIT_PRECLAMP_PERCENT, &CCuEliteIndexerDebug::OnEnChangeEditPreclampPercent)
	ON_BN_CLICKED(IDC_BUTTON_STAND_BY, &CCuEliteIndexerDebug::OnBnClickedButtonStandBy)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_REMOVE, &CCuEliteIndexerDebug::OnBnClickedButtonClampRemove)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP__REMOVE_DONE, &CCuEliteIndexerDebug::OnBnClickedButtonClamp)
    ON_EN_SETFOCUS(IDC_CLAMP_POSITION, &CCuEliteIndexerDebug::OnEnSetfocusClampPosition)
    ON_EN_SETFOCUS(IDC_EDIT2, &CCuEliteIndexerDebug::OnEnSetfocusEdit2)
  	ON_CBN_SELCHANGE(IDC_COMBO_INDEX_STEPPERS, &CCuEliteIndexerDebug::OnLbnSelchangeListStepperMotor)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_TRACK, &CCuEliteIndexerDebug::OnBnClickedButtonOpenTrack)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_TRACK, &CCuEliteIndexerDebug::OnBnClickedButtonCloseTrack)
END_MESSAGE_MAP()	
    
// CTopClampPage message handlers
BOOL CCuEliteIndexerDebug::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
	theApp.SetDlgFont(this);	//V1.1.W139 add
	GetDlgItem(IDC_BUTTON_DUMMY)->SetWindowPos(NULL, 1000, 1000, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    SetDlgItemTextW(IDC_STATIC_CLAMP_POSITION, theMsg.GetMsg(MSG_CLAMP_POS_COLON));
    SetDlgItemTextW(IDC_STATIC_MOTOR, theMsg.GetMsg(MSG_MOTOR));
    SetDlgItemTextW(IDC_STATIC_ACC_RATIO, theMsg.GetMsg(MSG_ACC_RATIO));
    SetDlgItemTextW(IDC_STATIC_CLAMPER, theMsg.GetMsg(MSG_CLAMPER));
    SetDlgItemTextW(IDC_STATIC_OPEN_POS, theMsg.GetMsg(MSG_OPEN_POS_COLON));
    SetDlgItemTextW(IDC_STATIC_PRE_CLAMP_POS, theMsg.GetMsg(MSG_PRECLAMP_POS));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_HOME, theMsg.GetMsg(MSG_HOME));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_OPEN, theMsg.GetMsg(MSG_OPEN));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_PRECLOSE, theMsg.GetMsg(MSG_PRECLAMP));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_CLOSE, theMsg.GetMsg(MSG_CLOSE));
    SetDlgItemTextW(IDC_STATIC_INDEXER, theMsg.GetMsg(MSG_INDEXER));
	SetDlgItemTextW(IDC_STATIC_MOVE_DIST, theMsg.GetMsg(MSG_MOVE_DIST) + TEXT(":"));
    SetDlgItemTextW(IDC_BUTTON_HOME_STEP, theMsg.GetMsg(MSG_HOME));
    SetDlgItemTextW(IDC_BUTTON_STAND_BY, theMsg.GetMsg(MSG_STANDBY_POS));
    SetDlgItemTextW(IDC_STATIC_REMOVE_CLAMP, theMsg.GetMsg(MSG_REMOVE_WINDOW_CLAMP));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_REMOVE, theMsg.GetMsg(MSG_REMOVE));
    SetDlgItemTextW(IDC_BUTTON_CLAMP__REMOVE_DONE, theMsg.GetMsg(MSG_RECOVER));

    CWnd* pWnd = GetDlgItem(IDC_COMBO_INDEX_STEPPERS);
    ((CComboBox*)pWnd)->ResetContent();
    ((CComboBox*)pWnd)->InsertString(0, theMsg.GetMsg(MSG_INDEXER));
	((CComboBox*)pWnd)->InsertString(1, theMsg.GetMsg(MSG_FRONT_RAIL));
    ((CComboBox*)pWnd)->InsertString(2, theMsg.GetMsg(MSG_REAR_RAIL));

	SetDlgItemTextW(IDC_BUTTON_OPEN_TRACK, theMsg.GetMsg(MSG_OPEN_TRACK));
	SetDlgItemTextW(IDC_BUTTON_CLOSE_TRACK, theMsg.GetMsg(MSG_CLOSE_TRACK));
	SetDlgItemTextW(IDC_BUTTON_CLAMP__REMOVE_DONE, theMsg.GetMsg(MSG_RECOVER));

    return FALSE;
}

BOOL CCuEliteIndexerDebug::OnSetActive( )
{
	CString sPath;

	if (CPropertyPage::OnSetActive()==FALSE)return TRUE;

    CUELITE_STEPPER_CLAMP_PARA* lpstCuElitePosPara=new CUELITE_STEPPER_CLAMP_PARA;
    theCuEliteLFClamp.GetClampRcd(lpstCuElitePosPara);

    m_lSafeClosePercent = lpstCuElitePosPara->lWindClampSafeClosePos;
    m_lPreclosePercent  = lpstCuElitePosPara->lWindClampPreClosePos;
    m_lClampPosition    = lpstCuElitePosPara->lWindClampClosePos;

	delete lpstCuElitePosPara;

	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		ST_AXIS_PARA* lpstWindowClampParam=new ST_AXIS_PARA;

		theCuEliteLFClamp.GetSpeedParam(lpstWindowClampParam);
		m_lJerk = lpstWindowClampParam->Vel;

		delete lpstWindowClampParam;
	}
	else
	{
		STEP_SPEED_PROFILE* lpstSpeedProfile = new STEP_SPEED_PROFILE;

		theCuEliteLFClamp.GetSpeedProfile(lpstSpeedProfile);
		m_lJerk = lpstSpeedProfile->lDriveVelocity;
		if (m_lJerk < 100) m_lJerk *= 1000;

		delete lpstSpeedProfile;
	}
   
	UpdateData(false);

	return TRUE;
}

HBRUSH CCuEliteIndexerDebug::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCuEliteIndexerDebug::OnBnClickedButtonHomeStep()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);
	pWnd->EnableWindow(FALSE);

    if (m_StepMotorId == ID_INDEX_MOTOR)
    {
	    theCuEliteTrack.SearchIndexStepHome();
	    Sleep(10);
	    theCuEliteTrack.WaitStepHomeFinish();
    }
    else if(m_StepMotorId == ID_FRONTRAIL_MOTOR)
    {
	    //theCuEliteTrack.SearchFrontRailStepHome();
	    //Sleep(10);
	    //theCuEliteTrack.WaitFrontRailStepHomeFinish();
    }
    else if(m_StepMotorId == ID_REARRAIL_MOTOR)
    {
	    //theCuEliteTrack.SearchRearRailStepHome();
	    //Sleep(10);
	    //theCuEliteTrack.WaitRearRailStepHomeFinish();
    }

	pWnd->EnableWindow(TRUE);
}

void CCuEliteIndexerDebug::OnEnSetfocusEditScale()
{
	 GetDlgItem(IDC_BUTTON_HOME_STEP)->SetFocus();

     theKeyPad.GetUserKeyInData(&m_lScale);

     UpdateData(false);
}

void CCuEliteIndexerDebug::OnBnClickedButtonClampHome()
{
	theServo.MoveBondHeadToSafePos();
	Sleep(100);  

	theCuEliteLFClamp.SearchHome();
	Sleep(100);
	theCuEliteLFClamp.WaitHomeFinish();

	Sleep(500);
	theCuEliteLFClamp.PreCloseClamp();
}

void CCuEliteIndexerDebug::OnBnClickedButtonClampClose()
{
	theCuEliteLFClamp.CloseClamp();
}

void CCuEliteIndexerDebug::OnBnClickedButtonClampOpen()
{
	theServo.MoveBondHeadToSafePos();
	Sleep(100);  

	theCuEliteLFClamp.MoveClampSafePos();
}

void CCuEliteIndexerDebug::OnBnClickedButtonClampPreclose()
{
	theCuEliteLFClamp.PreCloseClamp();
}

void CCuEliteIndexerDebug::OnEnSetfocusEditClampPercent()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lSafeClosePercent,50,0,100);

    UpdateData(false);

	OnEnChangeEditClampPercent();
}

void CCuEliteIndexerDebug::OnEnSetfocusEditPreclampPercent()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lPreclosePercent,80,0,100);

    UpdateData(false);

	OnEnChangeEditPreclampPercent();
}

void CCuEliteIndexerDebug::OnEnChangeEditClampPercent()
{
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(true);

	CUELITE_STEPPER_CLAMP_PARA* lpstCuElitePosPara=new CUELITE_STEPPER_CLAMP_PARA;

	theCuEliteLFClamp.GetClampRcd(lpstCuElitePosPara);
	lpstCuElitePosPara->lWindClampSafeClosePos =m_lSafeClosePercent;
	theCuEliteLFClamp.SetClampRcd(lpstCuElitePosPara);

	delete lpstCuElitePosPara;
}

void CCuEliteIndexerDebug::OnEnChangeEditPreclampPercent()
{
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(true);

	CUELITE_STEPPER_CLAMP_PARA* lpstCuElitePosPara=new CUELITE_STEPPER_CLAMP_PARA;

	theCuEliteLFClamp.GetClampRcd(lpstCuElitePosPara);
	lpstCuElitePosPara->lWindClampPreClosePos = m_lPreclosePercent;
	theCuEliteLFClamp.SetClampRcd(lpstCuElitePosPara);

	delete lpstCuElitePosPara;
}

void CCuEliteIndexerDebug::OnOK()
{
	UpdateData(true);

	CUELITE_STEPPER_CLAMP_PARA* lpstCuElitePosPara=new CUELITE_STEPPER_CLAMP_PARA;
	theCuEliteLFClamp.GetClampRcd(lpstCuElitePosPara);

	lpstCuElitePosPara->lWindClampSafeClosePos =m_lSafeClosePercent;
	lpstCuElitePosPara->lWindClampPreClosePos = m_lPreclosePercent;

	theCuEliteLFClamp.SetClampRcd(lpstCuElitePosPara);
	delete lpstCuElitePosPara;

	theScanKeyThread.EnableJoyStick();

	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		ST_AXIS_PARA* lpstWindowClampParam = new ST_AXIS_PARA;
		theCuEliteLFClamp.GetSpeedParam(lpstWindowClampParam);
		lpstWindowClampParam->Vel = m_lJerk;
		theCuEliteLFClamp.SetSpeedParam(lpstWindowClampParam);
		theStepper.SetSpeedParameter(theCuEliteLFClamp.GetStepperId(), *lpstWindowClampParam);
		delete lpstWindowClampParam;
	}
	else
	{
		STEP_SPEED_PROFILE* lpstSpeedProfile = new STEP_SPEED_PROFILE;
		theCuEliteLFClamp.GetSpeedProfile(lpstSpeedProfile);
		lpstSpeedProfile->lDriveVelocity = m_lJerk;
		theCuEliteLFClamp.SetSpeedProfile(lpstSpeedProfile);
		theStepper.SetSpeedParameter(theCuEliteLFClamp.GetStepperId(), lpstSpeedProfile, true);
		delete lpstSpeedProfile;
	}

	CPropertyPage::OnOK();
}

void CCuEliteIndexerDebug::OnBnClickedButtonStandBy()
{
    if (m_StepMotorId == ID_INDEX_MOTOR)
	{
	    theCuEliteTrack.MoveToLogicZero();
	}
    else if (m_StepMotorId == ID_FRONTRAIL_MOTOR)
    {
		//theCuEliteTrack.moveFrontRailToStandbyPos();
    }
    else if( m_StepMotorId == ID_REARRAIL_MOTOR)
    {
		//theCuEliteTrack.moveRearRailToStandbyPos();
    }
}

void CCuEliteIndexerDebug::OnBnClickedButtonClampRemove()
{
	GetDlgItem(IDC_BUTTON_CLAMP_REMOVE)->EnableWindow(FALSE);
	theScanKeyThread.DisableJoyStick();
	theServo.MoveBondHeadToSafePos();
	Sleep(100);

	theCuEliteLFClamp.OpenClamp();
	Sleep(200);

	theCuEliteTrack.MoveToLogicZero();

	GetParent()->GetDlgItem(IDOK)->EnableWindow(FALSE);
}

void CCuEliteIndexerDebug::OnBnClickedButtonClamp()
{
	GetDlgItem(IDC_BUTTON_CLAMP_REMOVE)->EnableWindow(TRUE);

	theCuEliteLFClamp.PreCloseClamp();

	Sleep(500);

	theScanKeyThread.EnableJoyStick();

	GetParent()->GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CCuEliteIndexerDebug::OnEnSetfocusClampPosition()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lClampPosition,10,0,40);

    UpdateData(false);

	CUELITE_STEPPER_CLAMP_PARA  stCuElitePosPara;

	theCuEliteLFClamp.GetClampRcd(&stCuElitePosPara);

    stCuElitePosPara.lWindClampClosePos = m_lClampPosition;

	theCuEliteLFClamp.SetClampRcd(&stCuElitePosPara);
}

void CCuEliteIndexerDebug::OnEnSetfocusEdit2()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    
     theKeyPad.GetUserKeyInData(&m_lJerk,10000,100,100000);

   //w.temp theKeyPad.GetUserKeyInData(&m_lJerk,20000,100,100000);

    UpdateData(false);

	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		ST_AXIS_PARA stWindowClampParam;
		theCuEliteLFClamp.GetSpeedParam(&stWindowClampParam);
		stWindowClampParam.Vel = m_lJerk;
		theCuEliteLFClamp.SetSpeedParam(&stWindowClampParam);

		theStepper.SetSpeedParameter(theCuEliteLFClamp.GetStepperId(), stWindowClampParam);
	}
	else
	{
		STEP_SPEED_PROFILE stSpeedProfile;

		theCuEliteLFClamp.GetSpeedProfile(&stSpeedProfile);
		stSpeedProfile.lDriveVelocity = m_lJerk;
		if (stSpeedProfile.lDriveVelocity < 1000)
			stSpeedProfile.lDriveVelocity = 1000;

		theCuEliteLFClamp.SetSpeedProfile(&stSpeedProfile);

		theStepper.SetSpeedParameter(theCuEliteLFClamp.GetStepperId(), &stSpeedProfile, true);
	}
}

void CCuEliteIndexerDebug::OnLbnSelchangeListStepperMotor()
{
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_INDEX_STEPPERS);
    m_StepMotorId = pComboBox->GetCurSel();
    switch( m_StepMotorId)
    {
        case ID_INDEX_MOTOR:
            m_bStepperAddr= CUELITE_DEVICE_INDEXER;
            break;
        case ID_FRONTRAIL_MOTOR:
            m_bStepperAddr= CUELITE_FRONT_RAIL ;
            break;
        case ID_REARRAIL_MOTOR:
            m_bStepperAddr= CUELITE_REAR_RAIL;
            break;
    }
}

void CCuEliteIndexerDebug::OnBnClickedButtonOpenTrack()
{
	short index[1]={MSG_SURE_TO_PRECLAMP};
	if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
	{
		theCuEliteLFClamp.PreCloseClamp();

		theCuEliteTrack.MoveFrontTrackHome();
		Sleep(10);
		theCuEliteTrack.MoveBackTrackHome();
		Sleep(10);
	}
}

void CCuEliteIndexerDebug::OnBnClickedButtonCloseTrack()
{
	theCuEliteTrack.MoveFrontTrackToStandbyPos();
	Sleep(10);
	theCuEliteTrack.MoveBackTrackToStandbyPos();
	Sleep(500);
	theCuEliteTrack.WaitFrontTrackStepHomeFinish();
	Sleep(10);
	theCuEliteTrack.WaitBackTrackStepHomeFinish();
}
