// TopDeviceParaPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "CuEliteMagParaPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TeachCuWorkHolderSheet.h"

// CCuEliteMagParaPage dialog

IMPLEMENT_DYNAMIC(CCuEliteMagParaPage, CPropertyPage)

CCuEliteMagParaPage::CCuEliteMagParaPage()
	: CPropertyPage(CCuEliteMagParaPage::IDD)
	, m_lMag1stLvHt(0)
	, m_lMagLvPitch(0)
	, m_lMagSlotNum(0)
	, m_lLastSlopToTopH(0)
    , m_lMagWidth(0)
    , m_lMagLength(0)
{

}

CCuEliteMagParaPage::~CCuEliteMagParaPage()
{
}

void CCuEliteMagParaPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAG_BOTM_HEIGHT, m_lMag1stLvHt);
	DDX_Text(pDX, IDC_EDIT_MAG_SLOT_HEIGHT, m_lMagLvPitch);
	DDX_Text(pDX, IDC_EDIT_MAG_SLOT_NUM, m_lMagSlotNum);
	DDX_Text(pDX, IDC_EDIT_MAG_TOP_HEIGHT, m_lLastSlopToTopH);
    DDX_Text(pDX, IDC_EDIT_MAG_WIDTH , m_lMagWidth);
    DDX_Text(pDX, IDC_EDIT_MAG_LENGTH, m_lMagLength);
}


BEGIN_MESSAGE_MAP(CCuEliteMagParaPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_MAG_BOTM_HEIGHT, &CCuEliteMagParaPage::OnEnSetfocusEditMagBotmHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_HEIGHT, &CCuEliteMagParaPage::OnEnSetfocusEditMagSlotHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_NUM, &CCuEliteMagParaPage::OnEnSetfocusEditMagSlotNum)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_TOP_HEIGHT, &CCuEliteMagParaPage::OnEnSetfocusEditMagTopHeight)
    ON_EN_SETFOCUS(IDC_EDIT_MAG_WIDTH, &CCuEliteMagParaPage::OnEnSetfocusEditMagWidth)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_LENGTH, &CCuEliteMagParaPage::OnEnSetfocusEditMagLength)
END_MESSAGE_MAP()


// CCuEliteMagParaPage message handlers

BOOL CCuEliteMagParaPage::OnInitDialog()
{

	CTeachCuWorkHolderSheet* psheet = (CTeachCuWorkHolderSheet*)GetParent();


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
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_ROW_COLON, szString);
     pWnd->SetWindowTextW(szString);

	 pWnd = GetDlgItem(IDC_STATIC_LF_COLUM);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_COLUMN_COLON, szString);
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

	
	TOP_MHS_DEVICE_RECORD  stTopDevicePara;
	theDeviceDataMgr.GetTopDevicePara(&stTopDevicePara);


	m_lMag1stLvHt      = stTopDevicePara.lMag1stLvHt  ;
	m_lMagLvPitch      = stTopDevicePara.lMagLvPitch  ;   
	m_lMagSlotNum      = stTopDevicePara.lMagSlotNum   ;   
	//m_lDeviceEdgeLen   = stTopDevicePara.lDeviceEdgeLen  ;
	

	CPropertyPage::OnInitDialog();

	

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCuEliteMagParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CCuEliteMagParaPage::OnEnSetfocusEditMagBotmHeight()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMag1stLvHt);

    UpdateData(false);

    
}

void CCuEliteMagParaPage::OnEnSetfocusEditMagSlotHeight()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMagLvPitch);

    UpdateData(false);
}

void CCuEliteMagParaPage::OnEnSetfocusEditMagSlotNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMagSlotNum);

    UpdateData(false);
}

void CCuEliteMagParaPage::OnEnSetfocusEditMagTopHeight()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lLastSlopToTopH);

    UpdateData(false);

	
}

void CCuEliteMagParaPage::OnEnSetfocusEditMagWidth()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMagWidth);

    UpdateData(false);
}

void CCuEliteMagParaPage::OnEnSetfocusEditMagLength()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lMagLength);

    UpdateData(false);
}

void CCuEliteMagParaPage::OnBnClickedButtonLoadDatabase()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	short index[10];
	index[0]=MSG_MENU_ITEM_WARNING_CHANGE_DEVICE;
	if(theMsgMgr.ShowMessage(HELP_MSG,1,index,true)==SHOW_MSG_RESULT_OK)  
	{


		CU_ELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	    theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

		stCuEliteDevicePara.lMag1stLvHt      = m_lMag1stLvHt;
		stCuEliteDevicePara.lMagLvPitch      = m_lMagLvPitch;
		stCuEliteDevicePara.lMagSlotNum      = m_lMagSlotNum;
		
    	stCuEliteDevicePara.lMagLastLvHt     = m_lLastSlopToTopH;



		theDeviceDataMgr.StoreCuEliteDevicePara(&stCuEliteDevicePara);

		//theTopTrack.RefreshMeasurementData(&stTopDevicePara);

		theLog.WriteLine(_T("User Choose and updated a new device"));

		
		CTeachCuWorkHolderSheet* psheet = (CTeachCuWorkHolderSheet*)GetParent();

		if (psheet->m_iPageMode == WH_CUTEACH)
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

void CCuEliteMagParaPage::OnLbnSelchangeListDeviceDatabase()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	CWnd* pWnd;

	HideInputDialog();

   	switch(m_iSlipListId)
	{
        // add parameter later
	  case ID_10R3C_LF:
		 
		  break;
	  default:
		  {

			  pWnd = GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_NUM);
			  pWnd->EnableWindow(TRUE);
			  
			  m_nLastHalfIndex = 0;

			  m_lMag1stLvHt = 15480;
			  m_lMagLvPitch = 5105;
			  m_lMagSlotNum = 20;
			  m_lLastSlopToTopH = 15000;

		  }
		  break;

	}
	UpdateData(false);
}

BOOL CCuEliteMagParaPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	CTeachCuWorkHolderSheet* psheet = (CTeachCuWorkHolderSheet*)GetParent();

	if (psheet->m_iPageMode == WH_CUTEACH)
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


int CCuEliteMagParaPage::HideInputDialog(void)
{
	CWnd* pWnd;

    pWnd = GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_MAG_SLOT_NUM);
	pWnd->EnableWindow(TRUE);

	return 0;
}




