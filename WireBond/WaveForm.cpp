/***************************************************************
WaveForm.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "WaveForm.h"
#include "DataManager.h"
#include "Debug.h"
#include "User.h"
#include "MachineConfig.h"
#include "servo.h"

// CWaveForm dialog

IMPLEMENT_DYNAMIC(CWaveForm, CDialog)

CWaveForm::CWaveForm(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveForm::IDD, pParent)
    , m_bShowTraj(FALSE)
    , m_bZoomIn(false)
    , m_TopLeftPt(0)
    , m_BottomRightPt(0)
    , m_dFactorX(0)
    , m_dFactorY(0)
    , m_OriPt(0)
    , m_iServoStatus(0)
    , m_iBondPtCount(0)
{
	//v3.0.T83 add
	m_bRefVelX = theMcConfig.IsShowRVX();
	m_bFbVelX  = theMcConfig.IsShowFVX();
	m_bRefPosX = theMcConfig.IsShowRPX();
	m_bFbPosX  = theMcConfig.IsShowFPX();
	m_bRefVelY = theMcConfig.IsShowRVY();
	m_bFbVelY  = theMcConfig.IsShowFVY();
	m_bRefPosY = theMcConfig.IsShowRPY();
	m_bFbPosY  = theMcConfig.IsShowFPY();
	m_bRefVelZ = theMcConfig.IsShowRVZ();
	m_bFbVelZ  = theMcConfig.IsShowFVZ();
	m_bRefPosZ = theMcConfig.IsShowRPZ();
	m_bFbPosZ  = theMcConfig.IsShowFPZ();
	m_bDcomZ   = theMcConfig.IsShowMFZ();
	m_bDcomWcl = theMcConfig.IsShowWCL();

	memset(m_adData, 0, sizeof(double) * MAX_SCOPE_DATA * SERVO_SCOPE_DATA_LENGTH);
}

CWaveForm::~CWaveForm()
{
}

void CWaveForm::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_RVX, m_bRefVelX);
    DDX_Check(pDX, IDC_CHECK_FVX, m_bFbVelX);
    DDX_Check(pDX, IDC_CHECK_RPX, m_bRefPosX);
    DDX_Check(pDX, IDC_CHECK_FPX, m_bFbPosX);
    DDX_Check(pDX, IDC_CHECK_RVY, m_bRefVelY);
    DDX_Check(pDX, IDC_CHECK_FVY, m_bFbVelY);
    DDX_Check(pDX, IDC_CHECK_RPY, m_bRefPosY);
    DDX_Check(pDX, IDC_CHECK_FPY, m_bFbPosY);
    DDX_Check(pDX, IDC_CHECK_RVZ, m_bRefVelZ);
    DDX_Check(pDX, IDC_CHECK_FVZ, m_bFbVelZ);
    DDX_Check(pDX, IDC_CHECK_RPZ, m_bRefPosZ);
    DDX_Check(pDX, IDC_CHECK_FPZ, m_bFbPosZ);
    DDX_Check(pDX, IDC_CHECK_DW, m_bDcomWcl);
    DDX_Check(pDX, IDC_CHECK_MZ, m_bDcomZ);
    DDX_Radio(pDX, IDC_RADIO_TIME, m_bShowTraj);
    DDX_Slider(pDX, IDC_SLIDER_BOND_PT, m_iBondPtCount);
	//DDV_MinMaxInt(pDX, m_iBondPtCount, 0, 6);	//v3.1.T69 delete
	DDV_MinMaxInt(pDX, m_iBondPtCount, 0, 12);	//v3.1.T69 add
}


BEGIN_MESSAGE_MAP(CWaveForm, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_RADIO_TIME, &CWaveForm::OnBnClickedRadioTime)
    ON_BN_CLICKED(IDC_RADIO_SPACE, &CWaveForm::OnBnClickedRadioSpace)
    ON_BN_CLICKED(IDC_CHECK_RVX, &CWaveForm::OnBnClickedCheckRvx)
    ON_BN_CLICKED(IDC_CHECK_FVX, &CWaveForm::OnBnClickedCheckFvx)
    ON_BN_CLICKED(IDC_CHECK_RPX, &CWaveForm::OnBnClickedCheckRpx)
    ON_BN_CLICKED(IDC_CHECK_FPX, &CWaveForm::OnBnClickedCheckFpx)
    ON_BN_CLICKED(IDC_CHECK_RVY, &CWaveForm::OnBnClickedCheckRvy)
    ON_BN_CLICKED(IDC_CHECK_FVY, &CWaveForm::OnBnClickedCheckFvy)
    ON_BN_CLICKED(IDC_CHECK_RPY, &CWaveForm::OnBnClickedCheckRpy)
    ON_BN_CLICKED(IDC_CHECK_FPY, &CWaveForm::OnBnClickedCheckFpy)
    ON_BN_CLICKED(IDC_CHECK_RVZ, &CWaveForm::OnBnClickedCheckRvz)
    ON_BN_CLICKED(IDC_CHECK_FVZ, &CWaveForm::OnBnClickedCheckFvz)
    ON_BN_CLICKED(IDC_CHECK_RPZ, &CWaveForm::OnBnClickedCheckRpz)
    ON_BN_CLICKED(IDC_CHECK_FPZ, &CWaveForm::OnBnClickedCheckFpz)
    ON_BN_CLICKED(IDC_CHECK_DW, &CWaveForm::OnBnClickedCheckDw)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONUP()
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CWaveForm::OnBnClickedButtonSave)
	ON_WM_MBUTTONDOWN()
    ON_BN_CLICKED(IDC_CHECK_MZ, &CWaveForm::OnBnClickedCheckDz)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOND_PT, &CWaveForm::OnNMReleasedcaptureSliderBondPt)
END_MESSAGE_MAP()


// CWaveForm message handlers

BOOL CWaveForm::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    m_pen[0].CreatePen(PS_SOLID, 1, RGB(255,0,0));
    m_pen[1].CreatePen(PS_SOLID, 1, RGB(192,192,192));
    m_pen[2].CreatePen(PS_SOLID, 1, RGB(128,128,0));
    m_pen[3].CreatePen(PS_SOLID, 1, RGB(0,128,128));
    m_pen[4].CreatePen(PS_SOLID, 1, RGB(0,255,255));
    m_pen[5].CreatePen(PS_SOLID, 1, RGB(255,0,255));
    m_pen[6].CreatePen(PS_SOLID, 1, RGB(128,0,0));
    m_pen[7].CreatePen(PS_SOLID, 1, RGB(128,0,128));
    m_pen[8].CreatePen(PS_DASH,  1, RGB(128,128,128));
    m_pen[9].CreatePen(PS_SOLID, 1, RGB(0,0,255));
    m_pen[10].CreatePen(PS_SOLID, 1, RGB(0,128,0));
    m_pen[11].CreatePen(PS_SOLID, 1, RGB(0,255,0));
    m_pen[12].CreatePen(PS_SOLID, 1, RGB(0,0,128));
    m_pen[13].CreatePen(PS_SOLID, 1, RGB(255,255,0));
    m_pen[14].CreatePen(PS_SOLID, 1, RGB(255,255,255));
    m_pen[15].CreatePen(PS_SOLID, 1, RGB(64,64,0));
    
    CWnd* pWnd = GetDlgItem(IDC_BUTTON_SAVE);
    pWnd->SetWindowPos(NULL, 20, 480, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);

    pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, 130, 480, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);

	//v1.6.T115 add, v3.0.T388 edit
	pWnd = GetDlgItem(IDC_CHECK_WCL);
	switch (theMcConfig.GetWaveformTypeWCL())
	{
	case 1: pWnd->SetWindowTextW(_T("Force")); break;
	case 2: pWnd->SetWindowTextW(_T("FS.Rst")); break;
	case 3: pWnd->SetWindowTextW(_T("FS.Out")); break;
	case 4: pWnd->SetWindowTextW(_T("UsgPat")); break;
	case 100: pWnd->SetWindowTextW(_T("Buf0")); break;		//v3.0.T396 add
	case 101: pWnd->SetWindowTextW(_T("Buf1")); break;		//v3.0.T396 add
	case 102: pWnd->SetWindowTextW(_T("Pgr")); break;
	case 103: pWnd->SetWindowTextW(_T("Restart")); break;	//v3.0.T396 add
	case 110: pWnd->SetWindowTextW(_T("Start0")); break;	//v3.0.T422 add
	case 111: pWnd->SetWindowTextW(_T("Start1")); break;	//v3.0.T422 add
	}
	if (theMcConfig.GetWaveformTypeWCL() > 0)	//v3.0.T407 add
	{
		m_bDcomWcl = TRUE;
		UpdateData(FALSE);
	}
	
	//v3.0.T388 add
	pWnd = GetDlgItem(IDC_CHECK_RPX);
	switch (theMcConfig.GetWaveformTypeRPX())
	{
	case 1: pWnd->SetWindowTextW(_T("Force")); break;
	case 2: pWnd->SetWindowTextW(_T("FS.Rst")); break;
	case 3: pWnd->SetWindowTextW(_T("FS.Out")); break;
	case 4: pWnd->SetWindowTextW(_T("UsgPat")); break;
	case 100: pWnd->SetWindowTextW(_T("Buf0")); break;
	case 101: pWnd->SetWindowTextW(_T("Buf1")); break;
	case 102: pWnd->SetWindowTextW(_T("Pgr")); break;
	case 103: pWnd->SetWindowTextW(_T("Restart")); break;
	case 110: pWnd->SetWindowTextW(_T("Start0")); break;	//v3.0.T422 add
	case 111: pWnd->SetWindowTextW(_T("Start1")); break;	//v3.0.T422 add
	}
	if (theMcConfig.GetWaveformTypeRPX() > 0)	//v3.0.T407 add
	{
		GetDlgItem(IDC_CHECK_RVX)->ShowWindow(SW_HIDE);
		m_bRefPosX = TRUE; m_bRefVelX = FALSE; UpdateData(FALSE);
	}

	//v3.0.T388 add
	pWnd = GetDlgItem(IDC_CHECK_FPX);
	switch (theMcConfig.GetWaveformTypeFPX())
	{
	case 1: pWnd->SetWindowTextW(_T("Force")); break;
	case 2: pWnd->SetWindowTextW(_T("FS.Rst")); break;
	case 3: pWnd->SetWindowTextW(_T("FS.Out")); break;
	case 4: pWnd->SetWindowTextW(_T("UsgPat")); break;
	case 100: pWnd->SetWindowTextW(_T("Buf0")); break;
	case 101: pWnd->SetWindowTextW(_T("Buf1")); break;
	case 102: pWnd->SetWindowTextW(_T("Pgr")); break;
	case 103: pWnd->SetWindowTextW(_T("Restart")); break;
	case 110: pWnd->SetWindowTextW(_T("Start0")); break;	//v3.0.T422 add
	case 111: pWnd->SetWindowTextW(_T("Start1")); break;	//v3.0.T422 add
	}
	if (theMcConfig.GetWaveformTypeFPX() > 0)	//v3.0.T407 add
	{
		GetDlgItem(IDC_CHECK_FVX)->ShowWindow(SW_HIDE);
		m_bFbPosX = TRUE; m_bFbVelX = FALSE; UpdateData(FALSE);
	}

	//v3.0.T394 add
	pWnd = GetDlgItem(IDC_CHECK_RPY);
	switch (theMcConfig.GetWaveformTypeRPY())
	{
	case 1: pWnd->SetWindowTextW(_T("Force")); break;
	case 2: pWnd->SetWindowTextW(_T("FS.Rst")); break;
	case 3: pWnd->SetWindowTextW(_T("FS.Out")); break;
	case 4: pWnd->SetWindowTextW(_T("UsgPat")); break;
	case 100: pWnd->SetWindowTextW(_T("Buf0")); break;
	case 101: pWnd->SetWindowTextW(_T("Buf1")); break;
	case 102: pWnd->SetWindowTextW(_T("Pgr")); break;
	case 103: pWnd->SetWindowTextW(_T("Restart")); break;
	case 110: pWnd->SetWindowTextW(_T("Start0")); break;	//v3.0.T422 add
	case 111: pWnd->SetWindowTextW(_T("Start1")); break;	//v3.0.T422 add
	}
	if (theMcConfig.GetWaveformTypeRPY() > 0)
	{
		GetDlgItem(IDC_CHECK_RVY)->ShowWindow(SW_HIDE);
		m_bRefPosY = TRUE; m_bRefVelY = FALSE; UpdateData(FALSE);
	}

	//v3.0.T394 add
	pWnd = GetDlgItem(IDC_CHECK_FPY);
	switch (theMcConfig.GetWaveformTypeFPY())
	{
	case 1: pWnd->SetWindowTextW(_T("Force")); break;
	case 2: pWnd->SetWindowTextW(_T("FS.Rst")); break;
	case 3: pWnd->SetWindowTextW(_T("FS.Out")); break;
	case 4: pWnd->SetWindowTextW(_T("UsgPat")); break;
	case 100: pWnd->SetWindowTextW(_T("Buf0")); break;
	case 101: pWnd->SetWindowTextW(_T("Buf1")); break;
	case 102: pWnd->SetWindowTextW(_T("Pgr")); break;
	case 103: pWnd->SetWindowTextW(_T("Restart")); break;
	case 110: pWnd->SetWindowTextW(_T("Start0")); break;	//v3.0.T422 add
	case 111: pWnd->SetWindowTextW(_T("Start1")); break;	//v3.0.T422 add
	}
	if (theMcConfig.GetWaveformTypeFPY() > 0)
	{
		GetDlgItem(IDC_CHECK_FVY)->ShowWindow(SW_HIDE);
		m_bFbPosY = TRUE; m_bFbVelY = FALSE; UpdateData(FALSE);
	}

	CRect rect;
    GetClientRect(&rect);

    pWnd = GetDlgItem(IDC_STATIC_WAVEFORM);
    pWnd->SetWindowPos(NULL, 260, 10, rect.right - 320, rect.bottom - 40, SWP_NOZORDER);

    if (theMcConfig.IsBypassAll()
		|| theMcConfig.IsDebugWaveformFile())	//v3.1.T458 add
    {
        m_iServoStatus = ReadMotionDataFromFile(m_adData);
        if (m_iServoStatus == FOK) CalcDeformInfo();
    }
    else
    {
        if (theDebug.IsDebugWaveform() || theDebug.IsDebugWireClamp()
			|| theMcConfig.IsDebugModeWaveform())	//v3.1.T462 add
        {
            theServo.GetServoScopeData(m_adData);
            CalcDeformInfo();
        }
    }

	//v1.6.T115 add, v1.6.T119 edit
	int i, j;
	double dScale = theMcConfig.GetWaveformScaleWCL();
	if (dScale == 0) dScale = -0.03;
	int nOffset = theMcConfig.GetWaveformOffsetWCL();	//v3.0.T388 add
	if (((theMcConfig.GetWaveformTypeWCL() != 0)
		|| (theMcConfig.GetWclType() != 0))	//v3.0.T367 add
		&& ((dScale != 1) || (nOffset != 0)))
	{
		for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			i = SCOPE_DCOM_WCL * SERVO_SCOPE_DATA_LENGTH + j;
			m_adData[i] = m_adData[i] * dScale + nOffset;
		}
	}

	//v3.0.T388 add
	dScale = theMcConfig.GetWaveformScaleRPX();
	if (dScale == 0) dScale = 1;
	nOffset = theMcConfig.GetWaveformOffsetRPX();
	if ((theMcConfig.GetWaveformTypeRPX() != 0)
		&& ((dScale != 1) || (nOffset != 0)))
	{
		for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			i = SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + j;
			m_adData[i] = m_adData[i] * dScale + nOffset;
		}
	}

	//v3.0.T388 add
	dScale = theMcConfig.GetWaveformScaleFPX();
	if (dScale == 0) dScale = 1;
	nOffset = theMcConfig.GetWaveformOffsetFPX();
	if ((theMcConfig.GetWaveformTypeFPX() != 0)
		&& ((dScale != 1) || (nOffset != 0)))
	{
		for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			i = SCOPE_FB_POS_X * SERVO_SCOPE_DATA_LENGTH + j;
			m_adData[i] = m_adData[i] * dScale + nOffset;
		}
	}

	//v3.0.T394 add
	dScale = theMcConfig.GetWaveformScaleRPY();
	if (dScale == 0) dScale = 1;
	nOffset = theMcConfig.GetWaveformOffsetRPY();
	if ((theMcConfig.GetWaveformTypeRPY() != 0)
		&& ((dScale != 1) || (nOffset != 0)))
	{
		for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			i = SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + j;
			m_adData[i] = m_adData[i] * dScale + nOffset;
		}
	}

	//v3.0.T394 add
	dScale = theMcConfig.GetWaveformScaleFPY();
	if (dScale == 0) dScale = 1;
	nOffset = theMcConfig.GetWaveformOffsetFPY();
	if ((theMcConfig.GetWaveformTypeFPY() != 0)
		&& ((dScale != 1) || (nOffset != 0)))
	{
		for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			i = SCOPE_FB_POS_Y * SERVO_SCOPE_DATA_LENGTH + j;
			m_adData[i] = m_adData[i] * dScale + nOffset;
		}
	}

	//v3.0.T387 add
	dScale = theMcConfig.GetWaveformScalePZ();
	nOffset = theMcConfig.GetWaveformOffsetPZ();
	if ((dScale != 1) || (nOffset != 0))	//v3.0.T388 edit
	{
		for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			i = SCOPE_REF_POS_Z * SERVO_SCOPE_DATA_LENGTH + j;
			m_adData[i] = m_adData[i] * dScale + nOffset;

			i = SCOPE_FB_POS_Z * SERVO_SCOPE_DATA_LENGTH + j;
			m_adData[i] = m_adData[i] * dScale + nOffset;
		}
	}

	//v3.0.T387 add
	for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
	{
		i = SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + j;
		m_adData[i] = m_adData[i] * 50;
	}

	if (theDebug.IsDebugSpWaveform())
        theServo.GetSpScopeData(m_anSpData);

    m_BondPtSlider.SubclassDlgItem(IDC_SLIDER_BOND_PT, this);
    m_BondPtSlider.SetRange(0,5);
    m_BondPtSlider.SetPos(m_iBondPtCount);

	this->ShowWindow(SW_MAXIMIZE);	//v3.0.T82 add

    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CWaveForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(RGB(0, 128, 128));
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CWaveForm::OnPaint()
{
    //CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages
    CRect rect;
    GetClientRect(&rect);

    CDialog::OnPaint();

    CWnd* pWnd = GetDlgItem(IDC_STATIC_WAVEFORM);
    pWnd->SetWindowPos(NULL, 260, 10, rect.right-320, rect.bottom-40, SWP_NOZORDER);
    
    if (m_iServoStatus == FOK)
    {
        if (m_bShowTraj == 0)
            ShowWaveform();
        else
            ShowTraj();
    }
}

void CWaveForm::ShowTraj(void)
{
	if (theUser.GetUserLevel() < USER_ENGINEER) return;

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_WAVEFORM);
	pWnd->GetClientRect(&rect);
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	CDC* pDC = pWnd->GetDC();
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->Rectangle(&rect);

	short i, j, k, nStartTime = 0, nEndTime = 0;

	//for (k=0; k<=m_iBondPtCount*2; k++)	//v3.0.T297 edit, v3.1.T69 delete
	for (k = 0; k <= m_iBondPtCount; k++)	//v3.1.T69 add
	{
		for (j = nStartTime; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			if (round(m_adData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + j]) != 0)	//v3.0.T295 edit
			{
				nStartTime = j;
				break;
			}
		}
		if (j >= SERVO_SCOPE_DATA_LENGTH) return;	//v3.0.T297 add

		for (j = nStartTime; j < SERVO_SCOPE_DATA_LENGTH; j++)
		{
			if (round(m_adData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + j]) == 0)
			{
				nStartTime = j;
				break;
			}
		}
		if (j >= SERVO_SCOPE_DATA_LENGTH) return;	//v3.0.T297 add
	}

	for (j = nStartTime; j < SERVO_SCOPE_DATA_LENGTH; j++)
	{
		if (round(m_adData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + j]) != 0)	//v3.0.T295 edit
		{
			nEndTime = j;
			break;
		}
	}
	if (nStartTime >= nEndTime) return;	//v3.0.T297 add

	double dDistX, dDistY, dWireAngle, dSinAngle, dCosAngle;
	dDistX = m_adData[SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + nEndTime]
			- m_adData[SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + nStartTime];
	dDistY = m_adData[SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + nEndTime]
			- m_adData[SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + nStartTime];
	dWireAngle = atan2(dDistY, dDistX);
	dSinAngle = sin(dWireAngle);
	dCosAngle = cos(dWireAngle);

	double adRefPos[2][SERVO_SCOPE_DATA_LENGTH], adFbPos[2][SERVO_SCOPE_DATA_LENGTH];
	for (j = nStartTime; j < nEndTime; j++)
	{
		adRefPos[0][j] = round((m_adData[SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + j] * dCosAngle
								+ m_adData[SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + j] * dSinAngle)
								* theServo.GetResolXY());
		adRefPos[1][j] = round(m_adData[SCOPE_REF_POS_Z * SERVO_SCOPE_DATA_LENGTH + j]
								* theServo.GetResolZ());
		adFbPos[0][j] = round((m_adData[SCOPE_FB_POS_X * SERVO_SCOPE_DATA_LENGTH + j] * dCosAngle
								+ m_adData[SCOPE_FB_POS_Y * SERVO_SCOPE_DATA_LENGTH + j] * dSinAngle)
								* theServo.GetResolXY());
		adFbPos[1][j] = round(m_adData[SCOPE_FB_POS_Z * SERVO_SCOPE_DATA_LENGTH + j]
								* theServo.GetResolZ());
	}

	if (!m_bZoomIn)
	{
		m_TopLeftPt.x = 1000000;
		m_TopLeftPt.y = -1000000;
		m_BottomRightPt.x = -1000000;
		m_BottomRightPt.y = 1000000;

		for (j = nStartTime; j < nEndTime; j++)
		{
			if (round(adRefPos[0][j]) > m_BottomRightPt.x)
				m_BottomRightPt.x = round(adRefPos[0][j]);

			if (round(adRefPos[0][j]) < m_TopLeftPt.x)
				m_TopLeftPt.x = round(adRefPos[0][j]);
	        
			if (round(adFbPos[0][j]) > m_BottomRightPt.x)
				m_BottomRightPt.x = round(adFbPos[0][j]);

			if (round(adFbPos[0][j]) < m_TopLeftPt.x)
				m_TopLeftPt.x = round(adFbPos[0][j]);
	        
			if (round(adRefPos[1][j]) > m_TopLeftPt.y)
				m_TopLeftPt.y = round(adRefPos[1][j]);

			if (round(adRefPos[1][j]) < m_BottomRightPt.y)
				m_BottomRightPt.y = round(adRefPos[1][j]);
	        
			if (round(adFbPos[1][j]) > m_TopLeftPt.y)
				m_TopLeftPt.y = round(adFbPos[1][j]);

			if (round(adFbPos[1][j]) < m_BottomRightPt.y)
				m_BottomRightPt.y = round(adFbPos[1][j]);
		}
	    
		m_TopLeftPt.x     -= round(abs(m_BottomRightPt.x - m_TopLeftPt.x) * 0.05);
		m_BottomRightPt.x += round(abs(m_BottomRightPt.x - m_TopLeftPt.x) * 0.05);
		m_TopLeftPt.y     += round(abs(m_BottomRightPt.y - m_TopLeftPt.y) * 0.05);
		m_BottomRightPt.y -= round(abs(m_BottomRightPt.y - m_TopLeftPt.y) * 0.05);
	}

	int iSpanX = m_BottomRightPt.x - m_TopLeftPt.x;
	m_BottomRightPt.x = m_TopLeftPt.x + iSpanX / 8 * 8;
	int iSpanY = m_TopLeftPt.y - m_BottomRightPt.y;
	m_TopLeftPt.y = m_BottomRightPt.y + iSpanY / 8 * 8;

	m_dFactorX = (double)rect.right / abs(m_BottomRightPt.x - m_TopLeftPt.x);
	m_dFactorY = (double)rect.bottom / abs(m_TopLeftPt.y - m_BottomRightPt.y);
	m_dFactorX = m_dFactorY = min(m_dFactorX, m_dFactorY);
	m_OriPt.x = - round(m_TopLeftPt.x * m_dFactorX);
	m_OriPt.y = round(m_TopLeftPt.y * m_dFactorY);

	CRect winRect;
	GetClientRect(&winRect);

	long lSpanX = m_BottomRightPt.x - m_TopLeftPt.x;
	long lSpanY = m_TopLeftPt.y - m_BottomRightPt.y;

	CDC* pWinDC = GetDC();
	pWinDC->SelectObject(m_pen[SCOPE_DCOM_Z]);
	pWinDC->SetBkColor(RGB(0, 128, 128));
	pDC->SelectObject(m_pen[SCOPE_DCOM_Z]);

	CString str;
	CPoint stStartPt, stObjPt;
	for (i = 1; i < 8; i++)
	{
		stStartPt.x = rect.left;
		stStartPt.y = round(rect.bottom / 8.0 * i);
		pDC->MoveTo(stStartPt);

		str.Format(_T("%d"), round(m_TopLeftPt.y - lSpanY / 8.0 * i));
		pWinDC->TextOutW(winRect.right - 55, stStartPt.y, str);

		stObjPt.x = rect.right;
		stObjPt.y = stStartPt.y;
		pDC->LineTo(stObjPt);

		stStartPt.x = round(rect.right / 8.0 * i);
		stStartPt.y = rect.top;
		pDC->MoveTo(stStartPt);

		stObjPt.x = stStartPt.x;
		stObjPt.y = rect.bottom;
		pDC->LineTo(stObjPt);
	    
		str.Format(_T("%d"), round(m_TopLeftPt.x + lSpanX / 8.0 * i));
		pWinDC->TextOutW(260 + stStartPt.x, winRect.bottom - 25, str);
	}
	ReleaseDC(pWinDC);

	pDC->SelectObject(m_pen[SCOPE_REF_VEL_X]);

	short nLastPt = nStartTime;
	for (j = nStartTime; j < nEndTime; j++)
	{
		stObjPt.x = m_OriPt.x + round(m_dFactorX * adRefPos[0][j]);
		stObjPt.y = m_OriPt.y - round(m_dFactorY * adRefPos[1][j]);

		if (rect.PtInRect(stObjPt))
		{
			if (j != (nLastPt + 1))
				pDC->MoveTo(stObjPt);
			else
				pDC->LineTo(stObjPt);
			nLastPt = j;
		}
	}

	pDC->SelectObject(m_pen[SCOPE_REF_VEL_Z]);
	nLastPt = nStartTime;
	bool bStartPt = true;
	for (j = nStartTime; j < nEndTime; j++)
	{
		stObjPt.x = m_OriPt.x + round(m_dFactorX * adFbPos[0][j]);
		stObjPt.y = m_OriPt.y - round(m_dFactorY * adFbPos[1][j]);

		if (rect.PtInRect(stObjPt))
		{
			if (bStartPt == true)
			{
				pDC->MoveTo(stObjPt);
				bStartPt = false;
			}
			else
				pDC->LineTo(stObjPt);
			nLastPt = j;
		}
		else
			bStartPt = true;
	}

	pWnd->ReleaseDC(pDC);
}

void CWaveForm::ShowWaveform(void)
{
	if (theUser.GetUserLevel() < USER_ENGINEER) return;

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_WAVEFORM);
	pWnd->GetClientRect(&rect);
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	CDC* pDC = pWnd->GetDC();
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->Rectangle(&rect);

	short i, j, nIdx;

	if (!m_bZoomIn)
	{
		m_TopLeftPt.x = 0;
		m_TopLeftPt.y = -1000000;
		m_BottomRightPt.x = 1000;
		m_BottomRightPt.y = 1000000;

		if (theDebug.IsDebugSpWaveform())
		{
			for (j = 0; j < SERVO_SP_SCOPE_DATA_LENGTH; j++)
			{
				if (m_anSpData[j] > m_TopLeftPt.y)
					m_TopLeftPt.y = m_anSpData[j];
				if (m_anSpData[j] < m_BottomRightPt.y)
					m_BottomRightPt.y = m_anSpData[j];
			}
			m_BottomRightPt.x = SERVO_SP_SCOPE_DATA_LENGTH / 20;
		}
		else
		{
			for (i = 0; i < MAX_SCOPE_DATA; i++)
			{
				switch (i)
				{
				case SCOPE_REF_POS_X:
					if (m_bRefPosX == FALSE) continue; break;
				case SCOPE_FB_POS_X:
					if (m_bFbPosX == FALSE) continue; break;
				case SCOPE_REF_POS_Y:
					if (m_bRefPosY == FALSE) continue; break;
				case SCOPE_FB_POS_Y:
					if (m_bFbPosY == FALSE) continue; break;
				case SCOPE_REF_POS_Z:
					if (m_bRefPosZ == FALSE) continue; break;
				case SCOPE_FB_POS_Z:
					if (m_bFbPosZ == FALSE) continue; break;
				case SCOPE_MFLAG_Z:
					if (m_bDcomZ == FALSE) continue; break;
				case SCOPE_DCOM_WCL:
					if (m_bDcomWcl == FALSE) continue; break;
				case SCOPE_REF_VEL_X:
					if (m_bRefVelX == FALSE) continue; break;
				case SCOPE_FB_VEL_X:
					if (m_bFbVelX == FALSE) continue; break;
				case SCOPE_REF_VEL_Y:
					if (m_bRefVelY == FALSE) continue; break;
				case SCOPE_FB_VEL_Y:
					if (m_bFbVelY == FALSE) continue; break;
				case SCOPE_REF_VEL_Z:
					if (m_bRefVelZ == FALSE) continue; break;
				case SCOPE_FB_VEL_Z:
					if (m_bFbVelZ == FALSE) continue; break;
				default: continue;
				}

				for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
				{
					nIdx = i * SERVO_SCOPE_DATA_LENGTH + j;
					if (round(m_adData[nIdx]) > m_TopLeftPt.y)
						m_TopLeftPt.y = round(m_adData[nIdx]);

					if (round(m_adData[nIdx]) < m_BottomRightPt.y)
						m_BottomRightPt.y = round(m_adData[nIdx]);
				}
			}
		}
	    
		m_TopLeftPt.x     -= round(abs(m_BottomRightPt.x - m_TopLeftPt.x) * 0.05);
		m_BottomRightPt.x += round(abs(m_BottomRightPt.x - m_TopLeftPt.x) * 0.05);
		m_TopLeftPt.y     += round(abs(m_BottomRightPt.y - m_TopLeftPt.y) * 0.05);
		m_BottomRightPt.y -= round(abs(m_BottomRightPt.y - m_TopLeftPt.y) * 0.05);
	}

	int iSpanX = m_BottomRightPt.x - m_TopLeftPt.x;
	m_BottomRightPt.x = m_TopLeftPt.x + iSpanX / 8 * 8;
	int iSpanY = m_TopLeftPt.y - m_BottomRightPt.y;
	m_TopLeftPt.y = m_BottomRightPt.y + iSpanY / 8 * 8;

	m_dFactorX = (double)rect.right / abs(m_BottomRightPt.x - m_TopLeftPt.x);
	m_dFactorY = (double)rect.bottom / abs(m_TopLeftPt.y - m_BottomRightPt.y);
	m_OriPt.x = - round(m_TopLeftPt.x * m_dFactorX);
	m_OriPt.y = round(m_TopLeftPt.y * m_dFactorY);

	CDC* pWinDC = GetDC();
	CRect winRect;
	GetClientRect(&winRect);

	long lSpanX = m_BottomRightPt.x - m_TopLeftPt.x;
	long lSpanY = m_TopLeftPt.y - m_BottomRightPt.y;

	pWinDC->SelectObject(m_pen[SCOPE_DCOM_Z]);
	pWinDC->SetBkColor(RGB(0, 128, 128));
	pDC->SelectObject(m_pen[SCOPE_DCOM_Z]);

	CString str;
	CPoint stStartPt, stObjPt;
	for (i = 1; i < 8; i++)
	{
		stStartPt.x = rect.left;
		stStartPt.y = round(rect.bottom / 8.0 * i);
		pDC->MoveTo(stStartPt);

		str.Format(_T("%d"), round(m_TopLeftPt.y - lSpanY / 8.0 * i));
		pWinDC->TextOutW(winRect.right - 55, stStartPt.y, str);

		stObjPt.x = rect.right;
		stObjPt.y = stStartPt.y;
		pDC->LineTo(stObjPt);

		stStartPt.x = round(rect.right / 8.0 * i);
		stStartPt.y = rect.top;
		pDC->MoveTo(stStartPt);

		stObjPt.x = stStartPt.x;
		stObjPt.y = rect.bottom;
		pDC->LineTo(stObjPt);
	    
		str.Format(_T("%d"), round(m_TopLeftPt.x + lSpanX / 8.0 * i));
		pWinDC->TextOutW(260 + stStartPt.x, winRect.bottom - 25, str);
	}
	ReleaseDC(pWinDC);

	short nLastPt;
	if (theDebug.IsDebugSpWaveform())
	{
		nLastPt = 0;
		pDC->SelectObject(m_pen[4]);
		for (j = 0; j < SERVO_SP_SCOPE_DATA_LENGTH; j++)
		{
			stObjPt.x = m_OriPt.x + round(m_dFactorX / 10.0 * j);
			stObjPt.y = m_OriPt.y - round(m_dFactorY * m_anSpData[j]);

			if (rect.PtInRect(stObjPt))
			{
				if (j != (nLastPt + 1))
					pDC->MoveTo(stObjPt);
				else
					pDC->LineTo(stObjPt);

				nLastPt = j;
			}
		}
	}
	else
	{
		for (i = 0; i < MAX_SCOPE_DATA; i++)
		{
			switch (i)
			{
			case SCOPE_REF_POS_X:
				if (m_bRefPosX == FALSE) continue; break;
			case SCOPE_FB_POS_X:
				if (m_bFbPosX == FALSE) continue; break;
			case SCOPE_REF_POS_Y:
				if (m_bRefPosY == FALSE) continue; break;
			case SCOPE_FB_POS_Y:
				if (m_bFbPosY == FALSE) continue; break;
			case SCOPE_REF_POS_Z:
				if (m_bRefPosZ == FALSE) continue; break;
			case SCOPE_FB_POS_Z:
				if (m_bFbPosZ == FALSE) continue; break;
			case SCOPE_DCOM_WCL:
				if (m_bDcomWcl == FALSE) continue; break;
			case SCOPE_MFLAG_Z:
				if (m_bDcomZ == FALSE) continue; break;
			case SCOPE_REF_VEL_X:
				if (m_bRefVelX == FALSE) continue; break;
			case SCOPE_FB_VEL_X:
				if (m_bFbVelX == FALSE) continue; break;
			case SCOPE_REF_VEL_Y:
				if (m_bRefVelY == FALSE) continue; break;
			case SCOPE_FB_VEL_Y:
				if (m_bFbVelY == FALSE) continue; break;
			case SCOPE_REF_VEL_Z:
				if (m_bRefVelZ == FALSE) continue; break;
			case SCOPE_FB_VEL_Z:
				if (m_bFbVelZ == FALSE) continue; break;
			default: continue;
			}

			pDC->SelectObject(m_pen[i]);

			nLastPt = 0;
			for (j = 0; j < SERVO_SCOPE_DATA_LENGTH; j++)
			{
				stObjPt.x = m_OriPt.x + round(m_dFactorX * j);
				//v1.6.T119 delete
				//if (i == SCOPE_MFLAG_Z)
				//    stObjPt.y = m_OriPt.y - round(m_dFactorY * m_adData[i * SERVO_SCOPE_DATA_LENGTH + j] * 10.0);
				//else
					stObjPt.y = m_OriPt.y - round(m_dFactorY * m_adData[i * SERVO_SCOPE_DATA_LENGTH + j]);

				if (rect.PtInRect(stObjPt))
				{
					if (j != (nLastPt + 1))
						pDC->MoveTo(stObjPt);
					else
						pDC->LineTo(stObjPt);

					nLastPt = j;
				}
			}
		}
	}

	pWnd->ReleaseDC(pDC);
}

void CWaveForm::OnBnClickedRadioTime()
{
	UpdateData(true);
	m_bZoomIn = false;
	Invalidate();
}

void CWaveForm::OnBnClickedRadioSpace()
{
	UpdateData(true);
	m_bZoomIn = false;
	//m_iBondPtCount = 0;
	Invalidate();
}

void CWaveForm::OnBnClickedCheckRvx()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckFvx()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckRpx()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckFpx()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckRvy()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckFvy()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckRpy()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckFpy()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckRvz()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckFvz()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckRpz()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckFpz()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedCheckDw()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);
    
    CRect rect, wRect;
    GetClientRect(&rect);
    wRect = CRect(260, 10, rect.right - 60, rect.bottom - 30);
    
    if (wRect.PtInRect(point))
    {
        m_TopLeftPt.x = round((point.x - 260 - m_OriPt.x) / m_dFactorX);
        m_TopLeftPt.y = round((m_OriPt.y - point.y + 10) / m_dFactorY);
    }
}

void CWaveForm::OnLButtonUp(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonUp(nFlags, point);
    
    CRect rect, wRect;
    GetClientRect(&rect);
    wRect = CRect(260, 10, rect.right - 60, rect.bottom - 30);
    
    if (wRect.PtInRect(point))
    {
        m_BottomRightPt.x = round((point.x - 260 - m_OriPt.x) / m_dFactorX);
        m_BottomRightPt.y = round((m_OriPt.y - point.y + 10) / m_dFactorY);

        if (abs(m_BottomRightPt.x - m_TopLeftPt.x) < 8)
        {
            if (m_BottomRightPt.x > m_TopLeftPt.x)
                m_BottomRightPt.x = m_TopLeftPt.x + 8;
            else
                m_BottomRightPt.x = m_TopLeftPt.x - 8;
        }

        if (abs(m_BottomRightPt.y - m_TopLeftPt.y) < 8)
        {
            if (m_BottomRightPt.y > m_TopLeftPt.y)
                m_BottomRightPt.y = m_TopLeftPt.y + 8;
            else
                m_BottomRightPt.y = m_TopLeftPt.y - 8;
        }
        
        m_bZoomIn = true;
        Invalidate();
    }
}

void CWaveForm::OnRButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnRButtonUp(nFlags, point);

	CRect rect, wRect;
	GetClientRect(&rect);
	wRect = CRect(260, 10, rect.right - 60, rect.bottom - 30);
	if (wRect.PtInRect(point))
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnBnClickedButtonSave()
{
    CString szPgmName;
    CFile file;
    char  chBuffer[255], chDataBuffer[640000], *pchBuffer;
    short i, nStringSize;
    long  lBufAddress = 0;

    pchBuffer = chDataBuffer;

    sprintf_s(chBuffer, 255, "[RPX, FPX, RPY, FPY, RPZ, FPZ, MFZ, DW] = [\n");
    nStringSize = strlen(chBuffer);
    memcpy_s(pchBuffer, 32000, chBuffer, nStringSize);
    lBufAddress += nStringSize;
    pchBuffer = &chDataBuffer[lBufAddress];

    for (i = 0; i < SERVO_SCOPE_DATA_LENGTH; i++)
    {
        sprintf_s(chBuffer, 255, "%f, %f, %f, %f, %f, %f, %f, %f;\n",
            m_adData[SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + i],
            m_adData[SCOPE_FB_POS_X * SERVO_SCOPE_DATA_LENGTH + i],
            m_adData[SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + i],
            m_adData[SCOPE_FB_POS_Y * SERVO_SCOPE_DATA_LENGTH + i],
            m_adData[SCOPE_REF_POS_Z * SERVO_SCOPE_DATA_LENGTH + i],
            m_adData[SCOPE_FB_POS_Z * SERVO_SCOPE_DATA_LENGTH + i],
            m_adData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + i],
            m_adData[SCOPE_DCOM_WCL * SERVO_SCOPE_DATA_LENGTH + i]);
        nStringSize = strlen(chBuffer);
        memcpy_s(pchBuffer, 32000, chBuffer, nStringSize);
        lBufAddress += nStringSize;
        pchBuffer = &chDataBuffer[lBufAddress];
    }

    sprintf_s(chBuffer, 255, "];\n");
    nStringSize = strlen(chBuffer);
    memcpy_s(pchBuffer, 32000, chBuffer, nStringSize);
    lBufAddress += nStringSize;
    pchBuffer = &chDataBuffer[lBufAddress];

    CFileDialog dlg(TRUE, _T("dat"), _T("*.dat"));
    if (dlg.DoModal() == IDOK)
    {
        szPgmName = dlg.GetPathName();

        if (file.Open(szPgmName, CFile::modeCreate | CFile::modeWrite) == 0) return;

        file.Write(chDataBuffer, lBufAddress);
        file.Close();
    }
}

void CWaveForm::CalcDeformInfo(void)
{
    short i, j, nStartTime = 0, nEndTime = SERVO_SCOPE_DATA_LENGTH;
    short nWireNum = 1, nBondPt = 1;
    double *padMFlagZ, *padEncVelZ, *padCmdVelZ, *padEncPosZ;

    padMFlagZ  = &m_adData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH];
    padEncVelZ = &m_adData[SCOPE_FB_VEL_Z * SERVO_SCOPE_DATA_LENGTH];
    padCmdVelZ = &m_adData[SCOPE_REF_VEL_Z * SERVO_SCOPE_DATA_LENGTH];
    padEncPosZ = &m_adData[SCOPE_FB_POS_Z * SERVO_SCOPE_DATA_LENGTH];

    m_sDeformInfo.Empty();

    for (i = 0; i < SERVO_SCOPE_DATA_LENGTH; i++)
	{
        if (round(*(padMFlagZ + i)) == 2)
		{
            for (j = i - 4; j > 0; j--)
            {
                if (*(padEncVelZ + j) < *(padCmdVelZ + j))
                {
                    nStartTime = j;
                    break;
                }
            }

            for (j = i; j < SERVO_SCOPE_DATA_LENGTH; j++)
            {
                if (round(*(padMFlagZ + j)) != 2)
                {
                    nEndTime = j;
                    break;
                }

                if (j == SERVO_SCOPE_DATA_LENGTH - 1)
                {
                    nEndTime = j;
                    break;
                }
            }

			if ((nStartTime < SERVO_SCOPE_DATA_LENGTH)
				&& (nEndTime < SERVO_SCOPE_DATA_LENGTH)
				&& (nStartTime < nEndTime))
			{
				m_sDeformInfo.AppendFormat(_T("Wire %d, BondPoint %d\r\n"), nWireNum, nBondPt);

				for (j = nStartTime; j < nEndTime; j++)
					m_sDeformInfo.AppendFormat(_T("%d: %.2f\r\n"), j, *(padEncPosZ + i) - *(padEncPosZ + j));
			}

            i = nEndTime;
            
            nBondPt++;
            if (nBondPt > 2)
            {
                nBondPt = 1;
                nWireNum++;
            }
        }
    }
}

void CWaveForm::OnMButtonDown(UINT nFlags, CPoint point)
{
	CRect rect, wRect;
    GetClientRect(&rect);
    wRect = CRect(260, 10, rect.right - 60, rect.bottom - 30);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_WAVEFORM);
	CDC* pDC = pWnd->GetDC();

	CPen pen;
    pen.CreatePen(PS_SOLID, 1, RGB(255,255,0));
	pDC->SelectObject(&pen);
	
	if (wRect.PtInRect(point))
	{
		pWnd->ScreenToClient(&point);
		pWnd->UpdateWindow();

		pDC->MoveTo(point.x, 0);
        pDC->LineTo(point.x, rect.bottom - 40);
	}

	pWnd->ReleaseDC(pDC);

	CDialog::OnMButtonDown(nFlags, point);
}

int CWaveForm::ReadMotionDataFromFile(double* pdWaveformData)
{
    CString szPgmName;
    CFile file;
    char  chBuffer[255], chDataBuffer[640000], *pchBuffer;
    short i, j, nStringSize;
    long  lBufAddress = 0;

    CFileDialog dlg(TRUE, _T("dat;m"), _T("*.dat;*.m"));
    if (dlg.DoModal() == IDOK)
    {
        szPgmName = dlg.GetPathName();

        if (file.Open(szPgmName, CFile::modeRead) == 0) return -1;
        file.Read(chDataBuffer, (UINT)file.GetLength());
        file.Close();
    }

    pchBuffer = chDataBuffer;

    sprintf_s(chBuffer, 255, "[RPX, FPX, RPY, FPY, RPZ, FPZ, MFZ, DW] = [\n");
    nStringSize = strlen(chBuffer);
    lBufAddress += nStringSize;
    pchBuffer = &chDataBuffer[lBufAddress];
    
    for (i = 0; i < SERVO_SCOPE_DATA_LENGTH; i++)
    {
        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ',')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 1;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }

        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ',')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_FB_POS_X * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 1;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }

        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ',')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 1;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }

        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ',')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_FB_POS_Y * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 1;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }

        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ',')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_REF_POS_Z * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 1;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }

        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ',')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_FB_POS_Z * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 1;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }

        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ',')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 1;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }

        for (j = 0; j < 255; j++)
        {
            if (pchBuffer[j] == ';')
            {
                chBuffer[j] = '\0';
                pdWaveformData[SCOPE_DCOM_WCL * SERVO_SCOPE_DATA_LENGTH + i] = atof(chBuffer);
                lBufAddress += j + 2;
                pchBuffer = &chDataBuffer[lBufAddress];
                break;
            }
            else
                chBuffer[j] = pchBuffer[j];
        }
    }

    pdWaveformData[SCOPE_REF_VEL_X * SERVO_SCOPE_DATA_LENGTH] = 0.0;
    pdWaveformData[SCOPE_FB_VEL_X * SERVO_SCOPE_DATA_LENGTH]  = 0.0;
    pdWaveformData[SCOPE_REF_VEL_Y * SERVO_SCOPE_DATA_LENGTH] = 0.0;
    pdWaveformData[SCOPE_FB_VEL_Y * SERVO_SCOPE_DATA_LENGTH]  = 0.0;
    pdWaveformData[SCOPE_REF_VEL_Z * SERVO_SCOPE_DATA_LENGTH] = 0.0;
    pdWaveformData[SCOPE_FB_VEL_Z * SERVO_SCOPE_DATA_LENGTH]  = 0.0;
    
	//v3.0.T295 delete
    //pdWaveformData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH]  = round(pdWaveformData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH]) & 0x02;
    pdWaveformData[SCOPE_DCOM_WCL * SERVO_SCOPE_DATA_LENGTH]  = pdWaveformData[SCOPE_DCOM_WCL * SERVO_SCOPE_DATA_LENGTH];

    for (i = 1; i < SERVO_SCOPE_DATA_LENGTH; i++)
    {
		if (theMcConfig.GetWaveformTypeRPX() == 0)
			pdWaveformData[SCOPE_REF_VEL_X * SERVO_SCOPE_DATA_LENGTH + i] =
				pdWaveformData[SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + i]
				- pdWaveformData[SCOPE_REF_POS_X * SERVO_SCOPE_DATA_LENGTH + i - 1];
		if (theMcConfig.GetWaveformTypeFPX() == 0)
			pdWaveformData[SCOPE_FB_VEL_X * SERVO_SCOPE_DATA_LENGTH + i]  =
				pdWaveformData[SCOPE_FB_POS_X * SERVO_SCOPE_DATA_LENGTH + i]
				- pdWaveformData[SCOPE_FB_POS_X * SERVO_SCOPE_DATA_LENGTH + i - 1];
		if (theMcConfig.GetWaveformTypeRPY() == 0)	//v3.0.T394 add
	        pdWaveformData[SCOPE_REF_VEL_Y * SERVO_SCOPE_DATA_LENGTH + i] =
				pdWaveformData[SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + i]
				- pdWaveformData[SCOPE_REF_POS_Y * SERVO_SCOPE_DATA_LENGTH + i - 1];
		if (theMcConfig.GetWaveformTypeFPY() == 0)	//v3.0.T394 add
		    pdWaveformData[SCOPE_FB_VEL_Y * SERVO_SCOPE_DATA_LENGTH + i] =
				pdWaveformData[SCOPE_FB_POS_Y * SERVO_SCOPE_DATA_LENGTH + i]
				- pdWaveformData[SCOPE_FB_POS_Y * SERVO_SCOPE_DATA_LENGTH + i - 1];
        pdWaveformData[SCOPE_REF_VEL_Z * SERVO_SCOPE_DATA_LENGTH + i] = pdWaveformData[SCOPE_REF_POS_Z * SERVO_SCOPE_DATA_LENGTH + i] - pdWaveformData[SCOPE_REF_POS_Z * SERVO_SCOPE_DATA_LENGTH + i - 1];
        pdWaveformData[SCOPE_FB_VEL_Z * SERVO_SCOPE_DATA_LENGTH + i]  = pdWaveformData[SCOPE_FB_POS_Z * SERVO_SCOPE_DATA_LENGTH + i] - pdWaveformData[SCOPE_FB_POS_Z * SERVO_SCOPE_DATA_LENGTH + i - 1];
        
		//v3.0.T295 delete
		//pdWaveformData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + i]  = round(pdWaveformData[SCOPE_MFLAG_Z * SERVO_SCOPE_DATA_LENGTH + i]) & 0x02;
        pdWaveformData[SCOPE_DCOM_WCL * SERVO_SCOPE_DATA_LENGTH + i]  = pdWaveformData[SCOPE_DCOM_WCL * SERVO_SCOPE_DATA_LENGTH + i];
    }

    return 0;
}

void CWaveForm::OnBnClickedCheckDz()
{
	UpdateData(true);
	if (m_bShowTraj == 0)
	{
		m_bZoomIn = false;
		Invalidate();
	}
}

void CWaveForm::OnNMReleasedcaptureSliderBondPt(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bShowTraj == 1)
	{
		m_iBondPtCount = m_BondPtSlider.GetPos();
		m_bZoomIn = false;
		Invalidate();
	}
	*pResult = 0;
}
