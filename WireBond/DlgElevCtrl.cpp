// DlgElevCtrl.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "DeviceData.h"
#include "DlgElevCtrl.h"
#include "MsgDef.h"
#include "StepperControl.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "R2RIndex.h"
#include "TopSteppperPage.h"
#include "WireBondDlg.h"
#include <memory>

extern CJpeg theJpegHelp;

// CDlgElevCtrl dialog
IMPLEMENT_DYNAMIC(CDlgElevCtrl, CDialog)

CDlgElevCtrl::CDlgElevCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgElevCtrl::IDD, pParent)
	, m_nCurSlot(1)
	, m_nElevID(TOP_ELEV_ONLOAD)
{
	m_pbmArrow = NULL;
}

CDlgElevCtrl::~CDlgElevCtrl()
{
	if (m_pbmArrow) delete m_pbmArrow;
}

void CDlgElevCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CURRENT_SLOT, m_nCurSlot);
}

BEGIN_MESSAGE_MAP(CDlgElevCtrl, CDialog)
	ON_WM_CTLCOLOR()	
	ON_EN_SETFOCUS(IDC_EDIT_CURRENT_SLOT, OnFocusCurrentSlot)
	ON_BN_CLICKED(IDC_BUTTON_ELEV_INPUT, OnElevInput)
	ON_BN_CLICKED(IDC_BUTTON_ELEV_OUTPUT, OnElevOutput)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_SLOT, OnEnChangeEditCurrentSlot)
	ON_BN_CLICKED(IDC_BUTTON_UP_SLOT, OnUpSlot)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_SLOT, OnDownSlot)
	ON_BN_CLICKED(IDC_BUTTON_ELEV_EJECT, OnElevEject)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CDlgElevCtrl message handlers
HBRUSH CDlgElevCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
        pDC->SetBkMode(TRANSPARENT);
        return theApp.m_brNull;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CDlgElevCtrl::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWireBondDlg *lpMain=(CWireBondDlg*)theApp.m_pMainWnd;
	m_pbmArrow = lpMain->Load(theApp.m_hInstance,IDR_PNG_ARROW,_T("PNG"));

    SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));
    SetDlgItemTextW(IDC_STATIC_CURRENT_SLOT, theMsg.GetMsg(MSG_CUR_SLOT));
    SetDlgItemTextW(IDC_BUTTON_ELEV_EJECT, theMsg.GetMsg(MSG_EJECT));
    SetDlgItemTextW(IDC_BUTTON_ELEV_INPUT, theMsg.GetMsg(MSG_INPUT));
    SetDlgItemTextW(IDC_BUTTON_ELEV_OUTPUT, theMsg.GetMsg(MSG_OUTPUT));

	m_nCurSlot = theTopOnloader.GetCurSlot();
	m_nElevID = TOP_ELEV_ONLOAD;

	return CDialog::OnInitDialog();
}

void CDlgElevCtrl::ShowErrMsg(short nMsgID)
{
	theMsg.ShowMsg(WARNING_MSG, 1, &nMsgID);
}

//check onload pusher status, if true display error
bool CDlgElevCtrl::IsPusherError()
{
	int iPusherStatus = FOK;
	if (theTopOnloader.m_InjectPusher.theHomeSensor.GetCurrentState() == SENSOR_OFF)
		iPusherStatus = theTopOnloader.m_InjectPusher.TriggerBacktoHome();
	if (iPusherStatus != FNOK) return false;
	ShowErrMsg(MSG_ONLOAD_PUSH_KICKER_WRONG);
	return true;
}

//check whether LF is on the track, if true display error
bool CDlgElevCtrl::IsLeadframeOnTrack(short nElevType)
{
	if (nElevType == TOP_ELEV_ONLOAD)	
	{
		if (theTopTrack.m_InjectSensor.GetCurrentState() == SENSOR_ON)
		{
			ShowErrMsg(MSG_REMOVE_LF_ON_TRACK);			
			return true;
		}
	}
	else if (theTopTrack.m_EjectSensor.GetCurrentState() == SENSOR_ON)
	{
		ShowErrMsg(MSG_REMOVE_LF_ON_TRACK);			
		return true;
	}
	return false;
}

//check whether magazine is below the holder, if true display error
bool CDlgElevCtrl::IsMagBelow(short nElevType)
{
	if (nElevType == TOP_ELEV_ONLOAD)	
	{
		if (theTopOnloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
		{
			ShowErrMsg(MSG_REMOVE_MAG_BELOW_ELEV);
			return true;
		}
	}
	else if (theTopOffloader.m_MagDropSensor.GetCurrentState() == SENSOR_ON)
	{
		ShowErrMsg(MSG_REMOVE_MAG_BELOW_ELEV);			
		return true;
	}
	return false;
}

void CDlgElevCtrl::OnElevInput()
{
	GetDlgItem(IDC_BUTTON_ELEV_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ELEV_OUTPUT)->EnableWindow(TRUE);

	m_nCurSlot = theTopOnloader.GetCurSlot();
	m_nElevID = TOP_ELEV_ONLOAD;

	UpdateData(false);
}

void CDlgElevCtrl::OnElevOutput()
{
	GetDlgItem(IDC_BUTTON_ELEV_INPUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ELEV_OUTPUT)->EnableWindow(FALSE);

	m_nCurSlot = theTopOffloader.GetCurSlot();
	m_nElevID = TOP_ELEV_OFFLOAD;

	UpdateData(false);
}

void CDlgElevCtrl::OnFocusCurrentSlot()
{
	GetDlgItem(IDOK)->SetFocus();

	UpdateData(true);

	long nSlot = 0;

    if (m_nElevID == TOP_ELEV_ONLOAD)
	{
		TOP_ONLOAD_RCD stOnloadData;
		theTopOnloader.GetRcd(&stOnloadData);
		nSlot = stOnloadData.lOnloadSlotNum;
	}
	else
	{
		TOP_OFFLOAD_RCD stOffloadData;
		theTopOffloader.GetRcd(&stOffloadData);
		nSlot = stOffloadData.lOffloadSlotNum;
	}

	if (theKeyPad.GetUserKeyInData(&m_nCurSlot,1,1,nSlot) == CANCEL_SELECTED) return;

	//OutputMagazineSlotSequence: Standard / Alternate
	TOP_TRACK_DATA stTrackData;
	theTopTrack.GetRcd(&stTrackData);
	if ((m_nElevID == TOP_ELEV_OFFLOAD)
		&& (stTrackData.iOutputMagazineSlotSequence > 0)
		&& (m_nCurSlot % 2) == 0)
	{
		MessageBox(_T("OutputMagazineSlotSequence: Alternate. Please key in an Odd number, 1,3,5,..."),_T("Wrong Input"),MB_OK);
		return;
	}

	nSlot = m_nElevID == TOP_ELEV_ONLOAD ? theTopOnloader.GetCurSlot() : theTopOffloader.GetCurSlot();

	if ((m_nCurSlot > nSlot) && IsMagBelow(m_nElevID)) return;
	if (IsLeadframeOnTrack(m_nElevID)) return;
	if ((m_nElevID == TOP_ELEV_ONLOAD) && IsPusherError()) return;

	UpdateData(false);
	if (m_nCurSlot <= 0) return;
	OnEnChangeEditCurrentSlot();
}

void CDlgElevCtrl::OnEnChangeEditCurrentSlot()
{
	UpdateData(true);

	if (m_nElevID == TOP_ELEV_ONLOAD)
	{
		theTopTrack.MoveIndexerToLogicZero();
		Sleep_x1ms(10);
		theTopTrack.WaitMoveIndexer();

		theTopOnloader.MoveMagHolderToSlot(m_nCurSlot);
		theTopOnloader.WaitMoveMagHolder();
	}
	else
	{
		theTopOffloader.MoveMagHolderToSlot(m_nCurSlot);
		theTopOffloader.WaitMoveMagHolder();
	}
}

void CDlgElevCtrl::OnUpSlot()
{
	GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(FALSE);

	if (IsLeadframeOnTrack(m_nElevID))
	{
		GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(TRUE);
		return ;
	}

	if ((m_nElevID == TOP_ELEV_ONLOAD) && IsPusherError())
	{
		GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(TRUE);
		return;
	}

	UpdateData(true);

	if (m_nCurSlot == 1)
	{
		GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(TRUE);
		return;
	}

	if (m_nElevID == TOP_ELEV_ONLOAD)
	{
		theTopTrack.MoveIndexerToLogicZero();
		Sleep_x1ms(10);
		theTopTrack.WaitMoveIndexer();

		theTopOnloader.MoveMagHolderToSlot(m_nCurSlot - 1);
		theTopOnloader.WaitMoveMagHolder();
		m_nCurSlot = theTopOnloader.GetCurSlot();
	}
	else
	{
		theTopOffloader.MoveMagHolderToSlot(m_nCurSlot - 1);
		theTopOffloader.WaitMoveMagHolder();
		m_nCurSlot = theTopOffloader.GetCurSlot();
	}

	UpdateData(false);

	GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(TRUE);
}

void CDlgElevCtrl::OnDownSlot()
{
	GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(FALSE);

	if (IsMagBelow(m_nElevID) || IsLeadframeOnTrack(m_nElevID))
	{
		GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(TRUE);
		return ;
	}
		
	if ((m_nElevID == TOP_ELEV_ONLOAD) && IsPusherError())
	{
		GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(TRUE);
		return;
	}

	UpdateData(true);

	if (m_nElevID == TOP_ELEV_ONLOAD)
	{
		TOP_ONLOAD_RCD stOnloadData;
		theTopOnloader.GetRcd(&stOnloadData);

		theTopTrack.MoveIndexerToLogicZero();
		Sleep_x1ms(10);
		theTopTrack.WaitMoveIndexer();

        if (m_nCurSlot + 1 > stOnloadData.lOnloadSlotNum)
        {
		    if (theTopOnloader.m_PullyMagExistSensor.GetCurrentState() == TRUE)	//check mag full sensor
		    {
				ShowErrMsg(MSG_ONLOAD_MAG_PULLY_FULL);
			    GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(TRUE);
			    return ;
		    }

            theTopOnloader.EjectOnloadMag();
		    m_nCurSlot = theTopOnloader.GetCurSlot();
        }
        else
        {
			theTopOnloader.MoveMagHolderToSlot(m_nCurSlot + 1);
			theTopOnloader.WaitMoveMagHolder();
		    m_nCurSlot = theTopOnloader.GetCurSlot();
        }
	}
	else
	{
		TOP_OFFLOAD_RCD stOffloadData;
		theTopOffloader.GetRcd(&stOffloadData);

		if (m_nCurSlot + 1 > stOffloadData.lOffloadSlotNum)
		{
			if(theTopOffloader.m_PullyMagExistSensor.GetCurrentState() == TRUE)
			{
				ShowErrMsg(MSG_OFFLOAD_MAG_PULLY_FULL);
				GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(TRUE);
				return ;
			}
			theTopOffloader.EjectOffloadMag();

			m_nCurSlot = theTopOffloader.GetCurSlot();
		}
		else
		{
			//OutputMagazineSlotSequence: Standard / Alternate
			TOP_TRACK_DATA stTrackData;
			theTopTrack.GetRcd(&stTrackData);

			theTopOffloader.MoveMagHolderToSlot(m_nCurSlot + 1 + stTrackData.iOutputMagazineSlotSequence);
			theTopOffloader.WaitMoveMagHolder();

			m_nCurSlot = theTopOffloader.GetCurSlot();
		}	
	}

	UpdateData(false);

	GetDlgItem(IDC_BUTTON_UP_SLOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DOWN_SLOT)->EnableWindow(TRUE);
}

void CDlgElevCtrl::OnElevEject()
{
    GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(FALSE);

	if (IsMagBelow(m_nElevID) || IsLeadframeOnTrack(m_nElevID))
	{
		GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(TRUE);
        return ;
    }

	if (m_nElevID == TOP_ELEV_ONLOAD)
	{
		if (theTopOnloader.m_PullyMagExistSensor.GetCurrentState() == TRUE)
		{
			ShowErrMsg(MSG_ONLOAD_MAG_PULLY_FULL);
			GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(TRUE);
			return ;
		}

		if ((m_nElevID == TOP_ELEV_ONLOAD) && IsPusherError())
		{
			GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(TRUE);
			return;
		}

		theTopOnloader.EjectOnloadMag();
		m_nCurSlot = theTopOnloader.GetCurSlot();
	}
	else
	{
		if (theTopOffloader.m_PullyMagExistSensor.GetCurrentState() == TRUE)
		{
			ShowErrMsg(MSG_OFFLOAD_MAG_PULLY_FULL);
			GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(TRUE);
			return ;
		}

		theTopOffloader.EjectOffloadMag();
		m_nCurSlot = theTopOffloader.GetCurSlot();		
	}

	GetDlgItem(IDC_BUTTON_ELEV_EJECT)->EnableWindow(TRUE);

	UpdateData(false);
}

BOOL CDlgElevCtrl::OnEraseBkgnd(CDC* pDC)	//V1.1.W150 add
{
	CRect rc;
	GetClientRect(&rc);

	theJpegHelp.Paint(pDC->m_hDC, rc);

	if (m_pbmArrow == NULL) return TRUE;

	CWireBondDlg *pMain = (CWireBondDlg*)theApp.m_pMainWnd;

	if (m_dcUp.m_hDC == NULL)
	{
		m_pbmArrow->RotateFlip(Rotate90FlipNone);
		pMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_UP_SLOT),pDC,m_dcUp,m_pbmArrow);
		m_pbmArrow->RotateFlip(Rotate270FlipNone);
	}

	if (m_dcDown.m_hDC == NULL)
	{
		m_pbmArrow->RotateFlip(Rotate270FlipNone);
		pMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_DOWN_SLOT),pDC,m_dcDown,m_pbmArrow);
		m_pbmArrow->RotateFlip(Rotate90FlipNone);
	}

	return TRUE;
}

void CDlgElevCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)	//V1.1.W150 add
{
	int nLeft = 0, nTop = 0;
	int nWidth = lpDIS->rcItem.right-lpDIS->rcItem.left;
	int nHeight = lpDIS->rcItem.bottom-lpDIS->rcItem.top;

	if (lpDIS->itemState & ODS_SELECTED)
	{
		nLeft = nTop = 1;
		nWidth = nWidth - 2;
		nHeight = nHeight - 2;

		DrawFrameControl(lpDIS->hDC,&lpDIS->rcItem,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);
	}

	if (nIDCtl == IDC_BUTTON_UP_SLOT)
		StretchBlt(lpDIS->hDC,nLeft,nTop,nWidth,nHeight,m_dcUp.m_hDC,nLeft,nTop,nWidth,nHeight,SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_DOWN_SLOT)
		StretchBlt(lpDIS->hDC,nLeft,nTop,nWidth,nHeight,m_dcDown.m_hDC,nLeft,nTop,nWidth,nHeight,SRCCOPY);

	CDialog::OnDrawItem(nIDCtl, lpDIS);
}
