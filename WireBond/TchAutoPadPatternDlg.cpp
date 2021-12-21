/***************************************************************
TchAutoPadPatternDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "TchAutoPadPatternDlg.h"
#include "VisionSystem.h"
#include "ScanKeyThread.h"
#include "LightingCtrlDlg.h"
#include "WireBondDlg.h"
#include "AdvWireLayoutPage.h"
#include "servo.h"

// CTchAutoPadPatternDlg dialog
IMPLEMENT_DYNAMIC(CTchAutoPadPatternDlg, CDialog)

CTchAutoPadPatternDlg::CTchAutoPadPatternDlg(char chTchMode, short nPadId, CWnd* pParent /*=NULL*/)
	: CDialog(CTchAutoPadPatternDlg::IDD, pParent)
    , m_nTeachStep(0)
    , m_chTchMode(chTchMode)
    , m_nEditPadId(nPadId)
{
}

CTchAutoPadPatternDlg::~CTchAutoPadPatternDlg()
{
}

void CTchAutoPadPatternDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTchAutoPadPatternDlg, CDialog)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CTchAutoPadPatternDlg message handlers
BOOL CTchAutoPadPatternDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    ModifyStyle(WS_CAPTION, 0);

    CRect Rect;
    GetParent()->GetParent()->GetWindowRect(&Rect);

    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    font.CreateFontW(38, 0, 0, 0, 500, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	GetDlgItem(IDC_STATIC_MSG_0)->SetFont(&font);

    SetDlgItemTextW(IDOK,theMsg.GetMsg(MSG_ENTER));
    SetDlgItemTextW(IDCANCEL,theMsg.GetMsg(MSG_ESCAPE));
	SetDlgItemTextW(IDC_STATIC_MSG_0,m_chTchMode == AUTO_PAD_TEACH ? theMsg.GetMsg(MSG_TCH_PAD_SIZE) : theMsg.GetMsg(MSG_EDIT_PAD_SIZE));
    SetDlgItemTextW(IDC_STATIC_MSG_1,theMsg.GetMsg(MSG_STEP1));
    SetDlgItemTextW(IDC_STATIC_MSG_2,theMsg.GetMsg(MSG_MOVE_TO_TOP_LEFT_PAD_PT));
    SetDlgItemTextW(IDC_STATIC_MSG_3,theMsg.GetMsg(MSG_ENTER_TO_CONTINUE));
    SetDlgItemTextW(IDC_STATIC_MSG_4,theMsg.GetMsg(MSG_ESC_TO_CANCEL));

    m_nTeachStep = 0;
	ZeroMemory(&m_stCurPadRcd,sizeof(AUTO_PAD_TYPE));

    if (m_chTchMode == AUTO_PAD_EDIT)
    {
        theVisionDisplay.EnableDisplay(false);

        theVisionSys.ClearDrawing();
    }

    return CDialog::OnInitDialog();
}

HBRUSH CTchAutoPadPatternDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTchAutoPadPatternDlg::OnOK()
{
    short nPatRcd;
    TBL_POS stPos, stVisionOffset;
    short nWidth, nHeight, nSideRedLight, nSideBlueLight;

    Invalidate();

    switch (m_nTeachStep)
    {
    case 0:
        if (!theMcConfig.IsBypassAll())
            theServo.GetTablePosition(&(m_astPadCornerPos[0]));
        else
        {
            m_astPadCornerPos[0].dx = -50 / theServo.GetResolXY();
            m_astPadCornerPos[0].dy =  50 / theServo.GetResolXY();
        }

        SetDlgItemTextW(IDC_STATIC_MSG_1,theMsg.GetMsg(MSG_STEP2));
        SetDlgItemTextW(IDC_STATIC_MSG_2,theMsg.GetMsg(MSG_MOVE_TO_TOP_RIGHT_PAD_PT));

        m_nTeachStep++;

        UpdateData(FALSE);

        break;

    case 1:
        if (!theMcConfig.IsBypassAll())
            theServo.GetTablePosition(&(m_astPadCornerPos[1]));
        else
        {
            m_astPadCornerPos[1].dx = 50 / theServo.GetResolXY();
            m_astPadCornerPos[1].dy = 50 / theServo.GetResolXY();
        }

        if (m_astPadCornerPos[0].dx - m_astPadCornerPos[1].dx < 70) //@2
        {
            break;
        }

        SetDlgItemTextW(IDC_STATIC_MSG_1,theMsg.GetMsg(MSG_STEP3));
        SetDlgItemTextW(IDC_STATIC_MSG_2,theMsg.GetMsg(MSG_MOVE_TO_BOTTOM_RIGHT_PAD_PT));

        m_nTeachStep++;

        UpdateData(FALSE);

        break;

    case 2:
    {
        if (!theMcConfig.IsBypassAll())
            theServo.GetTablePosition(&(m_astPadCornerPos[2]));
        else
        {
            m_astPadCornerPos[2].dx =  50 / theServo.GetResolXY();
            m_astPadCornerPos[2].dy = -50 / theServo.GetResolXY();
        }

        if (m_astPadCornerPos[1].dy - m_astPadCornerPos[2].dy < 70) //@2
        {
            break;
        }

        CalculatePadSize(&nWidth, &nHeight);
        m_stCurPadRcd.nPadWidth  = nWidth;
        m_stCurPadRcd.nPadHeight = nHeight;

        theScanKeyThread.DisableJoyStick();

		stPos.dx = (m_astPadCornerPos[0].dx + m_astPadCornerPos[1].dx) / 2.0;
        stPos.dy = (m_astPadCornerPos[1].dy + m_astPadCornerPos[2].dy) / 2.0;
        theServo.MoveTable2(stPos);	//v3.1.T212 edit

        stPos.dx = nWidth;
        stPos.dy = nHeight;
        theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stPos, &stVisionOffset);
        theVisionSys.SetSearchSize(round(stVisionOffset.dx), round(stVisionOffset.dy)); //@4
        
        SetDlgItemTextW(IDC_STATIC_MSG_1,theMsg.GetMsg(MSG_STEP4));
        SetDlgItemTextW(IDC_STATIC_MSG_2,theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));

        m_nTeachStep++;

        UpdateData(FALSE);

        break;
    }
    case 3:
        theVisionSys.EnableVideo(false);
        theVisionSys.GrabImage();

        stPos.dx = m_stCurPadRcd.nPadWidth;
        stPos.dy = m_stCurPadRcd.nPadHeight;
        theVisionSys.TablePosToVisionOffset(VISION_LOW_MAG, &stPos, &stVisionOffset);

        if (theVisionSys.LearnVisionPattern(false, round(stVisionOffset.dx), round(stVisionOffset.dy), &nPatRcd) == FOK)
        {
            m_stCurPadRcd.ulPrPatternID = nPatRcd;
            m_stCurPadRcd.bPadTypeValid = true;
            m_stCurPadRcd.chPadShape    = PAD_PATTERN_RECTANGLE;

            theVisionSys.GetLightingParameter(&(m_stCurPadRcd.stPrLightPara.nCoaxRedLight), &nSideRedLight, &(m_stCurPadRcd.stPrLightPara.nCoaxBlueLight), &nSideBlueLight);
            m_stCurPadRcd.stPrLightPara.bSideRedLight  = (byte)nSideRedLight;
            m_stCurPadRcd.stPrLightPara.bSideBlueLight = (byte)nSideBlueLight;

            theScanKeyThread.EnableJoyStick();
            theVisionSys.EnableVideo(true);
            
            if (m_chTchMode == AUTO_PAD_TEACH)
            {
                theAutoPadMgr.IncTotalAutoPad();
                theAutoPadMgr.SetRcd(theAutoPadMgr.GetTotalAutoPad() - 1, &m_stCurPadRcd);
            }
            else
            {
                AUTO_PAD_TYPE stAutoPadRcd;
                theAutoPadMgr.GetRcd(m_nEditPadId, &stAutoPadRcd);
                theVisionSys.ClearPatternRcd(
							(short)stAutoPadRcd.ulPrPatternID);
                theAutoPadMgr.SetRcd(m_nEditPadId, &m_stCurPadRcd);
            }
        }

        ((CTeachRecipeSheet*)GetParent())->m_TchAdvWireLayoutPage.UpdateMenuItems();

        DestroyWindow();

        break;

    default:

        break;
    }
}

void CTchAutoPadPatternDlg::OnCancel()
{
    DestroyWindow();
}

// Calculate pad width and height
void CTchAutoPadPatternDlg::CalculatePadSize(short* pnWidth, short* pnHeight)
{
    *pnWidth  = round(fabs(m_astPadCornerPos[0].dx - m_astPadCornerPos[1].dx));
    *pnHeight = round(fabs(m_astPadCornerPos[1].dy - m_astPadCornerPos[2].dy));
}

void CTchAutoPadPatternDlg::PostNcDestroy()
{
    theScanKeyThread.EnableJoyStick();
    theVisionSys.EnableVideo(true);
    theVisionDisplay.EnableDisplay(true); //@1

    CDialog::PostNcDestroy();

    delete this;
}
