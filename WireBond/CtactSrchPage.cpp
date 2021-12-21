/***************************************************************
CtactSrchPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Reference.h"
#include "TeachRecipeSheet.h"
#include "CtactSrchPage.h"
#include "ContactLevel.h"
#include "VisionSystem.h"
#include "Bond.h"
#include "MaterialHandlerThread.h"
#include "servo.h"
#include <comutil.h>

// CCtactSrchPage dialog

IMPLEMENT_DYNAMIC(CCtactSrchPage, CPropertyPage)

CCtactSrchPage::CCtactSrchPage()
	: CPropertyPage(CCtactSrchPage::IDD)
	, m_nTotalRef(1)
	, m_nCurRef(0)
	, m_nSrchSpeed(2)
	, m_nSrchTol(24) 
	, m_nObjectWireNum(1)
{
}

CCtactSrchPage::~CCtactSrchPage()
{
}

void CCtactSrchPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CTACT_SRCH_SPEED, m_nSrchSpeed);
	DDX_Text(pDX, IDC_CTACT_TOL, m_nSrchTol);
}

BEGIN_MESSAGE_MAP(CCtactSrchPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_CTACT_TOL, OnFocusCtactTol)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnPrev)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CCtactSrchPage::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CCtactSrchPage message handlers
BOOL CCtactSrchPage::OnInitDialog()
{
	m_nTotalRef = thePgm.GetUnitTotalDie() + 1;

	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_CTACT_PARA, theMsg.GetMsg(MSG_CTACT_PARA));
	SetDlgItemTextW(IDC_SRCH_SPEED, theMsg.GetMsg(MSG_SPEED));
	SetDlgItemTextW(IDC_SRCH_TOL, theMsg.GetMsg(MSG_THRESHOLD));
	SetDlgItemTextW(IDC_STATIC_REF_INFO, theMsg.GetMsg(MSG_REF_INFO));

	UpdateRefInfo();

	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(theRef.GetTotalRefNum() >= 1);

	SetDlgItemTextW(IDC_BUTTON_LAST, theMsg.GetMsg(MSG_PREV));
	SetDlgItemTextW(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->AddString(theMsg.GetMsg(MSG_START_TO_TEACH_CTACT_LEVEL));

	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
	{
		pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(false);
		pSheet->GetDlgItem(IDCANCEL)->EnableWindow(false);
	}

	return CPropertyPage::OnInitDialog();
}

void CCtactSrchPage::OnStart()
{
	theMHS.CloseWindowClamp();
	theCtact.ClearRcd();
	UpdateData();

	if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
		m_nObjectWireNum = thePgm.GetTotalWire() / 2;
	else
		m_nObjectWireNum = 1;

	const WIRE_RECORD *pstWire = theWire.GetRcd(m_nObjectWireNum);
	const REF_RECORD *pstRef;
	if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
		pstRef = theRef.GetRcd(pstWire->nRefNum[0]);
	else
		pstRef = theRef.GetRcd(pstWire->nRefNum[1]);

	theServo.MoveTable2(pstRef->stManuAlignPos[0]);	//v3.1.T43 edit
	theVisionSys.UpdateLightingParameter(
			pstRef->stManuAlignLightPara[0].nCoaxRedLight,
			pstRef->stManuAlignLightPara[0].bSideRedLight,
			pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
			pstRef->stManuAlignLightPara[0].bSideBlueLight);

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	pListBox->AddString(theMsg.GetMsg(MSG_TEACH_CTACT_LEVEL) + theMsg.GetMsg(MSG_LEAD));
	pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
	pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow();
	GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(false);
	GetDlgItem(IDC_COMBO_CTACT_SRCH_SPEED)->EnableWindow(false);

	UpdateData(false);
	m_nCurRef = 0;
}

void CCtactSrchPage::OnNext()
{
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);	//v1.6.P121 add

	if (theMcData.IsZeroBtosOptics() && (m_nCurRef == 0))
	{
		TBL_POS stCompPos = TBL_POS(round(250.0 / theServo.GetResolXY() * tan(18.0 / 180.0 * PI)),0);
		theServo.MoveTable2(stCompPos, MTN_REL);	//v3.1.T43 edit
		Sleep_x1ms(20);
	}

	theServo.MoveTable2(theMcData.GetBondTipOs(), MTN_REL);	//v3.1.T43 edit
	Sleep_x1ms(200);

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	long lCtactLvl;
	if (theBond.CtactSrch(m_nSrchSpeed+1, m_nSrchTol, &lCtactLvl) != FOK)
	{
		pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow();
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
		UpdateData(false);
		return;
	}

	pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_SUCCESS));
	long lLogicLv = lCtactLvl - theMcData.GetFlatLevel();
	pListBox->AddString(theMsg.GetMsg(MSG_NEW_CTACT_LEVEL) + _bstr_t(lLogicLv).GetBSTR());

	REF_RECORD *pstRef = theRef.GetRcd(m_nCurRef);
	pstRef->lCtactLvl = lCtactLvl;
	pstRef->bCtactLvlValid = true;

	short i, nUnitTotalDie, nUnitDieNum;
	switch (thePgm.GetDeviceSeq())
	{
	case PGM_HYBRID: case PGM_DIE_AHEAD:
	case PGM_MAT_HYBRID: case PGM_MAT_DIE_AHEAD:
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie();
		nUnitDieNum = (nUnitTotalDie == 1 ? 1 : m_nCurRef % nUnitTotalDie);

		if (m_nCurRef > 0)
		{
			for (i = 1; i < theRef.GetTotalRefNum(); i++)
			{
				if (nUnitTotalDie == 1)
				{
					pstRef = theRef.GetRcd(i);
					pstRef->lCtactLvl = lCtactLvl;
					pstRef->bCtactLvlValid = true;
				}
				else if ((i % nUnitTotalDie) == nUnitDieNum)
				{
					pstRef = theRef.GetRcd(i);
					pstRef->lCtactLvl = lCtactLvl;
					pstRef->bCtactLvlValid = true;
				}
			}
		}
		break;
	}
	case PGM_DIE_LEAD_AHEAD:
	{
		nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
		nUnitDieNum = m_nCurRef % nUnitTotalDie;

		for (i = 0; i < theRef.GetTotalRefNum(); i++)
		{
			if ((i % nUnitTotalDie) != nUnitDieNum) continue;

			pstRef = theRef.GetRcd(i);
			pstRef->lCtactLvl = lCtactLvl;
			pstRef->bCtactLvlValid = true;
		}
		break;
	}
	case PGM_SINGLE:	//v3.1.T29sc add
	case PGM_MATRIX:	//v3.1.T57sc add
	{
		if (!theMcConfig.IsSharePR()) break;

		if (m_nCurRef == 1)
		{
			REF_RECORD *pstCurRef;
			for (int i = 2; i < m_nTotalRef; i++)
			{
				pstCurRef = theRef.GetRcd(i);
				pstCurRef->lCtactLvl = pstRef->lCtactLvl;
				pstCurRef->bCtactLvlValid = true;
			}
			m_nCurRef = m_nTotalRef;
		}
		break;
	}
	}

	UpdateRefInfo();

	m_nCurRef++;
	if (m_nCurRef < m_nTotalRef)
	{
		if (thePgm.GetDeviceSeq() == PGM_MATRIX)
		{
			pstRef = theRef.GetRcd(m_nCurRef);
			theServo.MoveTable2(pstRef->stManuAlignPos[0]);	//v3.1.T43 edit
		}
		else
		{
			const WIRE_RECORD *pstWire = theWire.GetRcd(m_nObjectWireNum);
			pstRef = theRef.GetRcd(pstWire->nRefNum[0]);
			theServo.MoveTable2(pstRef->stManuAlignPos[0]);	//v3.1.T43 edit
		}

		theVisionSys.UpdateLightingParameter(
				pstRef->stManuAlignLightPara[0].nCoaxRedLight,
				pstRef->stManuAlignLightPara[0].bSideRedLight,
				pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
				pstRef->stManuAlignLightPara[0].bSideBlueLight);

		pListBox->AddString(_T(""));
		pListBox->AddString(theMsg.GetMsg(MSG_TEACH_CTACT_LEVEL)
				+ theMsg.GetMsg(MSG_DIE) + _bstr_t(m_nCurRef).GetBSTR());
		pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow();	//v1.6.P121 add
		UpdateData(false);
	}
	else 
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
		UpdateData(false);

		TBL_POS stPos = TBL_POS(-theMcData.GetBondTipOs().dx, -theMcData.GetBondTipOs().dy);
		theServo.MoveTable2(stPos, MTN_REL);	//v3.1.T43 edit

		CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
		if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
			pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow();
		else if (pSheet->m_lRecipeOperateMode == RECIPE_SETUP)
			pSheet->GetDlgItem(IDOK)->EnableWindow();
	}

	GetDlgItem(IDC_BUTTON_LAST)->EnableWindow();
}

HBRUSH CCtactSrchPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCtactSrchPage::OnFocusCtactTol()
{
	GetDlgItem(IDC_STATIC_CTACT_PARA)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSrchTol, 24, 10, 100);
	UpdateData(false);
}

void CCtactSrchPage::OnDestroy()
{
	theApp.ArchivePowerOnData(ARCHIVE_SAVE);
	CPropertyPage::OnDestroy();
}

void CCtactSrchPage::UpdateRefInfo(void)
{
	CString sRefInfo = theMsg.GetMsg(MSG_REFERENCE) + _T("\t");
	if (lstrlenA((LPSTR)sRefInfo.GetBuffer()) > 1)
		sRefInfo += _T("\t");
	sRefInfo += theMsg.GetMsg(MSG_TYPE) + _T("\t\t");
	sRefInfo += theMsg.GetMsg(MSG_CTACT_LEVEL) + _T("\r\n");

	const REF_RECORD *pstRef;
	short nTotalRef = thePgm.GetUnitTotalDie() + 1;
	for (short i = 0; i < nTotalRef; i++)
	{
		pstRef = theRef.GetRcd(i);
		sRefInfo.AppendFormat(_T("%d\t\t"), i);
		sRefInfo += theMsg.GetMsg(pstRef->nRefType == REF_TYPE_LEAD ? MSG_LEAD : MSG_DIE);
		sRefInfo.AppendFormat(_T("\t\t%d\r\n"), pstRef->lCtactLvl - theMcData.GetFlatLevel());
	}

	SetDlgItemText(IDC_STATIC_REF_INFO_LIST, sRefInfo);
}

void CCtactSrchPage::OnPrev()
{
	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	const REF_RECORD *pstRef;
	TBL_POS stPos;

	m_nCurRef--;
	if (m_nCurRef == 0)
	{
		pstRef = theRef.GetRcd(m_nCurRef);
		theServo.MoveTable2(pstRef->stManuAlignPos[0]);	//v3.1.T43 edit
		theVisionSys.UpdateLightingParameter(
				pstRef->stManuAlignLightPara[0].nCoaxRedLight,
				pstRef->stManuAlignLightPara[0].bSideRedLight,
				pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
				pstRef->stManuAlignLightPara[0].bSideBlueLight);

		pListBox->ResetContent();
		pListBox->AddString(theMsg.GetMsg(MSG_LEAD));
		pListBox->AddString(_T(""));
		pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow();
		GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(false);
		UpdateData(false);
	}
	else if (m_nCurRef > 0)
	{
		pListBox->ResetContent();
		pstRef = theRef.GetRcd(m_nCurRef);
		theServo.MoveTable2(pstRef->stManuAlignPos[0]);	//v3.1.T43 edit
		theVisionSys.UpdateLightingParameter(
				pstRef->stManuAlignLightPara[0].nCoaxRedLight,
				pstRef->stManuAlignLightPara[0].bSideRedLight,
				pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
				pstRef->stManuAlignLightPara[0].bSideBlueLight);
		Sleep_x1ms(200);

		pListBox->AddString(theMsg.GetMsg(MSG_DIE)+_bstr_t(m_nCurRef).GetBSTR());
		pListBox->AddString(_T(""));
		pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow();
		UpdateData(false);
	}
}

void CCtactSrchPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_RECIPE, 0);	//v3.0.T454 add
}
