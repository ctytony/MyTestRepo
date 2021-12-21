/***************************************************************
ChgCapillaryDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "Math.h"
#include "..\GkgLib\MathCalc\MathCalc.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "..\GkgLib\MachineData\MachineData.h"
#include "..\GkgLib\Program\Program.h"
#include "..\GkgLib\BondPara\BondPara.h"
#include "WireBondDlg.h"
#include "..\GkgLib\MachineConfig\MachineConfig.h"
#include "MaterialHandlerThread.h"
#include "Bond.h"
#include "..\GkgLib\BondStatistics\BondStatistics.h"
#include "ChgCapillaryDlg.h"
#include "..\GkgLib\TopClamp\TopClamp.h"
#include "..\GkgLib\CuClamp\CuClamp.h"
#include "servo.h"

// CChgCapillaryDlg dialog

IMPLEMENT_DYNAMIC(CChgCapillaryDlg, CDialog)

CChgCapillaryDlg::CChgCapillaryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChgCapillaryDlg::IDD, pParent)
    , m_nStep(0)
{

}

CChgCapillaryDlg::~CChgCapillaryDlg()
{
}

void CChgCapillaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChgCapillaryDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CChgCapillaryDlg message handlers

BOOL CChgCapillaryDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    ((CWireBondDlg*)(this->GetParent()))->InhibitSystemButton();

    ModifyStyle(WS_CAPTION, 0);

    CRect Rect;
	(this->GetParent())->GetWindowRect(&Rect);
    
    //this->SetWindowPos(NULL, Rect.left, LIGHT_DLG_TOP, TREE_DLG_WIDTH, LIGHT_DLG_HEIGHT, SWP_NOZORDER);
	CPoint pt(Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP);
    SetWindowPos(NULL, pt.x, pt.y, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    CWnd* pWnd;
    CString szString;

    pt.x = 130;
    pt.y = 430;

    pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_ENTER, szString);
    pWnd->SetWindowTextW(szString);

    pt.x += BUTTON_BOND_WIDTH;

    pWnd = GetDlgItem(IDCANCEL);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_ESCAPE, szString);
    pWnd->SetWindowTextW(szString);
   
    pWnd = GetDlgItem(IDC_STATIC_TITLE);
    pWnd->SetWindowPos(NULL, 60, 15, 350, 100, SWP_NOZORDER);
    font.CreateFontW(38, 0, 0, 0, 500, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial")); //@2
	pWnd->SetFont(&font);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_CHANGE_CAPILLARY_PROCEDURE, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_STATIC_MSG_1);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_STEP1, szString);
    pWnd->SetWindowTextW(szString);
	
    pWnd = GetDlgItem(IDC_STATIC_MSG_2);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_REMOVE_OLD_CAPILLARY, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_STATIC_MSG_3);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_INSERT_NEW_CAPILLARY, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_STATIC_MSG_4);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_PRESS_ENTER_TO_CONTINUE, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_STATIC_MSG_5);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_PRESS_ESC_TO_CANCEL, szString);
    pWnd->SetWindowTextW(szString);

    m_nStep = 0;

    if (theOperMgr.IsOffline() == false)
    {
        if (theMacConfig.GetMhsType() == TOP_MHS ||
			theMacConfig.GetMhsType() == R2R_MHS)
        {
            long lStartTime;

            theTopLfClamp.CloseClamp();
            Sleep(10);

            lStartTime = GetCurrentTime();
            do
            {
                theTopLfClamp.CheckClampStatus();
                Sleep(2);
                if (GetCurrentTime() - lStartTime > 4000) break;
            } while (theTopLfClamp.GetClampStatus() != WND_CLAMP_CLOSE);
        }
        else if (theMacConfig.GetMhsType() == CU_ELITE_MHS)
        {
            long lStartTime;

            theCuEliteLFClamp.CloseClamp();
            Sleep(10);
            lStartTime=GetCurrentTime();

            do
            {
                theCuEliteLFClamp.CheckClampStatus();
                Sleep(2);
                if(GetCurrentTime() - lStartTime > 4000)
                    break;
            }
            while (theCuEliteLFClamp.GetClampStatus() != CUELITE_WND_CLAMP_CLOSE);
        }
        theServo.SearchLimit(MOTOR_Z);
    }

    Sleep(20);

    WCHAR wcMsg[80];
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_CONTACT_SRCH, wcMsg);

    m_ptheTeachRecipeSheet = new CTeachRecipeSheet(wcMsg, RECIPE_SETUP); //@1

    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CChgCapillaryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CChgCapillaryDlg::PostNcDestroy()
{
    // TODO: Add your specialized code here and/or call the base class

    CDialog::PostNcDestroy();

    delete this;
}

void CChgCapillaryDlg::OnDestroy()
{
    ((CWireBondDlg*)(this->GetParent()))->EnableSystemButton();

    CDialog::OnDestroy();

    // TODO: Add your message handler code here
}

void CChgCapillaryDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    long            lStatus = FOK, lStartTime, lElapseTime, lCurDac, lTailLength;
    double          dPosError, dForceDac, dCtactLvl;
    DUMMY_BOND_PARA stDummyBondPara;
    MACHINE_DATA    stMcData;
    PGM_RECORD      stPgmRcd;
    CWnd*           pWnd;
    CString         szString;

    theMachineData.GetMachineDataRcd(&stMcData);

    m_nStep++;

    switch(m_nStep)
    {
    case 1:
        {
            pWnd = GetDlgItem(IDC_STATIC_MSG_1);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_STEP2, szString);
            pWnd->SetWindowTextW(szString);

            pWnd = GetDlgItem(IDC_STATIC_MSG_2);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_INSERT_CAPILLARY_JIG, szString);
            pWnd->SetWindowTextW(szString);

            pWnd = GetDlgItem(IDC_STATIC_MSG_3);
            szString = _T("");
            pWnd->SetWindowTextW(szString);

            UpdateData(FALSE);
        }

        break;
    case 2:
        {
            pWnd = GetDlgItem(IDC_STATIC_MSG_1);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_STEP3, szString);
            pWnd->SetWindowTextW(szString);

            pWnd = GetDlgItem(IDC_STATIC_MSG_2);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_TIGHT_CAPILLARY, szString);
            pWnd->SetWindowTextW(szString);

            UpdateData(FALSE);

            theBondParaMgr.GetDummyBondPara(&stDummyBondPara);

            if (theOperMgr.IsOffline() == false)
            {
                theServo.StartJogZ(MOTION_NEGATIVE, stDummyBondPara.nSrchSpeed);
                Sleep(40);

                lStartTime = GetCurrentTime();
                do
                {
                    lStatus = theServo.GetPositionError(MOTOR_Z, &dPosError);
                    if (lStatus != FOK) break;

                    lElapseTime = GetCurrentTime() - lStartTime;
                    if (lElapseTime > SERVO_TIMEOUT) break;
                } while (fabs(dPosError) < stDummyBondPara.nSrchTol);

                theServo.EndJogZ();

                theServo.GetBondHeadEncPosition(&dCtactLvl);

                dForceDac = (double)(-CHANGE_CAPILLARY_FORCE) * stMcData.dForceFactorKf + dCtactLvl * stMcData.dForceFactorKa + stMcData.dForceFactorIo;

                theServo.ReadBondHeadMFlags(&m_lBkupDac);

                lCurDac = m_lBkupDac | 0x0002;
                lCurDac = lCurDac & 0xFFFDFFFF;

                theServo.WriteBondHeadMFlags(lCurDac);
                theServo.WriteBondHeadDcom(dForceDac);
            }

            pWnd = GetDlgItem(IDCANCEL);
            pWnd->EnableWindow(FALSE);
        }

        break;
    case 3:
        {
            pWnd = GetDlgItem(IDC_STATIC_MSG_1);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_STEP3, szString);
            pWnd->SetWindowTextW(szString);

            pWnd = GetDlgItem(IDC_STATIC_MSG_2);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_REMOVE_CAPILLARY_JIG, szString);
            pWnd->SetWindowTextW(szString);

            UpdateData(FALSE);

            theProgram.GetRcd(&stPgmRcd);
            lTailLength = round((double)(stPgmRcd.nTailLengthUm) / theServo.GetBondHeadResolution());

            dForceDac = 0.0;

            if (theOperMgr.IsOffline() == false)
            {
                theServo.WriteBondHeadMFlags(m_lBkupDac);
                theServo.WriteBondHeadDcom(dForceDac);

                theServo.MoveAxisZ(lTailLength, MOTION_RELATIVE, MOTION_WITH_WAIT);
                Sleep(50);

                theBond.MoveBondHeadToFireLevel();
            }
            
            pWnd = GetDlgItem(IDCANCEL);
            pWnd->EnableWindow(TRUE);
        }

        break;
    case 4:
        {
            pWnd = GetDlgItem(IDC_STATIC_MSG_1);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_STEP4, szString);
            pWnd->SetWindowTextW(szString);

            pWnd = GetDlgItem(IDC_STATIC_MSG_2);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_CHECK_IMPEDANCE, szString);
            pWnd->SetWindowTextW(szString);

            UpdateData(FALSE);

            if (theProgram.IsProgramValid() == false)
            {
                m_nStep = 5;
                delete(m_ptheTeachRecipeSheet);
            }
        }

        break;

    case 5:
        {
            m_ptheTeachRecipeSheet->Create(this); //@1

            pWnd = GetDlgItem(IDC_STATIC_MSG_1);
            theMsgMgr.GetMsgString(MSG_MENU_ITEM_STEP5, szString);
            pWnd->SetWindowTextW(szString);

            pWnd = GetDlgItem(IDC_STATIC_MSG_2);
            theMsgMgr.GetMsgString(MSG_CTACT_SEARCH_SUCCESSFUL, szString);
            pWnd->SetWindowTextW(szString);

            UpdateData(FALSE);
        }

        break;

    case 6:
        theBondStatistics.ResetCapillaryCount();

        DestroyWindow();
        break;
    default:
        break;
    }
}

void CChgCapillaryDlg::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialog::OnCancel();
    if (m_nStep < 2)
        theBond.MoveBondHeadToFireLevel();

    DestroyWindow();
}
