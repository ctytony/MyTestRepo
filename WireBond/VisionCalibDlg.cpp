/***************************************************************
VisionCalibDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "Math.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "MenuConfig.h"
#include "VisionCalibDlg.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "VisionSystem.h"
#include "servo.h"
#include "Debug.h"
#include <memory>
#include "Busy.h"	//V1.1.W142 add

// CVisionCalibDlg dialog
IMPLEMENT_DYNAMIC(CVisionCalibDlg, CDialog)

CVisionCalibDlg::CVisionCalibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisionCalibDlg::IDD, pParent)
	, m_fHighMagFactorX((float)1.01)
	, m_fHighMagFactorY((float)1.01)
	, m_fLowMagFactorX((float)1.01)
	, m_fLowMagFactorY((float)1.01)
	, m_iCalibMag(0)
    , m_nPatternSizeX(168)	//V1.1.W132 edit
    , m_nPatternSizeY(168)	//V1.1.W132 edit
{
	m_bBusy=FALSE;	//V1.1.W142 add
}

CVisionCalibDlg::~CVisionCalibDlg()
{
}

void CVisionCalibDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_X_LOW, m_fLowMagFactorX);
    DDX_Text(pDX, IDC_EDIT_Y_LOW, m_fLowMagFactorY);
    DDX_CBIndex(pDX, IDC_COMBO_MAG, m_iCalibMag);
    DDV_MinMaxInt(pDX, m_iCalibMag, 0, 1);
    DDX_Text(pDX, IDC_EDIT_SIZE_X, m_nPatternSizeX);
    DDV_MinMaxShort(pDX, m_nPatternSizeX, 0, 511);
    DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_nPatternSizeY);
	DDV_MinMaxShort(pDX, m_nPatternSizeY, 0, 383);
}

BEGIN_MESSAGE_MAP(CVisionCalibDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATE, OnCalibrate)
	ON_CBN_SELCHANGE(IDC_COMBO_MAG, OnMag)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_SIZE_X, OnSizeX)
	ON_EN_CHANGE(IDC_EDIT_SIZE_Y, OnSizeY)
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W142 add
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CVisionCalibDlg::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CVisionCalibDlg message handlers
BOOL CVisionCalibDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	double dHighMagFactX, dHighMagFactY, dLowMagFactX, dLowMagFactY, dFactCos, dFactSin;
	theVisionSys.GetVisionFactor(&dHighMagFactX, &dHighMagFactY, &dLowMagFactX, &dLowMagFactY, &dFactCos, &dFactSin); //@4
	m_fHighMagFactorX = (float)dHighMagFactX;
	m_fHighMagFactorY = (float)dHighMagFactY;
	m_fLowMagFactorX  = (float)dLowMagFactX;
	m_fLowMagFactorY  = (float)dLowMagFactY;

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_BUTTON_CALIBRATE, theMsg.GetMsg(MSG_CALIBRATE));
	SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));
	SetDlgItemTextW(IDC_STATIC_FACTOR, theMsg.GetMsg(MSG_VISION_FACTOR));
	SetDlgItemTextW(IDC_STATIC_SETTING, theMsg.GetMsg(MSG_SETTING));
	SetDlgItemTextW(IDC_STATIC_MAG, theMsg.GetMsg(MSG_MAGNIFICATION));
	SetDlgItemTextW(IDC_STATIC_SIZE_X, theMsg.GetMsg(MSG_PATTERN_SIZE_X));
	SetDlgItemTextW(IDC_STATIC_SIZE_Y, theMsg.GetMsg(MSG_PATTERN_SIZE_Y));

    CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SIZE_X);
    pSpin->SetBuddy(GetDlgItem(IDC_EDIT_SIZE_X));
    pSpin->SetRange(0, 256);

    pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SIZE_Y);
    pSpin->SetBuddy(GetDlgItem(IDC_EDIT_SIZE_Y));
    pSpin->SetRange(0, 192);

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();

    pListBox->InsertString(0,
		    theMsg.GetMsg(MSG_MOVE_TO_UNIQUE_PATTERN_PT));
    pListBox->InsertString(1, theMsg.GetMsg(MSG_MAKE_SURE_LIGHT_OK));
    pListBox->InsertString(2,
			theMsg.GetMsg(MSG_CALI_TO_LEARN_VISION_FACTOR));

	return CDialog::OnInitDialog();
}

int CVisionCalibDlg::CalcVisionFactor(PR_SRCH_RESULT* pstSrchResult, double dSrchStep, TBL_POS* pstVisionFactor, double* pFactCos, double* pFactSin) //@4
{
    if ((fabs(pstSrchResult[0].dOffsetX) < 0.001)
		|| (fabs(pstSrchResult[2].dOffsetY) < 0.001))
    {
		short anMsgArray[1] = {MSG_VISION_SRCH_OFFSET_ZERO};
        theMsg.ShowMsg(ERROR_MSG, 1, anMsgArray);
        return FNOK;
    }

	if (pstSrchResult[0].dOffsetX == 0)
	{
		AfxMessageBox(_T("dOffsetX = 0"));
		return 0;
	}

    double dAngle = atan(pstSrchResult[0].dOffsetY / pstSrchResult[0].dOffsetX);
    double dCosAngle = cos(dAngle);

    if (dCosAngle < 0.9)
    {
		short anMsgArray[2] = {MSG_ROTATE_ANGLE_TOO_LARGE, MSG_ENSURE_VISION_NOT_EXPIRED};
        theMsg.ShowMsg(ERROR_MSG, 2, anMsgArray);
        return FNOK;
    }

	if (pstSrchResult[2].dOffsetY == 0)
	{
		AfxMessageBox(_T("dOffsetY = 0"));
		return 0;
	}

    dAngle = atan(pstSrchResult[2].dOffsetX / pstSrchResult[2].dOffsetY);
    dCosAngle = cos(dAngle);

    if (dCosAngle < 0.9)
    {
		short anMsgArray[1] = {MSG_ROTATE_ANGLE_TOO_LARGE};
        theMsg.ShowMsg(ERROR_MSG, 1, anMsgArray);
        return FNOK;
    }

	if (pstSrchResult[0].dOffsetX == 0)
	{
		AfxMessageBox(_T("dOffsetX = 0"));
		return 0;
	}

    dAngle = atan(pstSrchResult[2].dOffsetX / pstSrchResult[0].dOffsetX);
    *pFactCos = cos(dAngle);
    *pFactSin = sin(dAngle);

	if ((pstSrchResult[0].dOffsetX == 0) || (pstSrchResult[2].dOffsetY == 0))
	{
		AfxMessageBox(_T("dOffsetX = 0 || dOffsetY = 0"));
		return 0;
	}

    pstVisionFactor->dx = dSrchStep * cos(dAngle) / pstSrchResult[0].dOffsetX;
    pstVisionFactor->dy = dSrchStep * cos(dAngle) / pstSrchResult[2].dOffsetY;

    return FOK;
}

void CVisionCalibDlg::OnCalibrate()
{
	CBusy bs(&m_bBusy);	//V1.1.W142 add

	short nPatRcd;
    CString str;
    CStringList sList;

	TBL_POS stCurTblPos, stStepPos, stTblPos, stTempFactor(m_fLowMagFactorX, m_fLowMagFactorY);
    double dFactCos, dFactSin;
    PR_SRCH_RESULT stSrchResult[4];

    OnSizeX();

    theVisionSys.EnableVideo(false);
    theVisionSys.GrabImage();

    theDebug.ClearInfo();	//v3.0.T96 edit

    int lErrorStatus = theVisionSys.LearnVisionPattern(false,
					m_nPatternSizeX, m_nPatternSizeY, &nPatRcd);
	if (lErrorStatus == FOK)
    {
        lErrorStatus = theVisionSys.SearchVisionPattern(nPatRcd, VISION_MAX_SEARCH_RANGE, thePgm.GetLeadPrPassScore(), &(stSrchResult[0]));
        str.Format(_T("PrOS_%10.1f_%10.1f"),
							stSrchResult[0].dOffsetX,
							stSrchResult[0].dOffsetY);
		theDebug.AddInfo(str);

        theVisionSys.EnableVideo(true);

        theServo.GetTablePosition(&stCurTblPos);
		str.Format(_T("Pos_%10.1f_%10.1f"),
							stCurTblPos.dx, stCurTblPos.dy);
		theDebug.AddInfo(str);

        for (short i = 0; i < 4; i++)
		{
            if (i == 0)
            {
                stStepPos.dx =  VISION_CALIB_STEP;
                stStepPos.dy =  0;
            }
            else if (i == 1)
            {
                stStepPos.dx = -VISION_CALIB_STEP;
                stStepPos.dy =  0;
            }
            else if (i == 2)
            {
                stStepPos.dx =  0;
                stStepPos.dy =  VISION_CALIB_STEP;
            }
            else if (i == 3)
            {
                stStepPos.dx =  0;
                stStepPos.dy = -VISION_CALIB_STEP;
            }

            stTblPos.dx = stCurTblPos.dx + stStepPos.dx;
            stTblPos.dy = stCurTblPos.dy + stStepPos.dy;
            theServo.MoveTable2(stTblPos);	//v3.1.T206 edit
			Sleep_x1ms(500);

            str.Format(_T("%d_Pos_%10.1f_%10.1f"),
							i+1, stTblPos.dx, stTblPos.dy);
			theDebug.AddInfo(str);

            theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();
            theVisionSys.SetPassScore(nPatRcd, thePgm.GetLeadPrPassScore());
            lErrorStatus = theVisionSys.SearchVisionPattern(nPatRcd, VISION_MAX_SEARCH_RANGE, thePgm.GetLeadPrPassScore(), &(stSrchResult[i]));
            if (lErrorStatus != FOK) break;

			str.Format(_T("%d_PrOS_%10.1f_%10.1f"),
								i+1, stSrchResult[i].dOffsetX,
								stSrchResult[i].dOffsetY);
			theDebug.AddInfo(str);

			theVisionSys.EnableVideo(true);
        }

        if (CalcVisionFactor(stSrchResult, VISION_CALIB_STEP, &stTempFactor, &dFactCos, &dFactSin) != FOK)
        {
            theVisionSys.EnableVideo(true);
            theVisionSys.ClearPatternRcd(nPatRcd);
            return;
        }

	    if ((stTempFactor.dx == 0) || (stTempFactor.dy == 0))
		{
			AfxMessageBox(_T("stTempFactor.x = 0 || stTempFactor.y = 0"));
			return;
		}

        double dExpectDist = fabs(VISION_CALIB_STEP / stTempFactor.dx);
        if (fabs(dExpectDist - fabs(stSrchResult[1].dOffsetX)) > VISION_CALIB_TOL)
        {
			short anMsgArray[1] = {MSG_VISION_FACTOR_CALI_FAIL};
            theMsg.ShowMsg(ERROR_MSG, 1, anMsgArray);
        }

        dExpectDist = fabs(VISION_CALIB_STEP / stTempFactor.dy);
        if (fabs(dExpectDist - fabs(stSrchResult[3].dOffsetY)) > VISION_CALIB_TOL)
        {
			short anMsgArray[1] = {MSG_VISION_FACTOR_CALI_FAIL};
            theMsg.ShowMsg(ERROR_MSG, 1, anMsgArray);
        }

        theVisionSys.ClearPatternRcd(nPatRcd);
        theServo.MoveTable2(stCurTblPos);	//v3.1.T206 edit
    }
    else
    {
        sList.RemoveAll();
        str.Format(_T("Teach Error %d"), lErrorStatus);
        sList.AddTail(str);
        theMsg.ShowMsg(ERROR_MSG, sList);
    }

    theVisionSys.EnableVideo(true);
    
    if (lErrorStatus != FOK)
    {
        sList.RemoveAll();
        str.Format(_T("Pattern %d, Error %d"), nPatRcd, lErrorStatus);
        sList.AddTail(str);
        theMsg.ShowMsg(ERROR_MSG, sList);
        return;
    }

    if (m_iCalibMag == VISION_LOW_MAG)
    {
        m_fLowMagFactorX = (float)stTempFactor.dx;
        m_fLowMagFactorY = (float)stTempFactor.dy;
    }
    else
    {
        m_fHighMagFactorX = (float)stTempFactor.dx;
        m_fHighMagFactorY = (float)stTempFactor.dy;
    }
    
	UpdateData(FALSE);
	theVisionSys.UpdateVisionFactor(m_iCalibMag, stTempFactor.dx, stTempFactor.dy, dFactCos, dFactSin);
}

void CVisionCalibDlg::OnMag()
{
    theVisionSys.ChangeLensMag(m_iCalibMag);
}

HBRUSH CVisionCalibDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC) || (nCtlColor == CTLCOLOR_BTN))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }
    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CVisionCalibDlg::OnSizeX()
{
    UpdateData(true);
    theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
}

void CVisionCalibDlg::OnSizeY()
{
    UpdateData(true);
    theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
}

void CVisionCalibDlg::OnDestroy()	//V1.1.W142 add
{
	CDialog::OnDestroy();
	theVisionSys.ClearDrawing();
}

void CVisionCalibDlg::PostNcDestroy()
{
    CDialog::PostNcDestroy();
    delete this;
}

LRESULT CVisionCalibDlg::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W144 edit
{
	return ((m_bBusy == FALSE) && DestroyWindow()) * DIALOG_CLOSED;
}

void CVisionCalibDlg::OnBnClickedButtonDone()
{
    CDialog::PostNcDestroy();
    delete this;
}
