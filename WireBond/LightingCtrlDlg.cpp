/***************************************************************
LightingCtrlDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LightingCtrlDlg.h"
#include "VisionSystem.h"

extern CJpeg theJpegHelp;

// CLightingCtrlDlg dialog
IMPLEMENT_DYNAMIC(CLightingCtrlDlg, CDialog)

CLightingCtrlDlg::CLightingCtrlDlg(bool bModeless, CWnd* pParent /*=NULL*/)
	: CDialog(CLightingCtrlDlg::IDD, pParent)
	, m_nCoaxRedLight(70)
	, m_nCoaxBlueLight(70)
	, m_nSideRedLight(70)
	, m_nSideBlueLight(70)
	, m_nImageThres(100)
	, m_nBinaryImage(0)
	, m_bModeless(bModeless)
{
	m_pbrRed = new CBrush(RGB(255,32,32));
	m_pbrBlue = new CBrush(RGB(32,32,255));
	m_pbrGreen = new CBrush(RGB(32,255,32));
}

CLightingCtrlDlg::~CLightingCtrlDlg()
{
	delete m_pbrRed;
	delete m_pbrBlue;
	delete m_pbrGreen;
}

void CLightingCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COAX_RED, m_nCoaxRedLight);
	DDX_Text(pDX, IDC_EDIT_COAX_BLUE, m_nCoaxBlueLight);
	DDX_Text(pDX, IDC_EDIT_SIDE_RED, m_nSideRedLight);
	DDX_Text(pDX, IDC_EDIT_SIDE_BLUE, m_nSideBlueLight);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_nImageThres);
	DDX_Control(pDX, IDC_SLIDER_COAX_RED, m_CoaxRedSlider);
	DDX_Control(pDX, IDC_SLIDER_COAX_BLUE, m_CoaxBlueSlider);
	DDX_Control(pDX, IDC_SLIDER_SIDE_RED, m_SideRedSlider);
	DDX_Control(pDX, IDC_SLIDER_SIDE_BLUE, m_SideBlueSlider);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_ImageThresSlider);
}

BEGIN_MESSAGE_MAP(CLightingCtrlDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_EN_CHANGE(IDC_EDIT_COAX_RED, OnEditCoaxRed)
	ON_EN_CHANGE(IDC_EDIT_COAX_BLUE, OnEditCoaxBlue)
	ON_EN_CHANGE(IDC_EDIT_SIDE_RED, OnEditSideRed)
	ON_EN_CHANGE(IDC_EDIT_SIDE_BLUE, OnEditSideBlue)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, OnEditThreshold)
	ON_BN_CLICKED(IDC_BUTTON_BINARY, OnBinary)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_COAX_RED, OnFocusCoaxRed)
	ON_EN_SETFOCUS(IDC_EDIT_COAX_BLUE, OnFocusCoaxBlue)
	ON_EN_SETFOCUS(IDC_EDIT_SIDE_RED, OnFocusSideRed)
	ON_EN_SETFOCUS(IDC_EDIT_SIDE_BLUE, OnFocusSideBlue)
	ON_EN_SETFOCUS(IDC_EDIT_THRESHOLD, OnFocusThreshold)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_THRESHOLD, OnAutoThreshold)
	ON_WM_VSCROLL()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CLightingCtrlDlg message handlers
BOOL CLightingCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.SetDlgFont(this);	//V1.1.W139 add
	SetWindowPos(NULL, LIGHT_DLG_LEFT, LIGHT_DLG_TOP, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	UpdateDisp();
	return FALSE;
}

void CLightingCtrlDlg::OnOk()	//V1.1.W152 edit
{
	theVisionSys.EnableVideo(true);

	if (m_bModeless)
		ShowWindow(SW_HIDE);
	else
		CDialog::OnOK();
}

void CLightingCtrlDlg::OnEditCoaxRed()
{
	UpdateData(TRUE);
	m_CoaxRedSlider.SetPos(m_nCoaxRedLight);
	theVisionSys.UpdateLightingParameter(m_nCoaxRedLight, m_nSideRedLight, m_nCoaxBlueLight, m_nSideBlueLight);
}

void CLightingCtrlDlg::OnEditCoaxBlue()
{
	UpdateData(TRUE);
	m_CoaxBlueSlider.SetPos(m_nCoaxBlueLight);
	theVisionSys.UpdateLightingParameter(m_nCoaxRedLight, m_nSideRedLight, m_nCoaxBlueLight, m_nSideBlueLight);
}

void CLightingCtrlDlg::OnEditSideRed()
{
	UpdateData(TRUE);
	m_SideRedSlider.SetPos(m_nSideRedLight);
	theVisionSys.UpdateLightingParameter(m_nCoaxRedLight, m_nSideRedLight, m_nCoaxBlueLight, m_nSideBlueLight);
}

void CLightingCtrlDlg::OnEditSideBlue()
{
	UpdateData(TRUE);
	m_SideBlueSlider.SetPos(m_nSideBlueLight);
	theVisionSys.UpdateLightingParameter(m_nCoaxRedLight, m_nSideRedLight, m_nCoaxBlueLight, m_nSideBlueLight);
}

void CLightingCtrlDlg::OnEditThreshold()
{
	if (m_nBinaryImage == 1)
	{
		UpdateData(TRUE);
		m_ImageThresSlider.SetPos(m_nImageThres);
		theVisionSys.GrabImage();
		theVisionSys.UpdateBinaryThreshold(m_nImageThres);
	}
	else
	{
		UpdateData(TRUE);
		OnBinary();
	}
}

void CLightingCtrlDlg::OnBinary()
{
	theVisionSys.EnableVideo(m_nBinaryImage==1);

	if (m_nBinaryImage == 0)//Binary image
	{
		theVisionSys.GrabImage();
		theVisionSys.UpdateBinaryThreshold(m_nImageThres);
		m_ImageThresSlider.SetPos(m_nImageThres);
		UpdateData(FALSE);
	}

	m_nBinaryImage = !m_nBinaryImage;
	m_ImageThresSlider.EnableWindow(m_nBinaryImage);

	SetDlgItemTextW(IDC_BUTTON_BINARY, theMsg.GetMsg(m_nBinaryImage == 0 ? MSG_GREY : MSG_BINARY));
}

HBRUSH CLightingCtrlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	int nID = pWnd->GetDlgCtrlID();

	if ((nID == IDC_SLIDER_COAX_RED) || (nID == IDC_SLIDER_SIDE_RED)) return *m_pbrRed;
	else if ((nID == IDC_SLIDER_COAX_BLUE) || (nID == IDC_SLIDER_SIDE_BLUE)) return *m_pbrBlue;
	else if (nID == IDC_SLIDER_THRESHOLD) return *m_pbrGreen;
	else if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkMode(TRANSPARENT);
		return theApp.m_brNull;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLightingCtrlDlg::OnFocusCoaxRed()
{
	GetDlgItem(IDOK)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCoaxRedLight, 0, 0, 255);
	UpdateData(false);
	OnEditCoaxRed();
}

void CLightingCtrlDlg::OnFocusCoaxBlue()
{
	GetDlgItem(IDOK)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCoaxBlueLight, 0, 0, 255);
	UpdateData(false);
	OnEditCoaxBlue();
}

void CLightingCtrlDlg::OnFocusSideRed()
{
	GetDlgItem(IDOK)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSideRedLight, 0, 0, 255);
	UpdateData(false);
	OnEditSideRed();
}

void CLightingCtrlDlg::OnFocusSideBlue()
{
	GetDlgItem(IDOK)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSideBlueLight, 0, 0, 255);
	UpdateData(false);
	OnEditSideBlue();
}

void CLightingCtrlDlg::OnFocusThreshold()
{
	GetDlgItem(IDOK)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nImageThres, 0, 0, 255);
	UpdateData(false);
	OnEditThreshold();
}

void CLightingCtrlDlg::OnAutoThreshold()
{
	if (m_nBinaryImage != 1) return;

	//Binary image
	theVisionSys.EnableVideo(false);
	theVisionSys.GrabImage();
	theVisionSys.BinaryAutoThreshold(&m_nImageThres);
	theVisionSys.UpdateBinaryThreshold(m_nImageThres);
	m_ImageThresSlider.SetPos(m_nImageThres);
	UpdateData(FALSE);

	m_ImageThresSlider.EnableWindow(true);
}

void CLightingCtrlDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Timer.StartTimer();

	if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_COAX_RED)->m_hWnd)
	{
		m_nCoaxRedLight = m_CoaxRedSlider.GetPos();
		SetDlgItemInt(IDC_EDIT_COAX_RED, m_nCoaxRedLight);
	}
	else if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_COAX_BLUE)->m_hWnd)
	{
		m_nCoaxBlueLight = m_CoaxBlueSlider.GetPos();
		SetDlgItemInt(IDC_EDIT_COAX_BLUE, m_nCoaxBlueLight);
	}
	else if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_SIDE_RED)->m_hWnd)
	{
		m_nSideRedLight = m_SideRedSlider.GetPos();
		SetDlgItemInt(IDC_EDIT_SIDE_RED, m_nSideRedLight);
	}
	else if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_SIDE_BLUE)->m_hWnd)
	{
		m_nSideBlueLight = m_SideBlueSlider.GetPos();
		SetDlgItemInt(IDC_EDIT_SIDE_BLUE, m_nSideBlueLight);
	}
	else if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_THRESHOLD)->m_hWnd)
	{
		m_nImageThres = m_ImageThresSlider.GetPos();
		SetDlgItemInt(IDC_EDIT_THRESHOLD, m_nImageThres);
	}

	CString str;
	str.Format(_T("%0.3f"), m_Timer.GetTime_x1ms());
	SetDlgItemText(IDC_STC_TIME, str);

	CRect rc;
	GetDlgItem(IDC_STC_TIME)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(&rc);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLightingCtrlDlg::UpdateDisp()
{
	theVisionSys.GetLightingParameter(&m_nCoaxRedLight, &m_nSideRedLight, &m_nCoaxBlueLight, &m_nSideBlueLight);
	theVisionSys.GetBinaryThreshold(&m_nImageThres);

	m_CoaxRedSlider.SetRange(0,255);
	m_CoaxBlueSlider.SetRange(0,255);
	m_SideRedSlider.SetRange(0,255);
	m_SideBlueSlider.SetRange(0,255);
	m_ImageThresSlider.SetRange(0,255);

	m_CoaxRedSlider.SetPos(m_nCoaxRedLight);
	m_CoaxBlueSlider.SetPos(m_nCoaxBlueLight);
	m_SideRedSlider.SetPos(m_nSideRedLight);
	m_SideBlueSlider.SetPos(m_nSideBlueLight);
	m_ImageThresSlider.SetPos(m_nImageThres);
	m_ImageThresSlider.EnableWindow(false);

	SetDlgItemText(IDC_BUTTON_BINARY, theMsg.GetMsg(MSG_BINARY));
	SetDlgItemText(IDC_BUTTON_AUTO_THRESHOLD, theMsg.GetMsg(MSG_AUTO));
	SetDlgItemText(IDOK, theMsg.GetMsg(MSG_DONE));
	SetDlgItemText(IDC_STATIC_COAX, theMsg.GetMsg(MSG_COAX));
	SetDlgItemText(IDC_STATIC_COAX_RED, theMsg.GetMsg(MSG_RED));
	SetDlgItemText(IDC_STATIC_COAX_BLUE, theMsg.GetMsg(MSG_BLUE));
	SetDlgItemText(IDC_STATIC_SIDE, theMsg.GetMsg(MSG_SIDE));
	SetDlgItemText(IDC_STATIC_SIDE_RED, theMsg.GetMsg(MSG_RED));
	SetDlgItemText(IDC_STATIC_SIDE_BLUE,theMsg.GetMsg(MSG_BLUE) );
	SetDlgItemText(IDC_STATIC_THRESHOLD, theMsg.GetMsg(MSG_THRESHOLD));

	UpdateData(false);
}

LRESULT CLightingCtrlDlg::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}

BOOL CLightingCtrlDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	theJpegHelp.Paint(pDC->m_hDC, rc);
	return TRUE;
}
