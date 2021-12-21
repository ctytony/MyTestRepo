/***************************************************************
CuEliteSettingPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "CuEliteSettingPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "MaterialHandlerThread.h"
#include "CuIndex.h"
#include "CuClamp.h"

// CCuEliteSettingPage dialog

IMPLEMENT_DYNAMIC(CCuEliteSettingPage, CPropertyPage)

CCuEliteSettingPage::CCuEliteSettingPage()
	: CPropertyPage(CCuEliteSettingPage::IDD)
	, m_lHeatDelay(0)
	, m_lLimitTolerance(0)
	, m_lSrchBackLen(4000)
	, m_lStandbyLen(0)
	, m_nIndexSpeedCoeff(0)
	, m_nEjectSpeedCoeff(0)
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
	, m_lWindowClamperOverHeatTime(0)	//add window clamper over heat protection
	, m_sPRIndexScore(0)
	, m_iOutputMagazineSlotSequence(0)
	, m_lClawDelay(0)
{

}

CCuEliteSettingPage::~CCuEliteSettingPage()
{
}

void CCuEliteSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HEAT_DELAY, m_lHeatDelay);
	DDX_Text(pDX, IDC_EDIT_LIMIT_TOL, m_lLimitTolerance);
	//DDX_Text(pDX, IDC_EDIT_X_BACK, m_lSrchBackLen);
	DDX_Text(pDX, IDC_EDIT_X_STANDBY_LEN, m_lStandbyLen);
	DDX_CBIndex(pDX, IDC_COMBO_INDEX_SPEED_CUELITE, m_nIndexSpeedCoeff);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_SPEED_CUELITE, m_nEjectSpeedCoeff);
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
	DDX_Text(pDX, IDC_EDIT_HEAT_PROTECT, m_lWindowClamperOverHeatTime); 
	DDX_Text(pDX, IDC_EDIT_PR_INDEX_SCORE, m_sPRIndexScore);
    DDX_CBIndex(pDX, IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE, m_iOutputMagazineSlotSequence);
	DDX_Text(pDX, IDC_EDIT_CLAW_DELAY, m_lClawDelay);
}

BEGIN_MESSAGE_MAP(CCuEliteSettingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_HEAT_DELAY, &CCuEliteSettingPage::OnEnSetfocusEditHeatDelay)
	ON_EN_SETFOCUS(IDC_EDIT_LIMIT_TOL, &CCuEliteSettingPage::OnEnSetfocusEditLimitTol)
//	ON_EN_SETFOCUS(IDC_EDIT_X_BACK, &CCuEliteSettingPage::OnEnSetfocusEditXBack)
	ON_EN_SETFOCUS(IDC_EDIT_X_STANDBY_LEN, &CCuEliteSettingPage::OnEnSetfocusEditXStandbyLen)
	ON_CBN_SELCHANGE(IDC_COMBO_INDEX_SPEED_CUELITE, &CCuEliteSettingPage::OnCbnSelchangeComboIndexSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_EJECT_SPEED_CUELITE, &CCuEliteSettingPage::OnCbnSelchangeComboEjectSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_1ST_HEAT_DELAY, &CCuEliteSettingPage::OnEnSetfocusEdit1stHeatDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_PR_INDEX, &CCuEliteSettingPage::OnCbnSelchangeComboPrIndex)
	ON_EN_SETFOCUS(IDC_EDIT_PR_INDEX_TOL, &CCuEliteSettingPage::OnEnSetfocusEditPrIndexTol)
	ON_EN_SETFOCUS(IDC_EDIT_CLAW_OFFSET, &CCuEliteSettingPage::OnEnSetfocusEditClawOffset)
	ON_EN_SETFOCUS(IDC_EDIT_OPEN_DELAY, &CCuEliteSettingPage::OnEnSetfocusEditOpenDelay)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_PR_DELAY, &CCuEliteSettingPage::OnEnSetfocusEditIndexPrDelay)
//	ON_EN_CHANGE(IDC_EDIT_1ST_HEAT_DELAY, &CCuEliteSettingPage::OnEnChangeEdit1stHeatDelay)
    ON_EN_SETFOCUS(IDC_EDIT_INI_DELAY, &CCuEliteSettingPage::OnEnSetfocusEditIniDelay)
    ON_CBN_SELCHANGE(IDC_COMBO_VACUUM_DETECT, &CCuEliteSettingPage::OnCbnSelchangeComboVacuumControl)
    ON_WM_VSCROLL()
//  ON_STN_CLICKED(IDC_STATIC_INI_DELAY2, &CCuEliteSettingPage::OnStnClickedStaticIniDelay2)
    ON_EN_SETFOCUS(IDC_EDIT_VACUM_DELAY, &CCuEliteSettingPage::OnEnSetfocusEditVacumDelay) //@4
	ON_EN_SETFOCUS(IDC_EDIT_1ST_UNIT_DELAY, &CCuEliteSettingPage::OnEnSetfocusEdit1stUnitDelay) //@5
	ON_EN_SETFOCUS(IDC_EDIT_HEAT_PROTECT, &CCuEliteSettingPage::OnEnSetfocusEditHeatProtectTime)
	ON_EN_SETFOCUS(IDC_EDIT_PR_INDEX_SCORE, &CCuEliteSettingPage::OnEnSetfocusEditPrIndexScore)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE, &CCuEliteSettingPage::OnCbnSelchangeComboOutputMagazineSlotSequence)
	ON_EN_SETFOCUS(IDC_EDIT_CLAW_DELAY, &CCuEliteSettingPage::OnEnSetfocusEditClawDelay)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CCuEliteSettingPage message handlers

BOOL CCuEliteSettingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_1ST_PREHEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_1ST_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

    pWnd = GetDlgItem(IDC_STATIC_INI_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_1ST_HEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_INI_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

    pWnd = GetDlgItem(IDC_STATIC_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	//1st unit heater delay
	pWnd = GetDlgItem(IDC_STATIC_1ST_UNIT_DELAY); //@5
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_1ST_UNIT_HEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_1ST_UNIT_DELAY);
	pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	pWnd = GetDlgItem(IDC_STATIC_OPEN_CLAMP_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OPEN_CLAMP_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_OPEN_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

    pWnd = GetDlgItem(IDC_STATIC_INDEX_PR_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_PR_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_INDEX_PR_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

    pWnd = GetDlgItem(IDC_STATIC_VACUUM_DETECT);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_VACUUM_CONTROL));

	pWnd = GetDlgItem(IDC_COMBO_VACUUM_DETECT);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	pWnd = GetDlgItem(IDC_STATIC_VACUM_DELAY);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(_T("Vacuum Delay:"));

	pWnd = GetDlgItem(IDC_EDIT_VACUM_DELAY);
	pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

    pWnd = GetDlgItem(IDC_STATIC_PR_INDEX);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PR_INDEX));

	pWnd = GetDlgItem(IDC_COMBO_PR_INDEX);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	pWnd = GetDlgItem(IDC_STATIC_PR_INDEX_SCORE);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PR_INDEX_SCORE));

	pWnd = GetDlgItem(IDC_EDIT_PR_INDEX_SCORE);
	pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	pWnd = GetDlgItem(IDC_STATIC_PR_TOLERANCE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PR_INDEX_TOL));

	pWnd = GetDlgItem(IDC_EDIT_PR_INDEX_TOL);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	pWnd = GetDlgItem(IDC_STATIC_CLAW_OFFSET);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAW_OFFSET));

	pWnd = GetDlgItem(IDC_EDIT_CLAW_OFFSET);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
   
	pWnd = GetDlgItem(IDC_STATIC_INDEX_SPEED);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_SPEED));

	pWnd = GetDlgItem(IDC_COMBO_INDEX_SPEED_CUELITE);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	pWnd = GetDlgItem(IDC_STATIC_EJECT_SPEED);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_EJECT_SPEED));

	pWnd = GetDlgItem(IDC_COMBO_EJECT_SPEED_CUELITE);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

    pWnd = GetDlgItem(IDC_STATIC_STANDBY_LEN);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STANDBY_LEN_UM));

	pWnd = GetDlgItem(IDC_EDIT_X_STANDBY_LEN);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	pWnd = GetDlgItem(IDC_STATIC_LIMIT_TOLERANCE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RIGHT_CLEARANCE_UM));

	pWnd = GetDlgItem(IDC_EDIT_LIMIT_TOL);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

	//add time to auto open window clamp as idling
	pWnd = GetDlgItem(IDC_STATIC_HEAT_PROTECT);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HEAT_PROTECT_MINS));

	pWnd = GetDlgItem(IDC_EDIT_HEAT_PROTECT);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_INDEX_SPEED_CUELITE);
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_SLOWEST));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_SLOWER));
	pComboBox->InsertString(2, theMsg.GetMsg(MSG_SLOW));
	pComboBox->InsertString(3, theMsg.GetMsg(MSG_MEDIUM));
	pComboBox->InsertString(4, theMsg.GetMsg(MSG_FAST));
	pComboBox->InsertString(5, theMsg.GetMsg(MSG_FASTER));
	pComboBox->InsertString(6, theMsg.GetMsg(MSG_FASTEST));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_EJECT_SPEED_CUELITE);
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_SLOWEST));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_SLOW));
	pComboBox->InsertString(2, theMsg.GetMsg(MSG_MEDIUM));
	pComboBox->InsertString(3, theMsg.GetMsg(MSG_FAST));
	pComboBox->InsertString(4, theMsg.GetMsg(MSG_FASTEST));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PR_INDEX);
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_DISABLE));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_ENABLE));
	pComboBox->InsertString(2, theMsg.GetMsg(MSG_1ST_ONLY));

    pWnd = GetDlgItem(IDC_STATIC_OUTPUT_MAGAZINE_SLOT_SEQUENCE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OUTPUT_MAG_SLOT_SEQ));

	pWnd = GetDlgItem(IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_OUTPUT_MAGAZINE_SLOT_SEQUENCE);
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_STANDARD_SEQ));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_ALTERNATE_SEQ));

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
	CUELITE_TRACK_DATA  stTrackData;
	theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);

	pWnd = GetDlgItem(IDC_STATIC_CLAW_Delay);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PRECLAMP_CLOSE_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_CLAW_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
	m_nPageSize += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_NEXT_STEP_PUSHER_IN);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT_STEP_PUSHER_IN));

	if(stTrackData.nNextStepPusherIn>6)stTrackData.nNextStepPusherIn=2;

	pComboBox = (CComboBox*)GetDlgItem(IDC_CB_NEXT_STEP_PUSHER_IN);
    pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	for(char n=0;n<7;n++)pComboBox->AddString(CString((char)(n+0x30)));
	pComboBox->SetCurSel(stTrackData.nNextStepPusherIn);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//unit ms
	m_lHeatDelay        = stTrackData.lHeatingDelay;
	m_l1stSlipHeatDelay = stTrackData.l1stSlipHeatDelay;
	m_lIniDelay         = stTrackData.lInitDelay;
	m_iOutputMagazineSlotSequence = stTrackData.iOutputMagazineSlotSequence;
    m_l1stUnitHeatDelay = stTrackData.l1stUnitHeatDelay;
	m_lClawDelay        = stTrackData.lClawDelay;

	m_lOpenClampDelay   = stTrackData.lOpenClampDelay;
	m_lLimitTolerance   = stTrackData.lLimitTolerance;	//um
	m_lStandbyLen       = stTrackData.lLfStandbyLen;	//um
	m_nIndexSpeedCoeff  = stTrackData.nIndexSpeedCoeff -1;
	m_nEjectSpeedCoeff  = stTrackData.nEjectSpeedCoeff -1;
	m_iPrIndexEnable    = stTrackData.nPrIndexEnable;

	//unit um
	m_lPrIndexTol       = stTrackData.lPrIndexTolerance; 
	m_lIndexClawOffset  = stTrackData.lIndexPosOffset;
	m_lIndexPrDelay     = stTrackData.lIndexPrDelay;
    m_iVacuumControl    = stTrackData.bVacuumControl;
	m_lVacumDelay       = stTrackData.lVacuumDelay;
	m_sPRIndexScore     = stTrackData.sPRIndexScore;
    m_lWindowClamperOverHeatTime = round((double)stTrackData.lWindowClamperOverHeatTime / 60000);	//ms->mins

    if (iRowTop > PROPERTY_PAGE_HEIGHT)
    {
        m_nPageSize = iRowTop - PROPERTY_PAGE_HEIGHT;
		m_nPageSize+=PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        EnableScrollBarCtrl(SB_VERT, TRUE);
        SetScrollRange(SB_VERT, 0, m_nPageSize);
    }
    else
        EnableScrollBarCtrl(SB_VERT, FALSE);

	return CPropertyPage::OnInitDialog();
}

HBRUSH CCuEliteSettingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCuEliteSettingPage::OnOK()
{
	UpdateData(true);

	CUELITE_TRACK_DATA  stTrackData;
	theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);

	stTrackData.lClawDelay       = m_lClawDelay;
	stTrackData.iOutputMagazineSlotSequence = m_iOutputMagazineSlotSequence;
	stTrackData.lHeatingDelay    = m_lHeatDelay;
	stTrackData.l1stSlipHeatDelay = m_l1stSlipHeatDelay;
	stTrackData.l1stUnitHeatDelay = m_l1stUnitHeatDelay;
	stTrackData.lLimitTolerance  = m_lLimitTolerance;
	stTrackData.lSrchBackLen     = 4000;
	stTrackData.lLfStandbyLen    = m_lStandbyLen;
	stTrackData.nIndexSpeedCoeff = m_nIndexSpeedCoeff +1;
	stTrackData.nEjectSpeedCoeff = m_nEjectSpeedCoeff +1;
	stTrackData.lIndexPosOffset  = m_lIndexClawOffset;
	stTrackData.lOpenClampDelay  = m_lOpenClampDelay;
	stTrackData.lIndexPrDelay    = m_lIndexPrDelay;
	stTrackData.lInitDelay       = m_lIniDelay;
	stTrackData.lVacuumDelay     = m_lVacumDelay;

    if (m_iVacuumControl == 0)
        stTrackData.bVacuumControl    = false;
    else
        stTrackData.bVacuumControl    = true;

    stTrackData.nPrIndexEnable   = m_iPrIndexEnable;

	stTrackData.lPrIndexTolerance = m_lPrIndexTol; 

	stTrackData.sPRIndexScore = m_sPRIndexScore;
 
	stTrackData.lWindowClamperOverHeatTime = m_lWindowClamperOverHeatTime * 60000; //mins-->ms

	CWnd* pWnd=GetDlgItem(IDC_CB_NEXT_STEP_PUSHER_IN);
	stTrackData.nNextStepPusherIn=((CComboBox*)pWnd)->GetCurSel();
		
	theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackData);

	CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
	theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

	theCuEliteTrack.RefreshMeasurementData(&stCuEliteDevicePara);

	CPropertyPage::OnOK();
}

void CCuEliteSettingPage::OnEnSetfocusEditLimitTol()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();

	//theKeyPad.GetUserKeyInData(&m_lLimitTolerance, 5, 2, 20);
    theKeyPad.GetUserKeyInData(&m_lLimitTolerance, 11000, 100, 20000);

    UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditHeatDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lHeatDelay);

    UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditXStandbyLen()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    
   // theKeyPad.GetUserKeyInData(&m_lStandbyLen, 0, 0, 55); //35); //@1
    theKeyPad.GetUserKeyInData(&m_lStandbyLen, 0, 5000, 55000); //35); //@1

    UpdateData(false);
}

void CCuEliteSettingPage::OnCbnSelchangeComboIndexSpeed()
{
	UpdateData(true);
}

void CCuEliteSettingPage::OnCbnSelchangeComboEjectSpeed()
{
	UpdateData(true);
}

void CCuEliteSettingPage::OnEnSetfocusEdit1stHeatDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_l1stSlipHeatDelay);

    UpdateData(false);
}

void CCuEliteSettingPage::OnCbnSelchangeComboPrIndex()
{
	UpdateData(true);
}

void CCuEliteSettingPage::OnEnSetfocusEditPrIndexTol()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lPrIndexTol);

    UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditClawOffset()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lIndexClawOffset,8000,0,12000);

    UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditOpenDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lOpenClampDelay,0,0,6000);

    UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditIndexPrDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lIndexPrDelay,0,0,3000);

    UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditIniDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lIniDelay);

	UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditVacumDelay() //@4
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lVacumDelay);

	UpdateData(false);
}

void CCuEliteSettingPage::OnCbnSelchangeComboVacuumControl()
{
    UpdateData();
    CUELITE_TRACK_DATA stTrackRcd;
    theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

	stTrackRcd.bVacuumControl = (m_iVacuumControl == 0 ? false : true);

    theCuEliteTrack.UpdateCuEliteTrackRcd(&stTrackRcd);

    if (m_iVacuumControl == 0)
    {
        theCuEliteLFClamp.m_VacuumValve.SetRelayState(RELAY_OFF);       
    }
}

void CCuEliteSettingPage::OnEnSetfocusEdit1stUnitDelay() //@5
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_l1stUnitHeatDelay);

	UpdateData(false);
}

void CCuEliteSettingPage::OnCbnSelchangeComboOutputMagazineSlotSequence()
{
	UpdateData(true);
}

void CCuEliteSettingPage::OnEnSetfocusEditClawDelay()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lClawDelay);

	UpdateData(false);
}

void CCuEliteSettingPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    short       nOffset = 0;
    int         nMin, nMax, iCurPos = GetScrollPos(SB_VERT);
    SCROLLINFO  stScrollInfo;

    GetScrollInfo(SB_VERT, &stScrollInfo);
    nMin = stScrollInfo.nMin;
    nMax = stScrollInfo.nMax;

    switch(nSBCode)
    {
    case SB_LINEDOWN:
        {
            if (iCurPos == nMax)
                break;

            nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL;
            if (iCurPos + nOffset > nMax)
                nOffset = nMax - iCurPos;
        }
        break;

    case SB_LINEUP:
        {
            if (iCurPos == nMin)
                break;

            nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE_SMALL);
            if (iCurPos + nOffset < nMin)
                nOffset = nMin - iCurPos;
        }
        break;

    case SB_PAGEDOWN:
        {
            if (iCurPos == nMax)
                break;

            nOffset = m_nPageSize - iCurPos;
        }
        break;

    case SB_PAGEUP:
        {
            if (iCurPos == nMin)
                break;

            nOffset = -iCurPos;
        }
        break;

	case SB_THUMBTRACK:

		nOffset=nPos-iCurPos;

		break;

    default:
        break;
    }

    SetScrollPos(SB_VERT, iCurPos + nOffset);
    ScrollWindow(0, -nOffset);

    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCuEliteSettingPage::OnEnSetfocusEditHeatProtectTime()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lWindowClamperOverHeatTime);
	UpdateData(false);
}

void CCuEliteSettingPage::OnEnSetfocusEditPrIndexScore()
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_sPRIndexScore,60,20,100);

	UpdateData(false);
}

BOOL CCuEliteSettingPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta>0 ? SB_LINEUP : SB_LINEDOWN, NULL);

	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CCuEliteSettingPage::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY)->SetFocus();

	CPropertyPage::OnMouseMove(nFlags, point);
}
