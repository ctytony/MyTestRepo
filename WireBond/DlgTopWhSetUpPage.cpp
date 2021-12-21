// DlgTopWhSetUpPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DlgTopWhSetUpPage.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MenuConfig.h"
#include "PopUpCuEliteStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "MaterialHandlerThread.h"
#include "PopUpTopStepperCtr.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "servo.h"
#include "Busy.h"

// CDlgTopWhSetUpPage dialog

IMPLEMENT_DYNAMIC(CDlgTopWhSetUpPage, CDialog)

CDlgTopWhSetUpPage::CDlgTopWhSetUpPage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTopWhSetUpPage::IDD, pParent)
	, m_lIndexLogicZero(0)
	, m_lIndexRightLimit(0)
	, m_dSafePosX(0)
	, m_dSafePosY(0)
	, m_sSafePosX(_T("0"))
	, m_sSafePosY(_T("0"))
    , m_iElevatorId(0)
    , m_lMagClamperLimit(0)
    , m_lMagClamperPosition(0)
{
	m_bBusy = FALSE;	//V1.1.W147 add
}

CDlgTopWhSetUpPage::~CDlgTopWhSetUpPage()
{
}

void CDlgTopWhSetUpPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INDEX_LOGIC_ZERO, m_lIndexLogicZero);
	DDX_Text(pDX, IDC_EDIT_INDEX_RIGHT_LIMIT, m_lIndexRightLimit);
	DDX_Text(pDX, IDC_EDIT_SAFE_POS_X, m_sSafePosX);
	DDX_Text(pDX, IDC_EDIT_SAFE_POS_Y, m_sSafePosY);
   	DDX_CBIndex(pDX, IDC_COMBO_MAGCLAMPER_ID, m_iElevatorId);
    DDX_Text(pDX, IDC_EDIT_MAGCLAMPER_LIMIT, m_lMagClamperLimit);
    DDX_Text(pDX, IDC_EDIT_MAGCLAMPER_POSITION, m_lMagClamperPosition);
}

BEGIN_MESSAGE_MAP(CDlgTopWhSetUpPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_START_TCH_TABLE, OnStartTchTable)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
    ON_BN_CLICKED(IDC_BUTTON_MAGCLAMPER_EDIT, OnClampperEdit) 
    ON_CBN_SELCHANGE(IDC_COMBO_MAGCLAMPER_ID, OnElevatorSide)
    ON_EN_SETFOCUS(IDC_EDIT_MAGCLAMPER_LIMIT, OnFocusMagClamperLimit)
    ON_EN_SETFOCUS(IDC_EDIT_MAGCLAMPER_POSITION, OnFocusMagClamperPosition)
    ON_EN_CHANGE(IDC_EDIT_MAGCLAMPER_POSITION, OnEditMagClamperPosition)
    ON_EN_CHANGE(IDC_EDIT_MAGCLAMPER_LIMIT, OnEditMagClamperLimit)
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W147 add
	ON_BN_CLICKED(IDC_BUTTON_CLAMP_CLOSE, OnClampClose)
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CDlgTopWhSetUpPage::OnBnClickedButtonDone)	//v3.0.T453 add
	ON_BN_CLICKED(IDC_BUTTON_DONE2, &CDlgTopWhSetUpPage::OnBnClickedButtonDone2)
END_MESSAGE_MAP()

// CDlgTopWhSetUpPage message handlers
BOOL CDlgTopWhSetUpPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);

    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_BUTTON_DONE2, theMsg.GetMsg(MSG_DONE));	//v3.0.T453 add
	SetDlgItemTextW(IDC_BUTTON_START_TCH_TABLE, theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));	//v3.0.T453 add
	SetDlgItemTextW(IDC_STATIC_INDEXER_ALIGN, theMsg.GetMsg(MSG_INDEXER_ALIGN));
	SetDlgItemTextW(IDC_STATIC_INDEXER_LOGIC_0, theMsg.GetMsg(MSG_INDEXER_LOGIC_ZERO));
	SetDlgItemTextW(IDC_STATIC_INDEXER_RIGHT_LIMIT,
					theMsg.GetMsg(MSG_INDEXER_RIGHT_LIMIT));
	SetDlgItemTextW(IDC_STATIC_TABLE_SAFE_POS, theMsg.GetMsg(MSG_TCH_SAFE_POS));
	SetDlgItemTextW(IDC_BUTTON_CLAMP_CLOSE, theMsg.GetMsg(MSG_EDIT));		//V1.5.W83 add
	SetDlgItemTextW(IDC_BUTTON_MAGCLAMPER_EDIT, theMsg.GetMsg(MSG_EDIT));

	//V1.1.W149 add
	SetDlgItemTextW(IDC_STATIC_MAGCLAMPER, theMsg.GetMsg(MSG_MAG_CLAMPER));
	SetDlgItemTextW(IDC_STATIC_ELEVATOR, theMsg.GetMsg(MSG_ELEV_SIDE));
	SetDlgItemTextW(IDC_STATIC_CLAPM_POSITION, theMsg.GetMsg(MSG_CLAMP_DIST));
	SetDlgItemTextW(IDC_STATIC_MAGCLAMPER_POSITION,	theMsg.GetMsg(MSG_CALI_POSITION));

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MAGCLAMPER_ID);
    pComboBox->InsertString(0, theMsg.GetMsg(MSG_INPUT));
    pComboBox->InsertString(1, theMsg.GetMsg(MSG_OUTPUT));

	m_iElevatorId = 0;

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	m_lIndexLogicZero     = pstMcData->lZeroOffset;
	m_lIndexRightLimit    = pstMcData->lIndexerLimit; 
	m_dSafePosX           = pstMcData->stTbSafeStandby.dx;
	m_dSafePosY           = pstMcData->stTbSafeStandby.dy;
	m_sSafePosX.Format(_T("%.1f"), m_dSafePosX * theServo.GetResolXY());
	m_sSafePosY.Format(_T("%.1f"), m_dSafePosY * theServo.GetResolXY());
    m_lMagClamperLimit    = pstMcData->lOnLoadMagClamperLimit;
    m_lMagClamperPosition = pstMcData->lOnLoadMagClamperPosition;

	return CDialog::OnInitDialog();
}

void CDlgTopWhSetUpPage::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

HBRUSH CDlgTopWhSetUpPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDlgTopWhSetUpPage::OnDestroy()
{
	CDialog::OnDestroy();

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	pstMcData->lZeroOffset = m_lIndexLogicZero;
	pstMcData->lIndexerLimit = m_lIndexRightLimit;
	pstMcData->stTbSafeStandby.dx = m_dSafePosX;
	pstMcData->stTbSafeStandby.dy = m_dSafePosY;
}

void CDlgTopWhSetUpPage::OnStart()
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	pListBox->InsertString(0, theMsg.GetMsg(MSG_INSTALL_INDEX_ALIGN_JIG));
	pListBox->InsertString(1, theMsg.GetMsg(MSG_PRESS_UP_DOWN_UNTIL_JIG));
	pListBox->InsertString(2, theMsg.GetMsg(MSG_ALIGN_WITH_BOND_SITE));
	pListBox->InsertString(3, theMsg.GetMsg(MSG_OK_TO_CONTINUE));
	
	if (!theMcConfig.IsBypassIndex())
	{
		theTopTrack.HomeIndexer();
		Sleep_x1ms(100);
		theTopTrack.WaitHomeIndexer();
		Sleep_x1ms(100);
	}

	CPopUpTopStepperCtr clStepCtrl(theTopTrack.GetIndexStepID());
	clStepCtrl.DoModal();
	Sleep_x1ms(100);

	long lPos;
	if (!theMcConfig.IsBypassIndex())
		theStepper.GetPos(theTopTrack.GetIndexStepID(), &lPos);
	m_lIndexLogicZero = round(lPos * theStepper.GetResol(theTopTrack.GetIndexStepID()));
	UpdateData(false);
 
	theTopTrack.ChangeStepperMotionProfile(SRCH_SPEED_PROFILE);
	Sleep_x1ms(100);

	if (!theMcConfig.IsBypassIndex())
	{
		theStepper.MoveStep(theTopTrack.GetIndexStepID(), 1000000, MOTION_RELATIVE);
		do
		{
			Sleep_x1ms(10);
		} while (theStepper.IsBusy(theTopTrack.GetIndexStepID()));

		theStepper.GetPos(theTopTrack.GetIndexStepID(), &lPos);
	}

	m_lIndexRightLimit = round(lPos * theStepper.GetResol(theTopTrack.GetIndexStepID()));
	UpdateData(false);
	Sleep_x1ms(100);

	if (!theMcConfig.IsBypassIndex())
	{
		theStepper.MoveStep(theTopTrack.GetIndexStepID(), -5000, MOTION_RELATIVE);
		do
		{
			Sleep_x1ms(10);
		} while (theStepper.IsBusy(theTopTrack.GetIndexStepID()));
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	pListBox->ResetContent();
	pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_QUIT));
}

void CDlgTopWhSetUpPage::OnStartTchTable()
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);

    theServo.MoveTable2(theMcData.GetRcd()->stTbSafeStandby);	//v3.1.T198 edit
	Sleep_x1ms(100);

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_SAFE_POS));
}

void CDlgTopWhSetUpPage::OnNext()
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

	GetDlgItem(IDC_BUTTON_START_TCH_TABLE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

	TBL_POS stSafeTblPos;
	theServo.GetTablePosition(&stSafeTblPos);
	m_dSafePosX = stSafeTblPos.dx;
	m_dSafePosY = stSafeTblPos.dy;
	m_sSafePosX.Format(_T("%.1f"), m_dSafePosX * theServo.GetResolXY());
	m_sSafePosY.Format(_T("%.1f"), m_dSafePosY * theServo.GetResolXY());

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_FINISH));

	UpdateData(false);
}

void CDlgTopWhSetUpPage::OnClampperEdit()
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

    long lPos = 0;
 
	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	if (m_iElevatorId == 0)
    {
        CPopUpTopStepperCtr topStepperCtrl(theTopOnloader.m_nMagClamperStepperId, SPEED_FINE);
        topStepperCtrl.DoModal();

        theStepper.GetPos(theTopOnloader.m_nMagClamperStepperId, &lPos);
        m_lMagClamperPosition = round(lPos * theStepper.GetResol(theTopOnloader.m_nMagClamperStepperId));
        pstMcData->lOnLoadMagClamperPosition = m_lMagClamperPosition;
    }
    else
    {
        CPopUpTopStepperCtr topStepperCtrl(theTopOffloader.m_nMagClamperStepperId, SPEED_FINE );
        topStepperCtrl.DoModal();

        theStepper.GetPos(theTopOffloader.m_nMagClamperStepperId, &lPos);
        m_lMagClamperPosition = round(lPos * theStepper.GetResol(theTopOffloader.m_nMagClamperStepperId));
        pstMcData->lOffLoadMagClamperPosition = m_lMagClamperPosition;
    }

    UpdateData(false);
}

void CDlgTopWhSetUpPage::OnElevatorSide()
{
    m_iElevatorId = ((CComboBox*)GetDlgItem(IDC_COMBO_MAGCLAMPER_ID))->GetCurSel();

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	if (m_iElevatorId == 0)
    {    
        m_lMagClamperLimit = pstMcData->lOnLoadMagClamperLimit;
        m_lMagClamperPosition = pstMcData->lOnLoadMagClamperPosition;
    }
    else
    {
        m_lMagClamperLimit = pstMcData->lOffLoadMagClamperLimit;
        m_lMagClamperPosition = pstMcData->lOffLoadMagClamperPosition;
    }

    UpdateData(false);
}

 void CDlgTopWhSetUpPage::OnFocusMagClamperLimit()
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

    GetDlgItem(IDC_COMBO_MAGCLAMPER_ID)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_lMagClamperLimit);

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	if (m_iElevatorId == 0)
        pstMcData->lOnLoadMagClamperLimit = m_lMagClamperLimit;
    else 
        pstMcData->lOffLoadMagClamperLimit = m_lMagClamperLimit;

    UpdateData(false);
}

void CDlgTopWhSetUpPage::OnFocusMagClamperPosition()
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

    GetDlgItem(IDC_COMBO_MAGCLAMPER_ID)->SetFocus();
	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	if (m_iElevatorId == 0)
    {
        theKeyPad.GetUserKeyInData(&m_lMagClamperPosition, pstMcData->lOnLoadMagClamperPosition, 0, 700000);
        pstMcData->lOnLoadMagClamperPosition = m_lMagClamperPosition;
    }
    else 
    {
        theKeyPad.GetUserKeyInData(&m_lMagClamperPosition, pstMcData->lOffLoadMagClamperPosition, 0, 700000);
        pstMcData->lOffLoadMagClamperPosition = m_lMagClamperPosition;
    }

    UpdateData(false);
	OnEditMagClamperPosition();
 }

void CDlgTopWhSetUpPage::OnEditMagClamperPosition()
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

	if (m_iElevatorId == 0)
        theTopOnloader.SearchMoveMagClamperToClampPos();
	else
		theTopOffloader.SearchMoveMagClamperToClampPos();
	
	UpdateData(true);
}

void CDlgTopWhSetUpPage::OnEditMagClamperLimit()
{
    UpdateData(true);
}

LRESULT CDlgTopWhSetUpPage::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W147 add
{
	return (m_bBusy == FALSE&&DestroyWindow()) * DIALOG_CLOSED;
}

void CDlgTopWhSetUpPage::OnClampClose()		//V1.5.W83 add
{
	CBusy bs(&m_bBusy);		//V1.1.W147 add

	MACHINE_DATA* pstMcData = theMcData.GetRcd();

	if (m_iElevatorId == 0)
    {
        CPopUpTopStepperCtr topStepperCtrl(theTopOnloader.m_nMagClamperStepperId, SPEED_FINE);
        topStepperCtrl.DoModal();

        theStepper.GetPos(theTopOnloader.m_nMagClamperStepperId, &m_lMagClamperLimit);
        m_lMagClamperLimit = round(m_lMagClamperLimit * theStepper.GetResol(theTopOnloader.m_nMagClamperStepperId))-m_lMagClamperPosition;
        pstMcData->lOnLoadMagClamperLimit = m_lMagClamperLimit;

		theStepper.MoveDist(theTopOnloader.m_nMagClamperStepperId,m_lMagClamperPosition);
    }
    else
    {
        CPopUpTopStepperCtr topStepperCtrl(theTopOffloader.m_nMagClamperStepperId, SPEED_FINE );
        topStepperCtrl.DoModal();

        theStepper.GetPos(theTopOffloader.m_nMagClamperStepperId, &m_lMagClamperLimit);
        m_lMagClamperLimit = round(m_lMagClamperLimit * theStepper.GetResol(theTopOffloader.m_nMagClamperStepperId))-m_lMagClamperPosition;
        pstMcData->lOffLoadMagClamperLimit = m_lMagClamperLimit;

		theStepper.MoveDist(theTopOffloader.m_nMagClamperStepperId, m_lMagClamperPosition);
    }

	SetDlgItemInt(IDC_EDIT_MAGCLAMPER_LIMIT, m_lMagClamperLimit);
}

void CDlgTopWhSetUpPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_SETUP, 0);	//v3.0.T453 add
}

void CDlgTopWhSetUpPage::OnBnClickedButtonDone2()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_SETUP, 0);	//v3.0.T453 add
}
