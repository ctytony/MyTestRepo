/***************************************************************
IndexCtrlDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "Bond.h"
#include "IndexCtrlDlg.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"	
#include "VledClamp.h"
#include "StepperControl.h"
#include "MachineConfig.h"
#include "PopUpTopStepperCtr.h"
#include "PopUpCuEliteStepperCtr.h"
#include "VisionSystem.h"
#include "TowerLight.h"
#include "MSGDEF.H"
#include "ScanKeyThread.h"
#include "VEClamp.h"
#include "VEIndex.h"
#include "VEOffload.h"
#include "TopClamp.h"
#include "TopOnload.h"
#include "TopOffload.h"
#include "CuIndex.h"
#include "CuClamp.h"
#include "CuOnload.h"
#include "CuOffload.h"
#include "R2RIndex.h"
#include "R2ROnload.h"
#include "R2ROffload.h"
#include "servo.h"
#include "Debug.h"
#include "DlgElevCtrl.h"
#include "Busy.h"
#include "TopEliteIndex.h"

// CIndexCtrlDlg dialog

IMPLEMENT_DYNAMIC(CIndexCtrlDlg, CDialog)

CIndexCtrlDlg::CIndexCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIndexCtrlDlg::IDD, pParent)
    , m_nCurRow(1)
    , m_nCurCol(1)
	, m_nMatRow(0)
	, m_nMatCol(0)
    , m_nCurMat(0)
{
	m_bBusy = FALSE;				//V1.1.W155 add
	m_nPrevUnit = m_nCurUnit = 1;	//V1.1.W155 add
}

CIndexCtrlDlg::~CIndexCtrlDlg()
{
    FreeButtonArray();
}

void CIndexCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_INDEX, m_nCurUnit);
    DDX_Text(pDX, IDC_EDIT_ROW, m_nCurRow);
    DDX_Text(pDX, IDC_EDIT_COLUMN, m_nCurCol);
    DDX_Text(pDX, IDC_EDIT_MAT_ROW, m_nMatRow);
    DDX_Text(pDX, IDC_EDIT_MAT_COL, m_nMatCol);
    DDX_Text(pDX, IDC_EDIT_CURRENT_MAT, m_nCurMat);
}

BEGIN_MESSAGE_MAP(CIndexCtrlDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_LF_INDEX, OnLfIndex)
    ON_BN_CLICKED(IDC_BUTTON_RESET, OnRestOrAdjust)
    ON_BN_CLICKED(IDC_BUTTON_CLAMPER, OnClamp)
    ON_EN_CHANGE(IDC_EDIT_INDEX, OnEditIndex)
    ON_EN_CHANGE(IDC_EDIT_ROW, OnEditRow)
    ON_EN_CHANGE(IDC_EDIT_COLUMN, OnEditColumn)
    ON_EN_SETFOCUS(IDC_EDIT_INDEX, OnFocusIndex)
    ON_EN_SETFOCUS(IDC_EDIT_ROW, OnFocusRow)
    ON_EN_SETFOCUS(IDC_EDIT_COLUMN, OnFocusColumn)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_TRACK, OnClearTrack)
    ON_BN_CLICKED(IDC_BUTTON_PREV_SUBUNIT, OnPrevSubunit)
    ON_BN_CLICKED(IDC_BUTTON_NEXT_SUBUNIT, OnNextSubunit)
	ON_BN_CLICKED(IDC_BUTTON_MAT_PREV, OnMatPrev)
	ON_BN_CLICKED(IDC_BUTTON_MAT_NEXT, OnMatNext)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MAG, OnLoadMag)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_PITCH_FORWARD, OnMovePitchForward)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_PITCH_BACKWARD, OnMovePitchBackward)
	ON_BN_CLICKED(IDC_BUTTON_EJECT_MAG, OnEjectMag)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.1.W155 add
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CIndexCtrlDlg message handlers
BOOL CIndexCtrlDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

  	CRect Rect;
	(GetParent())->GetWindowRect(&Rect);

    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	UpdateMatrix();

    return CDialog::OnInitDialog();
}

HBRUSH CIndexCtrlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

// Free memory of button array
void CIndexCtrlDlg::FreeButtonArray(void)
{
	while(m_apButton.GetCount())
	{
		delete m_apButton.GetAt(0);
		m_apButton.RemoveAt(0);
	}
}

void CIndexCtrlDlg::DisplayTableIndex(short nIndex)
{
	short nRow, nCol;
    thePgm.GetRowCol(m_nPrevUnit, &nRow, &nCol);
    short nPos = thePgm.GetDeviceRowNum() * (nCol - 1) + nRow - 1;
	if ((nPos < 0) || (nPos > m_apButton.GetCount() - 1)) nPos = 0;

    CColorButton* pButton = m_apButton.GetAt(nPos);
	pButton->SetBkColor(theApp.m_bkColor);
	pButton->Invalidate();

    thePgm.GetRowCol(nIndex, &nRow, &nCol);
    nPos = thePgm.GetDeviceRowNum() * (nCol - 1) + nRow - 1;
	if ((nPos < 0) || (nPos > m_apButton.GetCount() - 1)) nPos = 0;

    pButton = m_apButton.GetAt(nPos);
	pButton->SetBkColor(RGB(255, 255, 0));
	pButton->Invalidate();
}

void CIndexCtrlDlg::OnLfIndex()
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	CWireBondDlg *pWbDlg = (CWireBondDlg*)GetParent();

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, FALSE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, FALSE);

	pWbDlg->SetProcessKeyboard(true);

	long lElapseTime, lLastTime;
    bool bIndexInProgress = true;
    short nMhsErrStatus, nCurUnit;
	short nUnitTotalWire;
 
	short nEvent = 1;

    GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_FOXCONN:	//v3.0.T308 add
		theMHS.SetRequestIndex(true);
		break;
	case MHS_TOP_LED:
	{
		if (theTopOnloader.m_MagDetectedSensor.GetCurrentState() == FALSE && theTopTrack.GetCurBondUnit() == 0)
		{
			GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();

			pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
			pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

			//v3.3.D166
			pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

			short anMsgArray[1] = {MSG_ONLOAD_ELEV_NO_MAG};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
			return;
		}
		else
		{
			if (theTopOnloader.GetErrorCode() == MHS_TOP_ONLOAD_NO_MAG)
			{
				theTopOnloader.ResetErrorCode();
			}
		}

		if (theTopTrack.IsTrackNeedClear())
		{
			if (theMHS.ClearTrack() != FOK)
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);

				GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

				//v3.3.D166
				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

				return;
			}
		}

		if (theTopOffloader.IsJamNotHandled())
		{
			CDlgElevCtrl objElev;
			if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)== FALSE)
			{
				TOP_OFFLOAD_RCD stOffloadData;
				theTopOffloader.GetRcd(&stOffloadData);

				if (theTopOffloader.GetCurSlot() < stOffloadData.lOffloadSlotNum)
				{
					TOP_TRACK_DATA stTrackData;
					theTopTrack.GetRcd(&stTrackData);
				    theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot() + 1 + stTrackData.iOutputMagazineSlotSequence);
					theTopOffloader.WaitMoveMagHolder();
				}
				else
					theTopOffloader.EjectOffloadMag();
			}
			else
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);

				GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

				//v3.3.D166
				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

				return ;
			}
		}

		if (theMHS.GetCurBondUnit() == 0)
		{
			theTopTrack.ResetErrorCode(1);
			theTopTrack.ResetErrorCode(0);

			int iPusherStatus = theTopOnloader.m_InjectPusher.TriggerForwardMotion();
			iPusherStatus += theTopOnloader.m_InjectPusher.TriggerBacktoHome();
			if (iPusherStatus != FOK)
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

				//v3.3.D166
				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

				short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
				theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
				return;
			}

			theTopOnloader.ResetAfterInit();
			theTopTrack.RequestOnload();
		}
		else if(theMHS.IsLastLf()==FALSE
			&&theTopTrack.m_InjectSensor.GetCurrentState()==SENSOR_OFF
			&&theTopTrack.GetNextLfState()==0)	//V3.3.D242
		{
			theTopOnloader.m_InjectPusher.TriggerForwardMotion();
			theTopOnloader.m_InjectPusher.TriggerBacktoHome();
		}

		theTopOnloader.SetFirstIndex((theTopTrack.m_InjectSensor.GetCurrentState() == SENSOR_ON), false);
       
		//******************************ACTION********************************************

		//theTopOnloader.ResetOnloadEvent();
		
        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep_x1ms(10);

        theServo.MoveBondHeadToSafePos();

        //theMHS.SetRequestIndex(true);

        if (theTopTrack.GetEvent() == INDEX_EVENT_IDLE)
            theTopTrack.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);

        Sleep_x1ms(100);

        lLastTime=GetCurrentTime();

        while (nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theTopTrack.GetEvent();

            if (theMHS.ShowError() != FOK) break;

            if (theTopTrack.GetIndexPrManualAlign() == INDEX_PR_ENTER_MANU_ALIGN)
            {
                theTopTrack.PerformPrIndex();
                lLastTime = GetCurrentTime();
            }

            lElapseTime = ::GetCurrentTime() - lLastTime;
            if (lElapseTime > 35000) break;

            Sleep_x1ms(1);
        }

		theVisionSys.EnableVideo(true);
        theMHS.SetAutoIndex(false);

		TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
        if (pstDevice->nHalfIndexOn > 0)
        {
            if (theTopTrack.GetCurBondUnit() == pstDevice->nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
					m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire  = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

					m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nBondedWire  = nUnitTotalWire * (m_nCurUnit - 1);
                }
            }
            else
            {
				m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                m_nCurUnit = 1;
                thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire  = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

			m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
            m_nCurUnit = 1;
            thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);
        }

        if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
        {
            short nIndexTime, nNextMat, nStatus = FOK;
            
            nIndexTime = theMHS.GetCurBondUnit();
            nStatus = thePgm.GetNextMatrix(nIndexTime, 0, &nNextMat);
            if (nStatus == FOK)
            {
                m_nCurMat = nNextMat;
                thePgm.GetMatRowCol(m_nCurMat, &m_nMatRow, &m_nMatCol);

                theBond.m_nCurMatRow = m_nMatRow;
                theBond.m_nCurMatCol = m_nMatCol;
            }
            else
            {
                theBond.m_nCurMatRow = 1;
                theBond.m_nCurMatCol = 1;
            }
        }
        else
        {
            m_nCurMat = 1;
            theBond.m_nCurMatRow = 1;
            theBond.m_nCurMatCol = 1;
        }

        UpdateData(FALSE);

        theBond.m_nCurRow = m_nCurRow;
        theBond.m_nCurCol = m_nCurCol;

        theBond.ResetMatrixPrStatus(1);

        pWbDlg->UpdateInfoView(INFOVIEW_ALL);
		break;
    }
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		if (theTopOnloader.m_MagDetectedSensor.GetCurrentState() == FALSE && theTopTrack.GetCurBondUnit() == 0)
		{
			GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();

			pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
			pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);

			pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());

			short anMsgArray[1] = {MSG_ONLOAD_ELEV_NO_MAG};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
			return;
		}
		else
		{
			if (theTopOnloader.GetErrorCode() == MHS_TOP_ONLOAD_NO_MAG)
				theTopOnloader.ResetErrorCode();
		}

		if (theTEIndex.IsTrackNeedClear())
		{
			if (theMHS.ClearTrack() != FOK)
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);

				GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);

				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());
				return;
			}
		}

		if (theTopOffloader.IsJamNotHandled())
		{
			CDlgElevCtrl objElev;
			if (objElev.IsLeadframeOnTrack(TOP_ELEV_OFFLOAD)== FALSE)
			{
				TOP_OFFLOAD_RCD stOffloadData;
				theTopOffloader.GetRcd(&stOffloadData);

				if (theTopOffloader.GetCurSlot() < stOffloadData.lOffloadSlotNum)
				{
					ST_TE_INDEX_RCD *pstTEIndex = theTEIndex.GetRcd();
				    theTopOffloader.MoveMagHolderToSlot(theTopOffloader.GetCurSlot() + 1 + pstTEIndex->iOutputMagazineSlotSequence);
					theTopOffloader.WaitMoveMagHolder();
				}
				else
					theTopOffloader.EjectOffloadMag();
			}
			else
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);

				GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);

				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());
				return ;
			}
		}

		if (theMHS.GetCurBondUnit() == 0)
		{
			theTEIndex.ResetErrorCode(1);
			theTEIndex.ResetErrorCode(0);

			int iPusherStatus = theTopOnloader.m_InjectPusher.TriggerForwardMotion();
			iPusherStatus += theTopOnloader.m_InjectPusher.TriggerBacktoHome();
			if (iPusherStatus != FOK)
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);

				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());

				short anMsgArray[1] = {MSG_ONLOAD_PUSH_KICKER_WRONG};
				theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);
				return;
			}

			theTopOnloader.ResetAfterInit();
			theTEIndex.RequestOnload();
		}
		else if ((theMHS.IsLastLf() == FALSE)
			&& (theTopTrack.m_InjectSensor.GetCurrentState() == SENSOR_OFF)
			&& (theTopTrack.GetNextLfState() == 0))
		{
			theTopOnloader.m_InjectPusher.TriggerForwardMotion();
			theTopOnloader.m_InjectPusher.TriggerBacktoHome();
		}

		theTopOnloader.SetFirstIndex((theTEIndex.m_InjectSensor.GetCurrentState() == SENSOR_ON), false);
       
		//******************************ACTION********************************************

		//theTopOnloader.ResetOnloadEvent();
		
        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep_x1ms(10);
        theServo.MoveBondHeadToSafePos();

        if (theTEIndex.GetEvent() == INDEX_EVENT_IDLE)
            theTEIndex.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);
        Sleep_x1ms(100);

        lLastTime = GetCurrentTime();
        while (nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theTEIndex.GetEvent();

            if (theMHS.ShowError() != FOK) break;

            if (theTEIndex.GetIndexPrManualAlign() == INDEX_PR_ENTER_MANU_ALIGN)
            {
                theTEIndex.PerformPrIndex();
                lLastTime = GetCurrentTime();
            }

            lElapseTime = ::GetCurrentTime() - lLastTime;
            if (lElapseTime > 35000) break;

            Sleep_x1ms(1);
        }

		theVisionSys.EnableVideo(true);
        theMHS.SetAutoIndex(false);

		TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
        if (pstDevice->nHalfIndexOn > 0)
        {
            if (theTEIndex.GetCurBondUnit() == pstDevice->nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
					m_nPrevUnit = m_nCurUnit;
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire  = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

					m_nPrevUnit = m_nCurUnit;
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nBondedWire  = nUnitTotalWire * (m_nCurUnit - 1);
                }
            }
            else
            {
				m_nPrevUnit = m_nCurUnit;
                m_nCurUnit = 1;
                thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire  = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

			m_nPrevUnit = m_nCurUnit;
            m_nCurUnit = 1;
            thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);
        }

        if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
        {
            short nIndexTime, nNextMat, nStatus = FOK;
            
            nIndexTime = theMHS.GetCurBondUnit();
            nStatus = thePgm.GetNextMatrix(nIndexTime, 0, &nNextMat);
            if (nStatus == FOK)
            {
                m_nCurMat = nNextMat;
                thePgm.GetMatRowCol(m_nCurMat, &m_nMatRow, &m_nMatCol);

                theBond.m_nCurMatRow = m_nMatRow;
                theBond.m_nCurMatCol = m_nMatCol;
            }
            else
            {
                theBond.m_nCurMatRow = 1;
                theBond.m_nCurMatCol = 1;
            }
        }
        else
        {
            m_nCurMat = 1;
            theBond.m_nCurMatRow = 1;
            theBond.m_nCurMatCol = 1;
        }

        UpdateData(FALSE);

        theBond.m_nCurRow = m_nCurRow;
        theBond.m_nCurCol = m_nCurCol;
        theBond.ResetMatrixPrStatus(1);
        pWbDlg->UpdateInfoView(INFOVIEW_ALL);
		break;
    }
	case MHS_R2R:
	{
		short nClearTrackId = 0, nCallBackId = MSG_CALLBACK_NONE;
		short i = 0;
		CStringList sList;

		if (theR2ROnloader.theErrorSnr.GetCurrentState() != FOK)
		{
			sList.AddTail(theMsg.GetMsg(MSG_ONLOAD_DOWN));
			i++;
		}

		if (theR2ROffloader.theErrorSnr.GetCurrentState() != FOK)
		{
			sList.AddTail(theMsg.GetMsg(MSG_OFFLOAD_DOWN));
			i++;
		}

		if (i > 0)
		{
			theMsg.ShowMhsMsg(WARNING_MSG, sList, nCallBackId, nClearTrackId);
			return;
		}

		TOP_DEVICE_RECORD  stDeviceRcd;
		theDevice.GetTopRcd(&stDeviceRcd);

		if (theMHS.GetCurBondUnit() == 0)
		{	
			theR2RTrack.ResetTrackErrorCode(1);
			theR2RTrack.ResetTrackErrorCode(0);
		}
       
		//******************************ACTION********************************************

		    //pOnload->ResetOnloadEvent();
		
        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep_x1ms(10);

        theServo.MoveBondHeadToSafePos();

        if(theR2RTrack.GetTopTrackEvent() == INDEX_EVENT_IDLE)
            theR2RTrack.SubmitIndexEvent(INDEX_EVENT_SINGLE_INDEX);

        Sleep_x1ms(100);

        lLastTime=GetCurrentTime();

        while(nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theR2RTrack.GetTopTrackEvent();

            nMhsErrStatus = theMHS.ShowError(); 
            if (nMhsErrStatus != FOK )
                break;

            if(theR2RTrack.GetIndexPrManuAlignFlag() == INDEX_PR_ENTER_MANU_ALIGN)
            {

                theR2RTrack.PerformIndexPrprogress();
                lLastTime = GetCurrentTime();

            }

            lElapseTime=::GetCurrentTime()-lLastTime;
            if (lElapseTime > 2000000) //w.tempx
                break;

            Sleep_x1ms(1);
        }

		theVisionSys.EnableVideo(true);
        theMHS.SetAutoIndex(false);

        if (stDeviceRcd.nHalfIndexOn > 0)
        {
            if (theR2RTrack.GetCurrentBondUnit() == stDeviceRcd.nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
					m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

					m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nBondedWire  = nUnitTotalWire * (m_nCurUnit - 1);
                }
            }
            else
            {
				m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                m_nCurUnit = 1;
                thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

			m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
            m_nCurUnit = 1;
            thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);
        }

        if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
        {
            short nIndexTime, nNextMat, nStatus = FOK;
            
            nIndexTime = theMHS.GetCurBondUnit();
            nStatus = thePgm.GetNextMatrix(nIndexTime, 0, &nNextMat);
            if (nStatus == FOK)
            {
                m_nCurMat = nNextMat;
                thePgm.GetMatRowCol(m_nCurMat, &m_nMatRow, &m_nMatCol);

                theBond.m_nCurMatRow = m_nMatRow;
                theBond.m_nCurMatCol = m_nMatCol;
            }
            else
            {
                theBond.m_nCurMatRow = 1;
                theBond.m_nCurMatCol = 1;
            }
        }
        else
        {
            m_nCurMat = 1;
            theBond.m_nCurMatRow = 1;
            theBond.m_nCurMatCol = 1;
        }

        UpdateData(FALSE);

        theBond.m_nCurRow = m_nCurRow;
        theBond.m_nCurCol = m_nCurCol;

        theBond.ResetMatrixPrStatus(1);

        ((CWireBondDlg*)GetParent())->UpdateInfoView(INFOVIEW_ALL);
		break;
    }
	case MHS_CU_ELITE:
	{
		CUELITE_DEVICE_RECORD stCuEliteDevicePara;
		theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

		if(theCuEliteOnloader.theMagDetectedSensor.GetCurrentState()==FALSE && theCuEliteTrack.GetCurrentBondUnit() == 0)
		{
			GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();

			pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
			pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
			pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

			//v3.3.D166
			pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

			short anMsgArray[1]={MSG_ONLOAD_ELEV_NO_MAG};
			theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

			return;
		}
		else if (theCuEliteOnloader.GetOnloadErrorCode() == MHS_CUELITE_ONLOAD_NO_MAG)
		{
			theCuEliteOnloader.ResetOnloadErrorCode();
		}

		if(theCuEliteTrack.GetTrackNeedClearFlag() == TRUE)
		{
			if(theMHS.ClearTrack()!=FOK)
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

				//v3.3.D166
				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

				return;
			}
		}

		if (theCuEliteOffloader.GetOffloadJamHandleFlag() == TRUE)
		{
			if (theCuEliteTrack.theEjectSnr.GetCurrentState() == FALSE)
			{
				//V3.3.D176
				if (theCuEliteOffloader.GetCurrentSlotNum()< stCuEliteDevicePara.lMagSlotNum)
				{
					CUELITE_TRACK_DATA stTrackData;
					theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);
					theCuEliteOffloader.MoveTotheSlot(theCuEliteOffloader.GetCurrentSlotNum() + 1 + stTrackData.iOutputMagazineSlotSequence);
					theCuEliteOffloader.WaitMagHoledrMoveFinish();
				}
				else
					theCuEliteOffloader.EjectOffloadMag();
			}
			else
			{
				short anMsgArray[1]={MSG_REMOVE_LF_ON_TRACK};
				theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);

				GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

				//v3.3.D166
				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

				return ;
			}
		}

		if (theMHS.GetCurBondUnit() == 0)
		{
			theCuEliteTrack.ResetTrackErrorCode(1);
			theCuEliteTrack.ResetTrackErrorCode(0);

			int iPusherStatus=theCuEliteOnloader.theInjectPusher.TriggerForwardMotion();
			iPusherStatus+=theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();

			if(iPusherStatus != FOK)
			{
				GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
				GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();
				GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
				GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();

				pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
				pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
				pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

				//v3.3.D166
				pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

				short anMsgArray[1]={MSG_ONLOAD_PUSH_KICKER_WRONG};
				theMsg.ShowMsg(WARNING_MSG, 1, anMsgArray);

				return;
			}
	
			theCuEliteOnloader.ResetOnloadInitStatus();
			theCuEliteTrack.RequestOnloadEvent();
		}
		else if(theMHS.IsLastLf()==FALSE
			&&theCuEliteTrack.theInjectSnr.GetCurrentState()==SENSOR_OFF
			&&theCuEliteTrack.GetNextLfState()==0)
		{
			theCuEliteOnloader.theInjectPusher.TriggerForwardMotion();
			theCuEliteOnloader.theInjectPusher.TriggerBacktoHome();
		}

		theCuEliteOnloader.SetFirstIndex((theCuEliteTrack.theInjectSnr.GetCurrentState() == SENSOR_ON), false);
       
		//******************************ACTION********************************************

		    //theTopOnloader.ResetOnloadEvent();
		
        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);
        Sleep_x1ms(10);

		theServo.MoveBondHeadToSafePos();

        //theMHS.SetRequestIndex(true);

        if(theCuEliteTrack.GetCuEliteTrackEvent() == INDEX_EVENT_IDLE)
            theCuEliteTrack.SubmitIndexEvent(INDEX_EVENT_AUTO_INDEX);

        Sleep_x1ms(100);

        lLastTime=GetCurrentTime();

        while(nEvent != INDEX_EVENT_IDLE)
        {
            nEvent = theCuEliteTrack.GetCuEliteTrackEvent();

            nMhsErrStatus = theMHS.ShowError(); //w.question:here
            if (nMhsErrStatus != FOK)
                break;

            if(theCuEliteTrack.GetIndexPrManuAlignFlag() == INDEX_PR_ENTER_MANU_ALIGN)
            {
                theCuEliteTrack.PerformIndexPrprogress();
                lLastTime = GetCurrentTime();
            }

            lElapseTime=::GetCurrentTime()-lLastTime;
            if (lElapseTime > 20000) 
                break;

            Sleep_x1ms(1);
        }

		theVisionSys.EnableVideo(true);
        theMHS.SetAutoIndex(false);

        if (stCuEliteDevicePara.nHalfIndexOn > 0)
        {
            if (theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn)  // for Inconsistent Index
            {
                if (thePgm.GetDeviceSeq() == PGM_MATRIX)
                {
					m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = 1;
                    theBond.m_nRestartWire = 1;
                    theBond.m_nBondedWire = 0;
                }
                else
                {
                    nUnitTotalWire = thePgm.GetUnitTotalWire();

					m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                    m_nCurUnit = thePgm.GetTotalUnit() / 2 + 1;
                    thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                    theBond.m_nCurWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nRestartWire = nUnitTotalWire * (m_nCurUnit - 1) + 1;
                    theBond.m_nBondedWire = nUnitTotalWire * (m_nCurUnit - 1);
                }
            }
            else
            {
				m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
                m_nCurUnit = 1;
                thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

                theBond.m_nCurWire = 1;
                theBond.m_nRestartWire = 1;
                theBond.m_nBondedWire  = 0;
            }
        }
        else
        {
            theBond.m_nCurWire = 1;
            theBond.m_nRestartWire = 1;
            theBond.m_nBondedWire  = 0;

            m_nCurUnit = 1;
            thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);
        }

        if ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID))
        {
            short nIndexTime, nNextMat, nStatus = FOK;
            
            nIndexTime = theMHS.GetCurBondUnit();
            nStatus = thePgm.GetNextMatrix(nIndexTime, 0, &nNextMat);
            if (nStatus == FOK)
            {
                m_nCurMat = nNextMat;
                thePgm.GetMatRowCol(m_nCurMat, &m_nMatRow, &m_nMatCol);

                theBond.m_nCurMatRow = m_nMatRow;
                theBond.m_nCurMatCol = m_nMatCol;
            }
            else
            {
                theBond.m_nCurMatRow = 1;
                theBond.m_nCurMatCol = 1;
            }
        }
        else
        {
            m_nCurMat = 1;
            theBond.m_nCurMatRow = 1;
            theBond.m_nCurMatCol = 1;
        }

        UpdateData(FALSE);

        theBond.m_nCurRow = m_nCurRow;
        theBond.m_nCurCol = m_nCurCol;

        theBond.ResetMatrixPrStatus(1);

        ((CWireBondDlg*)GetParent())->UpdateInfoView(INFOVIEW_ALL);
		break;
    }
	case MHS_VLED_ELITE:
	{
        nCurUnit = theVEliteTrack.GetBondUnitNum();

		theMHS.SetLastLf(false);
        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);

		if (nCurUnit == 0)
			theVEliteTrack.theLeftClaw.SetIndexEmpty(true);

		nMhsErrStatus = theMHS.ShowError();
		if (nMhsErrStatus == FOK)
		{
			theMHS.SetRequestIndex(true);
			Sleep_x1ms(200);

			lLastTime = GetCurrentTime();
			while (bIndexInProgress)
			{
				bIndexInProgress = theMHS.IsRequestIndex();

				nMhsErrStatus = theMHS.ShowError();
				if (nMhsErrStatus != FOK) break;

				lElapseTime = ::GetCurrentTime() - lLastTime;
				if (lElapseTime > 15000) break;

				Sleep_x1ms(1);
			}
		}

        theMHS.SetAutoIndex(false);

		m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
        m_nCurUnit = 1;
        UpdateData(false);

        theBond.m_nCurMatRow = 1;
        theBond.m_nCurMatCol = 1;
		break;
    }
	case MHS_VLED:
	{
        nCurUnit = theVledTrack.GetBondUnitNum();

		theMHS.SetLastLf(false);
        theMHS.SetAutoIndex(true);
        theMHS.SetDryRun(false);

		if (nCurUnit == 0)
			theVledTrack.theLeftClaw.SetIndexEmpty(true);

        theMHS.SetRequestIndex(true);
        Sleep_x1ms(200);

        lLastTime = GetCurrentTime();
        while(bIndexInProgress)
        {
            bIndexInProgress = theMHS.IsRequestIndex();

            nMhsErrStatus = theMHS.ShowError();
            if (nMhsErrStatus != FOK)
                break;

            lElapseTime=::GetCurrentTime()-lLastTime;
            if (lElapseTime > 15000) 
                break;

            Sleep_x1ms(1);
        }

        theMHS.SetAutoIndex(false);

		m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
        m_nCurUnit = 1;
        UpdateData(false);

        theBond.m_nCurMatRow = 1;
        theBond.m_nCurMatCol = 1;
		break;
    }
	}

	if (thePgm.IsProgramValid()) OnEditIndex();

	theBond.SetPrOn1stEnterDone(true);
	theBond.SetAutoDefineBond(false);

    GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
	GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();

	bIndexInProgress = (theMcConfig.GetMhsType() == MHS_TOP_LED)
				|| (theMcConfig.GetMhsType() == MHS_VLED)
				|| (theMcConfig.GetMhsType() == MHS_VLED_ELITE)
				|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
				|| (theMcConfig.GetMhsType() == MHS_FOXCONN)	//v3.0.T322 add
				|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE);	//v3.0.T410 add

	GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(bIndexInProgress);

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

	//v3.3.D166
	pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

	GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

	pWbDlg->SetProcessKeyboard(false);
}

void CIndexCtrlDlg::OnClearTrack()
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	CWireBondDlg *pWbDlg = (CWireBondDlg*)GetParent();
	pWbDlg->SetProcessKeyboard(true);

    GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, FALSE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, FALSE);

	theMHS.ClearTrack();

	if ((thePgm.IsProgramValid())
		&& ((theMcConfig.GetMhsType() == MHS_TOP_LED)
			|| (theMcConfig.GetMhsType() == MHS_FOXCONN)	//v3.0.T217 add
			|| (theMcConfig.GetMhsType() == MHS_R2R)
			|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
			|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE)))	//v3.0.T410 add
	{
		m_nPrevUnit = m_nCurUnit;		//V1.1.W155 add
		m_nCurUnit = 1; 
		thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);
		UpdateData(FALSE);
	}

	theBond.m_nCurMatRow = 1;
	theBond.m_nCurMatCol = 1;
	theBond.m_nCurWire = 1;
	theBond.m_nRestartWire = 1;
	theBond.m_nBondedWire  = 0;

	pWbDlg->UpdateInfoView(INFOVIEW_ALL);

	GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
	GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();
	GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();

	GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

	//v3.3.D166
	pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit

	pWbDlg->SetProcessKeyboard(false);
}

void CIndexCtrlDlg::OnRestOrAdjust()  //reset workholder
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	long  lLastTime, lElapseTime;

    GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);

	CWireBondDlg *pWbDlg = (CWireBondDlg*)GetParent();

	pWbDlg->SetProcessKeyboard(true);

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, FALSE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, FALSE);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
    {
		INDEX_PR_REF stRef;
		theDevice.GetIndexPrRef(&stRef);
		theServo.MoveTable2(stRef.stVisionAlignPos);	//v3.1.T201 edit
		Sleep_x1ms(10);

		if(theTopTrack.GetCurBondUnit()!=0)
		{
			if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
				theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);

	    	theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
			theTopClamp.PreCloseClamp();

		 	CPopUpTopStepperCtr theStepperCtrler(theTopTrack.GetIndexStepID());
	        theStepperCtrler.DoModal();

			theTopClamp.CloseClamp();
			theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
			theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
    {
		INDEX_PR_REF stRef;
		theDevice.GetIndexPrRef(&stRef);
		theServo.MoveTable2(stRef.stVisionAlignPos);	//v3.1.T201 edit
		Sleep_x1ms(10);

		if (theTopTrack.GetCurBondUnit() != 0)
		{
			if (theTEIndex.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
				theTEIndex.m_LFHolder.SetRelayState(RELAY_ON);

	    	theTEIndex.m_pTargetClaw->SetRelayState(RELAY_ON);
			theTopClamp.PreCloseClamp();

		 	CPopUpTopStepperCtr theStepperCtrler(theTEIndex.GetIndexStepID());
	        theStepperCtrler.DoModal();

			theTopClamp.CloseClamp();
			theTEIndex.m_pTargetClaw->SetRelayState(RELAY_OFF);
			theTEIndex.m_LFHolder.SetRelayState(RELAY_OFF);
		}
		break;
	}
	case MHS_R2R:
    {
		INDEX_PR_REF stRef;
		theDevice.GetIndexPrRef(&stRef);
		theServo.MoveTable2(stRef.stVisionAlignPos);	//v3.1.T201 edit
		Sleep_x1ms(10);

		if(theR2RTrack.GetCurrentBondUnit()!=0)
		{
	    	theR2RTrack.TargetClaw(RELAY_ON);
			theTopClamp.PreCloseClamp();

		 	CPopUpTopStepperCtr theStepperCtrler(theR2RTrack.m_ucIndexerStepperId);
	        theStepperCtrler.DoModal();

			theTopClamp.CloseClamp();
			theR2RTrack.TargetClaw(RELAY_OFF);
		}
		break;
	}
	case MHS_CU_ELITE:
    {
		CUELITE_TRACK_DATA stTrackRcd;
		theCuEliteTrack.GetCuEliteTrackRcd(&stTrackRcd);

		INDEX_PR_REF stRef;
		theDevice.GetIndexPrRef(&stRef);
		theServo.MoveTable2(stRef.stVisionAlignPos);	//v3.1.T201 edit
		Sleep_x1ms(10);

		if(theCuEliteTrack.GetCurrentBondUnit()!=0)
		{
			if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nSrchUnit)
				theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
	    	theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
			theCuEliteLFClamp.PreCloseClamp();

		 	CPopUpCuEliteStepperCtr theStepperCtrler(CUELITE_DEVICE_INDEXER, CUELITE_SPEED_SMALL);
			theStepperCtrler.PresetDlgPos(DLG_POS_BOTTOM_RIGHT);
			theStepperCtrler.Preset2ndCtrlID(CUELITE_WINDOW_CLAMPER);
	        theStepperCtrler.DoModal();

			theCuEliteLFClamp.CloseClamp();

			lLastTime = GetCurrentTime();
			do
			{
				theCuEliteLFClamp.CheckClampStatus();

				lElapseTime = ::GetCurrentTime() - lLastTime;
				if (lElapseTime > stTrackRcd.lClawDelay) break;
			} while (theCuEliteLFClamp.GetClampStatus() != CUELITE_WND_CLAMP_CLOSE);

			theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
			theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);
		}
		break;
	}
	case MHS_VLED:
		theMHS.ResetWorkHolder();
		break;
	}

    GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow();
	GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow();
    GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow();
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow();

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

	//v3.3.D166
	pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit
	GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

	pWbDlg->SetProcessKeyboard(false);
}

void CIndexCtrlDlg::OnClamp()
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	long lOffset;

	CWireBondDlg *pWbDlg = (CWireBondDlg*)GetParent();

	pWbDlg->SetProcessKeyboard(true);

	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(false);

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, FALSE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, FALSE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, FALSE);

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_VLED_ELITE:
    {
		theVEliteLfClamp.CheckLfClampStatus(FALSE);

		if (theVEliteLfClamp.GetClampStatus() == SINGLE_CLAMP_CLOSE)
			theVEliteLfClamp.OpenLfClamp();
		else
			theVEliteLfClamp.CloseLfClamp();

		do
		{
			Sleep_x1ms(10);
		} while (theStepper.IsBusy(VE_STEPPER_INDEX_CLAMPER));
		break;
	}
	case MHS_VLED:
    {
		theVledLfClamp.CheckLfClampStatus(FALSE);

		if (theVledLfClamp.GetClampStatus() == SINGLE_CLAMP_CLOSE)
			theVledLfClamp.OpenLfClamp();
		else
			theVledLfClamp.CloseLfClamp();

		do
		{
			Sleep_x1ms(1);
		} while (theStepper.IsBusy(SINGLE_CAP_CLAMPER));
		break;
	}
	case MHS_CU_ELITE:
    {
        theCuEliteLFClamp.CheckClampStatus();
		if (theCuEliteLFClamp.GetClampStatus()!= CUELITE_WND_CLAMP_SAFE_CLAMP)
		{
			theServo.MoveBondHeadToSafePos();
			Sleep_x1ms(10);
		    theCuEliteLFClamp.MoveClampSafePos();
		}
		else
		{
			INDEX_PR_REF stPrIndexRefRcd;
         	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

			CUELITE_TRACK_DATA  stTrackData;              
	        theCuEliteTrack.GetCuEliteTrackRcd(&stTrackData);

			theCuEliteLFClamp.PreCloseClamp();

			Sleep_x1ms(200);

			if ((stTrackData.nPrIndexEnable == 2  &&  //enable 1st unit only
				stPrIndexRefRcd.bIndexPrAvail  &&
				theCuEliteTrack.GetPrIndexMask() == false &&
				theCuEliteTrack.GetCurrentBondUnit() == 1)       ||

				(stTrackData.nPrIndexEnable == 1  && //enable for all
				stPrIndexRefRcd.bIndexPrAvail  &&
				theCuEliteTrack.GetPrIndexMask() == false ))
			{
				if(theCuEliteTrack.CaculatePrIndexOffset(&lOffset) == FOK)
				{
					CUELITE_TRACK_DATA m_stCuEliteTrackRcd;
					theCuEliteTrack.GetCuEliteTrackRcd(&m_stCuEliteTrackRcd);
					if(abs(lOffset) > m_stCuEliteTrackRcd.lPrIndexTolerance)
					{
						if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nInjectUnit)
							theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
						theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
						Sleep_x1ms(400);

						theStepper.MoveDist(theCuEliteTrack.m_ucIndexerStepperId, lOffset, MOTION_RELATIVE);
						do
						{
							Sleep_x1ms(2);
						} while (theStepper.IsBusy(theCuEliteTrack.m_ucIndexerStepperId));

						if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nInjectUnit)
							theCuEliteTrack.theLFHolder.SetRelayState(RELAY_OFF);
						theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
						Sleep_x1ms(10);
						theCuEliteLFClamp.CloseClamp();
					}
				}
				else
				{
					theCuEliteTrack.PerformIndexPrprogress();

					if(theCuEliteTrack.GetIndexPrManuAlignFlag() == INDEX_PR_MANU_ALIGN_OK)
					{

						if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nInjectUnit)
							theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
						theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_ON);
						Sleep_x1ms(400);

						theStepper.MoveDist(theCuEliteTrack.m_ucIndexerStepperId, theCuEliteTrack.m_loffsetXInUm, MOTION_RELATIVE);
						do
						{
							Sleep_x1ms(2);
						} while (theStepper.IsBusy(theCuEliteTrack.m_ucIndexerStepperId));

						if (theCuEliteTrack.GetCurrentBondUnit() < theCuEliteTrack.m_stIndexerVar.nInjectUnit)
							theCuEliteTrack.theLFHolder.SetRelayState(RELAY_ON);
						theCuEliteTrack.theTargetClaw->SetRelayState(RELAY_OFF);
						Sleep_x1ms(10);
						theCuEliteLFClamp.CloseClamp();
					}
					else
					{
						theCuEliteTrack.SetIndexPrManuAlignFlag(INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
					}
				}
			}
		}
		break;
    }
	case MHS_R2R:
	{
		theTopClamp.CheckClampStatus();
		if (theTopClamp.GetClampStatus()!= WND_CLAMP_SAFE_CLAMP)
		{
			theServo.MoveBondHeadToSafePos();
			Sleep_x1ms(10);
		    theTopClamp.MoveClampSafePos();
		}
		else
		{
			INDEX_PR_REF stPrIndexRefRcd;
         	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

			TOP_TRACK_DATA  stTrackData;              
	        theR2RTrack.GetTopTrackRcd(&stTrackData);

			theTopClamp.PreCloseClamp();

			Sleep_x1ms(200);

			if ((stTrackData.nPrIndexEnable == 2  &&  //enable 1st unit only
				  stPrIndexRefRcd.bIndexPrAvail  &&
			      theR2RTrack.GetPrIndexMask() == false &&
				  theR2RTrack.GetCurrentBondUnit() == 1)       ||
				
				 (stTrackData.nPrIndexEnable == 1  && //enable for all
				  stPrIndexRefRcd.bIndexPrAvail  &&
			      theR2RTrack.GetPrIndexMask() == false))
			{
				if(theR2RTrack.CaculatePrIndexOffset(&lOffset) == FOK)
				{
					theR2RTrack.TargetClaw(RELAY_ON);
					Sleep_x1ms(400);

					theStepper.MoveDist(theR2RTrack.m_ucIndexerStepperId, lOffset, MOTION_RELATIVE);
					do
					{
						Sleep_x1ms(2);
					} while (theStepper.IsBusy(theR2RTrack.m_ucIndexerStepperId));

					theR2RTrack.TargetClaw(RELAY_OFF);
					Sleep_x1ms(10);
		            theTopClamp.CloseClamp();
				}
				else
				{
					theR2RTrack.PerformIndexPrprogress();

					if (theR2RTrack.GetIndexPrManuAlignFlag() == INDEX_PR_MANU_ALIGN_OK)
					{
						theR2RTrack.TargetClaw(RELAY_ON);
						Sleep_x1ms(400);

						theStepper.MoveDist(theR2RTrack.m_ucIndexerStepperId, theR2RTrack.m_loffsetXInUm, MOTION_RELATIVE);
						do
						{
							Sleep_x1ms(2);
						} while (theStepper.IsBusy(theR2RTrack.m_ucIndexerStepperId));

						theR2RTrack.TargetClaw(RELAY_OFF);
						Sleep_x1ms(10);
		                theTopClamp.CloseClamp();
					}
					else
					{
						theR2RTrack.SetIndexPrManuAlignFlag(INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
					}
				}
			}
		}
		break;
	}
	case MHS_TOP_LED:
	{
		theTopClamp.CheckClampStatus();
		if (theTopClamp.GetClampStatus()!= WND_CLAMP_SAFE_CLAMP)
		{
			theServo.MoveBondHeadToSafePos();
			Sleep_x1ms(10);
		    theTopClamp.MoveClampSafePos();
		}
		else
		{
			INDEX_PR_REF stPrIndexRefRcd;
         	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

			TOP_TRACK_DATA stTrackData;              
	        theTopTrack.GetRcd(&stTrackData);

			theMcConfig.IsPrIndexHalfClamp() == FALSE ? theTopClamp.CloseClamp() : theTopClamp.PreCloseClamp();		//V1.5.W84 edit

			Sleep_x1ms(200);

			if (((stTrackData.nPrIndexEnable == 2)		//enable 1st unit only
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (theTopTrack.IsPrIndexMask() == false)
				&& (theTopTrack.GetCurBondUnit() == 1)) ||
				((stTrackData.nPrIndexEnable == 1)		//enable for all
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (theTopTrack.IsPrIndexMask() == false)))
			{
				theTopTrack.m_nPRIState=1;	//V1.5.W140 add

				if (theTopTrack.CalcPrIndexOffset(&lOffset) == FOK)
				{
					if(theMcConfig.IsPrIndexHalfClamp()==FALSE)theTopClamp.PreCloseClamp();		//V1.5.W84 add

					if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
						theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
					theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
					Sleep_x1ms(400);

					theStepper.MoveDist(theTopTrack.GetIndexStepID(), lOffset, MOTION_RELATIVE);

					do
					{
						Sleep_x1ms(2);
					} while (theStepper.IsBusy(theTopTrack.GetIndexStepID()));

					theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
					theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);

					Sleep_x1ms(10);
					theMHS.CloseWindowClamp();	//V1.5.W140 edit

					if(theDebug.IsDebugIndexAccuracy())	//V1.5.W140 add
					{
						Sleep_x1ms(100);

						theTopTrack.m_nPRIState=2;
						theTopTrack.CalcPrIndexOffset(&lOffset);
						theTopTrack.m_nPRIState=0;
					}
				}
				else
				{
					if(theMcConfig.IsPrIndexHalfClamp()==FALSE)theTopClamp.PreCloseClamp();		//V1.5.W84 add

					theTopTrack.PerformPrIndex();

					if (theTopTrack.GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_OK)
					{
						if (theTopTrack.GetCurBondUnit() < theTopTrack.m_stIndexerVar.nSrchUnit)
							theTopTrack.m_LFHolder.SetRelayState(RELAY_ON);
						theTopTrack.m_pTargetClaw->SetRelayState(RELAY_ON);
						Sleep_x1ms(400);

						theStepper.MoveDist(theTopTrack.GetIndexStepID(), theTopTrack.m_lOffsetx1um, MOTION_RELATIVE);
						do
						{
							Sleep_x1ms(2);
						} while (theStepper.IsBusy(theTopTrack.GetIndexStepID()));

						theTopTrack.m_pTargetClaw->SetRelayState(RELAY_OFF);
						theTopTrack.m_LFHolder.SetRelayState(RELAY_OFF);
						Sleep_x1ms(10);
		                theTopClamp.CloseClamp();
					}
					else
					{
						theTopTrack.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
					}
				}

				theTopTrack.m_nPRIState=0;	//V1.5.W140 add
			}
		}
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		theTopClamp.CheckClampStatus();
		if (theTopClamp.GetClampStatus()!= WND_CLAMP_SAFE_CLAMP)
		{
			theServo.MoveBondHeadToSafePos();
			Sleep_x1ms(10);
		    theTopClamp.MoveClampSafePos();
		}
		else
		{
			INDEX_PR_REF stPrIndexRefRcd;
         	theDevice.GetIndexPrRef(&stPrIndexRefRcd);

			ST_TE_INDEX_RCD *pstTrackData = theTEIndex.GetRcd();

			theMcConfig.IsPrIndexHalfClamp() == FALSE ? theTopClamp.CloseClamp() : theTopClamp.PreCloseClamp();		//V1.5.W84 edit

			Sleep_x1ms(200);

			if (((pstTrackData->nPrIndexEnable == 2)		//enable 1st unit only
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (theTEIndex.IsPrIndexMask() == false)
				&& (theTEIndex.GetCurBondUnit() == 1)) ||
				((pstTrackData->nPrIndexEnable == 1)		//enable for all
				&& stPrIndexRefRcd.bIndexPrAvail
				&& (theTEIndex.IsPrIndexMask() == false)))
			{
				theTEIndex.m_nPRIState = 1;

				if (theTEIndex.CalcPrIndexOffset(&lOffset) == FOK)
				{
					if (theMcConfig.IsPrIndexHalfClamp() == FALSE)
						theTopClamp.PreCloseClamp();

					if (theTEIndex.GetCurBondUnit() < theTEIndex.m_stIndexVar.nSrchUnit)
						theTEIndex.m_LFHolder.SetRelayState(RELAY_ON);
					theTEIndex.m_pTargetClaw->SetRelayState(RELAY_ON);
					Sleep_x1ms(400);

					theStepper.MoveDist(theTEIndex.GetIndexStepID(), lOffset, MOTION_RELATIVE);

					do
					{
						Sleep_x1ms(2);
					} while (theStepper.IsBusy(theTEIndex.GetIndexStepID()));

					theTEIndex.m_pTargetClaw->SetRelayState(RELAY_OFF);
					theTEIndex.m_LFHolder.SetRelayState(RELAY_OFF);

					Sleep_x1ms(10);
					theMHS.CloseWindowClamp();

					if (theDebug.IsDebugIndexAccuracy())
					{
						Sleep_x1ms(100);
						theTEIndex.m_nPRIState = 2;
						theTEIndex.CalcPrIndexOffset(&lOffset);
						theTEIndex.m_nPRIState = 0;
					}
				}
				else
				{
					if (theMcConfig.IsPrIndexHalfClamp() == FALSE)
						theTopClamp.PreCloseClamp();

					theTEIndex.PerformPrIndex();

					if (theTEIndex.GetIndexPrManualAlign() == INDEX_PR_MANU_ALIGN_OK)
					{
						if (theTEIndex.GetCurBondUnit() < theTEIndex.m_stIndexVar.nSrchUnit)
							theTEIndex.m_LFHolder.SetRelayState(RELAY_ON);
						theTEIndex.m_pTargetClaw->SetRelayState(RELAY_ON);
						Sleep_x1ms(400);

						theStepper.MoveDist(theTEIndex.GetIndexStepID(), theTopTrack.m_lOffsetx1um, MOTION_RELATIVE);
						do
						{
							Sleep_x1ms(2);
						} while (theStepper.IsBusy(theTEIndex.GetIndexStepID()));

						theTEIndex.m_pTargetClaw->SetRelayState(RELAY_OFF);
						theTEIndex.m_LFHolder.SetRelayState(RELAY_OFF);
						Sleep_x1ms(10);
		                theTopClamp.CloseClamp();
					}
					else
					{
						theTEIndex.SetIndexPrManualAlign(INDEX_PR_MANU_ALIGN_OK); //fixed bug: autobond no index
					}
				}

				theTEIndex.m_nPRIState = 0;
			}
		}
		break;
	}
	}

	GetDlgItem(IDC_BUTTON_LF_INDEX)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_CLAMPER)->EnableWindow(true);

	pWbDlg->EnableButton(ID_TOOL_BAR_LIGHT, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_INDEX, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_FINE_ADJUST, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_THREAD_WIRE, TRUE);
	pWbDlg->EnableButton(ID_TOOL_BAR_ELEV, TRUE);
	pWbDlg->EnableButton(IDC_TOOL_BAR_UNPLUG, TRUE);	//V1.1.W93 edit

	//v3.3.D166
	pWbDlg->ShowButton(IDC_TOOL_BAR_UNPLUG, thePgm.IsCapillaryAutoPlug());	//V1.1.W93 edit
	GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

	pWbDlg->SetProcessKeyboard(false);
}

void CIndexCtrlDlg::OnEditIndex()
{
    if (thePgm.IsProgramValid() == false)
    {
        UpdateData(true);
        return;
    }

    short nTotalUnit = thePgm.GetTotalUnit();
	short nBkupUnit = m_nCurUnit;

    UpdateData(true);

    if (m_nCurUnit > nTotalUnit)
    {
        m_nCurUnit = nBkupUnit;
        UpdateData(false);
        return;
    }
    else
        thePgm.GetRowCol(m_nCurUnit, &m_nCurRow, &m_nCurCol);

    UpdateData(false);

    DisplayTableIndex(m_nCurUnit);

    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
    {
        short nUnitTotalWire = thePgm.GetTotalWire() / nTotalUnit;
        theBond.m_nCurWire = (m_nCurUnit - 1) * nUnitTotalWire + 1;
        theBond.m_nRestartWire = 1;  //theBond.m_nCurWire;
        theBond.m_nBondedWire = theBond.m_nCurWire - 1;

		switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
		{
		case MHS_TOP_LED:
		{
			TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
			if ((pstDevice->nHalfIndexOn > 0)
				&& (theTopTrack.GetCurBondUnit() == pstDevice->nHalfIndexOn))
			{
				theBond.m_nRestartWire = theBond.m_nCurWire;  
			}
			break;
		}
		case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
		{
			TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
			if ((pstDevice->nHalfIndexOn > 0)
				&& (theTEIndex.GetCurBondUnit() == pstDevice->nHalfIndexOn))
			{
				theBond.m_nRestartWire = theBond.m_nCurWire;  
			}
			break;
		}
		case MHS_R2R:
		{
			TOP_DEVICE_RECORD *pstDevice = theDevice.GetTopRcd();
			if ((pstDevice->nHalfIndexOn > 0)
				&& (theR2RTrack.GetCurrentBondUnit() == pstDevice->nHalfIndexOn))
			{
				theBond.m_nRestartWire = theBond.m_nCurWire;  
			}
			break;
		}
		case MHS_CU_ELITE:
		{
			CUELITE_DEVICE_RECORD stCuEliteDevicePara;
	        theDevice.GetCuEliteRcd(&stCuEliteDevicePara);

			if (stCuEliteDevicePara.nHalfIndexOn > 0 && 
				theCuEliteTrack.GetCurrentBondUnit() == stCuEliteDevicePara.nHalfIndexOn)
			{
				 theBond.m_nRestartWire = theBond.m_nCurWire;  
			}
			break;
		}
		}
    }
    else
    {
        theBond.m_nCurWire = 1;
        theBond.m_nRestartWire = 1; //theBond.m_nCurWire;
        theBond.m_nBondedWire = theBond.m_nCurWire - 1;
    }

    theBond.m_nCurRow = m_nCurRow;
    theBond.m_nCurCol = m_nCurCol;

    theBond.ResetMatrixPrStatus(1);

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CIndexCtrlDlg::OnEditRow()
{
    short nTotalRow, nTotalCol, nBkupRow;
    
    nTotalRow = thePgm.GetDeviceRowNum();
    nTotalCol = thePgm.GetDeviceColNum();

    nBkupRow = m_nCurRow;

    UpdateData(true);

    if (m_nCurRow > nTotalRow)
    {
        m_nCurRow = nBkupRow;
        UpdateData(false);
        return;
    }
    else
		m_nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
    
    UpdateData(false);

    DisplayTableIndex(m_nCurUnit);

    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
    {
        short nUnitTotalWire = thePgm.GetTotalWire() / (nTotalRow * nTotalCol);
        theBond.m_nCurWire = (m_nCurUnit - 1) * nUnitTotalWire + 1;
        theBond.m_nRestartWire = theBond.m_nCurWire;
        theBond.m_nBondedWire  = theBond.m_nCurWire - 1;
    }

    theBond.m_nCurRow = m_nCurRow;
    theBond.m_nCurCol = m_nCurCol;

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CIndexCtrlDlg::OnEditColumn()
{
    short nTotalCol = thePgm.GetDeviceColNum();
    short nTotalRow = thePgm.GetDeviceRowNum();
    short nBkupCol = m_nCurCol;

    UpdateData(true);

    if (m_nCurCol > nTotalCol)
    {
        m_nCurCol = nBkupCol;
        UpdateData(false);
        return;
    }
    else
		m_nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);

    UpdateData(false);

    DisplayTableIndex(m_nCurUnit);

    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
    {
		if (nTotalRow == 0 || nTotalCol == 0) //throw "w.temp.14" ;
		{
			AfxMessageBox(_T("temp.14, TotalCol || TotalRow = 0"));
			return;
		}
        short nUnitTotalWire = thePgm.GetTotalWire() / (nTotalRow * nTotalCol);
        theBond.m_nCurWire = (m_nCurUnit - 1) * nUnitTotalWire + 1;
        theBond.m_nRestartWire = theBond.m_nCurWire;
        theBond.m_nBondedWire = theBond.m_nCurWire - 1;
    }

    theBond.m_nCurRow = m_nCurRow;
    theBond.m_nCurCol = m_nCurCol;

    ((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_ALL);
}

void CIndexCtrlDlg::OnPrevSubunit()
{
	if (m_nCurUnit < 2) return;

	m_nPrevUnit = m_nCurUnit--;	//V1.1.W155 add
	UpdateData(false);

	OnEditIndex();
}

void CIndexCtrlDlg::OnNextSubunit()
{
	if (m_nCurUnit >= thePgm.GetTotalUnit()) return;

	m_nPrevUnit = m_nCurUnit++;	//V1.1.W155 add
	UpdateData(false);

	OnEditIndex();
}

void CIndexCtrlDlg::OnFocusIndex()
{
	GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();

	short nUnit = m_nCurUnit;
	if (theKeyPad.GetUserKeyInData(&nUnit) == CANCEL_SELECTED) return;

	m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
	m_nCurUnit = nUnit;

	UpdateData(false);

	OnEditIndex();
}

void CIndexCtrlDlg::OnFocusRow()
{
    GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nCurRow);

    UpdateData(false);

    OnEditRow();
}

void CIndexCtrlDlg::OnFocusColumn()
{
    GetDlgItem(IDC_STATIC_UNIT_INFO)->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_nCurCol);

    UpdateData(false);

    OnEditColumn();
}

void CIndexCtrlDlg::OnMatPrev()
{
	if (m_nCurMat < 2) return;

	short nPrevMat;
	if (thePgm.GetPrevMatrix(theMHS.GetCurBondUnit(), m_nCurMat, &nPrevMat) != FOK) return;

	m_nCurMat = nPrevMat;
	thePgm.GetMatRowCol(m_nCurMat, &m_nMatRow, &m_nMatCol);

	theBond.m_nCurMatRow = m_nMatRow;
	theBond.m_nCurMatCol = m_nMatCol;

	m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
	m_nCurUnit = 1;

	UpdateData(false);

	OnEditIndex();
}

void CIndexCtrlDlg::OnMatNext()
{
	if (m_nCurMat >= thePgm.GetMatrixRow() * thePgm.GetMatrixCol()) return;

	short nNextMat, nIndexTime = theMHS.GetCurBondUnit();
	if (thePgm.GetNextMatrix(nIndexTime, m_nCurMat, &nNextMat) != FOK)return;

	m_nCurMat = nNextMat;
	thePgm.GetMatRowCol(m_nCurMat, &m_nMatRow, &m_nMatCol);

	theBond.m_nCurMatRow = m_nMatRow;
	theBond.m_nCurMatCol = m_nMatCol;

	m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
	m_nCurUnit = 1;
	UpdateData(false);

	OnEditIndex();
}

void CIndexCtrlDlg::UpdateMatrix()
{
	SetDlgItemTextW(IDC_BUTTON_LF_INDEX,theMsg.GetMsg(MSG_LF_INDEX));
	SetDlgItemTextW(IDC_BUTTON_CLAMPER,theMsg.GetMsg(MSG_CLAMPER_2));
	SetDlgItemTextW(IDC_BUTTON_CLEAR_TRACK,theMsg.GetMsg(MSG_CLEAR_TRACK));

	switch (theMcConfig.GetMhsType())	//v3.0.T150 edit
	{
	case MHS_TOP_LED:
	{
		SetDlgItemTextW(IDC_BUTTON_RESET, theMsg.GetMsg(MSG_ADJUST));
		theTopTrack.SetRunNoLf(false);
		break;
	}
	case MHS_TOP_ELITE:	//v3.0.T410 add, v3.0.T419 edit
	{
		SetDlgItemTextW(IDC_BUTTON_RESET, theMsg.GetMsg(MSG_ADJUST));
		theTEIndex.SetRunNoLf(false);
		break;
	}
	case MHS_R2R:
	{
		GetDlgItem(IDC_BUTTON_CLEAR_TRACK)->ShowWindow(SW_HIDE);
		SetDlgItemTextW(IDC_BUTTON_RESET,theMsg.GetMsg(MSG_ADJUST));
		theR2RTrack.SetAutoRunWithNoLf(FALSE);
		break;
	}
	case MHS_CU_ELITE:
	{
		SetDlgItemTextW(IDC_BUTTON_RESET,theMsg.GetMsg(MSG_ADJUST));
		theCuEliteTrack.SetAutoRunWithNoLf(FALSE);
		break;
	}
	case MHS_VLED_ELITE:
	{
		SetDlgItemTextW(IDC_BUTTON_LOAD_MAG,theMsg.GetMsg(MSG_LOAD_MAG));
		GetDlgItem(IDC_BUTTON_LOAD_MAG)->ShowWindow(SW_SHOW);

		SetDlgItemTextW(IDC_BUTTON_MOVE_PITCH_FORWARD,theMsg.GetMsg(MSG_MOVE_PITCH_FORWARD));
		GetDlgItem(IDC_BUTTON_MOVE_PITCH_FORWARD)->ShowWindow(SW_SHOW);

		SetDlgItemTextW(IDC_BUTTON_MOVE_PITCH_BACKWARD,theMsg.GetMsg(MSG_MOVE_PITCH_BACKWARD));
		GetDlgItem(IDC_BUTTON_MOVE_PITCH_BACKWARD)->ShowWindow(SW_SHOW);

		SetDlgItemTextW(IDC_BUTTON_EJECT_MAG,theMsg.GetMsg(MSG_EJECT_MAG));
		GetDlgItem(IDC_BUTTON_EJECT_MAG)->ShowWindow(SW_SHOW);
		break;
	}
	case MHS_VLED:
	{
		SetDlgItemTextW(IDC_BUTTON_RESET,theMsg.GetMsg(MSG_RESET_ALL_TRACK));
		break;
	}
	}

	SetDlgItemTextW(IDC_BUTTON_PREV_SUBUNIT,theMsg.GetMsg(MSG_PREV));
	SetDlgItemTextW(IDC_BUTTON_NEXT_SUBUNIT,theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_STATIC_UNIT_INFO,theMsg.GetMsg(MSG_UNIT_INFO));
	SetDlgItemTextW(IDC_STATIC_UNIT_LAYOUT,theMsg.GetMsg(MSG_UNIT_LAYOUT));
	SetDlgItemTextW(IDC_STATIC_CUR_UNIT,theMsg.GetMsg(MSG_CUR_UNIT_COLON));
	SetDlgItemTextW(IDC_STATIC_ROW,theMsg.GetMsg(MSG_ROW)+_T(":"));
	SetDlgItemTextW(IDC_STATIC_COLUMN,theMsg.GetMsg(MSG_COLUMN)+_T(":"));
	SetDlgItemTextW(IDC_STATIC_MATRIX_INFO,theMsg.GetMsg(MSG_MAT_INFO));
	SetDlgItemTextW(IDC_STATIC_MATRIX,theMsg.GetMsg(MSG_CUR_MAT));
	SetDlgItemTextW(IDC_STATIC_MAT_ROW,theMsg.GetMsg(MSG_MAT_ROW_COLON));
	SetDlgItemTextW(IDC_STATIC_MAT_COL,theMsg.GetMsg(MSG_MAT_COL_COLON));

	SetDlgItemTextW(IDC_BUTTON_MAT_PREV,theMsg.GetMsg(MSG_PREV));
	SetDlgItemTextW(IDC_BUTTON_MAT_NEXT,theMsg.GetMsg(MSG_NEXT));

	FreeButtonArray();

	if (thePgm.IsProgramValid())
	{
		short nTotalRow = thePgm.GetDeviceRowNum();
		short nTotalCol = thePgm.GetDeviceColNum();

		if (nTotalCol == 0 || nTotalRow == 0) //throw "w.temp.13" ;
		{
			AfxMessageBox(_T("temp.13, TotalCol || TotalRow = 0"));
			return;
		}

		short nUnitWidth  = round((double)LF_INDEX_AREA_WIDTH  / nTotalCol);
		short nUnitHeight = round((double)LF_INDEX_AREA_HEIGHT / nTotalRow);
		nUnitWidth  = min(nUnitWidth, nUnitHeight);

		CPoint pt, StartPt, pCentre;
		CRect pUnitRect;

		pCentre.x = LF_INDEX_AREA_LEFT + LF_INDEX_AREA_WIDTH / 2 + 14;
		pCentre.y = LF_INDEX_AREA_TOP + LF_INDEX_AREA_HEIGHT / 2 - 16;

		pt.x = pCentre.x + round(nUnitWidth * (nTotalCol - 1) / 2.0);
		pt.y = pCentre.y - round(nUnitWidth * (nTotalRow - 1) / 2.0);

		StartPt = pt;

		short i = 0, j = 0, nUnitId = IDB_SUBUNIT_BUTTON0;
		for (i = 0; i < nTotalCol; i++)
		{
			pt.y = StartPt.y;

			for (j = 0; j < nTotalRow; j++)
			{
				CColorButton* pButton = new CColorButton();
				pUnitRect.SetRect(pt.x - nUnitWidth / 2, pt.y - nUnitWidth / 2, pt.x + nUnitWidth / 2, pt.y + nUnitWidth / 2);
				pButton->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, pUnitRect, this, nUnitId++);
				pButton->SetBkColor(theApp.m_bkColor);

				m_apButton.Add(pButton);

				pt.y += nUnitWidth;
			}

			pt.x -= nUnitWidth;
		}

		m_nCurRow = theBond.m_nCurRow;
		m_nCurCol = theBond.m_nCurCol;

		m_nMatRow = theBond.m_nCurMatRow;
		m_nMatCol = theBond.m_nCurMatCol;

		m_nCurMat = thePgm.GetMatNum(m_nMatRow, m_nMatCol);

		GetDlgItem(IDC_EDIT_INDEX)->EnableWindow();
		GetDlgItem(IDC_EDIT_ROW)->EnableWindow();
		GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow();

		bool bEnable = (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
						|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID);
		GetDlgItem(IDC_BUTTON_MAT_PREV)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_MAT_NEXT)->EnableWindow(bEnable);

		m_nCurUnit = thePgm.GetUnitNum(m_nCurRow, m_nCurCol);
		DisplayTableIndex(m_nCurUnit);
	}
	else
	{
		m_nCurRow = 1;
		m_nCurCol = 1;
		m_nPrevUnit = m_nCurUnit;	//V1.1.W155 add
		m_nCurUnit = 1;

		m_nMatRow = theBond.m_nCurMatRow;
		m_nMatCol = theBond.m_nCurMatCol;
		m_nCurMat = thePgm.GetMatNum(m_nMatRow, m_nMatCol);

		GetDlgItem(IDC_EDIT_INDEX)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_ROW)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_MAT_PREV)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MAT_NEXT)->EnableWindow(FALSE);
	}

	UpdateData(false);
}

void CIndexCtrlDlg::OnLoadMag()
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	DWORD dwStartTime = GetCurrentTime();
	theVEliteOffload.SubmitEvent(VE_OFFLOAD_EVENT_LOAD_MAG);
	do
	{
		theVEliteOffload.ProcessEvent();
		Sleep_x1ms(10);
	} while ((GetCurrentTime() - dwStartTime < 8000)
			&& (theVEliteOffload.GetEvent() != VE_OFFLOAD_EVENT_IDLE));

	theMHS.ShowError();
}

void CIndexCtrlDlg::OnMovePitchForward()
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	DWORD dwStartTime = GetCurrentTime();
	theVEliteOffload.SubmitEvent(VE_OFFLOAD_EVENT_MAG_MOVE_PITCH_FORWARD);
	do
	{
		theVEliteOffload.ProcessEvent();
		Sleep_x1ms(10);
	} while ((GetCurrentTime() - dwStartTime < 8000)
			&& (theVEliteOffload.GetEvent() != VE_OFFLOAD_EVENT_IDLE));

	theMHS.ShowError();
}

void CIndexCtrlDlg::OnMovePitchBackward()
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	DWORD dwStartTime = GetCurrentTime();
	theVEliteOffload.SubmitEvent(VE_OFFLOAD_EVENT_MAG_MOVE_PITCH_BACKWARD);
	do
	{
		theVEliteOffload.ProcessEvent();
		Sleep_x1ms(10);
	} while ((GetCurrentTime() - dwStartTime < 8000)
			&& (theVEliteOffload.GetEvent() != VE_OFFLOAD_EVENT_IDLE));

	theMHS.ShowError();
}

void CIndexCtrlDlg::OnEjectMag()
{
	CBusy bs(&m_bBusy);	//V1.1.W155 add

	DWORD dwStartTime = GetCurrentTime();
	theVEliteOffload.SubmitEvent(VE_OFFLOAD_EVENT_EJECT_MAG);
	do
	{
		theVEliteOffload.ProcessEvent();
		Sleep_x1ms(10);
	} while ((GetCurrentTime() - dwStartTime < 8000)
			&& (theVEliteOffload.GetEvent() != VE_OFFLOAD_EVENT_IDLE));

	theMHS.ShowError();
}

LRESULT CIndexCtrlDlg::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W155 add
{
	return (m_bBusy==FALSE&&ShowWindow(SW_HIDE))*DIALOG_CLOSED;
}

void CIndexCtrlDlg::OnShowWindow(BOOL bShow, UINT nStatus)		//V1.1.W155 add
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow==TRUE)return;

    theMHS.PreCloseWindowClamp();
}
