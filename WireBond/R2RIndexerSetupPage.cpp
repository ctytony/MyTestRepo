// R2RIndexerSetupPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "R2RIndexerSetupPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "PopUpTopStepperCtr.h"
#include "MathCalc.h"
#include "TeachWorkHolderSheet.h"
#include "MachineConfig.h"
#include "VisionSystem.h"
#include "TowerLight.h"
#include "TopClamp.h"
#include "R2RIndex.h"
#include "servo.h"

// CR2RIndexerSetupPage dialog

IMPLEMENT_DYNAMIC(CR2RIndexerSetupPage, CPropertyPage)

CR2RIndexerSetupPage::CR2RIndexerSetupPage()
	: CPropertyPage(CR2RIndexerSetupPage::IDD)
	, m_nStep(0)
	, m_nIndexId(0)
	, m_iIndexTchMode(TCHMODE_MANUAL)
{

}

CR2RIndexerSetupPage::~CR2RIndexerSetupPage()
{
}

void CR2RIndexerSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_TCH_INDEX_MODE, m_iIndexTchMode);
}


BEGIN_MESSAGE_MAP(CR2RIndexerSetupPage, CPropertyPage)

	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_START, &CR2RIndexerSetupPage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CR2RIndexerSetupPage::OnBnClickedButtonNext)
//	ON_EN_SETFOCUS(IDC_EDIT_INDEX_LOGIC_ZERO, &CR2RIndexerSetupPage::OnEnSetfocusEditIndexLogicZero)
//	ON_EN_SETFOCUS(IDC_EDIT_INDEX_RIGHT_LIMIT, &CR2RIndexerSetupPage::OnEnSetfocusEditIndexRightLimit)
    ON_BN_CLICKED(IDC_BUTTON_STOP_TCH, &CR2RIndexerSetupPage::OnBnClickedButtonStopTch)
    ON_CBN_SELCHANGE(IDC_COMBO_TCH_INDEX_MODE, &CR2RIndexerSetupPage::OnCbnSelchangeComboTchIndexMode)
    ON_BN_CLICKED(IDC_BUTTON_RESET_OFFSET, &CR2RIndexerSetupPage::OnBnClickedButtonResetOffset)
	ON_LBN_DBLCLK(IDC_LIST_INDEX_ERROR_SHOW, &CR2RIndexerSetupPage::OnLbnDblclkListIndexErrorShow)

END_MESSAGE_MAP()


// CR2RIndexerSetupPage message handlers

HBRUSH CR2RIndexerSetupPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CR2RIndexerSetupPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_NEXT);
    pWnd->EnableWindow(0);
	/*pWnd = this->GetParent()->GetDlgItem(IDOK);
    pWnd->EnableWindow(0);*/

	pWnd = GetDlgItem(IDC_BUTTON_START);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));

	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT));

	pWnd = GetDlgItem(IDC_STATIC_INDEX_TUNE);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INDEX_TUNE));

	pWnd = GetDlgItem(IDC_BUTTON_STOP_TCH);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_STOP));

	pWnd = GetDlgItem(IDC_BUTTON_RESET_OFFSET);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RESET));

	//@ 11 July
	/*pWnd = GetDlgItem(IDC_STATIC_TCH_INDEX_MODE);
	theMsg.GetMsg(MSG_STOP, szString);
	pWnd->SetWindowTextW(szString);*/

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TCH_INDEX_MODE);
    //theMsg.GetMsg(MSG_SLOWEST, szString);
	pComboBox->InsertString(0, _T("Manual"));
	//theMsg.GetMsg(MSG_SLOWER, szString);
	pComboBox->InsertString(1, _T("Auto"));

	m_iIndexTchMode = 0;

	pWnd = GetDlgItem(IDC_BUTTON_DUMMMY);
    pWnd->SetWindowPos(NULL, 1000, 1000, 100, 100, SWP_NOZORDER);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->ResetContent();
	pListBox->InsertString(0, theMsg.GetMsg(MSG_START_TO_HOME_INDEXER));

	TOP_TRACK_DATA stTrackRcd;
	theR2RTrack.GetTopTrackRcd(&stTrackRcd);

	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);

	short nTotalUints;
	TOP_DEVICE_RECORD stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);
	if (stDeviceRcd.nDeviceColmNum % stDeviceRcd.nWndUnitNum != 0)
		nTotalUints = stDeviceRcd.nDeviceColmNum / stDeviceRcd.nWndUnitNum + 1;
	else
		nTotalUints = stDeviceRcd.nDeviceColmNum / stDeviceRcd.nWndUnitNum;
	
    if (nTotalUints > TOP_MAX_INDEX_TIME)
	{
		CStringList sList;
		sList.AddTail(theMsg.GetMsg(MSG_INDEX_TIME_TOO_LONG));
		theMsg.ShowMsg(ERROR_MSG, sList);
		return true;
	}

	for (int i = 0; i < nTotalUints; i++)
	{
		CString str;
		str.Format(_T("Index Id: %d, error: %d"),  i, stTrackRcd.lIndexError[i]);
		pListData->InsertString(i, str);
	}

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CR2RIndexerSetupPage::OnBnClickedButtonStart()
{
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	theMHS.InitAutoIndex();
	theR2RTrack.SetPrIndexMask(true);
	theMHS.SetLastLf(true);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->EnableWindow(0);

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);

	m_nStep = 0;  //start as step0

    pListBox->InsertString(0, theMsg.GetMsg(MSG_WAIT_INDEXER_HOME));

	theR2RTrack.ResetSlipRcd();
	theServo.MoveBondHeadToSafePos();
	Sleep(10);

	theR2RTrack.MoveToLogicZero();
	Sleep(10);

	pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));

	m_nIndexId = 0;

	pListData->ResetContent();
		
	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
	pWnd->EnableWindow(1);

	if (psheet->m_iPageMode == WH_EDIT)
	{
		pWnd = psheet->GetDlgItem(IDOK);
		pWnd->EnableWindow(0);
	}
	else
	{
		TOP_TRACK_DATA stTrackRcd;
		theR2RTrack.GetTopTrackRcd(&stTrackRcd);
		for (int i=0; i<TOP_MAX_INDEX_TIME; i++)
			stTrackRcd.lIndexError[i]= 0;
		theR2RTrack.UpdateTopTrackRcd(&stTrackRcd);
	}
}

void CR2RIndexerSetupPage::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	long lPos, lDest, lLastTime, lElapseTime;
	short nEvent =1;
	INDEX_PR_REF stPrIndexRefRcd;
	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

	TBL_POS stCurTblPos;
	short nSideRedLight, nSideBlueLight;
	short nPatRcd = 0;

	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();
	
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);

	TOP_TRACK_DATA stTrackRcd;
	theR2RTrack.GetTopTrackRcd(&stTrackRcd);

	m_nStep++;

	//------------index lead frame , and  PR offset----------------------------------------------------
	//Auto and is  step 2
	if(m_iIndexTchMode==TCHMODE_AUTO && m_nStep==2)
	{
		
		    theVisionSys.EnableVideo(false);
			theVisionSys.GrabImage();

			theVisionSys.LearnVisionPattern(false, 120, 120, &nPatRcd, true);

			theVisionSys.ClearPatternRcd(
					(short)stPrIndexRefRcd.ulVisionAlignPatternID);

			stPrIndexRefRcd.ulVisionAlignPatternID = nPatRcd;

			theVisionSys.SetPassScore(nPatRcd, thePgm.GetRcd()->nLeadPrPassScore);

		    if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
			{
				theServo.GetTablePosition(&(stPrIndexRefRcd.stVisionAlignPos));
				stCurTblPos = stPrIndexRefRcd.stVisionAlignPos;
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

		
		return;
	}

	//------------INDEX----------------------------------------------------
    //manual  as  >= step 1
    theMHS.SetAutoIndex(true);
    theMHS.SetDryRun(false);
	Sleep(10);

	theServo.MoveBondHeadToSafePos();
    Sleep(100);

    if (theR2RTrack.GetTopTrackEvent() == INDEX_EVENT_IDLE)
		theR2RTrack.SubmitIndexEvent(INDEX_EVENT_SINGLE_INDEX);
  
	Sleep(100);

	lLastTime = GetCurrentTime();

		while(nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theR2RTrack.GetTopTrackEvent();

            //nMhsErrStatus = theMHS.ShowError();//release later
            //if (nMhsErrStatus != FOK)
            //    break;

            lElapseTime=::GetCurrentTime()-lLastTime;
            if (lElapseTime > 20000)
                break;

            Sleep(1);
		}
		 			
		theMHS.SetAutoIndex(false);

	    //------------Adjust----------------------------------------------------------
        //manual  as  >= step 2
		long lOffset;
         
		//auto and as >= step3
		if(m_iIndexTchMode==TCHMODE_AUTO && m_nStep>2)
		{
			int iReturnStatus, iBusyState;
	        

			iReturnStatus = theR2RTrack.CaculatePrIndexOffset(&lOffset);
			Sleep(10);

			if(iReturnStatus == FOK)
			{
				theR2RTrack.TargetClaw(RELAY_ON);
				theTopClamp.PreCloseClamp();

			   	Sleep(400);
				theStepper.MoveStep(TOP_DEVICE_INDEXER, round((double)lOffset/theStepper.GetResol(TOP_DEVICE_INDEXER)), MOTION_RELATIVE);

				do
				{
					iBusyState = (int)theStepper.IsBusy(TOP_DEVICE_INDEXER);
					Sleep(2);
				} while (iBusyState != 0);

				Sleep(100);

				theR2RTrack.TargetClaw(RELAY_OFF);

    		}
			else
			{
				theServo.MoveBondHeadToSafePos();
                Sleep(10);

				theTowerLight.EnableIdleLight(); 
				theStepper.GetPos(TOP_DEVICE_INDEXER,&lPos);
				theR2RTrack.TargetClaw(RELAY_ON);
				Sleep(100);

				theTopClamp.MoveClampSafePos();

		 		CPopUpTopStepperCtr theStepperCtrler(TOP_DEVICE_INDEXER, SPEED_SMALL);
				theStepperCtrler.DoModal();

				theTopClamp.CloseClamp();
				theR2RTrack.TargetClaw(RELAY_OFF);

				theStepper.GetPos(TOP_DEVICE_INDEXER,&lDest);

				lOffset = round((double)(lDest - lPos)* theStepper.GetResol(TOP_DEVICE_INDEXER));
			}
		}
		else
		{
			theStepper.GetPos(TOP_DEVICE_INDEXER,&lPos);

			theR2RTrack.TargetClaw(RELAY_ON);
			Sleep(100);
			theTopClamp.PreCloseClamp();

		 	CPopUpTopStepperCtr theStepperCtrler(TOP_DEVICE_INDEXER, SPEED_SMALL);
	        theStepperCtrler.DoModal();

			theTopClamp.CloseClamp();
			theR2RTrack.TargetClaw(RELAY_OFF);

			theStepper.GetPos(TOP_DEVICE_INDEXER,&lDest);


			lOffset = round((double)(lDest - lPos)* theStepper.GetResol(TOP_DEVICE_INDEXER));
		}


	    //-----------Update records----------------------------------------------------
		//manual  as  >= step 1
			stTrackRcd.lIndexError[m_nIndexId] = stTrackRcd.lIndexError[m_nIndexId] + (short) lOffset;

            theR2RTrack.TailPosUpdate(lOffset);//must do , because lead frame is moved already

			CString czString;
			czString.AppendFormat(_T("Index Id: %d, error: %d. Updated!"),  m_nIndexId, stTrackRcd.lIndexError[m_nIndexId]);
			pListData->InsertString(m_nIndexId, czString);

		    theR2RTrack.UpdateTopTrackRcd(&stTrackRcd);
		    m_nIndexId ++;  // pointer to window clamp

			if (m_iIndexTchMode == TCHMODE_MANUAL)
	        {
				pListBox->InsertString(0, theMsg.GetMsg(MSG_NEXT_TO_INDEX_AND_TUNE));
				pListBox->InsertString(1, theMsg.GetMsg(MSG_DONE_TO_QUIT));
			}
			else  //MODE at auto, only for step2  only why ?
			{
				if (m_nStep == 1) //set the size 
				{
					//DISPLAY MSG
			        theVisionSys.SetSearchSize(120, 120);

					pListBox->InsertString(0, _T("Adjust Lighting and find a patten"));
					pListBox->InsertString(1, _T("Press Next to record"));
				}
			}

	    //-----------END , one time  only ---------------------------------------------------
			short nTotalUints;

			TOP_DEVICE_RECORD  stDeviceRcd;
	        theDevice.GetTopRcd(&stDeviceRcd);

			if( stDeviceRcd.nDeviceColmNum%stDeviceRcd.nWndUnitNum !=0)
			{
				nTotalUints = stDeviceRcd.nDeviceColmNum/stDeviceRcd.nWndUnitNum +1;
			}
			else
			{
				nTotalUints = stDeviceRcd.nDeviceColmNum/stDeviceRcd.nWndUnitNum;
			}


			if(theR2RTrack.GetCurrentBondUnit() >= nTotalUints-1)
			{
				CWnd* pWnd = GetDlgItem(IDC_BUTTON_NEXT);
				pWnd->EnableWindow(FALSE);

				pListBox->ResetContent();
                pListBox->InsertString(0, theMsg.GetMsg(MSG_DONE_TO_QUIT));

				//theMHS.InitAutoIndex();

				theTopClamp.CloseClamp();

                theApp.ArchiveMachineData(ARCHIVE_SAVE);
				Sleep(100);
				TOP_DEVICE_RECORD  stDeviceRcd;
				theDevice.GetTopRcd(&stDeviceRcd);
				theR2RTrack.RefreshMeasurementData(&stDeviceRcd);

				theR2RTrack.SetPrIndexMask(false);
				theMHS.SetLastLf(false);

				if (psheet->m_iPageMode == WH_TEACH)
				{
					CWnd* pWnd = psheet->GetDlgItem(ID_WIZNEXT);
					pWnd->EnableWindow(1);
				}
				else
				{

					CWnd* pWnd = psheet->GetDlgItem(IDOK);
					pWnd->EnableWindow(1);
				}
			}


}


BOOL CR2RIndexerSetupPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	if (psheet->m_iPageMode == WH_TEACH)
	{
		CWnd* pWnd = psheet->GetDlgItem(ID_WIZNEXT);
        pWnd->EnableWindow(0);
	}
	


	return CPropertyPage::OnSetActive();
}

void CR2RIndexerSetupPage::OnBnClickedButtonStopTch()
{
	// TODO: Add your control notification handler code here
	CTeachWorkHolderSheet* psheet = (CTeachWorkHolderSheet*)GetParent();

	if (psheet->m_iPageMode == WH_EDIT)
	{
		CWnd* pWnd = psheet->GetDlgItem(IDOK);
		pWnd->EnableWindow(1);
	}

}


void CR2RIndexerSetupPage::OnBnClickedButtonResetOffset()
{
	int i;
	CString czString;

	TOP_TRACK_DATA stTrackRcd;
	theR2RTrack.GetTopTrackRcd(&stTrackRcd);

	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
    pListData->ResetContent();


	short nTotalUints;

	TOP_DEVICE_RECORD  stDeviceRcd;
	theDevice.GetTopRcd(&stDeviceRcd);

	if( stDeviceRcd.nDeviceColmNum%stDeviceRcd.nWndUnitNum !=0)
	{
		nTotalUints = stDeviceRcd.nDeviceColmNum/stDeviceRcd.nWndUnitNum +1;
	}
	else
	{
		nTotalUints = stDeviceRcd.nDeviceColmNum/stDeviceRcd.nWndUnitNum;
	}

	for( i=nTotalUints-1; i > -1; i--)
	{
		 stTrackRcd.lIndexError[i]=0;
         czString.Empty();
		 czString.AppendFormat(_T("Index Id: %d, error: %d. Updated!"),  i, stTrackRcd.lIndexError[i]);
		 pListData->InsertString(m_nIndexId, czString);

	}
	theR2RTrack.UpdateTopTrackRcd(&stTrackRcd);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->EnableWindow(1);
    pWnd = GetDlgItem(IDC_BUTTON_NEXT);
    pWnd->EnableWindow(0);

}


void CR2RIndexerSetupPage::OnCbnSelchangeComboTchIndexMode()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

}

void CR2RIndexerSetupPage::OnLbnDblclkListIndexErrorShow()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox* pListData = (CListBox*)GetDlgItem(IDC_LIST_INDEX_ERROR_SHOW);
	int index = pListData->GetCurSel();
	long lIndexError=0;

	if (index > -1 && index < 16)
	{
		TOP_TRACK_DATA stTrackRcd;
		theTopTrack.GetRcd(&stTrackRcd);

		CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMMY);
		pWnd->SetFocus();

		theKeyPad.GetUserKeyInData(&lIndexError, stTrackRcd.lIndexError[index], -35000, 35000, true);

		stTrackRcd.lIndexError[index] = lIndexError;

		pListData->DeleteString(index);
		CString czString;
		czString.AppendFormat(_T("Index Id: %d, error: %d. Changed!"),  index, stTrackRcd.lIndexError[index]);
		pListData->InsertString(index, czString);

		theTopTrack.SetRcd(&stTrackRcd);

		TOP_DEVICE_RECORD stTopDevicePara;
		theDevice.GetTopRcd(&stTopDevicePara);
		theTopTrack.RefreshMeasurementData(&stTopDevicePara);
	}
}