/***************************************************************
BondEditPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "Wire.h"
#include "BondEditPage.h"
#include "Bond.h"
#include "VisionSystem.h"
#include "WireBondDlg.h"
#include "DlgDelBkPR.h"
#include "servo.h"

// CBondEditPage dialog

IMPLEMENT_DYNAMIC(CBondEditPage, CPropertyPage)

CBondEditPage::CBondEditPage()
	: CPropertyPage(CBondEditPage::IDD)
    , m_nAlignNum(1)
    , m_nBondPoint(0)
	, m_nBakUpId(0)
	, m_iAutoSwapMainPr(0)
	, m_nCurrentWireNum(1)
{
}

CBondEditPage::~CBondEditPage()
{
}

void CBondEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_AUTO_SWAP_PR, m_iAutoSwapMainPr);
}

BEGIN_MESSAGE_MAP(CBondEditPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_WIRE_NUM, OnEditWireNum)
	ON_EN_SETFOCUS(IDC_EDIT_WIRE_NUM, OnFocusWireNum)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_BK_PR_1, OnBkPr1)
	ON_BN_CLICKED(IDC_BUTTON_BK_PR_2, OnBkPr2)
	ON_BN_CLICKED(IDC_BUTTON_BK_PR_3, OnBkPr3)
	ON_BN_CLICKED(IDC_BUTTON_BK_PR_4, OnBkPr4)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALL, DeleteAllBackupPr)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_ALL, OnReplaceAll)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTO_SWAP_PR, OnAutoSwapPr)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CBondEditPage message handlers

HBRUSH CBondEditPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CBondEditPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add
	m_nCurrentWireNum = theBond.m_nCurWire;

    CWnd* pWnd = GetDlgItem(IDC_CURRENT_WIRE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CURRENT_WIRE));

	SetDlgItemInt(IDC_EDIT_WIRE_NUM, m_nCurrentWireNum);

    pWnd = GetDlgItem(IDC_COMBO_BOND_POINT);
    ((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_1ST_BOND_POINT));
    ((CComboBox*)pWnd)->AddString(theMsg.GetMsg(MSG_2ND_BOND_POINT));
	((CComboBox*)pWnd)->SetCurSel(0);

    pWnd = GetDlgItem(IDC_BOND_POINT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BOND_POINT));

    pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEW));

	pWnd = GetDlgItem(IDC_BUTTON_UPDATE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_UPDATE));
    pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_STATIC_BK_PR_VIEW);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BACKUP_ID));

	pWnd = GetDlgItem(IDC_BUTTON_REPLACE_ALL);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SWAP));
	pWnd->EnableWindow(FALSE);

	SetDlgItemTextW(IDC_BUTTON_DELETE, theMsg.GetMsg(MSG_DELETE));

	pWnd = GetDlgItem(IDC_BUTTON_DELETE_ALL);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_DELETE_ALL));
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_STATIC_AUTO_SWAP_PR);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_AUTO_SWAP_MAIN_PR));

	m_iAutoSwapMainPr = (thePgm.GetRcd()->bBkPrAutoSwap == true);

	m_nBondPoint = theWire.GetRcd(m_nCurrentWireNum)->nBkPrBondPoint;

	pWnd = GetDlgItem(IDC_COMBO_BOND_POINT);
	((CComboBox*)pWnd)->SetCurSel(m_nBondPoint);

	for (int i = 0; i < MAX_BAK_UP_PR_NUM; i++)
		GetDlgItem(IDC_BUTTON_BK_PR_1+i)->EnableWindow(FALSE);

	UpdateBkPrView();

    return FALSE;
}

void CBondEditPage::OnEditWireNum()
{
	theBond.m_nCurWire = m_nCurrentWireNum;
	theBond.MoveToWirePoint(m_nCurrentWireNum, m_nBondPoint);

	if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
	{
		if (thePgm.GetTotalUnit() == 0)
		{
			AfxMessageBox(_T("temp.11, TotalUnit = 0"));
			return;
		}

		short nUnitTotalWire = thePgm.GetTotalWire() / thePgm.GetTotalUnit();
		if (nUnitTotalWire == 0)
		{
			AfxMessageBox(_T("temp.12, UnitTotalWire = 0"));
			return;
		}

		short nUnitNum = (m_nCurrentWireNum - 1) / nUnitTotalWire + 1;
		thePgm.GetRowCol(nUnitNum, &(theBond.m_nCurRow), &(theBond.m_nCurCol));
	}
    
    theVisionDisplay.SetDisplayWire(m_nCurrentWireNum);
    theVisionDisplay.SetDisplayBondPt(m_nBondPoint);
    theVisionDisplay.UpdateVisionDisplay(true);
    Sleep(50);

    ((CWireBondDlg*)((GetParent())->GetParent()))->UpdateInfoView(INFOVIEW_ALL);
}

void CBondEditPage::OnFocusWireNum()
{
	GetDlgItem(IDC_CURRENT_WIRE)->SetFocus();

    if (theKeyPad.GetUserKeyInData(&m_nCurrentWireNum, m_nCurrentWireNum, 1, thePgm.GetTotalWire()) == CANCEL_SELECTED)
		return;

	UpdateBkPrView();
    SetDlgItemInt(IDC_EDIT_WIRE_NUM, m_nCurrentWireNum);
}

void CBondEditPage::UpdateBkPrView(void)
{
	const WIRE_RECORD *pstWire = theWire.GetRcd(m_nCurrentWireNum);
	REF_RECORD stRefRcd;
	theRef.GetRcd(&stRefRcd, pstWire->nRefNum[pstWire->nBkPrBondPoint]);

	((CComboBox*)GetDlgItem(IDC_COMBO_BOND_POINT))->SetCurSel(pstWire->nBkPrBondPoint);
	GetDlgItem(IDC_COMBO_BOND_POINT)->EnableWindow(stRefRcd.nBakUpIndex < 1);

	if (stRefRcd.nBakUpIndex > MAX_BAK_UP_PR_NUM)
		stRefRcd.nBakUpIndex = MAX_BAK_UP_PR_NUM;

	for (int i= 0; i < MAX_BAK_UP_PR_NUM; i++)
		GetDlgItem(IDC_BUTTON_BK_PR_1 + i)->EnableWindow(i < stRefRcd.nBakUpIndex);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(stRefRcd.nBakUpIndex<MAX_BAK_UP_PR_NUM);
	GetDlgItem(IDC_BUTTON_DELETE_ALL)->EnableWindow(stRefRcd.nBakUpIndex>0);
	GetDlgItem(IDC_BUTTON_REPLACE_ALL)->EnableWindow(stRefRcd.nBakUpIndex>0);
}

void CBondEditPage::OnStart()
{
	PrepareTchBkPr();

	const WIRE_RECORD *pstWire = theWire.GetRcd(theBond.m_nCurWire);
	const REF_RECORD *pstRef = theRef.GetRcd(pstWire->nRefNum[m_nBondPoint]);
	m_nBakUpId = pstRef->nBakUpIndex;
}

void CBondEditPage::OnUpdate()
{
    short i, nCurWire, nSideRedLight, nSideBlueLight, nUnitTotalDie, nUnitDieNum;
    short nPatRcd;
    bool bRotateEnable;

	WIRE_RECORD stWireRcd;
	REF_RECORD stRefRcd;
	REF_RECORD stCurRef;
    TBL_POS stTeachPos, stVisionAlignDist, stBkupAlignDist, stMovePos;
    MATRIX_RECORD stMatrixRcd;

    nCurWire = theBond.m_nCurWire;

    theWire.GetRcd(&stWireRcd, nCurWire);
    theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[m_nBondPoint]);

    theMatrix.GetRcd(&stMatrixRcd, stWireRcd.nRefNum[m_nBondPoint]);
    if (stMatrixRcd.nVisionStatus != VISION_SUCCESS) return;

    theVisionSys.EnableVideo(false);
    theVisionSys.GrabImage();

    if (stRefRcd.nLocateMode == REF_LOCATE_SINGLE_POINT)
        bRotateEnable = true;
    else
        bRotateEnable = false;

	theVisionSys.GetSearchSize(
		&stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeX[m_nAlignNum - 1],
		&stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeY[m_nAlignNum - 1]);

	stRefRcd.stBakUpPr[m_nBakUpId].nAlgo[m_nAlignNum - 1] = 0;

	int iErr = theVisionSys.LearnVisionPattern(bRotateEnable,
			stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeX[m_nAlignNum - 1], 
			stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeY[m_nAlignNum - 1], &nPatRcd);
	if (iErr != FOK) return;
	if (stRefRcd.stBakUpPr[m_nBakUpId].bValid)
		theVisionSys.ClearPatternRcd(
			(short)stRefRcd.stBakUpPr[m_nBakUpId].lPatID[m_nAlignNum - 1]);
	stRefRcd.stBakUpPr[m_nBakUpId].lPatID[m_nAlignNum - 1] = nPatRcd;

    if (!theMcConfig.IsBypassAll())
        theServo.GetTablePosition(&stTeachPos);
    else
    {
        stTeachPos.dx = 29000.0 / theServo.GetResolXY();
        stTeachPos.dy =  6900.0 / theServo.GetResolXY();
    }

    theMatrix.ReverseTransformPos(stWireRcd.nRefNum[m_nBondPoint], &stTeachPos);
    stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1] = stTeachPos;

    if (stRefRcd.nLocateMode == REF_LOCATE_SINGLE_POINT)
    {
        stRefRcd.stBakUpPr[m_nBakUpId].dAlignSin = -stMatrixRcd.dSinAln;
        stRefRcd.stBakUpPr[m_nBakUpId].dAlignCos = stMatrixRcd.dCosAln;

        stVisionAlignDist.dx = stRefRcd.stVisionAlignPos[1].dx - stRefRcd.stVisionAlignPos[0].dx;
        stVisionAlignDist.dy = stRefRcd.stVisionAlignPos[1].dy - stRefRcd.stVisionAlignPos[0].dy;
        stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dx =
			stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx
			+ stVisionAlignDist.dx;
        stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dy =
			stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy
			+ stVisionAlignDist.dy;
    }
    else if ((thePgm.GetRcd()->nPrAlignMode == 1)
			&& (stRefRcd.nRefType == REF_TYPE_LEAD)
			&& (m_nAlignNum == 1))
    {
        stVisionAlignDist.dx = stRefRcd.stVisionAlignPos[1].dx - stRefRcd.stVisionAlignPos[0].dx;
        stVisionAlignDist.dy = stRefRcd.stVisionAlignPos[1].dy - stRefRcd.stVisionAlignPos[0].dy;
        stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dx =
			stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum
			- 1].dx + stVisionAlignDist.dx;
        stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dy =
			stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum
			- 1].dy + stVisionAlignDist.dy;
    }

    theVisionSys.GetLightingParameter(
		&(stRefRcd.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1].nCoaxRedLight), 
        &nSideRedLight, 
        &(stRefRcd.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1].nCoaxBlueLight),
        &nSideBlueLight);

    stRefRcd.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1].bSideRedLight  = (byte)nSideRedLight;
    stRefRcd.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1].bSideBlueLight = (byte)nSideBlueLight;
    
    if ((stRefRcd.nLocateMode == REF_LOCATE_SINGLE_POINT)
		|| ((thePgm.GetRcd()->nPrAlignMode == 1) && (stRefRcd.nRefType == REF_TYPE_LEAD))
		|| (m_nAlignNum == 2))
    {
        stRefRcd.stBakUpPr[m_nBakUpId].bValid = true;
    }

	if (m_nAlignNum == 1)
    {
        if (stRefRcd.nBakUpIndex < m_nBakUpId +1)
	        stRefRcd.nBakUpIndex = m_nBakUpId +1;
    }

    theRef.SetRcd(&stRefRcd, stWireRcd.nRefNum[m_nBondPoint]);

	stWireRcd.nBkPrBondPoint = (BYTE)m_nBondPoint;
	theWire.SetRcd(&stWireRcd, nCurWire);

    if (stRefRcd.nLocateMode == REF_LOCATE_DUAL_POINT)
    {
        if (((thePgm.GetRcd()->nPrAlignMode == 1) && (stRefRcd.nRefType == REF_TYPE_LEAD))
			|| (m_nAlignNum == 2))
            theRef.CalcRefAngle(stWireRcd.nRefNum[m_nBondPoint], m_nBakUpId+2);
        else
        {
            if (stRefRcd.stBakUpPr[m_nBakUpId].bValid == false)
                stMovePos = stRefRcd.stVisionAlignPos[1];
            else
                stMovePos = stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[1];
            theMatrix.TransformPos(stWireRcd.nRefNum[m_nBondPoint], &stMovePos);
            theServo.MoveTable2(stMovePos);	//v3.1.T195 edit

            if (stRefRcd.stBakUpPr[m_nBakUpId].bValid == false)
                theVisionSys.ShowVisionPattern(
					(short)stRefRcd.ulVisionAlignPatternID[1]);
            else
                theVisionSys.ShowVisionPattern(
					(short)stRefRcd.stBakUpPr[m_nBakUpId].lPatID[1]);
        }
    }

    stBkupAlignDist.dx = stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx
							- stRefRcd.stVisionAlignPos[m_nAlignNum - 1].dx;
    stBkupAlignDist.dy = stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy
							- stRefRcd.stVisionAlignPos[m_nAlignNum - 1].dy;

    if ((thePgm.GetDeviceSeq() == PGM_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_DIE_AHEAD)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)
		|| (thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD))
    {
        nUnitTotalDie = thePgm.GetUnitTotalDie();
        nUnitDieNum = stWireRcd.nRefNum[m_nBondPoint] % nUnitTotalDie;
        
        if (stWireRcd.nRefNum[m_nBondPoint] != 0)
        {
            for (i = 1; i < theRef.GetTotalRefNum(); i++)
            {
                if ((i % nUnitTotalDie) == nUnitDieNum)
                {
                    theRef.GetRcd(&stCurRef, i);
                    
                    stCurRef.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1] =
						stRefRcd.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1];
                    stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx =
						stCurRef.stVisionAlignPos[m_nAlignNum - 1].dx + stBkupAlignDist.dx;
                    stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy =
						stCurRef.stVisionAlignPos[m_nAlignNum - 1].dy + stBkupAlignDist.dy;
                    stCurRef.stBakUpPr[m_nBakUpId].lPatID[m_nAlignNum - 1] =
						stRefRcd.stBakUpPr[m_nBakUpId].lPatID[m_nAlignNum - 1];
                    stCurRef.stBakUpPr[m_nBakUpId].nAlgo[m_nAlignNum - 1] =
						stRefRcd.stBakUpPr[m_nBakUpId].nAlgo[m_nAlignNum - 1];

                    if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
                    {
						stCurRef.stBakUpPr[m_nBakUpId].bValid = true;

                        stCurRef.stBakUpPr[m_nBakUpId].dAlignSin =
							stRefRcd.stBakUpPr[m_nBakUpId].dAlignSin;
                        stCurRef.stBakUpPr[m_nBakUpId].dAlignCos =
							stRefRcd.stBakUpPr[m_nBakUpId].dAlignCos;

                        stVisionAlignDist.dx = stCurRef.stVisionAlignPos[1].dx - stCurRef.stVisionAlignPos[0].dx;
                        stVisionAlignDist.dy = stCurRef.stVisionAlignPos[1].dy - stCurRef.stVisionAlignPos[0].dy;
                        stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dx =
							stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx
							+ stVisionAlignDist.dx;
                        stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dy =
							stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy
							+ stVisionAlignDist.dy;
                    }
                    else if ((thePgm.GetRcd()->nPrAlignMode == 1)
								&& (stRefRcd.nRefType == REF_TYPE_LEAD) && (m_nAlignNum == 1))
                    {
                        stVisionAlignDist.dx = stCurRef.stVisionAlignPos[1].dx
												- stCurRef.stVisionAlignPos[0].dx;
                        stVisionAlignDist.dy = stCurRef.stVisionAlignPos[1].dy
												- stCurRef.stVisionAlignPos[0].dy;
                        stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dx =
							stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx
							+ stVisionAlignDist.dx;
                        stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dy =
							stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy
							+ stVisionAlignDist.dy;
                    }
					else if (m_nAlignNum == 2)
					{
						stCurRef.stBakUpPr[m_nBakUpId].bValid = true;
					}
					
                    stCurRef.stBakUpPr[m_nBakUpId].nSrchSizeX[m_nAlignNum - 1] =
						stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeX[m_nAlignNum - 1];
                    stCurRef.stBakUpPr[m_nBakUpId].nSrchSizeY[m_nAlignNum - 1] =
						stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeY[m_nAlignNum - 1];

					if (stCurRef.nBakUpIndex < m_nBakUpId +1)
						stCurRef.nBakUpIndex = m_nBakUpId +1;

                    theRef.SetRcd(&stCurRef, i);

                    if (stCurRef.nLocateMode == REF_LOCATE_DUAL_POINT)
                    {
                        if (((thePgm.GetRcd()->nPrAlignMode == 1)
							&& (stRefRcd.nRefType == REF_TYPE_LEAD))
							|| (m_nAlignNum == 2))
                            theRef.CalcRefAngle(i, m_nBakUpId+2);
                    }
                }
            }
        }
    }

    if (thePgm.GetDeviceSeq() == PGM_DIE_LEAD_AHEAD)
    {
        nUnitTotalDie = thePgm.GetUnitTotalDie() + 1;
        nUnitDieNum = stWireRcd.nRefNum[m_nBondPoint] % nUnitTotalDie;
        
        for (i = 0; i < theRef.GetTotalRefNum(); i++)
        {
            if ((i % nUnitTotalDie) == nUnitDieNum)
            {
                theRef.GetRcd(&stCurRef, i);
                stCurRef.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1] =
					stRefRcd.stBakUpPr[m_nBakUpId].stLight[m_nAlignNum - 1];
                stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx =
					stCurRef.stVisionAlignPos[m_nAlignNum - 1].dx + stBkupAlignDist.dx;
                stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy =
					stCurRef.stVisionAlignPos[m_nAlignNum - 1].dy + stBkupAlignDist.dy;
                stCurRef.stBakUpPr[m_nBakUpId].lPatID[m_nAlignNum - 1] =
					stRefRcd.stBakUpPr[m_nBakUpId].lPatID[m_nAlignNum - 1];
				stCurRef.stBakUpPr[m_nBakUpId].nAlgo[m_nAlignNum - 1] =
					stRefRcd.stBakUpPr[m_nBakUpId].nAlgo[m_nAlignNum - 1];

                if (stCurRef.nLocateMode == REF_LOCATE_SINGLE_POINT)
                {
					stCurRef.stBakUpPr[m_nBakUpId].bValid = true;

                    stCurRef.stBakUpPr[m_nBakUpId].dAlignSin =
							stRefRcd.stBakUpPr[m_nBakUpId].dAlignSin;
                    stCurRef.stBakUpPr[m_nBakUpId].dAlignCos =
							stRefRcd.stBakUpPr[m_nBakUpId].dAlignCos;

                    stVisionAlignDist.dx = stCurRef.stVisionAlignPos[1].dx
											- stCurRef.stVisionAlignPos[0].dx;
                    stVisionAlignDist.dy = stCurRef.stVisionAlignPos[1].dy
											- stCurRef.stVisionAlignPos[0].dy;
                    stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dx =
						stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx
						+ stVisionAlignDist.dx;
                    stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dy =
						stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy
						+ stVisionAlignDist.dy;
                }
                else if ((thePgm.GetRcd()->nPrAlignMode == 1)
							&& (stRefRcd.nRefType == REF_TYPE_LEAD)
							&& (m_nAlignNum == 1))
                {
                    stVisionAlignDist.dx = stCurRef.stVisionAlignPos[1].dx
											- stCurRef.stVisionAlignPos[0].dx;
                    stVisionAlignDist.dy = stCurRef.stVisionAlignPos[1].dy
											- stCurRef.stVisionAlignPos[0].dy;
                    stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dx =
						stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dx
						+ stVisionAlignDist.dx;
                    stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum].dy =
						stCurRef.stBakUpPr[m_nBakUpId].stAlignPos[m_nAlignNum - 1].dy
						+ stVisionAlignDist.dy;
                }
				else if (m_nAlignNum == 2)
				{
					stCurRef.stBakUpPr[m_nBakUpId].bValid = true;
				}

                stCurRef.stBakUpPr[m_nBakUpId].nSrchSizeX[m_nAlignNum - 1] =
					stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeX[m_nAlignNum - 1];
                stCurRef.stBakUpPr[m_nBakUpId].nSrchSizeY[m_nAlignNum - 1] =
					stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeY[m_nAlignNum - 1];

				if (stCurRef.nBakUpIndex < m_nBakUpId + 1)
					stCurRef.nBakUpIndex = m_nBakUpId + 1;

                theRef.SetRcd(&stCurRef, i);

                if (stCurRef.nLocateMode == REF_LOCATE_DUAL_POINT)
                {
                    if (((thePgm.GetRcd()->nPrAlignMode == 1)
						&& (stRefRcd.nRefType == REF_TYPE_LEAD)) || (m_nAlignNum == 2))
                        theRef.CalcRefAngle(i, m_nBakUpId+2);
                }
            }
        }
    }

	theVisionSys.EnableVideo(true);

	CWnd*pWnd = GetDlgItem(IDC_BUTTON_UPDATE);
	
    if ((stRefRcd.nLocateMode == REF_LOCATE_SINGLE_POINT)
		|| ((thePgm.GetRcd()->nPrAlignMode == 1) && (stRefRcd.nRefType == REF_TYPE_LEAD))
		|| (m_nAlignNum == 2))
    {
        pWnd->EnableWindow(FALSE);
        m_nAlignNum = 1;

		((CWireBondDlg*)theApp.m_pMainWnd)->SetDrawAreaEnable(NULL);

		theVisionDisplay.EnableDisplay(true);
		theBond.SetPrPatternPassScore();
		theVisionSys.SavePatternRcd();	//v3.0.T70 edit
    }
    else
    {
        m_nAlignNum++;

		if (stRefRcd.stBakUpPr[m_nBakUpId].bValid == false)
		{
			stMovePos = stRefRcd.stVisionAlignPos[1];
			theMatrix.TransformPos(stWireRcd.nRefNum[m_nBondPoint], &stMovePos);

			theVisionSys.ChangeLensMag(stRefRcd.nVisionAlignMag[1]);

			theVisionSys.ShowVisionPattern(
					(short)stRefRcd.ulVisionAlignPatternID[1]);

			theVisionSys.SetSearchSize(stRefRcd.nVisionSrchSizeX[1], stRefRcd.nVisionSrchSizeY[1]);
		}
		else
		{
			stMovePos = stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[1];
			theMatrix.TransformPos(stWireRcd.nRefNum[m_nBondPoint], &stMovePos);

			theVisionSys.ShowVisionPattern(
				(short)stRefRcd.stBakUpPr[m_nBakUpId].lPatID[1]);
			theVisionSys.SetSearchSize(
				stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeX[1],
				stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeY[1]);
		}
		theServo.MoveTable2(stMovePos);	//v3.1.T195 edit
    }

    UpdateData(FALSE);
	UpdateBkPrView();
}

void CBondEditPage::PrepareTchBkPr(short nMode)
{
	if ((theBond.GetBondMode() != BONDMODE_STOP)
		&& (theBond.GetBondMode() != BONDMODE_IDLE))
	{
		theBond.InformAutoBondStop();
		Sleep(300);
	}

	theBond.ResetMatrixPrStatus(1);
	theBond.PrepareBonding();

	m_nBondPoint = ((CComboBox*)GetDlgItem(IDC_COMBO_BOND_POINT))->GetCurSel();
	m_nAlignNum  = 1;

	const WIRE_RECORD *pstWire = theWire.GetRcd(theBond.m_nCurWire);
	REF_RECORD stRefRcd;
	theRef.GetRcd(&stRefRcd, pstWire->nRefNum[m_nBondPoint]);

	UpdateData(FALSE);

	theVisionDisplay.EnableDisplay(false);  
	((CWireBondDlg*)theApp.m_pMainWnd)->SetDrawAreaEnable(m_hWnd);

	TBL_POS stMovePos;
	if (nMode == NEW_BK_PR)
	{
        stMovePos = stRefRcd.stVisionAlignPos[0];
        theMatrix.TransformPos(pstWire->nRefNum[m_nBondPoint], &stMovePos);
		theVisionSys.UpdateLightingParameter(stRefRcd.stVisionLightPara[0].nCoaxRedLight,
											stRefRcd.stVisionLightPara[0].bSideRedLight, 
											stRefRcd.stVisionLightPara[0].nCoaxBlueLight,
											stRefRcd.stVisionLightPara[0].bSideBlueLight);
		theVisionSys.ChangeLensMag(stRefRcd.nVisionAlignMag[0]);
		theVisionSys.SetSearchSize(stRefRcd.nVisionSrchSizeX[0], stRefRcd.nVisionSrchSizeY[0]); //@1
        theVisionSys.ShowVisionPattern(
					(short)stRefRcd.ulVisionAlignPatternID[0]);
		theServo.MoveTable2(stMovePos);	//v3.1.T195 edit
	}
	else
	{
        stMovePos = stRefRcd.stBakUpPr[m_nBakUpId].stAlignPos[0];
        theMatrix.TransformPos(pstWire->nRefNum[m_nBondPoint], &stMovePos);
		theVisionSys.UpdateLightingParameter(
			stRefRcd.stBakUpPr[m_nBakUpId].stLight[0].nCoaxRedLight, 
			stRefRcd.stBakUpPr[m_nBakUpId].stLight[0].bSideRedLight, 
			stRefRcd.stBakUpPr[m_nBakUpId].stLight[0].nCoaxBlueLight,
			stRefRcd.stBakUpPr[m_nBakUpId].stLight[0].bSideBlueLight);
		theVisionSys.SetSearchSize(
			stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeX[0],
			stRefRcd.stBakUpPr[m_nBakUpId].nSrchSizeY[0]);
        theVisionSys.ShowVisionPattern(
			(short)stRefRcd.stBakUpPr[m_nBakUpId].lPatID[0]);
		theServo.MoveTable2(stMovePos);	//v3.1.T195 edit
	}
	
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
	pWnd->EnableWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_BUTTON_UPDATE);
	pWnd->EnableWindow();
}

BOOL CBondEditPage::OnSetActive()
{
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_UPDATE);
    pWnd->EnableWindow(FALSE);
    UpdateData(FALSE);

    return CPropertyPage::OnSetActive();
}

void CBondEditPage::OnBkPr1()
{
	m_nBakUpId = 0;
	PrepareTchBkPr(EDIT_BK_PR);
}

void CBondEditPage::OnBkPr2()
{
	m_nBakUpId = 1;
	PrepareTchBkPr(EDIT_BK_PR);
}

void CBondEditPage::OnBkPr3()
{
	m_nBakUpId = 2;
	PrepareTchBkPr(EDIT_BK_PR);
}

void CBondEditPage::OnBkPr4()
{
	m_nBakUpId = 3;
	PrepareTchBkPr(EDIT_BK_PR);
}

void CBondEditPage::OnDelete()
{
	CDlgDelBkPR dlg(this);
	dlg.DoModal();

	if(dlg.IsDirty())UpdateBkPrView();
}

void CBondEditPage::DeleteAllBackupPr(void)
{
	short i, j;
	REF_RECORD stRefRcd;

	short index[10];
	index[0] = MSG_SURE_TO_DELETE_BACKUP_PR;
	if (theMsg.ShowMsg(HELP_MSG,1,index,true) == SHOW_MSG_RESULT_OK)  
	{
		for (i = 0; i < theRef.GetTotalRefNum(); i++)
		{
			theRef.GetRcd(&stRefRcd,i);
			stRefRcd.nBakUpIndex = 0;
			for (j = 0; j < MAX_BAK_UP_PR_NUM; j++)
			{
				if (stRefRcd.stBakUpPr[j].bValid == true)
				{
					stRefRcd.stBakUpPr[j].bValid = false;
					theVisionSys.ClearPatternRcd(
						(short)stRefRcd.stBakUpPr[j].lPatID[0]);
					if (stRefRcd.nLocateMode == REF_LOCATE_DUAL_POINT)
						theVisionSys.ClearPatternRcd(
							(short)stRefRcd.stBakUpPr[j].lPatID[1]);
				}
			}
			theRef.SetRcd(&stRefRcd, i);
		}

		m_nBakUpId = 0;

		CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
		pWnd->EnableWindow(true);

		pWnd = GetDlgItem(IDC_BUTTON_UPDATE);
		pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_BUTTON_DELETE_ALL);
		pWnd->EnableWindow(FALSE);

		for (i = IDC_BUTTON_BK_PR_1; i < IDC_BUTTON_BK_PR_1+MAX_BAK_UP_PR_NUM; i++)
		{
			pWnd = GetDlgItem(i);
			pWnd->ShowWindow(true);
			pWnd->EnableWindow(false);
		}
	}
}

void CBondEditPage::OnReplaceAll()
{
	short index[10];
	index[0] = MSG_SURE_TO_SWAP_BACKUP_PR;
	if (theMsg.ShowMsg(HELP_MSG,1,index,true) == SHOW_MSG_RESULT_OK)
	{
		const WIRE_RECORD *pstWire = theWire.GetRcd(theBond.m_nCurWire);
		theBond.SwapBkUpPrToMain(pstWire->nRefNum[m_nBondPoint], m_nBakUpId);
	}
}

void CBondEditPage::OnAutoSwapPr()
{
	UpdateData(TRUE);
	thePgm.GetRcd()->bBkPrAutoSwap = (m_iAutoSwapMainPr != 0);
}

void CBondEditPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	if (bShow) UpdateBkPrView();
}
