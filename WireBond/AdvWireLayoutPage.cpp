/***************************************************************
AdvWireLayoutPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "Math.h"
#include "MathCalc.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Reference.h"
#include "LightingCtrlDlg.h"
#include "VisionSystem.h"
#include "TchBondPoints.h"
#include "TchAutoPadPatternDlg.h"
#include "AdvWireLayoutPage.h"
#include "SwapWireDlg.h"
#include "TeachRecipeSheet.h"
#include "AutoPadMgr.h"
#include "Wire.h"
#include "AutoPad.h"
#include "PrAlign.h"
#include "Bond.h"
#include "ScanKeyThread.h"
#include "servo.h"
#include <comutil.h>
#include "Busy.h"

// CAdvWireLayoutPage dialog
IMPLEMENT_DYNAMIC(CAdvWireLayoutPage, CPropertyPage)

CAdvWireLayoutPage::CAdvWireLayoutPage()
	: CPropertyPage(CAdvWireLayoutPage::IDD)
    , m_nTaughtPads(0)
    , m_iCurrentPad(0)
    , m_iBondPt(0)
    , m_i1stBondOn(0)
    , m_nTaught1stPoints(0)
    , m_nCurrentBondPoint(0)
    , m_nTaught2ndPoints(0)
    , m_nCheckWireNum(0)
    , m_sBondOn1(_T(""))
    , m_sBondOn2(_T(""))
    , m_i2ndBondOn(0)
{
	m_bBusy = FALSE;		//V1.1.W153 add
	m_hwndChild = NULL;		//V1.1.W153 add
}

CAdvWireLayoutPage::~CAdvWireLayoutPage()
{
}

void CAdvWireLayoutPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_TAUGHT_PAD_TYPES, m_nTaughtPads);
    DDX_CBIndex(pDX, IDC_COMBO_CURRENT_PAD, m_iCurrentPad);
    DDX_CBIndex(pDX, IDC_COMBO_BOND_POINT, m_iBondPt);
    DDX_CBIndex(pDX, IDC_COMBO_BOND_ON, m_i1stBondOn);
    DDX_Text(pDX, IDC_STATIC_TAUGHT_BOND_PT, m_nTaught1stPoints);
    DDX_Text(pDX, IDC_EDIT_CUR_BOND_PT, m_nCurrentBondPoint);
    DDX_Text(pDX, IDC_STATIC_TAUGHT_2ND_PT, m_nTaught2ndPoints);
    DDX_Text(pDX, IDC_EDIT_CHECK_WIRE, m_nCheckWireNum);
    DDX_Text(pDX, IDC_STATIC_BOND_ON1, m_sBondOn1);
    DDX_Text(pDX, IDC_STATIC_BOND_ON2, m_sBondOn2);
    DDX_CBIndex(pDX, IDC_COMBO_BOND_ON_2, m_i2ndBondOn);
}

BEGIN_MESSAGE_MAP(CAdvWireLayoutPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_TCH_PAD_TYPE, OnTchPadType)
    ON_BN_CLICKED(IDC_BUTTON_SINGLE_TCH, OnSingleTch)
    ON_CBN_SELCHANGE(IDC_COMBO_BOND_POINT, OnBondPoint)
    ON_CBN_SELCHANGE(IDC_COMBO_BOND_ON, OnBondOn)
    ON_BN_CLICKED(IDC_BUTTON_GROUP_TCH, OnGroupTch)
    ON_BN_CLICKED(IDC_BUTTON_EDIT_PAD, OnEditPad)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, OnDelete)
    ON_BN_CLICKED(IDC_BUTTON_EDIT, OnEdit)
    ON_BN_CLICKED(IDC_BUTTON_INSERT, OnInsert)
    ON_BN_CLICKED(IDC_BUTTON_AUTO_PAD, OnAutoPad)
    ON_CBN_SELCHANGE(IDC_COMBO_CURRENT_PAD, OnCurrentPad)
    ON_EN_SETFOCUS(IDC_EDIT_CUR_BOND_PT, OnFocusCurBondPt)
    ON_EN_SETFOCUS(IDC_EDIT_CHECK_WIRE, OnFocusCheckWire)
    ON_CBN_SELCHANGE(IDC_COMBO_BOND_ON_2, OnBondOn2)
    ON_BN_CLICKED(IDC_BUTTON_SWAP, OnSwap)
    ON_BN_CLICKED(IDC_BUTTON_ILL, OnIll)
    ON_BN_CLICKED(IDC_BUTTON_ILL_LIGHT, OnIllLight)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W153 add
END_MESSAGE_MAP()

// CAdvWireLayoutPage message handlers
BOOL CAdvWireLayoutPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_STATIC_PAD_PATTERN_MGR, theMsg.GetMsg(MSG_PAD_PATTERN_MGR));
    SetDlgItemTextW(IDC_STATIC_TAUGHT_PAD, theMsg.GetMsg(MSG_PAD_PATTERN));
    SetDlgItemTextW(IDC_STATIC_CURRENT_PAD, theMsg.GetMsg(MSG_CURRENT));
    SetDlgItemTextW(IDC_BUTTON_TCH_PAD_TYPE, theMsg.GetMsg(MSG_NEW));
    SetDlgItemTextW(IDC_BUTTON_EDIT_PAD, theMsg.GetMsg(MSG_EDIT));
    SetDlgItemTextW(IDC_STATIC_PROPERTY, theMsg.GetMsg(MSG_BOND_ON_PROPERTY));
    SetDlgItemTextW(IDC_STATIC_WIRE, theMsg.GetMsg(MSG_EDIT_WIRE));
    SetDlgItemTextW(IDC_STATIC_WIRE_BOND_ON, theMsg.GetMsg(MSG_REF));
    SetDlgItemTextW(IDC_STATIC_BOND_POINT, theMsg.GetMsg(MSG_MODE));
    SetDlgItemTextW(IDC_STATIC_BOND_ON, theMsg.GetMsg(MSG_REF));

    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
	short i, nTotalRef = psheet->m_lRecipeOperateMode == RECIPE_EDIT ? thePgm.GetUnitTotalDie() + 1 : thePgm.GetTotalRef();

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BOND_ON);
    pComboBox->AddString(theMsg.GetMsg(MSG_LEAD));
    
    if (nTotalRef <= 2)
        pComboBox->AddString(theMsg.GetMsg(MSG_DIE));
    else
    {
        for (i = 1; i < nTotalRef; i++)
            pComboBox->AddString(theMsg.GetMsg(MSG_DIE) + _bstr_t(i).GetBSTR());
    }

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BOND_ON_2);
    pComboBox->AddString(theMsg.GetMsg(MSG_LEAD));
    
    if (nTotalRef <= 2)
        pComboBox->AddString(theMsg.GetMsg(MSG_DIE));
    else
    {
        for (i = 1; i < nTotalRef; i++)
            pComboBox->AddString(theMsg.GetMsg(MSG_DIE) + _bstr_t(i).GetBSTR());
    }

    m_iBondPt = WIRE_BOND;
    m_i1stBondOn = 1;
    m_i2ndBondOn = 0;

    GetDlgItem(IDC_COMBO_BOND_ON)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_BOND_ON_2)->EnableWindow(TRUE);

    SetDlgItemTextW(IDC_STATIC_TCH_BOND_POINT, theMsg.GetMsg(MSG_TCH_BOND_PT_2));
    SetDlgItemTextW(IDC_STATIC_TAUGHT_POINTS, theMsg.GetMsg(MSG_TAUGHT_BOND_PT));
    SetDlgItemTextW(IDC_STATIC_CURRENT_BOND_POINT, theMsg.GetMsg(MSG_CURRENT_3));
    SetDlgItemTextW(IDC_BUTTON_SINGLE_TCH, theMsg.GetMsg(MSG_SINGLE));
    SetDlgItemTextW(IDC_BUTTON_GROUP_TCH, theMsg.GetMsg(MSG_GROUP));
    SetDlgItemTextW(IDC_BUTTON_EDIT, theMsg.GetMsg(MSG_EDIT));
    SetDlgItemTextW(IDC_BUTTON_DELETE, theMsg.GetMsg(MSG_DELETE));
    SetDlgItemTextW(IDC_BUTTON_INSERT, theMsg.GetMsg(MSG_INSERT));
    SetDlgItemTextW(IDC_BUTTON_SWAP, theMsg.GetMsg(MSG_SWAP));

    if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
        GetDlgItem(IDC_BUTTON_SINGLE_TCH)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(TRUE);
    }
    else
    {
		BYTE bEnable = (thePgm.GetDeviceSeq() == PGM_SINGLE)
					|| (thePgm.GetDeviceSeq() == PGM_MATRIX);

		GetDlgItem(IDC_BUTTON_SINGLE_TCH)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(bEnable);
    }

	GetDlgItem(IDC_BUTTON_GROUP_TCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(TRUE);

    if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
        m_nTaught1stPoints = theWire.Get1stBondTaughtPts();
        m_nTaught2ndPoints = theWire.Get2ndBondTaughtPts();

        m_nCurrentBondPoint = max(m_nTaught1stPoints, m_nTaught2ndPoints);
    }
    else
    {
        m_nTaught1stPoints = thePgm.GetUnitTotalWire();
        m_nTaught2ndPoints = m_nTaught1stPoints;

        m_nCurrentBondPoint = m_nTaught1stPoints;
    }

	m_nCheckWireNum = m_nCurrentBondPoint > 0 ? 1 : 0;

    if (m_nCurrentBondPoint > 0)
    {
        WIRE_RECORD stWireRcd;
        theWire.GetRcd(&stWireRcd, m_nCurrentBondPoint);
        m_i1stBondOn = stWireRcd.nRefNum[0];
        m_i2ndBondOn = stWireRcd.nRefNum[1];
    }

    UpdateMenuItems();

    theVisionDisplay.EnableDisplay(true);

	if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
	{
		REF_RECORD *pstRef = theRef.GetRcd(1);
		theServo.MoveTable2(pstRef->stManuAlignPos[0]);	//v3.1.T194 add
		theVisionSys.UpdateLightingParameter(
				pstRef->stManuAlignLightPara[0].nCoaxRedLight,
				pstRef->stManuAlignLightPara[0].bSideRedLight,
				pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
				pstRef->stManuAlignLightPara[0].bSideBlueLight);
	}

    return CPropertyPage::OnInitDialog();
}

HBRUSH CAdvWireLayoutPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAdvWireLayoutPage::OnTchPadType()
{
	if (theAutoPadMgr.GetTotalAutoPad() >= MAX_AUTO_PAD_TYPE) return;

	CTchAutoPadPatternDlg* pDlg = new CTchAutoPadPatternDlg(AUTO_PAD_TEACH, 0);
	pDlg->Create(CTchAutoPadPatternDlg::IDD, GetParent());

	m_hwndChild = pDlg->m_hWnd;	//V1.1.W153 add
}

void CAdvWireLayoutPage::UpdateMenuItems(void)
{
    short i, nTotalRef;
    CComboBox* pComboBox;
    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();

    m_nTaughtPads = theAutoPadMgr.GetTotalAutoPad();

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CURRENT_PAD);
    pComboBox->ResetContent();
    if (m_nTaughtPads == 0)
    {
        pComboBox->AddString(_T("None"));
    }
    else
    {
		for (i = 1; i <= m_nTaughtPads; i++)
			pComboBox->AddString(CString(_T("PadId"))+_bstr_t(i).GetBSTR());
    }

    m_nTaught1stPoints = theWire.Get1stBondTaughtPts();
    m_nTaught2ndPoints = theWire.Get2ndBondTaughtPts();

    if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
        nTotalRef = thePgm.GetTotalRef();
    else
    {
        nTotalRef = thePgm.GetUnitTotalDie() + 1;

        if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
        {
            m_nTaught1stPoints = thePgm.GetUnitTotalWire();
            m_nTaught2ndPoints = m_nTaught1stPoints;
        }
    }

    WIRE_RECORD stWireRcd;
    if (m_nCheckWireNum > 0)
        theWire.GetRcd(&stWireRcd, m_nCheckWireNum);
    
    if ((m_nCheckWireNum > 0) && (m_nCheckWireNum <= m_nTaught1stPoints))
    {
        if (stWireRcd.nRefNum[0] == 0)
			m_sBondOn1 = theMsg.GetMsg(MSG_LEAD);
        else
        {
			m_sBondOn1 = theMsg.GetMsg(MSG_DIE);

            if (nTotalRef > 2)
                m_sBondOn1 += _bstr_t(stWireRcd.nRefNum[0]).GetBSTR();
        }
    }
    else
        m_sBondOn1 = _T("N/A");

    if ((m_nCheckWireNum > 0) && (m_nCheckWireNum <= m_nTaught2ndPoints))
    {
        if (stWireRcd.nRefNum[1] == 0)
			m_sBondOn2 = theMsg.GetMsg(MSG_LEAD);
        else
        {
			m_sBondOn2 = theMsg.GetMsg(MSG_DIE);

            if (nTotalRef > 2)
                m_sBondOn2 += _bstr_t(stWireRcd.nRefNum[1]).GetBSTR();
        }
    }
    else
        m_sBondOn2 = _T("N/A");

    UpdateData(FALSE);
}

void CAdvWireLayoutPage::OnSingleTch()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
	{
		if (CheckPrAlignment() == FNOK) return;

		WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurrentBondPoint);
		if (m_iBondPt != SECOND_BOND)
			theServo.MoveTable2(pstWire->stTchPos[0]);	//v3.1.T194 add
		else
			theServo.MoveTable2(pstWire->stTchPos[1]);	//v3.1.T194 add
	}

	CTchBondPoints *pDlg = new CTchBondPoints((short)m_iBondPt,
						m_i1stBondOn, m_i2ndBondOn, TCH_NEW_BOND_POINT,
						m_nCurrentBondPoint, pSheet->m_lRecipeOperateMode);
	pDlg->Create(CTchBondPoints::IDD, pSheet);
	m_hwndChild = pDlg->m_hWnd;	//V1.1.W153 add
}

void CAdvWireLayoutPage::OnBondPoint()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    UpdateData();

    CWnd* pWnd;
    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();

    if (m_iBondPt == FIRST_BOND)
    {
        m_nCurrentBondPoint = m_nTaught1stPoints;

        GetDlgItem(IDC_COMBO_BOND_ON)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO_BOND_ON_2)->EnableWindow(FALSE);

        pWnd = GetDlgItem(IDC_BUTTON_GROUP_TCH);
        if (m_i1stBondOn > 0)
        {
            if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
                pWnd->EnableWindow(TRUE);
            else
            {
                if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
					|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
                    pWnd->EnableWindow(TRUE);
                else
                    pWnd->EnableWindow(FALSE);
            }
        }
        else
            pWnd->EnableWindow(FALSE);

        GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(FALSE);
    }
    else if (m_iBondPt == SECOND_BOND)
    {
        m_nCurrentBondPoint = m_nTaught2ndPoints;

        GetDlgItem(IDC_COMBO_BOND_ON)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_BOND_ON_2)->EnableWindow(TRUE);

        pWnd = GetDlgItem(IDC_BUTTON_GROUP_TCH);
        if (m_i2ndBondOn > 0)
        {
            if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
                pWnd->EnableWindow(TRUE);
            else
            {
                if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
					|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
                    pWnd->EnableWindow(TRUE);
                else
                    pWnd->EnableWindow(FALSE);
            }
        }
        else
            pWnd->EnableWindow(FALSE);

        GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(FALSE);
    }
    else
    {
        if (m_nTaught1stPoints == m_nTaught2ndPoints)
        {
            m_nCurrentBondPoint = m_nTaught1stPoints;

            GetDlgItem(IDC_COMBO_BOND_ON)->EnableWindow(TRUE);
            GetDlgItem(IDC_COMBO_BOND_ON_2)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_GROUP_TCH)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(TRUE);
        }
        else
        {
            CStringList sList;
            sList.AddTail(theMsg.GetMsg(MSG_BOND_PT_NOT_MATCH));
            theMsg.ShowMsg(WARNING_MSG, sList);

            GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(FALSE);

            if (m_nTaught1stPoints < m_nTaught2ndPoints)
            {
                m_nCurrentBondPoint = m_nTaught1stPoints;
                m_iBondPt = FIRST_BOND;

                GetDlgItem(IDC_COMBO_BOND_ON)->EnableWindow(TRUE);
                GetDlgItem(IDC_COMBO_BOND_ON_2)->EnableWindow(FALSE);

                pWnd = GetDlgItem(IDC_BUTTON_GROUP_TCH);
                if (m_i1stBondOn > 0)
                {
                    if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
                        pWnd->EnableWindow(TRUE);
                    else
                    {
                        if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
							|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
                            pWnd->EnableWindow(TRUE);
                        else
                            pWnd->EnableWindow(FALSE);
                    }
                }
                else
                    pWnd->EnableWindow(FALSE);
            }
            else
            {
                m_nCurrentBondPoint = m_nTaught2ndPoints;
                m_iBondPt = SECOND_BOND;

                GetDlgItem(IDC_COMBO_BOND_ON)->EnableWindow(FALSE);
                GetDlgItem(IDC_COMBO_BOND_ON_2)->EnableWindow(TRUE);

                pWnd = GetDlgItem(IDC_BUTTON_GROUP_TCH);
                if (m_i2ndBondOn > 0)
                {
                    if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
                        pWnd->EnableWindow(TRUE);
                    else
                    {
                        if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
							|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
                            pWnd->EnableWindow(TRUE);
                        else
                            pWnd->EnableWindow(FALSE);
                    }
                }
                else
                    pWnd->EnableWindow(FALSE);
            }
        }
    }

    UpdateData(FALSE);
}

void CAdvWireLayoutPage::OnBondOn()
{
    UpdateData();

    REF_RECORD stRefRcd;
    theRef.GetRcd(&stRefRcd, m_i1stBondOn);
    theVisionSys.UpdateLightingParameter(
			stRefRcd.stManuAlignLightPara[0].nCoaxRedLight,
			stRefRcd.stManuAlignLightPara[0].bSideRedLight,
			stRefRcd.stManuAlignLightPara[0].nCoaxBlueLight,
			stRefRcd.stManuAlignLightPara[0].bSideBlueLight);

    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_GROUP_TCH);

    if ((m_iBondPt == FIRST_BOND) && (m_i1stBondOn > 0))
    {
        if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
            pWnd->EnableWindow(TRUE);
        else
        {
            if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
                pWnd->EnableWindow(TRUE);
            else
                pWnd->EnableWindow(FALSE);
        }
    }
    else
        pWnd->EnableWindow(FALSE);
}

void CAdvWireLayoutPage::OnGroupTch()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
	{
		if (CheckPrAlignment() == FNOK) return;

		WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurrentBondPoint);        
		if (m_iBondPt == FIRST_BOND)
			theServo.MoveTable2(pstWire->stTchPos[0]);	//v3.1.T194 add
		else
			theServo.MoveTable2(pstWire->stTchPos[1]);	//v3.1.T194 add
	}

	CTchBondPoints *pDlg = new CTchBondPoints((short)m_iBondPt,
						m_i1stBondOn, m_i2ndBondOn, TCH_GROUP_BOND_POINT,
						m_nCurrentBondPoint, pSheet->m_lRecipeOperateMode);
	pDlg->Create(CTchBondPoints::IDD, pSheet);
	m_hwndChild = pDlg->m_hWnd;	//V1.1.W153 add
}

void CAdvWireLayoutPage::OnEditPad()
{
	if (m_iCurrentPad >= theAutoPadMgr.GetTotalAutoPad()) return;

	CTchAutoPadPatternDlg* pDlg = new CTchAutoPadPatternDlg(AUTO_PAD_EDIT, m_iCurrentPad);
	pDlg->Create(CTchAutoPadPatternDlg::IDD, GetParent());

	m_hwndChild = pDlg->m_hWnd;	//V1.1.W153 add
}

void CAdvWireLayoutPage::OnDelete()
{
    if (m_nCurrentBondPoint < 1) return;
    
    if (m_iBondPt == FIRST_BOND)
        theWire.Delete1stBondPoint(m_nCurrentBondPoint);
    else if (m_iBondPt == SECOND_BOND)
        theWire.Delete2ndBondPoint(m_nCurrentBondPoint);
    else
    {
        theWire.Delete1stBondPoint(m_nCurrentBondPoint);
        theWire.Delete2ndBondPoint(m_nCurrentBondPoint);
    }

    UpdateMenuItems();

    if (m_iBondPt != SECOND_BOND)
    {
        if (m_nCurrentBondPoint > m_nTaught1stPoints)
            m_nCurrentBondPoint = m_nTaught1stPoints;
    }
    else
    {
        if (m_nCurrentBondPoint > m_nTaught2ndPoints)
            m_nCurrentBondPoint = m_nTaught2ndPoints;
    }

    theVisionDisplay.UpdateVisionDisplay(true);

    UpdateData(FALSE);
}

void CAdvWireLayoutPage::OnEdit()
{
    if (m_nCurrentBondPoint < 1) return;

	CBusy bs(&m_bBusy);	//V1.1.W153 add

    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();

    if (psheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        if (CheckPrAlignment() == FNOK) return;

        if (m_iBondPt != SECOND_BOND)
        {
            if (m_nCurrentBondPoint <= m_nTaught1stPoints)
                theBond.MoveToWirePoint(m_nCurrentBondPoint, FIRST_BOND);
        }
        else
        {
            if (m_nCurrentBondPoint <= m_nTaught2ndPoints)
                theBond.MoveToWirePoint(m_nCurrentBondPoint, SECOND_BOND);
        }
    }

    CTchBondPoints* pDlg = new CTchBondPoints((short)m_iBondPt,
				m_i1stBondOn, m_i2ndBondOn, TCH_EDIT_BOND_POINT,
				m_nCurrentBondPoint, psheet->m_lRecipeOperateMode);
    pDlg->Create(CTchBondPoints::IDD, psheet);

	m_hwndChild = pDlg->m_hWnd;	//V1.1.W153 add
}

void CAdvWireLayoutPage::OnInsert()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
	{
		if (CheckPrAlignment() == FNOK) return;

		WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurrentBondPoint);
		if (m_iBondPt == FIRST_BOND)
			theServo.MoveTable2(pstWire->stTchPos[0]);	//v3.1.T194 add
		else
			theServo.MoveTable2(pstWire->stTchPos[1]);	//v3.1.T194 add
	}

	CTchBondPoints *pDlg = new CTchBondPoints((short)m_iBondPt,
					m_i1stBondOn, m_i2ndBondOn, TCH_INSERT_BOND_POINT,
					m_nCurrentBondPoint + 1, pSheet->m_lRecipeOperateMode);
	pDlg->Create(CTchBondPoints::IDD, pSheet);
	m_hwndChild = pDlg->m_hWnd;	//V1.1.W153 add
}

void CAdvWireLayoutPage::OnAutoPad()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
    if (psheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        if (CheckPrAlignment() == FNOK) return;
    }

    theVisionDisplay.EnableDisplay(false);
    if (psheet->m_lRecipeOperateMode == RECIPE_EDIT)
        theAutoPad.PerformAutoPadWithPR();
    else
        theAutoPad.PerformAutoPadWithoutPR();

    theVisionDisplay.EnableDisplay(true);
    theVisionDisplay.UpdateVisionDisplay(true);
}

void CAdvWireLayoutPage::OnCurrentPad()
{
	UpdateData();

	if (m_iCurrentPad >= theAutoPadMgr.GetTotalAutoPad()) return;

	TBL_POS stPos, stVisionOffset;

	AUTO_PAD_TYPE stPadRcd;
	theAutoPadMgr.GetRcd(m_iCurrentPad, &stPadRcd);
	theVisionSys.UpdateLightingParameter(
					stPadRcd.stPrLightPara.nCoaxRedLight,
					stPadRcd.stPrLightPara.bSideRedLight,
					stPadRcd.stPrLightPara.nCoaxBlueLight,
					stPadRcd.stPrLightPara.bSideBlueLight);
	stPos.dx = stPadRcd.nPadWidth;
	stPos.dy = stPadRcd.nPadHeight;
	theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stPos, &stVisionOffset);
	theVisionSys.SetSearchSize(round(stVisionOffset.dx), round(stVisionOffset.dy));
}

void CAdvWireLayoutPage::OnFocusCurBondPt()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    GetDlgItem(IDC_BUTTON_EDIT)->SetFocus();

    if (m_iBondPt == SECOND_BOND)
    {
        theKeyPad.GetUserKeyInData(&m_nCurrentBondPoint, 1, 1, m_nTaught2ndPoints);
        theBond.MoveToWirePoint(m_nCurrentBondPoint, SECOND_BOND);
    }
    else
    {
        theKeyPad.GetUserKeyInData(&m_nCurrentBondPoint, 1, 1, m_nTaught1stPoints);
        theBond.MoveToWirePoint(m_nCurrentBondPoint, FIRST_BOND);
    }

    if (m_nCurrentBondPoint > 0)
    {
        WIRE_RECORD stWireRcd;
        theWire.GetRcd(&stWireRcd, m_nCurrentBondPoint);
        m_i1stBondOn = stWireRcd.nRefNum[0];
        m_i2ndBondOn = stWireRcd.nRefNum[1];
    }

    UpdateData(false);
}

LRESULT CAdvWireLayoutPage::OnWizardNext()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    if (m_nTaught1stPoints != m_nTaught2ndPoints)
    {
        CStringList sList;
        sList.AddTail(theMsg.GetMsg(MSG_BOND_PT_NOT_MATCH));
        theMsg.ShowMsg(WARNING_MSG, sList);
        return -1;
    }

    theWire.NormalizeWireLength();

    return CPropertyPage::OnWizardNext();
}

// Check PR alignment status
int CAdvWireLayoutPage::CheckPrAlignment(void)
{
    long lStatus = FOK;
    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();

    if (psheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        if (psheet->m_bPrAlignDone == false)
        {
            theBond.ResetMatrixPrStatus(1);

            bool bBkupIll = theBond.IsIll();
            theBond.SetIll(false);
          
            lStatus = theBond.PerformVisionAlignment(true, VISION_ACTION_SINGLE_UNIT);
            if (lStatus == QUIT_AUTO_BOND)
                lStatus = FNOK;
            else
            {
                lStatus = FOK;
				theVisionDisplay.EnableDisplay(true); 
                psheet->m_bPrAlignDone = true;
            }

			theBond.SetIll(bBkupIll);
            theVisionSys.EnableVideo(true);
        }
    }

    return lStatus;
}

void CAdvWireLayoutPage::OnFocusCheckWire()
{
    GetDlgItem(IDC_BUTTON_EDIT)->SetFocus();
    
    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
    short nTotalWire = psheet->m_lRecipeOperateMode == RECIPE_TEACH ? theWire.GetTotalWireNum() : thePgm.GetUnitTotalWire();
    if (nTotalWire < 1) return;

    theKeyPad.GetUserKeyInData(&m_nCheckWireNum, m_nCheckWireNum, 1, nTotalWire);
    
    UpdateMenuItems();

    if (m_nCheckWireNum <= m_nTaught1stPoints)
    {
        theBond.MoveToWirePoint(m_nCheckWireNum, FIRST_BOND);
    }
}

void CAdvWireLayoutPage::OnBondOn2()
{
    UpdateData();

    REF_RECORD stRefRcd;
    theRef.GetRcd(&stRefRcd, m_i2ndBondOn);
    theVisionSys.UpdateLightingParameter(
			stRefRcd.stManuAlignLightPara[0].nCoaxRedLight,
			stRefRcd.stManuAlignLightPara[0].bSideRedLight,
			stRefRcd.stManuAlignLightPara[0].nCoaxBlueLight,
			stRefRcd.stManuAlignLightPara[0].bSideBlueLight);

    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_GROUP_TCH);

    if ((m_iBondPt == SECOND_BOND) && (m_i2ndBondOn > 0))
    {
        if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
            pWnd->EnableWindow(TRUE);
        else
        {
            if ((thePgm.GetDeviceSeq() == PGM_SINGLE)
				|| (thePgm.GetDeviceSeq() == PGM_MATRIX))
                pWnd->EnableWindow(TRUE);
            else
                pWnd->EnableWindow(FALSE);
        }
    }
    else
        pWnd->EnableWindow(FALSE);
}

void CAdvWireLayoutPage::OnSwap()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    short nTotalWire, n1stSwapWire, n2ndSwapWire;
    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
    if (psheet->m_lRecipeOperateMode == RECIPE_TEACH)
        nTotalWire = theWire.GetTotalWireNum();
    else
        nTotalWire = thePgm.GetUnitTotalWire();

    if (nTotalWire < 2) return;

    CSwapWireDlg dlg(nTotalWire);
    dlg.DoModal();
    dlg.GetSwapWireNumber(&n1stSwapWire, &n2ndSwapWire);

    theWire.SwapWireRecord(n1stSwapWire, n2ndSwapWire);

    theVisionDisplay.UpdateVisionDisplay(true);
}

void CAdvWireLayoutPage::OnIll()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
    if ((psheet->m_lRecipeOperateMode == RECIPE_EDIT)
		&& (CheckPrAlignment() == FNOK)) return;

	theVisionDisplay.EnableDisplay(false);

	thePrAlign.PerformInteliLeadLocate(psheet->m_lRecipeOperateMode == RECIPE_EDIT);

	theVisionDisplay.EnableDisplay(true);
	theVisionDisplay.UpdateVisionDisplay(true);
}

void CAdvWireLayoutPage::OnIllLight()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    short nSideRedLight, nSideBlueLight, nWireNum = 0;
    int lMoveDistX, lMoveDistY;
    TBL_POS stCurTblPos;

    theScanKeyThread.DisableJoyStick();

    theServo.GetTablePosition(&stCurTblPos);

    if (thePrAlign.GetNextILLWire(0, FIRST_BOND, &nWireNum) == FOK)
    {
        if ((nWireNum > 0) && (nWireNum <= thePgm.GetTotalWire()))
        {
			WIRE_RECORD stWireRcd;
            theWire.GetRcd(&stWireRcd, nWireNum);

            if (stWireRcd.nRefNum[FIRST_BOND] == 0)
            {
                lMoveDistX = round(fabs(stCurTblPos.dx - stWireRcd.stTchPos[0].dx) * theServo.GetResolXY());
                lMoveDistY = round(fabs(stCurTblPos.dy - stWireRcd.stTchPos[0].dy) * theServo.GetResolXY());

                if ((lMoveDistX > 0) || (lMoveDistY > 0))
                {
                    theServo.SetServoSpeedProfile(MOTOR_X, GetTableAutoSpeedProfile(lMoveDistX), false);
                    theServo.SetServoSpeedProfile(MOTOR_Y, GetTableAutoSpeedProfile(lMoveDistY), false);
                    Sleep(1);
                    theServo.FastMoveTable(&stWireRcd.stTchPos[0], MOTION_ABSOLUTE, MOTION_WITH_WAIT);
                }
            }
        }
    }
    else
    {
        if (thePrAlign.GetNextILLWire(0, SECOND_BOND, &nWireNum) != FOK)return;

        if ((nWireNum > 0) && (nWireNum <= thePgm.GetTotalWire()))
        {
			WIRE_RECORD stWireRcd;
            theWire.GetRcd(&stWireRcd, nWireNum);

            if (stWireRcd.nRefNum[SECOND_BOND] == 0)
            {
                lMoveDistX = round(fabs(stCurTblPos.dx - stWireRcd.stTchPos[1].dx) * theServo.GetResolXY());
                lMoveDistY = round(fabs(stCurTblPos.dy - stWireRcd.stTchPos[1].dy) * theServo.GetResolXY());

                if ((lMoveDistX > 0) || (lMoveDistY > 0))
                {
                    theServo.SetServoSpeedProfile(MOTOR_X, GetTableAutoSpeedProfile(lMoveDistX), false);
                    theServo.SetServoSpeedProfile(MOTOR_Y, GetTableAutoSpeedProfile(lMoveDistY), false);
                    Sleep(1);
                    theServo.FastMoveTable(&stWireRcd.stTchPos[1], MOTION_ABSOLUTE, MOTION_WITH_WAIT);
                }
            }
        }
    }

    theScanKeyThread.EnableJoyStick();

    PGM_RECORD *pstPgm = thePgm.GetRcd();

    CLightingCtrlDlg* pLightCtrlDlg = new CLightingCtrlDlg(false);

    theVisionSys.UpdateLightingParameter(
						pstPgm->stVllLight.nCoaxRedLight,
						pstPgm->stVllLight.bSideRedLight, 
						pstPgm->stVllLight.nCoaxBlueLight,
						pstPgm->stVllLight.bSideBlueLight);
    
    pLightCtrlDlg->DoModal();

    delete pLightCtrlDlg;

    theVisionSys.GetLightingParameter(
		&pstPgm->stVllLight.nCoaxRedLight, &nSideRedLight,
		&pstPgm->stVllLight.nCoaxBlueLight, &nSideBlueLight);
    pstPgm->stVllLight.bSideRedLight = (byte)nSideRedLight;
    pstPgm->stVllLight.bSideBlueLight = (byte)nSideBlueLight;
    theVisionSys.GetBinaryThreshold(&pstPgm->stVllLight.nImageThreshold);
}

BOOL CAdvWireLayoutPage::OnSetActive()
{
	CTeachRecipeSheet* pSheet = (CTeachRecipeSheet*)GetParent();

	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
	{
		pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(FALSE);
	}

	return CPropertyPage::OnSetActive();
}

void CAdvWireLayoutPage::CopyUI(CWnd* pWnd)
{
	CString str;

	GetDlgItemText(IDC_STATIC_BOND_ON, str);
	pWnd->SetDlgItemText(IDC_STATIC_BOND_ON, str);

	CComboBox* pCbSrc = (CComboBox*)GetDlgItem(IDC_COMBO_BOND_ON);
	CComboBox* pCbDes = (CComboBox*)pWnd->GetDlgItem(IDC_COMBO_BOND_ON);
	for (BYTE n=0; n<pCbSrc->GetCount(); n++)
	{
		pCbSrc->GetLBText(n, str);
		pCbDes->AddString(str);
	}
	pCbDes->SetCurSel(pCbSrc->GetCurSel());

	pCbSrc = (CComboBox*)GetDlgItem(IDC_COMBO_BOND_ON_2);
	pCbDes = (CComboBox*)pWnd->GetDlgItem(IDC_COMBO_BOND_ON_2);
	for(BYTE n=0; n<pCbSrc->GetCount(); n++)
	{
		pCbSrc->GetLBText(n, str);
		pCbDes->AddString(str);
	}
	pCbDes->SetCurSel(pCbSrc->GetCurSel());
}

void CAdvWireLayoutPage::OnDestroy()
{
	theWire.NormalizeWireLength();

	theBond.m_nEndWire = theWire.GetTotalWireNum();
	if (theBond.m_nCurWire > theBond.m_nEndWire)
		theBond.m_nCurWire = theBond.m_nEndWire;
	if (theBond.m_nRestartWire > theBond.m_nEndWire)
		theBond.m_nRestartWire = theBond.m_nEndWire;
}

LRESULT CAdvWireLayoutPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W153 add
{
	return ((m_bBusy == TRUE) || (IsWindow(m_hwndChild) == TRUE));
}
