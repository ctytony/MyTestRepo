// TopElevSetupPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "TopElevSetupPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "PopUpTopStepperCtr.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "TeachWorkHolderSheet.h"
#include "MachineConfig.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "DlgElevCtrl.h"
#include "Busy.h"
#include "ControlState.h"

// CTopElevSetupPage dialog

IMPLEMENT_DYNAMIC(CTopElevSetupPage, CPropertyPage)

CTopElevSetupPage::CTopElevSetupPage()
	: CPropertyPage(CTopElevSetupPage::IDD)
	, m_lOnload1stSlotLvl(0)
	, m_lOffload1stSlotLvl(0)
	, m_nStep(0)
	, m_nElevID(0)
	, m_nSlotIndex(0)
	, m_lMagSlotPitch(0)
	, m_lMagSwapPitch(0)
	, m_lOnloadTotalSlot(0)
	, m_lOffloadTotalSlot(0)
{
	m_bBusy = FALSE;	//V1.1.W158 add
}

CTopElevSetupPage::~CTopElevSetupPage()
{
}

void CTopElevSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_ELEVATOR_SIDE, m_nElevID);
	DDX_Text(pDX, IDC_EDIT_SLOT_NUM, m_nSlotIndex);
	DDX_Text(pDX, IDC_EDIT_TOTAL_SLOT_NUM_OF_ONLOADER, m_lOnloadTotalSlot);
	DDX_Text(pDX, IDC_EDIT_TOTAL_SLOT_NUM_OF_OFFLOADER, m_lOffloadTotalSlot);
}

BEGIN_MESSAGE_MAP(CTopElevSetupPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
	ON_CBN_SELCHANGE(IDC_COMBO_ELEVATOR_SIDE, OnElevatorSide)
	ON_EN_SETFOCUS(IDC_EDIT_SLOT_NUM, OnFocusSlotNum)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_SLOT, OnNextSlot)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SLOT_POS, OnEditSlotPos)
	ON_EN_CHANGE(IDC_EDIT_SLOT_NUM, OnEditSlotNum)
	ON_BN_CLICKED(IDC_BUTTON_RESET_OFFSET, OnResetOffset)
	ON_MESSAGE(WM_ISDIALOGBUSY, IsDialogBusy)	//V1.1.W158 add
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CTopElevSetupPage::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CTopElevSetupPage message handlers
HBRUSH CTopElevSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CTopElevSetupPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
	SetDlgItemTextW(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));
	SetDlgItemTextW(IDC_STATIC_SLOT_NUM, theMsg.GetMsg(MSG_CUR_SLOT));
	SetDlgItemTextW(IDC_BUTTON_EDIT_SLOT_POS, theMsg.GetMsg(MSG_EDIT));
	SetDlgItemTextW(IDC_BUTTON_NEXT_SLOT, theMsg.GetMsg(MSG_NEXT_SLOT));
	SetDlgItemTextW(IDC_BUTTON_RESET_OFFSET, theMsg.GetMsg(MSG_RESET));
	SetDlgItemTextW(IDC_STATIC_VERIFY, theMsg.GetMsg(MSG_VERIFY));
	SetDlgItemTextW(IDC_STATIC_POSITION, theMsg.GetMsg(MSG_FINE_TUNE_SLOT_POS));
	SetDlgItemTextW(IDC_STATIC_ELEVATOR, theMsg.GetMsg(MSG_ELEVATOR));
	SetDlgItemTextW(IDC_STATIC_TOTAL_SLOT_NUM_OF_ONLOADER, theMsg.GetMsg(MSG_ONLOAD_TOTAL_SLOT));
	SetDlgItemTextW(IDC_STATIC_TOTAL_SLOT_NUM_OF_OFFLOADER, theMsg.GetMsg(MSG_OFFLOAD_TOTAL_SLOT));

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ELEVATOR_SIDE);
	pComboBox->AddString(theMsg.GetMsg(MSG_INPUT));
	pComboBox->AddString(theMsg.GetMsg(MSG_OUTPUT));

	m_nElevID = 0;
	m_nSlotIndex = theTopOnloader.GetCurSlot();

    CTeachWorkHolderSheet *pSheet = (CTeachWorkHolderSheet*)GetParent();
	if (pSheet->m_iPageMode == WH_TEACH)
	{
		GetDlgItem(IDC_EDIT_SLOT_NUM)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_EDIT_SLOT_POS)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO_ELEVATOR_SIDE)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_NEXT_SLOT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RESET_OFFSET)->ShowWindow(FALSE);

		theTopOnloader.ResetSlotOffset();	//V1.5.W85 add
		theTopOffloader.ResetSlotOffset();	//V1.5.W85 add
	}

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->AddString(theMsg.GetMsg(MSG_EDIT_TO_FINE_TUNE_SLOT_POS));
	pListBox->AddString(theMsg.GetMsg(MSG_START_TO_TCH_ELEV_POS));

	TOP_ONLOAD_RCD stOnload;
	theTopOnloader.GetRcd(&stOnload);
	m_lOnload1stSlotLvl = stOnload.lOnload1stSlotLevel;
	m_lMagSlotPitch = stOnload.lSlotPitch;
	m_lMagSwapPitch = stOnload.lSwapMagPitch;

	TOP_OFFLOAD_RCD stOffload;
	theTopOffloader.GetRcd(&stOffload);
	m_lOffload1stSlotLvl = stOffload.lOffload1stSlotLevel;

	const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
	m_lOnloadTotalSlot = pstDevice->lMagSlotNum;
	m_lOffloadTotalSlot = pstDevice->lMagSlotNum;

	return CPropertyPage::OnInitDialog();
}

void CTopElevSetupPage::OnStart()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	TOP_ONLOAD_RCD stOnload;
	theTopOnloader.GetRcd(&stOnload);

	TOP_OFFLOAD_RCD stOffload;
	theTopOffloader.GetRcd(&stOffload);

	CTeachWorkHolderSheet *pSheet = (CTeachWorkHolderSheet*)GetParent();
	if (pSheet->m_iPageMode == WH_EDIT)
	{
		short anMsgArray[1] = {MSG_DELETE_ELEV_DATA};
		if (theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray, true) == SHOW_MSG_RESULT_STOP) return;

		ZeroMemory(stOnload.lSlotOffset, offsetof(TOP_ONLOAD_RCD, lSlotPitch));		//V1.5.W85 add
		ZeroMemory(stOffload.lSlotOffset, offsetof(TOP_OFFLOAD_RCD, lSlotPitch));	//V1.5.W85 add
	}

	stOnload.lOnloadSlotNum = m_lOnloadTotalSlot;
	theTopOnloader.SetRcd(&stOnload);

	stOffload.lOffloadSlotNum = m_lOffloadTotalSlot;
	theTopOffloader.SetRcd(&stOffload);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(1);

	pSheet->GetDlgItem(pSheet->m_iPageMode == WH_EDIT ? IDOK : ID_WIZNEXT)->EnableWindow(0);

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_REMOVE_ALL_MAG));
    pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
    pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));

	m_nStep = 0;

	theTopTrack.MoveIndexerToLogicZero();

	CDlgElevCtrl objElev;
	if (objElev.IsLeadframeOnTrack(TOP_ELEV_ONLOAD)
		|| objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD))	
		return;

	if (objElev.IsPusherError() && (pSheet->m_iPageMode == WH_EDIT))
		pSheet->GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CTopElevSetupPage::OnNext()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	CTeachWorkHolderSheet *pSheet = (CTeachWorkHolderSheet*)GetParent();
	CControlState cs(GetDlgItem(IDC_BUTTON_NEXT)->m_hWnd, FALSE, TRUE);	//V1.1.W158 add
	cs.AddControl(pSheet->GetDlgItem(IDCANCEL)->m_hWnd, FALSE, TRUE);	//V1.1.W158 add

	long lPos;
	long lNext1stSlotPos;

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();

	m_nStep++;

	switch (m_nStep)
	{
	case 1:
	{
        pListBox->AddString(theMsg.GetMsg(MSG_WAIT_FORK_STANDBY));
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_ONLOADER));

		if (theMcConfig.IsBypassIndex())
			Sleep_x1ms(1000);
		else
		{
			theStepper.GetPos(theTopOnloader.m_nMagHolderStepperId,&lPos);
			theTopOnloader.HomeMagHolder();
			Sleep_x1ms(1000);
			theTopOnloader.WaitHomeMagHolder();
			Sleep_x1ms(100);
			theTopOnloader.m_MagPullyDcMotor.TriggerForwardMotion();
			Sleep_x1ms(100);

			if (theTopOnloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
			{
				CStringList sList;
				sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_MAG_DROP_DETECTED));
				theMsg.ShowMsg(WARNING_MSG, sList);
			}

			theTopOnloader.m_MagPullyDcMotor.TriggerBacktoHome();
			Sleep_x1ms(100);

			theStepper.MoveStep(theTopOnloader.m_nMagHolderStepperId,
				round(215000 / theStepper.GetResol(theTopOnloader.m_nMagHolderStepperId)));

			CHTimer timer;
			do
			{
				ProcessMessage();
			} while (theStepper.IsBusy(theTopOnloader.m_nMagHolderStepperId)
						&& (timer.GetTime_x1ms() < 10000));
		}

		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		CPopUpTopStepperCtr dlg(theTopOnloader.m_nMagHolderStepperId);
		dlg.DoModal();

		theStepper.GetPos(theTopOnloader.m_nMagHolderStepperId, &lPos);
		m_lOnload1stSlotLvl = round(lPos * theStepper.GetResol(theTopOnloader.m_nMagHolderStepperId));
		UpdateData(false);

		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LAST_SLOT));
		break;
	}
	case 2:
	{
		if (!theMcConfig.IsBypassIndex())
		{
			const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
			theStepper.MoveStep(theTopOnloader.m_nMagHolderStepperId,
				-round(pstDevice->lMagLvPitch * (m_lOnloadTotalSlot - 1)
				/ theStepper.GetResol(theTopOnloader.m_nMagHolderStepperId)),
				MOTION_RELATIVE);

			CHTimer timer;
			do
			{
				ProcessMessage();
			} while (theStepper.IsBusy(theTopOnloader.m_nMagHolderStepperId)
						&& (timer.GetTime_x1ms() < 10000));
		}

		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LAST_SLOT));
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		CPopUpTopStepperCtr dlg(theTopOnloader.m_nMagHolderStepperId, SPEED_NORMAL);
		dlg.DoModal();

		theStepper.GetPos(theTopOnloader.m_nMagHolderStepperId, &lPos);
		m_lLastSlotPos = round(lPos * theStepper.GetResol(theTopOnloader.m_nMagHolderStepperId));

		TOP_ONLOAD_RCD stOnload;
		theTopOnloader.GetRcd(&stOnload);
		stOnload.lOnload1stSlotLevel = m_lOnload1stSlotLvl;
		stOnload.lSlotPitch = round((double)(m_lOnload1stSlotLvl - m_lLastSlotPos)
												/ (stOnload.lOnloadSlotNum - 1));
		theTopOnloader.SetRcd(&stOnload); 
		
		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_NEXT_1ST));
		break;
	}
	case 3:
	{
		if (!theMcConfig.IsBypassIndex())
		{
			const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
			theStepper.MoveStep(theTopOnloader.m_nMagHolderStepperId,
				-round((pstDevice->lMag1stLvHt + pstDevice->lMagLastLvHt)
				/ theStepper.GetResol(theTopOnloader.m_nMagHolderStepperId)),
				MOTION_RELATIVE);

			CHTimer timer;
			do
			{
				ProcessMessage();
			} while (theStepper.IsBusy(theTopOnloader.m_nMagHolderStepperId)
						&& (timer.GetTime_x1ms() < 10000));
		}

		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_ONLOADER));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_NEXT_1ST));
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		CPopUpTopStepperCtr dlg(theTopOnloader.m_nMagHolderStepperId, SPEED_NORMAL);
		dlg.DoModal();

		theStepper.GetPos(theTopOnloader.m_nMagHolderStepperId, &lPos);
		lNext1stSlotPos = round(lPos * theStepper.GetResol(theTopOnloader.m_nMagHolderStepperId));
		
		TOP_ONLOAD_RCD stOnload;
		theTopOnloader.GetRcd(&stOnload);
		stOnload.lSwapMagPitch  = m_lLastSlotPos - lNext1stSlotPos;
		theTopOnloader.SetRcd(&stOnload); 

		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_OFFLOADER));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));
		break;
	}
	case 4:
	{
		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_WAIT_FORK_STANDBY));
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_OFFLOADER));
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		if (theMcConfig.IsBypassIndex())
			Sleep_x1ms(1000);
		else
		{
			theTopOffloader.HomeMagHolder();
			Sleep_x1ms(1000);
			theTopOffloader.WaitHomeMagHolder();
			Sleep_x1ms(100);
			theTopOffloader.m_MagPullyDcMotor.TriggerForwardMotion();
			Sleep_x1ms(100);

			if (theTopOffloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
			{
				CStringList sList;
				sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_MAG_DROP_DETECTED));
				theMsg.ShowMsg(WARNING_MSG, sList);
			}

			theTopOffloader.m_MagPullyDcMotor.TriggerBacktoHome();
			Sleep_x1ms(100);

			TOP_ONLOAD_RCD stOnload;
			theTopOnloader.GetRcd(&stOnload);
			theStepper.MoveStep(theTopOffloader.m_nMagHolderStepperId,
				round(stOnload.lOnload1stSlotLevel / theStepper.GetResol(theTopOffloader.m_nMagHolderStepperId)));

			CHTimer timer;
			do
			{
				ProcessMessage();
			} while (theStepper.IsBusy(theTopOffloader.m_nMagHolderStepperId)
						&& (timer.GetTime_x1ms() < 10000));
		}

		pListBox->ResetContent();
        pListBox->AddString(theMsg.GetMsg(MSG_SIDE_OFFLOADER));
        pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_1ST_SLOT));

		CPopUpTopStepperCtr dlg(theTopOffloader.m_nMagHolderStepperId);
		dlg.DoModal();

		theStepper.GetPos(theTopOffloader.m_nMagHolderStepperId, &lPos);
		m_lOffload1stSlotLvl = round(lPos * theStepper.GetResol(theTopOffloader.m_nMagHolderStepperId));

		TOP_ONLOAD_RCD stOnload;
		theTopOnloader.GetRcd(&stOnload);
		TOP_OFFLOAD_RCD stOffload;
		theTopOffloader.GetRcd(&stOffload);
		stOffload.lSlotPitch = stOnload.lSlotPitch;
		stOffload.lSwapMagPitch = stOnload.lSwapMagPitch;
		stOffload.lOffload1stSlotLevel = m_lOffload1stSlotLvl;
		theTopOffloader.SetRcd(&stOffload);

		if (m_lOnloadTotalSlot == m_lOffloadTotalSlot)
		{
			pListBox->ResetContent();
			pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_QUIT));
			m_nStep = 6;
		}
		else
		{
			pListBox->ResetContent();
			pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
			pListBox->AddString(theMsg.GetMsg(MSG_SIDE_OFFLOADER));
			pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LAST_SLOT));
		}
		break;
	}
	case 5:
	{
		const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
		TOP_OFFLOAD_RCD stOffload;
		theTopOffloader.GetRcd(&stOffload);

		if (!theMcConfig.IsBypassIndex())
		{
			theStepper.MoveStep(theTopOffloader.m_nMagHolderStepperId,
				round((m_lOnload1stSlotLvl - pstDevice->lMagLvPitch * (stOffload.lOffloadSlotNum - 1))
				/ theStepper.GetResol(theTopOffloader.m_nMagHolderStepperId)));

			CHTimer timer;
			do
			{
				ProcessMessage();
			} while (theStepper.IsBusy(theTopOffloader.m_nMagHolderStepperId)
						&& (timer.GetTime_x1ms() < 10000));
		}

		pListBox->ResetContent();
		pListBox->AddString(theMsg.GetMsg(MSG_SIDE_OFFLOADER));
		pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LAST_SLOT));
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		CPopUpTopStepperCtr dlg(theTopOffloader.m_nMagHolderStepperId, SPEED_NORMAL);
		dlg.DoModal();

		theStepper.GetPos(theTopOffloader.m_nMagHolderStepperId, &lPos);
		m_lLastSlotPos = round(lPos * theStepper.GetResol(theTopOffloader.m_nMagHolderStepperId));

		stOffload.lOffload1stSlotLevel = m_lOffload1stSlotLvl;
		stOffload.lSlotPitch = round((double)(m_lOffload1stSlotLvl - m_lLastSlotPos)
												/ (stOffload.lOffloadSlotNum - 1));
		theTopOffloader.SetRcd(&stOffload);

		pListBox->ResetContent();
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
		pListBox->AddString(theMsg.GetMsg(MSG_SIDE_OFFLOADER));
		pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_NEXT_1ST));
		break;
	}
	case 6:
	{
		const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
		TOP_OFFLOAD_RCD stOffload;
		theTopOffloader.GetRcd(&stOffload);

		if (!theMcConfig.IsBypassIndex())
		{
			theStepper.MoveStep(theTopOffloader.m_nMagHolderStepperId,
				-round((pstDevice->lMag1stLvHt + pstDevice->lMagLastLvHt)
						/ theStepper.GetResol(theTopOffloader.m_nMagHolderStepperId)),
				MOTION_RELATIVE);

			CHTimer timer;
			do
			{
				ProcessMessage();
			} while (theStepper.IsBusy(theTopOffloader.m_nMagHolderStepperId)
						&& (timer.GetTime_x1ms() < 10000));
		}

		pListBox->ResetContent();
		pListBox->AddString(theMsg.GetMsg(MSG_SIDE_OFFLOADER));
		pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_NEXT_1ST));
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS));

		CPopUpTopStepperCtr dlg(theTopOffloader.m_nMagHolderStepperId, SPEED_NORMAL);
		dlg.DoModal();

		theStepper.GetPos(theTopOffloader.m_nMagHolderStepperId, &lPos);
		lNext1stSlotPos = round(lPos * theStepper.GetResol(theTopOffloader.m_nMagHolderStepperId));

		stOffload.lSwapMagPitch = m_lLastSlotPos - lNext1stSlotPos;
		stOffload.lSwapMagPitch = stOffload.lSwapMagPitch;
		theTopOffloader.SetRcd(&stOffload);

		pListBox->ResetContent();
		pListBox->AddString(theMsg.GetMsg(MSG_DONE_TO_QUIT));
		break;
	}
	case 7:
	{
        theApp.ArchiveMachineData(ARCHIVE_SAVE);
		Sleep_x1ms(10);
        theApp.ArchivePowerOnData(ARCHIVE_SAVE);

		if (!theMcConfig.IsBypassIndex())
		{
			theTopOnloader.MoveMagHolderToSlot(1);
			theTopOnloader.WaitMoveMagHolder();

			theTopOffloader.MoveMagHolderToSlot(1);
			theTopOnloader.WaitMoveMagHolder();
		}

		m_nSlotIndex = 1;
		UpdateData(false);
		
		GetDlgItem(IDC_EDIT_SLOT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EDIT_SLOT_POS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NEXT_SLOT)->EnableWindow(TRUE);

		cs.UpdateFinalState(GetDlgItem(IDC_BUTTON_NEXT)->m_hWnd, FALSE);	//V1.1.W158 add
		pSheet->GetDlgItem(pSheet->m_iPageMode == WH_TEACH ? ID_WIZNEXT : IDOK)->EnableWindow();
		break;
	}
	}
}

void CTopElevSetupPage::OnElevatorSide()
{
	UpdateData(true);
	m_nSlotIndex = (m_nElevID == 0 ? theTopOnloader.GetCurSlot() : theTopOffloader.GetCurSlot());
	UpdateData(false);
}

void CTopElevSetupPage::OnFocusSlotNum()
{
	GetDlgItem(IDC_STATIC_TOTAL_SLOT_NUM_OF_ONLOADER)->SetFocus();
	const TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
    theKeyPad.GetUserKeyInData(&m_nSlotIndex, 1, 0, pstDevice->lMagSlotNum);
    UpdateData(false);
	OnEditSlotNum();
}

void CTopElevSetupPage::OnEditSlotNum()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	UpdateData(true);

	if (m_nElevID == 0)
	{
		theTopOnloader.MoveMagHolderToSlot(m_nSlotIndex);
		theTopOnloader.WaitMoveMagHolder();
	}
	else
	{
		theTopOffloader.MoveMagHolderToSlot(m_nSlotIndex);
		theTopOffloader.WaitMoveMagHolder();
	}
}

void CTopElevSetupPage::OnNextSlot()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add
	m_nSlotIndex++;
	UpdateData(false);

	if (m_nElevID == 0)
	{
		TOP_ONLOAD_RCD stOnload;
		theTopOnloader.GetRcd(&stOnload);
		if (m_nSlotIndex > stOnload.lOnloadSlotNum) m_nSlotIndex = 0;
		if (!theMcConfig.IsBypassOnload())	//v3.0.T462 add
		{
			theTopOnloader.MoveMagHolderToSlot(m_nSlotIndex);
			theTopOnloader.WaitMoveMagHolder();
		}
	}
	else
	{
		TOP_OFFLOAD_RCD stOffload;
		theTopOffloader.GetRcd(&stOffload);
		if (m_nSlotIndex > stOffload.lOffloadSlotNum) m_nSlotIndex = 0;
		if (!theMcConfig.IsBypassOffload())	//v3.0.T462 add
		{
			theTopOffloader.MoveMagHolderToSlot(m_nSlotIndex);
			theTopOffloader.WaitMoveMagHolder();
		}
	}
}

void CTopElevSetupPage::OnEditSlotPos()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	long lStartPos, lEndPos;
	if (m_nElevID == 0)
	{
		theStepper.GetPos(theTopOnloader.m_nMagHolderStepperId, &lStartPos);
		CPopUpTopStepperCtr dlg(theTopOnloader.m_nMagHolderStepperId);
	    dlg.DoModal();
		theStepper.GetPos(theTopOnloader.m_nMagHolderStepperId, &lEndPos);

		if (!theMcConfig.IsBypassOnload())
		{
			TOP_ONLOAD_RCD stOnload;
			theTopOnloader.GetRcd(&stOnload);
			long lMoveDist = round((lEndPos - lStartPos) * theStepper.GetResol(theTopOnloader.m_nMagHolderStepperId));
			stOnload.lSlotOffset[m_nSlotIndex] = stOnload.lSlotOffset[m_nSlotIndex] + lMoveDist;
			theTopOnloader.SetRcd(&stOnload);
		}
	}
	else
	{
		theStepper.GetPos(theTopOffloader.m_nMagHolderStepperId, &lStartPos);
		CPopUpTopStepperCtr dlg(theTopOffloader.m_nMagHolderStepperId);
	    dlg.DoModal();
		theStepper.GetPos(theTopOffloader.m_nMagHolderStepperId, &lEndPos);

		if (!theMcConfig.IsBypassOnload())
		{
			TOP_OFFLOAD_RCD stOffload;
			theTopOffloader.GetRcd(&stOffload);
			long lMoveDist = round((lEndPos - lStartPos) * theStepper.GetResol(theTopOffloader.m_nMagHolderStepperId));
			stOffload.lSlotOffset[m_nSlotIndex] = stOffload.lSlotOffset[m_nSlotIndex] + lMoveDist;
			theTopOffloader.SetRcd(&stOffload);
		}
	}
}

void CTopElevSetupPage::OnResetOffset()
{
	(m_nElevID == 0) ? theTopOnloader.ResetSlotOffset() : theTopOffloader.ResetSlotOffset();
}

LRESULT CTopElevSetupPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)	//V1.1.W158 add
{
	return (m_bBusy == TRUE);
}

void CTopElevSetupPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_RECIPE, 0);	//v3.0.T460 add
}
