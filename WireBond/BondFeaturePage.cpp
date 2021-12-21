/***************************************************************
BondFeaturePage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "Program.h"
#include "Reference.h"
#include "BondPara.h"
#include "LoopPara.h"
#include "BondFeaturePage.h"
#include "Bond.h"
#include "User.h"
#include "EfoProfile.h"
#include <memory>

// CBondFeaturePage dialog
IMPLEMENT_DYNAMIC(CBondFeaturePage, CPropertyPage)

CBondFeaturePage::CBondFeaturePage()
	: CPropertyPage(CBondFeaturePage::IDD)
    , m_iConstWireLen(0)
    , m_iTscEnable(0)
    , m_fTscFactor(0)
    , m_iZeroBtos(0)
    , m_iCherryBitEnable(0)
    , m_iBumpBondOption(0)
	, m_iMultiBtos(0)
	, m_iN2H2Enable(0)
	, m_nN2H2StableTime(0)
    , m_iTailCtrlMode(0)
    , m_iWireFeedCheck(0)
    , m_iUnplugEnable(0)
	, m_nWireFeedTimeout(0)
	, m_nHeadTimeOffset(0)
	, m_nTransducerIdleTime(0)
	, m_nTransducerLengthComp(0)
{
}

CBondFeaturePage::~CBondFeaturePage()
{
}

void CBondFeaturePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CONST_WIRE_LENGTH, m_iConstWireLen);
	DDX_CBIndex(pDX, IDC_COMBO_TSC, m_iTscEnable);
	DDX_Text(pDX, IDC_EDIT_TSC, m_fTscFactor);
	DDX_CBIndex(pDX, IDC_COMBO_ZERO_BTOS, m_iZeroBtos);
	DDX_CBIndex(pDX, IDC_COMBO_CHERRY_BIT, m_iCherryBitEnable);
	DDX_CBIndex(pDX, IDC_COMBO_BUMP_CTRL, m_iBumpBondOption);
	DDX_CBIndex(pDX, IDC_COMBO_MULTI_BTOS, m_iMultiBtos);
	DDX_CBIndex(pDX, IDC_COMBO_N2H2_ENABLE, m_iN2H2Enable);
	DDX_Text(pDX, IDC_EDIT_N2H2_STABLE_TIME, m_nN2H2StableTime);
	DDX_CBIndex(pDX, IDC_COMBO_TAIL_CTRL, m_iTailCtrlMode);
	DDX_CBIndex(pDX, IDC_COMBO_WIRE_FEED_CHECK, m_iWireFeedCheck);
	DDX_CBIndex(pDX, IDC_COMBO_UNPLUG, m_iUnplugEnable);
	DDX_Text(pDX, IDC_EDIT_WIRE_FEED_TIMEOUT, m_nWireFeedTimeout);
	DDX_Text(pDX, IDC_EDIT_HEAD_TIME_OFFSET, m_nHeadTimeOffset);
	DDX_Text(pDX, IDC_EDIT_TRANSDUCER_IDLE_TIME, m_nTransducerIdleTime);
	DDX_Text(pDX, IDC_EDIT_TRANSDUCER_LENGTH_COMP, m_nTransducerLengthComp);
}

BEGIN_MESSAGE_MAP(CBondFeaturePage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_TSC, OnFocusTsc)
	ON_CBN_SELCHANGE(IDC_COMBO_MULTI_BTOS, OnMultiBtos)
	ON_CBN_SELCHANGE(IDC_COMBO_CONST_WIRE_LENGTH, OnConstWireLength)
	ON_CBN_SELCHANGE(IDC_COMBO_TSC, OnTsc)
	ON_CBN_SELCHANGE(IDC_COMBO_ZERO_BTOS, OnZeroBtos)
	ON_CBN_SELCHANGE(IDC_COMBO_CHERRY_BIT, OnCherryBit)
	ON_CBN_SELCHANGE(IDC_COMBO_BUMP_CTRL, OnBumpCtrl)
	ON_CBN_SELCHANGE(IDC_COMBO_N2H2_ENABLE, OnN2h2Enable)
	ON_EN_SETFOCUS(IDC_EDIT_N2H2_STABLE_TIME, OnFocusN2h2StableTime)
    ON_CBN_SELCHANGE(IDC_COMBO_WIRE_FEED_CHECK, OnWireFeedCheck)
    ON_CBN_SELCHANGE(IDC_COMBO_TAIL_CTRL, OnTailCtrl)
    ON_CBN_SELCHANGE(IDC_COMBO_UPH_MODE, OnUphMode)
    ON_CBN_SELCHANGE(IDC_COMBO_UNPLUG, OnUnplug)
	ON_EN_SETFOCUS(IDC_EDIT_WIRE_FEED_TIMEOUT, OnFocusCheckWfNum)
	ON_EN_SETFOCUS(IDC_EDIT_HEAD_TIME_OFFSET, OnFocusHeadTimeOffset)
	ON_EN_SETFOCUS(IDC_EDIT_TRANSDUCER_IDLE_TIME, OnFocusTransducerIdleTime)
	ON_EN_SETFOCUS(IDC_EDIT_TRANSDUCER_LENGTH_COMP, OnFocusTransducerLengthComp)
	ON_WM_DESTROY()		//V1.4.W25 add
END_MESSAGE_MAP()

// CBondFeaturePage message handlers
BOOL CBondFeaturePage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	int iRowTop = PARA_COLUMN1_TOP;
	GetDlgItem(IDC_STATIC_CONST_WIRE_LENGTH)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_CONST_WIRE_LENGTH,theMsg.GetMsg(MSG_CONST_WIRE_LENGTH));
	GetDlgItem(IDC_COMBO_CONST_WIRE_LENGTH)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	//iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	GetDlgItem(IDC_STATIC_BUMP_CTRL)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_BUMP_CTRL,theMsg.GetMsg(MSG_BUMP_BALL_CTRL));
	GetDlgItem(IDC_COMBO_BUMP_CTRL)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH*2+5, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	GetDlgItem(IDC_STATIC_MULTI_BTOS)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_MULTI_BTOS,theMsg.GetMsg(MSG_MULTI_BTOS));
	GetDlgItem(IDC_COMBO_MULTI_BTOS)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_MULTI_BTOS)->EnableWindow(theUser.GetUserLevel() > USER_SERVICE);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	GetDlgItem(IDC_STATIC_N2H2_ENABLE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_N2H2_ENABLE,theMsg.GetMsg(MSG_N2H2_ENABLE));
	GetDlgItem(IDC_COMBO_N2H2_ENABLE)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	GetDlgItem(IDC_STATIC_N2H2_STABLE_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_N2H2_STABLE_TIME,theMsg.GetMsg(MSG_N2H2_STABLE_TIME));
	GetDlgItem(IDC_EDIT_N2H2_STABLE_TIME)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	GetDlgItem(IDC_STATIC_UNPLUG)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_UNPLUG,theMsg.GetMsg(MSG_CAP_AUTO_UNPLUG));
	GetDlgItem(IDC_COMBO_UNPLUG)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	if (theUser.GetUserLevel() > USER_SERVICE)
	{
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_TSC)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_COMBO_TSC)->SetWindowPos(NULL, PARA_COLUMN1_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_COMBO_TSC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TSC)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_TSC)->EnableWindow(FALSE);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_ZERO_BTOS)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_ZERO_BTOS,theMsg.GetMsg(MSG_ZERO_BTOS));
		GetDlgItem(IDC_COMBO_ZERO_BTOS)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_COMBO_ZERO_BTOS)->EnableWindow(FALSE);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_WIRE_FEED_CHECK)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_WIRE_FEED_CHECK,theMsg.GetMsg(MSG_WIRE_FEED_CHECK));
		GetDlgItem(IDC_COMBO_WIRE_FEED_CHECK)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_CHECK_WIRE_FEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_CHECK_WIRE_FEED,theMsg.GetMsg(MSG_WIRE_FEED_COUNT));
		GetDlgItem(IDC_EDIT_WIRE_FEED_TIMEOUT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_TAIL_CTRL)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_TAIL_CTRL,theMsg.GetMsg(MSG_FAST_TAIL));
		GetDlgItem(IDC_COMBO_TAIL_CTRL)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_HEAD_TIME_OFFSET)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		SetDlgItemTextW(IDC_STATIC_HEAD_TIME_OFFSET,theMsg.GetMsg(MSG_HEAD_TIME_OFFSET));
		GetDlgItem(IDC_EDIT_HEAD_TIME_OFFSET)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_RCD)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		CComboBox* pCb = (CComboBox*)GetDlgItem(IDC_COMBO_ENABLE);
		pCb->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pCb->AddString(theMsg.GetMsg(MSG_DISABLE_2));
		pCb->AddString(theMsg.GetMsg(MSG_ENABLE_2));
		pCb->SetCurSel(pstMcData->chRcdSample);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_TRANSDUCER_IDLE_TIME)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_TRANSDUCER_IDLE_TIME)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_TRANSDUCER_LENGTH_COMP)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_TRANSDUCER_LENGTH_COMP)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		//v1.5.T101 add
		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		GetDlgItem(IDC_STATIC_CHERRY_BIT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		GetDlgItem(IDC_COMBO_CHERRY_BIT)->SetWindowPos(NULL, PARA_COLUMN2_LEFT, iRowTop, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TSC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_TSC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TSC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ZERO_BTOS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_ZERO_BTOS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CHERRY_BIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CHERRY_BIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_CTRL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_TAIL_CTRL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WIRE_FEED_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_WIRE_FEED_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UPH_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_UPH_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CHECK_WIRE_FEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_WIRE_FEED_TIMEOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HEAD_TIME_OFFSET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_HEAD_TIME_OFFSET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RCD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_ENABLE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_TRANSDUCER_IDLE_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TRANSDUCER_IDLE_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TRANSDUCER_LENGTH_COMP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TRANSDUCER_LENGTH_COMP)->ShowWindow(SW_HIDE);
	}

	const PGM_RECORD *pstPgm = thePgm.GetRcd();
	m_iConstWireLen    = pstPgm->bConstWireLength;
	m_iCherryBitEnable = pstPgm->bFabTest;
	m_iMultiBtos       = pstPgm->nMultiBtos;
	m_iN2H2Enable      = pstPgm->bN2H2Enable;
	m_nN2H2StableTime  = pstPgm->nN2H2StableTime;
	m_nHeadTimeOffset  = pstPgm->nHeadTimeOffset;
	m_nWireFeedTimeout = pstPgm->nCheckWireFeedTimeout;
	m_iWireFeedCheck   = pstPgm->bWireFeedCheck;
	m_iUnplugEnable    = pstPgm->bCapillaryAutoPlug;

	if (m_iWireFeedCheck == FALSE)
		GetDlgItem(IDC_EDIT_WIRE_FEED_TIMEOUT)->EnableWindow(FALSE);

	if (m_iN2H2Enable == FALSE)
		GetDlgItem(IDC_EDIT_N2H2_STABLE_TIME)->EnableWindow(FALSE);

	m_iTscEnable            = pstMcData->bTscEnable;
	m_iZeroBtos             = pstMcData->bZeroBtosOptics;
	m_fTscFactor            = pstMcData->fTscFactor;
	m_iTailCtrlMode         = pstMcData->bTailCtrlMode;
	m_iWireFeedCheck        = pstMcData->bWireFeedCheck;
	m_nTransducerIdleTime   = pstMcData->nTransducerIdleTime;
	m_nTransducerLengthComp = pstMcData->nTransducerLengthComp;

	m_iBumpBondOption = theBond.GetBumpBondOption();

	return CPropertyPage::OnInitDialog();
}

HBRUSH CBondFeaturePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBondFeaturePage::OnDestroy()
{
    UpdateData();

	PGM_RECORD *pstPgm = thePgm.GetRcd();
	pstPgm->bConstWireLength   = (m_iConstWireLen == TRUE);
	pstPgm->bFabTest           = (m_iCherryBitEnable == TRUE);
	pstPgm->nMultiBtos         = m_iMultiBtos;
	pstPgm->bN2H2Enable        = (m_iN2H2Enable == TRUE);
    pstPgm->nN2H2StableTime    = m_nN2H2StableTime;
    pstPgm->nHeadTimeOffset    = m_nHeadTimeOffset;
	pstPgm->bCapillaryAutoPlug = (m_iUnplugEnable == TRUE);
    if (pstPgm->bCapillaryAutoPlug)
		MessageBox(_T("Unplug Enabled! Please be careful on capillary when unplugging."),_T("Information"),MB_OK);
	pstPgm->nCheckWireFeedTimeout = m_nWireFeedTimeout;
	pstPgm->bWireFeedCheck = m_iWireFeedCheck == TRUE;

    MACHINE_DATA* pstMcData = theMcData.GetRcd();
	pstMcData->bTscEnable            = (m_iTscEnable == TRUE);
	pstMcData->bZeroBtosOptics       = (m_iZeroBtos == TRUE);
    pstMcData->fTscFactor            = m_fTscFactor;
	pstMcData->bTailCtrlMode         = (m_iTailCtrlMode == TRUE);
	pstMcData->bWireFeedCheck        = (m_iWireFeedCheck == TRUE);
	pstMcData->chRcdSample           = (char)((CComboBox*)GetDlgItem(IDC_COMBO_ENABLE))->GetCurSel();
	pstMcData->nTransducerIdleTime   = (BYTE)m_nTransducerIdleTime;
	pstMcData->nTransducerLengthComp = m_nTransducerLengthComp;

	theApp.ArchiveMachineData(ARCHIVE_SAVE);
	theBond.SetBumpBondOption(m_iBumpBondOption);
    CPropertyPage::OnDestroy();
}

void CBondFeaturePage::OnFocusTsc()
{
    GetDlgItem(IDC_COMBO_CONST_WIRE_LENGTH)->SetFocus();

    double dTscFactor;
    theKeyPad.GetUserKeyInData(&dTscFactor);
    m_fTscFactor = (float)dTscFactor;

    UpdateData(false);
}

void CBondFeaturePage::OnMultiBtos()
{
	UpdateData(TRUE);
	if (m_iMultiBtos != 0)
		theRef.UpdateAllBtos(theMcData.GetBondTipOs());
}

void CBondFeaturePage::OnConstWireLength()
{
	UpdateData(TRUE);
}

void CBondFeaturePage::OnTsc()
{
	UpdateData(TRUE);
}

void CBondFeaturePage::OnZeroBtos()
{
	UpdateData(TRUE);
}

void CBondFeaturePage::OnCherryBit()
{
	UpdateData(TRUE);

	if (m_iCherryBitEnable != 1) return;
	if (thePgm.IsProgramValid() == false) return;

	//v3.1.T60 delete
	//PGM_RECORD *pstPgm = thePgm.GetRcd();
	//pstPgm->nTailLengthUm = 700;
	//pstPgm->nSparkDelay = 10;
	//v3.1.T60 add
	thePgm.GetRcd()->nSparkDelay = 10;
	theEfoProfile.GetProfile(0)->nTailLen_x1um = 700;

	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();
	pstBondPara->nSrchHeight2 = 250;
	pstBondPara->nSrchSpeed2 = 6;

	ST_LOOP_PARA stLoopPara;
	for (short i = 0; i < theLoopPara.m_nTotalLoopModeUsed; i++)
	{
		theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[i], &stLoopPara);
		stLoopPara.stLoopBase.nTrajFactor = 50;
		stLoopPara.stLoopBase.nPullDist = 300;
		stLoopPara.stLoopBase.nLoopHt = 200; 
		stLoopPara.stLoopBase.nRevDist = 50;
		stLoopPara.stLoopBase.nRevAngle = 0;
		stLoopPara.stLoopAdv.nPullSpeed = 10;
		stLoopPara.stLoopAdv.nTrajSpeed = 50;
		theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[i], &stLoopPara);
	}
}

void CBondFeaturePage::OnBumpCtrl()
{
	UpdateData(TRUE);
}

void CBondFeaturePage::OnN2h2Enable()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_N2H2_STABLE_TIME)->EnableWindow(m_iN2H2Enable);
}

void CBondFeaturePage::OnFocusN2h2StableTime()
{
	GetDlgItem(IDC_COMBO_CONST_WIRE_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nN2H2StableTime, m_nN2H2StableTime, 0, 30);	//unit: second
	UpdateData(false);
}

void CBondFeaturePage::OnWireFeedCheck()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_WIRE_FEED_TIMEOUT)->EnableWindow(m_iWireFeedCheck);
}

void CBondFeaturePage::OnTailCtrl()
{
	UpdateData(TRUE);
}

void CBondFeaturePage::OnUphMode()
{
	UpdateData(TRUE);
}

void CBondFeaturePage::OnUnplug()
{
    UpdateData(TRUE);
}

void CBondFeaturePage::OnFocusCheckWfNum()
{
	GetDlgItem(IDC_COMBO_CONST_WIRE_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nWireFeedTimeout, m_nWireFeedTimeout, 5, 100);	//unit: number
	UpdateData(false);
}

void CBondFeaturePage::OnFocusHeadTimeOffset()
{
	GetDlgItem(IDC_COMBO_CONST_WIRE_LENGTH)->SetFocus();
#if _SERVO_TYPE == 2
	theKeyPad.GetUserKeyInData(&m_nHeadTimeOffset, m_nHeadTimeOffset, -10, 0);	//v3.1.T430 add
#else
	theKeyPad.GetUserKeyInData(&m_nHeadTimeOffset, m_nHeadTimeOffset, -10, 10);
#endif
	UpdateData(false);
}

void CBondFeaturePage::OnFocusTransducerIdleTime()
{
	GetDlgItem(IDC_COMBO_CONST_WIRE_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTransducerIdleTime, m_nTransducerIdleTime, 0, 20);
	UpdateData(false);
}

void CBondFeaturePage::OnFocusTransducerLengthComp()
{
	GetDlgItem(IDC_COMBO_CONST_WIRE_LENGTH)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTransducerLengthComp, m_nTransducerLengthComp, -100, 100);
	UpdateData(false);
}
