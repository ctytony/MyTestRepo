/***************************************************************
DebugDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "Reference.h"
#include "Matrix.h"
#include "Wire.h"
#include "LoopPara.h"
#include "MenuConfig.h"
#include "VisionCalibDlg.h"
#include "VisionView.h"
#include "VisionSystem.h"
#include "WireBondDlg.h"
#include "DebugDlg.h"
#include "Bond.h"
#include "WaveForm.h"
#include "MaterialHandlerThread.h"
#include "ContactLevel.h"
#include "MachineConfig.h"
#include "Debug.h"
#include "User.h"
#include "servo.h"
#include "EfoProfile.h"

// CDebugDlg dialog

IMPLEMENT_DYNAMIC(CDebugDlg, CDialog)

CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
    , m_nItem(4)
    , m_nItemIdx(1)
    , m_sInfo(_T(""))
    , m_nMode(DEBUG_NONE)
	, m_bCorrectBond(0)
{
}

CDebugDlg::~CDebugDlg()
{
}

void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_ITEM, m_nItem);
    DDX_Text(pDX, IDC_EDIT_NUM, m_nItemIdx);
    DDX_Text(pDX, IDC_DEBUG_INFO, m_sInfo);
    DDX_CBIndex(pDX, IDC_COMBO_DEBUG_MODE, m_nMode);
    DDX_Check(pDX, IDC_CHECK_CORRECT_BOND, m_bCorrectBond);
}

BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, OnApply)
    ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO_DEBUG_MODE, OnDebugMode)
    ON_EN_SETFOCUS(IDC_EDIT_NUM, OnEditNum)
    ON_BN_CLICKED(IDC_BUTTON_WAVEFORM, OnWaveform)
    ON_BN_CLICKED(IDC_BUTTON_INFO, OnInfo)
    ON_CBN_SELCHANGE(IDC_COMBO_ITEM, OnItem)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_CHECK_CORRECT_BOND, OnCorrectBond)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CDebugDlg message handlers
BOOL CDebugDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    m_nMode = theDebug.GetDebugMode();

	CRect Rect;
	(GetParent())->GetWindowRect(&Rect);
    SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_BUTTON_APPLY,theMsg.GetMsg(MSG_APPLY));
	SetDlgItemTextW(IDC_BUTTON_CLEAR,theMsg.GetMsg(MSG_CLEAR));

	GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(theUser.GetUserLevel() > USER_SERVICE);
    GetDlgItem(IDC_BUTTON_WAVEFORM)->EnableWindow(theUser.GetUserLevel() > USER_PROCESS);

	PostMessage(WM_COMMAND,MAKELONG(IDC_BUTTON_INFO,BN_CLICKED),(LPARAM)GetDlgItem(IDC_BUTTON_INFO)->m_hWnd);

    return CDialog::OnInitDialog();
}

LRESULT CDebugDlg::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W148 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}

void CDebugDlg::OnDestroy()
{
	((CWireBondDlg*)GetParent())->UpdateInfoView(INFOVIEW_ALL);
    CDialog::OnDestroy();
}

void CDebugDlg::OnApply()
{
    UpdateData(TRUE);

    switch (m_nItem)
    {
    case 0:	//Program
	{
		const PGM_RECORD *pstPgm = thePgm.GetRcd();
	    
		m_sInfo.Empty();            
		m_sInfo.AppendFormat(_T("Pgm/Recipe Version: %d/%d, Valid: %d\r\n"),
				pstPgm->nVersion, pstPgm->nRecipeVersion, pstPgm->bPgmValid);
		m_sInfo.AppendFormat(_T("Ref/Wire: [%d, %d]\r\n"),
				pstPgm->nTotalRefNum, pstPgm->nTotalWireNum);
		m_sInfo.AppendFormat(_T("Col/Row: [%d, %d]\r\n"),
				pstPgm->nDeviceColNum, pstPgm->nDeviceRowNum);
		m_sInfo.AppendFormat(_T("Matrix Col/Row: [%d, %d]\r\n"),
				pstPgm->nMatrixColumn, pstPgm->nMatrixRow);	//v3.1.T530 add
		m_sInfo.AppendFormat(_T("Unit Row Offset: [%.2f, %.2f]\r\n"),
			pstPgm->stUnitRowOffset.dx, pstPgm->stUnitRowOffset.dy);
		m_sInfo.AppendFormat(_T("Unit Col Offset: [%.2f, %.2f]\r\n"),
			pstPgm->stUnitColOffset.dx, pstPgm->stUnitColOffset.dy);
		m_sInfo.AppendFormat(_T("Mat Row Offset: [%.2f, %.2f]\r\n"),
			pstPgm->stMatrixRowOffset.dx, pstPgm->stMatrixRowOffset.dy);
		m_sInfo.AppendFormat(_T("Mat Col Offset: [%.2f, %.2f]\r\n"),
			pstPgm->stMatrixColOffset.dx, pstPgm->stMatrixColOffset.dy);
		m_sInfo.AppendFormat(_T("Device Seq: %d"), pstPgm->nDeviceSeq);
		switch (pstPgm->nDeviceSeq)
		{
		case PGM_SINGLE:	m_sInfo.AppendFormat(_T(" - Single\r\n")); break;
		case PGM_MATRIX:	m_sInfo.AppendFormat(_T(" - Matrix\r\n")); break;
		case PGM_HYBRID:	m_sInfo.AppendFormat(_T(" - Hybrid\r\n")); break;
		case PGM_DIE_AHEAD:	m_sInfo.AppendFormat(_T(" - Die Ahead\r\n")); break;
		case PGM_DIE_LEAD_AHEAD:
			m_sInfo.AppendFormat(_T(" - Die Lead Ahead\r\n")); break;
		case PGM_MAT_DIE_AHEAD:
			m_sInfo.AppendFormat(_T(" - Matrix Die Ahead\r\n")); break;
		case PGM_MAT_HYBRID:
			m_sInfo.AppendFormat(_T(" - Matrix Hybrid\r\n")); break;
		default:
			m_sInfo.AppendFormat(_T(" - Unknown\r\n"));
		}
		m_sInfo.AppendFormat(_T("Bond Seq: %d"), pstPgm->nBondSeqOrientation);
		if (pstPgm->nBondSeqOrientation == SEQUENCE_VERTICAL)
			m_sInfo.AppendFormat(_T(" - Vertical\r\n"));
		else
			m_sInfo.AppendFormat(_T(" - Horizontal\r\n"));
		m_sInfo.AppendFormat(_T("Track Type: %d\r\n"), pstPgm->nTrackType);
		m_sInfo.AppendFormat(_T("Track Offset: [%.2f, %.2f]\r\n"),
					pstPgm->stTrackOffset.dx, pstPgm->stTrackOffset.dy);

		m_sInfo.AppendFormat(_T("PR Pass Score: %d\r\n"), pstPgm->nPrPassScore);
		m_sInfo.AppendFormat(_T("PR Ahead Col: %d\r\n"), pstPgm->nPrAheadCol);
		m_sInfo.AppendFormat(_T("PR Align Mode: %d\r\n"), pstPgm->nPrAlignMode);
		m_sInfo.AppendFormat(_T("Vision Move/Long Delay: [%d, %d]\r\n"),
					pstPgm->nVisionMoveDelay, pstPgm->nVisionLongMoveDelay);
		m_sInfo.AppendFormat(_T("Vision Pre Move: %d\r\n"), pstPgm->nVisionPreMove);

		//m_sInfo.AppendFormat(_T("Tail Length: %d\r\n"), pstPgm->nTailLengthUm);	//v3.1.T60 delete
		m_sInfo.AppendFormat(_T("Spark Delay: %d\r\n"), pstPgm->nSparkDelay);

		if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
			|| (theMcConfig.GetMhsType() == MHS_R2R)
			|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
			|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
		{
			INDEX_PR_REF stIndexPrRef;
			theDevice.GetIndexPrRef(&stIndexPrRef);
			m_sInfo.AppendFormat(_T("Index PR ID: [%d, %d]\r\n"),
								stIndexPrRef.ulVisionAlignPatternID,
								stIndexPrRef.ulVisionBkupPatternID);
		}

		UpdateData(FALSE);
		break;
	}
    case 1:	//Reference
	{
		const REF_RECORD *pstRef = theRef.GetRcd(m_nItemIdx);
	    
		m_sInfo.Empty();
		m_sInfo.AppendFormat(_T("Ref Num: %d, Type: %d"),
							pstRef->nRefNum, pstRef->nRefType);
		if (pstRef->nRefType == REF_TYPE_LEAD)
			m_sInfo.AppendFormat(_T(" - LEAD\r\n"));
		else
			m_sInfo.AppendFormat(_T(" - DIE\r\n"));
		m_sInfo.AppendFormat(_T("Ctact Valid: %d, Level: %d\r\n"),
							pstRef->bCtactLvlValid, pstRef->lCtactLvl);
		m_sInfo.AppendFormat(_T("1st Manual Pos: [%.2f, %.2f]\r\n"),
							pstRef->stManuAlignPos[0].dx,
							pstRef->stManuAlignPos[0].dy);
		m_sInfo.AppendFormat(_T("2nd Manual Pos: [%.2f, %.2f]\r\n"),
							pstRef->stManuAlignPos[1].dx,
							pstRef->stManuAlignPos[1].dy);
		m_sInfo.AppendFormat(_T("1st PR Pos: [%.2f, %.2f]\r\n"),
							pstRef->stVisionAlignPos[0].dx,
							pstRef->stVisionAlignPos[0].dy);
		m_sInfo.AppendFormat(_T("2nd PR Pos: [%.2f, %.2f]\r\n"),
							pstRef->stVisionAlignPos[1].dx,
							pstRef->stVisionAlignPos[1].dy);
		m_sInfo.AppendFormat(_T("Manual PatID: [%d, %d]\r\n"),
							pstRef->ulManuAlignPatternID[0],
							pstRef->ulManuAlignPatternID[1]);
		m_sInfo.AppendFormat(_T("PR PatID: [%d, %d]\r\n"),
							pstRef->ulVisionAlignPatternID[0],
							pstRef->ulVisionAlignPatternID[1]);

		m_sInfo.AppendFormat(_T("Manual Align Mag: [%d, %d]\r\n"),
							pstRef->nManuAlignMag[0],
							pstRef->nManuAlignMag[1]);
		m_sInfo.AppendFormat(_T("PR Align Mag: [%d, %d]\r\n"),
							pstRef->nVisionAlignMag[0],
							pstRef->nVisionAlignMag[1]);
		m_sInfo.AppendFormat(_T("1st Manual Pat Size: [%d, %d]\r\n"),
							pstRef->nManuSrchSizeX[0],
							pstRef->nManuSrchSizeY[0]);
		m_sInfo.AppendFormat(_T("2nd Manual Pat Size: [%d, %d]\r\n"),
							pstRef->nManuSrchSizeX[1],
							pstRef->nManuSrchSizeY[1]);
		m_sInfo.AppendFormat(_T("1st PR Pat Size: [%d, %d]\r\n"),
							pstRef->nVisionSrchSizeX[0],
							pstRef->nVisionSrchSizeY[0]);
		m_sInfo.AppendFormat(_T("2nd PR Pat Size: [%d, %d]\r\n"),
							pstRef->nVisionSrchSizeX[1],
							pstRef->nVisionSrchSizeY[1]);

		m_sInfo.AppendFormat(_T("Manual Sin/Cos: [%.2f, %.2f]\r\n"),
							pstRef->dManuSinAln, pstRef->dManuCosAln);
		m_sInfo.AppendFormat(_T("PR Sin/Cos: [%.2f, %.2f]\r\n"),
							pstRef->dVisionSinAln, pstRef->dVisionCosAln);
		m_sInfo.AppendFormat(_T("Locate Mode: %d"), pstRef->nLocateMode);
		if (pstRef->nLocateMode == REF_LOCATE_SINGLE_POINT)
			m_sInfo.AppendFormat(_T(" - 1-Point PR\r\n"));
		else
			m_sInfo.AppendFormat(_T(" - 2-Point PR\r\n"));

		m_sInfo.AppendFormat(_T("Mutiple BTOS X/Y: [%.2f, %.2f]\r\n"),
							pstRef->stBtos.dx, pstRef->stBtos.dy);

		m_sInfo.AppendFormat(_T("Backup 1 PatID: [%d, %d]\r\n"),
					pstRef->stBakUpPr[0].lPatID[0],
					pstRef->stBakUpPr[0].lPatID[1]);
		m_sInfo.AppendFormat(_T("Backup 2 PatID: [%d, %d]\r\n"),
					pstRef->stBakUpPr[1].lPatID[0],
					pstRef->stBakUpPr[1].lPatID[1]);
		m_sInfo.AppendFormat(_T("Backup 3 PatID: [%d, %d]\r\n"),
					pstRef->stBakUpPr[2].lPatID[0],
					pstRef->stBakUpPr[2].lPatID[1]);
		m_sInfo.AppendFormat(_T("Backup 4 PatID: [%d, %d]\r\n"),
					pstRef->stBakUpPr[3].lPatID[0],
					pstRef->stBakUpPr[3].lPatID[1]);

		UpdateData(FALSE);
        break;
	}
    case 2:	//Matrix
	{
		const MATRIX_RECORD *pstMat = theMatrix.GetRcd(m_nItemIdx);

		m_sInfo.Empty();
		m_sInfo.AppendFormat(_T("Matrix Num: %d, Status: %d"),
								pstMat->nMatrixNum, pstMat->nVisionStatus);
		switch (pstMat->nVisionStatus)
		{
		case VISION_NOT_DONE:
			m_sInfo.AppendFormat(_T(" - not done\r\n"));
			break;
		case VISION_SKIP_DIE:
			m_sInfo.AppendFormat(_T(" - skipped\r\n"));
			break;
		case VISION_FAILURE:
			m_sInfo.AppendFormat(_T(" - failed\r\n"));
			break;
		default:
			m_sInfo.AppendFormat(_T(" - success\r\n"));
			break;
		}
		m_sInfo.AppendFormat(_T("1st Align Pos: [%.2f, %.2f]\r\n"),
			pstMat->stVisionAlignPos[0].dx, pstMat->stVisionAlignPos[0].dy);
		m_sInfo.AppendFormat(_T("2nd Align Pos: [%.2f, %.2f]\r\n"),
			pstMat->stVisionAlignPos[1].dx, pstMat->stVisionAlignPos[1].dy);
		m_sInfo.AppendFormat(_T("Tch Center Pos: [%.2f, %.2f]\r\n"),
			pstMat->stOriginalCenterPos.dx, pstMat->stOriginalCenterPos.dy);
		m_sInfo.AppendFormat(_T("Cur Center Pos: [%.2f, %.2f]\r\n"),
			pstMat->stUpdatedCenterPos.dx, pstMat->stUpdatedCenterPos.dy);
		m_sInfo.AppendFormat(_T("Align Sin/Cos: [%.5f, %.5f]\r\n"),
			pstMat->dSinAln, pstMat->dCosAln);
		const REF_RECORD *pstRef = theRef.GetRcd(m_nItemIdx);
	    double dAlignDist = Calc2PtDist(
				pstRef->stManuAlignPos[0], pstRef->stManuAlignPos[1]);
		m_sInfo.AppendFormat(_T("Tch Align Dist: %.2f\r\n"), dAlignDist);
		dAlignDist = Calc2PtDist(pstMat->stVisionAlignPos[0],
								pstMat->stVisionAlignPos[1]);
		m_sInfo.AppendFormat(_T("Cur Align Dist: %.2f\r\n"), dAlignDist);

		UpdateData(FALSE);
        break;
	}
    case 3:	//Wire
	{
		const WIRE_RECORD *pstWire = theWire.GetRcd(m_nItemIdx);

		m_sInfo.Empty();
		m_sInfo.AppendFormat(_T("Wire Num: %d\r\n"), pstWire->nWireNum);
		m_sInfo.AppendFormat(_T("1st Bond Pos: [%.2f, %.2f]\r\n"), pstWire->stTchPos[0].dx, pstWire->stTchPos[0].dy);
		m_sInfo.AppendFormat(_T("2nd Bond Pos: [%.2f, %.2f]\r\n"), pstWire->stTchPos[1].dx, pstWire->stTchPos[1].dy);
		m_sInfo.AppendFormat(_T("Ref Num: [%d, %d]\r\n"), pstWire->nRefNum[0], pstWire->nRefNum[1]);
		m_sInfo.AppendFormat(_T("Loop Mode: %d\r\n"), pstWire->nLoopMode);
		m_sInfo.AppendFormat(_T("Vision Ahead: %d\r\n"), pstWire->bVisionAhead);
		m_sInfo.AppendFormat(_T("Check Nsop: %d\r\n"), pstWire->bCheckNsop);
		m_sInfo.AppendFormat(_T("Power Offset: [%.2f, %.2f]\r\n"), pstWire->fPowerOffset[0], pstWire->fPowerOffset[1]);
		m_sInfo.AppendFormat(_T("Time Offset: [%d, %d]\r\n"), pstWire->nTimeOffset[0], pstWire->nTimeOffset[1]);
		m_sInfo.AppendFormat(_T("Force Offset: [%d, %d]\r\n"), pstWire->nForceOffset[0], pstWire->nForceOffset[1]);

		UpdateData(FALSE);
        break;
	}
    case 4:	//Bond Wire
	{
		ST_WIRE_INFO stWireInfo;
		m_sInfo.Empty();

		TBL_POS stPos(0.0, 0.0);
		if (!theMcConfig.IsBypassAll())
			theServo.GetTablePosition(&stPos);

		int nErr;
		if (m_bCorrectBond)
		{
			EN_BOND_MODE nOriMode = theBond.GetBondMode();
			theBond.SetBondMode(BONDMODE_CORRECT);
			nErr = theBond.CalcWireInfo(m_nItemIdx, stPos, &stWireInfo, m_sInfo, true);
			theBond.SetBondMode(nOriMode);
		}
		else
			nErr = theBond.CalcWireInfo(m_nItemIdx, stPos, &stWireInfo, m_sInfo, true);
		UpdateData(FALSE);

		if (nErr != FOK) break;

		CWnd *pWnd = (this->GetParent())->GetDlgItem(IDC_IMG);
		CDC *pDC = pWnd->GetDC();
		CRect rect;
		pWnd->GetClientRect(&rect);
		pWnd->Invalidate();
		pWnd->UpdateWindow();

		pDC->SelectStockObject(BLACK_BRUSH);
		pDC->Rectangle(&rect);

		TBL_POS stLastKinkPos = stWireInfo.stTwistRevKinkPos;
		double dSpanX = Calc2PtDist(stLastKinkPos,
						stWireInfo.stBondPos[1]) * theServo.GetResolXY();
		double dSpanY = (stWireInfo.nRevHt
					+ stWireInfo.nRevDistHt
					+ stWireInfo.nPreKinkHt
					+ stWireInfo.nPreKinkDistHt
					+ stWireInfo.nKink2Ht
					+ stWireInfo.nKink2DistHt
					+ stWireInfo.nKink3Ht
					+ stWireInfo.nKink3DistHt
					+ stWireInfo.nTwistHt
					+ stWireInfo.nTwistRevDistHt
					+ stWireInfo.nTwistFwdDistHt
					+ stWireInfo.nSlopeLen
					+ stWireInfo.nAboveLoopTopHt)
					* theServo.GetResolZ();

		if (stWireInfo.nCtactLvl1 > stWireInfo.nCtactLvl2)
			dSpanY += (stWireInfo.nCtactLvl1 - stWireInfo.nCtactLvl2) * theServo.GetResolZ();

		double dFactorX = (rect.right - 160) / dSpanX;
		double dFactorY = (rect.bottom - 40) / dSpanY;
		dFactorX = min(dFactorX, dFactorY);

		double dDistX = stWireInfo.stBondPos[1].dx - stWireInfo.stBondPos[0].dx;
		double dDistY = stWireInfo.stBondPos[1].dy - stWireInfo.stBondPos[0].dy;
		double fWireAngle = atan2(dDistY, dDistX);
		double dSinAngle = sin(fWireAngle);
		double dCosAngle = cos(fWireAngle);

		long lBaseLvl = min(stWireInfo.nCtactLvl1, stWireInfo.nCtactLvl2);
	    
		CPen pen1, pen2;
		pen1.CreatePen(PS_SOLID, 1, RGB(255,255,0));
		pen2.CreatePen(PS_DASH, 1, RGB(128,128,128));

		CPoint OrigPt = CPoint(80, rect.bottom - 20);
		pDC->SelectObject(&pen2);
		pDC->MoveTo(OrigPt);
		pDC->LineTo(rect.right - 80, rect.bottom - 20);
	    
		TBL_POS stCurPos = stWireInfo.stBondPos[0];
		TBL_POS stDist;
		stDist.dx = stCurPos.dx - stLastKinkPos.dx;
		stDist.dy = stCurPos.dy - stLastKinkPos.dy;

		CPoint ObjectPt;
		ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

		double dCurLvl = stWireInfo.nCtactLvl1;
		ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);

		if (stWireInfo.nCtactLvl1 > stWireInfo.nCtactLvl2)
		{
			pDC->MoveTo(ObjectPt.x - 20, rect.bottom - 20);
			pDC->LineTo(ObjectPt.x - 20, ObjectPt.y);
			pDC->LineTo(ObjectPt.x + 20, ObjectPt.y);
			pDC->LineTo(ObjectPt.x + 20, rect.bottom - 20);
		}

		pDC->SelectObject(&pen1);
		pDC->MoveTo(ObjectPt);

		dCurLvl += stWireInfo.nRevHt;
		ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
		pDC->LineTo(ObjectPt);

		stCurPos = stWireInfo.stRevPos;
		stDist.dx = stCurPos.dx - stLastKinkPos.dx;
		stDist.dy = stCurPos.dy - stLastKinkPos.dy;

		ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

		dCurLvl += stWireInfo.nRevDistHt;
		ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
		pDC->LineTo(ObjectPt);

		if (stWireInfo.bPreKinkEnable)
		{
			dCurLvl += stWireInfo.nPreKinkHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);

			stCurPos = stWireInfo.stPreKinkPos;
			stDist.dx = stCurPos.dx - stLastKinkPos.dx;
			stDist.dy = stCurPos.dy - stLastKinkPos.dy;

			ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

			dCurLvl += stWireInfo.nPreKinkDistHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);
		}

		if (stWireInfo.bKink2Enable)
		{
			dCurLvl += stWireInfo.nKink2Ht;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);

			stCurPos = stWireInfo.stKink2Pos;
			stDist.dx = stCurPos.dx - stLastKinkPos.dx;
			stDist.dy = stCurPos.dy - stLastKinkPos.dy;

			ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

			dCurLvl += stWireInfo.nKink2DistHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);
		}

		if (stWireInfo.bKink3Enable)
		{
			dCurLvl += stWireInfo.nKink3Ht;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);

			stCurPos = stWireInfo.stKink3Pos;
			stDist.dx = stCurPos.dx - stLastKinkPos.dx;
			stDist.dy = stCurPos.dy - stLastKinkPos.dy;

			ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

			dCurLvl += stWireInfo.nKink3DistHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);
		}

		if (stWireInfo.bTwistEnable)
		{
			dCurLvl += stWireInfo.nTwistHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);

			stCurPos = stWireInfo.stTwistRevKinkPos;
			stDist.dx = stCurPos.dx - stLastKinkPos.dx;
			stDist.dy = stCurPos.dy - stLastKinkPos.dy;

			ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

			dCurLvl += stWireInfo.nTwistRevDistHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);

			stCurPos = stWireInfo.stTwistFwdKinkPos;
			stDist.dx = stCurPos.dx - stLastKinkPos.dx;
			stDist.dy = stCurPos.dy - stLastKinkPos.dy;

			ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

			dCurLvl += stWireInfo.nTwistFwdDistHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);
		}

		if ((stWireInfo.cLoopTopMode == ABOVE_LOOP_TOP_DISABLE)
			|| (stWireInfo.cLoopTopMode == ABOVE_LOOP_TOP_NORMAL))
		{
			dCurLvl += stWireInfo.nSlopeLen;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);
		}

		if ((stWireInfo.cLoopTopMode == ABOVE_LOOP_TOP_NORMAL)
			|| (stWireInfo.cLoopTopMode == ABOVE_LOOP_TOP_FAST))
		{
			stCurPos = stWireInfo.stAboveLoopTopPos;
			stDist.dx = stCurPos.dx - stLastKinkPos.dx;
			stDist.dy = stCurPos.dy - stLastKinkPos.dy;

			ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

			dCurLvl += stWireInfo.nAboveLoopTopHt;
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);
		}

		for (short i = 0; i < stWireInfo.nTrajPoint; i++)
		{
			stCurPos.dx += stWireInfo.adTrajPos[i];
			stCurPos.dy += stWireInfo.adTrajPos[stWireInfo.nTrajPoint + i];

			stDist.dx = stCurPos.dx - stLastKinkPos.dx;
			stDist.dy = stCurPos.dy - stLastKinkPos.dy;

			ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

			dCurLvl += stWireInfo.adTrajPos[stWireInfo.nTrajPoint * 2 + i];
			ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
			pDC->LineTo(ObjectPt);
		}

		stCurPos = stWireInfo.stPullPos;
		stDist.dx = stCurPos.dx - stLastKinkPos.dx;
		stDist.dy = stCurPos.dy - stLastKinkPos.dy;

		ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);

		dCurLvl = stWireInfo.nCtactLvl2;
		ObjectPt.y = OrigPt.y - round((dCurLvl - lBaseLvl) * theServo.GetResolZ() * dFactorX);
		pDC->LineTo(ObjectPt);
	    
		stCurPos = stWireInfo.stBondShiftPos2;
		stDist.dx = stCurPos.dx - stLastKinkPos.dx;
		stDist.dy = stCurPos.dy - stLastKinkPos.dy;

		ObjectPt.x = OrigPt.x + round((stDist.dx * dCosAngle + stDist.dy * dSinAngle) * theServo.GetResolXY() * dFactorX);
		pDC->LineTo(ObjectPt);

		if (stWireInfo.nCtactLvl1 < stWireInfo.nCtactLvl2)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(ObjectPt.x - 20, rect.bottom - 20);
			pDC->LineTo(ObjectPt.x - 20, ObjectPt.y);
			pDC->LineTo(ObjectPt.x + 20, ObjectPt.y);
			pDC->LineTo(ObjectPt.x + 20, rect.bottom - 20);
		}
	    
		pWnd->ReleaseDC(pDC);
        break;
	}
    case 5:	//Bond Bump
	{
		TBL_POS stPos(0.0, 0.0);
		if (!theMcConfig.IsBypassAll())
			theServo.GetTablePosition(&stPos);

		m_sInfo.Empty();
		ST_BALL_INFO stBallInfo;
		theBond.CalcBallInfo(m_nItemIdx, stPos, &stBallInfo, m_sInfo, true);
		UpdateData(FALSE);
        break;
	}
    case 6:	//Pattern ID
	{
		ST_PATTERN_RECORD stPatRcd;
		m_sInfo.Empty();
		for (short i = 0; i < MAX_VISION_PATTERN; i++)
		{
			theVisionSys.GetPatternRcd(i, &stPatRcd);
			if (stPatRcd.bValid)
				m_sInfo.AppendFormat(
					_T("Rcd_%d_PatID_%d_IndexPR_%d\r\n"),
					i, stPatRcd.lPatID, stPatRcd.bIndexPR);
		}
		UpdateData(FALSE);
        break;
	}
    case 7:	//ctact stat
	{
		if (m_nItemIdx < CTACT_WIRE_BUFFER)
		{
			ST_CTACT_STAT *pstCtact1 = theCtact.GetRcd(m_nItemIdx, FIRST_BOND);
			ST_CTACT_STAT *pstCtact2 = theCtact.GetRcd(m_nItemIdx, SECOND_BOND);

			m_sInfo.Empty();
			m_sInfo.AppendFormat(_T("SampleSize=%d,%d\r\n"),
						pstCtact1->cSampleSize, pstCtact2->cSampleSize);
			m_sInfo.AppendFormat(_T("LastContact=%d,%d\r\n"),
						pstCtact1->nCtactLvl, pstCtact2->nCtactLvl);
			m_sInfo.AppendFormat(_T("Mean=%.2f,%.2f\r\n"),
						pstCtact1->fMean, pstCtact2->fMean);
			//v1.6.T308 delete
			//m_sInfo.AppendFormat(_T("StdDev=%f,%f\r\n"),
			//			st1stCtactRcd.fStdDev, st2ndCtactRcd.fStdDev);
			//v1.6.T308 add
			ST_CTACT_LOG *pstAddLog, *pstGetLog;
			for (char j=0; j<=1; j++)
			{
				pstAddLog = theCtact.GetAddLog(m_nItemIdx, j);
				pstGetLog = theCtact.GetGetLog(m_nItemIdx, j);
				m_sInfo.AppendFormat(_T("W%d/%d_Add%d_Get%d\r\n"),
					m_nItemIdx, j+1, pstAddLog->cIdx, pstGetLog->cIdx);
				for (short i=0; i<CTACT_LOG_NUM; i++)
					m_sInfo.AppendFormat(_T("%d_%d/%d_%d/%d\r\n"), i,
						pstAddLog->anCtactLvl[i],
						pstAddLog->abValid[i] ? 1 : 0,
						pstGetLog->anCtactLvl[i],
						pstGetLog->abValid[i] ? 1 : 0);
			}
			UpdateData(FALSE);
		}
        break;
	}
	case 8:	//check bond power
	{
		m_sInfo.Empty();
		for (short i = 1; i <= WIRE_NUM; i++)
			m_sInfo.AppendFormat(_T("W%d: %d..%d, %d..%d\r\n"),
				i, theMcConfig.GetPower1LowLimit(i), theMcConfig.GetPower1HighLimit(i),
				theMcConfig.GetPower2LowLimit(i), theMcConfig.GetPower2HighLimit(i));
		UpdateData(FALSE);
		break;
	}
	case 9:	//check bond force
	{
		m_sInfo.Empty();
		for (short i = 1; i <= WIRE_NUM; i++)
			m_sInfo.AppendFormat(_T("W%d: %d..%d, %d..%d\r\n"),
				i, theMcConfig.GetForce1LowLimit(i), theMcConfig.GetForce1HighLimit(i),
				theMcConfig.GetForce2LowLimit(i), theMcConfig.GetForce2HighLimit(i));
		UpdateData(FALSE);
		break;
	}
	case 10:	//check bond time
	{
		m_sInfo.Empty();
		for (short i = 1; i <= WIRE_NUM; i++)
			m_sInfo.AppendFormat(_T("W%d: %d..%d, %d..%d\r\n"),
				i, theMcConfig.GetTime1LowLimit(i), theMcConfig.GetTime1HighLimit(i),
				theMcConfig.GetTime2LowLimit(i), theMcConfig.GetTime2HighLimit(i));
		UpdateData(FALSE);
		break;
	}
	case 11:	//v3.1.T64 - efo profile
	{
		m_sInfo.Empty();
		short nTotalFab = 2;
		if (theMcConfig.GetFabType() == 1) nTotalFab = 4;

		const ST_EFO_PROFILE *pstEfoProf;
		for (short i = 0; i < nTotalFab; i++)
		{
			pstEfoProf = theEfoProfile.GetProfile(i);
			m_sInfo.AppendFormat(_T("Fab %d:\r\n"), i + 1);
			m_sInfo.AppendFormat(_T("TailLen: %dum\r\n"), pstEfoProf->nTailLen_x1um);
			if (theMcConfig.GetEfoType() == 0)
				m_sInfo.AppendFormat(_T("Current/Time: %d, %d\r\n"),
									pstEfoProf->nCurrent1, pstEfoProf->nTime1);
			else
			{
				m_sInfo.AppendFormat(_T("Current/Time1: %d, %d\r\n"),
									pstEfoProf->nCurrent1, pstEfoProf->nTime1);
				m_sInfo.AppendFormat(_T("Current/Time2: %d, %d\r\n"),
									pstEfoProf->nCurrent2, pstEfoProf->nTime2);
				m_sInfo.AppendFormat(_T("Current/Time3: %d. %d\r\n"),
									pstEfoProf->nCurrent3, pstEfoProf->nTime3);
			}
		}
		UpdateData(FALSE);
		break;
	}
    default:
        break;
    }
}

HBRUSH CDebugDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDebugDlg::OnDebugMode()
{
    long lBkupDebugMode = m_nMode;

    UpdateData(true);
    theDebug.SetDebugMode(m_nMode);

    if (m_nMode == DEBUG_SERVO_TIMING)
        theServo.StartServoTimerDebug(theDebug.GetDebugMode());

	//if ((lBkupDebugMode == DEBUG_WIRE_CLAMP) || (m_nMode == DEBUG_WIRE_CLAMP))
		theServo.UpdateWireClampProfile(theDebug.GetDebugMode(), theBond.IsDryRun());

	if ((m_nMode == DEBUG_LOG_TRACE)
		|| (m_nMode == DEBUG_WAVEFORM))
		theLog.SetActive(TRUE, TRUE);
	else
		theLog.SetActive(FALSE, FALSE);
}

void CDebugDlg::UpdateDebugInfo(CString &sDebugInfo)
{
    m_sInfo = sDebugInfo;
    UpdateData(false);

	if (sDebugInfo.GetLength() > 0)
		GetDlgItem(IDC_DEBUG_INFO)->PostMessage(WM_VSCROLL, SB_BOTTOM, NULL);
}

void CDebugDlg::OnEditNum()
{
    GetDlgItem(IDC_BUTTON_APPLY)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nItemIdx);
    UpdateData(false);
}

void CDebugDlg::OnWaveform()
{
    CWaveForm dlg;
    dlg.DoModal();
    UpdateDebugInfo(dlg.m_sDeformInfo);
}

void CDebugDlg::PostNcDestroy()
{
    CDialog::PostNcDestroy();
    delete this;
}

void CDebugDlg::OnInfo()
{
    if (m_nMode == DEBUG_WORK_HOLDER)
        theMHS.UpdateDebugInfo();

    UpdateDebugInfo(theDebug.GetDebugInfo());	//v3.0.T97 edit
}

void CDebugDlg::OnItem()
{
    UpdateData();
}

void CDebugDlg::OnClear()
{
    theDebug.ClearInfo();	//v3.0.T96 edit
    UpdateDebugInfo(theDebug.GetDebugInfo());	//v3.0.T97 edit
}

void CDebugDlg::OnCorrectBond()
{
	UpdateData(TRUE);
}
