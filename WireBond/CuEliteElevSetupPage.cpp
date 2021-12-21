// TopElevSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "CuEliteElevSetupPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "PopUpCuEliteStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "TeachCuEliteWorkHolderSheet.h"
#include "MachineConfig.h"
#include "ScanKeyThread.h"
#include "CuIndex.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CCuEliteElevSetupPage dialog
IMPLEMENT_DYNAMIC(CCuEliteElevSetupPage, CPropertyPage)

CCuEliteElevSetupPage::CCuEliteElevSetupPage()
	: CPropertyPage(CCuEliteElevSetupPage::IDD)
	, m_lOnload1stSlotLv(0)
	, m_lOffload1stSlotLv(0)
	, m_nStep(0)
	, m_iElevatorId(0)
	, m_nSlotIndex(0)
	, m_lMagSlotPitch(0)
	, m_lMagSwapPitch(0)
	, m_nTotalSlotNum(1)
{
	m_lpstOnloadData=NULL;
	m_lpstOffloadData=NULL;
	m_lpstCuEliteDevicePara=NULL;
}

CCuEliteElevSetupPage::~CCuEliteElevSetupPage()
{
}

void CCuEliteElevSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_ELEVATOR_SIDE, m_iElevatorId);
	DDX_Text(pDX, IDC_EDIT_SLOT_NUM, m_nSlotIndex);
	DDX_Text(pDX, IDC_EDIT_TOTAL_SLOT_NUM, m_nTotalSlotNum);
}

BEGIN_MESSAGE_MAP(CCuEliteElevSetupPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEVATOR_SIDE, &CCuEliteElevSetupPage::OnCbnSelchangeComboElevatorSide)
	ON_EN_SETFOCUS(IDC_EDIT_SLOT_NUM, &CCuEliteElevSetupPage::OnEnSetfocusEditSlotNum)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_SLOT, &CCuEliteElevSetupPage::OnBnClickedButtonNextSlot)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SLOT_POS, &CCuEliteElevSetupPage::OnBnClickedButtonEditSlotPos)
	ON_EN_CHANGE(IDC_EDIT_SLOT_NUM, &CCuEliteElevSetupPage::OnEnChangeEditSlotNum)
	ON_BN_CLICKED(IDC_BUTTON_RESET_OFFSET, &CCuEliteElevSetupPage::OnBnClickedButtonResetOffset)
	ON_EN_SETFOCUS(IDC_EDIT_TOTAL_SLOT_NUM, &CCuEliteElevSetupPage::OnEnSetfocusEditTotalSlotNum)
	ON_EN_CHANGE(IDC_EDIT_TOTAL_SLOT_NUM, &CCuEliteElevSetupPage::OnEnChangeEditTotalSlotNum)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CCuEliteElevSetupPage message handlers
HBRUSH CCuEliteElevSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CCuEliteElevSetupPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_STATIC_SLOT_NUM, theMsg.GetMsg(MSG_CUR_SLOT));
	SetDlgItemTextW(IDC_BUTTON_EDIT_SLOT_POS, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BUTTON_NEXT_SLOT, theMsg.GetMsg(MSG_NEXT_SLOT));
	SetDlgItemTextW(IDC_BUTTON_RESET_OFFSET, theMsg.GetMsg(MSG_RESET));
	SetDlgItemTextW(IDC_STATIC_VERIFY, theMsg.GetMsg(MSG_VERIFY));
	SetDlgItemTextW(IDC_STATIC_POSITION, theMsg.GetMsg(MSG_FINE_TUNE_SLOT_POS));
	SetDlgItemTextW(IDC_STATIC_ELEVATOR, theMsg.GetMsg(MSG_ELEVATOR));
	SetDlgItemTextW(IDC_STATIC_TOTAL_SLOT_NUM, theMsg.GetMsg(MSG_SLOT_NUM));

    GetDlgItem(IDC_BUTTON_DUMMMY)->SetWindowPos(NULL, 1000, 1000, 100, 100, SWP_NOZORDER);

	CWnd* pWnd = GetDlgItem(IDC_COMBO_ELEVATOR_SIDE);

	((CComboBox*)pWnd)->InsertString(0, theMsg.GetMsg(MSG_INPUT));
	((CComboBox*)pWnd)->InsertString(1, theMsg.GetMsg(MSG_OUTPUT));

	m_iElevatorId = 0;
	m_nSlotIndex = theCuEliteOnloader.GetCurrentSlotNum();

 	m_lpstOnloadData=new CUELITE_ONLOAD_RCD;
	m_lpstOffloadData=new CUELITE_OFFLOAD_RCD;
	m_lpstCuEliteDevicePara=new CUELITE_DEVICE_RECORD;

	theCuEliteOnloader.GetCuEliteOnloadRcd(m_lpstOnloadData);
	theCuEliteOffloader.GetCuEliteOffloadRcd(m_lpstOffloadData);

	m_lOnload1stSlotLv      = m_lpstOnloadData->lOnload1stSlotLevel;
	m_lOffload1stSlotLv     = m_lpstOffloadData->lOffload1stSlotLevel;
	m_lMagSlotPitch         = m_lpstOnloadData->lSlotPitch;
	m_lMagSwapPitch         = m_lpstOnloadData->lSwapMagPitch;

	theDevice.GetCuEliteRcd(m_lpstCuEliteDevicePara);
	m_nTotalSlotNum = m_lpstCuEliteDevicePara->lMagSlotNum;

	return CPropertyPage::OnInitDialog();
}

void CCuEliteElevSetupPage::OnStart()
{
	short anMsgArray[1]; 

	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();
	if(psheet->m_iPageMode == WH_CUELITE_EDIT)
	{
		anMsgArray[0] = MSG_DELETE_ELEV_DATA;

		if(theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray, true) == SHOW_MSG_RESULT_STOP)return ; 
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(1);

	if (psheet->m_iPageMode == WH_CUELITE_EDIT)psheet->GetDlgItem(IDOK)->EnableWindow(0);
	else psheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(0);

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_REMOVE_ALL_MAG));
    pListBox->InsertString(1, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
    pListBox->InsertString(2, theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));

	m_nStep = 0;

	theCuEliteOnloader.ResetOffsetRecord();
	theCuEliteOffloader.ResetOffsetRecord();

	theCuEliteTrack.MoveToLogicZero();

	if(theCuEliteTrack.theInjectSnr.GetCurrentState() == SENSOR_ON ||
		theCuEliteTrack.theEjectSnr.GetCurrentState() == SENSOR_ON   )
	{ 
		anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;
		theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

		return ;
	}

	if(theCuEliteOnloader.theInjectPusher.TriggerBacktoHome() == FNOK)
	{
		anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
		theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

		if (psheet->m_iPageMode == WH_CUELITE_EDIT)psheet->GetDlgItem(IDOK)->EnableWindow(1);
	}
}

void CCuEliteElevSetupPage::OnNext()
{
	long lPos;
	long lNext1stSlotPos;

	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();
	
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();

	theDevice.GetCuEliteRcd(m_lpstCuEliteDevicePara);

	m_nStep++;

	if(m_nStep == 1)
	{
        pListBox->InsertString(0, theMsg.GetMsg(MSG_WAIT_FORK_STANDBY));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_SIDE_ONLOADER));
		Sleep_x1ms(10);

		theCuEliteOnloader.SearchOnloadElvStepHome();
		Sleep_x1ms(1000);
		theCuEliteOnloader.WaitStepHomeFinish();
		Sleep_x1ms(100);
		//theCuEliteOnloader.moveMagazineEjectorForward();
		Sleep_x1ms(800);
 
		//theCuEliteOnloader.moveMagazineEjectorBackToHome();

		DWORD dwStartTime = GetCurrentTime();

		theStepper.MoveStep(CUELITE_ONLOAD_ELEVATOR,round(215000/theStepper.GetResol(CUELITE_ONLOAD_ELEVATOR)));

		do
		{
			Sleep_x1ms(10);

			if (GetCurrentTime() - dwStartTime < 30000) continue;

			pListBox->InsertString(0, _T("Failed!"));
			m_nStep--;
			break;
		} while (theStepper.IsBusy(CUELITE_ONLOAD_ELEVATOR));

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));
		pListBox->InsertString(2, theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));
		Sleep_x1ms(10);

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_ONLOAD_ELEVATOR);
		theStepperCtrler.DoModal();

		theStepper.GetPos(CUELITE_ONLOAD_ELEVATOR,&lPos);

		m_lOnload1stSlotLv = round(lPos * theStepper.GetResol(CUELITE_ONLOAD_ELEVATOR));

		UpdateData(false);

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->InsertString(2, theMsg.GetMsg(MSG_ADJUST_LAST_SLOT));
	}
	else if (m_nStep == 2)
	{
		theStepper.MoveStep(CUELITE_ONLOAD_ELEVATOR,round((m_lOnload1stSlotLv - m_lpstCuEliteDevicePara->lMagLvPitch *(m_lpstCuEliteDevicePara->lMagSlotNum-1))/theStepper.GetResol(CUELITE_ONLOAD_ELEVATOR)));

		do
		{
			Sleep_x1ms(10);
		} while (theStepper.IsBusy(CUELITE_ONLOAD_ELEVATOR));

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_ADJUST_LAST_SLOT));
		pListBox->InsertString(2, theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		Sleep_x1ms(10);

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_ONLOAD_ELEVATOR, CUELITE_SPEED_SMALL);
		theStepperCtrler.DoModal();

		theStepper.GetPos(CUELITE_ONLOAD_ELEVATOR,&lPos);

		m_lLastSlotPos = round(lPos * theStepper.GetResol(CUELITE_ONLOAD_ELEVATOR));

		m_lpstOnloadData->lOnload1stSlotLevel = m_lOnload1stSlotLv;
		m_lpstOnloadData->lSlotPitch = round((double)(m_lOnload1stSlotLv - m_lLastSlotPos)/(m_lpstCuEliteDevicePara->lMagSlotNum - 1));
		m_lpstOffloadData->lSlotPitch = m_lpstOnloadData->lSlotPitch;

		theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadData); 
		theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadData);
		
		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->InsertString(2, theMsg.GetMsg(MSG_ADJUST_NEXT_1ST));
	}
	else if(m_nStep ==3)
	{
		theStepper.MoveStep(CUELITE_ONLOAD_ELEVATOR,-round((m_lpstCuEliteDevicePara->lMag1stLvHt + m_lpstCuEliteDevicePara->lMagLastLvHt)/theStepper.GetResol(CUELITE_ONLOAD_ELEVATOR)), MOTION_RELATIVE);

		do
		{
			Sleep_x1ms(10);
		} while (theStepper.IsBusy(CUELITE_ONLOAD_ELEVATOR));

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_ADJUST_NEXT_1ST));
		pListBox->InsertString(2, theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		Sleep_x1ms(10);

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_ONLOAD_ELEVATOR, CUELITE_SPEED_SMALL);
		theStepperCtrler.DoModal();

		theStepper.GetPos(CUELITE_ONLOAD_ELEVATOR,&lPos);

		lNext1stSlotPos = round(lPos * theStepper.GetResol(CUELITE_ONLOAD_ELEVATOR));

		m_lpstOnloadData->lSwapMagPitch  = m_lLastSlotPos - lNext1stSlotPos;
		m_lpstOffloadData->lSwapMagPitch = m_lpstOnloadData->lSwapMagPitch;

		theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadData); 
		theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadData);

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_SIDE_OFFLOADER));
        pListBox->InsertString(2, theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));
	}
	else if(m_nStep ==4)
	{
        pListBox->InsertString(0, theMsg.GetMsg(MSG_WAIT_FORK_STANDBY));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_SIDE_OFFLOADER));
		pListBox->InsertString(2, theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		theCuEliteOffloader.SearchOffloadElvStepHome();
		Sleep_x1ms(1000);
		theCuEliteOffloader.WaitStepHomeFinish();
		Sleep_x1ms(100);
		//theCuEliteOffloader.moveMagazineEjectorForward();
		Sleep_x1ms(100);

		//theCuEliteOffloader.moveMagazineEjectorBackToHome();

		theStepper.MoveStep(CUELITE_OFFLOAD_ELEVATOR,round(m_lpstOnloadData->lOnload1stSlotLevel/theStepper.GetResol(CUELITE_OFFLOAD_ELEVATOR)));

		do
		{
			Sleep_x1ms(10);
		} while (theStepper.IsBusy(CUELITE_OFFLOAD_ELEVATOR));

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_SIDE_OFFLOADER));
        pListBox->InsertString(1, theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));

		Sleep_x1ms(10);

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_OFFLOAD_ELEVATOR);
		theStepperCtrler.DoModal();

		theStepper.GetPos(CUELITE_OFFLOAD_ELEVATOR,&lPos);

		m_lOffload1stSlotLv = round(lPos * theStepper.GetResol(CUELITE_OFFLOAD_ELEVATOR));

		m_lpstOffloadData->lOffload1stSlotLevel = m_lOffload1stSlotLv;
		theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadData);

		pListBox->ResetContent();
        pListBox->InsertString(0, theMsg.GetMsg(MSG_DONE_TO_QUIT));
	}
	else if (m_nStep == 5)
	{
        theApp.ArchiveMachineData(ARCHIVE_SAVE);
		Sleep_x1ms(10);
        theApp.ArchivePowerOnData(ARCHIVE_SAVE);

		theCuEliteOnloader.MoveTotheSlot(1);
		theCuEliteOnloader.WaitMagHoledrMoveFinish();

		m_nSlotIndex = 1;
		UpdateData(false);

		GetDlgItem(IDC_EDIT_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EDIT_SLOT_POS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NEXT_SLOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

		if (psheet->m_iPageMode == WH_CUELITE_TEACH)psheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(1);
		else psheet->GetDlgItem(IDOK)->EnableWindow(1);
	}
}

void CCuEliteElevSetupPage::OnCbnSelchangeComboElevatorSide()
{
	UpdateData(true);

	if( m_iElevatorId == 0)m_nSlotIndex = theCuEliteOnloader.GetCurrentSlotNum();
	else m_nSlotIndex = theCuEliteOffloader.GetCurrentSlotNum();

	UpdateData(false);
}

void CCuEliteElevSetupPage::OnEnSetfocusEditSlotNum()
{
	GetDlgItem(IDC_BUTTON_DUMMMY)->SetFocus();

	theDevice.GetCuEliteRcd(m_lpstCuEliteDevicePara);
    
    theKeyPad.GetUserKeyInData(&m_nSlotIndex,1,0,m_lpstCuEliteDevicePara->lMagSlotNum);

    UpdateData(false);

	OnEnChangeEditSlotNum();
}

void CCuEliteElevSetupPage::OnEnChangeEditSlotNum()
{
	UpdateData(true);

	if (m_iElevatorId == 0)
	{
		theCuEliteOnloader.MoveTotheSlot(m_nSlotIndex);
		theCuEliteOnloader.WaitMagHoledrMoveFinish();
	}
	else
	{
		theCuEliteOffloader.moveMagClamperOpen();
		theCuEliteOffloader.WaitMagClamperMoveFinish();

		theCuEliteOffloader.MoveTotheSlot(m_nSlotIndex);
		theCuEliteOffloader.WaitMagHoledrMoveFinish();

		theCuEliteOffloader.moveMagClamperStepBackward();
		theCuEliteOffloader.WaitMagClamperMoveFinish();
	}
}

void CCuEliteElevSetupPage::OnBnClickedButtonNextSlot()
{
	m_nSlotIndex++;

	UpdateData(false);

	theDevice.GetCuEliteRcd(m_lpstCuEliteDevicePara);

	if(m_nSlotIndex > m_lpstCuEliteDevicePara->lMagSlotNum)
		m_nSlotIndex = 0;

	if (m_iElevatorId == 0)
	{
		theCuEliteOnloader.MoveTotheSlot(m_nSlotIndex);
		theCuEliteOnloader.WaitMagHoledrMoveFinish();
	}
	else
	{
		theCuEliteOffloader.MoveTotheSlot(m_nSlotIndex);
		theCuEliteOffloader.WaitMagHoledrMoveFinish();
	}
}

void CCuEliteElevSetupPage::OnBnClickedButtonEditSlotPos()
{
	long lStartPos, lEndPos;

	if( m_iElevatorId == 0)
	{
		theStepper.GetPos(CUELITE_ONLOAD_ELEVATOR,&lStartPos);

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_ONLOAD_ELEVATOR);
	    theStepperCtrler.DoModal();

		theStepper.GetPos(CUELITE_ONLOAD_ELEVATOR,&lEndPos);

		m_lpstOnloadData->lSlotOffset[m_nSlotIndex]= m_lpstOnloadData->lSlotOffset[m_nSlotIndex] + round((lEndPos-lStartPos) * theStepper.GetResol(CUELITE_ONLOAD_ELEVATOR));

		theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadData);
	}
	else
	{
		theStepper.GetPos(CUELITE_OFFLOAD_ELEVATOR,&lStartPos);

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_OFFLOAD_ELEVATOR);
	    theStepperCtrler.DoModal();

		theStepper.GetPos(CUELITE_OFFLOAD_ELEVATOR,&lEndPos);

		m_lpstOffloadData->lSlotOffset[m_nSlotIndex]= m_lpstOffloadData->lSlotOffset[m_nSlotIndex]+ round((lEndPos-lStartPos) * theStepper.GetResol(CUELITE_OFFLOAD_ELEVATOR));

		theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadData);
	}
}

void CCuEliteElevSetupPage::OnBnClickedButtonResetOffset()
{
	if( m_iElevatorId == 0)theCuEliteOnloader.ResetOffsetRecord();
	else theCuEliteOffloader.ResetOffsetRecord();
}

BOOL CCuEliteElevSetupPage::OnSetActive()
{
	if( m_iElevatorId == 0) 
		theScanKeyThread.SetJoyStickConfig(JOYSTICK_STEPPER_VERTICAL, CUELITE_ONLOAD_ELEVATOR );
	else
		theScanKeyThread.SetJoyStickConfig(JOYSTICK_STEPPER_VERTICAL, CUELITE_OFFLOAD_ELEVATOR );

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();

	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();

	if (psheet->m_iPageMode == WH_CUELITE_TEACH)
	{
		GetDlgItem(IDC_EDIT_TOTAL_SLOT_NUM)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_EDIT_SLOT_POS)->EnableWindow(0);
		GetDlgItem(IDC_COMBO_ELEVATOR_SIDE)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_NEXT_SLOT)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_RESET_OFFSET)->EnableWindow(0);

		pListBox->InsertString(0, theMsg.GetMsg(MSG_START_TO_TCH_ELEV_POS));
		pListBox->InsertString(1, theMsg.GetMsg(MSG_NEXT_TO_SKIP));
	}
	else
	{
		pListBox->InsertString(0, theMsg.GetMsg(MSG_START_TO_TCH_ELEV_POS));

		psheet->GetDlgItem(IDOK)->EnableWindow(1);
	    GetDlgItem(IDC_BUTTON_START)->EnableWindow(1);
	}

	return CPropertyPage::OnSetActive();
}

BOOL  CCuEliteElevSetupPage::OnKillActive()
{
	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	return CPropertyPage::OnKillActive();
}

void CCuEliteElevSetupPage::OnEnSetfocusEditTotalSlotNum()
{
	GetDlgItem(IDC_BUTTON_DUMMMY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nTotalSlotNum);

	UpdateData(false);

	OnEnChangeEditTotalSlotNum();
}

void CCuEliteElevSetupPage::OnEnChangeEditTotalSlotNum()
{
	UpdateData(TRUE);

	theDevice.GetCuEliteRcd(m_lpstCuEliteDevicePara);
	m_lpstCuEliteDevicePara->lMagSlotNum = m_nTotalSlotNum;
	theDevice.SetCuEliteRcd(m_lpstCuEliteDevicePara);
}

void CCuEliteElevSetupPage::OnDestroy()
{
	CPropertyPage::OnDestroy();

 	if(m_lpstOnloadData!=NULL)delete m_lpstOnloadData;
	if(m_lpstOffloadData!=NULL)delete m_lpstOffloadData;
	if(m_lpstCuEliteDevicePara!=NULL)delete m_lpstCuEliteDevicePara;
}
