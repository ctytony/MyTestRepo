/***************************************************************
AutoBondDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MacDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "Wire.h"
#include "BondPara.h"
#include "BallPara.h"
#include "Bond.h"
#include "DummyBond.h"
#include "MenuConfig.h"
#include "VisionCalibDlg.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "AutoBondSheet.h"
#include "AutoBondPage.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledOffload.h"
#include "MachineConfig.h"
#include "BsdControl.h"
#include "BondObjectDlg.h"
#include "VisionSystem.h"
#include "N2h2WaitStableDlg.h"
#include "BondFromToUnitDlg.h"
#include "UsgControl.h"
#include "AutoPad.h"
#include "EfoControl.h"
#include "MachineData.h"
#include "DataStruct.h"
#include "stdexcept"
#include "ScanKeyThread.h"
#include "TowerLight.h"
#include "CuIndex.h"
#include "CuOnload.h"
#include "TopOnload.h"
#include "R2RIndex.h"
#include "R2ROnload.h"
#include "R2ROffload.h"
#include "servo.h"
#include "Debug.h"
#include <memory>
#include "DlgRework.h"
#include "TopEliteIndex.h"
#include "Busy.h"

BYTE CAutoBondPage::m_nCount = 0;	//V1.1.W141 add

// CAutoBondPage dialog
IMPLEMENT_DYNAMIC(CAutoBondPage, CPropertyPage)

CAutoBondPage::CAutoBondPage()
	: CPropertyPage(CAutoBondPage::IDD)
    , m_nCurWire(1)
    , m_bCheckNsop(FALSE)
    , m_bCheckNsol(FALSE)
    , m_bCheckBall(FALSE)
    , m_bDryRun(FALSE)
    , m_bBondNoVision(FALSE)
    , m_bAutoIndex(FALSE)
    , m_nCurBondPt(0)
    , m_bCompleteBond(FALSE)
	, m_bLastBond(FALSE)
	, m_bAutoDefineBond(FALSE)
    , m_bIll(FALSE)
{
	m_nReworkRow = m_nReworkCol = m_nReworkWire = 0;
}

CAutoBondPage::~CAutoBondPage()
{
}

void CAutoBondPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_WIRE_NUM, m_nCurWire);
    DDX_Check(pDX, IDC_NSOP, m_bCheckNsop);
    DDX_Check(pDX, IDC_NSOL, m_bCheckNsol);
    DDX_Check(pDX, IDC_BALL_DETECT, m_bCheckBall);
    DDX_Check(pDX, IDC_DRY_RUN, m_bDryRun);
    DDX_Check(pDX, IDC_VISION, m_bBondNoVision);
    DDX_Check(pDX, IDC_AUTO_INDEX, m_bAutoIndex);
    DDX_Check(pDX, IDC_COMPLETE_BOND, m_bCompleteBond);
    DDX_Check(pDX, IDC_LAST_BOND, m_bLastBond);
    DDX_Check(pDX, IDC_DEFINE_BOND_NUM, m_bAutoDefineBond);
    DDX_Check(pDX, IDC_ILL, m_bIll);
}

BEGIN_MESSAGE_MAP(CAutoBondPage, CPropertyPage)
	ON_BN_CLICKED(IDC_SINGLE_BOND, StartSingleBond)
    ON_BN_CLICKED(IDC_AUTO_BOND, StartAutoBond)
    ON_BN_CLICKED(IDC_CORRECT_BOND, OnCorrectBond)
    ON_BN_CLICKED(IDC_BOND_TIP_OFFSET, OnBondTipOffset)
    ON_BN_CLICKED(IDC_AUTO_STOP, StopAutoBond)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_PREV, OnPrev)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
    ON_BN_CLICKED(IDC_NSOP, OnNsop)
    ON_BN_CLICKED(IDC_NSOL, OnNsol)
    ON_BN_CLICKED(IDC_BALL_DETECT, OnBallDetect)
    ON_BN_CLICKED(IDC_DRY_RUN, OnDryRun)
    ON_BN_CLICKED(IDC_VISION, OnVision)
    ON_BN_CLICKED(IDC_AUTO_INDEX, OnAutoIndex)
    ON_EN_CHANGE(IDC_EDIT_WIRE_NUM, OnEditWireNum)
    ON_BN_CLICKED(IDC_DUMMY_BOND, OnDummyBond)
    ON_BN_CLICKED(IDC_COMPLETE_BOND, OnCompleteBond)
    ON_EN_SETFOCUS(IDC_EDIT_WIRE_NUM, OnFocusWireNum)
    ON_WM_ACTIVATE()
    ON_BN_CLICKED(IDC_BUTTON_STOP_ALL, OnStopAll)
    ON_BN_CLICKED(IDC_BUTTON_REDO_PR, OnRedoPr)
	ON_BN_CLICKED(IDC_LAST_BOND, OnLastBond)
    ON_BN_CLICKED(IDC_DEFINE_BOND_NUM, OnSelfDefine)
    ON_BN_CLICKED(IDC_BUTTON_EDIT, OnEditBondPoint)
    ON_BN_CLICKED(IDC_BUTTON_AUTO_PAD, OnAutoPad)
    ON_BN_CLICKED(IDC_ILL, OnIll)
	ON_BN_CLICKED(IDC_BTTN_REWORK, OnRework)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CAutoBondPage message handlers
BOOL CAutoBondPage::OnInitDialog()
{
	theUSG.ResetProfileBuffer();
	theBond.InitUsgProfile();
	TRACE(_T("Sending initial USG profiles!\n"));

	theEFO.SendProfile();
	if (theMcConfig.IsGapLargeTime())	//v3.1.T484 add
		theEFO.SetGapLargeTime(theMcData.GetRcd()->cGapLargeDac);
	else
		theEFO.SetGapLargeDac(theMcData.GetRcd()->cGapLargeDac);

    m_nCurWire        = theBond.m_nCurWire;
    m_bCheckNsop      = theBond.IsCheckNsop();
    m_bCheckNsol      = theBond.IsCheckNsol();
    m_bCheckBall      = theBond.IsCheckFab();
    m_bDryRun         = theBond.IsDryRun();
    m_bBondNoVision   = theBond.IsBondNoVision();
    m_bAutoIndex      = theBond.IsAutoIndex();
    m_bCompleteBond   = theBond.IsCompleteBond();
	m_bAutoDefineBond = theBond.IsAutoDefineBond();
    m_bIll            = theBond.IsIll();

    if (m_nCurWire > thePgm.GetTotalWire())
        m_nCurWire = thePgm.GetTotalWire();

	m_bLastBond = theMHS.IsLastLf();

    m_font.CreateFontW(20, 0, 0, 0, 600, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, _T("Arial")); //@21
	theApp.SetDlgFont(this, &m_font);	//V1.1.W139 add
    
	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    SetDlgItemText(IDC_BUTTON_PREV, theMsg.GetMsg(MSG_PREV));
    SetDlgItemText(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
    SetDlgItemText(IDC_STATIC_MODE, theMsg.GetMsg(MSG_BOND_MODE));
    SetDlgItemText(IDC_SINGLE_BOND, theMsg.GetMsg(MSG_SINGLE_BOND));
    SetDlgItemText(IDC_AUTO_BOND, theMsg.GetMsg(MSG_AUTO_BOND));
    SetDlgItemText(IDC_AUTO_STOP, theMsg.GetMsg(MSG_STOP_BOND));
    SetDlgItemText(IDC_BOND_TIP_OFFSET, theMsg.GetMsg(MSG_BTO));
    SetDlgItemText(IDC_CORRECT_BOND, theMsg.GetMsg(MSG_CORRECT_BOND));
    SetDlgItemText(IDC_BUTTON_REDO_PR, theMsg.GetMsg(MSG_REDO_PR));
    SetDlgItemText(IDC_DUMMY_BOND, theMsg.GetMsg(MSG_DUMMY_BOND));
    SetDlgItemText(IDC_BUTTON_EDIT, theMsg.GetMsg(MSG_EDIT_PT));
    SetDlgItemText(IDC_BUTTON_STOP_ALL, theMsg.GetMsg(MSG_STOP_ALL));
    SetDlgItemText(IDC_STATIC_CONTROL, theMsg.GetMsg(MSG_BOND_CTRL));
    SetDlgItemText(IDC_BALL_DETECT, theMsg.GetMsg(MSG_BALL_DETECT));
    SetDlgItemText(IDC_DRY_RUN, theMsg.GetMsg(MSG_DRY_RUN));
    SetDlgItemText(IDC_VISION, theMsg.GetMsg(MSG_NO_VISION));
    SetDlgItemText(IDC_COMPLETE_BOND, theMsg.GetMsg(MSG_COMPLETE));
    SetDlgItemText(IDC_AUTO_INDEX, theMsg.GetMsg(MSG_AUTO_INDEX));
    SetDlgItemText(IDC_STATIC_OPERATE, theMsg.GetMsg(MSG_OPERATION));
    SetDlgItemText(IDC_CURRENT_WIRE, theMsg.GetMsg(MSG_CURRENT_WIRE));
    SetDlgItemText(IDC_BOND_POINT, theMsg.GetMsg(MSG_BOND_POINT));
    SetDlgItemText(IDC_LAST_BOND, theMsg.GetMsg(MSG_LAST_LF));
    SetDlgItemText(IDC_DEFINE_BOND_NUM, theMsg.GetMsg(MSG_SELF_DEFINE_2));
	SetDlgItemText(IDC_BTTN_REWORK, theMsg.GetMsg(MSG_REWORK));

    //theBond.PrepareBonding();
	theBond.PrepareBondingWithoutPr();
    theBond.MoveToWirePoint(theBond.m_nCurWire, FIRST_BOND);

    theVisionDisplay.SetDisplayWire(theBond.m_nCurWire);
    theVisionDisplay.SetDisplayBondPt(FIRST_BOND);
    theVisionDisplay.UpdateVisionDisplay(true);

    theServo.UpdateWireClampProfile(theDebug.GetDebugMode(), theBond.IsDryRun());
    
    if (!theMcConfig.IsBypassAll())
    {
		const MACHINE_DATA *pstMcData = theMcData.GetRcd();
		theBSD.SetStickPara((BYTE)pstMcData->fDieShortThreshold,
							(BYTE)pstMcData->fLeadShortThreshold,
							pstMcData->nNonStickSample,
							pstMcData->nTailShortSample);
    }

	m_nCount++;	//V1.1.W141 add
    
    return CPropertyPage::OnInitDialog();
}

void CAutoBondPage::StartSingleBond()	//v2.0.T39 rename
{
	CBusy bs(&m_bBusy);	//v3.1.T448 add
	if (((CWireBondDlg*)theApp.m_pMainWnd)->ContactSearch() == FALSE) return;	//V1.1.W141 add
    if (theRef.IsCtactValid() == false) return;	//V1.1.W141 edit

	theScanKeyThread.DisableJoyStick();

	theMHS.CloseWindowClamp();
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add
	WaitN2h2Stablized();

	theLog.WriteLine(_T("SingleBond"), true);	//v3.0.T115 add
	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		CString sMsg;
		sMsg.Format(_T("StartSingleBond_%d"), theBond.m_nCurWire);
		theDebug.AddInfo(sMsg);
	}

	theVisionSys.SetShowResult(true);	//v3.0.T131 add
	DisableButtonForAutoBond(true);
    theBond.PerformAutoBond(BONDMODE_SINGLE);
    DisableButtonForAutoBond(false);

    m_nCurWire = theBond.m_nCurWire;

	if (theMHS.IsLastLf() == false)
		m_bLastBond = FALSE;

    UpdateData(false);

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
    theApp.m_pMainWnd->SetActiveWindow();
}

bool CAutoBondPage::ShowCorrectBondDlg()
{
	CCorrectBondDlg dlg(m_nCurWire);
	dlg.Create(CCorrectBondDlg::IDD, this);
	dlg.ShowWindow(TRUE);
	Sleep_x1ms(10);

	MSG Msg;
	while (dlg.IsWindowVisible())
	{
		while (::PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			if ((Msg.message == WM_KEYDOWN) || (Msg.message == WM_KEYUP))
			{
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					if (theKeyPad.HotKeyHandler(&Msg) == TRUE)
						continue;
				}
			}

			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}

		Sleep_x1ms(10);
	}

	return dlg.m_bSelectCancel == false;
}

short CAutoBondPage::CalcWireNum()
{
	if (m_nReworkRow * m_nReworkCol * m_nReworkWire == 0) return 0;

	short nWireNum = thePgm.GetUnitTotalWire();
	if (thePgm.GetBondSeqOrientation() == SEQUENCE_HORIZONTAL)
		nWireNum = ((m_nReworkRow - 1) * thePgm.GetDeviceColNum() + m_nReworkCol - 1) * nWireNum + m_nReworkWire;
	else
		nWireNum = ((m_nReworkCol - 1) * thePgm.GetDeviceRowNum() + m_nReworkRow - 1) * nWireNum + m_nReworkWire;
	return nWireNum % thePgm.GetTotalWire();
}

void CAutoBondPage::OnCorrectBond()
{
	if (((CWireBondDlg*)theApp.m_pMainWnd)->ContactSearch() == FALSE) return;	//V1.1.W141 add
    if (theRef.IsCtactValid() == false) return;	//V1.1.W141 edit

	theBond.m_AirTensioner.SetRelayState(TRUE);	//v3.1.T439 add
	theScanKeyThread.DisableJoyStick();

	theMHS.CloseWindowClamp();
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add
	WaitN2h2Stablized();

	short nRwWire = CalcWireNum();
	if (nRwWire > 0)
	{
		m_nCurWire = nRwWire;
		theBond.ResetSingleMatrixPr(m_nReworkRow, m_nReworkCol, nRwWire);
		theBond.PrepareBonding();
	}

	WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurWire);
    theBond.MoveToWirePoint(m_nCurWire, FIRST_BOND);
	theVisionDisplay.UpdateVisionDisplay(true);

	theScanKeyThread.EnableJoyStick();

	if (ShowCorrectBondDlg() == false) return;

    if (!theMcConfig.IsBypassAll())
        theServo.GetTablePosition(&pstWire->stCorrectBondPos[0]);
    else
        pstWire->stCorrectBondPos[0] = pstWire->stTchPos[0];

    theBond.MoveToWirePoint(m_nCurWire, SECOND_BOND);
    
	if (ShowCorrectBondDlg() == false) return;

    if (!theMcConfig.IsBypassAll())
        theServo.GetTablePosition(&pstWire->stCorrectBondPos[1]);
    else
        pstWire->stCorrectBondPos[1] = pstWire->stTchPos[1];

	//v3.1.T93 edit
    short nOriBumpBondOption;
    if (pstWire->nBumpType != BUMP_NONE)
    {
        nOriBumpBondOption = theBond.GetBumpBondOption();

        CBondObjectDlg dlg;
        dlg.SetBondObject(BUMPOPT_NORMAL);
        dlg.DoModal();
        short nBumpBondOption = (short)dlg.GetBondObject();
		theBond.SetBumpBondOption(nBumpBondOption);
    }

    DisableButtonForAutoBond(true);
    theBond.PerformAutoBond(BONDMODE_CORRECT);
    DisableButtonForAutoBond(false);

	m_nReworkRow = m_nReworkCol = m_nReworkWire = 0;

    if (pstWire->nBumpType != BUMP_NONE)
        theBond.SetBumpBondOption(nOriBumpBondOption);

    m_nCurWire = theBond.m_nCurWire;
	theBond.m_nEndWire = theWire.GetTotalWireNum();

    UpdateData(false);

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
    theApp.m_pMainWnd->SetActiveWindow();
}

void CAutoBondPage::OnBondTipOffset()
{
    if (theRef.IsCtactValid() == false)
    {
		CStringList sList;
        sList.AddTail(_T(""));
        sList.AddTail(theMsg.GetMsg(MSG_UPDATE_CTACT_LEVEL));
        theMsg.ShowMsg(HELP_MSG, sList);
        return;
    }

	theScanKeyThread.DisableJoyStick();

	theMHS.CloseWindowClamp();
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add
	WaitN2h2Stablized();

    DisableButtonForAutoBond(true);
    theBond.PerformAutoBond(BONDMODE_BTOS);
    DisableButtonForAutoBond(false);

    m_nCurWire = theBond.m_nCurWire;
    UpdateData(false);

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

HBRUSH CAutoBondPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAutoBondPage::OnPrev()
{
	if (m_nCurBondPt == SECOND_BOND)
	{
		m_nCurBondPt--;
	}
	else
	{
		if (m_nCurWire > 1)
		{
			m_nCurWire--;
			m_nCurBondPt = SECOND_BOND;
			theBond.m_nCurWire = m_nCurWire;
			UpdateData(false);

			if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
			{
				if (thePgm.GetTotalUnit() == 0)
				{
					AfxMessageBox(_T("temp.7, TotalUnit = 0"));
					return;
				}

				short nUnitTotalWire = thePgm.GetTotalWire() / thePgm.GetTotalUnit();
				if (nUnitTotalWire == 0)
				{
					AfxMessageBox(_T("temp.8, UnitTotalWire = 0"));
					return;
				}

				short nUnitNum = (m_nCurWire - 1) / nUnitTotalWire + 1;
				thePgm.GetRowCol(nUnitNum, &(theBond.m_nCurRow), &(theBond.m_nCurCol));
			}
		}
	}

	int iPrStatus = FOK;
	if (theBond.CheckAheadAlignDone(&m_nCurWire, false) != 1)
		iPrStatus = theBond.PrepareBonding();
	if (iPrStatus == FOK)
		theBond.MoveToWirePoint(m_nCurWire, m_nCurBondPt);

	theVisionDisplay.SetDisplayWire(m_nCurWire);
	theVisionDisplay.SetDisplayBondPt(m_nCurBondPt);
	theVisionDisplay.UpdateVisionDisplay(true);
	Sleep_x1ms(50);

	((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CAutoBondPage::OnNext()
{
	if (m_nCurBondPt == FIRST_BOND)
	{
		m_nCurBondPt++;
	}
	else
	{
		if (m_nCurWire < thePgm.GetTotalWire())
		{
			m_nCurWire++;
			m_nCurBondPt = 0;
			theBond.m_nCurWire = m_nCurWire;
			UpdateData(false);

			if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
			{
				if (thePgm.GetTotalUnit() == 0)
				{
					AfxMessageBox(_T("temp.9, TotalUnit = 0"));
					return;
				}

				short nUnitTotalWire = thePgm.GetTotalWire() / thePgm.GetTotalUnit();
				if (nUnitTotalWire == 0)
				{
					AfxMessageBox(_T("temp.10, UnitTotalWire = 0"));
					return;
				}

				short nUnitNum = (m_nCurWire - 1) / nUnitTotalWire + 1;
				thePgm.GetRowCol(nUnitNum, &(theBond.m_nCurRow), &(theBond.m_nCurCol));
			}
		}
	}

	int iPrStatus = FOK;
	if (theBond.CheckAheadAlignDone(&m_nCurWire) != 1)
		iPrStatus = theBond.PrepareBonding();
	if (iPrStatus == FOK)
		theBond.MoveToWirePoint(m_nCurWire, m_nCurBondPt);

	theVisionDisplay.SetDisplayWire(m_nCurWire);
	theVisionDisplay.SetDisplayBondPt(m_nCurBondPt);
	theVisionDisplay.UpdateVisionDisplay(true);
	Sleep_x1ms(50);

	((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CAutoBondPage::OnNsop()
{
    UpdateData(true);
	theBond.SetCheckNsop(m_bCheckNsop == TRUE ? true : false);
}

void CAutoBondPage::OnNsol()
{
    UpdateData(true);
	theBond.SetCheckNsol(m_bCheckNsol == TRUE ? true : false);
}

void CAutoBondPage::OnBallDetect()
{
    UpdateData(true);
	theBond.SetCheckFab(m_bCheckBall == TRUE ? true : false);
}

void CAutoBondPage::OnDryRun()
{
    UpdateData(true);
	theBond.SetDryRun(m_bDryRun == TRUE ? true : false);
    theServo.UpdateWireClampProfile(theDebug.GetDebugMode(), theBond.IsDryRun());
}

void CAutoBondPage::OnSelfDefine()
{
	UpdateData(true);

	if (m_bAutoDefineBond == TRUE)
	{
		CBondFromToUnitDlg *pDlg = new CBondFromToUnitDlg();
		pDlg->Create(CBondFromToUnitDlg::IDD, this);
		theBond.SetAutoDefineBond(true);
	}
	else
	{
		theBond.m_nEndWire = theWire.GetTotalWireNum();
		theBond.SetAutoDefineBond(false);
	}
}

void CAutoBondPage::OnVision()
{
    UpdateData(true);
	theBond.SetBondNoVision(m_bBondNoVision == TRUE ? true : false);
}

void CAutoBondPage::OnAutoIndex()
{
	UpdateData(true);
	theBond.SetAutoIndex(m_bAutoIndex == TRUE ? true : false);
}

void CAutoBondPage::OnEditWireNum()
{
	short nOrgWireNum = m_nCurWire;
	UpdateData(true);

	if ((m_nCurWire > 0) && (m_nCurWire <= thePgm.GetTotalWire()))
	{
		m_nCurBondPt = 0;
		theBond.m_nCurWire = m_nCurWire;
		theBond.MoveToWirePoint(m_nCurWire, m_nCurBondPt);

		if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
		{
			if (thePgm.GetTotalUnit() == 0)
			{
				AfxMessageBox(_T("temp.11, TotalUnit = 0"));
				return;
			}

			short nUnitTotalWire = thePgm.GetTotalWire() / thePgm.GetTotalUnit();
			if (nUnitTotalWire == 0)
			{
				AfxMessageBox(_T("temp.12, UnitTotalWire = 0"));
				return;
			}

			short nUnitNum = (m_nCurWire - 1) / nUnitTotalWire + 1;
			thePgm.GetRowCol(nUnitNum, &(theBond.m_nCurRow), &(theBond.m_nCurCol));
		}
	}
	else
	{
		m_nCurWire = nOrgWireNum;
		UpdateData(false);
	}

	theVisionDisplay.SetDisplayWire(m_nCurWire);
	theVisionDisplay.SetDisplayBondPt(m_nCurBondPt);
	theVisionDisplay.UpdateVisionDisplay(true);
	Sleep_x1ms(50);

	((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CAutoBondPage::OnDummyBond()
{
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add
	if (((CWireBondDlg*)theApp.m_pMainWnd)->ContactSearch() == FALSE) return;	//V1.1.W141 add
	if (theRef.IsCtactValid() == false) return;		//V1.1.W141 add

	theBond.m_AirTensioner.SetRelayState(TRUE);	//v3.1.T439 add

	DUMMY_BOND_AREA_RCD stDummyArea;
	theBondPara.GetDummyBondAreaRcd(&stDummyArea);
	if (stDummyArea.bBondAreaValid)
	{
		theVisionSys.UpdateLightingParameter(
					stDummyArea.stLightPara.nCoaxRedLight,
					stDummyArea.stLightPara.bSideRedLight,
					stDummyArea.stLightPara.nCoaxBlueLight,
					stDummyArea.stLightPara.bSideBlueLight);
		TBL_POS stPos;
		stPos.dx = (stDummyArea.stLeftTopPos.dx + stDummyArea.stRightBottomPos.dx) / 2.0;
		stPos.dy = (stDummyArea.stLeftTopPos.dy + stDummyArea.stRightBottomPos.dy) / 2.0;
		stPos.dx -= theMcData.GetRcd()->stBondTipOs.dx;
		stPos.dy -= theMcData.GetRcd()->stBondTipOs.dy;
		theServo.MoveTable2(stPos);	//v3.1.T44 add
	}

	//v3.0.T324 add
	if (theMcConfig.GetMhsType() == MHS_FOXCONN)
		theServo.MoveTable2(stDummyArea.stLeftTopPos);	//v3.1.T44 add

	CStringList sList;
	sList.AddTail(_T(""));
	sList.AddTail(theMsg.GetMsg(MSG_MOVE_TO_DUMMY_BOND_POS)); 
	if (theMsg.ShowMsgS(HELP_MSG,sList, true) == SHOW_MSG_RESULT_STOP) return;

	ST_BUMP_BALL stBumpBall;
	switch (theWire.GetRcd(m_nCurWire)->nBumpType)	//v3.1.T88 add
	{
	case BUMP_BOS:
		theBallPara.GetBbosBall(&stBumpBall);
		break;
	case BUMP_SOB:
		theBallPara.GetBsobBall(&stBumpBall);
		break;
	}

	CBondObjectDlg dlg(1);
	dlg.SetBondObject(BUMPOPT_WIRE_ONLY);
	dlg.DoModal();
	short nBumpOption = (short)dlg.GetBondObject();
	char cFabType = (nBumpOption == BUMPOPT_WIRE_ONLY ? 0 : stBumpBall.chFabType);

	DisableButtonForAutoBond(true);
	TBL_POS stPos;
	theServo.GetTablePosition(&stPos);
	if (theMcConfig.GetMhsType() != MHS_FOXCONN)	//v3.0.T324 add
	{
		stPos.dx += theMcData.GetRcd()->stBondTipOs.dx;
		stPos.dy += theMcData.GetRcd()->stBondTipOs.dy;
	}
	theDummyBond.PerformDummyBond(cFabType, stPos);	//v3.1.T207 edit
	DisableButtonForAutoBond(false);
}

void CAutoBondPage::OnCompleteBond()
{
    UpdateData(true);
	theBond.SetCompleteBond(m_bCompleteBond == TRUE ? true : false);
}

void CAutoBondPage::OnLastBond()
{
	UpdateData(true);
	theMHS.SetLastLf(m_bLastBond == TRUE);
}

// Disable buttons durong auto bond
void CAutoBondPage::DisableButtonForAutoBond(bool bIsDisable)
{
	bIsDisable = !bIsDisable;

    CAutoBondSheet *pSheet = (CAutoBondSheet*)GetParent();
	pSheet->EnablePage(2, bIsDisable);	//V1.1.W169 add
	pSheet->GetDlgItem(IDOK)->EnableWindow(bIsDisable);

	GetDlgItem(IDC_SINGLE_BOND)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_AUTO_BOND)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BOND_TIP_OFFSET)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_CORRECT_BOND)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_DUMMY_BOND)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BUTTON_REDO_PR)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BUTTON_AUTO_PAD)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_EDIT_WIRE_NUM)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_AUTO_INDEX)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_VISION)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_DRY_RUN)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_NSOP)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_NSOL)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BALL_DETECT)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_DEFINE_BOND_NUM)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(bIsDisable);
	GetDlgItem(IDC_BTTN_REWORK)->EnableWindow(bIsDisable);

	CWireBondDlg *pDlg = (CWireBondDlg*)theApp.m_pMainWnd;
	pDlg->EnableButton(ID_TOOL_BAR_LIGHT, bIsDisable);
	pDlg->EnableButton(ID_TOOL_BAR_INDEX, bIsDisable);
	pDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, bIsDisable);
	pDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, bIsDisable);
	pDlg->EnableButton(ID_TOOL_BAR_ELEV, bIsDisable);
	pDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, bIsDisable);	//V1.1.W93 edit

	pSheet->SetFocus();
    if (bIsDisable == false) return;
	pDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.GetRcd()->bCapillaryAutoPlug);	//V1.1.W93 edit
}

void CAutoBondPage::OnFocusWireNum()
{
    GetDlgItem(IDC_AUTO_BOND)->SetFocus();    
    theKeyPad.GetUserKeyInData(&m_nCurWire);
    UpdateData(false);
    OnEditWireNum();
}

void CAutoBondPage::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    m_nCurWire			= theBond.m_nCurWire;
    m_bCheckNsop		= theBond.IsCheckNsop();
    m_bCheckNsol		= theBond.IsCheckNsol();
    m_bCheckBall		= theBond.IsCheckFab();
    m_bDryRun			= theBond.IsDryRun();
    m_bBondNoVision		= theBond.IsBondNoVision();
    m_bAutoIndex		= theBond.IsAutoIndex();
    m_bCompleteBond		= theBond.IsCompleteBond();
    m_bIll				= theBond.IsIll();
	m_bAutoDefineBond	= theBond.IsAutoDefineBond();
    UpdateData(false);

	((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
    CPropertyPage::OnActivate(nState, pWndOther, bMinimized);
}

void CAutoBondPage::OnStopAll()
{
    theBond.InformAutoBondStop();
    Sleep_x1ms(10);

    theMHS.SetStopAll(true);
    Sleep_x1ms(50);

    theMHS.SetRequestIndex(false);
    theMHS.SetBusy(false);
    Sleep_x1ms(10);

	if ((theMcConfig.GetMhsType() == MHS_VLED)
		|| (theMcConfig.GetMhsType() == MHS_VLED_ELITE))
    {
        theVledOnload.ResetOnloadEvent();
        theVledTrack.ResetIndexEvent();
        theVledOffload.ResetForkEvent();
        theBond.m_N2h2Port.SetRelayState(RELAY_OFF, true);
    }
    theMHS.SetStopAll(false);
}

void CAutoBondPage::OnRedoPr()
{
	theTowerLight.EnableBondingLight();
	theBond.m_PanelLight.SetRelayState(FALSE);
	theBond.m_AirTensioner.SetRelayState(TRUE);	//v3.1.T439 add

	theMHS.CloseWindowClamp();
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add

	theVisionSys.SetShowResult(true);	//v3.0.T131 add
    theBond.RedoPrAlignment(m_nReworkRow, m_nReworkCol, CalcWireNum());

	theBond.m_PanelLight.SetRelayState(TRUE);
    theVisionDisplay.UpdateVisionDisplay(true);
    //Sleep(50);
	theTowerLight.EnableIdleLight();
    //theBond.MoveToWirePoint(theBond.m_nCurWire, FIRST_BOND);
    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);

	if (theMcConfig.IsBypassAll())
		::AfxMessageBox(_T("Re-do PR done"));
}

int CAutoBondPage::WaitN2h2Stablized(void)
{
	UpdateData(TRUE);

	if (thePgm.IsN2H2Enable() == false) return 1;
	if (m_bDryRun) return 1;
	if (theBond.m_N2h2Port.GetRelayState() == RELAY_ON) return 0;

	theBond.m_N2h2Port.SetRelayState(RELAY_ON, true);

	CN2h2WaitStableDlg theN2h2DelayDlg;
	theN2h2DelayDlg.DoModal();
	return 0;
}

void CAutoBondPage::DuplicatePoints(EN_BOND_PT nBondPt, WIRE_RECORD *pstCurWire, TBL_POS stEditOffset)
{
	switch (thePgm.GetDeviceSeq())	//v1.6.T326 add
	{
	case PGM_HYBRID:
	case PGM_DIE_LEAD_AHEAD:
	case PGM_MAT_HYBRID:
	{
		short nRow, nCol, nUnitTotalWire = (short)thePgm.GetUnitTotalWire();
		short nUnitWireNum = nUnitTotalWire > 1 ? (pstCurWire->nWireNum - 1) % nUnitTotalWire + 1 : 1;

		//Get 1st unit pos
		TBL_POS stPos, stUnitOffset;
		nRow = pstCurWire->nWireNum / nUnitTotalWire;
		if (pstCurWire->nWireNum % nUnitTotalWire) nRow++;
		thePgm.GetRowCol(nRow, &nRow, &nCol);
		thePgm.CalcUnitOffset(nRow, nCol, &stUnitOffset);
		stPos.dx = pstCurWire->stTchPos[nBondPt].dx - stUnitOffset.dx;
		stPos.dy = pstCurWire->stTchPos[nBondPt].dy - stUnitOffset.dy;

		WIRE_RECORD *pstWire;
		short nWireNum;
		for (short i = 1; i < thePgm.GetTotalUnit() + 1; i++)
		{
			thePgm.GetRowCol(i, &nRow, &nCol);
			thePgm.CalcUnitOffset(nRow, nCol, &stUnitOffset);

			nWireNum = nUnitWireNum + (i - 1) * nUnitTotalWire;
			pstWire = theWire.GetRcd(nWireNum);
			pstWire->stTchPos[nBondPt].dx = stPos.dx + stUnitOffset.dx;
			pstWire->stTchPos[nBondPt].dy = stPos.dy + stUnitOffset.dy;
		}
		break;
	}
	case PGM_MAT_DIE_AHEAD:	//v1.6.T326 add
	{
		short nUnitTotalWire = (short)thePgm.GetUnitTotalWire();
		short nUnitWireNum = nUnitTotalWire > 1 ? (pstCurWire->nWireNum - 1) % nUnitTotalWire + 1 : 1;

		//Get 1st unit pos
		short nUnitNum = ((pstCurWire->nWireNum - 1) / nUnitTotalWire) + 1;
		short nRow, nCurCol;
		thePgm.GetRowCol(nUnitNum, &nRow, &nCurCol);
		TBL_POS stPos, stUnitOffset;
		thePgm.CalcUnitOffset(nRow, nCurCol, &stUnitOffset);
		stPos.dx = pstCurWire->stTchPos[nBondPt].dx - stUnitOffset.dx;
		stPos.dy = pstCurWire->stTchPos[nBondPt].dy - stUnitOffset.dy;

		WIRE_RECORD *pstWire;
		short nWireNum, nCol;
		for (short i = 1; i <= thePgm.GetTotalUnit(); i++)
		{
			thePgm.GetRowCol(i, &nRow, &nCol);
			thePgm.CalcUnitOffset(nRow, nCol, &stUnitOffset);

			//v1.6.T332 add, v1.6.T354 edit
			if ((theMcConfig.GetDeviceSeq() == 1) && ((nCol % 2) != (nCurCol % 2)))
				nWireNum = (i * nUnitTotalWire) - nUnitWireNum + 1;
			//v3.0.T15 add
			else if ((theMcConfig.GetDeviceSeq() == 2) && ((nCol % 2) != (nCurCol % 2)))
			{
				if (nUnitWireNum == 1) nUnitWireNum = 2;
				else if (nUnitWireNum == 2) nUnitWireNum = 1;
				else if (nUnitWireNum == 4) nUnitWireNum = 5;
				else if (nUnitWireNum == 5) nUnitWireNum = 4;
				nWireNum = (i * nUnitTotalWire) - nUnitWireNum + 1;
			}
			else
				nWireNum = ((i - 1) * nUnitTotalWire) + nUnitWireNum;
			pstWire = theWire.GetRcd(nWireNum);
			pstWire->stTchPos[nBondPt].dx = stPos.dx + stUnitOffset.dx;
			pstWire->stTchPos[nBondPt].dy = stPos.dy + stUnitOffset.dy;
		}
		break;
	}
	case PGM_DIE_AHEAD:	//v1.5.T98 add
	{
		short nUnitTotalWire = (short)thePgm.GetUnitTotalWire();
		short nUnitWireNum = nUnitTotalWire > 1 ? (pstCurWire->nWireNum - 1) % nUnitTotalWire + 1 : 1;

		WIRE_RECORD *pstWire;
		short nWireNum;
		for (short i = 1; i <= thePgm.GetTotalUnit(); i++)
		{
			nWireNum = nUnitWireNum + (i - 1) * nUnitTotalWire;
			if (nWireNum != pstCurWire->nWireNum)
			{
				pstWire = theWire.GetRcd(nWireNum);
				pstWire->stTchPos[nBondPt].dx += stEditOffset.dx;
				pstWire->stTchPos[nBondPt].dy += stEditOffset.dy;
			}
		}
	}
	default:
		break;
	}
}

void CAutoBondPage::OnEditBondPoint()
{
	theMHS.CloseWindowClamp();
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add

    if (theBond.CheckAheadAlignDone(&m_nCurWire) != 1)
    {
        theBond.ResetMatrixPrStatus(m_nCurWire);
        theBond.PrepareBonding();
    }

	WIRE_RECORD stWireRcd;
    theWire.GetRcd(&stWireRcd, m_nCurWire);

	TBL_POS stOldPos[2];
	for (short i = 0; i < 2; i++)
		stOldPos[i] = stWireRcd.stTchPos[i];

    theBond.MoveToWirePoint(stWireRcd.nWireNum, FIRST_BOND);

    theVisionDisplay.SetDisplayMode(VISION_DISPLAY_TEACH_MODE);
    theVisionDisplay.SetDisplayWire(m_nCurWire);
    theVisionDisplay.SetDisplayBondPt(FIRST_BOND);

	if (ShowPosAdjDlg() == false)
	{
        theVisionDisplay.SetDisplayMode(VISION_DISPLAY_NORMAL_MODE);
	    return;
    }

    if (!theMcConfig.IsBypassAll())
        theServo.GetTablePosition(&stWireRcd.stTchPos[0]);

    theMatrix.ReverseTransformPos(stWireRcd.nRefNum[0], &stWireRcd.stTchPos[0]);
    stWireRcd.nWireLength = round(Calc2PtDist(stWireRcd.stTchPos[0], stWireRcd.stTchPos[1]) * theServo.GetResolXY());
	theWire.SetRcd(&stWireRcd, stWireRcd.nWireNum);

	//v1.5.T98 add
	TBL_POS stEditOffset;
	stEditOffset.dx = stWireRcd.stTchPos[0].dx - stOldPos[0].dx;
	stEditOffset.dy = stWireRcd.stTchPos[0].dy - stOldPos[0].dy;
	DuplicatePoints(FIRST_BOND, &stWireRcd, stEditOffset);

    theBond.MoveToWirePoint(stWireRcd.nWireNum, SECOND_BOND);

    theVisionDisplay.SetDisplayBondPt(SECOND_BOND);

	if (ShowPosAdjDlg() == false)
    {
        theVisionDisplay.SetDisplayMode(VISION_DISPLAY_NORMAL_MODE);
	    return;
    }

    if (!theMcConfig.IsBypassAll())
        theServo.GetTablePosition(&stWireRcd.stTchPos[1]);

    theMatrix.ReverseTransformPos(stWireRcd.nRefNum[1], &stWireRcd.stTchPos[1]);

    if (stWireRcd.bConstWireLength == true)
    {
        TBL_POS stBondPos1 = stWireRcd.stTchPos[0];
        TBL_POS stBondPos2 = stWireRcd.stTchPos[1];

        theMatrix.TransformPos(stWireRcd.nRefNum[0], &stBondPos1);
        theMatrix.TransformPos(stWireRcd.nRefNum[1], &stBondPos2);

        double dOrgLength = stWireRcd.nWireLength / theServo.GetResolXY();
		stWireRcd.stTchPos[1] = CalcPosAlongWire(stBondPos1, stBondPos2, dOrgLength);	//v3.1.T372 add
    }

    stWireRcd.nWireLength = round(Calc2PtDist(stWireRcd.stTchPos[0], stWireRcd.stTchPos[1]) * theServo.GetResolXY());
    theWire.SetRcd(&stWireRcd, stWireRcd.nWireNum);
    //theWire.UpdateEditedWirePoint(lpstWireRcd->nWireNum);

	//v1.5.T98 add
	stEditOffset.dx = stWireRcd.stTchPos[1].dx - stOldPos[1].dx;
	stEditOffset.dy = stWireRcd.stTchPos[1].dy - stOldPos[1].dy;
	DuplicatePoints(SECOND_BOND, &stWireRcd, stEditOffset);

    theVisionDisplay.SetDisplayMode(VISION_DISPLAY_NORMAL_MODE);
    theVisionDisplay.UpdateVisionDisplay(true);
    Sleep_x1ms(50);

    theWire.GetRcd(&stWireRcd, m_nCurWire);
	TBL_POS stNewPos[2];
	for (short i = 0; i < 2; i++)
		stNewPos[i] = stWireRcd.stTchPos[i];
	if ((stOldPos[0].dx != stNewPos[0].dx)
		|| (stOldPos[0].dy != stNewPos[0].dy)
		|| (stOldPos[1].dx != stNewPos[1].dx)
		|| (stOldPos[1].dy != stNewPos[1].dy))
	{
		theLog.WriteLong(_T("EditWire_%d"), m_nCurWire, true);	//v3.0.T48 edit
		if (stOldPos[0].dx != stNewPos[0].dx)
			theLog.WriteDouble3(_T("pt 1x: %.1f -> %.1f (%.2f)"),
				stOldPos[0].dx, stNewPos[0].dx,
				stNewPos[0].dx - stOldPos[0].dx, true);
		if (stOldPos[0].dy != stNewPos[0].dy)
			theLog.WriteDouble3(_T("pt 1y: %.1f -> %.1f (%.2f)"),
				stOldPos[0].dy, stNewPos[0].dy,
				stNewPos[0].dy - stOldPos[0].dy, true);
		if (stOldPos[1].dx != stNewPos[1].dx)
			theLog.WriteDouble3(_T("pt 2x: %.1f -> %.1f (%.2f)"),
				stOldPos[1].dx, stNewPos[1].dx,
				stNewPos[1].dx - stOldPos[1].dx, true);
		if (stOldPos[1].dy != stNewPos[1].dy)
			theLog.WriteDouble3(_T("pt 2y: %.1f -> %.1f (%.2f)"),
				stOldPos[1].dy, stNewPos[1].dy,
				stNewPos[1].dy - stOldPos[1].dy, true);
	}

	((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CAutoBondPage::OnAutoPad()
{
	theMHS.CloseWindowClamp();

    if (theBond.CheckAheadAlignDone(&m_nCurWire) != 1)
    {
        theBond.ResetMatrixPrStatus(m_nCurWire);
        if (theBond.PrepareBonding() == FNOK) return;
    }

    theVisionDisplay.EnableDisplay(false);
    DisableButtonForAutoBond(true);
    theAutoPad.PerformAutoPadWithPR();
    DisableButtonForAutoBond(false);
    theVisionDisplay.EnableDisplay(true);

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CAutoBondPage::OnIll()
{
    UpdateData(true);
	theBond.SetIll(m_bIll == TRUE ? true : false);
}

void CAutoBondPage::StartAutoBond(void)
{
	CBusy bs(&m_bBusy);	//v3.1.T448 add
	UpdateData(true);

	if (((CWireBondDlg*)theApp.m_pMainWnd)->ContactSearch() == FALSE) return;	//V1.1.W141 add
    if (theRef.IsCtactValid() == false) return;	//V1.1.W141 edit

	if (!theMcConfig.IsBypassMHS())	//v3.1.T265 add
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
		{
		case MHS_TOP_LED:
		{
			if ((theTopOnloader.m_MagDetectedSensor.GetCurrentState() == FALSE)
				&& (theTopTrack.GetCurBondUnit() == 0)
				&& !theMcConfig.IsBypassOnlElevNoMag())		//v3.1.T304 add
			{
				short anMsgArray[1] = {MSG_ONLOAD_ELEV_NO_MAG};
				theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
				return;
			}
			else
			{
				if (theTopOnloader.GetErrorCode() == MHS_TOP_ONLOAD_NO_MAG)
					theTopOnloader.ResetErrorCode();
			}
			break;
		}
		case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
		{
			if ((theTopOnloader.m_MagDetectedSensor.GetCurrentState() == FALSE)
				&& (theTEIndex.GetCurBondUnit() == 0)
				&& !theMcConfig.IsBypassOnlElevNoMag())		//v3.1.T304 add
			{
				short anMsgArray[1] = {MSG_ONLOAD_ELEV_NO_MAG};
				theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
				return;
			}
			else
			{
				if (theTopOnloader.GetErrorCode() == MHS_TOP_ONLOAD_NO_MAG)
					theTopOnloader.ResetErrorCode();
			}
			break;
		}
		case MHS_CU_ELITE:
		{
			if ((theCuEliteOnloader.theMagDetectedSensor.GetCurrentState() == FALSE)
				&& (theCuEliteTrack.GetCurrentBondUnit() == 0)
				&& !theMcConfig.IsBypassOnlElevNoMag())		//v3.1.T304 add
			{
				short anMsgArray[1] = {MSG_ONLOAD_ELEV_NO_MAG};
				theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
				return;
			}
			else
			{
				if (theCuEliteOnloader.GetOnloadErrorCode() == MHS_CUELITE_ONLOAD_NO_MAG)
					theCuEliteOnloader.ResetOnloadErrorCode();
			}
			break;
		}
		case MHS_R2R:
		{
			short i = 0;
			CStringList sList;

			if (theR2ROnloader.theErrorSnr.GetCurrentState() != FOK )
			{
				sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_DOWN));
				i++;
			}

			if (theR2ROffloader.theErrorSnr.GetCurrentState() != FOK)
			{
				sList.AddTail(theMsg.GetMsg(MSG_OFFLOAD_DOWN));
				i++;
			}

			if (i > 0)
			{
				theMsg.ShowMhsMsg(WARNING_MSG, sList, MSG_CALLBACK_NONE, 0);
				return;
			}
			break;
		}
		}
	}

	theScanKeyThread.DisableJoyStick();

	theMHS.CloseWindowClamp();
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add
	WaitN2h2Stablized();

	//v3.0.T115 add
	CString sMsg;
	long t = theBond.GetIdleTime_x1s();
	sMsg.Format(_T("Start_%d_%d_%d"), t/3600, (t/60)%60, t%60);
	theLog.WriteLine(sMsg, true);

	if (theMcConfig.IsDebugModeStartStop())	//v3.1.T465 add
	{
		sMsg.Format(_T("StartAutoBond_%d"), theBond.m_nRestartWire);
		theDebug.AddInfo(sMsg);
	}

	theVisionSys.SetShowResult(true);	//v3.0.T122 add
    DisableButtonForAutoBond(true);
    theBond.PerformAutoBond(BONDMODE_AUTO);
	DisableButtonForAutoBond(false);
	theVisionSys.SetShowResult(true);	//v3.0.T122 add

	t = theBond.GetBondTime_x1s();
	sMsg.Format(_T("Stop_%d_%d_%d"), t/3600, (t/60)%60, t%60);
	theLog.WriteLine(sMsg, true);

    m_nCurWire = theBond.m_nCurWire;
	theBond.StopLfCycleTimer();

	if ((theBond.m_nCurWire > theBond.m_nEndWire)
		|| (theBond.IsAutoDefineBond() == false))
	{
		m_bAutoDefineBond = FALSE;
		theBond.SetAutoDefineBond(false);
		theBond.m_nEndWire = theWire.GetTotalWireNum();
	}

	if (theMHS.IsLastLf() == false)
		m_bLastBond = FALSE;

    UpdateData(false);

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CAutoBondPage::StopAutoBond(void)
{
    theBond.InformAutoBondStop();
}

void CAutoBondPage::OnRework()
{
	if (theTopTrack.IsClampReady() == FALSE) return;	//V1.5.W17 add

	CDlgRework dlg(this, &m_font);

	dlg.m_nReworkRow = m_nReworkRow;
	dlg.m_nReworkCol = m_nReworkCol;
	dlg.m_nReworkWire = m_nReworkWire;

	if (dlg.DoModal() == IDCANCEL) return;

	m_nReworkRow = dlg.m_nReworkRow;
	m_nReworkCol = dlg.m_nReworkCol;
	m_nReworkWire = dlg.m_nReworkWire;
}

void CAutoBondPage::Clean()	//V1.1.W141 add
{
	m_nCount = 0;
}

BYTE CAutoBondPage::IsDirty()	//V1.1.W141 add
{
	return m_nCount > 1;
}

void CAutoBondPage::OnDestroy()	//V1.1.W151 add
{
    StopAutoBond();
	theTopTrack.SetRunNoLf(false);

    theApp.ArchivePowerOnData(ARCHIVE_SAVE);
	Sleep_x1ms(100);
    theApp.ArchiveMachineData(ARCHIVE_SAVE);

	CPropertyPage::OnDestroy();
}

bool CAutoBondPage::ShowPosAdjDlg()
{
	CPositionAdjustDlg dlg;
	dlg.Create(CPositionAdjustDlg::IDD, this);
	dlg.SetAdjustMax(theMcConfig.GetEditBondPtMax());
	dlg.ShowWindow(TRUE);
	Sleep_x1ms(10);

	MSG Msg;
	while (dlg.IsWindowVisible())
	{
		while (::PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			if ((Msg.message == WM_KEYDOWN) || (Msg.message == WM_KEYUP))
			{
				if ((theMcConfig.GetKeyPadType() >= 1)
					&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
				{
					if (theKeyPad.HotKeyHandler(&Msg) == TRUE)
						continue;
				}
			}

			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}

		Sleep_x1ms(10);
	}

	return dlg.m_bSelectCancel == false;
}
