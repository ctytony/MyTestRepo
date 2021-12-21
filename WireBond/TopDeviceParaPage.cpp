// TopDeviceParaPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "TopDeviceParaPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TeachWorkHolderSheet.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "Busy.h"

// CTopDeviceParaPage dialog
IMPLEMENT_DYNAMIC(CTopDeviceParaPage, CPropertyPage)

CTopDeviceParaPage::CTopDeviceParaPage()
	: CPropertyPage(CTopDeviceParaPage::IDD)
	, m_lMag1stLvHt(0)
	, m_lMagLvPitch(0)
	, m_lMagSlotNum(0)
	, m_nDeviceRowNum(0)
	, m_nDeviceColmNum(0)
	, m_lDeviceColmPitch(0)
	, m_nWndUnitNum(0)
	, m_lLastSlopToTopH(0)
	, m_iSlipListId(0)
	, m_nLastHalfIndex(0)
	, m_nDeviceId(0)
	, m_lSlipLength(0)
{
	m_bBusy = FALSE;	//V1.1.W158 add
}

CTopDeviceParaPage::~CTopDeviceParaPage()
{
}

void CTopDeviceParaPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
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
}

BEGIN_MESSAGE_MAP(CTopDeviceParaPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_MAG_BOTM_HEIGHT, OnFocusMagBotmHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_HEIGHT, OnFocusMagSlotHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_NUM, OnFocusMagSlotNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_ROW_NUM, OnFocusSlipRowNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_COLUM_NUM, OnFocusSlipColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_PITCH, OnFocusSlipPitch)
	ON_EN_SETFOCUS(IDC_EDIT_WND_COLUM_NUM, OnFocusWndColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_TOP_HEIGHT, OnFocusMagTopHeight)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATABASE, OnLoadDatabase)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_DATABASE, OnListDatabase)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_LEN, OnFocusSlipLen)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W158 add
END_MESSAGE_MAP()

// CTopDeviceParaPage message handlers
BOOL CTopDeviceParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_DATABASE, theMsg.GetMsg(MSG_DATABASE));
	SetDlgItemTextW(IDC_BUTTON_LOAD_DATABASE, theMsg.GetMsg(MSG_APPLY));
	SetDlgItemTextW(IDC_STATIC_MAGAZINE, theMsg.GetMsg(MSG_MAGAZINE));
	SetDlgItemTextW(IDC_STATIC_BOTTOM_TO_1ST, theMsg.GetMsg(MSG_BOTTOM_TO_1ST_SLOT));
	SetDlgItemTextW(IDC_STATIC_LAST_TO_TOP, theMsg.GetMsg(MSG_LAST_SLOT_TO_TOP));
	SetDlgItemTextW(IDC_STATIC_SLOT_HEIGHT, theMsg.GetMsg(MSG_SLOT_PITCH));
	SetDlgItemTextW(IDC_STATIC_SLOT_NUMBER, theMsg.GetMsg(MSG_SLOT_NUM));
	SetDlgItemTextW(IDC_STATIC_LEADFRAME, theMsg.GetMsg(MSG_LEADFRAME));
	SetDlgItemTextW(IDC_STATIC_LF_ROW, theMsg.GetMsg(MSG_ROW)+_T(":"));
	SetDlgItemTextW(IDC_STATIC_LF_COLUM, theMsg.GetMsg(MSG_COLUMN)+_T(":"));
	SetDlgItemTextW(IDC_STATIC_PITCH, theMsg.GetMsg(MSG_PITCH));
	SetDlgItemTextW(IDC_STATIC_WND_CLAMP, theMsg.GetMsg(MSG_CLAMP_COL));
	SetDlgItemTextW(IDC_STATIC_LENGTH, theMsg.GetMsg(MSG_LENGTH));

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_DEVICE_DATABASE);
	pListBox->AddString(theMsg.GetMsg(MSG_SELF_DEFINE)); 
	pListBox->AddString(_T("5050 (3 die)"));
	pListBox->AddString(_T("14R * 5C"));
	pListBox->AddString(_T("14R * 6C"));
	pListBox->AddString(_T("5R  * 3C  LF"));
	pListBox->AddString(_T("14R * 4C  LF"));
	pListBox->AddString(_T("10R * 3C  LF"));
	pListBox->AddString(_T("10R * 6C  LF"));
	pListBox->AddString(_T("10W High Power"));
	pListBox->AddString(_T("100W High Power"));
	pListBox->AddString(_T("PCB-LONG"));
	pListBox->AddString(_T("Discret 0603"));
	pListBox->AddString(_T("Discret SOP"));
	pListBox->AddString(_T("SOP7 1C"));
	pListBox->AddString(_T("SOP7 2C"));
	pListBox->AddString(_T("CEDAR"));

	const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();

	m_lMag1stLvHt      = pstDevice->lMag1stLvHt;
	m_lMagLvPitch      = pstDevice->lMagLvPitch;   
	m_lMagSlotNum      = pstDevice->lMagSlotNum;   
	m_nDeviceRowNum    = pstDevice->nDeviceRowNum; 
	m_nDeviceColmNum   = pstDevice->nDeviceColmNum;
	m_lDeviceColmPitch = pstDevice->lDeviceColmPitch;
	m_nWndUnitNum      = pstDevice->nWndUnitNum;
	m_lLastSlopToTopH  = pstDevice->lMagLastLvHt;
	m_nLastHalfIndex   = pstDevice->nHalfIndexOn;
	m_lSlipLength      = pstDevice->lDeviceTotalLen;
	m_nDeviceId        = pstDevice->nDeviceId;

	return CPropertyPage::OnInitDialog();
}

HBRUSH CTopDeviceParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTopDeviceParaPage::OnFocusMagBotmHeight()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lMag1stLvHt);
    UpdateData(false); 
}

void CTopDeviceParaPage::OnFocusMagSlotHeight()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lMagLvPitch);
    UpdateData(false);
}

void CTopDeviceParaPage::OnFocusMagSlotNum()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lMagSlotNum);
    UpdateData(false);
}

void CTopDeviceParaPage::OnFocusSlipRowNum()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nDeviceRowNum);
    UpdateData(false);
}

void CTopDeviceParaPage::OnFocusSlipColumNum()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nDeviceColmNum);
	m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
    UpdateData(false);
}

void CTopDeviceParaPage::OnFocusSlipPitch()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lDeviceColmPitch);
	m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
    UpdateData(false);
}

void CTopDeviceParaPage::OnFocusWndColumNum()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nWndUnitNum);
    UpdateData(false);
}

void CTopDeviceParaPage::OnFocusMagTopHeight()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lLastSlopToTopH);
    UpdateData(false);	
}

void CTopDeviceParaPage::OnFocusSlipLen()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lSlipLength);
	UpdateData(false);
}

void CTopDeviceParaPage::OnLoadDatabase()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	UpdateData(TRUE);

	short index[1] = {MSG_SURE_TO_OVERWRITE_DEVICE};
	if (theMsg.ShowMsg(HELP_MSG,1,index,true) != SHOW_MSG_RESULT_OK) return;

	TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();

	pstDevice->nDeviceId        = m_nDeviceId;
	pstDevice->lMag1stLvHt      = m_lMag1stLvHt;
	pstDevice->lMagLvPitch      = m_lMagLvPitch;
	pstDevice->lMagSlotNum      = m_lMagSlotNum;
	pstDevice->nDeviceRowNum    = m_nDeviceRowNum;
	pstDevice->nDeviceColmNum   = m_nDeviceColmNum;
	pstDevice->lDeviceColmPitch = m_lDeviceColmPitch;
	//lpstTopDevicePara->lDeviceEdgeLen   = m_lDeviceColmPitch/2;
    pstDevice->lDeviceEdgeLen   = (m_lSlipLength - m_lDeviceColmPitch * m_nDeviceColmNum) / 2;
	pstDevice->nWndUnitNum      = m_nWndUnitNum;
	pstDevice->lMagLastLvHt     = m_lLastSlopToTopH;
	pstDevice->nHalfIndexOn     = m_nLastHalfIndex;
	pstDevice->lDeviceTotalLen  = m_lSlipLength;

    if ((theTopOnloader.m_ForkRemoveDetectedSensor.GetCurrentState() != SENSOR_ON)
        || (theTopOffloader.m_ForkRemoveDetectedSensor.GetCurrentState() != SENSOR_ON))
    {
        if ((theTopOnloader.m_Fork1PresentSensor.GetCurrentState() == SENSOR_ON) ||
            (theTopOnloader.m_Fork2PresentSensor.GetCurrentState() == SENSOR_ON) ||
            (theTopOffloader.m_Fork1PresentSensor.GetCurrentState() == SENSOR_ON) ||
            (theTopOffloader.m_Fork2PresentSensor.GetCurrentState() == SENSOR_ON))
        {
			short anMsgArray[2] = {MSG_FORK_DETECTED, MSG_FORK_DETECTED};
		    theMsg.ShowMsg(WARNING_MSG, 2, anMsgArray);
        }
    }

	theTopTrack.RefreshMeasurementData(pstDevice);
	
	GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(FALSE);

	((CPropertySheet *)GetParent())->PressButton(PSBTN_NEXT);
}

void CTopDeviceParaPage::OnListDatabase()
{
	UpdateData(true);

	HideInputDialog();

	switch (m_iSlipListId)
	{ //not follow sequence number, refer to "DeviceDataMgr.h"
	case ID_5R3C_LF:
	{
		m_nDeviceId = ID_5R3C_LF;
		m_lMag1stLvHt = 8590;
		m_lMagLvPitch = 4404;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 13030;
		m_nDeviceRowNum = 5;                          
		m_nDeviceColmNum = 12;
		//m_lDeviceEdgeLen = 5880;
		m_lDeviceColmPitch = 11770;
		m_nWndUnitNum = 3;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;
		break;
	}
	case ID_14R4C_LF:
	{
		m_nDeviceId = ID_14R4C_LF;
		m_lMag1stLvHt = 14960;
		m_lMagLvPitch = 5184;
		m_lLastSlopToTopH = 9340;
		m_lMagSlotNum = 20;
		m_nDeviceRowNum = 14;
		m_nDeviceColmNum = 20;
		//m_lDeviceEdgeLen = 3570;
		m_lDeviceColmPitch = 7440;
		m_nWndUnitNum = 4;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
		break;
	}
	case ID_10R3C_LF:
	{
		m_nDeviceId = ID_10R3C_LF;
		m_lMag1stLvHt = 8590;
		m_lMagLvPitch = 4404;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 13030;
		m_nDeviceRowNum = 10;
		m_nDeviceColmNum = 18;
		//m_lDeviceEdgeLen = 3920;
		m_lDeviceColmPitch = 7800;
		m_nWndUnitNum = 3;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;
		break;
	}
	case ID_10R6C_LF:
	{
		m_nDeviceId = ID_10R6C_LF;
		m_lMag1stLvHt = 8590;
		m_lMagLvPitch = 4404;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 13030;
		m_nDeviceRowNum = 10;
		m_nDeviceColmNum = 18;
		//m_lDeviceEdgeLen = 3920;
		m_lDeviceColmPitch = 7800;
		m_nWndUnitNum = 6;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
		break;
	}
	case ID_10W_HIGHPWR:
	{
		m_nDeviceId = ID_10W_HIGHPWR;
		m_lMag1stLvHt = 13000;
		m_lMagLvPitch = 7000;
		m_lMagSlotNum = 15;
		m_lLastSlopToTopH = 15000;
		m_nDeviceRowNum = 2;
		m_nDeviceColmNum = 10;
		//m_lDeviceEdgeLen = 6060;
		m_lDeviceColmPitch = 12250;
		m_nWndUnitNum = 4;
		m_nLastHalfIndex = 3;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
		break;
	}
	case ID_14R5C_LF:
	{
		m_nDeviceId =ID_14R5C_LF;
		m_lMag1stLvHt = 15480;
		m_lMagLvPitch = 5105;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 15000;
		m_nDeviceRowNum = 14;
		m_nDeviceColmNum = 20;
		//m_lDeviceEdgeLen = 3640;
		m_lDeviceColmPitch = 7500;
		m_nWndUnitNum = 5;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
		break;
	}
	case ID_5050_LF:	//m_iSlipListId =1
	{
		m_nDeviceId = ID_5050_LF;
		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 9185;
		m_nDeviceRowNum = 8;
		m_nDeviceColmNum = 16;
		m_lDeviceColmPitch = 9600;
		m_nWndUnitNum = 4;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
		break;
	}
	case ID_14R6C_LF:
	{
		m_nDeviceId =ID_14R6C_LF;
		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 4190;
		m_nDeviceRowNum = 14;
		m_nDeviceColmNum = 24;
		m_lDeviceColmPitch = 5973;
		m_nWndUnitNum = 6;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
		break;
	}
	case ID_DIC_0603:  //17 times index
	{
		m_nDeviceId = ID_DIC_0603;
		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 4190;
		m_nDeviceRowNum = 46;
		m_nDeviceColmNum = 68;
		m_lDeviceColmPitch = 1698;
		m_nWndUnitNum = 4;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
		break;
	}
	case ID_100W_HIGHPWR:
	{
		m_nDeviceId = ID_100W_HIGHPWR;
		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 4190;
		m_nDeviceRowNum = 1;
		m_nDeviceColmNum = 2;
		m_lDeviceColmPitch = 62000;
		m_lSlipLength = 124000;
		m_nWndUnitNum = 1;
		m_nLastHalfIndex = 0;
		break;
	}
	case ID_PCB_LONG:
	{
		m_nDeviceId = ID_PCB_LONG;

		GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_LEN)->EnableWindow(TRUE);

		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 15;
		m_lLastSlopToTopH = 4190;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = 176;
		m_lDeviceColmPitch = 3314;
		m_lSlipLength = 580000;
		m_nWndUnitNum = 16;
		m_nLastHalfIndex = 0;
		break;
	}
	case ID_SOP16:
	{
		m_nDeviceId = ID_SOP16;

		GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_LEN)->EnableWindow(TRUE);

		m_lMag1stLvHt = 12700;
		m_lMagLvPitch = 2540;
		m_lMagSlotNum = 40;
		m_lLastSlopToTopH = 1270;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = 14;
		m_lDeviceColmPitch = 16307;
		m_lSlipLength = 228295;
		m_nWndUnitNum = 2;
		m_nLastHalfIndex = 0;
		break;
	}
	case ID_SOP7_1C:
	{
		m_nDeviceId = ID_SOP7_1C;

		GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_LEN)->EnableWindow(TRUE);

		m_lMag1stLvHt = 12700;
		m_lMagLvPitch = 5080;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 1270;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = 14;
		m_lDeviceColmPitch = 16307;
		m_lSlipLength = 228295;
		m_nWndUnitNum = 1;
		m_nLastHalfIndex = 0;
		break;
	}
	case ID_SOP7_2C:
	{
		m_nDeviceId = ID_SOP7_2C;

		GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_LEN)->EnableWindow(TRUE);

		m_lMag1stLvHt = 12700;
		m_lMagLvPitch = 5080;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 1270;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = 14;
		m_lDeviceColmPitch = 16307;
		m_lSlipLength = 228295;
		m_nWndUnitNum = 2;
		m_nLastHalfIndex = 0;
		break;
	}
	case ID_CEDAR:
	{
		m_nDeviceId = ID_CEDAR;

		GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_LEN)->EnableWindow(TRUE);

		m_lMag1stLvHt = 12700;
		m_lMagLvPitch = 5080;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 1270;
		m_nDeviceRowNum = 48;
		m_nDeviceColmNum = 54;
		m_lDeviceColmPitch = 1400;
		m_lSlipLength = 85000;
		m_nWndUnitNum = 14;
		m_nLastHalfIndex = 0;
		break;
	}
	default:	//m_iSlipListId =0 or m_iSlipListId >15
	{
		m_nDeviceId = 255;

		GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_ROW_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_LEN)->EnableWindow(TRUE);

		const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();

		m_lMag1stLvHt      = pstDevice->lMag1stLvHt;
		m_lMagLvPitch      = pstDevice->lMagLvPitch;
		m_lMagSlotNum      = pstDevice->lMagSlotNum;
		m_lLastSlopToTopH  = pstDevice->lMagLastLvHt;
		m_nDeviceRowNum    = pstDevice->nDeviceRowNum;
		m_nDeviceColmNum   = pstDevice->nDeviceColmNum;
		m_lDeviceColmPitch = pstDevice->lDeviceColmPitch;
		m_nWndUnitNum      = pstDevice->nWndUnitNum;
		m_lSlipLength      = m_nDeviceColmNum * m_lDeviceColmPitch;

		if (m_nDeviceColmNum % m_nWndUnitNum == m_nWndUnitNum / 2 && m_nWndUnitNum > 1)
		{
			m_nLastHalfIndex = m_nDeviceColmNum / m_nWndUnitNum + 1;
		}
		else
		{
			m_nLastHalfIndex = 0;
		}
		break;
	}
	}

	UpdateData(false);
}

BOOL CTopDeviceParaPage::OnSetActive()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();
	psheet->GetDlgItem(psheet->m_iPageMode == WH_TEACH ? ID_WIZNEXT : IDOK)->EnableWindow(FALSE);

	return CPropertyPage::OnSetActive();
}

void CTopDeviceParaPage::HideInputDialog(void)
{
    GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_ROW_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SLIP_LEN)->EnableWindow(FALSE);
}

LRESULT CTopDeviceParaPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W158 add
{
	return (m_bBusy == TRUE);
}
