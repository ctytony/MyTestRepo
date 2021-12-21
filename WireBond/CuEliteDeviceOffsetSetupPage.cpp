// TopTestingPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MachineData.h"	//v2.18.6.T20 add
#include "DeviceDataMgr.h"	//v2.18.6.T20 add
#include "CuEliteDeviceOffsetSetupPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h" //v2.20.Q1 edit
#include "MachineConfig.h"
#include "PopUpCuEliteStepperCtr.h"
#include "CuEliteWorkHolderSetupPage.h"
#include "CommFunc.h"
#include "TeachCuEliteWorkHolderSheet.h"
#include "ScanKeyThread.h"

// CCuEliteDeviceOffsetSetupPage dialog

IMPLEMENT_DYNAMIC(CCuEliteDeviceOffsetSetupPage, CPropertyPage)

CCuEliteDeviceOffsetSetupPage::CCuEliteDeviceOffsetSetupPage()
	: CPropertyPage(CCuEliteDeviceOffsetSetupPage::IDD)
	, m_nElevatorId(ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
    , m_nStepMotorId(ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
    , m_nRailMotorId(ID_DEVOFFSET_FRONTRAIL_MOTOR)

//v2.18.3.S1 edit
    , m_lMagDevLength(0)
    , m_lMagDevWidth(0)
    , m_lLFTrackDevWidth(0)
    , m_lMagSysLength(0)
    , m_lMagSysWidth(0)
//v2.8.15.w4 delete  , m_lLFTrackSysWidth(0)
//v2.8.15.w4 delete    , m_lMagLengthOffset(0)
//v2.8.15.w4 delete   , m_lMagWidthOffset(0)
//v2.8.15.w4 delete    , m_lLFTrackWidthOffset(0)
    , m_lLFTrackOffset(0)
    , m_lOnLoadMagZineYOneCurPos(0)
    , m_lOnLoadMagZineYTwoCurPos(0)
    , m_lOffLoadMagZineYOneCurPos(0)
    , m_lOffLoadMagZineYTwoCurPos(0)


{
     m_bStepperAddr =  ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR;
}

CCuEliteDeviceOffsetSetupPage::~CCuEliteDeviceOffsetSetupPage()
{
}

void CCuEliteDeviceOffsetSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	/*v2.18.5.w4 delete {
	DDX_CBIndex(pDX, IDC_COMBO_SIDE_ID, m_nElevatorId);
    DDX_CBIndex(pDX, IDC_COMBO_YAXIS_STEPPERS, m_nStepMotorId);
    DDX_CBIndex(pDX, IDC_COMBO_INDEX_STEPPERS, m_nRailMotorId);
	//v2.18.3.S1 edit

    DDX_Text(pDX, IDC_EDIT_MAG_LENGTH, m_lMagDevLength);
    DDX_Text(pDX, IDC_EDIT_MAG_WIDTH, m_lMagDevWidth);
    DDX_Text(pDX, IDC_EDIT_TRACK_WIDTH,  m_lLFTrackDevWidth);
    DDX_Text(pDX, IDC_EDIT_MAG_SYSTEM_LENGTH, m_lMagSysLength);
    DDX_Text(pDX, IDC_EDIT_MAG_SYSTEM_WIDTH, m_lMagSysWidth);
    DDX_Text(pDX, IDC_EDIT_TRACK_SYSTEM_WIDTH, m_lLFTrackSysWidth);
    DDX_Text(pDX, IDC_EDIT_MAG_LENGTH_OFFSET, m_lMagLengthOffset);
    DDX_Text(pDX, IDC_EDIT_MAG_WIDTH_OFFSET, m_lMagWidthOffset);
    DDX_Text(pDX, IDC_EDIT_MAG_WIDTH_OFFSET, m_lMagWidthOffset);
    DDX_Text(pDX, IDC_EDIT_TRACK_WIDTH_OFFSET,  m_lLFTrackWidthOffset);
     }*/
    DDX_Text(pDX, IDC_EDIT_TRACK_DIST,  m_lTrackStandby);
    DDX_Text(pDX, IDC_EDIT_MAG_LENGTH_DIST,  m_lLengthStandby);
	DDX_Text(pDX, IDC_EDIT_MAG_WIDTH_Y2,  m_lWidthStandby);
	DDX_Text(pDX, IDC_EDIT_MAG_CLAMP_DEPTH,  m_lDepthStandby);


	DDX_CBIndex(pDX, IDC_COMBO_TRACK_MOTOR,m_nRailMotorId );
    DDX_CBIndex(pDX, IDC_COMBO_ELEVATOR_MOTOR, m_nElevatorId);
    DDX_CBIndex(pDX, IDC_COMBO_MAG_WIDTH_MOTOR,m_nStepMotorId );
    DDX_CBIndex(pDX, IDC_COMBO_MAG_CLAMP_MOTOR,m_nClampMotorId );
	DDX_Text(pDX, IDC_EDIT_FILE3, m_sID);
}


BEGIN_MESSAGE_MAP(CCuEliteDeviceOffsetSetupPage, CPropertyPage)
	//v2.18.3.S1 edit
    ON_WM_CTLCOLOR()
	/* v2.18.5.w4 delete {
	ON_CBN_SELCHANGE(IDC_COMBO_SIDE_ID, &CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeComboSideId)
    ON_CBN_SELCHANGE(IDC_COMBO_INDEX_STEPPERS, &CCuEliteDeviceOffsetSetupPage::OnLbnSelchangeLFTrackStepperMotor)
    ON_CBN_SELCHANGE(IDC_COMBO_YAXIS_STEPPERS, &CCuEliteDeviceOffsetSetupPage::OnLbnSelchangeYAxisStepperMotor)

    ON_EN_SETFOCUS(IDC_EDIT_MAG_LENGTH, &CCuEliteDeviceOffsetSetupPage::OnEnChangeEditMagLength)
    ON_EN_SETFOCUS(IDC_EDIT_MAG_WIDTH, &CCuEliteDeviceOffsetSetupPage::OnEnChangeEditMagWidth)
    ON_EN_SETFOCUS(IDC_EDIT_TRACK_WIDTH, &CCuEliteDeviceOffsetSetupPage::OnEnChangeEditTrackWidth)
    ON_BN_CLICKED(IDC_EDIT_TRACK_MOTOR, &CCuEliteDeviceOffsetSetupPage::OnBnClickedEditTrackMotor)
    ON_BN_CLICKED(IDC_EDIT_XAXIS_MOTOR, &CCuEliteDeviceOffsetSetupPage::OnBnClickedEditXaxisMotor)
    ON_BN_CLICKED(IDC_EDIT_YAXIS_MOTOR, &CCuEliteDeviceOffsetSetupPage::OnBnClickedEditYaxisMotor)
    //ON_EN_SETFOCUS(IDC_EDIT_MAG_LENGTH, &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagLength)
    //ON_EN_SETFOCUS(IDC_EDIT_MAG_WIDTH,  &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagWidth)
    //ON_EN_SETFOCUS(IDC_EDIT_TRACK_WIDTH, &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditLFTrackWidth)
	ON_BN_CLICKED(IDC_EDIT_TRACK_MOTOR3, &CCuEliteDeviceOffsetSetupPage::OnBnClickedEditTrackMotor3)

	}*/

	//v2.18.5.w3 add {
	ON_BN_CLICKED(IDC_BUTTON_TRACK_HOME, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonTrackHome)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_STANDBY, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonTrackStandby)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_TEACH, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonTrackTeach)
	ON_BN_CLICKED(IDC_BUTTON_MAG_LENGTH_HOME, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagLengthHome)
	ON_BN_CLICKED(IDC_BUTTON_MAG_LENGTH_STANDBY, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagLengthStandby)
	ON_BN_CLICKED(IDC_BUTTON_MAG_LENGTH_TEACH, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagLengthTeach)
	ON_BN_CLICKED(IDC_BUTTON_MAG_WIDTH_HOME, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthHome)
	ON_BN_CLICKED(IDC_BUTTON_MAG_WIDTH_STANDBY, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthStandby)
	ON_BN_CLICKED(IDC_BUTTON_MAG_WIDTH_TEACH, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthTeach) 
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_OPEN, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagClampOpen)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_CLOSE, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagClampClose)
	ON_BN_CLICKED(IDC_BUTTON_MAG_CLAMP_TEACH, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagClampTeach)
	ON_EN_SETFOCUS(IDC_EDIT_TRACK_DIST, &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditTrackDist)
    ON_EN_SETFOCUS(IDC_EDIT_MAG_LENGTH_DIST, &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagLength)
    ON_EN_SETFOCUS(IDC_EDIT_MAG_WIDTH_Y2,  &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagWidth)
    ON_EN_SETFOCUS(IDC_EDIT_MAG_CLAMP_DEPTH, &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagClampDepth)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACK_MOTOR, &CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeTrackMotor)	
	ON_CBN_SELCHANGE(IDC_COMBO_ELEVATOR_MOTOR, &CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeElevatorMotor)	
	ON_CBN_SELCHANGE(IDC_COMBO_MAG_WIDTH_MOTOR, &CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeMagWidthMotor)	
	ON_CBN_SELCHANGE(IDC_COMBO_MAG_CLAMP_MOTOR, &CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeMagClampMotor)
	ON_BN_CLICKED(IDC_BUTTON_INITE2, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonInite2)


	//}

	ON_BN_CLICKED(IDC_BUTTON_MAG_WIDTH_STANDBY2, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthStandby2)
	ON_BN_CLICKED(IDC_BUTTON_MAG_WIDTH_TEACH2, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthTeach2)
	ON_EN_SETFOCUS(IDC_EDIT_FILE3, &CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditFile3)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATABASE, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonLoadDatabase)




	ON_BN_CLICKED(IDC_BUTTON_INITE2, &CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonInite2)
END_MESSAGE_MAP()


// CCuEliteDeviceOffsetSetupPage message handlers

BOOL CCuEliteDeviceOffsetSetupPage::OnInitDialog()
{

    CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        pChildWnd->SetFont(theApp.m_pCurrentFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

    CString szString,sString;

    CWnd* pWnd  = GetDlgItem(IDC_STATIC_TRACK_MOTOR);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOTORS, szString);
    pWnd->SetWindowTextW(szString);

    pWnd  = GetDlgItem(IDC_STATIC_TRACK_MOTOR2);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOTORS, szString);
    pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_TRACK_DIST);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
	szString.Format(_T("%s:"),sString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_MAG_LENGTH_DIST);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
	szString.Format(_T("%s:"),sString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_MOTOR);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOTORS, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_DIST);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
	szString.Format(_T("%s:"),sString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_MAG_CLAMP_CURRENT_DEPTH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_CURRENT_DEPTH, sString);
	szString.Format(_T("%s:"),sString);
	pWnd->SetWindowTextW(szString);


    pWnd = GetDlgItem(IDC_BUTTON_INITE2);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_INITIALIZE, szString);
	pWnd->SetWindowTextW(szString);


	pWnd =GetDlgItem(IDC_STATIC_LABEL_TRACK);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TRACK, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_LABEL_MAG_LENGTH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_LENGTH, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_LABEL_MAG_WIDTH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_WIDTH, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_STATIC_LABEL_MAG_CLAMP);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
	pWnd->SetWindowTextW(szString);






     CPropertyPage::OnInitDialog();
    // TODO:  Add extra initialization here


    //v2.18.5.w4  not in used {
  	//v2.18.3.S1 add

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);



    //v2.8.15.w4 delete m_lMagDevLength         = stMcData.lOnLoadMagazineDevLength;
    //v2.8.15.w4 delete m_lMagDevWidth          = stMcData.lOnLoadMagazineDevWidth;
    //v2.8.15.w4 delete m_lLFTrackDevWidth      = stMcData.lLeadFrameTrackDevWidth;
    //v2.8.15.w4 delete m_lMagSysLength         = stMcData.lOnLoadMagazineSysLength;
    //v2.8.15.w4 delete m_lMagSysWidth          = stMcData.lOnLoadMagazineSysWidth;
    //v2.18.5.w4 delete m_lLFTrackSysWidth      = stMcData.lLeadFrameTrackSysWidth;
    //v2.18.5.w4 delete  m_lMagLengthOffset      = stMcData.lOnLoadMagazineLengthOffset;
    //v2.18.3.S2 delete
    //v2.18.5.w4 m_lMagWidthOffset       = stMcData.lOnLoadMagzieWidthOffset;
    //v2.18.5.w4 m_lMagWidthOffset       = stMcData.lOnLoadMagazineYOneOffset;
	//v2.18.5.w4 m_lLFTrackWidthOffset   = stMcData.lLFFrontTrackOffset;
    //}
	UpdateData(false);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}




BOOL CCuEliteDeviceOffsetSetupPage::OnSetActive( )
{
	CString sPath, szString,sString;
      
    setRefFile();

    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

    sPath=stCuEliteDevicePara.cDevicePath;
    m_newFile.setPathTitleExt( sPath );

    m_sID=stCuEliteDevicePara.cDeviceID;



	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();

	if (psheet->m_iPageMode == WH_CUELITE_TEACH)
	{

		CWnd* pWnd = GetDlgItem(IDC_EDIT_FILE3);
		pWnd->EnableWindow(SW_HIDE);


	}
	else
	{
		CWnd* pWnd = psheet->GetDlgItem(IDOK);
		pWnd->EnableWindow(true);

		pWnd = GetDlgItem(IDC_EDIT_FILE3);
		pWnd->EnableWindow(SW_HIDE);

	}


  if(CPropertyPage::OnSetActive()) {

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);


	CWnd* pWnd =GetDlgItem(IDC_BUTTON_TRACK_HOME);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_TRACK_STANDBY);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS , szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_TRACK_TEACH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TEACH, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_LENGTH_HOME);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_LENGTH_STANDBY);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS , szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_LENGTH_TEACH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TEACH, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_HOME);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_SHIFT, szString);
	pWnd->SetWindowTextW(szString);
	pWnd->ShowWindow(false);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH2);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TEACH, szString);
	pWnd->SetWindowTextW(szString);
	pWnd->ShowWindow(false);


	pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS , szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TEACH, szString);
	pWnd->SetWindowTextW(szString);


	pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_OPEN);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_CLOSE);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	pWnd->SetWindowTextW(szString);

	pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_TEACH);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TEACH, szString);
	pWnd->SetWindowTextW(szString);


    pWnd = GetDlgItem(IDC_COMBO_TRACK_MOTOR);
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRACK_MOTOR);
    pComboBox->ResetContent();
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FRONT_RAIL, szString);
	pComboBox->InsertString(0, szString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_REAR_RAIL, szString);
    pComboBox->InsertString(1, szString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FRONT_REAR_RAIL, szString);
    pComboBox->InsertString(2, szString);


    pWnd = GetDlgItem(IDC_COMBO_ELEVATOR_MOTOR);
    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEVATOR_MOTOR);
    pComboBox->ResetContent();
	theMsgMgr.GetMsgString(MSG_MAGAZINE_SIDE_CLAMPER, szString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, sString);
	pComboBox->InsertString(0, sString+szString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, sString);
    pComboBox->InsertString(1, sString+szString);

    pWnd = GetDlgItem(IDC_COMBO_MAG_WIDTH_MOTOR);
    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAG_WIDTH_MOTOR);
    pComboBox->ResetContent();
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
	pComboBox->InsertString(0, sString+szString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_REAR_CLAMPER, szString);
    pComboBox->InsertString(1, sString+szString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
	pComboBox->InsertString(2, sString+szString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_REAR_CLAMPER, szString);
	pComboBox->InsertString(3, sString+szString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FRONT_REAR_CLAMPER, szString);
    pComboBox->InsertString(4, sString+szString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_FRONT_REAR_CLAMPER, szString);
	pComboBox->InsertString(5, sString+szString);



    pWnd = GetDlgItem(IDC_COMBO_MAG_CLAMP_MOTOR);
    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAG_CLAMP_MOTOR);
    pComboBox->ResetContent();

	theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
	pComboBox->InsertString(0, sString+szString);

    theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MAGAZINE_FRONT_CLAMPER, szString);
	pComboBox->InsertString(1, sString+szString);

	theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
    pComboBox->InsertString(2, sString+szString); 

	theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFFLOAD, sString);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_ELEVATOR, szString);
    pComboBox->InsertString(3, sString+szString); 

	//v2.18.3.S1 edit

    m_nRailMotorId=ID_DEVOFFSET_FRONTRAIL_MOTOR;
	theCuEliteTrack.sLogicToUI(CUELITE_FRONT_RAIL,stTrackRcd.lLFFrontTrackStandby, &m_lTrackStandby);	

	m_nStepMotorId = ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR;
	theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagazineYTwoStandby, &m_lWidthStandby);

    m_nElevatorId=ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR;
	theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_XAXIS,stOnloadData.lOnLoadMagazineLengthStandby, &m_lLengthStandby);	

    m_nClampMotorId=ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR;
	theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_XAXIS,stOnloadData.lOnLoadMagClamperDepth, &m_lDepthStandby);


 

	UpdateData(false);

  }

	return true;
}


BOOL CCuEliteDeviceOffsetSetupPage::OnKillActive()
{

	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	CCuEliteWorkHolderTeachSheet* psheet = (CCuEliteWorkHolderTeachSheet*)GetParent();

	if (psheet->m_iPageMode == WH_CUELITE_TEACH)
	{

	}
	else
	{

	}

	return ( CPropertyPage::OnKillActive());

}






HBRUSH CCuEliteDeviceOffsetSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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






void CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeElevatorMotor()
{


    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	// TODO: Add your control notification handler code here

	UpdateData(true);

	if(m_nElevatorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
	}
	else if( m_nElevatorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
	}


	if(m_bStepperAddr == CUELITE_ONLOAD_XAXIS)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_XAXIS,stOnloadData.lOnLoadMagazineLengthStandby, &m_lLengthStandby);	

	}
	else if(m_bStepperAddr ==  CUELITE_OFFLOAD_XAXIS)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_XAXIS,stOffloadData.lOffLoadMagazineLengthStandby, &m_lLengthStandby);	

	}
	UpdateData(false);


}

void CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagLength()
{
	//don't use IDC_STATIC_CURRENT_DEPTH, very important 
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

    long lExpectedDist;  

	UpdateData(true);

	// TODO: Add your control notification handler code here
	if(m_nElevatorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
	}
	else if( m_nElevatorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
	}

	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_XAXIS:

		if(!theKeyPad.GetUserKeyInData(&m_lLengthStandby, 0, 0,CUELITE_ONLOAD_XAXIS_MAX/10)){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lLengthStandby,&lExpectedDist);

			if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stOnloadData.lOnLoadMagazineLengthStandby = lExpectedDist;
				UpdateData(false);
				theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 

			}
		}
		break;

	case CUELITE_OFFLOAD_XAXIS:

		if(!theKeyPad.GetUserKeyInData(&m_lLengthStandby, 0, 0,CUELITE_OFFLOAD_XAXIS_MAX/10 )){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lLengthStandby,&lExpectedDist);

			if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stOffloadData.lOffLoadMagazineLengthStandby =  lExpectedDist;
				UpdateData(false);
				theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

			}
		}
		break;

	}

}


void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagLengthTeach()
{

	long lPos,lPosPulse;

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	// TODO: Add your control notification handler code here

	UpdateData(true);
	if(m_nElevatorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
	}
	else if( m_nElevatorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
	}



	CPopUpCuEliteStepperCtr stepDlg(m_bStepperAddr);
	if( stepDlg.DoModal()==IDOK){
		switch(m_bStepperAddr){
			case CUELITE_ONLOAD_XAXIS:
				theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
	            theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagazineLengthStandby=  lPos -stMcData.lOnLoadMagazineLengthOffset ; 
				break;

			case CUELITE_OFFLOAD_XAXIS:
				theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
	            theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagazineLengthStandby=  lPos -stMcData.lOffLoadMagazineLengthOffset ; 	
				break;
		}//switch
	}//if

	if(m_bStepperAddr == CUELITE_ONLOAD_XAXIS)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_XAXIS,stOnloadData.lOnLoadMagazineLengthStandby, &m_lLengthStandby);	
	}
	else if(m_bStepperAddr == CUELITE_OFFLOAD_XAXIS)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_XAXIS,stOffloadData.lOffLoadMagazineLengthStandby, &m_lLengthStandby);	

	}

	UpdateData(false);
    theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
    theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);


}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagLengthHome()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nElevatorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
	}
	else if( m_nElevatorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
	}
	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_XAXIS:

            theCuEliteOnLoader.SearchOnloadXAxisStepHome();
	        Sleep(10);
	        theCuEliteOnLoader.WaitOnloadXAxisStepHomeFinish();
		break;

	case CUELITE_OFFLOAD_XAXIS:

	        theCuEliteOffLoader.SearchOffloadXAxisStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitOffloadXAxisStepHomeFinish();
		break;
	}

}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagLengthStandby()
{

	MACHINE_DATA stMcData;
	theMachineData.GetMachineDataRcd(&stMcData);
	// TODO: Add your control notification handler code here
    UpdateData(true);
	if(m_nElevatorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_XAXIS;
	}
	else if( m_nElevatorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_XAXIS;
	}
	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_XAXIS:

             theCuEliteOnLoader.moveXaxisToStandbyPos();

		break;

	case CUELITE_OFFLOAD_XAXIS:

			 theCuEliteOffLoader.moveXaxisToStandbyPos();
		break;
	}
 
}



void CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeMagWidthMotor()
{
    CWnd *pWnd;

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

    CString szString,sString;

	UpdateData(true);

	// TODO: Add your control notification handler code here
	if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE_YTWO;
	}

	switch(m_bStepperAddr) {
		case CUELITE_ONLOAD_YTWO:
			 pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
             pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH2);
	         pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_HOME);
	         pWnd->ShowWindow(true);

	         pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_DIST);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
			 szString.Format(_T("%s:"),sString);
	         pWnd->SetWindowTextW(szString);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	         pWnd->SetWindowTextW(szString);

		     theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagazineYTwoStandby, &m_lWidthStandby);	

			 break;
		case CUELITE_ONLOAD_YONE:
			 pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
             pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH2);
	         pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_HOME);
	         pWnd->ShowWindow(true);

	         pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_DIST);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
			 szString.Format(_T("%s:"),sString);
	         pWnd->SetWindowTextW(szString);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	         pWnd->SetWindowTextW(szString);

		     theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YONE,stOnloadData.lOnLoadMagazineYOneStandby, &m_lWidthStandby);	

			 break;
		case CUELITE_ONLOAD_YONE_YTWO:
			 pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
             pWnd->ShowWindow(true);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH2);
	         pWnd->ShowWindow(true);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_HOME);
	         pWnd->ShowWindow(false);

	         pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_DIST);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOVE_DIST, sString);
			 szString.Format(_T("%s:"),sString);
	         pWnd->SetWindowTextW(szString);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_SHIFT, szString);
	         pWnd->SetWindowTextW(szString);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_WIDTH, szString);
	         pWnd->SetWindowTextW(szString);

			 m_lWidthStandby = 0;//um
			 break;
		case CUELITE_OFFLOAD_YTWO:
			 pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
             pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH2);
	         pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_HOME);
	         pWnd->ShowWindow(true);

	         pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_DIST);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
			 szString.Format(_T("%s:"),sString);
	         pWnd->SetWindowTextW(szString);
			 pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	         pWnd->SetWindowTextW(szString);

		     theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,stOffloadData.lOffLoadMagazineYTwoStandby, &m_lWidthStandby);	


			 break;
		case CUELITE_OFFLOAD_YONE:
			 pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
             pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH2);
	         pWnd->ShowWindow(false);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_HOME);
	         pWnd->ShowWindow(true);

	         pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_DIST);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
			 szString.Format(_T("%s:"),sString);
	         pWnd->SetWindowTextW(szString);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	         pWnd->SetWindowTextW(szString);

		     theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YONE,stOffloadData.lOffLoadMagazineYOneStandby, &m_lWidthStandby);	

			 break;
		case CUELITE_OFFLOAD_YONE_YTWO:
			 pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
             pWnd->ShowWindow(true);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_TEACH2);
	         pWnd->ShowWindow(true);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_HOME);
	         pWnd->ShowWindow(false);

	         pWnd =GetDlgItem(IDC_STATIC_MAG_WIDTH_DIST);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOVE_DIST, sString);
			 szString.Format(_T("%s:"),sString);
	         pWnd->SetWindowTextW(szString);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY2);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_SHIFT, szString);
	         pWnd->SetWindowTextW(szString);
	         pWnd =GetDlgItem(IDC_BUTTON_MAG_WIDTH_STANDBY);
	         theMsgMgr.GetMsgString(MSG_MENU_ITEM_WIDTH, szString);
	         pWnd->SetWindowTextW(szString);

			 m_lWidthStandby = 0;//um
		     break;

	}

	UpdateData(false);


}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthTeach()
{

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	long lPos,lPos2,lPosPulse,lPosPulse2;
	long lOnloadPos, lOnloadPos2, lOnloadPosPulse, lOnloadPosPulse2;
	long lOffloadPos, lOffloadPos2,lOffloadPosPulse, lOffloadPosPulse2;

	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE_YTWO_WIDTH;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE_YTWO_WIDTH;
	}

	theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE, &lOnloadPosPulse);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YONE, lOnloadPosPulse,  &lOnloadPos);
	theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lOnloadPosPulse2);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lOnloadPosPulse2,  &lOnloadPos2);

	theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lOffloadPosPulse2);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YTWO, lOffloadPosPulse2,  &lOffloadPos2);
	theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE, &lOffloadPosPulse);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YONE, lOffloadPosPulse,   &lOffloadPos);

	CPopUpCuEliteStepperCtr stepDlg(m_bStepperAddr);
	if( stepDlg.DoModal()==IDOK){

		switch(m_bStepperAddr){
			case CUELITE_ONLOAD_YTWO:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagazineYTwoStandby=  lPos ; 
				break;

			case CUELITE_ONLOAD_YONE:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagazineYOneStandby=  lPos  ; 		
				break;
			case CUELITE_OFFLOAD_YTWO:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagazineYTwoStandby=  lPos ;
				break;
			case CUELITE_OFFLOAD_YONE:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagazineYOneStandby=  lPos ; 	
				break;
			case CUELITE_ONLOAD_YONE_YTWO_WIDTH:
		        theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lPosPulse2);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lPosPulse2,  &lPos2);
				stOnloadData.lOnLoadMagazineYTwoStandby=  lPos2  ;

		        theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YONE, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagazineYOneStandby=  lPos ; 
				break;
			case CUELITE_OFFLOAD_YONE_YTWO_WIDTH:

				theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lPosPulse2);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YTWO, lPosPulse2,  &lPos2);
				stOffloadData.lOffLoadMagazineYTwoStandby=  lPos2 ; 

		        theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YONE, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagazineYOneStandby=  lPos ; 
				break;

		}//switch
	}//if

	switch(m_bStepperAddr) {
		case CUELITE_ONLOAD_YTWO:

			    theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagazineYTwoStandby, &m_lWidthStandby);	

				break;
        case CUELITE_ONLOAD_YONE:

			    theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YONE,stOnloadData.lOnLoadMagazineYOneStandby, &m_lWidthStandby);	

				break;
		case CUELITE_ONLOAD_YONE_YTWO:

			    theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,(lPos-lOnloadPos), &m_lWidthStandby);	

				break;
		case CUELITE_OFFLOAD_YTWO:

			    theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,stOffloadData.lOffLoadMagazineYTwoStandby, &m_lWidthStandby);	

			    break;
		case CUELITE_OFFLOAD_YONE:

			    theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YONE,stOffloadData.lOffLoadMagazineYOneStandby, &m_lWidthStandby);	

                break;
		case CUELITE_OFFLOAD_YONE_YTWO:

				theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,(lPos-lOffloadPos), &m_lWidthStandby);	

                break;

	}

	UpdateData(false);
    theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
    theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthTeach2()
{

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	long lPos,lPos2,lPosPulse,lPosPulse2;
	long lOnloadPos, lOnloadPos2, lOnloadPosPulse, lOnloadPosPulse2;
	long lOffloadPos, lOffloadPos2,lOffloadPosPulse, lOffloadPosPulse2;

	// TODO: Add your control notification handler code here
 	UpdateData(true);
	if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE_YTWO;
	}


	theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE, &lOnloadPosPulse);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YONE, lOnloadPosPulse,  &lOnloadPos);
	theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lOnloadPosPulse2);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lOnloadPosPulse2,  &lOnloadPos2);

	theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lOffloadPosPulse2);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YTWO, lOffloadPosPulse2,  &lOffloadPos2);
	theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE, &lOffloadPosPulse);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YONE, lOffloadPosPulse,   &lOffloadPos);

	CPopUpCuEliteStepperCtr stepDlg(m_bStepperAddr);
	if( stepDlg.DoModal()==IDOK){

		switch(m_bStepperAddr){
			case CUELITE_ONLOAD_YTWO:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagazineYTwoStandby=  lPos ; 
				break;

			case CUELITE_ONLOAD_YONE:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagazineYOneStandby=  lPos  ; 		
				break;
			case CUELITE_OFFLOAD_YTWO:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagazineYTwoStandby=  lPos ; 
				break;
			case CUELITE_OFFLOAD_YONE:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagazineYOneStandby=  lPos ; 		
				break;
			case CUELITE_ONLOAD_YONE_YTWO:
		        theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lPosPulse2);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lPosPulse2,  &lPos2);
				stOnloadData.lOnLoadMagazineYTwoStandby=  lPos2  ; 

		        theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YONE, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagazineYOneStandby=  lPos ; 

				break;
			case CUELITE_OFFLOAD_YONE_YTWO:
				theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lPosPulse2);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YTWO, lPosPulse2,  &lPos2);
				stOffloadData.lOffLoadMagazineYTwoStandby=  lPos2 ; 

		        theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YONE, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagazineYOneStandby=  lPos ; 

				break;

		}//switch
	}//if

	switch(m_bStepperAddr) {
		case CUELITE_ONLOAD_YTWO:

		        theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagazineYTwoStandby, &m_lWidthStandby);	

				break;
        case CUELITE_ONLOAD_YONE:

		        theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YONE,stOnloadData.lOnLoadMagazineYOneStandby, &m_lWidthStandby);	

				break;
		case CUELITE_ONLOAD_YONE_YTWO:

		        theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,(lPos-lOnloadPos), &m_lWidthStandby);	
					
				break;
		case CUELITE_OFFLOAD_YTWO:

		        theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,stOffloadData.lOffLoadMagazineYTwoStandby, &m_lWidthStandby);	

			    break;
		case CUELITE_OFFLOAD_YONE:

		        theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YONE,stOffloadData.lOffLoadMagazineYOneStandby, &m_lWidthStandby);	

                break;
		case CUELITE_OFFLOAD_YONE_YTWO:

		        theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,(lPos-lOffloadPos), &m_lWidthStandby);	


                break;

	}

	UpdateData(false);
    theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
    theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

}

void CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagWidth()
{
	//don't use IDC_STATIC_CURRENT_DEPTH, very important 
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();



    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	long lPos;
    long lExpectedDist, lExpectedDist2;

	UpdateData(true);
	// TODO: Add your control notification handler code here
	if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE_YTWO;
	}



	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_YTWO:

		if(!theKeyPad.GetUserKeyInData(&m_lWidthStandby, 0, 0, CUELITE_ONLOAD_YTWO_MAX/10 )){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lWidthStandby,&lExpectedDist);

		     if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stOnloadData.lOnLoadMagazineYTwoStandby=lExpectedDist ; 
				UpdateData(false);
                theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 

			}
		}
		break;

	case CUELITE_ONLOAD_YONE:

		if(!theKeyPad.GetUserKeyInData(&m_lWidthStandby, 0, 0,CUELITE_ONLOAD_YONE_MAX/10 )){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lWidthStandby,&lExpectedDist);

		    if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stOnloadData.lOnLoadMagazineYOneStandby = lExpectedDist; 
				UpdateData(false);
                theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
			}
		}
		break;
	case CUELITE_OFFLOAD_YTWO:

		if(!theKeyPad.GetUserKeyInData(&m_lWidthStandby, 0, 0,CUELITE_OFFLOAD_YTWO_MAX/10 )){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lWidthStandby,&lExpectedDist);

		    if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stOffloadData.lOffLoadMagazineYTwoStandby =lExpectedDist;
				UpdateData(false);
                theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);
			}
		}
		break;

	case CUELITE_OFFLOAD_YONE:

		if(!theKeyPad.GetUserKeyInData(&m_lWidthStandby, 0, 0,CUELITE_OFFLOAD_YONE_MAX/10 )){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lWidthStandby,&lExpectedDist);

		    if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stOffloadData.lOffLoadMagazineYOneStandby =lExpectedDist;
				UpdateData(false);
                theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);
			}
		}
		break;
	case CUELITE_ONLOAD_YONE_YTWO:

		if(!theKeyPad.GetUserKeyInData(&m_lWidthStandby, 0, -3000,3000)){//um

		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YONE,m_lWidthStandby,&lExpectedDist);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YTWO,m_lWidthStandby,&lExpectedDist2);

				m_lExpectedDist2 =lExpectedDist2;
				m_lExpectedDist=lExpectedDist;
			    UpdateData(false);
                theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
                theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

		}

		break;

	case CUELITE_OFFLOAD_YONE_YTWO:

		if(!theKeyPad.GetUserKeyInData(&m_lWidthStandby, 0, -3000,3000)){//um
  
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YONE,m_lWidthStandby,&lExpectedDist);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YTWO,m_lWidthStandby,&lExpectedDist2);


				m_lExpectedDist2=lExpectedDist2;
				m_lExpectedDist =lExpectedDist;

				UpdateData(false);
                theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
                theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);



		}

		break;

	}

}


void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthHome()
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
	if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE_YTWO;
	}


	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_YTWO:

	        theCuEliteOnLoader.SearchOnloadYTwoAxisStepHome();
	        Sleep(10);
	        theCuEliteOnLoader.WaitOnloadYTwoAxisStepHomeFinish();
		break;

	case CUELITE_ONLOAD_YONE:

	        theCuEliteOnLoader.SearchOnloadYOneAxisStepHome();
	        Sleep(10);
	        theCuEliteOnLoader.WaitOnloadYOneAxisStepHomeFinish();
		break;
	case CUELITE_OFFLOAD_YTWO:

	        theCuEliteOffLoader.SearchOffloadYTwoAxisStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitOffloadYTwoAxisStepHomeFinish();
		break;

	case CUELITE_OFFLOAD_YONE:
	        theCuEliteOffLoader.SearchOffloadYOneAxisStepHome();
	        Sleep(10);
	        theCuEliteOffLoader.WaitOffloadYOneAxisStepHomeFinish();
		break;

	case CUELITE_ONLOAD_YONE_YTWO:

		break;

	case CUELITE_OFFLOAD_YONE_YTWO:

		break;
	}





}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthStandby()
{
    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	long lExpectedDist, lExpectedDist2;
	long lPos,lPos2,lPosPulse,lPosPulse2;



	// TODO: Add your control notification handler code here
    UpdateData(true);
	if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE_YTWO;
	}

	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_YTWO:
			 theCuEliteOnLoader.moveYTwoAxisToStandbyPos();
		break;

	case CUELITE_ONLOAD_YONE:

			 theCuEliteOnLoader.moveYOneAxisToStandbyPos();
		break;
	case CUELITE_OFFLOAD_YTWO:

			 theCuEliteOffLoader.moveYTwoAxisToStandbyPos();
		break;

	case CUELITE_OFFLOAD_YONE:

			 theCuEliteOffLoader.moveYOneAxisToStandbyPos();
		break;
	case CUELITE_ONLOAD_YONE_YTWO:

	
	         theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE,  &lPosPulse);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YONE, lPosPulse,  &lPos);
	         theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lPosPulse2);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lPosPulse2,  &lPos2);

			 lExpectedDist2=lPos2-m_lExpectedDist;
			 lExpectedDist =lPos-m_lExpectedDist;

			 if( ! theCuEliteTrack.isOutOfSoftLimit(CUELITE_ONLOAD_YONE,lExpectedDist,1) &&
					! theCuEliteTrack.isOutOfSoftLimit(CUELITE_ONLOAD_YTWO,lExpectedDist2,1)){

				 stOnloadData.lOnLoadMagazineYTwoStandby =lExpectedDist2;
				 stOnloadData.lOnLoadMagazineYOneStandby = lExpectedDist;

                 theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 			 
			     //update before move
	
				 theCuEliteOnLoader.moveYOneAxisToStandbyPos();
				 theCuEliteOnLoader.moveYTwoAxisToStandbyPos();
			 }

		break;

	case CUELITE_OFFLOAD_YONE_YTWO:

	         theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE,  &lPosPulse);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YONE, lPosPulse,  &lPos);
	         theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lPosPulse2);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YTWO, lPosPulse2,  &lPos2);

			 lExpectedDist2=lPos2-m_lExpectedDist2;
			 lExpectedDist =lPos-m_lExpectedDist;

			 if( ! theCuEliteTrack.isOutOfSoftLimit(CUELITE_OFFLOAD_YONE,lExpectedDist,1) &&
					! theCuEliteTrack.isOutOfSoftLimit(CUELITE_OFFLOAD_YTWO,lExpectedDist2,1)){


				stOffloadData.lOffLoadMagazineYTwoStandby =lExpectedDist2;
				stOffloadData.lOffLoadMagazineYOneStandby =lExpectedDist;
				theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);
                //update before move

				theCuEliteOffLoader.moveYOneAxisToStandbyPos();
				theCuEliteOffLoader.moveYTwoAxisToStandbyPos();
			 }

		break;
	}



}


void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagWidthStandby2()
{
    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

    long lExpectedDist, lExpectedDist2;
	long lPos,lPos2,lPosPulse,lPosPulse2;

	// TODO: Add your control notification handler code here
    UpdateData(true);
	if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YONE_YTWO;
	}
	else if( m_nStepMotorId == ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YONE_YTWO;
	}

	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_YTWO:
	
		break;

	case CUELITE_ONLOAD_YONE:

		break;
	case CUELITE_OFFLOAD_YTWO:

		break;

	case CUELITE_OFFLOAD_YONE:

		break;
	case CUELITE_ONLOAD_YONE_YTWO:
	
	         theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE,  &lPosPulse);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YONE, lPosPulse,  &lPos);
	         theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lPosPulse2);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lPosPulse2,  &lPos2);


			 lExpectedDist2=lPos2+m_lExpectedDist2;
			 lExpectedDist =lPos-m_lExpectedDist;

			 if( ! theCuEliteTrack.isOutOfSoftLimit(CUELITE_ONLOAD_YONE,lExpectedDist,1) &&
					! theCuEliteTrack.isOutOfSoftLimit(CUELITE_ONLOAD_YTWO,lExpectedDist2,1)){

				stOnloadData.lOnLoadMagazineYTwoStandby =lExpectedDist2;
				stOnloadData.lOnLoadMagazineYOneStandby =lExpectedDist;

				theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
                //update before move

				theCuEliteOnLoader.moveYOneAxisToStandbyPos();
				theCuEliteOnLoader.moveYTwoAxisToStandbyPos();
			 }

		break;

	case CUELITE_OFFLOAD_YONE_YTWO:

	         theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE,  &lPosPulse);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YONE, lPosPulse,  &lPos);
	         theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lPosPulse2);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_OFFLOAD_YTWO, lPosPulse2,  &lPos2);

			 lExpectedDist2=lPos2+m_lExpectedDist2;
			 lExpectedDist =lPos-m_lExpectedDist;

			 if( ! theCuEliteTrack.isOutOfSoftLimit(CUELITE_OFFLOAD_YONE,lExpectedDist,1) &&
					! theCuEliteTrack.isOutOfSoftLimit(CUELITE_OFFLOAD_YTWO,lExpectedDist2,1)){

				stOffloadData.lOffLoadMagazineYTwoStandby =lExpectedDist2;
				stOffloadData.lOffLoadMagazineYOneStandby =lExpectedDist;
				theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);
                //update before move

				theCuEliteOffLoader.moveYOneAxisToStandbyPos();
				theCuEliteOffLoader.moveYTwoAxisToStandbyPos();
			}

		break;
	}



}


void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagClampTeach()
{
	long lPos,lPosPulse;

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);


	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_ELEVATOR;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_ELEVATOR;
	}




	CPopUpCuEliteStepperCtr stepDlg(m_bStepperAddr);
	if( stepDlg.DoModal()==IDOK){
		switch(m_bStepperAddr){
			case CUELITE_ONLOAD_YTWO:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOnloadData.lOnLoadMagClamperDepth=  lPos-stOnloadData.lOnLoadMagazineYTwoStandby ;
				break;

			case CUELITE_OFFLOAD_YTWO:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOffloadData.lOffLoadMagClamperDepth= lPos-stOffloadData.lOffLoadMagazineYTwoStandby ; 
				break;
			case CUELITE_ONLOAD_ELEVATOR:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOnloadData.lOnload1stSlotLevel= lPos ; 
				break;
			case CUELITE_OFFLOAD_ELEVATOR:
		        theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stOffloadData.lOffload1stSlotLevel=  lPos ;
				break;

		}//switch
	}//if

	if(m_bStepperAddr == CUELITE_ONLOAD_YTWO)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagClamperDepth, &m_lDepthStandby);	
	}
	else if( m_bStepperAddr== CUELITE_OFFLOAD_YTWO)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,stOffloadData.lOffLoadMagClamperDepth, &m_lDepthStandby);	
	}
	else if( m_bStepperAddr== CUELITE_ONLOAD_ELEVATOR)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_ELEVATOR,stOnloadData.lOnload1stSlotLevel, &m_lDepthStandby);	
	}
	else if( m_bStepperAddr== CUELITE_OFFLOAD_ELEVATOR)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_ELEVATOR,stOffloadData.lOffload1stSlotLevel, &m_lDepthStandby);	
	}



	UpdateData(false);
    theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
    theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

}




void CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeMagClampMotor()
{
    CString  szString,sString;

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	// TODO: Add your control notification handler code here



	UpdateData(true);

   if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;

		CWnd* pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_OPEN);
	    theMsgMgr.GetMsgString(MSG_MENU_ITEM_OPEN, szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_CLOSE);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_CLAMP , szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_STATIC_MAG_CLAMP_CURRENT_DEPTH);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_CURRENT_DEPTH, sString);
		szString.Format(_T("%s:"),sString);
		pWnd->SetWindowTextW(szString);



	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;

		CWnd* pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_OPEN);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_OPEN, szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_CLOSE);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_CLAMP , szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_STATIC_MAG_CLAMP_CURRENT_DEPTH);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_CURRENT_DEPTH, sString);
		szString.Format(_T("%s:"),sString);
		pWnd->SetWindowTextW(szString);

	}
    if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_ELEVATOR;

		CWnd * pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_OPEN);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_CLOSE);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_STATIC_MAG_CLAMP_CURRENT_DEPTH);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
		szString.Format(_T("%s:"),sString);
		pWnd->SetWindowTextW(szString);

	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_ELEVATOR;

		CWnd * pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_OPEN);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_HOME, szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_BUTTON_MAG_CLAMP_CLOSE);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
		pWnd->SetWindowTextW(szString);

		pWnd =GetDlgItem(IDC_STATIC_MAG_CLAMP_CURRENT_DEPTH);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
		szString.Format(_T("%s:"),sString);
		pWnd->SetWindowTextW(szString);

	}



	if(m_bStepperAddr == CUELITE_ONLOAD_YTWO)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagClamperDepth, &m_lDepthStandby);	
	}
	else if(m_bStepperAddr == CUELITE_OFFLOAD_YTWO)
	{
	    theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,stOffloadData.lOffLoadMagClamperDepth, &m_lDepthStandby);	
	}
	else if(m_bStepperAddr == CUELITE_ONLOAD_ELEVATOR)
	{
	    theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_ELEVATOR,stOnloadData.lOnload1stSlotLevel, &m_lDepthStandby);	
	}
	else if(m_bStepperAddr == CUELITE_OFFLOAD_ELEVATOR)
	{
	    theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_ELEVATOR,stOffloadData.lOffload1stSlotLevel, &m_lDepthStandby);	
	}


	UpdateData(false);

}
void CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditMagClampDepth()
{
	//don't use IDC_STATIC_CURRENT_DEPTH, very important 
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	long lPos,lPosPulse;

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

    long lExpectedDist;

	UpdateData(true);

	// TODO: Add your control notification handler code here
	if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}



	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_YTWO:
		if(!theKeyPad.GetUserKeyInData(&m_lDepthStandby , 300, 0,3000)){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lDepthStandby,&lExpectedDist);	

			if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,0)){
				stOnloadData.lOnLoadMagClamperDepth= lExpectedDist;
				UpdateData(false);
				theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 



			}
		}
		break;
	case CUELITE_OFFLOAD_YTWO:
		if(!theKeyPad.GetUserKeyInData(&m_lDepthStandby, 300, 0,3000)){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lDepthStandby,&lExpectedDist);

			if( ! theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,0)){
			    stOffloadData.lOffLoadMagClamperDepth = lExpectedDist;
			    UpdateData(false);
		    	theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);

		   }
		}
		break;

	}


}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagClampOpen()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_ELEVATOR;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_ELEVATOR;
	}



	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_YTWO:

        theCuEliteOnLoader.moveMagClamperStepBackward();
		break;

	case CUELITE_OFFLOAD_YTWO:

        theCuEliteOffLoader.moveMagClamperStepBackward();
		break;
	case CUELITE_ONLOAD_ELEVATOR:

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
		break;
	case CUELITE_OFFLOAD_ELEVATOR:

		//theCuEliteOffLoader
		theCuEliteOffLoader.SearchOffloadElvStepHome();
		Sleep(100);
		theCuEliteOffLoader.WaitStepHomeFinish();
		break;

	}


}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonMagClampClose()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_YTWO;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_YTWO;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_ONLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_ONLOAD_ELEVATOR;
	}
	else if(m_nClampMotorId == ID_DEVOFFSET_OFFLOAD_Y_AXIS_MOTOR)
	{
		m_bStepperAddr = CUELITE_OFFLOAD_ELEVATOR;
	}



	switch(m_bStepperAddr)
	{
	case CUELITE_ONLOAD_YTWO:

           theCuEliteOnLoader.moveMagClamperStepForward();
		break;

	case CUELITE_OFFLOAD_YTWO:

          theCuEliteOffLoader.moveMagClamperStepForward();
		break;
	case CUELITE_ONLOAD_ELEVATOR:


		theCuEliteOnLoader.moveElevatorAxisToStandbyPos();


		break;
	case CUELITE_OFFLOAD_ELEVATOR:

		theCuEliteOffLoader.moveElevatorAxisToStandbyPos();


		break;

	}
	
}






void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonTrackTeach()
{



    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

	long lPos,lPos2,lPosPulse,lPosPulse2;
	long lTrackPos, lTrackPos2, lTrackPosPulse, lTrackPosPulse2;


	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nRailMotorId == ID_DEVOFFSET_FRONTRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_RAIL;
	}
	else if(m_nRailMotorId == ID_DEVOFFSET_REARRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_REAR_RAIL;
	}
	else if( m_nRailMotorId  == ID_DEVOFFSET_FRONT_REAR_RAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_REAR;
	}

	theStepperControl.CheckStepMotionStatus(CUELITE_REAR_RAIL, &lTrackPosPulse);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_REAR_RAIL, lTrackPosPulse,  &lTrackPos); //rear
	theStepperControl.CheckStepMotionStatus(CUELITE_FRONT_RAIL, &lTrackPosPulse2);
	theCuEliteTrack.xPhysicToLogicPos(CUELITE_FRONT_RAIL, lTrackPosPulse2,  &lTrackPos2); //front


	CPopUpCuEliteStepperCtr stepDlg(m_bStepperAddr);
	if( stepDlg.DoModal()==IDOK){
		switch(m_bStepperAddr){
			case CUELITE_FRONT_RAIL:
				theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
	            theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stTrackRcd.lLFFrontTrackStandby=  lPos ; 
				break;

			case CUELITE_REAR_RAIL:
				theStepperControl.CheckStepMotionStatus(m_bStepperAddr, &lPosPulse);
	            theCuEliteTrack.xPhysicToLogicPos(m_bStepperAddr, lPosPulse,  &lPos);
				stTrackRcd.lLFRearTrackStandby=  lPos  ; 		
				break;

			case CUELITE_FRONT_REAR:
		        theStepperControl.CheckStepMotionStatus(CUELITE_FRONT_RAIL, &lPosPulse2);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_FRONT_RAIL, lPosPulse2,  &lPos2);
				stTrackRcd.lLFFrontTrackStandby=  lPos2  ; //rear

		        theStepperControl.CheckStepMotionStatus(CUELITE_REAR_RAIL, &lPosPulse);
                theCuEliteTrack.xPhysicToLogicPos(CUELITE_REAR_RAIL, lPosPulse,  &lPos);
				stTrackRcd.lLFRearTrackStandby=  lPos ;  //front

		}//switch
	}//if

	if(m_bStepperAddr == CUELITE_FRONT_RAIL)
	{
	    theCuEliteTrack.sLogicToUI(CUELITE_FRONT_RAIL,stTrackRcd.lLFFrontTrackStandby, &m_lTrackStandby);	

	}
	else if(m_bStepperAddr == CUELITE_REAR_RAIL)
	{
	    theCuEliteTrack.sLogicToUI(CUELITE_REAR_RAIL,stTrackRcd.lLFRearTrackStandby, &m_lTrackStandby);	
	}
	else if(m_bStepperAddr == CUELITE_FRONT_REAR)
	{
		theCuEliteTrack.sLogicToUI(CUELITE_FRONT_RAIL,(lPos-lTrackPos), &m_lTrackStandby);	
		
	}

 	UpdateData(false);
    theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);



}
void CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeTrackMotor()
{
    CString sString,szString;

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);
	// TODO: Add your control notification handler code here

	UpdateData(true);

	if(m_nRailMotorId == ID_DEVOFFSET_FRONTRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_RAIL;
	}
	else if( m_nRailMotorId == ID_DEVOFFSET_REARRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_REAR_RAIL;
	}
    else if( m_nRailMotorId == ID_DEVOFFSET_FRONT_REAR_RAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_REAR;
	}



	if(m_bStepperAddr == CUELITE_FRONT_RAIL)
	{
		CWnd* pWnd =GetDlgItem(IDC_BUTTON_TRACK_HOME);
		pWnd->ShowWindow(SW_NORMAL);
		pWnd =GetDlgItem(IDC_BUTTON_TRACK_STANDBY);
		pWnd->ShowWindow(SW_NORMAL);
		pWnd =GetDlgItem(IDC_BUTTON_TRACK_TEACH);
		pWnd->ShowWindow(SW_NORMAL);

		pWnd =GetDlgItem(IDC_STATIC_TRACK_DIST);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
		szString.Format(_T("%s:"),sString);
		pWnd->SetWindowTextW(szString);

	    pWnd =GetDlgItem(IDC_BUTTON_TRACK_STANDBY);
	    theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	    pWnd->SetWindowTextW(szString);


	    theCuEliteTrack.sLogicToUI(CUELITE_FRONT_RAIL,stTrackRcd.lLFFrontTrackStandby, &m_lTrackStandby);	
	}
	else if(m_bStepperAddr== CUELITE_REAR_RAIL)
	{

		CWnd* pWnd =GetDlgItem(IDC_BUTTON_TRACK_HOME);
		pWnd->ShowWindow(SW_NORMAL);
		pWnd =GetDlgItem(IDC_BUTTON_TRACK_STANDBY);
		pWnd->ShowWindow(SW_NORMAL);
		pWnd =GetDlgItem(IDC_BUTTON_TRACK_TEACH);
		pWnd->ShowWindow(SW_NORMAL);

		pWnd =GetDlgItem(IDC_STATIC_TRACK_DIST);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIST_TO_HOME, sString);
		szString.Format(_T("%s:"),sString);
		pWnd->SetWindowTextW(szString);

	    pWnd =GetDlgItem(IDC_BUTTON_TRACK_STANDBY);
	    theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, szString);
	    pWnd->SetWindowTextW(szString);


	    theCuEliteTrack.sLogicToUI(CUELITE_REAR_RAIL,stTrackRcd.lLFRearTrackStandby, &m_lTrackStandby);	
	}
	else if(m_bStepperAddr== CUELITE_FRONT_REAR)
	{

		CWnd* pWnd =GetDlgItem(IDC_BUTTON_TRACK_HOME);
		pWnd->ShowWindow(SW_HIDE);
		pWnd =GetDlgItem(IDC_BUTTON_TRACK_STANDBY);
		pWnd->ShowWindow(SW_NORMAL);
		pWnd =GetDlgItem(IDC_BUTTON_TRACK_TEACH);
		pWnd->ShowWindow(SW_NORMAL);

		pWnd =GetDlgItem(IDC_STATIC_TRACK_DIST);
		theMsgMgr.GetMsgString(MSG_MENU_ITEM_MOVE_DIST, sString);
		szString.Format(_T("%s:"),sString);
		pWnd->SetWindowTextW(szString);

	    pWnd =GetDlgItem(IDC_BUTTON_TRACK_STANDBY);
	    theMsgMgr.GetMsgString(MSG_MENU_ITEM_SHIFT, szString);
	    pWnd->SetWindowTextW(szString);

	    m_lTrackStandby = 0;//um
	}


	UpdateData(false);
}



void CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditTrackDist()
{
	//don't use IDC_STATIC_CURRENT_DEPTH, very important 
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	long lPos,lPosPulse;

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

    long lExpectedDist, lExpectedDist2;


	UpdateData(true);
	
	// TODO: Add your control notification handler code here
	if(m_nRailMotorId == ID_DEVOFFSET_FRONTRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_RAIL;
	}
	else if( m_nRailMotorId== ID_DEVOFFSET_REARRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_REAR_RAIL;
	}
	else if( m_nRailMotorId== ID_DEVOFFSET_FRONT_REAR_RAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_REAR;
	}


switch(m_bStepperAddr)
	{
	case CUELITE_FRONT_RAIL:

		if(!theKeyPad.GetUserKeyInData(&m_lTrackStandby, 0, 0,CUELITE_FRONT_RAIL_MAX/10 )){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lTrackStandby,&lExpectedDist);

		    if( !  theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stTrackRcd.lLFFrontTrackStandby= lExpectedDist;
				UpdateData(false);
                theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

			}
		}
		break;

	case CUELITE_REAR_RAIL:

		if(!theKeyPad.GetUserKeyInData(&m_lTrackStandby, 0, 0, CUELITE_REAR_RAIL_MAX/10 )){//um

		    theCuEliteTrack.sUIToLogic(m_bStepperAddr,m_lTrackStandby,&lExpectedDist);

		    if( !  theCuEliteTrack.isOutOfSoftLimit(m_bStepperAddr,lExpectedDist,1)){
				stTrackRcd.lLFRearTrackStandby = lExpectedDist;
				UpdateData(false);
                theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

			}
		}
		break;


	case CUELITE_FRONT_REAR:

		if(!theKeyPad.GetUserKeyInData(&m_lTrackStandby, 0, -3000,3000)){//um
  
		    theCuEliteTrack.sUIToLogic(CUELITE_REAR_RAIL,m_lTrackStandby,&lExpectedDist);
		    theCuEliteTrack.sUIToLogic(CUELITE_FRONT_RAIL,m_lTrackStandby,&lExpectedDist2);

				m_lExpectedDist2=lExpectedDist2; //front
				m_lExpectedDist =lExpectedDist;  //rear

				UpdateData(false);
                theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

		}
		break;

	}



}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonTrackHome()
{
	
	// TODO: Add your control notification handler code here
   UpdateData(true);
	if(m_nRailMotorId == ID_DEVOFFSET_FRONTRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_RAIL;
	}
	else if( m_nRailMotorId  == ID_DEVOFFSET_REARRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_REAR_RAIL;
	}
	switch(m_bStepperAddr)
	{
	case CUELITE_FRONT_RAIL:

	    theCuEliteTrack.SearchFrontRailStepHome();
	    Sleep(10);
	    theCuEliteTrack.WaitFrontRailStepHomeFinish();
		break;

	case CUELITE_REAR_RAIL:

	    theCuEliteTrack.SearchRearRailStepHome();
	    Sleep(10);
	    theCuEliteTrack.WaitRearRailStepHomeFinish();
		break;
	}


}


void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonTrackStandby()
{
	// TODO: Add your control notification handler code here

    long lExpectedDist, lExpectedDist2;
	long lPos,lPos2,lPosPulse,lPosPulse2;

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);


	UpdateData(true);
	if(m_nRailMotorId == ID_DEVOFFSET_FRONTRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_RAIL;
	}
	else if( m_nRailMotorId  == ID_DEVOFFSET_REARRAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_REAR_RAIL;
	}
	else if( m_nRailMotorId  == ID_DEVOFFSET_FRONT_REAR_RAIL_MOTOR)
	{
		m_bStepperAddr = CUELITE_FRONT_REAR;
	}



	switch(m_bStepperAddr)
	{
	case CUELITE_FRONT_RAIL:

        theCuEliteTrack.moveFrontRailToStandbyPos();
		break;

	case CUELITE_REAR_RAIL:

 		theCuEliteTrack.moveRearRailToStandbyPos();
		break;

	case CUELITE_FRONT_REAR:

	         theStepperControl.CheckStepMotionStatus(CUELITE_REAR_RAIL,  &lPosPulse);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_REAR_RAIL, lPosPulse,  &lPos);
	         theStepperControl.CheckStepMotionStatus(CUELITE_FRONT_RAIL, &lPosPulse2);
             theCuEliteTrack.xPhysicToLogicPos(CUELITE_FRONT_RAIL, lPosPulse2,  &lPos2);

			 lExpectedDist2=lPos2+m_lExpectedDist2; //front
			 lExpectedDist =lPos-m_lExpectedDist; //rear

			 if( ! theCuEliteTrack.isOutOfSoftLimit(CUELITE_REAR_RAIL,lExpectedDist,1) &&
					! theCuEliteTrack.isOutOfSoftLimit(CUELITE_FRONT_RAIL,lExpectedDist2,1)){

				stTrackRcd.lLFFrontTrackStandby =lExpectedDist2; //front
				stTrackRcd.lLFRearTrackStandby =lExpectedDist;   //rear

				theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd); 
                //update before move

				theCuEliteTrack.moveFrontRailToStandbyPos();
				theCuEliteTrack.moveRearRailToStandbyPos();
			 }
		break;

	}


}




/*v2.18.5.w4 delete {

void CCuEliteDeviceOffsetSetupPage::OnLbnSelchangeYAxisStepperMotor()
{
	//v2.18.3.S1 Add
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_YAXIS_STEPPERS);
    m_nStepMotorId = pComboBox->GetCurSel();

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);



    if(m_nElevatorId ==  ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
    {
        if( m_nStepMotorId  == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
        {
            m_lMagWidthOffset = stMcData.lOnLoadMagazineYOneOffset;
        }
        else if( m_nStepMotorId  == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
        {
            m_lMagWidthOffset = stMcData.lOnLoadMagazineYTwoOffset;
        }
		else if( m_nStepMotorId  == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR){
		

		}

    }
    else    
    {
        if( m_nStepMotorId  == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
        {
            m_lMagWidthOffset = stMcData.lOffLoadMagazineYOneOffset;
        }
        else if( m_nStepMotorId  == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
        {
            m_lMagWidthOffset = stMcData.lOffLoadMagazineYTwoOffset;
		}
		else if( m_nStepMotorId  == ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR){
		

		}


    }
	UpdateData(false);
   
}


*/


void CCuEliteDeviceOffsetSetupPage::OnLbnSelchangeLFTrackStepperMotor()
{
	//v2.18.3.S1 ADD
    // TODO: Add your control notification handler code here
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_INDEX_STEPPERS);
    m_nRailMotorId = pComboBox->GetCurSel();

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

    if(m_nRailMotorId == ID_DEVOFFSET_FRONTRAIL_MOTOR)
    {
        m_lLFTrackWidthOffset  = stMcData.lLFFrontTrackOffset;
    }
    else    
    {
        m_lLFTrackWidthOffset  = stMcData.lLFRearTrackOffset;
    } 

	UpdateData(false);
}





/*v1.18.5.w4  no more used
void CCuEliteDeviceOffsetSetupPage::OnCbnSelchangeComboSideId()
{
	// TODO: Add your control notification handler code here
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SIDE_ID);
	m_nElevatorId = pComboBox->GetCurSel();
	//v2.18.3.S1 add
    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
    if(m_nElevatorId ==  ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
    {
     //v2.8.15.w4 delete    m_lMagDevLength         = stMcData.lOnLoadMagazineDevLength;
     //v2.8.15.w4 delete    m_lMagDevWidth          = stMcData.lOnLoadMagazineDevWidth;
     //v2.8.15.w4 delete    m_lMagSysLength         = stMcData.lOnLoadMagazineSysLength;
     //v2.8.15.w4 delete   m_lMagSysWidth          = stMcData.lOnLoadMagazineSysWidth;
        m_lMagLengthOffset      = stMcData.lOnLoadMagazineLengthOffset;
        //v2.18.3.S2 edit
        //m_lMagWidthOffset       = stMcData.lOnLoadMagzieWidthOffset;
        m_lMagWidthOffset        = stMcData.lOnLoadMagazineYOneOffset;
    }
    else
    {
		m_nStepMotorId  = ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR;
       //v2.8.15.w4 delete  m_lMagDevLength         = stMcData.lOffLoadMagazineDevLength;
       //v2.8.15.w4 delete  m_lMagDevWidth          = stMcData.lOffLoadMagazineDevWidth;
       //v2.8.15.w4 delete  m_lMagSysLength         = stMcData.lOffLoadMagazineSysLength;
       //v2.8.15.w4 delete  m_lMagSysWidth          = stMcData.lOffLoadMagazineSysWidth;
        m_lMagLengthOffset      = stMcData.lOffLoadMagazineLengthOffset;
        //v2.18.3.S2 edit
       // m_lMagWidthOffset       = stMcData.lOffLoadMagzieWidthOffset;
        m_lMagWidthOffset       = stMcData.lOffLoadMagazineYOneOffset;

    }

	UpdateData(false);

}
*/

void CCuEliteDeviceOffsetSetupPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	


	CPropertyPage::OnOK();
}


/*v2.18.5.w3 delete {
//v2.18.3.S1 add
void CCuEliteDeviceOffsetSetupPage::OnEnChangeEditMagLength()
{
    CWnd* pWnd = GetDlgItem(IDC_COMBO_SIDE_ID);
    pWnd->SetFocus();

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
    if(m_nElevatorId ==  ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
    {
        theKeyPad.GetUserKeyInData(&m_lMagDevLength, stMcData.lOnLoadMagazineDevLength, CUELITE_MAG_MINI_LENTH,
            CUELITE_MAG_MAX_LENTH);
        stMcData.lOnLoadMagazineDevLength = m_lMagDevLength;
		m_lMagLengthOffset = stMcData.lOnLoadMagazineLengthOffset = stMcData.lOnLoadMagazineSysLength - stMcData.lOnLoadMagazineDevLength ;

    }
    else
    {
        theKeyPad.GetUserKeyInData(&m_lMagDevLength, stMcData.lOffLoadMagazineDevLength, CUELITE_MAG_MINI_LENTH,
            CUELITE_MAG_MAX_LENTH);
         stMcData.lOffLoadMagazineDevLength = m_lMagDevLength;
		 m_lMagLengthOffset = stMcData.lOffLoadMagazineLengthOffset = stMcData.lOffLoadMagazineSysLength - stMcData.lOffLoadMagazineDevLength ;

    }
    theMachineData.StoreMachineDataRcd(&stMcData);
    UpdateData(false);
}
*/

/*v2.18.5.w4 no more used
//v2.18.3.S1 add   
void CCuEliteDeviceOffsetSetupPage::OnEnChangeEditMagWidth()
{
    CWnd* pWnd = GetDlgItem(IDC_COMBO_SIDE_ID);
    pWnd->SetFocus();

    long lValue;
    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

    if(m_nElevatorId ==  ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
    {
        theKeyPad.GetUserKeyInData(&lValue, stMcData.lOnLoadMagazineDevWidth, CUELITE_MAG_MIN_WIDTH/10,
            CUELITE_MAG_MAX_WIDTH/10);

        stMcData.lOnLoadMagazineDevWidth = m_lMagDevWidth=lValue*10;
		m_lMagWidthOffset = stMcData.lOnLoadMagazineWidthOffset = stMcData.lOnLoadMagazineSysWidth - stMcData.lOnLoadMagazineDevWidth ;


    }
    else
    {
         theKeyPad.GetUserKeyInData(&lValue, stMcData.lOffLoadMagazineDevWidth, CUELITE_MAG_MIN_WIDTH/10,
            CUELITE_MAG_MAX_WIDTH/10);
        stMcData.lOffLoadMagazineDevWidth = m_lMagDevWidth=lValue *10;
		m_lMagWidthOffset = stMcData.lOffLoadMagazineWidthOffset = stMcData.lOffLoadMagazineSysWidth - stMcData.lOffLoadMagazineDevWidth ;

    }
    theMachineData.StoreMachineDataRcd(&stMcData);
    UpdateData(false);
}
)*/
/* v2.18.5.w4 no more used
//v2.18.3.S1 add   
void CCuEliteDeviceOffsetSetupPage::OnEnChangeEditTrackWidth()
{
    CWnd* pWnd = GetDlgItem(IDC_COMBO_INDEX_STEPPERS);
    pWnd->SetFocus();

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
    
    if(stMcData.nLeadFrameTrackType ==  0)
    {
        theKeyPad.GetUserKeyInData(&m_lLFTrackDevWidth, stMcData.lLeadFrameTrackDevWidth, CUELITE_LF_MIN_WIDTH_SET1,
            CUELITE_LF_MAX_WIDTH_SET1);
        stMcData.lLeadFrameTrackDevWidth = m_lLFTrackDevWidth;

    }
    else
    {
         theKeyPad.GetUserKeyInData(&m_lLFTrackDevWidth, stMcData.lLeadFrameTrackDevWidth, CUELITE_LF_MIN_WIDTH_SET2,
            CUELITE_LF_MAX_WIDTH_SET2);
        stMcData.lLeadFrameTrackDevWidth = m_lLFTrackDevWidth;
    }
	//m_lLFTrackWidthOffset = 
    stMcData.lLeadFrameTrackOffset = stMcData.lLeadFrameTrackSysWidth - stMcData.lLeadFrameTrackDevWidth ;
    theMachineData.StoreMachineDataRcd(&stMcData);
    UpdateData(false);
}
}*/



//v2.18.3.S1 addt   //v2.18.5.w4 no more used
void CCuEliteDeviceOffsetSetupPage::OnBnClickedEditTrackMotor()
{
    // TODO: Add your control notification handler code here
     CUELITE_TRACK_DATA stTrackRcd;
     theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

    long lFrontPos = 0, lRearPos = 0, lFrontRailCurrentPos = 0, lRearRailCurrentPos = 0;
	long lPos,lPosPulse;
    double dRes = 1;
    long lFrontRailOffset = 0, lRearRailOffset = 0;

    /*v2.18.5.w1 delete {
    m_lLFFrontRailCurPos = theCuEliteTrack.getFrontRailDevMoveDist();
    m_lLFRearRailCurPos = theCuEliteTrack.getRearRailDevMoveDist();
    }*/   
 

    if(m_nRailMotorId ==  ID_DEVOFFSET_FRONTRAIL_MOTOR)
    {

	     /*v2.18.5.w1 delete {  -for don't know what is doing !
         CPopUpCuEliteStepperCtr stepDlg(CUELITE_FRONT_RAIL);
         stepDlg.DoModal();
         theStepperControl.CheckStepMotionStatus(CUELITE_FRONT_RAIL, &lFrontPos);
	     dRes = theStepperControl.GetStepResolution(CUELITE_FRONT_RAIL);
         lFrontRailCurrentPos = abs(round(double(lFrontPos *dRes)));
         m_lLFTrackOffset = lFrontRailCurrentPos - theCuEliteTrack.getFrontRailDevMoveDist();
         stMcData.lLFFrontTrackOffset = m_lLFTrackOffset;
         m_lLFFrontRailCurPos =  lFrontRailCurrentPos;		  m_lLFTrackWidthOffset = m_lLFTrackOffset;
         }*/


         //v2.18.5.w1 add {
         CPopUpCuEliteStepperCtr stepDlg(CUELITE_FRONT_RAIL);
		 if( stepDlg.DoModal()==IDOK){
			 theStepperControl.CheckStepMotionStatus(CUELITE_FRONT_RAIL, &lPosPulse);
	         theCuEliteTrack.xPhysicToLogicPos(CUELITE_FRONT_RAIL, lPosPulse,  &lPos);
			 stTrackRcd.lLFFrontTrackStandby=  lPos ;
		 }

         //}
	  
    }
    else
    {
	    /*v2.18.5.w1 delete {  -for don't know what is doing !
        CPopUpCuEliteStepperCtr stepDlg(CUELITE_REAR_RAIL);
        stepDlg.DoModal();
        theStepperControl.CheckStepMotionStatus(CUELITE_REAR_RAIL, &lRearPos);
	    dRes = theStepperControl.GetStepResolution(CUELITE_REAR_RAIL);
	    lRearRailCurrentPos = round(double(lRearPos *dRes));

        m_lLFTrackOffset = lRearRailCurrentPos - theCuEliteTrack.getRearRailDevMoveDist();
        stMcData.lLFRearTrackOffset = m_lLFTrackOffset;
        m_lLFRearRailCurPos = lRearRailCurrentPos;
		 m_lLFTrackWidthOffset = m_lLFTrackOffset ;
		 }*/



         //v2.18.5.w1 add {
         CPopUpCuEliteStepperCtr stepDlg(CUELITE_REAR_RAIL);
		 if( stepDlg.DoModal()==IDOK){
			theStepperControl.CheckStepMotionStatus(CUELITE_REAR_RAIL, &lPos);
			stTrackRcd.lLFRearTrackStandby=  lPos ;
		 }
	     //}



    }
    

	/*v2.18.5.w1 delete {  -for don't know what is doing !
    lFrontRailDist = CUELITE_LF_MIN_WIDTH_SET2 - abs(m_lLFFrontRailCurPos);
    if(stMcData.nLeadFrameTrackType == 0)
    {
        lRearRailDist = 300000 - m_lLFRearRailCurPos;
    }
    else    
    {
        lRearRailDist = CUELITE_LF_MIN_WIDTH_SET2 - m_lLFRearRailCurPos;
    }
    
    m_lLFTrackSysWidth =  stMcData.lLeadFrameTrackSysWidth = lFrontRailDist + lRearRailDist;
    /*m_lLFTrackWidthOffset = *///stMcData.lLeadFrameTrackOffset = stMcData.lLeadFrameTrackSysWidth - stMcData.lLeadFrameTrackDevWidth;

    //v2.18.5.w4  m_lLFTrackWidthOffset=  stMcData.lLFFrontTrackOffset+ stMcData.lLFRearTrackOffset;

    //v2.18.5.w4  m_lLFTrackSysWidth =  stMcData.lLeadFrameTrackSysWidth = stMcData.lLeadFrameTrackDevWidth + m_lLFTrackWidthOffset;

    theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);



    UpdateData(false);

}
//v2.18.3.S1 add    //v2.18.5.w4 no more used
void CCuEliteDeviceOffsetSetupPage::OnBnClickedEditXaxisMotor()
{
       // TODO: Add your control notification handler code here
    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

	long lPos,lPosPulse;
    long  lCurrPos = 0;
    double dXRes = 1;
    long lCurrDist =0;
    long lBeforePos =0 ,lAfterPos ;

    if(m_nElevatorId ==  ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
    {
	    /*v2.18.5.w1 delete {  -for don't know what is doing !
        CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_ONLOAD_XAXIS);
        theStepperCtrler.DoModal();
        dRes = theStepperControl.GetStepResolution(CUELITE_ONLOAD_XAXIS);
        theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_XAXIS, &lPos);

        lCurrPos = round(double(lPos * dRes));
         m_lMagSysLength = CUELITE_MAG_MAX_LENTH - lCurrPos;

        stMcData.lOnLoadMagazineSysLength = m_lMagSysLength;
        m_lMagLengthOffset = stMcData.lOnLoadMagazineLengthOffset = stMcData.lOnLoadMagazineSysLength - stMcData.lOnLoadMagazineDevLength;
         }*/

        //v2.18.5.w1 add {
         CPopUpCuEliteStepperCtr stepDlg(CUELITE_ONLOAD_XAXIS);
		 if( stepDlg.DoModal()==IDOK){
			theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_XAXIS, &lPosPulse);
	        theCuEliteTrack.xPhysicToLogicPos(CUELITE_ONLOAD_XAXIS, lPosPulse,  &lPos);
			stOnloadData.lOnLoadMagazineLengthStandby=  lPos ;
		 }
	    //}



    }
    else
    {
	    /*v2.18.5.w1 delete {  -for don't know what is doing !
        CPopUpCuEliteStepperCtr theStepperCtrler( CUELITE_OFFLOAD_XAXIS);
        theStepperCtrler.DoModal();
        dRes = theStepperControl.GetStepResolution(CUELITE_OFFLOAD_XAXIS);
        theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_XAXIS, &lPos);
        lCurrPos = round(double(lPos * dRes));
         m_lMagSysLength  = CUELITE_MAG_MAX_LENTH - lCurrPos;
        stMcData.lOffLoadMagazineSysLength =  m_lMagSysLength ;
        m_lMagLengthOffset = stMcData.lOffLoadMagazineLengthOffset = stMcData.lOffLoadMagazineSysLength - stMcData.lOffLoadMagazineDevLength ;
        }*/

        //v2.18.5.w1 add {
		 CPopUpCuEliteStepperCtr stepDlg(CUELITE_OFFLOAD_XAXIS);
		 if( stepDlg.DoModal()==IDOK){
			theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_XAXIS, &lAfterPos);
			stOffloadData.lOffLoadMagazineLengthStandby=  lAfterPos;
		 }
	     //}

    }
     theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
     theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);


     UpdateData(false);
}

/*v2.18.5.w4 no more used
//v2.18.3.S1 add   
void CCuEliteDeviceOffsetSetupPage::OnBnClickedEditYaxisMotor()
{
    //v2.18.3.S2 edit
    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
    long lY1Pos = 0, lCurrY1Pos = 0, lY2Pos = 0, lCurrY2Pos = 0;
    double dYRes = 1, dYTwoRes = 1;
    long lBeforePos =0, lAfterPos =0,lAfterPosPulse,lBeforePosPulse ;

 
    m_lOnLoadMagZineYOneCurPos = theCuEliteOnLoader.getOnLoadYOneDevDist();
    m_lOnLoadMagZineYTwoCurPos = theCuEliteOnLoader.getOnLoadYTwoDevDist();
    m_lOffLoadMagZineYOneCurPos = theCuEliteOffLoader.getOffLoadYOneDevDist();
    m_lOffLoadMagZineYTwoCurPos = theCuEliteOffLoader.getOffLoadYTwoDevDist();


    if(m_nElevatorId ==  ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR)
    {
        if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
        {

	    v2.18.5.w1 delete {  -for don't know what is doing !
            theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE ,&lBeforePos);

            CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_ONLOAD_YONE );
            theStepperCtrler.DoModal();
            theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE , &lAfterPos );

            dYRes = theStepperControl.GetStepResolution(CUELITE_ONLOAD_YONE);
             lYOffset= round((double)(lAfterPos-lBeforePos) *dYRes);  //in 0.1um
            stMcData.lOnLoadMagazineYOneOffset=stMcData.lOnLoadMagazineYOneOffset +  lYOffset;

            m_lMagYOneOffset=  stMcData.lOnLoadMagazineYOneOffset;
         }
        //v2.18.5.w1 add {
			CPopUpCuEliteStepperCtr stepDlg(CUELITE_ONLOAD_YONE);
			if( stepDlg.DoModal()==IDOK){
				theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE, &lAfterPos);
				stMcData.lOnLoadMagazineYOneStandby=  lAfterPos ;
			 }
		//}




        }
        else if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
        {
	        v2.18.5.w1 delete {  -for don't know what is doing !
            CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_ONLOAD_YTWO );
            theStepperCtrler.DoModal();
            dY2Res = theStepperControl.GetStepResolution(CUELITE_ONLOAD_YTWO);
            theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO , &lY2Pos );
            lCurrY2Pos = round(double(lY2Pos * dY2Res));

            m_lMagYTwoOffset = lCurrY2Pos - theCuEliteOnLoader.getOnLoadYTwoDevDist();
            m_lMagWidthOffset = stMcData.lOnLoadMagazineYTwoOffset = m_lMagYTwoOffset;
            m_lOnLoadMagZineYTwoCurPos = lCurrY2Pos;

			}


       //v2.18.5.w1 add {
			 CPopUpCuEliteStepperCtr stepDlg(CUELITE_ONLOAD_YTWO);
			 if( stepDlg.DoModal()==IDOK){
				theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lAfterPosPulse);
                theCuEliteOnLoader.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lAfterPosPulse,  &lAfterPos);

				stMcData.lOnLoadMagazineYTwoStandby=  lAfterPos;
			}
		//}


        }
		else if(ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR){

			 CPopUpCuEliteStepperCtr stepDlg(CUELITE_ONLOAD_YONE_YTWO);

			 if( stepDlg.DoModal()==IDOK){
				theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YONE, &lAfterPosPulse);
                theCuEliteOnLoader.xPhysicToLogicPos(CUELITE_ONLOAD_YONE, lAfterPosPulse,  &lAfterPos);
				stMcData.lOnLoadMagazineYOneStandby=  lAfterPos ;

				theStepperControl.CheckStepMotionStatus(CUELITE_ONLOAD_YTWO, &lAfterPosPulse);
                theCuEliteOnLoader.xPhysicToLogicPos(CUELITE_ONLOAD_YTWO, lAfterPosPulse,  &lAfterPos);
				stMcData.lOnLoadMagazineYTwoStandby=  lAfterPos ;
			}

		}

        
       // lY1CurrDist = CUELITE_MAG_MAX_WIDTH - (lCurrY1Pos + lCurrY2Pos);
        //lY2CurrDist = CUELITE_MAG_MAX_WIDTH - lCurrY2Pos;

        m_lMagSysWidth = stMcData.lOnLoadMagazineSysWidth =  CUELITE_MAG_MAX_WIDTH - (m_lOnLoadMagZineYOneCurPos + m_lOnLoadMagZineYTwoCurPos);

        //m_lMagWidthOffset = stMcData.lOnLoadMagazineWidthOffset = stMcData.lOnLoadMagazineSysWidth - stMcData.lOnLoadMagazineDevWidth ;

       //v2.18.5.w4 m_lMagWidthOffset=  stMcData.lOnLoadMagazineYOneOffset+ stMcData.lOnLoadMagazineYTwoOffset;

  



    }
    else
    {
        if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR)
        {
	       *v2.18.5.w1 delete {  -for don't know what is doing !
            CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_OFFLOAD_YONE );
            theStepperCtrler.DoModal();
            dYOneRes = theStepperControl.GetStepResolution(CUELITE_OFFLOAD_YONE);
            theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE , &lY1Pos );
            lCurrY1Pos = abs(round(double(lY1Pos * dYOneRes)));

            m_lMagYOneOffset = lCurrY1Pos - theCuEliteOnLoader.getOnLoadYOneDevDist();
            m_lMagWidthOffset = stMcData.lOffLoadMagazineYOneOffset = m_lMagYOneOffset;
            m_lOffLoadMagZineYOneCurPos = lCurrY1Pos;
            }*

         //v2.18.5.w1 add {
			CPopUpCuEliteStepperCtr stepDlg(CUELITE_OFFLOAD_YONE );
			if( stepDlg.DoModal()==IDOK){
				theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE , &lAfterPos);
				stMcData.lOffLoadMagazineYOneStandby=  lAfterPos ;
			}
		//}
        }
        else if(m_nStepMotorId == ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR)
        {   
	       *v2.18.5.w1 delete {  -for don't know what is doing !
            CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_OFFLOAD_YTWO );
            theStepperCtrler.DoModal();
            dYTwoRes = theStepperControl.GetStepResolution(CUELITE_OFFLOAD_YTWO);
            theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO , &lY2Pos );
            lCurrY2Pos = round(double(lY2Pos * dYTwoRes));

            m_lMagYOneOffset = lCurrY2Pos - theCuEliteOnLoader.getOnLoadYOneDevDist();
            m_lMagWidthOffset = stMcData.lOffLoadMagazineYTwoOffset = m_lMagYOneOffset;
            m_lOffLoadMagZineYTwoCurPos = lCurrY2Pos;
           }*


        //v2.18.5.w1 add {
		//	CPopUpCuEliteStepperCtr stepDlg(CUELITE_OFFLOAD_YTWO );
		//	if( stepDlg.DoModal()==IDOK){
		//		theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lAfterPos);
		//		stMcData.lOffLoadMagazineYTwoStandby=  lAfterPos ;
		//	}
		//}

        }   
		else if(ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR){

			 CPopUpCuEliteStepperCtr stepDlg(CUELITE_OFFLOAD_YONE_YTWO);
			 if( stepDlg.DoModal()==IDOK){
				 theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YONE, &lAfterPos);
				 stMcData.lOffLoadMagazineYOneStandby=  lAfterPos ;
				 theStepperControl.CheckStepMotionStatus(CUELITE_OFFLOAD_YTWO, &lAfterPos);
				 stMcData.lOffLoadMagazineYTwoStandby=  lAfterPos ;
			 }

		}
     
        *lY1CurrDist = CUELITE_MAG_MAX_WIDTH - lCurrY1Pos;
        lY2CurrDist = CUELITE_MAG_MAX_WIDTH - lCurrY2Pos;*
        m_lMagSysWidth = stMcData.lOffLoadMagazineSysWidth =  CUELITE_MAG_MAX_WIDTH - (m_lOffLoadMagZineYOneCurPos + m_lOffLoadMagZineYTwoCurPos);
       //m_lMagWidthOffset = stMcData.lOffLoadMagazineWidthOffset = stMcData.lOffLoadMagazineSysWidth - stMcData.lOffLoadMagazineDevWidth;

       //v2.18.5.w4 m_lMagWidthOffset=  stMcData.lOffLoadMagazineYTwoOffset+ stMcData.lOffLoadMagazineYTwoOffset;



    }
	theMachineData.StoreMachineDataRcd(&stMcData);

     UpdateData(false);

}
}*/


void CCuEliteDeviceOffsetSetupPage::OnEnSetfocusEditFile3()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	if( m_sID.CompareNoCase( ID_CUELITE)==0)
	{

	  CString str;
	  CStringList szStringList;

      theMsgMgr.GetMsgString(MSG_MENU_ITEM_STANDBY_POS, str);
	  szStringList.AddTail(str);
      int iRet=theMsgMgr.ShowMessageX(WARNING_MSG, szStringList,0,0,2);
	  if( iRet==SHOW_MSG_RESULT_OK)       loadStandbyFromID_CUELITEini();
      else if(iRet==SHOW_MSG_RESULT_STOP) saveStandbyToID_CUELITEini();
	  else {};

	}

}
void CCuEliteDeviceOffsetSetupPage::loadStandbyFromID_CUELITEini()
{       

    CString s_ID;
	long lValue;

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
 
    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

    CUELITE_STEPPER_CLAMP_PARA  stCuElitePosPara;
    theCuEliteLFClamp.GetClampRcd(&stCuElitePosPara);

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);


 
	// TODO: Add your control notification handler code here
	if(! m_newFile.save(false)){

		m_newFile.getTitle(&s_ID);

		if( s_ID.CompareNoCase( ID_CUELITE)==0){

			CString str;
			CStringList szStringList;
			theMsgMgr.GetMsgString(MSG_MENU_ITEM_CANNOT_LOAD_FROM_CUELITE, str);
			szStringList.AddTail(str);
			theMsgMgr.ShowMessage(WARNING_MSG, szStringList);

		}
		else {

		    m_newFile.get(_T("motor standby position"),_T("onload elevator"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_ELEVATOR,lValue,&stOnloadData.lOnload1stSlotLevel);

            m_newFile.get(_T("motor standby position"),_T("onload side clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_XAXIS,lValue,&stOnloadData.lOnLoadMagazineLengthStandby);
		    m_newFile.get(_T("motor standby position"),_T("onload rear clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YONE,lValue,&stOnloadData.lOnLoadMagazineYOneStandby);
            m_newFile.get(_T("motor standby position"),_T("onload front clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YTWO,lValue,&stOnloadData.lOnLoadMagazineYTwoStandby);
            m_newFile.get(_T("motor standby position"),_T("onload clamp depth"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_ONLOAD_YTWO,lValue,&stOnloadData.lOnLoadMagClamperDepth);

		    m_newFile.get(_T("motor standby position"),_T("front rail"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_FRONT_RAIL,lValue,&stTrackRcd.lLFFrontTrackStandby);
            m_newFile.get(_T("motor standby position"),_T("window clamper"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_WINDOW_CLAMPER,lValue,&stCuElitePosPara.lWindClampPreClosePos);
		    m_newFile.get(_T("motor standby position"),_T("indexer"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_DEVICE_INDEXER,lValue,&stMcData.lZeroOffset);
            m_newFile.get(_T("motor standby position"),_T("rear rail"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_REAR_RAIL,lValue,&stTrackRcd.lLFRearTrackStandby);

		    m_newFile.get(_T("motor standby position"),_T("offload elevator"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_ELEVATOR,lValue,&stOffloadData.lOffload1stSlotLevel);

            m_newFile.get(_T("motor standby position"),_T("offload side clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_XAXIS,lValue,&stOffloadData.lOffLoadMagazineLengthStandby);
		    m_newFile.get(_T("motor standby position"),_T("offload rear clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YONE,lValue,&stOffloadData.lOffLoadMagazineYOneStandby);
            m_newFile.get(_T("motor standby position"),_T("offload front clamp"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YTWO,lValue,&stOffloadData.lOffLoadMagazineYTwoStandby);
            m_newFile.get(_T("motor standby position"),_T("offload clamp depth"),&lValue);
		    theCuEliteTrack.sUIToLogic(CUELITE_OFFLOAD_YTWO,lValue,&stOffloadData.lOffLoadMagClamperDepth);

		}

      theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);
      theCuEliteLFClamp.UpdateClampRcd(&stCuElitePosPara);
      theCuEliteOnLoader.UpdateCuEliteOnloadRcd(&stOnloadData); 
      theCuEliteOffLoader.UpdateCuEliteOffloadRcd(&stOffloadData);
      theMachineData.StoreMachineDataRcd(&stMcData);

	}

		UpdateData(false);
}
void CCuEliteDeviceOffsetSetupPage::saveStandbyToID_CUELITEini()
{
    CString s_ID;
	long lValue;

    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);

    CUELITE_ONLOAD_RCD  stOnloadData;
    theCuEliteOnLoader.GetCuEliteOnloadRcd(&stOnloadData);

    CUELITE_OFFLOAD_RCD  stOffloadData;
    theCuEliteOffLoader.GetCuEliteOffloadRcd(&stOffloadData);

    CUELITE_STEPPER_CLAMP_PARA  stCuElitePosPara;
    theCuEliteLFClamp.GetClampRcd(&stCuElitePosPara);

    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

	// TODO: Add your control notification handler code here
    if(! m_newFile.save(true)){

		m_newFile.getTitle(&s_ID);

		if( s_ID.CompareNoCase( ID_CUELITE)==0){

			CString str;
			CStringList szStringList;
			theMsgMgr.GetMsgString(MSG_MENU_ITEM_CANNOT_SAVE_TO_CUELITE, str);
			szStringList.AddTail(str);
			theMsgMgr.ShowMessage(WARNING_MSG, szStringList);
		}
		else{

	
			theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_ELEVATOR,stOnloadData.lOnload1stSlotLevel, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("onload elevator"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_XAXIS,stOnloadData.lOnLoadMagazineLengthStandby, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("onload side clamp"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YONE,stOnloadData.lOnLoadMagazineYOneStandby, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("onload rear clamp"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagazineYTwoStandby, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("onload front clamp"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOnloadData.lOnLoadMagClamperDepth, &lValue);
			m_newFile.set(_T("motor standby position"),_T("onload clamp depth"),lValue);

			theCuEliteTrack.sLogicToUI(CUELITE_FRONT_RAIL,stTrackRcd.lLFFrontTrackStandby, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("front rail"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_WINDOW_CLAMPER,stCuElitePosPara.lWindClampPreClosePos, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("window clamper"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_DEVICE_INDEXER,stMcData.lZeroOffset, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("indexer"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_REAR_RAIL,stTrackRcd.lLFRearTrackStandby, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("rear rail"),lValue);


			theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_ELEVATOR,stOffloadData.lOffload1stSlotLevel, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("offload elevator"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_XAXIS,stOffloadData.lOffLoadMagazineLengthStandby, &lValue);
			m_newFile.set(_T("motor standby position"),_T("offload side clamp"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YONE,stOffloadData.lOffLoadMagazineYOneStandby, &lValue);		
			m_newFile.set(_T("motor standby position"),_T("offload rear clamp"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_OFFLOAD_YTWO,stOffloadData.lOffLoadMagazineYTwoStandby, &lValue);	
			m_newFile.set(_T("motor standby position"),_T("offload front clamp"),lValue);
			theCuEliteTrack.sLogicToUI(CUELITE_ONLOAD_YTWO,stOffloadData.lOffLoadMagClamperDepth, &lValue);
			m_newFile.set(_T("motor standby position"),_T("offload clamp depth"),lValue);

		}
	}

}

void CCuEliteDeviceOffsetSetupPage::setRefFile(void){

        CString s_ID,sPath;

	    sPath=WHS_FILE_PATH ;
		sPath=sPath+ ID_CUELITE+_T(".ini") ;
		m_refFile.setPathTitleExt(sPath );
}

void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonLoadDatabase()
{
	// TODO: Add your control notification handler code here
}


void CCuEliteDeviceOffsetSetupPage::OnBnClickedButtonInite2()
{
	// TODO: Add your control notification handler code here

	    theMaterialHandlerThread.WarmResetMaterialHandler();


}
