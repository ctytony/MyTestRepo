/***************************************************************
TchAlignPointPage.cpp : implementation file
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
#include "TchAlignPointPage.h"
#include "TeachRecipeSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "VisionSystem.h"
#include "Bond.h"
#include "PrAlign.h"
#include "MaterialHandlerThread.h"
#include "MachineConfig.h"
#include "servo.h"
#include <memory>
#include <comutil.h>
#include "Busy.h"

// CTchAlignPointPage dialog
IMPLEMENT_DYNAMIC(CTchAlignPointPage, CPropertyPage)

CTchAlignPointPage::CTchAlignPointPage()
	: CPropertyPage(CTchAlignPointPage::IDD)
    , m_nUnitTotalDie(1)
    , m_nTotalAlignStep(0)
    , m_nAlignStep(0)
{
	m_bBusy = FALSE;	//V1.1.W153 add
}

CTchAlignPointPage::~CTchAlignPointPage()
{
}

void CTchAlignPointPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_DIE_NUM, m_nUnitTotalDie);
}

BEGIN_MESSAGE_MAP(CTchAlignPointPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
    ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnContinue)
    ON_EN_SETFOCUS(IDC_EDIT_DIE_NUM, OnEditDieNum)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W153 add
END_MESSAGE_MAP()

// CTchAlignPointPage message handlers
BOOL CTchAlignPointPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	pSheet->m_bPrAlignDone = false;

    SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_CONTINUE, theMsg.GetMsg(MSG_CONTINUE));
    SetDlgItemTextW(IDC_STATIC_REF_NUM, theMsg.GetMsg(MSG_REF_NUM));
    SetDlgItemTextW(IDC_STATIC_DIE_NUM, theMsg.GetMsg(MSG_DIE_NUM));
    SetDlgItemTextW(IDC_STATIC_HINT_DIE_NUM, theMsg.GetMsg(MSG_HINT_MULTI_DIE));

    if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        SetDlgItemTextW(IDC_STATIC_CAPTION, theMsg.GetMsg(MSG_EDIT_ALIGN_PT));
        m_nUnitTotalDie = thePgm.GetUnitTotalDie();
        GetDlgItem(IDC_EDIT_DIE_NUM)->EnableWindow(FALSE);
    }
    else
    {
        SetDlgItemTextW(IDC_STATIC_CAPTION, theMsg.GetMsg(MSG_TEACH_ALIGN_PT));
        GetDlgItem(IDC_EDIT_DIE_NUM)->EnableWindow();
    }

    ((CListBox*)GetDlgItem(IDC_LIST_INFO))->AddString(theMsg.GetMsg(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ? MSG_START_TO_TEACH_ALIGN_PT : MSG_START_TO_EDIT_ALIGN_PT));
	theVisionDisplay.EnableDisplay(false);
	m_nTotalAlignStep = 0;
    return CPropertyPage::OnInitDialog();
}

BOOL CTchAlignPointPage::OnSetActive()
{
    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
	if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
        pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(FALSE);
    return CPropertyPage::OnSetActive();
}

HBRUSH CTchAlignPointPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTchAlignPointPage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
		CStringList sList;
        sList.AddTail(theMsg.GetMsg(MSG_SURE_TO_DELETE_RECIPE));
        sList.AddTail(theMsg.GetMsg(MSG_TCH_NEW_RECIPE));
        if (theMsg.ShowMsg(HELP_MSG, sList, true) == SHOW_MSG_RESULT_STOP) return;
    }

    UpdateData(TRUE);
    GetDlgItem(IDC_EDIT_DIE_NUM)->EnableWindow(FALSE);

    m_nTotalAlignStep = m_nUnitTotalDie * 2 + 2;
    m_nAlignStep = 0;

    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
        thePgm.ClearProgram();
        theBond.ResetBondData();
        theVisionSys.ClearBondVisionPattern();
        ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateOverView();
        thePgm.CreateProgram();
    }
    else
    {
		theBond.m_nCurWire = 1;
		theBond.m_nRestartWire = 1;
		theBond.SetBondNoVision(false);

        if (pSheet->m_bPrAlignDone == false)
        {
            theBond.ResetMatrixPrStatus(1);

			bool bBkupIll = theBond.IsIll();
			theBond.SetIll(false);

            int nStatus = theBond.PerformVisionAlignment();
            if (nStatus == QUIT_AUTO_BOND)
                nStatus = FNOK;
            else
            {
                nStatus = FOK;
                pSheet->m_bPrAlignDone = true;
            }

			theBond.SetIll(bBkupIll);
            theVisionSys.EnableVideo(true);
            if (nStatus == FNOK) return;
            thePrAlign.UpdateManualAlignPoint();
        }
    }

	CString str = theMsg.GetMsg(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ?
						MSG_TEACH_ALIGN_POINT_COLON : MSG_EDIT_ALIGN_PT_COLON);
    str += theMsg.GetMsg(MSG_LEAD);
    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(str);
    pListBox->AddString(_T(""));
    pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_LEAD_1ST_ALIGN_PT));
    pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
    pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow();

    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
        theRef.AddRcd(theMcData.GetLocateMode());

	const REF_RECORD *pstRef = theRef.GetRcd(0);
    theVisionSys.SetSearchSize(pstRef->nManuSrchSizeX[0], pstRef->nManuSrchSizeY[0]);

    if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
		TBL_POS stMovePos = pstRef->stManuAlignPos[0];
		theMatrix.TransformPos(0, &stMovePos);
		theServo.MoveTable2(stMovePos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
		theVisionSys.ShowVisionPattern((short)pstRef->ulManuAlignPatternID[0]);
		theVisionSys.UpdateLightingParameter(
						pstRef->stManuAlignLightPara[0].nCoaxRedLight,
						pstRef->stManuAlignLightPara[0].bSideRedLight,
						pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
						pstRef->stManuAlignLightPara[0].bSideBlueLight);
    }
    else
    {	// lead 1st point
		MACHINE_DATA_LIGHT *pstLight = theMcData.GetLightRcd();
        theVisionSys.UpdateLightingParameter(
				pstLight->LeadLightRcd.nCoaxRedLight,
				pstLight->LeadLightRcd.bSideRedLight,
				pstLight->LeadLightRcd.nCoaxBlueLight,
				pstLight->LeadLightRcd.bSideBlueLight);
    }

    UpdateData(false);
}

void CTchAlignPointPage::OnContinue()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(TRUE);

	short nCurRef = m_nAlignStep / 2;
	short nNextRef = (m_nAlignStep + 1) / 2;
	short nRefPt = m_nAlignStep % 2;

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        if (theMatrix.GetRcd(nCurRef)->nVisionStatus != VISION_SUCCESS)
        {
            pListBox->AddString(theMsg.GetMsg(MSG_PR_FAIL_2));
            GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
            UpdateData(FALSE);
            return;
        }
    }
    
    short nSideRedLight, nSideBlueLight;
    short nPatID = -1;
    CString str;
	REF_RECORD stRef;
	theRef.GetRcd(&stRef, nCurRef);
    if (nRefPt == 0)
	{
		stRef.nRefType = nCurRef == 0 ? REF_TYPE_LEAD : REF_TYPE_DIE;

        if (theMcConfig.IsSharePR() && (nCurRef >= 2))	//v3.1.T22 add
		{
			const REF_RECORD *pstRef1 = theRef.GetRcd(1);
			stRef.nManuSrchSizeX[0] = pstRef1->nManuSrchSizeX[0];
			stRef.nManuSrchSizeY[0] = pstRef1->nManuSrchSizeY[0];
			stRef.ulManuAlignPatternID[0] = pstRef1->ulManuAlignPatternID[0];
			stRef.stManuAlignLightPara[0].nCoaxRedLight =
								pstRef1->stManuAlignLightPara[0].nCoaxRedLight;
			stRef.stManuAlignLightPara[0].nCoaxBlueLight =
								pstRef1->stManuAlignLightPara[0].nCoaxBlueLight;
			stRef.stManuAlignLightPara[0].bSideRedLight =
								pstRef1->stManuAlignLightPara[0].bSideRedLight;
			stRef.stManuAlignLightPara[0].bSideBlueLight =
								pstRef1->stManuAlignLightPara[0].bSideBlueLight;
		}
		else
		{
			theVisionSys.SetSearchSize(stRef.nManuSrchSizeX[0], stRef.nManuSrchSizeY[0]);
			theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();
			theVisionSys.LearnVisionPattern(false, stRef.nManuSrchSizeX[0],
											stRef.nManuSrchSizeY[0], &nPatID);
			if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
				theVisionSys.ClearPatternRcd((short)stRef.ulManuAlignPatternID[0]);
			stRef.ulManuAlignPatternID[0] = nPatID;

			theVisionSys.GetLightingParameter(
				&stRef.stManuAlignLightPara[0].nCoaxRedLight, &nSideRedLight,
				&stRef.stManuAlignLightPara[0].nCoaxBlueLight, &nSideBlueLight);
			stRef.stManuAlignLightPara[0].bSideRedLight = (byte)nSideRedLight;
			stRef.stManuAlignLightPara[0].bSideBlueLight = (byte)nSideBlueLight;
		}

		MACHINE_DATA_LIGHT *pstLight = theMcData.GetLightRcd();
        if (stRef.nRefType == REF_TYPE_LEAD)
        {
			pstLight->LeadLightRcd.nCoaxRedLight  = stRef.stManuAlignLightPara[0].nCoaxRedLight;
			pstLight->LeadLightRcd.bSideRedLight  = stRef.stManuAlignLightPara[0].bSideRedLight;
			pstLight->LeadLightRcd.nCoaxBlueLight = stRef.stManuAlignLightPara[0].nCoaxBlueLight;
			pstLight->LeadLightRcd.bSideBlueLight = stRef.stManuAlignLightPara[0].bSideBlueLight;
        }
        else
        {
			pstLight->DieLightRcd.nCoaxRedLight  = stRef.stManuAlignLightPara[0].nCoaxRedLight;
			pstLight->DieLightRcd.bSideRedLight  = stRef.stManuAlignLightPara[0].bSideRedLight;
			pstLight->DieLightRcd.nCoaxBlueLight = stRef.stManuAlignLightPara[0].nCoaxBlueLight;
			pstLight->DieLightRcd.bSideBlueLight = stRef.stManuAlignLightPara[0].bSideBlueLight;
        }
        if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
            stRef.stManuAlignLightPara[1] = stRef.stManuAlignLightPara[0];

		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
        {
            theServo.GetTablePosition(&stRef.stManuAlignPos[0]);
            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
                theMatrix.ReverseTransformPos(nCurRef, &stRef.stManuAlignPos[0]);
        }
        else
        {
            stRef.stManuAlignPos[0].dx = -2000.0 / theServo.GetResolXY();
            stRef.stManuAlignPos[0].dy = 8000.0 / theServo.GetResolXY();
        }
		theRef.SetRcd(&stRef, nCurRef);

        theVisionSys.EnableVideo(true);
        theVisionSys.SetSearchSize(stRef.nManuSrchSizeX[1], stRef.nManuSrchSizeY[1]);

        if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
        {
			TBL_POS stMovePos = stRef.stManuAlignPos[1];
			theMatrix.TransformPos(nCurRef, &stMovePos);
			theServo.MoveTable2(stMovePos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
			theVisionSys.ShowVisionPattern((short)stRef.ulManuAlignPatternID[1]);
        }
        
        theVisionSys.UpdateLightingParameter(
					stRef.stManuAlignLightPara[1].nCoaxRedLight,
					stRef.stManuAlignLightPara[1].bSideRedLight,
					stRef.stManuAlignLightPara[1].nCoaxBlueLight,
					stRef.stManuAlignLightPara[1].bSideBlueLight);

		str = theMsg.GetMsg(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ?
					MSG_TEACH_ALIGN_POINT_COLON : MSG_EDIT_ALIGN_PT_COLON);
		if (stRef.nRefType == REF_TYPE_LEAD)
            str += theMsg.GetMsg(MSG_LEAD);
        else
        {
            str += theMsg.GetMsg(MSG_DIE);
            if (m_nTotalAlignStep > 4)
				str += _bstr_t(nCurRef).GetBSTR();
        }
        pListBox->AddString(str);
        pListBox->AddString(_T(""));
        pListBox->AddString(theMsg.GetMsg(stRef.nRefType == REF_TYPE_LEAD ? MSG_MOVE_TO_LEAD_2ND_ALIGN_PT : MSG_MOVE_TO_DIE_2ND_ALIGN_PT));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
        pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
    }
    else
	{
		if (theMcConfig.IsSharePR() && (nCurRef >= 2))	//v3.1.T22 add
		{
			const REF_RECORD *pstRef1 = theRef.GetRcd(1);
			stRef.nManuSrchSizeX[1] = pstRef1->nManuSrchSizeX[1];
			stRef.nManuSrchSizeY[1] = pstRef1->nManuSrchSizeY[1];
			stRef.ulManuAlignPatternID[1] = pstRef1->ulManuAlignPatternID[1];
			stRef.stManuAlignLightPara[1].nCoaxRedLight =
								pstRef1->stManuAlignLightPara[1].nCoaxRedLight;
			stRef.stManuAlignLightPara[1].nCoaxBlueLight =
								pstRef1->stManuAlignLightPara[1].nCoaxBlueLight;
			stRef.stManuAlignLightPara[1].bSideRedLight =
								pstRef1->stManuAlignLightPara[1].bSideRedLight;
			stRef.stManuAlignLightPara[1].bSideBlueLight =
								pstRef1->stManuAlignLightPara[1].bSideBlueLight;
		}
		else
		{
	        theVisionSys.SetSearchSize(stRef.nManuSrchSizeX[1], stRef.nManuSrchSizeY[1]);
		    theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();
	        if (theVisionSys.LearnVisionPattern(false, stRef.nManuSrchSizeX[1],
										stRef.nManuSrchSizeY[1], &nPatID) == FOK)
			{
				if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
					theVisionSys.ClearPatternRcd((short)stRef.ulManuAlignPatternID[1]);
				stRef.ulManuAlignPatternID[1] = nPatID;
			}

			theVisionSys.GetLightingParameter(
				&stRef.stManuAlignLightPara[1].nCoaxRedLight, &nSideRedLight,
				&stRef.stManuAlignLightPara[1].nCoaxBlueLight, &nSideBlueLight);
			stRef.stManuAlignLightPara[1].bSideRedLight  = (byte)nSideRedLight;
			stRef.stManuAlignLightPara[1].bSideBlueLight = (byte)nSideBlueLight;
		}

		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
        {
            theServo.GetTablePosition(&stRef.stManuAlignPos[1]);
            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
                theMatrix.ReverseTransformPos(nCurRef, &stRef.stManuAlignPos[1]);
        }
        else
        {
            stRef.stManuAlignPos[1].dx = 10000.0 / theServo.GetResolXY();
            stRef.stManuAlignPos[1].dy =  -8000.0 / theServo.GetResolXY();
        }
		theRef.SetRcd(&stRef, nCurRef);
        theRef.CalcRefAngle(nCurRef, MANUAL_PR_ALIGN);
        theMatrix.CoordinateMatrixInfo(nCurRef);

        if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
        {
			TBL_POS stPosOffset;
			short i, nUnitTotalDie, nUnitDieNum, nUnitNum, nRow, nCol;
			REF_RECORD *pstCurRef;

			switch (thePgm.GetDeviceSeq())
			{
			case PGM_HYBRID: case PGM_DIE_AHEAD:
			case PGM_MAT_HYBRID: case PGM_MAT_DIE_AHEAD:
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
							pstCurRef->stManuAlignLightPara[0] = stRef.stManuAlignLightPara[0];
							pstCurRef->stManuAlignLightPara[1] = stRef.stManuAlignLightPara[1];
							pstCurRef->nManuSrchSizeX[0]       = stRef.nManuSrchSizeX[0];
							pstCurRef->nManuSrchSizeY[0]       = stRef.nManuSrchSizeY[0];
							pstCurRef->nManuSrchSizeX[1]       = stRef.nManuSrchSizeX[1];
							pstCurRef->nManuSrchSizeY[1]       = stRef.nManuSrchSizeY[1];
							pstCurRef->ulManuAlignPatternID[0] = stRef.ulManuAlignPatternID[0];
							pstCurRef->ulManuAlignPatternID[1] = stRef.ulManuAlignPatternID[1];
							nUnitNum = (i - 1) / nUnitTotalDie + 1;
							thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
							thePgm.CalcUnitOffset(nRow, nCol, &stPosOffset);
							pstCurRef->stManuAlignPos[0].dx = stRef.stManuAlignPos[0].dx + stPosOffset.dx;
							pstCurRef->stManuAlignPos[0].dy = stRef.stManuAlignPos[0].dy + stPosOffset.dy;
							pstCurRef->stManuAlignPos[1].dx = stRef.stManuAlignPos[1].dx + stPosOffset.dx;
							pstCurRef->stManuAlignPos[1].dy = stRef.stManuAlignPos[1].dy + stPosOffset.dy;
							theRef.CalcRefAngle(i, MANUAL_PR_ALIGN);
						}
					}
				}
				break;
			}
			case PGM_DIE_LEAD_AHEAD:
			{
				nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
				nUnitDieNum = nCurRef % nUnitTotalDie;

				for (i = 0; i < theRef.GetTotalRefNum(); i++)
				{
					if ((i % nUnitTotalDie) == nUnitDieNum)
					{
						pstCurRef = theRef.GetRcd(i);
						pstCurRef->stManuAlignLightPara[0] = stRef.stManuAlignLightPara[0];
						pstCurRef->stManuAlignLightPara[1] = stRef.stManuAlignLightPara[1];
						pstCurRef->nManuSrchSizeX[0]       = stRef.nManuSrchSizeX[0];
						pstCurRef->nManuSrchSizeY[0]       = stRef.nManuSrchSizeY[0];
						pstCurRef->nManuSrchSizeX[1]       = stRef.nManuSrchSizeX[1];
						pstCurRef->nManuSrchSizeY[1]       = stRef.nManuSrchSizeY[1];
						pstCurRef->ulManuAlignPatternID[0] = stRef.ulManuAlignPatternID[0];
						pstCurRef->ulManuAlignPatternID[1] = stRef.ulManuAlignPatternID[1];
						nUnitNum = i / nUnitTotalDie + 1;
						thePgm.GetRowCol(nUnitNum, &nRow, &nCol);
						thePgm.CalcUnitOffset(nRow, nCol, &stPosOffset);
						pstCurRef->stManuAlignPos[0].dx = stRef.stManuAlignPos[0].dx + stPosOffset.dx;
						pstCurRef->stManuAlignPos[0].dy = stRef.stManuAlignPos[0].dy + stPosOffset.dy;
						pstCurRef->stManuAlignPos[1].dx = stRef.stManuAlignPos[1].dx + stPosOffset.dx;
						pstCurRef->stManuAlignPos[1].dy = stRef.stManuAlignPos[1].dy + stPosOffset.dy;
						theRef.CalcRefAngle(i, MANUAL_PR_ALIGN);
					}
				}
				break;
			}
			}
        }

        theVisionSys.EnableVideo(true);
        ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateOverView();

        if (m_nAlignStep < m_nTotalAlignStep - 1)
        {
            if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
                theRef.AddRcd(theMcData.GetLocateMode());
            theRef.GetRcd(&stRef, nNextRef);
            theVisionSys.SetSearchSize(stRef.nManuSrchSizeX[0], stRef.nManuSrchSizeY[0]);
            
            if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
            {
				TBL_POS stMovePos = stRef.stManuAlignPos[0];
				theMatrix.TransformPos(nNextRef, &stMovePos);
				theServo.MoveTable2(stMovePos, MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
				theVisionSys.ShowVisionPattern((short)stRef.ulManuAlignPatternID[0]);
				theVisionSys.UpdateLightingParameter(
						stRef.stManuAlignLightPara[0].nCoaxRedLight,
						stRef.stManuAlignLightPara[0].bSideRedLight,
						stRef.stManuAlignLightPara[0].nCoaxBlueLight,
						stRef.stManuAlignLightPara[0].bSideBlueLight);
            }
            else //load die lighting data
            {
				MACHINE_DATA_LIGHT *pstLight = theMcData.GetLightRcd();
				theVisionSys.UpdateLightingParameter(
						pstLight->DieLightRcd.nCoaxRedLight,
						pstLight->DieLightRcd.bSideRedLight,
						pstLight->DieLightRcd.nCoaxBlueLight,
						pstLight->DieLightRcd.bSideBlueLight); 

				if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
					|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
					|| (theMcConfig.GetMhsType() == MHS_R2R)
					|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
				{
					if (nCurRef == 0)
					{
						theRef.GetRcd(&stRef, 0);
						theServo.MoveTable2(stRef.stManuAlignPos[0], MTN_ABS, MTN_NO_WAIT);	//v3.1.T43 edit
					}
				}
            }

			str = theMsg.GetMsg(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ?
					MSG_TEACH_ALIGN_POINT_COLON : MSG_EDIT_ALIGN_PT_COLON);
            str += theMsg.GetMsg(MSG_DIE);
            if (m_nTotalAlignStep > 4)
				str += _bstr_t(nCurRef + 1).GetBSTR();
            pListBox->AddString(str);
            pListBox->AddString(_T(""));
            pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_DIE_1ST_ALIGN_PT));
            pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
            pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
        }
    }
    UpdateData(false);

    m_nAlignStep++;
    if (m_nAlignStep == m_nTotalAlignStep)
    {
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);

        if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
        {
            pListBox->AddString(theMsg.GetMsg(MSG_TEACH_ALIGN_PT_FINISH));
            pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_TEACH_PATTERN));
        }
        else
        {
            pListBox->AddString(theMsg.GetMsg(MSG_EDIT_ALIGN_PT_FINISH));
            pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_EDIT_VISION_PATTERN));
        }

		if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
			pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow();
    }
}

void CTchAlignPointPage::OnCancel()
{
    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if ((m_nTotalAlignStep > 0) && (pSheet->m_lRecipeOperateMode == RECIPE_TEACH))
    {
        thePgm.ClearProgram();
        theVisionSys.ClearBondVisionPattern();
    }
    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateOverView();
    CPropertyPage::OnCancel();
}

void CTchAlignPointPage::OnEditDieNum()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    GetDlgItem(IDC_STATIC_CAPTION)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nUnitTotalDie);
    UpdateData(false);
}

LRESULT CTchAlignPointPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.1.W153 add
{
	return m_bBusy;
}
