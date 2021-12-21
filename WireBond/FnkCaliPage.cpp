/***************************************************************
FnkCaliPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MachineData.h"
#include "Program.h"
#include "FnkCaliPage.h"
#include "UsgControl.h"
#include "MsgDef.h"
#include "Fnk.h"
#include "MachineConfig.h"

// CFnkCaliPage dialog

IMPLEMENT_DYNAMIC(CFnkCaliPage, CDialog)

CFnkCaliPage::CFnkCaliPage(CWnd* pParent /*=NULL*/)
	: CDialog(CFnkCaliPage::IDD, pParent)
{
}

CFnkCaliPage::~CFnkCaliPage()
{
}

void CFnkCaliPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFnkCaliPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(ID_BTN_REFRESH, OnRefresh)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BTN_CALI_POWER, &CFnkCaliPage::OnBnClickedBtnCaliPower)
END_MESSAGE_MAP()

// CFnkCaliPage message handlers

HBRUSH CFnkCaliPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CFnkCaliPage::OnInitDialog()
{
	m_BkBrush.CreateSolidBrush(RGB(0, 128, 128));
	theApp.SetDlgFont(this);

	CRect rcMain, rc;
	GetWindowRect(&rc);
	theApp.m_pMainWnd->GetWindowRect(&rcMain);

    SetWindowPos(NULL, (rcMain.Width()-rc.Width())/2,
						(rcMain.Height()-rc.Height())/2, 0, 0,
						SWP_NOZORDER | SWP_NOSIZE);

	CString str = _T("FNK - ");
	switch (theMcConfig.GetFnkConstMode())
	{
	case 1: str.Append(_T("Constant Current Mode")); break;
	case 2: str.Append(_T("Constant Power Mode")); break;
	default: str.Append(_T("Constant Voltage Mode"));
	}
	theFnk.SetConstMode((BYTE)theMcConfig.GetFnkConstMode());	//v3.0.T432 add
	theFnk.SetScaleInput(theMcConfig.GetFnkScaleInput());		//v3.0.T432 add

	SetDlgItemTextW(IDC_STATIC_USG_INFO, str);
	SetDlgItemTextW(IDC_STATIC_USG_IMP, theMsg.GetMsg(MSG_IMPEDANCE));
	SetDlgItemTextW(IDC_STATIC_USG_FREQ, theMsg.GetMsg(MSG_FREQUENCY));
	SetDlgItemTextW(ID_BTN_REFRESH, theMsg.GetMsg(MSG_UPDATE));
	SetDlgItemTextW(ID_BTN_CALI_POWER, theMsg.GetMsg(MSG_CALIBRATE));	//v3.1.T2 add
	SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));

    SetTimer(1, 100, NULL);

	return CDialog::OnInitDialog();
}

void CFnkCaliPage::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

void CFnkCaliPage::OnOk()
{
	DestroyWindow();
}

void CFnkCaliPage::OnRefresh()
{
	CFont font;

	font.CreateFontW(26, 0, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SHOW_RESULT);
	pWnd->SetFont(&font);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_CALI_IN_PROGRESS));

	CString str;

	//v3.1.T2 edit
	pWnd = GetDlgItem(IDC_DISP_SCALE_INPUT);
	pWnd->SetFont(&font);
	str.Format(_T("%d"), theMcConfig.GetFnkScaleInput());
	pWnd->SetWindowText(str);
	theFnk.SetScaleInput(theMcConfig.GetFnkScaleInput());
	Sleep_x1ms(200);
	theFnk.OutPower(255, 100);

	pWnd = GetDlgItem(IDC_DISP_IMP);
	pWnd->SetFont(&font);
	double dImp = theFnk.GetImp();	//v3.0.T347 add
	str.Format(_T("%.2f"), dImp);
	pWnd->SetWindowText(str);

	pWnd = GetDlgItem(IDC_DISP_FREQ);
	pWnd->SetFont(&font);
	str.Format(_T("%.2f"), theFnk.GetFreq() / 1000.0);
	pWnd->SetWindowText(str);

	//v3.0.T354 add
	pWnd = GetDlgItem(IDC_DISP_MAX_POWER);
	pWnd->SetFont(&font);
	str.Format(_T("%.3f"), theFnk.GetPower() / 1000.0);
	pWnd->SetWindowText(str);

	pWnd = GetDlgItem(IDC_STATIC_SHOW_RESULT);
	pWnd->SetFont(&font);
	if ((dImp > theMcData.GetRcd()->nImpUpperLimit)
		|| (dImp < theMcData.GetRcd()->nImpLowerLimit))
		str = theMsg.GetMsg(MSG_IMPEDANCE_OUT_RANGE);
	else
		str = theMsg.GetMsg(MSG_USG_CALI_SUCCESS);
	pWnd->SetWindowText(str);
}

void CFnkCaliPage::OnPaint()
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

void CFnkCaliPage::OnTimer(UINT_PTR nIDEvent)
{
    KillTimer(1);
    OnRefresh();
    CDialog::OnTimer(nIDEvent);
}

//v3.1.T1 add
void CFnkCaliPage::AutoCaliPower()
{
	CFont font;

	font.CreateFontW(26, 0, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SHOW_RESULT);
	pWnd->SetFont(&font);
	pWnd->SetWindowText(theMsg.GetMsg(MSG_CALI_IN_PROGRESS));

	//v3.1.T4 add
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(ID_BTN_REFRESH)->EnableWindow(FALSE);
	GetDlgItem(ID_BTN_CALI_POWER)->EnableWindow(FALSE);

	CString str;
	double dCurPowerDiff, dMinPowerDiff_mW = 10000.0;
	double dCurPower, dBestPower;
	short nCurScaleInput, nBestScaleInput;
	nCurScaleInput = nBestScaleInput = theMcConfig.GetFnkScaleInput();
	short nScaleInputMin = theMcConfig.GetFnkScaleInputMin();
	short nScaleInputMax = theMcConfig.GetFnkScaleInputMax();
	bool bMinPowerDiffFound = false;
	double dImp;
	int nRetry = 0;
	do
	{
		pWnd = GetDlgItem(IDC_DISP_SCALE_INPUT);
		pWnd->SetFont(&font);
		str.Format(_T("%d"), nCurScaleInput);
		pWnd->SetWindowText(str);
		theFnk.SetScaleInput(nCurScaleInput);
		Sleep_x1ms(200);
		theFnk.OutPower(255, 100);

		pWnd = GetDlgItem(IDC_DISP_IMP);
		pWnd->SetFont(&font);
		dImp = theFnk.GetImp();
		str.Format(_T("%.2f"), dImp);
		pWnd->SetWindowText(str);

		pWnd = GetDlgItem(IDC_DISP_FREQ);
		pWnd->SetFont(&font);
		str.Format(_T("%.2f"), theFnk.GetFreq() / 1000.0);
		pWnd->SetWindowText(str);

		pWnd = GetDlgItem(IDC_DISP_MAX_POWER);
		pWnd->SetFont(&font);
		dCurPower = theFnk.GetPower();
		str.Format(_T("%.3f"), dCurPower / 1000.0);
		pWnd->SetWindowText(str);

		Invalidate();

		dCurPowerDiff = dCurPower - theMcConfig.GetFnkMaxPower();	//v3.1.T3 edit
		if (abs(dCurPowerDiff) < abs(dMinPowerDiff_mW))
		{
			dMinPowerDiff_mW = dCurPowerDiff;
			dBestPower = dCurPower;
			nBestScaleInput = nCurScaleInput;
			if (dCurPowerDiff < 0)
				nCurScaleInput++;
			else
				nCurScaleInput--;
			nRetry = 0;
		}
		else
		{
			if (abs(dMinPowerDiff_mW) <= theMcConfig.GetFnkMaxPowerTol())
			{
				bMinPowerDiffFound = true;
				theMcConfig.SetFnkScaleInput(nBestScaleInput);
			}
			else
			{
				if (dCurPowerDiff < 0)
					nCurScaleInput++;
				else
					nCurScaleInput--;
				nRetry++;
			}
		}
	} while (!bMinPowerDiffFound && (nRetry < 100)
				&& (nCurScaleInput >= nScaleInputMin)
				&& (nCurScaleInput <= nScaleInputMax));

	pWnd = GetDlgItem(IDC_STATIC_SHOW_RESULT);
	pWnd->SetFont(&font);
	if (!bMinPowerDiffFound)
		str = theMsg.GetMsg(MSG_USG_CALI_FAIL);
	else if ((dImp > theMcData.GetRcd()->nImpUpperLimit)
			|| (dImp < theMcData.GetRcd()->nImpLowerLimit))
		str = theMsg.GetMsg(MSG_IMPEDANCE_OUT_RANGE);
	else
		str = theMsg.GetMsg(MSG_USG_CALI_SUCCESS);
	pWnd->SetWindowText(str);
	OnRefresh();

	//v3.1.T4 add
	GetDlgItem(IDOK)->EnableWindow();
	GetDlgItem(ID_BTN_REFRESH)->EnableWindow();
	GetDlgItem(ID_BTN_CALI_POWER)->EnableWindow();

}

//v3.1.T2 add
void CFnkCaliPage::OnBnClickedBtnCaliPower()
{
	AutoCaliPower();
}
