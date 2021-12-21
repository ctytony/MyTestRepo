/***************************************************************
WireBondDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "Math.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "DataStruct.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "Wire.h"
#include "BondPara.h"
#include "BallPara.h"
#include "AutoPadMgr.h"
#include "Bond.h"
#include "MsgManager.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "MenuConfig.h"
#include "Msgdef.h"
#include "visioncalibdlg.h"
#include "AutoBondSheet.h"
#include "DebugDlg.h"
#include "DummyProgram.h"
#include "BondParaSheet.h"
#include "BallParaSheet.h"
//#include "LoopParaSheet.h"	//V1.5.W170 delete
#include "BondSetupSheet.h"
#include "TeachRecipeSheet.h"
#include "ServoConfigSheet.h"
#include "MhsConfigSheet.h"
#include "BondProcessSheet.h"
#include "AdvParaMapDlg.h"
#include "ParaMapBase.h"
#include "LightingCtrlDlg.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "BondHeadCalibSheet.h"
#include "ScanKeyThread.h"
#include "VisionCfgDlg.h"
#include "JoystickSpeedDlg.h"
#include "StickDetectDlg.h"
#include "VisionSystem.h"
#include "ThreadWireDlg.h"
#include "PositionAdjustDlg.h"
#include "InitDlg.h"
#include "FileOpenDlg.h"
#include "SoftwareMgrDlg.h"
#include "VisionParaSheet.h"
#include "DebugToolSheet.h"
#include "MachineConfig.h"
#include "BondStatistics.h"
#include "UPSWarnPage.h"
#include "MhsUtilitySheet.h"
#include "CtactSrchTestPage.h"
#include "UsgControl.h"
#include "EfoProfile.h"
#include "AfpToolSheet.h"
#include "DlgElevCtrl.h"
#include "TeachWorkHolderSheet.h"
#include "DlgTopWhSetUpPage.h"
#include "SleepModeDlg.h"
#include "UsgDebugPage.h"
#include "BondToolsSheet.h"
#include "DigitalIO.h"
#include "UsbBsdDebugPage.h"
#include "ChgCapillaryDlg.h"
#include "VisionDisplay.h"
#include "HardwareTestSheet.h"
#include "EfoControl.h"
#include "UsgInfoDispPage.h"
#include "FnkCaliPage.h"
#include "LoopParaBaseDlg.h"
#include "UsgSetupDlg.h"
#include "DummyBond.h"
#include "MatrixSchedule.h"
#include "TeachCuEliteWorkHolderSheet.h"
#include "DlgCuEliteElevCtrl.h"
#include "MachineConfig.h"
#include "DeviceData.h"
#include "TeachR2RWorkHolderSheet.h"
#include "R2RWhSetUpPage.h"
#include "DlgCuEliteElevSetupPage.h"
#include "DlgCuEliteWHSetupPage.h"
#include "Register.h"
#include "Jpeg.h"
#include "VledClamp.h"
#include "CuIndex.h"
#include "CuClamp.h"
#include "CuOnload.h"
#include "CuOffload.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "servo.h"
#include "Debug.h"
#include "User.h"
#include "ColorMenu.h"	//V1.1.W38 add
#include "ForceSwitchSheet.h"
#include "TopEliteIndex.h"
#include <memory>
#include <shellapi.h>
#include "busy.h"	//v3.1.T5 add

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool gbAPC = false;
BYTE gnAlgorithm = ALGORITHM_BLOB_PLUS_EDGE;

CWireBondDlg::CWireBondDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWireBondDlg::IDD, pParent)
, m_bButtonInhibited(false)
, m_iUserLevel(0)
, m_nScreenType(1)
, m_lOxidationPreventTimer(0)
, m_nN2h2TimeInterval(0)
, m_bProcessKeyboard(false)
, m_b1stMeasurePtSelected(false)	//v3.0.T313 add
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nMenuTreeIndex = -1;
    m_CurPos.dx = -56789.4;
    m_CurPos.dy = -56789.6;
    m_dCurBondHeadLevel = 3456.6;

	m_pInitDlg = NULL;
	m_hwndClient = NULL;

	m_pDlgOverView = NULL; //V1.1.W116 add
	m_hwndFront = NULL;		//V1.1.W142 add

	GdiplusStartupInput gsi;
	GdiplusStartup(&m_lToken,&gsi,NULL);

	m_pDlgSpider = NULL;	//V1.1.W161 add
}

CWireBondDlg::~CWireBondDlg()
{
	GdiplusShutdown(m_lToken);
}

void CWireBondDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMG, m_Image);

	DDX_Control(pDX, IDC_TOOL_BAR_UNPLUG, m_btnUnplug);
	DDX_Control(pDX, ID_TOOL_BAR_LIGHT, m_btnLight);
	DDX_Control(pDX, ID_TOOL_BAR_INDEX, m_btnIndex);
	DDX_Control(pDX, ID_TOOL_BAR_FINE_ADJUST, m_btnAdjust);
	DDX_Control(pDX, ID_TOOL_BAR_THREAD_WIRE, m_btnWire);
	DDX_Control(pDX, ID_TOOL_BAR_ELEV, m_btnElevator);
	DDX_Control(pDX, IDC_BUTTON_SPEED_UP, m_btnUp);
	DDX_Control(pDX, IDC_BUTTON_SPEED_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BUTTON_SETUP, m_btnSetup);
	DDX_Control(pDX, IDC_BUTTON_PROGRAM, m_btnProgram);
	DDX_Control(pDX, IDC_BUTTON_BOND, m_btnBond);
	DDX_Control(pDX, IDC_BUTTON_DEBUG, m_btnDebug);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_btnConfig);
}

BEGIN_MESSAGE_MAP(CWireBondDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SETUP, OnSetup)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM, OnProgram)
	ON_BN_CLICKED(IDC_BUTTON_BOND, OnBond)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, OnConfig)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MENU_TREE, OnMenuTree)
    ON_BN_CLICKED(ID_TOOL_BAR_LIGHT, OnLight)
    ON_BN_CLICKED(ID_TOOL_BAR_INDEX, OnIndex)
    ON_BN_CLICKED(ID_TOOL_BAR_FINE_ADJUST, OnAdjust)
    ON_BN_CLICKED(ID_TOOL_BAR_THREAD_WIRE, OnThread)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
	ON_COMMAND_RANGE(IDM_OPERATOR, IDM_ENGINEER, OnUserLvl)			//V1.1.W38 edit
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SPEED_UP, OnSpeedUp)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_DOWN, OnSpeedDown)
	ON_BN_CLICKED(ID_TOOL_BAR_ELEV, OnElevator)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_DEBUG, OnDebug)
    ON_BN_CLICKED(IDC_TOOL_BAR_UNPLUG, OnUnplug)
	ON_MESSAGE(WM_CLICK_KEYBOARDSHORTCUT,OnClickKeyboardShortcut)
END_MESSAGE_MAP()

DWORD WINAPI CWireBondDlg::InitProc(LPVOID lpVoid)	//V3.3.W155 add
{
	if (lpVoid == NULL) return NOERROR;

	CWireBondDlg* pThis = (CWireBondDlg*)theApp.m_pMainWnd;

	if (pThis->m_pInitDlg == NULL)
	{
		pThis->m_pInitDlg = new CInitDlg();
		pThis->m_pInitDlg->Create(CInitDlg::IDD);
	}

	SleepEx_x1ms((LPDWORD)lpVoid);

	pThis->m_pInitDlg->DestroyWindow();
	pThis->m_pInitDlg = NULL;

	return NOERROR;
}

void CWireBondDlg::ConvertRecipeName(LPWSTR psBuf, BYTE nSize)
{
	BYTE bDirty = FALSE;

	PGM_RECORD stPgmRcd;
	thePgm.GetRcd(&stPgmRcd);

	if ((lstrlenA((LPSTR)stPgmRcd.cPgmName) == 1)
		&& (lstrlenA((LPSTR)(stPgmRcd.cPgmName + 1)) == 1)
		&& (lstrlen(stPgmRcd.cPgmName) > 4))
	{
		bDirty = TRUE;
		CString str = stPgmRcd.cPgmName;
		wcstombs_s(NULL, (LPSTR)stPgmRcd.cPgmName, sizeof(stPgmRcd.cPgmName), str, sizeof(stPgmRcd.cPgmName));
	}

	if ((psBuf != NULL) && (nSize > 0))
	{
		bDirty = TRUE;
		memcpy(&stPgmRcd.bFabTest, psBuf, nSize);
	}


	if (bDirty == TRUE) thePgm.SetRcd(&stPgmRcd);
}

BYTE CWireBondDlg::CloseFrontWindow()	//V1.1.W144 edit
{
	return (m_hwndFront == NULL)
			|| (IsWindow(m_hwndFront) == FALSE)
			|| (::SendMessage(m_hwndFront,WM_CLOSEDIALOG,0,0) == DIALOG_CLOSED);
}

BYTE CWireBondDlg::HideFrontWindow()	//V1.1.W155 add
{
	if (m_pIndexCtrlDlg->IsWindowVisible() == FALSE) return TRUE;

	return m_pIndexCtrlDlg->SendMessage(WM_CLOSEDIALOG) == DIALOG_CLOSED;
}

BYTE CWireBondDlg::SwapFrontWindow(HWND hwndFront)	//V1.1.W144 edit
{
	if (CloseFrontWindow() == FALSE) return FALSE;

	m_hwndFront = hwndFront;

	return TRUE;
}

// CWireBondDlg message handlers
BOOL CWireBondDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetFont(theApp.GetFont());

	theApp.SetDlgFont(this);	//V1.1.W139 add

	m_ToolTip.Create(this);

 	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    m_ImageList.Create(16,16,ILC_COLOR8,8,8);
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_FOLDER));		//V1.1.W118 edit
    m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_FOLDER));		//V1.1.W118 edit

	SetWindowPos(NULL, APP_DLG_LEFT, APP_DLG_TOP, APP_DLG_WIDTH, APP_DLG_HEIGHT, SWP_NOZORDER);

	m_pInfoView = GetDlgItem(IDC_INFO_VIEW);
	m_pInfoView->SetWindowPos(NULL, INFO_VIEW_LEFT, INFO_VIEW_TOP, INFO_VIEW_WIDTH, INFO_VIEW_HEIGHT, SWP_NOZORDER);

    m_pDlgOverView = new CDlgOverView();
	m_pDlgOverView->Create(IDD_OVERVIEW, this);
	m_pDlgOverView->SetWindowPos(NULL, OVER_VIEW_LEFT, OVER_VIEW_TOP, OVER_VIEW_WIDTH, OVER_VIEW_HEIGHT, SWP_NOZORDER);

    m_pDlgSpider = new CDlgSpider();
	m_pDlgSpider->Create(IDD_SPIDER, &m_Image);
	m_pDlgSpider->ShowWindow(SW_SHOW);

	m_Image.SetWindowPos(NULL, VISION_VIEW_LEFT, VISION_VIEW_TOP, VISION_VIEW_WIDTH, VISION_VIEW_HEIGHT, SWP_NOZORDER);

	GetScreenResolution();

	int nPos = VISION_VIEW_LEFT + VISION_VIEW_WIDTH - BIG_BUTTON_BITMAP_LEN;
	m_btnDown.SetBkColor(theApp.m_bkColor);
	m_btnDown.SetImage(IDB_DOWN);
    m_btnDown.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BIG_BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos -= BIG_BUTTON_BITMAP_LEN;
	m_btnUp.SetBkColor(theApp.m_bkColor);
	m_btnUp.SetImage(IDB_UP);
    m_btnUp.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BIG_BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED: case MHS_CU_ELITE:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		nPos -= BUTTON_BITMAP_LEN;
		m_btnElevator.SetBkColor(theApp.m_bkColor);
		m_btnElevator.SetImage(IDB_ELEVATOR);
		m_btnElevator.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);
		break;
	case MHS_VLED: case MHS_VLED_ELITE: case MHS_R2R:
	case MHS_FOXCONN:	//v3.0.T319 add
		m_btnElevator.ShowWindow(FALSE);
		break;
	}

	nPos -= BUTTON_BITMAP_LEN;
	m_btnWire.SetBkColor(theApp.m_bkColor);
	m_btnWire.SetImage(IDB_THREAD);
    m_btnWire.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos -= BUTTON_BITMAP_LEN;
	m_btnAdjust.SetBkColor(theApp.m_bkColor);
	m_btnAdjust.SetImage(IDB_ADJUST);
    m_btnAdjust.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos -= BUTTON_BITMAP_LEN;
	m_btnIndex.SetBkColor(theApp.m_bkColor);
	m_btnIndex.SetImage(IDB_INDEX);
    m_btnIndex.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos -= BUTTON_BITMAP_LEN;
	m_btnLight.SetBkColor(theApp.m_bkColor);
	m_btnLight.SetImage(IDB_LIGHT);
    m_btnLight.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos -= BUTTON_BITMAP_LEN;
	m_btnUnplug.SetBkColor(theApp.m_bkColor);
	m_btnUnplug.SetImage(IDB_UNPLUG);
    m_btnUnplug.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP, BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos = MENU_TREE_LEFT;
	m_btnSetup.SetBkColor(theApp.m_bkColor);
	m_btnSetup.SetImage(IDB_BITMAP_SETUP);
    m_btnSetup.SetWindowPos(NULL, nPos, BUTTON_SETUP_TOP,
		BIG_BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos += BIG_BUTTON_BITMAP_LEN;
	m_btnProgram.SetBkColor(theApp.m_bkColor);
	m_btnProgram.SetImage(IDB_BITMAP_PGM);
    m_btnProgram.SetWindowPos(NULL, nPos, BUTTON_PROGRAM_TOP,
		BIG_BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos += BIG_BUTTON_BITMAP_LEN;
	m_btnBond.SetBkColor(theApp.m_bkColor);
	m_btnBond.SetImage(IDB_BITMAP_RUN);
    m_btnBond.SetWindowPos(NULL, nPos, BUTTON_BOND_TOP,
		BIG_BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos += BIG_BUTTON_BITMAP_LEN;
	m_btnDebug.SetBkColor(theApp.m_bkColor);
	m_btnDebug.SetImage(IDB_BITMAP_DEBUG);
    m_btnDebug.SetWindowPos(NULL, nPos , BUTTON_CONFIG_TOP,
		BIG_BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

	nPos += BIG_BUTTON_BITMAP_LEN;
	m_btnConfig.SetBkColor(theApp.m_bkColor);
	m_btnConfig.SetImage(IDB_BITMAP_CONFIG);
    m_btnConfig.SetWindowPos(NULL, nPos, BUTTON_CONFIG_TOP,
		BIG_BUTTON_BITMAP_LEN, BUTTON_BITMAP_SIZE, SWP_NOZORDER);

    GetDlgItem(IDC_MENU_TREE)->SetWindowPos(NULL, MENU_TREE_LEFT,
		MENU_TREE_TOP, MENU_TREE_WIDTH, MENU_TREE_HEIGHT, SWP_NOZORDER);
	UpdateMenuTree(MENU_TREE_SETUP);
	SetTooltip();	//V1.1.W149 add

	m_pIndexCtrlDlg = new CIndexCtrlDlg;
	m_pIndexCtrlDlg->Create(CIndexCtrlDlg::IDD);     
	m_pIndexCtrlDlg->ShowWindow(SW_HIDE);

	m_pLightCtrlDlg = new CLightingCtrlDlg;
	m_pLightCtrlDlg->Create(CLightingCtrlDlg::IDD);
	m_pLightCtrlDlg->ShowWindow(SW_HIDE);

    m_iUserLevel = theUser.GetUserLevel();
    
	//Vision, Servo, MHS power on
	if (!theMcConfig.IsBypassMHS()		//v3.1.T172 add
		&& theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
	{
		theStepperIO.UpdateInputState(CANADDR_IO);
		if (theStepperIO.GetPortState(theStepperIO.m_Input.m_Emergency))		//V1.1.W161 edit
		{
			short anMsgArray[1] = {MSG_EMERGENCY_PRESSED};
			theMsg.ShowMsg(ERROR_MSG, 1, anMsgArray);
		}
	}

	if (SystemPowerOn() == FALSE)
	{
		DestroyWindow();
		return FALSE;
	}

	theServo.GetTablePosition(&m_stLastPos);

	if (!theMcConfig.IsBypassUSG()) ShowUsgInfoDialog();	//v3.0.T392 add

    SetTimer(1, TIMER1_INTERVAL, NULL);
    SetTimer(2, TIMER2_INTERVAL, NULL);

	ConvertRecipeName(NULL, 0);

	ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 add

	return SetClassLong(m_hWnd,GCL_HCURSOR,NULL);	//V1.5.W106 edit, v1.5.T136 delete, v1.6.T3 add
	//return FALSE;		//v1.5.T136 add, v1.6.T3 delete
}

int CWireBondDlg::SystemPowerOn(void)
{
	DWORD dwTimeout = -1;
	HANDLE hThread = CreateThread(NULL,0,InitProc,&dwTimeout,0,NULL);
	Sleep_x1ms(200);

	if (m_pInitDlg == NULL)
	{
		//Signal quit
		dwTimeout = 0;
		WaitThread(hThread,100);		//V3.3.W180 add

		return FALSE;
	}

	int iStatus;
	CStringList sList;

	if (!theMcConfig.IsBypassAll())	//v3.0.T392 add
    {
		//init vision system
		m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_VISION));
		Sleep_x1ms(1000);
    	iStatus = theVisionSys.Init(m_Image.m_hWnd);	//v1.6.T339 edit
    	if (iStatus != VISION_OK)
    	{
			CString str;
			str.Format(_T("Vision initialize error: %d"), iStatus);
			m_pInitDlg->ShowInfoMsg(str.GetBuffer());
			Sleep_x1ms(1000);
    	}
    }

	//load power-on parameter
	m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_LOAD_POWERON_PARA));
	Sleep_x1ms(1000);
	theApp.ArchivePowerOnData(ARCHIVE_LOAD);

    if (!theMcConfig.IsBypassCamera())
    {
		//load pr pattern
		m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_LOAD_PR_PATTERN));
		Sleep_x1ms(1000);
		iStatus = theVisionSys.LoadPatternRcd();	//v3.0.T70 edit
		if (iStatus != VISION_OK)
		{
			CString str;
			str.Format(_T("Load Vision Pattern error: %d"), iStatus);
			m_pInitDlg->ShowInfoMsg(str.GetBuffer());
			Sleep_x1ms(1000);
		}

		Sleep_x1ms(10);
		theBond.SetPrPatternPassScore();
    }

	//init bonder
	m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_BONDER));
	Sleep_x1ms(1000);
	theBond.Init();

	if (!theMcConfig.IsBypassEFO())	//v3.0.T392 add
    {
		//init efo
		m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_EFO));
		Sleep_x1ms(1000);
		if (theEFO.Init() == -1)	//V1.1.W254 edit, v1.6.T170 edit
		{
			m_pInitDlg->ShowInfoMsg(_T("EFO com connection failed!"));	//v1.6.T170 add
			//::AfxMessageBox(_T("EFO Connection fail!"));	//v1.6.T170 delete
		}
    }

    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
	{
		//init wire clamp
		m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_WIRE_CLAMP));
		Sleep_x1ms(1000);
		theServo.InitWireClamp();
	}

	if (!theMcConfig.IsBypassMHS())			//v3.1.T172 add
		theMHS.BeginThread();

	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		//Bond Head go to upper limit, then Y motor go to positive limit 
	    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		{
			//move bonding to home
			if (theServo.ServoPreparePowerOn())
			{
				sList.AddTail(theMsg.GetMsg(MSG_SERVO_ERROR));
				theMsg.ShowMsg(ERROR_MSG, sList);		

				//Signal quit
				dwTimeout = 0;
				WaitThread(hThread,100);		//V3.3.W180 edit

				return FALSE;
			}
#if _SERVO_TYPE == 2	//v3.1.T474 add
			theServo.MoveBondHeadToSafePos();
#endif
		}

		if (!theMcConfig.IsBypassMHS())			//v3.1.T172 add
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_MHS));
			Sleep_x1ms(1000);
			theMHS.WarmReset();
		}

		if (!theMcConfig.IsBypassAll())
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_XY_TABLE));
			Sleep_x1ms(1000);
			theServo.PowerOnServo();
            theBond.MoveBondHeadToFireLevel();

            long lStartTime = GetCurrentTime();
			do 
			{
				if ((theStepper.IsBusy(theTopOnloader.m_nMagHolderStepperId) == false)
					&& (theStepper.IsBusy(theTopOffloader.m_nMagHolderStepperId) == false))
					break;

                if (GetCurrentTime() - lStartTime > 6000) break;
			} while (TRUE);
		}
		break;
	}
	case MHS_FOXCONN:	//v3.0.T178 add
	{
		//Bond Head go to upper limit, then Y motor go to positive limit 
	    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		{
			//move bonding to home
			if( theServo.ServoPreparePowerOn())
			{
				sList.AddTail(theMsg.GetMsg(MSG_SERVO_ERROR));
				theMsg.ShowMsg(ERROR_MSG, sList);		

				//Signal quit
				dwTimeout = 0;
				WaitThread(hThread,100);		//V3.3.W180 edit

				return FALSE;
			}
		}

		if (!theMcConfig.IsBypassMHS())			//v3.1.T172 add
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_MHS));
			Sleep_x1ms(1000);
			theMHS.WarmReset();
		}

		if (!theMcConfig.IsBypassAll())
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_XY_TABLE));
			Sleep_x1ms(1000);
			theServo.PowerOnServo();
            theBond.MoveBondHeadToFireLevel();
		}
		break;
	}
	case MHS_CU_ELITE:
	{
		//Bond Head go to upper limit, then Y motor go to positive limit 
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		{
			//move bonding to home
			if (theServo.ServoPreparePowerOn())
			{
				sList.AddTail(theMsg.GetMsg(MSG_SERVO_ERROR));
				theMsg.ShowMsg(ERROR_MSG, sList);

				//Signal quit
				dwTimeout = 0;
				WaitThread(hThread,100);		//V3.3.W180 edit

				return FALSE;
			}
		}

		if (!theMcConfig.IsBypassMHS())			//v3.1.T172 add
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_MHS));
			Sleep_x1ms(1000);
			theMHS.WarmReset();
		}

        if (!theMcConfig.IsBypassAll())
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_XY_TABLE));
			Sleep_x1ms(1000);
			theServo.PowerOnServo();
            theBond.MoveBondHeadToFireLevel();

            long lStartTime = GetCurrentTime();
			do 
			{
				if ((theStepper.IsBusy(CUELITE_ONLOAD_ELEVATOR) == false)
					&& (theStepper.IsBusy(CUELITE_OFFLOAD_ELEVATOR) == false))
					break;

                if (GetCurrentTime() - lStartTime > 6000) break;
			} while (TRUE);
		}
		break;
	}
	case MHS_VLED: case MHS_VLED_ELITE:
	{
		if (!theMcConfig.IsBypassAll())
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_XY_TABLE));
			Sleep_x1ms(1000);
			iStatus = theServo.ServoPreparePowerOn();
			if (iStatus == FOK)
				theServo.PowerOnServo();

            theBond.MoveBondHeadToFireLevel();
		}

		if (!theMcConfig.IsBypassAll())
		{
			m_pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_MHS));
			Sleep_x1ms(1000);
			theMHS.WarmReset();
		}
		break;
	}
	}

	if (!theMcConfig.IsBypassMHS())			//v3.1.T172 add
		theScanKeyThread.BeginThread();

	//Signal quit
	dwTimeout = 0;
	WaitThread(hThread,100);		//V3.3.W180 edit

	return TRUE;
}

void CWireBondDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int x = (rect.Width() - GetSystemMetrics(SM_CXICON) + 1) / 2;
		int y = (rect.Height() - GetSystemMetrics(SM_CYICON) + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CJpeg jpeg;
		jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_BK01);
		jpeg.Paint(dc.m_hDC,rect);

        UpdateData(FALSE);
        UpdateOverView();

        CDC* pCtrlDC = m_pInfoView->GetDC();

        CBrush brush(RGB(218, 221, 223));
        pCtrlDC->SelectObject(&brush);

        rect = CRect(0, 0, INFO_VIEW_WIDTH, INFO_VIEW_HEIGHT);
        m_pInfoView->InvalidateRect(rect);
        m_pInfoView->UpdateWindow();
        pCtrlDC->FillRect(rect, &brush);
        pCtrlDC->SelectStockObject(BLACK_BRUSH);
        m_pInfoView->ReleaseDC(pCtrlDC);

        UpdateInfoView(INFOVIEW_ALL);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWireBondDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWireBondDlg::OnOK()
{
}

void CWireBondDlg::OnCancel()
{
}

void CWireBondDlg::SetTooltip()	//V1.1.W149 add
{
	// Add tool tips to the controls, both languages
	m_ToolTip.SetFont(theApp.GetFont());

	//V1.1.W73 add - begin
	m_ToolTip.DelTool(&m_btnUnplug);
	m_ToolTip.AddTool(&m_btnUnplug, theMsg.GetMsg(MSG_UNPLUG));

	m_ToolTip.DelTool(&m_btnLight);
	m_ToolTip.AddTool(&m_btnLight, theMsg.GetMsg(MSG_LIGHT));

	m_ToolTip.DelTool(&m_btnIndex);
	m_ToolTip.AddTool(&m_btnIndex, theMsg.GetMsg(MSG_INDEX_2));

	m_ToolTip.DelTool(&m_btnAdjust);
	m_ToolTip.AddTool(&m_btnAdjust, theMsg.GetMsg(MSG_ADJUST));

	m_ToolTip.DelTool(&m_btnWire);
	m_ToolTip.AddTool(&m_btnWire, theMsg.GetMsg(MSG_THREAD));

	m_ToolTip.DelTool(&m_btnElevator);
	m_ToolTip.AddTool(&m_btnElevator, theMsg.GetMsg(MSG_ELEVATOR));		//V1.1.W94 add

	m_ToolTip.DelTool(&m_btnUp);
	m_ToolTip.AddTool(&m_btnUp, theMsg.GetMsg(MSG_UP));

	m_ToolTip.DelTool(&m_btnDown);
	m_ToolTip.AddTool(&m_btnDown, theMsg.GetMsg(MSG_DOWN));
	//V1.1.W73 add - end

	m_ToolTip.DelTool(&m_btnSetup);
	m_ToolTip.AddTool(&m_btnSetup, theMsg.GetMsg(MSG_SETUP));

	m_ToolTip.DelTool(&m_btnProgram);
	m_ToolTip.AddTool(&m_btnProgram, theMsg.GetMsg(MSG_PROGRAM));

	m_ToolTip.DelTool(&m_btnBond);
	m_ToolTip.AddTool(&m_btnBond, theMsg.GetMsg(MSG_RUN));

	m_ToolTip.DelTool(&m_btnDebug);
	m_ToolTip.AddTool(&m_btnDebug, theMsg.GetMsg(MSG_DEBUG_2));

	m_ToolTip.DelTool(&m_btnConfig);
	m_ToolTip.AddTool(&m_btnConfig, theMsg.GetMsg(MSG_CONFIGURE));

	m_ToolTip.Activate(TRUE);
}

void CWireBondDlg::ToggleLanguage()
{
	if (m_bButtonInhibited == true)return;

    theMcConfig.ToggleLanguage();
    
    SetFont(theApp.GetFont());

	theApp.SetDlgFont(this);	//V1.1.W139 add

    UpdateMenuTree(MENU_TREE_REFRESH);
	SetTooltip();	//V1.1.W149 add

	Invalidate();
}

void CWireBondDlg::ShutDown()
{
	if (CloseFrontWindow() == FALSE) return;

	short nMsg = MSG_CONFIRM_TO_EXIT;
	if ((short)theMsg.ShowMsg(WARNING_MSG, 1, &nMsg, true, false, true) == SHOW_MSG_RESULT_STOP)
		return;

	if (!theMcConfig.IsBypassMHS())		//v3.1.T172 add
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
		{
		case MHS_TOP_LED:
			theTopTrack.ResetRelayErrorState();
			theTopTrack.MoveIndexerToLogicZero();
			break;
		case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
			theTEIndex.ResetRelayErrorState();
			theTEIndex.MoveIndexerToLogicZero();
			break;
		case MHS_R2R:
			theR2RTrack.ResetRelayErrorState();
			theR2RTrack.MoveToLogicZero();
			break;
		case MHS_CU_ELITE:
			theCuEliteTrack.ResetRelayErrorState();
			theCuEliteTrack.MoveToLogicZero();
			break;
		}
	}
	 
    KillTimer(1);

    if (!theMcConfig.IsBypassAll())
    {
        theMHS.SetThreadActive(false);
    }

    theScanKeyThread.StopThread();

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		theServo.ClearAllBuffer();

	if (!theMcConfig.IsBypassMHS())		//v3.1.T172 add
    	theBond.m_N2h2Port.SetRelayState(FALSE, true);

	Sleep_x1ms(100);

	TBL_POS stTblPos;
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		stTblPos.dx = theMcData.GetRcd()->stTbSafeStandby.dx;
		stTblPos.dy = theMcData.GetRcd()->stTbSafeStandby.dy;
		break;
	default:
		stTblPos.dx = 41000;	//10000;
		stTblPos.dy = 20000;
	}
  
	theMHS.CloseWindowClamp();
	Sleep_x1ms(100);

    theApp.ArchivePowerOnData(ARCHIVE_SAVE);
    Sleep_x1ms(100);
    theApp.ArchiveMachineData(ARCHIVE_SAVE);
    Sleep_x1ms(100);
	theVisionSys.SavePatternRcd();	//v3.0.T70 edit
    Sleep_x1ms(200);

	theApp.RecordCurrentTime(FILE_COLDEND);

    if (!theMcConfig.IsBypassCamera())
    {
    	theVisionSys.FreeVisionResource();
    	Sleep_x1ms(100);
    }

	if (!theMcConfig.IsBypassAll())
	{
		theServo.MoveTable2(stTblPos);	//v3.1.T205 edit
		Sleep_x1ms(100);
		theBond.MoveBondHeadToRelaxPosition();
		Sleep_x1ms(100);

		//theDigitalIO.ReleaseIOCard();
	}

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
	{
		theServo.DisableServoController();
	}

	if (!theMcConfig.IsBypassMHS())		//v3.1.T172 add
    {
        theStepper.DisableStepper();
        Sleep_x1ms(100);
    }

	m_pLightCtrlDlg->DestroyWindow();
	m_pIndexCtrlDlg->DestroyWindow();
	m_pDlgOverView->DestroyWindow();
	m_pDlgSpider->DestroyWindow();

	delete m_pLightCtrlDlg;
	delete m_pIndexCtrlDlg;
	delete m_pDlgOverView;
	delete m_pDlgSpider;
  	
	CDialog::OnOK();
}

void CWireBondDlg::OnSetup()
{
	if (HideFrontWindow() == FALSE) return;	//V1.1.W155 add
	if (SwapFrontWindow(NULL) == FALSE) return;

    UpdateMenuTree(MENU_TREE_SETUP);
}

void CWireBondDlg::OnProgram()
{
	if (HideFrontWindow() == FALSE) return;	//V1.1.W155 add
	if (SwapFrontWindow(NULL) == FALSE) return;

    UpdateMenuTree(MENU_TREE_PROGRAM);
}

BYTE CWireBondDlg::ContactSearch()		//V1.1.W141 add
{
    if (theRef.IsCtactValid()) return TRUE;

	TCHAR wcMsg[80] = {MSG_UPDATE_CTACT_LEVEL, MSG_CONFIRM_TO_UPDATE};
	if (theMsg.ShowMsg(HELP_MSG, 2, (PSHORT)wcMsg, true) == SHOW_MSG_RESULT_STOP) return FALSE;

	theMsg.GetMsg(MSG_CTACT_SRCH, wcMsg);
	CTeachRecipeSheet *pSheet = new CTeachRecipeSheet(wcMsg, RECIPE_SETUP);
	pSheet->Create(this);
	return SwapFrontWindow(pSheet->m_hWnd);
}

void CWireBondDlg::OnBond()	//V1.1.W144 edit
{
	if (HideFrontWindow() == FALSE) return;	//V1.1.W155 add
	if (FindWindow(NULL, theMsg.GetMsg(MSG_RUN)) != NULL) return;
	if (SwapFrontWindow(NULL) == FALSE) return;

	if (thePgm.IsProgramValid() == false)
	{
		short anMsgArray[] = {MSG_NO_RECIPE, MSG_CONFIRM_TO_LOAD_RECIPE};
		if ((theMsg.ShowMsg(HELP_MSG,2,anMsgArray,true) == SHOW_MSG_RESULT_STOP)
			|| (LoadRecipe() == false)) return;

		CAutoBondPage::Clean();
	}

	CAutoBondSheet* pAutoBondSheet = new CAutoBondSheet(theMsg.GetMsg(MSG_RUN));
	pAutoBondSheet->Create(this);
	SwapFrontWindow(pAutoBondSheet->m_hWnd);

	if (CAutoBondPage::IsDirty() == FALSE) ContactSearch();
}

void CWireBondDlg::OnConfig()	//V1.1.W144 edit
{
	if (HideFrontWindow() == FALSE) return;	//V1.1.W155 add
	if (SwapFrontWindow(NULL) == FALSE) return;
    
    UpdateMenuTree(MENU_TREE_CONFIG);
}

void CWireBondDlg::OnDebug()	//V1.1.W144 edit
{
	if (HideFrontWindow() == FALSE) return;	//V1.1.W155 add
	if (SwapFrontWindow(NULL) == FALSE) return;

	UpdateMenuTree(MENU_TREE_DEBUG);
}

void CWireBondDlg::DisplayUserLevel()	//V1.1.W38 add
{
	CString strSpaces = TEXT("  ");
	CColorMenu* pMenu = new CColorMenu;

	pMenu->SetBkColor(theApp.m_bkColor);
	pMenu->SetFont(theApp.GetFont());
	pMenu->SetSize(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);

	pMenu->AllocBuf(4);
	pMenu->CreatePopupMenu();

	pMenu->AppendItem(IDM_OPERATOR, strSpaces+theMsg.GetMsg(MSG_OPERATOR));
	pMenu->AppendItem(IDM_PROCESS, strSpaces+theMsg.GetMsg(MSG_PROCESS));
	pMenu->AppendItem(IDM_SERVICE, strSpaces+theMsg.GetMsg(MSG_SERVICE));
	pMenu->AppendItem(IDM_ENGINEER, strSpaces+theMsg.GetMsg(MSG_ENGINEER));

    CPoint pt;
	GetCursorPos(&pt);
    pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
 
	pMenu->DestroyMenu();

	delete pMenu;
}

void CWireBondDlg::UpdateMenuTree(short nMenuTreeIndex)
{
	WCHAR wcMsg[80];

	if (m_nMenuTreeIndex == nMenuTreeIndex)
		return;
    
	if (nMenuTreeIndex != MENU_TREE_REFRESH)
		m_nMenuTreeIndex = nMenuTreeIndex;

	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_MENU_TREE);
    pTree->SelectItem(NULL);
    pTree->DeleteAllItems();
	pTree->SetBkColor(theApp.m_bkColor);
	pTree->SetImageList(&m_ImageList, TVSIL_NORMAL); 

	//Tree structure common values
	TV_INSERTSTRUCT stTvInsert;
	stTvInsert.hParent = NULL;
	stTvInsert.hInsertAfter = TVI_LAST;

	stTvInsert.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	stTvInsert.item.hItem = NULL;
	stTvInsert.item.state = 0;
	stTvInsert.item.stateMask = 0;
	stTvInsert.item.iSelectedImage = 2;
	stTvInsert.item.cChildren = 0;
	stTvInsert.item.lParam = 0;
    stTvInsert.item.iImage  = 2;
	stTvInsert.item.pszText = NULL;
    pTree->InsertItem(&stTvInsert);
	
    stTvInsert.item.iSelectedImage = 1;
    stTvInsert.item.iImage  = 0;
	stTvInsert.item.pszText = wcMsg;
	
	switch (m_nMenuTreeIndex)
	{
	case MENU_TREE_SETUP:
	{
		theMsg.GetMsg(MSG_LANGUAGE, wcMsg);
		pTree->InsertItem(&stTvInsert);

		theMsg.GetMsg(MSG_USER_LEVEL, wcMsg);
		pTree->InsertItem(&stTvInsert);

		HTREEITEM hTeach = NULL;
	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T442 add
			|| theMcConfig.IsOperSetupVisionCali())
		{
			theMsg.GetMsg(MSG_VISION_CALI, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		if ((theUser.GetUserLevel() > USER_OPERATOR)
			|| theMcConfig.IsOperSetupUsgCali())		//v3.0.T442 add
		{
			theMsg.GetMsg(MSG_USG_CALI, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		HTREEITEM hServoCali = NULL;
		if ((theUser.GetUserLevel() > USER_OPERATOR)
			|| theMcConfig.IsOperSetupServoCali())		//v3.0.T442 add
		{
#if _SERVO_TYPE == 2	//v3.1.T561 add
			if (theMcConfig.GetWclType() == 0)
			{
				theMsg.GetMsg(MSG_SERVO_CALI, wcMsg);
				hServoCali = pTree->InsertItem(&stTvInsert);
			}
#else
			theMsg.GetMsg(MSG_SERVO_CALI, wcMsg);
			hServoCali = pTree->InsertItem(&stTvInsert);
#endif
		}

		if ((theUser.GetUserLevel() > USER_OPERATOR)
			|| theMcConfig.IsOperSetupMhsCali())		//v3.0.T442 add
		{
			if (theMcConfig.GetMhsType() == MHS_CU_ELITE)
			{
				theMsg.GetMsg(MSG_ELEV_CALI, wcMsg);
				pTree->InsertItem(&stTvInsert);
			}

			if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
				|| (theMcConfig.GetMhsType() == MHS_R2R)
				|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
				|| (theMcConfig.GetMhsType() == MHS_FOXCONN)	//v3.0.T290 add
				|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
			{
				theMsg.GetMsg(MSG_MHS_CALI, wcMsg);
				pTree->InsertItem(&stTvInsert);
			}
		}

		if ((theUser.GetUserLevel() > USER_OPERATOR)
			|| theMcConfig.IsOperSetupMinimizeWindow())		//v3.0.T442 add
		{
			theMsg.GetMsg(MSG_MINIMIZE_WINDOW, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		theMsg.GetMsg(MSG_LOAD_CONFIG, wcMsg);
		pTree->InsertItem(&stTvInsert);

		if ((theUser.GetUserLevel() > USER_OPERATOR)
			|| theMcConfig.IsOperSetupShutDown())		//v3.0.T442 add
		{
			theMsg.GetMsg(MSG_SHUTDOWN, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		if (hServoCali != NULL)	//v3.0.T442 add
		{
			stTvInsert.hParent = hServoCali;

#if _SERVO_TYPE != 2	//v3.1.T561 add
			theMsg.GetMsg(MSG_CALI_POSITION, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_CALI_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);
#endif
			if (theMcConfig.GetWclType() == 0)	//v3.1.T561 add
			{
				theMsg.GetMsg(MSG_WIRE_CLAMP, wcMsg);
				pTree->InsertItem(&stTvInsert);
			}
		}

		break;
	}
	case MENU_TREE_PROGRAM:
	{
		//Top level
		HTREEITEM hPgmMgr = NULL;	//v3.0.T320 add
	    if ((theUser.GetUserLevel() != USER_OPERATOR)
			|| theMcConfig.IsOperRecipeManage())
		{
			theMsg.GetMsg(MSG_RECIPE_MANAGE, wcMsg);
			hPgmMgr = pTree->InsertItem(&stTvInsert);
		}

		HTREEITEM hTeach = NULL;	//v3.0.T320 add
	    if ((theUser.GetUserLevel() != USER_OPERATOR)
			|| theMcConfig.IsOperRecipeTeach())
		{
			theMsg.GetMsg(MSG_TEACH_RECIPE, wcMsg);
			hTeach = pTree->InsertItem(&stTvInsert);	//v3.0.T332 edit
		}

		HTREEITEM hRecipeEdit = NULL;	//v3.0.T320 add
	    if ((theUser.GetUserLevel() != USER_OPERATOR)
			|| theMcConfig.IsOperRecipeEdit())
		{
			theMsg.GetMsg(MSG_EDIT_RECIPE, wcMsg);
			hRecipeEdit = pTree->InsertItem(&stTvInsert);	//v3.0.T332 edit
		}

		HTREEITEM hBondSetup = NULL;
	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeBondSetup())
		{
			theMsg.GetMsg(MSG_BOND_SETUP, wcMsg);
			hBondSetup = pTree->InsertItem(&stTvInsert);
		}

	    if ((theUser.GetUserLevel() != USER_OPERATOR)	//v3.0.T320 add
			|| theMcConfig.IsOperRecipeBondPara())
		{
			theMsg.GetMsg(MSG_BOND_PARAMETER, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		//v3.0.T429 add
		//theMsg.GetMsg(MSG_WEDGE_ON_BUMP_PARA, wcMsg);
		//pTree->InsertItem(&stTvInsert);

		//v3.0.T429 add
		//theMsg.GetMsg(MSG_BBOS_PARA, wcMsg);
		//pTree->InsertItem(&stTvInsert);

	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeLoopPara())
		{
			theMsg.GetMsg(MSG_LOOP_PARAMETER, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		HTREEITEM hWireOffset = NULL;
	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeWireOffset())
		{
			theMsg.GetMsg(MSG_WIRE_OFFSET, wcMsg);
			hWireOffset = pTree->InsertItem(&stTvInsert);
		}

	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeBsobPara())
		{
			//v3.0.T429 add
			theMsg.GetMsg(MSG_BSOB_PARA, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		HTREEITEM hBsobOffset = NULL;
	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeBsobOffset())
		{
			theMsg.GetMsg(MSG_BSOB_OFFSET, wcMsg);
			hBsobOffset = pTree->InsertItem(&stTvInsert);
		}

	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeBbosPara())
		{
			//v3.0.T429 add
			theMsg.GetMsg(MSG_BBOS_PARA, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		HTREEITEM hBbosOffset = NULL;
	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeBbosOffset())
		{
			//v3.0.T433 add
			theMsg.GetMsg(MSG_BBOS_OFFSET, wcMsg);
			hBbosOffset = pTree->InsertItem(&stTvInsert);
		}

	    if ((theUser.GetUserLevel() != USER_OPERATOR)		//v3.0.T441 add
			|| theMcConfig.IsOperRecipeMhsPara())
		{
			//v3.0.T428 add
			theMsg.GetMsg(MSG_MHS_PARA, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		//Second level
		if (hPgmMgr != NULL)	//v3.0.T320 add
		{
			stTvInsert.hParent = hPgmMgr;
			theMsg.GetMsg(MSG_LOAD_RECIPE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_SAVE_RECIPE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_CLEAR_RECIPE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			if (theUser.GetUserLevel() >= USER_ENGINEER)
			{
				theMsg.GetMsg(MSG_VERIFY_RECIPE, wcMsg);
				pTree->InsertItem(&stTvInsert);
			}
		}

		if (hRecipeEdit != NULL)	//v3.0.T320 add
		{
			stTvInsert.hParent = hRecipeEdit;
			if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
				|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
				|| (theMcConfig.GetMhsType() == MHS_R2R)
				|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
			{
				theMsg.GetMsg(MSG_EDIT_MHS_PROGRAM, wcMsg);
				pTree->InsertItem(&stTvInsert);
			}

			theMsg.GetMsg(MSG_ALIGNMENT, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_PR_PATTERN, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_EDIT_WIRE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BOND_SEQ, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_MAT_BOND_SCHEDULE, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		if (hBondSetup != NULL)	//v3.0.T441 add
		{
			stTvInsert.hParent = hBondSetup;
			theMsg.GetMsg(MSG_CTACT_SRCH, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BOND_TIP_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_FIRE_LEVEL, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_DUMMY_BOND, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		if (hTeach != NULL)	//v3.0.T320 add
		{
			if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
				|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
				|| (theMcConfig.GetMhsType() == MHS_R2R)
				|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
			{
				stTvInsert.hParent = hTeach;
				theMsg.GetMsg(MSG_TEACH_MHS_PROGRAM, wcMsg);
				pTree->InsertItem(&stTvInsert);

				theMsg.GetMsg(MSG_TCH_BOND_PROGRAM, wcMsg);
				pTree->InsertItem(&stTvInsert);
			}
		}

		if (hWireOffset != NULL)	//v3.0.T441 add
		{
			stTvInsert.hParent = hWireOffset;

			theMsg.GetMsg(MSG_NSOP, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_BALL, wcMsg);	//v3.1.T358 rename
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_SCRUB, wcMsg);
			HTREEITEM hScrubCtrl = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_WIRE_LEN_CTRL, wcMsg);
			HTREEITEM hWireLengthCtrl = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_INTELLIGENT_PAD_LOCATOR, wcMsg);
			HTREEITEM hAutoPadCtrl = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_INTELLIGENT_LEAD_LOCATOR, wcMsg);
			HTREEITEM hIllCtrl = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_LOOP, wcMsg);
			HTREEITEM hLoopCtrl = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BOND_PARA, wcMsg);
			HTREEITEM h1stBondPara = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PARA, wcMsg);
			HTREEITEM h2ndBondPara = pTree->InsertItem(&stTvInsert);

			//v3.1.T401 delete
			//theMsg.GetMsg(MSG_BUMP_WIRE_BOND_POWER, wcMsg);
			//pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_FAB_TYPE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			stTvInsert.hParent = hScrubCtrl;

			theMsg.GetMsg(MSG_1ST_BOND_SCRUB, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_SCRUB, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_BALL_SCRUB, wcMsg);
			pTree->InsertItem(&stTvInsert);

			stTvInsert.hParent = h1stBondPara;

			theMsg.GetMsg(MSG_1ST_PRE_IMPACT_TIME, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_PRE_IMPACT_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_PRE_IMPACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_IMPACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_IMPACT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_IMPACT_FORCE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BASE_TIME, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BASE_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BASE_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//v3.0.T2 add
			theMsg.GetMsg(MSG_1ST_SRCH_HT, wcMsg);
			pTree->InsertItem(&stTvInsert);

			stTvInsert.hParent = h2ndBondPara;

			theMsg.GetMsg(MSG_2ND_PRE_IMPACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_PRE_IMPACT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_PRE_IMPACT_FORCE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_IMPACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_IMPACT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_IMPACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BASE_TIME, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BASE_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BASE_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_PRE_SHIFT_2, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//v3.0.T2 add
			theMsg.GetMsg(MSG_2ND_SRCH_HT, wcMsg);
			pTree->InsertItem(&stTvInsert);

			stTvInsert.hParent = hLoopCtrl;

			theMsg.GetMsg(MSG_LOOP_GROUP, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_WIRE_LEN_CORRECT, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_FAST_LOOP, wcMsg);
			pTree->InsertItem(&stTvInsert);

			stTvInsert.hParent = hWireLengthCtrl;
			theMsg.GetMsg(MSG_CONST_WIRE_LEN, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_WIRE_LEN, wcMsg);
			pTree->InsertItem(&stTvInsert);

			stTvInsert.hParent = hAutoPadCtrl;
			theMsg.GetMsg(MSG_1ST_BOND_PT_PAD_ID, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BOND_PT_X_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BOND_PT_Y_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PT_PAD_ID, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PT_X_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PT_Y_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			stTvInsert.hParent = hIllCtrl;
			theMsg.GetMsg(MSG_1ST_BOND_PT_ILL, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BOND_PT_LEAD_CAP_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BOND_PT_LEAD_X_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_1ST_BOND_PT_LEAD_Y_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PT_ILL, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PT_LEAD_CAP_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PT_LEAD_X_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BOND_PT_LEAD_Y_OFFSET, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		//bsob offset
		if (hBsobOffset != NULL)	//v3.0.T441 add
		{
			stTvInsert.hParent = hBsobOffset;

			theMsg.GetMsg(MSG_1ST_BUMP_PARA, wcMsg);
			HTREEITEM h1stBumpPara = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BUMP_PARA, wcMsg);
			HTREEITEM h2ndBumpPara = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_SHAPE_PARA, wcMsg);
			HTREEITEM hBumpShapePara = pTree->InsertItem(&stTvInsert);

			//v3.1.T401 add
			theMsg.GetMsg(MSG_BUMP_WIRE_PARA, wcMsg);
			HTREEITEM hBumpWirePara = pTree->InsertItem(&stTvInsert);

			//v3.1.T388 add
			theMsg.GetMsg(MSG_BSOB_FAB_TYPE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//v3.1.T401 add - bsob wire para offset
			stTvInsert.hParent = hBumpWirePara;
			theMsg.GetMsg(MSG_BSOB_WIRE_PRE_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_PRE_CTACT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_PRE_CTACT_FORCE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_CTACT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_CTACT_FORCE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_BOND_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_BOND_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_BOND_FORCE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_RLS_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_RLS_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BSOB_WIRE_RLS_FORCE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			//bsob bump shape parameter
			stTvInsert.hParent = hBumpShapePara;
			theMsg.GetMsg(MSG_BASE_HT, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_SPAN_LEN, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BALL_THICKNESS, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_SCRUB_DIST, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_CUTTING_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_CUTTING_POWER_DELAY, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_MOTION_SPEED, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_DIRECTION_ANGLE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			//v3.1.T362 add
			theMsg.GetMsg(MSG_BUMP_SHAPE_MODE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			//bsob 1st bump parameter
			stTvInsert.hParent = h1stBumpPara;
			theMsg.GetMsg(MSG_BUMP_1ST_PRE_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_1ST_PRE_CTACT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_1ST_PRE_CTACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_1ST_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_1ST_CTACT_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BUMP_1ST_CTACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BUMP_1ST_BOND_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BUMP_1ST_BOND_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BUMP_1ST_BOND_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//bsob 2nd bump parameter
			stTvInsert.hParent = h2ndBumpPara;
			theMsg.GetMsg(MSG_BUMP_2ND_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BUMP_2ND_CTACT_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BUMP_2ND_CTACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_2ND_BOND_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BUMP_2ND_BOND_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_2ND_BOND_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		//bbos offset
		if (hBbosOffset != NULL)	//v3.0.T441 add
		{
			stTvInsert.hParent = hBbosOffset;

			theMsg.GetMsg(MSG_1ST_BUMP_PARA, wcMsg);
			HTREEITEM h1stBumpPara = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_2ND_BUMP_PARA, wcMsg);
			HTREEITEM h2ndBumpPara = pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BUMP_SHAPE_PARA, wcMsg);
			HTREEITEM hBumpShapePara = pTree->InsertItem(&stTvInsert);

			//v3.1.T381 add
			theMsg.GetMsg(MSG_BBOS_FAB_TYPE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//bbos 1st bump parameter
			stTvInsert.hParent = h1stBumpPara;
			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_PRE_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_PRE_CTACT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_PRE_CTACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_CTACT_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_CTACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_BOND_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_BOND_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BBOS_BUMP_1ST_BOND_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//bbos 2nd bump parameter
			stTvInsert.hParent = h2ndBumpPara;
			theMsg.GetMsg(MSG_BBOS_BUMP_2ND_CTACT_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BBOS_BUMP_2ND_CTACT_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BBOS_BUMP_2ND_CTACT_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_BUMP_2ND_BOND_TIME, wcMsg); 
			pTree->InsertItem(&stTvInsert); 

			theMsg.GetMsg(MSG_BBOS_BUMP_2ND_BOND_POWER, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_BUMP_2ND_BOND_FORCE, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//bbos bump shape parameter
			stTvInsert.hParent = hBumpShapePara;
			theMsg.GetMsg(MSG_BBOS_BASE_HT, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_SPAN_LEN, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_BALL_THICKNESS, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_SCRUB_DIST, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_CUT_POWER, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_CUT_POWER_DELAY, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_MOTION_SPEED, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_BBOS_DIRECTION_ANGLE, wcMsg); 
			pTree->InsertItem(&stTvInsert);

			//v3.1.T362 add
			theMsg.GetMsg(MSG_BBOS_BUMP_SHAPE_MODE, wcMsg); 
			pTree->InsertItem(&stTvInsert);
		}

		break;
	}
	case MENU_TREE_BOND:
		break;
	case MENU_TREE_CONFIG:
	{
		//Top level
		if (theUser.GetUserLevel() > USER_OPERATOR)
		{
			theMsg.GetMsg(MSG_CONFIG_VISION, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_CONFIG_SERVO, wcMsg);
			pTree->InsertItem(&stTvInsert);
		    
			theMsg.GetMsg(MSG_CONFIG_BSD, wcMsg);
			pTree->InsertItem(&stTvInsert);
	        
			theMsg.GetMsg(MSG_CONFIG_BOND_PROCESS, wcMsg);
			pTree->InsertItem(&stTvInsert);

			//v1.6.T126 add
			if (theMcConfig.GetAcsBufferType() == 1)
			{
				theMsg.GetMsg(MSG_CONFIG_BOND_PROCESS_2, wcMsg);
				pTree->InsertItem(&stTvInsert);
			}
		}

		//v3.0.T428 delete
		//if (theUser.GetUserLevel() > USER_OPERATOR)
		//{
		//	theMsg.GetMsg(MSG_CONFIG_MHS, wcMsg);
		//	pTree->InsertItem(&stTvInsert);
		//}

		break;
	}
	case MENU_TREE_DEBUG:
	{			
		theMsg.GetMsg(MSG_MHS_UTILITY, wcMsg);
		pTree->InsertItem(&stTvInsert);

		theMsg.GetMsg(MSG_BOND_TOOLS, wcMsg);
		pTree->InsertItem(&stTvInsert);

		theMsg.GetMsg(MSG_HARDWARE_TEST, wcMsg);
		pTree->InsertItem(&stTvInsert);

		if (theUser.GetUserLevel() > USER_PROCESS)
		{
			theMsg.GetMsg(MSG_BSD_DEBUG, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_CTACT_LEVEL_SRCH, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_USG_SETUP, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_DIAGNOSTIC_TOOL, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_DUMMY_PROGRAM, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_DEBUG, wcMsg);
			pTree->InsertItem(&stTvInsert);

			theMsg.GetMsg(MSG_ENTER_STANDBY_MODE, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		theMsg.GetMsg(MSG_SOFTWARE_MANAGER, wcMsg);
		pTree->InsertItem(&stTvInsert);

		if (theMcConfig.IsBypassAll())
		{
			theMsg.GetMsg(MSG_AFP_MODE, wcMsg);
			pTree->InsertItem(&stTvInsert);
		}

		break;
	}

    default:

		break;
	}
	
    pTree->SelectItem(NULL);
}

bool CWireBondDlg::LoadRecipe()		//V1.1.W141 add
{
	CFileDialog dlg(TRUE, _T("rcp"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		_T("Recipe Files (*.rcp)|*.rcp||"));
	dlg.m_pOFN->lpstrInitialDir = _T(DIR_RECIPE);
	if (dlg.DoModal() != IDOK) return false;
	CString sRcpPath = dlg.GetPathName();
	CString sRcpFile = dlg.GetFileName();

	PGM_RECORD stPgmRcd;
	thePgm.GetRcd(&stPgmRcd);
	if (sRcpFile.GetLength() > sizeof(stPgmRcd.cPgmName) - 1)
	{
		short nMsg = MSG_INVALID_FILE_NAME;
		theMsg.ShowMsg(WARNING_MSG,1,&nMsg);
		UpdateOverView();
		UpdateInfoView(INFOVIEW_ALL);
		return false;
	}

	wchar_t acBuf[16];
	memcpy(acBuf, &stPgmRcd.bFabTest, sizeof(acBuf));

	//rename .rcp to .zip
	CString sZipPath(sRcpPath);
	sZipPath.Replace(_T(".rcp"), _T(".zip"));
	CFile::Rename(sRcpPath, sZipPath);

	//unzip .zip
	CString sDir(DIR_VISION);
	theDataMgr.DeleteVisionPattern(sDir);
	theDataMgr.UnZipPrFile(sZipPath, sDir);

	CString sRecPath(sRcpFile);
	sRecPath.Replace(_T(".rcp"),_T(".rec"));
	sRecPath.Insert(0, _T(DIR_WBDATA_VISION));

	short anMsg[1] = {MSG_LOAD_MHS_DATA};
	bool bLoadWH = (theMsg.ShowMsg(WARNING_MSG, 1, anMsg,
						true, false, true) == SHOW_MSG_RESULT_OK);
	theApp.ArchiveProgram(sRecPath, ARCHIVE_LOAD, bLoadWH);

	theBond.ResetBondData();
	theBond.SetPrPatternPassScore();
	theServo.SetTableKillDec();

	DeleteFile(sRecPath);
	ConvertRecipeName(acBuf, sizeof(acBuf));

	UpdateOverView();
	UpdateInfoView(INFOVIEW_ALL);

	//3.0.T46 add
	sRcpPath.Insert(0, _T("LoadRecipe_"));
	theLog.WriteLine(sRcpPath, true);

	return true;
}

void CWireBondDlg::OnMenuTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CBusy bs(&m_bBusy);	//v3.1.T5 add

	//v3.1.T49 clean
	*pResult = 0;
	LPNMTREEVIEW pNMTreeView = (LPNMTREEVIEW)pNMHDR;
	if (pNMTreeView->itemNew.hItem == NULL) return;

	WCHAR wcMenuMsg[80];
	TV_ITEM item;
	item.mask = TVIF_HANDLE | TVIF_TEXT;
	item.hItem = pNMTreeView->itemNew.hItem;
	item.pszText = wcMenuMsg;
	item.cchTextMax = 80;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_MENU_TREE);
	pTree->GetItem(&item);

	switch (m_nMenuTreeIndex)
	{
	case MENU_TREE_SETUP:
	{
		if (theMsg.IsEqual(MSG_LANGUAGE, wcMenuMsg))	//V1.1.W50 add
		{
			ToggleLanguage();
			pTree->SelectItem(NULL);
		}			
		else if (theMsg.IsEqual(MSG_USER_LEVEL, wcMenuMsg))//V1.1.W38 add
		{
			DisplayUserLevel();
			pTree->SelectItem(NULL);
		}
		else if (theMsg.IsEqual(MSG_VISION_CALI, wcMenuMsg))
		{
			CVisionCalibDlg* pDlg = new CVisionCalibDlg();
			pDlg->Create(CVisionCalibDlg::IDD,this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pDlg->m_hWnd);	//V1.1.W142 add
		}
		else if (theMsg.IsEqual(MSG_USG_CALI, wcMenuMsg))
		{
			CUsgSetupDlg* lpDlg = new CUsgSetupDlg();
			lpDlg->Create(CUsgSetupDlg::IDD,this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpDlg->m_hWnd);	//V1.1.W144 add
		}
		else if (theMsg.IsEqual(MSG_ELEV_CALI,wcMenuMsg)&&
			theMcConfig.GetMhsType() == MHS_CU_ELITE)
		{
			CDlgCuEliteElevSetupPage* pdlg = new CDlgCuEliteElevSetupPage();
			pdlg->Create(CDlgCuEliteElevSetupPage::IDD,this);
			pTree->SelectItem(NULL);
		}
		else if (theMsg.IsEqual(MSG_MHS_CALI, wcMenuMsg))
		{
			switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
			{
			case MHS_TOP_LED:
			case MHS_FOXCONN:	//v3.0.T293 add
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				CDlgTopWhSetUpPage* pDlg = new CDlgTopWhSetUpPage();
				pDlg->Create(CDlgTopWhSetUpPage::IDD,this);
				pTree->SelectItem(NULL);
				SwapFrontWindow(pDlg->m_hWnd);		//V1.1.W147 add
				break;
			}
			case MHS_R2R:
			{
				CR2RWhSetupPage* pR2RWhSetupPage = new CR2RWhSetupPage();
				pR2RWhSetupPage->Create(CR2RWhSetupPage::IDD,this);
				pTree->SelectItem(NULL);
				break;
			}
			case MHS_CU_ELITE:
			{
				CDlgCuEliteWHSetupPage* pDlg = new CDlgCuEliteWHSetupPage();
				pDlg->Create(CDlgCuEliteWHSetupPage::IDD,this);
				pTree->SelectItem(NULL);
				break;
			}
			}
		}
		else if (theMsg.IsEqual(MSG_MINIMIZE_WINDOW, wcMenuMsg))	//V1.1.W51 add
		{
			MinimizeWindow();
			pTree->SelectItem(NULL);
		}
		else if (theMsg.IsEqual(MSG_LOAD_CONFIG,wcMenuMsg))	//v1.6.P238sc  add
		{
			theMcConfig.LoadConfig();
			theMcConfig.LoadAccessControl();	//v3.0.T320 add
#if _SERVO_TYPE < 2	//v3.1.T415 add
			theServo.InitConfig();	//v3.0.T64 add
			theServo.ReDownloadBufferProgram();	//v3.0.T47 add
#endif
			theEFO.InitConfig();	//v3.0.T165 add
			if (theMcConfig.GetWireSpoolType())	//v3.0.L302 add
			{
				short nDist = theMcConfig.GetWireSpoolDist();
				short nSpeed = theMcConfig.GetWireSpoolSpeed();
				theStepperIO.SetWireSpoolPara(CANADDR_OFFLOAD, 5, MOVEREL, nDist);
				theStepperIO.SetWireSpoolPara(CANADDR_OFFLOAD, 5, SETVEL, nSpeed);
			}
//#if _SERVO_TYPE == 2	//v3.1.T397 add, v3.1.T447 delete
//			theServo.InitWireClamp();
//#endif
			::AfxMessageBox(_T("Config loaded"));
		}
		else if (theMsg.IsEqual(MSG_SHUTDOWN, wcMenuMsg))
		{
			ShutDown();
			pTree->SelectItem(NULL);
		}
		else if (theMsg.IsEqual(MSG_CALI_POSITION, wcMenuMsg))
		{
			CBondHeadCalibSheet *pBhCalibSheet = new CBondHeadCalibSheet(wcMenuMsg);
			pBhCalibSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pBhCalibSheet->m_hWnd);	//V1.1.W145 add
		}
		else if (theMsg.IsEqual(MSG_CALI_FORCE, wcMenuMsg))
		{
			CBondHeadCalibSheet *pBhCalibSheet = new CBondHeadCalibSheet(wcMenuMsg);
			pBhCalibSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pBhCalibSheet->m_hWnd);	//V1.1.W145 add
		}
		else if (theMsg.IsEqual(MSG_WIRE_CLAMP, wcMenuMsg))
		{
			CBondHeadCalibSheet *pBhCalibSheet = new CBondHeadCalibSheet(wcMenuMsg);
			pBhCalibSheet->Create(this);
			pTree->SelectItem(NULL); 

			SwapFrontWindow(pBhCalibSheet->m_hWnd);	//V1.1.W145 add
		}
		break;
	}
	case MENU_TREE_PROGRAM:
	{
		//Top level
		if (theMsg.IsEqual(MSG_LOAD_RECIPE, wcMenuMsg))
		{
			LoadRecipe();
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
		}
		else if (theMsg.IsEqual(MSG_SAVE_RECIPE, wcMenuMsg))
		{
			CFileOpenDlg dlg(FALSE, _T("rcp"), NULL,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
				_T("Recipe Files (*.rcp)|*.rcp||"));
			dlg.m_pOFN->lpstrInitialDir = _T(DIR_RECIPE);
			if (dlg.DoModal() == IDOK)
			{
				CString sRcpPath = dlg.GetPathName();
				CString sRcpFile = dlg.GetFileName();

				PGM_RECORD stPgmRcd;
				thePgm.GetRcd(&stPgmRcd);
				if (sRcpFile.GetLength() > sizeof(stPgmRcd.cPgmName)-1)
				{
					short nMsg = MSG_INVALID_FILE_NAME;
					theMsg.ShowMsg(WARNING_MSG, 1, &nMsg);

					UpdateOverView();
					UpdateInfoView(INFOVIEW_ALL);
					pTree->SelectItem(pNMTreeView->itemOld.hItem);
					break;
				}

				wcstombs_s(NULL, (LPSTR)stPgmRcd.cPgmName, sizeof(stPgmRcd.cPgmName), sRcpFile, sizeof(stPgmRcd.cPgmName));
				thePgm.SetRcd(&stPgmRcd);

				theApp.ArchivePowerOnData(ARCHIVE_SAVE);
				theVisionSys.SavePatternRcd();	//v3.0.T70 edit

				//delete existing recipe
				theDataMgr.DeleteFile(sRcpPath);

				//save new recipe
				CString sRecPath(sRcpPath);
				sRecPath.Replace(_T(".rcp"), _T(".rec"));
				theApp.ArchiveProgram(sRecPath, ARCHIVE_SAVE);

				//zip new recipe
				CString sZipPath(sRcpPath);
				sZipPath.Replace(_T(".rcp"), _T(".zip"));
				CString sRecFile(sRcpFile);
				sRecFile.Replace(_T(".rcp"), _T(".rec"));
				CString sDir(DIR_VISION);
				theDataMgr.ZipPrFile(sZipPath, sRecPath, sRecFile, sDir);
				DeleteFile(sRecPath);

				theApp.BackupWbdata();
				if (theMcConfig.IsMcdata()) theApp.BackupMcdata();
			}

			UpdateOverView();
			UpdateInfoView(INFOVIEW_ALL);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
		}
		else if (theMsg.IsEqual(MSG_CLEAR_RECIPE, wcMenuMsg))
		{
			short index[1] = {MSG_SURE_TO_DELETE_PROGRAM};
			if (theMsg.ShowMsg(HELP_MSG, 1, index, true) == SHOW_MSG_RESULT_OK)  
			{
				theApp.ArchiveProgram(NULL, ARCHIVE_CLEAR);
				theDataMgr.DeleteVisionPattern(CString(DIR_VISION));

				theVisionSys.ClearDrawing();
				theVisionSys.UpdateLightingParameter(0,0,0,0);
			}

			UpdateOverView();
			UpdateInfoView(INFOVIEW_ALL);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
		}
		else if (theMsg.IsEqual(MSG_VERIFY_RECIPE,wcMenuMsg))
		{
			VerifyRecipe();	//v2.0.T55 add
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
		}
		else if (theMsg.IsEqual(MSG_TEACH_MHS_PROGRAM, wcMenuMsg))
		{
			switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
			{
			case MHS_CU_ELITE:
			{
				CCuEliteWorkHolderTeachSheet *pTeachCuElite = new CCuEliteWorkHolderTeachSheet(wcMenuMsg, WH_TEACH);
				pTeachCuElite->SetWizardMode();
				pTeachCuElite->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				break;
			}
			case MHS_R2R:
			{
				CTeachR2RWorkHolderSheet *pTeachR2R = new CTeachR2RWorkHolderSheet(wcMenuMsg, WH_TEACH);
				pTeachR2R->SetWizardMode();//must call this  
				pTeachR2R->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				break;
			}
			case MHS_TOP_LED: case MHS_VLED: case MHS_VLED_ELITE:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				CTeachWorkHolderSheet *pTeachWH = new CTeachWorkHolderSheet(wcMenuMsg, WH_TEACH);
				pTeachWH->SetWizardMode();
				pTeachWH->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);

				SwapFrontWindow(pTeachWH->m_hWnd);	//V1.1.W158 add
				break;
			}
			}
		}
		else if (theMsg.IsEqual(MSG_EDIT_MHS_PROGRAM, wcMenuMsg))
		{
			switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
			{
			case MHS_CU_ELITE:
			{
				CCuEliteWorkHolderTeachSheet *pTeachWH = new CCuEliteWorkHolderTeachSheet(wcMenuMsg, WH_EDIT);
				pTeachWH->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				break;
			}
			case MHS_R2R:
			{
				CTeachR2RWorkHolderSheet *pTeachR2R = new CTeachR2RWorkHolderSheet(wcMenuMsg, WH_EDIT);
				pTeachR2R->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				break;
			}
			case MHS_TOP_LED: case MHS_VLED: case MHS_VLED_ELITE:
			case MHS_TOP_ELITE:	//v3.0.T410 add
			{
				CTeachWorkHolderSheet *pTeachWH = new CTeachWorkHolderSheet(wcMenuMsg, WH_EDIT);
				pTeachWH->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);

				SwapFrontWindow(pTeachWH->m_hWnd);	//V1.1.W158 add
				break;
			}
			}
		}
		else if (theMsg.IsEqual(MSG_ALIGNMENT, wcMenuMsg))
		{
			if (thePgm.IsProgramValid() == true)
			{
				CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_EDIT);
				pTeachRecipeSheet->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
			}
			else
				EnableSystemButton();
		}
		else if (theMsg.IsEqual(MSG_PR_PATTERN, wcMenuMsg))
		{
			if (thePgm.IsProgramValid())
			{
				CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_EDIT);
				pTeachRecipeSheet->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
			}
			else
				EnableSystemButton();
		}
		else if (theMsg.IsEqual(MSG_EDIT_WIRE,wcMenuMsg))
		{
			if (thePgm.IsProgramValid())
			{
				CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_EDIT);
				pTeachRecipeSheet->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
			}
			else
				EnableSystemButton();
		}
		else if (theMsg.IsEqual(MSG_BOND_SEQ, wcMenuMsg))
		{
			if (thePgm.IsProgramValid())
			{
				CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_EDIT);
				pTeachRecipeSheet->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
			}
			else
				EnableSystemButton();
		}
		else if (theMsg.IsEqual(MSG_MAT_BOND_SCHEDULE, wcMenuMsg))
		{
			if (thePgm.IsProgramValid())
			{
				CMatrixSchedule* pMatSchedule = new CMatrixSchedule();
				pMatSchedule->Create(CMatrixSchedule::IDD, this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);

				SwapFrontWindow(pMatSchedule->m_hWnd);	//V1.1.W156 add
			}
			else
				EnableSystemButton();
		}
		else if (theMsg.IsEqual(MSG_TEACH_RECIPE, wcMenuMsg))
		{
			if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
				|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
				|| (theMcConfig.GetMhsType() == MHS_R2R)
				|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
				break;

			CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_TEACH);
			pTeachRecipeSheet->SetWizardMode();
			pTeachRecipeSheet->Create(this);
			pTree->SelectItem(NULL);
			SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
		}
		else if (theMsg.IsEqual(MSG_TCH_BOND_PROGRAM, wcMenuMsg))
		{
			CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_TEACH);
			pTeachRecipeSheet->SetWizardMode();
			pTeachRecipeSheet->Create(this);
			pTree->SelectItem(NULL);
			SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
		}
		else if (theMsg.IsEqual(MSG_CTACT_SRCH,wcMenuMsg))
		{
			if (thePgm.IsProgramValid())
			{
				CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_SETUP);
				pTeachRecipeSheet->Create(this);
				pTree->SelectItem(pNMTreeView->itemOld.hItem);
				SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
			}
			else
				EnableSystemButton();
		}
		else if (theMsg.IsEqual(MSG_BOND_TIP_OFFSET,wcMenuMsg))
		{
			CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_SETUP);
			pTeachRecipeSheet->Create(this);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
		}
		else if (theMsg.IsEqual(MSG_FIRE_LEVEL,wcMenuMsg))
		{
			CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_SETUP);
			pTeachRecipeSheet->Create(this);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
		}
		else if (theMsg.IsEqual(MSG_DUMMY_BOND, wcMenuMsg))
		{
			CTeachRecipeSheet *pTeachRecipeSheet = new CTeachRecipeSheet(wcMenuMsg, RECIPE_SETUP);
			pTeachRecipeSheet->Create(this);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pTeachRecipeSheet->m_hWnd);	//V1.1.W153 add
		}
		else if (theMsg.IsEqual(MSG_BOND_PARAMETER, wcMenuMsg))
		{
			CBondParaSheet* lpBondParaSheet = new CBondParaSheet(wcMenuMsg);
			lpBondParaSheet->Create(this);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpBondParaSheet->m_hWnd);	//V1.1.W159 add
		}
		else if (theMsg.IsEqual(MSG_BSOB_PARA, wcMenuMsg))
		{
			CBallParaSheet *pSheet = new CBallParaSheet(wcMenuMsg, 1);
			pSheet->Create(this);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pSheet->m_hWnd);	//V1.1.W244 add
		}
		else if (theMsg.IsEqual(MSG_BBOS_PARA, wcMenuMsg))
		{
			CBallParaSheet* lpBallParaSheet = new CBallParaSheet(wcMenuMsg, 0);
			lpBallParaSheet->Create(this);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpBallParaSheet->m_hWnd);	//V1.1.W244 add
		}
		else if (theMsg.IsEqual(MSG_LOOP_PARAMETER, wcMenuMsg))
		{
			CLoopParaBaseDlg* lpDlg = new CLoopParaBaseDlg();
			lpDlg->Create(CLoopParaBaseDlg::IDD, this);
			lpDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpDlg->m_hWnd);	//V1.1.W246 add
		}
		else if (theMsg.IsEqual(MSG_LOOP_GROUP, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_LOOP_GROUP);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_WIRE_LEN_CORRECT, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_WIRE_LENGTH_CORRECT);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_FAST_LOOP,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_FAST_LOOP);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_NSOP,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_NSOP);
			lpDlg->SetHelpMessage(theMsg.GetMsg(MSG_BSD_MAP_MSG));
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_BALL, wcMenuMsg))	//V1.5.W27 edit, v3.1.T358 rename
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BUMP_BALL);
			pDlg->SetHelpMessage(theMsg.GetMsg(MSG_BUMP_MAP_MSG));
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_SCRUB,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_BOND_SCRUB);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_SCRUB,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_BOND_SCRUB);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_BALL_SCRUB,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BUMP_BALL_SCRUB);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_CONST_WIRE_LEN,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_WIRE_LENGTH_CTRL);
			lpDlg->SetHelpMessage(theMsg.GetMsg(MSG_0_NO_1_YES));
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_WIRE_LEN, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_WIRE_LENGTH);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_PT_PAD_ID, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_POINT_PAD_ID);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_PT_X_OFFSET, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_POINT_X_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_PT_Y_OFFSET, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_POINT_Y_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_PT_PAD_ID,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_POINT_PAD_ID);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_PT_X_OFFSET,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_POINT_X_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_PT_Y_OFFSET,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_POINT_Y_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_PT_ILL, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_POINT_ILL);
			lpDlg->SetHelpMessage(theMsg.GetMsg(MSG_0_NO_1_YES));
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_PT_LEAD_CAP_OFFSET,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_POINT_LEAD_CAPI_OFFSET);
			lpDlg->SetHelpMessage(theMsg.GetMsg(MSG_0_NO_1_YES));
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_PT_LEAD_X_OFFSET, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_POINT_LEAD_X_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BOND_PT_LEAD_Y_OFFSET, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_POINT_LEAD_Y_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_PT_ILL, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_POINT_ILL);
			lpDlg->SetHelpMessage(theMsg.GetMsg(MSG_0_NO_1_YES));
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_PT_LEAD_CAP_OFFSET, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_POINT_LEAD_CAPI_OFFSET);
			lpDlg->SetHelpMessage(theMsg.GetMsg(MSG_0_NO_1_YES));
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_PT_LEAD_X_OFFSET, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_POINT_LEAD_X_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BOND_PT_LEAD_Y_OFFSET, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_POINT_LEAD_Y_OFFSET);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_PRE_IMPACT_POWER,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_PRE_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_PRE_IMPACT_FORCE,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_PRE_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_PRE_IMPACT_TIME,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_PRE_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		//v3.0.T2 add
		else if (theMsg.IsEqual(MSG_1ST_SRCH_HT, wcMenuMsg))	//v3.0.T2 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_1ST_SRCH_HT);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		//v3.0.T2 add
		else if (theMsg.IsEqual(MSG_2ND_SRCH_HT, wcMenuMsg))
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_2ND_SRCH_HT);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_PRE_IMPACT_POWER,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_PRE_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_PRE_IMPACT_TIME,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_PRE_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_PRE_IMPACT_FORCE,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_PRE_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BASE_TIME,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_BASE_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BASE_POWER,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_BASE_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_BASE_FORCE,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_BASE_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BASE_TIME,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_BASE_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BASE_POWER,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_BASE_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_BASE_FORCE,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_BASE_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_PRE_SHIFT_2,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_PRE_SHIFT_2);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_IMPACT_TIME, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_IMPACT_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_IMPACT_FORCE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_IMPACT_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_1ST_IMPACT_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_1ST_IMPACT_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_IMPACT_TIME, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_IMPACT_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_IMPACT_FORCE,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_IMPACT_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_2ND_IMPACT_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_2ND_IMPACT_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		//v3.1.T401 delete
		//else if (theMsg.IsEqual(MSG_BUMP_WIRE_BOND_POWER, wcMenuMsg))	//V1.5.W27 edit
		//{
		//	CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BUMP_WIRE_BOND_POWER);
		//	lpDlg->Create(IDD_PARA_MAP_BASE, this);
		//	lpDlg->ShowWindow(SW_SHOW);

		//	pTree->SelectItem(pNMTreeView->itemOld.hItem);

		//	SwapFrontWindow(lpDlg->m_hWnd);
		//}
		else if (theMsg.IsEqual(MSG_BSOB_FAB_TYPE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_FAB_TYPE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_FAB_TYPE, wcMenuMsg))	//v3.1.T381 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_FAB_TYPE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_FAB_TYPE,wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_WIRE_FAB_TYPE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_BOND_FORCE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_BOND_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_2ND_BOND_FORCE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_2ND_BOND_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_BOND_TIME, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_BOND_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_2ND_BOND_TIME, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_2ND_BOND_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_BOND_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_BOND_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_2ND_BOND_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_2ND_BOND_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_PRE_CTACT_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_PRE_CTACT_FORCE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_PRE_CTACT_TIME, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_CTACT_FORCE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_CTACT_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_2ND_CTACT_FORCE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_2ND_CTACT_FORCE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_CTACT_TIME, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_CTACT_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_2ND_CTACT_TIME, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_2ND_CTACT_TIME);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_1ST_CTACT_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_1ST_CTACT_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_2ND_CTACT_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_BUMP_2ND_CTACT_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BASE_HT, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_BASE_HT);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_SPAN_LEN, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_SPAN_LEN);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BALL_THICKNESS, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_BALL_THICKNESS);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_SCRUB_DIST, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_SCRUB_DIST);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_CUTTING_POWER, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_CUT_POWER);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_CUTTING_POWER_DELAY, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_CUT_POWER_DELAY);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_MOTION_SPEED, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_MOTION_SPEED);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_DIRECTION_ANGLE, wcMenuMsg))	//V1.5.W27 edit
		{
			CParaMapBase* lpDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_DIRECTION_ANGLE);
			lpDlg->Create(IDD_PARA_MAP_BASE, this);
			lpDlg->ShowWindow(SW_SHOW);

			pTree->SelectItem(pNMTreeView->itemOld.hItem);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BUMP_SHAPE_MODE, wcMenuMsg))	//v3.1.T362 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_SHAPE_BUMP_SHAPE_MODE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		//v3.0.T428 add
		else if (theMsg.IsEqual(MSG_MHS_PARA, wcMenuMsg))
		{
			CMaterialHandleConfigSheet *pSheet = new CMaterialHandleConfigSheet(wcMenuMsg);
			pSheet->Create(this);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pSheet->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BASE_HT, wcMenuMsg))	//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_BASE_HT);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_SPAN_LEN, wcMenuMsg))	//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_SPAN_LEN);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BALL_THICKNESS, wcMenuMsg))	//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_BALL_THICKNESS);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_SCRUB_DIST, wcMenuMsg))		//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_SCRUB_DIST);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_CUT_POWER, wcMenuMsg))	//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_CUT_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_CUT_POWER_DELAY, wcMenuMsg))	//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_CUT_POWER_DELAY);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_MOTION_SPEED, wcMenuMsg))	//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_MOTION_SPEED);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_DIRECTION_ANGLE, wcMenuMsg))	//v3.0.T433 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_DIRECTION_ANGLE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_SHAPE_MODE, wcMenuMsg))	//v3.1.T362 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_SHAPE_BUMP_SHAPE_MODE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_PRE_CTACT_TIME, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_PRE_CTACT_POWER, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_PRE_CTACT_FORCE, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_CTACT_TIME, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_CTACT_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_2ND_CTACT_TIME, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_2ND_CTACT_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_CTACT_POWER, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_CTACT_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_2ND_CTACT_POWER, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_2ND_CTACT_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_CTACT_FORCE, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_CTACT_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_2ND_CTACT_FORCE, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_2ND_CTACT_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_BOND_TIME, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_BOND_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_2ND_BOND_TIME, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_2ND_BOND_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_BOND_POWER, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_BOND_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_2ND_BOND_POWER, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_2ND_BOND_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_1ST_BOND_FORCE, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_1ST_BOND_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BBOS_BUMP_2ND_BOND_FORCE, wcMenuMsg))	//v3.0.T434 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BBOS_BUMP_2ND_BOND_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_PRE_CTACT_TIME, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_PRE_CTACT_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
        break;
	}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_PRE_CTACT_FORCE, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_PRE_CTACT_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_PRE_CTACT_POWER, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_PRE_CTACT_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_CTACT_TIME, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_CTACT_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_CTACT_FORCE, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_CTACT_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_CTACT_POWER, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_CTACT_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_BOND_TIME, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_BOND_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_BOND_FORCE, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_BOND_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_BOND_POWER, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_BOND_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_RLS_TIME, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_RLS_TIME);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_RLS_FORCE, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_RLS_FORCE);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
		if (theMsg.IsEqual(MSG_BSOB_WIRE_RLS_POWER, wcMenuMsg))	//v3.1.T401 add
		{
			CParaMapBase *pDlg = new CParaMapBase(PARA_TYPE_BSOB_WIRE_RLS_POWER);
			pDlg->Create(IDD_PARA_MAP_BASE, this);
			pDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(pNMTreeView->itemOld.hItem);
			SwapFrontWindow(pDlg->m_hWnd);
			break;
		}
        break;
	}
	case MENU_TREE_CONFIG:
	{
		//Top level
		if (theMsg.IsEqual(MSG_CONFIG_VISION, wcMenuMsg))	//V1.1.W259 edit
		{
			CVisionParaSheet* lpDlg = new CVisionParaSheet(wcMenuMsg);
			lpDlg->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_CONFIG_SERVO, wcMenuMsg))
		{
			CServoConfigSheet* lpSheet = new CServoConfigSheet(wcMenuMsg);	//V1.4.W21 edit
			lpSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpSheet->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_CONFIG_BSD, wcMenuMsg))		//V1.4.W24 edit
		{
			CStickDetectDlg* lpDlg = new CStickDetectDlg(this);
			lpDlg->Create(IDD_STICK_DETECT,this);
			lpDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_CONFIG_BOND_PROCESS, wcMenuMsg)) //V1.4.W25 edit
		{
			CBondProcessSheet* lpSheet = new CBondProcessSheet(wcMenuMsg);
			lpSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpSheet->m_hWnd);
		}
		//v3.0.T428 delete
		//else if (theMsg.IsEqual(MSG_CONFIG_MHS,wcMenuMsg))
		//{
		//	CMaterialHandleConfigSheet* pMaterialHandleConfigSheet = new CMaterialHandleConfigSheet(wcMenuMsg);
		//	pMaterialHandleConfigSheet->Create(this);
		//	pTree->SelectItem(NULL);
		//	SwapFrontWindow(pMaterialHandleConfigSheet->m_hWnd);
		//}
		else if (theMsg.IsEqual(MSG_CONFIG_BOND_PROCESS_2,wcMenuMsg))	//v1.6.T126 add
		{
			CForceSwitchSheet *pForceSwitchSheet = new CForceSwitchSheet(wcMenuMsg);
			pForceSwitchSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pForceSwitchSheet->m_hWnd);
		}
        break;
	}
	case MENU_TREE_DEBUG:
	{
		if (theMsg.IsEqual(MSG_MHS_UTILITY, wcMenuMsg))	//V1.4.W23 edit
		{
			CMhsUtilitySheet* lpSheet = new CMhsUtilitySheet(wcMenuMsg);
			lpSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpSheet->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BOND_TOOLS,wcMenuMsg))	//V1.4.W22 edit
		{
			CBondToolsSheet* lpSheet = new CBondToolsSheet(wcMenuMsg);
			lpSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpSheet->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_HARDWARE_TEST, wcMenuMsg))	//V1.4.W26 edit
		{
			CHardwareTestSheet* lpSheet = new CHardwareTestSheet(wcMenuMsg);
			lpSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpSheet->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_DUMMY_PROGRAM, wcMenuMsg))	//V1.4.W42 add
		{
			CDummyProgram* pDlg = new CDummyProgram();
			pDlg->Create(CDummyProgram::IDD,this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_DEBUG, wcMenuMsg))
		{
			CDebugDlg* lpDlg = new CDebugDlg();
			lpDlg->Create(CDebugDlg::IDD,this);

			pTree->SelectItem(NULL);

			SwapFrontWindow(lpDlg->m_hWnd);	//V1.1.W245 add
		}
		else if (theMsg.IsEqual(MSG_DIAGNOSTIC_TOOL,wcMenuMsg))	//V1.4.W43 edit
		{
			CDebugToolSheet* lpSheet = new CDebugToolSheet(wcMenuMsg);
			lpSheet->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpSheet->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_USG_SETUP, wcMenuMsg))	//V1.4.W41 edit
		{
			CUsgDebugPage* pDlg = new CUsgDebugPage();
			pDlg->Create(CUsgDebugPage::IDD,this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_BSD_DEBUG,wcMenuMsg))
		{
			CUsbBsdDebugPage* pDlg = new CUsbBsdDebugPage();
			pDlg->Create(CUsbBsdDebugPage::IDD,this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pDlg->m_hWnd);	//V1.4.W38 add
		}
		else if (theMsg.IsEqual(MSG_AFP_MODE,wcMenuMsg))	//V1.5.W67 edit
		{
			CAfpToolSheet* lpDlg = new CAfpToolSheet(wcMenuMsg);
			lpDlg->Create(this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_ENTER_STANDBY_MODE, wcMenuMsg))
		{
			short nInputPassWord = 0;
			theKeyPad.GetUserKeyInData(&nInputPassWord, 0, 0, 0, false, true);

			if (nInputPassWord == 1618)
			{
				CSleepModeDlg SleepModeDlg;
				SleepModeDlg.DoModal();
			}

			EnableSystemButton();
			pTree->SelectItem(NULL);
		}
		else if (theMsg.IsEqual(MSG_SOFTWARE_MANAGER, wcMenuMsg))
		{
			CSoftwareMgrDlg* lpDlg = new CSoftwareMgrDlg;
			lpDlg->Create(IDD_SOFTWARE_MGR,this);
			lpDlg->ShowWindow(SW_SHOW);
			pTree->SelectItem(NULL);

			SwapFrontWindow(lpDlg->m_hWnd);
		}
		else if (theMsg.IsEqual(MSG_CTACT_LEVEL_SRCH, wcMenuMsg))	//V1.4.W39 edit
		{
			CCtactSrchTestPage* pDlg = new CCtactSrchTestPage();
			pDlg->Create(CCtactSrchTestPage::IDD, this);
			pTree->SelectItem(NULL);

			SwapFrontWindow(pDlg->m_hWnd);
		}
		break;
	}
    default:
		break;
	}
}

void CWireBondDlg::InhibitSystemButton(void)
{
    m_bButtonInhibited = true;    
}

void CWireBondDlg::EnableSystemButton(void)
{
    m_bButtonInhibited = false;
}

BYTE CWireBondDlg::CalcMatrixRow()	//V1.5.W89 add
{
	if (thePgm.GetMatrixRow() < 2)
		return m_pDlgOverView->SetMatrixRow(1);

	TBL_POS tp;
	theServo.GetTablePosition(&tp,MOTION_WITHOUT_WAIT);

	//v1.5.T137 edit
	REF_RECORD stRefRcd;
	theRef.GetRcd(&stRefRcd, 1);
	double dblOrigin = stRefRcd.stVisionAlignPos[0].dy;

	double dblSpan = thePgm.GetDeviceRowNum() * thePgm.GetUnitRowOffset().dy;	//v1.5.T137 edit

	//Row 1
	if (tp.dy > dblOrigin + dblSpan) return m_pDlgOverView->SetMatrixRow(1);

	//Row 2
	if (tp.dy < dblOrigin + thePgm.GetMatrixRowOffset().dy)
		return m_pDlgOverView->SetMatrixRow(2);

	return m_pDlgOverView->GetMatrixRow();
}

// Update program over view
void CWireBondDlg::UpdateOverView(void)
{
	if (theMcConfig.IsBypassServo()) return;	//v3.1.T132 edit

    CDC *pCDC = m_pDlgOverView->GetDC();
    m_pDlgOverView->Invalidate();
    m_pDlgOverView->UpdateWindow();
    
    pCDC->SelectStockObject(BLACK_BRUSH);
    pCDC->Rectangle(0, 0, OVER_VIEW_WIDTH-5, OVER_VIEW_HEIGHT-5);

	CPen penWhite;
    penWhite.CreatePen(PS_SOLID, 1, RGB(255,255,255));

	TBL_POS stCursorPos;
	CPoint stObjPt;
	double dPosLmtX, dNegLmtX, dPosLmtY, dNegLmtY;
	double dScaleX, dScaleY;
	double dOriX = OVER_VIEW_WIDTH / 2.0 - 1, dOriY = OVER_VIEW_HEIGHT / 2.0 - 1;
    if (thePgm.IsProgramValid())
    {
        thePgm.CalcProgramWorkArea(&dPosLmtX, &dPosLmtY, &dNegLmtX, &dNegLmtY);

        dScaleX = (OVER_VIEW_WIDTH  - 10) / (dPosLmtX - dNegLmtX) / 1.2;
        dScaleY = (OVER_VIEW_HEIGHT - 10) / (dPosLmtY - dNegLmtY) / 1.2;
        dScaleX = min(dScaleX, dScaleY);
        dScaleY = -dScaleX;
        dScaleX = dScaleY;
        dOriX -= (dPosLmtX + dNegLmtX) / 2.0 * dScaleX;
        dOriY -= (dPosLmtY + dNegLmtY) / 2.0 * dScaleY;

        m_stOriPos.dx = dOriX;
        m_stOriPos.dy = dOriY;
        m_dPlotScaleX = dScaleX;
        m_dPlotScaleY = dScaleY;

		short nTrackNum;
		const PGM_RECORD *pstPgm = thePgm.GetRcd();
        if (pstPgm->nTrackType == DUAL_TRACK)
            nTrackNum = 2;
        else
            nTrackNum = 1;

		short nTotalRef = theRef.GetTotalRefNum();
		short nTotalWire = min(theWire.Get1stBondTaughtPts(), theWire.Get2ndBondTaughtPts());

		CFont font;
		font.CreateFontW(10, 0, 0, 0, 500, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
        CPen penYellow, penRed;
	    penYellow.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	    penRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		pCDC->SelectObject(&penYellow);
        pCDC->SetBkColor(RGB(0, 0, 0));
        pCDC->SetTextColor(RGB(0, 255, 0));
		pCDC->SelectObject(&font);

	    CString str;
		const WIRE_RECORD *pstWire;
		const REF_RECORD *pstRef;
		TBL_POS stTrackOffset, st1stBondPt, st2ndBondPt;
		short i, j;
        for (j = 0; j < nTrackNum; j++)
        {
            stTrackOffset.dx = pstPgm->stTrackOffset.dx * j;
            stTrackOffset.dy = pstPgm->stTrackOffset.dy * j;
            
            for (i = 0; i < nTotalRef; i++)
            {
                pstRef = theRef.GetRcd(i);
				if (theMatrix.GetRcd(pstRef->nRefNum)->nVisionStatus != VISION_SUCCESS)
			        pCDC->SelectObject(&penRed);
				else
			        pCDC->SelectObject(&penYellow);

				//manual align pt 1
                stObjPt.x = round(dOriX + (pstRef->stManuAlignPos[0].dx + stTrackOffset.dx) * dScaleX);
                stObjPt.y = round(dOriY + (pstRef->stManuAlignPos[0].dy + stTrackOffset.dy) * dScaleY);
                pCDC->MoveTo(stObjPt.x - 5, stObjPt.y);
                pCDC->LineTo(stObjPt.x + 5, stObjPt.y);
                pCDC->MoveTo(stObjPt.x, stObjPt.y - 5);
                pCDC->LineTo(stObjPt.x, stObjPt.y + 5);

				//manual align pt 2
                stObjPt.x = round(dOriX + (pstRef->stManuAlignPos[1].dx + stTrackOffset.dx) * dScaleX);
                stObjPt.y = round(dOriY + (pstRef->stManuAlignPos[1].dy + stTrackOffset.dy) * dScaleY);
                pCDC->MoveTo(stObjPt.x - 5, stObjPt.y);
                pCDC->LineTo(stObjPt.x + 5, stObjPt.y);
                pCDC->MoveTo(stObjPt.x, stObjPt.y - 5);
                pCDC->LineTo(stObjPt.x, stObjPt.y + 5);
            }

            for (i = 1; i <= nTotalWire; i++)
            {
                //theWire.CalcBondPoint(i, &st1stBondPt, &st2ndBondPt);
                pstWire = theWire.GetRcd(i);
                st1stBondPt = pstWire->stTchPos[0];
                st2ndBondPt = pstWire->stTchPos[1];
                stObjPt.x = round(dOriX + (st1stBondPt.dx + stTrackOffset.dx) * dScaleX);
                stObjPt.y = round(dOriY + (st1stBondPt.dy + stTrackOffset.dy) * dScaleY);
                pCDC->MoveTo(stObjPt.x, stObjPt.y);

                stObjPt.x = round(dOriX + (st2ndBondPt.dx + stTrackOffset.dx) * dScaleX);
                stObjPt.y = round(dOriY + (st2ndBondPt.dy + stTrackOffset.dy) * dScaleY);
                pCDC->LineTo(stObjPt.x, stObjPt.y);

                stObjPt.x = round(dOriX + ((st1stBondPt.dx + st2ndBondPt.dx) / 2.0 + stTrackOffset.dx) * dScaleX);
                stObjPt.y = round(dOriY + ((st1stBondPt.dy + st2ndBondPt.dy) / 2.0 + stTrackOffset.dy) * dScaleY);
                str.Format(_T("%d"), i);
                pCDC->TextOut(stObjPt.x + 1, stObjPt.y + 1, str);
                pCDC->SelectObject(&penYellow);
            }
        }

        pCDC->SelectObject(&penWhite);
         
		theServo.GetTablePosition(&stCursorPos, MOTION_WITHOUT_WAIT);
		stCursorPos.dy -= (CalcMatrixRow() - 1) * thePgm.GetMatrixRowOffset().dy;	//V1.5.W89 edit, v1.5.T137 edit
        stObjPt.x = round(dOriX + stCursorPos.dx * dScaleX);
        stObjPt.y = round(dOriY + stCursorPos.dy * dScaleY);
        if ((stObjPt.y > 0) && (stObjPt.y < OVER_VIEW_HEIGHT - 2))
        {
            pCDC->MoveTo(0, stObjPt.y);
            pCDC->LineTo(OVER_VIEW_WIDTH - 2, stObjPt.y);
        }
        if ((stObjPt.x > 0) && (stObjPt.x < OVER_VIEW_WIDTH - 2))
        {
            pCDC->MoveTo(stObjPt.x, 0);
            pCDC->LineTo(stObjPt.x, OVER_VIEW_HEIGHT - 2);
        }

		m_pDlgOverView->DrawMatrixRow();	//V1.5.W88 add
	}
    else
    {
        theServo.GetTableMechLimit(&dPosLmtX, &dNegLmtX, &dPosLmtY, &dNegLmtY);

        dScaleX = (OVER_VIEW_WIDTH  - 10) / (dPosLmtX - dNegLmtX) / 1.2;
        dScaleY = (OVER_VIEW_HEIGHT - 10) / (dPosLmtY - dNegLmtY) / 1.2;
        dScaleX = min(dScaleX, dScaleY);
        dScaleY = -dScaleX;
        dScaleX = dScaleY;
        dOriX -= (dPosLmtX + dNegLmtX) / 2.0 * dScaleX;
        dOriY -= (dPosLmtY + dNegLmtY) / 2.0 * dScaleY;

        m_stOriPos.dx = dOriX;
        m_stOriPos.dy = dOriY;
        m_dPlotScaleX = dScaleX;
        m_dPlotScaleY = dScaleY;

        pCDC->SelectObject(&penWhite);

		theServo.GetTablePosition(&stCursorPos, MOTION_WITHOUT_WAIT);
        stObjPt.x = round(dOriX + stCursorPos.dx * dScaleX);
        stObjPt.y = round(dOriY + stCursorPos.dy * dScaleY);
        if ((stObjPt.y > 0) && (stObjPt.y < OVER_VIEW_HEIGHT - 2))
        {
            pCDC->MoveTo(0, stObjPt.y);
            pCDC->LineTo(OVER_VIEW_WIDTH - 2, stObjPt.y);
        }

        if ((stObjPt.x > 0) && (stObjPt.x < OVER_VIEW_WIDTH - 2))
        {
            pCDC->MoveTo(stObjPt.x, 0);
            pCDC->LineTo(stObjPt.x, OVER_VIEW_HEIGHT - 2);
        }
    }

    pCDC->SelectStockObject(BLACK_BRUSH);
    m_pDlgOverView->ReleaseDC(pCDC);
}

CString CWireBondDlg::GetUserLevel()		//V1.5.W88 edit
{
	if (m_iUserLevel == IDM_OPERATOR - IDM_OPERATOR) return theMsg.GetMsg(MSG_OPERATOR);
	if (m_iUserLevel == IDM_PROCESS - IDM_OPERATOR) return theMsg.GetMsg(MSG_PROCESS);
	if (m_iUserLevel == IDM_SERVICE - IDM_OPERATOR) return theMsg.GetMsg(MSG_SERVICE);
	if (m_iUserLevel == IDM_ENGINEER - IDM_OPERATOR) return theMsg.GetMsg(MSG_ENGINEER);

	return _T("");
}

void CWireBondDlg::UpdateInfoView(EN_INFOVIEW_TYPE nType)	//v3.0.T110 edit, v3.0.T463 edit
{
	CFont font;
	font.CreateFontW(20, 0, 0, 0, 500, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    CDC *pCDC = m_pInfoView->GetDC();
	pCDC->SetBkColor(theApp.m_bkColor);
    pCDC->SetTextColor(RGB(0, 0, 0));
    pCDC->SelectObject(&font);
	pCDC->SelectStockObject(BLACK_BRUSH);

    m_pInfoView->Invalidate();

    CString str;
	if (nType == INFOVIEW_ALL)
	{
		CRect rc;
		m_pInfoView->GetClientRect(&rc);
		pCDC->FillRect(&rc, &CBrush(theApp.m_bkColor));

		str.Format(_T("%s: %s"), theMsg.GetMsg(MSG_USER_LEVEL), GetUserLevel());
		pCDC->TextOut(INFO_VIEW_1ST_COL, INFO_VIEW_1ST_ROW, str);	//V1.1.W38 add

		//recipe name
		if (thePgm.IsProgramValid())
		    str.Format(_T("%s"), CString((LPSTR)thePgm.GetRcd()->cPgmName));
		else
			str = theMsg.GetMsg(MSG_NO_RECIPE);
	    str.AppendFormat(_T(" (%s%d - %s)"), theMsg.GetMsg(MSG_MC_NUM),
					theMcConfig.GetMachineID(), theApp.GetSoftwareVersion());
		if (theDebug.GetDebugMode() > 0)
			str.AppendFormat(_T(" debug %d"), theDebug.GetDebugMode());
		pCDC->TextOut(INFO_VIEW_2ND_COL, INFO_VIEW_1ST_ROW, str);

		//NSOL Sample
		str.Format(_T("%s %d"), theMsg.GetMsg(MSG_NSOL_SAMPLE_COLON),
								theBond.m_nBondStickSample);
		pCDC->TextOut(INFO_VIEW_4TH_COL, INFO_VIEW_3RD_ROW, str);
	}

	if ((nType == INFOVIEW_ALL) || (nType == INFOVIEW_WIRE_INFO))
	{
		//Total Wire
		str.Format(_T("%s %d"), theMsg.GetMsg(MSG_TOTAL_WIRE), thePgm.GetTotalWire());
		pCDC->TextOut(INFO_VIEW_1ST_COL, INFO_VIEW_2ND_ROW, str);

		//Restart Wire
		str.Format(_T("%s %d"), theMsg.GetMsg(MSG_RESTART_WIRE), theBond.m_nRestartWire);
		pCDC->TextOut(INFO_VIEW_2ND_COL, INFO_VIEW_2ND_ROW, str);

		double dWireLen = 0.0;
	    short nLoopMode = -1;
	    CString sBondType = _T("");
		short nRefNum[2] = {-1, -1};	//v3.0.T469 edit
		//nRefNum[0] = nRefNum[1] = -1;	//v3.0.T469 delete
		CString asVisionStatus[2] = {_T(""), _T("")};		//v3.0.T469 edit
		//asVisionStatus[0] = asVisionStatus[1] = _T("");	//v3.0.T469 delete

		if (min(theWire.Get1stBondTaughtPts(), theWire.Get2ndBondTaughtPts()) > 0)
		{
			if (theBond.m_nCurWire > thePgm.GetTotalWire())
				theBond.m_nCurWire = thePgm.GetTotalWire();

			dWireLen = theWire.GetWireLength(theBond.m_nCurWire);

			const WIRE_RECORD *pstWire = theWire.GetRcd(theBond.m_nCurWire);
			switch (pstWire->nBumpType)	//v3.0.T465 edit
			{
			case BUMP_NONE:	sBondType = _T("Wire"); break;
			case BUMP_BOS:	sBondType = _T("BBOS"); break;	//v3.0.T429 rename
			case BUMP_SOB:	sBondType = _T("BSOB"); break;	//v3.0.T429 rename
			case BUMP_BSB:	sBondType = _T("BSB"); break;	//v3.1.T88 add
			default:		sBondType = _T("Err");
			}

			nLoopMode = pstWire->nLoopMode;

			const MATRIX_RECORD *pstMat;
			nRefNum[0] = pstWire->nRefNum[0];
			pstMat = theMatrix.GetRcd(nRefNum[0]);
			asVisionStatus[0] = GetVisionStatusString(pstMat->nVisionStatus);

			nRefNum[1] = pstWire->nRefNum[1];
			pstMat = theMatrix.GetRcd(nRefNum[1]);
			asVisionStatus[1] = GetVisionStatusString(pstMat->nVisionStatus);
		}

		//Current Wire
		str.Format(_T("%s %d (%d um)"), theMsg.GetMsg(MSG_CUR_WIRE),
								theBond.m_nCurWire, round(dWireLen));
		pCDC->TextOut(INFO_VIEW_3RD_COL, INFO_VIEW_2ND_ROW, str);

		//Row/Column
		str.Format(_T("%s/%s: %d/%d"),
			theMsg.GetMsg(MSG_ROW), theMsg.GetMsg(MSG_COL),
			theBond.m_nCurRow, theBond.m_nCurCol);
		pCDC->TextOut(INFO_VIEW_1ST_COL, INFO_VIEW_3RD_ROW, str);

		//Ref 1
		str.Format(_T("%s: %d (%s)"), theMsg.GetMsg(MSG_REF1),
								nRefNum[0], asVisionStatus[0]);
		pCDC->TextOut(INFO_VIEW_2ND_COL, INFO_VIEW_3RD_ROW, str);

		//Ref 2
		str.Format(_T("%s: %d (%s)"), theMsg.GetMsg(MSG_REF2),
								nRefNum[1], asVisionStatus[1]);
		pCDC->TextOut(INFO_VIEW_3RD_COL, INFO_VIEW_3RD_ROW, str);

		//Bond Type
		str.Format(_T("%s: %s"),
			theMsg.GetMsg(MSG_TYPE), sBondType);
		pCDC->TextOut(INFO_VIEW_4TH_COL, INFO_VIEW_4TH_ROW, str);

		//Loop Mode
		str = theMsg.GetMsg(MSG_LOOP);
		if (nLoopMode == -1)
			str.AppendFormat(_T(": -     "));
		else
			str.AppendFormat(_T(": %d"), nLoopMode);
		pCDC->TextOut(INFO_VIEW_5TH_COL, INFO_VIEW_4TH_ROW, str);
	}

	if ((nType == INFOVIEW_ALL) || (nType == INFOVIEW_HEATER))
	{
		//Heater Status
		str.Format(_T("%s:"), theMsg.GetMsg(MSG_HEATER));
		pCDC->TextOut(INFO_VIEW_5TH_COL, INFO_VIEW_3RD_ROW, str);            

	    CRect rectHeater(INFO_VIEW_5TH_COL+60, INFO_VIEW_3RD_ROW,
							INFO_VIEW_5TH_COL+110, INFO_VIEW_3RD_ROW+20);
		if (theScanKeyThread.m_HeaterDetector.GetSensorState() == SENSOR_OFF)
		{
		    CBrush oGreenBrush(RGB(0, 255, 0));
			pCDC->SelectObject(&oGreenBrush);
			pCDC->FillRect(&rectHeater, &oGreenBrush);
		}
		else
		{
		    CBrush oRedBrush(RGB(255, 0, 0));
			pCDC->SelectObject(&oRedBrush);
			pCDC->FillRect(&rectHeater, &oRedBrush);
		}
		pCDC->SelectStockObject(BLACK_BRUSH);
	}

	if ((nType == INFOVIEW_ALL) || (nType == INFOVIEW_TABLE_POSITION))
	{
		str.Format(_T("X: %-10.1f"), round(m_CurPos.dx) * theServo.GetResolXY());
		pCDC->TextOut(INFO_VIEW_1ST_COL, INFO_VIEW_4TH_ROW, str);

		str.Format(_T("Y: %-10.1f"), round(m_CurPos.dy) * theServo.GetResolXY());
		pCDC->TextOut(INFO_VIEW_2ND_COL, INFO_VIEW_4TH_ROW, str);

		str.Format(_T("Z: %.1f (um)"), round(m_dCurBondHeadLevel) * theServo.GetResolZ());
		pCDC->TextOut(INFO_VIEW_3RD_COL, INFO_VIEW_4TH_ROW, str);
	}

	//Time
	str.Format(_T("%s %s"), theMsg.GetMsg(MSG_TIME),
		COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));
	pCDC->TextOut(INFO_VIEW_5TH_COL, INFO_VIEW_1ST_ROW, str);

    m_pInfoView->ReleaseDC(pCDC);
}

// Tool bar lighting control dialog box
void CWireBondDlg::OnLight(void)
{
	m_pLightCtrlDlg->UpdateDisp();
	m_pLightCtrlDlg->ShowWindow(SW_SHOW);
}

// Tool bar lighting control dialog box
void CWireBondDlg::OnIndex(void)
{
	if (m_pIndexCtrlDlg->IsWindowVisible()) return;

	m_pIndexCtrlDlg->UpdateMatrix();
	m_pIndexCtrlDlg->ShowWindow(SW_SHOW);

	if (m_hwndFront != NULL)
		::PostMessageW(m_hwndFront,WM_SAVESETTING,NULL,NULL);	//V1.5.W117
}

void CWireBondDlg::ShowUsgInfoDialog(void)	//v3.0.T179 rename
{
	CDialog *pDlg;
	if (theMcConfig.GetUsgType() == 1)	//v3.0.T345 add
	{
		pDlg = new CFnkCaliPage();
		pDlg->Create(CFnkCaliPage::IDD, this);
	}
	else
	{
		pDlg = new CUsgInfoDispPage();
		pDlg->Create(CUsgInfoDispPage::IDD, this);
	}
}

HBRUSH CWireBondDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC )
    {
		pDC-> SetBkMode(TRANSPARENT);
		return theApp.m_brNull;
    }
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CWireBondDlg::OnTimer(UINT nIDEvent)
{
    switch (nIDEvent)
    {
    case 1:
	{
        if (theBond.GetBondMode() == BONDMODE_IDLE)
        {
            TBL_POS stBkupTblPos = m_CurPos;

            //N2H2 Auto Stop
            if (thePgm.IsN2H2Enable() == true)
            {
                if (theBond.m_N2h2Port.GetRelayState() == RELAY_ON)
                {
                    m_nN2h2TimeInterval = m_nN2h2TimeInterval + 1;

                    if (m_nN2h2TimeInterval > N2H2_AUTO_OFF_DELAY)
                    {
                        theBond.m_N2h2Port.SetRelayState(RELAY_OFF, true);
                        m_nN2h2TimeInterval = 0;
						theBond.m_N2h2Port.GetRelayState() ;	//test
                    }
                }
                else
                {
                    m_nN2h2TimeInterval = 0;
                }
            }

			if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
            {
#if _SERVO_TYPE == 2	//v3.1.T291 add
				theServo.GetTablePosition(&m_CurPos);
#else
				theServo.GetTablePosition(&m_CurPos, MOTION_WITHOUT_WAIT);
#endif
                theServo.GetBondHeadPosition(&m_dCurBondHeadLevel);
                m_dCurBondHeadLevel = m_dCurBondHeadLevel - theMcData.GetFlatLevel();
            }

			UpdateInfoView(INFOVIEW_TABLE_POSITION);

            if ((fabs(stBkupTblPos.dx - m_CurPos.dx) > 1.0)
				|| (fabs(stBkupTblPos.dy - m_CurPos.dy) > 1.0))
            {
                //UpdateInfoView(INFOVIEW_TABLE_POSITION);
                UpdateOverView();
                //UpdateVisionDrawing();
            }

            //if(theVisionSys.GetWireLayoutFlag() == true && (m_stLastPos.dx !=m_CurPos.dx || m_stLastPos.dy !=m_CurPos.dy))
            if ((fabs(m_stLastPos.dx - m_CurPos.dx) > 0.1) || (fabs(m_stLastPos.dy - m_CurPos.dy) > 0.1))
            {
                if (thePgm.IsProgramValid() == true)
                {
					if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
                    {
						//theVisionSys.DrawLine(&m_CurPos, theBond.m_nCurWire);
						theVisionDisplay.SetMatrixRowOffset(theBond.m_nCurMatRow-CalcMatrixRow());	//V1.5.W89 add
//#if _SERVO_TYPE != 2	//v3.1.T475 add, v3.1.T479 delete
						theVisionDisplay.UpdateVisionDisplay();
//#endif
						m_stLastPos =  m_CurPos;
                    }
                }
            }
        }
        else
        {
            if (thePgm.IsN2H2Enable() == true)
            {
                if (m_nN2h2TimeInterval != 0)
                    m_nN2h2TimeInterval = 0;
            }
        }

        //oxidation
        if (theMHS.IsOxidationProtect())
        {
			if ((theMcConfig.GetMhsType() == MHS_VLED)
				|| (theMcConfig.GetMhsType() == MHS_VLED_ELITE))
            {
                theVledLfClamp.CheckLfClampStatus(TRUE);
                if (theVledLfClamp.GetClampStatus() == SINGLE_CLAMP_CLOSE) 
                {
                    m_lOxidationPreventTimer += TIMER1_INTERVAL;

                    if ((m_lOxidationPreventTimer > thePgm.GetRcd()->lOxiPrevtTimeOut)
						&& (thePgm.GetRcd()->lOxiPrevtTimeOut > TIMER1_INTERVAL))
                    {
                        theVledLfClamp.OpenLfClamp();
                    }
                }
                else if (theVledLfClamp.GetClampStatus() == SINGLE_CLAMP_OPEN)
                    m_lOxidationPreventTimer = 0;
            }
        }
        else
            m_lOxidationPreventTimer = 0;

        break;
	}
    case 2:
    {
		//start auto bond
        if ((theMcConfig.GetKeyPadType() >= 1)
			&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
        {
            if (m_bButtonInhibited == false)
            {
                if (theKeyPad.IsKeyDown(KEY_BOND) == SENSOR_ON)
                {
					if (FindWindow(NULL, theMsg.GetMsg(MSG_RUN)) == NULL)	//v3.0.T486 add
					{
						theKeyPad.SetKeyDown(KEY_BOND, false);
						OnBond();
					}
                }
            }
        }

		if (theBond.IsUpdateBondData())
        {
            theBond.SetUpdateBondData(false);
            UpdateInfoView(INFOVIEW_ALL);
        }
		break;
	}
	case 3:
		VerifyRegister();
		break;
    default:
        break;
    }
}

// Table position fine adjust tool bar
void CWireBondDlg::OnAdjust(void)
{
    CPositionAdjustDlg dlg;
    dlg.DoModal();
}

// Thread wire tool bar
void CWireBondDlg::OnThread(void)
{
	static BYTE bAlready;

	if (bAlready) return;

	bAlready = TRUE;

    CThreadWireDlg dlg;
    dlg.DoModal();

	bAlready = FALSE;
}

void CWireBondDlg::OnUserLvl(UINT nID)
{
	if (m_bButtonInhibited) return;

	nID = nID - IDM_OPERATOR;

	if (nID > m_iUserLevel)
	{
		short nInputPassword, nExpectedPassword = theUser.GetPassword(nID);
		theKeyPad.GetUserKeyInData(&nInputPassword, 0, 0, 0, false, true);

		if (nInputPassword == nExpectedPassword)
			m_iUserLevel = nID;
	}
	else m_iUserLevel = nID;

    theUser.SetUserLevel(m_iUserLevel);
    UpdateMenuTree(MENU_TREE_REFRESH);

	UpdateInfoView(INFOVIEW_ALL);
}

void CWireBondDlg::MinimizeWindow()
{
	WINDOWPLACEMENT wndpl;

	GetWindowPlacement(&wndpl);
	wndpl.showCmd = SW_SHOWMINIMIZED;
	SetWindowPlacement(&wndpl);
}

void CWireBondDlg::GetScreenResolution(void)
{
	int nFullWidth = GetSystemMetrics(SM_CXSCREEN);     
    int nFullHeight = GetSystemMetrics(SM_CYSCREEN); 

	if (nFullWidth == 1024 && nFullHeight == 768)
		m_nScreenType = 0;
	else if (nFullWidth == 1280 && nFullHeight == 768)
		m_nScreenType = 1;
	else
		m_nScreenType = 1;
}

// Enable or disable tool bar button
void CWireBondDlg::EnableButton(int nID, BOOL bEnable)
{
    GetDlgItem(nID)->EnableWindow(bEnable);
}

void CWireBondDlg::ShowButton(int nID, BYTE bShow)
{
	GetDlgItem(nID)->ShowWindow(bShow ? SW_NORMAL : SW_HIDE);
}

void CWireBondDlg::OnSpeedUp()
{
	theServo.JogSpeedUp();
}

void CWireBondDlg::OnSpeedDown()
{
	theServo.JogSpeedDown();
}

void CWireBondDlg::OnElevator()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		CDlgElevCtrl dlg;
		dlg.DoModal();
		break;
	}
	case MHS_CU_ELITE:
	{
		CDlgCuEliteElevCtrl dlg;
		dlg.DoModal();
		break;
	}
    }
}

void CWireBondDlg::AutoPadCentering()
{
	if (gbAPC == false) return;

	PGM_RECORD stPgmRcd;
	thePgm.GetRcd(&stPgmRcd);

	if (stPgmRcd.bAutoPadCentering == FALSE) return;

	theVisionSys.ClearDrawing();
	theVisionSys.DrawRectangle();
	theServo.GetTablePosition(&m_stLastPos);

	Sleep_x1ms(20);

    TBL_POS stOffset;
	if (theVisionSys.GetCentrePos(gnAlgorithm, stPgmRcd.nGapFill, stPgmRcd.bBinarize, stPgmRcd.bCircleFit, &stOffset) == FALSE) return;

    PR_SRCH_RESULT stSrchResult;
	stSrchResult.dOffsetX = stOffset.dx;
	stSrchResult.dOffsetY = stOffset.dy;

	theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stOffset);
	theServo.MoveTable2(stOffset, false);	//v3.1.T205 edit
	theServo.GetTablePosition(&m_stLastPos);
}

void CWireBondDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_b1stMeasurePtSelected = false;	//v3.0.T313 add
	if (theScanKeyThread.IsJoyStickValid()
		&& (theScanKeyThread.IsDirectionKeyPressed() == false)
		&& (m_bProcessKeyboard == false))
	{
		CPoint pt;
		TBL_POS stOffset;
		CRect rect;
		m_Image.GetWindowRect(&rect);
		if (rect.PtInRect(point))
		{
			//pt.x =point.x - ((rect.left + rect.right) / 2 - 1);
			//pt.y =point.y - ((rect.top + rect.bottom) / 2 - 1); 
			pt.x = point.x - (rect.left + VISION_VIEW_WIDTH / 2 - 2);	//V1.1.W129 edit
			pt.y = point.y - (rect.top + VISION_VIEW_HEIGHT / 2 - 2);	//V1.1.W129 edit

			PR_SRCH_RESULT stSrchResult;
			stSrchResult.dAngle = 0;
			stSrchResult.dOffsetX = pt.x;
			stSrchResult.dOffsetY = pt.y;
			stSrchResult.dScale = 1;
			stSrchResult.dScore = 1;
			theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stOffset);

			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
			theServo.MoveTable2(stOffset, false);	//v3.1.T205 edit

			AutoPadCentering();
		}

		if (m_pDlgOverView->IsWindowVisible())	//V1.1.W116 edit
		{
			m_pDlgOverView->GetWindowRect(&rect);
			if (rect.PtInRect(point))
			{
				if (m_dPlotScaleX == 0 || m_dPlotScaleY == 0) //throw "w.temp.29" ;
				{
					AfxMessageBox(_T("temp.29, m_dPlotScaleX = 0 || m_dPlotScaleX = 0"));
					return;
				}
				pt.x = point.x - rect.left;
				pt.y = point.y - rect.top; 
				stOffset.dx = round((pt.x - m_stOriPos.dx) / m_dPlotScaleX);
				stOffset.dy = round((pt.y - m_stOriPos.dy) / m_dPlotScaleY) + (m_pDlgOverView->GetMatrixRow() - 1) * thePgm.GetMatrixRowOffset().dy;	//V1.5.W88 edit, v1.5.T137 edit
				theServo.MoveTable2(stOffset);	//v3.1.T205 edit
			}
		}
	}
	CDialog::OnRButtonDown(nFlags, point);
}

void CWireBondDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	m_Image.GetWindowRect(&rect);

	short nSrchX, nSrchY, nIncreX = 0, nIncreY = 0;
	if (rect.PtInRect(point))
	{
        if (nFlags & MK_LBUTTON)
		{
            if (theVisionSys.IsEditPatternMask())
            {
                theVisionSys.EditPatternMask((short)(point.x - rect.left), (short)(point.y - rect.top));
            }
			else if (m_hwndClient != NULL)
			{
				theVisionSys.GetSearchSize(&nSrchX, &nSrchY);

				if ((point.x >= (rect.left+rect.right)/2 - nSrchX/2)
					&& (point.x <= (rect.left+rect.right)/2 + nSrchX/2)
					&& (point.y >= (rect.top+rect.bottom)/2 - nSrchY/2)
					&& (point.y <= (rect.top+rect.bottom)/2 + nSrchY/2))
				{
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));

					nIncreX = (short)(abs(point.x - (rect.left+rect.right)/2) - abs(m_LeftBtnDownPoint.x));
					nIncreY = (short)(abs(point.y - (rect.top+rect.bottom)/2) - abs(m_LeftBtnDownPoint.y));

					::PostMessage(m_hwndClient, WM_REFRESH, MAKELONG(nSrchX + nIncreX, nSrchY+nIncreY), NULL);

					theVisionSys.SetSearchSize(nSrchX + 2*nIncreX, nSrchY + 2*nIncreY);

					m_LeftBtnDownPoint.x = point.x - (rect.left+rect.right)/2;
					m_LeftBtnDownPoint.y = point.y - (rect.top+rect.bottom)/2;
				}
			}
        }
        else
            SetCursor(LoadCursor(NULL,IDC_CROSS));	//V1.5.W106 edit
	}
	else
		SetCursor(LoadCursor(NULL,IDC_ARROW));	//V1.5.W106 add

	//V1.1.W116 add
	if (m_pDlgOverView->IsWindowVisible() == FALSE)
	{
		CRect rc;
		m_pInfoView->GetWindowRect(&rc);

		m_pDlgOverView->GetWindowRect(&rect);
		rect.IntersectRect(&rc,&rect);
		if (rect.PtInRect(point))
		{
			m_pDlgOverView->ShowWindow(SW_SHOW);
			UpdateOverView();
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CWireBondDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CRect rect;
	m_Image.GetWindowRect(&rect);
	if (rect.PtInRect(point))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CDialog::OnRButtonUp(nFlags, point);
}

// for draw pattern size
void CWireBondDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (theMcConfig.IsBypassCamera()) theVisionSys.GrabImage();	//v3.0.T392 add
	theVisionSys.ToggleShowResult();

	//v3.0.T313 add
	if (!m_b1stMeasurePtSelected)
	{
		m_st1stMeasurePt.dx = point.x;
		m_st1stMeasurePt.dy = point.y;
		m_b1stMeasurePtSelected = true;
	}
	else
	{
		double dLen =  theServo.GetResolXY()
				* sqrt(pow((point.x-m_st1stMeasurePt.dx)*theVisionSys.GetLowMagFactorX(),2)
				+ pow((point.y-m_st1stMeasurePt.dy)*theVisionSys.GetLowMagFactorY(),2));
		//if (dLen > 45) dLen += 25;
		//else if (dLen < 20) dLen -= 6;
		CString str;
		str.Format(_T("Length=%.1fum"), dLen);
		::AfxMessageBox(str);
		m_b1stMeasurePtSelected = false;
	}

	if ((m_hwndClient != NULL) && (theVisionSys.IsEditPatternMask() == false))
	{
		CRect rect;
		m_Image.GetWindowRect(&rect);

		if (rect.PtInRect(point))
		{
			m_LeftBtnDownPoint.x = point.x - (rect.left+rect.right) / 2;
			m_LeftBtnDownPoint.y = point.y - (rect.top+rect.bottom) / 2;

			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CWireBondDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);
}

void CWireBondDlg::SetDrawAreaEnable(HWND hClient)
{
	m_hwndClient = hClient;
}

void CWireBondDlg::OnUnplug()
{
    CStringList sList;

	DUMMY_BOND_AREA_RCD stDummyBondAreaRcd;
    theBondPara.GetDummyBondAreaRcd(&stDummyBondAreaRcd);
    if (stDummyBondAreaRcd.bBondAreaValid == false)
    {
        sList.AddTail(theMsg.GetMsg(MSG_TCH_DUMMY_BOND_AREA_2)); 
        theMsg.ShowMsg(HELP_MSG, sList);
        return;
    }

	sList.AddTail(theMsg.GetMsg(MSG_UNPLUG_2));
	if (theMsg.ShowMsgX(WARNING_MSG,sList,0,0,1)
		== SHOW_MSG_RESULT_STOP) return;

    theDummyBond.CapillaryUnplug();
}

CString CWireBondDlg::GetVisionStatusString(short nVisionStatus)
{
	switch (nVisionStatus)
	{
	case 0:
		return (_T("No PR"));
		break;
	case 1:
		return (_T("PR OK"));
		break;
	case 2:
		return (_T("Skip"));
		break;
	case 3:
		return (_T("Fail"));
		break;
	default:
		return (_T(""));
		break;
	}
}

void CWireBondDlg::SetDefaultMachineOffset(short nMaterialHandlerType)
{
	switch (nMaterialHandlerType)
	{
	case MHS_CU_ELITE:
		CUELITE_STEPPER_CLAMP_PARA  stCuElitePosPara;		
		theCuEliteLFClamp.GetClampRcd(&stCuElitePosPara);		
		stCuElitePosPara.lWindClampClosePos = 10;
		theCuEliteLFClamp.SetClampRcd(&stCuElitePosPara);

		CUELITE_TRACK_DATA stTrackRcd;		
		theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);		
		stTrackRcd.lLFFrontTrackStandby = 59720;				//0.1um
		stTrackRcd.lLFBackTrackStandby = 2680;					//0.1um
		theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

		CUELITE_ONLOAD_RCD  stOnloadData;		
		theCuEliteOnloader.GetCuEliteOnloadRcd(&stOnloadData);		
		stOnloadData.lOnload1stSlotLevel = 0;					//0.1um
		stOnloadData.lOnLoadMagLengthStandby = 122320;		//0.1um
		stOnloadData.lOnLoadMagClamperBackStandby = 10640;		//0.1um
		stOnloadData.lOnLoadMagClamperFrontStandby = 12720;		//0.1um
		stOnloadData.lOnLoadMagClamperDepth = 720;				//0.1um
		theCuEliteOnloader.UpdateCuEliteOnloadRcd(&stOnloadData); 

		CUELITE_OFFLOAD_RCD  stOffloadData;		
		theCuEliteOffloader.GetCuEliteOffloadRcd(&stOffloadData);		
		stOffloadData.lOffload1stSlotLevel = 0;					//0.1um
		stOffloadData.lOffLoadMagLengthStandby = 120960;	//0.1um
		stOffloadData.lOffLoadMagClamperBackStandby = 10840;		//0.1um
		stOffloadData.lOffLoadMagClamperFrontStandby = 10840;		//0.1um
		stOffloadData.lOffLoadMagClamperDepth = 720;			//0.1um
		theCuEliteOffloader.UpdateCuEliteOffloadRcd(&stOffloadData);

		MACHINE_DATA* pstMcData = theMcData.GetRcd();

		pstMcData->lZeroOffset = 0;	//um
		pstMcData->lOnLoadMagazineElevatorOffset = 0;
		pstMcData->lOnLoadMagazineLengthOffset = 0;
		pstMcData->lOnLoadMagazineYOneOffset = 0;
		pstMcData->lOnLoadMagazineYTwoOffset = 0;

		pstMcData->lOffLoadMagazineElevatorOffset = 0;
		pstMcData->lOffLoadMagazineLengthOffset = 0;
		pstMcData->lOffLoadMagazineYOneOffset = 0;
		pstMcData->lOffLoadMagazineYTwoOffset = 0;

		pstMcData->lLFFrontTrackOffset = 0;
		pstMcData->lLFRearTrackOffset = 0;
		pstMcData->lLFIndexerOffset = 0;
		pstMcData->lLFWindowClamperOffset = 0;

		break;
	}
}

LRESULT CWireBondDlg::OnClickKeyboardShortcut(WPARAM wParam, LPARAM lParam)
{
	if (m_bProcessKeyboard || (theBond.GetBondMode() != BONDMODE_IDLE)) return true;

	CDlgElevCtrl objElev;
	m_bProcessKeyboard = true;
	switch (wParam)
	{
	case KEY_SETUP:		//v3.0.T452 add
		OnSetup();
		break;
	case KEY_RECIPE:		//v3.0.T454 add
		OnProgram();
		break;
	case KEY_CHG_CAPILLARY:		//V1.1.W270 add
	{
		if (theKeyPad.IsKeyProhibited(KEY_CHG_CAPILLARY)) break;
		PostMessage(WM_COMMAND,
			MAKELONG(ID_TOOL_BAR_THREAD_WIRE, BN_CLICKED),
			(LPARAM)GetDlgItem(ID_TOOL_BAR_THREAD_WIRE)->m_hWnd);
		break;
	}
	case KEY_INDEXING:
	{
		if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
		{
			theMHS.IndexLeadFrame();
			theBond.SetUpdateBondData(true);
		}
		break;
	}
	case KEY_CLEAR_TRACK:
	{
		if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
		{
			theMHS.ClearTrack();
			theBond.m_nCurMatRow = 1;
			theBond.m_nCurMatCol = 1;
			theBond.m_nCurWire = 1;
			theBond.m_nRestartWire = 1;
			theBond.m_nBondedWire = 0;
			theBond.SetUpdateBondData(true);
		}
		break;
	}
	case KEY_IN_MAG_UP:
	{
		if (objElev.IsLeadframeOnTrack(TOP_ELEV_ONLOAD))
		{
			m_bProcessKeyboard = false;
			return false;
		}
		if (objElev.IsPusherError())
		{
			m_bProcessKeyboard = false;
			return false;
		}
		if (theTopOnloader.GetCurSlot()-1 > 0)
			theTopOnloader.MoveMagHolderToSlot(theTopOnloader.GetCurSlot()-1);
		break;
	}
	case KEY_IN_MAG_DOWN:
	{
		if (objElev.IsLeadframeOnTrack(TOP_ELEV_ONLOAD)	
			|| objElev.IsMagBelow(TOP_ELEV_ONLOAD))
		{
			m_bProcessKeyboard = false;
			return false;
		}
		if (objElev.IsPusherError())
		{
			m_bProcessKeyboard = false;
			return false;
		}
		theTopTrack.MoveIndexerToLogicZero();
		Sleep(10);
		theTopTrack.WaitMoveIndexer();

		TOP_ONLOAD_RCD stOnload;
		theTopOnloader.GetRcd(&stOnload);
		if (theTopOnloader.GetCurSlot() == stOnload.lOnloadSlotNum)
		{
			theTopOnloader.m_PullyMagExistSensor.UpdateSensorState();
			if (theTopOnloader.m_PullyMagExistSensor.GetSensorState() == TRUE)	//check mag full sensor
			{
				short nMsgNum, anMsgArray[10]; 
				nMsgNum = 1;
				anMsgArray[0] = MSG_ONLOAD_MAG_PULLY_FULL;
				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
				m_bProcessKeyboard = false;
				return false;
			}
			theTopOnloader.EjectOnloadMag();
		}
		else
			theTopOnloader.MoveMagHolderToSlot(theTopOnloader.GetCurSlot()+1);
		break;
	}
	case KEY_IN_MAG_HOME:
	{
		if (objElev.IsLeadframeOnTrack(TOP_ELEV_ONLOAD)
			|| objElev.IsMagBelow(TOP_ELEV_ONLOAD))
		{
			m_bProcessKeyboard = false;
			return false;
		}
		if (objElev.IsPusherError())
		{
			m_bProcessKeyboard = false;
			return false;
		}
		theTopTrack.MoveIndexerToLogicZero();
		Sleep(10);
		theTopTrack.WaitMoveIndexer();
		Sleep(100);
		theTopOnloader.HomeAll();
		break;
	}
	case KEY_OUT_MAG_UP:
	{
		if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD))
		{
			m_bProcessKeyboard = false;
			return false;
		}
		if (theTopOffloader.GetCurSlot() - 1 > 0)
			theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot()-1);
		break;
	}
	case KEY_OUT_MAG_DOWN:
	{
		if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)
			|| objElev.IsMagBelow(TOP_ELEV_OFFLOAD))
		{
			m_bProcessKeyboard = false;
			return false;
		}
		theTopTrack.MoveIndexerToLogicZero();
		Sleep(10);
		theTopTrack.WaitMoveIndexer();

		TOP_OFFLOAD_RCD stOffload;
		theTopOffloader.GetRcd(&stOffload);
		if (theTopOffloader.GetCurSlot() == stOffload.lOffloadSlotNum)
		{
			theTopOffloader.m_PullyMagExistSensor.UpdateSensorState();
			if (theTopOffloader.m_PullyMagExistSensor.GetSensorState() == TRUE)	//check mag full sensor
			{
				short nMsgNum, anMsgArray[10]; 
				nMsgNum = 1;
				anMsgArray[0] = MSG_ONLOAD_MAG_PULLY_FULL;
				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
				m_bProcessKeyboard = false;
				return false;
			}
			theTopOffloader.EjectOffloadMag();
		}
		else
			theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot()+1);
		break;
	}
	case KEY_OUT_MAG_HOME:
	{
		//v3.0.T484 add
		if (theMcConfig.GetKeyPadType() == 3)
		{
			if (FindWindow(NULL, theMsg.GetMsg(MSG_RUN)) == NULL)	//v3.0.T486 add
			{
				theKeyPad.SetKeyDown(KEY_OUT_MAG_HOME, false);
				OnBond();
			}
			break;
		}

		if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)
			|| objElev.IsMagBelow(TOP_ELEV_OFFLOAD))
		{
			m_bProcessKeyboard = false;
			return false;
		}
		theTopTrack.MoveIndexerToLogicZero();
		Sleep(10);
		theTopTrack.WaitMoveIndexer();
		theTopOffloader.HomeAll();
		break;
	}
	case KEY_WINDOW_CLAMP:
	{
		if (theMcConfig.GetKeyPadType() == 3)	//v3.0.T487 add
		{
			if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
				WinExec("osk.exe", SW_NORMAL);
		}
		else
		{
			if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
				theMHS.ToggleLfClamper();
		}
		break;
	}
	}
	m_bProcessKeyboard = false;
	return true;
}

void CWireBondDlg::SetProcessKeyboard(bool bProcess)
{
	m_bProcessKeyboard = bProcess;
	theMsg.SetProcessKeyboard(bProcess);
}

BOOL CWireBondDlg::PreTranslateMessage(MSG *pMsg)
{
	m_ToolTip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

Bitmap* CWireBondDlg::Load(HINSTANCE hInstance,UINT nRes,LPTSTR lpszType)	//V1.1.W147 add
{
	HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(nRes), lpszType);
	if (hResInfo == NULL) return NULL;

	HANDLE hRes = LoadResource(hInstance, hResInfo);
	if (hRes == NULL) return NULL;

	LPSTR lpBuf = (LPSTR)LockResource(hRes);

	int nSize = 0;
	if (lpBuf != NULL)
		nSize = SizeofResource(hInstance, hResInfo);

	Bitmap *lpBM = NULL;

	if (nSize > 0)
	{
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
		LPVOID lpData = GlobalLock(hGlobal);

		memcpy(lpData, lpBuf, nSize);

		IStream *lpStream = NULL;
		CreateStreamOnHGlobal(hGlobal, TRUE, &lpStream);

		if (lpStream)
		{
			lpBM = Bitmap::FromStream(lpStream);
			lpStream->Release();
		}

		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
	}

	UnlockResource(hRes); 
	FreeResource(hRes);

	return lpBM;
}

void CWireBondDlg::CreateBttnDC(CWnd *lpBttn, CDC *lpDC, CDC &dc, Bitmap *lpBM)	//V1.1.W147 add
{
	CRect rc;
	lpBttn->GetWindowRect(&rc);
	lpBttn->GetParent()->ScreenToClient(&rc);

	CBitmap bm;
	bm.CreateCompatibleBitmap(lpDC, rc.Width(), rc.Height());

	dc.CreateCompatibleDC(lpDC);
	dc.SelectObject(&bm);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),lpDC,rc.left,rc.top,SRCCOPY);

	Graphics g(dc.m_hDC);

	float fAlpha = lpBttn->IsWindowEnabled() ? 1.00f : 0.66f;
	ColorMatrix cm = {1.00f, 0.00f, 0.00f, 0.00f, 0.00f,
						0.00f, 1.00f, 0.00f, 0.00f, 0.00f,
						0.00f, 0.00f, 1.00f, 0.00f, 0.00f,
						0.00f, 0.00f, 0.00f, fAlpha, 0.00f,
						0.00f, 0.00f, 0.00f, 0.00f, 1.00f};

	ImageAttributes ia;
	ia.SetColorMatrix(&cm);

	RectF rcf(0,0,(short)rc.Width(),(short)rc.Height());
	g.DrawImage(lpBM,rcf,0,0,(short)lpBM->GetWidth(),(short)lpBM->GetHeight(),UnitPixel,&ia);
}

//2.0.T55 add
void CWireBondDlg::VerifyRecipe(void)
{
	CString sRcpPath;
	sRcpPath.Format(_T("%s%s"), _T(DIR_DATALOG), _T("Ref.rcp"));
	CString sZipPath(sRcpPath);
	sZipPath.Replace(_T(".rcp"),_T(".zip"));
	CFile::Rename(sRcpPath, sZipPath);

	CString str;
	str.Format(_T(DIR_DATALOG_VISION));
	theDataMgr.DeleteVisionPattern(str);
	theDataMgr.UnZipPrFile(sZipPath, str);

	CString sRecPath;
	sRecPath.Format(_T("%s%s"), _T(DIR_DATALOG_VISION), _T("*.rec"));
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(sRecPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		::AfxMessageBox(_T("No *.rec file found"));
		return;
	}
	FindClose(hFind);
	sRecPath.Format(_T("%s%s"), _T(DIR_DATALOG_VISION), CString((LPCWSTR)FindFileData.cFileName));

	int nFileLen;
	if (theDataMgr.LoadDataFromFile(sRecPath, &nFileLen) != 0) return;
	if (thePgm.VerifyData() == FNOK) goto quit;
	theRef.VerifyData();	//v2.0.T59 edit
	theWire.VerifyData();
	theAutoPadMgr.VerifyData();
	theEfoProfile.VerifyProfile();
	theBondPara.VerifyData();
	theBallPara.VerifyData();		//v3.0.T54 add
	theLoopPara.VerifyData();		//v3.0.T54 add
	theServo.VerifySpeedData();		//v3.0.T54 add
	theServo.VerifyControlData();	//v3.0.T54 add
	theMcData.VerifyData();			//v3.0.T56 add
	theMHS.VerifyData();			//v3.0.T58 add

quit:
	str.Format(_T("%s%s"), _T(DIR_DATALOG), _T("VerifyRecipe.txt"));
	ShellExecute(NULL, _T("open"), _T("NotePad.exe"),
						str, NULL, SW_SHOWMAXIMIZED);
}
