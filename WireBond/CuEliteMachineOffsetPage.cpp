// CuEliteMachineOffset.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "DeviceDataMgr.h"	//v2.18.6.T20 add
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "CuEliteMachineOffsetPage.h"
#include "CuEliteWorkHolderSetupPage.h"
#include "TeachCuEliteWorkHolderSheet.h"
#include "MachineData.h" 
#include "DataStruct.h"
#include "StepperControl.h"//v2.20.Q1 edit
#include "ScanKeyThread.h"
#include "MachineConfig.h"

// CCuEliteMachineOffsetPage dialog

IMPLEMENT_DYNAMIC(CCuEliteMachineOffsetPage, CPropertyPage)

CCuEliteMachineOffsetPage::CCuEliteMachineOffsetPage()
	: CPropertyPage(CCuEliteMachineOffsetPage::IDD)
{

}

CCuEliteMachineOffsetPage::~CCuEliteMachineOffsetPage()
{
}

void CCuEliteMachineOffsetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ONLOAD_ELV, m_lOnLoadMagazineElevatorOffset);
    DDX_Text(pDX, IDC_EDIT_ONLOAD_F_CLAMP2, m_lOnLoadMagazineYTwoOffset);
    DDX_Text(pDX, IDC_EDIT_ONLOAD_R_CLAMP2, m_lOnLoadMagazineYOneOffset);
    DDX_Text(pDX, IDC_EDIT_ONLOAD_SIDE_CLAMP, m_lOnLoadMagazineLengthOffset);
    DDX_Text(pDX, IDC_EDIT_INDEX_FRONT_RAIL, m_lLFFrontTrackOffset);
    DDX_Text(pDX, IDC_EDIT_INDEX_REAR_RAIL, m_lLFRearTrackOffset);
    DDX_Text(pDX, IDC_EDIT_INDEX_INDEXER, m_lLFIndexerOffset);
    DDX_Text(pDX, IDC_EDIT_INDEX_WIN_CLAMP, m_lLFWindowClamperOffset);
    DDX_Text(pDX, IDC_EDIT_OFFLOAD_ELEVATOR, m_lOffLoadMagazineElevatorOffset);
    DDX_Text(pDX, IDC_EDIT_OFFLOAD_FRONT_CLAMP, m_lOffLoadMagazineYTwoOffset);
    DDX_Text(pDX, IDC_EDIT_OFFLOAD_REAR_CLAMP, m_lOffLoadMagazineYOneOffset);
    DDX_Text(pDX, IDC_EDIT_OFFLOAD_SIDE_CLAMP, m_lOffLoadMagazineLengthOffset);
    DDX_Text(pDX, IDC_EDIT_FILE2, m_sID);

}


BEGIN_MESSAGE_MAP(CCuEliteMachineOffsetPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_ONLOAD_ELV, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadElevator)
//	ON_EN_SETFOCUS(IDC_EDIT_ONLOAD_F_CLAMP, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadFrontClamp)
	ON_EN_SETFOCUS(IDC_EDIT_ONLOAD_R_CLAMP2, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadRearClamp)
	ON_EN_SETFOCUS(IDC_EDIT_ONLOAD_SIDE_CLAMP, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadSideClamp)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_FRONT_RAIL, &CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexFrontRail)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_REAR_RAIL, &CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexRearRail)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_INDEXER, &CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexIndexer)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_WIN_CLAMP, &CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexWinClamp)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOAD_ELEVATOR, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadElevator)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOAD_FRONT_CLAMP, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadFrontClamp)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOAD_REAR_CLAMP, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadRearClamp)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOAD_SIDE_CLAMP, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadSideClamp)
	ON_EN_SETFOCUS(IDC_EDIT_FILE2, &CCuEliteMachineOffsetPage::OnEnSetfocusEditFile2)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATABASE, &CCuEliteMachineOffsetPage::OnBnClickedButtonLoadDatabase)
	ON_EN_SETFOCUS(IDC_EDIT_ONLOAD_F_CLAMP2, &CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadFClamp2)
END_MESSAGE_MAP()


// CCuEliteMachineOffsetPage message handlers


BOOL CCuEliteMachineOffsetPage::OnInitDialog()
{
   CCuEliteWorkHolderSetupSheet* pPage  = (CCuEliteWorkHolderSetupSheet*)GetParent();

    CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        pChildWnd->SetFont(theApp.m_pCurrentFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

	CString szString,sString;


    CWnd* pWnd  = GetDlgItem(IDC_STATIC_ONLOAD);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, szString);
    pWnd->SetWindowTextW(szString);

    pWnd  = GetDlgItem(IDC_STATIC_OFFLOAD);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, szString);
    pWnd->SetWindowTextW(szString);

    pWnd  = GetDlgItem(IDC_STATIC_INDEX);
	theMsgMgr.GetMsgString(MSG_TOOL_BAR_INDEX, szString);
    pWnd->SetWindowTextW(szString);


    pWnd  = GetDlgItem(IDC_STATIC_ONLOAD_ELEVATOR);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
    pWnd->SetWindowTextW(szString);

    pWnd  = GetDlgItem(IDC_STATIC_ONLOAD_FRONT_CLAMP);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
    pWnd->SetWindowTextW(szString);

	pWnd  = GetDlgItem(IDC_STATIC_ONLOAD_REAR_CLAMP);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_REAR_CLAMPER, szString);
    pWnd->SetWindowTextW(szString);

    pWnd  = GetDlgItem(IDC_STATIC_ONLOAD_SIDE_CLAMP);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_SIDE_CLAMPER, szString);
    pWnd->SetWindowTextW(szString);

	pWnd  = GetDlgItem(IDC_STATIC_OFFLOAD_ELEVATOR);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
    pWnd->SetWindowTextW(szString);

	pWnd  = GetDlgItem(IDC_STATIC_OFFLOAD_FRONT_CLAMP);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
    pWnd->SetWindowTextW(szString);

    pWnd  = GetDlgItem(IDC_STATIC_OFFLOAD_REAR_CLAMP);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_REAR_CLAMPER, szString);
    pWnd->SetWindowTextW(szString);

	pWnd  = GetDlgItem(IDC_STATIC_OFFLOAD_SIDE_CLAMP);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_SIDE_CLAMPER, szString);
    pWnd->SetWindowTextW(szString);

	pWnd  = GetDlgItem(IDC_STATIC_INDEX_FRONT_RAIL);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FRONT_RAIL, szString);
    pWnd->SetWindowTextW(szString);

	pWnd  = GetDlgItem(IDC_STATIC_INDEX_REAR_RAIL);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_REAR_RAIL, szString);
    pWnd->SetWindowTextW(szString);

    pWnd  = GetDlgItem(IDC_STATIC_INDEX_WIN_CLAMP);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_LF_CLAMP, szString);
    pWnd->SetWindowTextW(szString);

	pWnd  = GetDlgItem(IDC_STATIC_INDEX_INDEXER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_INDEXER_SETUP_PAGE, szString);
    pWnd->SetWindowTextW(szString);


    CPropertyPage::OnInitDialog();
    // TODO:  Add extra initialization here

    return TRUE;
}

BOOL CCuEliteMachineOffsetPage::OnSetActive( )
{
  CString sPath;
  CWnd* pWnd;

  if(CPropertyPage::OnSetActive()) {


    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

    CUELITE_ONLOAD_RCD  stOnloadData;		
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;		
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);	

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

    CUELITE_STEPPER_CLAMP_PARA  stCuElitePosPara;		
    theCuEliteLFClamp.GetClampRcd(&stCuElitePosPara);		



	sPath=WHS_FILE_PATH;
	sPath=sPath + ID_CUELITE+_T(".ini") ;
	m_refFile.setPathTitleExt(sPath );

    m_sID=stCuEliteDevicePara.cDeviceID;//update UI only

	if( m_sID.CompareNoCase( ID_CUELITE)==0){
		pWnd = GetDlgItem(IDC_EDIT_FILE2);
		pWnd->EnableWindow(SW_NORMAL);


        long lValue;

		    m_refFile.get(_T("motor standby position"),_T("onload elevator"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_ELEVATOR,lValue,&m_lOnLoadMagazineElevatorStandbyRef);
            m_refFile.get(_T("motor standby position"),_T("onload side clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_XAXIS,lValue,&m_lOnLoadMagazineLengthStandbyRef);
		    m_refFile.get(_T("motor standby position"),_T("onload rear clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YONE,lValue,&m_lOnLoadMagazineYOneStandbyRef);
            m_refFile.get(_T("motor standby position"),_T("onload front clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YTWO,lValue,&m_lOnLoadMagazineYTwoStandbyRef);

		    m_refFile.get(_T("motor standby position"),_T("front rail"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_FRONT_RAIL,lValue,&m_lLFFrontTrackStandbyRef);
            m_refFile.get(_T("motor standby position"),_T("window clamper"),&m_lLFWindowClamperStandbyRef);//% base
		   // theCuEliteTrack.sUIToLogic(CUELITE_WINDOW_CLAMPER,lValue,&m_lLFWindowClamperStandbyRef);
		    m_refFile.get(_T("motor standby position"),_T("indexer"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,lValue,&m_lLFIndexerStandbyRef);
            m_refFile.get(_T("motor standby position"),_T("rear rail"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_REAR_RAIL,lValue,&m_lLFRearTrackStandbyRef);
		    m_refFile.get(_T("motor standby position"),_T("offload elevator"),&lValue);

 		    m_refFile.get(_T("motor standby position"),_T("offload elevator"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_ELEVATOR,lValue,&m_lOffLoadMagazineElevatorStandbyRef);
            m_refFile.get(_T("motor standby position"),_T("offload side clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_XAXIS,lValue,&m_lOffLoadMagazineLengthStandbyRef);
		    m_refFile.get(_T("motor standby position"),_T("offload rear clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YONE,lValue,&m_lOffLoadMagazineYOneStandbyRef);
            m_refFile.get(_T("motor standby position"),_T("offload front clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YTWO,lValue,&m_lOffLoadMagazineYTwoStandbyRef);


        //these offset are just for showing only, not for any
		lValue	=stOnloadData.lOnload1stSlotLevel-m_lOnLoadMagazineElevatorStandbyRef;
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_ELEVATOR,lValue, &m_lOnLoadMagazineElevatorOffset);	

		lValue	=stOnloadData.lOnLoadMagazineLengthStandby-m_lOnLoadMagazineLengthStandbyRef; 
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_XAXIS,lValue, &m_lOnLoadMagazineLengthOffset);

		lValue	=stOnloadData.lOnLoadMagazineYOneStandby-m_lOnLoadMagazineYOneStandbyRef; 
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YONE,lValue, &m_lOnLoadMagazineYOneOffset);

		lValue	=stOnloadData.lOnLoadMagazineYTwoStandby-m_lOnLoadMagazineYTwoStandbyRef;  
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,lValue, &m_lOnLoadMagazineYTwoOffset );

		lValue	=stOffloadData.lOffload1stSlotLevel-m_lOffLoadMagazineElevatorStandbyRef;
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_ELEVATOR,lValue, &m_lOffLoadMagazineElevatorOffset);

		lValue	=stOffloadData.lOffLoadMagazineLengthStandby-m_lOffLoadMagazineLengthStandbyRef;
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_XAXIS,lValue, &m_lOffLoadMagazineLengthOffset);

		lValue	=stOffloadData.lOffLoadMagazineYOneStandby-m_lOffLoadMagazineYOneStandbyRef;
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YONE,lValue, &m_lOffLoadMagazineYOneOffset);

		lValue	=stOffloadData.lOffLoadMagazineYTwoStandby-m_lOffLoadMagazineYTwoStandbyRef;
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,lValue, &m_lOffLoadMagazineYTwoOffset);	

		lValue	=stMcData.lZeroOffset-m_lLFIndexerStandbyRef;
	    theCuEliteTrack.sLogicToUI(CUELITE_DEVICE_INDEXER,lValue, &m_lLFIndexerOffset);

		lValue	=stTrackRcd.lLFFrontTrackStandby-m_lLFFrontTrackStandbyRef;  
		theCuEliteTrack.sLogicToUI(CUELITE_FRONT_RAIL,lValue, &m_lLFFrontTrackOffset);

		lValue	=stTrackRcd.lLFRearTrackStandby-m_lLFRearTrackStandbyRef; 
		theCuEliteTrack.sLogicToUI(CUELITE_REAR_RAIL,lValue, &m_lLFRearTrackOffset);

		lValue	=stCuElitePosPara.lWindClampPreClosePos-m_lLFWindowClamperStandbyRef;
	    theCuEliteTrack.sLogicToUI(CUELITE_WINDOW_CLAMPER,lValue, &m_lLFWindowClamperOffset);


	}
	else {
		pWnd = GetDlgItem(IDC_EDIT_FILE2);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_ONLOAD_ELV);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_ONLOAD_F_CLAMP2);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_ONLOAD_R_CLAMP2);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_ONLOAD_SIDE_CLAMP);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_INDEX_FRONT_RAIL);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_INDEX_REAR_RAIL);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_INDEX_INDEXER);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_INDEX_WIN_CLAMP);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_OFFLOAD_ELEVATOR);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_OFFLOAD_FRONT_CLAMP);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_OFFLOAD_REAR_CLAMP);
		pWnd->EnableWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_OFFLOAD_SIDE_CLAMP);
		pWnd->EnableWindow(SW_HIDE);

		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_ELEVATOR,stMcData.lOnLoadMagazineElevatorOffset, &m_lOnLoadMagazineElevatorOffset);	
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_XAXIS,stMcData.lOnLoadMagazineLengthOffset, &m_lOnLoadMagazineLengthOffset);
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YONE,stMcData.lOnLoadMagazineYOneOffset, &m_lOnLoadMagazineYOneOffset);
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stMcData.lOnLoadMagazineYTwoOffset, &m_lOnLoadMagazineYTwoOffset );

		theCuEliteTrack.sLogicToUI(CUELITE_FRONT_RAIL,stMcData.lLFFrontTrackOffset, &m_lLFFrontTrackOffset);
		theCuEliteTrack.sLogicToUI(CUELITE_WINDOW_CLAMPER,stMcData.lLFWindowClamperOffset, &m_lLFWindowClamperOffset);
		theCuEliteTrack.sLogicToUI(CUELITE_DEVICE_INDEXER,stMcData.lLFIndexerOffset, &m_lLFIndexerOffset);	
		theCuEliteTrack.sLogicToUI(CUELITE_REAR_RAIL,stMcData.lLFRearTrackOffset, &m_lLFRearTrackOffset);	

		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_ELEVATOR,stMcData.lOffLoadMagazineElevatorOffset, &m_lOffLoadMagazineElevatorOffset);
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_XAXIS,stMcData.lOffLoadMagazineLengthOffset, &m_lOffLoadMagazineLengthOffset);
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YONE,stMcData.lOffLoadMagazineYOneOffset, &m_lOffLoadMagazineYOneOffset);
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,stMcData.lOffLoadMagazineYTwoOffset, &m_lOffLoadMagazineYTwoOffset);	

	}

	UpdateData(false);

  }
	return true;
}


BOOL CCuEliteMachineOffsetPage::OnKillActive()
{

	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	//save to machine McData.dat also
	theApp.ArchiveMachineData(DATA_MGR_SAVE); 
	//set offset to stepper controller
	if (theMachineConfig.GetMaterialHandlerType() == CU_ELITE_MHS)
		theCuEliteTrack.SetMachineOffset();
	//theMachineData.setMachineOffsetToStepContorller(CU_ELITE_MHS);	//v2.18.6.T26 delete

	return ( CPropertyPage::OnKillActive());

}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadElevator()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOnLoadMagazineElevatorOffset)){

        CUELITE_ONLOAD_RCD  stOnloadData;		
        theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_ELEVATOR,m_lOnLoadMagazineElevatorOffset, &lValue);
        stOnloadData.lOnload1stSlotLevel=m_lOnLoadMagazineElevatorStandbyRef + lValue;    

		theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 

	}

    UpdateData(false);
}
HBRUSH CCuEliteMachineOffsetPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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



void CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadFrontClamp()
{
	// TODO: Add your control notification handler code here

}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadRearClamp()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOnLoadMagazineYOneOffset)){

        CUELITE_ONLOAD_RCD  stOnloadData;		
        theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YONE,m_lOnLoadMagazineYOneOffset, &lValue);
        stOnloadData.lOnLoadMagazineYOneStandby=m_lOnLoadMagazineYOneStandbyRef + lValue;   

		theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 


	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadSideClamp()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOnLoadMagazineLengthOffset)){

        CUELITE_ONLOAD_RCD  stOnloadData;		
        theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_XAXIS,m_lOnLoadMagazineLengthOffset, &lValue);
        stOnloadData.lOnLoadMagazineLengthStandby=m_lOnLoadMagazineLengthStandbyRef + lValue; 

		theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexFrontRail()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lLFFrontTrackOffset)){

        CUELITE_TRACK_DATA stTrackRcd;
        theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

		theCuEliteTrack.sUIToLogic(CUELITE_FRONT_RAIL,m_lLFFrontTrackOffset, &lValue);
        stTrackRcd.lLFFrontTrackStandby=m_lLFFrontTrackStandbyRef + lValue;   

        theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexRearRail()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lLFRearTrackOffset)){

        CUELITE_TRACK_DATA stTrackRcd;
        theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

		theCuEliteTrack.sUIToLogic(CUELITE_REAR_RAIL,m_lLFRearTrackOffset, &lValue);
        stTrackRcd.lLFRearTrackStandby=m_lLFRearTrackStandbyRef + lValue;

        theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexIndexer()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);


	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lLFIndexerOffset)){

        CUELITE_TRACK_DATA stTrackRcd;
        theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

		theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,m_lLFIndexerOffset, &lValue);
        stMcData.lZeroOffset=m_lLFIndexerStandbyRef + lValue;   

        theMachineData.StoreMachineDataRcd(&stMcData);

	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditIndexWinClamp()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lLFWindowClamperOffset)){

        CUELITE_STEPPER_CLAMP_PARA  stCuElitePosPara;
        theCuEliteLFClamp.GetClampRcd(&stCuElitePosPara);

		theCuEliteTrack.sUIToLogic(CUELITE_WINDOW_CLAMPER ,m_lLFWindowClamperOffset, &lValue);
        stCuElitePosPara.lWindClampPreClosePos=m_lLFWindowClamperStandbyRef + lValue; 

        theCuEliteLFClamp.UpdateClampRcd(&stCuElitePosPara);


	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadElevator()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOffLoadMagazineElevatorOffset)){

        CUELITE_OFFLOAD_RCD  stOffloadData;
        theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_ELEVATOR ,m_lOffLoadMagazineElevatorOffset, &lValue);
        stOffloadData.lOffload1stSlotLevel=m_lOffLoadMagazineElevatorStandbyRef + lValue;   

        theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);


	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadFrontClamp()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOffLoadMagazineYTwoOffset)){

        CUELITE_OFFLOAD_RCD  stOffloadData;
        theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YTWO,m_lOffLoadMagazineYTwoOffset, &lValue);
        stOffloadData.lOffLoadMagazineYTwoStandby=m_lOffLoadMagazineYTwoStandbyRef + lValue;   

        theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadRearClamp()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOffLoadMagazineYOneOffset)){

        CUELITE_OFFLOAD_RCD  stOffloadData;
        theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YONE,m_lOffLoadMagazineYOneOffset, &lValue);
        stOffloadData.lOffLoadMagazineYOneStandby=m_lOffLoadMagazineYOneStandbyRef + lValue;   

        theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

	}

    UpdateData(false);
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOffloadSideClamp()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOffLoadMagazineLengthOffset)){

        CUELITE_OFFLOAD_RCD  stOffloadData;
        theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_XAXIS,m_lOffLoadMagazineLengthOffset, &lValue);
        stOffloadData.lOffLoadMagazineLengthStandby=m_lOffLoadMagazineLengthStandbyRef + lValue;

        theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

	}

    UpdateData(false);
}



void CCuEliteMachineOffsetPage::OnEnSetfocusEditFile2()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	  CString str;
      CStringList szStringList;


      theMsgMgr.GetMsgString(MSG_MENU_ITEM_SAVE_MACHINE_OFFSET, str);
	  szStringList.AddTail(str);
      int iRet=theMsgMgr.ShowMessageX(WARNING_MSG, szStringList,0,0,1);
	  if( iRet==SHOW_MSG_RESULT_OK)    
	  {
	      //save to machine ID_CUELITE.ini 
		  saveOffsetToID_CUELITEini();
	      //save to machine McData.dat 
          theApp.ArchiveMachineData(DATA_MGR_SAVE); 
		  //set offset to stepper controller
		  //theMachineData.setMachineOffsetToStepContorller(CU_ELITE_MHS);	//v2.18.6.T26 delete
		  
		  //v2.18.6.T26 add
			if (theMachineConfig.GetMaterialHandlerType() == CU_ELITE_MHS)
				theCuEliteTrack.SetMachineOffset();
	  }
	  else if(iRet==SHOW_MSG_RESULT_STOP) {};

}
void CCuEliteMachineOffsetPage::saveOffsetToID_CUELITEini()
{
	// TODO: Add your control notification handler code here
 
	MACHINE_DATA stMcData;
	theMachineData.GetMachineDataRcd(&stMcData);

	CString sPath;
	long lValue;

        UpdateData(true);

	    sPath=WHS_FILE_PATH ;
		sPath=sPath+ ID_CUELITE+_T(".ini") ;
		m_refFile.setPathTitleExt(sPath );
	
		m_refFile.set(_T("machine offset"),_T("onload elevator"),m_lOnLoadMagazineElevatorOffset);
		m_refFile.set(_T("machine offset"),_T("onload side clamp"),m_lOnLoadMagazineLengthOffset);
		m_refFile.set(_T("machine offset"),_T("onload rear clamp"),m_lOnLoadMagazineYOneOffset);
		m_refFile.set(_T("machine offset"),_T("onload front clamp"),m_lOnLoadMagazineYTwoOffset);
	
		m_refFile.set(_T("machine offset"),_T("offload elevator"),m_lOffLoadMagazineElevatorOffset);
		m_refFile.set(_T("machine offset"),_T("offload side clamp"),m_lOffLoadMagazineLengthOffset);
		m_refFile.set(_T("machine offset"),_T("offload rear clamp"),m_lOffLoadMagazineYOneOffset);	
		m_refFile.set(_T("machine offset"),_T("offload front clamp"),m_lOffLoadMagazineYTwoOffset);


		m_refFile.set(_T("machine offset"),_T("front rail"),m_lLFFrontTrackOffset);
		m_refFile.set(_T("machine offset"),_T("window clamper"),m_lLFWindowClamperOffset);
		m_refFile.set(_T("machine offset"),_T("indexer"),m_lLFIndexerOffset);
		m_refFile.set(_T("machine offset"),_T("rear rail"),m_lLFRearTrackOffset);


}


void CCuEliteMachineOffsetPage::OnBnClickedButtonLoadDatabase()
{
	// TODO: Add your control notification handler code here
}

void CCuEliteMachineOffsetPage::OnEnSetfocusEditOnloadFClamp2()
{

	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

	long lValue;
  
	if(! theKeyPad.GetUserKeyInData(&m_lOnLoadMagazineYTwoOffset)){

        CUELITE_ONLOAD_RCD  stOnloadData;		
        theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

		theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YTWO,m_lOnLoadMagazineYTwoOffset, &lValue);
        stOnloadData.lOnLoadMagazineYTwoStandby=m_lOnLoadMagazineYTwoStandbyRef + lValue;

		theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData);


	}

    UpdateData(false);
}
