// CR2RWhSetupPage dialog

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MachineData.h"
#include "R2RWhSetUpPage.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MenuConfig.h"
#include "PopUpCuEliteStepperCtr.h"
#include "MathCalc.h"
#include "MaterialHandlerThread.h"
#include "PopUpTopStepperCtr.h"
#include "R2RIndex.h"
#include "servo.h"

// CR2RWhSetupPage dialog

IMPLEMENT_DYNAMIC(CR2RWhSetupPage, CPropertyPage)

CR2RWhSetupPage::CR2RWhSetupPage()
:CPropertyPage(CR2RWhSetupPage::IDD)  
	, m_lIndexLogicZero(0)
	, m_lIndexRightLimit(0)
	, m_dWhTbX(0)
	, m_dWhTbY(0)
{

}

CR2RWhSetupPage::~CR2RWhSetupPage()
{
}

void CR2RWhSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INDEX_LOGIC_ZERO, m_lIndexLogicZero);
	DDX_Text(pDX, IDC_EDIT_INDEX_RIGHT_LIMIT, m_lIndexRightLimit);
	DDX_Text(pDX, IDC_EDIT_WH_TB_X, m_dWhTbX);
	DDX_Text(pDX, IDC_EDIT_WH_TB_Y, m_dWhTbY);




}


BEGIN_MESSAGE_MAP(CR2RWhSetupPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CR2RWhSetupPage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_START, &CR2RWhSetupPage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_START_TCH_TABLE, &CR2RWhSetupPage::OnBnClickedButtonStartTchTable)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CR2RWhSetupPage::OnBnClickedButtonNext)
    ON_EN_CHANGE(IDC_EDIT_MAGCLAMPER_LIMIT, &CR2RWhSetupPage::OnEnChangeEditMagClamperLimit)
END_MESSAGE_MAP()

// CR2RWhSetupPage message handlers
BOOL CR2RWhSetupPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	((CWireBondDlg*)GetParent())->InhibitSystemButton();

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);

	CPoint pt(Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP);
    SetWindowPos(NULL, pt.x, pt.y, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	pt.x = TREE_DONE_BUTTON_LEFT;
	pt.y = TREE_BUTTON_TOP;

	CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_DONE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

	pWnd = GetDlgItem(IDC_BUTTON_START);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));

	pWnd = GetDlgItem(IDC_BUTTON_START_TCH_TABLE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));

	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT));
    pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_STATIC_INDEXER_ALIGN);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEXER_ALIGN));

	pWnd = GetDlgItem(IDC_STATIC_INDEXER_LOGIC_0);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEXER_LOGIC_ZERO));

	pWnd = GetDlgItem(IDC_STATIC_INDEXER_RIGHT_LIMIT);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEXER_RIGHT_LIMIT));

	pWnd = GetDlgItem(IDC_STATIC_TABLE_SAFE_POS);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TCH_SAFE_POS));

	pWnd = GetDlgItem(IDC_STATIC_TABLE_X);
	pWnd->SetWindowTextW(_T("X : "));

	pWnd = GetDlgItem(IDC_STATIC_TABLE_Y);
	pWnd->SetWindowTextW(_T("Y : "));

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	m_lIndexLogicZero  = pstMcData->lZeroOffset;
	m_lIndexRightLimit = pstMcData->lIndexerLimit; 
	m_dWhTbX = pstMcData->stTbSafeStandby.dx;
	m_dWhTbY = pstMcData->stTbSafeStandby.dy;

	pWnd = GetDlgItem(IDC_STATIC_INFO_SAFE);
	pWnd->SetWindowTextW(_T(""));

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CR2RWhSetupPage::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();

	delete this;
}

HBRUSH CR2RWhSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
       pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CR2RWhSetupPage::OnDestroy()
{
	
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CR2RWhSetupPage::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	DestroyWindow();
}

void CR2RWhSetupPage::OnBnClickedOk()
{
	MACHINE_DATA* pstMcData = theMcData.GetRcd();

    pstMcData->lZeroOffset = m_lIndexLogicZero;
	pstMcData->lIndexerLimit = m_lIndexRightLimit;

	pstMcData->stTbSafeStandby.dx = m_dWhTbX;
	pstMcData->stTbSafeStandby.dy = m_dWhTbY;

	((CWireBondDlg*)(this->GetParent()))->EnableSystemButton();	

	DestroyWindow();
}

void CR2RWhSetupPage::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	long lPos;
	BOOL bHome=0, bPLimit=0, bNLimit=0;
	int iBusy;

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->EnableWindow(0);

	
	theR2RTrack.SearchIndexStepHome();

	Sleep(100);

	theR2RTrack.WaitStepHomeFinish();

	Sleep(100);

	CPopUpTopStepperCtr theStepperCtrler(TOP_DEVICE_INDEXER);
	theStepperCtrler.DoModal();


	Sleep(100);

	theStepper.GetPos(TOP_DEVICE_INDEXER,&lPos);

	m_lIndexLogicZero =round((double)lPos *theStepper.GetResol(TOP_DEVICE_INDEXER));
	UpdateData(false);

 
	theR2RTrack.ChangeStepperMotionProfile(SRCH_SPEED_PROFILE);

	Sleep(100);

	theStepper.MoveStep(TOP_DEVICE_INDEXER, 1000000, MOTION_RELATIVE);
		do
		{
			theStepper.GetMotorStopReason(TOP_DEVICE_INDEXER,&bHome,&bNLimit,&bPLimit);
		}
		while(bPLimit == FALSE);

		theStepper.MoveStep(TOP_DEVICE_INDEXER, -5000, MOTION_RELATIVE);

		do
		{
			iBusy = (int)theStepper.IsBusy(TOP_DEVICE_INDEXER);
		} while (iBusy == 1);

		theStepper.GetPos(TOP_DEVICE_INDEXER,&lPos);

		m_lIndexRightLimit =round((double) lPos * theStepper.GetResol(TOP_DEVICE_INDEXER));
		UpdateData(false);

		Sleep(100);

		pWnd = GetDlgItem(IDC_BUTTON_START);
        pWnd->EnableWindow(1);

}

void CR2RWhSetupPage::OnBnClickedButtonStartTchTable()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START_TCH_TABLE);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->EnableWindow(TRUE);

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
	    theServo.MoveTable2(theMcData.GetRcd()->stTbSafeStandby);	//v3.1.T212 edit
	Sleep(100);

	pWnd = GetDlgItem(IDC_STATIC_INFO_SAFE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_MOVE_TO_SAFE_POS));
}

void CR2RWhSetupPage::OnBnClickedButtonNext()
{
	CWnd*pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->EnableWindow(FALSE);

	if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
    {
		TBL_POS stSafeTblPos;
        theServo.GetTablePosition(&stSafeTblPos);
		m_dWhTbX = stSafeTblPos.dx;
		m_dWhTbY = stSafeTblPos.dy;
	}

	pWnd = GetDlgItem(IDC_STATIC_INFO_SAFE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FINISH));

	//MSG_TBL_POS_OUT_OF_LIMIT

	UpdateData(false);
}

void CR2RWhSetupPage::OnEnChangeEditMagClamperLimit()
{
    UpdateData(true);
}
