/***************************************************************
N2h2WaitStableDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "N2h2WaitStableDlg.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "Bond.h"

// CN2h2WaitStableDlg dialog

IMPLEMENT_DYNAMIC(CN2h2WaitStableDlg, CDialog)

CN2h2WaitStableDlg::CN2h2WaitStableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CN2h2WaitStableDlg::IDD, pParent)
	, m_nN2h2WaitStable(0)
{

}

CN2h2WaitStableDlg::~CN2h2WaitStableDlg()
{
}

void CN2h2WaitStableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CN2h2WaitStableDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CN2h2WaitStableDlg message handlers

BOOL CN2h2WaitStableDlg::OnInitDialog()
{
	this->SetWindowPos(NULL, 0, 0, 400, 120, SWP_NOZORDER);
	this->CenterWindow();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_WARN_MSG);
	pWnd->SetWindowPos(NULL, 135, 20, 260, 100, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_WAIT_N2H2_STABLE));

	CDialog::OnInitDialog();

	SetTimer(1, 1000, NULL);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CN2h2WaitStableDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return theApp.m_brNull;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CN2h2WaitStableDlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmBackground;
	bmBackground.LoadBitmap(IDB_BITMAP_UPS);

	//IMAGE ID      
	BITMAP bitmap;      
	bmBackground.GetBitmap(&bitmap);      
	CBitmap *pbmpOld = dcMem.SelectObject(&bmBackground);      
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,      
					bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);     
}

void CN2h2WaitStableDlg::OnTimer(UINT_PTR nIDEvent)
{
	theBond.m_N2h2Port.SetRelayState(RELAY_ON, true);

	m_nN2h2WaitStable = m_nN2h2WaitStable + TIMER1_DELAY;
	if (m_nN2h2WaitStable >= thePgm.GetN2H2StableTime())
		OnOK();

	CDialog::OnTimer(nIDEvent);
}
