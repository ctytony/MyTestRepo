// TopDeviceParaPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "CuEliteDeviceParaPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TeachCuEliteWorkHolderSheet.h"


IMPLEMENT_DYNAMIC(CCuEliteDeviceParaPage, CPropertyPage)

CCuEliteDeviceParaPage::CCuEliteDeviceParaPage()
	: CPropertyPage(CCuEliteDeviceParaPage::IDD)
	, m_lMag1stLvHt(0)
	, m_lMagLvPitch(0)
	, m_lMagSlotNum(0)
    , m_lMagLength(200000)
    , m_lMagWidth(1000)
	, m_nDeviceRowNum(0)
	, m_nDeviceColmNum(0)
	, m_lDeviceColmPitch(0)
    , m_lSlipWidth(1000)
    , m_lSlipLength(0)
	, m_nWndUnitNum(0)
	, m_lLastSlopToTopH(0)
	, m_iSlipListId(0)
	, m_nLastHalfIndex(0)
	, m_nDeviceId(0)
{

}

CCuEliteDeviceParaPage::~CCuEliteDeviceParaPage()
{
}

void CCuEliteDeviceParaPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_MAG_WIDTH, m_lMagWidth);
    DDX_Text(pDX, IDC_EDIT_MAG_LENGTH, m_lMagLength);
	DDX_Text(pDX, IDC_EDIT_MAG_BOTM_HEIGHT, m_lMag1stLvHt);
	DDX_Text(pDX, IDC_EDIT_MAG_SLOT_HEIGHT, m_lMagLvPitch);
	DDX_Text(pDX, IDC_EDIT_MAG_SLOT_NUM, m_lMagSlotNum);
	DDX_Text(pDX, IDC_EDIT_SLIP_ROW_NUM, m_nDeviceRowNum);
	DDX_Text(pDX, IDC_EDIT_SLIP_COLUM_NUM, m_nDeviceColmNum);
	//DDX_Text(pDX, IDC_EDIT_SLIP_1ST_TO_EDGE_LEN, m_lDeviceEdgeLen);
	DDX_Text(pDX, IDC_EDIT_SLIP_PITCH, m_lDeviceColmPitch);
	DDX_Text(pDX, IDC_EDIT_WND_COLUM_NUM, m_nWndUnitNum);
	DDX_Text(pDX, IDC_EDIT_MAG_TOP_HEIGHT, m_lLastSlopToTopH);
	DDX_LBIndex(pDX, IDC_LIST_DEVICE_DATABASE, m_iSlipListId);
	DDX_Text(pDX, IDC_EDIT_SLIP_LEN, m_lSlipLength);
    DDX_Text(pDX, IDC_EDIT_SLIP_WIDTH, m_lSlipWidth);
}


BEGIN_MESSAGE_MAP(CCuEliteDeviceParaPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_MAG_BOTM_HEIGHT, &CCuEliteDeviceParaPage::OnEnSetfocusEditMagBotmHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_HEIGHT, &CCuEliteDeviceParaPage::OnEnSetfocusEditMagSlotHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_NUM, &CCuEliteDeviceParaPage::OnEnSetfocusEditMagSlotNum)
    ON_EN_SETFOCUS(IDC_EDIT_MAG_WIDTH, &CCuEliteDeviceParaPage::OnEnSetfocusEditMagWidth)
    ON_EN_SETFOCUS(IDC_EDIT_MAG_LENGTH, &CCuEliteDeviceParaPage::OnEnSetfocusEditMagLength)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_ROW_NUM, &CCuEliteDeviceParaPage::OnEnSetfocusEditSlipRowNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_COLUM_NUM, &CCuEliteDeviceParaPage::OnEnSetfocusEditSlipColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_PITCH, &CCuEliteDeviceParaPage::OnEnSetfocusEditSlipPitch)
	ON_EN_SETFOCUS(IDC_EDIT_WND_COLUM_NUM, &CCuEliteDeviceParaPage::OnEnSetfocusEditWndColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_TOP_HEIGHT, &CCuEliteDeviceParaPage::OnEnSetfocusEditMagTopHeight)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATABASE, &CCuEliteDeviceParaPage::OnBnClickedButtonLoadDatabase)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_DATABASE, &CCuEliteDeviceParaPage::OnLbnSelchangeListDeviceDatabase)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_LEN, &CCuEliteDeviceParaPage::OnEnSetfocusEditSlipLen)
    ON_EN_SETFOCUS(IDC_EDIT_SLIP_WIDTH, &CCuEliteDeviceParaPage::OnEnSetfocusEditSlipWidth)

END_MESSAGE_MAP()


// CCuEliteDeviceParaPage message handlers

BOOL CCuEliteDeviceParaPage::OnInitDialog()
{

	CTeachCuEliteWorkHolderSheet* psheet = (CTeachCuEliteWorkHolderSheet*)GetParent();


	CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        pChildWnd->SetFont(theApp.m_pCurrentFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

    CString szString;


	 CWnd* pWnd = GetDlgItem(IDC_STATIC_DATABASE);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_DEVICE_DATABASE, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_APPLY, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_MAGAZINE);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAGAZINE, szString);
     pWnd->SetWindowTextW(szString);

     pWnd = GetDlgItem(IDC_STATIC_MAG_WIDTH);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_WIDTH, szString);
     pWnd->SetWindowTextW(szString);

      pWnd = GetDlgItem(IDC_STATIC_MAG_LENGTH);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_LENGTH, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_BOTTOM_TO_1ST);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_BOTOM_TO_1ST, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_LAST_TO_TOP);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_MAG_LAST_TO_TOP, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_SLOT_HEIGHT);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_SLOT_PITCH, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_SLOT_NUMBER);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_SLOT_NUM, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_LEADFRAME);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_LEAD_FRAME, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_LF_ROW);
	 theMsgMgr.GetMsgString(MSG_ROW, szString);
	 szString.AppendFormat(_T(":"));
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_LF_COLUM);
	 theMsgMgr.GetMsgString(MSG_COLUMN, szString);
	 szString.AppendFormat(_T(":"));
     pWnd->SetWindowTextW(szString);

	
	  pWnd = GetDlgItem(IDC_STATIC_PITCH);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_LEADFRAME_PITCH, szString);
     pWnd->SetWindowTextW(szString);

	  pWnd = GetDlgItem(IDC_STATIC_WND_CLAMP);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_WINDOW_CLAMP_COLUMN, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_LENGTH);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_DEVICE_TOTAL_LEN, szString);
	 pWnd->SetWindowTextW(szString);
     
     pWnd = GetDlgItem( IDC_STATIC_SLIP_WIDTH);
	 theMsgMgr.GetMsgString( MSG_MENU_ITEM_DEVICE_TOTAL_WIDTH, szString);
	 pWnd->SetWindowTextW(szString);
   

     pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
	 pWnd->EnableWindow(FALSE);
     pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_SLIP_ROW_NUM);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_NUM);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
	 pWnd->EnableWindow(FALSE);
	 pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
	 pWnd->EnableWindow(FALSE);
     pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH);
	 pWnd->EnableWindow(FALSE);
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_DEVICE_DATABASE);
    pListBox->ResetContent();
    
    pListBox->InsertString(0, _T("5R  * 3C  LF"));
	pListBox->InsertString(1, _T("14R * 4C  LF"));
	pListBox->InsertString(2, _T("10R * 3C  LF"));
	pListBox->InsertString(3, _T("10R * 6C  LF"));
	pListBox->InsertString(4, _T("10W High Power"));
	pListBox->InsertString(5, _T("14R * 5C"));
	pListBox->InsertString(6, _T("5050 (3 die)"));
	pListBox->InsertString(7, _T("14R * 6C"));
	pListBox->InsertString(8, _T("Discret 0603"));
	pListBox->InsertString(9, _T("100W High Power"));
	pListBox->InsertString(10,_T("PCB-LONG"));
	pListBox->InsertString(11,_T("Discret SOP"));
  	pListBox->InsertString(12, _T("Self-define(×Ô¶¨Òå)"));

	
    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

	m_lMag1stLvHt      = stCuEliteDevicePara.lMag1stLvHt;
	m_lMagLvPitch      = stCuEliteDevicePara.lMagLvPitch;   
	m_lMagSlotNum      = stCuEliteDevicePara.lMagSlotNum;  
    m_lMagWidth        = stCuEliteDevicePara.lMagWidth; 
    m_lMagLength        = stCuEliteDevicePara.lMagLength; 

	m_nDeviceRowNum    = stCuEliteDevicePara.nDeviceRowNum; 
	m_nDeviceColmNum   = stCuEliteDevicePara.nDeviceColmNum;
	m_lDeviceColmPitch = stCuEliteDevicePara.lDeviceColmPitch;
	m_nWndUnitNum      = stCuEliteDevicePara.nWndUnitNum;
	m_lLastSlopToTopH  = stCuEliteDevicePara.lMagLastLvHt;
	m_nLastHalfIndex  =  stCuEliteDevicePara.nHalfIndexOn;
	m_lSlipLength     =  stCuEliteDevicePara.lDeviceTotalLen;
    m_lSlipWidth     =  stCuEliteDevicePara.lDeviceTotalWidth;

	m_nDeviceId =    stCuEliteDevicePara.nDeviceId;


	CPropertyPage::OnInitDialog();

	

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCuEliteDeviceParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CCuEliteDeviceParaPage::OnEnSetfocusEditMagBotmHeight()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMag1stLvHt);

    UpdateData(false);

    
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditMagSlotHeight()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMagLvPitch);

    UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditMagSlotNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMagSlotNum);

    UpdateData(false);
}
void CCuEliteDeviceParaPage::OnEnSetfocusEditMagWidth()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);
    theKeyPad.GetUserKeyInData(&m_lMagWidth, stCuEliteDevicePara.lMagWidth, CUELITE_MAG_MIN_WIDTH, 
        CUELITE_MAG_MAX_WIDTH);

    UpdateData(false);
}
void CCuEliteDeviceParaPage::OnEnSetfocusEditMagLength()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();

    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);
    theKeyPad.GetUserKeyInData(&m_lMagLength, stCuEliteDevicePara.lMagLength, CUELITE_MAG_MINI_LENTH,
        CUELITE_MAG_MAX_LENTH);

    UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditSlipRowNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nDeviceRowNum);

    UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditSlipColumNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nDeviceColmNum);

    UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditSlipPitch()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lDeviceColmPitch);

    UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditWndColumNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nWndUnitNum);

    UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditMagTopHeight()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lLastSlopToTopH);

    UpdateData(false);

	
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditSlipLen()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lSlipLength);

	UpdateData(false);
	
}

void CCuEliteDeviceParaPage::OnEnSetfocusEditSlipWidth()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

    //theKeyPad.GetUserKeyInData(&m_lSlipWidth); //V2.18.2.S9 delete
    //V2.18.2.S9 add
    MACHINE_DATA stMcData;
    theMachineData.GetMachineDataRcd(&stMcData);
    CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);
    
    if(stMcData.nLeadFrameTrackType == CUELITE_REAR_RAIL_NARROW )
	    theKeyPad.GetUserKeyInData(&m_lSlipWidth, stCuEliteDevicePara.lDeviceTotalWidth, CUELITE_LF_MIN_WIDTH_SET1, CUELITE_LF_MAX_WIDTH_SET1);
    else
        theKeyPad.GetUserKeyInData(&m_lSlipWidth, stCuEliteDevicePara.lDeviceTotalWidth, CUELITE_LF_MIN_WIDTH_SET2, CUELITE_LF_MAX_WIDTH_SET2);		//v2.18.3.S1 edit
    //theKeyPad.GetUserKeyInData(&m_lSlipWidth, stCuEliteDevicePara.lDeviceTotalWidth, CUELITE_LF_MIN_WIDTH_SET2, CUELITE_LF_MAX_WIDTH_SET2);		//v2.18.3.S1 edit
    stCuEliteDevicePara.lDeviceTotalWidth = m_lSlipWidth;
    theDeviceDataMgr.StoreCuEliteDevicePara(&stCuEliteDevicePara);


	UpdateData(false);
	
}

void CCuEliteDeviceParaPage::OnBnClickedButtonLoadDatabase()
{
	// TODO: Add your control notification handler code here
    short   nMsgNum, anMsgArray[10]; 
	UpdateData(TRUE);

	short index[10];
	index[0]=MSG_MENU_ITEM_WARNING_CHANGE_DEVICE;
	if(theMsgMgr.ShowMessage(HELP_MSG,1,index,true)==SHOW_MSG_RESULT_OK)  
	{


		CUELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	    theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

		stCuEliteDevicePara.nDeviceId         = m_nDeviceId;
		stCuEliteDevicePara.lMag1stLvHt       = m_lMag1stLvHt;
		stCuEliteDevicePara.lMagLvPitch       = m_lMagLvPitch;
		stCuEliteDevicePara.lMagSlotNum       = m_lMagSlotNum;
        stCuEliteDevicePara.lMagWidth         = m_lMagWidth;
        stCuEliteDevicePara.lMagLength        = m_lMagLength;


		stCuEliteDevicePara.nDeviceRowNum    = m_nDeviceRowNum;
		stCuEliteDevicePara.nDeviceColmNum   = m_nDeviceColmNum;
		stCuEliteDevicePara.lDeviceColmPitch = m_lDeviceColmPitch;
		//stCuEliteDevicePara.lDevice  EdgeLen   = m_lDeviceColmPitch/2;
        stCuEliteDevicePara.lDeviceEdgeLen   = (m_lSlipLength - m_lDeviceColmPitch * m_nDeviceColmNum) / 2;

		stCuEliteDevicePara.nWndUnitNum      = m_nWndUnitNum;
		stCuEliteDevicePara.lMagLastLvHt     = m_lLastSlopToTopH;
		stCuEliteDevicePara.nHalfIndexOn     = m_nLastHalfIndex;

		stCuEliteDevicePara.lDeviceTotalLen  = m_lSlipLength;
        stCuEliteDevicePara.lDeviceTotalWidth = m_lSlipWidth;
		theDeviceDataMgr.StoreCuEliteDevicePara(&stCuEliteDevicePara);

       /* theCuEliteOnLoader.theForkRemoveDetectedSensor.UpdateSensorState();
        theCuEliteOnLoader.theFork1PresentSensor.UpdateSensorState();
        theCuEliteOnLoader.theFork2PresentSensor.UpdateSensorState();
        theCuEliteOffLoader.theForkRemoveDetectedSensor.UpdateSensorState();
        theCuEliteOffLoader.theFork1PresentSensor.UpdateSensorState();
        theCuEliteOffLoader.theFork2PresentSensor.UpdateSensorState();
        if(stCuEliteDevicePara.lMagLength >= CUELITE_MAG_MINI_LENTH && stCuEliteDevicePara.lMagLength <= CUELITE_MAG_MEDIUM_LENTH)
        {
            if( theCuEliteOnLoader.theForkRemoveDetectedSensor.GetSensorState() == SENSOR_OFF ||
                theCuEliteOffLoader.theForkRemoveDetectedSensor.GetSensorState() == SENSOR_OFF)
            {
			    nMsgNum = 2;
			    anMsgArray[0] = MSG_MENU_ITEM_FORK_DETECTED;
                anMsgArray[1] = MSG_MENU_ITEM_FORK_DETECTED;
			    theMsgMgr.ShowMessage(WARNING_MSG, nMsgNum, anMsgArray);
            }
        }
        else if(stCuEliteDevicePara.lMagLength >= CUELITE_MAG_MEDIUM_LENTH  && stCuEliteDevicePara.lMagLength <= CUELITE_MAG_MEDIUM2_LENTH)
        {
            if(theCuEliteOnLoader.theFork1PresentSensor.GetSensorState() == SENSOR_ON ||
                theCuEliteOffLoader.theFork1PresentSensor.GetSensorState() == SENSOR_ON ||
                theCuEliteOnLoader.theFork2PresentSensor.GetSensorState() == SENSOR_ON ||
                theCuEliteOffLoader.theFork2PresentSensor.GetSensorState() == SENSOR_ON)
            {
                nMsgNum = 2;
			    anMsgArray[0] = MSG_MENU_ITEM_FORK_DETECTED;
                anMsgArray[1] = MSG_MENU_ITEM_FORK_DETECTED;
			    theMsgMgr.ShowMessage(WARNING_MSG, nMsgNum, anMsgArray);
            }
        }
        else if(stCuEliteDevicePara.lMagLength >= CUELITE_MAG_MEDIUM2_LENTH &&  stCuEliteDevicePara.lMagLength  <= CUELITE_MAG_MAX_LENTH)
        {
            if(theCuEliteOnLoader.theFork2PresentSensor.GetSensorState() == SENSOR_ON ||
                theCuEliteOffLoader.theFork2PresentSensor.GetSensorState() == SENSOR_ON)
            {
                nMsgNum = 2;
			    anMsgArray[0] = MSG_MENU_ITEM_FORK_DETECTED;
                anMsgArray[1] = MSG_MENU_ITEM_FORK_DETECTED;
			    theMsgMgr.ShowMessage(WARNING_MSG, nMsgNum, anMsgArray);
            }
        }*/

		theCuEliteTrack.RefreshMeasurementData(&stCuEliteDevicePara); // add later

		theLog.WriteLine(_T("User Choose and updated a new device"));
		//v2.18.3.S1 add
       // adjust magzine holer and track rails
        theCuEliteOnLoader.theMagDetectedSensor.UpdateSensorState(); 
        theCuEliteOffLoader.theMagDetectedSensor.UpdateSensorState();
        if(theCuEliteOnLoader.theMagDetectedSensor.GetSensorState() == SENSOR_ON ||
			theCuEliteOffLoader.theMagDetectedSensor.GetSensorState() == SENSOR_ON)
        {
            nMsgNum = 2;
		    anMsgArray[0] = MSG_MENU_MSG_REMOVE_MAGS;
		    anMsgArray[1] = MSG_MENU_MSG_REMOVE_MAGS_EN;
		    theMsgMgr.ShowMessage(WARNING_MSG, nMsgNum, anMsgArray);
        }
        theCuEliteOnLoader.adjustMagzineHolderLength();
        Sleep(10);
        theCuEliteOffLoader.adjustMagzineHolderLength();
        Sleep(10);
        theCuEliteOnLoader.adjustMagzineHolderWidth();
        Sleep(10);
        theCuEliteOffLoader.adjustMagzineHolderWidth();
        Sleep(100);
        theCuEliteTrack.AdjustTrackWidth();
        Sleep(100); //may remove later
        
		
		CTeachCuEliteWorkHolderSheet* psheet = (CTeachCuEliteWorkHolderSheet*)GetParent();

		if (psheet->m_iPageMode == WH_CUELITE_TEACH)
		{
			CWnd* pWnd = psheet->GetDlgItem(ID_WIZNEXT);
			pWnd->EnableWindow(1);
		}
		else
		{

			CWnd* pWnd = psheet->GetDlgItem(IDOK);
			pWnd->EnableWindow(1);
		}
	}


}

void CCuEliteDeviceParaPage::OnLbnSelchangeListDeviceDatabase()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	CWnd* pWnd;

	HideInputDialog();

   	switch(m_iSlipListId)
	{
	  case ID_5R3C_LF:
		  {
			  m_nDeviceId =ID_5R3C_LF;

              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 8590;
			  m_lMagLvPitch = 4404;
			  m_lMagSlotNum = 25;
			  m_lLastSlopToTopH = 13030;

			  m_nDeviceRowNum =5;
			  m_nDeviceColmNum =12;
			  //m_lDeviceEdgeLen =5880;
			  m_lDeviceColmPitch =11770;
			  m_nWndUnitNum = 3;

			  m_nLastHalfIndex = 0;


			   m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_14R4C_LF:
		  {
			  m_nDeviceId =ID_14R4C_LF;
            
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 14960;
			  m_lMagLvPitch = 5184;
			  m_lLastSlopToTopH = 9340;
			  m_lMagSlotNum = 20;

			  m_nDeviceRowNum =14;
			  m_nDeviceColmNum =20;
			  //m_lDeviceEdgeLen =3570;
			  m_lDeviceColmPitch =7440;
			  m_nWndUnitNum = 4;

			  m_nLastHalfIndex = 0;

			  m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_10R3C_LF:
		  {
			  m_nDeviceId =ID_10R3C_LF;

              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 8590;
			  m_lMagLvPitch = 4404;
			  m_lMagSlotNum = 25;
			  m_lLastSlopToTopH = 13030;

			  m_nDeviceRowNum =10;
			  m_nDeviceColmNum =18;
			  //m_lDeviceEdgeLen =3920;
			  m_lDeviceColmPitch =7610;
			  m_nWndUnitNum = 3;

			  m_nLastHalfIndex = 0;

			   m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_10R6C_LF:
		  {
			  m_nDeviceId =ID_10R6C_LF;

              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 8590;
			  m_lMagLvPitch = 4404;
			  m_lMagSlotNum = 25;
			  m_lLastSlopToTopH = 13030;

			  m_nDeviceRowNum =10;
			  m_nDeviceColmNum =18;
			  //m_lDeviceEdgeLen =3920;
			  m_lDeviceColmPitch =7800;
			  m_nWndUnitNum = 6;

			  m_nLastHalfIndex = 0;

			   m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_10W_HIGHPWR:
		  {
			  m_nDeviceId =ID_10W_HIGHPWR;

              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 13000;
			  m_lMagLvPitch = 7000;
			  m_lMagSlotNum = 15;
			  m_lLastSlopToTopH = 15000;

			  m_nDeviceRowNum =2;
			  m_nDeviceColmNum =10;
			  //m_lDeviceEdgeLen =6060;
			  m_lDeviceColmPitch =12250;
			  m_nWndUnitNum = 4;

			  m_nLastHalfIndex = 3;

			   m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;


		  }
		  break;

	  case ID_14R5C_LF:
		  {
			  m_nDeviceId =ID_14R5C_LF;
            
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 15480;
			  m_lMagLvPitch = 5105;
			  m_lMagSlotNum = 20;
			  m_lLastSlopToTopH = 15000;

			  m_nDeviceRowNum =14;
			  m_nDeviceColmNum =20;
			  //m_lDeviceEdgeLen =3640;
			  m_lDeviceColmPitch =7384;
			  m_nWndUnitNum = 5;

			  m_nLastHalfIndex = 0;

			   m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_5050_LF:
		  {
			  m_nDeviceId =ID_5050_LF;

              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 14450;
			  m_lMagLvPitch = 4995;
			  m_lMagSlotNum = 25;
			  m_lLastSlopToTopH = 4190;

			  m_nDeviceRowNum =8;
			  m_nDeviceColmNum =16;

			  m_lDeviceColmPitch =9551;
			  m_nWndUnitNum = 4;

			  m_nLastHalfIndex = 0;

			   m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_14R6C_LF:
		  {
			  m_nDeviceId =ID_14R6C_LF;
               
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 14450;
			  m_lMagLvPitch = 4995;
			  m_lMagSlotNum = 25;
			  m_lLastSlopToTopH = 4190;

			  m_nDeviceRowNum =14;
			  m_nDeviceColmNum =24;

			  m_lDeviceColmPitch =5973;
			  m_nWndUnitNum = 6;

			  m_nLastHalfIndex = 0;

			   m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_DIC_0603:  //17 times index
		  {
			  m_nDeviceId =ID_DIC_0603;

              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 14450;
			  m_lMagLvPitch = 4995;
			  m_lMagSlotNum = 25;
			  m_lLastSlopToTopH = 4190;

			  m_nDeviceRowNum =46;
			  m_nDeviceColmNum =68;

			  m_lDeviceColmPitch =1698;
			  m_nWndUnitNum = 4;

			  m_nLastHalfIndex = 0;

			  m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		  }
		  break;

	  case ID_100W_HIGHPWR:
		  {
			  m_nDeviceId =ID_100W_HIGHPWR;
                
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 14450;
			  m_lMagLvPitch = 4995;
			  m_lMagSlotNum = 25;
			  m_lLastSlopToTopH = 4190;

			  m_nDeviceRowNum =1;
			  m_nDeviceColmNum =2;

			  m_lDeviceColmPitch =62000;
			  m_lSlipLength = 124000;
			  m_nWndUnitNum = 1;

			  m_nLastHalfIndex = 0;
		  }
		  break;

	  case ID_PCB_LONG:
		  {
			  m_nDeviceId =ID_PCB_LONG;


			  pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
			  pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
			  pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);


			  m_lMag1stLvHt = 14450;
			  m_lMagLvPitch = 4995;
			  m_lMagSlotNum = 15;
			  m_lLastSlopToTopH = 4190;

			  m_nDeviceRowNum =5;
			  m_nDeviceColmNum =176;

			  m_lDeviceColmPitch =3314;
			  m_lSlipLength = 580000;
			  m_nWndUnitNum = 16;

			  m_nLastHalfIndex = 0;
		  }
		  break;

      case ID_SOP16:
		  {
			  m_nDeviceId =ID_SOP16;


			  pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
			  pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);

			  m_lMag1stLvHt = 12700;
			  m_lMagLvPitch = 2540;
			  m_lMagSlotNum = 40;
			  m_lLastSlopToTopH = 1270;

			  m_nDeviceRowNum =5;
			  m_nDeviceColmNum =14;

			  m_lDeviceColmPitch =16307;
			  m_lSlipLength = 228295;
			  m_nWndUnitNum = 2;

			  m_nLastHalfIndex = 0;

		  }
		  break;

	  default:
		  {

			  m_nDeviceId =255;


			  pWnd = GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_ROW_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
			  pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_WIDTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_MAG_LENGTH);
              pWnd->EnableWindow(TRUE);
              pWnd = GetDlgItem(IDC_EDIT_SLIP_WIDTH); 
              pWnd->EnableWindow(TRUE);
                
			  
			  m_nLastHalfIndex = 0;

			  m_lMag1stLvHt = 15480;
			  m_lMagLvPitch = 5105;
			  m_lMagSlotNum = 20;
			  m_lLastSlopToTopH = 15000;

			  m_nDeviceRowNum =10;
			  m_nDeviceColmNum =16;

			  m_lDeviceColmPitch =7384;
			  m_nWndUnitNum = 4;

              m_lMagWidth = 2000;  // just add defaukt value
              m_lMagLength = 20000;
              m_lSlipWidth = 1000;

		  }
		  break;

	}
	UpdateData(false);
	
}

BOOL CCuEliteDeviceParaPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	CTeachCuEliteWorkHolderSheet* psheet = (CTeachCuEliteWorkHolderSheet*)GetParent();

	if (psheet->m_iPageMode == WH_CUELITE_TEACH)
	{
		CWnd* pWnd = psheet->GetDlgItem(ID_WIZNEXT);
        pWnd->EnableWindow(0);
	}
	else
	{

		CWnd* pWnd = psheet->GetDlgItem(IDOK);
		pWnd->EnableWindow(0);
	}


	return CPropertyPage::OnSetActive();
}


int CCuEliteDeviceParaPage::HideInputDialog(void)
{


	CWnd* pWnd;

	    pWnd = GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT);
		pWnd->EnableWindow(FALSE);
       	pWnd = GetDlgItem(IDC_EDIT_SLIP_ROW_NUM);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_NUM);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
		pWnd->EnableWindow(FALSE);
     


	return 0;
}




