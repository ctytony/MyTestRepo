/***************************************************************
StickDetectDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "StickDetectDlg.h"
#include "MathCalc.h"
#include "User.h"

// CStickDetectDlg dialog
IMPLEMENT_DYNAMIC(CStickDetectDlg, CDialog)

CStickDetectDlg::CStickDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStickDetectDlg::IDD, pParent)
    , m_fApplyVoltage(0)
    , m_fDieThreshold(0)
    , m_fLeadThreshold(0)
    , m_nTailShortThreshold(0)
    , m_nNonStickThreshold(0)
    , m_nTailBreakNonstickThreshold(0)
    , m_iCoarseDetectionFlag(0)
    , m_nCoarseDetectSample(0)
{
}

CStickDetectDlg::~CStickDetectDlg()
{
}

void CStickDetectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_APPLY_VOLTAGE, m_fApplyVoltage);
    DDX_Text(pDX, IDC_EDIT_DIE_THRESHOLD, m_fDieThreshold);
    DDX_Text(pDX, IDC_EDIT_LEAD_THRESHOLD, m_fLeadThreshold);
    DDX_Text(pDX, IDC_EDIT_TAIL_SHORT_THRESHOLD, m_nTailShortThreshold);
    DDX_Text(pDX, IDC_EDIT_NONSTICK_THRESHOLD, m_nNonStickThreshold);
    DDX_Text(pDX, IDC_EDIT_TAIL_BREAK_THRESHOLD, m_nTailBreakNonstickThreshold);
    DDX_CBIndex(pDX, IDC_COMBO_COARSE_DETECT, m_iCoarseDetectionFlag);
    DDX_Text(pDX, IDC_EDIT_COARSE_DETECT_SAMPLE, m_nCoarseDetectSample);
}

BEGIN_MESSAGE_MAP(CStickDetectDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_APPLY_VOLTAGE, OnFocusApplyVoltage)
    ON_EN_SETFOCUS(IDC_EDIT_DIE_THRESHOLD, OnFocusDieThreshold)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_THRESHOLD, OnFocusLeadThreshold)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_SHORT_THRESHOLD, OnFocusTailShortThreshold)
    ON_EN_SETFOCUS(IDC_EDIT_NONSTICK_THRESHOLD, OnFocusNonstickThreshold)
    ON_EN_SETFOCUS(IDC_EDIT_TAIL_BREAK_THRESHOLD, OnFocusTailBreakThreshold)
    ON_CBN_SELCHANGE(IDC_COMBO_COARSE_DETECT, OnCoarseDetect)
    ON_EN_SETFOCUS(IDC_EDIT_COARSE_DETECT_SAMPLE, OnFocusCoarseDetectSample)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.4.W24 add
	ON_WM_DESTROY()		//V1.4.W24 add
END_MESSAGE_MAP()

// CStickDetectDlg message handlers
BOOL CStickDetectDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
    
	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    int iRowTop;

    SetDlgItemTextW(IDC_STATIC_HARDWARE_PARA,theMsg.GetMsg(MSG_HARDWARE_PARA));
    if (theUser.GetUserLevel() > USER_PROCESS)
    {
        iRowTop = 50;

        GetDlgItem(IDC_STATIC_HARDWARE_PARA)->ShowWindow(TRUE);
        GetDlgItem(IDC_STATIC_HARDWARE_PARA)->SetWindowPos(NULL, MENU_TREE_LEFT, 10, MENU_TREE_WIDTH - 25, 90, SWP_NOZORDER);

        GetDlgItem(IDC_STATIC_DIE_THRESHOLD)->SetWindowPos(NULL, PARA_LABEL_LEFT + 10, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        SetDlgItemTextW(IDC_STATIC_DIE_THRESHOLD,theMsg.GetMsg(MSG_DIE_DETECT_THRES_COLON));

        GetDlgItem(IDC_EDIT_DIE_THRESHOLD)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE + 20;
    }
    else
    {
        GetDlgItem(IDC_STATIC_HARDWARE_PARA)->ShowWindow(FALSE);

        GetDlgItem(IDC_STATIC_DIE_THRESHOLD)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_DIE_THRESHOLD)->ShowWindow(FALSE);

        iRowTop    = 10;
    }

    GetDlgItem(IDC_STATIC_2ND_STICK_PARA)->SetWindowPos(NULL, MENU_TREE_LEFT, iRowTop, MENU_TREE_WIDTH - 25, 250, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_2ND_STICK_PARA,theMsg.GetMsg(MSG_BOND_DETECT_PARA));    

    iRowTop += 40;

    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetWindowPos(NULL, PARA_LABEL_LEFT + 10, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_TAIL_SHORT_THRESHOLD,theMsg.GetMsg(MSG_TAIL_SHORT_THRES_COLON));

    GetDlgItem(IDC_EDIT_TAIL_SHORT_THRESHOLD)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    GetDlgItem(IDC_STATIC_NONSTICK_THRESHOLD)->SetWindowPos(NULL, PARA_LABEL_LEFT + 10, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_NONSTICK_THRESHOLD,theMsg.GetMsg(MSG_NONSTICK_THRES_COLON));

    GetDlgItem(IDC_EDIT_NONSTICK_THRESHOLD)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    GetDlgItem(IDC_STATIC_TAIL_BREAK_NONSTICK_THRESHOLD)->SetWindowPos(NULL, PARA_LABEL_LEFT + 10, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_TAIL_BREAK_NONSTICK_THRESHOLD,theMsg.GetMsg(MSG_TAIL_BREAK_THRES));

    GetDlgItem(IDC_EDIT_TAIL_BREAK_THRESHOLD)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    GetDlgItem(IDC_STATIC_COARSE_DETECT)->SetWindowPos(NULL, PARA_LABEL_LEFT + 10, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_COARSE_DETECT,theMsg.GetMsg(MSG_COARSE_DETECT));

    GetDlgItem(IDC_COMBO_COARSE_DETECT)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_COMBO_COARSE_DETECT)->EnableWindow(theUser.GetUserLevel() > USER_PROCESS);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    GetDlgItem(IDC_STATIC_COARSE_DETECT_SAMPLE)->SetWindowPos(NULL, PARA_LABEL_LEFT + 10, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_COARSE_DETECT_SAMPLE,theMsg.GetMsg(MSG_COARSE_DETECT_SAMPLE));

    GetDlgItem(IDC_EDIT_COARSE_DETECT_SAMPLE)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    GetDlgItem(IDC_EDIT_COARSE_DETECT_SAMPLE)->EnableWindow(theUser.GetUserLevel() > USER_PROCESS);

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

    m_fApplyVoltage               = pstMcData->fBsdApplyVoltage;
    m_fDieThreshold               = pstMcData->fDieShortThreshold;
    m_fLeadThreshold              = pstMcData->fLeadShortThreshold;
    m_nTailShortThreshold         = pstMcData->nTailShortSample;
    m_nNonStickThreshold          = pstMcData->nNonStickSample;

    m_nTailBreakNonstickThreshold = pstMcData->nTailBreakNonStickSample;

    m_fDieThreshold = (float)round(m_fDieThreshold * 10.0 / 255.0);

    m_iCoarseDetectionFlag        = pstMcData->bStickCoarseDetect;
    m_nCoarseDetectSample         = pstMcData->nStickCoarseDetectSamples;

    return CDialog::OnInitDialog();
}

HBRUSH CStickDetectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CStickDetectDlg::OnDestroy()
{
    UpdateData();

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	m_fDieThreshold = (BYTE)round((double)m_fDieThreshold * 255 / 10.0);

    pstMcData->fBsdApplyVoltage          = m_fApplyVoltage;
    pstMcData->fDieShortThreshold        = m_fDieThreshold;
    pstMcData->fLeadShortThreshold       = m_fLeadThreshold;
    pstMcData->nTailShortSample          = m_nTailShortThreshold;
    pstMcData->nNonStickSample           = m_nNonStickThreshold;
    pstMcData->nTailBreakNonStickSample  = m_nTailBreakNonstickThreshold;
    pstMcData->bStickCoarseDetect        = m_iCoarseDetectionFlag!=0;
    pstMcData->nStickCoarseDetectSamples = m_nCoarseDetectSample;

    CDialog::OnDestroy();
}

void CStickDetectDlg::OnFocusApplyVoltage()
{
    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetFocus();
    
    double dData = m_fApplyVoltage;
    theKeyPad.GetUserKeyInData(&dData,0, 5, 10);
    m_fApplyVoltage = (float)dData;

    UpdateData(false);
    UpdateData();
}

void CStickDetectDlg::OnFocusDieThreshold()
{
    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetFocus();
    
    double dData = m_fDieThreshold;
    theKeyPad.GetUserKeyInData(&dData, 5, 0, 10);
	m_fDieThreshold = (float)dData;

	UpdateData(false);
    UpdateData();
}

void CStickDetectDlg::OnFocusLeadThreshold()
{
    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetFocus();
    
    double dData = m_fLeadThreshold;
    theKeyPad.GetUserKeyInData(&dData,0, 0, 10);
    m_fLeadThreshold = (float)dData;

    UpdateData(false);
    UpdateData();
}

void CStickDetectDlg::OnFocusTailShortThreshold()
{
    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nTailShortThreshold,0,0,30);

    UpdateData(false);
}

void CStickDetectDlg::OnFocusNonstickThreshold()
{
    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nNonStickThreshold,25,0,100);

    UpdateData(false);
}

void CStickDetectDlg::OnFocusTailBreakThreshold()
{
    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nTailBreakNonstickThreshold,0,0,100);

    UpdateData(false);
}

void CStickDetectDlg::OnCoarseDetect()
{
    UpdateData();
}

void CStickDetectDlg::OnFocusCoarseDetectSample()
{
    GetDlgItem(IDC_STATIC_TAIL_SHORT_THRESHOLD)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nCoarseDetectSample, 3, 1, 50);

    UpdateData(false);
}

LRESULT CStickDetectDlg::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.4.W24 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}

void CStickDetectDlg::PostNcDestroy()	//V1.4.W24 add
{
	CDialog::PostNcDestroy();

	delete this;
}
