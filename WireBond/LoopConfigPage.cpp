/***************************************************************
LoopConfigPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "BondPara.h"
#include "LoopPara.h"
#include "LoopConfigPage.h"

// CLoopConfigPage dialog
IMPLEMENT_DYNAMIC(CLoopConfigPage, CPropertyPage)

CLoopConfigPage::CLoopConfigPage()
	: CPropertyPage(CLoopConfigPage::IDD)
    , m_nRevHtPremove(0)
    , m_nRevDistPremove(0)
    , m_nKink2HtPremove(0)
    , m_nKink2DistPremove(0)
    , m_nLoopTopPremove(0)
    , m_nWclTailClosePretrig(0)
    , m_nWclLoopPeakClosePretrig(0)
    , m_nWcl1stSrchHtPretrig(0)
    , m_nWcl2ndSrchHtPretrig(0)
    , m_nKink3HtPremove(0)
    , m_nTwistHtPremove(0)
    , m_nKink3DistPremove(0)
    , m_nTwistFwdDistPremove(0)
{
}

CLoopConfigPage::~CLoopConfigPage()
{
}

void CLoopConfigPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_NECK_HEIGHT_PRETRIG, m_nRevHtPremove);
    DDX_Text(pDX, IDC_EDIT_NECK_DIST_PRETRIG, m_nRevDistPremove);
    DDX_Text(pDX, IDC_EDIT_REV_HEIGHT2_PRETRIG, m_nKink2HtPremove);
    DDX_Text(pDX, IDC_EDIT_REV_DIST2_PRETRIG, m_nKink2DistPremove);
    DDX_Text(pDX, IDC_EDIT_LOOP_TOP_PRETRIG, m_nLoopTopPremove);
    DDX_Text(pDX, IDC_EDIT_TAIL_HEIGHT_WCL_PRETRIG, m_nWclTailClosePretrig);
    DDX_Text(pDX, IDC_EDIT_LOOP_PEAK_WCL_PRETRIG, m_nWclLoopPeakClosePretrig);
    DDX_Text(pDX, IDC_EDIT_1ST_SRCH_WCL_PRETRIG, m_nWcl1stSrchHtPretrig);
    DDX_Text(pDX, IDC_EDIT_2ND_SRCH_WCL_PRETRIG, m_nWcl2ndSrchHtPretrig);
    DDX_Text(pDX, IDC_EDIT_REV_HEIGHT3_PRETRIG, m_nKink3HtPremove);
    DDX_Text(pDX, IDC_EDIT_REV_HEIGHT4_PRETRIG, m_nTwistHtPremove);
    DDX_Text(pDX, IDC_EDIT_REV_DIST3_PRETRIG, m_nKink3DistPremove);
    DDX_Text(pDX, IDC_EDIT_TWIST_PRETRIG, m_nTwistFwdDistPremove);
}

BEGIN_MESSAGE_MAP(CLoopConfigPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_NECK_HEIGHT_PRETRIG, OnFocusNeckHeightPretrig)
    ON_EN_SETFOCUS(IDC_EDIT_NECK_DIST_PRETRIG, OnFocusNeckDistPretrig)
    ON_EN_SETFOCUS(IDC_EDIT_REV_HEIGHT2_PRETRIG, OnFocusRevHeight2Pretrig)
    ON_EN_SETFOCUS(IDC_EDIT_REV_DIST2_PRETRIG, OnFocusRevDist2Pretrig)
    ON_EN_SETFOCUS(IDC_EDIT_LOOP_TOP_PRETRIG, OnFocusLoopTopPretrig)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_HEIGHT_WCL_PRETRIG, OnFocusTailHeightWclPretrig)
    ON_EN_SETFOCUS(IDC_EDIT_LOOP_PEAK_WCL_PRETRIG, OnFocusLoopPeakWclPretrig)
    ON_EN_SETFOCUS(IDC_EDIT_1ST_SRCH_WCL_PRETRIG, OnFocus1stSrchWclPretrig)
    ON_EN_SETFOCUS(IDC_EDIT_2ND_SRCH_WCL_PRETRIG, OnFocus2ndSrchWclPretrig)
    ON_EN_SETFOCUS(IDC_EDIT_REV_HEIGHT3_PRETRIG, OnFocusRevHeight3Pretrig)
    ON_EN_SETFOCUS(IDC_EDIT_REV_DIST3_PRETRIG, OnFocusRevDist3Pretrig)
    ON_EN_SETFOCUS(IDC_EDIT_REV_HEIGHT4_PRETRIG, OnFocusRevHeight4Pretrig)
    ON_EN_SETFOCUS(IDC_EDIT_TWIST_PRETRIG, OnFocusTwistPretrig)
	ON_WM_DESTROY()		//V1.4.W25	add
END_MESSAGE_MAP()

// CLoopConfigPage message handlers
BOOL CLoopConfigPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    int iRowTop = 20;
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_NECK_HEIGHT_PRETRIG,theMsg.GetMsg(MSG_NECK_HT_PREMOVE));
    GetDlgItem(IDC_EDIT_NECK_HEIGHT_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_NECK_DIST_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_NECK_DIST_PRETRIG,theMsg.GetMsg(MSG_NECK_DIST_PREMOVE));
    GetDlgItem(IDC_EDIT_NECK_DIST_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_REVERSE_HEIGHT2_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_REVERSE_HEIGHT2_PRETRIG,theMsg.GetMsg(MSG_REV_HT2_PREMOVE));
    GetDlgItem(IDC_EDIT_REV_HEIGHT2_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_REVERSE_DIST2_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_REVERSE_DIST2_PRETRIG,theMsg.GetMsg(MSG_REV_DIST2_PREMOVE));
    GetDlgItem(IDC_EDIT_REV_DIST2_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_REVERSE_HEIGHT3_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_REVERSE_HEIGHT3_PRETRIG,theMsg.GetMsg(MSG_REV_HT3_PREMOVE));
    GetDlgItem(IDC_EDIT_REV_HEIGHT3_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_REVERSE_DIST3_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_REVERSE_DIST3_PRETRIG,theMsg.GetMsg(MSG_REV_DIST3_PREMOVE));
    GetDlgItem(IDC_EDIT_REV_DIST3_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	//v3.1.T187 delete
    //iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    //GetDlgItem(IDC_STATIC_REVERSE_HEIGHT4_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    //SetDlgItemTextW(IDC_STATIC_REVERSE_HEIGHT4_PRETRIG,theMsg.GetMsg(MSG_REV_HT4));
    //GetDlgItem(IDC_EDIT_REV_HEIGHT4_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	//v3.1.T187 add
	GetDlgItem(IDC_STATIC_REVERSE_HEIGHT4_PRETRIG)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_EDIT_REV_HEIGHT4_PRETRIG)->ShowWindow(SW_HIDE);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_TWIST_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_TWIST_PRETRIG,theMsg.GetMsg(MSG_TWIST_PREMOVE));
    GetDlgItem(IDC_EDIT_TWIST_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_LOOP_TOP_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_LOOP_TOP_PRETRIG,theMsg.GetMsg(MSG_LOOP_TOP_PREMOVE));
    GetDlgItem(IDC_EDIT_LOOP_TOP_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_LOOP_PEAK_WCL_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH + 15, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_LOOP_PEAK_WCL_PRETRIG,theMsg.GetMsg(MSG_LOOP_PEAK_CLOSE));
    GetDlgItem(IDC_EDIT_LOOP_PEAK_WCL_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_TAIL_HEIGHT_WCL_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_TAIL_HEIGHT_WCL_PRETRIG,theMsg.GetMsg(MSG_TAIL_CLOSE));
    GetDlgItem(IDC_EDIT_TAIL_HEIGHT_WCL_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_1ST_SRCH_WCL_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_1ST_SRCH_WCL_PRETRIG,theMsg.GetMsg(MSG_1ST_BOND_OPEN));
    GetDlgItem(IDC_EDIT_1ST_SRCH_WCL_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
    GetDlgItem(IDC_STATIC_2ND_SRCH_WCL_PRETRIG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_2ND_SRCH_WCL_PRETRIG,theMsg.GetMsg(MSG_2ND_BOND_OPEN));
    GetDlgItem(IDC_EDIT_2ND_SRCH_WCL_PRETRIG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	ST_LOOP_CONFIG_PARA stLoopConfig;
	theLoopPara.GetConfigRcd(&stLoopConfig);
	m_nRevHtPremove        = stLoopConfig.nNeckHtPremove;
	m_nRevDistPremove      = stLoopConfig.nNeckDistPremove;
	m_nKink2HtPremove      = stLoopConfig.nKink2HtPremove;
	m_nKink2DistPremove    = stLoopConfig.nKink2DistPremove;
	m_nKink3HtPremove      = stLoopConfig.nKink3HtPremove;
	m_nKink3DistPremove    = stLoopConfig.nKink3DistPremove;
	m_nTwistHtPremove      = stLoopConfig.nTwistHtPremove;
	m_nTwistFwdDistPremove = stLoopConfig.nTwistFwdDistPremove;
	m_nLoopTopPremove      = stLoopConfig.nLoopTopPremove;

	BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
	m_nWclTailClosePretrig     = pstBondPara->nWclTailClosePretrig;
	m_nWclLoopPeakClosePretrig = pstBondPara->nWclLoopPeakClosePretrig;
	m_nWcl1stSrchHtPretrig     = pstBondPara->nWcl1stBondOpenPretrig;
	m_nWcl2ndSrchHtPretrig     = pstBondPara->nWcl2ndBondOpenPretrig;

	return CPropertyPage::OnInitDialog();
}

HBRUSH CLoopConfigPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLoopConfigPage::OnDestroy()
{
	UpdateData();

	ST_LOOP_CONFIG_PARA stLoopConfig;
	theLoopPara.GetConfigRcd(&stLoopConfig);
	stLoopConfig.nNeckHtPremove       = m_nRevHtPremove;
	stLoopConfig.nNeckDistPremove     = m_nRevDistPremove;
	stLoopConfig.nKink2HtPremove      = m_nKink2HtPremove;
	stLoopConfig.nKink2DistPremove    = m_nKink2DistPremove;
	stLoopConfig.nLoopTopPremove      = m_nLoopTopPremove;
	stLoopConfig.nKink3HtPremove      = m_nKink3HtPremove;
	stLoopConfig.nKink3DistPremove    = m_nKink3DistPremove;
	stLoopConfig.nTwistHtPremove      = m_nTwistHtPremove;
	stLoopConfig.nTwistFwdDistPremove = m_nTwistFwdDistPremove;
	theLoopPara.SetConfigRcd(&stLoopConfig);

	BOND_PARA_RCD* pstBondPara = theBondPara.GetRcd();
	pstBondPara->nWclTailClosePretrig     = m_nWclTailClosePretrig;
	pstBondPara->nWclLoopPeakClosePretrig = m_nWclLoopPeakClosePretrig;
	pstBondPara->nWcl1stBondOpenPretrig   = m_nWcl1stSrchHtPretrig;
	pstBondPara->nWcl2ndBondOpenPretrig   = m_nWcl2ndSrchHtPretrig;

	theApp.ArchivePowerOnData(ARCHIVE_SAVE);

	CPropertyPage::OnDestroy();
}

void CLoopConfigPage::OnFocusNeckHeightPretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nRevHtPremove, 0, -500, 100);	//v3.1.T185 edit
    UpdateData(false);
}

void CLoopConfigPage::OnFocusNeckDistPretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nRevDistPremove, 0, -500, 100);	//v3.1.T185 edit
    UpdateData(false);
}

void CLoopConfigPage::OnFocusRevHeight2Pretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nKink2HtPremove, 0, -500, 100);	//v3.1.T185 edit
    UpdateData(false);
}

void CLoopConfigPage::OnFocusRevDist2Pretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nKink2DistPremove, 0, -500, 100);	//v3.1.T185 edit
    UpdateData(false);
}

void CLoopConfigPage::OnFocusLoopTopPretrig()
{
	GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
	//theKeyPad.GetUserKeyInData(&m_nLoopTopPremove, 0, 0, 100);	//v3.1.T395 delete
	theKeyPad.GetUserKeyInData(&m_nLoopTopPremove, 0, -500, 100);	//v3.1.T395 add
	UpdateData(false);
}

void CLoopConfigPage::OnFocusTailHeightWclPretrig()
{
	GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T399 add
	theKeyPad.GetUserKeyInData(&m_nWclTailClosePretrig, 1, 0, 8);
#else
	theKeyPad.GetUserKeyInData(&m_nWclTailClosePretrig, 1, -10, 8);
#endif
	UpdateData(false);
}

void CLoopConfigPage::OnFocusLoopPeakWclPretrig()
{
	GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T399 add
	theKeyPad.GetUserKeyInData(&m_nWclLoopPeakClosePretrig, 1, 0, 10);
#else
	theKeyPad.GetUserKeyInData(&m_nWclLoopPeakClosePretrig, 1, -4, 10);
#endif
	UpdateData(false);
}

void CLoopConfigPage::OnFocus1stSrchWclPretrig()
{
	GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nWcl1stSrchHtPretrig, 4, 1, 16);
	UpdateData(false);
}

void CLoopConfigPage::OnFocus2ndSrchWclPretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
#if _SERVO_TYPE == 2	//v3.1.T399 add
    theKeyPad.GetUserKeyInData(&m_nWcl2ndSrchHtPretrig, 3, 0, 16);
#else
	theKeyPad.GetUserKeyInData(&m_nWcl2ndSrchHtPretrig, 3, -1, 16);
#endif
	UpdateData(false);
}

void CLoopConfigPage::OnFocusRevHeight3Pretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nKink3HtPremove, 0, -500, 100);	//v3.1.T187 edit
    UpdateData(false);
}

void CLoopConfigPage::OnFocusRevDist3Pretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nKink3DistPremove, 0, -500, 100);	//v3.1.T187 edit
    UpdateData(false);
}

void CLoopConfigPage::OnFocusRevHeight4Pretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTwistHtPremove, 0, 0, 100);
    UpdateData(false);
}

void CLoopConfigPage::OnFocusTwistPretrig()
{
    GetDlgItem(IDC_STATIC_NECK_HEIGHT_PRETRIG)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTwistFwdDistPremove, 0, -500, 100);	//v3.1.T187 edit
    UpdateData(false);
}
