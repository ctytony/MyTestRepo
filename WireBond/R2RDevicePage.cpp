// R2RDevicePage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "R2RDevicePage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TeachWorkHolderSheet.h"
#include "R2RIndex.h"

// CR2RDevicePage dialog

IMPLEMENT_DYNAMIC(CR2RDevicePage, CPropertyPage)

CR2RDevicePage::CR2RDevicePage()
	: CPropertyPage(CR2RDevicePage::IDD)
	, m_lMagSlotNum(0)
	, m_nDeviceRowNum(0)
	, m_nDeviceColmNum(0)
	, m_lDeviceColmPitch(0)
	, m_nWndUnitNum(0)
	, m_iSlipListId(0)
	, m_nLastHalfIndex(0)
	, m_nDeviceId(0)
	, m_lSlipLength(0)
{
}

CR2RDevicePage::~CR2RDevicePage()
{
}

void CR2RDevicePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SLIP_ROW_NUM, m_nDeviceRowNum);
	DDX_Text(pDX, IDC_EDIT_SLIP_COLUM_NUM, m_nDeviceColmNum);
	DDX_Text(pDX, IDC_EDIT_SLIP_PITCH, m_lDeviceColmPitch);
	DDX_Text(pDX, IDC_EDIT_WND_COLUM_NUM, m_nWndUnitNum);
	DDX_LBIndex(pDX, IDC_LIST_DEVICE_DATABASE, m_iSlipListId);
	DDX_Text(pDX, IDC_EDIT_SLIP_LEN, m_lSlipLength);
}


BEGIN_MESSAGE_MAP(CR2RDevicePage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_ROW_NUM, &CR2RDevicePage::OnEnSetfocusEditSlipRowNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_COLUM_NUM, &CR2RDevicePage::OnEnSetfocusEditSlipColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_PITCH, &CR2RDevicePage::OnEnSetfocusEditSlipPitch)
	ON_EN_SETFOCUS(IDC_EDIT_WND_COLUM_NUM, &CR2RDevicePage::OnEnSetfocusEditWndColumNum)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATABASE, &CR2RDevicePage::OnBnClickedButtonLoadDatabase)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_DATABASE, &CR2RDevicePage::OnLbnSelchangeListDeviceDatabase)
    ON_EN_SETFOCUS(IDC_EDIT_SLIP_LEN, &CR2RDevicePage::OnEnSetfocusEditSlipLen)
 
END_MESSAGE_MAP()


// CR2RDevicePage message handlers

BOOL CR2RDevicePage::OnInitDialog()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_STATIC_DATABASE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DATABASE));

	pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_APPLY));

	pWnd = GetDlgItem(IDC_STATIC_LEADFRAME);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LEADFRAME));

	pWnd = GetDlgItem(IDC_STATIC_LF_ROW);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ROW) + _T(":"));

	pWnd = GetDlgItem(IDC_STATIC_LF_COLUM);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_COLUMN) + _T(":"));
	
	pWnd = GetDlgItem(IDC_STATIC_PITCH);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PITCH));

	pWnd = GetDlgItem(IDC_STATIC_WND_CLAMP);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAMP_COL));

	pWnd = GetDlgItem(IDC_STATIC_LENGTH);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LENGTH));

	pWnd = GetDlgItem(IDC_EDIT_SLIP_ROW_NUM);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_STATIC_WND_CLAMP);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
	pWnd->EnableWindow(FALSE);

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_DEVICE_DATABASE);
    pListBox->ResetContent();
    
	short nIndex = 0;
	pListBox->InsertString(nIndex++, theMsg.GetMsg(MSG_SELF_DEFINE));
	pListBox->InsertString(nIndex++, _T("5050 (3 die)"));
	pListBox->InsertString(nIndex++, _T("14R * 5C"));
	pListBox->InsertString(nIndex++, _T("14R * 6C"));
	pListBox->InsertString(nIndex++, _T("5R  * 3C  LF"));
	pListBox->InsertString(nIndex++, _T("14R * 4C  LF"));
	pListBox->InsertString(nIndex++, _T("10R * 3C  LF"));
	pListBox->InsertString(nIndex++, _T("10R * 6C  LF"));
	pListBox->InsertString(nIndex++, _T("10W High Power"));
	pListBox->InsertString(nIndex++, _T("100W High Power"));
	pListBox->InsertString(nIndex++, _T("PCB-LONG"));
	pListBox->InsertString(nIndex++, _T("Discret 0603"));
	pListBox->InsertString(nIndex++, _T("Discret SOP"));
	
	TOP_DEVICE_RECORD  stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);

	m_lMagSlotNum      = stDeviceRcd.lMagSlotNum   ;   
	m_nDeviceRowNum    = stDeviceRcd.nDeviceRowNum  ; 
	m_nDeviceColmNum   = stDeviceRcd.nDeviceColmNum   ;
	m_lDeviceColmPitch = stDeviceRcd.lDeviceColmPitch ;
	m_nWndUnitNum      = stDeviceRcd.nWndUnitNum      ;

	m_nLastHalfIndex  =  stDeviceRcd.nHalfIndexOn;
	m_lSlipLength     =  stDeviceRcd.lDeviceTotalLen;

	m_nDeviceId =    stDeviceRcd.nDeviceId;

	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CR2RDevicePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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




void CR2RDevicePage::OnEnSetfocusEditSlipRowNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nDeviceRowNum);

    UpdateData(false);
}

void CR2RDevicePage::OnEnSetfocusEditSlipColumNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nDeviceColmNum);

	m_nWndUnitNum = m_nDeviceColmNum; 

    UpdateData(false);
}

void CR2RDevicePage::OnEnSetfocusEditSlipPitch()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lDeviceColmPitch);

    UpdateData(false);
}

void CR2RDevicePage::OnEnSetfocusEditWndColumNum()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nWndUnitNum);

    UpdateData(false);
}



void CR2RDevicePage::OnEnSetfocusEditSlipLen()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_LOAD_DATABASE);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lSlipLength);

	UpdateData(false);
	
}



void CR2RDevicePage::OnBnClickedButtonLoadDatabase()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	short index[10];
	index[0] = MSG_SURE_TO_OVERWRITE_DEVICE;
	if (theMsg.ShowMsg(HELP_MSG,1,index,true) == SHOW_MSG_RESULT_OK)  
	{
		TOP_DEVICE_RECORD  stDeviceRcd;
		theDevice.GetTopRcd(&stDeviceRcd);

		stDeviceRcd.nDeviceId        = m_nDeviceId;
		stDeviceRcd.lMagSlotNum      = m_lMagSlotNum;
		stDeviceRcd.nDeviceRowNum    = m_nDeviceRowNum;
		stDeviceRcd.nDeviceColmNum   = m_nDeviceColmNum;
		
		stDeviceRcd.lDeviceColmPitch = m_lDeviceColmPitch;

        stDeviceRcd.lDeviceEdgeLen   = (m_lSlipLength - m_lDeviceColmPitch * m_nDeviceColmNum) / 2;

		stDeviceRcd.nWndUnitNum     = m_nWndUnitNum;

		stDeviceRcd.nHalfIndexOn     = m_nLastHalfIndex;

		stDeviceRcd.lDeviceTotalLen  = m_lSlipLength;

		theDevice.SetTopRcd(&stDeviceRcd);

		theR2RTrack.RefreshMeasurementData(&stDeviceRcd);
		
		CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

		if (psheet->m_iPageMode == WH_TEACH)
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

void CR2RDevicePage::OnLbnSelchangeListDeviceDatabase()
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

			  pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
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

			  pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
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

			  pWnd = GetDlgItem(IDC_EDIT_SLIP_ROW_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
			  pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
			  pWnd->EnableWindow(TRUE);

			 // pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
			 // pWnd->EnableWindow(TRUE);
			  pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
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

		  }
		  break;

	}
	//special design for R2R, only one window clamp{
	m_nWndUnitNum = m_nDeviceColmNum; 

    //}

	UpdateData(false);
	pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
    pWnd->EnableWindow(FALSE);


}

BOOL CR2RDevicePage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	if (psheet->m_iPageMode == WH_TEACH)
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


int CR2RDevicePage::HideInputDialog(void)
{


	CWnd* pWnd;

		pWnd = GetDlgItem(IDC_EDIT_SLIP_ROW_NUM);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SLIP_PITCH);
		pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_EDIT_WND_COLUM_NUM);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_EDIT_SLIP_LEN);
		pWnd->EnableWindow(FALSE);


	return 0;
}



