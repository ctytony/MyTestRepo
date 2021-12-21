/***************************************************************
FireLevelDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MacDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "Program.h"
#include "TeachRecipeSheet.h"
#include "FireLevelPage.h"
#include "servo.h"
#include "Busy.h"
#include "EfoProfile.h"
#include "MachineConfig.h"

// CFireLevelDlg dialog
IMPLEMENT_DYNAMIC(CFireLevelPage, CPropertyPage)

CFireLevelPage::CFireLevelPage()
	: CPropertyPage(CFireLevelPage::IDD)
    , m_nCurFireLvl(3000)
    , m_nNewFireLvl(3000)
    , m_nTailLen(0)
    , m_bAllowAdjust(false)
    , m_nAdjustStep(10)
{
	m_bBusy = FALSE;	//V1.1.W153 add
}

CFireLevelPage::~CFireLevelPage()
{
}

void CFireLevelPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_CUR_LVL, m_nCurFireLvl);
    DDX_Text(pDX, IDC_EDIT_NEW_LVL, m_nNewFireLvl);
    DDX_Text(pDX, IDC_EDIT_TAIL_LENGTH, m_nTailLen);
    DDX_Text(pDX, IDC_EDIT_STEP, m_nAdjustStep);
}

BEGIN_MESSAGE_MAP(CFireLevelPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_UP, OnUp)
    ON_BN_CLICKED(IDC_BUTTON_DOWN, OnDown)
    ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
    ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnConfirm)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancel)
    ON_EN_CHANGE(IDC_EDIT_NEW_LVL, OnEditNewLvl)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_LENGTH, OnFocusTailLength)
    ON_EN_SETFOCUS(IDC_EDIT_CUR_LVL, OnFocusCurLvl)
    ON_EN_SETFOCUS(IDC_EDIT_NEW_LVL, OnFocusNewLvl)
    ON_EN_SETFOCUS(IDC_EDIT_STEP, OnFocusStep)
    ON_WM_DESTROY()
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W153 add
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CFireLevelPage::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CFireLevelDlg message handlers
BOOL CFireLevelPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_BUTTON_UP, theMsg.GetMsg(MSG_UP));
    SetDlgItemTextW(IDC_BUTTON_DOWN, theMsg.GetMsg(MSG_DOWN));
    SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_CONFIRM, theMsg.GetMsg(MSG_CONFIRM));
    SetDlgItemTextW(IDC_BUTTON_CANCEL, theMsg.GetMsg(MSG_CANCEL));
    SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));
    SetDlgItemTextW(IDC_STATIC_PARAMETER, theMsg.GetMsg(MSG_PARAMETERS));
    SetDlgItemTextW(IDC_STATIC_TAIL_LENGTH, theMsg.GetMsg(MSG_TAIL_LENGTH));
    SetDlgItemTextW(IDC_STATIC_CUR_FIRE_LEVEL, theMsg.GetMsg(MSG_CUR_FIRE_LEVEL));
    SetDlgItemTextW(IDC_STATIC_NEW_FIRE_LEVEL, theMsg.GetMsg(MSG_NEW_FIRE_LEVEL));
    SetDlgItemTextW(IDC_STATIC_ADJUST_STEP, theMsg.GetMsg(MSG_ADJUST_STEP));

    m_nNewFireLvl = m_nCurFireLvl = round((theMcData.GetResetLevel() - theMcData.GetFlatLevel()) * theServo.GetResolZ());
	//m_nTailLen = thePgm.GetRcd()->nTailLengthUm;	//v3.1.T60 delete
	m_nTailLen = theEfoProfile.GetProfile(0)->nTailLen_x1um;	//v3.1.T60 add

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->AddString(theMsg.GetMsg(MSG_START_TO_TEACH_FIRE_LEVEL));

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
        pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(0);
        pSheet->SetWizardButtons(PSWIZB_FINISH);
        pSheet->GetDlgItem(ID_WIZFINISH)->EnableWindow(0);
    }
  
    return CPropertyPage::OnInitDialog();
}

HBRUSH CFireLevelPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CFireLevelPage::OnUp()
{
	if (m_bAllowAdjust == false) return;
	if (m_nNewFireLvl + m_nAdjustStep > theBondPara.GetBondParaLimit()->nFireLevelH) return;

	CBusy bs(&m_bBusy);	//V1.1.W153 add

	UpdateData(true);
	theServo.MoveAxisZ(round(m_nAdjustStep / theServo.GetResolZ()), MOTION_RELATIVE, MOTION_WITH_WAIT);

	double dCurPos;
	theServo.GetBondHeadPosition(&dCurPos);
	dCurPos = dCurPos - theMcData.GetFlatLevel();
	m_nNewFireLvl = round(dCurPos * theServo.GetResolZ()) + m_nTailLen;
	UpdateData(false);
}

void CFireLevelPage::OnDown()
{
	if (m_bAllowAdjust == false) return;
	if (m_nNewFireLvl - m_nAdjustStep < theBondPara.GetBondParaLimit()->nFireLevelL) return;

	CBusy bs(&m_bBusy);	//V1.1.W153 add

	UpdateData(true);
	theServo.MoveAxisZ(-round(m_nAdjustStep / theServo.GetResolZ()), MOTION_RELATIVE, MOTION_WITH_WAIT);

	double dCurPos;
	theServo.GetBondHeadPosition(&dCurPos);
	dCurPos = dCurPos - theMcData.GetFlatLevel();
	m_nNewFireLvl = round(dCurPos * theServo.GetResolZ()) + m_nTailLen;
	UpdateData(false);
}

void CFireLevelPage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    m_bAllowAdjust = true;
    
	MACHINE_DATA *pstMcData = theMcData.GetRcd();
    long lMoveDist = pstMcData->lResetLevel - round((double)m_nTailLen / theServo.GetResolZ());
    theServo.MoveAxisZ(lMoveDist, MOTION_ABSOLUTE, MOTION_WITH_WAIT);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_CONFIRM)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_NEW_LVL)->EnableWindow(1);

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_UP_DOWN_TO_ADJUST_FIRE_LEVEL));
    pListBox->AddString(theMsg.GetMsg(MSG_KEY_IN_FIRE_LEVEL));
    pListBox->AddString(theMsg.GetMsg(MSG_CONFIRM_TO_UPDATE_FIRE_LEVEL));
}

void CFireLevelPage::OnConfirm()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    m_bAllowAdjust = false;
    double dCurPos;
	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
    {
        theServo.GetBondHeadPosition(&dCurPos);
        m_nNewFireLvl = round((dCurPos - theMcData.GetFlatLevel()) * theServo.GetResolZ()) + m_nTailLen;
        UpdateData(false);
    }
    else
        dCurPos = m_nNewFireLvl;

    long lMoveDist = round(dCurPos + m_nTailLen / theServo.GetResolZ());
    theServo.MoveAxisZ(lMoveDist, MOTION_ABSOLUTE, MOTION_WITH_WAIT);

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	pstMcData->lResetLevel = lMoveDist;

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_START_TO_TEACH_FIRE_LEVEL));

    GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_CONFIRM)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(0);
	GetDlgItem(IDC_EDIT_NEW_LVL)->EnableWindow(0);

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
        pSheet->GetDlgItem(ID_WIZFINISH)->EnableWindow(1);
}

void CFireLevelPage::OnCancel()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    m_bAllowAdjust = false;

    theServo.MoveAxisZ(theMcData.GetResetLevel(), MOTION_ABSOLUTE, MOTION_WITH_WAIT);
    m_nNewFireLvl = round((theMcData.GetResetLevel() - theMcData.GetFlatLevel()) * theServo.GetResolZ());

    UpdateData(false);

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_START_TO_TEACH_FIRE_LEVEL));

    GetDlgItem(IDC_BUTTON_START)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON_CONFIRM)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(0);
	GetDlgItem(IDC_EDIT_NEW_LVL)->EnableWindow(0);
}

void CFireLevelPage::OnEditNewLvl()
{
	if (m_bAllowAdjust == false) return;

	CBusy bs(&m_bBusy);	//V1.1.W153 add

	UpdateData(true);

	long lAbsLv = round(theMcData.GetFlatLevel() * theServo.GetResolZ()) + m_nNewFireLvl;
	long lMoveDist =  round((lAbsLv - m_nTailLen) / theServo.GetResolZ());
	theServo.MoveAxisZ(lMoveDist, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
}

void CFireLevelPage::OnFocusTailLength()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PARAMETER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nTailLen);
    UpdateData(false);
}

void CFireLevelPage::OnFocusCurLvl()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PARAMETER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nCurFireLvl);
    UpdateData(false);
}

void CFireLevelPage::OnFocusNewLvl()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PARAMETER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nNewFireLvl, 5000,
				theBondPara.GetBondParaLimit()->nFireLevelL,
				theBondPara.GetBondParaLimit()->nFireLevelH, true);
    UpdateData(false);
    OnEditNewLvl();
}

void CFireLevelPage::OnFocusStep()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PARAMETER)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nAdjustStep, 20, 10, 100);
    UpdateData(false);
}

void CFireLevelPage::OnDestroy()
{
    theApp.ArchiveMachineData(ARCHIVE_SAVE);
	CPropertyPage::OnDestroy();
}

LRESULT CFireLevelPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W153 add
{
	return m_bBusy;
}

void CFireLevelPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_RECIPE, 0);	//v3.0.T455 add
}
