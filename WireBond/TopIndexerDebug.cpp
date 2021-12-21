// TopIndexerDebug.cpp : implementation file

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
#include "TopIndexerDebug.h"
#include "ScanKeyThread.h"
#include "MachineConfig.h"
#include "TopClamp.h"
#include "R2RIndex.h"
#include "servo.h"
#include "Busy.h"
#include "FoxconnIndex.h"
#include "TopEliteIndex.h"

// CTopClampPage dialog
IMPLEMENT_DYNAMIC(CTopIndexerDebug, CPropertyPage)

CTopIndexerDebug::CTopIndexerDebug()
	: CPropertyPage(CTopIndexerDebug::IDD)
	, m_lScale(1000)
	, m_lPreclosePercent(0)
	, m_lSafeClosePercent(0)
    , m_lClampPosition(0)
    , m_lJerk(0)
{
	m_bBusy = FALSE;

	switch (theMcConfig.GetMhsType())
	{
	case MHS_TOP_LED:
		m_bStepperAddr = theTopTrack.GetIndexStepID();
		break;
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
		m_bStepperAddr = theTEIndex.GetIndexStepID();
		break;
	case MHS_FOXCONN:
		m_bStepperAddr = theFoxconnTrack.GetStepperID();
		break;
	}
}

CTopIndexerDebug::~CTopIndexerDebug()
{
}

void CTopIndexerDebug::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SCALE, m_lScale);
    DDX_Text(pDX, IDC_EDIT_PRECLAMP_PERCENT, m_lPreclosePercent);
    DDX_Text(pDX, IDC_EDIT_CLAMP_PERCENT, m_lSafeClosePercent);
    DDX_Text(pDX, IDC_CLAMP_POSITION, m_lClampPosition);
    DDX_Text(pDX, IDC_EDIT2, m_lJerk);
}

BEGIN_MESSAGE_MAP(CTopIndexerDebug, CPropertyPage)
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FORWARD_, OnForward)
	ON_BN_CLICKED(IDC_BUTTON_BACKWARD_, OnBackward)
	ON_BN_CLICKED(IDC_BUTTON_HOME_STEP, OnHomeStep)
	ON_EN_SETFOCUS(IDC_EDIT_SCALE, OnFocusScale)
	ON_EN_CHANGE(IDC_EDIT_SCALE, OnEditScale)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_HOME, OnClampHome)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_CLOSE, OnClampClose)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_OPEN, OnClampOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_PRECLOSE, OnClampPreclose)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_PERCENT, OnFocusClampPercent)
	ON_EN_SETFOCUS(IDC_EDIT_PRECLAMP_PERCENT, OnFocusPreclampPercent)
	ON_EN_CHANGE(IDC_EDIT_CLAMP_PERCENT, OnEditClampPercent)
	ON_EN_CHANGE(IDC_EDIT_PRECLAMP_PERCENT, OnEditPreclampPercent)
	ON_BN_CLICKED(IDC_BUTTON_STAND_BY, OnStandBy)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_REMOVE, OnClampRemove)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP__REMOVE_DONE, OnClamp)
    ON_EN_SETFOCUS(IDC_CLAMP_POSITION, OnFocusClampPosition)
    ON_EN_SETFOCUS(IDC_EDIT2, OnFocus2)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.4.W23 add
	ON_WM_DESTROY()		//V1.4.W23 add
END_MESSAGE_MAP()	
    
// CTopClampPage message handlers
BOOL CTopIndexerDebug::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_STATIC_CLAMPER,theMsg.GetMsg(MSG_CLAMPER));
    SetDlgItemTextW(IDC_STATIC_OPEN_POS,theMsg.GetMsg(MSG_OPEN_POS_COLON));
    SetDlgItemTextW(IDC_STATIC_PRE_CLAMP_POS,theMsg.GetMsg(MSG_PRECLAMP_POS));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_HOME,theMsg.GetMsg(MSG_HOME));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_OPEN,theMsg.GetMsg(MSG_OPEN));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_PRECLOSE,theMsg.GetMsg(MSG_PRECLAMP));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_CLOSE,theMsg.GetMsg(MSG_CLOSE));
    SetDlgItemTextW(IDC_STATIC_INDEXER,theMsg.GetMsg(MSG_INDEXER));
    SetDlgItemTextW(IDC_STATIC_SCALE,theMsg.GetMsg(MSG_ADJUST_STEP));
    SetDlgItemTextW(IDC_BUTTON_HOME_STEP,theMsg.GetMsg(MSG_HOME));
    SetDlgItemTextW(IDC_BUTTON_BACKWARD_,theMsg.GetMsg(MSG_BACKWARD));
    SetDlgItemTextW(IDC_BUTTON_FORWARD_,theMsg.GetMsg(MSG_FORWARD));
    SetDlgItemTextW(IDC_BUTTON_STAND_BY,theMsg.GetMsg(MSG_STANDBY_POS));
    SetDlgItemTextW(IDC_STATIC_REMOVE_CLAMP,theMsg.GetMsg(MSG_REMOVE_WINDOW_CLAMP));
    SetDlgItemTextW(IDC_BUTTON_CLAMP_REMOVE,theMsg.GetMsg(MSG_REMOVE));
    SetDlgItemTextW(IDC_BUTTON_CLAMP__REMOVE_DONE,theMsg.GetMsg(MSG_RECOVER));
	SetDlgItemTextW(IDC_STATIC_CLAMP_POSITION,theMsg.GetMsg(MSG_CLAMP_POS_COLON));	//V1.1.W149 add
	SetDlgItemTextW(IDC_STATIC_SPEED,theMsg.GetMsg(MSG_DRIVE_SPEED));	//V1.1.W149 add

    return FALSE;
}

BOOL CTopIndexerDebug::OnSetActive( )
{
	if (CPropertyPage::OnSetActive() == FALSE) return TRUE;

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
	m_lSafeClosePercent = pstTopClamp->lWindClampSafeClosePos;
    m_lPreclosePercent  = pstTopClamp->lWindClampPreClosePos;
    m_lClampPosition    = pstTopClamp->lWindClampClosePos;

	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		ST_AXIS_PARA stWindowClampParam;
		theTopClamp.GetAxisPara(&stWindowClampParam);
		m_lJerk = stWindowClampParam.Vel;
	}
	else
	{
		STEP_SPEED_PROFILE stSpeedProfile;
		theTopClamp.GetSpeedProfile(&stSpeedProfile);
		m_lJerk = stSpeedProfile.lDriveVelocity;
		if (m_lJerk < 100) m_lJerk *= 1000;
	}
   
	return UpdateData(false);
}

HBRUSH CTopIndexerDebug::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTopIndexerDebug::OnForward()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	switch (theMcConfig.GetMhsType())
	{
	case MHS_TOP_LED:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		theStepper.MoveStep(m_bStepperAddr, round(m_lScale/theStepper.GetResol(m_bStepperAddr)), MOTION_RELATIVE);
		break;
	case MHS_FOXCONN:
		theFoxconnTrack.MoveIndexRight(m_lScale);
		//theFoxconnTrack.MoveIndexRight(theMcConfig.GetIndexPitch());
		//long lStep;
		//theStepper.GetPos(m_bStepperAddr, &lStep);
		//CString str;
		//str.Format(_T("pos=%d"), round(lStep * theStepper.GetResol(m_bStepperAddr)));
		//::AfxMessageBox(str);
		break;
	}
}

void CTopIndexerDebug::OnBackward()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	switch (theMcConfig.GetMhsType())
	{
	case MHS_TOP_LED:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		theStepper.MoveStep(m_bStepperAddr, -round(m_lScale/theStepper.GetResol(m_bStepperAddr)), MOTION_RELATIVE);
		break;
	case MHS_FOXCONN:
		theFoxconnTrack.MoveIndexLeft(m_lScale);
		//theFoxconnTrack.MoveIndexLeft(theMcConfig.GetIndexPitch());
		//long lStep;
		//theStepper.GetPos(m_bStepperAddr, &lStep);
		//CString str;
		//str.Format(_T("pos=%d"), round(lStep * theStepper.GetResol(m_bStepperAddr)));
		//::AfxMessageBox(str);
		break;
	}
}

void CTopIndexerDebug::OnHomeStep()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	GetDlgItem(IDC_BUTTON_HOME_STEP)->EnableWindow(FALSE);

	switch (theMcConfig.GetMhsType())
	{
	case MHS_TOP_LED:
	{
		theTopTrack.HomeIndexer();
		Sleep_x1ms(10);
		theTopTrack.WaitHomeIndexer();
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTEIndex.HomeIndexer();
		Sleep_x1ms(10);
		theTEIndex.WaitHomeIndexer();
		break;
	}
	case MHS_FOXCONN:
	{
		theFoxconnTrack.HomeIndexer();
		Sleep_x1ms(10);
		theFoxconnTrack.WaitHomeIndexer();
		break;
	}
	}

	GetDlgItem(IDC_BUTTON_HOME_STEP)->EnableWindow(TRUE);
}

void CTopIndexerDebug::OnFocusScale()
{
	 GetDlgItem(IDC_BUTTON_HOME_STEP)->SetFocus();
     theKeyPad.GetUserKeyInData(&m_lScale);
     UpdateData(false);
}

void CTopIndexerDebug::OnEditScale()
{
	 UpdateData(true);
}

void CTopIndexerDebug::OnClampHome()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	theServo.MoveBondHeadToSafePos();
	Sleep_x1ms(100);  
	theTopClamp.HomeClamp();
	Sleep_x1ms(100);
	theTopClamp.WaitHomeClamp();
	Sleep_x1ms(500);
	theTopClamp.PreCloseClamp();
}

void CTopIndexerDebug::OnClampClose()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	theTopClamp.CloseClamp();
}

void CTopIndexerDebug::OnClampOpen()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	theServo.MoveBondHeadToSafePos();
	Sleep_x1ms(100);
	theTopClamp.MoveClampSafePos();
}

void CTopIndexerDebug::OnClampPreclose()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	theTopClamp.PreCloseClamp();
}

void CTopIndexerDebug::OnFocusClampPercent()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lSafeClosePercent,50,0,100);
    UpdateData(false);
	OnEditClampPercent();
}

void CTopIndexerDebug::OnFocusPreclampPercent()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lPreclosePercent,80,0,100);
    UpdateData(false);
	OnEditPreclampPercent();
}

void CTopIndexerDebug::OnEditClampPercent()
{
	UpdateData(true);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
	pstTopClamp->lWindClampSafeClosePos = m_lSafeClosePercent;
}

void CTopIndexerDebug::OnEditPreclampPercent()
{
	UpdateData(true);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
	pstTopClamp->lWindClampPreClosePos = m_lPreclosePercent;
}

LRESULT CTopIndexerDebug::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.4.W23 add
{
	return m_bBusy;
}

void CTopIndexerDebug::OnDestroy()
{
	UpdateData(true);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
	pstTopClamp->lWindClampSafeClosePos = m_lSafeClosePercent;
	pstTopClamp->lWindClampPreClosePos = m_lPreclosePercent;

	theScanKeyThread.EnableJoyStick();
	CPropertyPage::OnDestroy();
}

void CTopIndexerDebug::OnStandBy()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add
	if (theMcConfig.GetMhsType() == MHS_FOXCONN)
		theFoxconnTrack.MoveToFirstIndexPos();	//v3.0.T194 add
	else
		theTopTrack.MoveIndexerToLogicZero();
}

void CTopIndexerDebug::OnClampRemove()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	GetDlgItem(IDC_BUTTON_CLAMP_REMOVE)->EnableWindow(FALSE);

	theScanKeyThread.DisableJoyStick();
	theServo.MoveBondHeadToSafePos();
	Sleep_x1ms(100);

	theTopClamp.OpenClamp();
	Sleep_x1ms(200);

	theTopTrack.MoveIndexerToLogicZero();
}

void CTopIndexerDebug::OnClamp()
{
	CBusy bs(&m_bBusy);		//V1.4.W23 add

	GetDlgItem(IDC_BUTTON_CLAMP_REMOVE)->EnableWindow(TRUE);

	theTopClamp.PreCloseClamp();
	Sleep_x1ms(500);
	theScanKeyThread.EnableJoyStick();
}

void CTopIndexerDebug::OnFocusClampPosition()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lClampPosition,10,0,40);
    UpdateData(false);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
    pstTopClamp->lWindClampClosePos = m_lClampPosition;
}

void CTopIndexerDebug::OnFocus2()
{
	GetDlgItem(IDC_BUTTON_CLAMP_HOME)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lJerk,10000,100,100000);
    UpdateData(false);

	if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		ST_AXIS_PARA stWindowClampParam;
		theTopClamp.GetAxisPara(&stWindowClampParam);
		stWindowClampParam.Vel = m_lJerk;
		theTopClamp.SetAxisPara(&stWindowClampParam);
		theStepper.SetSpeedParameter(theTopClamp.GetStepID(), stWindowClampParam);
	}
	else
	{
		STEP_SPEED_PROFILE stSpeedProfile;
		theTopClamp.GetSpeedProfile(&stSpeedProfile);
		stSpeedProfile.lDriveVelocity = m_lJerk;
		if (stSpeedProfile.lDriveVelocity < 1000)
			stSpeedProfile.lDriveVelocity = 1000;
		theTopClamp.SetSpeedProfile(&stSpeedProfile);

		theStepper.SetSpeedParameter(theTopClamp.GetStepID(), &stSpeedProfile, true);
	}
}
