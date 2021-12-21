/***************************************************************
ThreadWireDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Reference.h"
#include "ThreadWireDlg.h"
#include "MachineData.h"
#include "BondStatistics.h"
#include "Bond.h"
#include "UsgControl.h"
#include "DigitalIO.h"
#include "MachineConfig.h"
#include "MaterialHandlerThread.h"
#include "TopClamp.h"
#include "CuClamp.h"
#include "servo.h"

// CThreadWireDlg dialog

IMPLEMENT_DYNAMIC(CThreadWireDlg, CDialog)

CThreadWireDlg::CThreadWireDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreadWireDlg::IDD, pParent)
    , m_lThreadWirePosX(0)
    , m_lThreadWirePosY(0)
    , m_lCapillaryLimit(0)
    , m_lCapillaryCount(0)
    , m_lWireUsage(0)
{

}

CThreadWireDlg::~CThreadWireDlg()
{
}

void CThreadWireDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_X_POS, m_lThreadWirePosX);
    DDX_Text(pDX, IDC_STATIC_Y_POS, m_lThreadWirePosY);
    DDX_Text(pDX, IDC_STATIC_CAPILLARY_LIMIT, m_lCapillaryLimit);
    DDX_Text(pDX, IDC_STATIC_CAPILLARY_COUNT, m_lCapillaryCount);
    DDX_Text(pDX, IDC_STATIC_WIRE_COUNT, m_lWireUsage);
}

BEGIN_MESSAGE_MAP(CThreadWireDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CThreadWireDlg::OnBnClickedButtonUpdate)
    ON_BN_CLICKED(IDC_BUTTON_RESET, &CThreadWireDlg::OnBnClickedButtonReset)
    ON_BN_CLICKED(IDC_BUTTON_RESET_WIRE_USAGE, &CThreadWireDlg::OnBnClickedButtonResetWireUsage)
	ON_BN_CLICKED(ID_BUTTON_USG, &CThreadWireDlg::OnBnClickedButtonUsg)
END_MESSAGE_MAP()

// CThreadWireDlg message handlers
BOOL CThreadWireDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDOK);
    //pWnd->SetWindowPos(NULL, 160, 350, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

	pWnd = GetDlgItem(IDC_STATIC_CAPILLARY_USAGE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CAP_USAGE));

	pWnd = GetDlgItem(IDC_STATIC_LIMIT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CAP_LIMIT));

    pWnd = GetDlgItem(IDC_STATIC_COUNT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CAP_COUNT));

    pWnd = GetDlgItem(IDC_BUTTON_RESET);
    pWnd->SetWindowPos(NULL, 275, 50, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RESET));

    pWnd = GetDlgItem(IDC_STATIC_WIRE_USG);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_WIRE_USAGE));

    pWnd = GetDlgItem(IDC_STATIC_WIRE_USAGE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BONDED_WIRE));

    pWnd = GetDlgItem(IDC_BUTTON_RESET_WIRE_USAGE);
    pWnd->SetWindowPos(NULL, 275, 150, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RESET));

    pWnd = GetDlgItem(IDC_BUTTON_UPDATE);
    pWnd->SetWindowPos(NULL, 275, 250, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_UPDATE));

	pWnd = GetDlgItem(IDC_STATIC_THREAD_WIRE_POS);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_THREAD_WIRE_POS));

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
    m_lThreadWirePosX = round(pstMcData->stThreadWirePos.dx);
    m_lThreadWirePosY = round(pstMcData->stThreadWirePos.dy);
    m_lCapillaryLimit = pstMcData->lCapillaryLimit;

	const BOND_STATISTICS_RCD *pstStatRcd = theStat.GetRcd();

    m_lCapillaryCount = pstStatRcd->lCapillaryCount;
    m_lWireUsage = pstStatRcd->lBondedWire;

	switch (theMcConfig.GetMhsType())	//v3.0.T154 edit
	{
	case MHS_TOP_LED: case MHS_R2R:
	{
		theTopClamp.CloseClamp();
		Sleep(10);
		
		long lStartTime = GetCurrentTime();
		do
		{
			theTopClamp.CheckClampStatus();
			Sleep(2);
			if (GetCurrentTime() - lStartTime > 4000) break;
		} while (theTopClamp.GetClampStatus() != WND_CLAMP_CLOSE);
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTopClamp.CloseClamp();
		Sleep_x1ms(10);
		
		CHTimer timer;
		do
		{
			theTopClamp.CheckClampStatus();
			Sleep_x1ms(2);
		} while ((theTopClamp.GetClampStatus() != WND_CLAMP_CLOSE)
					&& (timer.GetTime_x1ms() < 4000));
		break;
	}
	case MHS_CU_ELITE:
	{
		theCuEliteLFClamp.CloseClamp();
		Sleep(10);

		long lStartTime = GetCurrentTime();
		do
		{
			theCuEliteLFClamp.CheckClampStatus();
			Sleep(2);
			if (GetCurrentTime() - lStartTime > 4000) break;
		} while (theCuEliteLFClamp.GetClampStatus() != CUELITE_WND_CLAMP_CLOSE);
		break;
	}
	}
	
    theServo.MoveTable2(pstMcData->stThreadWirePos);	//v3.1.T209 edit
    theServo.SearchLimit(MOTOR_Z);
#if _SERVO_TYPE != 2	//v3.1.T390 add
	//theServo.MoveAxisZ(-1000, MOTION_RELATIVE, MOTION_WITH_WAIT);	//v1.1.288.T1 delete
	theServo.MoveAxisZ(-500, MOTION_RELATIVE, MOTION_WITH_WAIT);	//v1.1.288.T1 add
#else
	theServo.MoveAxisZ(theMcConfig.GetThreadWireMoveBH(), MOTION_RELATIVE, MOTION_WITH_WAIT);	//v3.1.T494 add
#endif
	//theBond.m_AirTensioner.SetRelayState(RELAY_ON, true);	//v3.1.T439 delete
	theBond.m_AirTensioner.SetRelayState(RELAY_OFF);	//v3.1.T439 add
    theBond.m_PanelLight.SetRelayState(RELAY_ON, true);

    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CThreadWireDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CThreadWireDlg::OnBnClickedButtonUpdate()
{
    if (theMcConfig.IsBypassAll()) return;

    MACHINE_DATA* pstMcData = theMcData.GetRcd();
	theServo.GetTablePosition(&pstMcData->stThreadWirePos);
	m_lThreadWirePosX = round(pstMcData->stThreadWirePos.dx);
	m_lThreadWirePosY = round(pstMcData->stThreadWirePos.dy);

	UpdateData(false);
}

void CThreadWireDlg::OnBnClickedButtonReset()
{
    theStat.ResetCapillaryCount();
    m_lCapillaryCount = 0;
	theRef.ResetCtactValid();
    UpdateData(false);
}

void CThreadWireDlg::OnBnClickedButtonResetWireUsage()
{
    theStat.ResetWireUsageCount();
    m_lWireUsage = 0;
    UpdateData(false);
}

void CThreadWireDlg::OnOK()
{
	//theBond.m_AirTensioner.SetRelayState(RELAY_OFF, true);	//v3.1.T439 delete
	theBond.m_AirTensioner.SetRelayState(RELAY_ON);	//v3.1.T439 add
	theBond.MoveBondHeadToFireLevel();
	CDialog::OnOK();
}

void CThreadWireDlg::OnBnClickedButtonUsg()
{
	theBond.UsgSelfDefineBond(50000, 200);
}
