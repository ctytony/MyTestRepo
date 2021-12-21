/***************************************************************
TopIndexTchPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "Program.h"
#include "TopIndexTchPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "VisionSystem.h"
#include "MaterialHandlerThread.h"
#include "MachineConfig.h"
#include "StepperControl.h"
#include "MathCalc.h"
#include "PopUpTopStepperCtr.h"
#include "TeachWorkHolderSheet.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "R2RIndex.h"
#include "servo.h"
#include "DlgElevCtrl.h"
#include "Busy.h"
#include "ControlState.h"

// CTopIndexTchPage dialog
IMPLEMENT_DYNAMIC(CTopIndexTchPage, CPropertyPage)

CTopIndexTchPage::CTopIndexTchPage()
	: CPropertyPage(CTopIndexTchPage::IDD)
	, m_nPatternSizeX(120)
	, m_nPatternSizeY(120)
	, m_nStepId(0)
	, m_bTchNewActived(false)
{
}

CTopIndexTchPage::~CTopIndexTchPage()
{
}

void CTopIndexTchPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, m_nPatternSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_nPatternSizeY);
	DDX_Text(pDX, IDC_STATIC_AVAILABLE, m_szPRAvailble);
}

BEGIN_MESSAGE_MAP(CTopIndexTchPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_SIZE_X, OnEditSizeX)
	ON_EN_CHANGE(IDC_EDIT_SIZE_Y, OnEditSizeY)
	ON_EN_SETFOCUS(IDC_EDIT_SIZE_X, OnFocusSizeX)
	ON_EN_SETFOCUS(IDC_EDIT_SIZE_Y, OnFocusSizeY)
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnContinue)
	ON_BN_CLICKED(IDC_BUTTON_RESET_PR, OnResetPr)
END_MESSAGE_MAP()

// CTopIndexTchPage message handlers
BOOL CTopIndexTchPage::OnInitDialog()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

    SetDlgItemTextW(IDC_BUTTON_START,theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_RESET_PR,theMsg.GetMsg(MSG_TCH_NEW));
    SetDlgItemTextW(IDC_STATIC_PGM_AVAIL,theMsg.GetMsg(MSG_PROGRAM));
    SetDlgItemTextW(IDC_STATIC_STATUS,theMsg.GetMsg(MSG_PR_INDEX));
    SetDlgItemTextW(IDC_STATIC_PARA,theMsg.GetMsg(MSG_SETTING));
    SetDlgItemTextW(IDC_STATIC_SIZE_X,theMsg.GetMsg(MSG_PATTERN_SIZE_X));
    SetDlgItemTextW(IDC_STATIC_SIZE_Y,theMsg.GetMsg(MSG_PATTERN_SIZE_Y));
	SetDlgItemTextW(IDC_BUTTON_CONTINUE,theMsg.GetMsg(MSG_NEXT));

	m_szPRAvailble = theMsg.GetMsg(stPrIndexRefRcd.bIndexPrAvail ? MSG_PGM_EXIST : MSG_NO_RECIPE);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->AddString(theMsg.GetMsg(MSG_START_TO_TCH_INDEX_PR));

	if (psheet->m_iPageMode == WH_TEACH)
    {
        psheet->SetWizardButtons(PSWIZB_FINISH);
        psheet->GetDlgItem(ID_WIZFINISH)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET_PR)->EnableWindow(FALSE);		
    }
	else
	{
		pListBox->AddString(theMsg.GetMsg(MSG_TCH_NEW_TO_INDEX_LF));
	}

	return CPropertyPage::OnInitDialog();
}

HBRUSH CTopIndexTchPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTopIndexTchPage::OnResetPr()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();
	if (psheet->m_iPageMode == WH_EDIT)
    {
		short anMsgArray[1] = {MSG_DELETE_PR_INDEX_DATA};
		if (theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray, true) == SHOW_MSG_RESULT_STOP) return;
	}

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);
	if (stPrIndexRefRcd.bIndexPrAvail == true)
    {		
		theVisionSys.ClearAllIndexPrPattern();
	}

	theDevice.DeleteIndexPrRef();

	m_nStepId = 0;
	m_bTchNewActived = true;

	m_szPRAvailble = theMsg.GetMsg(MSG_NO_RECIPE);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(0);

	UpdateData(false);
}

void CTopIndexTchPage::OnEditSizeX()
{
	 if (UpdateData(true) != FALSE)
        theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
}

void CTopIndexTchPage::OnEditSizeY()
{
	if (UpdateData(true) != FALSE)
        theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
}

void CTopIndexTchPage::OnFocusSizeX()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	GetDlgItem(IDC_STATIC_STATUS)->SetFocus();
       
    theKeyPad.GetUserKeyInData(&m_nPatternSizeX, 0,0, 256);

    UpdateData(false);
    
    OnEditSizeX();
}

void CTopIndexTchPage::OnFocusSizeY()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	GetDlgItem(IDC_STATIC_STATUS)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nPatternSizeY, 0,0, 256);

    UpdateData(false);

    OnEditSizeY();
}

void CTopIndexTchPage::OnStart()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);
	if(m_bTchNewActived ==false)
	{
		if(stPrIndexRefRcd.bIndexPrAvail == true)	
			theVisionSys.ClearAllIndexPrPattern();

		theDevice.DeleteIndexPrRef();

		m_nStepId = 1;				
	}
	else
	{
		if(stPrIndexRefRcd.bIndexPrAvail == true)
			theVisionSys.ClearAllIndexPrPattern();

		theDevice.DeleteIndexPrRef();

		m_nStepId = 0;
	}
		
	theTopTrack.SetPrIndexMask(true);
	theVisionDisplay.EnableDisplay(false);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);

	if (m_bTchNewActived ==false)
	{
		if(psheet->m_iPageMode == WH_EDIT)
			psheet->GetDlgItem(IDOK)->EnableWindow(0);

		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_ADJ_INDEX_OFFSET));
	}
	else
	{
		theMHS.ClearTrack();

		Sleep_x1ms(100);

		CDlgElevCtrl objElev;
		if (objElev.IsLeadframeOnTrack(TOP_ELEV_ONLOAD)) return;

		 int iPusherStatus;
		 iPusherStatus=theTopOnloader.m_InjectPusher.TriggerForwardMotion();
		 iPusherStatus+=theTopOnloader.m_InjectPusher.TriggerBacktoHome();
		 if(iPusherStatus != FOK)
		 {
			 short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
			 theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
			 GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(TRUE);
			 return;
		 }

		 if (!theTopTrack.m_InjectSensor.GetCurrentState())
		 {
			 theTopOnloader.SubmitOnloadEvent(TOP_ONLOAD_EVENT_START);
			 Sleep_x1ms(10);

			 DWORD dwLastTime=GetCurrentTime();

			 do
			 {
				 theTopOnloader.ProcessOnloadEvent();

				 Sleep_x1ms(10);
			 }
			 while(theTopOnloader.GetEvent()!= TOP_ONLOAD_EVENT_IDLE&&::GetCurrentTime()-dwLastTime < 10000);
		 }

		 pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));
	}

	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(TRUE);
}

void CTopIndexTchPage::OnContinue()
{
	CBusy bs(&m_bBusy);		//V1.1.W158 add

	CTeachWorkHolderSheet* pSheet = (CTeachWorkHolderSheet*)GetParent();

	CControlState cs(GetDlgItem(IDC_BUTTON_CONTINUE)->m_hWnd,FALSE,TRUE);//V1.1.W158 add
	cs.AddControl(pSheet->GetDlgItem(IDCANCEL)->m_hWnd,FALSE,TRUE);		//V1.1.W158 add
	cs.AddControl(pSheet->GetDlgItem(pSheet->m_iPageMode == WH_EDIT ? IDOK : ID_WIZFINISH)->m_hWnd,FALSE,TRUE);		//V1.1.W158 add

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();

	m_nStepId++;

	short nPatRcd = 0;
    short nSideRedLight, nSideBlueLight;
	TBL_POS stCurTblPos, stMovePos;

	CPopUpTopStepperCtr theStepperCtrler(theTopTrack.GetIndexStepID(), SPEED_SMALL);
	theStepperCtrler.PresetDlgPos(DLG_POS_BOTTOM_RIGHT);
	theStepperCtrler.Preset2ndCtrlID(TOP_WINDOW_CLAMPER);

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	switch(m_nStepId)
	{
	case 1:
		{
		theMHS.InitAutoIndex();

		Sleep_x1ms(100);

		theMHS.SetAutoIndex(true);
		theMHS.SetDryRun(false);
	
		Sleep_x1ms(10);

		theMHS.SetRequestIndex(true);

		Sleep_x1ms(100);

		DWORD dwLastTime=GetCurrentTime();

		do
		{
			Sleep_x1ms(10);
		}
		while(theTopTrack.GetEvent() != INDEX_EVENT_IDLE&&::GetCurrentTime()-dwLastTime < 20000);

		theMHS.SetAutoIndex(false);

		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_ADJ_LF_POS));		

		break;	
		}
	case 2:
		//find the left bottom of the device
		//adjust the device by indexer
		theTopClamp.PreCloseClamp();

		Sleep_x1ms(200);
		
		if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
			theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
		theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);		
		
	    theStepperCtrler.DoModal();
	
		theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
		theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);

		theTopClamp.CloseClamp();

		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

		break;

	case 3:
		theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);

		pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
		pListBox->AddString(theMsg.GetMsg(MSG_TEACH_ALIGN_POINT_COLON));
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

		break;

	case 4:
		//adjust lighting, find the manu alignment of PR point
		stPrIndexRefRcd.nManuSrchSizeX = m_nPatternSizeX;
		stPrIndexRefRcd.nManuSrchSizeY = m_nPatternSizeY;
				
		theVisionSys.EnableVideo(false);
		theVisionSys.GrabImage();

		if (theVisionSys.LearnVisionPattern(false, m_nPatternSizeX, m_nPatternSizeY, &nPatRcd, true) == FOK)
		{
			stPrIndexRefRcd.ulManuAlignPatternID = nPatRcd;
		}

		theVisionSys.GetLightingParameter(&(stPrIndexRefRcd.stManuAlignLightPara.nCoaxRedLight), &nSideRedLight, &(stPrIndexRefRcd.stManuAlignLightPara.nCoaxBlueLight), &nSideBlueLight);

		stPrIndexRefRcd.stManuAlignLightPara.bSideRedLight  = (byte)nSideRedLight;
		stPrIndexRefRcd.stManuAlignLightPara.bSideBlueLight = (byte)nSideBlueLight;

		if (!theMcConfig.IsBypassAll())
		{
			theServo.GetTablePosition(&stPrIndexRefRcd.stManuAlignPos);
			stPrIndexRefRcd.nManuAlignMag = 0;
		}
		else
		{
			stPrIndexRefRcd.stManuAlignPos.dx = 40000 / theServo.GetResolXY();
			stPrIndexRefRcd.stManuAlignPos.dy =  8000 / theServo.GetResolXY();
		}

		 stPrIndexRefRcd.stVisionLightPara = stPrIndexRefRcd.stManuAlignLightPara;

		 //SAVE RCD
		 theDevice.SetIndexPrRef(&stPrIndexRefRcd);

		 theVisionSys.EnableVideo(true);

		if(pSheet->m_iPageMode == WH_EDIT && m_bTchNewActived ==false)
		{
			theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
		}
		else
		{
			 theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
		}

		pListBox->AddString(theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
		pListBox->AddString(theMsg.GetMsg(MSG_TEACH_VISION_PATTERN));
		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

		break;

	case 5:
	{
		//adjust lighting, find the area of PR pattern
		//verification, approval go; reject, m_nStepId--
		stPrIndexRefRcd.nVisionSrchSizeX = m_nPatternSizeX;
		stPrIndexRefRcd.nVisionSrchSizeY = m_nPatternSizeY;

		theVisionSys.SetSearchSize(stPrIndexRefRcd.nVisionSrchSizeX, stPrIndexRefRcd.nVisionSrchSizeY);
		theVisionSys.EnableVideo(false);
		theVisionSys.GrabImage();

		if(theVisionSys.LearnVisionPattern(false, m_nPatternSizeX, m_nPatternSizeY, &nPatRcd, true) == FOK)
		{
			stPrIndexRefRcd.ulVisionAlignPatternID = nPatRcd;
		}

		TOP_TRACK_DATA stTrackData;
		theTopTrack.GetRcd(&stTrackData);
		theVisionSys.SetPassScore(nPatRcd, stTrackData.sPRIndexScore);

		if (!theMcConfig.IsBypassAll())
		{
			theServo.GetTablePosition(&stCurTblPos);
			stPrIndexRefRcd.stVisionAlignPos = stCurTblPos;
			stPrIndexRefRcd.nVisionAlignMag = 0;
		}
		else
		{
			stPrIndexRefRcd.stVisionAlignPos.dx = 40000 / theServo.GetResolXY();
			stPrIndexRefRcd.stVisionAlignPos.dy =  8000 / theServo.GetResolXY();
		}

		theVisionSys.GetLightingParameter(&(stPrIndexRefRcd.stVisionLightPara.nCoaxRedLight), &nSideRedLight, &(stPrIndexRefRcd.stVisionLightPara.nCoaxBlueLight), &nSideBlueLight);
		stPrIndexRefRcd.stVisionLightPara.bSideRedLight  = (byte)nSideRedLight;
		stPrIndexRefRcd.stVisionLightPara.bSideBlueLight = (byte)nSideBlueLight;

		stPrIndexRefRcd.bIndexPrAvail = true;

		//SAVE RCD
        theDevice.SetIndexPrRef(&stPrIndexRefRcd);

        theVisionSys.EnableVideo(true);

        if (pSheet->m_iPageMode == WH_EDIT)
            pSheet->GetDlgItem(IDOK)->EnableWindow(0);

        theTopClamp.CloseClamp();

        Sleep_x1ms(100);

        pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_FINISH));

        m_nStepId++;

		break;
	}
	case 6:
	{
		//adjust lighting, find the area of BACK-UP PR pattern
		//verification, approval go; reject, m_nStepId--
		stPrIndexRefRcd.nBkupAlignSrchSizeX = m_nPatternSizeX;
		stPrIndexRefRcd.nBkupAlignSrchSizeY = m_nPatternSizeY;

		theVisionSys.SetSearchSize(stPrIndexRefRcd.nBkupAlignSrchSizeX, stPrIndexRefRcd.nBkupAlignSrchSizeY);
		theVisionSys.EnableVideo(false);
		theVisionSys.GrabImage();

		theVisionSys.LearnVisionPattern(false, m_nPatternSizeX, m_nPatternSizeY, &nPatRcd, true);
		if(pSheet->m_iPageMode == WH_EDIT && m_bTchNewActived ==false)
			theVisionSys.ClearPatternRcd(
				(short)stPrIndexRefRcd.ulVisionBkupPatternID);

		stPrIndexRefRcd.ulVisionBkupPatternID = nPatRcd;

		theVisionSys.SetPassScore(nPatRcd, thePgm.GetRcd()->nLeadPrPassScore);

		if (!theMcConfig.IsBypassAll())
		{
			theServo.GetTablePosition(&stPrIndexRefRcd.stVisionBkupAlignPos);
			stCurTblPos = stPrIndexRefRcd.stVisionBkupAlignPos;
		}
		else
		{
			stPrIndexRefRcd.stVisionBkupAlignPos.dx = 40000.0 / theServo.GetResolXY();
			stPrIndexRefRcd.stVisionBkupAlignPos.dy =  8000.0 / theServo.GetResolXY();
		}

		theVisionSys.GetLightingParameter(&(stPrIndexRefRcd.stVisionBkupLightPara.nCoaxRedLight), &nSideRedLight, &(stPrIndexRefRcd.stVisionBkupLightPara.nCoaxBlueLight), &nSideBlueLight);
		stPrIndexRefRcd.stVisionBkupLightPara.bSideRedLight  = (byte)nSideRedLight;
		stPrIndexRefRcd.stVisionBkupLightPara.bSideBlueLight = (byte)nSideBlueLight;

		stPrIndexRefRcd.bIndexPrAvail = true;

		//SAVE RCD
		theDevice.SetIndexPrRef(&stPrIndexRefRcd);
		 		
		theVisionSys.EnableVideo(true);

		if (pSheet->m_iPageMode == WH_EDIT)
			pSheet->GetDlgItem(IDOK)->EnableWindow(0);
		  
		theTopClamp.CloseClamp();
		Sleep_x1ms(100);

		pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_FINISH));
		break;
	}
	case 7:
		//Index one unit to test
		// agree ->finish; disagree -> Redo -> m_nStepId = 0;
        m_szPRAvailble = theMsg.GetMsg(MSG_PGM_EXIST);

		cs.UpdateFinalState(GetDlgItem(IDC_BUTTON_CONTINUE)->m_hWnd,FALSE);

        theTopTrack.SetPrIndexMask(false);
        theVisionDisplay.EnableDisplay(true);

        theApp.ArchivePowerOnData(ARCHIVE_SAVE);
        Sleep_x1ms(10);
        theVisionSys.SavePatternRcd();	//v3.0.T70 edit
        break;
    default:
        break;
    }

	UpdateData(false);
}

BOOL CTopIndexTchPage::OnSetActive()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	if (psheet->m_iPageMode == WH_TEACH)
		psheet->GetDlgItem(ID_WIZNEXT)->EnableWindow(0);

	return CPropertyPage::OnSetActive();
}

LRESULT CTopIndexTchPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W158 add
{
	return m_bBusy==TRUE;
}
