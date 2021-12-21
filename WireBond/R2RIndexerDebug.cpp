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
#include "R2RIndexerDebug.h"
#include "ScanKeyThread.h"
#include "MachineConfig.h"
#include "TopClamp.h"
#include "R2RIndex.h"
#include "servo.h"

// CR2RIndexerDebug dialog

IMPLEMENT_DYNAMIC(CR2RIndexerDebug, CPropertyPage)

CR2RIndexerDebug::CR2RIndexerDebug()
	: CPropertyPage(CR2RIndexerDebug::IDD)
	, m_lScale(1000)
	, m_lPreclosePercent(0)
	, m_lSafeClosePercent(0)
    , m_lClampPosition(0)
    , m_lJerk(0)
{

	m_bStepperAddr=theR2RTrack.m_ucIndexerStepperId;

}

CR2RIndexerDebug::~CR2RIndexerDebug()
{
}

void CR2RIndexerDebug::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SCALE, m_lScale);
    DDX_Text(pDX, IDC_EDIT_PRECLAMP_PERCENT, m_lPreclosePercent);
    DDX_Text(pDX, IDC_EDIT_CLAMP_PERCENT, m_lSafeClosePercent);
    DDX_Text(pDX, IDC_CLAMP_POSITION, m_lClampPosition);
    DDX_Text(pDX, IDC_EDIT2, m_lJerk);
}


BEGIN_MESSAGE_MAP(CR2RIndexerDebug, CPropertyPage)
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FORWARD_, &CR2RIndexerDebug::OnBnClickedButtonForward)
	ON_BN_CLICKED(IDC_BUTTON_BACKWARD_, &CR2RIndexerDebug::OnBnClickedButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON_HOME_STEP, &CR2RIndexerDebug::OnBnClickedButtonHomeStep)
	ON_EN_SETFOCUS(IDC_EDIT_SCALE, &CR2RIndexerDebug::OnEnSetfocusEditScale)
	ON_EN_CHANGE(IDC_EDIT_SCALE, &CR2RIndexerDebug::OnEnChangeEditScale)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_HOME, &CR2RIndexerDebug::OnBnClickedButtonClampHome)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_CLOSE, &CR2RIndexerDebug::OnBnClickedButtonClampClose)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_OPEN, &CR2RIndexerDebug::OnBnClickedButtonClampOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_PRECLOSE, &CR2RIndexerDebug::OnBnClickedButtonClampPreclose)
	ON_EN_SETFOCUS(IDC_EDIT_CLAMP_PERCENT, &CR2RIndexerDebug::OnEnSetfocusEditClampPercent)
	ON_EN_SETFOCUS(IDC_EDIT_PRECLAMP_PERCENT, &CR2RIndexerDebug::OnEnSetfocusEditPreclampPercent)
	ON_EN_CHANGE(IDC_EDIT_CLAMP_PERCENT, &CR2RIndexerDebug::OnEnChangeEditClampPercent)
	ON_EN_CHANGE(IDC_EDIT_PRECLAMP_PERCENT, &CR2RIndexerDebug::OnEnChangeEditPreclampPercent)
	ON_BN_CLICKED(IDC_BUTTON_STAND_BY, &CR2RIndexerDebug::OnBnClickedButtonStandBy)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_REMOVE, &CR2RIndexerDebug::OnBnClickedButtonClampRemove)
	ON_BN_CLICKED(IDC_BUTTON_CLAMP__REMOVE_DONE, &CR2RIndexerDebug::OnBnClickedButtonClamp)
    ON_EN_SETFOCUS(IDC_CLAMP_POSITION, &CR2RIndexerDebug::OnEnSetfocusClampPosition)
    ON_EN_SETFOCUS(IDC_EDIT2, &CR2RIndexerDebug::OnEnSetfocusEdit2)
END_MESSAGE_MAP()	
    
// CR2RIndexerDebug message handlers

BOOL CR2RIndexerDebug::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetWindowPos(NULL, 1000, 1000, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	pWnd = GetDlgItem(IDC_STATIC_CLAMPER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAMPER));

	pWnd = GetDlgItem(IDC_STATIC_OPEN_POS);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OPEN_POS_COLON));

	pWnd = GetDlgItem(IDC_STATIC_PRE_CLAMP_POS);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRECLAMP_POS));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_HOME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_OPEN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OPEN));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_PRECLOSE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRECLAMP));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_CLOSE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLOSE));

	pWnd = GetDlgItem(IDC_STATIC_INDEXER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEXER));

	pWnd = GetDlgItem(IDC_STATIC_SCALE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ADJUST_STEP));

	pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HOME));

	pWnd = GetDlgItem(IDC_BUTTON_BACKWARD_);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BACKWARD));

	pWnd = GetDlgItem(IDC_BUTTON_FORWARD_);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FORWARD));

	pWnd = GetDlgItem(IDC_BUTTON_STAND_BY);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STANDBY_POS));

	pWnd = GetDlgItem(IDC_STATIC_REMOVE_CLAMP);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REMOVE_WINDOW_CLAMP));

	pWnd = GetDlgItem(IDC_BUTTON_CLAMP_REMOVE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_REMOVE));

    pWnd = GetDlgItem(IDC_BUTTON_CLAMP__REMOVE_DONE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RECOVER));

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
	pWnd = GetDlgItem(IDC_STATIC);
	pWnd->ShowWindow(false);


    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CR2RIndexerDebug::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CR2RIndexerDebug::OnBnClickedButtonForward()
{
	// TODO: Add your control notification handler code here
	theStepper.MoveStep(m_bStepperAddr,round((double)m_lScale/theStepper.GetResol(m_bStepperAddr)), MOTION_RELATIVE);
}

void CR2RIndexerDebug::OnBnClickedButtonBackward()
{
	// TODO: Add your control notification handler code here
	theStepper.MoveStep(m_bStepperAddr,-round((double)m_lScale/theStepper.GetResol(m_bStepperAddr)), MOTION_RELATIVE);
}

void CR2RIndexerDebug::OnBnClickedButtonHomeStep()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);

	pWnd->EnableWindow(FALSE);

	theR2RTrack.SearchIndexStepHome();
	Sleep(10);
	theR2RTrack.WaitStepHomeFinish();

	pWnd->EnableWindow(TRUE);

}

void CR2RIndexerDebug::OnEnSetfocusEditScale()
{
	// TODO: Add your control notification handler code here
	 CWnd* pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);
     pWnd->SetFocus();
    
     theKeyPad.GetUserKeyInData(&m_lScale);
 
     UpdateData(false);
}

void CR2RIndexerDebug::OnEnChangeEditScale()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	 UpdateData(true);

}

void CR2RIndexerDebug::OnBnClickedButtonClampHome()
{
	theServo.MoveBondHeadToSafePos();
	Sleep(100);  

	theTopClamp.HomeClamp();
	Sleep(100);
	theTopClamp.WaitHomeClamp();
	Sleep(500);
	theTopClamp.PreCloseClamp();
}

void CR2RIndexerDebug::OnBnClickedButtonClampClose()
{
	// TODO: Add your control notification handler code here
	theTopClamp.CloseClamp();
}

void CR2RIndexerDebug::OnBnClickedButtonClampOpen()
{
	theServo.MoveBondHeadToSafePos();
	Sleep(100);  

	theTopClamp.MoveClampSafePos();
}

void CR2RIndexerDebug::OnBnClickedButtonClampPreclose()
{
	// TODO: Add your control notification handler code here
	theTopClamp.PreCloseClamp();
}

void CR2RIndexerDebug::OnEnSetfocusEditClampPercent()
{
	// TODO: Add your control notification handler code here

	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAMP_HOME);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lSafeClosePercent,50,0,100);

    UpdateData(false);

	OnEnChangeEditClampPercent();

}

void CR2RIndexerDebug::OnEnSetfocusEditPreclampPercent()
{
	// TODO: Add your control notification handler code here

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAMP_HOME);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lPreclosePercent,80,0,100);

    UpdateData(false);

	OnEnChangeEditPreclampPercent();

}

void CR2RIndexerDebug::OnEnChangeEditClampPercent()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(true);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
	pstTopClamp->lWindClampSafeClosePos =m_lSafeClosePercent;
}

void CR2RIndexerDebug::OnEnChangeEditPreclampPercent()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(true);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
	pstTopClamp->lWindClampPreClosePos = m_lPreclosePercent;
}

void CR2RIndexerDebug::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(true);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
	pstTopClamp->lWindClampSafeClosePos = m_lSafeClosePercent;
	pstTopClamp->lWindClampPreClosePos = m_lPreclosePercent;

	theScanKeyThread.EnableJoyStick();
	CPropertyPage::OnOK();
}

void CR2RIndexerDebug::OnBnClickedButtonStandBy()
{
	// TODO: Add your control notification handler code here
	theR2RTrack.MoveToLogicZero();

}

void CR2RIndexerDebug::OnBnClickedButtonClampRemove()
{
	// TODO: Add your control notification handler code here

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAMP_REMOVE);
	pWnd->EnableWindow(FALSE);

	theScanKeyThread.DisableJoyStick();

	theServo.MoveBondHeadToSafePos();
	Sleep(100);

	theTopClamp.OpenClamp();
	Sleep(200);

	theR2RTrack.MoveToLogicZero();

	pWnd =this->GetParent()->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);

}

void CR2RIndexerDebug::OnBnClickedButtonClamp()
{
	// TODO: Add your control notification handler code here

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAMP_REMOVE);
	pWnd->EnableWindow(TRUE);


	theTopClamp.PreCloseClamp();

	Sleep(500);

	theScanKeyThread.EnableJoyStick();


	pWnd =this->GetParent()->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);


}

void CR2RIndexerDebug::OnEnSetfocusClampPosition()
{
    // TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAMP_HOME);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lClampPosition,10,0,40);

    UpdateData(false);

	TOP_CLAMP_RCD *pstTopClamp = theTopClamp.GetRcd();
    pstTopClamp->lWindClampClosePos = m_lClampPosition;
}

void CR2RIndexerDebug::OnEnSetfocusEdit2()
{
    // TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_CLAMP_HOME);
    pWnd->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lJerk,20000,100,100000);

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
			stSpeedProfile.lStartVelocity = 100;

		theTopClamp.SetSpeedProfile(&stSpeedProfile);

		theStepper.SetSpeedParameter(theTopClamp.GetStepID(), &stSpeedProfile, true);
	}
}
