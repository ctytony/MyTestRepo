/***************************************************************
TchVisionPatternPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "TchVisionPatternPage.h"
#include "TeachRecipeSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "VisionSystem.h"
#include "Bond.h"
#include "MaterialHandlerThread.h"
#include "stdexcept"
#include "servo.h"
#include <memory>
#include <atlimage.h>
#include <comutil.h>
#include "Busy.h"

// CTchVisionPatternPage dialog
IMPLEMENT_DYNAMIC(CTchVisionPatternPage, CPropertyPage)

CTchVisionPatternPage::CTchVisionPatternPage()
	: CPropertyPage(CTchVisionPatternPage::IDD)
	, m_nVisionAlgo(0)
	, m_nPatSizeX(192)
	, m_nPatSizeY(144)
	, m_nLensMag(0)
	, m_nTotalAlignStep(0)
	, m_nAlignStep(0)
	, m_nLocateMode(0)
	, m_nLastRef(0)
	, m_nCopyFrom(0)	//v3.1.T422 add
{
	m_bBusy = FALSE;	//V1.1.W153 add
}

CTchVisionPatternPage::~CTchVisionPatternPage()
{
}

void CTchVisionPatternPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_ALGORITHM, m_nVisionAlgo);
    DDX_Text(pDX, IDC_EDIT_SIZE_X, m_nPatSizeX);
    DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_nPatSizeY);
    DDX_CBIndex(pDX, IDC_COMBO_LENS_MAG, m_nLensMag);
    DDX_CBIndex(pDX, IDC_COMBO_LOCATE_MODE, m_nLocateMode);
    DDX_CBIndex(pDX, IDC_COMBO_COPY_FROM, m_nCopyFrom);	//v3.1.T422 add
}

BEGIN_MESSAGE_MAP(CTchVisionPatternPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
    ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnContinue)
    ON_CBN_SELCHANGE(IDC_COMBO_LENS_MAG, OnLensMag)
    ON_EN_CHANGE(IDC_EDIT_SIZE_X, OnEditSizeX)
    ON_EN_CHANGE(IDC_EDIT_SIZE_Y, OnEditSizeY)
    ON_EN_SETFOCUS(IDC_EDIT_SIZE_X, OnFocusSizeX)
    ON_EN_SETFOCUS(IDC_EDIT_SIZE_Y, OnFocusSizeY)
    ON_CBN_SELCHANGE(IDC_COMBO_LOCATE_MODE, OnLocateMode)
    ON_CBN_SELCHANGE(IDC_COMBO_ALGORITHM, OnAlgorithm)
    ON_BN_CLICKED(IDC_BUTTON_SKIP, OnSkip)
    ON_BN_CLICKED(IDC_BUTTON_MASK, OnMask)
	ON_MESSAGE(WM_REFRESH, OnRefresh)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W153 add
END_MESSAGE_MAP()

// CTchVisionPatternPage message handlers
BOOL CTchVisionPatternPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	pSheet->m_bPrAlignDone = false;
	GetDlgItem(IDC_BUTTON_SKIP)->ShowWindow(pSheet->m_lRecipeOperateMode != RECIPE_TEACH);

    SetDlgItemText(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
    SetDlgItemText(IDC_BUTTON_CONTINUE, theMsg.GetMsg(MSG_CONTINUE));
    SetDlgItemText(IDC_BUTTON_SKIP, theMsg.GetMsg(MSG_SKIP));
	SetDlgItemText(IDC_BUTTON_MASK, theMsg.GetMsg(MSG_ENHANCE));
    SetDlgItemText(IDC_STATIC_PATTERN_PARA, theMsg.GetMsg(MSG_PATTERN_PARA));
    SetDlgItemText(IDC_STATIC_VISION_ALGORITHM, theMsg.GetMsg(MSG_VISION_ALGORITHM));
    SetDlgItemText(IDC_STATIC_LOCATE_MODE, theMsg.GetMsg(MSG_LOCATE_MODE));
    SetDlgItemText(IDC_STATIC_SIZE_X, theMsg.GetMsg(MSG_PATTERN_SIZE_X));
    SetDlgItemText(IDC_STATIC_SIZE_Y, theMsg.GetMsg(MSG_PATTERN_SIZE_Y));
    SetDlgItemText(IDC_STATIC_LENS_SETTING, theMsg.GetMsg(MSG_LENS_SETTING));
    SetDlgItemText(IDC_STATIC_MAGNIFICATION, theMsg.GetMsg(MSG_MAGNIFICATION));

    CWnd* pWnd = GetDlgItem(IDC_COMBO_ALGORITHM);
    ((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_GREY_LEVEL));
    ((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_BINARY));

    pWnd = GetDlgItem(IDC_COMBO_LOCATE_MODE);
    ((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_DUAL_PT));
    ((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_SINGLE_PT));

    pWnd = GetDlgItem(IDC_SPIN_SIZE_X);
    ((CSpinButtonCtrl*)pWnd)->SetBuddy(GetDlgItem(IDC_EDIT_SIZE_X));
    ((CSpinButtonCtrl*)pWnd)->SetRange(0, 256);

    pWnd = GetDlgItem(IDC_SPIN_SIZE_Y);
    ((CSpinButtonCtrl*)pWnd)->SetBuddy(GetDlgItem(IDC_EDIT_SIZE_Y));
    ((CSpinButtonCtrl*)pWnd)->SetRange(0, 192);

	//v3.1.T422 add
	pWnd = GetDlgItem(IDC_COMBO_COPY_FROM);
    ((CComboBox*)pWnd)->AddString(_T("None"));

	pWnd = GetDlgItem(IDC_LIST_INFO);
    ((CListBox*)pWnd)->AddString(theMsg.GetMsg(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ? MSG_START_TO_TEACH_VISION_PATTERN : MSG_START_TO_EDIT_PROGRAM));

	theVisionDisplay.EnableDisplay(false);

	((CWireBondDlg*)theApp.m_pMainWnd)->SetDrawAreaEnable(m_hWnd);

    return CPropertyPage::OnInitDialog();
}

HBRUSH CTchVisionPatternPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }
    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CTchVisionPatternPage::OnSetActive()
{
	CTeachRecipeSheet* pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
		pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(0);
	return CPropertyPage::OnSetActive();
}

void CTchVisionPatternPage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	short nTotalRef = theRef.GetTotalRefNum();
	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
		nTotalRef = thePgm.GetUnitTotalDie() + 1;

	m_nTotalAlignStep = nTotalRef * 2;
	m_nAlignStep = 0;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow();

	if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
	{
		GetDlgItem(IDC_BUTTON_SKIP)->EnableWindow();

		theBond.m_nCurWire = 1;
		theBond.m_nRestartWire = 1;
		theBond.SetBondNoVision(false);

		if (pSheet->m_bPrAlignDone == false)
		{
			theBond.ResetMatrixPrStatus(1);

			bool bBkupIll = theBond.IsIll();
			theBond.SetIll(false);

			long lStatus = theBond.PerformVisionAlignment();
			if (lStatus == QUIT_AUTO_BOND)
				lStatus = FNOK;
			else
			{
				lStatus = FOK;
				pSheet->m_bPrAlignDone = true;
			}

			theBond.SetIll(bBkupIll);
			theVisionSys.EnableVideo(true);

			if (lStatus == QUIT_AUTO_BOND)
			{
				GetDlgItem(IDC_BUTTON_START)->EnableWindow();
				GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SKIP)->EnableWindow(FALSE);
				return;
			}
		}
	}

	REF_RECORD *pstRef = theRef.GetRcd(0);	//v3.1.T16 edit
	m_nLocateMode = pstRef->nLocateMode;
	m_nPatSizeX = pstRef->nVisionSrchSizeX[0];
	m_nPatSizeY = pstRef->nVisionSrchSizeY[0];
	theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);

	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
	{
		theServo.MoveTable2(pstRef->stManuAlignPos[0], MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
	    
		const MACHINE_DATA_LIGHT *pstLight = theMcData.GetLightRcd();
		theVisionSys.UpdateLightingParameter(
						pstLight->PrLeadRcd.nCoaxRedLight,
						pstLight->PrLeadRcd.bSideRedLight,
						pstLight->PrLeadRcd.nCoaxBlueLight,
						pstLight->PrLeadRcd.bSideBlueLight);

		m_nLensMag = pstRef->nManuAlignMag[0];
	}
	else
	{
		TBL_POS stPos = pstRef->stVisionAlignPos[0];
		theMatrix.TransformPos(0, &stPos);
		theServo.MoveTable2(stPos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
	    
		theVisionSys.UpdateLightingParameter(
				pstRef->stVisionLightPara[0].nCoaxRedLight,
				pstRef->stVisionLightPara[0].bSideRedLight,
				pstRef->stVisionLightPara[0].nCoaxBlueLight,
				pstRef->stVisionLightPara[0].bSideBlueLight);
		m_nLensMag = pstRef->nVisionAlignMag[0];
		//theVisionSys.ShowVisionPattern(stRefRcd.ulVisionAlignPatternID[0]);

		m_nVisionAlgo = pstRef->nVisionAlgorithm[0];
		if (m_nVisionAlgo == 1)
		{
			theVisionSys.UpdateBinaryThreshold(pstRef->stVisionLightPara[0].nImageThreshold);
			theVisionSys.EnableVideo(true);
		}
	}

	//theVisionSys.SetSearchSize(stRefRcd.nVisionSrchSizeX[0], stRefRcd.nVisionSrchSizeY[0]);
	theVisionSys.ChangeLensMag(m_nLensMag);

	GetDlgItem(IDC_BUTTON_MASK)->EnableWindow(TRUE);

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
	{
		pListBox->AddString(theMsg.GetMsg(MSG_TEACH_VISION_PATTERN)+theMsg.GetMsg(MSG_LEAD));
		pListBox->AddString(_T(""));
	}
	else
		pListBox->AddString(theMsg.GetMsg(MSG_EDIT_VISION_PATTERN)+theMsg.GetMsg(MSG_LEAD));
	pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_LEAD_1ST_PAT_PT));
	pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
	pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
	UpdateData(false);
}

BYTE CTchVisionPatternPage::CheckPatternQuality()
{
	if (theVisionSys.GetImageQuality(m_nPatSizeX, m_nPatSizeY) > 30.0)
		return TRUE;

	CBusy bs(&m_bBusy);	//V1.1.W153 add

	short anMsg[1] = {MSG_PR_QUALITY_REJECT};
	if (theMsg.ShowMsg(WARNING_MSG,1,anMsg,true) != SHOW_MSG_RESULT_STOP)
		return TRUE;

	GetDlgItem(IDC_BUTTON_SKIP)->EnableWindow();
	return FALSE;
}

void CTchVisionPatternPage::OnContinue()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(TRUE);

    CString str, str2;
	short nCurRef = m_nAlignStep / 2;
    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();

	const MATRIX_RECORD *pstMat = theMatrix.GetRcd(nCurRef);
	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        if (pstMat->nVisionStatus != VISION_SUCCESS)
        {
            pListBox->AddString(theMsg.GetMsg(MSG_PR_FAIL_2));
            GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
            return;
        }
    }

	if (CheckPatternQuality() == FALSE) return;

	short nRefPt = m_nAlignStep % 2;
	short nNextRef = (m_nAlignStep + 1) / 2;
	m_nLastRef = nCurRef;
	MACHINE_DATA_LIGHT *pstLight = theMcData.GetLightRcd();

    REF_RECORD stRef;	//v3.1.T17 rename
	theRef.GetRcd(&stRef, nCurRef);
	stRef.nVisionSrchSizeX[nRefPt] = m_nPatSizeX;
	stRef.nVisionSrchSizeY[nRefPt] = m_nPatSizeY;

	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
		stRef.nLocateMode = m_nLocateMode;

    if (nRefPt == 0)
	{
        theVisionSys.SetSearchSize(stRef.nVisionSrchSizeX[0],
									stRef.nVisionSrchSizeY[0]);
        theVisionSys.EnableVideo(false);
        theVisionSys.GrabImage();

		stRef.nVisionAlgorithm[0] = m_nVisionAlgo;	//1: Binary, 0: Grey Level
        if (m_nVisionAlgo == 1)
        {
			short nThreshold;
            theVisionSys.GetBinaryThreshold(&nThreshold);
            stRef.stVisionLightPara[0].nImageThreshold = nThreshold;
            theVisionSys.UpdateBinaryThreshold(nThreshold);
        }

		bool bRotateEnable = (stRef.nRefType != REF_TYPE_LEAD)
						&& (stRef.nLocateMode != REF_LOCATE_DUAL_POINT);
		short nPatRcd = -1;
        if (theVisionSys.LearnVisionPattern(bRotateEnable, m_nPatSizeX,
											m_nPatSizeY, &nPatRcd) == FOK)
        {
            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
                theVisionSys.ClearPatternRcd(
						(short)stRef.ulVisionAlignPatternID[0]);
            stRef.ulVisionAlignPatternID[0] = nPatRcd;
        }

        GetDlgItem(IDC_EDIT_SIZE_X)->EnableWindow();
        GetDlgItem(IDC_EDIT_SIZE_Y)->EnableWindow();

		if (!theMcConfig.IsBypassAll())
        {
            theServo.GetTablePosition(&stRef.stVisionAlignPos[0]);

            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
            {
                theMatrix.ReverseTransformPos(nCurRef, &stRef.stVisionAlignPos[0]);
                if (stRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
                {
                    stRef.dVisionSinAln = -pstMat->dSinAln;
                    stRef.dVisionCosAln = pstMat->dCosAln;
                }
            }
            else
            {
                if (stRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
                {
                    stRef.dVisionSinAln = 0.0;
                    stRef.dVisionCosAln = 1.0;
                }
            }
        }
        else
        {
            stRef.stVisionAlignPos[0].dx = 9000.0 / theServo.GetResolXY();
            stRef.stVisionAlignPos[0].dy =  6900.0 / theServo.GetResolXY();

            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
            {
                theMatrix.ReverseTransformPos(nCurRef, &stRef.stVisionAlignPos[0]);
                if (stRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
                {
                    stRef.dVisionSinAln = -pstMat->dSinAln;
                    stRef.dVisionCosAln = pstMat->dCosAln;
                }
            }
            else
            {
                if (stRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
                {
                    stRef.dVisionSinAln = 0.0;
                    stRef.dVisionCosAln = 1.0;
                }
            }
        }

        stRef.nVisionAlignMag[0] = m_nLensMag;
		short nSideRedLight, nSideBlueLight;
        theVisionSys.GetLightingParameter(
			&stRef.stVisionLightPara[0].nCoaxRedLight, &nSideRedLight,
			&stRef.stVisionLightPara[0].nCoaxBlueLight, &nSideBlueLight);
        stRef.stVisionLightPara[0].bSideRedLight = (byte)nSideRedLight;
        stRef.stVisionLightPara[0].bSideBlueLight = (byte)nSideBlueLight;
        theRef.SetRcd(&stRef, nCurRef);

		if (stRef.nRefType == REF_TYPE_LEAD)
		{
			pstLight->PrLeadRcd.nCoaxRedLight  = stRef.stVisionLightPara[0].nCoaxRedLight;
			pstLight->PrLeadRcd.bSideRedLight  = stRef.stVisionLightPara[0].bSideRedLight;
			pstLight->PrLeadRcd.nCoaxBlueLight = stRef.stVisionLightPara[0].nCoaxBlueLight;
			pstLight->PrLeadRcd.bSideBlueLight = stRef.stVisionLightPara[0].bSideBlueLight;
		}
		else
		{
			pstLight->PrDieRcd.nCoaxRedLight  = stRef.stVisionLightPara[0].nCoaxRedLight;
			pstLight->PrDieRcd.bSideRedLight  = stRef.stVisionLightPara[0].bSideRedLight;
			pstLight->PrDieRcd.nCoaxBlueLight = stRef.stVisionLightPara[0].nCoaxBlueLight;
			pstLight->PrDieRcd.bSideBlueLight = stRef.stVisionLightPara[0].bSideBlueLight;
			theMcData.GetRcd()->nLocateMode = m_nLocateMode;
		}

        theVisionSys.EnableVideo(true);

        if (stRef.nLocateMode == REF_LOCATE_DUAL_POINT)
		{
            if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
            {
				theServo.MoveTable2(stRef.stManuAlignPos[1], MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
				theVisionSys.SetSearchSize(stRef.nVisionSrchSizeX[0], stRef.nVisionSrchSizeY[0]);

				if (stRef.nRefType == REF_TYPE_LEAD)
					str2 = theMsg.GetMsg(MSG_LEAD);
				else
				{
					str2 = theMsg.GetMsg(MSG_DIE);
					if (m_nTotalAlignStep > 4)
						str2 += _bstr_t(nCurRef).GetBSTR();
				}
				str = theMsg.GetMsg(MSG_TEACH_VISION_PATTERN) + str2;
            }
            else
            {
				GetDlgItem(IDC_BUTTON_SKIP)->EnableWindow(0);

				m_nPatSizeX = stRef.nVisionSrchSizeX[1];
				m_nPatSizeY = stRef.nVisionSrchSizeY[1];
				theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);

				TBL_POS stPos = stRef.stVisionAlignPos[1];
				theMatrix.TransformPos(nCurRef, &stPos);
				theServo.MoveTable2(stPos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
				//Use lighting parameter of lead 1st point
				theVisionSys.UpdateLightingParameter(
							stRef.stVisionLightPara[1].nCoaxRedLight,
							stRef.stVisionLightPara[1].bSideRedLight,
							stRef.stVisionLightPara[1].nCoaxBlueLight,
							stRef.stVisionLightPara[1].bSideBlueLight);

				m_nVisionAlgo = stRef.nVisionAlgorithm[1];
				if (m_nVisionAlgo == 1)
				{
					theVisionSys.UpdateBinaryThreshold(stRef.stVisionLightPara[1].nImageThreshold);
					theVisionSys.EnableVideo(true);
				}

				if (stRef.nRefType == REF_TYPE_LEAD)
					str2 = theMsg.GetMsg(MSG_LEAD);
				else
				{
					str2 = theMsg.GetMsg(MSG_DIE);
					if (m_nTotalAlignStep > 4)
						str2 += _bstr_t(nCurRef).GetBSTR();
				}
				str = theMsg.GetMsg(MSG_EDIT_VISION_PATTERN) + str2;
            }

            pListBox->AddString(str);
            pListBox->AddString(_T(""));
            pListBox->AddString(theMsg.GetMsg(stRef.nRefType == REF_TYPE_LEAD ? MSG_MOVE_TO_LEAD_2ND_PAT_PT : MSG_MOVE_TO_DIE_2ND_PAT_PT));
            pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
            pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
		}
        else	//single point PR
		{
            m_nAlignStep++;
			nNextRef = (m_nAlignStep + 1) / 2;

            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
			{
                short i, nUnitTotalDie, nUnitDieNum, nUnitNum, nRow, nCol;
                TBL_POS stPosOffset;
				REF_RECORD *pstCurRef;	//v3.1.T18 edit

                if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
					|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
					|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
                {
                    nUnitTotalDie = thePgm.GetUnitTotalDie();
                    nUnitDieNum = nCurRef % nUnitTotalDie;

                    if (stRef.nRefType == REF_TYPE_DIE)
                    {
                        for (i = 1; i < theRef.GetTotalRefNum(); i++)
                        {
                            if ((i % nUnitTotalDie) == nUnitDieNum)
                            {
                                pstCurRef = theRef.GetRcd(i);
                                pstCurRef->stVisionLightPara[0]      = stRef.stVisionLightPara[0];
                                pstCurRef->stVisionLightPara[1]      = stRef.stVisionLightPara[1];
                                pstCurRef->nVisionSrchSizeX[0]       = stRef.nVisionSrchSizeX[0];
                                pstCurRef->nVisionSrchSizeY[0]       = stRef.nVisionSrchSizeY[0];
                                pstCurRef->nVisionSrchSizeX[1]       = stRef.nVisionSrchSizeX[1];
                                pstCurRef->nVisionSrchSizeY[1]       = stRef.nVisionSrchSizeY[1];
                                pstCurRef->ulVisionAlignPatternID[0] = stRef.ulVisionAlignPatternID[0];
                                pstCurRef->ulVisionAlignPatternID[1] = stRef.ulVisionAlignPatternID[1];
                                pstCurRef->nVisionAlgorithm[0]       = stRef.nVisionAlgorithm[0];
                                pstCurRef->nVisionAlgorithm[1]       = stRef.nVisionAlgorithm[1];
	                            if (nUnitTotalDie  == 0)
								{
									AfxMessageBox(_T("temp.20, nUnitTotalDie = 0"));
									return;
								}
                                nUnitNum = (i - 1) / nUnitTotalDie + 1;
                                thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
                                thePgm.CalcUnitOffset(nRow, nCol, &stPosOffset);
                                pstCurRef->stVisionAlignPos[0].dx = stRef.stVisionAlignPos[0].dx + stPosOffset.dx;
                                pstCurRef->stVisionAlignPos[0].dy = stRef.stVisionAlignPos[0].dy + stPosOffset.dy;
                                pstCurRef->stVisionAlignPos[1].dx = stRef.stVisionAlignPos[1].dx + stPosOffset.dx;
                                pstCurRef->stVisionAlignPos[1].dy = stRef.stVisionAlignPos[1].dy + stPosOffset.dy;
								if (stRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
								{
									pstCurRef->dVisionSinAln = stRef.dVisionSinAln;
									pstCurRef->dVisionCosAln = stRef.dVisionCosAln;
								}
                                theRef.CalcRefAngle(i, VISION_PR_ALIGN);
                            }
                        }
                    }
                }
                else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
                {
                    nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
                    nUnitDieNum = nCurRef % nUnitTotalDie;

                    for (i = 0; i < theRef.GetTotalRefNum(); i++)
                    {
                        if ((i % nUnitTotalDie) == nUnitDieNum)
                        {
                            pstCurRef = theRef.GetRcd(i);
                            pstCurRef->stVisionLightPara[0]      = stRef.stVisionLightPara[0];
                            pstCurRef->stVisionLightPara[1]      = stRef.stVisionLightPara[1];
                            pstCurRef->nVisionSrchSizeX[0]       = stRef.nVisionSrchSizeX[0];
                            pstCurRef->nVisionSrchSizeY[0]       = stRef.nVisionSrchSizeY[0];
                            pstCurRef->nVisionSrchSizeX[1]       = stRef.nVisionSrchSizeX[1];
                            pstCurRef->nVisionSrchSizeY[1]       = stRef.nVisionSrchSizeY[1];
                            pstCurRef->ulVisionAlignPatternID[0] = stRef.ulVisionAlignPatternID[0];
                            pstCurRef->ulVisionAlignPatternID[1] = stRef.ulVisionAlignPatternID[1];
                            pstCurRef->nVisionAlgorithm[0]       = stRef.nVisionAlgorithm[0];
                            pstCurRef->nVisionAlgorithm[1]       = stRef.nVisionAlgorithm[1];
	                        if (nUnitTotalDie  == 0)
							{
								AfxMessageBox(_T("temp.21, nUnitTotalDie = 0"));
								return;
							}
                            nUnitNum = i / nUnitTotalDie + 1;
                            thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
                            thePgm.CalcUnitOffset(nRow, nCol, &stPosOffset);
                            pstCurRef->stVisionAlignPos[0].dx = stRef.stVisionAlignPos[0].dx + stPosOffset.dx;
                            pstCurRef->stVisionAlignPos[0].dy = stRef.stVisionAlignPos[0].dy + stPosOffset.dy;
                            pstCurRef->stVisionAlignPos[1].dx = stRef.stVisionAlignPos[1].dx + stPosOffset.dx;
                            pstCurRef->stVisionAlignPos[1].dy = stRef.stVisionAlignPos[1].dy + stPosOffset.dy;
							if (stRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
							{
								pstCurRef->dVisionSinAln = stRef.dVisionSinAln;
								pstCurRef->dVisionCosAln = stRef.dVisionCosAln;
							}
                            theRef.CalcRefAngle(i, VISION_PR_ALIGN);
                        }
                    }
                }
            }

            if (m_nAlignStep < m_nTotalAlignStep - 1)
			{
                theRef.GetRcd(&stRef, nNextRef);
                m_nLocateMode = stRef.nLocateMode;

                if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
                {
					theServo.MoveTable2(stRef.stManuAlignPos[0], MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
					theVisionSys.UpdateLightingParameter(
									pstLight->PrDieRcd.nCoaxRedLight,
									pstLight->PrDieRcd.bSideRedLight,
									pstLight->PrDieRcd.nCoaxBlueLight,
									pstLight->PrDieRcd.bSideBlueLight);
					m_nLensMag = stRef.nManuAlignMag[0];
					theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);
                }
                else
                {
					m_nPatSizeX = stRef.nVisionSrchSizeX[0];
					m_nPatSizeY = stRef.nVisionSrchSizeY[0];
					theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);

					TBL_POS stPos = stRef.stVisionAlignPos[0];
					theMatrix.TransformPos(nNextRef, &stPos);
					theServo.MoveTable2(stPos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit

					theVisionSys.UpdateLightingParameter(
								stRef.stVisionLightPara[0].nCoaxRedLight,
								stRef.stVisionLightPara[0].bSideRedLight,
								stRef.stVisionLightPara[0].nCoaxBlueLight,
								stRef.stVisionLightPara[0].bSideBlueLight);
					m_nLensMag = stRef.nVisionAlignMag[0];
					m_nVisionAlgo = stRef.nVisionAlgorithm[0];
					if (m_nVisionAlgo == 1)
					{
						theVisionSys.UpdateBinaryThreshold(stRef.stVisionLightPara[0].nImageThreshold);
						theVisionSys.EnableVideo(true);
					}
                }
                
                str2 = theMsg.GetMsg(MSG_DIE);
                if (m_nTotalAlignStep > 4)
                    str2 += _bstr_t(nCurRef + 1).GetBSTR();
                str = theMsg.GetMsg(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ? MSG_TEACH_VISION_PATTERN : MSG_EDIT_VISION_PATTERN) + str2;
                pListBox->AddString(str);
                pListBox->AddString(_T(""));
                pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_DIE_1ST_PAT_PT));
                pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
                pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
            }
        }
    }
    else	//2nd ref point
	{
		theVisionSys.SetSearchSize(stRef.nVisionSrchSizeX[1],
									stRef.nVisionSrchSizeY[1]);
        theVisionSys.EnableVideo(false);
        theVisionSys.GrabImage();

		stRef.nVisionAlgorithm[1] = m_nVisionAlgo;	//1: Binary, 0: Grey Level
        if (m_nVisionAlgo == 1)
        {
			short nThreshold;
            theVisionSys.GetBinaryThreshold(&nThreshold);
            stRef.stVisionLightPara[1].nImageThreshold = nThreshold;
            theVisionSys.UpdateBinaryThreshold(nThreshold);
        }

		bool bRotateEnable = (stRef.nRefType != REF_TYPE_LEAD)
						&& (stRef.nRefType != REF_LOCATE_DUAL_POINT);
		short nPatRcd = -1;
        if (theVisionSys.LearnVisionPattern(bRotateEnable, m_nPatSizeX,
											m_nPatSizeY, &nPatRcd) == FOK)
        {
            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
                theVisionSys.ClearPatternRcd(
						(short)stRef.ulVisionAlignPatternID[1]);
            stRef.ulVisionAlignPatternID[1] = nPatRcd;
        }

        GetDlgItem(IDC_EDIT_SIZE_X)->EnableWindow(1);
        GetDlgItem(IDC_EDIT_SIZE_Y)->EnableWindow(1);

		if (!theMcConfig.IsBypassAll())
        {
            theServo.GetTablePosition(&stRef.stVisionAlignPos[1]);
            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
                theMatrix.ReverseTransformPos(nCurRef, &stRef.stVisionAlignPos[1]);
        }
        else
        {
            stRef.stVisionAlignPos[1].dx = -9000.0 / theServo.GetResolXY();
            stRef.stVisionAlignPos[1].dy = -6900.0 / theServo.GetResolXY();
        }

        stRef.nVisionAlignMag[1] = m_nLensMag;
		short nSideRedLight, nSideBlueLight;
        theVisionSys.GetLightingParameter(
				&stRef.stVisionLightPara[1].nCoaxRedLight, &nSideRedLight,
				&stRef.stVisionLightPara[1].nCoaxBlueLight, &nSideBlueLight);
        stRef.stVisionLightPara[1].bSideRedLight = (byte)nSideRedLight;
        stRef.stVisionLightPara[1].bSideBlueLight = (byte)nSideBlueLight;
        theRef.SetRcd(&stRef, nCurRef);
        theRef.CalcRefAngle(nCurRef, VISION_PR_ALIGN);

        if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
		{
            short i, nUnitTotalDie, nUnitDieNum, nUnitNum, nRow, nCol;
            TBL_POS stPosOffset;
			REF_RECORD *pstCurRef;	//v3.1.T18 edit

			GetDlgItem(IDC_BUTTON_SKIP)->EnableWindow(TRUE);

            if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
            {
                nUnitTotalDie = thePgm.GetUnitTotalDie();
                nUnitDieNum = nCurRef % nUnitTotalDie;

                if (stRef.nRefType == REF_TYPE_DIE)
                {
                    for (i = 1; i < theRef.GetTotalRefNum(); i++)
                    {
                        if ((i % nUnitTotalDie) == nUnitDieNum)
                        {
                            pstCurRef = theRef.GetRcd(i);
                            pstCurRef->stVisionLightPara[0]      = stRef.stVisionLightPara[0];
                            pstCurRef->stVisionLightPara[1]      = stRef.stVisionLightPara[1];
                            pstCurRef->nVisionSrchSizeX[0]       = stRef.nVisionSrchSizeX[0];
                            pstCurRef->nVisionSrchSizeY[0]       = stRef.nVisionSrchSizeY[0];
                            pstCurRef->nVisionSrchSizeX[1]       = stRef.nVisionSrchSizeX[1];
                            pstCurRef->nVisionSrchSizeY[1]       = stRef.nVisionSrchSizeY[1];
                            pstCurRef->ulVisionAlignPatternID[0] = stRef.ulVisionAlignPatternID[0];
                            pstCurRef->ulVisionAlignPatternID[1] = stRef.ulVisionAlignPatternID[1];
                            pstCurRef->nVisionAlgorithm[0]       = stRef.nVisionAlgorithm[0];
                            pstCurRef->nVisionAlgorithm[1]       = stRef.nVisionAlgorithm[1];
	                        if (nUnitTotalDie  == 0)
							{
								AfxMessageBox(_T("temp.22, nUnitTotalDie = 0"));
								return;
							}
                            nUnitNum = (i - 1) / nUnitTotalDie + 1;
                            thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
                            thePgm.CalcUnitOffset(nRow, nCol, &stPosOffset);
                            pstCurRef->stVisionAlignPos[0].dx = stRef.stVisionAlignPos[0].dx + stPosOffset.dx;
                            pstCurRef->stVisionAlignPos[0].dy = stRef.stVisionAlignPos[0].dy + stPosOffset.dy;
                            pstCurRef->stVisionAlignPos[1].dx = stRef.stVisionAlignPos[1].dx + stPosOffset.dx;
                            pstCurRef->stVisionAlignPos[1].dy = stRef.stVisionAlignPos[1].dy + stPosOffset.dy;
                            theRef.CalcRefAngle(i, VISION_PR_ALIGN);
                        }
                    }
                }
            }
            else if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
            {
                nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
                nUnitDieNum = nCurRef % nUnitTotalDie;

                for (i = 0; i < theRef.GetTotalRefNum(); i++)
                {
                    if ((i % nUnitTotalDie) == nUnitDieNum)
                    {
                        pstCurRef = theRef.GetRcd(i);
                        pstCurRef->stVisionLightPara[0]      = stRef.stVisionLightPara[0];
                        pstCurRef->stVisionLightPara[1]      = stRef.stVisionLightPara[1];
                        pstCurRef->nVisionSrchSizeX[0]       = stRef.nVisionSrchSizeX[0];
                        pstCurRef->nVisionSrchSizeY[0]       = stRef.nVisionSrchSizeY[0];
                        pstCurRef->nVisionSrchSizeX[1]       = stRef.nVisionSrchSizeX[1];
                        pstCurRef->nVisionSrchSizeY[1]       = stRef.nVisionSrchSizeY[1];
                        pstCurRef->ulVisionAlignPatternID[0] = stRef.ulVisionAlignPatternID[0];
                        pstCurRef->ulVisionAlignPatternID[1] = stRef.ulVisionAlignPatternID[1];
                        pstCurRef->nVisionAlgorithm[0]       = stRef.nVisionAlgorithm[0];
                        pstCurRef->nVisionAlgorithm[1]       = stRef.nVisionAlgorithm[1];
	                    if (nUnitTotalDie  == 0)
						{
							AfxMessageBox(_T("temp.23, nUnitTotalDie = 0"));
							return;
						}
                        nUnitNum = i / nUnitTotalDie + 1;
                        thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
                        thePgm.CalcUnitOffset(nRow, nCol, &stPosOffset);
                        pstCurRef->stVisionAlignPos[0].dx = stRef.stVisionAlignPos[0].dx + stPosOffset.dx;
                        pstCurRef->stVisionAlignPos[0].dy = stRef.stVisionAlignPos[0].dy + stPosOffset.dy;
                        pstCurRef->stVisionAlignPos[1].dx = stRef.stVisionAlignPos[1].dx + stPosOffset.dx;
                        pstCurRef->stVisionAlignPos[1].dy = stRef.stVisionAlignPos[1].dy + stPosOffset.dy;
                        theRef.CalcRefAngle(i, VISION_PR_ALIGN);
                    }
                }
            }
        }

        theVisionSys.EnableVideo(true);

        if (m_nAlignStep < m_nTotalAlignStep - 1)
        {
            theRef.GetRcd(&stRef, nNextRef);
            m_nLocateMode = stRef.nLocateMode;
            m_nPatSizeX = stRef.nVisionSrchSizeX[0];
            m_nPatSizeY = stRef.nVisionSrchSizeY[0];
            theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);
            
            if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
            {
				theServo.MoveTable2(stRef.stManuAlignPos[0], MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
				theVisionSys.UpdateLightingParameter(
							pstLight->PrDieRcd.nCoaxRedLight,
							pstLight->PrDieRcd.bSideRedLight,
							pstLight->PrDieRcd.nCoaxBlueLight,
							pstLight->PrDieRcd.bSideBlueLight);
				m_nLensMag = stRef.nManuAlignMag[0];
            }
            else
            {
				TBL_POS stPos = stRef.stVisionAlignPos[0];
				theMatrix.TransformPos(nNextRef, &stPos);
				theServo.MoveTable2(stPos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit

				theVisionSys.UpdateLightingParameter(
							stRef.stVisionLightPara[0].nCoaxRedLight,
							stRef.stVisionLightPara[0].bSideRedLight,
							stRef.stVisionLightPara[0].nCoaxBlueLight,
							stRef.stVisionLightPara[0].bSideBlueLight);
				m_nLensMag = stRef.nVisionAlignMag[0];
				m_nVisionAlgo = stRef.nVisionAlgorithm[0];
				if (m_nVisionAlgo == 1)
				{
					theVisionSys.UpdateBinaryThreshold(stRef.stVisionLightPara[0].nImageThreshold);
					theVisionSys.EnableVideo(true);
				}
            }

            theVisionSys.ChangeLensMag(m_nLensMag);

			str = theMsg.GetMsg(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ? MSG_TEACH_VISION_PATTERN : MSG_EDIT_VISION_PATTERN);
            str2 = theMsg.GetMsg(MSG_DIE);
            if (m_nTotalAlignStep > 4)
                str2 += _bstr_t(nCurRef + 1).GetBSTR();
            str += str2;
            pListBox->AddString(str);
            pListBox->AddString(_T(""));
            pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_DIE_1ST_PAT_PT));
            pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
            pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
        }
    }
    
    UpdateData(false);
	m_nAlignStep++;
	if (theMcConfig.IsSharePR())	//v3.1.T15 add, v3.1.T19 edit
	{
		if (m_nAlignStep < 4) return;
		//copy die 1 pattern to other dies
		const REF_RECORD *pstRef1 = theRef.GetRcd(1);
		REF_RECORD *pstCurRef;
		for (int i = 2; i < thePgm.GetTotalRef(); i++)
		{
			pstCurRef = theRef.GetRcd(i);
			pstCurRef->nRefType = pstRef1->nRefType;
			pstCurRef->nLocateMode = pstRef1->nLocateMode;
			pstCurRef->dVisionSinAln = pstRef1->dVisionSinAln;
			pstCurRef->dVisionCosAln = pstRef1->dVisionCosAln;
			for (int j = 0; j < 2; j++)
			{
				pstCurRef->nVisionAlgorithm[j] = pstRef1->nVisionAlgorithm[j];
				pstCurRef->ulVisionAlignPatternID[j] =
										pstRef1->ulVisionAlignPatternID[j];
				pstCurRef->stVisionAlignPos[j].dx = pstRef1->stVisionAlignPos[j].dx
					- pstRef1->stManuAlignPos[j].dx + pstCurRef->stManuAlignPos[j].dx;
				pstCurRef->stVisionAlignPos[j].dy = pstRef1->stVisionAlignPos[j].dy
					- pstRef1->stManuAlignPos[j].dy + pstCurRef->stManuAlignPos[j].dy;
				pstCurRef->nVisionAlignMag[j] = pstRef1->nVisionAlignMag[j];

				pstCurRef->nVisionSrchSizeX[j] = pstRef1->nVisionSrchSizeX[j];
				pstCurRef->nVisionSrchSizeY[j] = pstRef1->nVisionSrchSizeY[j];
				pstCurRef->stVisionLightPara[j] = pstRef1->stVisionLightPara[j];
			}
		}
	}
	else if (m_nAlignStep < m_nTotalAlignStep) return;

	//v3.1.T422 add
    CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COPY_FROM);
	pComboBox->ResetContent();
	pComboBox->AddString(_T("None"));
	for (short i = 1; i < m_nAlignStep / 2; i++)
	{
		str.Format(_T("Die %d"), i);
		pComboBox->AddString(str);
	}

	theVisionSys.ClearDrawing();
	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
	{
		pListBox->AddString(theMsg.GetMsg(MSG_TEACH_VISION_PATTERN_FINISH));
		pListBox->AddString(theMsg.GetMsg(MSG_PRESS_NEXT_TO_TEACH_WIRE_LAYOUT));
	}
	else
	{
		pListBox->AddString(theMsg.GetMsg(MSG_EDIT_VISION_PATTERN_FINISH));
		GetDlgItem(IDC_BUTTON_SKIP)->EnableWindow(FALSE);

		theApp.ArchivePowerOnData(ARCHIVE_SAVE);
		theBond.SetPrPatternPassScore();
		theVisionSys.SavePatternRcd();	//v3.0.T70 edit
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);

	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
		pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow();
}

void CTchVisionPatternPage::OnLensMag()
{
    UpdateData(true);
    theVisionSys.ChangeLensMag(m_nLensMag);
}

void CTchVisionPatternPage::OnEditSizeX()
{
    if (UpdateData(true) != FALSE)
        theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);
}

void CTchVisionPatternPage::OnEditSizeY()
{
    if (UpdateData(true) != FALSE)
        theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);
}

void CTchVisionPatternPage::OnCancel()
{
    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
        thePgm.ClearProgram();
        theVisionSys.ClearBondVisionPattern();
    }
    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateOverView();
    CPropertyPage::OnCancel();
}

void CTchVisionPatternPage::OnFocusSizeX()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PATTERN_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPatSizeX, 0, 0,
							theMcConfig.GetMaxPatSize());
    UpdateData(false);
    OnEditSizeX();
}

void CTchVisionPatternPage::OnFocusSizeY()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	GetDlgItem(IDC_STATIC_PATTERN_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nPatSizeY, 0, 0,
							theMcConfig.GetMaxPatSize());
    UpdateData(false);
    OnEditSizeY();
}

void CTchVisionPatternPage::OnLocateMode()
{
    UpdateData(TRUE);
	UpdateData(FALSE);
}

void CTchVisionPatternPage::OnSkip()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

	UpdateData(TRUE);
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    short nCurRefNum  = m_nAlignStep / 2;
    short nNextRefNum = (m_nAlignStep + 1) / 2;

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        if (theMatrix.GetRcd(nCurRefNum)->nVisionStatus != VISION_SUCCESS) return;
    }

	REF_RECORD stRefRcd;
	theRef.GetRcd(&stRefRcd, nCurRefNum);
	if (stRefRcd.nLocateMode == REF_LOCATE_SINGLE_POINT)
	{
		m_nAlignStep++;
		nNextRefNum = (m_nAlignStep + 1) / 2;
	}

    if (m_nAlignStep < m_nTotalAlignStep - 1)
    {
        theRef.GetRcd(&stRefRcd, nNextRefNum);

		if ((stRefRcd.nLocateMode == REF_LOCATE_SINGLE_POINT)
			|| (stRefRcd.nLocateMode == REF_LOCATE_DUAL_POINT)
			&& (m_nAlignStep % 2))
		{
			m_nLocateMode = stRefRcd.nLocateMode;
			m_nPatSizeX = stRefRcd.nVisionSrchSizeX[0];
			m_nPatSizeY = stRefRcd.nVisionSrchSizeY[0];
			theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);

		    TBL_POS stPos = stRefRcd.stVisionAlignPos[0];
			theMatrix.TransformPos(nNextRefNum, &stPos);
			theServo.MoveTable2(stPos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit

			theVisionSys.UpdateLightingParameter(
						stRefRcd.stVisionLightPara[0].nCoaxRedLight,
						stRefRcd.stVisionLightPara[0].bSideRedLight,
						stRefRcd.stVisionLightPara[0].nCoaxBlueLight,
						stRefRcd.stVisionLightPara[0].bSideBlueLight);
			m_nLensMag = stRefRcd.nVisionAlignMag[0];
			m_nVisionAlgo = stRefRcd.nVisionAlgorithm[0];
			if (m_nVisionAlgo == 1)
			{
				theVisionSys.UpdateBinaryThreshold(stRefRcd.stVisionLightPara[0].nImageThreshold);
				theVisionSys.EnableVideo(true);
			}
		}
		else
		{
			m_nLocateMode = stRefRcd.nLocateMode;
			m_nPatSizeX = stRefRcd.nVisionSrchSizeX[1];
			m_nPatSizeY = stRefRcd.nVisionSrchSizeY[1];
			theVisionSys.SetSearchSize(m_nPatSizeX, m_nPatSizeY);

		    TBL_POS stPos = stRefRcd.stVisionAlignPos[1];
			theMatrix.TransformPos(nNextRefNum, &stPos);
			theServo.MoveTable2(stPos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit

			theVisionSys.UpdateLightingParameter(
							stRefRcd.stVisionLightPara[1].nCoaxRedLight,
							stRefRcd.stVisionLightPara[1].bSideRedLight,
							stRefRcd.stVisionLightPara[1].nCoaxBlueLight,
							stRefRcd.stVisionLightPara[1].bSideBlueLight);
			m_nLensMag = stRefRcd.nVisionAlignMag[1];
			m_nVisionAlgo = stRefRcd.nVisionAlgorithm[1];
			if (m_nVisionAlgo == 1)
			{
				theVisionSys.UpdateBinaryThreshold(stRefRcd.stVisionLightPara[1].nImageThreshold);
				theVisionSys.EnableVideo(true);
			}
		}

	    CString str, str2;
        str2 = theMsg.GetMsg(MSG_DIE);
        if (m_nTotalAlignStep > 4)
            str2 += _bstr_t(nCurRefNum + 1).GetBSTR();
        str = theMsg.GetMsg(MSG_EDIT_VISION_PATTERN) + str2;
        pListBox->AddString(str);
        pListBox->AddString(_T(""));
        pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_DIE_1ST_PAT_PT));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
        pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
    }
 
	theVisionSys.EnableVideo(true);
	UpdateData(false);
	if (++m_nAlignStep != m_nTotalAlignStep) return;

	theVisionSys.ClearDrawing();
	pListBox->AddString(theMsg.GetMsg(MSG_EDIT_VISION_PATTERN_FINISH));

	theApp.ArchivePowerOnData(ARCHIVE_SAVE);
	theBond.SetPrPatternPassScore();
	theVisionSys.SavePatternRcd();	//v3.0.T70 edit

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SKIP)->EnableWindow(FALSE);
}

void CTchVisionPatternPage::OnMask()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
	if (!theMcConfig.IsBypassAll())
        theVisionSys.PrepareEditPatternMask(m_nPatSizeX, m_nPatSizeY);
    GetDlgItem(IDC_EDIT_SIZE_X)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_SIZE_Y)->EnableWindow(FALSE);
}

void CTchVisionPatternPage::OnAlgorithm()
{
	UpdateData(TRUE);

	switch (m_nVisionAlgo)
	{
	case PR_GRAY_LEVEL:
		break;
	case PR_BINARY:
		break;
	case PR_SHAPE_REG:
		break;
	default:
		break;
	}
}

LRESULT CTchVisionPatternPage::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	SetDlgItemInt(IDC_EDIT_SIZE_X, LOWORD(wParam));
	SetDlgItemInt(IDC_EDIT_SIZE_Y, HIWORD(wParam));
	return UpdateData(TRUE);
}

LRESULT CTchVisionPatternPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.1.W153 add
{
	return m_bBusy;
}
