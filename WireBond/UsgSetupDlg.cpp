// UsgSetupDlg.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "UsgSetupDlg.h"
#include "MachineConfig.h"
#include "UsgInfoDispPage.h"
#include "FnkCaliPage.h"

// CUsgSetupDlg dialog

IMPLEMENT_DYNAMIC(CUsgSetupDlg, CDialog)

CUsgSetupDlg::CUsgSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsgSetupDlg::IDD, pParent)
    , m_iUsgLevel(0)
{
	m_hwndPage = NULL;	//V1.1.W144 add
}

CUsgSetupDlg::~CUsgSetupDlg()
{
}

void CUsgSetupDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_USG_LEVEL, m_iUsgLevel);
}

BEGIN_MESSAGE_MAP(CUsgSetupDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, OnCalibration)
    ON_CBN_SELCHANGE(IDC_COMBO_USG_LEVEL, OnUsgLevel)
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W144 add
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION2, &CUsgSetupDlg::OnBnClickedButtonCalibration2)
END_MESSAGE_MAP()

// CUsgSetupDlg message handlers
BOOL CUsgSetupDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetWindowPos(NULL, TREE_DLG_LEFT, TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_STATIC_PARAMETER,theMsg.GetMsg(MSG_PARAMETERS));
	SetDlgItemTextW(IDC_STATIC_USG_LEVEL,theMsg.GetMsg(MSG_USG_OUTPUT_LEVEL));
    SetDlgItemTextW(IDC_BUTTON_CALIBRATION,theMsg.GetMsg(MSG_CALIBRATE));
    SetDlgItemTextW(IDC_BUTTON_DONE,theMsg.GetMsg(MSG_DONE));	//v3.0.T451 add

    m_iUsgLevel = thePgm.GetRcd()->chUsgOutputLvl;

	//V1.1.W149 add - begin
	CComboBox* pCb=(CComboBox*)GetDlgItem(IDC_COMBO_USG_LEVEL);
	pCb->AddString(theMsg.GetMsg(MSG_HIGH));
	pCb->AddString(theMsg.GetMsg(MSG_LOW));
	pCb->AddString(theMsg.GetMsg(MSG_ULTRA_LOW));
	pCb->SetCurSel(m_iUsgLevel);
	//V1.1.W149 add - end

    return CDialog::OnInitDialog();
}

HBRUSH CUsgSetupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CUsgSetupDlg::OnCalibration()
{
	if (IsWindow(m_hwndPage)) return;

	CDialog *pDlg;
	if (theMcConfig.GetUsgType() == 1)	//v3.0.T345 add
	{
		pDlg = new CFnkCaliPage();
		pDlg->Create(CFnkCaliPage::IDD, this);
	}
	else
	{
		pDlg = new CUsgInfoDispPage();
		pDlg->Create(CUsgInfoDispPage::IDD, this);
	}
	m_hwndPage = pDlg->m_hWnd;
}

void CUsgSetupDlg::OnUsgLevel()
{
	UpdateData();
	thePgm.GetRcd()->chUsgOutputLvl = m_iUsgLevel;
}

LRESULT CUsgSetupDlg::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W144 add
{
	return (((m_hwndPage == NULL) || (IsWindow(m_hwndPage) == FALSE)) && DestroyWindow()) * DIALOG_CLOSED;
}

void CUsgSetupDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

void CUsgSetupDlg::OnBnClickedButtonCalibration2()
{
	CDialog::PostNcDestroy();
	delete this;
}
