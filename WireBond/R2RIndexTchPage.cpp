#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "Program.h"
#include "MachineData.h"
#include "R2RIndexTchPage.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "VisionSystem.h"
#include "MaterialHandlerThread.h"
#include "MachineConfig.h"
#include "MathCalc.h"
#include "PopUpTopStepperCtr.h"
#include "TeachWorkHolderSheet.h"
#include "ScanKeyThread.h"
#include "TopClamp.h"
#include "R2RIndex.h"
#include "servo.h"

// CR2RIndexTchPage dialog

IMPLEMENT_DYNAMIC(CR2RIndexTchPage, CPropertyPage)

CR2RIndexTchPage::CR2RIndexTchPage()
	: CPropertyPage(CR2RIndexTchPage::IDD)
	, m_nPatternSizeX(120)
	, m_nPatternSizeY(120)
	, m_nStepId(0)
	, m_szPRAvailble(_T(""))
	, m_bTchNewActived(false)
{

}

CR2RIndexTchPage::~CR2RIndexTchPage()
{
}

void CR2RIndexTchPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, m_nPatternSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_nPatternSizeY);
	DDX_Text(pDX, IDC_STATIC_AVAILABLE, m_szPRAvailble);
}


BEGIN_MESSAGE_MAP(CR2RIndexTchPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_SIZE_X, &CR2RIndexTchPage::OnEnChangeEditSizeX)
	ON_EN_CHANGE(IDC_EDIT_SIZE_Y, &CR2RIndexTchPage::OnEnChangeEditSizeY)
	ON_EN_SETFOCUS(IDC_EDIT_SIZE_X, &CR2RIndexTchPage::OnEnSetfocusEditSizeX)
	ON_EN_SETFOCUS(IDC_EDIT_SIZE_Y, &CR2RIndexTchPage::OnEnSetfocusEditSizeY)
	ON_BN_CLICKED(IDC_BUTTON_START, &CR2RIndexTchPage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, &CR2RIndexTchPage::OnBnClickedButtonContinue)
	ON_BN_CLICKED(IDC_BUTTON_RESET_PR, &CR2RIndexTchPage::OnBnClickedButtonResetPr)
END_MESSAGE_MAP()


// CR2RIndexTchPage message handlers

void CR2RIndexTchPage::OnBnClickedButtonResetPr()
{
	short nMsgNum, anMsgArray[10];
    long lStopSelected;

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();
	if (psheet->m_iPageMode == WH_EDIT)
    {
		nMsgNum = 1;
        anMsgArray[0] = MSG_DELETE_PR_INDEX_DATA;
        lStopSelected = theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray, true);
		if(lStopSelected == SHOW_MSG_RESULT_STOP)
		return;
	}

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	if (stPrIndexRefRcd.bIndexPrAvail)
		theVisionSys.ClearAllIndexPrPattern();

	theDevice.DeleteIndexPrRef();
	m_nStepId = 0;
	m_bTchNewActived = true;

	m_szPRAvailble = theMsg.GetMsg(MSG_NO_RECIPE);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
	pWnd->EnableWindow(1);
	pWnd = GetDlgItem(IDC_BUTTON_CONTINUE);
	pWnd->EnableWindow(0);

	UpdateData(false);
}

BOOL CR2RIndexTchPage::OnInitDialog()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();
	theApp.SetDlgFont(this);	//V1.1.W139 add

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));

	pWnd = GetDlgItem(IDC_BUTTON_RESET_PR);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TCH_NEW));

	pWnd = GetDlgItem(IDC_STATIC_PGM_AVAIL);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PROGRAM));

	pWnd = GetDlgItem(IDC_STATIC_STATUS);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PR_INDEX));

	pWnd = GetDlgItem(IDC_STATIC_PARA);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SETTING));

	pWnd = GetDlgItem(IDC_STATIC_SIZE_X);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PATTERN_SIZE_X));

    pWnd = GetDlgItem(IDC_STATIC_SIZE_Y);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PATTERN_SIZE_Y));
	
	if (stPrIndexRefRcd.bIndexPrAvail == false)
		m_szPRAvailble = theMsg.GetMsg(MSG_NO_RECIPE);
	else
		m_szPRAvailble = theMsg.GetMsg(MSG_PGM_EXIST);

	pWnd = GetDlgItem(IDC_BUTTON_CONTINUE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT));
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BTN_DUMMY);
	pWnd->SetWindowPos(NULL, 1000, 1000, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
	pListBox->InsertString(0, theMsg.GetMsg(MSG_START_TO_TCH_INDEX_PR));

	if (psheet->m_iPageMode == WH_TEACH)
    {
        psheet->SetWizardButtons(PSWIZB_FINISH);
        pWnd = psheet->GetDlgItem(ID_WIZFINISH);
        pWnd->EnableWindow(0);

		pWnd = GetDlgItem(IDC_BUTTON_START);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_BUTTON_RESET_PR);
		pWnd->EnableWindow(FALSE);
    }
	else
		pListBox->InsertString(1, theMsg.GetMsg(MSG_TCH_NEW_TO_INDEX_LF));

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CR2RIndexTchPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	// TODO:  Change any attributes of the DC here
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CR2RIndexTchPage::OnEnChangeEditSizeX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (UpdateData(true) != FALSE)
        theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
}

void CR2RIndexTchPage::OnEnChangeEditSizeY()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (UpdateData(true) != FALSE)
        theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
}

void CR2RIndexTchPage::OnEnSetfocusEditSizeX()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BTN_DUMMY);
    pWnd->SetFocus();
       
    theKeyPad.GetUserKeyInData(&m_nPatternSizeX, 0,0, 256);

    UpdateData(false);
    
    OnEnChangeEditSizeX();
}

void CR2RIndexTchPage::OnEnSetfocusEditSizeY()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BTN_DUMMY);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nPatternSizeY, 0,0, 256);

    UpdateData(false);

    OnEnChangeEditSizeY();

}

void CR2RIndexTchPage::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	//Indexer clear track, pre open clamp,heat blk, 
	//INFO: 

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	if(m_bTchNewActived ==false) //pointer to index a new window
	{

		m_nStepId = 1;
				
	}
	else
	{

		if(stPrIndexRefRcd.bIndexPrAvail == true)
		{		
			theVisionSys.ClearAllIndexPrPattern();
			
		}

		theDevice.DeleteIndexPrRef();

		m_nStepId = 0;
	

	}
	
	
	theR2RTrack.SetPrIndexMask(true);
	theVisionDisplay.EnableDisplay(false);


	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();


	CWnd *pWnd =GetDlgItem(IDC_BUTTON_START);
	pWnd->EnableWindow(FALSE);

	pWnd =GetDlgItem(IDC_BUTTON_CONTINUE);
	pWnd->EnableWindow(FALSE);

	if (m_bTchNewActived ==false)
	{
		if(psheet->m_iPageMode == WH_EDIT)
		{
			pWnd = psheet->GetDlgItem(IDOK);
			pWnd->EnableWindow(0);
		}
		
		/*pLfClamp->TriggerPrecloseClamper();
	    Sleep(200);*/
		pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_ADJ_INDEX_OFFSET));
	}
	else
	{
/*w.tempx
		 theMHS.ClearTrack();

		 Sleep(100);


		 theR2ROnloader.SubmitOnloadEvent(ONLOAD_EVENT_START);
		 Sleep(10);

		 lLastTime=GetCurrentTime();

		 do
		 {
			 theR2ROnloader.ProcessOnloadEvent();

			 lElapseTime=::GetCurrentTime()-lLastTime;
			 if (lElapseTime > 10000) 
				 break;

			 Sleep(2);
		 }
		 while( theR2ROnloader.GetOnloadEvent()!= TOP_ONLOAD_EVENT_IDLE);
*/
		pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));
	}

	pWnd = GetDlgItem(IDC_BUTTON_CONTINUE);
	pWnd->EnableWindow(TRUE);
}

void CR2RIndexTchPage::OnBnClickedButtonContinue()
{
	// TODO: Add your control notification handler code here

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();


	m_nStepId++;

	TBL_POS stCurTblPos;

	int iBusy =1;
	short nPatRcd = 0;
    short nSideRedLight, nSideBlueLight;
	int iErrorStatus = FOK;
	short nEvent =1;
	long  lLastTime, lElapseTime;
	TBL_POS stMovePos;

	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	CPopUpTopStepperCtr theStepperCtrler(TOP_DEVICE_INDEXER, SPEED_SMALL);

	switch(m_nStepId)
	{
	case 1://index a new window
		{
		theMHS.InitAutoIndex();
		Sleep(100);

        theR2RTrack.TargetClaw(RELAY_ON);
        Sleep(50);

		theServo.MoveBondHeadToSafePos();
        Sleep(100);

		theMHS.SetAutoIndex(true);
		theMHS.SetDryRun(false);
		Sleep(10);

		theMHS.SetRequestIndex(true);
		Sleep(100);

		lLastTime=GetCurrentTime();

        while(nEvent != INDEX_EVENT_IDLE)
		{
			nEvent = theR2RTrack.GetTopTrackEvent();

			//nMhsErrStatus = theMHS.ShowError(); //release later
			//if (nMhsErrStatus != FOK)
			//    break;

			lElapseTime=::GetCurrentTime()-lLastTime;
			if (lElapseTime > 20000) 
				break;

			Sleep(1);
		}
		 			

		theMHS.SetAutoIndex(false);

        theR2RTrack.TargetClaw(RELAY_OFF);
        Sleep(50);

		pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_ADJ_LF_POS));

		}
		break;

	case 2:
		//find the left bottom coner  of the window
		//adjust the device by indexer

		//theMsg.GetMsg(MSG_DONE_TO_CONFIRM_FINAL_POS, szString);
		//pListBox->InsertString(0, szString);
		
		theTopClamp.PreCloseClamp();
		Sleep(200);
		
		theR2RTrack.TargetClaw(RELAY_ON);	
	    theStepperCtrler.DoModal();

		theR2RTrack.TargetClaw(RELAY_OFF);
		theTopClamp.CloseClamp();

		pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
		break;
	case 3://move to PR point
		// edit mode, and is not a new index leadframe
		if (psheet->m_iPageMode == WH_EDIT && m_bTchNewActived == false)
		{
			stMovePos = stPrIndexRefRcd.stManuAlignPos; 
			m_nPatternSizeX = stPrIndexRefRcd.nManuSrchSizeX;
			m_nPatternSizeY = stPrIndexRefRcd.nManuSrchSizeY;
			theServo.MoveTable2(stMovePos, true, false);	//v3.1.T211 edit
        
            theVisionSys.UpdateLightingParameter(
									stPrIndexRefRcd.stManuAlignLightPara.nCoaxRedLight, 
									stPrIndexRefRcd.stManuAlignLightPara.bSideRedLight, 
									stPrIndexRefRcd.stManuAlignLightPara.nCoaxBlueLight,
									stPrIndexRefRcd.stManuAlignLightPara.bSideBlueLight);
			theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);			
		}
		else
		{
			theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
		}

		pListBox->InsertString(0, theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
		pListBox->InsertString(1, theMsg.GetMsg(MSG_TEACH_ALIGN_POINT_COLON));
		pListBox->InsertString(2, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
		break;
	case 4://adjust lighting, find the manu alignment of PR point
		   // move to align point, snap a PR ID
			stPrIndexRefRcd.nManuSrchSizeX = m_nPatternSizeX;
			stPrIndexRefRcd.nManuSrchSizeY = m_nPatternSizeY;
					
			theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();

			iErrorStatus = theVisionSys.LearnVisionPattern(false, m_nPatternSizeX, m_nPatternSizeY, &nPatRcd, true);

			if (iErrorStatus == FOK)
			{
				if(psheet->m_iPageMode == WH_EDIT && m_bTchNewActived ==false)
					theVisionSys.ClearPatternRcd(
						(short)stPrIndexRefRcd.ulManuAlignPatternID);

				stPrIndexRefRcd.ulManuAlignPatternID = nPatRcd;
			}
			
			theVisionSys.GetLightingParameter(&(stPrIndexRefRcd.stManuAlignLightPara.nCoaxRedLight), 
					&nSideRedLight, 
					&(stPrIndexRefRcd.stManuAlignLightPara.nCoaxBlueLight),
					&nSideBlueLight);

			stPrIndexRefRcd.stManuAlignLightPara.bSideRedLight  = (byte)nSideRedLight;
			stPrIndexRefRcd.stManuAlignLightPara.bSideBlueLight = (byte)nSideBlueLight;

		    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
			{
				theServo.GetTablePosition(&stPrIndexRefRcd.stManuAlignPos);
				stPrIndexRefRcd.nManuAlignMag = 0;
			}
			else
			{
				stPrIndexRefRcd.stManuAlignPos.dx = 40000.0 / theServo.GetResolXY();
				stPrIndexRefRcd.stManuAlignPos.dy =  8000.0 / theServo.GetResolXY();
			}

			stPrIndexRefRcd.stVisionLightPara = stPrIndexRefRcd.stManuAlignLightPara;

			//SAVE RCD
			theDevice.SetIndexPrRef(&stPrIndexRefRcd);

			theVisionSys.EnableVideo(true);

			if(psheet->m_iPageMode == WH_EDIT && m_bTchNewActived ==false)
			{
				stMovePos = stPrIndexRefRcd.stVisionAlignPos; 
				m_nPatternSizeX = stPrIndexRefRcd.nVisionSrchSizeX;
				m_nPatternSizeY = stPrIndexRefRcd.nVisionSrchSizeY;
				
				theServo.MoveTable2(stMovePos, true, false);	//v3.1.T211 edit
				theVisionSys.UpdateLightingParameter(
											stPrIndexRefRcd.stVisionLightPara.nCoaxRedLight, 
											stPrIndexRefRcd.stVisionLightPara.bSideRedLight, 
											stPrIndexRefRcd.stVisionLightPara.nCoaxBlueLight,
											stPrIndexRefRcd.stVisionLightPara.bSideBlueLight);
				theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
			}
			else
			{
				 theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
			}

		    pListBox->InsertString(0, theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
		    pListBox->InsertString(1, theMsg.GetMsg(MSG_TEACH_VISION_PATTERN));
	    	pListBox->InsertString(2, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
		break;
	case 5:
			//adjust lighting, find the area of PR pattern
			//verification, approval go; reject, m_nStepId--
		
			stPrIndexRefRcd.nVisionSrchSizeX = m_nPatternSizeX;
			stPrIndexRefRcd.nVisionSrchSizeY = m_nPatternSizeY;

			theVisionSys.SetSearchSize(stPrIndexRefRcd.nVisionSrchSizeX, stPrIndexRefRcd.nVisionSrchSizeY);
			theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();

			iErrorStatus = theVisionSys.LearnVisionPattern(false, m_nPatternSizeX, m_nPatternSizeY, &nPatRcd, true);

			if (iErrorStatus == FOK)
			{
				if(psheet->m_iPageMode == WH_EDIT && m_bTchNewActived ==false)
					theVisionSys.ClearPatternRcd(
						(short)stPrIndexRefRcd.ulVisionAlignPatternID);

				stPrIndexRefRcd.ulVisionAlignPatternID = nPatRcd;
			}

			theVisionSys.SetPassScore(nPatRcd, thePgm.GetRcd()->nLeadPrPassScore);

		    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
			{
				theServo.GetTablePosition(&(stPrIndexRefRcd.stVisionAlignPos));
				stCurTblPos = stPrIndexRefRcd.stVisionAlignPos;
				stPrIndexRefRcd.nVisionAlignMag = 0;
			}
			else
			{
				stPrIndexRefRcd.stVisionAlignPos.dx = 40000.0 / theServo.GetResolXY();
				stPrIndexRefRcd.stVisionAlignPos.dy =  8000.0 / theServo.GetResolXY();
			}

			theVisionSys.GetLightingParameter(&(stPrIndexRefRcd.stVisionLightPara.nCoaxRedLight), 
					&nSideRedLight, 
					&(stPrIndexRefRcd.stVisionLightPara.nCoaxBlueLight),
					&nSideBlueLight);
			stPrIndexRefRcd.stVisionLightPara.bSideRedLight  = (byte)nSideRedLight;
			stPrIndexRefRcd.stVisionLightPara.bSideBlueLight = (byte)nSideBlueLight;

			stPrIndexRefRcd.bIndexPrAvail = true;


			//SAVE RCD
            theDevice.SetIndexPrRef(&stPrIndexRefRcd);

            theVisionSys.EnableVideo(true);

            if (1) //Temperory disable index backup PR to protect Cu kit. //@1
            {      //why to backup a PR ??

                if (psheet->m_iPageMode == WH_EDIT)
                {
                    pWnd = psheet->GetDlgItem(IDOK);
                    pWnd->EnableWindow(0);
                }

                theTopClamp.CloseClamp();

                Sleep(100);

                pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_FINISH));

                m_nStepId++;
            }
            else
            {
                if(psheet->m_iPageMode == WH_EDIT && m_bTchNewActived ==false)
                {
                    stMovePos = stPrIndexRefRcd.stVisionBkupAlignPos; 
                    m_nPatternSizeX = stPrIndexRefRcd.nBkupAlignSrchSizeX;
                    m_nPatternSizeY = stPrIndexRefRcd.nBkupAlignSrchSizeY;

					theServo.MoveTable2(stMovePos, true, false);	//v3.1.T211 edit
                    theVisionSys.UpdateLightingParameter(
									stPrIndexRefRcd.stVisionBkupLightPara.nCoaxRedLight, 
									stPrIndexRefRcd.stVisionBkupLightPara.bSideRedLight, 
									stPrIndexRefRcd.stVisionBkupLightPara.nCoaxBlueLight,
									stPrIndexRefRcd.stVisionBkupLightPara.bSideBlueLight);
                    theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
                }
                else
                {
                    theVisionSys.SetSearchSize(m_nPatternSizeX, m_nPatternSizeY);
                }

                pListBox->InsertString(0, theMsg.GetMsg(MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE));
                pListBox->InsertString(1, theMsg.GetMsg(MSG_ARRANGE_BACKUP_PR_AREA));
                pListBox->InsertString(2, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
            }
		break;

	case 6:
			//adjust lighting, find the area of BACK-UP PR pattern
			//verification, approval go; reject, m_nStepId--

			stPrIndexRefRcd.nBkupAlignSrchSizeX = m_nPatternSizeX;
			stPrIndexRefRcd.nBkupAlignSrchSizeY = m_nPatternSizeY;

			theVisionSys.SetSearchSize(stPrIndexRefRcd.nBkupAlignSrchSizeX, stPrIndexRefRcd.nBkupAlignSrchSizeY);
			theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();

			theVisionSys.LearnVisionPattern(false, m_nPatternSizeX, m_nPatternSizeY, &nPatRcd, true);
			if(psheet->m_iPageMode == WH_EDIT && m_bTchNewActived ==false)
				theVisionSys.ClearPatternRcd(
					(short)stPrIndexRefRcd.ulVisionBkupPatternID);

			stPrIndexRefRcd.ulVisionBkupPatternID = nPatRcd;

			theVisionSys.SetPassScore(nPatRcd, thePgm.GetRcd()->nLeadPrPassScore);

		    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
			{
				theServo.GetTablePosition(&stPrIndexRefRcd.stVisionBkupAlignPos);
				stCurTblPos = stPrIndexRefRcd.stVisionBkupAlignPos;
			}
			else
			{
				stPrIndexRefRcd.stVisionBkupAlignPos.dx = 40000.0 / theServo.GetResolXY();
				stPrIndexRefRcd.stVisionBkupAlignPos.dy =  8000.0 / theServo.GetResolXY();
			}

			theVisionSys.GetLightingParameter(&(stPrIndexRefRcd.stVisionBkupLightPara.nCoaxRedLight), 
					&nSideRedLight, 
					&(stPrIndexRefRcd.stVisionBkupLightPara.nCoaxBlueLight),
					&nSideBlueLight);
			stPrIndexRefRcd.stVisionBkupLightPara.bSideRedLight  = (byte)nSideRedLight;
			stPrIndexRefRcd.stVisionBkupLightPara.bSideBlueLight = (byte)nSideBlueLight;

			stPrIndexRefRcd.bIndexPrAvail = true;

			//SAVE RCD
			theDevice.SetIndexPrRef(&stPrIndexRefRcd);
			 		
			theVisionSys.EnableVideo(true);
			

		  if (psheet->m_iPageMode == WH_EDIT)
		  {
			  pWnd = psheet->GetDlgItem(IDOK);
			  pWnd->EnableWindow(0);
		  }

		  
		   theTopClamp.CloseClamp();

		   Sleep(100);

		   pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_FINISH));
		  
	  	break;
	case 7:
		//Index one unit to test
		// agree ->finish; disagree -> Redo -> m_nStepId = 0;

        m_szPRAvailble = theMsg.GetMsg(MSG_PGM_EXIST);

        pWnd = GetDlgItem(IDC_BUTTON_CONTINUE);
        pWnd->EnableWindow(FALSE);

        theR2RTrack.SetPrIndexMask(false);
        theVisionDisplay.EnableDisplay(true);

        theApp.ArchivePowerOnData(ARCHIVE_SAVE);
        Sleep(10);
        theVisionSys.SavePatternRcd();	//v3.0.T70 edit

        if (psheet->m_iPageMode == WH_EDIT)
        {
            pWnd = psheet->GetDlgItem(IDOK);
            pWnd->EnableWindow(1);
        }
        else
        {
            pWnd = psheet->GetDlgItem(ID_WIZFINISH);
            pWnd->EnableWindow(1);
        }
        break;
    default:
        break;
    }

	UpdateData(false);
}

BOOL CR2RIndexTchPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	if (psheet->m_iPageMode == WH_EDIT)
	{

	}
	else
	{
		
		CWnd* pWnd = psheet->GetDlgItem(ID_WIZNEXT);
        pWnd->EnableWindow(0);
	}

    theScanKeyThread.SetJoyStickConfig(JOYSTICK_NONE, 0);//v12.19.1.w35

	return CPropertyPage::OnSetActive();
}


BOOL CR2RIndexTchPage::OnKillActive()
{

	theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);//v12.19.1.w35

	return ( CPropertyPage::OnKillActive());

}