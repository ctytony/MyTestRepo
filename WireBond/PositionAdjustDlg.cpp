/****************************************************************************
PositionAdjustDlg.cpp : implementation file
****************************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "PositionAdjustDlg.h"
#include "VisionSystem.h"
#include "LightingCtrlDlg.h"
#include "Bond.h"
#include "ScanKeyThread.h"
#include "servo.h"
#include <memory>
#include "WireBondDlg.h"
#include "KeyPadHandler.h"

extern CJpeg theJpegHelp;

// CPositionAdjustDlg dialog
IMPLEMENT_DYNAMIC(CPositionAdjustDlg, CDialog)

CPositionAdjustDlg::CPositionAdjustDlg(short nDlgType, CWnd* pParent /*=NULL*/)
	: CDialog(CPositionAdjustDlg::IDD, pParent)
	, m_bSelectCancel(false)
	, m_nCircleRadius(8)
	, m_b2ndBondEnable(FALSE)
	, m_nAdjustX(0)
	, m_nAdjustY(0)
	, m_nAdjustMax(0)
	, m_nAdjStep(1)
{
    m_nDlgType = nDlgType;
	m_pLightCtrlDlg = NULL;
	m_pbmArrow = m_pbmFaster = m_pbmSlower = NULL;
}

CPositionAdjustDlg::~CPositionAdjustDlg()
{
	if (m_pbmArrow) delete m_pbmArrow;
	if (m_pbmFaster) delete m_pbmFaster;
	if (m_pbmSlower) delete m_pbmSlower;
}

void CPositionAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_BTOS_2ND_ENABLE, m_b2ndBondEnable);
    DDX_Text(pDX, IDC_STATIC_ADJ_STEP, m_sAdjStep);
    DDX_Text(pDX, IDC_STATIC_ADJ_X, m_sAdjustX);
    DDX_Text(pDX, IDC_STATIC_ADJ_Y, m_sAdjustY);
}

BEGIN_MESSAGE_MAP(CPositionAdjustDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_UP, OnUp)
    ON_BN_CLICKED(IDC_BUTTON_LEFT, OnLeft)
    ON_BN_CLICKED(IDC_BUTTON_DOWN, OnDown)
    ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnRight)
    ON_BN_CLICKED(IDC_BUTTON_Z_UP, OnZUp)
    ON_BN_CLICKED(IDC_BUTTON_Z_DOWN, OnZDown)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_AJUST_LIGHT, OnAjustLight)
	ON_BN_CLICKED(IDC_CHECK_BTOS_2ND_ENABLE, OnBtos2ndEnable)
	ON_STN_CLICKED(IDC_STATIC_BTOS_2ND_ENABLE, OnBtos2ndEnable)
    ON_BN_CLICKED(IDC_BUTTON_FASTER, OnFaster)
    ON_BN_CLICKED(IDC_BUTTON_SLOWER, OnSlower)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CPositionAdjustDlg message handlers
BOOL CPositionAdjustDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CWireBondDlg* lpMain = (CWireBondDlg*)theApp.m_pMainWnd;
	m_pbmArrow = lpMain->Load(theApp.m_hInstance,IDR_PNG_ARROW,_T("PNG"));

    SetWindowPos(NULL, 780, 300, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

    SetDlgItemTextW(IDC_BUTTON_AJUST_LIGHT, theMsg.GetMsg(MSG_LIGHT));
	SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));
	SetDlgItemTextW(IDC_BUTTON_CANCEL, theMsg.GetMsg(MSG_CANCEL));
	SetDlgItemTextW(IDC_STATIC_CIRCLE, theMsg.GetMsg(MSG_CIRCLE_SIZE));
	SetDlgItemTextW(IDC_STATIC_BTOS_2ND_ENABLE,
					theMsg.GetMsg(MSG_ENABLE_LEAD_BTOS));

	if ((m_nDlgType == BTOS_DLG) && (thePgm.GetRcd()->nMultiBtos != BTOS_DIR_DISABLE))
	{
		GetDlgItem(IDC_CHECK_BTOS_2ND_ENABLE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_BTOS_2ND_ENABLE)->ShowWindow(TRUE);

		m_b2ndBondEnable = TRUE;
		theBond.Set2ndPointBtosEnable(true);
	}
	else
	{
		GetDlgItem(IDC_CHECK_BTOS_2ND_ENABLE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_BTOS_2ND_ENABLE)->ShowWindow(FALSE);
	}

    theServo.JogSpeedSlowest();

	//for enforce the keyboard to use Servo at this moment
    theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);

	GetDlgItem(IDC_BUTTON_LEFT)->SubclassDlgItem(IDC_BUTTON_LEFT, this);
	UpdateDisplay();

    return CDialog::OnInitDialog();
}

HBRUSH CPositionAdjustDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)	//V1.1.W140 edit
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkMode(TRANSPARENT);
        return theApp.m_brNull;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPositionAdjustDlg::OnUp()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(m_nAdjustMax - m_nAdjustY);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustY += nAdjStep;
	}
	UpdateDisplay();

    theServo.MoveTable2(TBL_POS(0.0, nAdjStep), false);		//v3.1.T202 edit
}

void CPositionAdjustDlg::OnLeft()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(m_nAdjustMax - m_nAdjustX);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustX += nAdjStep;
	}
	UpdateDisplay();

    theServo.MoveTable2(TBL_POS(nAdjStep, 0.0), false);		//v3.1.T202 edit
}

void CPositionAdjustDlg::OnDown()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(-m_nAdjustMax - m_nAdjustY);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustY -= nAdjStep;
	}
	UpdateDisplay();

	theServo.MoveTable2(TBL_POS(0.0, -nAdjStep), false);	//v3.1.T202 edit
}

void CPositionAdjustDlg::OnRight()
{
	short nAdjStep = m_nAdjStep;
	if (m_nAdjustMax > 0)
	{
		nAdjStep = abs(-m_nAdjustMax - m_nAdjustX);
		if (nAdjStep > m_nAdjStep) nAdjStep = m_nAdjStep;
		m_nAdjustX -= nAdjStep;
	}
	UpdateDisplay();

	theServo.MoveTable2(TBL_POS(-nAdjStep, 0.0), false);	//v3.1.T202 edit
}

void CPositionAdjustDlg::OnZUp()
{
	theVisionSys.DrawCircleforBTO(++m_nCircleRadius);
}

void CPositionAdjustDlg::OnZDown()
{
	theVisionSys.DrawCircleforBTO(m_nCircleRadius>1 ? --m_nCircleRadius : m_nCircleRadius);
}

void CPositionAdjustDlg::Exit()		//V1.1.W141 add
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

void CPositionAdjustDlg::OnOk()
{
	m_bSelectCancel = false;
    theScanKeyThread.SetJoyStickConfig(JOYSTICK_SERVO, 0);
	Exit();
}

void CPositionAdjustDlg::OnCancel()
{
	m_bSelectCancel = true;
	Exit();
}

void CPositionAdjustDlg::OnAjustLight()
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

void CPositionAdjustDlg::OnBtos2ndEnable()
{
	CRect rc;
	CButton* pBttn = (CButton*)GetDlgItem(IDC_CHECK_BTOS_2ND_ENABLE);
	pBttn->GetWindowRect(&rc);

	if (rc.PtInRect(CPoint(GetMessagePos())) == FALSE)
		pBttn->SetCheck(!pBttn->GetCheck());

	UpdateData(TRUE);

	theBond.Set2ndPointBtosEnable(m_b2ndBondEnable == TRUE);
}

void CPositionAdjustDlg::OnFaster()
{
	m_nAdjStep *= 2;
	if (m_nAdjStep > 16) m_nAdjStep = 16;
	UpdateDisplay();

	theServo.JogSpeedUp();
}

void CPositionAdjustDlg::OnSlower()
{
	m_nAdjStep /= 2;
	if (m_nAdjStep < 1) m_nAdjStep = 1;
	UpdateDisplay();

	theServo.JogSpeedDown();
}

void CPositionAdjustDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)	//V1.1.W140 add
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

BOOL CPositionAdjustDlg::OnEraseBkgnd(CDC* pDC)	//V1.1.W140 add
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

LRESULT CPositionAdjustDlg::OnNcHitTest(CPoint pt)	//V1.1.W140 add
{
	return HTCAPTION;
}

void CPositionAdjustDlg::SetAdjustMax(short nMax)
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

void CPositionAdjustDlg::UpdateDisplay()
{
	if (m_nAdjustMax > 0)
	{
		m_sAdjustX.Format(_T("%.1f um"), -m_nAdjustX*0.5);
		m_sAdjustY.Format(_T("%.1f um"), m_nAdjustY*0.5);
	}
	m_sAdjStep.Format(_T("%.1f um"), m_nAdjStep*0.5);
	UpdateData(false);
	Invalidate();
}