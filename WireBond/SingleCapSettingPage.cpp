/***************************************************************
SingleCapSettingPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "SingleCapSettingPage.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledClamp.h"
#include "VledOffload.h"
#include "VEIndex.h"
#include "VEClamp.h"
#include "VEOnload.h"
#include "User.h"

// CSingleCapSettingPage dialog

IMPLEMENT_DYNAMIC(CSingleCapSettingPage, CPropertyPage)

CSingleCapSettingPage::CSingleCapSettingPage()
	: CPropertyPage(CSingleCapSettingPage::IDD)
	, m_nInitUnit(1)
	, m_nIndexUnit(8)
	, m_nOutputSteps(9)
	, m_nOpenPretrig(20)
	, m_nPreClampTime(100)
	, m_iLfUnits(20)
	, m_nForkBuff(200)
	, m_nSwitchBuffer(50)
	, m_nEndOffsetUnit(0)
	, m_nPreheatTime(0)
	, m_nAvoidCrashPulse(710)
    , m_nNormalLfPreheatTime(0) //@6
	, m_nAvoidCrashPulseCompen(0)
    , m_nLfPickDelay(0)
	, m_lOvershootLen(0)
	, m_nOnloaderType(ONLOADER_NORMAL_LF)
{
}

CSingleCapSettingPage::~CSingleCapSettingPage()
{
}

void CSingleCapSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPEN_PRETRIG, m_nOpenPretrig);
	DDX_Text(pDX, IDC_EDIT_PRECLAMP_TIME, m_nPreClampTime);
	DDX_CBIndex(pDX, IDC_COMBO_LF_UNITS, m_iLfUnits);
	DDX_Text(pDX, IDC_EDIT_OFFLOAD_FORK_BUFF, m_nForkBuff);
	DDX_Text(pDX, IDC_EDIT_SWITCH_BUFF, m_nSwitchBuffer);
	DDX_Text(pDX, IDC_EDIT_PREHEAT_TIME, m_nPreheatTime);
	DDX_Text(pDX, IDC_EDIT_NORMAL_LF_PREHEAT_TIME, m_nNormalLfPreheatTime);
	DDX_Text(pDX, IDC_EDIT_AVOID_CRASH_TUNE, m_nAvoidCrashPulseCompen);
	DDX_Text(pDX, IDC_EDIT_LF_PICK_DELAY, m_nLfPickDelay);
	DDX_Text(pDX, IDC_EDIT_LF_PICK_OVERSHOOT, m_lOvershootLen);
	DDX_CBIndex(pDX, IDC_COMBO_COPPER_LF, m_nOnloaderType);
}

BEGIN_MESSAGE_MAP(CSingleCapSettingPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_OPEN_PRETRIG, &CSingleCapSettingPage::OnEnSetfocusEditOpenPretrig)
	ON_EN_SETFOCUS(IDC_EDIT_PRECLAMP_TIME, &CSingleCapSettingPage::OnEnSetfocusEditPreclampTime)
	ON_CBN_SELCHANGE(IDC_COMBO_LF_UNITS, &CSingleCapSettingPage::OnCbnSelchangeComboLfUnits)
    ON_EN_SETFOCUS(IDC_EDIT_OFFLOAD_FORK_BUFF, &CSingleCapSettingPage::OnEnSetfocusEditOffloadForkBuff)
	ON_EN_SETFOCUS(IDC_EDIT_SWITCH_BUFF, &CSingleCapSettingPage::OnEnSetfocusEditSwitchBuff)
	ON_EN_SETFOCUS(IDC_EDIT_PREHEAT_TIME, &CSingleCapSettingPage::OnEnSetfocusEditPreheatTime)
    ON_EN_SETFOCUS(IDC_EDIT_NORMAL_LF_PREHEAT_TIME, &CSingleCapSettingPage::OnEnSetfocusEditNormalLfPreheatTime)
	ON_EN_SETFOCUS(IDC_EDIT_AVOID_CRASH_TUNE, &CSingleCapSettingPage::OnEnSetfocusEditAvoidCrashTune)
    ON_EN_SETFOCUS(IDC_EDIT_LF_PICK_DELAY, &CSingleCapSettingPage::OnEnSetfocusEditLfPickDelay)
	ON_EN_SETFOCUS(IDC_EDIT_LF_PICK_OVERSHOOT, &CSingleCapSettingPage::OnEnSetfocusEditLfPickOvershoot)
	ON_CBN_SELCHANGE(IDC_COMBO_COPPER_LF, &CSingleCapSettingPage::OnCbnSelchangeComboCopperLf)
END_MESSAGE_MAP()

// CSingleCapSettingPage message handlers
BOOL CSingleCapSettingPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetWindowPos(NULL, 1000, 1000, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	pWnd = GetDlgItem(IDC_STATIC_GENERAL);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_GENERAL));

	pWnd = GetDlgItem(IDC_STATIC_LF_UNIT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_TIMES));

	pWnd = GetDlgItem(IDC_STATIC_FORK_BUFF);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FORK_CAPACITY));

	pWnd = GetDlgItem(IDC_STATIC_INDEX);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX));

	pWnd = GetDlgItem(IDC_STATIC_PREHEAT_TIME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_1ST_LF_PREHEAT_TIME));

	pWnd = GetDlgItem(IDC_STATIC_NORMAL_LF_PREHEAT_TIME);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NORMAL_LF_PREHEAT_TIME));

    pWnd = GetDlgItem(IDC_STATIC_PRETRIG_OPEN);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAMP_OPEN_PRETRIG_CLAW));

	pWnd = GetDlgItem(IDC_STATIC_PRETRIG_CLOSE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CLAW_PRETRIG_CLAMP_CLOSE));

	pWnd = GetDlgItem(IDC_STATIC_FORK_SWITCH_BUFF);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FORK_SWITCH_BUFFER));

	pWnd = GetDlgItem(IDC_STATIC_AVOID_CRASH_PULSE_COMPEN);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INCOMPLETE_ROTATE_COMPENSATION));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LF_UNITS);

	pComboBox->InsertString(0,_T("4"));
	pComboBox->InsertString(1,_T("5"));
	pComboBox->InsertString(2,_T("6"));
	pComboBox->InsertString(3,_T("7"));
   
    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COPPER_LF); 
    pComboBox->InsertString(0, _T("NO"));
    pComboBox->InsertString(1, _T("YES"));

	SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteTrack.GetRcd(&pstLfTrackRcd);
		break;
	case MHS_VLED:
		theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
		break;
	}

	m_nInitUnit            = pstLfTrackRcd.nInitUnitNum;
	m_nIndexUnit           = pstLfTrackRcd.nIndexSteps;
	m_nOutputSteps         = pstLfTrackRcd.nOutputSteps;
 	m_nPreClampTime        = pstLfTrackRcd.nClawActionRate;
	m_nEndOffsetUnit       = pstLfTrackRcd.nEndBondOffset;
	m_nPreheatTime         = pstLfTrackRcd.nPreheatTime; //@4
    m_nNormalLfPreheatTime = pstLfTrackRcd.nNormalLFPreheatTime; //@6

    if (pstLfTrackRcd.nLeadFrameUnits == 4)
        m_iLfUnits = 0;
    else if (pstLfTrackRcd.nLeadFrameUnits == 5)
        m_iLfUnits = 1;
	else if(pstLfTrackRcd.nLeadFrameUnits == 6)
		m_iLfUnits = 2;
	else if(pstLfTrackRcd.nLeadFrameUnits == 7) //@8
		m_iLfUnits = 3;
	else
		m_iLfUnits = 0;

	SINGLE_CAP_CLAMP_RCD pstLfClampRcd;
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteLfClamp.GetRcd(&pstLfClampRcd);
		break;
	case MHS_VLED:
		theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
		break;
	}

    m_nOpenPretrig           = pstLfClampRcd.nOpenPretrigRate;
	m_nAvoidCrashPulseCompen = pstLfTrackRcd.nAvoidCrashCompen ;
	m_nAvoidCrashPulse       = pstLfTrackRcd.nAvoidCrashPulse - m_nAvoidCrashPulseCompen;

	FORK_OFFLOAD_RCD pstLfOffloadRcd;
	theVledOffload.GetForkOffloadRcd(&pstLfOffloadRcd);
	m_nForkBuff     = pstLfOffloadRcd.nForkCapacity;
	m_nSwitchBuffer = pstLfOffloadRcd.nForkSwitchBuffer; 
	
    LEADFRAME_ONLOAD_RCD stLfOnloadRcd;
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOnload.GetRcd(&stLfOnloadRcd);
		break;
	case MHS_VLED:
		theVledOnload.GetLfOnloadRcd(&stLfOnloadRcd);
		break;
	}

    m_nLfPickDelay  = stLfOnloadRcd.nPickerDelayTime;
	m_lOvershootLen = stLfOnloadRcd.lOvershootLen;
	m_nOnloaderType = stLfOnloadRcd.bOnloaderType;

	if (theUser.GetUserLevel() > USER_PROCESS) //@7
    {
		pWnd = GetDlgItem(IDC_STATIC_LF_TYPE);
        pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_STATIC_COPPER_LF);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_COMBO_COPPER_LF);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_OPEN_PRETRIG); //@9
        pWnd->EnableWindow(TRUE);

        pWnd = GetDlgItem(IDC_EDIT_PRECLAMP_TIME);
        pWnd->EnableWindow(TRUE);
        
        pWnd = GetDlgItem(IDC_STATIC_LF_PICK_DELAY);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_LF_PICK_DELAY);
        pWnd->ShowWindow(SW_SHOWNORMAL);

		pWnd = GetDlgItem(IDC_STATIC_PICKER_OVERSHOOT);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_LF_PICK_OVERSHOOT);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
		pWnd = GetDlgItem(IDC_STATIC_LF_TYPE);
        pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_STATIC_COPPER_LF);
        pWnd->ShowWindow(SW_HIDE);

        pWnd = GetDlgItem(IDC_COMBO_COPPER_LF);
        pWnd->ShowWindow(SW_HIDE);

        pWnd = GetDlgItem(IDC_EDIT_OPEN_PRETRIG); //@9
        pWnd->EnableWindow(FALSE);

        pWnd = GetDlgItem(IDC_EDIT_PRECLAMP_TIME);
        pWnd->EnableWindow(FALSE);

        pWnd = GetDlgItem(IDC_STATIC_LF_PICK_DELAY);
        pWnd->ShowWindow(SW_HIDE);

        pWnd = GetDlgItem(IDC_EDIT_LF_PICK_DELAY);
        pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_STATIC_PICKER_OVERSHOOT);
        pWnd->ShowWindow(SW_HIDE);

        pWnd = GetDlgItem(IDC_EDIT_LF_PICK_OVERSHOOT);
        pWnd->ShowWindow(SW_HIDE);
    }

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSingleCapSettingPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CSingleCapSettingPage::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData(true);

	SINGLE_CAP_TRACK_RCD pstLfTrackRcd;
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteTrack.GetRcd(&pstLfTrackRcd);
		break;
	case MHS_VLED:
		theVledTrack.GetLfTrackRcd(&pstLfTrackRcd);
		break;
	}

	pstLfTrackRcd.nInitUnitNum         = m_nInitUnit;
	pstLfTrackRcd.nIndexSteps          = m_nIndexUnit;
	pstLfTrackRcd.nOutputSteps         = m_nOutputSteps;
	pstLfTrackRcd.nClawActionRate      = m_nPreClampTime;
	pstLfTrackRcd.nEndBondOffset       = m_nEndOffsetUnit;
	pstLfTrackRcd.nPreheatTime         = m_nPreheatTime;   //@4
    pstLfTrackRcd.nNormalLFPreheatTime = m_nNormalLfPreheatTime; //@6
	pstLfTrackRcd.nAvoidCrashPulse     = m_nAvoidCrashPulse +m_nAvoidCrashPulseCompen;
	pstLfTrackRcd.nAvoidCrashCompen    = m_nAvoidCrashPulseCompen;


    if (m_iLfUnits == 0)
        pstLfTrackRcd.nLeadFrameUnits = 4;
    else if (m_iLfUnits == 1)
        pstLfTrackRcd.nLeadFrameUnits = 5;
	else if (m_iLfUnits == 2)
        pstLfTrackRcd.nLeadFrameUnits = 6;
	else if (m_iLfUnits == 3) //@8
        pstLfTrackRcd.nLeadFrameUnits = 7;

	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteTrack.SetRcd(&pstLfTrackRcd);
		break;
	case MHS_VLED:
		theVledTrack.UpdateLfTrackRcd(&pstLfTrackRcd);
		break;
	}

	SINGLE_CAP_CLAMP_RCD pstLfClampRcd;
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteLfClamp.GetRcd(&pstLfClampRcd);
		pstLfClampRcd.nOpenPretrigRate = m_nOpenPretrig;
		theVEliteLfClamp.SetRcd(&pstLfClampRcd);
		break;
	case MHS_VLED:
		theVledLfClamp.GetLfClampRcd(&pstLfClampRcd);
		pstLfClampRcd.nOpenPretrigRate = m_nOpenPretrig;
		theVledLfClamp.UpdateLfClampRcd(&pstLfClampRcd);
		break;
	}

	FORK_OFFLOAD_RCD pstLfOffloadRcd;
	theVledOffload.GetForkOffloadRcd(&pstLfOffloadRcd);
	pstLfOffloadRcd.nForkCapacity     = m_nForkBuff;
	pstLfOffloadRcd.nForkSwitchBuffer = m_nSwitchBuffer;
	theVledOffload.UpdateForkOffloadRcd(&pstLfOffloadRcd);

    LEADFRAME_ONLOAD_RCD stLfOnloadRcd;
	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
		theVEliteOnload.GetRcd(&stLfOnloadRcd);
		stLfOnloadRcd.nPickerDelayTime = m_nLfPickDelay;
		stLfOnloadRcd.lOvershootLen    = m_lOvershootLen;
		stLfOnloadRcd.bOnloaderType    = m_nOnloaderType;
		theVEliteOnload.SetRcd(&stLfOnloadRcd);
		break;
	case MHS_VLED:
		theVledOnload.GetLfOnloadRcd(&stLfOnloadRcd);
		stLfOnloadRcd.nPickerDelayTime = m_nLfPickDelay;
		stLfOnloadRcd.lOvershootLen    = m_lOvershootLen;
		stLfOnloadRcd.bOnloaderType    = m_nOnloaderType;
		theVledOnload.UpdateLfOnloadRcd(&stLfOnloadRcd);
		break;
	}

	CPropertyPage::OnOK();
}

void CSingleCapSettingPage::OnEnSetfocusEditOpenPretrig()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nOpenPretrig, 100, 0, 100); //@9

    UpdateData(false);

}

void CSingleCapSettingPage::OnEnSetfocusEditPreclampTime()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nPreClampTime, 57, 40, 60); //@9

    UpdateData(false);
}

void CSingleCapSettingPage::OnCbnSelchangeComboLfUnits()
{
	// TODO: Add your control notification handler code here
    UpdateData(true);

    switch(m_iLfUnits)
    {
    case 0:
        m_nInitUnit = 0;
        m_nIndexUnit = 3;
        m_nOutputSteps = 2;
        m_nAvoidCrashPulse = 705;
        break;
    case 1:
        m_nInitUnit = 0;
        m_nIndexUnit = 3;
        m_nOutputSteps = 3;
        m_nAvoidCrashPulse = 750;
        break;
    case 2:
        m_nInitUnit = 0;
        m_nIndexUnit = 3;
        m_nOutputSteps = 4;
        m_nAvoidCrashPulse = 750;
        break;
    case 3: //@8
        m_nInitUnit = 0;
        m_nIndexUnit = 3;
        m_nOutputSteps = 5;
        m_nAvoidCrashPulse = 705;
        break;
    default:
        break;
    }
}

void CSingleCapSettingPage::OnEnSetfocusEditOffloadForkBuff()
{
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nForkBuff, 200, 50, 300);

    UpdateData(false);
}

void CSingleCapSettingPage::OnEnSetfocusEditSwitchBuff()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nSwitchBuffer, 50, 1, 300);	//can not be 0

    UpdateData(false);
}

void CSingleCapSettingPage::OnEnSetfocusEditPreheatTime()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nPreheatTime, 2000, 0, 5000);   //@3,can not be 0

    UpdateData(false);
}

void CSingleCapSettingPage::OnEnSetfocusEditNormalLfPreheatTime() //@6
{
    // TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nNormalLfPreheatTime, 0, 0, 3000);

    UpdateData(false);
}

void CSingleCapSettingPage::OnEnSetfocusEditAvoidCrashTune()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nAvoidCrashPulseCompen, 0, -25, 25);

	UpdateData(false);
}

void CSingleCapSettingPage::OnEnSetfocusEditLfPickDelay() //@7
{
    // TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nLfPickDelay, 200, 200, 2000);

	UpdateData(false);
}

void CSingleCapSettingPage::OnEnSetfocusEditLfPickOvershoot()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
	pWnd->SetFocus();

	theKeyPad.GetUserKeyInData(&m_lOvershootLen, 0, 0, 100);

	UpdateData(false);
}

void CSingleCapSettingPage::OnCbnSelchangeComboCopperLf()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}
