/***************************************************************
TchSequencePage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "Wire.h"
#include "MachineConfig.h"
#include "Bond.h"
#include "TchSequencePage.h"
#include "TeachRecipeSheet.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "VisionSystem.h"
#include "MaterialHandlerThread.h"
#include "ScanKeyThread.h"
#include "MachineData.h"
#include "servo.h"
#include "Busy.h"
#include "FoxconnIndex.h"

//v1.5.T91 add
#define BONDSEQ_AREA_LEFT		15
#define BONDSEQ_AREA_TOP		265
#define BONDSEQ_AREA_WIDTH		420
#define BONDSEQ_AREA_HEIGHT		190

// CTchSequencePage dialog

IMPLEMENT_DYNAMIC(CTchSequencePage, CPropertyPage)

CTchSequencePage::CTchSequencePage(UINT nID)	//V1.5.W100 edit
	: CPropertyPage(nID)
    , m_nDevSeq(PGM_SINGLE)
    , m_nUnitRow(1)
    , m_nUnitCol(1)
    , m_nBondSeqOrientation(SEQUENCE_HORIZONTAL)
    , m_nTeachStep(0)
    , m_nTrackType(INNER_TRACK)
    , m_nAheadCol(1)
    , m_nMatRow(1)
    , m_nMatCol(1)
{
    m_stTrackOffset.dx = 0.0;
    m_stTrackOffset.dy = 0.0;

	m_bBusy = FALSE;	//V1.1.W153 add
}

CTchSequencePage::~CTchSequencePage()
{
    FreeButtonArray();
}

void CTchSequencePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nDevSeq);
    DDX_Text(pDX, IDC_EDIT_ROW, m_nUnitRow);
    DDX_Text(pDX, IDC_EDIT_COLUMN, m_nUnitCol);
    DDX_CBIndex(pDX, IDC_COMBO_BOND_SEQ, m_nBondSeqOrientation);
    DDX_Text(pDX, IDC_EDIT_AHEAD_COL, m_nAheadCol);
    DDX_Text(pDX, IDC_EDIT_MAT_ROW, m_nMatRow);
    DDX_Text(pDX, IDC_EDIT_MAT_COL, m_nMatCol);
}

BEGIN_MESSAGE_MAP(CTchSequencePage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
    ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnContinue)
    ON_CBN_SELCHANGE(IDC_COMBO_BOND_SEQ, OnBondSeq)
    ON_EN_SETFOCUS(IDC_EDIT_ROW, OnFocusRow)
    ON_EN_SETFOCUS(IDC_EDIT_COLUMN, OnFocusColumn)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnProgramType)
    ON_EN_SETFOCUS(IDC_EDIT_AHEAD_COL, OnFocusAheadCol)
    ON_BN_CLICKED(IDC_BUTTON_BACK, OnBack)
    ON_EN_SETFOCUS(IDC_EDIT_MAT_ROW, OnFocusMatRow)
    ON_EN_SETFOCUS(IDC_EDIT_MAT_COL, OnFocusMatCol)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W153 add
END_MESSAGE_MAP()

// CTchSequencePage message handlers
BOOL CTchSequencePage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_BUTTON_START,theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_CONTINUE,theMsg.GetMsg(MSG_CONTINUE));
    SetDlgItemTextW(IDC_BUTTON_BACK,theMsg.GetMsg(MSG_BACK));
    SetDlgItemTextW(IDC_STATIC_SEQUENCE_SETTING,theMsg.GetMsg(MSG_SETTING));
    SetDlgItemTextW(IDC_STATIC_TYPE,theMsg.GetMsg(MSG_SEQ_TYPE));
    SetDlgItemTextW(IDC_STATIC_ROW,theMsg.GetMsg(MSG_NO_OF_ROW_COL));	//v1.5.T91 edit
    SetDlgItemTextW(IDC_STATIC_AHEAD_COL,theMsg.GetMsg(MSG_AHEAD_COL));
    SetDlgItemTextW(IDC_STATIC_BOND_SEQUENCE,theMsg.GetMsg(MSG_BOND_SEQ));
    SetDlgItemTextW(IDC_STATIC_SUBUNIT,theMsg.GetMsg(MSG_UNIT_LAYOUT));
    SetDlgItemTextW(IDC_STATIC_MATRIX_ROW_COL,theMsg.GetMsg(MSG_MAT_ROW_COL));

    CComboBox* pCb = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
    pCb->AddString(theMsg.GetMsg(MSG_SINGLE_UNIT));
    pCb->AddString(theMsg.GetMsg(MSG_MATRIX));
    pCb->AddString(theMsg.GetMsg(MSG_HYBRID));
    pCb->AddString(theMsg.GetMsg(MSG_DIE_AHEAD));
    pCb->AddString(theMsg.GetMsg(MSG_DIE_LEAD_AHEAD));
    pCb->AddString(theMsg.GetMsg(MSG_MAT_DIE_AHEAD));
    pCb->AddString(theMsg.GetMsg(MSG_MAT_HYBRID));

	BYTE bTrue = (theMcConfig.GetMhsType() == MHS_VLED)
				|| (theMcConfig.GetMhsType() == MHS_VLED_ELITE);
	m_nBondSeqOrientation = bTrue ? SEQUENCE_HORIZONTAL : SEQUENCE_VERTICAL;

	const PGM_RECORD *pstPgm = thePgm.GetRcd();
    m_nUnitRow	= pstPgm->nDeviceRowNum;
    m_nUnitCol	= pstPgm->nDeviceColNum;
    m_nDevSeq	= pstPgm->nDeviceSeq;
    m_nMatRow	= pstPgm->nMatrixRow;
    m_nMatCol	= pstPgm->nMatrixColumn;
    m_nAheadCol	= pstPgm->nPrAheadCol;

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
        m_nBondSeqOrientation = pstPgm->nBondSeqOrientation;

	CString str;
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
        str = theMsg.GetMsg(MSG_START_TO_TEACH_SEQ);
    else
		str = theMsg.GetMsg(m_nDevSeq == PGM_SINGLE ? MSG_SELECT_PROGRAM_TYPE : MSG_CONVERT_TO_SINGLE_SEQ);
    ((CListBox*)GetDlgItem(IDC_LIST_INFO))->AddString(str);

    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
    {
		bTrue = (m_nDevSeq == PGM_DIE_AHEAD)
				|| (m_nDevSeq == PGM_DIE_LEAD_AHEAD)
				|| (m_nDevSeq == PGM_MAT_DIE_AHEAD);
		GetDlgItem(IDC_EDIT_AHEAD_COL)->EnableWindow(bTrue);

		bTrue = (m_nDevSeq == PGM_MAT_DIE_AHEAD)
				|| (m_nDevSeq == PGM_MAT_HYBRID)
				|| (m_nDevSeq == PGM_DIE_AHEAD);	//v1.5.T93 add
		GetDlgItem(IDC_EDIT_MAT_ROW)->EnableWindow(bTrue);
		GetDlgItem(IDC_EDIT_MAT_COL)->EnableWindow(bTrue);

        GetDlgItem(IDC_COMBO_BOND_SEQ)->EnableWindow(true);
    }
    else
    {
		bTrue = (m_nDevSeq == PGM_SINGLE);
		GetDlgItem(IDC_EDIT_ROW)->EnableWindow(bTrue);
		GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(bTrue);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(bTrue);
		GetDlgItem(IDC_EDIT_AHEAD_COL)->EnableWindow(bTrue);
		GetDlgItem(IDC_EDIT_MAT_ROW)->EnableWindow(bTrue);
		GetDlgItem(IDC_EDIT_MAT_COL)->EnableWindow(bTrue);
		GetDlgItem(IDC_COMBO_BOND_SEQ)->EnableWindow(bTrue);

        UpdateSubUnitLayout();
    }

	//v3.0.Y490 add
	if (theBond.m_nRestartWire > 1)	//start wire
		GetDlgItem(IDC_COMBO_PROGRAM_TYPE)->EnableWindow(false);
	else
		GetDlgItem(IDC_COMBO_PROGRAM_TYPE)->EnableWindow();

	return CPropertyPage::OnInitDialog();
}

BOOL CTchSequencePage::OnSetActive()
{
    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
        pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(0);
    
    return CPropertyPage::OnSetActive();
}

HBRUSH CTchSequencePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTchSequencePage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(true);

    CListBox *pListBox;
    if ((m_nDevSeq == PGM_HYBRID)
		|| (m_nDevSeq == PGM_DIE_AHEAD)
		|| (m_nDevSeq == PGM_DIE_LEAD_AHEAD)
		|| (m_nDevSeq == PGM_MAT_HYBRID)
		|| (m_nDevSeq == PGM_MAT_DIE_AHEAD))
    {
        if ((m_nUnitRow < 2) && (m_nUnitCol < 2))
        {
            pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
            pListBox->ResetContent();
            pListBox->AddString(theMsg.GetMsg(MSG_UNIT_ROW_COL_NUM_WRONG));
            return;
        }
    }

    GetDlgItem(IDC_EDIT_ROW)->EnableWindow(0);
    GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(0);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
    GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(1);
    GetDlgItem(IDC_EDIT_MAT_ROW)->EnableWindow(false);
    GetDlgItem(IDC_EDIT_MAT_COL)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(1);

    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
        pSheet->GetDlgItem(ID_WIZFINISH)->EnableWindow(0);

    UpdateSubUnitLayout();

    m_nTeachStep = 0;
    if (m_nDevSeq == PGM_SINGLE)
    {
        m_nTeachStep = 4;

        pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
        pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_TEACH_SEQ_FINISH));

		theBond.SetCurTrack(INNER_TRACK);        

        UpdateDevSeq();
        ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateOverView();

        GetDlgItem(IDC_EDIT_ROW)->EnableWindow();
        GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow();
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
        GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(0);
        GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(0);

        if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
            pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow();
    }
    else
    {
        pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
        pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_TOP_RIGHT_ALIGN_PT));
        pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));

        m_pButtonArray.GetAt(0)->SetWindowTextW(_T("X"));	//v1.5.T91 add

        const REF_RECORD *pstRef = theRef.GetRcd(0);
		TBL_POS stPos = pstRef->stManuAlignPos[0];
        theMatrix.TransformPos(0, &stPos);
		theServo.MoveTable2(stPos);	//v3.1.T38 add, v3.1.T43 edit
        theVisionSys.UpdateLightingParameter(
						pstRef->stManuAlignLightPara[0].nCoaxRedLight,
						pstRef->stManuAlignLightPara[0].bSideRedLight, 
						pstRef->stManuAlignLightPara[0].nCoaxBlueLight,
						pstRef->stManuAlignLightPara[0].bSideBlueLight);
        theVisionSys.ChangeLensMag(pstRef->nManuAlignMag[0]);
    }
}

// Free memory of button array
void CTchSequencePage::FreeButtonArray(void)
{
	for (short i = 0; i < m_pButtonArray.GetCount(); i++)
		delete m_pButtonArray.GetAt(i);

	m_pButtonArray.RemoveAll();
}

void CTchSequencePage::OnContinue()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();

    switch (m_nTeachStep)
    {
    case 0:
	{
	    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		{
            theServo.GetTablePosition(&m_stTopRightPt);
			m_stTopLeftPt = m_stBottomLeftPt = m_stTopRightPt;	//v3.1.T55 add
		}
        else
        {
            m_stTopRightPt.dx = -3000.0 / theServo.GetResolXY();
            m_stTopRightPt.dy =  3000.0 / theServo.GetResolXY();
        }

		if (theMcConfig.GetMhsType() == MHS_FOXCONN)
			theMcConfig.SetFirstIndexPos(theFoxconnTrack.GetIndexPos());

        m_pButtonArray.GetAt(0)->SetWindowTextW(_T(""));	//v1.5.T91 add

		switch (theMcConfig.GetMhsType())
		{
		//case MHS_TOP_LED:	//v3.1.T40 delete
		case MHS_CU_ELITE: case MHS_R2R:
		case MHS_FOXCONN:	//v3.0.T192 add
		case MHS_TOP_ELITE:	//v3.0.T410 add
		{
			if (m_nUnitCol == 1)	//v3.1.T68 add
			{
				m_nTeachStep = 9;
	            pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_DOWN_UNIT2_PT));
		        m_pButtonArray.GetAt(1)->SetWindowTextW(_T("X"));	//v3.1.T40 edit
			}
			else
			{
	            m_nTeachStep = 8;	//v3.0.T311 edit
	            pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_LEFT_UNIT2_PT));
		        m_pButtonArray.GetAt(m_nUnitRow)->SetWindowTextW(_T("X"));	//v3.1.T40 edit
			}
			break;
        }
		case MHS_TOP_LED:	//v3.1.T67 add
		{
			if (m_nUnitCol == 1)	//v3.1.T68 add
			{
				m_nTeachStep = 2;
				pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_BOTTOM_LEFT_ALIGN_PT));
				m_pButtonArray.GetAt(m_nUnitRow - 1)->SetWindowTextW(_T("X"));
			}
			else
			{
				m_nTeachStep = 1;
				pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_TOP_LEFT_ALIGN_PT));
				m_pButtonArray.GetAt(m_nUnitRow * (m_nUnitCol - 1))->SetWindowTextW(_T("X"));
			}
			break;
		}
		default:
        {
            m_nTeachStep = 2;
            pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_BOTTOM_LEFT_ALIGN_PT));
            m_pButtonArray.GetAt(m_nUnitRow * m_nUnitCol - 1)->SetWindowTextW(_T("X"));	//v1.5.T91 add
        }
		}
        pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
        break;
	}
	case 8:	//v3.0.T311 add
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		{
			TBL_POS stLeftUnit2;
			theServo.GetTablePosition(&stLeftUnit2);
			m_stTopLeftPt.dx = (stLeftUnit2.dx - m_stTopRightPt.dx)
								* (m_nUnitCol - 1) + m_stTopRightPt.dx;
			m_stTopLeftPt.dy = stLeftUnit2.dy;
			theServo.MoveTable2(m_stTopLeftPt);	//v3.1.T38 add, v3.1.T43 edit
		}
		m_nTeachStep = 1;
		pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_TOP_LEFT_ALIGN_PT));
		m_pButtonArray.GetAt(m_nUnitRow)->SetWindowTextW(_T(""));	//v3.1.T40 edit, v3.1.T68 edit
        m_pButtonArray.GetAt(m_nUnitRow * (m_nUnitCol - 1))->SetWindowTextW(_T("X"));
		break;
	}
	case 1: //get top-left position
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
			theServo.GetTablePosition(&m_stTopLeftPt); 
		else
		{
            m_stTopLeftPt.dx = m_stTopRightPt.dx + 6000.0 * (m_nUnitCol - 1) / theServo.GetResolXY();
			m_stTopLeftPt.dy = m_stTopRightPt.dy; 
		}

		if (theMcConfig.GetMhsType() == MHS_FOXCONN)
		{
			double dPitch_x1um = abs(m_stTopRightPt.dx - m_stTopLeftPt.dx)
									/ (m_nUnitCol - 1) * m_nUnitCol * theServo.GetResolXY();
			theMcConfig.SetIndexPitch(round(dPitch_x1um));

			m_nTeachStep = 9;	//v3.0.T311 edit
			m_pButtonArray.GetAt(m_nUnitRow * (m_nUnitCol - 1))->SetWindowTextW(_T(""));
			m_pButtonArray.GetAt(m_nUnitRow * (m_nUnitCol - 1) + 1)->SetWindowTextW(_T("X"));
			pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_DOWN_UNIT2_PT));
		}
		else	//v3.1.T67 add
		{
			m_nTeachStep = 2;
			m_pButtonArray.GetAt(m_nUnitRow * (m_nUnitCol - 1))->SetWindowTextW(_T(""));
			m_pButtonArray.GetAt((m_nUnitRow * m_nUnitCol) - 1)->SetWindowTextW(_T("X"));
			pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_BOTTOM_LEFT_ALIGN_PT));
		}
		pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
		break;
	}
	case 9:	//v3.0.T311 add
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		{
			TBL_POS stDownUnit2;
			theServo.GetTablePosition(&stDownUnit2);
			m_stBottomLeftPt.dx = stDownUnit2.dx;
			m_stBottomLeftPt.dy = (stDownUnit2.dy - m_stTopLeftPt.dy)
									* (m_nUnitRow - 1) + m_stTopLeftPt.dy;
			theServo.MoveTable2(m_stBottomLeftPt);	//v3.1.T38 add, v3.1.T43 edit
		}
		m_nTeachStep = 2;
		m_pButtonArray.GetAt(m_nUnitRow * (m_nUnitCol - 1) + 1)->SetWindowTextW(_T(""));
        m_pButtonArray.GetAt((m_nUnitRow * m_nUnitCol) - 1)->SetWindowTextW(_T("X"));

		pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_BOTTOM_LEFT_ALIGN_PT));
		pListBox->AddString(theMsg.GetMsg(MSG_CONTINUE_WHEN_READY));
		break;
	}
	case 2:	//get bottom-left position
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
            theServo.GetTablePosition(&m_stBottomLeftPt);
        else
        {
            m_stBottomLeftPt.dx = m_stTopLeftPt.dx;
            m_stBottomLeftPt.dy = m_stTopLeftPt.dy - 6000.0 * (m_nUnitRow-1) / theServo.GetResolXY();
        }

        //m_pButtonArray.GetAt(m_nUnitRow * m_nUnitCol - 1)->SetState(FALSE);		//v1.5.T91 delete
        m_pButtonArray.GetAt(m_nUnitRow * m_nUnitCol - 1)->SetWindowTextW(_T(""));	//v1.5.T91 add
        pListBox->AddString(theMsg.GetMsg(MSG_SELECT_BOND_SEQ));
        OnBondSeq();

		if (m_nDevSeq == PGM_MAT_HYBRID)
        {
            m_stMatCol2TopRightPt = m_stTopRightPt;
            m_stMatRow2TopRightPt = m_stTopRightPt;
            m_nTeachStep = 6;
        }
		//v1.5.T93 add
		else if	(((m_nDevSeq == PGM_DIE_AHEAD)
				|| (m_nDevSeq == PGM_MAT_DIE_AHEAD))
				&& ((m_nMatRow > 1) || (m_nMatCol > 1)))
		{
            m_stMatCol2TopRightPt = m_stTopRightPt;
            m_stMatRow2TopRightPt = m_stTopRightPt;
            m_nTeachStep = 6;
		}
        else
        {
            pListBox->AddString(theMsg.GetMsg(MSG_TEACH_SEQ_FINISH));
            m_nTeachStep = 4;
        }
        break;
	}
	case 6:	//teach matrix pitch
	{
		pListBox->AddString(theMsg.GetMsg(MSG_TCH_MAT_PITCH));

        if (m_nMatCol > 1)
        {
			theServo.MoveTable2(m_stTopLeftPt);	//v3.1.T38 add, v3.1.T43 edit
			pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_2ND_COL_TOP_RIGHT_ALIGN_PT));
            m_nTeachStep = 3;
        }
        else
        {
			//v1.5.T93 add
			TBL_POS stPos;
			stPos.dx = m_stTopRightPt.dx;
			stPos.dy = m_stBottomLeftPt.dy;
			theServo.MoveTable2(stPos);	//v3.1.T38 add, v3.1.T43 edit

            pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_2ND_ROW_TOP_RIGHT_ALIGN_PT));
            m_nTeachStep = 5;
        }
        break;
	}
	case 3:	//get matrix 2nd col top-right position
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
            theServo.GetTablePosition(&m_stMatCol2TopRightPt);
        else
        {
            m_stMatCol2TopRightPt.dx = m_stTopRightPt.dx + 20000.0;
            m_stMatCol2TopRightPt.dy = m_stTopRightPt.dy;
        }

        if (m_nMatRow > 1)
        {
			//v1.5.T95 add
			TBL_POS stPos;
			stPos.dx = m_stTopRightPt.dx;
			stPos.dy = m_stBottomLeftPt.dy;
			theServo.MoveTable2(stPos);	//v3.1.T38 add, v3.1.T43 edit

			pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_2ND_ROW_TOP_RIGHT_ALIGN_PT));
            m_nTeachStep = 5;
        }
        else
        {
            pListBox->AddString(theMsg.GetMsg(MSG_TEACH_SEQ_FINISH));
            m_nTeachStep = 4;
        }
        break;
	}
	case 5:	//get matrix 2nd row top-right position
	{
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
            theServo.GetTablePosition(&m_stMatRow2TopRightPt);
        else
        {
            m_stMatRow2TopRightPt.dx = m_stTopRightPt.dx;
            m_stMatRow2TopRightPt.dy = m_stTopRightPt.dy - 20000.0;
        }

        pListBox->AddString(theMsg.GetMsg(MSG_TEACH_SEQ_FINISH));
        m_nTeachStep = 4;
        break;
	}
	case 4:	//create program
	{
        m_nTeachStep = 7;
		theBond.SetCurTrack(INNER_TRACK);        
        UpdateDevSeq();
        ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateOverView();

        GetDlgItem(IDC_EDIT_ROW)->EnableWindow(0);
        GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(0);
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
        GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(0);
        GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(0);
		Sleep_x1ms(100);

		theBond.ResetMatrixPrStatus(1);
        bool bBkupIll = theBond.IsIll();
        theBond.SetIll(false);
		theBond.PerformVisionAlignment(true, VISION_ACTION_ALL_UNIT);

        if (((theMcConfig.GetMhsType() == MHS_VLED)
				|| (theMcConfig.GetMhsType() == MHS_VLED_ELITE))
			&& ((thePgm.GetDeviceSeq() == PGM_HYBRID)
				|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)))
        {
            CaliUnitPitch();
        }

        theBond.SetIll(bBkupIll);

		theVisionSys.EnableVideo(true);
		theScanKeyThread.EnableJoyStick(); 
		theVisionSys.ClearDrawing(); 
		theVisionDisplay.EnableDisplay(true); 

        CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
        if (pSheet->m_lRecipeOperateMode == RECIPE_TEACH)
            pSheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(1);
        break;
	}
    default:
        break;
    }        
}

void CTchSequencePage::OnBondSeq()
{
    UpdateData(TRUE);
    Invalidate();
    UpdateWindow();

    CDC* pDC = GetDC();
    CPen pen;
    pen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
    pDC->SelectObject(&pen);

    CPoint CurPt, NextPt;
    CurPt = NextPt = m_LayoutStartPt;
    short i = 0, j = 0;
    if (m_nBondSeqOrientation == SEQUENCE_VERTICAL)
    {
        for (i = 0; i < m_nUnitCol; i++)
        {
            if (m_nUnitRow > 1)
            {
                for (j = 0; j < m_nUnitRow; j++)
                {
                    CurPt = NextPt;
                    if (j == m_nUnitRow - 1)
                        NextPt.x -= m_nUnitWidth;
                    else if (i % 2 == 0)
                        NextPt.y += m_nUnitWidth;
                    else
                        NextPt.y -= m_nUnitWidth;

                    if ((i == (m_nUnitCol - 1)) && (j == (m_nUnitRow - 1)))
                        continue;
                    PlotSeqArrow(&CurPt, &NextPt, pDC);
                }
            }
            else
            {
                CurPt = NextPt;
                NextPt.x -= m_nUnitWidth;
                if (i == (m_nUnitCol - 1)) continue;
                PlotSeqArrow(&CurPt, &NextPt, pDC);
            }
        }
    }
    else if (m_nBondSeqOrientation == SEQUENCE_HORIZONTAL)
    {
        for (i = 0; i < m_nUnitRow; i++)
        {
            if (m_nUnitCol > 1)
            {
                for (j = 0; j < m_nUnitCol; j++)
                {
                    CurPt = NextPt;
                    if (j == m_nUnitCol - 1)
                    {
                        NextPt.x = m_LayoutStartPt.x;
                        NextPt.y += m_nUnitWidth;
                    }
                    else
                        NextPt.x -= m_nUnitWidth;

                    if ((i == (m_nUnitRow - 1)) && (j == (m_nUnitCol - 1)))
                        continue;
                    PlotSeqArrow(&CurPt, &NextPt, pDC);
                }
            }
            else
            {
                CurPt = NextPt;
                NextPt.y += m_nUnitWidth;
                if (i == (m_nUnitRow - 1)) continue;
                PlotSeqArrow(&CurPt, &NextPt, pDC);
            }
        }
    }

    ReleaseDC(pDC);
}

void CTchSequencePage::PlotSeqArrow(CPoint* pCurPt, CPoint* pNextPt, CDC* pCtrlDC)
{
	pCtrlDC->MoveTo(*pCurPt);
	pCtrlDC->LineTo(*pNextPt);

	CPoint LeftPt, RightPt;
	if (pCurPt->x == pNextPt->x)
	{
		LeftPt.x = pCurPt->x - 3;
		RightPt.x = pCurPt->x + 3;
		if (pNextPt->y > pCurPt->y)
			LeftPt.y = RightPt.y = pNextPt->y - 5;
		else
			LeftPt.y = RightPt.y = pNextPt->y + 5;
	}
	else if (pCurPt->y == pNextPt->y)
	{
		LeftPt.y = pCurPt->y - 3;
		RightPt.y = pCurPt->y + 3;
		if (pNextPt->x > pCurPt->x)
			LeftPt.x = RightPt.x = pNextPt->x - 5;
		else 
			LeftPt.x = RightPt.x = pNextPt->x + 5;
	}
	else
		return;

	pCtrlDC->MoveTo(LeftPt);
	pCtrlDC->LineTo(*pNextPt);
	pCtrlDC->MoveTo(RightPt);
	pCtrlDC->LineTo(*pNextPt);
}

void CTchSequencePage::UpdateDevSeq(void)
{
    TBL_POS stUnitRowOffset = TBL_POS(0.0, 0.0);	//v3.1.T54 edit
	TBL_POS stUnitColOffset = TBL_POS(0.0, 0.0);	//v3.1.T54 edit
	if (m_nUnitRow == 1)
	{
		if (m_nUnitCol > 1)
		{
			stUnitColOffset.dx = (m_stBottomLeftPt.dx - m_stTopRightPt.dx) / (m_nUnitCol - 1);
			stUnitColOffset.dy = (m_stBottomLeftPt.dy - m_stTopRightPt.dy) / (m_nUnitCol - 1);
		}
	}
	else	//m_nUnitRow > 1
	{
		if (m_nUnitCol > 1)
		{
			stUnitColOffset.dx = (m_stTopLeftPt.dx - m_stTopRightPt.dx) / (m_nUnitCol - 1);
			stUnitColOffset.dy = (m_stTopLeftPt.dy - m_stTopRightPt.dy) / (m_nUnitCol - 1);
		}

		stUnitRowOffset.dx = (m_stBottomLeftPt.dx - m_stTopLeftPt.dx ) / (m_nUnitRow - 1);
		stUnitRowOffset.dy = (m_stBottomLeftPt.dy - m_stTopLeftPt.dy) / (m_nUnitRow - 1);
	}

	TBL_POS stMatRowOffset = TBL_POS(0.0, 0.0);	//v3.1.T54 edit
	TBL_POS stMatColOffset = TBL_POS(0.0, 0.0);	//v3.1.T54 edit
    if ((m_nDevSeq == PGM_MAT_DIE_AHEAD)
		|| (m_nDevSeq == PGM_MAT_HYBRID)
		|| (m_nDevSeq == PGM_DIE_AHEAD))	//v1.5.T93 add
    {
        if (m_nMatRow > 1)
        {
            stMatRowOffset.dx = m_stMatRow2TopRightPt.dx - m_stTopRightPt.dx;
            stMatRowOffset.dy = m_stMatRow2TopRightPt.dy - m_stTopRightPt.dy;
        }
        if (m_nMatCol > 1)
        {
            stMatColOffset.dx = m_stMatCol2TopRightPt.dx - m_stTopRightPt.dx;
            stMatColOffset.dy = m_stMatCol2TopRightPt.dy - m_stTopRightPt.dy;
        }
    }

	PGM_RECORD *pstPgm = thePgm.GetRcd();
    pstPgm->nDeviceSeq			= m_nDevSeq;
    pstPgm->stUnitRowOffset		= stUnitRowOffset;
    pstPgm->stUnitColOffset		= stUnitColOffset;
    pstPgm->nBondSeqOrientation	= m_nBondSeqOrientation;
    pstPgm->nTrackType			= m_nTrackType;
    pstPgm->stTrackOffset		= m_stTrackOffset;
	if (m_nDevSeq == PGM_DIE_AHEAD)
	{
	    pstPgm->nDeviceRowNum = m_nUnitRow * m_nMatRow;
		pstPgm->nDeviceColNum = m_nUnitCol * m_nMatCol;	//v1.5.T95 edit
	    pstPgm->nMatrixRow    = 1;
		pstPgm->nMatrixColumn = 1;
	}
	else
	{
	    pstPgm->nDeviceRowNum = m_nUnitRow;
		pstPgm->nDeviceColNum = m_nUnitCol;
	    pstPgm->nMatrixRow    = m_nMatRow;
		pstPgm->nMatrixColumn = m_nMatCol;
	}
    pstPgm->stMatrixRowOffset   = stMatRowOffset;
    pstPgm->stMatrixColOffset   = stMatColOffset;
    pstPgm->nPrAheadCol         = m_nAheadCol;

    if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
		|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
		|| (theMcConfig.GetMhsType() == MHS_R2R)
		|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
    {
        pstPgm->nPrAlignMode = 0;
    }
    else
    {
        if ((m_nDevSeq == PGM_DIE_LEAD_AHEAD)
			|| (m_nDevSeq == PGM_DIE_AHEAD)
			|| (m_nDevSeq == PGM_MAT_DIE_AHEAD))
            pstPgm->nPrAlignMode = 1; //one-pt lead alignment
        else
            pstPgm->nPrAlignMode = 0; //two-pt lead alignment
    }

    TBL_POS stUnitOS;
    REF_RECORD stRefRcd;
    WIRE_RECORD stWireRcd;
    short i, j, nCurRow, nCurCol;
	short nTotalUnit, nUnitTotalDie;
	short nUnitTotalWire = theWire.GetTotalWireNum();

	switch (m_nDevSeq)
	{
	case PGM_DIE_AHEAD:
    {
        nUnitTotalDie = theRef.GetTotalRefNum() - 1;	//Tony: exclude ref 0

		TBL_POS stMatPanelOS = TBL_POS(0,0);
		int nTotalCol = m_nUnitCol * m_nMatCol;	//v1.5.T95 add
		int nTotalRow = m_nUnitRow * m_nMatRow;
		nTotalUnit = nTotalRow * nTotalCol;
        for (i = 1; i < nTotalUnit; i++)
		{
            if (m_nBondSeqOrientation == SEQUENCE_VERTICAL)
            {
                nCurCol = i / nTotalRow;
                nCurRow = i % nTotalRow;

                if ((nCurCol % 2) == 1)
                    nCurRow = nTotalRow - nCurRow - 1;	//invert direction
			
				//add matrix row offset when bonding in 2nd panel onwards
				//this logic is not easy to understand, change with cautious
				stMatPanelOS.dx = (stMatColOffset.dx - (stUnitColOffset.dx * m_nUnitCol))
										* ((nCurCol / m_nUnitCol) % m_nMatCol);
				stMatPanelOS.dy = (stMatRowOffset.dy - (stUnitRowOffset.dy * m_nUnitRow))
										* ((nCurRow / m_nUnitRow) % m_nMatRow);
			}
            else if (m_nBondSeqOrientation == SEQUENCE_HORIZONTAL)
            {
                nCurRow = i / m_nUnitCol;
                nCurCol = i % m_nUnitCol;
            }

            for (j = 1; j <= nUnitTotalDie; j++)
            {
                theRef.AddRcd(theMcData.GetLocateMode());
                theRef.GetRcd(&stRefRcd, j); //Get die reference of 1st unit
                thePgm.CalcUnitOffset(nCurRow+1, nCurCol+1, &stUnitOS);
                stRefRcd.stManuAlignPos[0].dx += stUnitOS.dx + stMatPanelOS.dx;
                stRefRcd.stManuAlignPos[0].dy += stUnitOS.dy + stMatPanelOS.dy;
                stRefRcd.stManuAlignPos[1].dx += stUnitOS.dx + stMatPanelOS.dx;
                stRefRcd.stManuAlignPos[1].dy += stUnitOS.dy + stMatPanelOS.dy;
                stRefRcd.stVisionAlignPos[0].dx += stUnitOS.dx + stMatPanelOS.dx;
                stRefRcd.stVisionAlignPos[0].dy += stUnitOS.dy + stMatPanelOS.dy;
                stRefRcd.stVisionAlignPos[1].dx += stUnitOS.dx + stMatPanelOS.dx;
                stRefRcd.stVisionAlignPos[1].dy += stUnitOS.dy + stMatPanelOS.dy;
                stRefRcd.nRefNum = i * nUnitTotalDie + j;
                theRef.SetRcd(&stRefRcd, stRefRcd.nRefNum);
                theMatrix.CoordinateMatrixInfo(stRefRcd.nRefNum);
            }

            for (j = 1; j <= nUnitTotalWire; j++)  
            {
                theWire.AddRcd();
                theWire.GetRcd(&stWireRcd, j);
				if (stWireRcd.nRefNum[0] != 0)
                    stWireRcd.nRefNum[0] += i * nUnitTotalDie;
				if (stWireRcd.nRefNum[1] != 0)
					stWireRcd.nRefNum[1] += i * nUnitTotalDie;
                stWireRcd.nWireNum = (i * nUnitTotalWire) + j;
                stWireRcd.stTchPos[0].dx += stUnitOS.dx + stMatPanelOS.dx;
                stWireRcd.stTchPos[0].dy += stUnitOS.dy + stMatPanelOS.dy;
                stWireRcd.stTchPos[1].dx += stUnitOS.dx + stMatPanelOS.dx;
                stWireRcd.stTchPos[1].dy += stUnitOS.dy + stMatPanelOS.dy;
                theWire.SetRcd(&stWireRcd, stWireRcd.nWireNum);
                theWire.AppendBondPoint();
            }
        }
		break;
    }
	case PGM_MAT_DIE_AHEAD:	//v1.6.T325 add
    {
        nUnitTotalDie = theRef.GetTotalRefNum() - 1;	//Tony: exclude ref 0

		nTotalUnit = m_nUnitRow * m_nUnitCol;
        for (i = 1; i < nTotalUnit; i++)
		{
            if (m_nBondSeqOrientation == SEQUENCE_VERTICAL)
            {
                nCurCol = i / m_nUnitRow;
                nCurRow = i % m_nUnitRow;

                if ((nCurCol % 2) == 1)
                    nCurRow = m_nUnitRow - nCurRow - 1;
            }
            else if (m_nBondSeqOrientation == SEQUENCE_HORIZONTAL)
            {
                nCurRow = i / m_nUnitCol;
                nCurCol = i % m_nUnitCol;
            }

            for (j = 1; j <= nUnitTotalDie; j++)
            {
                theRef.AddRcd(theMcData.GetLocateMode());
                theRef.GetRcd(&stRefRcd, j); //Get die reference of 1st unit
                thePgm.CalcUnitOffset(nCurRow+1, nCurCol+1, &stUnitOS);
                stRefRcd.stManuAlignPos[0].dx += stUnitOS.dx;
                stRefRcd.stManuAlignPos[0].dy += stUnitOS.dy;
                stRefRcd.stManuAlignPos[1].dx += stUnitOS.dx;
                stRefRcd.stManuAlignPos[1].dy += stUnitOS.dy;
                stRefRcd.stVisionAlignPos[0].dx += stUnitOS.dx;
                stRefRcd.stVisionAlignPos[0].dy += stUnitOS.dy;
                stRefRcd.stVisionAlignPos[1].dx += stUnitOS.dx;
                stRefRcd.stVisionAlignPos[1].dy += stUnitOS.dy;
				stRefRcd.nRefNum = (i * nUnitTotalDie) + j;
                theRef.SetRcd(&stRefRcd, stRefRcd.nRefNum);
                theMatrix.CoordinateMatrixInfo(stRefRcd.nRefNum);
            }

			short nCurWire;
            for (j = 1; j <= nUnitTotalWire; j++)  
            {
                theWire.AddRcd();
				if (((nCurCol % 2) == 1) && (theMcConfig.GetDeviceSeq() == 1))	//v1.6.T331 add
					theWire.GetRcd(&stWireRcd, nUnitTotalWire - j + 1);
				//v3.0.T15 add
				else if (((nCurCol % 2) == 1) && (theMcConfig.GetDeviceSeq() == 2))	//v1.6.T331 add, v3.1.T323 edit
				{
					nCurWire = j;
					if (nCurWire == 1) nCurWire = 2;
					else if (nCurWire == 2) nCurWire = 1;
					else if (nCurWire == 4) nCurWire = 5;
					else if (nCurWire == 5) nCurWire = 4;
					theWire.GetRcd(&stWireRcd, nUnitTotalWire - nCurWire + 1);
				}
				else
					theWire.GetRcd(&stWireRcd, j);
				if (stWireRcd.nRefNum[0] != 0)
					stWireRcd.nRefNum[0] += i * nUnitTotalDie;
				if (stWireRcd.nRefNum[1] != 0)
					stWireRcd.nRefNum[1] += i * nUnitTotalDie;
				stWireRcd.nWireNum = (i * nUnitTotalWire) + j;
                stWireRcd.stTchPos[0].dx += stUnitOS.dx;
                stWireRcd.stTchPos[0].dy += stUnitOS.dy;
                stWireRcd.stTchPos[1].dx += stUnitOS.dx;
                stWireRcd.stTchPos[1].dy += stUnitOS.dy;
                theWire.SetRcd(&stWireRcd, stWireRcd.nWireNum);
                theWire.AppendBondPoint();
            }
        }
		break;
    }
	case PGM_HYBRID:
	case PGM_MAT_HYBRID:
    {
        nUnitTotalDie = theRef.GetTotalRefNum() - 1;	//Tony: exclude ref 0

		nTotalUnit = m_nUnitRow * m_nUnitCol;
        for (i = 1; i < nTotalUnit; i++)
		{
            if (m_nBondSeqOrientation == SEQUENCE_VERTICAL)
            {
                nCurCol = i / m_nUnitRow;
                nCurRow = i % m_nUnitRow;

                if ((nCurCol % 2) == 1)
                    nCurRow = m_nUnitRow - nCurRow - 1;
            }
            else if (m_nBondSeqOrientation == SEQUENCE_HORIZONTAL)
            {
                nCurRow = i / m_nUnitCol;
                nCurCol = i % m_nUnitCol;
            }

            for (j = 1; j <= nUnitTotalDie; j++)
            {
                theRef.AddRcd(theMcData.GetLocateMode());
                theRef.GetRcd(&stRefRcd, j); //Get die reference of 1st unit
                thePgm.CalcUnitOffset(nCurRow+1, nCurCol+1, &stUnitOS);
                stRefRcd.stManuAlignPos[0].dx += stUnitOS.dx;
                stRefRcd.stManuAlignPos[0].dy += stUnitOS.dy;
                stRefRcd.stManuAlignPos[1].dx += stUnitOS.dx;
                stRefRcd.stManuAlignPos[1].dy += stUnitOS.dy;
                stRefRcd.stVisionAlignPos[0].dx += stUnitOS.dx;
                stRefRcd.stVisionAlignPos[0].dy += stUnitOS.dy;
                stRefRcd.stVisionAlignPos[1].dx += stUnitOS.dx;
                stRefRcd.stVisionAlignPos[1].dy += stUnitOS.dy;
                stRefRcd.nRefNum = i * nUnitTotalDie + j;
                theRef.SetRcd(&stRefRcd, stRefRcd.nRefNum);
                theMatrix.CoordinateMatrixInfo(stRefRcd.nRefNum);
            }

            for (j = 1; j <= nUnitTotalWire; j++)  
            {
                theWire.AddRcd();
                theWire.GetRcd(&stWireRcd, j);
				if (stWireRcd.nRefNum[0] != 0)
                    stWireRcd.nRefNum[0] += i * nUnitTotalDie;
				if (stWireRcd.nRefNum[1] != 0)
					stWireRcd.nRefNum[1] += i * nUnitTotalDie;
                stWireRcd.nWireNum = (i * nUnitTotalWire) + j;
                stWireRcd.stTchPos[0].dx += stUnitOS.dx;
                stWireRcd.stTchPos[0].dy += stUnitOS.dy;
                stWireRcd.stTchPos[1].dx += stUnitOS.dx;
                stWireRcd.stTchPos[1].dy += stUnitOS.dy;
                theWire.SetRcd(&stWireRcd, stWireRcd.nWireNum);
                theWire.AppendBondPoint();
            }
        }
		break;
    }
	case PGM_DIE_LEAD_AHEAD:
	{
        nUnitTotalDie = theRef.GetTotalRefNum(); 

		nTotalUnit = m_nUnitRow * m_nUnitCol;
        for (i = 1; i < nTotalUnit; i++)
        {
            if (m_nBondSeqOrientation == SEQUENCE_VERTICAL)
            {
                nCurCol = i / m_nUnitRow;
                nCurRow = i % m_nUnitRow;

                if ((nCurCol % 2) == 1)
                    nCurRow = m_nUnitRow - nCurRow - 1;
            }
            else if (m_nBondSeqOrientation == SEQUENCE_HORIZONTAL)
            {
                nCurRow = i / m_nUnitCol;
                nCurCol = i % m_nUnitCol;
            }

            for (j = 0; j < nUnitTotalDie; j++)  //j=0 or 1
            {
                theRef.AddRcd(theMcData.GetLocateMode());
                theRef.GetRcd(&stRefRcd, j); //Get lead reference of 1st unit
                thePgm.CalcUnitOffset(nCurRow+1, nCurCol+1, &stUnitOS);
                stRefRcd.stManuAlignPos[0].dx += stUnitOS.dx;
                stRefRcd.stManuAlignPos[0].dy += stUnitOS.dy;
                stRefRcd.stManuAlignPos[1].dx += stUnitOS.dx;
                stRefRcd.stManuAlignPos[1].dy += stUnitOS.dy;
                stRefRcd.stVisionAlignPos[0].dx += stUnitOS.dx;
                stRefRcd.stVisionAlignPos[0].dy += stUnitOS.dy;
                stRefRcd.stVisionAlignPos[1].dx += stUnitOS.dx;
                stRefRcd.stVisionAlignPos[1].dy += stUnitOS.dy;
                stRefRcd.nRefNum = i * nUnitTotalDie + j;
                theRef.SetRcd(&stRefRcd, stRefRcd.nRefNum);
                theMatrix.CoordinateMatrixInfo(stRefRcd.nRefNum);
            }
            
            for (j = 1; j <= nUnitTotalWire; j++)
            {
                theWire.AddRcd();
                theWire.GetRcd(&stWireRcd, j);
                stWireRcd.nRefNum[0] += i * nUnitTotalDie;
                stWireRcd.nRefNum[1] += i * nUnitTotalDie;
                stWireRcd.nWireNum = (i * nUnitTotalWire) + j;
                stWireRcd.stTchPos[0].dx += stUnitOS.dx;
                stWireRcd.stTchPos[0].dy += stUnitOS.dy;
                stWireRcd.stTchPos[1].dx += stUnitOS.dx;
                stWireRcd.stTchPos[1].dy += stUnitOS.dy;
                theWire.SetRcd(&stWireRcd, stWireRcd.nWireNum);
                theWire.AppendBondPoint();
            }
        }
		break;
    }
	default:
		break;
	}

    theBond.ResetBondData();
    theApp.ArchivePowerOnData(ARCHIVE_SAVE);
    theBond.SetPrPatternPassScore();
    theVisionSys.SavePatternRcd();	//v3.0.T70 edit
}

void CTchSequencePage::OnFocusRow()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(true);
    GetDlgItem(IDC_STATIC_SEQUENCE_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nUnitRow, 1, 1, 70);	//v3.0.T191 add
    UpdateData(false);
}

void CTchSequencePage::OnFocusColumn()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(true);
    GetDlgItem(IDC_STATIC_SEQUENCE_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nUnitCol, 1, 1, 70);	//v3.0.T191 add
    UpdateData(false);
}

void CTchSequencePage::OnProgramType()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    int nBkupSeqType = m_nDevSeq;
    UpdateData(TRUE);

    CListBox* pListBox;
    CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    if (pSheet->m_lRecipeOperateMode == RECIPE_EDIT)
    {
        if (nBkupSeqType != m_nDevSeq)
        {
            if ((nBkupSeqType != PGM_SINGLE) && (m_nDevSeq != PGM_SINGLE))
            {
				short anMsgArray[1] = {MSG_CONVERT_TO_SINGLE_SEQ};
				theMsg.ShowMsg(HELP_MSG, 1, anMsgArray);

                m_nDevSeq = nBkupSeqType;
                UpdateData(FALSE);

                return;
            }

            if (m_nDevSeq == PGM_SINGLE)
            {
                GetDlgItem(IDC_EDIT_ROW)->EnableWindow(0);
                GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(0);
                GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
                GetDlgItem(IDC_EDIT_AHEAD_COL)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDIT_MAT_ROW)->EnableWindow(false);
                GetDlgItem(IDC_EDIT_MAT_COL)->EnableWindow(false);
                GetDlgItem(IDC_COMBO_BOND_SEQ)->EnableWindow(false);

                thePgm.ResetDeviceSeqToSingle();

				const PGM_RECORD *pstPgm = thePgm.GetRcd();
                m_nUnitRow	= pstPgm->nDeviceRowNum;
                m_nUnitCol	= pstPgm->nDeviceColNum;
                m_nDevSeq	= pstPgm->nDeviceSeq;
                m_nMatRow	= pstPgm->nMatrixRow;
                m_nMatCol	= pstPgm->nMatrixColumn;
                m_nAheadCol	= pstPgm->nPrAheadCol;

                pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_IF_CHANGE_PROGRAM_TYPE));
                pListBox->AddString(theMsg.GetMsg(MSG_THEN_EDIT_MANUAL_AND_PR_ALIGN));
                pListBox->AddString(theMsg.GetMsg(MSG_IF_NOT_CHANGE_PROGRAM_TYPE));
                pListBox->AddString(theMsg.GetMsg(MSG_SELECT_PROGRAM_TYPE));
                UpdateData(FALSE);

                UpdateSubUnitLayout();
            }
            else
            {
                GetDlgItem(IDC_EDIT_ROW)->EnableWindow();
                GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow();
                GetDlgItem(IDC_BUTTON_START)->EnableWindow();
                GetDlgItem(IDC_COMBO_BOND_SEQ)->EnableWindow();

                pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_START_TO_TEACH_SEQ));

                UpdateData(FALSE);
            }
        }
    }

    if ((m_nDevSeq == PGM_DIE_AHEAD)
		|| (m_nDevSeq == PGM_DIE_LEAD_AHEAD)
		|| (m_nDevSeq == PGM_MAT_DIE_AHEAD))
    {
        GetDlgItem(IDC_EDIT_AHEAD_COL)->EnableWindow(true);
    }
    else
    {
        GetDlgItem(IDC_EDIT_AHEAD_COL)->EnableWindow(false);
        m_nAheadCol = 1;
        UpdateData(FALSE);
    }

    if ((m_nDevSeq == PGM_MAT_DIE_AHEAD)
		|| (m_nDevSeq == PGM_MAT_HYBRID)
		|| (m_nDevSeq == PGM_DIE_AHEAD))	//v1.5.T95 add
    {
        GetDlgItem(IDC_EDIT_MAT_ROW)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_MAT_COL)->EnableWindow(true);
    }
    else
    {
        GetDlgItem(IDC_EDIT_MAT_ROW)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_MAT_COL)->EnableWindow(false);
        m_nMatRow = 1;
        m_nMatCol = 1;
        UpdateData(FALSE);
    }
}

void CTchSequencePage::OnFocusAheadCol()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(true);
    GetDlgItem(IDC_STATIC_SEQUENCE_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nAheadCol);
    UpdateData(false);
}

void CTchSequencePage::OnBack()
{
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(1);
    GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(0);
    GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(0);
}

void CTchSequencePage::OnFocusMatRow()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(true);
    GetDlgItem(IDC_STATIC_SEQUENCE_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nMatRow, 1, 1, 10);	//v1.5.T91 edit
    UpdateData(false);
}

void CTchSequencePage::OnFocusMatCol()
{
	CBusy bs(&m_bBusy);	//V1.1.W153 add
    UpdateData(true);
    GetDlgItem(IDC_STATIC_SEQUENCE_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nMatCol, 1, 1, 10);	//v1.5.T91 edit
    UpdateData(false);
}

// Update subunit layout for bonding sequence
void CTchSequencePage::UpdateSubUnitLayout(void)
{
    FreeButtonArray();

	CTeachRecipeSheet *pSheet = (CTeachRecipeSheet*)GetParent();
    short nUnitWidth = round((double)BONDSEQ_AREA_WIDTH  / m_nUnitCol);
	short nUnitHeight = round((double)(pSheet->m_lRecipeOperateMode == RECIPE_TEACH ? INDEX_AREA_HEIGHT : BONDSEQ_AREA_HEIGHT) / m_nUnitRow);	//V1.5.W100 edit
    nUnitWidth = min(nUnitWidth, nUnitHeight);

    CPoint pt, pCentre;
	pCentre.x = BONDSEQ_AREA_LEFT + BONDSEQ_AREA_WIDTH / 2;
	pCentre.y = BONDSEQ_AREA_TOP + (pSheet->m_lRecipeOperateMode == RECIPE_TEACH ? INDEX_AREA_HEIGHT : BONDSEQ_AREA_HEIGHT) / 2;	//V1.5.W100 edit
    pt.x = pCentre.x + round((double)nUnitWidth * (double)(m_nUnitCol - 1) / 2.0);
    pt.y = pCentre.y - round((double)nUnitWidth * (double)(m_nUnitRow - 1) / 2.0);
    m_LayoutStartPt = pt;
    m_nUnitWidth = nUnitWidth;

    CRect pUnitRect;
    short i = 0, j = 0, nUnitId = IDB_SUBUNIT_BUTTON0;
    CString szString = _T("");
    for (i = 0; i < m_nUnitCol; i++)
    {
        pt.y = m_LayoutStartPt.y;

        for (j = 0; j < m_nUnitRow; j++)
        {
            CButton* pButton = new CButton();
            pUnitRect.SetRect(pt.x - nUnitWidth / 2, pt.y - nUnitWidth / 2, pt.x + nUnitWidth / 2, pt.y + nUnitWidth / 2);
            pButton->Create(szString, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, pUnitRect, this, nUnitId);
            m_pButtonArray.Add(pButton);

            pt.y += nUnitWidth;
            nUnitId++;
        }

        pt.x -= nUnitWidth;
    }
}

// Calibrate pitch between units
void CTchSequencePage::CaliUnitPitch(void)
{
    short i, nUnitTotalDie, nTotalUnit, nRefNum;
    short j, nWireNum, nUnitTotalWire = thePgm.GetUnitTotalWire();
    double dOffset, dDiePitchX, dDiePitchY;

	double *pdDieIndex = new double[120];
	double *pdDieCentreX = pdDieIndex + 40;
	double *pdDieCentreY = pdDieIndex + 80;

    MATRIX_RECORD stMatrixRcd;
    nUnitTotalDie = thePgm.GetUnitTotalDie();
    nTotalUnit = m_nUnitRow * m_nUnitCol;
    for (i = 0; i < nTotalUnit; i++)
    {
        nRefNum = 1 + i * nUnitTotalDie;
        pdDieIndex[i] = i;

        theMatrix.GetRcd(&stMatrixRcd, nRefNum);

        if (stMatrixRcd.nVisionStatus != VISION_SUCCESS)
		{
			delete pdDieIndex;
            return;
		}

        pdDieCentreX[i] = stMatrixRcd.stUpdatedCenterPos.dx;
        pdDieCentreY[i] = stMatrixRcd.stUpdatedCenterPos.dy;
    }
    
    CurveFit(nTotalUnit, pdDieIndex, pdDieCentreX, &dDiePitchX, &dOffset);
    CurveFit(nTotalUnit, pdDieIndex, pdDieCentreY, &dDiePitchY, &dOffset);

	PGM_RECORD *pstPgm = thePgm.GetRcd();
    pstPgm->stUnitColOffset.dx = dDiePitchX;
    pstPgm->stUnitColOffset.dy = dDiePitchY;

	const REF_RECORD *pstRef;
	WIRE_RECORD *pstRefWire = (WIRE_RECORD*)(new char[sizeof(WIRE_RECORD)*2]);
	WIRE_RECORD *pstWire = pstRefWire + 1;

    for (i = 1; i < nTotalUnit; i++)
    {
        for (j = 1; j <= nUnitTotalWire; j++)
        {
            nWireNum = i * nUnitTotalWire + j;
            
            theWire.GetRcd(pstRefWire, j);
            theWire.GetRcd(pstWire, nWireNum);

            pstRef = theRef.GetRcd(pstRefWire->nRefNum[0]);
            if (pstRef->nRefType == REF_TYPE_LEAD)
            {
                pstWire->stTchPos[0].dx = pstRefWire->stTchPos[0].dx + dDiePitchX * i;
                pstWire->stTchPos[0].dy = pstRefWire->stTchPos[0].dy + dDiePitchY * i;
            }

            pstRef = theRef.GetRcd(pstRefWire->nRefNum[1]);
            if (pstRef->nRefType == REF_TYPE_LEAD)
            {
                pstWire->stTchPos[1].dx = pstRefWire->stTchPos[1].dx + dDiePitchX * i;
                pstWire->stTchPos[1].dy = pstRefWire->stTchPos[1].dy + dDiePitchY * i;
            }
            
            theWire.SetRcd(pstWire, nWireNum);
        }
    }

	delete pdDieIndex;
	delete pstRefWire;
}

void CTchSequencePage::OnDestroy()
{
	CPropertyPage::OnDestroy();
}

LRESULT CTchSequencePage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W153 add
{
	return m_bBusy;
}
