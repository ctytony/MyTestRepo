/***************************************************************
WarnMsgDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "WarnMsgDlg.h"
#include "Bond.h"
#include "TowerLight.h"
#include "MaterialHandlerThread.h"
#include "BSDDebug.h"
#include "MachineConfig.h"
#include "Jpeg.h"
#include "VledOffload.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2ROnload.h"
#include "R2ROffload.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CWarnMsgDlg dialog

IMPLEMENT_DYNAMIC(CWarnMsgDlg, CDialog)

CWarnMsgDlg::CWarnMsgDlg(short nCallBackId, short nClearTrack, CWnd* pParent /*=NULL*/)
	: CDialog(CWarnMsgDlg::IDD, pParent)
    , m_sMsg0(_T(""))
    , m_sMsg1(_T(""))
    , m_sMsg2(_T(""))
    , m_sMsg3(_T(""))
    , m_sMsgTitle(_T("Warning Message"))
    , m_nMsgType(0)
{
    m_nCallBackId = nCallBackId;
    m_nClearTrack = nClearTrack;
}

CWarnMsgDlg::~CWarnMsgDlg()
{
}

void CWarnMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MSG_LIST0, m_sMsg0);
    DDX_Text(pDX, IDC_MSG_LIST1, m_sMsg1);
    DDX_Text(pDX, IDC_MSG_LIST2, m_sMsg2);
    DDX_Text(pDX, IDC_MSG_LIST3, m_sMsg3);
    DDX_Text(pDX, IDC_MSG_TITLE, m_sMsgTitle);
}

BEGIN_MESSAGE_MAP(CWarnMsgDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CWarnMsgDlg::OnBnClickedButtonClearTrack)
	ON_BN_CLICKED(IDC_BUTTON_SKIP, &CWarnMsgDlg::OnBnClickedButtonCancel)
    ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CWarnMsgDlg message handlers

void CWarnMsgDlg::SetMessageBoxTitle(short nMsgType, short nMsgID)
{
    m_sMsgTitle = theMsg.GetMsg(nMsgID);
    m_nMsgType = nMsgType;
}

void CWarnMsgDlg::SetMessageList(CStringList& sList)
{
    short nMsgNum = sList.GetSize();

	if (nMsgNum > 4)
	{
		m_sMsg0 = sList.GetAt(sList.FindIndex(nMsgNum - 4));
		m_sMsg1 = sList.GetAt(sList.FindIndex(nMsgNum - 3));
		m_sMsg2 = sList.GetAt(sList.FindIndex(nMsgNum - 2));
		m_sMsg3 = sList.GetAt(sList.FindIndex(nMsgNum - 1));
	}
	else
	{
		if (nMsgNum > 0)
			m_sMsg0 = sList.GetAt(sList.FindIndex(0));
		if (nMsgNum > 1)
			m_sMsg1 = sList.GetAt(sList.FindIndex(1));
		if (nMsgNum > 2)
			m_sMsg2 = sList.GetAt(sList.FindIndex(2));
		if (nMsgNum > 3)
			m_sMsg3 = sList.GetAt(sList.FindIndex(3)); 
	}
}

BOOL CWarnMsgDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_LIGHT);
	pWnd->ShowWindow(SW_HIDE);

	SetWindowPos(NULL, LIGHT_DLG_LEFT, LIGHT_DLG_TOP, 480, LIGHT_DLG_HEIGHT, SWP_NOZORDER);	//V1.1.W26 edit

	CPoint pt;

	if (m_nClearTrack == 0)
		pt.x = 220;
	else
		pt.x = 165;

	pt.y = 245;

	pWnd = GetDlgItem(IDC_MSG_TITLE);
	font.CreateFontW(38, 0, 0, 0, 700, false, false, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
						DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	pWnd->SetFont(&font);
	pWnd->SetWindowPos(NULL, 30, 15, 380, 100, SWP_NOZORDER);

	pWnd = GetDlgItem(IDOK);
	pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

	if ((m_nCallBackId == MSG_CALLBACK_ONLOAD_EMPTY) ||
	    (m_nCallBackId == MSG_CALLBACK_OFFLOAD_FORK_FULL))
	    pWnd->ShowWindow(SW_HIDE);
	else
	    pWnd->ShowWindow(SW_SHOWNORMAL);

	pWnd = GetDlgItem(IDC_BUTTON_STOP);
	if (m_nClearTrack == 0)
		pWnd->ShowWindow(SW_HIDE);
	else
	{
		pt.x += BUTTON_BOND_WIDTH;

		pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLEAR_TRACK));
		pWnd->ShowWindow(SW_SHOWNORMAL);
	}

	pWnd = GetDlgItem(IDC_BUTTON_SKIP);
	if (m_nCallBackId == MSG_CALLBACK_AUTOSAMPLING_ONOK)
	{
	    pt.x += BUTTON_BOND_WIDTH;
	    pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
	    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SKIP));
	    pWnd->ShowWindow(1);
	}
	else
	    pWnd->ShowWindow(SW_HIDE);

	if (m_nMsgType == WARNING_MSG)
		theTowerLight.EnableWarningLight(true);
	else if (m_nMsgType == ERROR_MSG)
	{
		theTowerLight.EnableErrorLight();
		theTowerLight.EnableBlinkRedLight(true);
	}

	//MsgHandleEvent(); //@5
	if (m_nCallBackId != MSG_CALLBACK_NONE)
		SetTimer(1, 1000, NULL);

    return CDialog::OnInitDialog();
}

/* @5
int CWarnMsgDlg::MsgHandleEvent()
{
    switch(m_nCallBackId) //@1
	{
	case MSG_CALLBACK_LF_NOT_EXIST:
        {
			theMHS.LastLeadframeHandle();
        }
        break;

	default:
		break;
	}
	
	return 0;
}
*/

HBRUSH CWarnMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC )
    {
        pDC-> SetBkMode(TRANSPARENT);
        return theApp.m_brNull;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CWarnMsgDlg::PostNcDestroy()
{
    CDialog::PostNcDestroy();

	//delete this;
}

void CWarnMsgDlg::OnOK()
{
    GetDlgItem(IDOK)->EnableWindow(FALSE);
	KillTimer(1);

	switch (theMcConfig.GetMhsType())	//v3.0.T154 edit
	{
	case MHS_TOP_LED:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		theTopOnloader.SetBufferEmptyReported(false);
		theTopOnloader.SetBufferEmpty(false);
		theTopOnloader.SetEmptyReported(false);
		theTopOnloader.SetEmpty(false);
		theTopOnloader.SetPusherFullReported(false);
		theTopOnloader.SetPusherFull(false);
		theTopOnloader.SetDropReported(false);
		theTopOnloader.SetDrop(false);

		theTopOffloader.SetBufferEmptyReported(false);
		theTopOffloader.SetBufferEmpty(false);
		theTopOffloader.SetEmptyReported(false);
		theTopOffloader.SetEmpty(false);
		theTopOffloader.SetPusherFullReported(false);
		theTopOffloader.SetPusherFull(false);
		theTopOffloader.SetDropReported(false);
		theTopOffloader.SetDrop(false);

		theTopOnloader.ResetErrorCode();
		theTopOffloader.ResetErrorCode();
		break;
	}
	case MHS_CU_ELITE:
	{
		theCuEliteOnloader.SetOnloadBufferEmptyReported(false);
		theCuEliteOnloader.SetOnloadBufferEmptyflag(false);
		theCuEliteOnloader.SetOnloadEmptyReported(false);
		theCuEliteOnloader.SetOnloadEmptyflag(false);
		theCuEliteOnloader.SetOnloadPullyFullReported(false);
		theCuEliteOnloader.SetOnloadPullyFullflag(false);
		theCuEliteOnloader.SetOnloadDrogReported(false);
		theCuEliteOnloader.SetOnloadDrogflag(false);

		theCuEliteOffloader.SetOffloadBufferEmptyReported(false);
		theCuEliteOffloader.SetOffloadBufferEmptyflag(false);
		theCuEliteOffloader.SetOffloadEmptyReported(false);
		theCuEliteOffloader.SetOffloadEmptyflag(false);
		theCuEliteOffloader.SetOffloadPullyFullReported(false);
		theCuEliteOffloader.SetOffloadPullyFullflag(false);
		theCuEliteOffloader.SetOffloadDrogReported(false);
		theCuEliteOffloader.SetOffloadDrogflag(false);

		theCuEliteOnloader.ResetOnloadErrorCode();
		theCuEliteOffloader.ResetOffloadErrorCode();
		break;
	}
	case MHS_R2R:
	{
		theR2ROnloader.ResetOnloadErrorCode();
		theR2ROffloader.ResetOffloadErrorCode();
		theR2ROnloader.SetOnloadErrorReported(false);
		theR2ROffloader.SetOffloadErrorReported(false);
		break;
	}
	}

    GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

    theBond.HelpMsgCallBackFunction(m_nCallBackId);

	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableWarningLight(false);

	PostMessage(WM_CLOSE);
}

void CWarnMsgDlg::OnBnClickedButtonClearTrack()
{
	switch (theMcConfig.GetMhsType())	//v3.0.T154 edit
	{
	case MHS_VLED_ELITE:
	    theMHS.ClearLastLf();
		break;
	default:
		theMHS.ClearTrack();
	}
}

void CWarnMsgDlg::OnBnClickedButtonCancel()
{
    GetDlgItem(IDOK)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	KillTimer(1);

	theBSDDebug.ResetSampling();
	theBSDDebug.ResetFalseAlmcnter();
	theBSDDebug.SetAutoSamplingFlag(false);

	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableWarningLight(false);

	PostMessage(WM_CLOSE);
}

void CWarnMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(m_nCallBackId)
	{
	case MSG_CALLBACK_ONLOAD_EMPTY:
	{
		if (theVledOnload.GetOnloadEmptyFlag() == false)
		{
			theTowerLight.EnableBlinkRedLight(false);
			KillTimer(1);
			PostMessage(WM_CLOSE);
			//theTowerLight.EnableWarningLight(false);				
		}
		else
		{
			//theTowerLight.EnableWarningLight(true);
			theTowerLight.EnableBlinkRedLight(true);
		}
		break;
	}
	case MSG_CALLBACK_OFFLOAD_FORK_FULL:
	{
		if (theVledOffload.CheckOffloadFull() == false)
		{
			theTowerLight.EnableBlinkRedLight(false);
			KillTimer(1);
			PostMessage(WM_CLOSE);
			//theTowerLight.EnableWarningLight(false);
		}
		else
		{
			theTowerLight.EnableBlinkRedLight(true);
			//theTowerLight.EnableWarningLight(true);
		}
		break;
	}
	case MSG_CALLBACK_ONLOAD_MAG_BUF_EMPTY:
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T154 edit
		{
		case MHS_TOP_LED:
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			if (theTopOnloader.IsBufferEmpty() == false)
			{
				KillTimer(1);

				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theTopOnloader.SetBufferEmpty(false);
				theTopOnloader.SetBufferEmptyReported(false);
				theTopOffloader.SetBufferEmptyReported(false);
				theTopOnloader.SetPusherFullReported(false);
				theTopOffloader.SetPusherFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		case MHS_CU_ELITE:
		{
			if (theCuEliteOnloader.GetOnloadBufferEmptyflag() == false)
			{
				KillTimer(1);
				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theCuEliteOnloader.SetOnloadBufferEmptyflag(false);
				theCuEliteOnloader.SetOnloadBufferEmptyReported(false);
				theCuEliteOffloader.SetOffloadBufferEmptyReported(false);
				theCuEliteOnloader.SetOnloadPullyFullReported(false);
				theCuEliteOffloader.SetOffloadPullyFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		}
		break;
	}
	case MSG_CALLBACK_OFFLOAD_MAG_BUF_EMPTY:
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T154 edit
		{
		case MHS_TOP_LED:
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			if (theTopOffloader.IsBufferEmpty() == false)
			{
				KillTimer(1);
				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theTopOffloader.SetBufferEmpty(false);
				theTopOnloader.SetBufferEmptyReported(false);
				theTopOffloader.SetBufferEmptyReported(false);
				theTopOnloader.SetPusherFullReported(false);
				theTopOffloader.SetPusherFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		case MHS_CU_ELITE:
		{
			if (theCuEliteOffloader.GetOffloadBufferEmptyflag() == false)
			{
				KillTimer(1);
				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theCuEliteOffloader.SetOffloadBufferEmptyflag(false);
				theCuEliteOnloader.SetOnloadBufferEmptyReported(false);
				theCuEliteOffloader.SetOffloadBufferEmptyReported(false);
				theCuEliteOnloader.SetOnloadPullyFullReported(false);
				theCuEliteOffloader.SetOffloadPullyFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		}
		break;
	}
	case MSG_CALLBACK_ONLOAD_PULLY_FULL:
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T154 edit
		{
		case MHS_TOP_LED:
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			if (theTopOnloader.IsPusherFull() == false)
			{
				KillTimer(1);
				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theTopOnloader.SetPusherFull(false);
				theTopOnloader.SetBufferEmptyReported(false);
				theTopOffloader.SetBufferEmptyReported(false);
				theTopOnloader.SetPusherFullReported(false);
				theTopOffloader.SetPusherFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		case MHS_CU_ELITE:
		{
			if (theCuEliteOnloader.GetOnloadPullyFullflag() == false)
			{
				KillTimer(1);
				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theCuEliteOnloader.SetOnloadPullyFullflag(false);
				theCuEliteOnloader.SetOnloadBufferEmptyReported(false);
				theCuEliteOffloader.SetOffloadBufferEmptyReported(false);
				theCuEliteOnloader.SetOnloadPullyFullReported(false);
				theCuEliteOffloader.SetOffloadPullyFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		}
		break;
	}
	case MSG_CALLBACK_OFFLOAD_PULLY_FULL:
	{
		switch (theMcConfig.GetMhsType())	//v3.0.T154 edit
		{
		case MHS_TOP_LED:
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			if (theTopOffloader.IsPusherFull() == false)
			{
				KillTimer(1);
				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theTopOffloader.SetPusherFull(false);
				theTopOnloader.SetBufferEmptyReported(false);
				theTopOffloader.SetBufferEmptyReported(false);
				theTopOnloader.SetPusherFullReported(false);
				theTopOffloader.SetPusherFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		case MHS_CU_ELITE:
		{
			if (theCuEliteOffloader.GetOffloadPullyFullflag() == false)
			{
				KillTimer(1);
				PostMessage(WM_CLOSE);
				theTowerLight.EnableWarningLight(false);
				theCuEliteOffloader.SetOffloadPullyFullflag(false);
				theCuEliteOnloader.SetOnloadBufferEmptyReported(false);
				theCuEliteOffloader.SetOffloadBufferEmptyReported(false);
				theCuEliteOnloader.SetOnloadPullyFullReported(false);
				theCuEliteOffloader.SetOffloadPullyFullReported(false);
				theMHS.ShowError();
			}
			else
			{
				theTowerLight.EnableWarningLight(true);
				//theTowerLight.BlinkYellowLight();
			}
			break;
		}
		}
		break;
	}
	default:
		break;
	}

    CDialog::OnTimer(nIDEvent);
}

void CWarnMsgDlg::OnPaint()
{
  	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (m_nMsgType == HELP_MSG)
		jpeg.Load(hInst,IDR_JPEG_HELP);
    else if (m_nMsgType == ERROR_MSG)
		jpeg.Load(hInst,IDR_JPEG_ERROR);
    else
		jpeg.Load(hInst,IDR_JPEG_WARN);

	jpeg.Paint(dc.m_hDC,rect);
}

void CWarnMsgDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CWarnMsgDlg::OnClose()
{
	theMsg.SetMsgLock(false);

    //OnDestroy();
	CDialog::OnClose();
}
