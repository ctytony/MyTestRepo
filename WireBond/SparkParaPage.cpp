/***************************************************************
SparkParaPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "BondPara.h"
#include "Bond.h"
#include "SparkParaPage.h"
#include "EfoControl.h"
#include "User.h"
#include "servo.h"
#include "ForceSwitch.h"

// CSparkParaPage dialog
IMPLEMENT_DYNAMIC(CSparkParaPage, CPropertyPage)

CSparkParaPage::CSparkParaPage()
	: CPropertyPage(CSparkParaPage::IDD)
	, m_nSparkDelay(0)
	, m_iCopperSequence(0)
	, m_bTailMonitor(FALSE)
	, m_iFabType(0)
	, m_nCurrent1(0)
	, m_nCurrent2(0)
	, m_nCurrent3(0)
	, m_nTime1(0)
	, m_nTime2(0)
	, m_nTime3(0)
	, m_nGapLargeDac(0)
	, m_nTailTime(0)
	, m_nTailBreakDist(0)
	, m_nTailBreakTime(0)
	, m_nFab2TailTime(0)
	, m_nFab2TailBreakDist(0)
	, m_nFab2TailBreakTime(0)
{
	//v3.1.T61 add
	for (int i = 0; i < MAX_PROFILE_NUM; i++)
		m_anTailLen[i] = 0;
}

CSparkParaPage::~CSparkParaPage()
{
}

void CSparkParaPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_TAIL_LENGTH, m_anTailLen[0]);	//v3.1.T61 add
	DDX_Text(pDX, IDC_EDIT_TAIL_LEN, m_nTailLen);		//v3.1.T63 add
	DDX_Text(pDX, IDC_EDIT_SPARK_DELAY, m_nSparkDelay);
	DDX_CBIndex(pDX, IDC_COMBO_COPPER_SEQUENCE, m_iCopperSequence);
	DDX_Text(pDX, IDC_EDIT_FAB2_TAIL_LENGTH, m_anTailLen[1]);	//v3.1.T61 add
	DDX_Check(pDX, IDC_CHECK_TAIL_MONITOR, m_bTailMonitor);
	DDX_CBIndex(pDX, IDC_COMBO_SEL_FAB, m_iFabType);
	DDX_Text(pDX, IDC_EDIT_SEG1_I, m_nCurrent1);
	DDX_Text(pDX, IDC_EDIT_SEG1_T, m_nTime1);
	DDX_Text(pDX, IDC_EDIT_SEG2_I, m_nCurrent2);
	DDX_Text(pDX, IDC_EDIT_SEG2_T, m_nTime2);
	DDX_Text(pDX, IDC_EDIT_SEG3_I, m_nCurrent3);
	DDX_Text(pDX, IDC_EDIT_SEG3_T, m_nTime3);
	DDX_Text(pDX, IDC_EDIT_GAP_LARGE_DAC, m_nGapLargeDac);
	DDX_Text(pDX, IDC_EDIT_TAIL_LENGTH_SMPL, m_nTailTime);
	DDX_Text(pDX, IDC_EDIT_TAIL_BREAK_UM, m_nTailBreakDist);
	DDX_Text(pDX, IDC_EDIT_FAB2_TAIL_LENGTH_SMPL, m_nFab2TailTime);
	DDX_Text(pDX, IDC_EDIT_FAB2_TAIL_BREAK, m_nFab2TailBreakDist);
	DDX_Text(pDX, IDC_EDIT_FAB2_TAIL_BREAK_SMPL, m_nFab2TailBreakTime);
	DDX_Text(pDX, IDC_EDIT_TAIL_BREAK_SMPL, m_nTailBreakTime);
}

BEGIN_MESSAGE_MAP(CSparkParaPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_TAIL_LEN, OnFocusTailLen)	//v3.1.T63 add
	ON_EN_SETFOCUS(IDC_EDIT_TAIL_LENGTH, OnFocusTailLength)
	ON_EN_SETFOCUS(IDC_EDIT_SPARK_DELAY, OnFocusSparkDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_COPPER_SEQUENCE, OnCopperSequence)
	ON_EN_SETFOCUS(IDC_EDIT_FAB2_TAIL_LENGTH, OnFocusFab2TailLength)
	ON_BN_CLICKED(IDC_CHECK_TAIL_MONITOR, OnTailMonitor)
	ON_EN_SETFOCUS(IDC_EDIT_SEG1_I, OnFocusSeg1I)
	ON_EN_SETFOCUS(IDC_EDIT_SEG1_T, OnFocusSeg1T)
	ON_EN_SETFOCUS(IDC_EDIT_SEG2_I, OnFocusSeg2I)
	ON_EN_SETFOCUS(IDC_EDIT_SEG2_T, OnFocusSeg2T)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_FAB, OnSelFab)
	ON_EN_SETFOCUS(IDC_EDIT_GAP_LARGE_DAC, OnFocusGapLargeDac)
	ON_EN_SETFOCUS(IDC_EDIT_TAIL_LENGTH_SMPL, &CSparkParaPage::OnEnSetfocusEditTailLengthSmpl)
	ON_EN_SETFOCUS(IDC_EDIT_TAIL_BREAK_UM, &CSparkParaPage::OnEnSetfocusEditTailBreakUm)
	ON_EN_SETFOCUS(IDC_EDIT_TAIL_BREAK_SMPL, &CSparkParaPage::OnEnSetfocusEditTailBreakSmpl)
	ON_EN_SETFOCUS(IDC_EDIT_FAB2_TAIL_LENGTH_SMPL, &CSparkParaPage::OnEnSetfocusEditFab2TailLengthSmpl)
	ON_EN_SETFOCUS(IDC_EDIT_FAB2_TAIL_BREAK, &CSparkParaPage::OnEnSetfocusEditFab2TailBreak)
	ON_EN_SETFOCUS(IDC_EDIT_FAB2_TAIL_BREAK_SMPL, &CSparkParaPage::OnEnSetfocusEditFab2TailBreakSmpl)
	ON_EN_SETFOCUS(IDC_EDIT_SEG3_T, &CSparkParaPage::OnFocusSeg3T)
	ON_EN_SETFOCUS(IDC_EDIT_SEG3_I, &CSparkParaPage::OnFocusSeg3I)
END_MESSAGE_MAP()

// CSparkParaPage message handlers

BOOL CSparkParaPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
	
	if (theMcConfig.GetAcsBufferType() == 1)
	{
		//v3.1.T63 add
		GetDlgItem(IDC_STATIC_TAIL_LEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TAIL_LEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_LEN_UM)->ShowWindow(SW_HIDE);

		//1
		GetDlgItem(IDC_STATIC_TAIL_LENGTH)->SetWindowPos(NULL, 24, 40, 155, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_TAIL_LENGTH)->SetWindowPos(NULL, 180, 40, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_TAIL_LENGTH_UM)->SetWindowPos(NULL, 250, 40, 40, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_TAIL_LENGTH_SMPL)->SetWindowPos(NULL, 300, 40, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_TAIL_LENGTH_SMPL)->SetWindowPos(NULL, 370, 40, 60, 25, SWP_NOZORDER);

		//2
		GetDlgItem(IDC_STATIC_TAIL_BREAK_1)->SetWindowPos(NULL, 24, 70, 155, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_TAIL_BREAK_UM)->SetWindowPos(NULL, 180, 70, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_TAIL_BREAK1_UM)->SetWindowPos(NULL, 250, 70, 40, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_TAIL_BREAK_SMPL)->SetWindowPos(NULL, 300, 70, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_TAIL_BREAK1_SMPL)->SetWindowPos(NULL, 370, 70, 60, 25, SWP_NOZORDER);

		//3
		GetDlgItem(IDC_STATIC_FAB2_TAIL_LENGTH)->SetWindowPos(NULL, 24, 100, 155, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_FAB2_TAIL_LENGTH)->SetWindowPos(NULL, 180, 100, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_TAIL_FAB2_UM)->SetWindowPos(NULL, 250, 100, 40, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_FAB2_TAIL_LENGTH_SMPL)->SetWindowPos(NULL, 300, 100, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_TAIL_FAB2_SMPL)->SetWindowPos(NULL, 370, 100, 60, 25, SWP_NOZORDER);

		//4
		GetDlgItem(IDC_STATIC_TAIL_BREAK_FAB2)->SetWindowPos(NULL, 24, 130, 155, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_FAB2_TAIL_BREAK)->SetWindowPos(NULL, 180, 130, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_FAB2_UM)->SetWindowPos(NULL, 250, 130, 40, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_FAB2_TAIL_BREAK_SMPL)->SetWindowPos(NULL, 300, 130, 60, 25, SWP_NOZORDER);
		GetDlgItem(IDC_STATIC_FAB2_SMPL)->SetWindowPos(NULL, 370, 130, 60, 25, SWP_NOZORDER);

		//5
		GetDlgItem(IDC_STATIC_SPARK_DELAY)->SetWindowPos(NULL, 24, 160, 129, 25, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_SPARK_DELAY)->SetWindowPos(NULL, 310, 160, 83, 25, SWP_NOZORDER);

		//6
		GetDlgItem(IDC_STATIC_COPPER_SEQUENCE)->SetWindowPos(NULL, 24, 190, 129, 25, SWP_NOZORDER);
		GetDlgItem(IDC_COMBO_COPPER_SEQUENCE)->SetWindowPos(NULL, 310, 190, 83, 25, SWP_NOZORDER);

		//7
		GetDlgItem(IDC_CHECK_TAIL_MONITOR)->SetWindowPos(NULL, 24, 220, 180, 25, SWP_NOZORDER);

		SetDlgItemTextW(IDC_STATIC_TAIL_BREAK_1, theMsg.GetMsg(MSG_TAIL_BREAK));
		SetDlgItemTextW(IDC_STATIC_TAIL_BREAK_FAB2, theMsg.GetMsg(MSG_TAIL_BREAK));
	}
	else
	{
		GetDlgItem(IDC_EDIT_TAIL_LENGTH_SMPL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_LENGTH_SMPL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_TAIL_BREAK_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TAIL_BREAK_UM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_BREAK1_UM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TAIL_BREAK_SMPL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_BREAK1_SMPL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_FAB2_TAIL_LENGTH_SMPL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_FAB2_SMPL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_TAIL_BREAK_FAB2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_FAB2_TAIL_BREAK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FAB2_UM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_FAB2_TAIL_BREAK_SMPL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FAB2_SMPL)->ShowWindow(SW_HIDE);

		//v3.1.T62 add
		GetDlgItem(IDC_STATIC_GAP_LARGE_DAC)->SetWindowPos(NULL, 24, 45, 200, 30, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_GAP_LARGE_DAC)->SetWindowPos(NULL, 310, 45, 120, 30, SWP_NOZORDER);

		//v3.1.T62 add
		GetDlgItem(IDC_STATIC_TAIL_LENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TAIL_LENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_LENGTH_UM)->ShowWindow(SW_HIDE);

		//v3.1.T62 add
		GetDlgItem(IDC_STATIC_FAB2_TAIL_LENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_FAB2_TAIL_LENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TAIL_FAB2_UM)->ShowWindow(SW_HIDE);

		//3
		GetDlgItem(IDC_STATIC_SPARK_DELAY)->SetWindowPos(NULL, 24, 85, 200, 30, SWP_NOZORDER);
		GetDlgItem(IDC_EDIT_SPARK_DELAY)->SetWindowPos(NULL, 310, 85, 120, 30, SWP_NOZORDER);

		//4
		GetDlgItem(IDC_STATIC_COPPER_SEQUENCE)->SetWindowPos(NULL, 24, 125, 200, 30, SWP_NOZORDER);
		GetDlgItem(IDC_COMBO_COPPER_SEQUENCE)->SetWindowPos(NULL, 310, 125, 120, 30, SWP_NOZORDER);

        //5
		GetDlgItem(IDC_CHECK_TAIL_MONITOR)->SetWindowPos(NULL, 24, 165, 200, 30, SWP_NOZORDER);
	}

	if(theMcConfig.GetEfoType() == 0)
	{
		GetDlgItem(IDC_STATIC_SEG1_I)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SEG2_I)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SEG3_I)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SEG2_T)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SEG3_T)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_SEG1_I)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SEG2_I)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SEG3_I)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SEG2_T)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SEG3_T)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_MA1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MA2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MA3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_US2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_US3)->ShowWindow(SW_HIDE);

		SetDlgItemTextW(IDC_STATIC_SEG1_T, theMsg.GetMsg(MSG_TIME));
	}
	else
	{
		SetDlgItemTextW(IDC_STATIC_SEG1_T, theMsg.GetMsg(MSG_TIME1));
		SetDlgItemTextW(IDC_STATIC_SEG1_I, theMsg.GetMsg(MSG_CURRENT1));
		SetDlgItemTextW(IDC_STATIC_SEG2_T, theMsg.GetMsg(MSG_TIME2));
		SetDlgItemTextW(IDC_STATIC_SEG2_I, theMsg.GetMsg(MSG_CURRENT2));
		SetDlgItemTextW(IDC_STATIC_SEG3_T, theMsg.GetMsg(MSG_TIME3));
		SetDlgItemTextW(IDC_STATIC_SEG3_I, theMsg.GetMsg(MSG_CURRENT3));
	}

	SetDlgItemTextW(IDC_STATIC_PARAMETERS, theMsg.GetMsg(MSG_PARAMETERS));
	SetDlgItemTextW(IDC_STATIC_TAIL_LENGTH, theMsg.GetMsg(MSG_TAIL_LENGTH));
	SetDlgItemTextW(IDC_STATIC_FAB2_TAIL_LENGTH, theMsg.GetMsg(MSG_FAB2_TAIL_LENGTH));
	SetDlgItemTextW(IDC_STATIC_SPARK_DELAY, theMsg.GetMsg(MSG_SPARK_DELAY_COLON));
	SetDlgItemTextW(IDC_STATIC_COPPER_SEQUENCE, theMsg.GetMsg(MSG_COPPER_SEQ));
	SetDlgItemTextW(IDC_CHECK_TAIL_MONITOR, theMsg.GetMsg(MSG_TAIL_LEN_MONITOR));
	GetDlgItem(IDC_CHECK_TAIL_MONITOR)->EnableWindow(theUser.GetUserLevel() > USER_PROCESS);
	SetDlgItemTextW(IDC_STATIC_EFO_CONFIG, theMsg.GetMsg(MSG_SPARK_CONFIG));
	if (theMcConfig.IsGapLargeTime())	//v3.1.T484 add
		SetDlgItemTextW(IDC_STATIC_GAP_LARGE_DAC, theMsg.GetMsg(MSG_GAP_LARGE_TIME));
	else
		SetDlgItemTextW(IDC_STATIC_GAP_LARGE_DAC, theMsg.GetMsg(MSG_GAP_LARGE_DAC));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SEL_FAB);
	pComboBox->AddString(_T("FAB1"));
	pComboBox->AddString(_T("FAB2"));
	//v3.1.T34 add
	if (theMcConfig.GetFabType() == 1)
	{
		pComboBox->AddString(_T("FAB3"));
		pComboBox->AddString(_T("FAB4"));
	}
	pComboBox->SetCurSel(0);

	const PGM_RECORD *pstPgm = thePgm.GetRcd();
	m_nSparkDelay       = pstPgm->nSparkDelay;
	m_iCopperSequence   = pstPgm->bCopperSequence;
	m_bTailMonitor      = pstPgm->bTailMonitor;

	//v3.1.T60 edit
	const ST_EFO_PROFILE *pstEfoProf = theEfoProfile.GetProfile(0);
	m_nCurrent1 = pstEfoProf->nCurrent1;
	m_nTime1    = pstEfoProf->nTime1;
	m_nCurrent2 = pstEfoProf->nCurrent2;
	m_nTime2    = pstEfoProf->nTime2;
	m_nCurrent3 = pstEfoProf->nCurrent3;
	m_nTime3    = pstEfoProf->nTime3;
	m_nTailLen  = pstEfoProf->nTailLen_x1um;	//v3.1.T63 add

	//v3.1.T60 add
	m_anTailLen[0] = pstEfoProf->nTailLen_x1um;
	m_anTailLen[1] = theEfoProfile.GetProfile(1)->nTailLen_x1um;

	m_nGapLargeDac = theMcData.GetRcd()->cGapLargeDac;	//v3.1.T62 edit

	FORCE_SWITCH_PARA *pstForceSwitch = theForceSwitch.GetRcd();
	m_nTailTime          = pstForceSwitch->nTailTime;
	m_nTailBreakDist     = pstForceSwitch->nTailBreakDist;
	m_nTailBreakTime     = pstForceSwitch->nTailBreakTime;
	m_nFab2TailTime      = pstForceSwitch->nFab2TailTime;
	m_nFab2TailBreakDist = pstForceSwitch->nFab2TailBreakDist;
	m_nFab2TailBreakTime = pstForceSwitch->nFab2TailBreakTime;

	return CPropertyPage::OnInitDialog();
}

void CSparkParaPage::OnSelFab()
{
	UpdateData(TRUE);

	const ST_EFO_PROFILE *pstEfoProf = theEfoProfile.GetProfile(m_iFabType);
	m_nCurrent1 = pstEfoProf->nCurrent1;
	m_nTime1    = pstEfoProf->nTime1;
	m_nCurrent2 = pstEfoProf->nCurrent2;
	m_nTime2    = pstEfoProf->nTime2;
	m_nCurrent3 = pstEfoProf->nCurrent3;
	m_nTime3    = pstEfoProf->nTime3;
	m_nTailLen  = pstEfoProf->nTailLen_x1um;	//v3.1.T63 add

	UpdateData(false);
}

HBRUSH CSparkParaPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}
	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CSparkParaPage::OnFocusTailLength()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_anTailLen[0], 200,
				theBondPara.GetBondParaLimit()->nTailLengthL,
				theBondPara.GetBondParaLimit()->nTailLengthH, true);
	UpdateData(false);

	//v3.1.T60 add
	ST_EFO_PROFILE *pstEfoProf = theEfoProfile.GetProfile(0);
	theMcData.GetRcd()->lResetLevel += round((m_anTailLen[0]
						- pstEfoProf->nTailLen_x1um) / theServo.GetResolZ());
	pstEfoProf->nTailLen_x1um = m_anTailLen[0];

	theBond.MoveBondHeadToFireLevel();
}

//v3.1.T63 add
void CSparkParaPage::OnFocusTailLen()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTailLen, 200,
				theBondPara.GetBondParaLimit()->nTailLengthL,
				theBondPara.GetBondParaLimit()->nTailLengthH, true);
	UpdateData(false);

	ST_EFO_PROFILE *pstEfoProf = theEfoProfile.GetProfile(m_iFabType);
	theMcData.GetRcd()->lResetLevel += round((m_nTailLen
						- pstEfoProf->nTailLen_x1um) / theServo.GetResolZ());
	pstEfoProf->nTailLen_x1um = m_nTailLen;

	theBond.MoveBondHeadToFireLevel();
}

void CSparkParaPage::OnFocusSparkDelay()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSparkDelay, 5,
			theBondPara.GetBondParaLimit()->nSparkDelayL,
			theBondPara.GetBondParaLimit()->nSparkDelayH, true);
	UpdateData(false);
	thePgm.GetRcd()->nSparkDelay = m_nSparkDelay;
}

void CSparkParaPage::OnCopperSequence()
{
	UpdateData(true);
	thePgm.SetCopperSequence(m_iCopperSequence != 0);
}

void CSparkParaPage::OnFocusFab2TailLength()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_anTailLen[1], 400,
			theBondPara.GetBondParaLimit()->nTailLength2L,
			theBondPara.GetBondParaLimit()->nTailLength2H, true);
	UpdateData(false);
	theEfoProfile.GetProfile(1)->nTailLen_x1um = m_anTailLen[1];	//v3.1.T60 add
}

void CSparkParaPage::OnTailMonitor()
{
	UpdateData(true);
	thePgm.GetRcd()->bTailMonitor = (m_bTailMonitor == TRUE);
}

void CSparkParaPage::OnFocusSeg1I()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	if (theMcConfig.GetEfoType() == 2)	//v3.0.T343 add
		theKeyPad.GetUserKeyInData(&m_nCurrent1,20,0,100);
	else
		theKeyPad.GetUserKeyInData(&m_nCurrent1,20,0,47);
	UpdateData(false);

	ST_EFO_PROFILE stEfoProfileRcd;
	theEfoProfile.GetProfile(m_iFabType, &stEfoProfileRcd);
	stEfoProfileRcd.nCurrent1 =(WORD)m_nCurrent1;
	theEfoProfile.SetProfile(m_iFabType, &stEfoProfileRcd);	//v1.5.T69 edit
	theEFO.SendProfile();
}

void CSparkParaPage::OnFocusSeg1T()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	long lMaxLmt = (long)(255 / theEFO.GetTimeFactor());	//v3.1.T396 add
	if (theMcConfig.GetEfoType() > 0)		//v3.1.T48 add
		//theKeyPad.GetUserKeyInData(&m_nTime1, 1000, 50, lMaxLmt);	//v3.1.T444 delete
		theKeyPad.GetUserKeyInData(&m_nTime1, 1000, 0, lMaxLmt);	//v3.1.T444 add
	else
		theKeyPad.GetUserKeyInData(&m_nTime1, 1000, 200, lMaxLmt);
	UpdateData(false);

	ST_EFO_PROFILE stEfoProfileRcd;
	theEfoProfile.GetProfile(m_iFabType, &stEfoProfileRcd);
	stEfoProfileRcd.nTime1 = (WORD)m_nTime1;
	theEfoProfile.SetProfile(m_iFabType, &stEfoProfileRcd);	//v1.5.T69 edit
	theEFO.SendProfile();
}

void CSparkParaPage::OnFocusSeg2I()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	if (theMcConfig.GetEfoType() == 2)	//v3.0.T343 add
		theKeyPad.GetUserKeyInData(&m_nCurrent2,20,0,100);
	else
		theKeyPad.GetUserKeyInData(&m_nCurrent2,20,0,47);
	UpdateData(false);

	ST_EFO_PROFILE stEfoProfileRcd;
	theEfoProfile.GetProfile(m_iFabType, &stEfoProfileRcd);
	stEfoProfileRcd.nCurrent2 = (WORD)m_nCurrent2;
	theEfoProfile.SetProfile(m_iFabType, &stEfoProfileRcd);	//v1.5.T69 edit
	theEFO.SendProfile();
}

void CSparkParaPage::OnFocusSeg2T()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	long lMaxLmt = (long)(255 / theEFO.GetTimeFactor());		//v3.1.T396 add
	theKeyPad.GetUserKeyInData(&m_nTime2, 1000, 0, lMaxLmt);	//v3.1.T396 edit
	UpdateData(false);

	ST_EFO_PROFILE stEfoProfileRcd;
	theEfoProfile.GetProfile(m_iFabType, &stEfoProfileRcd);
	stEfoProfileRcd.nTime2 = (WORD)m_nTime2;
	theEfoProfile.SetProfile(m_iFabType, &stEfoProfileRcd);	//v1.5.T69 edit
	theEFO.SendProfile();
}

void CSparkParaPage::OnFocusGapLargeDac()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	if (theMcConfig.IsGapLargeTime())
	{
		//v3.1.T577 add
		long lHighLmt = theMcConfig.GetGapLargeTimeHighLimit();
		if (lHighLmt < 50) lHighLmt = 50;
		theKeyPad.GetUserKeyInData(&m_nGapLargeDac, 40, 10, lHighLmt);	//v3.1.T501 add, v3.1.T577 edit
	}
	else
		theKeyPad.GetUserKeyInData(&m_nGapLargeDac, 200, 0, 255);
	UpdateData(false);
	theMcData.GetRcd()->cGapLargeDac = (BYTE)m_nGapLargeDac;
	if (theMcConfig.IsGapLargeTime())	//v3.1.T484 add
		theEFO.SetGapLargeTime((BYTE)m_nGapLargeDac);
	else
		theEFO.SetGapLargeDac((BYTE)m_nGapLargeDac);
}

void CSparkParaPage::OnEnSetfocusEditTailLengthSmpl()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(& m_nTailTime, 8, 5, 20);
	UpdateData(false);
	theForceSwitch.GetRcd()->nTailTime = m_nTailTime;
}

void CSparkParaPage::OnEnSetfocusEditTailBreakUm()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTailBreakDist, -1, -1, 50);   //v1.6.P152sc  edit
	UpdateData(false);
	theForceSwitch.GetRcd()->nTailBreakDist = m_nTailBreakDist;
}

void CSparkParaPage::OnEnSetfocusEditTailBreakSmpl()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTailBreakTime, 4, 2, 12); //v1.6.P152sc  edit
	UpdateData(false);
	theForceSwitch.GetRcd()->nTailBreakTime = m_nTailBreakTime;
}

void CSparkParaPage::OnEnSetfocusEditFab2TailLengthSmpl()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nFab2TailTime, 8, 5, 20);
	UpdateData(false);
	theForceSwitch.GetRcd()->nFab2TailTime = m_nFab2TailTime;
}

void CSparkParaPage::OnEnSetfocusEditFab2TailBreak()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nFab2TailBreakDist, -1, -1, 50);
	UpdateData(false);
	theForceSwitch.GetRcd()->nFab2TailBreakDist = m_nFab2TailBreakDist;
}

void CSparkParaPage::OnEnSetfocusEditFab2TailBreakSmpl()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nFab2TailBreakTime, 4, 2, 12);
	UpdateData(false);
	theForceSwitch.GetRcd()->nFab2TailBreakTime = m_nFab2TailBreakTime;
}

void CSparkParaPage::OnFocusSeg3T()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	long lMaxLmt = (long)(255 / theEFO.GetTimeFactor());		//v3.1.T396 add
	theKeyPad.GetUserKeyInData(&m_nTime3, 1000, 0, lMaxLmt);	//v3.1.T396 edit
	UpdateData(false);

	ST_EFO_PROFILE stEfoProfileRcd;
	theEfoProfile.GetProfile(m_iFabType, &stEfoProfileRcd);
	stEfoProfileRcd.nTime3 = (WORD)m_nTime3;
	theEfoProfile.SetProfile(m_iFabType, &stEfoProfileRcd);
	theEFO.SendProfile();
}

void CSparkParaPage::OnFocusSeg3I()
{
	GetDlgItem(IDC_STATIC_PARAMETERS)->SetFocus();
	if (theMcConfig.GetEfoType() == 2)	//v3.0.T343 add
		theKeyPad.GetUserKeyInData(&m_nCurrent3,20,0,100);
	else
		theKeyPad.GetUserKeyInData(&m_nCurrent3,20,0,47);
	UpdateData(false);

	ST_EFO_PROFILE stEfoProfileRcd;
	theEfoProfile.GetProfile(m_iFabType, &stEfoProfileRcd);
	stEfoProfileRcd.nCurrent3 =(WORD)m_nCurrent3;
	theEfoProfile.SetProfile(m_iFabType, &stEfoProfileRcd);
	theEFO.SendProfile();
}
