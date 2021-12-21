/***************************************************************
PopUpCuElitepStepperCtr.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "PopUpCuEliteStepperCtr.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "StepperControl.h"
#include "MaterialHandlerThread.h"
#include "MathCalc.h"
#include "ScanKeyThread.h"
#include "MachineConfig.h"
#include "CuIndex.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CPopUpCuEliteStepperCtr dialog

IMPLEMENT_DYNAMIC(CPopUpCuEliteStepperCtr, CDialog)

CPopUpCuEliteStepperCtr::CPopUpCuEliteStepperCtr(byte nCtrlModuleId, short nSpeed,  CWnd* pParent /*=NULL*/)
	: CDialog(CPopUpCuEliteStepperCtr::IDD, pParent)
	, m_nCtrModule(CUELITE_DEVICE_INDEXER)
	, m_nStepResol(2000)  // 1 to 10000
	, m_nSpeed(0)
{
	m_nCtrModule = nCtrlModuleId;

	m_nSpeed = nSpeed;

	if (m_nSpeed == CUELITE_SPEED_NORMAL)
		m_nStepResol = 2000;
	else if(m_nSpeed == CUELITE_SPEED_SMALL)
		m_nStepResol = 500;
	else if(m_nSpeed == CUELITE_SPEED_FINE)
		m_nStepResol = 10;

	m_nDlgPos=DLG_POS_CENTRE;
	m_n2ndCtrlID=0;
}

CPopUpCuEliteStepperCtr::~CPopUpCuEliteStepperCtr()
{
	 KillTimer(1);
}

void CPopUpCuEliteStepperCtr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPopUpCuEliteStepperCtr, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_UP_CTR, OnUp)
	ON_BN_CLICKED(IDC_DOWN_CTR, OnDown)
	ON_BN_CLICKED(IDC_RIGHT_CTR, OnRight)
	ON_BN_CLICKED(IDC_LEFT_CTRL, OnLeft)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_SCALE_INCREASE, &CPopUpCuEliteStepperCtr::OnBnClickedScaleIncrease)
	ON_BN_CLICKED(IDC_SCALE_DECREASE, &CPopUpCuEliteStepperCtr::OnBnClickedScaleDecrease)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnRecover)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnClose)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CPopUpCuEliteStepperCtr message handlers
BOOL CPopUpCuEliteStepperCtr::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));

	CWnd* pWnd = GetDlgItem(IDC_BTN_RECOVER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RECOVER));
	pWnd->ShowWindow(theMcConfig.GetMhsType() == MHS_CU_ELITE);

	SetDlgItemTextW(IDC_BTN_CANCEL, theMsg.GetMsg(MSG_QUIT));

	m_bitmapLeftArrow.LoadBitmapW(IDB_LEFT_ARROW);    
    m_bitmapRightArrow.LoadBitmapW(IDB_RIGHT_ARROW);
    m_bitmapUpArrow.LoadBitmapW(IDB_UP_ARROW);
    m_bitmapDownArrow.LoadBitmapW(IDB_DOWN_ARROW);

	pWnd = GetDlgItem(IDC_LEFT_CTRL);
    ((CButton*)pWnd)->SetBitmap((HBITMAP)m_bitmapLeftArrow.GetSafeHandle());
	pWnd->EnableWindow(FALSE);

    pWnd = GetDlgItem(IDC_RIGHT_CTR);
     ((CButton*)pWnd)->SetBitmap((HBITMAP)m_bitmapRightArrow.GetSafeHandle());
    pWnd->EnableWindow(FALSE);
    
    pWnd = GetDlgItem(IDC_UP_CTR);
	((CButton*)pWnd)->SetBitmap((HBITMAP)m_bitmapUpArrow.GetSafeHandle());
	pWnd->EnableWindow(m_n2ndCtrlID == CUELITE_WINDOW_CLAMPER);

    pWnd = GetDlgItem(IDC_DOWN_CTR);
    ((CButton*)pWnd)->SetBitmap((HBITMAP)m_bitmapDownArrow.GetSafeHandle());
	pWnd->EnableWindow(m_n2ndCtrlID == CUELITE_WINDOW_CLAMPER);

	if ((m_nCtrModule == CUELITE_DEVICE_INDEXER)
		|| (m_nCtrModule == CUELITE_ONLOAD_XAXIS)
		|| (m_nCtrModule == CUELITE_OFFLOAD_XAXIS))
	{
		GetDlgItem(IDC_LEFT_CTRL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RIGHT_CTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_RECOVER)->EnableWindow(m_nCtrModule == CUELITE_DEVICE_INDEXER);

        //theCuEliteTrack.xLogicToPhysicPos(m_nCtrModule,m_nStepResol, &lExpectedPosPulse);
		theStepper.SetJogStep(m_nStepResol);
		theScanKeyThread.SetJoyStickConfig(JOYSTICK_STEPPER_HORIZONTAL, m_nCtrModule);
	}
	else if ((m_nCtrModule == CUELITE_ONLOAD_ELEVATOR)
			|| (m_nCtrModule == CUELITE_ONLOAD_YONE)
			|| (m_nCtrModule == CUELITE_ONLOAD_YTWO)
			|| (m_nCtrModule == CUELITE_OFFLOAD_ELEVATOR)
			|| (m_nCtrModule == CUELITE_OFFLOAD_YONE)
			|| (m_nCtrModule == CUELITE_OFFLOAD_YTWO)
			|| (m_nCtrModule == CUELITE_FRONT_RAIL)
			|| (m_nCtrModule == CUELITE_REAR_RAIL)
			|| (m_nCtrModule == CUELITE_WINDOW_CLAMPER))
	{
		GetDlgItem(IDC_UP_CTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOWN_CTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_RECOVER)->EnableWindow(FALSE);

        //theCuEliteTrack.xLogicToPhysicPos(m_nCtrModule,m_nStepResol, &lExpectedPosPulse);
		theStepper.SetJogStep(m_nStepResol);
		theScanKeyThread.SetJoyStickConfig(JOYSTICK_STEPPER_VERTICAL, m_nCtrModule);
	}
	else if ((m_nCtrModule == CUELITE_ONLOAD_YONE_YTWO)
			|| (m_nCtrModule == CUELITE_OFFLOAD_YONE_YTWO)
			|| (m_nCtrModule == CUELITE_ONLOAD_YONE_YTWO_WIDTH)
			|| (m_nCtrModule == CUELITE_OFFLOAD_YONE_YTWO_WIDTH)
			|| (m_nCtrModule == CUELITE_FRONT_REAR))
	{
		GetDlgItem(IDC_UP_CTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOWN_CTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_RECOVER)->EnableWindow(FALSE);

        //theCuEliteTrack.xLogicToPhysicPos(m_nCtrModule,m_nStepResol, &lExpectedPosPulse);
		theStepper.SetJogStep(m_nStepResol);
	}

	if(m_nDlgPos==DLG_POS_TOP_LEFT)SetWindowPos(NULL, 0, 0, 460, 300, SWP_NOZORDER);
	else if(m_nDlgPos==DLG_POS_TOP_RIGHT)SetWindowPos(NULL, 820, 0, 460, 300, SWP_NOZORDER);
	else if(m_nDlgPos==DLG_POS_BOTTOM_RIGHT)SetWindowPos(NULL, 820, 468, 460, 300, SWP_NOZORDER);
	else if(m_nDlgPos==DLG_POS_BOTTOM_LEFT)SetWindowPos(NULL, 0, 468, 460, 300, SWP_NOZORDER);
	else SetWindowPos(NULL, 450, 200, 460, 300, SWP_NOZORDER);

    if (theMcConfig.GetMhsType() == MHS_CU_ELITE)
	    SetTimer(1, 2000, NULL);

	return FALSE;
}

HBRUSH CPopUpCuEliteStepperCtr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(RGB(0, 192, 20));
        return theApp.m_DlgBkBrushA;
    }

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPopUpCuEliteStepperCtr::RefreshData()
{
	do
	{
		Sleep_x1ms(10);
	} while (theStepper.IsBusy(m_nCtrModule));

	GetParent()->PostMessage(WM_REFRESH,m_nCtrModule);
}

void CPopUpCuEliteStepperCtr::OnUp()
{
	if (m_nCtrModule == CUELITE_FRONT_RAIL || m_nCtrModule == CUELITE_ONLOAD_YTWO ||
		m_nCtrModule == CUELITE_OFFLOAD_YTWO || m_nCtrModule == CUELITE_ONLOAD_ELEVATOR || 
		m_nCtrModule == CUELITE_OFFLOAD_ELEVATOR)
	{
		if (!IsOutOfSoftLimit(m_nCtrModule,m_nStepResol))
			theStepper.MoveStep(m_nCtrModule, round((double)m_nStepResol / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}
	else if(m_nCtrModule == CUELITE_REAR_RAIL || m_nCtrModule == CUELITE_ONLOAD_YONE ||
		m_nCtrModule == CUELITE_OFFLOAD_YONE || m_nCtrModule == CUELITE_ONLOAD_ELEVATOR || 
		m_nCtrModule == CUELITE_OFFLOAD_ELEVATOR)
	{
		if (!IsOutOfSoftLimit(m_nCtrModule,-m_nStepResol))
			theStepper.MoveStep(m_nCtrModule, round((double)(0 - m_nStepResol) / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}
	else if (m_n2ndCtrlID == CUELITE_WINDOW_CLAMPER)
		theStepper.MoveStep(m_n2ndCtrlID, -round(10 / theStepper.GetResol(m_n2ndCtrlID)), MOTION_RELATIVE);

	RefreshData();
}

void CPopUpCuEliteStepperCtr::OnDown()
{
	if (m_nCtrModule == CUELITE_FRONT_RAIL
		|| m_nCtrModule == CUELITE_ONLOAD_YTWO
		|| m_nCtrModule == CUELITE_OFFLOAD_YTWO 
		|| m_nCtrModule == CUELITE_ONLOAD_ELEVATOR 
		|| m_nCtrModule == CUELITE_OFFLOAD_ELEVATOR)
	{
		if (!IsOutOfSoftLimit(m_nCtrModule,-m_nStepResol))
			theStepper.MoveStep(m_nCtrModule, round((double)(0 - m_nStepResol) / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}
	else if (m_nCtrModule == CUELITE_REAR_RAIL 
			|| m_nCtrModule == CUELITE_ONLOAD_YONE 
			|| m_nCtrModule == CUELITE_OFFLOAD_YONE 
			|| m_nCtrModule == CUELITE_ONLOAD_ELEVATOR 
			|| m_nCtrModule == CUELITE_OFFLOAD_ELEVATOR)
	{
		if (!IsOutOfSoftLimit(m_nCtrModule, m_nStepResol))
			theStepper.MoveStep(m_nCtrModule, round((double)m_nStepResol / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}
	else if (m_n2ndCtrlID == CUELITE_WINDOW_CLAMPER)
		theStepper.MoveStep(m_n2ndCtrlID, round(10 / theStepper.GetResol(m_n2ndCtrlID)), MOTION_RELATIVE);

	RefreshData();
}

void CPopUpCuEliteStepperCtr::OnRight()
{
	if (m_nCtrModule == CUELITE_ONLOAD_XAXIS || m_nCtrModule == CUELITE_DEVICE_INDEXER)
	{
		theStepper.MoveStep(m_nCtrModule, round((double)m_nStepResol / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}
	else if(m_nCtrModule == CUELITE_OFFLOAD_XAXIS)
	{
		theStepper.MoveStep(m_nCtrModule, round((double)(0 - m_nStepResol) / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}

	RefreshData();
}

void CPopUpCuEliteStepperCtr::OnLeft()
{
	if ((m_nCtrModule == CUELITE_ONLOAD_XAXIS)
		|| (m_nCtrModule == CUELITE_DEVICE_INDEXER))
	{
		theStepper.MoveStep(m_nCtrModule,round((double)(0 - m_nStepResol) / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}
	else if (m_nCtrModule == CUELITE_OFFLOAD_XAXIS)
	{
		theStepper.MoveStep(m_nCtrModule,round((double)m_nStepResol / theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
	}

	RefreshData();
}

void CPopUpCuEliteStepperCtr::OnOk()
{
	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	OnOK();
}

void CPopUpCuEliteStepperCtr::OnBnClickedScaleIncrease()
{
	m_nStepResol = m_nStepResol*2;
	if (m_nStepResol >= 1500)
		m_nStepResol = 1500;

	//theCuEliteTrack.xLogicToPhysicPos(m_nCtrModule, m_nStepResol, &lExpectedPosPulse);
	theStepper.SetJogStep(m_nStepResol);
}

void CPopUpCuEliteStepperCtr::OnBnClickedScaleDecrease()
{
	m_nStepResol = m_nStepResol/2;
	if(m_nStepResol <= 5)	//V3.3.D231
		m_nStepResol = 5;

	//theCuEliteTrack.xLogicToPhysicPos(m_nCtrModule, m_nStepResol, &lExpectedPosPulse);
	theStepper.SetJogStep(m_nStepResol);
}

void CPopUpCuEliteStepperCtr::OnTimer(UINT_PTR nIDEvent)
{
    if (theMcConfig.GetMhsType() == MHS_CU_ELITE)
    {
        if ((theCuEliteTrack.theInputClaw.GetDuration() > RELAY_SAFTY_DURATION)  ||
            (theCuEliteTrack.theOutputClaw.GetDuration() > RELAY_SAFTY_DURATION) ||
            (theCuEliteTrack.theEjector.GetDuration() > RELAY_SAFTY_DURATION))
        {
            theCuEliteTrack.ResetRelayErrorState();
        }
    }

    CDialog::OnTimer(nIDEvent);
}

void CPopUpCuEliteStepperCtr::OnRecover()
{
	//recover the claw to do indexing, just for protection of overheat at claw
	if (theMcConfig.GetMhsType() == MHS_CU_ELITE)
         theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
}

void CPopUpCuEliteStepperCtr::OnClose()
{
	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	OnCancel();
}

bool CPopUpCuEliteStepperCtr::IsOutOfSoftLimit(BYTE bStepperId ,double dStepResol)
{
	long lpos;double dpos;
	theStepper.GetPos(bStepperId, &lpos);
	dpos = lpos * theStepper.GetResol(bStepperId);

	switch(bStepperId)
	{
	case CUELITE_ONLOAD_XAXIS:
		if (dpos + dStepResol > theCuEliteOnloader.m_lOnloadMagLenLimit || dpos + dStepResol <= 0)
			return true;
	case CUELITE_OFFLOAD_XAXIS:
		if (dpos + dStepResol > theCuEliteOffloader.m_lOffloadMagLenLimit || dpos + dStepResol <= 0)
			return true;
		else
			return false;
	case CUELITE_ONLOAD_YONE:
	case CUELITE_OFFLOAD_YONE:
	case CUELITE_ONLOAD_YTWO:
	case CUELITE_OFFLOAD_YTWO:
		if (dpos + dStepResol > 200000 || dpos + dStepResol <= 0)
			return true;
		else
			return false;
	case CUELITE_REAR_RAIL:
	case CUELITE_FRONT_RAIL:
		if (dpos + dStepResol > 200000 || dpos + dStepResol <= 0)
			return true;
		else
			return false;
	default:
		return false;
	}
}

void CPopUpCuEliteStepperCtr::PresetDlgPos(BYTE nDlgPos)
{
	m_nDlgPos=nDlgPos;
}

void CPopUpCuEliteStepperCtr::Preset2ndCtrlID(BYTE nID)
{
	m_n2ndCtrlID=nID;
}

LRESULT CPopUpCuEliteStepperCtr::OnNcHitTest(CPoint point)
{
	CDialog::OnNcHitTest(point);

	return HTCAPTION;
}
