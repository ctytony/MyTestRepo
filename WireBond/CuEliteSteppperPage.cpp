// CuEliteSteppperPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MachineData.h"	//v2.18.6.T20 add
#include "DeviceDataMgr.h"	//v2.18.6.T20 add
#include "CuEliteSteppperPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "StepperControl.h"//v2.20.Q1 edit
#include "MaterialHandlerThread.h"
#include "CommFunc.h"
#include "MachineConfig.h"
// CCuEliteSteppperPage dialog


IMPLEMENT_DYNAMIC(CCuEliteSteppperPage, CPropertyPage)

CCuEliteSteppperPage::CCuEliteSteppperPage()
	: CPropertyPage(CCuEliteSteppperPage::IDD)
	, m_nElevatorId(ID_ONLOAD_ELEVATOR_MOTOR)
	, m_nCurrentSlot(0)
    , m_nStepMotorId(ID_ONLOAD_X_AXIS_MOTOR)
    , m_lScale(1000)

{
    m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
}

CCuEliteSteppperPage::~CCuEliteSteppperPage()
{
}

void CCuEliteSteppperPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_ELEVATOR, m_nElevatorId);
    DDX_Text(pDX, IDC_EDIT_CURRENT_DEPTH,m_lCurrentMagClampDepth);
	DDX_Text(pDX, IDC_EDIT_CURRENT_SLOT, m_nCurrentSlot);
    DDX_CBIndex(pDX, IDC_COMBO_MOTOR, m_nStepMotorId);
    DDX_Text(pDX, IDC_EDIT_SCALE, m_lScale);
}


BEGIN_MESSAGE_MAP(CCuEliteSteppperPage, CPropertyPage)
	ON_WM_CTLCOLOR()



	ON_EN_SETFOCUS(IDC_EDIT_SCALE, &CCuEliteSteppperPage::OnEnSetfocusEditScale)
    ON_CBN_SELCHANGE(IDC_COMBO_ELEVATOR, &CCuEliteSteppperPage::OnCbnSelchangeComboSideId)
    ON_BN_CLICKED(IDC_BUTTON_HOLDER_HOME, &CCuEliteSteppperPage::OnBnClickedButtonHolderHome)
	ON_BN_CLICKED(IDC_BUTTON_HOLDER_STOP, &CCuEliteSteppperPage::OnBnClickedButtonHolderStop)
	ON_BN_CLICKED(IDC_BUTTON_PULLY_HOME, &CCuEliteSteppperPage::OnBnClickedButtonPullyHome)
	ON_BN_CLICKED(IDC_BUTTON_PULLY_LIMIT, &CCuEliteSteppperPage::OnBnClickedButtonPullyLimit)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_SLOT, &CCuEliteSteppperPage::OnBnClickedButtonNextSlot)
   // ON_BN_CLICKED(IDC_BUTTON_STANDBY, &CCuEliteSteppperPage::OnBnClickedButtonStandby)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD, &CCuEliteSteppperPage::OnBnClickedButtonUnload)
	ON_EN_SETFOCUS(IDC_EDIT_CURRENT_SLOT, &CCuEliteSteppperPage::OnEnSetfocusEditCurrentSlot)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_SLOT, &CCuEliteSteppperPage::OnEnChangeEditCurrentSlot)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_HOME, &CCuEliteSteppperPage::OnBnClickedButtonPusherHome)
	ON_BN_CLICKED(IDC_BUTTON_PUSHER_LIMIT, &CCuEliteSteppperPage::OnBnClickedButtonPusherLimit)
    ON_CBN_SELCHANGE(IDC_COMBO_MOTOR, &CCuEliteSteppperPage::OnLbnSelchangeListStepperMotor)
    ON_BN_CLICKED(IDC_BUTTON_FORWARD_, &CCuEliteSteppperPage::OnBnClickedButtonForward)
	ON_BN_CLICKED(IDC_BUTTON_BACKWARD_, &CCuEliteSteppperPage::OnBnClickedButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON_HOME_STEP, &CCuEliteSteppperPage::OnBnClickedButtonHomeStep)
    ON_BN_CLICKED(IDC_BUTTON_HOLDER_STANDBY, &CCuEliteSteppperPage::OnBnClickedButtonHolderStandBy)
  

	//v2.18.3.S1 add
    ON_BN_CLICKED(IDC_BUTTON_MAGCLAMP_FORWARD, &CCuEliteSteppperPage::OnBnClickedButtonMagclampForward)
    ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_BACKWARD, &CCuEliteSteppperPage::OnBnClickedButtonMagClampBackward)

	/*v2.18.5.w4 delete {
	ON_STN_CLICKED(IDC_STATIC_CURRENT_SLOT2, &CCuEliteSteppperPage::OnStnClickedStaticCurrentSlot2)
    }*/

    ON_EN_SETFOCUS(IDC_EDIT_CURRENT_DEPTH, &CCuEliteSteppperPage::OnEnSetfocusCurrentDepth)
	ON_BN_CLICKED(IDC_BUTTON_STAND_BY, &CCuEliteSteppperPage::OnBnClickedButtonStandBy)



END_MESSAGE_MAP()


// CCuEliteSteppperPage message handlers

BOOL CCuEliteSteppperPage::OnInitDialog()
{
	CString szString,sString;



	CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        pChildWnd->SetFont(theApp.m_pCurrentFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LABLE_ELEV);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
	pWnd->SetWindowTextW(szString);
  
	pWnd = GetDlgItem(IDC_STATIC_ELEVATOR);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_STATIC_MOVE_DIST);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOVE_DIST, sString);
	szString.Format(_T("%s:"),sString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_PULLY_MOTOR);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_PULLY_MOTOR, szString);
	pWnd->SetWindowTextW(szString);

	//v2.18.3.S1 add
    pWnd =GetDlgItem(IDC_STATIC_FRONT_CLAMPER);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_CLAMPER, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_HOLDING_FORK);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOLDING_FORK, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_CURRENT_SLOT);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_CURRENT_SLOT, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_CURRENT_DEPTH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_CURRENT_DEPTH, sString);
	szString.Format(_T("%s:"),sString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_FRONT_CLAMPER);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, sString);
	szString.Format(_T("%s:"),sString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_BUTTON_PULLY_HOME);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_STATIC_MOTOR);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOTORS, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_PULLY_LIMIT);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FORWARD, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_HOLDER_HOME);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_HOLDER_STOP);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_STOP, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_NEXT_SLOT);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_NEXT_SLOT, szString);
	pWnd->SetWindowTextW(szString);

    pWnd =GetDlgItem(IDC_BUTTON_HOLDER_STANDBY);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_UNLOAD);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_INITIALIZE, szString);
	pWnd->SetWindowTextW(szString);

	 CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEVATOR);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, sString);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
     pComboBox->InsertString(0, sString+szString); 
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, sString);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
     pComboBox->InsertString(1, sString+szString); 

    pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
    pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_BUTTON_BACKWARD_);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_BACKWARD, szString);
    pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_BUTTON_FORWARD_);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FORWARD, szString);
    pWnd->SetWindowTextW(szString);


	pWnd = GetDlgItem(IDC_BUTTON_STAND_BY);
	theMsgMgr.GetMsgString( MSG_MENU_MAGCLAMP_POSITION, szString);
    pWnd->SetWindowTextW(szString);

	 pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_BACKWARD);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_OPEN, szString);
	 pWnd->SetWindowTextW(szString);


	 pWnd =GetDlgItem(IDC_BUTTON_MAGCLAMP_FORWARD);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_CLAMP, szString);
	 pWnd->SetWindowTextW(szString);

	
	 pWnd =GetDlgItem(IDC_STATIC_PUSH_KICKER);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOADER_PUSHER_KICKER, szString);
	 pWnd->SetWindowTextW(szString);

	 pWnd =GetDlgItem(IDC_BUTTON_PUSHER_HOME);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	 pWnd->SetWindowTextW(szString);

	 pWnd =GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_PUSH_LIMIT, szString);
	 pWnd->SetWindowTextW(szString);


    pWnd = GetDlgItem(IDC_COMBO_MOTOR);
    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTOR);
    pComboBox->ResetContent();

	theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
    pComboBox->InsertString(0, sString+szString); 
	theMsgMgr.GetMsgString(MSG_MAGAZINE_SIDE_CLAMPER, szString);
	pComboBox->InsertString(1, sString+szString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
    pComboBox->InsertString(2, sString+szString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_REAR_CLAMPER, szString);
    pComboBox->InsertString(3, sString+szString);

	theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
    pComboBox->InsertString(4, sString+szString); 
	theMsgMgr.GetMsgString(MSG_MAGAZINE_SIDE_CLAMPER, szString);
	pComboBox->InsertString(5, sString+szString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
    pComboBox->InsertString(6, sString+szString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_REAR_CLAMPER, szString);
    pComboBox->InsertString(7, sString+szString);

    m_nStepMotorId = ID_ONLOAD_Z_AXIS_MOTOR;
	pWnd = GetDlgItem(IDC_BUTTON_STAND_BY);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS , szString);
	pWnd->SetWindowTextW(szString);
			

	CPropertyPage::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCuEliteSteppperPage::OnSetActive( )
{
  CString sPath;
  CWnd* pWnd;

  if(CPropertyPage::OnSetActive()) {

    CUELITE_ONLOAD_RCD  stOnloadData;		
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);		


	m_nElevatorId = ID_ONLOAD_ELEVATOR_MOTOR;
	m_nCurrentSlot = theCuEliteOnLoader.GetCurrentSlotNum();                
	theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO, stOnloadData.lOnLoadMagClamperDepth ,  & m_lCurrentMagClampDepth);

	UpdateData(false);

  }
	return true;
}



HBRUSH CCuEliteSteppperPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	// TODO:  Change any attributes of the DC here
	   HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CCuEliteSteppperPage::OnCbnSelchangeComboSideId()
{
	// TODO: Add your control notification handler code here

    CWnd* pWnd;
    CUELITE_ONLOAD_RCD  stOnloadData;		
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;		
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);		



	UpdateData(true);
		if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
		//theCuEliteOnLoader 
		m_nCurrentSlot = theCuEliteOnLoader.GetCurrentSlotNum();

        theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO, stOnloadData.lOnLoadMagClamperDepth ,  & m_lCurrentMagClampDepth);

		pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_PUSH_KICKER);
		pWnd->ShowWindow(TRUE);

	}
	else
	{
		//theCuEliteOffLoader
		m_nCurrentSlot = theCuEliteOffLoader.GetCurrentSlotNum();

        theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO, stOffloadData.lOffLoadMagClamperDepth ,  & m_lCurrentMagClampDepth);

		pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_PUSH_KICKER);
		pWnd->ShowWindow(FALSE);


	}
	UpdateData(false);
}

void CCuEliteSteppperPage::OnBnClickedButtonHolderHome()
{
	// TODO: Add your control notification handler code here
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
		
		int iPusherStatus;
		iPusherStatus=theCuEliteOnLoader.theInjectPusher.TriggerBacktoHome();
		if(iPusherStatus == FNOK)
		{

			CString str;
			CStringList szStringList;
		    theMsgMgr.GetMsgString(MSG_ONLOAD_PUSH_KICKER_WRONG_STATE, str);
	        szStringList.AddTail(str);
	        theMsgMgr.ShowMessage(WARNING_MSG, szStringList);


			return;

		}
		
    	theCuEliteTrack.MoveToLogicZero();
		    Sleep(100);

		//theCuEliteOnLoader 
		theCuEliteOnLoader.SearchOnloadElvStepHome();
		Sleep(100);
		theCuEliteOnLoader.WaitStepHomeFinish();
		
	}
	else
	{
		//theCuEliteOffLoader
		theCuEliteOffLoader.SearchOffloadElvStepHome();
		Sleep(100);
		theCuEliteOffLoader.WaitStepHomeFinish();
	
	}

}

void CCuEliteSteppperPage::OnBnClickedButtonHolderStop()
{
	// TODO: Add your control notification handler code here
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
		theStepperControl.StopStep(CUELITE_ONLOAD_ELEVATOR);
		
	}
	else
	{
		theStepperControl.StopStep(CUELITE_OFFLOAD_ELEVATOR);
	
	}

}

void CCuEliteSteppperPage::OnBnClickedButtonPullyHome()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PULLY_HOME);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_PULLY_LIMIT);
	pWnd->EnableWindow(FALSE);

	
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{

		theCuEliteOnLoader.moveMagazineEjectorBackToHome();
		
	}
	else
	{

		theCuEliteOffLoader.moveMagazineEjectorBackToHome();
	
	}

	pWnd = GetDlgItem(IDC_BUTTON_PULLY_HOME);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_PULLY_LIMIT);
	pWnd->EnableWindow(TRUE);



}

void CCuEliteSteppperPage::OnBnClickedButtonPullyLimit()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PULLY_HOME);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_PULLY_LIMIT);
	pWnd->EnableWindow(FALSE);

	
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
		theCuEliteOnLoader.moveMagazineEjectorForward();
		
	}
	else
	{
		theCuEliteOffLoader.moveMagazineEjectorForward();
	
	}

	pWnd = GetDlgItem(IDC_BUTTON_PULLY_HOME);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_BUTTON_PULLY_LIMIT);
	pWnd->EnableWindow(FALSE);
}


void CCuEliteSteppperPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	


	CPropertyPage::OnOK();
}



void CCuEliteSteppperPage::OnBnClickedButtonNextSlot()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_NEXT_SLOT);
	pWnd->EnableWindow(FALSE);


	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_UNLOAD);
	pWnd->EnableWindow(FALSE);

	long lStartTime = GetCurrentTime();
	
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{


		theCuEliteOnLoader.SubmitOnloadEvent(CUELITE_ONLOAD_EVENT_START);

		do
		{

			theCuEliteOnLoader.ProcessOnloadEvent();

			Sleep(2);

			if(GetCurrentTime() - lStartTime > 30000)
				break;

		}
		while(theCuEliteOnLoader.GetOnloadEvent()!=CUELITE_ONLOAD_EVENT_IDLE);


		m_nCurrentSlot = theCuEliteOnLoader.GetCurrentSlotNum();

		UpdateData(false);

		
	}
	else
	{
		theCuEliteOffLoader.SubmitOffloadEvent(CUELITE_OFFLOAD_EVENT_START);

		do
		{

			theCuEliteOffLoader.ProcessOffloadEvent();

			Sleep(2);

			if(GetCurrentTime() - lStartTime > 30000)
			break;

		}
		while(theCuEliteOffLoader.GetOffloadEvent()!=CUELITE_OFFLOAD_EVENT_IDLE);


		m_nCurrentSlot = theCuEliteOffLoader.GetCurrentSlotNum();

		UpdateData(false);
	
	}

	pWnd = GetDlgItem(IDC_BUTTON_NEXT_SLOT);	pWnd->EnableWindow(TRUE);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_BUTTON_UNLOAD);
	pWnd->EnableWindow(TRUE);


}




void CCuEliteSteppperPage::OnBnClickedButtonUnload()  //@1
{
	// TODO: Add your control notification handler code here

	int iBusyState1, iBusyState2;

	CWnd* pWnd =GetDlgItem(IDC_BUTTON_UNLOAD);
	pWnd->EnableWindow(FALSE);

	theCuEliteTrack.MoveToLogicZero();

	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR) 
	{

		theCuEliteOnLoader.thePullyMagExistSensor.UpdateSensorState();
		if(theCuEliteOnLoader.thePullyMagExistSensor.GetSensorState() == TRUE)
		{
			short   nMsgNum, anMsgArray[10]; 

			nMsgNum = 1;
			anMsgArray[0] = MSG_MENU_TOP_ONLOAD_FULL;

			theMsgMgr.ShowMessage(WARNING_MSG, nMsgNum, anMsgArray);

			return ;


		}

		
		int iPusherStatus;
		iPusherStatus=theCuEliteOnLoader.theInjectPusher.TriggerBacktoHome();
		if(iPusherStatus == FNOK)
		{
			short   nMsgNum, anMsgArray[10]; 

			nMsgNum = 1;
			anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG_STATE;

			theMsgMgr.ShowMessage(WARNING_MSG, nMsgNum, anMsgArray);

			return;

		}

		/*theCuEliteOnLoader.theMagClampCylinder.TriggerForwardMotion();
	    Sleep(300);*/

		theCuEliteOnLoader.moveMagazineEjectorBackToHome();

		theCuEliteOnLoader.SearchOnloadElvStepHome();
	    Sleep(10);
		theCuEliteOnLoader.WaitStepHomeFinish();
	    Sleep(10);
		theCuEliteOnLoader.moveMagazineEjectorForward();
		Sleep(200);

		theCuEliteOnLoader.moveMagazineEjectorBackToHome();

		theCuEliteOnLoader.MoveTotheSlot(1);
	    theCuEliteOnLoader.SetCurrentSlotNum(1);
	    Sleep(100);
		do
	    {

		   iBusyState1 = theStepperControl.CheckStepBusyState(CUELITE_ONLOAD_ELEVATOR);
		
		}
		while(iBusyState1 != 0);

		/*theCuEliteOnLoader.theMagClampCylinder.TriggerBackwardMotion();*/

	}
	else
	{

		theCuEliteOffLoader.thePullyMagExistSensor.UpdateSensorState();
		if(theCuEliteOffLoader.thePullyMagExistSensor.GetSensorState() == TRUE)
		{
			short   nMsgNum, anMsgArray[10]; 

			nMsgNum = 1;
			anMsgArray[0] = MSG_MENU_TOP_OFFLOAD_FULL;

			theMsgMgr.ShowMessage(WARNING_MSG, nMsgNum, anMsgArray);

			return ;


		}

		/*theCuEliteOffLoader.theMagClampCylinder.TriggerForwardMotion();
		Sleep(300);*/

		theCuEliteOffLoader.moveMagazineEjectorBackToHome();

		Sleep(200);
		theCuEliteOffLoader.SearchOffloadElvStepHome();
			Sleep(10);
		theCuEliteOffLoader.WaitStepHomeFinish();
			Sleep(10);
		theCuEliteOffLoader.moveMagazineEjectorForward();
			Sleep(200);

		theCuEliteOffLoader.moveMagazineEjectorBackToHome();

		theCuEliteOffLoader.MoveTotheSlot(1);
		theCuEliteOffLoader.SetCurrentSlotNum(1);
			Sleep(100);

		do
		{
			
			iBusyState2 = theStepperControl.CheckStepBusyState(CUELITE_OFFLOAD_ELEVATOR);
				Sleep(10);
		}
		while(iBusyState2 != 0);

		
		/*theCuEliteOffLoader.theMagClampCylinder.TriggerBackwardMotion();*/

	}


	pWnd =GetDlgItem(IDC_BUTTON_UNLOAD);
	pWnd->EnableWindow(TRUE);


	m_nCurrentSlot = 1;
	UpdateData(false);


}

//void CCuEliteSteppperPage::OnBnClickedButtonPusherHome()
//{
//	// TODO: Add your control notification handler code here
//	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
//	pWnd->EnableWindow(FALSE);
//	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
//	pWnd->EnableWindow(FALSE);
//
//	
//	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
//	{
//		theCuEliteOnLoader.theDevPusherDcMotor.TriggerBacktoHome();
//		
//	}
//
//
//	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
//	pWnd->EnableWindow(TRUE);
//	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
//	pWnd->EnableWindow(TRUE);
//
//
//}

//void CCuEliteSteppperPage::OnBnClickedButtonPusherLimit()
//{
//	// TODO: Add your control notification handler code here
//	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
//	pWnd->EnableWindow(FALSE);
//	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
//	pWnd->EnableWindow(FALSE);
//
//	
//	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
//	{
//		theCuEliteOnLoader.theDevPusherDcMotor.TriggerForwardMotion();
//		
//	}
//
//
//	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
//	pWnd->EnableWindow(TRUE);
//	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
//	pWnd->EnableWindow(TRUE);
//
//
//}

void CCuEliteSteppperPage::OnEnSetfocusEditCurrentSlot()
{
	// TODO: Add your control notification handler code here
   // UpdateData(false);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PULLY_HOME);
    pWnd->SetFocus();

	CUELITE_MHS_DEVICE_RECORD  stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);
    
	if(!theKeyPad.GetUserKeyInData(&m_nCurrentSlot,1,0,stCuEliteDevicePara.lMagSlotNum)){;

	  // UpdateData(true);
		 UpdateData(false);

		 OnEnChangeEditCurrentSlot();
	}
}

void CCuEliteSteppperPage::OnEnChangeEditCurrentSlot()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(true);

	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
		theCuEliteOnLoader.MoveTotheSlot(m_nCurrentSlot);
		
	}
	else
	{
		theCuEliteOffLoader.MoveTotheSlot(m_nCurrentSlot);
		
	}

	// TODO:  Add your control notification handler code here
}

void CCuEliteSteppperPage::OnBnClickedButtonPusherHome()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
	pWnd->EnableWindow(FALSE);

	theCuEliteOnLoader.theInjectPusher.TriggerBacktoHome();

	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
	pWnd->EnableWindow(TRUE);
}

void CCuEliteSteppperPage::OnBnClickedButtonPusherLimit()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
	pWnd->EnableWindow(FALSE);

	theCuEliteOnLoader.theInjectPusher.TriggerForwardMotion();

	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_BUTTON_PUSHER_LIMIT);
	pWnd->EnableWindow(TRUE);
}

void CCuEliteSteppperPage::OnLbnSelchangeListStepperMotor()
{	

	CString szString;
	CWnd* pWnd;
     /* v2.18.5.w4 delete {
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MOTOR);
    m_nStepMotorId = pComboBox->GetCurSel(); 
	////v2.18.3.S1 delete
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_STAND_BY);
     }*/
	UpdateData(true);

	switch (m_nStepMotorId ) {
		case ID_ONLOAD_Z_AXIS_MOTOR:  //elevator
		case ID_OFFLOAD_Z_AXIS_MOTOR:  //elevator
			pWnd = GetDlgItem(IDC_BUTTON_STAND_BY);
			theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS , szString);
		    pWnd->SetWindowTextW(szString);
			break;
		case ID_ONLOAD_X_AXIS_MOTOR:    //side clamp
		case ID_ONLOAD_YTWO_AXIS_MOTOR: //front clamp
		case ID_ONLOAD_YONE_AXIS_MOTOR: //rear clamp
		case ID_OFFLOAD_X_AXIS_MOTOR:    //side clamp
		case ID_OFFLOAD_YTWO_AXIS_MOTOR: //front clamp
		case ID_OFFLOAD_YONE_AXIS_MOTOR: //rear clamp
			pWnd = GetDlgItem(IDC_BUTTON_STAND_BY);
			theMsgMgr.GetMsgString( MSG_MENU_MAGCLAMP_POSITION, szString);
			pWnd->SetWindowTextW(szString);
			break;
		default:
			break;
	}


}


void CCuEliteSteppperPage::OnBnClickedButtonForward()
{

	// TODO: Add your control notification handler code here
	//v2.18.3.S1 add
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
    pWnd->EnableWindow(FALSE);


       UpdateData(true);
	   long lExpectedPos;

     //v2.18.5.w3 add{
       if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YTWO;
       }   

       else if( m_nStepMotorId == ID_OFFLOAD_X_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
       }   
       else if( m_nStepMotorId == ID_ONLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_ONLOAD_ELEVATOR;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_OFFLOAD_ELEVATOR;
       } 

    switch(m_bStepperAddr)
    {
		case CUELITE_ONLOAD_ELEVATOR:
        case CUELITE_ONLOAD_XAXIS:
        case CUELITE_ONLOAD_YONE:
        case CUELITE_ONLOAD_YTWO:
		    theCuEliteTrack.sUIToLogic(m_bStepperAddr, m_lScale,  & lExpectedPos);
            theCuEliteTrack.moveMotor(m_bStepperAddr,lExpectedPos,0);
            break;

        case CUELITE_OFFLOAD_ELEVATOR:
        case CUELITE_OFFLOAD_XAXIS:
        case CUELITE_OFFLOAD_YONE:
        case CUELITE_OFFLOAD_YTWO:
		    theCuEliteTrack.sUIToLogic(m_bStepperAddr, m_lScale,  & lExpectedPos);
            theCuEliteTrack.moveMotor(m_bStepperAddr,lExpectedPos,0);
            break;
    }

       //}


	 /*v2.18.5.w1 delete{
    if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
    {
       if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
        {
            m_bStepperAddr = CUELITE_ONLOAD_YONE;
        }
        else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
        {
            m_bStepperAddr = CUELITE_ONLOAD_YTWO;
        }   
    }
    else
    {
        if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
       }   
	}
    if( m_bStepperAddr == CUELITE_ONLOAD_YONE ||  m_bStepperAddr == CUELITE_ONLOAD_YTWO)
    {
        theCuEliteOnLoader.moveYAxisStepMotor(m_bStepperAddr, -m_lScale, 0);

    }
    else if(m_bStepperAddr == CUELITE_OFFLOAD_YONE ||  m_bStepperAddr == CUELITE_OFFLOAD_YTWO)
    {
         theCuEliteOffLoader.moveYAxisStepMotor(m_bStepperAddr, m_lScale, 0);
    }
	//v2.18.3.S1 add
    else if(m_bStepperAddr == CUELITE_FRONT_RAIL || m_bStepperAddr == CUELITE_REAR_RAIL )
    {
        theCuEliteTrack.moveLFRailStepMotor(m_bStepperAddr, m_lScale, 0);
    }
    else 
	    theStepperControl.MoveMotorSteps(m_bStepperAddr,round((double)m_lScale/theStepperControl.GetStepResolution(m_bStepperAddr)),0);

    }*/
}

void CCuEliteSteppperPage::OnBnClickedButtonBackward()
{

	// TODO: Add your control notification handler code here
	//v2.18.3.S1 add
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
    pWnd->EnableWindow(FALSE);

    long lExpectedPos;
        
	   UpdateData(true);

     //v2.18.5.w3 add{
       if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YTWO;
       }   

       else if( m_nStepMotorId == ID_OFFLOAD_X_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
       }   
       else if( m_nStepMotorId == ID_ONLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_ONLOAD_ELEVATOR;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_OFFLOAD_ELEVATOR;
       } 


    switch(m_bStepperAddr)
    {
		case CUELITE_ONLOAD_ELEVATOR:
        case CUELITE_ONLOAD_XAXIS:
        case CUELITE_ONLOAD_YONE:
        case CUELITE_ONLOAD_YTWO:
		    theCuEliteTrack.sUIToLogic(m_bStepperAddr, m_lScale,  & lExpectedPos);
            theCuEliteTrack.moveMotor(m_bStepperAddr,-lExpectedPos,0);
            break;

        case CUELITE_OFFLOAD_ELEVATOR:
        case CUELITE_OFFLOAD_XAXIS:
        case CUELITE_OFFLOAD_YONE:
        case CUELITE_OFFLOAD_YTWO:
		    theCuEliteTrack.sUIToLogic(m_bStepperAddr, m_lScale,  & lExpectedPos);
            theCuEliteTrack.moveMotor(m_bStepperAddr,-lExpectedPos,0);
            break;
    }

       //}



     /* v2.18.5.w1 delete (

    if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
    {
       if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
        {
            m_bStepperAddr = CUELITE_ONLOAD_YONE;
        }
        else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
        {
            m_bStepperAddr = CUELITE_ONLOAD_YTWO;
        }   
    }
    else
    {
        if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
       }   
	}

    if( m_bStepperAddr == CUELITE_ONLOAD_YONE ||  m_bStepperAddr == CUELITE_ONLOAD_YTWO)
    {
        theCuEliteOnLoader.moveYAxisStepMotor(m_bStepperAddr, m_lScale, 0);

    }
    else if(m_bStepperAddr == CUELITE_OFFLOAD_YONE ||  m_bStepperAddr == CUELITE_OFFLOAD_YTWO)
    {
         theCuEliteOffLoader.moveYAxisStepMotor(m_bStepperAddr, -m_lScale, 0);
    }
	//v2.18.3.S1 add
    else if(m_bStepperAddr == CUELITE_FRONT_RAIL || m_bStepperAddr == CUELITE_REAR_RAIL )
    {
        theCuEliteTrack.moveLFRailStepMotor(m_bStepperAddr, -m_lScale, 0);
    }
    else 
	    theStepperControl.MoveMotorSteps(m_bStepperAddr,-round((double)m_lScale/theStepperControl.GetStepResolution(m_bStepperAddr)),0);
    } */
}

void CCuEliteSteppperPage::OnBnClickedButtonHomeStep()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_PUSHER_HOME);
    pWnd->EnableWindow(FALSE);

	/* v2.18.15.w3 delete {
    if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
    {
       if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
        {
            m_bStepperAddr = CUELITE_ONLOAD_YONE;
        }
        else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
        {
            m_bStepperAddr = CUELITE_ONLOAD_YTWO;
        }   
    }
    else
    {
        if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
       }   

    }
	}*/
     //v2.18.5.w3 add{

		UpdateData(true);

       if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YTWO;
       }   

       else if( m_nStepMotorId == ID_OFFLOAD_X_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
       }   
       else if( m_nStepMotorId == ID_ONLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_ONLOAD_ELEVATOR;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_OFFLOAD_ELEVATOR;
       } 
       //}


    switch(m_bStepperAddr)
    {
        case CUELITE_ONLOAD_XAXIS:
             theCuEliteOnLoader.SearchOnloadXAxisStepHome();
	        Sleep(10);
	        theCuEliteOnLoader.WaitOnloadXAxisStepHomeFinish();
            break;
       case CUELITE_ONLOAD_YONE:
	        theCuEliteOnLoader.SearchOnloadYOneAxisStepHome();
	        Sleep(10);
	        theCuEliteOnLoader.WaitOnloadYOneAxisStepHomeFinish();
            break;
        case CUELITE_ONLOAD_YTWO:
	        theCuEliteOnLoader.SearchOnloadYTwoAxisStepHome();
	        Sleep(10);
	        theCuEliteOnLoader.WaitOnloadYTwoAxisStepHomeFinish();
            break;
        case CUELITE_OFFLOAD_XAXIS:
	        theCuEliteOffLoader.SearchOffloadXAxisStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitOffloadXAxisStepHomeFinish();
            break;
        case CUELITE_OFFLOAD_YONE:
	        theCuEliteOffLoader.SearchOffloadYOneAxisStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitOffloadYOneAxisStepHomeFinish();
            break;
        case CUELITE_OFFLOAD_YTWO:
	        theCuEliteOffLoader.SearchOffloadYTwoAxisStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitOffloadYTwoAxisStepHomeFinish();
            break;
        case CUELITE_ONLOAD_ELEVATOR:
	        theCuEliteOnLoader.SearchOnloadElvStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitStepHomeFinish();
            break;
        case CUELITE_OFFLOAD_ELEVATOR:
	        theCuEliteOffLoader.SearchOffloadElvStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitStepHomeFinish();
            break;



    }

	pWnd->EnableWindow(TRUE);

}
 void CCuEliteSteppperPage::OnBnClickedButtonHolderStandBy()
{
    // TODO: Add your control notification handler code here
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
		
    	theCuEliteTrack.MoveToLogicZero();
		 Sleep(200);
		theCuEliteOnLoader.MoveTotheSlot(1);
		theCuEliteOnLoader.SetCurrentSlotNum(0);
		m_nCurrentSlot = theCuEliteOnLoader.GetCurrentSlotNum();
		
	}
	else
	{
		theCuEliteOffLoader.MoveTotheSlot(1);
		theCuEliteOffLoader.SetCurrentSlotNum(1);
		m_nCurrentSlot = theCuEliteOffLoader.GetCurrentSlotNum();
	
	}

	UpdateData(false);

}

/*v2.18.5.w3 delete {
void CCuEliteSteppperPage::OnBnClickedButtonStandby()
{
	// TODO: Add your control notification handler code here
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
		
    	theCuEliteTrack.MoveToLogicZero();
		 Sleep(200);
		theCuEliteOnLoader.MoveTotheSlot(1);
		theCuEliteOnLoader.SetCurrentSlotNum(0);
		m_nCurrentSlot = theCuEliteOnLoader.GetCurrentSlotNum();
		
	}
	else
	{
		theCuEliteOffLoader.MoveTotheSlot(1);
		theCuEliteOffLoader.SetCurrentSlotNum(1);
		m_nCurrentSlot = theCuEliteOffLoader.GetCurrentSlotNum();
	
	}

	UpdateData(false);


}
}*/





//v2.18.3.S1 add
void CCuEliteSteppperPage::OnBnClickedButtonMagclampForward()
{

	
	if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
	    theCuEliteOnLoader.moveMagClamperStepForward();
    }
    else    
    {
         theCuEliteOffLoader.moveMagClamperStepForward();
    }

}

//v2.18.5.w3 add{

void CCuEliteSteppperPage::OnEnSetfocusCurrentDepth()
{

	//TODO: Add your control notification handler code here
        CUELITE_ONLOAD_RCD  stOnloadData;		
        theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

        CUELITE_OFFLOAD_RCD  stOffloadData;		
        theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);	
        
	    UpdateData(true);

		//don't use IDC_STATIC_CURRENT_DEPTH, very important 
	    CWnd* pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);
        pWnd->SetFocus();

		long lValue;

		if(!theKeyPad.GetUserKeyInData(&m_lCurrentMagClampDepth, 300, 0,3000)){ ;//um    
     
 

			if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)

                theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YTWO, m_lCurrentMagClampDepth ,  & stOnloadData.lOnLoadMagClamperDepth);

			else
                theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YTWO, m_lCurrentMagClampDepth ,  & stOffloadData.lOffLoadMagClamperDepth);   
  
            theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
            theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

			UpdateData(false); 
		}

   
}

void CCuEliteSteppperPage::OnEnSetfocusEditScale()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_HOME_STEP);    
	pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lScale);

    UpdateData(false);

}

//}

//v2.18.3.S1 add
void CCuEliteSteppperPage::OnBnClickedButtonMagClampBackward()
{

    if(m_nElevatorId == ID_ONLOAD_ELEVATOR_MOTOR)
	{
	    theCuEliteOnLoader.moveMagClamperStepBackward();
    }
    else    
    {
        theCuEliteOffLoader.moveMagClamperStepBackward();
    }
}




void CCuEliteSteppperPage::OnBnClickedButtonStandBy()
{

	// TODO: Add your control notification handler code here
    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

     //v2.18.5.w3 add{

		UpdateData(true);
       if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
       {
            m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_ONLOAD_YTWO;
       }   

       else if( m_nStepMotorId == ID_OFFLOAD_X_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YONE_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YONE;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_YTWO_AXIS_MOTOR)
       {
           m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
       }   
       else if( m_nStepMotorId == ID_ONLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_ONLOAD_ELEVATOR;
       }
       else if( m_nStepMotorId == ID_OFFLOAD_Z_AXIS_MOTOR)
       {
           m_bStepperAddr =  CUELITE_OFFLOAD_ELEVATOR;
       } 

    switch(m_bStepperAddr)
    {
		case CUELITE_ONLOAD_ELEVATOR:
			 theCuEliteTrack.MoveToLogicZero();
		     Sleep(200);
			 theCuEliteOnLoader.MoveTotheSlot(1);
			 theCuEliteOnLoader.SetCurrentSlotNum(0);
			 m_nCurrentSlot = theCuEliteOnLoader.GetCurrentSlotNum();
             break;
        case CUELITE_ONLOAD_XAXIS:

			 theCuEliteOnLoader.moveXaxisToStandbyPos();
			 break;
        case CUELITE_ONLOAD_YONE:

			 theCuEliteOnLoader.moveYOneAxisToStandbyPos();
			 break;
        case CUELITE_ONLOAD_YTWO:

			 theCuEliteOnLoader.moveYTwoAxisToStandbyPos();
			 break;
        case CUELITE_OFFLOAD_ELEVATOR:

			 theCuEliteOffLoader.MoveTotheSlot(1);
	    	 theCuEliteOffLoader.SetCurrentSlotNum(1);
		     m_nCurrentSlot = theCuEliteOffLoader.GetCurrentSlotNum();
             break;
        case CUELITE_OFFLOAD_XAXIS:

			 theCuEliteOffLoader.moveXaxisToStandbyPos();
			 break;
        case CUELITE_OFFLOAD_YONE:

			 theCuEliteOffLoader.moveYOneAxisToStandbyPos();
			 break;
        case CUELITE_OFFLOAD_YTWO:

			 theCuEliteOffLoader.moveYTwoAxisToStandbyPos();
			 break;
    }

       //}


    
/*v2.18.5.w1 delete {
    if(m_nElevatorId ==  ID_ONLOAD_ELEVATOR_MOTOR)
    {
        if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
        {
           theCuEliteOnLoader.adjustMagzineHolderLength(); //v2.18.2.S9
	       //add Onload function
        }
        else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
        {
             //add Onload function
            theCuEliteOnLoader.OnLoadMoveYOneMotortoMagPos(); //v2.18.2.S9
        }
        else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
        {
	         //add Onload function
              theCuEliteOnLoader.OnLoadMoveYTwoMotortoMagPos(); //v2.18.2.S9
        }
    }
    else
    {
        if( m_nStepMotorId == ID_ONLOAD_X_AXIS_MOTOR)
        {
	        //add Offload function
            theCuEliteOffLoader.adjustMagzineHolderLength(); //v2.18.2.S9
        }
        else if( m_nStepMotorId == ID_ONLOAD_YONE_AXIS_MOTOR)
        {
             //add Offload function
             theCuEliteOffLoader.OffLoadMoveYOneMotortoMagPos(); //v2.18.2.S9
        }
        else if( m_nStepMotorId == ID_ONLOAD_YTWO_AXIS_MOTOR)
        {
	        //add Offload function
             theCuEliteOffLoader.OffLoadMoveYTwoMotortoMagPos(); //v2.18.2.S9
        }
    }
	}*/
    
}
