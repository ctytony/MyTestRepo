/***************************************************************
CfgAccPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DummyBondPage.h"
#include "VisionSystem.h"
#include "TeachRecipeSheet.h"
#include "Bond.h"
#include "ScanKeyThread.h"
#include "servo.h"
#include "Busy.h"

IMPLEMENT_DYNAMIC(CDummyBondPage, CPropertyPage)

// CDummyBondPage dialog

CDummyBondPage::CDummyBondPage()
	: CPropertyPage(CDummyBondPage::IDD)
    , m_nSrchSpeed(3)
    , m_nSrchTol(5)
    , m_nBondTime(20)
    , m_nBondPower(60)
    , m_nBondForce(60)
    , m_sTeachStatus(_T(""))
    , m_nBondAreaStep(0)
	, m_lBondPosX(0)	//v3.0.T324 add
    , m_lBondPosY(0)	//v3.0.T324 add
{
	m_bBusy = FALSE;	//V1.1.W153 add
}

CDummyBondPage::~CDummyBondPage()
{
}

void CDummyBondPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SRCH_SPEED, m_nSrchSpeed);
    DDX_Text(pDX, IDC_EDIT_SRCH_TOL, m_nSrchTol);
    DDX_Text(pDX, IDC_EDIT_BOND_TIME, m_nBondTime);
    DDX_Text(pDX, IDC_EDIT_BOND_POWER, m_nBondPower);
    DDX_Text(pDX, IDC_EDIT_BOND_FORCE, m_nBondForce);
    DDX_Text(pDX, IDC_STATIC_TEACH_STATUS, m_sTeachStatus);
	DDX_Text(pDX, IDC_STATIC_X_POS, m_lBondPosX);	//v3.0.T324 add
    DDX_Text(pDX, IDC_STATIC_Y_POS, m_lBondPosY);	//v3.0.T324 add
}

BEGIN_MESSAGE_MAP(CDummyBondPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_SPEED, OnFocusSrchSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_SRCH_TOL, OnFocusSrchTol)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_TIME, OnFocusBondTime)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_POWER, OnFocusBondPower)
    ON_EN_SETFOCUS(IDC_EDIT_BOND_FORCE, OnFocusBondForce)
    ON_BN_CLICKED(IDC_BUTTON_TEACH, OnTeach)
    ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnContinue)
    ON_WM_DESTROY()
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W153 add
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDummyBondPage::OnBnClickedButtonUpdate)	//v3.0.T324 add
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CDummyBondPage::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CDummyBondPage message handlers
BOOL CDummyBondPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	//v3.0.T324 add
	if (theMcConfig.GetMhsType() == MHS_FOXCONN)
	{
		GetDlgItem(IDC_STATIC_AREA_VALID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEACH_STATUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_TEACH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_CONTINUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DONE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_INFORM)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DUMMY_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DUMMY_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_X_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Y_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_UPDATE)->ShowWindow(SW_HIDE);
	}

	DUMMY_BOND_AREA_RCD stDummyRcd;
    theBondPara.GetDummyBondAreaRcd(&stDummyRcd);
	m_lBondPosX = round(stDummyRcd.stLeftTopPos.dx);
	m_lBondPosY = round(stDummyRcd.stLeftTopPos.dy);

    SetDlgItemTextW(IDC_STATIC_CTACT_PARA, theMsg.GetMsg(MSG_CTACT_PARA));
    SetDlgItemTextW(IDC_STATIC_SRCH_SPEED, theMsg.GetMsg(MSG_SRCH_SPEED));
    SetDlgItemTextW(IDC_STATIC_SRCH_TOL, theMsg.GetMsg(MSG_SRCH_THRES));
    SetDlgItemTextW(IDC_STATIC_BOND_PARA, theMsg.GetMsg(MSG_BOND_PARAMETER));
    SetDlgItemTextW(IDC_STATIC_TIME, theMsg.GetMsg(MSG_TIME_MS));
    SetDlgItemTextW(IDC_STATIC_POWER, theMsg.GetMsg(MSG_POWER_DAC));
    SetDlgItemTextW(IDC_STATIC_FORCE, theMsg.GetMsg(MSG_FORCE_GM));
    SetDlgItemTextW(IDC_STATIC_BOND_AREA, theMsg.GetMsg(MSG_BOND_AREA));
    SetDlgItemTextW(IDC_STATIC_AREA_VALID, theMsg.GetMsg(MSG_AREA_TEACH_STATUS));
    SetDlgItemTextW(IDC_BUTTON_TEACH, theMsg.GetMsg(MSG_TEACH));
    SetDlgItemTextW(IDC_BUTTON_CONTINUE, theMsg.GetMsg(MSG_CONTINUE));
    SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));
    SetDlgItemTextW(IDC_BUTTON_UPDATE, theMsg.GetMsg(MSG_UPDATE));

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
        pSheet->SetWizardButtons(PSWIZB_FINISH);
        pSheet->GetDlgItem(ID_WIZFINISH)->EnableWindow(0);
        pSheet->GetDlgItem(IDCANCEL)->EnableWindow(0);
    }

	DUMMY_BOND_PARA stDummyBondPara;
	theBondPara.GetDummyBondPara(&stDummyBondPara);
    m_nSrchSpeed = stDummyBondPara.nSrchSpeed;
    m_nSrchTol   = stDummyBondPara.nSrchTol;
    m_nBondTime  = stDummyBondPara.nBondTime;
    m_nBondPower = round(stDummyBondPara.fBondPower * 2.55);
    m_nBondForce = stDummyBondPara.nBondForce;

	DUMMY_BOND_AREA_RCD stDummyBondArea;
    theBondPara.GetDummyBondAreaRcd(&stDummyBondArea);
	m_sTeachStatus = stDummyBondArea.bBondAreaValid ? _T("Yes") : _T("No");

    return CPropertyPage::OnInitDialog();
}

HBRUSH CDummyBondPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

     return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDummyBondPage::OnDestroy()
{
	DUMMY_BOND_PARA stDummyBondPara;
	theBondPara.GetDummyBondPara(&stDummyBondPara);
	stDummyBondPara.nSrchSpeed = m_nSrchSpeed;
    stDummyBondPara.nSrchTol   = m_nSrchTol;
    stDummyBondPara.nBondTime  = m_nBondTime;
    stDummyBondPara.fBondPower = (float)m_nBondPower / (float)2.55;
    stDummyBondPara.nBondForce = m_nBondForce;
    theBondPara.SetDummyBondPara(&stDummyBondPara);

	CPropertyPage::OnDestroy();
}

void CDummyBondPage::OnFocusSrchSpeed()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    GetDlgItem(IDC_STATIC_CTACT_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchSpeed, 15);
    UpdateData(false);
}

void CDummyBondPage::OnFocusSrchTol()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    GetDlgItem(IDC_STATIC_CTACT_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nSrchTol,36);
    UpdateData(false);
}

void CDummyBondPage::OnFocusBondTime()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    GetDlgItem(IDC_STATIC_CTACT_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondTime, 20, 10, 50);
    UpdateData(false);
}

void CDummyBondPage::OnFocusBondPower()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    GetDlgItem(IDC_STATIC_CTACT_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondPower, 30, 10, 255);
    UpdateData(false);
}

void CDummyBondPage::OnFocusBondForce()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    GetDlgItem(IDC_STATIC_CTACT_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nBondForce, 100, 10, 300);
    UpdateData(false);
}

void CDummyBondPage::OnTeach()
{
    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFORM);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_TCH_DUMMY_BOND_AREA));
    pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_TOP_LEFT_PT));
    pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

    GetDlgItem(IDC_BUTTON_TEACH)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(TRUE);

	DUMMY_BOND_AREA_RCD stDummyBondAreaRcd;
    theBondPara.GetDummyBondAreaRcd(&stDummyBondAreaRcd);
    stDummyBondAreaRcd.bBondAreaValid = false;
    theBondPara.SetDummyBondAreaRcd(&stDummyBondAreaRcd);

    m_sTeachStatus = _T("No");
    m_nBondAreaStep = 0;

	//add for mouse movement
	CScanKeyThread theScanKeyThread;
    theScanKeyThread.EnableJoyStick();
    theVisionSys.EnableVideo(true);

	UpdateData(FALSE);
}

void CDummyBondPage::OnContinue()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFORM);
	pListBox->ResetContent();
	MACHINE_DATA *pstMcData = theMcData.GetRcd();
	DUMMY_BOND_AREA_RCD stDummyBondArea;
	theBondPara.GetDummyBondAreaRcd(&stDummyBondArea);

	switch (m_nBondAreaStep)
	{
	case 0:
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
			theServo.GetTablePosition(&(stDummyBondArea.stLeftTopPos));
		else
		{
			stDummyBondArea.stLeftTopPos.dx = 100.0;
			stDummyBondArea.stLeftTopPos.dy = 100.0;
		}
		stDummyBondArea.stLeftTopPos.dx += pstMcData->stBondTipOs.dx;
		stDummyBondArea.stLeftTopPos.dy += pstMcData->stBondTipOs.dy;

		pListBox->AddString(theMsg.GetMsg(MSG_TCH_DUMMY_BOND_AREA));
		pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_BOTTOM_RIGHT_PT));
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
		break;
	}
	case 1:
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
			theServo.GetTablePosition(&(stDummyBondArea.stRightBottomPos));
		else
		{
			stDummyBondArea.stRightBottomPos.dx = 100.0;
			stDummyBondArea.stRightBottomPos.dy = 100.0;
		}
		stDummyBondArea.stRightBottomPos.dx += pstMcData->stBondTipOs.dx;
		stDummyBondArea.stRightBottomPos.dy += pstMcData->stBondTipOs.dy;

		pListBox->AddString(theMsg.GetMsg(MSG_TCH_DUMMY_BOND_AREA));
		pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
		pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_TO_LEARN_CTACT_LEVEL));
		break;
	}
	case 2:
	{
		TBL_POS stDummyBondPos;
		stDummyBondPos.dx = stDummyBondArea.stLeftTopPos.dx
			+ (stDummyBondArea.stRightBottomPos.dx - stDummyBondArea.stLeftTopPos.dx) / 2;
		stDummyBondPos.dy = stDummyBondArea.stLeftTopPos.dy
			+ (stDummyBondArea.stRightBottomPos.dy - stDummyBondArea.stLeftTopPos.dy) / 2;
		theServo.MoveTable2(stDummyBondPos);	//v3.1.T199 edit
		Sleep_x1ms(100);

		long lCtactLvl;
		int nStatus = theBond.CtactSrch(m_nSrchSpeed, m_nSrchTol, &lCtactLvl);

		TBL_POS stTblPos;
		stTblPos.dx = -pstMcData->stBondTipOs.dx;
		stTblPos.dy = -pstMcData->stBondTipOs.dy;
		theServo.MoveTable2(stTblPos, false);	//v3.1.T199 edit
		Sleep_x1ms(40);

		if (nStatus == 0)
		{
			pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_SUCCESS));
	    
			stDummyBondArea.lCtactLvl = lCtactLvl;
			short nSideRedLight, nSideBlueLight;
			theVisionSys.GetLightingParameter(
				&(stDummyBondArea.stLightPara.nCoaxRedLight), &nSideRedLight,
				&(stDummyBondArea.stLightPara.nCoaxBlueLight), &nSideBlueLight);
			stDummyBondArea.stLightPara.bSideRedLight  = (byte)nSideRedLight;
			stDummyBondArea.stLightPara.bSideBlueLight = (byte)nSideBlueLight;
			stDummyBondArea.bBondAreaValid = true;

			m_sTeachStatus = _T("Yes");
		}
		else
			pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));

		GetDlgItem(IDC_BUTTON_TEACH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);

		CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
		if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
			pSheet->GetDlgItem(ID_WIZFINISH)->EnableWindow(1);
		break;
	}
	}

	theBondPara.SetDummyBondAreaRcd(&stDummyBondArea);
	m_nBondAreaStep++;
	UpdateData(FALSE);
}

LRESULT CDummyBondPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W153 add
{
	return m_bBusy;
}

//v3.0.T324 add
void CDummyBondPage::OnBnClickedButtonUpdate()
{
	DUMMY_BOND_AREA_RCD stDummyRcd;
	theServo.GetTablePosition(&(stDummyRcd.stLeftTopPos));
	stDummyRcd.bBondAreaValid = true;
    theBondPara.SetDummyBondAreaRcd(&stDummyRcd);
	m_lBondPosX = round(stDummyRcd.stLeftTopPos.dx);
	m_lBondPosY = round(stDummyRcd.stLeftTopPos.dy);

	UpdateData(false);
}

void CDummyBondPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_RECIPE, 0);	//v3.0.T456 add
}
