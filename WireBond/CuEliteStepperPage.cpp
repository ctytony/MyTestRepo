// CuEliteStepperPage.cpp : 实现文件

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "CuEliteStepperPage.h"
#include "DeviceData.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "StepperControl.h"
#include "MaterialHandlerThread.h"
#include "MathCalc.h"
#include "MachineConfig.h"
#include "CuIndex.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CCuEliteStepperPage 对话框
IMPLEMENT_DYNAMIC(CCuEliteStepperPage, CPropertyPage)

CCuEliteStepperPage::CCuEliteStepperPage()
	: CPropertyPage(CCuEliteStepperPage::IDD)
	, m_iElevatorId(0)
	, m_nCurrentSlot(0)
{
}

CCuEliteStepperPage::~CCuEliteStepperPage()
{
}

void CCuEliteStepperPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_ELEVATOR, m_iElevatorId);
	DDX_Text(pDX, IDC_EDIT_CURRENT_SLOT, m_nCurrentSlot);
}

BEGIN_MESSAGE_MAP(CCuEliteStepperPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_ELEVATOR, &CCuEliteStepperPage::OnCbnSelchangeComboElevator)
	ON_BN_CLICKED(IDC_BUTTON_MAG_LENGTH_HOME, &CCuEliteStepperPage::OnBnClickedButtonMagLengthHome)
	ON_BN_CLICKED(IDC_BUTTON_MAG_LENGTH_STANDBY_POS, &CCuEliteStepperPage::OnBnClickedButtonMagLengthStandbyPos)
	ON_BN_CLICKED(IDC_BUTTON_MAG_LENGTH_STOP, &CCuEliteStepperPage::OnBnClickedButtonMagLengthStop)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_BACK_HOME, &CCuEliteStepperPage::OnBnClickedButtonMagClampBackHome)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_BACK_STANDBY_POS, &CCuEliteStepperPage::OnBnClickedButtonMagClampBackStandbyPos)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_BACK_STOP, &CCuEliteStepperPage::OnBnClickedButtonMagClampBackStop)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_FRONT_HOME, &CCuEliteStepperPage::OnBnClickedButtonMagClampFrontHome)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_FRONT_STANDBY_POS, &CCuEliteStepperPage::OnBnClickedButtonMagClampFrontStandbyPos)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_FRONT_STOP, &CCuEliteStepperPage::OnBnClickedButtonMagClampFrontStop)
	ON_BN_CLICKED(IDC_BUTTON_PULLY_HOME, &CCuEliteStepperPage::OnBnClickedButtonPullyHome)
	ON_BN_CLICKED(IDC_BUTTON_PULLY_LIMIT, &CCuEliteStepperPage::OnBnClickedButtonPullyLimit)
	ON_BN_CLICKED(IDC_BUTTON_HOLDER_HOME, &CCuEliteStepperPage::OnBnClickedButtonHolderHome)
	ON_BN_CLICKED(IDC_BUTTON_HOLDER_STOP, &CCuEliteStepperPage::OnBnClickedButtonHolderStop)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_SLOT, &CCuEliteStepperPage::OnBnClickedButtonNextSlot)
	ON_BN_CLICKED(IDC_BUTTON_HOLDER_STANDBY, &CCuEliteStepperPage::OnBnClickedButtonHolderStandby)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD, &CCuEliteStepperPage::OnBnClickedButtonUnload)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_HOME, &CCuEliteStepperPage::OnBnClickedButtonPusherHome)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_LIMIT, &CCuEliteStepperPage::OnBnClickedButtonPusherLimit)
	ON_EN_SETFOCUS(IDC_EDIT_CURRENT_SLOT, &CCuEliteStepperPage::OnEnSetfocusEditCurrentSlot)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_SLOT, &CCuEliteStepperPage::OnEnChangeEditCurrentSlot)
END_MESSAGE_MAP()

// CCuEliteStepperPage 消息处理程序
BOOL CCuEliteStepperPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_LABLE_ELEV, theMsg.GetMsg(MSG_ELEVATOR));
	SetDlgItemTextW(IDC_STATIC_ELEVATOR, theMsg.GetMsg(MSG_ELEVATOR));
	SetDlgItemTextW(IDC_STATIC_MAG_LENGTH, theMsg.GetMsg(MSG_CLAMPER_LEN_POS));
	SetDlgItemTextW(IDC_STATIC_BACK_CLAMPER, theMsg.GetMsg(MSG_CLAMPER_BACK_POS));
	SetDlgItemTextW(IDC_STATIC_FRONT_CLAMPER, theMsg.GetMsg(MSG_CLAMPER_FRONT_POS));
	SetDlgItemTextW(IDC_STATIC_PULLY_MOTOR, theMsg.GetMsg(MSG_PULLY_MOTOR));
	SetDlgItemTextW(IDC_STATIC_HOLDING_FORK, theMsg.GetMsg(MSG_HOLDING_FORK));
	SetDlgItemTextW(IDC_STATIC_CURRENT_SLOT, theMsg.GetMsg(MSG_CUR_SLOT));
	SetDlgItemTextW(IDC_BUTTON_MAG_LENGTH_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_MAG_LENGTH_STANDBY_POS, theMsg.GetMsg(MSG_STANDBY_POS));
	SetDlgItemTextW(IDC_BUTTON_MAG_LENGTH_STOP, theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_BUTTON_MAG_CLAMP_BACK_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_MAG_CLAMP_BACK_STANDBY_POS, theMsg.GetMsg(MSG_STANDBY_POS));
	SetDlgItemTextW(IDC_BUTTON_MAG_CLAMP_BACK_STOP, theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_BUTTON_MAG_CLAMP_FRONT_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_MAG_CLAMP_FRONT_STANDBY_POS, theMsg.GetMsg(MSG_STANDBY_POS));
	SetDlgItemTextW(IDC_BUTTON_MAG_CLAMP_FRONT_STOP, theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_BUTTON_PULLY_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_PULLY_LIMIT, theMsg.GetMsg(MSG_FORWARD));
	SetDlgItemTextW(IDC_BUTTON_HOLDER_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_HOLDER_STOP, theMsg.GetMsg(MSG_STOP));
	SetDlgItemTextW(IDC_BUTTON_NEXT_SLOT, theMsg.GetMsg(MSG_NEXT_SLOT));
	SetDlgItemTextW(IDC_BUTTON_HOLDER_STANDBY, theMsg.GetMsg(MSG_STANDBY_POS));
	SetDlgItemTextW(IDC_BUTTON_UNLOAD, theMsg.GetMsg(MSG_INITIALIZE));

	m_iElevatorId = ELEVATOR_ONLOADER;

	CWnd* pWnd = GetDlgItem(IDC_COMBO_ELEVATOR);
	((CComboBox*)pWnd)->InsertString(0, theMsg.GetMsg(MSG_INPUT));
	((CComboBox*)pWnd)->InsertString(1, theMsg.GetMsg(MSG_OUTPUT));
	((CComboBox*)pWnd)->SetCurSel(m_iElevatorId);

	m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();

	SetDlgItemTextW(IDC_STATIC_PUSH_KICKER, theMsg.GetMsg(MSG_ONLOAD_PUSHER_KICKER));
	SetDlgItemTextW(IDC_BUTTON_PUSHER_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_PUSHER_LIMIT, theMsg.GetMsg(MSG_LIMIT));

	return FALSE;
}

HBRUSH CCuEliteStepperPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCuEliteStepperPage::OnCbnSelchangeComboElevator()
{
	UpdateData(true);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		//Onloader
		m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();

		GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(TRUE);
	}
	else
	{
		//Offloader
		m_nCurrentSlot = theCuEliteOffloader.GetCurrentSlotNum();

		GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(FALSE);
	}

	UpdateData(false);

	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagLengthHome()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MAG_LENGTH_HOME);
	pWnd->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		//theCuEliteOnloader 
		theCuEliteOnloader.SearchOnloadMagLengthStepHome();
		Sleep_x1ms(100);
		theCuEliteOnloader.WaitOnloadMagLengthStepHomeFinish();
	}
	else
	{
		//theCuEliteOffloader
		theCuEliteOffloader.SearchOffloadMagLengthStepHome();
		Sleep_x1ms(100);
		theCuEliteOffloader.WaitOffloadMagLengthStepHomeFinish();
	}

	pWnd->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagLengthStandbyPos()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MAG_LENGTH_STANDBY_POS);
	pWnd->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		//theCuEliteOnloader 
		theCuEliteOnloader.OnloadMagLengthMovetoStandyPos();
		Sleep_x1ms(100);
		theCuEliteOnloader.WaitOnloadMagLengthStepHomeFinish();
	}
	else
	{
		//theCuEliteOffloader
		theCuEliteOffloader.OffloadMagLengthMovetoStandyPos();
		Sleep_x1ms(100);
		theCuEliteOffloader.WaitOffloadMagLengthStepHomeFinish();
	}

	pWnd->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagLengthStop()
{
	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theCuEliteOnloader.OnloadMagLengthStop();
	}
	else
	{
		theCuEliteOffloader.OffloadMagLengthStop();
	}

	GetDlgItem(IDC_BUTTON_MAG_LENGTH_STANDBY_POS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAG_LENGTH_HOME)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagClampBackHome()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MAG_CLAMP_BACK_HOME);
	pWnd->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		//theCuEliteOnloader 
		theCuEliteOnloader.SearchOnloadMagClamperBackStepHome();
		Sleep_x1ms(100);
		theCuEliteOnloader.WaitOnloadMagClamperBackStepHomeFinish();
	}
	else
	{
		//theCuEliteOffloader
		theCuEliteOffloader.SearchOffloadMagClamperBackStepHome();
		Sleep_x1ms(100);
		theCuEliteOffloader.WaitOffloadMagClamperBackStepHomeFinish();
	}

	pWnd->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagClampBackStandbyPos()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MAG_CLAMP_BACK_STANDBY_POS);
	pWnd->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		//theCuEliteOnloader 
		theCuEliteOnloader.OnloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(100);
		theCuEliteOnloader.WaitOnloadMagClamperBackStepHomeFinish();
	}
	else
	{
		//theCuEliteOffloader
		theCuEliteOffloader.OffloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(100);
		theCuEliteOffloader.WaitOffloadMagClamperBackStepHomeFinish();
	}

	pWnd->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagClampBackStop()
{
	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theCuEliteOnloader.OnloadMagClamperBackStop();
	}
	else
	{
		theCuEliteOffloader.OffloadMagClamperBackStop();
	}

	GetDlgItem(IDC_BUTTON_MAG_CLAMP_BACK_STANDBY_POS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAG_CLAMP_BACK_HOME)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagClampFrontHome()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MAG_CLAMP_FRONT_HOME);
	pWnd->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		//theCuEliteOnloader 
		theCuEliteOnloader.SearchOnloadMagClamperFrontStepHome();
		Sleep_x1ms(100);
		theCuEliteOnloader.WaitOnloadMagClamperFrontStepHomeFinish();
	}
	else
	{
		//theCuEliteOffloader
		theCuEliteOffloader.SearchOffloadMagClamperFrontStepHome();
		Sleep_x1ms(100);
		theCuEliteOffloader.WaitOffloadMagClamperFrontStepHomeFinish();
	}

	pWnd->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagClampFrontStandbyPos()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MAG_CLAMP_FRONT_STANDBY_POS);
	pWnd->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		//theCuEliteOnloader 
		theCuEliteOnloader.OnloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(100);
		theCuEliteOnloader.WaitOnloadMagClamperFrontStepHomeFinish();
	}
	else
	{
		//theCuEliteOffloader
		theCuEliteOffloader.OffloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(100);
		theCuEliteOffloader.WaitOffloadMagClamperFrontStepHomeFinish();
	}

	pWnd->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonMagClampFrontStop()
{
	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theCuEliteOnloader.OnloadMagClamperFrontStop();
	}
	else
	{
		theCuEliteOffloader.OffloadMagClamperFrontStop();
	}

	GetDlgItem(IDC_BUTTON_MAG_CLAMP_FRONT_STANDBY_POS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAG_CLAMP_FRONT_HOME)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonPullyHome()
{
	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(FALSE);

	if (m_iElevatorId == ELEVATOR_ONLOADER)
	{
		if (theCuEliteOnloader.theMagDropSensor.GetCurrentState() == SENSOR_ON)
		{
			CStringList sList;
			sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
		}

		theCuEliteOnloader.theMagPullyDcMotor.TriggerBacktoHome();
	}
	else
	{
		if (theCuEliteOffloader.theMagDropSensor.GetCurrentState() == SENSOR_ON)
		{
			CStringList sList;
			sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
		}

		theCuEliteOffloader.theMagPullyDcMotor.TriggerBacktoHome();
	}

	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonPullyLimit()
{
	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theCuEliteOnloader.theMagPullyDcMotor.TriggerForwardMotion();
	}
	else
	{
		theCuEliteOffloader.theMagPullyDcMotor.TriggerForwardMotion();
	}

	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonHolderHome()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_HOLDER_HOME);
	pWnd->EnableWindow(FALSE);

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		int iPusherStatus = FOK;

		if(theCuEliteOnloader.theInjectPusher.theHomeSensor.GetCurrentState() == false)
			iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();

		if(iPusherStatus == FNOK)
		{
			short anMsgArray[1]; 

			anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

			pWnd->EnableWindow(TRUE);
			return;
		}

		theCuEliteTrack.MoveToLogicZero();
		Sleep_x1ms(100);

		//theCuEliteOnloader 
		theCuEliteOnloader.SearchOnloadElvStepHome();
		Sleep_x1ms(100);
		theCuEliteOnloader.WaitStepHomeFinish();
	}
	else
	{
		//theCuEliteOffloader
		theCuEliteOffloader.SearchOffloadElvStepHome();
		Sleep_x1ms(100);
		theCuEliteOffloader.WaitStepHomeFinish();
	}

	pWnd->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonHolderStop()
{
	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theStepper.StopMotor(theCuEliteOnloader.m_ucForkHolderStepId);
	}
	else
	{
		theStepper.StopMotor(theCuEliteOffloader.m_ucForkHolderStepId);
	}
}

void CCuEliteStepperPage::OnBnClickedButtonNextSlot()
{
	GetDlgItem(IDC_BUTTON_NEXT_SLOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HOLDER_STANDBY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(FALSE);

	long lStartTime = GetCurrentTime();

	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theCuEliteOnloader.SubmitOnloadEvent(CUELITE_ONLOAD_EVENT_START);

		do
		{
			theCuEliteOnloader.ProcessOnloadEvent();

			Sleep_x1ms(2);

			if(GetCurrentTime() - lStartTime > 30000)
				break;
		}
		while(theCuEliteOnloader.GetOnloadEvent()!=CUELITE_ONLOAD_EVENT_IDLE);

		m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();

		UpdateData(false);
	}
	else
	{
		theCuEliteOffloader.SubmitOffloadEvent(CUELITE_OFFLOAD_EVENT_START);

		do
		{
			theCuEliteOffloader.ProcessOffloadEvent();

			Sleep_x1ms(2);

			if(GetCurrentTime() - lStartTime > 30000)
				break;
		}
		while(theCuEliteOffloader.GetOffloadEvent()!=CUELITE_OFFLOAD_EVENT_IDLE);

		m_nCurrentSlot = theCuEliteOffloader.GetCurrentSlotNum();

		UpdateData(false);
	}

	GetDlgItem(IDC_BUTTON_NEXT_SLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HOLDER_STANDBY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonHolderStandby()
{
	if(m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theCuEliteTrack.MoveToLogicZero();
		Sleep_x1ms(200);

		theCuEliteOnloader.MoveTotheSlot(1);
		theCuEliteOnloader.WaitMagHoledrMoveFinish();
		m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();
	}
	else
	{
		theCuEliteOffloader.MoveTotheSlot(1);
		theCuEliteOffloader.WaitMagHoledrMoveFinish();
		m_nCurrentSlot = theCuEliteOffloader.GetCurrentSlotNum();
	}

	UpdateData(false);
}

void CCuEliteStepperPage::OnBnClickedButtonUnload()
{
	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(FALSE);

	theCuEliteTrack.MoveToLogicZero();

	if(m_iElevatorId == ELEVATOR_ONLOADER) 
	{
		if(theCuEliteOnloader.thePullyMagExistSensor.GetCurrentState() == TRUE)
		{
			short anMsgArray[1]; 
			anMsgArray[0] = MSG_ONLOAD_MAG_PULLY_FULL;
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

			GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);
			return ;
		}

		int iPusherStatus = FOK;

		if(theCuEliteOnloader.theInjectPusher.theHomeSensor.GetCurrentState() == false)
			iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
		if(iPusherStatus == FNOK)
		{
			short anMsgArray[1]; 
			anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

			GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);

			return;
		}

		theCuEliteOnloader.moveMagClamperStepForward();
		Sleep_x1ms(300);
		theCuEliteOnloader.theMagDropSensor.UpdateSensorState();
		if (theCuEliteOffloader.theMagDropSensor.GetSensorState() == SENSOR_ON)
		{
			CStringList sList;
			sList.AddTail(theMsg.GetMsg( MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
		}

		theCuEliteOnloader.theMagPullyDcMotor.TriggerBacktoHome();
		Sleep_x1ms(200);
		theCuEliteOnloader.SearchOnloadElvStepHome();
		Sleep_x1ms(10);
		theCuEliteOnloader.WaitStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOnloader.theMagPullyDcMotor.TriggerForwardMotion();
		Sleep_x1ms(200);
		if (theCuEliteOffloader.theMagDropSensor.GetSensorState() == SENSOR_ON)
		{
			CStringList sList;
			sList.AddTail(theMsg.GetMsg( MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
		}

		theCuEliteOnloader.theMagPullyDcMotor.TriggerBacktoHome();
		Sleep_x1ms(200);
		theCuEliteOnloader.MoveTotheSlot(m_nCurrentSlot);
		Sleep_x1ms(100);

		do
		{
			Sleep_x1ms(10);
		} while(theStepper.IsBusy(theCuEliteOnloader.m_ucForkHolderStepId));

		theCuEliteOnloader.moveMagClamperStepBackward();
	}
	else
	{
		theCuEliteOffloader.thePullyMagExistSensor.UpdateSensorState();
		if(theCuEliteOffloader.thePullyMagExistSensor.GetSensorState() == TRUE)
		{
			short  anMsgArray[1]; 
			anMsgArray[0] = MSG_OFFLOAD_MAG_PULLY_FULL;
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

			GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);
			return ;
		}

		theCuEliteOffloader.moveMagClamperStepForward();
		Sleep_x1ms(300);
		theCuEliteOffloader.theMagDropSensor.UpdateSensorState();
		if (theCuEliteOffloader.theMagDropSensor.GetSensorState() == SENSOR_ON)
		{
			CStringList sList;
			sList.AddTail(theMsg.GetMsg( MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
		}

		theCuEliteOffloader.theMagPullyDcMotor.TriggerBacktoHome();
		Sleep_x1ms(200);
		theCuEliteOffloader.SearchOffloadElvStepHome();
		Sleep_x1ms(10);
		theCuEliteOffloader.WaitStepHomeFinish();
		Sleep_x1ms(10);
		theCuEliteOffloader.theMagPullyDcMotor.TriggerForwardMotion();
		Sleep_x1ms(200);
		theCuEliteOffloader.theMagPullyDcMotor.TriggerBacktoHome();
		Sleep_x1ms(200);
		theCuEliteOffloader.MoveTotheSlot(m_nCurrentSlot);
		theCuEliteOffloader.SetCurrentSlotNum(1);
		Sleep_x1ms(100);

		while (theStepper.IsBusy(theCuEliteOffloader.m_ucForkHolderStepId))
		{
			Sleep_x1ms(10);
		}

		theCuEliteOffloader.moveMagClamperStepBackward();
	}

	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);

	//m_nCurrentSlot = 1;
	UpdateData(false);
}

void CCuEliteStepperPage::OnBnClickedButtonPusherHome()
{
	GetDlgItem(IDC_BUTTON_PUSHER_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PUSHER_LIMIT)->EnableWindow(FALSE);

	if (theCuEliteOnloader.theInjectPusher.TriggerBacktoHome() != FOK)
	{
		short anMsgArray[1]; 
		anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
		theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
	}

	GetDlgItem(IDC_BUTTON_PUSHER_LIMIT)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnBnClickedButtonPusherLimit()
{
	GetDlgItem(IDC_BUTTON_PUSHER_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PUSHER_LIMIT)->EnableWindow(FALSE);

	int iPusherStatus;
	iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerForwardMotion();
	if (iPusherStatus != FOK)
	{
		short anMsgArray[1]; 
		anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;
		theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
	}

	GetDlgItem(IDC_BUTTON_PUSHER_HOME)->EnableWindow(TRUE);
}

void CCuEliteStepperPage::OnEnSetfocusEditCurrentSlot()
{
	GetDlgItem(IDC_STATIC_LABLE_ELEV)->SetFocus();

	CUELITE_DEVICE_RECORD* lpstCuEliteDevicePara=new CUELITE_DEVICE_RECORD;
	theDevice.GetCuEliteRcd(lpstCuEliteDevicePara);

	theKeyPad.GetUserKeyInData(&m_nCurrentSlot,1,0,lpstCuEliteDevicePara->lMagSlotNum);

	delete lpstCuEliteDevicePara;

	UpdateData(false);

	OnEnChangeEditCurrentSlot();
}

void CCuEliteStepperPage::OnEnChangeEditCurrentSlot()
{
	UpdateData(true);

	if (m_iElevatorId == ELEVATOR_ONLOADER)
	{
		theCuEliteOnloader.MoveTotheSlot(m_nCurrentSlot);
		theCuEliteOnloader.WaitMagHoledrMoveFinish();
	}
	else
	{
		theCuEliteOffloader.MoveTotheSlot(m_nCurrentSlot);
		theCuEliteOffloader.WaitMagHoledrMoveFinish();
	}
}
