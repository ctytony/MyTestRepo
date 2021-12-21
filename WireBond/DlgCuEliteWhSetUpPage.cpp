// DlgCuEliteWHSetupPage.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DlgCuEliteWHSetupPage.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "MachineConfig.h"
#include "PopUpCuEliteStepperCtr.h"
#include "WireBondDlg.h"
#include "MathCalc.h"
#include "ScanKeyThread.h"
#include "CuIndex.h"
#include "R2RIndex.h"
#include "CuClamp.h"
#include "servo.h"

// CDlgCuEliteWHSetupPage 对话框

IMPLEMENT_DYNAMIC(CDlgCuEliteWHSetupPage, CDialog)

CDlgCuEliteWHSetupPage::CDlgCuEliteWHSetupPage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCuEliteWHSetupPage::IDD, pParent)
	, m_lIndexLogicZero(0)
	, m_lIndexRightLimit(0)
	, m_nWhTbX(0)
	, m_nWhTbY(0)
	, m_nLeadFrameTrackType(0)
	, m_lFrontTrackStandby(0)
	, m_lBackTrackStandby(0)
{
	m_nCurCtrl=0;
}

CDlgCuEliteWHSetupPage::~CDlgCuEliteWHSetupPage()
{
}

void CDlgCuEliteWHSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INDEX_LOGIC_ZERO, m_lIndexLogicZero);
	DDX_Text(pDX, IDC_EDIT_INDEX_RIGHT_LIMIT, m_lIndexRightLimit);
	DDX_Text(pDX, IDC_EDIT_WH_TB_X, m_nWhTbX);
	DDX_Text(pDX, IDC_EDIT_WH_TB_Y, m_nWhTbY);
	DDX_Text(pDX, IDC_EDIT_FRONT_TRACK_POSITION, m_lFrontTrackStandby);
	DDX_Text(pDX, IDC_EDIT_BACK_TRACK_POSITION, m_lBackTrackStandby);
}

BEGIN_MESSAGE_MAP(CDlgCuEliteWHSetupPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_START_TCH_TABLE, OnStartTchTable)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
	ON_EN_SETFOCUS(IDC_EDIT_FRONT_TRACK_POSITION, OnFrontTrackPosition)
	ON_EN_SETFOCUS(IDC_EDIT_BACK_TRACK_POSITION, OnBackTrackPosition)
	ON_BN_CLICKED(IDC_BUTTON_FRONT_TRACK_EDIT, OnFrontTrackEdit)
	ON_BN_CLICKED(IDC_BUTTON_BACK_TRACK_EDIT, OnBackTrackEdit)
	ON_BN_CLICKED(IDC_BUTTON_FRONT_TRACK_HOME, OnFrontHome)
	ON_BN_CLICKED(IDC_BUTTON_BACK_TRACK_HOME, OnBackHome)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_FRONT_STANDBY, OnFrontStandby)
	ON_BN_CLICKED(IDC_BUTTON_BACK_STANDBY, OnBackStandby)
	ON_MESSAGE(WM_REFRESH,OnRefresh)
END_MESSAGE_MAP()

// CDlgCuEliteWHSetupPage 消息处理程序
BOOL CDlgCuEliteWHSetupPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	((CWireBondDlg*)GetParent())->InhibitSystemButton();

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);

	CPoint pt(Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP);
	SetWindowPos(NULL, pt.x, pt.y, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT));
	pWnd->EnableWindow(FALSE);

	SetDlgItemTextW(IDC_STATIC_INDEXER_ALIGN, theMsg.GetMsg(MSG_INDEXER_ALIGN));
	SetDlgItemTextW(IDC_STATIC_INDEXER_LOGIC_0, theMsg.GetMsg(MSG_LOGIC_ZERO_TO_HOME));
	SetDlgItemTextW(IDC_STATIC_INDEXER_RIGHT_LIMIT, theMsg.GetMsg(MSG_LOGIC_RIGHT_LIMIT_TO_HOME));
	SetDlgItemTextW(IDC_STATIC_TABLE_SAFE_POS, theMsg.GetMsg(MSG_TCH_SAFE_POS));
	SetDlgItemTextW(IDC_STATIC_FRONT_TRACK_POSITION, theMsg.GetMsg(MSG_FRONT_TRACK_POS));
	SetDlgItemTextW(IDC_STATIC_BACK_TRACK_POSITION, theMsg.GetMsg(MSG_BACK_TRACK_POS));
	SetDlgItemTextW(IDC_BUTTON_FRONT_TRACK_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BUTTON_BACK_TRACK_EDIT, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BUTTON_FRONT_TRACK_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_BACK_TRACK_HOME, theMsg.GetMsg(MSG_HOME));
	SetDlgItemTextW(IDC_BUTTON_FRONT_STANDBY, theMsg.GetMsg(MSG_STANDBY_POS));
	SetDlgItemTextW(IDC_BUTTON_BACK_STANDBY, theMsg.GetMsg(MSG_STANDBY_POS));

	SetDlgItemTextW(IDC_STATIC_TABLE_X,_T("X : "));
	SetDlgItemTextW(IDC_STATIC_TABLE_Y,_T("Y : "));

	SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));

	//display help message
	SetDlgItemTextW(IDC_STATIC_INFO_SAFE,_T(""));

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	m_lIndexLogicZero = pstMcData->lZeroOffset;		//length from physical home sensor
	m_lIndexRightLimit = pstMcData->lIndexerLimit;	//length from physical home sensor
	m_nWhTbX = (int)pstMcData->stTbSafeStandby.dx;	//V3.3.194 edit
	m_nWhTbY = (int)pstMcData->stTbSafeStandby.dy;	//V3.3.194 edit

	CUELITE_TRACK_DATA* lpstTrackRcd=new CUELITE_TRACK_DATA;
	theCuEliteTrack.GetCuEliteTrackRcd(lpstTrackRcd);

	m_lFrontTrackStandby = lpstTrackRcd->lLFFrontTrackStandby;
	m_lBackTrackStandby = lpstTrackRcd->lLFBackTrackStandby;

	delete lpstTrackRcd;

	UpdateData(false);

	return FALSE;
}

HBRUSH CDlgCuEliteWHSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDlgCuEliteWHSetupPage::OnStart()
{
	long lPos;
	BOOL bHome=0, bPLimit=0, bNLimit=0;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	theCuEliteTrack.SearchIndexStepHome();
	Sleep_x1ms(100);

	theCuEliteTrack.WaitStepHomeFinish();
	Sleep_x1ms(100);

	CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_DEVICE_INDEXER);
	theStepperCtrler.DoModal();

	Sleep_x1ms(100);

	theStepper.GetPos(CUELITE_DEVICE_INDEXER,&lPos);

	m_lIndexLogicZero =round(lPos *theStepper.GetResol(CUELITE_DEVICE_INDEXER));
	UpdateData(false);

	theCuEliteTrack.ChangeStepperMotionProfile(SRCH_SPEED_PROFILE);

	Sleep_x1ms(100);

	theStepper.MoveStep(CUELITE_DEVICE_INDEXER, 1000000, MOTION_RELATIVE);
	do
	{
		theStepper.GetMotorStopReason(CUELITE_DEVICE_INDEXER,&bHome,&bNLimit,&bPLimit);
	}
	while(bPLimit == FALSE);

	theStepper.MoveStep(CUELITE_DEVICE_INDEXER, -5000, MOTION_RELATIVE);

	do
	{
		Sleep_x1ms(10);
	} while (theStepper.IsBusy(CUELITE_DEVICE_INDEXER));

	theStepper.GetPos(CUELITE_DEVICE_INDEXER,&lPos);

	m_lIndexRightLimit =round((double) lPos * theStepper.GetResol(CUELITE_DEVICE_INDEXER));
	UpdateData(false);

	Sleep_x1ms(100);

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	pstMcData->lZeroOffset = m_lIndexLogicZero;//length from physical home sensor
	pstMcData->lIndexerLimit = m_lIndexRightLimit;//length from physical home sensor

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteWHSetupPage::OnStartTchTable()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		theServo.MoveTable2(theMcData.GetRcd()->stTbSafeStandby);	//v3.1.T212 edit

	Sleep_x1ms(100);

	SetDlgItemTextW(IDC_STATIC_INFO_SAFE, theMsg.GetMsg(MSG_MOVE_TO_SAFE_POS));

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteWHSetupPage::OnNext()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
	{
		TBL_POS stSafeTblPos;
		theServo.GetTablePosition(&stSafeTblPos);
		m_nWhTbX = (int)stSafeTblPos.dx;	//V3.3.194 edit
		m_nWhTbY = (int)stSafeTblPos.dy;	//V3.3.194 edit
	}

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	pstMcData->stTbSafeStandby.dx = m_nWhTbX;
	pstMcData->stTbSafeStandby.dy = m_nWhTbY;

	SetDlgItemTextW(IDC_STATIC_INFO_SAFE, theMsg.GetMsg(MSG_FINISH));

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
	//MSG_TBL_POS_OUT_OF_LIMIT

	UpdateData(false);
}

void CDlgCuEliteWHSetupPage::OnFrontTrackPosition()
{
	CUELITE_TRACK_DATA* lpstTrackRcd=new CUELITE_TRACK_DATA;
	theCuEliteTrack.GetCuEliteTrackRcd(lpstTrackRcd);

	GetDlgItem(IDC_STATIC_TRACK)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lFrontTrackStandby,  lpstTrackRcd->lLFFrontTrackStandby, -1000, 1000);
	lpstTrackRcd->lLFFrontTrackStandby = m_lFrontTrackStandby;

	theCuEliteTrack.UpdateCuEliteTrackRcd(lpstTrackRcd);
	delete lpstTrackRcd;

	UpdateData(false);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	short index[1]={MSG_SURE_TO_PRECLAMP};
	if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
	{
		theCuEliteLFClamp.PreCloseClamp();
		theCuEliteTrack.MoveFrontTrackToStandbyPos();
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteWHSetupPage::OnBackTrackPosition()
{
	GetDlgItem(IDC_STATIC_TRACK)->SetFocus();

	CUELITE_TRACK_DATA* lpstTrackRcd=new CUELITE_TRACK_DATA;

	theCuEliteTrack.GetCuEliteTrackRcd(lpstTrackRcd);
	theKeyPad.GetUserKeyInData(&m_lBackTrackStandby,  lpstTrackRcd->lLFBackTrackStandby, -1000, 1000);
	lpstTrackRcd->lLFBackTrackStandby = m_lBackTrackStandby;
	theCuEliteTrack.UpdateCuEliteTrackRcd(lpstTrackRcd);

	delete lpstTrackRcd;

	UpdateData(false);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	short index[1]={MSG_SURE_TO_PRECLAMP};
	if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
	{
		theCuEliteLFClamp.PreCloseClamp();
		theCuEliteTrack.MoveBackTrackToStandbyPos();
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteWHSetupPage::OnFrontTrackEdit()
{
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->SetFocus();

	short index[1]={MSG_SURE_TO_PRECLAMP};
	if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
	{
		theCuEliteLFClamp.PreCloseClamp();
		m_nCurCtrl=theCuEliteTrack.m_ucFrontTrackStepperId;
		CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, CUELITE_SPEED_FINE,this);
		CuEliteStepperCtrl.DoModal();
	}
}

void CDlgCuEliteWHSetupPage::OnBackTrackEdit()
{
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->SetFocus();

	short index[1]={MSG_SURE_TO_PRECLAMP};
	if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
	{
		theCuEliteLFClamp.PreCloseClamp();
		m_nCurCtrl=theCuEliteTrack.m_ucBackTrackStepperId;
		CPopUpCuEliteStepperCtr CuEliteStepperCtrl(m_nCurCtrl, CUELITE_SPEED_FINE,this);
		CuEliteStepperCtrl.DoModal();
	}
}

void CDlgCuEliteWHSetupPage::OnOk()
{
	UpdateData(TRUE);

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	pstMcData->lZeroOffset = m_lIndexLogicZero;
	pstMcData->lIndexerLimit = m_lIndexRightLimit;

	pstMcData->stTbSafeStandby.dx = m_nWhTbX;
	pstMcData->stTbSafeStandby.dy = m_nWhTbY;

	CUELITE_TRACK_DATA* lpstTrackRcd=new CUELITE_TRACK_DATA;
	theCuEliteTrack.GetCuEliteTrackRcd(lpstTrackRcd);

	theCuEliteTrack.UpdateCuEliteTrackRcd(lpstTrackRcd);
	delete lpstTrackRcd;

	((CWireBondDlg*)GetParent())->EnableSystemButton();	
	OnOK();
}

BYTE CDlgCuEliteWHSetupPage::RemovePlateAndClamper(BYTE nStepper)
{
	BOOL bHome, bPLimit, bNLimit;
	theStepper.GetHomeSensor(nStepper,&bHome,&bNLimit,&bPLimit);

	if(bHome==FALSE)return FALSE;

	CStringList sList;
	sList.AddTail(theMsg.GetMsg(MSG_REMOVE_TOP_PLATE_WINDOW_CLAMP));
	theMsg.ShowMsg(WARNING_MSG, sList);

	theServo.MoveBondHeadToSafePos();
	Sleep_x1ms(100);

	theCuEliteLFClamp.SearchHome();
	Sleep_x1ms(100);
	theCuEliteLFClamp.WaitHomeFinish();
	Sleep_x1ms(100);

	sList.RemoveAll();
	sList.AddTail(theMsg.GetMsg(MSG_IS_TOP_PLATE_WINDOW_CLAMP_REMOVED));

	bHome = theMsg.ShowMsg(WARNING_MSG,sList,TRUE) != SHOW_MSG_RESULT_STOP;

	theCuEliteLFClamp.CloseClamp();
	Sleep_x1ms(600);

	return (BYTE)bHome;
}

void CDlgCuEliteWHSetupPage::OnFrontHome()
{
	if(RemovePlateAndClamper(CUELITE_FRONT_RAIL)==FALSE)return;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	theCuEliteTrack.MoveFrontTrackHome();
	Sleep(100);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteWHSetupPage::OnBackHome()
{
	if(RemovePlateAndClamper(CUELITE_REAR_RAIL)==FALSE)return;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	theCuEliteTrack.MoveBackTrackHome();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteWHSetupPage::OnFrontStandby()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	theCuEliteTrack.MoveFrontTrackToStandbyPos();
	theCuEliteTrack.WaitFrontTrackStepHomeFinish();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

void CDlgCuEliteWHSetupPage::OnBackStandby()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);

	theCuEliteTrack.MoveBackTrackToStandbyPos();
	theCuEliteTrack.WaitBackTrackStepHomeFinish();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_EDIT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FRONT_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_BACK_TRACK_HOME)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
}

LRESULT CDlgCuEliteWHSetupPage::OnRefresh(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=m_nCurCtrl)return NOERROR;

	CUELITE_TRACK_DATA* lpstTrackRcd=new CUELITE_TRACK_DATA;
	theCuEliteTrack.GetCuEliteTrackRcd(lpstTrackRcd);

	CUELITE_DEVICE_RECORD* lpstCuEliteDevicePara=new CUELITE_DEVICE_RECORD;
	theDevice.GetCuEliteRcd(lpstCuEliteDevicePara);

	theStepper.GetPos(m_nCurCtrl, &lParam);
	lParam = round(lParam * theStepper.GetResol(m_nCurCtrl)); 
	if(lParam)lParam = lParam%10<5 ? lParam/10*10 : (lParam/10+1)*10;

	if(m_nCurCtrl==theCuEliteTrack.m_ucFrontTrackStepperId)
	{
		if (lpstTrackRcd->iLFTrackRange == 0)
			m_lFrontTrackStandby = lParam - ((FRONT_TRACK_LIMIT - lpstCuEliteDevicePara->lDeviceTotalWidth)/20 - lpstCuEliteDevicePara->lDeviceCenterOffset);
		else
			m_lFrontTrackStandby = lParam - ((FRONT_TRACK_LIMIT - lpstCuEliteDevicePara->lDeviceTotalWidth)/20 + 1000 - lpstCuEliteDevicePara->lDeviceCenterOffset);

		lpstTrackRcd->lLFFrontTrackStandby = m_lFrontTrackStandby;
	}
	else if(m_nCurCtrl==theCuEliteTrack.m_ucBackTrackStepperId)
	{
		if (lpstTrackRcd->iLFTrackRange == 0)
			m_lBackTrackStandby = lParam - ((BACK_TRACK_LIMIT - lpstCuEliteDevicePara->lDeviceTotalWidth)/20 - lpstCuEliteDevicePara->lDeviceCenterOffset);
		else
			m_lBackTrackStandby = lParam - ((BACK_TRACK_LIMIT - lpstCuEliteDevicePara->lDeviceTotalWidth)/20 + 1000 - lpstCuEliteDevicePara->lDeviceCenterOffset);

		lpstTrackRcd->lLFBackTrackStandby = m_lBackTrackStandby;
	}

	theCuEliteTrack.UpdateCuEliteTrackRcd(lpstTrackRcd);

	delete lpstTrackRcd;
	delete lpstCuEliteDevicePara;

	UpdateData(false);

	return NOERROR;
}
