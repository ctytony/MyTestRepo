// DlgElevCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "DlgCuEliteElevCtrl.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "StepperControl.h"
#include "MaterialHandlerThread.h"
#include "MachineConfig.h"
#include "CuIndex.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CDlgCuEliteElevCtrl dialog

IMPLEMENT_DYNAMIC(CDlgCuEliteElevCtrl, CDialog)

CDlgCuEliteElevCtrl::CDlgCuEliteElevCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCuEliteElevCtrl::IDD, pParent)
	, m_nCurrentSlot(0)
	, m_nElevId(INPUT_ELEVATOR)
{

}

CDlgCuEliteElevCtrl::~CDlgCuEliteElevCtrl()
{
}

void CDlgCuEliteElevCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CURRENT_SLOT, m_nCurrentSlot);
}


BEGIN_MESSAGE_MAP(CDlgCuEliteElevCtrl, CDialog)
	ON_WM_CTLCOLOR()
	
	ON_EN_SETFOCUS(IDC_EDIT_CURRENT_SLOT, &CDlgCuEliteElevCtrl::OnEnSetfocusEditCurrentSlot)
	ON_BN_CLICKED(IDC_BUTTON_ELEV_INPUT, &CDlgCuEliteElevCtrl::OnBnClickedButtonElevInput)
	ON_BN_CLICKED(IDC_BUTTON_ELEV_OUTPUT, &CDlgCuEliteElevCtrl::OnBnClickedButtonElevOutput)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_SLOT, &CDlgCuEliteElevCtrl::OnEnChangeEditCurrentSlot)
	ON_BN_CLICKED(IDC_BUTTON_UP_SLOT, &CDlgCuEliteElevCtrl::OnBnClickedButtonUpSlot)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_SLOT, &CDlgCuEliteElevCtrl::OnBnClickedButtonDownSlot)
	ON_BN_CLICKED(IDC_BUTTON_ELEV_EJECT, &CDlgCuEliteElevCtrl::OnBnClickedButtonElevEject)
END_MESSAGE_MAP()


// CDlgCuEliteElevCtrl message handlers

HBRUSH CDlgCuEliteElevCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
		   pDC->SetBkColor(RGB(130, 200, 205));
           return m_BkBrush;
    }

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CDlgCuEliteElevCtrl::OnInitDialog()
{
	m_BkBrush.CreateSolidBrush(RGB(130, 200, 205));
    
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect Rect;
	(this->GetParent())->GetWindowRect(&Rect);

    this->SetWindowPos(NULL, Rect.left + 500, Rect.top +250, 285, 380, SWP_NOZORDER);

	m_bitmapUpArrow.LoadBitmapW(IDB_UP_ARROW);
    HBITMAP hBitmapUp = (HBITMAP)m_bitmapUpArrow.GetSafeHandle();

    m_bitmapDownArrow.LoadBitmapW(IDB_DOWN_ARROW);
    HBITMAP hBitmapDown = (HBITMAP)m_bitmapDownArrow.GetSafeHandle();

	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_UP_SLOT);
    pButton->SetBitmap(hBitmapUp);

    pButton = (CButton*)GetDlgItem(IDC_BUTTON_DOWN_SLOT);
    pButton->SetBitmap(hBitmapDown);

	CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DONE));

	pWnd = GetDlgItem(IDC_STATIC_CURRENT_SLOT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CUR_SLOT));

	pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_EJECT));

	pWnd = GetDlgItem(IDC_BUTTON_ELEV_INPUT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_INPUT));
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_ELEV_OUTPUT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OUTPUT));
	pWnd->EnableWindow(TRUE);

	m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();
	m_nElevId = INPUT_ELEVATOR;

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCuEliteElevCtrl::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}



void CDlgCuEliteElevCtrl::OnBnClickedButtonElevInput()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_ELEV_INPUT);
   	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_ELEV_OUTPUT);
   	pWnd->EnableWindow(TRUE);

	m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();
	m_nElevId = INPUT_ELEVATOR;

	UpdateData(false);

	
}

void CDlgCuEliteElevCtrl::OnBnClickedButtonElevOutput()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_ELEV_INPUT);
   	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_BUTTON_ELEV_OUTPUT);
   	pWnd->EnableWindow(FALSE);

	m_nCurrentSlot = theCuEliteOffloader.GetCurrentSlotNum();
	m_nElevId = OUTPUT_ELEVATOR;

	UpdateData(false);

}

void CDlgCuEliteElevCtrl::OnEnSetfocusEditCurrentSlot()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDOK);
    pWnd->SetFocus();

	CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
	theDevice.GetCuEliteRcd(&stCuEliteDevicePara);
    
    theKeyPad.GetUserKeyInData(&m_nCurrentSlot,1,0,stCuEliteDevicePara.lMagSlotNum);

	//OutputMagazineSlotSequence: Standard / Alternate
	CUELITE_TRACK_DATA  stTrackData;
	theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);

	if ( (m_nElevId == OUTPUT_ELEVATOR) && stTrackData.iOutputMagazineSlotSequence > 0 && m_nCurrentSlot % 2 == 0 )
	{
		::MessageBox(NULL,_T("OutputMagazineSlotSequence: Alternate. Please key in an Odd number, 1,3,5,..."),_T("Wrong Input"),MB_OK);
		return;
	}
    UpdateData(false);

	if ( m_nCurrentSlot <= 0 )
	{
		return;
	}

	OnEnChangeEditCurrentSlot();

}

void CDlgCuEliteElevCtrl::OnEnChangeEditCurrentSlot()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	theCuEliteTrack.theInjectSnr.UpdateSensorState();
	theCuEliteTrack.theEjectSnr.UpdateSensorState();

	if (((m_nElevId == INPUT_ELEVATOR) && (theCuEliteTrack.theInjectSnr.GetSensorState() == TRUE)) ||
		((m_nElevId == OUTPUT_ELEVATOR) && (theCuEliteTrack.theEjectSnr.GetSensorState() == TRUE)))
	{ 
		short nMsgNum, anMsgArray[10]; 
		nMsgNum = 1;
		anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;
		theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);
		return ;
	}

	UpdateData(true);
        
	if (m_nElevId == INPUT_ELEVATOR)
	{
		theCuEliteTrack.MoveToLogicZero();
		Sleep(10);
		theCuEliteTrack.CheckAtLogicZeroPos();

		theCuEliteOnloader.MoveTotheSlot(m_nCurrentSlot);
		theCuEliteOnloader.WaitMagHoledrMoveFinish();
	}
	else
	{
		theCuEliteOffloader.MoveTotheSlot(m_nCurrentSlot);
		theCuEliteOffloader.WaitMagHoledrMoveFinish();	
	}
}

void CDlgCuEliteElevCtrl::OnBnClickedButtonUpSlot()
{
	// TODO: Add your control notification handler code here

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
   	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   	pWnd->EnableWindow(FALSE);

	theCuEliteTrack.theInjectSnr.UpdateSensorState();
	theCuEliteTrack.theEjectSnr.UpdateSensorState();

	if (((m_nElevId == INPUT_ELEVATOR) && (theCuEliteTrack.theInjectSnr.GetSensorState() == TRUE)) ||
		((m_nElevId == OUTPUT_ELEVATOR) && (theCuEliteTrack.theEjectSnr.GetSensorState() == TRUE)))
	{ 

		short   nMsgNum, anMsgArray[10]; 

		nMsgNum = 1;
		anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;

		theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

		pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
   		pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   		pWnd->EnableWindow(TRUE);



		return ;

	}

	//if (m_nElevId == INPUT_ELEVATOR)	//v3.3.D235
	//{
	//	int iPusherStatus;
	//	iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
	//	if(iPusherStatus == FNOK)
	//	{
	//		short   nMsgNum, anMsgArray[10]; 

	//		nMsgNum = 1;
	//		anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;

	//		theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

	//		
	//		pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
 //  			pWnd->EnableWindow(TRUE);

	//		pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
 //  			pWnd->EnableWindow(TRUE);


	//		return;

	//	}
	//}
	
	UpdateData(true);

	if (m_nCurrentSlot > 1)  
	{       
		if (m_nElevId == INPUT_ELEVATOR) 
		{
			theCuEliteTrack.MoveToLogicZero();
			Sleep(10);
			theCuEliteTrack.CheckAtLogicZeroPos();
			theCuEliteOnloader.MoveTotheSlot(m_nCurrentSlot - 1);
			theCuEliteOnloader.WaitMagHoledrMoveFinish();
			m_nCurrentSlot = theCuEliteOnloader .GetCurrentSlotNum();
		
		}
		else if (m_nElevId == OUTPUT_ELEVATOR)
		{
			theCuEliteOffloader.MoveTotheSlot(m_nCurrentSlot - 1);
			theCuEliteOffloader.WaitMagHoledrMoveFinish();
			m_nCurrentSlot = theCuEliteOffloader .GetCurrentSlotNum();
		}
	}

	UpdateData(false);

	pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   	pWnd->EnableWindow(TRUE);
}

void CDlgCuEliteElevCtrl::OnBnClickedButtonDownSlot()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
   	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   	pWnd->EnableWindow(FALSE);

    CUELITE_DEVICE_RECORD  stCuEliteDevicePara;
	theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

	theCuEliteTrack.theInjectSnr.UpdateSensorState();
	theCuEliteTrack.theEjectSnr.UpdateSensorState();

	if (((m_nElevId == INPUT_ELEVATOR) && (theCuEliteTrack.theInjectSnr.GetSensorState() == TRUE)) ||
		((m_nElevId == OUTPUT_ELEVATOR) && (theCuEliteTrack.theEjectSnr.GetSensorState() == TRUE)))
	{ 

		short   nMsgNum, anMsgArray[10]; 

		nMsgNum = 1;
		anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;

		theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

		
		pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
   		pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   		pWnd->EnableWindow(TRUE);


		return ;

	}

	//if (m_nElevId == INPUT_ELEVATOR)	//v3.3.D235
	//{
	//	int iPusherStatus;
	//	iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
	//	if(iPusherStatus == FNOK)
	//	{
	//		short   nMsgNum, anMsgArray[10]; 

	//		nMsgNum = 1;
	//		anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;

	//		theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

	//		
	//		pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
 //  			pWnd->EnableWindow(TRUE);

	//		pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
 //  			pWnd->EnableWindow(TRUE);


	//		return;

	//	}
	//}
	

	UpdateData(true);
         
	if(m_nElevId == INPUT_ELEVATOR) 
	{

		theCuEliteTrack.MoveToLogicZero();
		Sleep(10);
		theCuEliteTrack.CheckAtLogicZeroPos();

        if (m_nCurrentSlot+1 > stCuEliteDevicePara.lMagSlotNum)
        {
            theCuEliteOnloader.thePullyMagExistSensor.UpdateSensorState();
		    if(theCuEliteOnloader.thePullyMagExistSensor.GetSensorState() == TRUE)	//check mag full sensor
		    {
			    short   nMsgNum, anMsgArray[10]; 

			    nMsgNum = 1;
			    anMsgArray[0] = MSG_ONLOAD_MAG_PULLY_FULL;

			    theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

			    pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
                pWnd->EnableWindow(TRUE);
				pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
   				pWnd->EnableWindow(TRUE);
				pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   				pWnd->EnableWindow(TRUE);

			    return ;

		    }
            theCuEliteOnloader.EjectOnloadMag();

		    m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();
        }
        else
        {
			theCuEliteOnloader.MoveTotheSlot(m_nCurrentSlot + 1);
			theCuEliteOnloader.WaitMagHoledrMoveFinish();
		    m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();
        }	
	}
	else if (m_nElevId == OUTPUT_ELEVATOR) 
	{
        if (m_nCurrentSlot+1 > stCuEliteDevicePara.lMagSlotNum)
        {
            theCuEliteOffloader.thePullyMagExistSensor.UpdateSensorState();
		    if (theCuEliteOffloader.thePullyMagExistSensor.GetSensorState() == TRUE)	//check mag full sensor
		    {
			    short nMsgNum, anMsgArray[10]; 
			    nMsgNum = 1;
			    anMsgArray[0] = MSG_OFFLOAD_MAG_PULLY_FULL;
			    theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

			    pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
                pWnd->EnableWindow(TRUE);
				pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
   				pWnd->EnableWindow(TRUE);
				pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   				pWnd->EnableWindow(TRUE);

			    return ;
		    }
            theCuEliteOffloader.EjectOffloadMag();

		    m_nCurrentSlot = theCuEliteOffloader.GetCurrentSlotNum();
        }
		else
		{
			//theCuEliteOffloader.MoveTotheSlot(m_nCurrentSlot + 1);
			//OutputMagazineSlotSequence: Standard / Alternate
			CUELITE_TRACK_DATA  stTrackData;
			theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);

			theCuEliteOffloader.MoveTotheSlot(m_nCurrentSlot + 1 + stTrackData.iOutputMagazineSlotSequence);
			theCuEliteOffloader.WaitMagHoledrMoveFinish();

			m_nCurrentSlot = theCuEliteOffloader .GetCurrentSlotNum();
		}
	}
	
	UpdateData(false);

	pWnd = GetDlgItem(IDC_BUTTON_UP_SLOT);
   	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_BUTTON_DOWN_SLOT);
   	pWnd->EnableWindow(TRUE);


}

void CDlgCuEliteElevCtrl::OnBnClickedButtonElevEject()
{
	// TODO: Add your control notification handler code here

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
    pWnd->EnableWindow(FALSE);

    //-check inject sensor or eject senaor slip is there  ----------------------------------
    theCuEliteTrack.theInjectSnr.UpdateSensorState();
    theCuEliteTrack.theEjectSnr.UpdateSensorState();

    if (((m_nElevId == INPUT_ELEVATOR) && (theCuEliteTrack.theInjectSnr.GetSensorState() == TRUE)) ||
        ((m_nElevId == OUTPUT_ELEVATOR) && (theCuEliteTrack.theEjectSnr.GetSensorState() == TRUE)))
    { 
        short   nMsgNum, anMsgArray[10]; 
		
        nMsgNum = 1;
        anMsgArray[0] = MSG_REMOVE_LF_ON_TRACK;

        theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

		pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
        pWnd->EnableWindow(TRUE);


        return ;
    }

    //------------------------------------------------------------

	if(m_nElevId == INPUT_ELEVATOR)
	{
		//check extract pully
		theCuEliteOnloader.thePullyMagExistSensor.UpdateSensorState();
		if(theCuEliteOnloader.thePullyMagExistSensor.GetSensorState() == TRUE)
		{
			short   nMsgNum, anMsgArray[10]; 

			nMsgNum = 1;
			anMsgArray[0] = MSG_ONLOAD_MAG_PULLY_FULL;

			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

			pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
            pWnd->EnableWindow(TRUE);


			return ;


		}

		     //check inject pusher
			int iPusherStatus;
			iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
			if(iPusherStatus == FNOK)
			{
				short   nMsgNum, anMsgArray[10]; 

				nMsgNum = 1;
				anMsgArray[0] = MSG_ONLOAD_PUSH_KICKER_WRONG;

				theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

				pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
                pWnd->EnableWindow(TRUE);


				return;

			}

		

		theCuEliteOnloader.EjectOnloadMag();

		m_nCurrentSlot = theCuEliteOnloader.GetCurrentSlotNum();
		
	}
	else
	{
		//check extract pully  
		theCuEliteOffloader.thePullyMagExistSensor.UpdateSensorState();
		if (theCuEliteOffloader.thePullyMagExistSensor.GetSensorState() == TRUE)
		{
			short nMsgNum, anMsgArray[10];
			nMsgNum = 1;
			anMsgArray[0] = MSG_OFFLOAD_MAG_PULLY_FULL;
			theMsg.ShowMsg(WARNING_MSG, nMsgNum, anMsgArray);

			pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
            pWnd->EnableWindow(TRUE);

			return ;
		}

		theCuEliteOffloader.EjectOffloadMag();
		m_nCurrentSlot = theCuEliteOffloader.GetCurrentSlotNum();
	}

	pWnd = GetDlgItem(IDC_BUTTON_ELEV_EJECT);
   	pWnd->EnableWindow(TRUE);

	UpdateData(false);

}

