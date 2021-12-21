/***************************************************************
BtosPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "Program.h"
#include "Reference.h"
#include "TeachRecipeSheet.h"
#include "BtosPage.h"
#include "math.h"
#include "Bond.h"
#include "servo.h"
#include "Busy.h"

// CBtosPage dialog
IMPLEMENT_DYNAMIC(CBtosPage, CPropertyPage)

CBtosPage::CBtosPage()
	: CPropertyPage(CBtosPage::IDD)
    , m_lNewBtosX(0)
    , m_lNewBtosY(0)
    , m_nSrchSpeed(3)
    , m_nSrchTol(30)
    , m_nBtosStage(0)
    , m_nAdjustStep(10)
    , m_bAllowAdjust(false)
	, m_bVerify(false)
{
	m_bBusy = FALSE;	//V1.1.W153 add
}

CBtosPage::~CBtosPage()
{
}

void CBtosPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BTOS_NEW_X, m_lNewBtosX);
	DDX_Text(pDX, IDC_BTOS_NEW_Y, m_lNewBtosY);
	DDX_Text(pDX, IDC_EDIT_STEP, m_nAdjustStep);
}

BEGIN_MESSAGE_MAP(CBtosPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnUp)
	ON_EN_CHANGE(IDC_BTOS_NEW_X, OnEditBtosNewX)
	ON_EN_CHANGE(IDC_BTOS_NEW_Y, OnEditBtosNewY)
	ON_EN_SETFOCUS(IDC_BTOS_NEW_X, OnFocusBtosNewX)
	ON_EN_SETFOCUS(IDC_BTOS_NEW_Y, OnFocusBtosNewY)
	ON_EN_SETFOCUS(IDC_EDIT_STEP, OnFocusEditStep)
	ON_BN_CLICKED(IDC_BUTTON_CORRECT, OnCorrect)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_XY, OnVerify)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W153 add
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CBtosPage::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CBtosPage message handlers
BOOL CBtosPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	SetDlgItemInt(IDC_BTOS_CUR_X, round(pstMcData->stBondTipOs.dx));
	SetDlgItemInt(IDC_BTOS_CUR_Y, round(pstMcData->stBondTipOs.dy));
	SetDlgItemTextW(IDC_CUR_BTOS, theMsg.GetMsg(MSG_BTOS_CURRENT));
	SetDlgItemTextW(IDC_NEW_BTOS, theMsg.GetMsg(MSG_BTOS_NEW));
	SetDlgItemTextW(IDC_BUTTON_DOWN, theMsg.GetMsg(MSG_DOWN));
	SetDlgItemTextW(IDC_BUTTON_UP, theMsg.GetMsg(MSG_UP));
	SetDlgItemTextW(IDC_BUTTON_MOVE_XY, theMsg.GetMsg(MSG_MOVE));
	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_BOND));
	SetDlgItemTextW(IDC_BUTTON_CORRECT, theMsg.GetMsg(MSG_OK));
	SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));
	SetDlgItemTextW(IDC_STATIC_ADJUST_STEP, theMsg.GetMsg(MSG_ADJUST_STEP));
	SetDlgItemTextW(IDC_STATIC_PARAMETER, theMsg.GetMsg(MSG_PARAMETERS));

	m_lNewBtosX = round(pstMcData->stBondTipOs.dx);
	m_lNewBtosY = round(pstMcData->stBondTipOs.dy);

	m_bAllowAdjust = true;
	m_bVerify = false;

	StartAndVerifyHandle();

	CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
	if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
		psheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(false);

	return CPropertyPage::OnInitDialog();
}

HBRUSH CBtosPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CBtosPage::OnDown()
{
	if (!m_bAllowAdjust) return;
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	UpdateData(true);

	long lMoveDist = -round(m_nAdjustStep / theServo.GetResolZ());
	theServo.MoveAxisZ(lMoveDist, MOTION_RELATIVE, MOTION_WITH_WAIT);

	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow();
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow();
}

void CBtosPage::OnUp()
{
	if (!m_bAllowAdjust) return;
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	UpdateData(true);

	long lMoveDist = round(m_nAdjustStep / theServo.GetResolZ());
	theServo.MoveAxisZ(lMoveDist, MOTION_RELATIVE, MOTION_WITH_WAIT);

	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow();
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow();
}

void CBtosPage::OnEditBtosNewX()
{
	UpdateData(true);
	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	pstMcData->stBondTipOs.dx = m_lNewBtosX;
	theRef.UpdateAllBtos(pstMcData->stBondTipOs);
}

void CBtosPage::OnEditBtosNewY()
{
	UpdateData(true);
	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	pstMcData->stBondTipOs.dy = m_lNewBtosY;
	theRef.UpdateAllBtos(pstMcData->stBondTipOs);
}

void CBtosPage::OnFocusBtosNewX()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PARAMETER)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lNewBtosX);
	UpdateData(false);
	OnEditBtosNewX();
}

void CBtosPage::OnFocusBtosNewY()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PARAMETER)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_lNewBtosY);
	UpdateData(false);
	OnEditBtosNewY();
}

void CBtosPage::OnFocusEditStep()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PARAMETER)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nAdjustStep, 20, 20, 200);
	UpdateData(false);
}

void CBtosPage::StartAndVerifyHandle(void)
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
	pListBox->AddString(theMsg.GetMsg(MSG_UP_DOWN_TO_ADJUST_CAP_LEVEL));
	pListBox->AddString(theMsg.GetMsg(MSG_BOND_TO_MAKE_CAP_MARK));

	m_bAllowAdjust = true;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CORRECT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_MOVE_XY)->EnableWindow(true);
}

//v3.1.T259 clean
void CBtosPage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	TBL_POS stDesPos;
	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	if (m_bVerify)
	{
		stDesPos.dx = pstMcData->stBondTipOs.dx;
		stDesPos.dy = pstMcData->stBondTipOs.dy;
		theServo.MoveTable2(stDesPos, MTN_REL);	//v3.1.T42 add, v3.1.T43 edit
	}

	if (theMcConfig.IsBypassAll())
	{
		stOrgPos.dx = 15000.0 / theServo.GetResolXY();
		stOrgPos.dy = 100.0 / theServo.GetResolXY();
	}
	else
	{
		long lCtactLvl;
		theBond.CtactSrch(m_nSrchSpeed, m_nSrchTol, &lCtactLvl);
		theServo.GetTablePosition(&stOrgPos);
	}

	stDesPos.dx = -pstMcData->stBondTipOs.dx;
	stDesPos.dy = -pstMcData->stBondTipOs.dy;
	theServo.MoveTable2(stDesPos, MTN_REL);	//v3.1.T42 add, v3.1.T43 edit

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_SUCCESS));
	pListBox->AddString(theMsg.GetMsg(MSG_MOVE_CURSOR_TO_CAP_MARK));
	pListBox->AddString(theMsg.GetMsg(MSG_OK_TO_CONTINUE));

	m_bAllowAdjust = true;
	m_bVerify = true;
	theServo.JogSpeedSlowest();
	UpdateData(false);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_VERIFY));
	GetDlgItem(IDC_BUTTON_CORRECT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_MOVE_XY)->EnableWindow(true);
}

void CBtosPage::OnCorrect()
{
	if (theMcConfig.IsBypassAll())
	{
		stNewPos.dx = 0.0 / theServo.GetResolXY();
		stNewPos.dy = 0.0 / theServo.GetResolXY();
	}
	else
		theServo.GetTablePosition(&stNewPos);

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	pstMcData->stBondTipOs.dx = stOrgPos.dx - stNewPos.dx;
	pstMcData->stBondTipOs.dy = stOrgPos.dy - stNewPos.dy;
	theRef.UpdateAllBtos(pstMcData->stBondTipOs);

	m_lNewBtosX = round(pstMcData->stBondTipOs.dx);
	m_lNewBtosY = round(pstMcData->stBondTipOs.dy);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	pListBox->AddString(theMsg.GetMsg(MSG_BOND_TIP_OFFSET_UPDATED));
	pListBox->AddString(theMsg.GetMsg(MSG_VERIFY_TO_CHECK_CAP_MARK));
	pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_QUIT));
	UpdateData(false);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CORRECT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_MOVE_XY)->EnableWindow(true);

	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
		pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(true);
}

void CBtosPage::OnVerify()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	TBL_POS stDesPos;
	stDesPos.dx = pstMcData->stBondTipOs.dx;
	stDesPos.dy = pstMcData->stBondTipOs.dy;
	theServo.MoveTable2(stDesPos, MTN_REL);	//v3.1.T42 add, v3.1.T43 edit

	m_bVerify = false;

	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_BOND));
	GetDlgItem(IDC_BUTTON_CORRECT)->EnableWindow(false);
}

void CBtosPage::OnDestroy()
{
	theApp.ArchiveMachineData(ARCHIVE_SAVE);
	theBond.MoveBondHeadToFireLevel();
	CPropertyPage::OnDestroy();
}

LRESULT CBtosPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.1.W153 add
{
	return m_bBusy;
}

void CBtosPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_RECIPE, 0);	//v3.0.T454 add
}
