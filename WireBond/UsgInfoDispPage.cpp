/***************************************************************
UsgInfoDispPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MachineData.h"
#include "Program.h"
#include "UsgInfoDispPage.h"
#include "UsgControl.h"
#include "MsgDef.h"

// CUsgInfoDispPage dialog

IMPLEMENT_DYNAMIC(CUsgInfoDispPage, CDialog)

CUsgInfoDispPage::CUsgInfoDispPage(CWnd* pParent /*=NULL*/)
	: CDialog(CUsgInfoDispPage::IDD, pParent)
{
}

CUsgInfoDispPage::~CUsgInfoDispPage()
{
}

void CUsgInfoDispPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUsgInfoDispPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(ID_BTN_REFRESH, OnRefresh)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CUsgInfoDispPage message handlers

HBRUSH CUsgInfoDispPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CUsgInfoDispPage::OnInitDialog()
{
	m_BkBrush.CreateSolidBrush(RGB(0, 128, 128));
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect rcMain, rc;
	GetWindowRect(&rc);
	theApp.m_pMainWnd->GetWindowRect(&rcMain);

    SetWindowPos(NULL, (rcMain.Width()-rc.Width())/2,
						(rcMain.Height()-rc.Height())/2, 0, 0,
						SWP_NOZORDER | SWP_NOSIZE);

	SetDlgItemTextW(IDC_STATIC_USG_INFO, theMsg.GetMsg(MSG_USG_SETUP));
	SetDlgItemTextW(IDC_STATIC_USG_IMP, theMsg.GetMsg(MSG_IMPEDANCE));
	SetDlgItemTextW(IDC_STATIC_USG_FREQ, theMsg.GetMsg(MSG_FREQUENCY));
	SetDlgItemTextW(ID_BTN_REFRESH, theMsg.GetMsg(MSG_UPDATE));
	SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));

    SetTimer(1, 100, NULL);

	return CDialog::OnInitDialog();
}

void CUsgInfoDispPage::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

void CUsgInfoDispPage::OnOk()
{
	DestroyWindow();
}

void CUsgInfoDispPage::OnRefresh()
{
	double dFreq = 0.0, dImp = 0.0;
	CFont fontBig, font_noStrike;

	fontBig.CreateFontW(28, 0, 0, 0, 500, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	font_noStrike.CreateFontW(26, 0, 0, 0, 500, FALSE, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    CWnd* pWnd = GetDlgItem(IDC_STATIC_SHOW_RESULT);
	pWnd->SetFont(&font_noStrike);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_CALI_IN_PROGRESS));

    theUSG.CalibrateUsg(thePgm.GetRcd()->chUsgOutputLvl, &dImp, &dFreq);

	CString str;

	pWnd = GetDlgItem(IDC_DISP_IMP);
	pWnd->SetFont(&fontBig);
	str.Format(_T("%6.1f"), dImp);
	pWnd->SetWindowText(str);

	pWnd = GetDlgItem(IDC_DISP_FREQ);
	pWnd->SetFont(&fontBig);
	str.Format(_T("%6.1f"), dFreq);
	pWnd->SetWindowText(str);

	pWnd = GetDlgItem(IDC_STATIC_SHOW_RESULT);
	pWnd->SetFont(&font_noStrike);
	if ((dImp > theMcData.GetRcd()->nImpUpperLimit)
		|| (dImp < theMcData.GetRcd()->nImpLowerLimit))
		str = theMsg.GetMsg(MSG_IMPEDANCE_OUT_RANGE);
	else
		str = theMsg.GetMsg(MSG_USG_CALI_SUCCESS);
	pWnd->SetWindowText(str);
}

void CUsgInfoDispPage::OnPaint()
{
	CPaintDC dc(this);
	CFont fontBig, font_noStrike;

	fontBig.CreateFontW(28, 0, 0, 0, 500, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	font_noStrike.CreateFontW(26, 0, 0, 0, 500, FALSE, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	GetDlgItem(IDC_DISP_IMP)->SetFont(&fontBig);
	GetDlgItem(IDC_DISP_FREQ)->SetFont(&fontBig);
	GetDlgItem(IDC_STATIC_SHOW_RESULT)->SetFont(&font_noStrike);
}

void CUsgInfoDispPage::OnTimer(UINT_PTR nIDEvent)
{
    KillTimer(1);
    OnRefresh();
    CDialog::OnTimer(nIDEvent);
}
