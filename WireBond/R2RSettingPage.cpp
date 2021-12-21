// R2RSettingPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "R2RSettingPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MaterialHandlerThread.h"
#include "TopClamp.h"
#include "R2RIndex.h"

// CR2RSettingPage dialog

IMPLEMENT_DYNAMIC(CR2RSettingPage, CPropertyPage)

CR2RSettingPage::CR2RSettingPage()
	: CPropertyPage(CR2RSettingPage::IDD)
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
    , m_iVacuumControl(0) //@2
	, m_lVacumDelay(0) //@4
	, m_l1stUnitHeatDelay(0) //@5
	, m_sPRIndexScore(0)
{

}

CR2RSettingPage::~CR2RSettingPage()
{
}

void CR2RSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HEAT_DELAY, m_lHeatDelay);
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
	DDX_Text(pDX, IDC_EDIT_VACUM_DELAY, m_lVacumDelay); //@4
	DDX_Text(pDX, IDC_EDIT_1ST_UNIT_DELAY, m_l1stUnitHeatDelay); //@5
	DDX_Text(pDX, IDC_EDIT_PR_INDEX_SCORE, m_sPRIndexScore);
}


BEGIN_MESSAGE_MAP(CR2RSettingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_HEAT_DELAY, &CR2RSettingPage::OnEnSetfocusEditHeatDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_INDEX_SPEED, &CR2RSettingPage::OnCbnSelchangeComboIndexSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_EJECT_SPEED, &CR2RSettingPage::OnCbnSelchangeComboEjectSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_1ST_HEAT_DELAY, &CR2RSettingPage::OnEnSetfocusEdit1stHeatDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_PR_INDEX, &CR2RSettingPage::OnCbnSelchangeComboPrIndex)
	ON_EN_SETFOCUS(IDC_EDIT_PR_INDEX_TOL, &CR2RSettingPage::OnEnSetfocusEditPrIndexTol)
	ON_EN_SETFOCUS(IDC_EDIT_CLAW_OFFSET, &CR2RSettingPage::OnEnSetfocusEditClawOffset)
	ON_EN_SETFOCUS(IDC_EDIT_OPEN_DELAY, &CR2RSettingPage::OnEnSetfocusEditOpenDelay)
	ON_EN_SETFOCUS(IDC_EDIT_INDEX_PR_DELAY, &CR2RSettingPage::OnEnSetfocusEditIndexPrDelay)
//	ON_EN_CHANGE(IDC_EDIT_1ST_HEAT_DELAY, &CR2RSettingPage::OnEnChangeEdit1stHeatDelay)
    ON_EN_SETFOCUS(IDC_EDIT_INI_DELAY, &CR2RSettingPage::OnEnSetfocusEditIniDelay)
    ON_CBN_SELCHANGE(IDC_COMBO_VACUUM_DETECT, &CR2RSettingPage::OnCbnSelchangeComboVacuumControl)
    ON_WM_VSCROLL()
//  ON_STN_CLICKED(IDC_STATIC_INI_DELAY2, &CR2RSettingPage::OnStnClickedStaticIniDelay2)
    ON_EN_SETFOCUS(IDC_EDIT_VACUM_DELAY, &CR2RSettingPage::OnEnSetfocusEditVacumDelay) //@4
	ON_EN_SETFOCUS(IDC_EDIT_1ST_UNIT_DELAY, &CR2RSettingPage::OnEnSetfocusEdit1stUnitDelay) //@5
	ON_EN_SETFOCUS(IDC_EDIT_PR_INDEX_SCORE, &CR2RSettingPage::OnEnSetfocusEditPrIndexScore)
END_MESSAGE_MAP()

// CR2RSettingPage message handlers
BOOL CR2RSettingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetWindowPos(NULL, 1000, 1000, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    int iRowTop = PARA_COLUMN1_TOP + 25;

	pWnd = GetDlgItem(IDC_STATIC_1ST_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_1ST_PREHEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_1ST_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_INI_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_1ST_HEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_INI_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_HEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_HEAT_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//1st unit heater delay
	pWnd = GetDlgItem(IDC_STATIC_1ST_UNIT_DELAY);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_1ST_UNIT_HEAT_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_1ST_UNIT_DELAY);
	pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_OPEN_CLAMP_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OPEN_CLAMP_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_OPEN_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_INDEX_PR_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_PR_DELAY));

	pWnd = GetDlgItem(IDC_EDIT_INDEX_PR_DELAY);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_VACUUM_DETECT);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_VACUUM_CONTROL));

	pWnd = GetDlgItem(IDC_COMBO_VACUUM_DETECT);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_VACUM_DELAY); //@4
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(_T("Vacuum Delay:"));

	pWnd = GetDlgItem(IDC_EDIT_VACUM_DELAY);
	pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

  /*  pWnd = GetDlgItem(IDC_STATIC_INDEX); //@4
    pWnd->SetWindowPos(NULL, 10, 300, 420, 320, SWP_NOZORDER);
	theMsg.GetMsg(MSG_MENU_ITEM_LF_INDEX, szString);
    pWnd->SetWindowTextW(szString);

    iRowTop += 50;*/

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

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_PR_TOLERANCE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PR_INDEX_TOL));

	pWnd = GetDlgItem(IDC_EDIT_PR_INDEX_TOL);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_CLAW_OFFSET);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAW_OFFSET));

	pWnd = GetDlgItem(IDC_EDIT_CLAW_OFFSET);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
   
	pWnd = GetDlgItem(IDC_STATIC_INDEX_SPEED);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_SPEED));

	pWnd = GetDlgItem(IDC_COMBO_INDEX_SPEED);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_EJECT_SPEED);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_EJECT_SPEED));

	pWnd = GetDlgItem(IDC_COMBO_EJECT_SPEED);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    if (iRowTop > PROPERTY_PAGE_HEIGHT)
    {
        m_nPageSize = iRowTop - PROPERTY_PAGE_HEIGHT;

        EnableScrollBarCtrl(SB_VERT, TRUE);
        SetScrollRange(SB_VERT, 0, m_nPageSize);
    }
    else
        EnableScrollBarCtrl(SB_VERT, FALSE);

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_INDEX_SPEED);
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_SLOWEST));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_SLOWER));
	pComboBox->InsertString(2, theMsg.GetMsg(MSG_SLOW));
	pComboBox->InsertString(3, theMsg.GetMsg(MSG_MEDIUM));
	pComboBox->InsertString(4, theMsg.GetMsg(MSG_FAST));
	pComboBox->InsertString(5, theMsg.GetMsg(MSG_FASTER));
	pComboBox->InsertString(6, theMsg.GetMsg(MSG_FASTEST));


	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_EJECT_SPEED);
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_SLOWEST));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_SLOW));
	pComboBox->InsertString(2, theMsg.GetMsg(MSG_MEDIUM));
	pComboBox->InsertString(3, theMsg.GetMsg(MSG_FAST));
	pComboBox->InsertString(4, theMsg.GetMsg(MSG_FASTEST));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PR_INDEX);
	pComboBox->InsertString(0, theMsg.GetMsg(MSG_DISABLE));
	pComboBox->InsertString(1, theMsg.GetMsg(MSG_ENABLE));
	pComboBox->InsertString(2, theMsg.GetMsg(MSG_1ST_ONLY));

	TOP_TRACK_DATA stTrackRcd;
	theR2RTrack.GetTopTrackRcd(&stTrackRcd);

	//unit ms
	m_lHeatDelay        = stTrackRcd.lHeatingDelay;
	m_l1stSlipHeatDelay = stTrackRcd.l1stSlipHeatDelay;
	m_lIniDelay = stTrackRcd.lInitDelay; //@5

    m_l1stUnitHeatDelay = stTrackRcd.l1stUnitHeatDelay; //@5

	m_lOpenClampDelay   = stTrackRcd.lOpenClampDelay;

	//unit mm
	m_lLimitTolerance   = stTrackRcd.lLimitTolerance/1000;
	//m_lSrchBackLen      = stTrackRcd.lSrchBackLen/1000; 
	m_lStandbyLen       = stTrackRcd.lLfStandbyLen/1000;

	m_iIndexSpeedCo     = stTrackRcd.nIndexSpeedCoeff - 1;
	m_iEjectSpeed       = stTrackRcd.nEjectSpeedCoeff - 1;
	m_iPrIndexEnable    = stTrackRcd.nPrIndexEnable;

	//unit um
	m_lPrIndexTol       = stTrackRcd.lPrIndexTolerance; 
	m_lIndexClawOffset  = stTrackRcd.lIndexPosOffset;

	m_lIndexPrDelay     =stTrackRcd.lIndexPrDelay;


	//m_lIniDelay = stTrackRcd.lInitDelay; //@5

    m_iVacuumControl = stTrackRcd.bVacuumControl; //@2
		
	m_lVacumDelay = stTrackRcd.lVacuumDelay; //@4

	m_sPRIndexScore = stTrackRcd.sPRIndexScore;
		
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CR2RSettingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	// TODO:  Change any attributes of the DC here
	   HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CR2RSettingPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(true);

	TOP_TRACK_DATA  stTrackRcd;
	theR2RTrack.GetTopTrackRcd(&stTrackRcd);

	stTrackRcd.lHeatingDelay    = m_lHeatDelay;
	stTrackRcd.l1stSlipHeatDelay = m_l1stSlipHeatDelay;
	stTrackRcd.l1stUnitHeatDelay =m_l1stUnitHeatDelay; //@5

	stTrackRcd.lLimitTolerance  = m_lLimitTolerance*1000;
	stTrackRcd.lSrchBackLen     = 4000;
	stTrackRcd.lLfStandbyLen    = m_lStandbyLen*1000;

	stTrackRcd.nIndexSpeedCoeff = m_iIndexSpeedCo + 1;
	stTrackRcd.nEjectSpeedCoeff = m_iEjectSpeed + 1;
	stTrackRcd.lIndexPosOffset  = m_lIndexClawOffset;
	stTrackRcd.lOpenClampDelay  = m_lOpenClampDelay;
	stTrackRcd.lIndexPrDelay    = m_lIndexPrDelay;
	stTrackRcd.lInitDelay       = m_lIniDelay;
	stTrackRcd.lVacuumDelay     = m_lVacumDelay; //@4

    if (m_iVacuumControl == 0) //@2
        stTrackRcd.bVacuumControl    = false;
    else
        stTrackRcd.bVacuumControl    = true;

    stTrackRcd.nPrIndexEnable= m_iPrIndexEnable & 0xff;

	stTrackRcd.lPrIndexTolerance = m_lPrIndexTol; 

	stTrackRcd.sPRIndexScore = m_sPRIndexScore;
		
	theR2RTrack.UpdateTopTrackRcd(&stTrackRcd);

	TOP_DEVICE_RECORD  stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);

	theR2RTrack.RefreshMeasurementData(&stDeviceRcd);

	CPropertyPage::OnOK();
}

void CR2RSettingPage::OnEnSetfocusEditLimitTol()
{
	// TODO: Add your control notification handler code here

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lLimitTolerance, 5, 2, 20);

    UpdateData(false);
	
}


void CR2RSettingPage::OnEnSetfocusEditHeatDelay()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lHeatDelay);

    UpdateData(false);

	

}

//void CR2RSettingPage::OnEnSetfocusEditXBack()
//{
//	// TODO: Add your control notification handler code here
//	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
//    pWnd->SetFocus();
//    
//    theKeyPad.GetUserKeyInData(&m_lSrchBackLen, 4, 0, 20);
//
//    UpdateData(false);
//
//}

void CR2RSettingPage::OnEnSetfocusEditXStandbyLen()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lStandbyLen, 0, 0, 55); //35); //@1


    UpdateData(false);
}

void CR2RSettingPage::OnCbnSelchangeComboIndexSpeed()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CR2RSettingPage::OnCbnSelchangeComboEjectSpeed()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CR2RSettingPage::OnEnSetfocusEdit1stHeatDelay()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_l1stSlipHeatDelay);


    UpdateData(false);

	
}

void CR2RSettingPage::OnCbnSelchangeComboPrIndex()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CR2RSettingPage::OnEnSetfocusEditPrIndexTol()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lPrIndexTol);

    UpdateData(false);

	
}

void CR2RSettingPage::OnEnSetfocusEditClawOffset()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lIndexClawOffset,8000,0,12000);

    UpdateData(false);

}

void CR2RSettingPage::OnEnSetfocusEditOpenDelay()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lOpenClampDelay,0,0,10000);
    UpdateData(false);
}

void CR2RSettingPage::OnEnSetfocusEditIndexPrDelay()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_lIndexPrDelay,0,0,3000);

    UpdateData(false);
}

void CR2RSettingPage::OnEnSetfocusEditIniDelay()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lIniDelay);

	UpdateData(false);


}

void CR2RSettingPage::OnEnSetfocusEditVacumDelay() //@4
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lVacumDelay);

	UpdateData(false);
	
}

void CR2RSettingPage::OnCbnSelchangeComboVacuumControl()
{
    UpdateData();

    if (m_iVacuumControl == 0)
		theTopClamp.SetVacuumOn(false);	//v3.0.T378 edit
}

void CR2RSettingPage::OnEnSetfocusEdit1stUnitDelay()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_l1stUnitHeatDelay);

	UpdateData(false);
}

void CR2RSettingPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
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

            nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
            if (iCurPos + nOffset > nMax)
                nOffset = nMax - iCurPos;
        }
        break;

    case SB_LINEUP:
        {
            if (iCurPos == nMin)
                break;

            nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE);
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

    default:
        break;
    }

    SetScrollPos(SB_VERT, iCurPos + nOffset);
    ScrollWindow(0, -nOffset);

    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CR2RSettingPage::OnEnSetfocusEditPrIndexScore()
{
	// TODO: 在此添加控件通知处理程序代码
}
