// SleepModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "SleepModeDlg.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MachineConfig.h"
#include "VisionSystem.h"
#include "ScanKeyThread.h"
#include "Bond.h"
#include "Jpeg.h"
#include "servo.h"

// CSleepModeDlg dialog

IMPLEMENT_DYNAMIC(CSleepModeDlg, CDialog)

CSleepModeDlg::CSleepModeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSleepModeDlg::IDD, pParent)
	, m_bClickProtectionFlag(false)
{

}

CSleepModeDlg::~CSleepModeDlg()
{
}

void CSleepModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSleepModeDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


HBRUSH CSleepModeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC )
	{
		pDC-> SetBkMode(TRANSPARENT);

		return theApp.m_brNull; 
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CSleepModeDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_INIDLG);
	jpeg.Paint(dc.m_hDC,rect); 
}


// CSleepModeDlg message handlers

BOOL CSleepModeDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
	SetWindowPos(NULL, 0, 0, 700, 450, SWP_NOZORDER);
	CenterWindow();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_STANDBY_INFO);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STANDBY_MODE));

	pWnd = GetDlgItem(IDC_STATIC_HOW_TO_QUIT);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_QUIT_STANDBY_MODE));

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

    theApp.ArchivePowerOnData(ARCHIVE_SAVE);
	Sleep(100);
    theApp.ArchiveMachineData(ARCHIVE_SAVE);
	Sleep(100);

	TBL_POS stTblPos;
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED: case MHS_CU_ELITE: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		stTblPos.dx = theMcData.GetRcd()->stTbSafeStandby.dx;
		stTblPos.dy = theMcData.GetRcd()->stTbSafeStandby.dy;
		break;
	}
	default:
		stTblPos.dx = 41000; //10000;
		stTblPos.dy = 20000; 
	}

	if (!theMcConfig.IsBypassAll())
	{
		theServo.MoveTable2(stTblPos);	//v3.1.T209 edit
		Sleep(100);

		//Turn off side light
		theVisionSys.ClearDrawing();
		theVisionSys.UpdateLightingParameter(0,0,0,0);

		theScanKeyThread.DisableJoyStick();
	}
	else
	{
		Sleep(1000);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSleepModeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	short nPassWord, nInputPassWord;

	nPassWord = 1618;

	theKeyPad.GetUserKeyInData(&nInputPassWord, 0,0, 0, false,true);

	if (nInputPassWord == nPassWord)
	{

		if(m_bClickProtectionFlag == false)
		{
			CSleepModeDlg::OnOK();
			m_bClickProtectionFlag = true;

		}
	}
	

	CDialog::OnLButtonDown(nFlags, point);
}

void CSleepModeDlg::OnOK()
{
	if (!theMcConfig.IsBypassAll())
	{
		theVisionSys.UpdateLightingParameter(70,0,0,0);
		theScanKeyThread.EnableJoyStick();
	}
	else
	{
		Sleep(1000);
	}

	CDialog::OnOK();
}
