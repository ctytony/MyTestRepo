// TopSteppperPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "TopSteppperPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "StepperControl.h"
#include "MaterialHandlerThread.h"
#include "MathCalc.h"
#include "MachineConfig.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "DlgElevCtrl.h"
#include "Busy.h"

// CTopSteppperPage dialog

IMPLEMENT_DYNAMIC(CTopSteppperPage, CPropertyPage)

CTopSteppperPage::CTopSteppperPage()
	: CPropertyPage(CTopSteppperPage::IDD)
	, m_iElevatorId(TOP_ELEV_ONLOAD)
{
	m_bBusy=FALSE;		//V1.1.W248 add
}

CTopSteppperPage::~CTopSteppperPage()
{
}

void CTopSteppperPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_SIDE_ID, m_iElevatorId);
}

BEGIN_MESSAGE_MAP(CTopSteppperPage, CPropertyPage)
	ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO_SIDE_ID, OnSideId)
    ON_BN_CLICKED(IDC_BUTTON_HOLDER_HOME, OnHolderHome)
	ON_BN_CLICKED(IDC_BUTTON_HOLDER_STOP, OnMagHolderLimit)
	ON_BN_CLICKED(IDC_BUTTON_PULLY_HOME, OnPullyHome)
	ON_BN_CLICKED(IDC_BUTTON_PULLY_LIMIT, OnPullyLimit)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_SLOT, OnNextSlot)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY, OnStandby)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD, OnUnload)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_HOME, OnPusherHome)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_LIMIT, OnPusherLimit)
    ON_BN_CLICKED(IDC_BUTTON_MAGCLAMP_HOME, OnClamperHome)
    ON_BN_CLICKED(IDC_BUTTON_MAGCLAMP_POSITION, OnClamperStandBy)
	ON_BN_CLICKED(IDC_BUTTON_MAGCLAMP_FORWARD, OnClamperForWard)
    ON_BN_CLICKED(IDC_BUTTON_MAGCLAMP_BACKWARD, OnClamperBackWard)
    ON_BN_CLICKED(IDC_BUTTON_MAGCLAMP_STOP, OnClamperStop)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W248 add
END_MESSAGE_MAP()

// CTopSteppperPage message handlers
BOOL CTopSteppperPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	m_iElevatorId = TOP_ELEV_ONLOAD;

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SIDE_ID);
	pComboBox->AddString(theMsg.GetMsg(MSG_INPUT));
	pComboBox->AddString(theMsg.GetMsg(MSG_OUTPUT));

	SetDlgItemText(IDC_STATIC_LABLE_ELEV, theMsg.GetMsg(MSG_ELEVATOR));
	SetDlgItemText(IDC_STATIC_ELEVATOR, theMsg.GetMsg(MSG_ELEVATOR));
	SetDlgItemText(IDC_STATIC_PULLY_MOTOR, theMsg.GetMsg(MSG_PULLY_MOTOR));
	SetDlgItemText(IDC_STATIC_HOLDING_FORK, theMsg.GetMsg(MSG_HOLDING_FORK));
	SetDlgItemText(IDC_BUTTON_PULLY_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemText(IDC_BUTTON_PULLY_LIMIT, theMsg.GetMsg(MSG_FORWARD));
	SetDlgItemText(IDC_BUTTON_HOLDER_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemText(IDC_BUTTON_HOLDER_STOP, theMsg.GetMsg(MSG_LIMIT));
	SetDlgItemText(IDC_BUTTON_NEXT_SLOT, theMsg.GetMsg(MSG_NEXT_SLOT));
	SetDlgItemText(IDC_BUTTON_STANDBY, theMsg.GetMsg(MSG_STANDBY_POS));
	SetDlgItemText(IDC_BUTTON_UNLOAD, theMsg.GetMsg(MSG_INITIALIZE));
	SetDlgItemText(IDC_STATIC_PUSH_KICKER, theMsg.GetMsg(MSG_ONLOAD_PUSHER_KICKER));
	SetDlgItemText(IDC_BUTTON_PUSHER_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemText(IDC_BUTTON_PUSHER_LIMIT, theMsg.GetMsg(MSG_LIMIT));
	SetDlgItemText(IDC_STATIC_MAGCLAMPER, theMsg.GetMsg(MSG_MAG_CLAMPER));
	SetDlgItemText(IDC_BUTTON_MAGCLAMP_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemText(IDC_BUTTON_MAGCLAMP_STOP, theMsg.GetMsg(MSG_STOP));
	SetDlgItemText(IDC_BUTTON_MAGCLAMP_POSITION, theMsg.GetMsg( MSG_CLAMP_POS));
	SetDlgItemText(IDC_BUTTON_MAGCLAMP_FORWARD, theMsg.GetMsg(MSG_FORWARD));
	SetDlgItemText(IDC_BUTTON_MAGCLAMP_BACKWARD, theMsg.GetMsg(MSG_BACKWARD));

	return CPropertyPage::OnInitDialog();
}

HBRUSH CTopSteppperPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTopSteppperPage::OnSideId()
{
	UpdateData(true);
	m_iElevatorId == TOP_ELEV_ONLOAD ? theTopOnloader.GetCurSlot() : theTopOffloader.GetCurSlot();
	UpdateData(false);

	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnHolderHome()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_HOLDER_HOME);
	pWnd->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_HOLDER_STOP)->EnableWindow(FALSE);

	if (m_iElevatorId == TOP_ELEV_ONLOAD)
	{
		CDlgElevCtrl objElev;
		if (objElev.IsPusherError( ))
		{
			pWnd->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_HOLDER_STOP)->EnableWindow(TRUE);
			return;
		}

    	theTopTrack.MoveIndexerToLogicZero();
		Sleep_x1ms(100);

		//theTopOnloader 
		theTopOnloader.HomeMagHolder();
		Sleep_x1ms(100);
		theTopOnloader.WaitHomeMagHolder();
	}
	else
	{
		//theTopOffloader
		theTopOffloader.HomeMagHolder();
		Sleep_x1ms(100);
		theTopOffloader.WaitHomeMagHolder();
	}

	pWnd->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HOLDER_STOP)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnMagHolderLimit()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_HOLDER_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HOLDER_STOP)->EnableWindow(FALSE);

	if(m_iElevatorId==TOP_ELEV_ONLOAD)
	{
		theTopOnloader.MoveMagHolderToSlot(1);
		theTopOnloader.WaitMoveMagHolder();
	}
	else
	{
		theTopOffloader.MoveMagHolderToSlot(1);
		theTopOffloader.WaitMoveMagHolder();
	}

	GetDlgItem(IDC_BUTTON_HOLDER_HOME)->EnableWindow();
	GetDlgItem(IDC_BUTTON_HOLDER_STOP)->EnableWindow();

}

void CTopSteppperPage::OnPullyHome()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(FALSE);
	
	if (m_iElevatorId == TOP_ELEV_ONLOAD)
	{
        if (theTopOffloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
        {
            CStringList sList;
			sList.AddTail(theMsg.GetMsg( MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
        }

		theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();		
	}
	else
	{
        if (theTopOffloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
        {
            CStringList sList;
			sList.AddTail(theMsg.GetMsg( MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
        }

		theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
	}

	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnPullyLimit()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PULLY_LIMIT)->EnableWindow(FALSE);
	
	if(m_iElevatorId == TOP_ELEV_ONLOAD)
		theTopOnloader.m_MagPullyDcMotor.TriggerForwardMotion();
	else
		theTopOffloader.m_MagPullyDcMotor.TriggerForwardMotion();

	GetDlgItem(IDC_BUTTON_PULLY_HOME)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnNextSlot()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_NEXT_SLOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STANDBY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(FALSE);

	long lStartTime = GetCurrentTime();
	
	if(m_iElevatorId == TOP_ELEV_ONLOAD)
	{
		theTopOnloader.SubmitOnloadEvent(TOP_ONLOAD_EVENT_START);

		do
		{
			theTopOnloader.ProcessOnloadEvent();

			Sleep_x1ms(2);
		}
		while(theTopOnloader.GetEvent()!=TOP_ONLOAD_EVENT_IDLE && GetCurrentTime() - lStartTime < 30000);
	}
	else
	{
		theTopOffloader.SubmitOffloadEvent(TOP_OFFLOAD_EVENT_START);

		do
		{
			theTopOffloader.ProcessOffloadEvent();

			Sleep_x1ms(2);
		}
		while(theTopOffloader.GetEvent()!=TOP_OFFLOAD_EVENT_IDLE && GetCurrentTime() - lStartTime < 30000);
	}

	UpdateData(false);

	GetDlgItem(IDC_BUTTON_NEXT_SLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STANDBY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnStandby()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	if (m_iElevatorId == TOP_ELEV_ONLOAD)
	{		
    	theTopTrack.MoveIndexerToLogicZero();
		Sleep_x1ms(200);

		theTopOnloader.MoveMagHolderToSlot(1);
		theTopOnloader.WaitMoveMagHolder();
	}
	else
	{
		theTopOffloader.MoveMagHolderToSlot(1);
		theTopOffloader.WaitMoveMagHolder();
	}

	UpdateData(false);
}

void CTopSteppperPage::OnUnload()  //@1
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(FALSE);

	theTopTrack.MoveIndexerToLogicZero();

	if(m_iElevatorId == TOP_ELEV_ONLOAD) 
	{
		if(theTopOnloader.m_PullyMagExistSensor.GetCurrentState() == TRUE)
		{
			short anMsgArray[1] = {MSG_ONLOAD_MAG_PULLY_FULL};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

			GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);

			return ;
		}

		CDlgElevCtrl objElev;
		if (objElev.IsPusherError())
		{
			GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);
			return;
		}

		theTopOnloader.MoveMagClamperForward();
	    Sleep_x1ms(300);

        if (theTopOffloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
        {
            CStringList sList;
			sList.AddTail(theMsg.GetMsg( MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
        }

		theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
		Sleep_x1ms(200);
		theTopOnloader.HomeMagHolder();
	    Sleep_x1ms(10);
		theTopOnloader.WaitHomeMagHolder();
	    Sleep_x1ms(10);
		theTopOnloader.m_MagPullyDcMotor.TriggerForwardMotion();
		Sleep_x1ms(200);

        if (theTopOffloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
        {
            CStringList sList;
			sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
        }

		theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
	    Sleep_x1ms(200);

		do
	    {
			Sleep_x1ms(10);
		}
		while(theStepper.IsBusy(theTopOnloader.m_nMagHolderStepperId) != 0);

		theTopOnloader.MoveMagClamperBackward();
	}
	else
	{
		if(theTopOffloader.m_PullyMagExistSensor.GetCurrentState() == TRUE)
		{
			short anMsgArray[1] = {MSG_OFFLOAD_MAG_PULLY_FULL};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

			GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);

			return ;
		}

		theTopOffloader.MoveMagClamperForward();
		Sleep_x1ms(300);

        if (theTopOffloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
        {
            CStringList sList;
			sList.AddTail(theMsg.GetMsg( MSG_ONLOAD_MAG_DROP_DETECTED));
			theMsg.ShowMsg(WARNING_MSG, sList);
        }

		theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
		Sleep_x1ms(200);
		theTopOffloader.HomeMagHolder();
		Sleep_x1ms(10);
		theTopOffloader.WaitHomeMagHolder();
		Sleep_x1ms(10);
		theTopOffloader.m_MagPullyDcMotor.TriggerForwardMotion();
		Sleep_x1ms(200);
		theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
		Sleep_x1ms(200);
		theTopOffloader.SetCurSlot(1);

		do
		{
			Sleep_x1ms(10);
		}
		while (theStepper.IsBusy(theTopOffloader.m_nMagHolderStepperId) != 0);

		theTopOffloader.MoveMagClamperBackward();
	}

	GetDlgItem(IDC_BUTTON_UNLOAD)->EnableWindow(TRUE);

	UpdateData(false);
}

void CTopSteppperPage::OnPusherHome()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_PUSHER_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PUSHER_LIMIT)->EnableWindow(FALSE);

	if (theTopOnloader.m_InjectPusher.TriggerBacktoHome() != FOK)
	{
		short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
		theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
	}

	GetDlgItem(IDC_BUTTON_PUSHER_LIMIT)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnPusherLimit()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_PUSHER_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PUSHER_LIMIT)->EnableWindow(FALSE);

	if (theTopOnloader.m_InjectPusher.TriggerForwardMotion() != FOK)
	{
		short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
		theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
	}

	GetDlgItem(IDC_BUTTON_PUSHER_HOME)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnClamperHome()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

    GetDlgItem(IDC_BUTTON_MAGCLAMP_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MAGCLAMP_POSITION)->EnableWindow(FALSE);

    if (m_iElevatorId == TOP_ELEV_ONLOAD)
    {
        theTopOnloader.HomeMagClamper();
        Sleep_x1ms(10);
        theTopOnloader.WaitHomeMagClamper();
    }
    else
    {
        theTopOffloader.HomeMagClamper();
        Sleep_x1ms(10);
        theTopOffloader.WaitHomeMagClamper();
    }

	GetDlgItem(IDC_BUTTON_MAGCLAMP_HOME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAGCLAMP_POSITION)->EnableWindow(TRUE);
}
	
void CTopSteppperPage::OnClamperForWard()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

    GetDlgItem(IDC_BUTTON_MAGCLAMP_FORWARD)->EnableWindow(FALSE);

    if(m_iElevatorId == TOP_ELEV_ONLOAD)
        theTopOnloader.MoveMagClamperForward();
    else 
        theTopOffloader.MoveMagClamperForward();

	GetDlgItem(IDC_BUTTON_MAGCLAMP_FORWARD)->EnableWindow(TRUE);
}
    
void CTopSteppperPage::OnClamperBackWard()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

    GetDlgItem(IDC_BUTTON_MAGCLAMP_BACKWARD)->EnableWindow(FALSE);

    if(m_iElevatorId == TOP_ELEV_ONLOAD)
        theTopOnloader.MoveMagClamperBackward();
    else 
        theTopOffloader.MoveMagClamperBackward();

	GetDlgItem(IDC_BUTTON_MAGCLAMP_BACKWARD)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnClamperStandBy()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

	GetDlgItem(IDC_BUTTON_MAGCLAMP_HOME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MAGCLAMP_POSITION)->EnableWindow(FALSE);

    if (m_iElevatorId == TOP_ELEV_ONLOAD)
        theTopOnloader.SearchMoveMagClamperToClampPos();
    else 
        theTopOffloader.SearchMoveMagClamperToClampPos();

	GetDlgItem(IDC_BUTTON_MAGCLAMP_HOME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAGCLAMP_POSITION)->EnableWindow(TRUE);
}

void CTopSteppperPage::OnClamperStop()
{
	CBusy bs(&m_bBusy);		//V1.1.W248 add

    if (m_iElevatorId == TOP_ELEV_ONLOAD)
        theTopOnloader.StopMagClamper();
    else 
        theTopOffloader.StopMagClamper();

	GetDlgItem(IDC_BUTTON_MAGCLAMP_HOME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MAGCLAMP_POSITION)->EnableWindow(TRUE);
}

LRESULT CTopSteppperPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.1.W248 add
{
	return m_bBusy;
}
