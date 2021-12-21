/***************************************************************
HelpMsgDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "HelpMsgDlg.h"
#include "Bond.h"
#include "TowerLight.h"
#include "WireBondDlg.h"
#include "Jpeg.h"
#include "servo.h"

// CHelpMsgDlg dialog

IMPLEMENT_DYNAMIC(CHelpMsgDlg, CDialog)

CHelpMsgDlg::CHelpMsgDlg(bool bStopValid, bool bSkipValid, short nCallBackId, bool bSpeedUpButton, CWnd* pParent, bool bCtrLight, short nMode)
	: CDialog(CHelpMsgDlg::IDD, pParent)
    , m_sMsg0(_T(""))
    , m_sMsg1(_T(""))
    , m_sMsg2(_T(""))
    , m_sMsg3(_T(""))
    , m_sMsgTitle(_T("Help Message"))
    , m_nMsgType(0)
    , m_bStopSelected(false)
    , m_bSkipSelected(false)
{
    m_bStopButtonValid = bStopValid;
    m_bSkipButtonValid = bSkipValid;
    m_nCallBackId = nCallBackId;
    m_bSpeedUpButton = bSpeedUpButton;
	m_bCtrLight = bCtrLight;
	m_nMode = nMode;
	m_bCenter = false;
}

CHelpMsgDlg::~CHelpMsgDlg()
{
}

void CHelpMsgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MSG_LIST0, m_sMsg0);
    DDX_Text(pDX, IDC_MSG_LIST1, m_sMsg1);
    DDX_Text(pDX, IDC_MSG_LIST2, m_sMsg2);
    DDX_Text(pDX, IDC_MSG_LIST3, m_sMsg3);
    DDX_Text(pDX, IDC_MSG_TITLE, m_sMsgTitle);
}

BEGIN_MESSAGE_MAP(CHelpMsgDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CHelpMsgDlg::OnBnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_SKIP, &CHelpMsgDlg::OnBnClickedButtonSkip)
    ON_BN_CLICKED(IDC_BUTTON_SPEED_DOWN, &CHelpMsgDlg::OnBnClickedButtonSpeedDown)
    ON_BN_CLICKED(IDC_BUTTON_SPEED_UP, &CHelpMsgDlg::OnBnClickedButtonSpeedUp)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CHelpMsgDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_LIGHT, &CHelpMsgDlg::OnBnClickedLight)
END_MESSAGE_MAP()

void CHelpMsgDlg::SetMessageBoxTitle(short nMsgType, short nMsgID)
{
	m_sMsgTitle = theMsg.GetMsg(nMsgID);
    m_nMsgType = nMsgType;
}

void CHelpMsgDlg::SetMessageList(short nMsgNum, short *pnMsgIndex)
{
	if (nMsgNum > 0)
		m_sMsg0 = theMsg.GetMsg(*(pnMsgIndex + 0));
	if (nMsgNum > 1)
		m_sMsg1 = theMsg.GetMsg(*(pnMsgIndex + 1));
	if (nMsgNum > 2)
		m_sMsg2 = theMsg.GetMsg(*(pnMsgIndex + 2));
	if (nMsgNum > 3)
		m_sMsg3 = theMsg.GetMsg(*(pnMsgIndex + 3));
}

void CHelpMsgDlg::SetMessageList(CStringList& sList)
{
    short nMsgNum = sList.GetSize();

	if (nMsgNum  > 4)
	{
		m_sMsg0 = sList.GetAt(sList.FindIndex(nMsgNum-4));
		m_sMsg1 = sList.GetAt(sList.FindIndex(nMsgNum-3));
		m_sMsg2 = sList.GetAt(sList.FindIndex(nMsgNum-2));
		m_sMsg3 = sList.GetAt(sList.FindIndex(nMsgNum-1));
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

void CHelpMsgDlg::CenterDialog(bool bCenter)
{
	m_bCenter = bCenter;
}

BOOL CHelpMsgDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
 
	if (m_bCenter)
		CenterWindow();
	else
		SetWindowPos(NULL, 510, LIGHT_DLG_TOP, 570, LIGHT_DLG_HEIGHT, SWP_NOZORDER);

    CPoint pt;

	CWnd* pWnd = GetDlgItem(IDC_MSG_TITLE);
	font.CreateFontW(38, 0, 0, 0, 700, false, false, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
						DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	pWnd->SetFont(&font);
	pWnd->SetWindowPos(NULL, 30, 15, 380, 100, SWP_NOZORDER);
	
	switch (m_nMode)
	{
	case 0: //orginal hard code !!
	{
		if (!m_bStopButtonValid && !m_bSkipButtonValid)
		{
			pt.x = 233;
			pt.y = 245;

			pWnd = GetDlgItem(IDOK);
			pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
			pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

			pWnd = GetDlgItem(IDC_BUTTON_STOP);
			pWnd->ShowWindow(SW_HIDE);

			pWnd = GetDlgItem(IDC_BUTTON_SKIP);
			pWnd->ShowWindow(SW_HIDE);

			pWnd = GetDlgItem(IDC_LIGHT);
			pWnd->ShowWindow(SW_HIDE);
		}
		else if (m_bStopButtonValid && !m_bSkipButtonValid)
		{
			pt.x = 170;
			pt.y = 245;

			pWnd = GetDlgItem(IDOK);
			pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
			pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CONFIRM));

			pt.x += BUTTON_BOND_WIDTH;

			pWnd = GetDlgItem(IDC_BUTTON_STOP);
			pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
			pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

			pWnd = GetDlgItem(IDC_BUTTON_SKIP);
			pWnd->ShowWindow(SW_HIDE);

			if (m_bCtrLight)
			{
				pt.x = 235;
				pt.y = 140;
				pWnd = GetDlgItem(IDC_LIGHT);
				pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
				pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LIGHT_2));
			}
			else
			{
				pWnd = GetDlgItem(IDC_LIGHT);
				pWnd->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			pt.x = 128;
			pt.y = 245;

			pWnd = GetDlgItem(IDOK);
			pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
			pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CONFIRM));

			pt.x += BUTTON_BOND_WIDTH;

			pWnd = GetDlgItem(IDC_BUTTON_STOP);
			pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
			pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

			pt.x += BUTTON_BOND_WIDTH;

			pWnd = GetDlgItem(IDC_BUTTON_SKIP);
			pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
			pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SKIP));
			pWnd = GetDlgItem(IDC_LIGHT);
			pWnd->ShowWindow(SW_HIDE);
		}

		if (m_bSpeedUpButton)
		{
			 pt.x = 170;
			 pt.y = 200;
			 pWnd = GetDlgItem(IDC_BUTTON_SPEED_UP);
			 pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, 40, SWP_NOZORDER);
			 pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INCREASE));
			 pWnd->EnableWindow(1);
			 pWnd->ShowWindow(1);

			 pt.x = pt.x + BUTTON_BOND_WIDTH;

			 pWnd = GetDlgItem(IDC_BUTTON_SPEED_DOWN);
			 pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, 40, SWP_NOZORDER);
			 pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DECREASE));
			 pWnd->EnableWindow(1);
			 pWnd->ShowWindow(1);
		}
		break;
	}
	case 1: //insert [yes, no], bad coding !!  
	{
		pt.x = 180;
		pt.y = 245;

		pWnd = GetDlgItem(IDOK);
		pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_YES));

		pt.x += BUTTON_BOND_WIDTH;

		pWnd = GetDlgItem(IDC_BUTTON_STOP);
		pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NO));

		pWnd = GetDlgItem(IDC_BUTTON_SKIP);
		pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_LIGHT);
		pWnd->ShowWindow(SW_HIDE);
		break;
	}
	case 2://inser [load, save, cancel], bad coding !!
	{
		pt.x = 128;
		pt.y = 245;

		pWnd = GetDlgItem(IDOK);
		pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LOAD));

		pt.x += BUTTON_BOND_WIDTH;

		pWnd = GetDlgItem(IDC_BUTTON_STOP);
		pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SAVE));

		pt.x += BUTTON_BOND_WIDTH;

		pWnd = GetDlgItem(IDC_BUTTON_SKIP);
		pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CANCEL));

		pWnd = GetDlgItem(IDC_LIGHT);
		pWnd->ShowWindow(SW_HIDE);
		break;
	}
	default:
		 break;
	}

	if (m_nMsgType == WARNING_MSG)
	{
		theTowerLight.EnableIdleLight();
		theTowerLight.EnableWarningLight(true);
	}
	else if (m_nMsgType == ERROR_MSG)
	{
		theTowerLight.EnableWarningLight(false);
		theTowerLight.EnableErrorLight();
		theTowerLight.EnableBlinkRedLight(true);
	}
    
    return CDialog::OnInitDialog();
}

HBRUSH CHelpMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC )
    {
        pDC->SetBkMode(TRANSPARENT);
		return theApp.m_brNull; 
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CHelpMsgDlg::OnBnClickedButtonStop()
{
    m_bStopSelected = true;
	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableWarningLight(false);
    OnOK();
}

void CHelpMsgDlg::OnBnClickedButtonSkip()
{
    m_bSkipSelected = true;
	theTowerLight.EnableBlinkRedLight(false);
	theTowerLight.EnableWarningLight(false);
    OnOK();
}

void CHelpMsgDlg::OnOK()
{
    if (m_nCallBackId > 0)
    {
        theBond.HelpMsgCallBackFunction(m_nCallBackId);
        DestroyWindow();
    }
    else
        CDialog::OnOK();
}

void CHelpMsgDlg::OnBnClickedButtonSpeedDown()
{
    theServo.JogSpeedDown();
}

void CHelpMsgDlg::OnBnClickedButtonSpeedUp()
{
    theServo.JogSpeedUp();
}

void CHelpMsgDlg::OnPaint()
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

void CHelpMsgDlg::OnBnClickedOk()
{
	theTowerLight.EnableWarningLight(false);
	theTowerLight.EnableBlinkRedLight(false);
	OnOK();
}

void CHelpMsgDlg::OnBnClickedLight()
{
    //CLightingCtrlDlg* pLightCtrlDlg = new CLightingCtrlDlg();

    //pLightCtrlDlg->Create(CLightingCtrlDlg::IDD, this);
	((CWireBondDlg*)(this->GetParent()))->m_pLightCtrlDlg->UpdateDisp();
	((CWireBondDlg*)(this->GetParent()))->m_pLightCtrlDlg->ShowWindow(SW_SHOW);
}
