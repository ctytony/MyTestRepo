// UsbBsdDebugPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "UsbBsdDebugPage.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MenuConfig.h"
#include "BsdControl.h"
#include "Busy.h"

// CUsbBsdDebugPage dialog
IMPLEMENT_DYNAMIC(CUsbBsdDebugPage, CDialog)

CUsbBsdDebugPage::CUsbBsdDebugPage(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbBsdDebugPage::IDD, pParent)
	, m_sDebugInfo(_T(""))
	, m_iDebugMode(0)
	, m_nDac1(0)
	, m_nDac2(255)
{
	m_bBusy = FALSE;	//V1.4.W38 add
}

CUsbBsdDebugPage::~CUsbBsdDebugPage()
{
}

void CUsbBsdDebugPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEBUG_DISPLAY, m_sDebugInfo);
	DDX_CBIndex(pDX, IDC_COMBO_BSD_DEBUG_MODE, m_iDebugMode);
	DDX_Text(pDX, IDC_EDIT_DAC_1, m_nDac1);
	DDX_Text(pDX, IDC_EDIT_DAC2, m_nDac2);
}

BEGIN_MESSAGE_MAP(CUsbBsdDebugPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_BSD_CONN, OnBsdConn)
	ON_BN_CLICKED(IDC_BUTTON_BSD_CLOSE, OnBsdClose)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_INFO, OnClearInfo)
	ON_EN_SETFOCUS(IDC_EDIT_DAC_1, OnFocusDac1)
	ON_EN_SETFOCUS(IDC_EDIT_DAC2, OnFocusDac2)
	ON_EN_CHANGE(IDC_EDIT_DAC_1, OnEditDac1)
	ON_EN_CHANGE(IDC_EDIT_DAC2, OnEditDac2)
	ON_BN_CLICKED(IDC_BUTTON_DATA_RETRIVE, OnDataRetrieve)
	ON_BN_CLICKED(IDC_BUTTON_BSD_RESET, OnBsdReset)
	ON_BN_CLICKED(IDC_BUTTON_BSD_RESULT, OnBsdResult)
	ON_CBN_SELCHANGE(IDC_COMBO_BSD_DEBUG_MODE, OnBsdDebugMode)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.4.W38 add
END_MESSAGE_MAP()

// CUsbBsdDebugPage message handlers
BOOL CUsbBsdDebugPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BSD_DEBUG_MODE);
	pComboBox->AddString(_T("None"));
	pComboBox->AddString(_T("Capture"));

    BYTE bVer;
	theBSD.GetVersion(&bVer);

	m_sDebugInfo.AppendFormat(_T("Firmware Version: 1.%d"), bVer);

	return CDialog::OnInitDialog();
}

void CUsbBsdDebugPage::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

HBRUSH CUsbBsdDebugPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CUsbBsdDebugPage::OnBsdConn()
{
	if (m_sDebugInfo.GetLength() > 0)
		m_sDebugInfo.Append(_T("\r\n"));

	if (theBSD.OpenUsbDevice() == BSD_COMM_OK)
		m_sDebugInfo.Append(_T("Connection Success!"));
	else
		m_sDebugInfo.Append(_T("Connection Error!"));

	UpdateData(FALSE);
}

void CUsbBsdDebugPage::OnBsdClose()
{
	CBusy bs(&m_bBusy);		//V1.4.W38 add
	theBSD.CloseUsbDevice();
}

void CUsbBsdDebugPage::OnClearInfo()
{
	m_sDebugInfo.Empty();
	UpdateData(FALSE);
}

void CUsbBsdDebugPage::OnDataRetrieve()
{
	CBusy bs(&m_bBusy);		//V1.4.W38 add

	BSD_SETTING stBsdSetting;
	theBSD.GetPara(&stBsdSetting);

	m_sDebugInfo.Empty();
	m_sDebugInfo.AppendFormat(_T("NSOP Dac=%d\r\n"), stBsdSetting.NsopDac);
	m_sDebugInfo.AppendFormat(_T("NSOP Sample=%d\r\n"), stBsdSetting.NsopSample);
	m_sDebugInfo.AppendFormat(_T("NSOL Dac=%d\r\n"), stBsdSetting.NsolDac);
	m_sDebugInfo.AppendFormat(_T("NSOL Sample=%d\r\n"),stBsdSetting.NsolSample);
	m_sDebugInfo.AppendFormat(_T("Tail Short Sample=%d\r\n"), stBsdSetting.TailShortSample);

	UpdateData(FALSE);
}

void CUsbBsdDebugPage::OnBsdReset()
{
	CBusy bs(&m_bBusy);		//V1.4.W38 add

	theBSD.ResetBsd();
    theBSD.ClearWireFeedCount();	//v1.6.T159 edit
	//theBondStickDetect.szReultInfo.Empty();
	m_sDebugInfo.Empty();
	UpdateData(FALSE);
}

void CUsbBsdDebugPage::OnBsdResult()
{
	CBusy bs(&m_bBusy);		//V1.4.W38 add

	byte nStickResult = 0xff;
	WORD nNsopCount = 0xffff, nNsolCount = 0xffff, nWireFeedCount = 0xffff;

	theBSD.GetStickResult(&nStickResult);
	theBSD.GetStickCount(&nNsopCount, &nNsolCount);
	theBSD.GetWireFeedCount(&nWireFeedCount);	//v1.6.T159 edit

	m_sDebugInfo.AppendFormat(_T("BSD Result=%d\r\n"), nStickResult);
	m_sDebugInfo.AppendFormat(_T("NSOP Result=%d\r\n"), (BOOL)(nStickResult & RESULT_NSOP));
	m_sDebugInfo.AppendFormat(_T("NSOL Result=%d\r\n"), (BOOL)((nStickResult & RESULT_NSOL) >> 1));
	m_sDebugInfo.AppendFormat(_T("NSOP Count=%d\r\n"), nNsopCount);
	m_sDebugInfo.AppendFormat(_T("NSOL Count=%d\r\n"), nNsolCount);
	m_sDebugInfo.AppendFormat(_T("WireFeed Count=%d\r\n"), nWireFeedCount);

	UpdateData(FALSE);
}

void CUsbBsdDebugPage::OnFocusDac1()
{
	GetDlgItem(IDC_STATIC_BSD_VERSION_AREA)->SetFocus();
	theKeyPad.GetUserKeyInData((short*)&m_nDac1, 0, 0, 255);
	UpdateData(false);
	OnEditDac1();
}

void CUsbBsdDebugPage::OnFocusDac2()
{
	GetDlgItem(IDC_STATIC_BSD_VERSION_AREA)->SetFocus();
	theKeyPad.GetUserKeyInData((short*)&m_nDac2, 255, 0, 255);
	UpdateData(false);
	OnEditDac2();
}

void CUsbBsdDebugPage::OnEditDac1()	//nsop threshold
{
	UpdateData(TRUE);
	theBSD.SetDualDacValue(m_nDac1, m_nDac2);
}

void CUsbBsdDebugPage::OnEditDac2()	//apply voltage
{
	UpdateData(TRUE);
	theBSD.SetDualDacValue(m_nDac1, m_nDac2);
}

void CUsbBsdDebugPage::OnBsdDebugMode()
{
	UpdateData(TRUE);
}

LRESULT CUsbBsdDebugPage::CloseDialog(WPARAM wParam, LPARAM lParam)	//V1.4.W38 add
{
	return ((m_bBusy == FALSE) && DestroyWindow()) * DIALOG_CLOSED;
}
