// DlgCuEliteElevSetupPage.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DlgCuEliteElevSetupPage.h"
#include "MachineData.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MenuConfig.h"
#include "PopUpCuEliteStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "MaterialHandlerThread.h"
#include "PopUpTopStepperCtr.h"
#include "DeviceData.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CDlgCuEliteElevSetupPage 对话框
IMPLEMENT_DYNAMIC(CDlgCuEliteElevSetupPage, CDialog)

CDlgCuEliteElevSetupPage::CDlgCuEliteElevSetupPage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCuEliteElevSetupPage::IDD, pParent)
	, m_lOnloaderMagClamperDepth(0)
	, m_lOffloaderMagClamperDepth(0)
	, m_lOnloaderMagClamperFrontStandby(0)
	, m_lOffloaderMagClamperFrontStandby(0)
	, m_lOnloaderMagClamperBackStandby(0)
	, m_lOffloaderMagClamperBackStandby(0)
	, m_lOnloaderMagLengthStandby(0)
	, m_lOffloaderMagLengthStandby(0)
{
	m_nCurCtrl=0;

	m_lpstOnloadRcd=NULL;
	m_lpstOffloadRcd=NULL;
}

CDlgCuEliteElevSetupPage::~CDlgCuEliteElevSetupPage()
{
}

void CDlgCuEliteElevSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ONLOADER_MAG_CLAMPER_DEPTH, m_lOnloaderMagClamperDepth);
	DDX_Text(pDX, IDC_EDIT_OFFLOADER_MAG_CLAMPER_DEPTH, m_lOffloaderMagClamperDepth);
	DDX_Text(pDX, IDC_EDIT_ONLOADER_MAG_CLAMPER_FRONT_POSITION, m_lOnloaderMagClamperFrontStandby);
	DDX_Text(pDX, IDC_EDIT_OFFLOADER_MAG_CLAMPER_FRONT_POSITION, m_lOffloaderMagClamperFrontStandby);
	DDX_Text(pDX, IDC_EDIT_ONLOADER_MAG_CLAMPER_BACK_POSITION, m_lOnloaderMagClamperBackStandby);
	DDX_Text(pDX, IDC_EDIT_OFFLOADER_MAG_CLAMPER_BACK_POSITION, m_lOffloaderMagClamperBackStandby);
	DDX_Text(pDX, IDC_EDIT_ONLOADER_MAG_LENGTH_POSITION, m_lOnloaderMagLengthStandby);
	DDX_Text(pDX, IDC_EDIT_OFFLOADER_MAG_LENGTH_POSITION, m_lOffloaderMagLengthStandby);
}

BEGIN_MESSAGE_MAP(CDlgCuEliteElevSetupPage, CDialog)
	ON_BN_CLICKED(IDC_BTN_ONLOADER_MAG_CLAMPER_FRONT_EDIT, OnOnloaderMagClamperFrontEdit)
	ON_BN_CLICKED(IDC_BTN_ONLOADER_MAG_CLAMPER_BACK_EDIT, OnOnloaderMagClamperBackEdit)
	ON_BN_CLICKED(IDC_BTN_ONLOADER_MAG_CLAMPER_LENGTH_EDIT, OnOnloaderMagClamperLengthEdit)
	ON_BN_CLICKED(IDC_BTN_OFFLOADER_MAG_CLAMPER_FRONT_EDIT, OnOffloaderMagClamperFrontEdit)
	ON_BN_CLICKED(IDC_BTN_OFFLOADER_MAG_CLAMPER_BACK_EDIT, OnOffloaderMagClamperBackEdit)
	ON_BN_CLICKED(IDC_BTN_OFFLOADER_MAG_CLAMPER_LENGTH_EDIT, OnOffloaderMagClamperLengthEdit)
	ON_EN_SETFOCUS(IDC_EDIT_ONLOADER_MAG_CLAMPER_DEPTH, OnOnloaderMagClamperDepth)
	ON_EN_SETFOCUS(IDC_EDIT_ONLOADER_MAG_CLAMPER_FRONT_POSITION, OnEditOnloaderMagClamperFrontPosition)
	ON_EN_SETFOCUS(IDC_EDIT_ONLOADER_MAG_CLAMPER_BACK_POSITION, OnEditOnloaderMagClamperBackPosition)
	ON_EN_SETFOCUS(IDC_EDIT_ONLOADER_MAG_LENGTH_POSITION, OnEditOnloaderMagLengthPosition)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOADER_MAG_CLAMPER_DEPTH, OnOffloaderMagClamperDepth)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOADER_MAG_CLAMPER_FRONT_POSITION, OnEditOffloaderMagClamperFrontPosition)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOADER_MAG_CLAMPER_BACK_POSITION, OnEditOffloaderMagClamperBackPosition)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLOADER_MAG_LENGTH_POSITION, OnEditOffloaderMagLengthPosition)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_MESSAGE(WM_REFRESH,OnRefresh)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CDlgCuEliteElevSetupPage 消息处理程序
BOOL CDlgCuEliteElevSetupPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	((CWireBondDlg*)GetParent())->InhibitSystemButton();

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);

	CPoint pt(Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP);
	SetWindowPos(NULL, pt.x, pt.y, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_STATIC_ONLOADER, theMsg.GetMsg(MSG_INPUT));
	SetDlgItemTextW(IDC_STATIC_OFFLOADER, theMsg.GetMsg(MSG_OUTPUT));
	SetDlgItemTextW(IDC_STATIC_FRONT_CLAMP_DISTANCE, theMsg.GetMsg(MSG_NEXT_MAG_HOLD_POS));
	SetDlgItemTextW(IDC_STATIC_FRONT_CLAMP_DISTANCE2, theMsg.GetMsg(MSG_NEXT_MAG_HOLD_POS));
	SetDlgItemTextW(IDC_STATIC_FRONT_CLAMPER_POSITION, theMsg.GetMsg(MSG_CLAMPER_FRONT_POS));
	SetDlgItemTextW(IDC_STATIC_FRONT_CLAMPER_POSITION2, theMsg.GetMsg(MSG_CLAMPER_FRONT_POS));
	SetDlgItemTextW(IDC_STATIC_BACK_CLAMPER_POSITION, theMsg.GetMsg(MSG_CLAMPER_BACK_POS));
	SetDlgItemTextW(IDC_STATIC_BACK_CLAMPER_POSITION2, theMsg.GetMsg(MSG_CLAMPER_BACK_POS));
	SetDlgItemTextW(IDC_STATIC_MAG_LENGTH_POSITION, theMsg.GetMsg(MSG_CLAMPER_LEN_POS));
	SetDlgItemTextW(IDC_STATIC_MAG_LENGTH_POSITION2, theMsg.GetMsg(MSG_CLAMPER_LEN_POS));
	SetDlgItemTextW(IDC_BTN_ONLOADER_MAG_CLAMPER_FRONT_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BTN_OFFLOADER_MAG_CLAMPER_FRONT_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BTN_ONLOADER_MAG_CLAMPER_BACK_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BTN_OFFLOADER_MAG_CLAMPER_BACK_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BTN_ONLOADER_MAG_CLAMPER_LENGTH_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BTN_OFFLOADER_MAG_CLAMPER_LENGTH_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));

	m_lpstOnloadRcd=new CUELITE_ONLOAD_RCD;
	m_lpstOffloadRcd=new CUELITE_OFFLOAD_RCD;

	theCuEliteOnloader.GetCuEliteOnloadRcd(m_lpstOnloadRcd);

	m_lOnloaderMagClamperDepth = m_lpstOnloadRcd->lOnLoadMagClamperDepth;
	m_lOnloaderMagClamperFrontStandby = m_lpstOnloadRcd->lOnLoadMagClamperFrontStandby;
	m_lOnloaderMagClamperBackStandby = m_lpstOnloadRcd->lOnLoadMagClamperBackStandby;
	m_lOnloaderMagLengthStandby = m_lpstOnloadRcd->lOnLoadMagLengthStandby;

	theCuEliteOffloader.GetCuEliteOffloadRcd(m_lpstOffloadRcd);

	m_lOffloaderMagClamperDepth = m_lpstOffloadRcd->lOffLoadMagClamperDepth;
	m_lOffloaderMagClamperFrontStandby = m_lpstOffloadRcd->lOffLoadMagClamperFrontStandby;
	m_lOffloaderMagClamperBackStandby = m_lpstOffloadRcd->lOffLoadMagClamperBackStandby;
	m_lOffloaderMagLengthStandby = m_lpstOffloadRcd->lOffLoadMagLengthStandby;

	UpdateData(false);

	return FALSE;
}

HBRUSH CDlgCuEliteElevSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDlgCuEliteElevSetupPage::OnOnloaderMagClamperFrontEdit()
{
	m_nCurCtrl=theCuEliteOnloader.m_ucMagClamperFrontStepId;

	CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, SPEED_FINE,this);
	CuEliteStepperCtrl.DoModal();
}

void CDlgCuEliteElevSetupPage::OnOnloaderMagClamperBackEdit()
{
	m_nCurCtrl=theCuEliteOnloader.m_ucMagClamperBackStepId;

	CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, SPEED_FINE,this);
	CuEliteStepperCtrl.DoModal();
}

void CDlgCuEliteElevSetupPage::OnOnloaderMagClamperLengthEdit()
{
	m_nCurCtrl=theCuEliteOnloader.m_ucMagLengthStepId;

	CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, SPEED_FINE,this);
	CuEliteStepperCtrl.DoModal();
}

void CDlgCuEliteElevSetupPage::OnOffloaderMagClamperFrontEdit()
{
	m_nCurCtrl=theCuEliteOffloader.m_ucMagClamperFrontStepId;

	CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, SPEED_FINE,this);
	CuEliteStepperCtrl.DoModal();
}

void CDlgCuEliteElevSetupPage::OnOffloaderMagClamperBackEdit()
{
	m_nCurCtrl=theCuEliteOffloader.m_ucMagClamperBackStepId;

	CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, SPEED_FINE,this);
	CuEliteStepperCtrl.DoModal();
}

void CDlgCuEliteElevSetupPage::OnOffloaderMagClamperLengthEdit()
{
	m_nCurCtrl=theCuEliteOffloader.m_ucMagLengthStepId;

	CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, SPEED_FINE,this);
	CuEliteStepperCtrl.DoModal();
}

void CDlgCuEliteElevSetupPage::OnOnloaderMagClamperDepth()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lOnloaderMagClamperDepth,300,-100,500);

	m_lpstOnloadRcd->lOnLoadMagClamperDepth = m_lOnloaderMagClamperDepth;
	theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadRcd);

	UpdateData(false);
}

void CDlgCuEliteElevSetupPage::OnEditOnloaderMagClamperFrontPosition()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_lOnloaderMagClamperFrontStandby,  m_lpstOnloadRcd->lOnLoadMagClamperFrontStandby, -3000, 3000) == CANCEL_SELECTED)return;

	m_lpstOnloadRcd->lOnLoadMagClamperFrontStandby = m_lOnloaderMagClamperFrontStandby;
	theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadRcd);

	UpdateData(false);

	GetDlgItem(IDOK)->EnableWindow(false);
	theCuEliteOnloader.OnloadMagClamperFrontMovetoStandyPos();
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteElevSetupPage::OnEditOnloaderMagClamperBackPosition()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_lOnloaderMagClamperBackStandby,  m_lpstOnloadRcd->lOnLoadMagClamperBackStandby, -3000, 3000) == CANCEL_SELECTED)return;

	m_lpstOnloadRcd->lOnLoadMagClamperBackStandby = m_lOnloaderMagClamperBackStandby;
	theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadRcd);

	UpdateData(false);

	GetDlgItem(IDOK)->EnableWindow(false);
	theCuEliteOnloader.OnloadMagClamperBackMovetoStandyPos();
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteElevSetupPage::OnEditOnloaderMagLengthPosition()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_lOnloaderMagLengthStandby,  m_lpstOnloadRcd->lOnLoadMagLengthStandby, -6000, 6000) == CANCEL_SELECTED)return;

	m_lpstOnloadRcd->lOnLoadMagLengthStandby = m_lOnloaderMagLengthStandby;
	theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadRcd);

	UpdateData(false);

	GetDlgItem(IDOK)->EnableWindow(false);
	theCuEliteOnloader.OnloadMagLengthMovetoStandyPos();
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteElevSetupPage::OnOffloaderMagClamperDepth()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lOffloaderMagClamperDepth,300,-100,500);

	m_lpstOffloadRcd->lOffLoadMagClamperDepth = m_lOffloaderMagClamperDepth;
	theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadRcd);

	UpdateData(false);
}

void CDlgCuEliteElevSetupPage::OnEditOffloaderMagClamperFrontPosition()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_lOffloaderMagClamperFrontStandby,  m_lpstOffloadRcd->lOffLoadMagClamperFrontStandby, -3000, 3000) == CANCEL_SELECTED)return;

	m_lpstOffloadRcd->lOffLoadMagClamperFrontStandby = m_lOffloaderMagClamperFrontStandby;
	theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadRcd);

	UpdateData(false);

	GetDlgItem(IDOK)->EnableWindow(false);
	theCuEliteOffloader.OffloadMagClamperFrontMovetoStandyPos();
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteElevSetupPage::OnEditOffloaderMagClamperBackPosition()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_lOffloaderMagClamperBackStandby,  m_lpstOffloadRcd->lOffLoadMagClamperBackStandby, -3000, 3000))return;

	m_lpstOffloadRcd->lOffLoadMagClamperBackStandby = m_lOffloaderMagClamperBackStandby;
	theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadRcd);

	UpdateData(false);

	GetDlgItem(IDOK)->EnableWindow(false);
	theCuEliteOffloader.OffloadMagClamperBackMovetoStandyPos();
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteElevSetupPage::OnEditOffloaderMagLengthPosition()
{
	GetDlgItem(IDC_STATIC_ONLOADER)->SetFocus();

	if(theKeyPad.GetUserKeyInData(&m_lOffloaderMagLengthStandby,  m_lpstOffloadRcd->lOffLoadMagLengthStandby, -6000, 6000)==CANCEL_SELECTED)return;

	m_lpstOffloadRcd->lOffLoadMagLengthStandby = m_lOffloaderMagLengthStandby;
	theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadRcd);

	UpdateData(false);

	GetDlgItem(IDOK)->EnableWindow(false);
	theCuEliteOffloader.OffloadMagLengthMovetoStandyPos();
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteElevSetupPage::OnOk()
{
	m_lpstOnloadRcd->lOnLoadMagClamperFrontStandby = m_lOnloaderMagClamperFrontStandby;
	m_lpstOnloadRcd->lOnLoadMagClamperBackStandby = m_lOnloaderMagClamperBackStandby;
	m_lpstOnloadRcd->lOnLoadMagLengthStandby = m_lOnloaderMagLengthStandby;
	m_lpstOnloadRcd->lOnLoadMagClamperDepth = m_lOnloaderMagClamperDepth;

	theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadRcd);

	m_lpstOffloadRcd->lOffLoadMagClamperFrontStandby = m_lOffloaderMagClamperFrontStandby;
	m_lpstOffloadRcd->lOffLoadMagClamperBackStandby = m_lOffloaderMagClamperBackStandby;
	m_lpstOffloadRcd->lOffLoadMagLengthStandby = m_lOffloaderMagLengthStandby;
	m_lpstOffloadRcd->lOffLoadMagClamperDepth = m_lOffloaderMagClamperDepth;

	theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadRcd);

	((CWireBondDlg*)GetParent())->EnableSystemButton();	
	OnOK();
}

LRESULT CDlgCuEliteElevSetupPage::OnRefresh(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=m_nCurCtrl)return NOERROR;

	CUELITE_DEVICE_RECORD* lpstCuEliteDevicePara=new CUELITE_DEVICE_RECORD;
	theDevice.GetCuEliteRcd(lpstCuEliteDevicePara);

	theStepper.GetPos(m_nCurCtrl, &lParam);
	lParam = round(lParam * theStepper.GetResol(m_nCurCtrl));
	if(lParam)lParam = lParam%10<5 ? lParam/10*10 : (lParam/10+1)*10;

	if(m_nCurCtrl==theCuEliteOnloader.m_ucMagClamperFrontStepId
		||m_nCurCtrl==theCuEliteOnloader.m_ucMagClamperBackStepId
		||m_nCurCtrl==theCuEliteOnloader.m_ucMagLengthStepId)
	{
		if(m_nCurCtrl==theCuEliteOnloader.m_ucMagClamperFrontStepId)
			m_lpstOnloadRcd->lOnLoadMagClamperFrontStandby = m_lOnloaderMagClamperFrontStandby = lParam - (MAG_WIDTH_LIMIT - lpstCuEliteDevicePara->lMagWidth*3) / 20;
		else if(m_nCurCtrl==theCuEliteOnloader.m_ucMagClamperBackStepId)
			m_lpstOnloadRcd->lOnLoadMagClamperBackStandby = m_lOnloaderMagClamperBackStandby = lParam - (MAG_WIDTH_LIMIT - lpstCuEliteDevicePara->lMagWidth*3) / 20;
		else if(m_nCurCtrl==theCuEliteOnloader.m_ucMagLengthStepId)
			m_lpstOnloadRcd->lOnLoadMagLengthStandby = m_lOnloaderMagLengthStandby = lParam - (MAG_LEN_LIMIT - lpstCuEliteDevicePara->lMagLength*3)/10;

		theCuEliteOnloader.UpdateCuEliteOnloadRcd(m_lpstOnloadRcd);
	}
	else if(m_nCurCtrl==theCuEliteOffloader.m_ucMagClamperFrontStepId
		||m_nCurCtrl==theCuEliteOffloader.m_ucMagClamperBackStepId
		||m_nCurCtrl==theCuEliteOffloader.m_ucMagLengthStepId)
	{
		if(m_nCurCtrl==theCuEliteOffloader.m_ucMagClamperFrontStepId)
			m_lpstOffloadRcd->lOffLoadMagClamperFrontStandby = m_lOffloaderMagClamperFrontStandby = lParam - (MAG_WIDTH_LIMIT - lpstCuEliteDevicePara->lMagWidth*3) / 20;
		else if(m_nCurCtrl==theCuEliteOffloader.m_ucMagClamperBackStepId)
			m_lpstOffloadRcd->lOffLoadMagClamperBackStandby = m_lOffloaderMagClamperBackStandby = lParam - (MAG_WIDTH_LIMIT - lpstCuEliteDevicePara->lMagWidth*3) / 20;
		else if(m_nCurCtrl==theCuEliteOffloader.m_ucMagLengthStepId)
			m_lpstOffloadRcd->lOffLoadMagLengthStandby = m_lOffloaderMagLengthStandby = lParam - (MAG_LEN_LIMIT - lpstCuEliteDevicePara->lMagLength*3)/10;

		theCuEliteOffloader.UpdateCuEliteOffloadRcd(m_lpstOffloadRcd);
	}

	delete lpstCuEliteDevicePara;

	return UpdateData(false);
}

void CDlgCuEliteElevSetupPage::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_lpstOnloadRcd!=NULL)delete m_lpstOnloadRcd;
	if(m_lpstOffloadRcd!=NULL)delete m_lpstOffloadRcd;
}
