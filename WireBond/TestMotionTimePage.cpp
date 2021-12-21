// TestMotionTimePage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "TestMotionTimePage.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledClamp.h"
#include "VledOffload.h"
#include "MachineConfig.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "CuIndex.h"
#include "CuClamp.h"
#include "R2RIndex.h"
#include "R2ROnload.h"
#include "R2ROffload.h"
#include "TopEliteIndex.h"
#include "Busy.h"

// CTestMotionTimePage dialog
IMPLEMENT_DYNAMIC(CTestMotionTimePage, CPropertyPage)

CTestMotionTimePage::CTestMotionTimePage()
	: CPropertyPage(CTestMotionTimePage::IDD)
	, m_bCurveType(FALSE)
	, m_iMotorId(0)
	, m_lMoveDist(400)
	, m_lVelocity(6000)
	, m_lMaxVel(8000)
	, m_lAcc(600000)
	, m_lJerk(10000000)
	, m_bStepperAddr(0)
	, m_bMoionMode(FALSE)
{
	m_bBusy = FALSE;	//V1.4.W43 add

	stMotorSpeedProfile.bTCurveAcc			= 15;	//15:S, 0:T
	stMotorSpeedProfile.lAcc				= m_lAcc;
	stMotorSpeedProfile.lAccRatio			= m_lJerk;
	stMotorSpeedProfile.lDriveVelocity		= m_lVelocity;
	stMotorSpeedProfile.lMaxDriveVelocity	= m_lMaxVel;
	stMotorSpeedProfile.lStartVelocity		= 1500;
}

CTestMotionTimePage::~CTestMotionTimePage()
{
}

void CTestMotionTimePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_S_CURVE, m_bCurveType);
	DDX_CBIndex(pDX, IDC_COMBO_MOTOR, m_iMotorId);
	DDX_Text(pDX, IDC_EDIT_DIST, m_lMoveDist);
	DDX_Text(pDX, IDC_EDIT_VELOCITY, m_lVelocity);
	DDX_Text(pDX, IDC_EDIT_MAX_VEL, m_lMaxVel);
	DDX_Text(pDX, IDC_EDIT_ACC, m_lAcc);
	DDX_Text(pDX, IDC_EDIT_JERK, m_lJerk);
	DDX_Radio(pDX, IDC_RADIO_Absolute, m_bMoionMode);
}

BEGIN_MESSAGE_MAP(CTestMotionTimePage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_CBN_SELCHANGE(IDC_COMBO_MOTOR, OnMotor)
	ON_EN_SETFOCUS(IDC_EDIT_DIST, OnFocusDist)
	ON_EN_SETFOCUS(IDC_EDIT_VELOCITY, OnFocusVelocity)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_VEL, OnFocusMaxVel)
	ON_EN_SETFOCUS(IDC_EDIT_ACC, OnFocusAcc)
	ON_EN_SETFOCUS(IDC_EDIT_JERK, OnFocusJerk)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnClear)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.4.W43 add
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTestMotionTimePage message handlers
BOOL CTestMotionTimePage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->AddString(_T("Testing Result:"));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTOR);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		pComboBox->AddString(_T("Onload Elevator"));
		pComboBox->AddString(_T("Window Clamp"));
		pComboBox->AddString(_T("Indexer"));
		pComboBox->AddString(_T("Offload Elevator"));
		m_bStepperAddr = theTopTrack.GetIndexStepID();
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		pComboBox->AddString(_T("Onload Elevator"));
		pComboBox->AddString(_T("Window Clamp"));
		pComboBox->AddString(_T("Indexer"));
		pComboBox->AddString(_T("Offload Elevator"));
		m_bStepperAddr = theTEIndex.GetIndexStepID();
		break;
	}
	case MHS_R2R:
	{
		pComboBox->AddString(_T("Onloader"));
		pComboBox->AddString(_T("Clamper"));
		pComboBox->AddString(_T("Indexer"));
		pComboBox->AddString(_T("Offloader"));
		m_bStepperAddr = theR2RTrack.m_ucIndexerStepperId;
		break;
	}
	case MHS_CU_ELITE:
	{
		pComboBox->AddString(_T("Onload Elevator"));
        pComboBox->AddString(_T("Onload X Axis"));
        pComboBox->AddString(_T("Onload Y1 Axis"));
        pComboBox->AddString(_T("Onload Y2 Axis"));
        pComboBox->AddString(_T("Offload Elevator"));
        pComboBox->AddString(_T("Offload X Axis"));
        pComboBox->AddString(_T("Offload Y1 Axis"));
        pComboBox->AddString(_T("Offload Y2 Axis"));
        pComboBox->AddString(_T("Front Rail"));
		pComboBox->AddString(_T("Clamper"));
		pComboBox->AddString(_T("Indexer"));
		pComboBox->AddString(_T("Rear Rail"));
		m_bStepperAddr = CUELITE_DEVICE_INDEXER;
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		pComboBox->AddString(theMsg.GetMsg(MSG_ONLOAD_PICKER));
		pComboBox->AddString(theMsg.GetMsg(MSG_INJECTOR_STEPPER));
		pComboBox->AddString(theMsg.GetMsg(MSG_OFFLOAD_KICKER));
		pComboBox->AddString(theMsg.GetMsg(MSG_WINDOW_CLAMP));
		pComboBox->AddString(theMsg.GetMsg(MSG_LEFT_CLAW));
		pComboBox->AddString(theMsg.GetMsg(MSG_RIGHT_CLAW));
		m_bStepperAddr = SINGLE_CAP_ONLOAD_PICKER;
		break;
	}
	}

	m_iMotorId = 2;

	return CPropertyPage::OnInitDialog();
}

HBRUSH CTestMotionTimePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTestMotionTimePage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.4.W43 add

	UpdateData(true);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
	
	if (m_bCurveType == FALSE)
		stMotorSpeedProfile.bTCurveAcc = 15;
	else
		stMotorSpeedProfile.bTCurveAcc = 0;

	stMotorSpeedProfile.lAcc = m_lAcc;
	stMotorSpeedProfile.lAccRatio = m_lJerk;
	stMotorSpeedProfile.lDriveVelocity = m_lVelocity;
	stMotorSpeedProfile.lMaxDriveVelocity = m_lMaxVel;

	theStepper.SetSpeedProfile(m_bStepperAddr, &stMotorSpeedProfile);
	Sleep(100);

	if (m_bMoionMode == FALSE)
		theStepper.MoveStep(m_bStepperAddr, m_lMoveDist);
	else
		theStepper.MoveStep(m_bStepperAddr, m_lMoveDist, MOTION_RELATIVE);

	CHTimer timer; double dTime_x1ms;
	do
	{
		dTime_x1ms = timer.GetTime_x1ms();
		Sleep_x1ms(10);
	} while (theStepper.IsBusy(m_bStepperAddr) && (dTime_x1ms < 10000));

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);

	CString str;
	str.Format(_T("Duration: %.3fms"), dTime_x1ms);
	pListBox->AddString(str);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
}

void CTestMotionTimePage::OnMotor()
{
	UpdateData(true);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		switch(m_iMotorId)
		{
		case 0:
			m_bStepperAddr = theTopOnloader.m_nMagHolderStepperId;
			break;
		case 1:
			m_bStepperAddr = theTopClamp.GetStepID();
			break;
		case 2:
			m_bStepperAddr = theTopTrack.GetIndexStepID();
			break;
		case 3:
			m_bStepperAddr = theTopOffloader.m_nMagHolderStepperId;
			break;
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		switch(m_iMotorId)
		{
		case 0:
			m_bStepperAddr = theTopOnloader.m_nMagHolderStepperId;
			break;
		case 1:
			m_bStepperAddr = theTopClamp.GetStepID();
			break;
		case 2:
			m_bStepperAddr = theTEIndex.GetIndexStepID();
			break;
		case 3:
			m_bStepperAddr = theTopOffloader.m_nMagHolderStepperId;
			break;
		}
		break;
	}
	case MHS_R2R:
	{
		switch(m_iMotorId)
		{
		case 0:
			m_bStepperAddr = theR2ROnloader.m_ucForkHolderStepId;   
			break;
		case 1:
			m_bStepperAddr = theTopClamp.GetStepID(); 
			break;
		case 2:
			m_bStepperAddr = theR2RTrack.m_ucIndexerStepperId;     
			break;
		case 3:
			m_bStepperAddr = theR2ROffloader.m_ucForkHolderStepId;  
			break;	   
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		switch(m_iMotorId)
		{
		case 0:
		    m_bStepperAddr = CUELITE_ONLOAD_ELEVATOR;
			break;
		case 1:
		    m_bStepperAddr = CUELITE_ONLOAD_XAXIS ;
			break;
		case 2:
		    m_bStepperAddr = CUELITE_ONLOAD_YONE;
			break;
		case 3:
		    m_bStepperAddr = CUELITE_ONLOAD_YTWO;
			break;
        case 4:
		    m_bStepperAddr = CUELITE_OFFLOAD_ELEVATOR;
			break;
		case 5:
		    m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
			break;
		case 6:
		    m_bStepperAddr = CUELITE_OFFLOAD_YONE;
			break;
		case 7:
		    m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
			break;
        case 8:
		    m_bStepperAddr = CUELITE_FRONT_RAIL;
			break;
		case 9:
		    m_bStepperAddr = CUELITE_WINDOW_CLAMPER;
			break;
		case 10:
		    m_bStepperAddr = CUELITE_DEVICE_INDEXER;
			break;
		case 11:
		    m_bStepperAddr = CUELITE_REAR_RAIL;
			break;
		}
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		switch(m_iMotorId)
		{
		case 0:
			m_bStepperAddr = SINGLE_CAP_ONLOAD_PICKER;
			break;
		case 1:
			m_bStepperAddr = SINGLE_CAP_INJECTOR;
			break;
		case 2:
			m_bStepperAddr = SINGLE_CAP_OFFLOAD_KICKER;
			break;
		case 3:
			m_bStepperAddr = SINGLE_CAP_CLAMPER;
			break;
		case 4:
			m_bStepperAddr = SINGLE_CAP_LEFT_CLAW;
			break;
		case 5:
			m_bStepperAddr = SINGLE_CAP_RIGHT_CLAW;
			break;
		default:
			m_bStepperAddr = SINGLE_CAP_ONLOAD_PICKER;
			break;
		}
		break;
	}
	}
}

void CTestMotionTimePage::OnFocusDist()
{
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lMoveDist);
	UpdateData(false);
}

void CTestMotionTimePage::OnFocusVelocity()
{
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lVelocity);
	UpdateData(false);
}

void CTestMotionTimePage::OnFocusMaxVel()
{
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lMaxVel);
	UpdateData(false);
}

void CTestMotionTimePage::OnFocusAcc()
{
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lAcc);
	UpdateData(false);
}

void CTestMotionTimePage::OnFocusJerk()
{
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lJerk);
	UpdateData(false);
}

void CTestMotionTimePage::OnClear()
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
	pListBox->AddString(_T("Testing Result:"));
}

LRESULT CTestMotionTimePage::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.4.W43 add
{
	return m_bBusy;
}

void CTestMotionTimePage::OnDestroy()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED:
	{
		theTopClamp.Init();
		theTopTrack.Init();
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTopClamp.Init();
		theTEIndex.Init();
		break;
	}
	case MHS_R2R:
	{
		theTopClamp.Init();
		theR2RTrack.Initialize();
		break;
	}
	case MHS_CU_ELITE:
	{
		theCuEliteLFClamp.Init();
		theCuEliteTrack.Initialize();
		break;
	}
	case MHS_VLED:
	{
		theVledOnload.Initialize();
		theVledOffload.Initialize();
		theVledTrack.Initialize();
		theVledLfClamp.Initialize();
		break;
	}
	}

	CPropertyPage::OnDestroy();
}
