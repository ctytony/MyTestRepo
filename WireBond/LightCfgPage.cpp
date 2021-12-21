// LightCfgPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LightCfgPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MachineData.h"
#include "VisionSystem.h"

// CLightCfgPage dialog
IMPLEMENT_DYNAMIC(CLightCfgPage, CPropertyPage)

CLightCfgPage::CLightCfgPage()
	: CPropertyPage(CLightCfgPage::IDD)
    , m_Die_Coxblue(70)
    , m_Die_Coxred(70)
    , m_Die_Sideblue(70)
    , m_Die_Sidered(70)
    , m_Die_black(120)
    , m_Lead_Coxblue(70)
    , m_Lead_Coxred(70)
    , m_Lead_Sideblue(70)
    , m_Lead_Sidered(70)
    , m_Lead_black(120)
	, m_nCoaxRed(0)
	, m_nCoaxBlue(0)
	, m_nSideRed(0)
	, m_nSideBlue(0)
{
}

CLightCfgPage::~CLightCfgPage()
{
}

void CLightCfgPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIE_COBLUE, m_Die_Coxblue);
	DDX_Text(pDX, IDC_EDIT_DIE_CORED, m_Die_Coxred);
	DDX_Text(pDX, IDC_EDIT_DIE_SIDEBLUE, m_Die_Sideblue);
	DDX_Text(pDX, IDC_EDIT_DIE_SIDERED, m_Die_Sidered);
	DDX_Text(pDX, IDC_EDIT_DIE_BLACK, m_Die_black);
	DDX_Text(pDX, IDC_EDIT_LEAD_COBLUE, m_Lead_Coxblue);
	DDX_Text(pDX, IDC_EDIT_LEAD_CORED, m_Lead_Coxred);
	DDX_Text(pDX, IDC_EDIT_LEAD_SIDEBLUE, m_Lead_Sideblue);
	DDX_Text(pDX, IDC_EDIT_LEAD_SIDERED, m_Lead_Sidered);
	DDX_Text(pDX, IDC_EDIT_LEAD_BLACK, m_Lead_black);
	DDX_Text(pDX, IDC_EDIT_COAX_RED, m_nCoaxRed);
	DDX_Text(pDX, IDC_EDIT_COAX_BLUE, m_nCoaxBlue);
	DDX_Text(pDX, IDC_EDIT_SIDE_RED, m_nSideRed);
	DDX_Text(pDX, IDC_EDIT_SIDE_BLUE, m_nSideBlue);
}

BEGIN_MESSAGE_MAP(CLightCfgPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_DIE_COBLUE, OnFocusDieCoblue)
    ON_EN_SETFOCUS(IDC_EDIT_DIE_CORED, OnFocusDieCored)
    ON_EN_SETFOCUS(IDC_EDIT_DIE_SIDEBLUE, OnFocusDieSideblue)
    ON_EN_SETFOCUS(IDC_EDIT_DIE_SIDERED, OnFocusDieSidered)
    ON_EN_SETFOCUS(IDC_EDIT_DIE_BLACK, OnFocusDieBlack)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_COBLUE, OnFocusLeadCoblue)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_CORED, OnFocusLeadCored)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_SIDEBLUE, OnFocusLeadSideblue)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_SIDERED, OnFocusLeadSidered)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_BLACK, OnFocusLeadBlack)
	ON_EN_SETFOCUS(IDC_EDIT_COAX_RED, OnFocusCoaxRed)
	ON_EN_SETFOCUS(IDC_EDIT_COAX_BLUE, OnFocusCoaxBlue)
	ON_EN_SETFOCUS(IDC_EDIT_SIDE_RED, OnFocusSideRed)
	ON_EN_SETFOCUS(IDC_EDIT_SIDE_BLUE, OnFocusSideBlue)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CLightCfgPage message handlers
BOOL CLightCfgPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_LIGHT_DEFAULT, theMsg.GetMsg(MSG_TEACH_RECIPE));
	SetDlgItemTextW(IDC_STATIC_DIE_LIGHT, theMsg.GetMsg(MSG_LEAD));
	SetDlgItemTextW(IDC_STATIC_LEAD_LIGHT, theMsg.GetMsg(MSG_DIE));
	SetDlgItemTextW(IDC_STATIC_CO_BLUE, theMsg.GetMsg(MSG_BLUE_COAX));
	SetDlgItemTextW(IDC_STATIC_CO_RED, theMsg.GetMsg(MSG_RED_COAX));
	SetDlgItemTextW(IDC_STATIC_SIDE_BLUE, theMsg.GetMsg(MSG_BLUE_SIDE));
	SetDlgItemTextW(IDC_STATIC_SIDE_RED, theMsg.GetMsg(MSG_RED_SIDE));
	SetDlgItemTextW(IDC_STATIC_BLACK_THRESH, theMsg.GetMsg(MSG_THRESHOLD_2));
	SetDlgItemTextW(IDC_STATIC_IDLE, theMsg.GetMsg(MSG_THREAD_WIRE));
	SetDlgItemTextW(IDC_STATIC_RED, theMsg.GetMsg(MSG_RED));
	SetDlgItemTextW(IDC_STATIC_BLUE, theMsg.GetMsg(MSG_BLUE));
	SetDlgItemTextW(IDC_STATIC_COAX, theMsg.GetMsg(MSG_COAX));
	SetDlgItemTextW(IDC_STATIC_SIDE, theMsg.GetMsg(MSG_SIDE));

	MACHINE_DATA_LIGHT* pstLight = theMcData.GetLightRcd();

	m_Die_Coxblue   = pstLight->DieLightRcd.nCoaxBlueLight;
	m_Die_Coxred    = pstLight->DieLightRcd.nCoaxRedLight;
	m_Die_Sideblue  = pstLight->DieLightRcd.bSideBlueLight;
	m_Die_Sidered   = pstLight->DieLightRcd.bSideRedLight;
	m_Die_black     = pstLight->DieLightRcd.nImageThreshold;
	m_Lead_Coxblue  = pstLight->LeadLightRcd.nCoaxBlueLight;
	m_Lead_Coxred   = pstLight->LeadLightRcd.nCoaxRedLight;
	m_Lead_Sideblue = pstLight->LeadLightRcd.bSideBlueLight;
	m_Lead_Sidered  = pstLight->LeadLightRcd.bSideRedLight;
	m_Lead_black    = pstLight->LeadLightRcd.nImageThreshold;
	m_nCoaxRed      = pstLight->stIdleLight.nCoaxRedLight;
	m_nCoaxBlue     = pstLight->stIdleLight.nCoaxBlueLight;
	m_nSideRed      = pstLight->stIdleLight.bSideRedLight;
	m_nSideBlue     = pstLight->stIdleLight.bSideBlueLight;

	theVisionSys.UpdateLightingParameter(m_nCoaxRed, m_nSideRed, m_nCoaxBlue, m_nSideBlue);

	return CPropertyPage::OnInitDialog();
}

HBRUSH CLightCfgPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
       pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLightCfgPage::OnDestroy()		//V1.1.W259 edit
{
	CPropertyPage::OnDestroy();

	MACHINE_DATA_LIGHT* pstLight = theMcData.GetLightRcd();
	UpdateData(true);

	pstLight->DieLightRcd.nCoaxBlueLight  = m_Die_Coxblue;
	pstLight->DieLightRcd.nCoaxRedLight   = m_Die_Coxred;
	pstLight->DieLightRcd.bSideBlueLight  = (BYTE)m_Die_Sideblue;
	pstLight->DieLightRcd.bSideRedLight   = (BYTE)m_Die_Sidered;
	pstLight->DieLightRcd.nImageThreshold = m_Die_black;

	pstLight->LeadLightRcd.nCoaxBlueLight  = m_Lead_Coxblue;
	pstLight->LeadLightRcd.nCoaxRedLight   = m_Lead_Coxred;
	pstLight->LeadLightRcd.bSideBlueLight  = (BYTE)m_Lead_Sideblue;
	pstLight->LeadLightRcd.bSideRedLight   = (BYTE)m_Lead_Sidered;
	pstLight->LeadLightRcd.nImageThreshold = m_Lead_black;

	pstLight->stIdleLight.nCoaxRedLight  = m_nCoaxRed;
	pstLight->stIdleLight.nCoaxBlueLight = m_nCoaxBlue;
	pstLight->stIdleLight.bSideRedLight  = (BYTE)m_nSideRed;
	pstLight->stIdleLight.bSideBlueLight = (BYTE)m_nSideBlue;
}

void CLightCfgPage::OnFocusDieCoblue()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Die_Coxblue, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusDieCored()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Die_Coxred, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusDieSideblue()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Die_Sideblue, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusDieSidered()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Die_Sidered, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusDieBlack()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Die_black, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusLeadCoblue()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Lead_Coxblue, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusLeadCored()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Lead_Coxred, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusLeadSideblue()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Lead_Sideblue, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusLeadSidered()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Lead_Sidered, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusLeadBlack()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_Lead_black, 0, 0, 255);
	UpdateData(false);
}

void CLightCfgPage::OnFocusCoaxRed()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCoaxRed, 0, 0, 255);
	UpdateData(false);

	theVisionSys.UpdateLightingParameter(m_nCoaxRed, m_nSideRed, m_nCoaxBlue, m_nSideBlue);
}

void CLightCfgPage::OnFocusCoaxBlue()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCoaxBlue, 0, 0, 255);
	UpdateData(false);

	theVisionSys.UpdateLightingParameter(m_nCoaxRed, m_nSideRed, m_nCoaxBlue, m_nSideBlue);
}

void CLightCfgPage::OnFocusSideRed()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSideRed, 0, 0, 255);
	UpdateData(false);

	theVisionSys.UpdateLightingParameter(m_nCoaxRed, m_nSideRed, m_nCoaxBlue, m_nSideBlue);
}

void CLightCfgPage::OnFocusSideBlue()
{
	GetDlgItem(IDC_STATIC_LIGHT_DEFAULT)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSideBlue, 0, 0, 255);
	UpdateData(false);

	theVisionSys.UpdateLightingParameter(m_nCoaxRed, m_nSideRed, m_nCoaxBlue, m_nSideBlue);
}
