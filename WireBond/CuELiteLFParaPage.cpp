// TopDeviceParaPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "CuELiteLFParaPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TeachCuWorkHolderSheet.h"

// CCuELiteLFParaPage dialog

IMPLEMENT_DYNAMIC(CCuELiteLFParaPage, CPropertyPage)

CCuELiteLFParaPage::CCuELiteLFParaPage()
	: CPropertyPage(CCuELiteLFParaPage::IDD)
	, m_nDeviceRowNum(0)
	, m_nDeviceColmNum(0)
	, m_lDeviceColmPitch(0)
	, m_nWndUnitNum(0)
	, m_iSlipListId(0)
	, m_nLastHalfIndex(0)
	, m_nDeviceId(0)
	, m_lSlipLength(0)
    , m_lDeviceWidth(0)
{

}

CCuELiteLFParaPage::~CCuELiteLFParaPage()
{
}

void CCuELiteLFParaPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SLIP_ROW_NUM, m_nDeviceRowNum);
	DDX_Text(pDX, IDC_EDIT_SLIP_COLUM_NUM, m_nDeviceColmNum);
	//DDX_Text(pDX, IDC_EDIT_SLIP_1ST_TO_EDGE_LEN, m_lDeviceEdgeLen);
	DDX_Text(pDX, IDC_EDIT_SLIP_PITCH, m_lDeviceColmPitch);
	DDX_Text(pDX, IDC_EDIT_WND_COLUM_NUM, m_nWndUnitNum);
	DDX_LBIndex(pDX, IDC_LIST_DEVICE_DATABASE, m_iSlipListId);
	DDX_Text(pDX, IDC_EDIT_SLIP_LEN, m_lSlipLength);
    DDX_Text(pDX, IDC_EDIT_SLIP_WIDTH , m_lDeviceWidth);  //v2.18.1.S6 add
}


BEGIN_MESSAGE_MAP(CCuELiteLFParaPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_ROW_NUM, &CCuELiteLFParaPage::OnEnSetfocusEditSlipRowNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_COLUM_NUM, &CCuELiteLFParaPage::OnEnSetfocusEditSlipColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_PITCH, &CCuELiteLFParaPage::OnEnSetfocusEditSlipPitch)
	ON_EN_SETFOCUS(IDC_EDIT_WND_COLUM_NUM, &CCuELiteLFParaPage::OnEnSetfocusEditWndColumNum)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATABASE, &CCuELiteLFParaPage::OnBnClickedButtonLoadDatabase)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_DATABASE, &CCuELiteLFParaPage::OnLbnSelchangeListDeviceDatabase)
    ON_EN_SETFOCUS(IDC_EDIT_SLIP_LEN, &CCuELiteLFParaPage::OnEnSetfocusEditSlipLen)
    ON_EN_SETFOCUS(IDC_EDIT_SLIP_WIDTH, &CCuELiteLFParaPage::OnEnSetfocusEditSlipWidth) //v2.18.1.S6 add
END_MESSAGE_MAP()


// CCuELiteLFParaPage message handlers

BOOL CCuELiteLFParaPage::OnInitDialog()
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

     pWnd = GetDlgItem(IDC_STATIC_WIDTH);
	 theMsgMgr.GetMsgString(MSG_MENU_ITEM_DEVICE_TOTAL_WIDTH, szString);
	 pWnd->SetWindowTextW(szString);
	
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
    
   
	pListBox->InsertString(0, _T("Self-define(×Ô¶¨Òå)"));

	
	CU_ELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

	m_nDeviceRowNum    = stCuEliteDevicePara.nDeviceRowNum  ; 
	m_nDeviceColmNum   = stCuEliteDevicePara.nDeviceColmNum   ;
	//m_lDeviceEdgeLen   = stTopDevicePara.lDeviceEdgeLen  ;
	m_lDeviceColmPitch = stCuEliteDevicePara.lDeviceColmPitch ;
	m_nWndUnitNum      = stCuEliteDevicePara.nWndUnitNum      ;
	m_nLastHalfIndex  =  stCuEliteDevicePara.nHalfIndexOn;
	m_lSlipLength     =  stCuEliteDevicePara.lDeviceTotalLen;
    m_lDeviceWidth    =  stCuEliteDevicePara.lDeviceTotalWidth; //v2.18.1.S6 add

	m_nDeviceId =    stCuEliteDevicePara.nDeviceId;


	CPropertyPage::OnInitDialog();

	

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCuELiteLFParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CCuELiteLFParaPage::OnEnSetfocusEditSlipRowNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nDeviceRowNum);

    UpdateData(false);
}

void CCuELiteLFParaPage::OnEnSetfocusEditSlipColumNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nDeviceColmNum);

    UpdateData(false);
}

void CCuELiteLFParaPage::OnEnSetfocusEditSlipPitch()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lDeviceColmPitch);

    UpdateData(false);
}

void CCuELiteLFParaPage::OnEnSetfocusEditWndColumNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nWndUnitNum);

    UpdateData(false);
}


void CCuELiteLFParaPage::OnEnSetfocusEditSlipLen()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lSlipLength);

	UpdateData(false);
	
}

//v2.18.1.S6 add
void CCuELiteLFParaPage::OnEnSetfocusEditSlipWidth()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lDeviceWidth);

	UpdateData(false);
	
}



void CCuELiteLFParaPage::OnBnClickedButtonLoadDatabase()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	short index[10];
	index[0]=MSG_MENU_ITEM_WARNING_CHANGE_DEVICE;
	if(theMsgMgr.ShowMessage(HELP_MSG,1,index,true)==SHOW_MSG_RESULT_OK)  
	{

		
	    CU_ELITE_MHS_DEVICE_RECORD stCuEliteDevicePara;
	    theDeviceDataMgr.GetCuEliteDevicePara(&stCuEliteDevicePara);

	  	stCuEliteDevicePara.nDeviceId    = m_nDeviceId;
		stCuEliteDevicePara.nDeviceRowNum    = m_nDeviceRowNum;
		stCuEliteDevicePara.nDeviceColmNum   = m_nDeviceColmNum;
		
		stCuEliteDevicePara.lDeviceColmPitch = m_lDeviceColmPitch;
		//stTopDevicePara.lDeviceEdgeLen   = m_lDeviceColmPitch/2;
        stCuEliteDevicePara.lDeviceEdgeLen   = (m_lSlipLength - m_lDeviceColmPitch * m_nDeviceColmNum) / 2;

		stCuEliteDevicePara.nWndUnitNum      = m_nWndUnitNum;
		stCuEliteDevicePara.nHalfIndexOn     = m_nLastHalfIndex;

		stCuEliteDevicePara.lDeviceTotalLen  = m_lSlipLength;


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

void CCuELiteLFParaPage::OnLbnSelchangeListDeviceDatabase()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	CWnd* pWnd;

	HideInputDialog();

   	switch(m_iSlipListId)
	{
	  case ID_5R3C_LF:
		 //device parameter will add later
		  break;      

	  default:
		  {
			  m_nDeviceId =255;
			 
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_ROW_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
			  pWnd->EnableWindow(TRUE);
			  
			  m_nLastHalfIndex = 0;
			  m_nDeviceRowNum =10;
			  m_nDeviceColmNum =16;
			  m_lDeviceColmPitch =7384;
			  m_nWndUnitNum = 4;

		  }
		  break;

	}
	UpdateData(false);
}

BOOL CCuELiteLFParaPage::OnSetActive()
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


int CCuELiteLFParaPage::HideInputDialog(void)
{
	CWnd* pWnd;

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




