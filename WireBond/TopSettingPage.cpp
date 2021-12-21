/***************************************************************
TopSettingPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "TopSettingPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TopClamp.h"
#include "R2RIndex.h"
#include "MathCalc.h"

// CTopSettingPage dialog
IMPLEMENT_DYNAMIC(CTopSettingPage, CPropertyPage)

CTopSettingPage::CTopSettingPage()
	: CPropertyPage(CTopSettingPage::IDD)
	, m_lHeatDelay(0)
	, m_lLimitTolerance(0)
	, m_lSrchBackLen(4000)
	, m_lStandbyLen(0)
	, m_iIndexSpeedCo(0)
	, m_iEjectSpeed(0)
	, m_l1stSlipHeatDelay(0)
	, m_iPrIndexEnable(false)
	, m_lPrIndexTol(0)
	, m_lIndexClawOffset(0)
	, m_lOpenClampDelay(0)
	, m_lIndexPrDelay(0)
	, m_lIniDelay(0)
    , m_iVacuumControl(0)
	, m_lVacumDelay(0)
	, m_l1stUnitHeatDelay(0)
	, m_sPRIndexScore(0)
	, m_iOutputMagazineSlotSequence(0)
{
}

CTopSettingPage::~CTopSettingPage()
{
}

void CTopSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HEAT_DELAY, m_lHeatDelay);
	DDX_Text(pDX, IDC_EDIT_LIMIT_TOL, m_lLimitTolerance);
	DDX_Text(pDX, IDC_EDIT_X_STANDBY_LEN, m_lStandbyLen);
	DDX_CBIndex(pDX, IDC_COMBO_INDEX_SPEED, m_iIndexSpeedCo);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_SPEED, m_iEjectSpeed);
	DDX_Text(pDX, IDC_EDIT_1ST_HEAT_DELAY, m_l1stSlipHeatDelay);
	DDX_CBIndex(pDX, IDC_COMBO_PR_INDEX, m_iPrIndexEnable);
	DDX_Text(pDX, IDC_EDIT_PR_INDEX_TOL, m_lPrIndexTol);
	DDX_Text(pDX, IDC_EDIT_CLAW_OFFSET, m_lIndexClawOffset);
	DDX_Text(pDX, IDC_EDIT_OPEN_DELAY, m_lOpenClampDelay);
	DDX_Text(pDX, IDC_EDIT_INDEX_PR_DELAY, m_lIndexPrDelay);
	DDX_Text(pDX, IDC_EDIT_INI_DELAY, m_lIniDelay);
	DDX_CBIndex(pDX, IDC_COMBO_VACUUM_DETECT, m_iVacuumControl);
	DDX_Text(pDX, IDC_EDIT_VACUM_DELAY, m_lVacumDelay);
	DDX_Text(pDX, IDC_EDIT_1ST_UNIT_DELAY, m_l1stUnitHeatDelay);
	DDX_Text(pDX, IDC_EDIT_PR_INDEX_SCORE, m_sPRIndexScore);
    DDX_CBIndex(pDX, IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE, m_iOutputMagazineSlotSequence);
}

BEGIN_MESSAGE_MAP(CTopSettingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_HEAT_DELAY, OnFocusHeatDelay)
	ON_EN_SETFOCUS(IDC_EDIT_LIMIT_TOL, OnFocusLimitTol)
	ON_EN_SETFOCUS(IDC_EDIT_X_STANDBY_LEN, OnFocusXStandbyLen)
	ON_EN_SETFOCUS(IDC_EDIT_LF_WAIT_POS, OnLFWaitPos)
	ON_CBN_SELCHANGE(IDC_COMBO_INDEX_SPEED, OnIndexSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_EJECT_SPEED, OnEjectSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_1ST_HEAT_DELAY, OnFocus1stHeatDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_PR_INDEX, OnPrIndex)
	ON_EN_SETFOCUS(IDC_EDIT_PR_INDEX_TOL, OnFocusPrIndexTol)
	ON_CBN_SELCHANGE(IDC_COMBO_ENABLE, OnFlattenLF)
	ON_EN_SETFOCUS(IDC_EDIT_SPEED, OnFocusSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_CLAW_OFFSET, OnFocusClawOffset)
	ON_EN_SETFOCUS(IDC_EDIT_OPEN_DELAY, OnFocusOpenDelay)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_PR_DELAY, OnFocusIndexPrDelay)
    ON_EN_SETFOCUS(IDC_EDIT_INI_DELAY, OnFocusIniDelay)
    ON_CBN_SELCHANGE(IDC_COMBO_VACUUM_DETECT, OnVacuumControl)
    ON_WM_VSCROLL()
    ON_EN_SETFOCUS(IDC_EDIT_VACUM_DELAY, OnFocusVacumDelay)
	ON_EN_SETFOCUS(IDC_EDIT_1ST_UNIT_DELAY, OnFocus1stUnitDelay)
	ON_EN_SETFOCUS(IDC_EDIT_PR_INDEX_SCORE, OnFocusPrIndexScore)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE, OnOutputMagazineSlotSequence)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_CB_HEAT_DELAY_UNIT, OnHeatDelayUnit)
	ON_MESSAGE(WM_SAVESETTING,SaveSetting)	//V1.5.W117 add
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTopSettingPage message handlers
BOOL CTopSettingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	TOP_TRACK_DATA stTrackData;
	theTopTrack.GetRcd(&stTrackData);

	m_nSel = stTrackData.nHeatDelayUnit;

	int iRowTop = PARA_COLUMN1_TOP + 5;

	GetDlgItem(IDC_STATIC_HEAT_DELAY_UNIT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_HEAT_DELAY_UNIT,theMsg.GetMsg(MSG_HEAT_DELAY_UNIT));

	CWnd* pWnd=GetDlgItem(IDC_CB_HEAT_DELAY_UNIT);
	pWnd->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_MILLISECOND));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_SECOND));
	((CComboBox*)pWnd)->SetCurSel(stTrackData.nHeatDelayUnit);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_1ST_HEAT_DELAY,theMsg.GetMsg(MSG_1ST_PREHEAT_DELAY));

	GetDlgItem(IDC_EDIT_1ST_HEAT_DELAY)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_INI_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_INI_DELAY,theMsg.GetMsg(MSG_1ST_HEAT_DELAY));

	GetDlgItem(IDC_EDIT_INI_DELAY)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_HEAT_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_HEAT_DELAY,theMsg.GetMsg(MSG_HEAT_DELAY));

	GetDlgItem(IDC_EDIT_HEAT_DELAY)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	//1st unit heater delay
	GetDlgItem(IDC_STATIC_1ST_UNIT_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_1ST_UNIT_DELAY,theMsg.GetMsg(MSG_1ST_UNIT_HEAT_DELAY));

	GetDlgItem(IDC_EDIT_1ST_UNIT_DELAY)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_OPEN_CLAMP_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_OPEN_CLAMP_DELAY,theMsg.GetMsg(MSG_OPEN_CLAMP_DELAY));

	GetDlgItem(IDC_EDIT_OPEN_DELAY)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_INDEX_PR_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_INDEX_PR_DELAY,theMsg.GetMsg(MSG_INDEX_PR_DELAY));

	GetDlgItem(IDC_EDIT_INDEX_PR_DELAY)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_VACUUM_DETECT)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_VACUUM_DETECT,theMsg.GetMsg(MSG_VACUUM_CONTROL));

	GetDlgItem(IDC_COMBO_VACUUM_DETECT)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_VACUM_DELAY)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_VACUM_DELAY,theMsg.GetMsg(MSG_VACUUM_DELAY));

	GetDlgItem(IDC_EDIT_VACUM_DELAY)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_PR_INDEX)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_PR_INDEX,theMsg.GetMsg(MSG_PR_INDEX));

	GetDlgItem(IDC_COMBO_PR_INDEX)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_PR_INDEX_SCORE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_PR_INDEX_SCORE,theMsg.GetMsg(MSG_PR_INDEX_SCORE));

	GetDlgItem(IDC_EDIT_PR_INDEX_SCORE)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_PR_TOLERANCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_PR_TOLERANCE,theMsg.GetMsg(MSG_PR_INDEX_TOL));

	GetDlgItem(IDC_EDIT_PR_INDEX_TOL)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	//V1.4.W35 add - begin
	GetDlgItem(IDC_STATIC_FEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_FEED, theMsg.GetMsg(MSG_FLATTEN_LF_BEFORE_PR_INDEX));

	pWnd = GetDlgItem(IDC_COMBO_ENABLE);
	pWnd->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_DISABLE));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_ENABLE));
	((CComboBox*)pWnd)->SetCurSel(stTrackData.bFlattenLF);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_SPEED, theMsg.GetMsg(MSG_LF_FLATTEN_DURATION));

	GetDlgItem(IDC_EDIT_SPEED)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemInt(IDC_EDIT_SPEED, stTrackData.wFlattenDuration);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;
	//V1.4.W35 add - end

	GetDlgItem(IDC_STATIC_CLAW_OFFSET)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_CLAW_OFFSET,theMsg.GetMsg(MSG_CLAW_OFFSET));

	GetDlgItem(IDC_EDIT_CLAW_OFFSET)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_INDEX_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_INDEX_SPEED,theMsg.GetMsg(MSG_INDEX_SPEED));

	GetDlgItem(IDC_COMBO_INDEX_SPEED)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_EJECT_SPEED)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_EJECT_SPEED,theMsg.GetMsg(MSG_EJECT_SPEED));

	GetDlgItem(IDC_COMBO_EJECT_SPEED)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_STANDBY_LEN)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_STANDBY_LEN,theMsg.GetMsg(MSG_STANDBY_LEN));

	GetDlgItem(IDC_EDIT_X_STANDBY_LEN)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_LF_WAIT_POS)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_LF_WAIT_POS,theMsg.GetMsg(MSG_NEXT_LF_WAIT_POS));

	GetDlgItem(IDC_EDIT_LF_WAIT_POS)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemInt(IDC_EDIT_LF_WAIT_POS, stTrackData.nLFWaitPos);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	GetDlgItem(IDC_STATIC_LIMIT_TOLERANCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_LIMIT_TOLERANCE,theMsg.GetMsg(MSG_RIGHT_CLEARANCE));

	GetDlgItem(IDC_EDIT_LIMIT_TOL)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	pWnd = GetDlgItem(IDC_COMBO_INDEX_SPEED);
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_SLOWEST));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_SLOWER));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_SLOW));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_MEDIUM));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_FAST));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_FASTER));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_FASTEST));

	pWnd = GetDlgItem(IDC_COMBO_EJECT_SPEED);
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_SLOWEST));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_SLOW));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_MEDIUM));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_FAST));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_FASTEST));

	pWnd = GetDlgItem(IDC_COMBO_PR_INDEX);
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_DISABLE));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_ENABLE));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_1ST_ONLY));

	GetDlgItem(IDC_STATIC_OUTPUT_MAGAZINE_SLOT_SEQUENCE)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_OUTPUT_MAGAZINE_SLOT_SEQUENCE,theMsg.GetMsg(MSG_OUTPUT_MAG_SLOT_SEQ));

	GetDlgItem(IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE)->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	pWnd = GetDlgItem(IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE);
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_STANDARD_SEQ));
	((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_ALTERNATE_SEQ));

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	GetDlgItem(IDC_STATIC_NEXT_STEP_PUSHER_IN)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemTextW(IDC_STATIC_NEXT_STEP_PUSHER_IN,theMsg.GetMsg(MSG_NEXT_STEP_PUSHER_IN));

	if(stTrackData.nNextStepPusherIn>6)stTrackData.nNextStepPusherIn=2;

	pWnd = GetDlgItem(IDC_CB_NEXT_STEP_PUSHER_IN);
	pWnd->SetWindowPos(NULL, COLUMN2_LEFT, iRowTop, COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	for(char n=0;n<7;n++)((CComboBox*)pWnd)->AddString(CString((char)(n+0x30)));
	((CComboBox*)pWnd)->SetCurSel(stTrackData.nNextStepPusherIn);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//unit ms
	m_lHeatDelay        = stTrackData.lHeatingDelay;
	m_l1stSlipHeatDelay = stTrackData.l1stSlipHeatDelay;
	m_lIniDelay         = stTrackData.lInitDelay;
	m_l1stUnitHeatDelay = stTrackData.l1stUnitHeatDelay;
	m_lOpenClampDelay   = stTrackData.lOpenClampDelay;

	if (m_nSel == 1)
	{
		m_lHeatDelay        = round(m_lHeatDelay / 1000.0);
		m_l1stSlipHeatDelay = round(m_l1stSlipHeatDelay / 1000.0);
		m_lIniDelay         = round(m_lIniDelay / 1000.0);
		m_l1stUnitHeatDelay = round(m_l1stUnitHeatDelay / 1000.0);
	}

	//unit mm
	m_lLimitTolerance             = stTrackData.lLimitTolerance/1000;
	//m_lSrchBackLen      = lpstTrackData->lSrchBackLen/1000; 
	m_lStandbyLen                 = stTrackData.lLfStandbyLen/1000;
	m_iIndexSpeedCo               = stTrackData.nIndexSpeedCoeff - 1;
	m_iEjectSpeed                 = stTrackData.nEjectSpeedCoeff - 1;
	m_iPrIndexEnable              = stTrackData.nPrIndexEnable;

	//unit um
	m_lPrIndexTol                 = stTrackData.lPrIndexTolerance; 
	m_lIndexClawOffset            = stTrackData.lIndexPosOffset;
	m_lIndexPrDelay               = stTrackData.lIndexPrDelay;
	//m_lIniDelay = lpstTrackData->lInitDelay;
	m_iVacuumControl              = stTrackData.bVacuumControl;
	m_lVacumDelay                 = stTrackData.lVacuumDelay;
	m_sPRIndexScore               = stTrackData.sPRIndexScore;
	m_iOutputMagazineSlotSequence = stTrackData.iOutputMagazineSlotSequence;

	UpdateUI(stTrackData.nHeatDelayUnit);

	if (iRowTop > PROPERTY_PAGE_HEIGHT)
	{
		//m_nPageSize = iRowTop - PROPERTY_PAGE_HEIGHT;
		m_nPageSize = iRowTop - PROPERTY_PAGE_HEIGHT + 30;
		m_nPageSize+=PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

		EnableScrollBarCtrl(SB_VERT, TRUE);
		SetScrollRange(SB_VERT, 0, m_nPageSize);
	}
	else
		EnableScrollBarCtrl(SB_VERT, FALSE);

	UpdateData(false);

	PostMessage(WM_COMMAND, MAKELONG(IDC_COMBO_PR_INDEX, CBN_SELCHANGE), (LPARAM)GetDlgItem(IDC_COMBO_PR_INDEX)->m_hWnd);	//V1.4.W35 add
	PostMessage(WM_COMMAND, MAKELONG(IDC_COMBO_ENABLE, CBN_SELCHANGE), (LPARAM)GetDlgItem(IDC_COMBO_ENABLE)->m_hWnd);		//V1.5.W8 add

	return CPropertyPage::OnInitDialog();
}

HBRUSH CTopSettingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CTopSettingPage::SaveSetting(WPARAM wParam, LPARAM lParam)		//V1.5.W117 add
{
	UpdateData(true);

	TOP_TRACK_DATA stTrackData;
	theTopTrack.GetRcd(&stTrackData);

	stTrackData.iOutputMagazineSlotSequence = m_iOutputMagazineSlotSequence;
	stTrackData.lHeatingDelay     = m_lHeatDelay;
	stTrackData.l1stSlipHeatDelay = m_l1stSlipHeatDelay;
	stTrackData.l1stUnitHeatDelay = m_l1stUnitHeatDelay;
	stTrackData.lLimitTolerance   = m_lLimitTolerance*1000;
	stTrackData.lSrchBackLen      = 4000;
	stTrackData.lLfStandbyLen     = m_lStandbyLen*1000;
	stTrackData.nIndexSpeedCoeff  = m_iIndexSpeedCo + 1;
	stTrackData.nEjectSpeedCoeff  = m_iEjectSpeed + 1;
	stTrackData.lIndexPosOffset   = m_lIndexClawOffset;
	stTrackData.lOpenClampDelay   = m_lOpenClampDelay;
	stTrackData.lIndexPrDelay     = m_lIndexPrDelay;
	stTrackData.lInitDelay        = m_lIniDelay;
	stTrackData.lVacuumDelay      = m_lVacumDelay;
    stTrackData.bVacuumControl    = m_iVacuumControl != 0;
    stTrackData.nPrIndexEnable    = (byte)m_iPrIndexEnable;
	stTrackData.lPrIndexTolerance = m_lPrIndexTol; 
	stTrackData.sPRIndexScore     = m_sPRIndexScore;
	stTrackData.bFlattenLF        = ((CComboBox*)GetDlgItem(IDC_COMBO_ENABLE))->GetCurSel();	//V1.4.W35 add
	stTrackData.wFlattenDuration  = GetDlgItemInt(IDC_EDIT_SPEED);						//V1.4.W35 add
	stTrackData.nNextStepPusherIn = (BYTE)((CComboBox*)GetDlgItem(IDC_CB_NEXT_STEP_PUSHER_IN))->GetCurSel();
	stTrackData.nHeatDelayUnit    = m_nSel;

	if (m_nSel == 1)
	{
		stTrackData.lHeatingDelay     *= 1000;
		stTrackData.l1stSlipHeatDelay *= 1000;
		stTrackData.l1stUnitHeatDelay *= 1000;
		stTrackData.lInitDelay        *= 1000;
	}

	theTopTrack.SetRcd(&stTrackData);

	TOP_DEVICE_RECORD stTopDevicePara;
	theDevice.GetTopRcd(&stTopDevicePara);
	theTopTrack.RefreshMeasurementData(&stTopDevicePara);

	return NOERROR;
}

void CTopSettingPage::OnDestroy()		//V1.5.W117 edit
{
	SendMessageW(WM_SAVESETTING);

	CPropertyPage::OnDestroy();
}

void CTopSettingPage::OnFocusLimitTol()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lLimitTolerance, 5, 2, 40);
    UpdateData(false);
}

void CTopSettingPage::OnFocusHeatDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lHeatDelay, m_lHeatDelay, 0, m_nSel == 0 ? 300000 : 300);
    UpdateData(false);
}

void CTopSettingPage::OnFocusXStandbyLen()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lStandbyLen, 0, 0, 100);
    UpdateData(false);
}

void CTopSettingPage::OnLFWaitPos()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	TOP_TRACK_DATA stTrackData;
	theTopTrack.GetRcd(&stTrackData);

	long lPos = 0;
    if (theKeyPad.GetUserKeyInData(&lPos, stTrackData.nLFWaitPos, 0, 30) == CANCEL_SELECTED)
		return;

	SetDlgItemInt(IDC_EDIT_LF_WAIT_POS, lPos);
	stTrackData.nLFWaitPos = (BYTE)lPos;

	theTopTrack.SetRcd(&stTrackData);
}

void CTopSettingPage::OnIndexSpeed()
{
	UpdateData(true);
}

void CTopSettingPage::OnEjectSpeed()
{
	UpdateData(true);
}

void CTopSettingPage::OnFocus1stHeatDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_l1stSlipHeatDelay, m_l1stSlipHeatDelay, 0, m_nSel == 0 ? 600000 : 600);
	UpdateData(false);
}

void CTopSettingPage::OnPrIndex()	//V1.4.W35 edit, V1.5.W8 edit
{
	UpdateData(true);

	GetDlgItem(IDC_EDIT_PR_INDEX_SCORE)->EnableWindow(m_iPrIndexEnable);
	GetDlgItem(IDC_EDIT_PR_INDEX_TOL)->EnableWindow(m_iPrIndexEnable);
}

void CTopSettingPage::OnFocusPrIndexTol()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lPrIndexTol);
    UpdateData(false);
}

void CTopSettingPage::OnFlattenLF()	//V1.5.W8 edit
{
	CComboBox *pCb = (CComboBox*)GetDlgItem(IDC_COMBO_ENABLE);
	GetDlgItem(IDC_EDIT_SPEED)->EnableWindow(pCb->GetCurSel());
}

void CTopSettingPage::OnFocusClawOffset()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lIndexClawOffset, 8000, 0, 12000);
    UpdateData(false);
}

void CTopSettingPage::OnFocusOpenDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lOpenClampDelay, 0, 0, 10000);
    UpdateData(false);
}

void CTopSettingPage::OnFocusIndexPrDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lIndexPrDelay, 0, 0, 3000);
    UpdateData(false);
}

void CTopSettingPage::OnFocusIniDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lIniDelay, m_lIniDelay, 0, m_nSel == 0 ? 600000 : 600);
	UpdateData(false);
}

void CTopSettingPage::OnFocusVacumDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lVacumDelay);
	UpdateData(false);
}

void CTopSettingPage::OnVacuumControl()
{
	UpdateData();
	if (m_iVacuumControl != 0) return;
	theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
}

void CTopSettingPage::OnFocus1stUnitDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_l1stUnitHeatDelay, m_l1stUnitHeatDelay, 0, m_nSel == 0 ? 300000 : 300);
	UpdateData(false);
}

void CTopSettingPage::OnOutputMagazineSlotSequence()
{
	UpdateData(true);
}

void CTopSettingPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    short nOffset = 0;
    int nMin, nMax, iCurPos = GetScrollPos(SB_VERT);
    SCROLLINFO stScrollInfo;

    GetScrollInfo(SB_VERT, &stScrollInfo);
    nMin = stScrollInfo.nMin;
    nMax = stScrollInfo.nMax;

    switch(nSBCode)
    {
    case SB_LINEDOWN:
	{
		if (iCurPos == nMax) break;

		nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;
		if (iCurPos + nOffset > nMax)
			nOffset = nMax - iCurPos;
		break;
	}
    case SB_LINEUP:
	{
		if (iCurPos == nMin) break;

		nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY);
		if (iCurPos + nOffset < nMin)
			nOffset = nMin - iCurPos;
		break;
	}
    case SB_PAGEDOWN:
	{
		if (iCurPos == nMax) break;
		nOffset = m_nPageSize - iCurPos;
		break;
	}
    case SB_PAGEUP:
	{
		if (iCurPos == nMin) break;
		nOffset = -iCurPos;
		break;
	}
	case SB_THUMBTRACK:
		nOffset = nPos - iCurPos;
		break;
    default:
        break;
    }

    SetScrollPos(SB_VERT, iCurPos + nOffset);
    ScrollWindow(0, -nOffset);

    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTopSettingPage::OnFocusPrIndexScore()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_sPRIndexScore, 60, 20, 100);
	UpdateData(false);
}

BOOL CTopSettingPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, NULL);
	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CTopSettingPage::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	CPropertyPage::OnMouseMove(nFlags, point);
}

void CTopSettingPage::UpdateUI(BYTE nSel)
{
	CString str, strOld, strNew;

	if (nSel == 0)
	{
		strOld = "(s)";
		strNew = "(ms)";
	}
	else
	{
		strOld = "(ms)";
		strNew = "(s)";
	}

	GetDlgItemText(IDC_STATIC_1ST_HEAT_DELAY, str);
	str.Replace(strOld, strNew);
	SetDlgItemText(IDC_STATIC_1ST_HEAT_DELAY, str);

	GetDlgItemText(IDC_STATIC_INI_DELAY, str);
	str.Replace(strOld, strNew);
	SetDlgItemText(IDC_STATIC_INI_DELAY, str);

	GetDlgItemText(IDC_STATIC_HEAT_DELAY, str);
	str.Replace(strOld, strNew);
	SetDlgItemText(IDC_STATIC_HEAT_DELAY, str);

	GetDlgItemText(IDC_STATIC_1ST_UNIT_DELAY, str);
	str.Replace(strOld, strNew);
	SetDlgItemText(IDC_STATIC_1ST_UNIT_DELAY, str);
}

void CTopSettingPage::OnHeatDelayUnit()
{
	BYTE nSel = (BYTE)((CComboBox*)GetDlgItem(IDC_CB_HEAT_DELAY_UNIT))->GetCurSel();
	if (m_nSel == nSel) return;

	double dbl = (nSel == 0 ? 1000 : 0.001);

	m_l1stSlipHeatDelay = round(m_l1stSlipHeatDelay * dbl);
	m_lIniDelay         = round(m_lIniDelay * dbl);
	m_lHeatDelay        = round(m_lHeatDelay * dbl);
	m_l1stUnitHeatDelay = round(m_l1stUnitHeatDelay * dbl);

	m_nSel = nSel;
	UpdateUI(nSel);
	UpdateData(FALSE);
}

void CTopSettingPage::OnFocusSpeed()	//V1.4.W35 add
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	int nDuration = 1000;
	if (theKeyPad.GetUserKeyInData(&nDuration, nDuration, 500, 5000) == CANCEL_SELECTED)
		return;
	SetDlgItemInt(IDC_EDIT_SPEED, nDuration);
}
