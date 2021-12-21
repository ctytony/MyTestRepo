/****************************************************************************
PositionAdjustDlg.cpp : implementation file
****************************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "CorrectBondDlg.h"
#include "VisionSystem.h"
#include "LightingCtrlDlg.h"
#include "Bond.h"
#include "ScanKeyThread.h"
#include "servo.h"
#include <memory>
#include "WireBondDlg.h"
#include "KeyPadHandler.h"

extern CJpeg theJpegHelp;

// CCorrectBondDlg dialog
IMPLEMENT_DYNAMIC(CCorrectBondDlg, CDialog)

CCorrectBondDlg::CCorrectBondDlg(short nWireNum, CWnd* pParent /*=NULL*/)
	: CDialog(CCorrectBondDlg::IDD, pParent)
	, m_bSelectCancel(false)
	, m_nCircleRadius(8)
	, m_nAdjustX(0)
	, m_nAdjustY(0)
	, m_nAdjustMax(0)
	, m_nAdjStep(1)
	, m_bShowParaOffset(0)
{
	m_pLightCtrlDlg = NULL;
	m_pbmArrow = m_pbmFaster = m_pbmSlower = NULL;
	m_nWireNum = nWireNum;
}

CCorrectBondDlg::~CCorrectBondDlg()
{
	if (m_pbmArrow) delete m_pbmArrow;
	if (m_pbmFaster) delete m_pbmFaster;
	if (m_pbmSlower) delete m_pbmSlower;
}

void CCorrectBondDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_STATIC_ADJ_STEP, m_sAdjStep);
    DDX_Text(pDX, IDC_STATIC_ADJ_X, m_sAdjustX);
    DDX_Text(pDX, IDC_STATIC_ADJ_Y, m_sAdjustY);
	DDX_Text(pDX, IDC_EDIT_WIRE_NUM, m_nWireNum);
	DDX_Text(pDX, IDC_EDIT_STANDBY_POWER1, m_nStandbyPower1);
	DDX_Text(pDX, IDC_EDIT_STANDBY_POWER2, m_nStandbyPower2);
	DDX_Text(pDX, IDC_EDIT_POWER1, m_nPower1);
	DDX_Text(pDX, IDC_EDIT_POWER2, m_nPower2);
	DDX_Text(pDX, IDC_EDIT_FORCE1, m_nForce1);
	DDX_Text(pDX, IDC_EDIT_FORCE2, m_nForce2);
	DDX_Text(pDX, IDC_EDIT_TIME1, m_nTime1);
	DDX_Text(pDX, IDC_EDIT_TIME2, m_nTime2);
    DDX_Check(pDX, IDC_CHECK_SHOW_PARA_OFFSET, m_bShowParaOffset);
}

BEGIN_MESSAGE_MAP(CCorrectBondDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_UP, OnUp)
    ON_BN_CLICKED(IDC_BUTTON_LEFT, OnLeft)
    ON_BN_CLICKED(IDC_BUTTON_DOWN, OnDown)
    ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnRight)
    ON_BN_CLICKED(IDC_BUTTON_Z_UP, OnZUp)
    ON_BN_CLICKED(IDC_BUTTON_Z_DOWN, OnZDown)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_AJUST_LIGHT, OnAdjustLight)
    ON_BN_CLICKED(IDC_BUTTON_FASTER, OnFaster)
    ON_BN_CLICKED(IDC_BUTTON_SLOWER, OnSlower)
    ON_EN_SETFOCUS(IDC_EDIT_STANDBY_POWER1, OnFocusStandbyPower1)
    ON_EN_SETFOCUS(IDC_EDIT_STANDBY_POWER2, OnFocusStandbyPower2)
    ON_EN_SETFOCUS(IDC_EDIT_POWER1, OnFocusPower1)
    ON_EN_SETFOCUS(IDC_EDIT_POWER2, OnFocusPower2)
    ON_EN_SETFOCUS(IDC_EDIT_FORCE1, OnFocusForce1)
    ON_EN_SETFOCUS(IDC_EDIT_FORCE2, OnFocusForce2)
    ON_EN_SETFOCUS(IDC_EDIT_TIME1, OnFocusTime1)
    ON_EN_SETFOCUS(IDC_EDIT_TIME2, OnFocusTime2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PARA_OFFSET, OnShowParaOffset)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CCorrectBondDlg message handlers

BOOL CCorrectBondDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWireBondDlg* lpMain = (CWireBondDlg*)theApp.m_pMainWnd;
	m_pbmArrow = lpMain->Load(theApp.m_hInstance,IDR_PNG_ARROW,_T("PNG"));

    SetWindowPos(NULL, 780, 250, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

    SetDlgItemTextW(IDC_BUTTON_AJUST_LIGHT, theMsg.GetMsg(MSG_LIGHT));
	SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));
	SetDlgItemTextW(IDC_BUTTON_CANCEL, theMsg.GetMsg(MSG_CANCEL));
	SetDlgItemTextW(IDC_STATIC_CIRCLE, theMsg.GetMsg(MSG_CIRCLE_SIZE));
	SetDlgItemTextW(IDC_STATIC_BOND_PARA, theMsg.GetMsg(MSG_BOND_PARAMETER));
	SetDlgItemTextW(IDC_STATIC_WIRE_NUM, theMsg.GetMsg(MSG_WIRE_NUM));
	SetDlgItemTextW(IDC_STATIC_STANDBY_POWER, theMsg.GetMsg(MSG_STANDBY_POWER));
	SetDlgItemTextW(IDC_STATIC_POWER, theMsg.GetMsg(MSG_BASE_POWER));
	SetDlgItemTextW(IDC_STATIC_FORCE, theMsg.GetMsg(MSG_BASE_FORCE));
	SetDlgItemTextW(IDC_STATIC_TIME, theMsg.GetMsg(MSG_BASE_TIME));

	UpdateBondPara();

	theServo.JogSpeedSlowest();

	//for enforce the keyboard to use Servo at this moment
    theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	GetDlgItem(IDC_BUTTON_LEFT)->SubclassDlgItem(IDC_BUTTON_LEFT, this);
	UpdateDisplay();

    return CDialog::OnInitDialog();
}

HBRUSH CCorrectBondDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)	//V1.1.W140 edit
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkMode(TRANSPARENT);
        return theApp.m_brNull;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCorrectBondDlg::OnUp()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(m_nAdjustMax - m_nAdjustY);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustY += nAdjStep;
	}
	UpdateDisplay();

	theServo.MoveTable2(TBL_POS(0.0, nAdjStep), false);	//v3.1.T196 edit
}

void CCorrectBondDlg::OnLeft()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(m_nAdjustMax - m_nAdjustX);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustX += nAdjStep;
	}
	UpdateDisplay();

	theServo.MoveTable2(TBL_POS(nAdjStep, 0.0), false);	//v3.1.T196 edit
}

void CCorrectBondDlg::OnDown()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(-m_nAdjustMax - m_nAdjustY);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustY -= nAdjStep;
	}
	UpdateDisplay();

	theServo.MoveTable2(TBL_POS(0.0, -nAdjStep), false);	//v3.1.T196 edit
}

void CCorrectBondDlg::OnRight()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(-m_nAdjustMax - m_nAdjustX);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustX -= nAdjStep;
	}
	UpdateDisplay();

	theServo.MoveTable2(TBL_POS(-nAdjStep, 0.0), false);	//v3.1.T196 edit
}

void CCorrectBondDlg::OnZUp()
{
	theVisionSys.DrawCircleforBTO(++m_nCircleRadius);
}

void CCorrectBondDlg::OnZDown()
{
	theVisionSys.DrawCircleforBTO(m_nCircleRadius>1 ? --m_nCircleRadius : m_nCircleRadius);
}

void CCorrectBondDlg::Exit()		//V1.1.W141 add
{
	if (m_nAdjustMax > 0)
	{
		theKeyPad.SetKeyProhibit(KEY_UP, false);
		theKeyPad.SetKeyProhibit(KEY_DOWN, false);
		theKeyPad.SetKeyProhibit(KEY_LEFT, false);
		theKeyPad.SetKeyProhibit(KEY_RIGHT, false);
	}

	OnOK();

	if (m_pLightCtrlDlg == NULL) return;
	m_pLightCtrlDlg->DestroyWindow();
	delete m_pLightCtrlDlg;
	m_pLightCtrlDlg = NULL;
}

void CCorrectBondDlg::OnOk()
{
	m_bSelectCancel = false;
    theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);
	Exit();
}

void CCorrectBondDlg::OnCancel()
{
	m_bSelectCancel = true;
	Exit();
}

void CCorrectBondDlg::OnAdjustLight()
{
	if (m_pLightCtrlDlg == NULL)
	{
		m_pLightCtrlDlg = new CLightingCtrlDlg(TRUE, this);
		m_pLightCtrlDlg->Create(CLightingCtrlDlg::IDD);
	}
	else
	{
		m_pLightCtrlDlg->UpdateDisp();
		m_pLightCtrlDlg->ShowWindow(SW_SHOW);
	}
}

void CCorrectBondDlg::OnFaster()
{
	m_nAdjStep *= 2;
	if (m_nAdjStep > 16) m_nAdjStep = 16;
	UpdateDisplay();

	theServo.JogSpeedUp();
}

void CCorrectBondDlg::OnSlower()
{
	m_nAdjStep /= 2;
	if (m_nAdjStep < 1) m_nAdjStep = 1;
	UpdateDisplay();

	theServo.JogSpeedDown();
}

void CCorrectBondDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)	//V1.1.W140 add
{
	int nLeft = 0, nTop = 0;
	int nWidth = lpDIS->rcItem.right - lpDIS->rcItem.left;
	int nHeight = lpDIS->rcItem.bottom - lpDIS->rcItem.top;

	if (lpDIS->itemState&ODS_SELECTED)
	{
		nLeft = nTop = 1;
		nWidth = nWidth - 2;
		nHeight = nHeight - 2;

		DrawFrameControl(lpDIS->hDC, &lpDIS->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
	}

	if (nIDCtl == IDC_BUTTON_LEFT)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcLeft.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_RIGHT)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcRight.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_UP)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcUp.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_DOWN)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcDown.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_Z_UP)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcZUp.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_Z_DOWN)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcZDown.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_FASTER)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcFaster.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);
	else if (nIDCtl == IDC_BUTTON_SLOWER)
		StretchBlt(lpDIS->hDC, nLeft, nTop, nWidth, nHeight, m_dcSlower.m_hDC, nLeft, nTop, nWidth, nHeight, SRCCOPY);

	CDialog::OnDrawItem(nIDCtl, lpDIS);
}

BOOL CCorrectBondDlg::OnEraseBkgnd(CDC* pDC)	//V1.1.W140 add
{
	CRect rc;
	GetClientRect(&rc);

	theJpegHelp.Paint(pDC->m_hDC, rc);

	CWireBondDlg* lpMain = (CWireBondDlg*)theApp.m_pMainWnd;

	if (m_dcLeft.m_hDC == NULL&&m_pbmArrow)
	{
		m_pbmArrow->RotateFlip(Rotate180FlipX);
		lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_LEFT), pDC, m_dcLeft, m_pbmArrow);
	}

	if (m_dcRight.m_hDC == NULL&&m_pbmArrow)
	{
		m_pbmArrow->RotateFlip(Rotate180FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_RIGHT), pDC, m_dcRight, m_pbmArrow);
		m_pbmArrow->RotateFlip(Rotate180FlipNone);
	}

	if (m_dcUp.m_hDC == NULL&&m_pbmArrow)
	{
		m_pbmArrow->RotateFlip(Rotate90FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_UP), pDC, m_dcUp, m_pbmArrow);
		m_pbmArrow->RotateFlip(Rotate270FlipNone);
	}

	if (m_dcDown.m_hDC == NULL&&m_pbmArrow)
	{
		m_pbmArrow->RotateFlip(Rotate270FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_DOWN), pDC, m_dcDown, m_pbmArrow);
		m_pbmArrow->RotateFlip(Rotate90FlipNone);
	}

	if (m_dcZUp.m_hDC == NULL&&m_pbmArrow)
	{
		m_pbmArrow->RotateFlip(Rotate90FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_Z_UP), pDC, m_dcZUp, m_pbmArrow);
		m_pbmArrow->RotateFlip(Rotate270FlipNone);
	}

	if ((m_dcZDown.m_hDC == NULL) && m_pbmArrow)
	{
		m_pbmArrow->RotateFlip(Rotate270FlipNone);
		lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_Z_DOWN), pDC, m_dcZDown, m_pbmArrow);
		m_pbmArrow->RotateFlip(Rotate90FlipNone);
	}

	if (m_dcFaster.m_hDC == NULL)
	{
		m_pbmFaster = new Bitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_UP));
		if (m_pbmFaster)
			lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_FASTER), pDC, m_dcFaster, m_pbmFaster);
	}

	if (m_dcSlower.m_hDC == NULL)
	{
		m_pbmSlower = new Bitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_DOWN));
		if (m_pbmFaster)
			lpMain->CreateBttnDC(GetDlgItem(IDC_BUTTON_SLOWER), pDC, m_dcSlower, m_pbmSlower);
	}

	return TRUE;
}

LRESULT CCorrectBondDlg::OnNcHitTest(CPoint pt)	//V1.1.W140 add
{
	return HTCAPTION;
}

void CCorrectBondDlg::SetAdjustMax(short nMax)
{
	m_nAdjustMax = nMax;
	m_nAdjustX = 0;
	m_nAdjustY = 0;
	m_nAdjStep = 1;
	UpdateDisplay();

	if (m_nAdjustMax > 0)
	{
		//disable these keys
		theKeyPad.SetKeyProhibit(KEY_UP, true);
		theKeyPad.SetKeyProhibit(KEY_DOWN, true);
		theKeyPad.SetKeyProhibit(KEY_LEFT, true);
		theKeyPad.SetKeyProhibit(KEY_RIGHT, true);
	}
}

void CCorrectBondDlg::UpdateDisplay()
{
	if (m_nAdjustMax > 0)
	{
		m_sAdjustX.Format(_T("%.1f um"), -m_nAdjustX*0.5);
		m_sAdjustY.Format(_T("%.1f um"), m_nAdjustY*0.5);
	}
	m_sAdjStep.Format(_T("%.1f um"), m_nAdjStep*0.5);
	UpdateData(FALSE);
	Invalidate();
}

void CCorrectBondDlg::OnFocusForce1()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nForce1);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->nCrtBondForceOffset1 = m_nForce1;
	else
		theBondPara.GetRcd()->nCrtBondForceOffset1 = m_nForce1
					- theBondPara.GetRcd()->nBaseForce[0]
					- theWire.GetRcd(m_nWireNum)->nForceOffset[0];
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnFocusForce2()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nForce2);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->nCrtBondForceOffset2 = m_nForce2;
	else
		theBondPara.GetRcd()->nCrtBondForceOffset2 = m_nForce2
					- theBondPara.GetRcd()->nBaseForce[1]
					- theWire.GetRcd(m_nWireNum)->nForceOffset[1];
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnFocusTime1()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTime1);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->nCrtBondTimeOffset1 = m_nTime1;
	else
		theBondPara.GetRcd()->nCrtBondTimeOffset1 = m_nTime1
					- theBondPara.GetRcd()->nBaseTime[0]
					- theWire.GetRcd(m_nWireNum)->nTimeOffset[0];
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnFocusTime2()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTime1);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->nCrtBondTimeOffset2 = m_nTime2;
	else
		theBondPara.GetRcd()->nCrtBondTimeOffset2 = m_nTime2
					- theBondPara.GetRcd()->nBaseTime[1]
					- theWire.GetRcd(m_nWireNum)->nTimeOffset[1];
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnFocusPower1()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPower1);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->fCrtBondPowerOffset1 = (float)(m_nPower1 / 2.55);
	else
		theBondPara.GetRcd()->fCrtBondPowerOffset1 = (float)(m_nPower1 / 2.55)
					- theBondPara.GetRcd()->fBasePower[0]
					- theWire.GetRcd(m_nWireNum)->fPowerOffset[0];
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnFocusPower2()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nPower2);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->fCrtBondPowerOffset2 = (float)(m_nPower2 / 2.55);
	else
		theBondPara.GetRcd()->fCrtBondPowerOffset2 = (float)(m_nPower2 / 2.55)
					- theBondPara.GetRcd()->fBasePower[1]
					- theWire.GetRcd(m_nWireNum)->fPowerOffset[1];
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnFocusStandbyPower1()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nStandbyPower1);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->fCrtStandbyPowerOffset[0] = (float)(m_nStandbyPower1 / 2.55);
	else
		theBondPara.GetRcd()->fCrtStandbyPowerOffset[0] = (float)(m_nStandbyPower1 / 2.55)
					- theBondPara.GetRcd()->fStandbyPower[0]
					- theWire.GetRcd(m_nWireNum)->fPrePowerOffset[0];
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnFocusStandbyPower2()
{
	GetDlgItem(IDC_STATIC_WIRE_NUM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nStandbyPower2);
	if (m_bShowParaOffset)
		theBondPara.GetRcd()->fCrtStandbyPowerOffset[1] = (float)(m_nStandbyPower2 / 2.55);
	else
		theBondPara.GetRcd()->fCrtStandbyPowerOffset[1] = (float)(m_nStandbyPower2 / 2.55)
					- theBondPara.GetRcd()->fStandbyPower[1]
					- theWire.GetRcd(m_nWireNum)->fPrePowerOffset[1];
	UpdateData(FALSE);
}

void CCorrectBondDlg::UpdateBondPara()
{
	WIRE_RECORD *pstWire = theWire.GetRcd(m_nWireNum);
	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();
	if (m_bShowParaOffset)
	{
		m_nStandbyPower1 = (short)(pstBondPara->fCrtStandbyPowerOffset[0] * 2.55);
		m_nStandbyPower2 = (short)(pstBondPara->fCrtStandbyPowerOffset[1] * 2.55);
		m_nPower1 = (short)(pstBondPara->fCrtBondPowerOffset1 * 2.55);
		m_nPower2 = (short)(pstBondPara->fCrtBondPowerOffset2 * 2.55);
		m_nForce1 = pstBondPara->nCrtBondForceOffset1;
		m_nForce2 = pstBondPara->nCrtBondForceOffset2;
		m_nTime1 = pstBondPara->nCrtBondTimeOffset1;
		m_nTime2 = pstBondPara->nCrtBondTimeOffset2;
	}
	else
	{
		m_nStandbyPower1 = (short)((pstBondPara->fStandbyPower[0]
							+ pstBondPara->fCrtStandbyPowerOffset[0]
							+ pstWire->fPrePowerOffset[0]) * 2.55);
		m_nStandbyPower2 = (short)((pstBondPara->fStandbyPower[1]
							+ pstBondPara->fCrtStandbyPowerOffset[1]
							+ pstWire->fPrePowerOffset[1]) * 2.55);
		m_nPower1 = (short)((pstBondPara->fBasePower[0]
							+ pstBondPara->fCrtBondPowerOffset1
							+ pstWire->fPowerOffset[0]) * 2.55);
		m_nPower2 = (short)((pstBondPara->fBasePower[1]
							+ pstBondPara->fCrtBondPowerOffset2
							+ pstWire->fPowerOffset[1]) * 2.55);
		m_nForce1 = pstBondPara->nBaseForce[0]
					+ pstBondPara->nCrtBondForceOffset1
					+ pstWire->nForceOffset[0];
		m_nForce2 = pstBondPara->nBaseForce[1]
					+ pstBondPara->nCrtBondForceOffset2
					+ pstWire->nForceOffset[1];
		m_nTime1 = pstBondPara->nBaseTime[0]
					+ pstBondPara->nCrtBondTimeOffset1
					+ pstWire->nTimeOffset[0];
		m_nTime2 = pstBondPara->nBaseTime[1]
					+ pstBondPara->nCrtBondTimeOffset2
					+ pstWire->nTimeOffset[1];
	}
	UpdateData(FALSE);
}

void CCorrectBondDlg::OnShowParaOffset()
{
	UpdateData(TRUE);
	UpdateBondPara();
}