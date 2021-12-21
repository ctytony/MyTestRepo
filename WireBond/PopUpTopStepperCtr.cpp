/***************************************************************
PopUpTopStepperCtr.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "PopUpTopStepperCtr.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "StepperControl.h"
#include "MaterialHandlerThread.h"
#include "MathCalc.h"
#include "ScanKeyThread.h"
#include "MachineConfig.h"
#include "Program.h"
#include "R2RIndex.h"
#include "Jpeg.h"
#include "WireBondDlg.h"
#include "TopEliteIndex.h"
#include <Gdiplus.h>
using namespace Gdiplus;

extern CJpeg theJpegHelp;
Bitmap* g_lpbmArrow=NULL;	//V1.1.W147 add

// CPopUpTopStepperCtr dialog
IMPLEMENT_DYNAMIC(CPopUpTopStepperCtr, CDialog)

CPopUpTopStepperCtr::CPopUpTopStepperCtr(byte nCtrlModuleId, short nSpeed,  CWnd* pParent /*=NULL*/)
	: CDialog(CPopUpTopStepperCtr::IDD, pParent)
	, m_nCtrModule(TOP_DEVICE_INDEXER)
	, m_nStepResol(2000)  // 1 to 10000
	, m_nSpeed(0)
{
	m_nCtrModule = nCtrlModuleId;

	m_nSpeed = nSpeed;

	if (m_nSpeed == SPEED_NORMAL)
		m_nStepResol = 2000;
	else if(m_nSpeed == SPEED_SMALL)
		m_nStepResol = 500;
	else if(m_nSpeed == SPEED_FINE) 
		m_nStepResol = 20;

	m_nDlgPos=DLG_POS_CENTRE;
	m_n2ndCtrlID=0;

	g_lpbmArrow=NULL;
}

CPopUpTopStepperCtr::~CPopUpTopStepperCtr()
{ 
    KillTimer(1);

	if(g_lpbmArrow)delete g_lpbmArrow;
}

void CPopUpTopStepperCtr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPopUpTopStepperCtr, CDialog)
	ON_BN_CLICKED(IDC_UP_CTR, OnUpCtr)
	ON_BN_CLICKED(IDC_DOWN_CTR, OnDownCtr)
	ON_BN_CLICKED(IDC_RIGHT_CTR, OnRightCtr)
	ON_BN_CLICKED(IDC_LEFT_CTRL, OnLeftCtrl)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_SCALE_INCREASE, OnScaleIncrease)
	ON_BN_CLICKED(IDC_SCALE_DECREASE, OnScaleDecrease)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnRecover)
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// CPopUpTopStepperCtr message handlers
BOOL CPopUpTopStepperCtr::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWireBondDlg* lpMain=(CWireBondDlg*)theApp.m_pMainWnd;
	g_lpbmArrow=lpMain->Load(theApp.m_hInstance,IDR_PNG_ARROW,_T("PNG"));

	CWnd* pWnd = GetDlgItem(IDC_BTN_RECOVER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RECOVER));

	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_TOP_LED: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
        pWnd->ShowWindow(SW_SHOWNORMAL);
		break;
	default:
        pWnd->ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_LEFT_CTRL)->EnableWindow(FALSE);
    GetDlgItem(IDC_RIGHT_CTR)->EnableWindow(FALSE);
    GetDlgItem(IDC_UP_CTR)->EnableWindow(m_n2ndCtrlID==TOP_WINDOW_CLAMPER);
    GetDlgItem(IDC_DOWN_CTR)->EnableWindow(m_n2ndCtrlID==TOP_WINDOW_CLAMPER);

	if (((theMcConfig.GetMhsType() == MHS_TOP_LED)
			&& (m_nCtrModule == theTopTrack.GetIndexStepID()))
		|| ((theMcConfig.GetMhsType() == MHS_TOP_ELITE)		//v3.0.T410 add
			&& (m_nCtrModule == theTEIndex.GetIndexStepID()))	//v3.0.T419 edit
		|| ((theMcConfig.GetMhsType() == MHS_R2R)
			&& (m_nCtrModule == theR2RTrack.m_ucIndexerStepperId))
		|| (((theMcConfig.GetMhsType() == MHS_VLED)
				|| (theMcConfig.GetMhsType() == MHS_VLED_ELITE))
			&& ((m_nCtrModule == SINGLE_CAP_INJECTOR)
				|| (m_nCtrModule == SINGLE_CAP_OFFLOAD_KICKER))))
	{
		GetDlgItem(IDC_LEFT_CTRL)->EnableWindow();
		GetDlgItem(IDC_RIGHT_CTR)->EnableWindow();

		theStepper.SetJogStep(round(m_nStepResol / theStepper.GetResol(m_nCtrModule)));

		theScanKeyThread.SetJoyStickConfig(JOYSTICK_STEPPER_HORIZONTAL, m_nCtrModule);
	}
	else
	{
		GetDlgItem(IDC_UP_CTR)->EnableWindow();
		GetDlgItem(IDC_DOWN_CTR)->EnableWindow();
		GetDlgItem(IDC_BTN_RECOVER)->EnableWindow(FALSE);

		theStepper.SetJogStep(round(m_nStepResol / theStepper.GetResol(m_nCtrModule)));

		theScanKeyThread.SetJoyStickConfig(JOYSTICK_STEPPER_VERTICAL, m_nCtrModule);
	}

	if(m_nDlgPos==DLG_POS_TOP_LEFT)SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	else if(m_nDlgPos==DLG_POS_TOP_RIGHT)SetWindowPos(NULL, 800, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	else if(m_nDlgPos==DLG_POS_BOTTOM_RIGHT)SetWindowPos(NULL, 800, 366, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	else if(m_nDlgPos==DLG_POS_BOTTOM_LEFT)SetWindowPos(NULL, 0, 366, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	else SetWindowPos(NULL, 430, 180, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

    if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
		|| (theMcConfig.GetMhsType() == MHS_R2R)
		|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
	    SetTimer(1, 2000, NULL);

	return FALSE;
}

void CPopUpTopStepperCtr::OnUpCtr()
{
	if (m_n2ndCtrlID == TOP_WINDOW_CLAMPER)
	{
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			theStepper.MoveStep(WINDOW_CLAMP_EX, -round(m_nStepResol / theStepper.GetResol(m_n2ndCtrlID)), MOTION_RELATIVE);	//v3.3.P417 add
		else
			theStepper.MoveStep(m_n2ndCtrlID, -round(100 / theStepper.GetResol(m_n2ndCtrlID)), MOTION_RELATIVE);
	}
	else
		theStepper.MoveStep(m_nCtrModule, round(m_nStepResol/theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
}

void CPopUpTopStepperCtr::OnDownCtr()
{
	if (m_n2ndCtrlID == TOP_WINDOW_CLAMPER)
	{
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
			theStepper.MoveStep(WINDOW_CLAMP_EX, round(m_nStepResol / theStepper.GetResol(m_n2ndCtrlID)), MOTION_RELATIVE);	//v3.3.P417 add
		else
			theStepper.MoveStep(m_n2ndCtrlID, round(100 / theStepper.GetResol(m_n2ndCtrlID)), MOTION_RELATIVE);
	}
	else
		theStepper.MoveStep(m_nCtrModule,-round(m_nStepResol/theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
}

void CPopUpTopStepperCtr::OnRightCtr()
{
	theStepper.MoveStep(m_nCtrModule, round(m_nStepResol/theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
}

void CPopUpTopStepperCtr::OnLeftCtrl()
{
	theStepper.MoveStep(m_nCtrModule, -round(m_nStepResol/theStepper.GetResol(m_nCtrModule)), MOTION_RELATIVE);
}

void CPopUpTopStepperCtr::OnOk()
{
	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);
	OnOK();
}

void CPopUpTopStepperCtr::OnScaleIncrease()
{
	m_nStepResol=m_nStepResol*2;

	WORD wScale=theMcConfig.GetStepperType()==STEP_TYPE_STEPPERIO ? 1600 : 16000;
	if(m_nStepResol>wScale)m_nStepResol=wScale;

	theStepper.SetJogStep(round(m_nStepResol/theStepper.GetResol(m_nCtrModule)));
}

void CPopUpTopStepperCtr::OnScaleDecrease()
{
	m_nStepResol = m_nStepResol/2;
	if (m_nStepResol < 20)
		m_nStepResol = 20;

	theStepper.SetJogStep(round(m_nStepResol / theStepper.GetResol(m_nCtrModule)));
}

void CPopUpTopStepperCtr::OnTimer(UINT_PTR nIDEvent)
{
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
    {
		//for safety, the InputClaw, OutputClaw, Ejector opened if over RELAY_SAFETY_DURATION
		// change RELAY_SAFETY_DURATION to be configurable,maximum 2 minutes
		long lDuration = thePgm.GetRcd()->lRelaySafetyDuration;
        if ((theTopTrack.m_InputClaw.GetDuration() > lDuration)
			|| (theTopTrack.m_OutputClaw.GetDuration() > lDuration)
			|| (theTopTrack.m_Ejector.GetDuration() > lDuration))
            theTopTrack.ResetRelayErrorState();
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
		//for safety, the InputClaw, OutputClaw, Ejector opened if over RELAY_SAFETY_DURATION
		// change RELAY_SAFETY_DURATION to be configurable,maximum 2 minutes
		long lDuration = thePgm.GetRcd()->lRelaySafetyDuration;
        if ((theTEIndex.m_InputClaw.GetDuration() > lDuration)
			|| (theTEIndex.m_OutputClaw.GetDuration() > lDuration)
			|| (theTEIndex.m_Ejector.GetDuration() > lDuration))
            theTEIndex.ResetRelayErrorState();
		break;
    }
	case MHS_R2R:
    {
        if ((theR2RTrack.theInputClaw.GetDuration() > RELAY_SAFTY_DURATION)  ||
            (theR2RTrack.theOutputClaw.GetDuration() > RELAY_SAFTY_DURATION) ||
            (theR2RTrack.theEjector.GetDuration() > RELAY_SAFTY_DURATION))
        {
            theR2RTrack.ResetRelayErrorState();
        }
		break;
    }
	}

    CDialog::OnTimer(nIDEvent);
}

void CPopUpTopStepperCtr::OnRecover()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
        theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
		theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
        theTEIndex.m_pTargetClaw->SetRelayState(RELAY_ON);
		theTEIndex.m_LFHolder.SetRelayState(RELAY_ON);
		break;
	}
	case MHS_R2R:
        theR2RTrack.TargetClaw(RELAY_ON);
	}
}

void CPopUpTopStepperCtr::PresetDlgPos(BYTE nDlgPos)
{
	m_nDlgPos=nDlgPos;
}

void CPopUpTopStepperCtr::Preset2ndCtrlID(BYTE nID)
{
	m_n2ndCtrlID=nID;
}

LRESULT CPopUpTopStepperCtr::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}

BOOL CPopUpTopStepperCtr::OnEraseBkgnd(CDC* pDC)	//V1.1.W147 add
{
	CRect rc;
	GetClientRect(&rc);

	theJpegHelp.Paint(pDC->m_hDC, rc);

	if(g_lpbmArrow==NULL)return TRUE;

	CWireBondDlg* lpMain=(CWireBondDlg*)theApp.m_pMainWnd;

	if(m_dcLeft.m_hDC==NULL)
	{
		g_lpbmArrow->RotateFlip(Rotate180FlipX);
		lpMain->CreateBttnDC(GetDlgItem(IDC_LEFT_CTRL),pDC,m_dcLeft,g_lpbmArrow);
	}

	if(m_dcRight.m_hDC==NULL)
	{
		g_lpbmArrow->RotateFlip(Rotate180FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_RIGHT_CTR),pDC,m_dcRight,g_lpbmArrow);
		g_lpbmArrow->RotateFlip(Rotate180FlipNone);
	}

	if(m_dcUp.m_hDC==NULL)
	{
		g_lpbmArrow->RotateFlip(Rotate90FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_UP_CTR),pDC,m_dcUp,g_lpbmArrow);
		g_lpbmArrow->RotateFlip(Rotate270FlipNone);
	}

	if(m_dcDown.m_hDC==NULL)
	{
		g_lpbmArrow->RotateFlip(Rotate270FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_DOWN_CTR),pDC,m_dcDown,g_lpbmArrow);
		g_lpbmArrow->RotateFlip(Rotate90FlipNone);
	}

	return TRUE;
}

void CPopUpTopStepperCtr::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)
{
	int nLeft=0,nTop=0;
	int nWidth=lpDIS->rcItem.right-lpDIS->rcItem.left;
	int nHeight=lpDIS->rcItem.bottom-lpDIS->rcItem.top;

	if(lpDIS->itemState&ODS_SELECTED)
	{
		nLeft=nTop=1;
		nWidth=nWidth-2;
		nHeight=nHeight-2;

		DrawFrameControl(lpDIS->hDC,&lpDIS->rcItem,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);
	}

	if(nIDCtl==IDC_LEFT_CTRL)StretchBlt(lpDIS->hDC,nLeft,nTop,nWidth,nHeight,m_dcLeft.m_hDC,nLeft,nTop,nWidth,nHeight,SRCCOPY);
	else if(nIDCtl==IDC_RIGHT_CTR)StretchBlt(lpDIS->hDC,nLeft,nTop,nWidth,nHeight,m_dcRight.m_hDC,nLeft,nTop,nWidth,nHeight,SRCCOPY);
	else if(nIDCtl==IDC_UP_CTR)StretchBlt(lpDIS->hDC,nLeft,nTop,nWidth,nHeight,m_dcUp.m_hDC,nLeft,nTop,nWidth,nHeight,SRCCOPY);
	else if(nIDCtl==IDC_DOWN_CTR)StretchBlt(lpDIS->hDC,nLeft,nTop,nWidth,nHeight,m_dcDown.m_hDC,nLeft,nTop,nWidth,nHeight,SRCCOPY);

	CDialog::OnDrawItem(nIDCtl, lpDIS);
}
