/***************************************************************
TchBondPoints.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "MathCalc.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "Matrix.h"
#include "Wire.h"
#include "TchBondPoints.h"
#include "AdvWireLayoutPage.h"
#include "AutoPadMgr.h"
#include "ScanKeyThread.h"
#include "TeachRecipeSheet.h"
#include "servo.h"
#include "KeyPadHandler.h"
#include "WireBondDlg.h"
#include <comutil.h>

extern bool gbAPC;
extern BYTE gnAlgorithm;

// CTchBondPoints dialog
IMPLEMENT_DYNAMIC(CTchBondPoints, CDialog)

CTchBondPoints::CTchBondPoints(short nBondPt, int iBondOn1, int iBondOn2, short nTchMode, short nCurrentPt, int lOperateMode, CWnd* pParent)
	: m_nBondPt(nBondPt)
    , m_nCurBondPt(nBondPt) //@1
    , m_iBondOn1(iBondOn1)
    , m_iBondOn2(iBondOn2)
    , m_iPadType(0)
    , m_nTchMode(nTchMode)
    , m_nTaughtPoints(0)
    , m_nTeachingPoint(0)
    , m_nTchGroupStep(0)
    , bErrorHandlerMode(false)
    , m_lOperateMode(lOperateMode)
{
    m_nTeachingPoint = nCurrentPt;
    if (m_nCurBondPt == WIRE_BOND) //@1
        m_nCurBondPt = FIRST_BOND;
}

CTchBondPoints::~CTchBondPoints()
{
}

void CTchBondPoints::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_TAUGHT_PT, m_nTaughtPoints);
    DDX_Text(pDX, IDC_STATIC_TEACHING_PT, m_nTeachingPoint);
    DDX_CBIndex(pDX, IDC_COMBO_PAD_TYPE, m_iPadType);
}

BEGIN_MESSAGE_MAP(CTchBondPoints, CDialog)
    ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO_PAD_TYPE, OnComboPadType)
	ON_CBN_SELCHANGE(IDC_COMBO_BOND_ON, OnFirstPoint)
	ON_CBN_SELCHANGE(IDC_COMBO_BOND_ON_2, OnSecondPoint)
    ON_EN_SETFOCUS(IDC_EDIT_GAP_FILL, OnGapFill)
	ON_CBN_SELCHANGE(IDC_CB_AUTO_PAD_CENTERING, OnAutoPadCentering)
	ON_CBN_SELCHANGE(IDC_CB_BINARIZE, SaveAutoPadCentering)
	ON_CBN_SELCHANGE(IDC_COMBO_ALGORITHM, OnAlgorithm)
	ON_CBN_SELCHANGE(IDC_CB_CENTERING_TYPE, SaveAutoPadCentering)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTchBondPoints message handlers
BOOL CTchBondPoints::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CRect Rect;
	CString str;

    GetParent()->GetParent()->GetWindowRect(&Rect);

    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    SetDlgItemTextW(IDOK,theMsg.GetMsg(MSG_ENTER));
    SetDlgItemTextW(IDCANCEL,theMsg.GetMsg(MSG_ESCAPE));
   
    m_font.CreateFontW(38, 0, 0, 0, 500, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	GetDlgItem(IDC_STATIC_TITLE)->SetFont(&m_font);
    SetDlgItemTextW(IDC_STATIC_TITLE,theMsg.GetMsg(MSG_TCH_BOND_PT));

	switch (m_nTchMode)
	{
	case TCH_NEW_BOND_POINT: case TCH_INSERT_BOND_POINT:
		str = theMsg.GetMsg(m_nBondPt != SECOND_BOND ?
				MSG_MOVE_TO_1ST_BOND_PT : MSG_MOVE_TO_2ND_BOND_PT);
		break;
	case TCH_GROUP_BOND_POINT:
		str = theMsg.GetMsg(MSG_MOVE_TO_GROUP_1ST_PT);
		break;
	case TCH_EDIT_BOND_POINT:
    {
        TBL_POS st1stBondPt, st2ndBondPt;

        if (m_lOperateMode == RECIPE_EDIT)
            theWire.CalcBondPoint(m_nTeachingPoint, &st1stBondPt, &st2ndBondPt);
        else
        {
			WIRE_RECORD stWireRcd;
			theWire.GetRcd(&stWireRcd, m_nTeachingPoint);
            st1stBondPt = stWireRcd.stTchPos[0];
            st2ndBondPt = stWireRcd.stTchPos[1];
        }

        if (m_nBondPt != SECOND_BOND)
        {
            str = theMsg.GetMsg(MSG_MOVE_TO_1ST_BOND_PT);
            theServo.MoveTable2(st1stBondPt);	//v3.1.T200 edit
        }
        else
        {
            str = theMsg.GetMsg(MSG_MOVE_TO_2ND_BOND_PT);
            theServo.MoveTable2(st2ndBondPt);	//v3.1.T200 edit
        }
		break;
    }
	}

    SetDlgItemTextW(IDC_STATIC_MSG_1, str);
    SetDlgItemTextW(IDC_STATIC_MSG_2, _T(""));
    SetDlgItemTextW(IDC_STATIC_MSG_3, theMsg.GetMsg(MSG_ENTER_TO_CONTINUE));
    SetDlgItemTextW(IDC_STATIC_MSG_4, theMsg.GetMsg(MSG_ESC_TO_CANCEL));

    if (m_lOperateMode == RECIPE_EDIT)
        m_nTaughtPoints = thePgm.GetUnitTotalWire();
    else
		m_nTaughtPoints = m_nBondPt != SECOND_BOND ? theWire.Get1stBondTaughtPts() : theWire.Get2ndBondTaughtPts();

    if ((m_nTchMode == TCH_NEW_BOND_POINT) ||
        (m_nTchMode == TCH_GROUP_BOND_POINT))
        m_nTeachingPoint = m_nTaughtPoints + 1;

    short i, nTaughtPads = theAutoPadMgr.GetTotalAutoPad();

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PAD_TYPE);
    pComboBox->ResetContent();
    pComboBox->AddString(_T("None"));
    for (i = 1; i <= nTaughtPads; i++)
    {
        pComboBox->AddString(CString(_T("PadId"))+_bstr_t(i).GetBSTR());
    }
    pComboBox->ShowWindow(FALSE);

    theVisionDisplay.SetDisplayBondPt(m_nBondPt);
    
    if ((m_nTchMode == TCH_EDIT_BOND_POINT) ||
        (m_nTchMode == TCH_NEW_BOND_POINT))
    {
        theVisionDisplay.SetDisplayMode(VISION_DISPLAY_TEACH_MODE);
        theVisionDisplay.SetDisplayWire(m_nTeachingPoint);
    }

	CTeachRecipeSheet* pSheet = (CTeachRecipeSheet*)GetParent();
	pSheet->m_TchAdvWireLayoutPage.CopyUI(this);

    const PGM_RECORD *pstPgm = thePgm.GetRcd();

	SetDlgItemText(IDC_STATIC_AUTO_PAD_CENTERING, theMsg.GetMsg(MSG_AUTO_PAD_CENTERING));

	pComboBox = (CComboBox*)GetDlgItem(IDC_CB_AUTO_PAD_CENTERING);
	pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
	pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));
	pComboBox->SetCurSel(pstPgm->bAutoPadCentering);

	SetDlgItemText(IDC_STATIC_VISION_ALGORITHM, theMsg.GetMsg(MSG_VISION_ALGORITHM));

	pComboBox=(CComboBox*)GetDlgItem(IDC_COMBO_ALGORITHM);
	pComboBox->AddString(theMsg.GetMsg(MSG_BLOB_PLUS_EDGE));
	pComboBox->AddString(theMsg.GetMsg(MSG_EDGE_PLUS_BLOB));
	pComboBox->AddString(theMsg.GetMsg(MSG_BLOB));
	pComboBox->AddString(theMsg.GetMsg(MSG_EDGE));
	pComboBox->SetCurSel(gnAlgorithm);

	SetDlgItemText(IDC_STATIC_GAP_FILL, theMsg.GetMsg(MSG_GAP_FILL));

	BYTE nGapFill = pstPgm->nGapFill;
	if ((nGapFill < 1) || (nGapFill > 6)) nGapFill = 3;
	SetDlgItemInt(IDC_EDIT_GAP_FILL, nGapFill);

	SetDlgItemText(IDC_STATIC_BINARIZE, theMsg.GetMsg(MSG_BINARY_2));

	pComboBox = (CComboBox*)GetDlgItem(IDC_CB_BINARIZE);
	pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
	pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));
	pComboBox->SetCurSel(pstPgm->bBinarize);

	SetDlgItemText(IDC_STATIC_CENTERING_TYPE, theMsg.GetMsg(MSG_CENTERING_TYPE));

	pComboBox = (CComboBox*)GetDlgItem(IDC_CB_CENTERING_TYPE);
	pComboBox->AddString(theMsg.GetMsg(MSG_GRAVITY));
	pComboBox->AddString(theMsg.GetMsg(MSG_CIRCLE_2));
	pComboBox->SetCurSel(pstPgm->bCircleFit);

	gbAPC = true;
	((CWireBondDlg*)theApp.m_pMainWnd)->SetDrawAreaEnable(m_hWnd);
	theVisionSys.SetSearchSize(110, 110);

	PostMessage(WM_COMMAND, MAKELONG(IDC_CB_AUTO_PAD_CENTERING, CBN_SELCHANGE), (LPARAM)GetDlgItem(IDC_CB_AUTO_PAD_CENTERING)->m_hWnd);

    return CDialog::OnInitDialog();
}

HBRUSH CTchBondPoints::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTchBondPoints::OnOK()
{
    Invalidate();

    switch (m_nTchMode)
    {
    case TCH_NEW_BOND_POINT:
        TeachNewBondPoint();
        break;
    case TCH_GROUP_BOND_POINT:
        TeachGroupBondPoint();
        break;
    case TCH_EDIT_BOND_POINT:
        EditBondPoint();
        break;
    case TCH_INSERT_BOND_POINT:
        InsertBondPoint();
        break;
    default:
        break;
    }

    UpdateData(FALSE);

    if ((m_nTchMode == TCH_NEW_BOND_POINT) ||
        (m_nTchMode == TCH_INSERT_BOND_POINT) ||
        (m_nTchMode == TCH_EDIT_BOND_POINT))
    {
        theVisionDisplay.SetDisplayMode(VISION_DISPLAY_TEACH_MODE);
        theVisionDisplay.SetDisplayWire(m_nTeachingPoint);
    }

    theVisionDisplay.UpdateVisionDisplay(true);
    Sleep_x1ms(50);

    CTeachRecipeSheet* psheet = (CTeachRecipeSheet*)GetParent();
    psheet->m_TchAdvWireLayoutPage.UpdateMenuItems();

    if (bErrorHandlerMode == true)return;

	if (((m_nTchMode == TCH_GROUP_BOND_POINT) && (m_nTchGroupStep == 3)) ||
		((m_nTchMode == TCH_INSERT_BOND_POINT) && (m_nCurBondPt == WIRE_BOND)) ||
		((m_nTchMode == TCH_EDIT_BOND_POINT) && (m_nTaughtPoints < m_nTeachingPoint)))
	{
		theVisionDisplay.SetDisplayMode(VISION_DISPLAY_NORMAL_MODE);
		theVisionDisplay.SetDisplayWire(m_nTeachingPoint);
		theVisionDisplay.UpdateVisionDisplay(true);

		Sleep_x1ms(50);

		DestroyWindow();
	}
}

// Teach new bond point
void CTchBondPoints::TeachNewBondPoint(void)
{
    short nPadType;
    long lVisionError = FOK;
    TBL_POS stPos, stSrchOffset;

    if (bErrorHandlerMode == false)
        theScanKeyThread.DisableJoyStick();
    else
    {
        theScanKeyThread.EnableJoyStick();
        theVisionSys.EnableVideo(true);
    }

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
    {
        theServo.GetTablePosition(&stPos);
        Sleep_x1ms(20);
    }
    else
    {
        if ((m_nBondPt == FIRST_BOND) ||
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
        {
            stPos.dx = -50 / theServo.GetResolXY();
            stPos.dy =  50 / theServo.GetResolXY();
        }
        else
        {
            stPos.dx = -950 / theServo.GetResolXY();
            stPos.dy =   60 / theServo.GetResolXY();
        }
    }

    if (bErrorHandlerMode == false)
    {
        lVisionError = SearchPadPattern(&stSrchOffset);

        if (lVisionError == -1)
        {
            nPadType = 0;
            lVisionError = FOK;
        }
        else if (lVisionError == FOK)
        {
            nPadType = m_iPadType;

            stPos.dx += stSrchOffset.dx;
            stPos.dy += stSrchOffset.dy;
            theServo.MoveTable2(stPos);	//v3.1.T200 edit
            Sleep_x1ms(20);
        }
        else
        {
            bErrorHandlerMode = true;

            SetDlgItemTextW(IDC_STATIC_MSG_1, theMsg.GetMsg(MSG_PAD_LOCATE_FAIL_SELECT_PAD_TYPE));
            SetDlgItemTextW(IDC_STATIC_MSG_2, theMsg.GetMsg(MSG_THEN_MANUAL_LOCATE_PAD));

            GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_SHOWNORMAL);
            GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_HIDE);
        }
    }
    else
    {
        bErrorHandlerMode = false;
        nPadType = m_iPadType;

        SetDlgItemTextW(IDC_STATIC_MSG_1,
				theMsg.GetMsg(m_nBondPt == FIRST_BOND ?
				MSG_MOVE_TO_1ST_BOND_PT : MSG_MOVE_TO_2ND_BOND_PT));
        SetDlgItemTextW(IDC_STATIC_MSG_2, _T(""));

        GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_SHOWNORMAL);
        GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_SHOWNORMAL);
    }

    if (lVisionError == FOK)
    {
        int iBondOn; //@1

        if ((m_nBondPt == FIRST_BOND) ||
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
            iBondOn = m_iBondOn1;
        else
            iBondOn = m_iBondOn2;

        if (m_lOperateMode == RECIPE_EDIT)
            theMatrix.ReverseTransformPos(iBondOn, &stPos);

        if ((m_nBondPt == FIRST_BOND) ||
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
            theWire.Append1stBondPoint(iBondOn, nPadType, &stPos);
        else
            theWire.Append2ndBondPoint(iBondOn, nPadType, &stPos);

        if (m_nBondPt == WIRE_BOND)
        {
            m_nCurBondPt++;

            if (m_nCurBondPt == WIRE_BOND) //@2
            {
				m_nTaughtPoints = m_lOperateMode == RECIPE_EDIT ? thePgm.GetUnitTotalWire() : theWire.Get1stBondTaughtPts();
                m_nTeachingPoint = m_nTaughtPoints + 1;
                
                m_nCurBondPt = FIRST_BOND;
            }

            theVisionDisplay.SetDisplayBondPt(m_nCurBondPt);

            WIRE_RECORD stWireRcd;
            TBL_POS st1stBondPt, st2ndBondPt;

            if (m_nCurBondPt != SECOND_BOND)
            {
                if (m_lOperateMode == RECIPE_EDIT)
                    theWire.CalcBondPoint(m_nTeachingPoint - 1, &st1stBondPt, &st2ndBondPt);
                else
                {
					theWire.GetRcd(&stWireRcd, m_nTeachingPoint - 1);

                    st1stBondPt = stWireRcd.stTchPos[0];
                    st2ndBondPt = stWireRcd.stTchPos[1];
                }
            }
            else if (m_nTeachingPoint > 1)
			{
				if (m_lOperateMode == RECIPE_EDIT)
					theWire.CalcBondPoint(m_nTeachingPoint - 1, &st1stBondPt, &st2ndBondPt);
				else
				{
					theWire.GetRcd(&stWireRcd, m_nTeachingPoint - 1);

					st1stBondPt = stWireRcd.stTchPos[0];
					st2ndBondPt = stWireRcd.stTchPos[1];
				}
			}

            if (m_nCurBondPt != SECOND_BOND)
                theServo.MoveTable2(st1stBondPt);	//v3.1.T200 edit
            else if (m_nTeachingPoint > 1)
                theServo.MoveTable2(st2ndBondPt);	//v3.1.T200 edit

            SetDlgItemTextW(IDC_STATIC_MSG_1,
				theMsg.GetMsg(m_nCurBondPt != SECOND_BOND ?
				MSG_MOVE_TO_1ST_BOND_PT : MSG_MOVE_TO_2ND_BOND_PT));
        }
        else //@2
        {
            if (m_lOperateMode == RECIPE_EDIT)
            {
                m_nTaughtPoints = thePgm.GetUnitTotalWire();
            }
            else
            {
				m_nTaughtPoints = m_nBondPt == FIRST_BOND ? theWire.Get1stBondTaughtPts() : theWire.Get2ndBondTaughtPts();
            }

            m_nTeachingPoint = m_nTaughtPoints + 1;
        }
    }

    theScanKeyThread.EnableJoyStick();
}

// Teach a group of bond points
void CTchBondPoints::TeachGroupBondPoint(void)
{
    short       nPadType;
    long        lVisionError = FOK, lStopSelected = SHOW_MSG_RESULT_OK;
    TBL_POS     stPos, stSrchOffset;

    if (bErrorHandlerMode == false)
        theScanKeyThread.DisableJoyStick();
    else
    {
        theScanKeyThread.EnableJoyStick();
        theVisionSys.EnableVideo(true);
    }

    if (m_nTchGroupStep == 0)
    {
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
            theServo.GetTablePosition(&stPos);
        else
        {
            if (m_nBondPt == FIRST_BOND)
            {
                stPos.dx = -50 / theServo.GetResolXY();
                stPos.dy =  50 / theServo.GetResolXY();
            }
            else
            {
                stPos.dx = -950 / theServo.GetResolXY();
                stPos.dy =   60 / theServo.GetResolXY();
            }
        }

        if (bErrorHandlerMode == false)
        {
            lVisionError = SearchPadPattern(&stSrchOffset);
            if (lVisionError == -1)
            {
                nPadType = 0;
                lVisionError = FOK;
            }
            else if (lVisionError == FOK)
            {
                nPadType = m_iPadType;

                stPos.dx += stSrchOffset.dx;
                stPos.dy += stSrchOffset.dy;
                theServo.MoveTable2(stPos);	//v3.1.T200 edit
            }
            else
            {
                bErrorHandlerMode = true;

                SetDlgItemTextW(IDC_STATIC_MSG_1, theMsg.GetMsg(MSG_PAD_LOCATE_FAIL_SELECT_PAD_TYPE));
                SetDlgItemTextW(IDC_STATIC_MSG_2, theMsg.GetMsg(MSG_THEN_MANUAL_LOCATE_PAD));

                GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_SHOWNORMAL);
                GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_HIDE);
            }
        }
        else
        {
            bErrorHandlerMode = false;
            nPadType = m_iPadType;

            SetDlgItemTextW(IDC_STATIC_MSG_1,
							theMsg.GetMsg(MSG_MOVE_TO_GROUP_2ND_PT));
            SetDlgItemTextW(IDC_STATIC_MSG_2, _T(""));

            GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_SHOWNORMAL);
            GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_SHOWNORMAL);
        }

        if (lVisionError == FOK)
        {
            stGroup1stPoint = stPos;
            m_nTchGroupStep++;
            SetDlgItemTextW(IDC_STATIC_MSG_1,
							theMsg.GetMsg(MSG_MOVE_TO_GROUP_2ND_PT));
        }
    }
    else if (m_nTchGroupStep == 1)
    {
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
            theServo.GetTablePosition(&stPos);
        else
        {
            if (m_nBondPt == FIRST_BOND)
            {
                stPos.dx = -50 / theServo.GetResolXY();
                stPos.dy = 150 / theServo.GetResolXY();
            }
            else
            {
                stPos.dx = -950 / theServo.GetResolXY();
                stPos.dy =  160 / theServo.GetResolXY();
            }
        }

        if (bErrorHandlerMode == false)
        {
            lVisionError = SearchPadPattern(&stSrchOffset);
            if (lVisionError == -1)
            {
                nPadType = 0;
                lVisionError = FOK;
            }
            else if (lVisionError == FOK)
            {
                nPadType = m_iPadType;

                stPos.dx += stSrchOffset.dx;
                stPos.dy += stSrchOffset.dy;
                theServo.MoveTable2(stPos);	//v3.1.T200 edit
            }
            else
            {
                bErrorHandlerMode = true;

                SetDlgItemTextW(IDC_STATIC_MSG_1, theMsg.GetMsg(MSG_PAD_LOCATE_FAIL_SELECT_PAD_TYPE));
                SetDlgItemTextW(IDC_STATIC_MSG_2, theMsg.GetMsg(MSG_THEN_MANUAL_LOCATE_PAD));

                GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_SHOWNORMAL);
                GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_HIDE);
            }
        }
        else
        {
            bErrorHandlerMode = false;
            nPadType = m_iPadType;

            SetDlgItemTextW(IDC_STATIC_MSG_1,
							theMsg.GetMsg(MSG_MOVE_TO_GROUP_LAST_PT));
            SetDlgItemTextW(IDC_STATIC_MSG_2, _T(""));

            GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_SHOWNORMAL);
            GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_SHOWNORMAL);
        }

        if (lVisionError == FOK)
        {
            stGroup2ndPoint = stPos;
            m_nTchGroupStep++;
            SetDlgItemTextW(IDC_STATIC_MSG_1,
							theMsg.GetMsg(MSG_MOVE_TO_GROUP_LAST_PT));
        }
    }
    else
    {
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
            theServo.GetTablePosition(&stPos);
        else
        {
            if (m_nBondPt == FIRST_BOND)
            {
                stPos.dx = -50 / theServo.GetResolXY();
                stPos.dy = 550 / theServo.GetResolXY();
            }
            else
            {
                stPos.dx = -950 / theServo.GetResolXY();
                stPos.dy =  560 / theServo.GetResolXY();
            }
        }

        if (bErrorHandlerMode == false)
        {
            lVisionError = SearchPadPattern(&stSrchOffset);
            if (lVisionError == -1)
            {
                nPadType = 0;
                lVisionError = FOK;
            }
            else if (lVisionError == FOK)
            {
                nPadType = m_iPadType;

                stPos.dx += stSrchOffset.dx;
                stPos.dy += stSrchOffset.dy;
                theServo.MoveTable2(stPos);	//v3.1.T200 edit
            }
            else
            {
                bErrorHandlerMode = true;

                SetDlgItemTextW(IDC_STATIC_MSG_1, theMsg.GetMsg(MSG_PAD_LOCATE_FAIL_SELECT_PAD_TYPE));
                SetDlgItemTextW(IDC_STATIC_MSG_2, theMsg.GetMsg(MSG_THEN_MANUAL_LOCATE_PAD));

                GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_SHOWNORMAL);
                GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_HIDE);
            }
        }
        else
        {
            bErrorHandlerMode = false;
            nPadType = m_iPadType;

            SetDlgItemTextW(IDC_STATIC_MSG_1,
							theMsg.GetMsg(MSG_MOVE_TO_GROUP_LAST_PT));
            SetDlgItemTextW(IDC_STATIC_MSG_2, _T(""));

            GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_SHOWNORMAL);
            GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_SHOWNORMAL);
        }

        if (lVisionError == FOK)
        {
            stGroupLastPoint = stPos;

			int iBondOn = m_nBondPt != SECOND_BOND ? m_iBondOn1 : m_iBondOn2;

            stPos = stGroup1stPoint;
            if (m_lOperateMode == RECIPE_EDIT)
                theMatrix.ReverseTransformPos(iBondOn, &stPos);

            if (m_nBondPt == FIRST_BOND)
                theWire.Append1stBondPoint(iBondOn, m_iPadType, &stPos);
            else
                theWire.Append2ndBondPoint(iBondOn, m_iPadType, &stPos);

            stPos = stGroup2ndPoint;
            if (m_lOperateMode == RECIPE_EDIT)
                theMatrix.ReverseTransformPos(iBondOn, &stPos);

            if (m_nBondPt == FIRST_BOND)
                theWire.Append1stBondPoint(iBondOn, m_iPadType, &stPos);
            else
                theWire.Append2ndBondPoint(iBondOn, m_iPadType, &stPos);

            short i, nTotalPoints;
            TBL_POS stBondPtOffset;

            stBondPtOffset.dx = stGroup2ndPoint.dx - stGroup1stPoint.dx;
            stBondPtOffset.dy = stGroup2ndPoint.dy - stGroup1stPoint.dy;
		   if(stBondPtOffset.dx == 0 || stBondPtOffset.dy == 0 ) //throw "w.temp.16" ;
		   {
			   AfxMessageBox(_T("temp.16, stBondPtOffset.x || stBondPtOffset.y = 0"));
			   return;
		   }
            if (fabs(stBondPtOffset.dx) > fabs(stBondPtOffset.dy))
                nTotalPoints = round((stGroupLastPoint.dx  - stGroup1stPoint.dx) / stBondPtOffset.dx) + 1;
            else
                nTotalPoints = round((stGroupLastPoint.dy  - stGroup1stPoint.dy) / stBondPtOffset.dy) + 1;

            for (i = 2; i < nTotalPoints - 1; i++)
            {
                stPos.dx = stGroup1stPoint.dx + stBondPtOffset.dx * i;
                stPos.dy = stGroup1stPoint.dy + stBondPtOffset.dy * i;
                theServo.MoveTable2(stPos);	//v3.1.T200 edit
                Sleep_x1ms(thePgm.GetRcd()->nVisionMoveDelay);

                lVisionError = SearchPadPattern(&stSrchOffset);
                if (lVisionError == -1)
                {
                    nPadType = 0;
                    lVisionError = FOK;
                }
                else if (lVisionError == FOK)
                {
                    nPadType = m_iPadType;

                    stPos.dx += stSrchOffset.dx;
                    stPos.dy += stSrchOffset.dy;
                    theServo.MoveTable2(stPos);	//v3.1.T200 edit
                }
                else
                {
					CStringList sList;
                    sList.AddTail(theMsg.GetMsg(MSG_PAD_LOCATE_FAIL));
                    sList.AddTail(theMsg.GetMsg(MSG_MANUAL_LOCATE_PAD));
                    sList.AddTail(theMsg.GetMsg(MSG_ENTER_TO_CONTINUE));
                    sList.AddTail(theMsg.GetMsg(MSG_ESC_TO_CANCEL));
                    lStopSelected = theMsg.ShowMsg(HELP_MSG, sList, true);
                    if (lStopSelected == SHOW_MSG_RESULT_OK)
                    {
						if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
                            theServo.GetTablePosition(&stPos);
                        else
                        {
                            if (m_nBondPt == FIRST_BOND)
                            {
                                stPos.dx = -50 / theServo.GetResolXY();
                                stPos.dy = 550 / theServo.GetResolXY();
                            }
                            else
                            {
                                stPos.dx = -950 / theServo.GetResolXY();
                                stPos.dy =  560 / theServo.GetResolXY();
                            }
                        }

                        lVisionError = FOK;
                    }
                }

                if (lVisionError == FOK)
                {
                    if (m_lOperateMode == RECIPE_EDIT)
                        theMatrix.ReverseTransformPos(iBondOn, &stPos); //@1
                    
                    if (m_nBondPt == FIRST_BOND)
                        theWire.Append1stBondPoint(iBondOn, m_iPadType, &stPos);
                    else
                        theWire.Append2ndBondPoint(iBondOn, m_iPadType, &stPos);
                }

                if (lStopSelected == SHOW_MSG_RESULT_STOP)
                    break;
            }

            if (lVisionError == FOK)
            {
                stPos = stGroupLastPoint;
                if (m_lOperateMode == RECIPE_EDIT)
                    theMatrix.ReverseTransformPos(iBondOn, &stPos); //@1

                if (m_nBondPt == FIRST_BOND)
                    theWire.Append1stBondPoint(iBondOn, m_iPadType, &stPos);
                else
                    theWire.Append2ndBondPoint(iBondOn, m_iPadType, &stPos);
            }

            m_nTchGroupStep++;

            if (m_lOperateMode == RECIPE_EDIT) //@2
            {
                m_nTaughtPoints = thePgm.GetUnitTotalWire();
            }
            else
            {
                if (m_nBondPt == FIRST_BOND)
                    m_nTaughtPoints = theWire.Get1stBondTaughtPts();
                else
                    m_nTaughtPoints = theWire.Get2ndBondTaughtPts();
            }

            m_nTeachingPoint = m_nTaughtPoints + 1;
        }
    }

    theScanKeyThread.EnableJoyStick();
}

// Edit bond point
void CTchBondPoints::EditBondPoint(void)
{
    short nPadType, i, nRow, nCol, nWireNum, nUnitWireNum, nUnitTotalWire, nUnitTotalDie;
    long lVisionError = FOK;
    TBL_POS stPos, stSrchOffset, stUnitOffset;

    if (bErrorHandlerMode == false)
        theScanKeyThread.DisableJoyStick();
    else
    {
        theScanKeyThread.EnableJoyStick();
        theVisionSys.EnableVideo(true);
    }

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
        theServo.GetTablePosition(&stPos);
    else
    {
        if ((m_nBondPt == FIRST_BOND) || //@1
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
        {
            stPos.dx = -50 / theServo.GetResolXY();
            stPos.dy =  50 / theServo.GetResolXY();
        }
        else
        {
            stPos.dx = -950 / theServo.GetResolXY();
            stPos.dy =   60 / theServo.GetResolXY();
        }
    }

    if (bErrorHandlerMode == false)
    {
        lVisionError = SearchPadPattern(&stSrchOffset);
        if (lVisionError == -1)
        {
            nPadType = 0;
            lVisionError = FOK;
        }
        else if (lVisionError == FOK)
        {
            nPadType = m_iPadType;

            stPos.dx += stSrchOffset.dx;
            stPos.dy += stSrchOffset.dy;
            theServo.MoveTable2(stPos);	//v3.1.T200 edit
        }
        else
        {
            bErrorHandlerMode = true;

            SetDlgItemText(IDC_STATIC_MSG_1, theMsg.GetMsg(MSG_PAD_LOCATE_FAIL_SELECT_PAD_TYPE));
            SetDlgItemText(IDC_STATIC_MSG_2, theMsg.GetMsg(MSG_THEN_MANUAL_LOCATE_PAD));

            GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_SHOWNORMAL);
            GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_HIDE);
        }
    }
    else
    {
        bErrorHandlerMode = false;
        nPadType = m_iPadType;

        SetDlgItemTextW(IDC_STATIC_MSG_1,
				theMsg.GetMsg(m_nBondPt == FIRST_BOND ?
				MSG_MOVE_TO_1ST_BOND_PT : MSG_MOVE_TO_2ND_BOND_PT));
        SetDlgItemText(IDC_STATIC_MSG_2, _T(""));

        GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_SHOWNORMAL);
        GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_SHOWNORMAL);
    }

    if (lVisionError == FOK)
    {
        int iBondOn;

		WIRE_RECORD stWireRcd;
		WIRE_RECORD stCurWireRcd;

		theWire.GetRcd(&stWireRcd, m_nTeachingPoint);

        if ((m_nBondPt == FIRST_BOND) ||
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
            iBondOn = m_iBondOn1;
        else
            iBondOn = m_iBondOn2;

        if (m_lOperateMode == RECIPE_EDIT)
            theMatrix.ReverseTransformPos(iBondOn, &stPos);
        
        if ((m_nBondPt == FIRST_BOND) ||
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
        {
            stWireRcd.stTchPos[0]  = stPos;
            stWireRcd.chPadType[0] = (char)nPadType;
            stWireRcd.nRefNum[0]   = iBondOn;
        }
        else
        {
            stWireRcd.stTchPos[1]  = stPos;
            stWireRcd.chPadType[1] = (char)nPadType;
            stWireRcd.nRefNum[1]   = iBondOn;
        }

        theWire.SetRcd(&stWireRcd, m_nTeachingPoint);
        
        if (m_lOperateMode == RECIPE_EDIT) //@2
        {
            //theWire.UpdateEditedWirePoint(m_nTeachingPoint);
            if ((thePgm.GetDeviceSeq() == PGM_HYBRID) || //@3
                (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD) ||
                (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD) ||
                (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID) || //@4
                (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
            {
                nUnitTotalDie = thePgm.GetUnitTotalDie();

                for (i = 1; i < thePgm.GetTotalUnit(); i++)
                {
                    thePgm.GetRowCol(i + 1, &nRow, &nCol);
                    thePgm.CalcUnitOffset(nRow, nCol, &stUnitOffset);

                    nUnitTotalWire = thePgm.GetUnitTotalWire();
					nUnitWireNum = nUnitTotalWire > 1 ? (m_nTeachingPoint - 1) % nUnitTotalWire + 1 : nUnitWireNum = 1;

                    nWireNum = nUnitWireNum + i * nUnitTotalWire;
                    theWire.GetRcd(&stCurWireRcd, nWireNum);

                    if (m_nCurBondPt == FIRST_BOND)
                    {
                        stCurWireRcd.stTchPos[0].dx = stWireRcd.stTchPos[0].dx + stUnitOffset.dx;
                        stCurWireRcd.stTchPos[0].dy = stWireRcd.stTchPos[0].dy + stUnitOffset.dy;
                        stCurWireRcd.chPadType[0] = (char)nPadType;

                        if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD) ||
                            (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
                        {
                            stCurWireRcd.nRefNum[0] = iBondOn + i * nUnitTotalDie;
                        }
                        else
                        {
							stCurWireRcd.nRefNum[0] = iBondOn == 0 ? iBondOn : iBondOn + i * nUnitTotalDie;
                        }
                    }
                    else
                    {
                        stCurWireRcd.stTchPos[1].dx = stWireRcd.stTchPos[1].dx + stUnitOffset.dx;
                        stCurWireRcd.stTchPos[1].dy = stWireRcd.stTchPos[1].dy + stUnitOffset.dy;
                        stCurWireRcd.chPadType[1] = (char)nPadType;

                        if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD) ||
                            (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
                        {
                            stCurWireRcd.nRefNum[1] = iBondOn + i * nUnitTotalDie;
                        }
                        else
                        {
							stCurWireRcd.nRefNum[1] = iBondOn == 0 ? iBondOn : iBondOn + i * nUnitTotalDie;
                        }
                    }

                    theWire.SetRcd(&stCurWireRcd, nWireNum);
                }
            }
        }

        if (m_nBondPt == WIRE_BOND) //@1
        {
            m_nCurBondPt++;

            if (m_nCurBondPt == WIRE_BOND)
            {
                m_nTeachingPoint++;
                m_nCurBondPt = FIRST_BOND;
            }
        }
        else
            m_nTeachingPoint++;

        theVisionDisplay.SetDisplayBondPt(m_nCurBondPt);

        TBL_POS st1stBondPt, st2ndBondPt;

        if (m_nTeachingPoint <= m_nTaughtPoints) //@2
        {
            if (m_nTeachingPoint > 0)
            {
                if (m_lOperateMode == RECIPE_EDIT)
                    theWire.CalcBondPoint(m_nTeachingPoint, &st1stBondPt, &st2ndBondPt);
                else
                {
					theWire.GetRcd(&stWireRcd, m_nTeachingPoint);

                    st1stBondPt = stWireRcd.stTchPos[0];
                    st2ndBondPt = stWireRcd.stTchPos[1];
                }
            }

            if (m_nCurBondPt != SECOND_BOND)
            {
                if (m_nTeachingPoint > 0)
                    theServo.MoveTable2(st1stBondPt);	//v3.1.T200 edit
            }
            else
            {
                if (m_nTeachingPoint > 0)
                    theServo.MoveTable2(st2ndBondPt);	//v3.1.T200 edit
            }

            SetDlgItemTextW(IDC_STATIC_MSG_1,
				theMsg.GetMsg(m_nCurBondPt != SECOND_BOND ?
				MSG_MOVE_TO_1ST_BOND_PT : MSG_MOVE_TO_2ND_BOND_PT));
        }
    }

    theScanKeyThread.EnableJoyStick();
}

// Insert bond point
void CTchBondPoints::InsertBondPoint(void)
{
    short nPadType;
    long lVisionError = FOK;

	TBL_POS stPos, stSrchOffset;

    if (bErrorHandlerMode == false)
        theScanKeyThread.DisableJoyStick();
    else
    {
        theScanKeyThread.EnableJoyStick();
        theVisionSys.EnableVideo(true);
    }

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
        theServo.GetTablePosition(&stPos);
    else
    {
        if ((m_nBondPt == FIRST_BOND) || //@1
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
        {
            stPos.dx = -50 / theServo.GetResolXY();
            stPos.dy =  50 / theServo.GetResolXY();
        }
        else
        {
            stPos.dx = -950 / theServo.GetResolXY();
            stPos.dy =   60 / theServo.GetResolXY();
        }
    }

    if (bErrorHandlerMode == false)
    {
        lVisionError = SearchPadPattern(&stSrchOffset);
        if (lVisionError == -1)
        {
            nPadType = 0;
            lVisionError = FOK;
        }
        else if (lVisionError == FOK)
        {
            nPadType = m_iPadType;

            stPos.dx += stSrchOffset.dx;
            stPos.dy += stSrchOffset.dy;
            theServo.MoveTable2(stPos);	//v3.1.T200 edit
        }
        else
        {
            bErrorHandlerMode = true;

            SetDlgItemText(IDC_STATIC_MSG_1, theMsg.GetMsg(MSG_PAD_LOCATE_FAIL_SELECT_PAD_TYPE));
            SetDlgItemText(IDC_STATIC_MSG_2, theMsg.GetMsg(MSG_THEN_MANUAL_LOCATE_PAD));

            GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_SHOWNORMAL);
            GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_HIDE);
        }
    }
    else
    {
        bErrorHandlerMode = false;
        nPadType = m_iPadType;

        SetDlgItemText(IDC_STATIC_MSG_1,
				theMsg.GetMsg(m_nBondPt == FIRST_BOND ?
				MSG_MOVE_TO_1ST_BOND_PT : MSG_MOVE_TO_2ND_BOND_PT));
        SetDlgItemText(IDC_STATIC_MSG_2, _T(""));

        GetDlgItem(IDC_COMBO_PAD_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TAUGHT)->ShowWindow(SW_SHOWNORMAL);
        GetDlgItem(IDC_STATIC_TAUGHT_PT)->ShowWindow(SW_SHOWNORMAL);
    }

    if (lVisionError == FOK)
    {
        int iBondOn;

		WIRE_RECORD stWireRcd;

        if ((m_nBondPt == FIRST_BOND) ||
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
            iBondOn = m_iBondOn1;
        else
            iBondOn = m_iBondOn2;

        if (m_lOperateMode == RECIPE_EDIT)
            theMatrix.ReverseTransformPos(iBondOn, &stPos);

        if ((m_nBondPt == FIRST_BOND) ||
            ((m_nBondPt == WIRE_BOND) && (m_nCurBondPt == FIRST_BOND)))
        {
            theWire.Insert1stBondPoint(m_nTeachingPoint, iBondOn, nPadType, &stPos);
        }
        else
        {
            theWire.GetRcd(&stWireRcd, m_nTeachingPoint);
            stWireRcd.nRefNum[1]   = iBondOn;
            stWireRcd.chPadType[1] = (char)nPadType;
            stWireRcd.stTchPos[1]  = stPos;
            theWire.SetRcd(&stWireRcd, m_nTeachingPoint);
        }

        if (m_nBondPt == WIRE_BOND)
        {
            m_nCurBondPt++;

            if (m_nCurBondPt == WIRE_BOND) //@2
            {
				m_nTaughtPoints = m_lOperateMode == RECIPE_EDIT ? thePgm.GetUnitTotalWire() : theWire.Get1stBondTaughtPts();
                m_nTeachingPoint++;
            }

            theVisionDisplay.SetDisplayBondPt(m_nCurBondPt);

            TBL_POS st1stBondPt, st2ndBondPt;

            if (m_nCurBondPt != SECOND_BOND)
            {
                theWire.GetRcd(&stWireRcd, m_nTeachingPoint);

                if (m_lOperateMode == RECIPE_EDIT)
                    theWire.CalcBondPoint(m_nTeachingPoint, &st1stBondPt, &st2ndBondPt);
                else
                {
                    st1stBondPt = stWireRcd.stTchPos[0];
                    st2ndBondPt = stWireRcd.stTchPos[1];
                }
            }
            else
            {
                theWire.GetRcd(&stWireRcd, m_nTeachingPoint - 1);

                if (m_lOperateMode == RECIPE_EDIT)
                    theWire.CalcBondPoint(m_nTeachingPoint - 1, &st1stBondPt, &st2ndBondPt);
                else
                {
                    st1stBondPt = stWireRcd.stTchPos[0];
                    st2ndBondPt = stWireRcd.stTchPos[1];
                }

                theWire.Insert2ndBondPoint(m_nTeachingPoint, stWireRcd.nRefNum[1], stWireRcd.chPadType[1], &st2ndBondPt);
            }

            if (m_nCurBondPt != SECOND_BOND)
                theServo.MoveTable2(st1stBondPt);	//v3.1.T200 edit
            else if (m_nTeachingPoint > 1)
                theServo.MoveTable2(st2ndBondPt);	//v3.1.T200 edit

            SetDlgItemText(IDC_STATIC_MSG_1,
				theMsg.GetMsg(m_nCurBondPt != SECOND_BOND ?
				MSG_MOVE_TO_1ST_BOND_PT : MSG_MOVE_TO_2ND_BOND_PT));
        }
        else
        {
            if (m_lOperateMode == RECIPE_EDIT)
                m_nTaughtPoints = thePgm.GetUnitTotalWire();
            else
				m_nTaughtPoints = m_nBondPt == FIRST_BOND ?
					theWire.Get1stBondTaughtPts() : theWire.Get2ndBondTaughtPts();

            m_nTeachingPoint++;
        }
    }

    theScanKeyThread.EnableJoyStick();
}

// Search pad pattern for current bond point
int CTchBondPoints::SearchPadPattern(TBL_POS* pstSrchOffset)
{
    short i, nTotalPads, anSrchPad[MAX_AUTO_PAD_TYPE];
    long lVisionError = FOK;
    TBL_POS stSrchRangeEnc, stSrchRangePr, stSrchOffset;

    PR_SRCH_RESULT  stSrchResult;

    *pstSrchOffset = stSrchOffset;

    if (m_nCurBondPt == 0)
    {
        if (m_iBondOn1 <= 0)
            return -1;
    }
    else if (m_iBondOn2 <= 0)
		return -1;

    nTotalPads = theAutoPadMgr.GetTotalAutoPad();
    if (nTotalPads <= 0)
        return -1;

    theVisionDisplay.EnableDisplay(false);
    Sleep_x1ms(20);

    for (i = 0; i < MAX_AUTO_PAD_TYPE; i++)
    {
        if (i < nTotalPads)
        {
            if (m_iPadType > 0)
            {
                if (i == 0)
                    anSrchPad[0] = m_iPadType;
                else if (i < m_iPadType)
                    anSrchPad[i] = i;
                else
                    anSrchPad[i] = i + 1;
            }
            else
            {
                anSrchPad[i] = i + 1;
            }
        }
        else
            anSrchPad[i] = 0;
    }

    theVisionSys.EnableVideo(false);

    AUTO_PAD_TYPE stPadRcd;

    for (i = 0; i < nTotalPads; i++)
    {
        if (anSrchPad[i] <= 0)continue;

		theAutoPadMgr.GetRcd(anSrchPad[i] - 1, &stPadRcd);

		theVisionSys.UpdateLightingParameter(stPadRcd.stPrLightPara.nCoaxRedLight, stPadRcd.stPrLightPara.bSideRedLight, stPadRcd.stPrLightPara.nCoaxBlueLight, stPadRcd.stPrLightPara.bSideBlueLight);
		Sleep_x1ms(20);

		theVisionSys.GrabImage();
		Sleep_x1ms(50);

		stSrchRangeEnc.dx = stPadRcd.nPadWidth * 1.5;
		stSrchRangeEnc.dy = stPadRcd.nPadHeight * 1.5;

		theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stSrchRangeEnc, &stSrchRangePr);

		lVisionError = theVisionSys.SearchVisionPattern(
			(short)stPadRcd.ulPrPatternID,
			round(stSrchRangePr.dx), round(stSrchRangePr.dy),
			thePgm.GetRcd()->nPrPassScore, &stSrchResult);
		Sleep_x1ms(50);

		if (lVisionError != FOK)continue;

		theVisionSys.VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stSrchOffset);

		*pstSrchOffset = stSrchOffset;
		m_iPadType = anSrchPad[i];

		break;
    }

    theVisionSys.EnableVideo(true);

    theVisionDisplay.EnableDisplay(true);
    Sleep_x1ms(40);

    return lVisionError;
}

void CTchBondPoints::OnComboPadType()
{
    UpdateData();

    if (m_iPadType == 0) return;
    if (m_iPadType > theAutoPadMgr.GetTotalAutoPad()) return;

	TBL_POS stPos, stVisionOffset;

	AUTO_PAD_TYPE stPadRcd;
	theAutoPadMgr.GetRcd(m_iPadType - 1, &stPadRcd);

	theVisionSys.UpdateLightingParameter(stPadRcd.stPrLightPara.nCoaxRedLight, stPadRcd.stPrLightPara.bSideRedLight, stPadRcd.stPrLightPara.nCoaxBlueLight, stPadRcd.stPrLightPara.bSideBlueLight);

	stPos.dx = stPadRcd.nPadWidth;
	stPos.dy = stPadRcd.nPadHeight;

	theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stPos, &stVisionOffset);

	theVisionSys.SetSearchSize(round(stVisionOffset.dx), round(stVisionOffset.dy)); //@4
}

void CTchBondPoints::PostNcDestroy()
{
    theVisionDisplay.SetDisplayBondPt(FIRST_BOND);
    theVisionDisplay.SetDisplayMode(VISION_DISPLAY_NORMAL_MODE);

    CDialog::PostNcDestroy();

    delete this;
}

void CTchBondPoints::OnDestroy()
{
	gbAPC = false;
	((CWireBondDlg*)theApp.m_pMainWnd)->SetDrawAreaEnable(NULL);

	if (m_lOperateMode == RECIPE_TEACH&&m_nTaughtPoints > 0)
	{
		GetParent()->GetDlgItem(ID_WIZNEXT)->EnableWindow();
	}

    CDialog::OnDestroy();
}

void CTchBondPoints::OnCancel()
{
    DestroyWindow();
}

void CTchBondPoints::SaveAutoPadCentering()
{
	PGM_RECORD *pstPgm = thePgm.GetRcd();

	CWnd* pWnd = GetDlgItem(IDC_CB_AUTO_PAD_CENTERING);
	pstPgm->bAutoPadCentering = (BYTE)((CComboBox*)pWnd)->GetCurSel();

	pstPgm->nGapFill = (BYTE)GetDlgItemInt(IDC_EDIT_GAP_FILL);

	pWnd = GetDlgItem(IDC_CB_BINARIZE);
	pstPgm->bBinarize = (BYTE)((CComboBox*)pWnd)->GetCurSel();

	pWnd = GetDlgItem(IDC_CB_CENTERING_TYPE);
	pstPgm->bCircleFit = (BYTE)((CComboBox*)pWnd)->GetCurSel();
}

void CTchBondPoints::OnAlgorithm()
{
	CWnd* pWnd = GetDlgItem(IDC_COMBO_ALGORITHM);
	gnAlgorithm = (BYTE)((CComboBox*)pWnd)->GetCurSel();

	BYTE bEnable = gnAlgorithm != ALGORITHM_BLOB;

	GetDlgItem(IDC_EDIT_GAP_FILL)->EnableWindow(bEnable);
	GetDlgItem(IDC_CB_BINARIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CB_CENTERING_TYPE)->EnableWindow(bEnable);
}

void CTchBondPoints::OnGapFill()
{
	GetDlgItem(IDC_STATIC_MSG_1)->SetFocus();

	long lGapFill=0;
	if(theKeyPad.GetUserKeyInData(&lGapFill, 3, 1, 6)==CANCEL_SELECTED)return;

	if(lGapFill<=0)return;
	
	SetDlgItemInt(IDC_EDIT_GAP_FILL, lGapFill);

	SaveAutoPadCentering();
}

void CTchBondPoints::OnAutoPadCentering()
{
	CWnd* pWnd=GetDlgItem(IDC_CB_AUTO_PAD_CENTERING);
	BOOL bEnable=((CComboBox*)pWnd)->GetCurSel();

	pWnd=GetDlgItem(IDC_COMBO_ALGORITHM);
	pWnd->EnableWindow(bEnable);

	if(bEnable)bEnable=((CComboBox*)pWnd)->GetCurSel()!=ALGORITHM_BLOB;

	GetDlgItem(IDC_EDIT_GAP_FILL)->EnableWindow(bEnable);
	GetDlgItem(IDC_CB_BINARIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CB_CENTERING_TYPE)->EnableWindow(bEnable);

	SaveAutoPadCentering();
}

void CTchBondPoints::OnFirstPoint()
{
	m_iBondOn1=((CComboBox*)GetDlgItem(IDC_COMBO_BOND_ON))->GetCurSel();

	REF_RECORD stRefRcd;
	theRef.GetRcd(&stRefRcd, m_iBondOn1);
	theVisionSys.UpdateLightingParameter(stRefRcd.stManuAlignLightPara[0].nCoaxRedLight, stRefRcd.stManuAlignLightPara[0].bSideRedLight, stRefRcd.stManuAlignLightPara[0].nCoaxBlueLight, stRefRcd.stManuAlignLightPara[0].bSideBlueLight);
}

void CTchBondPoints::OnSecondPoint()
{
	m_iBondOn2=((CComboBox*)GetDlgItem(IDC_COMBO_BOND_ON_2))->GetCurSel();

    REF_RECORD stRefRcd;
    theRef.GetRcd(&stRefRcd, m_iBondOn2);
    theVisionSys.UpdateLightingParameter(stRefRcd.stManuAlignLightPara[0].nCoaxRedLight, stRefRcd.stManuAlignLightPara[0].bSideRedLight, stRefRcd.stManuAlignLightPara[0].nCoaxBlueLight, stRefRcd.stManuAlignLightPara[0].bSideBlueLight);
}
