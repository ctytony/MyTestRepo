// CuEliteDeviceParaPage.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "CuEliteDeviceParaPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TeachWorkHolderSheet.h"
#include "DeviceData.h"
#include "CuIndex.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CCuEliteDeviceParaPage 对话框

IMPLEMENT_DYNAMIC(CCuEliteDeviceParaPage, CPropertyPage)

CCuEliteDeviceParaPage::CCuEliteDeviceParaPage()
	: CPropertyPage(CCuEliteDeviceParaPage::IDD)
	, m_lMag1stLvHt(0)
	, m_lMagLvPitch(0)
	, m_lMagSlotNum(0)
	, m_nDeviceRowNum(0)
	, m_nDeviceColmNum(0)
	, m_lDeviceColmPitch(0)
	, m_nWndUnitNum(0)
	, m_lLastSlopToTopH(0)
	, m_nSlipListId(0)
	, m_nLastHalfIndex(0)
	, m_nDeviceId(0)
	, m_lSlipLength(0)
	, m_lMagWidth(0)
	, m_lMagLength(0)
	, m_lDeviveWidth(0)
	, m_lSlipCenterOffet(0)
{
}

CCuEliteDeviceParaPage::~CCuEliteDeviceParaPage()
{
}

void CCuEliteDeviceParaPage::DoDataExchange(CDataExchange* pDX)
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
	DDX_LBIndex(pDX, IDC_LIST_DEVICE_DATABASE, m_nSlipListId);
	DDX_Text(pDX, IDC_EDIT_SLIP_LEN, m_lSlipLength);
	DDX_Text(pDX, IDC_EDIT_MAG_WIDTH, m_lMagWidth);
	DDX_Text(pDX, IDC_EDIT_DEVICE_WIDTH, m_lDeviveWidth);
	DDX_Text(pDX, IDC_EDIT_MAG_LENGTH, m_lMagLength);
	DDX_Text(pDX, IDC_EDIT_CENTER_OFFSET, m_lSlipCenterOffet);
}

BEGIN_MESSAGE_MAP(CCuEliteDeviceParaPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_MAG_BOTM_HEIGHT, OnEditMagBotmHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_HEIGHT, OnEditMagSlotHeight)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_SLOT_NUM, OnEditMagSlotNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_ROW_NUM, OnEditSlipRowNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_COLUM_NUM, OnEditSlipColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_SLIP_PITCH, OnEditSlipPitch)
	ON_EN_SETFOCUS(IDC_EDIT_WND_COLUM_NUM, OnEditWndColumNum)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_TOP_HEIGHT, OnEditMagTopHeight)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATABASE, OnLoadDatabase)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_DATABASE, OnListDeviceDatabase)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_WIDTH, OnEditMagWidth)
	ON_EN_SETFOCUS(IDC_EDIT_MAG_LENGTH, OnEditMagLength)
	ON_EN_SETFOCUS(IDC_EDIT_DEVICE_WIDTH, OnEditDeviceWidth)
	ON_EN_SETFOCUS(IDC_EDIT_CENTER_OFFSET, OnEditCenterOffset)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PARA, OnSave)
END_MESSAGE_MAP()

// CCuEliteDeviceParaPage message handlers
BOOL CCuEliteDeviceParaPage::OnInitDialog()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_DATABASE, theMsg.GetMsg(MSG_DATABASE));
	SetDlgItemTextW(IDC_BUTTON_LOAD_DATABASE, theMsg.GetMsg(MSG_FULL_CONVERSION));
	SetDlgItemTextW(IDC_BUTTON_SAVE_PARA, theMsg.GetMsg(MSG_PARTIAL_CONVERSION));
	SetDlgItemTextW(IDC_STATIC_MAGAZINE, theMsg.GetMsg(MSG_MAGAZINE));
	SetDlgItemTextW(IDC_STATIC_BOTTOM_TO_1ST, theMsg.GetMsg(MSG_BOTTOM_TO_1ST_SLOT));
	SetDlgItemTextW(IDC_STATIC_LAST_TO_TOP, theMsg.GetMsg(MSG_LAST_SLOT_TO_TOP));
	SetDlgItemTextW(IDC_STATIC_SLOT_HEIGHT, theMsg.GetMsg(MSG_SLOT_PITCH));
	SetDlgItemTextW(IDC_STATIC_SLOT_NUMBER, theMsg.GetMsg(MSG_SLOT_NUM));
	SetDlgItemTextW(IDC_STATIC_LEADFRAME, theMsg.GetMsg(MSG_LEADFRAME));
	SetDlgItemTextW(IDC_STATIC_LF_ROW, theMsg.GetMsg(MSG_ROW) + _T(":"));
	SetDlgItemTextW(IDC_STATIC_LF_COLUM, theMsg.GetMsg(MSG_COLUMN) + _T(":"));
	SetDlgItemTextW(IDC_STATIC_PITCH, theMsg.GetMsg(MSG_PITCH));
	SetDlgItemTextW(IDC_STATIC_WND_CLAMP, theMsg.GetMsg(MSG_CLAMP_COL));
	SetDlgItemTextW(IDC_STATIC_LENGTH, theMsg.GetMsg(MSG_LENGTH));
	SetDlgItemTextW(IDC_STATIC_DEVICE_WIDTH, theMsg.GetMsg(MSG_WIDTH));
	SetDlgItemTextW(IDC_STATIC_MAG_WIDTH, theMsg.GetMsg(MSG_MAG_WIDTH));
	SetDlgItemTextW(IDC_STATIC_MAG_LENGTH, theMsg.GetMsg(MSG_MAG_LEN));
	SetDlgItemTextW(IDC_STATIC_CENTER_OFFSET, theMsg.GetMsg(MSG_CENTER_OFFSET));

	GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_ROW_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(FALSE);

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
	pListBox->InsertString(nIndex++, _T("SOP7 1C"));
	pListBox->InsertString(nIndex++, _T("SOP7 2C"));

	CUELITE_DEVICE_RECORD* lpstCuEliteDevicePara = new CUELITE_DEVICE_RECORD;
	theDevice.GetCuEliteRcd(lpstCuEliteDevicePara);
	m_lMag1stLvHt      = lpstCuEliteDevicePara->lMag1stLvHt;
	m_lMagLvPitch      = lpstCuEliteDevicePara->lMagLvPitch;   
	m_lMagSlotNum      = lpstCuEliteDevicePara->lMagSlotNum;   
	m_nDeviceRowNum    = lpstCuEliteDevicePara->nDeviceRowNum; 
	m_nDeviceColmNum   = lpstCuEliteDevicePara->nDeviceColmNum;
	m_lDeviceColmPitch = lpstCuEliteDevicePara->lDeviceColmPitch;
	m_nWndUnitNum      = lpstCuEliteDevicePara->nWndUnitNum;
	m_lLastSlopToTopH  = lpstCuEliteDevicePara->lMagLastLvHt;
	m_nLastHalfIndex   = lpstCuEliteDevicePara->nHalfIndexOn;
	m_lSlipLength      = lpstCuEliteDevicePara->lDeviceTotalLen;
	m_lMagWidth        = lpstCuEliteDevicePara->lMagWidth;
	m_lMagLength       = lpstCuEliteDevicePara->lMagLength;
	m_lDeviveWidth     = lpstCuEliteDevicePara->lDeviceTotalWidth;
	m_lSlipCenterOffet = lpstCuEliteDevicePara->lDeviceCenterOffset;
	m_nDeviceId        = lpstCuEliteDevicePara->nDeviceId;
	m_nSlipListId      = lpstCuEliteDevicePara->nSlipListId;

	pListBox->SetCurSel(m_nSlipListId);
	pListBox->SetTopIndex(m_nSlipListId);

	delete lpstCuEliteDevicePara;

	PostMessageW(WM_COMMAND,MAKELONG(IDC_LIST_DEVICE_DATABASE,LBN_SELCHANGE),(LPARAM)pListBox->m_hWnd);

	return CPropertyPage::OnInitDialog();
}

HBRUSH CCuEliteDeviceParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCuEliteDeviceParaPage::OnEditMagBotmHeight()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lMag1stLvHt);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditMagSlotHeight()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lMagLvPitch);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditMagSlotNum()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lMagSlotNum);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditSlipRowNum()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nDeviceRowNum);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditSlipColumNum()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nDeviceColmNum);
	m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;

	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditSlipPitch()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lDeviceColmPitch);
	m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditWndColumNum()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nWndUnitNum);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditMagTopHeight()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lLastSlopToTopH);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditMagLength()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lMagLength, 160000, 100000, 600000, true);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditMagWidth()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lMagWidth, m_lMagWidth, 20000, 100000, true);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnEditDeviceWidth()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lDeviveWidth, m_lDeviveWidth, 20000, 80000,true);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnLoadDatabase()
{
	UpdateData(TRUE);

	short index[1] = {MSG_SURE_TO_OVERWRITE_DEVICE};

	if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
	{
		CUELITE_DEVICE_RECORD* lpstCuEliteDevicePara=new CUELITE_DEVICE_RECORD;
		theDevice.GetCuEliteRcd(lpstCuEliteDevicePara);

		lpstCuEliteDevicePara->nDeviceId           = m_nDeviceId;
		lpstCuEliteDevicePara->lMag1stLvHt         = m_lMag1stLvHt;
		lpstCuEliteDevicePara->lMagLvPitch         = m_lMagLvPitch;
		lpstCuEliteDevicePara->lMagSlotNum         = (short)m_lMagSlotNum;
		lpstCuEliteDevicePara->nDeviceRowNum       = (short)m_nDeviceRowNum;
		lpstCuEliteDevicePara->nDeviceColmNum      = (short)m_nDeviceColmNum;

		lpstCuEliteDevicePara->lDeviceColmPitch    = m_lDeviceColmPitch;
		//lpstCuEliteDevicePara->lDeviceEdgeLen    = m_lDeviceColmPitch / 2;
		lpstCuEliteDevicePara->lDeviceEdgeLen      = (m_lSlipLength - m_lDeviceColmPitch * m_nDeviceColmNum) / 2;

		lpstCuEliteDevicePara->nWndUnitNum         = (short)m_nWndUnitNum;
		lpstCuEliteDevicePara->lMagLastLvHt        = m_lLastSlopToTopH;
		lpstCuEliteDevicePara->nHalfIndexOn        = m_nLastHalfIndex;

		lpstCuEliteDevicePara->lDeviceTotalLen     = m_lSlipLength;
		lpstCuEliteDevicePara->lMagLength          = m_lMagLength;
		lpstCuEliteDevicePara->lMagWidth           = m_lMagWidth;
		lpstCuEliteDevicePara->lDeviceTotalWidth   = m_lDeviveWidth;
		lpstCuEliteDevicePara->lDeviceCenterOffset = m_lSlipCenterOffet;

		lpstCuEliteDevicePara->nSlipListId = (short)m_nSlipListId;

		theDevice.SetCuEliteRcd(lpstCuEliteDevicePara);

		if ((theCuEliteOnloader.theForkRemoveDetectedSensor.GetCurrentState() != SENSOR_ON)
			|| (theCuEliteOffloader.theForkRemoveDetectedSensor.GetCurrentState() != SENSOR_ON))
		{
			if (theCuEliteOnloader.theFork1PresentSensor.GetCurrentState() == SENSOR_ON ||
				theCuEliteOnloader.theFork2PresentSensor.GetCurrentState() == SENSOR_ON ||
				theCuEliteOffloader.theFork1PresentSensor.GetCurrentState() == SENSOR_ON ||
				theCuEliteOffloader.theFork2PresentSensor.GetCurrentState() == SENSOR_ON)
			{
				short anMsgArray[2]={MSG_FORK_DETECTED,MSG_FORK_DETECTED};
				theMsg.ShowMsg(WARNING_MSG, 2, anMsgArray);
			}
		}

		theCuEliteTrack.RefreshMeasurementData(lpstCuEliteDevicePara);
		delete lpstCuEliteDevicePara;

		theCuEliteOnloader.OnloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOnloader.OnloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOffloader.OffloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOffloader.OffloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOnloader.OnloadMagLengthMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOffloader.OffloadMagLengthMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteTrack.MoveFrontTrackToStandbyPos();
		Sleep_x1ms(50);
		theCuEliteTrack.MoveBackTrackToStandbyPos();
		Sleep_x1ms(50);

		CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();
		psheet->GetDlgItem(psheet->m_iPageMode == WH_TEACH ? ID_WIZNEXT : IDOK)->EnableWindow();
	}
}

void CCuEliteDeviceParaPage::OnListDeviceDatabase()
{
	UpdateData(true);

	HideInputDialog();

	CUELITE_DEVICE_RECORD* lpstCuEliteDevicePara=new CUELITE_DEVICE_RECORD;
	theDevice.GetCuEliteRcd(lpstCuEliteDevicePara);

	switch (m_nSlipListId)
	{
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
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;

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
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
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
		m_lDeviceColmPitch = 7610;
		m_nWndUnitNum = 3;
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;

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
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
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
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
		m_nLastHalfIndex = 3;
		m_lSlipLength = m_nDeviceColmNum*m_lDeviceColmPitch;

		break;
	}
	case ID_14R5C_LF:
	{
		m_nDeviceId = ID_14R5C_LF;

		m_lMag1stLvHt = 15480;
		m_lMagLvPitch = 5105;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 15000;
		m_nDeviceRowNum = 14;
		m_nDeviceColmNum = 20;
		//m_lDeviceEdgeLen = 3640;
		m_lDeviceColmPitch = 7384;
		m_nWndUnitNum = 5;
		m_lMagLength = 160000;
		m_lMagWidth = 67500;
		m_lDeviveWidth = 60000;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;

		break;
	}
	case ID_5050_LF:
	{
		m_nDeviceId = ID_5050_LF;

		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 4190;
		m_nDeviceRowNum = 8;
		m_nDeviceColmNum = 16;
		m_lDeviceColmPitch = 9600;
		m_nWndUnitNum = 4;
		m_lMagLength = 160000;
		m_lMagWidth = 67500;
		m_lDeviveWidth = 60000;
		m_nLastHalfIndex = 0;
		m_lSlipLength = m_nDeviceColmNum * m_lDeviceColmPitch;

		break;
	}
	case ID_14R6C_LF:
	{
		m_nDeviceId = ID_14R6C_LF;

		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 20;
		m_lLastSlopToTopH = 4190;
		m_nDeviceRowNum = 14;
		m_nDeviceColmNum = 24;
		m_lDeviceColmPitch = 5973;
		m_nWndUnitNum = 6;
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
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
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
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
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
		m_nLastHalfIndex = 0;

		break;
	}
	case ID_PCB_LONG:
	{
		m_nDeviceId = ID_PCB_LONG;

		GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);

		m_lMag1stLvHt = 14450;
		m_lMagLvPitch = 4995;
		m_lMagSlotNum = 15;
		m_lLastSlopToTopH = 4190;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = 176;
		m_lDeviceColmPitch = 3314;
		m_lSlipLength = 580000;
		m_nWndUnitNum = 16;
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
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

		m_lMag1stLvHt = 12700;
		m_lMagLvPitch = 2540;
		m_lMagSlotNum = 40;
		m_lLastSlopToTopH = 1270;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = 14;
		m_lDeviceColmPitch = 16307;
		m_lSlipLength = m_lDeviceColmPitch * m_nDeviceColmNum;
		m_nWndUnitNum = 2;
		m_lMagLength = 233500;
		m_lMagWidth = 58500;
		m_lDeviveWidth = 50800;
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

		m_lMag1stLvHt = 12700;
		m_lMagLvPitch = 5080;
		m_lMagSlotNum = lpstCuEliteDevicePara->lMagSlotNum;					//Default 20
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;						//Default 160000
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;							//Default 67500
		m_lLastSlopToTopH = 1270;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = lpstCuEliteDevicePara->nDeviceColmNum;	//Default 14
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;			//Default 60000
		m_lDeviceColmPitch = lpstCuEliteDevicePara->lDeviceColmPitch;		//Default 16307
		m_lSlipLength = lpstCuEliteDevicePara->lDeviceTotalLen;					//Default 228295
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

		m_lMag1stLvHt = 12700;
		m_lMagLvPitch = 5080;
		m_lMagSlotNum = lpstCuEliteDevicePara->lMagSlotNum;					//Default 20
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;						//Default 160000
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;							//Default 67500
		m_lLastSlopToTopH = 1270;
		m_nDeviceRowNum = 5;
		m_nDeviceColmNum = lpstCuEliteDevicePara->nDeviceColmNum;	//Default 14
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;			//Default 60000
		m_lDeviceColmPitch = lpstCuEliteDevicePara->lDeviceColmPitch;		//Default 16307
		m_lSlipLength = lpstCuEliteDevicePara->lDeviceTotalLen;					//Default 228295
		m_nWndUnitNum = 2;
		m_nLastHalfIndex = 0;

		break;
	}
	default:
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
		GetDlgItem(IDC_EDIT_MAG_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAG_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEVICE_WIDTH)->EnableWindow(TRUE);

		//m_nLastHalfIndex = 0;
		m_lMag1stLvHt = lpstCuEliteDevicePara->lMag1stLvHt;
		m_lMagLvPitch = lpstCuEliteDevicePara->lMagLvPitch;
		m_lMagSlotNum = lpstCuEliteDevicePara->lMagSlotNum;
		m_lMagLength = lpstCuEliteDevicePara->lMagLength;
		m_lMagWidth = lpstCuEliteDevicePara->lMagWidth;
		m_lLastSlopToTopH = lpstCuEliteDevicePara->lMagLastLvHt;
		m_nDeviceRowNum = lpstCuEliteDevicePara->nDeviceRowNum;
		m_nDeviceColmNum = lpstCuEliteDevicePara->nDeviceColmNum;
		m_lDeviceColmPitch = lpstCuEliteDevicePara->lDeviceColmPitch;
		m_lDeviveWidth = lpstCuEliteDevicePara->lDeviceTotalWidth;
		m_nWndUnitNum = lpstCuEliteDevicePara->nWndUnitNum;
		m_lSlipLength      = lpstCuEliteDevicePara->lDeviceTotalLen;

		if (m_nDeviceColmNum % m_nWndUnitNum == m_nWndUnitNum / 2 && m_nWndUnitNum > 1)
		{
			m_nLastHalfIndex = (short)(m_nDeviceColmNum / m_nWndUnitNum + 1);
		}
		else
		{
			m_nLastHalfIndex = 0;
		}

		break;
	}
	}

	delete lpstCuEliteDevicePara;

	UpdateData(false);
}

BOOL CCuEliteDeviceParaPage::OnSetActive()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();
	psheet->GetDlgItem(psheet->m_iPageMode == WH_TEACH ? ID_WIZNEXT : IDOK)->EnableWindow(FALSE);
	return CPropertyPage::OnSetActive();
}

void CCuEliteDeviceParaPage::HideInputDialog(void)
{
	GetDlgItem(IDC_EDIT_MAG_BOTM_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_TOP_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_SLOT_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_ROW_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SLIP_COLUM_NUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SLIP_PITCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAG_SLOT_NUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_WND_COLUM_NUM)->EnableWindow(TRUE);
}

void CCuEliteDeviceParaPage::OnEditCenterOffset()
{
	GetDlgItem(IDC_BUTTON_LOAD_DATABASE)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lSlipCenterOffet, 0, -10000, 10000, true);
	UpdateData(false);
}

void CCuEliteDeviceParaPage::OnSave()
{
	UpdateData(TRUE);

	short index[1] = {MSG_SURE_TO_SAVE_DEVICE};

	if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
	{
		CUELITE_DEVICE_RECORD* lpstCuEliteDevicePara=new CUELITE_DEVICE_RECORD;
		theDevice.GetCuEliteRcd(lpstCuEliteDevicePara);

		lpstCuEliteDevicePara->nDeviceId           = m_nDeviceId;
		lpstCuEliteDevicePara->lMag1stLvHt         = m_lMag1stLvHt;
		lpstCuEliteDevicePara->lMagLvPitch         = m_lMagLvPitch;
		lpstCuEliteDevicePara->lMagSlotNum         = (short)m_lMagSlotNum;
		lpstCuEliteDevicePara->nDeviceRowNum       = (short)m_nDeviceRowNum;
		lpstCuEliteDevicePara->nDeviceColmNum      = (short)m_nDeviceColmNum;

		lpstCuEliteDevicePara->lDeviceColmPitch    = m_lDeviceColmPitch;
		//lpstCuEliteDevicePara->lDeviceEdgeLen    = m_lDeviceColmPitch / 2;
		lpstCuEliteDevicePara->lDeviceEdgeLen      = (m_lSlipLength - m_lDeviceColmPitch * m_nDeviceColmNum) / 2;

		lpstCuEliteDevicePara->nWndUnitNum         = (short)m_nWndUnitNum;
		lpstCuEliteDevicePara->lMagLastLvHt        = m_lLastSlopToTopH;
		lpstCuEliteDevicePara->nHalfIndexOn        = m_nLastHalfIndex;

		lpstCuEliteDevicePara->lDeviceTotalLen     = m_lSlipLength;
		lpstCuEliteDevicePara->lMagLength          = m_lMagLength;
		lpstCuEliteDevicePara->lMagWidth           = m_lMagWidth;
		lpstCuEliteDevicePara->lDeviceTotalWidth   = m_lDeviveWidth;
		lpstCuEliteDevicePara->lDeviceCenterOffset = m_lSlipCenterOffet;

		lpstCuEliteDevicePara->nSlipListId = (short)m_nSlipListId;

		theDevice.SetCuEliteRcd(lpstCuEliteDevicePara);

		delete lpstCuEliteDevicePara;

		if ((theCuEliteOnloader.theForkRemoveDetectedSensor.GetCurrentState() != SENSOR_ON)
			|| (theCuEliteOffloader.theForkRemoveDetectedSensor.GetCurrentState() != SENSOR_ON))
		{
			if (theCuEliteOnloader.theFork1PresentSensor.GetCurrentState() == SENSOR_ON ||
				theCuEliteOnloader.theFork2PresentSensor.GetCurrentState() == SENSOR_ON ||
				theCuEliteOffloader.theFork1PresentSensor.GetCurrentState() == SENSOR_ON ||
				theCuEliteOffloader.theFork2PresentSensor.GetCurrentState() == SENSOR_ON)
			{
				short anMsgArray[2]={MSG_FORK_DETECTED,MSG_FORK_DETECTED};
				theMsg.ShowMsg(WARNING_MSG, 2, anMsgArray);
			}
		}

		theCuEliteOnloader.OnloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOnloader.OnloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOffloader.OffloadMagClamperFrontMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOffloader.OffloadMagClamperBackMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOnloader.OnloadMagLengthMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteOffloader.OffloadMagLengthMovetoStandyPos();
		Sleep_x1ms(50);
		theCuEliteTrack.MoveFrontTrackToStandbyPos();
		Sleep_x1ms(50);
		theCuEliteTrack.MoveBackTrackToStandbyPos();
		Sleep_x1ms(50);
	}
}
