/***************************************************************
PositionCalibPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "PositionCalibPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "DataStruct.h"
#include "ServoGeneralPage.h"
#include "Bond.h"
#include "MachineConfig.h"
#include "MaterialHandlerThread.h"
#include "WireBondDlg.h"
#include "TopClamp.h"
#include "CuClamp.h"
#include "servo.h"
#include "Debug.h"
#include "Busy.h"
#include "KeyPadHandler.h"

// CPositionCalibPage dialog
IMPLEMENT_DYNAMIC(CPositionCalibPage, CPropertyPage)

CPositionCalibPage::CPositionCalibPage()
	: CPropertyPage(CPositionCalibPage::IDD)
	, m_lRelaxLevel(0)
    , m_fPosCompKp(0)
    , m_fPosCompOs(0)
    , m_fNewCompKp(0)
    , m_fNewCompOs(0)
    , m_lNegLimitX(0)
    , m_lPosLimitX(0)
    , m_lNegLimitY(0)
    , m_lPosLimitY(0)
	, m_nCaliStep(0)
	, m_lFlatLevel(0)
{
	m_bBusy = FALSE;	//V1.1.W145 add
}

CPositionCalibPage::~CPositionCalibPage()
{
	ResumeBondHead();	//v3.0.T144 add
}

void CPositionCalibPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXT_RELEX_LEVEL, m_lRelaxLevel);
	DDX_Text(pDX, IDC_STATIC_PARA_K, m_fPosCompKp);
	DDX_Text(pDX, IDC_STATIC_PARA_OS, m_fPosCompOs);
	DDX_Text(pDX, IDC_STATIC_NEW_KP, m_fNewCompKp);
	DDX_Text(pDX, IDC_STATIC_NEW_OS, m_fNewCompOs);
	DDX_Text(pDX, IDC_STATIC_X_NEG_LMT, m_lNegLimitX);
	DDX_Text(pDX, IDC_STATIC_X_POS_LMT, m_lPosLimitX);
	DDX_Text(pDX, IDC_STATIC_Y_NEG_LMT, m_lNegLimitY);
	DDX_Text(pDX, IDC_STATIC_Y_POS_LMT, m_lPosLimitY);
	DDX_Text(pDX, IDC_TXT_FLAT_LEVEL, m_lFlatLevel);
}

BEGIN_MESSAGE_MAP(CPositionCalibPage, CPropertyPage)
	ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
    ON_BN_CLICKED(IDC_BUTTON_START_SRCH_LMT, OnStartSrchLmt)
	ON_MESSAGE(WM_ISDIALOGBUSY,IsDialogBusy)	//V1.1.W145 add
	ON_BN_CLICKED(IDC_BUTTON_RESET_POSITION, &CPositionCalibPage::OnBnClickedButtonResetPosition)
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CPositionCalibPage::OnBnClickedButtonDone)
	ON_BN_CLICKED(IDC_BUTTON_SRCH_DONE, &CPositionCalibPage::OnBnClickedButtonSrchDone)
END_MESSAGE_MAP()

// CPositionCalibPage message handlers
BOOL CPositionCalibPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CString str;

    SetDlgItemTextW(IDC_STATIC_BOND_HEAD, theMsg.GetMsg(MSG_BOND_HEAD));
    SetDlgItemTextW(IDC_STATIC_COMPENSATION, theMsg.GetMsg(MSG_COMPENSATION));

    str.Format(_T("Kp[%.2f,%.2f]"), BH_POS_COMP_KP_LOW_LMT, BH_POS_COMP_KP_UP_LMT);
    SetDlgItemTextW(IDC_STATIC_KP_LABEL, str);

    str.Format(_T("Os[%d,%d]"), BH_POS_COMP_OS_LOW_LMT, BH_POS_COMP_OS_UP_LMT);
    SetDlgItemTextW(IDC_STATIC_OS_LABEL, str);

    SetDlgItemTextW(IDC_STATIC_CUR_VALUE, theMsg.GetMsg(MSG_CURRENT));
    SetDlgItemTextW(IDC_STATIC_NEW_VALUE, theMsg.GetMsg(MSG_NEW_COLON));
    SetDlgItemTextW(IDC_STATIC_RELEX_LEVEL, theMsg.GetMsg(MSG_RELAX_POS));
	SetDlgItemTextW(IDC_STATIC_FLAT_LEVEL, theMsg.GetMsg(MSG_FLAT_LEVEL));
    SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
    SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));	//v3.0.T452 add
    SetDlgItemTextW(IDC_STATIC_TABLE, theMsg.GetMsg(MSG_TABLE));
    SetDlgItemTextW(IDC_STATIC_X_LMT, theMsg.GetMsg(MSG_X_LIMIT));
    SetDlgItemTextW(IDC_STATIC_Y_LMT, theMsg.GetMsg(MSG_Y_LIMIT));
    SetDlgItemTextW(IDC_BUTTON_START_SRCH_LMT, theMsg.GetMsg(MSG_AUTO_SRCH));
    SetDlgItemTextW(IDC_BUTTON_SRCH_DONE, theMsg.GetMsg(MSG_DONE));	//v3.0.T452 add
	SetDlgItemTextW(IDC_BUTTON_RESET_POSITION, theMsg.GetMsg(MSG_RESET_TO_DEFAULT));//v1.6.P203 add

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	m_lRelaxLevel	= pstMcData->lRelaxLevel;
    m_fPosCompKp	= (float)pstMcData->dPosCompKp;
    m_fPosCompOs	= (float)pstMcData->dPosCompOs;
    m_lNegLimitX	= pstMcData->lTblXNegLimit;
    m_lPosLimitX	= pstMcData->lTblXPosLimit;
    m_lNegLimitY	= pstMcData->lTblYNegLimit;
    m_lPosLimitY	= pstMcData->lTblYPosLimit;
	m_lFlatLevel	= pstMcData->lFlatLevel;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CPositionCalibPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPositionCalibPage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.1.W145 add

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
    pListBox->InsertString(1, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

    GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(1);

    theServo.SetServoControlParameter();
    theServo.SetServoSpeedProfile(MOTOR_Z, 0);
	m_nCaliStep = 0;

    UpdateData(false);
}

void CPositionCalibPage::OnNext()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();

    UpdateData(false);

    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(0);

	m_nCaliStep++;
	
    long lStatus, m_lCtactLvl;
    double dPosCompKp, dPosCompOs;
	double dBondHeadPos;

	if (m_nCaliStep == 1)
	{
		lStatus = theBond.CtactSrch(3, BH_FORCE_CALIB_SRCH_TOL, &m_lCtactLvl);
		if (lStatus == FOK)
		{
			lStatus = theServo.PositionCompen(m_lCtactLvl, &dPosCompKp, &dPosCompOs);
			if (lStatus == FOK)
			{
				m_fNewCompKp = (float)(dPosCompKp * 327.67);
				m_fNewCompOs = (float)(dPosCompOs * 327.67);

				if ((m_fNewCompKp > BH_POS_COMP_KP_LOW_LMT) && 
					(m_fNewCompKp < BH_POS_COMP_KP_UP_LMT) &&
					(m_fNewCompOs > BH_POS_COMP_OS_LOW_LMT) &&
					(m_fNewCompOs < BH_POS_COMP_OS_UP_LMT))
				{
					MACHINE_DATA* pstMcData = theMcData.GetRcd();
					pstMcData->dPosCompKp = dPosCompKp * 327.67;
					pstMcData->dPosCompOs = dPosCompOs * 327.67;
	    
					theServo.SetPosCompFactor(pstMcData->dPosCompKp, pstMcData->dPosCompOs);

					pListBox->InsertString(0, theMsg.GetMsg(MSG_POS_COMP_SUCCESS));
				}
				else
				{
					pListBox->InsertString(0, theMsg.GetMsg(MSG_POS_COMP_OUT_RANGE));
				}
			}
			else
			{
				pListBox->InsertString(0, theMsg.GetMsg(MSG_POS_COMP_FAIL));
			}
		}

		theServo.DisableMotor(MOTOR_Z);
		Sleep_x1ms(200);

		short i;
		int iCheckCounter = 0;
		short nlevel[3];
		short nErr1, nErr2;
		short nErr = 0;
		do
		{
			for (i = 0; i < 3; i++)
			{
				theServo.GetBondHeadPosition(&dBondHeadPos);
				nlevel[i] = (short)dBondHeadPos;
				Sleep_x1ms(800);
			}

			nErr1 = nlevel[1] - nlevel[0];
			nErr2 = nlevel[2] - nlevel[1];
			nErr = (abs(nErr1) + abs(nErr2)) / 2;

			iCheckCounter++;

			if (iCheckCounter > 30)
			{
				AfxMessageBox(_T("Attempt to search Z relex position fails, Please try again!"));
				break;
			}
		} while (nErr > 30);

		m_lRelaxLevel = nlevel[1];
		UpdateData(false);

		MACHINE_DATA *pstMcData = theMcData.GetRcd();
		pstMcData->lRelaxLevel = m_lRelaxLevel;

		pListBox->InsertString(0, theMsg.GetMsg(MSG_SETUP_JIG));
		pListBox->InsertString(1, theMsg.GetMsg(MSG_NEXT_TO_RECORD_FLAT_LEVEL));

		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(1);
	}
	else if (m_nCaliStep == 2)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);

		theServo.GetBondHeadPosition(&dBondHeadPos);
		m_lFlatLevel = round(dBondHeadPos);

		MACHINE_DATA *pstMcData = theMcData.GetRcd();
		pstMcData->lFlatLevel = m_lFlatLevel;

		UpdateData(FALSE);

		pListBox->InsertString(0, theMsg.GetMsg(MSG_REMOVE_JIG));
		pListBox->InsertString(1, theMsg.GetMsg(MSG_NEXT_TO_FINISH));
	}
	else if (m_nCaliStep == 3)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(0);

		//v3.0.T144 delete
		//theServo.EnableMotor(MOTOR_Z);
		//Sleep_x1ms(1000);
		//theBond.MoveBondHeadToFireLevel();
		ResumeBondHead();	//v3.0.T144 add

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(1);
	}
}

void CPositionCalibPage::OnStartSrchLmt()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

    CStringList sList;
    sList.AddTail(_T(""));
    sList.AddTail(theMsg.GetMsg(MSG_START_SRCH_LIMIT)); 
    sList.AddTail(theMsg.GetMsg(MSG_SURE_BOND_AREA_CLEAR)); 
	if (theMsg.ShowMsgS(HELP_MSG, sList, true) == SHOW_MSG_RESULT_STOP) return;

    theServo.DisableTableLimit();

	MACHINE_DATA *pstMcData = theMcData.GetRcd();
    pstMcData->lTblXNegLimit = -400000;
    pstMcData->lTblXPosLimit =  400000;
    pstMcData->lTblYNegLimit = -400000;
    pstMcData->lTblYPosLimit =  400000;

    GetDlgItem(IDC_BUTTON_START_SRCH_LMT)->EnableWindow(0);

    // Search X positive limit
    TBL_POS stOrgPos;
	switch (theMcConfig.GetMhsType())	//v3.0.T152 edit
	{
	case MHS_TOP_LED: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
	{
		stOrgPos.dx = 0;
		stOrgPos.dy = 0;
  		theTopClamp.CloseClamp();
    	Sleep_x1ms(500);
		break;
	}
	case MHS_CU_ELITE:
	{
		stOrgPos.dx = 0;
		stOrgPos.dy = 0;
    	theCuEliteLFClamp.CloseClamp();
    	Sleep_x1ms(500);
		break;
	}
	default:
	    stOrgPos.dx = 40000;
		stOrgPos.dy = 40000;
	}

	if (!theMcConfig.IsBypassServo())	//v3.0.T452 add
	{
		theServo.MoveTable2(stOrgPos);	//v3.1.T203 edit
		theServo.SearchTableLimit(MOTOR_X, MOTION_POSITIVE);
		Sleep_x1ms(50);

		TBL_POS stTblPos;
		theServo.GetTablePosition(&stTblPos);
		long lPosLimitX = round(stTblPos.dx) - 1000;

		// X negative limit
		theServo.MoveTable2(stOrgPos);	//v3.1.T203 edit
		theServo.SearchTableLimit(MOTOR_X, MOTION_NEGATIVE);
		Sleep_x1ms(50);
		theServo.GetTablePosition(&stTblPos);
		long lNegLimitX = round(stTblPos.dx) + 1000;

		// Y Positive limit
		theServo.MoveTable2(stOrgPos);	//v3.1.T203 edit
		theServo.SearchTableLimit(MOTOR_Y, MOTION_POSITIVE);
		Sleep_x1ms(50);
		theServo.GetTablePosition(&stTblPos);
		long lPosLimitY = round(stTblPos.dy) - 1000;

		// Y Negative limit
		theServo.MoveTable2(stOrgPos);	//v3.1.T203 edit
		theServo.SearchTableLimit(MOTOR_Y, MOTION_NEGATIVE);
		Sleep_x1ms(50);
		theServo.GetTablePosition(&stTblPos);
		long lNegLimitY = round(stTblPos.dy) + 1000;

		pstMcData->lTblXNegLimit = lNegLimitX;
		pstMcData->lTblXPosLimit = lPosLimitX;
		pstMcData->lTblYNegLimit = lNegLimitY;
		pstMcData->lTblYPosLimit = lPosLimitY;

		m_lNegLimitX = pstMcData->lTblXNegLimit;
		m_lPosLimitX = pstMcData->lTblXPosLimit;
		m_lNegLimitY = pstMcData->lTblYNegLimit;
		m_lPosLimitY = pstMcData->lTblYPosLimit;

		GetDlgItem(IDC_BUTTON_START_SRCH_LMT)->EnableWindow(1);

		theServo.MoveTable2(stOrgPos);	//v3.1.T203 edit
		theServo.EnableTableLimit();
	}

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_SRCH_LIMIT_FINISH));
	pListBox->InsertString(1, theMsg.GetMsg(MSG_DONE_TO_QUIT));

    UpdateData(false);
}

LRESULT CPositionCalibPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W145 add
{
	return m_bBusy;
}

void CPositionCalibPage::OnBnClickedButtonResetPosition()	//v1.6.P203sc add
{
	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	pstMcData->dPosCompKp = 0.043;
	pstMcData->dPosCompOs = 135.5;

	m_fPosCompKp = (float)pstMcData->dPosCompKp;
	m_fPosCompOs = (float)pstMcData->dPosCompOs;
	UpdateData(FALSE);
}

//v3.0.T144 add
void CPositionCalibPage::ResumeBondHead()
{
	theServo.EnableMotor(MOTOR_Z);
	Sleep_x1ms(1000);
	theBond.MoveBondHeadToFireLevel();
}

void CPositionCalibPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_SETUP, 0);	//v3.0.T452 add
}

void CPositionCalibPage::OnBnClickedButtonSrchDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_SETUP, 0);	//v3.0.T452 add
}
