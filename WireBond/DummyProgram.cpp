/***************************************************************
DummyProgram.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "macdef.h"
#include "MathCalc.h"
#include "msgdef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "MenuConfig.h"
#include "VisionCalibDlg.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "DummyProgram.h"
#include "DataStruct.h"
#include "Bond.h"
#include "servo.h"
#include "Busy.h"

// CDummyProgram dialog

IMPLEMENT_DYNAMIC(CDummyProgram, CDialog)

CDummyProgram::CDummyProgram(CWnd* pParent /*=NULL*/)
: CDialog(CDummyProgram::IDD, pParent)
, m_nPgmType(1)
, m_nWirePitch(200)
, m_nTeachStep(0)
, m_nWireLen(1000)
{
	m_bBusy = FALSE;	//V1.4.W42 add
	m_InfoMessage = theMsg.GetMsg(MSG_START_DUMMY_PROGRAM);
}

CDummyProgram::~CDummyProgram()
{
}

void CDummyProgram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_PROGRAM_TYPE, m_nPgmType);
	DDX_Text(pDX, IDC_EDIT_WIRE_PITCH, m_nWirePitch);
	DDX_Text(pDX, IDC_EDIT3, m_InfoMessage);
	DDX_Text(pDX, IDC_EDIT_WIRE_LENGTH, m_nWireLen);
}

BEGIN_MESSAGE_MAP(CDummyProgram, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_WIRE_PITCH, OnFocusWirePitch)
	ON_EN_SETFOCUS(IDC_EDIT_WIRE_LENGTH, OnFocusWireLength)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.4.W42 add
END_MESSAGE_MAP()

// CDummyProgram message handlers
BOOL CDummyProgram::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP,
					TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);
	return CDialog::OnInitDialog();
}

void CDummyProgram::OnStart()
{
	UpdateData(TRUE);
	m_InfoMessage = theMsg.GetMsg(MSG_MOVE_TO_TOP_LEFT_POS);
	m_InfoMessage += _T(" ") + theMsg.GetMsg(MSG_NEXT_TO_CONTINUE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow();
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	UpdateData(FALSE);
	m_nTeachStep = 0;
}

void CDummyProgram::OnNext()
{
	CBusy bs(&m_bBusy);	//V1.4.W42 add

	int nWirePitchEnc, nWireLenEnc;
	if (m_nTeachStep == 0)
	{
		if (theMcConfig.IsBypassAll())
		{
			m_stLeftTopPos.dx = 12000.0 / theServo.GetResolXY();
			m_stLeftTopPos.dy = 12000.0 / theServo.GetResolXY();
		}
		else
			theServo.GetTablePosition(&m_stLeftTopPos);

		m_InfoMessage = theMsg.GetMsg(MSG_MOVE_TO_BOTTOM_RIGHT_POS);
		m_InfoMessage += _T(" ") + theMsg.GetMsg(MSG_NEXT_TO_CONTINUE);
		UpdateData(FALSE);

		m_nTeachStep = 1;
	}
	else if (m_nTeachStep == 1)
	{
		if (theMcConfig.IsBypassAll())
		{
			m_stRightBottomPos.dx = -10000.0 / theServo.GetResolXY();
			m_stRightBottomPos.dy = -10000.0 / theServo.GetResolXY();
		}
		else
			theServo.GetTablePosition(&m_stRightBottomPos);

		nWirePitchEnc  = round((double)m_nWirePitch / theServo.GetResolXY());
		nWireLenEnc = round((double)m_nWireLen / theServo.GetResolXY());

		thePgm.CreateDummyProgram(m_stLeftTopPos, m_stRightBottomPos,
			m_nPgmType, nWirePitchEnc, nWireLenEnc, &theVisionSys);
		theApp.ArchivePowerOnData(ARCHIVE_SAVE);
		theBond.ResetBondData();
		theBond.SetAutoIndex(false);
		theBond.SetBondNoVision(true);
		((CWireBondDlg*)theApp.m_pMainWnd)->UpdateOverView();	

		m_InfoMessage = theMsg.GetMsg(MSG_DUMMY_PGM_COMPLETED);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow();
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		UpdateData(FALSE);

		m_nTeachStep = 2;
	}
	else
		m_nTeachStep = 0;
}

HBRUSH CDummyProgram::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDummyProgram::OnFocusWirePitch()
{
	UpdateData(true);
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nWirePitch, 200, 50, 500);
	UpdateData(false);
}

void CDummyProgram::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

void CDummyProgram::OnFocusWireLength()
{
	UpdateData(true);
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nWireLen, 1000, 100, 6000);
	UpdateData(false);
}

LRESULT CDummyProgram::CloseDialog(WPARAM wParam, LPARAM lParam)	//V1.4.W42 add
{
	return (m_bBusy == FALSE && DestroyWindow()) * DIALOG_CLOSED;
}
