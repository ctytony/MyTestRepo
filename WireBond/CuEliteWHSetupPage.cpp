// TopTestingPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MachineData.h"	//v2.18.6.T20 add
#include "DeviceDataMgr.h"	//v2.18.6.T20 add
#include "CuEliteWHSetupPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"//v2.20.Q1 edit
#include "MachineConfig.h"
#include "PopUpCuEliteStepperCtr.h"
#include "CuEliteWorkHolderSetupPage.h"
#include "WireBondDlg.h"
#include "CommFunc.h"
#include "ScanKeyThread.h"


// CCuEliteWHSetupPage dialog

IMPLEMENT_DYNAMIC(CCuEliteWHSetupPage, CPropertyPage)

CCuEliteWHSetupPage::CCuEliteWHSetupPage()
	: CPropertyPage(CCuEliteWHSetupPage::IDD)
	, m_lIndexLogicZero(0)
	, m_lIndexRightLimit(0)
	, m_dWhTbX(0)
	, m_dWhTbY(0)
    , m_iElevatorId(0)
    , m_lMagClamperLimit(0)
    , m_nLeadFrameTrackType(0)
{

}

CCuEliteWHSetupPage::~CCuEliteWHSetupPage()
{
}

void CCuEliteWHSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INDEX_LOGIC_ZERO, m_lIndexLogicZero);
	DDX_Text(pDX, IDC_EDIT_INDEX_RIGHT_LIMIT, m_lIndexRightLimit);
	DDX_Text(pDX, IDC_EDIT_WH_TB_X, m_dWhTbX);
	DDX_Text(pDX, IDC_EDIT_WH_TB_Y, m_dWhTbY);
	/*v2.18.5.w4 {
    DDX_CBIndex(pDX, IDC_COMBO_MAGCLAMPER_ID, m_iElevatorId);
    DDX_Text(pDX, IDC_EDIT_MAGCLAMPER_LIMIT, m_lMagClamperLimit);
    DDX_CBIndex(pDX, IDC_COMBO_LF_TRACK_TYPE, m_nLeadFrameTrackType);
	}*/

}


BEGIN_MESSAGE_MAP(CCuEliteWHSetupPage, CPropertyPage)
    ON_WM_CTLCOLOR()
	//ON_BN_CLICKED(IDOK, &CCuEliteWHSetupPage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_START, &CCuEliteWHSetupPage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_START_TCH_TABLE, &CCuEliteWHSetupPage::OnBnClickedButtonStartTchTable)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CCuEliteWHSetupPage::OnBnClickedButtonNext)

	/*v2.18.5.w4 delete {
    ON_BN_CLICKED(IDC_BUTTON_MAGCLAMPER_EDIT, &CCuEliteWHSetupPage::OnBnClickedButtonClampperEdit)
    //ON_BN_CLICKED(IDC_BUTTON_MAGCLAMPER_DONE, &CCuEliteWHSetupPage::OnBnClickedButtonClampperDone)
    ON_CBN_SELCHANGE(IDC_COMBO_MAGCLAMPER_ID, &CCuEliteWHSetupPage::OnCbnSelchangeComboElevatorSide)
    ON_EN_SETFOCUS(IDC_EDIT_MAGCLAMPER_LIMIT, &CCuEliteWHSetupPage::OnEnSetfocusEditMagClamperLimit)
    ON_CBN_SELCHANGE(IDC_COMBO_LF_TRACK_TYPE, &CCuEliteWHSetupPage::OnCbnSelchangeComboLeadFrameType)
    }*/

	ON_EN_SETFOCUS(IDC_EDIT_INDEX_RIGHT_LIMIT, &CCuEliteWHSetupPage::OnEnSetfocusEditIndexRightLimit)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_LOGIC_ZERO, &CCuEliteWHSetupPage::OnEnSetfocusEditIndexLogicZero)
	ON_BN_CLICKED(IDC_BUTTON_INDEX_HOME, &CCuEliteWHSetupPage::OnBnClickedButtonIndexHome)
	ON_BN_CLICKED(IDC_BUTTON_INDEX_TEACH, &CCuEliteWHSetupPage::OnBnClickedButtonIndexTeach)
END_MESSAGE_MAP()


// CCuEliteWHSetupPage message handlers

BOOL CCuEliteWHSetupPage::OnInitDialog()
{
    CCuEliteWorkHolderSetupSheet* pPage  = (CCuEliteWorkHolderSetupSheet*)GetParent();

    CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        pChildWnd->SetFont(theApp.m_pCurrentFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

    CString szString;

  // CWnd* pWnd = GetDlgItem(IDOK);
 ////   pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_DONE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	//theMsgMgr.GetMsgString(MSG_DIALOG_BUTTON_DONE, szString);
 //   pWnd->SetWindowTextW(szString);



	CWnd* pWnd = GetDlgItem(IDC_BUTTON_INDEX_HOME);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_BUTTON_INDEX_TEACH);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TEACH, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_BUTTON_START);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_BUTTON_START_TCH_TABLE);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_START, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_NEXT, szString);
	pWnd->SetWindowTextW(szString);
    pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_STATIC_INDEXER_ALIGN);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_INDEXER_ALIGN, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_STATIC_INDEXER_LOGIC_0);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_LOGIC_ZERO_TO_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_STATIC_INDEXER_RIGHT_LIMIT);
   	theMsgMgr.GetMsgString(MSG_MENU_ITEM_LOGIC_RIGHT_LIMIT_TO_HOME, szString);
	pWnd->SetWindowTextW(szString);


	pWnd = GetDlgItem(IDC_STATIC_TABLE_SAFE_POS);
   	theMsgMgr.GetMsgString(MSG_MENU_INFO_TCH_SAFE_POS_LABLE, szString);
	pWnd->SetWindowTextW(szString);

	pWnd = GetDlgItem(IDC_STATIC_TABLE_X);	
	pWnd->SetWindowTextW(_T("X : "));

	pWnd = GetDlgItem(IDC_STATIC_TABLE_Y);
	pWnd->SetWindowTextW(_T("Y : "));

	//display help message
	pWnd = GetDlgItem(IDC_STATIC_INFO_SAFE);
	pWnd->SetWindowTextW(_T(""));

    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCuEliteWHSetupPage::OnSetActive( )
{
  CString sPath,szString;
  CWnd* pWnd;

  if(CPropertyPage::OnSetActive()) {
	MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

	m_lIndexLogicZero   = stMcData.lZeroOffset; //um
	m_lIndexRightLimit  = stMcData.lIndexerLimit; //um

	m_dWhTbX =stMcData.stTbSafeStandby.dx;
	m_dWhTbY =stMcData.stTbSafeStandby.dy;

    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

    m_sID=stCuEliteDevicePara.cDeviceID;

     //doing offset  
	if( m_sID.CompareNoCase( ID_CUELITE)==0)
	{
		pWnd = GetDlgItem(IDC_EDIT_INDEX_RIGHT_LIMIT);
		pWnd->ShowWindow(SW_HIDE);
        /*w.temp
        pWnd = GetDlgItem(IDC_EDIT_WH_TB_X);
		pWnd->ShowWindow(SW_HIDE);
        pWnd = GetDlgItem(IDC_EDIT_WH_TB_Y);
	    pWnd->ShowWindow(SW_HIDE);
        */
        pWnd = GetDlgItem(IDC_BUTTON_START_TCH_TABLE);
		pWnd->ShowWindow(SW_HIDE);
        pWnd = GetDlgItem(IDC_BUTTON_NEXT);
		pWnd->ShowWindow(SW_HIDE);


		pWnd = GetDlgItem(IDC_BUTTON_START);
   		theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
		pWnd->SetWindowTextW(szString);

		pWnd = GetDlgItem(IDC_BUTTON_INDEX_HOME);
		pWnd->ShowWindow(SW_NORMAL);

		pWnd = GetDlgItem(IDC_BUTTON_INDEX_TEACH);
		pWnd->ShowWindow(SW_NORMAL);



	}
	else
	{
		pWnd = GetDlgItem(IDC_BUTTON_START);
   		theMsgMgr.GetMsgString(MSG_MENU_ITEM_START, szString);
		pWnd->SetWindowTextW(szString);

		pWnd = GetDlgItem(IDC_BUTTON_INDEX_HOME);
		pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_BUTTON_INDEX_TEACH);
		pWnd->ShowWindow(SW_HIDE);

	}



	UpdateData(false);

  }
	return true;
}

BOOL CCuEliteWHSetupPage::OnKillActive()
{

	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);
	//must updat to theCuEliteTrack, once changed
	theCuEliteTrack.RefreshMeasurementData(&stCuEliteDevicePara);

	return ( CPropertyPage::OnKillActive());

}




HBRUSH CCuEliteWHSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CCuEliteWHSetupPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	


	CPropertyPage::OnOK();
}
//void CCuEliteWHSetupPage::OnBnClickedOk()
//{
//	// TODO: Add your control notification handler code here
//
//	MACHINE_DATA stMcData;
//    theMachineData.GetMachineDataRcd(&stMcData);
//
//
//
//    stMcData.lZeroOffset = m_lIndexLogicZero;
//	stMcData.lIndexerLimit = m_lIndexRightLimit;
//
//	stMcData.stTbSafeStandby.dx = m_dWhTbX;
//	stMcData.stTbSafeStandby.dy = m_dWhTbY;
//
//	theMachineData.StoreMachineDataRcd(&stMcData);
//
//	theLog.WriteLine(_T("User Calibrate the Track logical Zero Position!"));
//
//
//	((CWireBondDlg*)(this->GetParent()))->EnableSystemButton();	
//
//	DestroyWindow();
//}
void CCuEliteWHSetupPage::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	long lPos;
	BOOL bHome=0, bPLimit=0, bNLimit=0;
	int iBusy;



      //doing offset
	if( m_sID.CompareNoCase( ID_CUELITE)==0)
	{
        theCuEliteTrack.MoveToLogicZero();
	}
	else
	{
		CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
		pWnd->EnableWindow(0);
	
		theCuEliteTrack.SearchIndexStepHome();

		Sleep(100);

		theCuEliteTrack.WaitStepHomeFinish();

		Sleep(100);

		CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_DEVICE_INDEXER);
		theStepperCtrler.DoModal();


		Sleep(100);

		theStepperControl.CheckStepMotionStatus(CUELITE_DEVICE_INDEXER,&lPos);

		m_lIndexLogicZero =round((double)lPos *theStepperControl.GetStepResolution(CUELITE_DEVICE_INDEXER));
		UpdateData(false);

 
		theCuEliteTrack.ChangeStepperMotionProfile(SRCH_SPEED_PROFILE);

		Sleep(100);

		theStepperControl.MoveMotorSteps(CUELITE_DEVICE_INDEXER, 1000000,0);
			do
			{

				theStepperControl.ReadMotorStopReason(CUELITE_DEVICE_INDEXER,&bHome,&bNLimit,&bPLimit);

			}
			while(bPLimit == FALSE);
	
	
			theStepperControl.MoveMotorSteps(CUELITE_DEVICE_INDEXER,-5000,0);

			do
			{
				iBusy = theStepperControl.CheckStepBusyState(CUELITE_DEVICE_INDEXER);

			}
			while(iBusy ==1);

			theStepperControl.CheckStepMotionStatus(CUELITE_DEVICE_INDEXER,&lPos);

			m_lIndexRightLimit =round((double) lPos * theStepperControl.GetStepResolution(CUELITE_DEVICE_INDEXER));
			UpdateData(false);

			Sleep(100);

			MACHINE_DATA stMcData;
			theMachineData.GetMachineDataRcd(&stMcData);
			stMcData.lZeroOffset = m_lIndexLogicZero;//length from physical home sensor
			stMcData.lIndexerLimit = m_lIndexRightLimit;//length from physical home sensor
			theMachineData.StoreMachineDataRcd(&stMcData);
			pWnd = GetDlgItem(IDC_BUTTON_START);
			pWnd->EnableWindow(1);

	}


}

void CCuEliteWHSetupPage::OnBnClickedButtonStartTchTable()
{
	// TODO: Add your control notification handler code here

	 CString szString;

	 MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);


	 CWnd* pWnd = GetDlgItem(IDC_BUTTON_START_TCH_TABLE);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	 pWnd->EnableWindow(TRUE);

	AFP_OPTION stAfpOptions;
    theApp.GetAfpOptions(&stAfpOptions);

	if (theApp.IsOnline()||
		   stAfpOptions.bServoEnable == true) //v2.18.5.w4 add
    {
	    int ret = theServoController.SynchroMoveTable(&(stMcData.stTbSafeStandby), MOTION_ABSOLUTE, MOTION_WITH_WAIT);
        if(ret == 1) 		//v2.18.1.T2 add
        {
            short msgType = 0, msgNum =0, msgIndex = 0;
            bool bStopValid = false, bSkipVlaid = false;
            theServoController.GetMsgMgrShowMessage(msgType, msgNum, &msgIndex, bStopValid, bSkipVlaid);
            theMsgMgr.ShowMessage(msgType, msgNum, &msgIndex);
        }

	}

	Sleep(100);

	 pWnd = GetDlgItem(IDC_STATIC_INFO_SAFE);
	 theMsgMgr.GetMsgString(MSG_MENU_INFO_TCH_SAFE_POS, szString);
	 pWnd->SetWindowTextW(szString);
	
	
}

void CCuEliteWHSetupPage::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here

	CString szString;

	TBL_POS stSafeTblPos;
	MACHINE_DATA stMcData;
	theMachineData.GetMachineDataRcd(&stMcData);


	CWnd*pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->EnableWindow(FALSE);


	AFP_OPTION stAfpOptions;
    theApp.GetAfpOptions(&stAfpOptions);

	if (theApp.IsOnline() ||
             (stAfpOptions.bServoEnable == true)) //v2.18.5.w4 add
    {
          theServoController.GetTablePosition(&stSafeTblPos);

		  m_dWhTbX = stSafeTblPos.dx;
		  m_dWhTbY = stSafeTblPos.dy;

	}
	stMcData.stTbSafeStandby.dx = m_dWhTbX;
	stMcData.stTbSafeStandby.dy = m_dWhTbY;

	theMachineData.StoreMachineDataRcd(&stMcData);

	pWnd = GetDlgItem(IDC_STATIC_INFO_SAFE);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FINISH, szString);
	pWnd->SetWindowTextW(szString);


	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->EnableWindow(SW_NORMAL);

    pWnd = GetDlgItem(IDC_BUTTON_START_TCH_TABLE);
    pWnd->EnableWindow(SW_NORMAL);
	//MSG_TBL_POS_OUT_OF_LIMIT

	UpdateData(false);
}

void CCuEliteWHSetupPage::OnBnClickedButtonIndexHome()
{
	// TODO: Add your control notification handler code here
	theCuEliteTrack.SearchIndexStepHome();
	Sleep(10);
	theCuEliteTrack.WaitStepHomeFinish();

}

void CCuEliteWHSetupPage::OnBnClickedButtonIndexTeach()
{
	// TODO: Add your control notification handler code here


	MACHINE_DATA stMcData;
	theMachineData.GetMachineDataRcd(&stMcData);

	long lPos;
	long lPosPulse;


	CPopUpCuEliteStepperCtr stepDlg(CUELITE_DEVICE_INDEXER);
	if( stepDlg.DoModal()==IDOK){

				theStepperControl.CheckStepMotionStatus(CUELITE_DEVICE_INDEXER, &lPosPulse);
	            theCuEliteTrack.xPhysicToLogicPos(CUELITE_DEVICE_INDEXER, lPosPulse,  &lPos);
				stMcData.lZeroOffset =  lPos ; 

                theMachineData.StoreMachineDataRcd(&stMcData);

	}//if


	theCuEliteTrack.sLogicToUI(CUELITE_DEVICE_INDEXER,stMcData.lZeroOffset, &m_lIndexLogicZero);	

 	UpdateData(false);




}




void CCuEliteWHSetupPage::OnEnSetfocusEditIndexLogicZero()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_SETUP_DUMMY);
	pWnd->SetFocus();


    long  lExpectedDist,lExpected;

	MACHINE_DATA stMcData;
	theMachineData.GetMachineDataRcd(&stMcData);

     //doing offset  
	if( m_sID.CompareNoCase( ID_CUELITE)==0)
	{
		if(!theKeyPad.GetUserKeyInData(&m_lIndexLogicZero, 64000, 0,CUELITE_DEVICE_INDEXER_MAX )){//um
  
			theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,m_lIndexLogicZero,&stMcData.lZeroOffset);
		    theMachineData.StoreMachineDataRcd(&stMcData);
		}


	}
	else
	{
		if(!theKeyPad.GetUserKeyInData(&m_lIndexLogicZero, 64000, 0,CUELITE_DEVICE_INDEXER_MAX )){//um
  
			lExpected=m_lIndexLogicZero;
			theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,lExpected,&lExpectedDist);

			if( !  theCuEliteTrack.isOutOfSoftLimit(CUELITE_DEVICE_INDEXER,lExpectedDist,1))
			{

				theCuEliteTrack.SearchIndexStepHome();
				Sleep(10);
				theCuEliteTrack.WaitStepHomeFinish();
				Sleep(10);
				theCuEliteTrack.moveMotor(CUELITE_DEVICE_INDEXER,lExpectedDist,1);
				Sleep(10);
				theCuEliteTrack.waitMotor(CUELITE_DEVICE_INDEXER,TIMEOUT_3);

			    theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,m_lIndexLogicZero,&stMcData.lZeroOffset);
		        theMachineData.StoreMachineDataRcd(&stMcData);

			}
		}

	}
  
	UpdateData(false);


}



void CCuEliteWHSetupPage::OnEnSetfocusEditIndexRightLimit()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_SETUP_DUMMY);
	pWnd->SetFocus();

   long  lExpectedDist, lExpected;

	MACHINE_DATA stMcData;
	theMachineData.GetMachineDataRcd(&stMcData);

	if(!theKeyPad.GetUserKeyInData(&m_lIndexRightLimit, 125000, 100000,CUELITE_DEVICE_INDEXER_MAX )){//um

		lExpected=m_lIndexRightLimit ;
		theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,lExpected,&lExpectedDist);

		if( !  theCuEliteTrack.isOutOfSoftLimit(CUELITE_DEVICE_INDEXER,lExpectedDist,1))
		{

			theCuEliteTrack.SearchIndexStepHome();
			Sleep(10);
			theCuEliteTrack.WaitStepHomeFinish();
			Sleep(10);
            theCuEliteTrack.moveMotor(CUELITE_DEVICE_INDEXER,lExpectedDist,1);
			Sleep(10);
            theCuEliteTrack.waitMotor(CUELITE_DEVICE_INDEXER,TIMEOUT_3);

			theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,m_lIndexRightLimit,&stMcData.lIndexerLimit);
		    theMachineData.StoreMachineDataRcd(&stMcData);

		}
	}

	UpdateData(false);

}




/* v2.18.5.w4 delete {
void CCuEliteWHSetupPage::OnBnClickedButtonClampperEdit()
{
    if(m_iElevatorId == 0)
    {
        CPopUpCuEliteStepperCtr topStepperCtrl(CUELITE_ONLOAD_YONE, CUELITE_SPEED_FINE );
        topStepperCtrl.DoModal();

    }
    else
    {
        CPopUpCuEliteStepperCtr topStepperCtrl(CUELITE_OFFLOAD_YONE, CUELITE_SPEED_FINE);
        topStepperCtrl.DoModal();
    }

}
 void CCuEliteWHSetupPage::OnCbnSelchangeComboElevatorSide()
{
	MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAGCLAMPER_ID);
    m_iElevatorId = pComboBox->GetCurSel();

    if(m_iElevatorId == 0) m_lMagClamperLimit = stMcData.lOnLoadMagClamperLimit;
    else  m_lMagClamperLimit = stMcData.lOffLoadMagClamperLimit;
    UpdateData(false);
}
 void CCuEliteWHSetupPage::OnEnSetfocusEditMagClamperLimit()
{
    CWnd* pWnd = GetDlgItem(IDC_COMBO_MAGCLAMPER_ID);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMagClamperLimit);
    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
	if(m_iElevatorId == 0)
		stMcData.lOnLoadMagClamperLimit = m_lMagClamperLimit;
	else 
		stMcData.lOffLoadMagClamperLimit = m_lMagClamperLimit;
    theMachineData.StoreMachineDataRcd(&stMcData);
    UpdateData(false);

}

void CCuEliteWHSetupPage::OnCbnSelchangeComboLeadFrameType()
{
    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LF_TRACK_TYPE);
    m_nLeadFrameTrackType = pComboBox->GetCurSel();
    stMcData.nLeadFrameTrackType =  m_nLeadFrameTrackType;
    theMachineData.StoreMachineDataRcd(&stMcData);
    if(m_nLeadFrameTrackType == 2)
    {
         CString szString;
         CStringList szStringList;
         theMsgMgr.GetMsgString(MSG_MENU_ITEM_REAR_RAIL_TYPE,szString);
         szStringList.AddTail(szString);
         theMsgMgr.ShowMessage(WARNING_MSG, szStringList);
    }

    UpdateData(false);
 
}

}*/

//BOOL CCuEliteWHSetupPage::OnSetActive()
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	CCuEliteWorkHolderSetupSheet* pPage  = (CCuEliteWorkHolderSetupSheet*)GetParent();
//
//	CWnd* pWnd = pPage->GetDlgItem(IDOK);
//	pWnd->EnableWindow(0);
//
//
//	return CPropertyPage::OnSetActive();
//}



