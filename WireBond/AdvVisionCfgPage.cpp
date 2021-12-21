/***************************************************************
AdvVisionCfgPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"	
#include "KeyPadHandler.h"
#include "AdvVisionCfgPage.h"
#include "VisionSystem.h"
#include "MachineConfig.h"
#include "MachineData.h"

// CAdvVisionCfgPage dialog

IMPLEMENT_DYNAMIC(CAdvVisionCfgPage, CPropertyPage)

CAdvVisionCfgPage::CAdvVisionCfgPage()
: CPropertyPage(CAdvVisionCfgPage::IDD)
, m_nRotateAngle(0)
, m_iPrRetry(0)
, m_iPrFilter(0)
, m_nInteliPadLocateDelay(0)
, m_nLeadTipOffset(0)
, m_nIllDelay(0)
, m_nLeadToolOffset(0)
, m_nMultiPr(0)
{
}

CAdvVisionCfgPage::~CAdvVisionCfgPage()
{
}

void CAdvVisionCfgPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROTATE_ANGLE, m_nRotateAngle);
	DDX_CBIndex(pDX, IDC_COMBO_PR_RETRY, m_iPrRetry);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_nContrast);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_nBrightness);
	DDX_Text(pDX, IDC_EDIT_RED_GAIN, m_nRedGain);
	DDX_Text(pDX, IDC_EDIT_SATURATION, m_nReserved);
	DDX_CBIndex(pDX, IDC_COMBO_PR_FILTER, m_iPrFilter);
	DDX_Text(pDX, IDC_EDIT_IPL_DELAY, m_nInteliPadLocateDelay);
	DDX_Text(pDX, IDC_EDIT_LEAD_TIP_OFFSET, m_nLeadTipOffset);
	DDX_Text(pDX, IDC_EDIT_ILL_DELAY, m_nIllDelay);
	DDX_Text(pDX, IDC_EDIT_LEAD_TOOL_OFFSET, m_nLeadToolOffset);
	DDX_CBIndex(pDX, IDC_COMBO_MULTI_PR, m_nMultiPr);
    DDX_CBIndex(pDX, IDC_COMBO_CAMER_AGC, m_iAGC);
}

BEGIN_MESSAGE_MAP(CAdvVisionCfgPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_ROTATE_ANGLE, OnFocusRotateAngle)
	ON_EN_SETFOCUS(IDC_EDIT_CONTRAST, OnFocusContrast)
	ON_EN_SETFOCUS(IDC_EDIT_RED_GAIN, OnFocusRedGain)
	ON_EN_SETFOCUS(IDC_EDIT_BRIGHTNESS, OnFocusBrightness)
	ON_EN_SETFOCUS(IDC_EDIT_SATURATION, OnFocusSaturation)
	ON_EN_CHANGE(IDC_EDIT_CONTRAST, OnEditContrast)
	ON_EN_CHANGE(IDC_EDIT_RED_GAIN, OnEditRedGain)
	ON_EN_CHANGE(IDC_EDIT_BRIGHTNESS, OnEditBrightness)
	ON_EN_CHANGE(IDC_EDIT_SATURATION, OnEditSaturation)
	ON_CBN_SELCHANGE(IDC_COMBO_PR_FILTER, OnPrFilter)
    ON_EN_SETFOCUS(IDC_EDIT_IPL_DELAY, OnFocusIplDelay)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_TIP_OFFSET, OnFocusLeadTipOffset)
    ON_EN_SETFOCUS(IDC_EDIT_ILL_DELAY, OnFocusIllDelay)
    ON_EN_SETFOCUS(IDC_EDIT_LEAD_TOOL_OFFSET, OnFocusLeadToolOffset)
	ON_CBN_SELCHANGE(IDC_COMBO_MULTI_PR, OnMultiPr)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMER_AGC, OnCamerAgc)
	ON_EN_SETFOCUS(IDC_EDIT_SERIAL_SKIP_NUM, OnFocusSerialSkipNum)	//V1.1.W262 add
	ON_EN_SETFOCUS(IDC_EDIT_SKIP_NUM_PER_INDEX, OnFocusSkipNumPerIndex)	//V1.1.W262 add
	ON_WM_DESTROY()	//V1.1.W259 add
END_MESSAGE_MAP()

// CAdvVisionCfgPage message handlers
BOOL CAdvVisionCfgPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    int iRowTop    = 45;

    SetDlgItemTextW(IDC_STATIC_SETTING,theMsg.GetMsg(MSG_SETTING));
    
    CWnd* pWnd = GetDlgItem(IDC_STATIC_ROTATE_ANGLE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ROTATE_ANGLE));
    
    GetDlgItem(IDC_EDIT_ROTATE_ANGLE)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    //iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

    pWnd = GetDlgItem(IDC_STATIC_PR_RETRY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PR_RETRY));
    
    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PR_RETRY);
    pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
    pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	pWnd = GetDlgItem(IDC_STATIC_PR_FILTER);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PR_FILTER));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PR_FILTER);
	pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
	pComboBox->AddString(_T("1"));
	pComboBox->AddString(_T("2"));
	pComboBox->AddString(_T("3"));
	pComboBox->AddString(_T("4"));

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	pWnd = GetDlgItem(IDC_STATIC_CAMERA_AGC);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_AGC));

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CAMER_AGC);
    pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
    pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	if (theMcConfig.GetCameraType() == CAMERA_MSZ300_USB)
	{
		pWnd = GetDlgItem(IDC_STATIC_CONTRAST);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CONTRAST));
		pWnd->ShowWindow(TRUE);

		GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

		pWnd = GetDlgItem(IDC_STATIC_RED_GAIN);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_RED_GAIN));

		GetDlgItem(IDC_EDIT_RED_GAIN)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

		pWnd = GetDlgItem(IDC_STATIC_BRIGHTNESS);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_BRIGHTNESS));

		GetDlgItem(IDC_EDIT_BRIGHTNESS)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

		iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

		pWnd = GetDlgItem(IDC_STATIC_SATURATION);
		pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
		pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SATURATION));

		GetDlgItem(IDC_EDIT_SATURATION)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	}
	else
	{
		GetDlgItem(IDC_STATIC_CONTRAST)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_RED_GAIN)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_RED_GAIN)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_BRIGHTNESS)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->ShowWindow(FALSE);
    }

    pWnd = GetDlgItem(IDC_STATIC_IPL_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_IPL_DELAY));

    GetDlgItem(IDC_EDIT_IPL_DELAY)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

    pWnd = GetDlgItem(IDC_STATIC_LEAD_TIP_OFFSET);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LEAD_TIP_OFFSET));

    GetDlgItem(IDC_EDIT_LEAD_TIP_OFFSET)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

    pWnd = GetDlgItem(IDC_STATIC_ILL_DELAY);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_ILL_DELAY));

    GetDlgItem(IDC_EDIT_ILL_DELAY)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

    pWnd = GetDlgItem(IDC_STATIC_LEAD_TOOL_OFFSET);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_LEAD_CAP_OFFSET));

    GetDlgItem(IDC_EDIT_LEAD_TOOL_OFFSET)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	pWnd = GetDlgItem(IDC_STATIC_MULTI_PR);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pWnd->SetWindowTextW(theMsg.GetMsg(MSG_MULTI_PR));

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MULTI_PR);
	pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
	pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	const PGM_RECORD *pstPgm = thePgm.GetRcd();

	SetDlgItemText(IDC_STATIC_AUTO_PAD_CENTERING, theMsg.GetMsg(MSG_AUTO_PAD_CENTERING));
	GetDlgItem(IDC_STATIC_AUTO_PAD_CENTERING)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	pComboBox = (CComboBox*)GetDlgItem(IDC_CB_AUTO_PAD_CENTERING);
	pComboBox->AddString(theMsg.GetMsg(MSG_DISABLE));
	pComboBox->AddString(theMsg.GetMsg(MSG_ENABLE));
	pComboBox->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	pComboBox->SetCurSel(pstPgm->bAutoPadCentering);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

	//V1.1.W262 add - begin
    GetDlgItem(IDC_STATIC_SERIAL_SKIP_NUM)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_SERIAL_SKIP_NUM,theMsg.GetMsg(MSG_SERIAL_SKIP_NUM));
    GetDlgItem(IDC_EDIT_SERIAL_SKIP_NUM)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemInt(IDC_EDIT_SERIAL_SKIP_NUM, pstPgm->nSerialSkipNum);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

    GetDlgItem(IDC_STATIC_SKIP_NUM_PER_INDEX)->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    SetDlgItemTextW(IDC_STATIC_SKIP_NUM_PER_INDEX,theMsg.GetMsg(MSG_SKIP_PER_INDEX));
    GetDlgItem(IDC_EDIT_SKIP_NUM_PER_INDEX)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	SetDlgItemInt(IDC_EDIT_SKIP_NUM_PER_INDEX, pstPgm->nSkipNumPerIndex);
	//V1.1.W262 add - end

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE_TINY;

    pWnd = GetDlgItem(IDC_STATIC_SETTING);
    pWnd->SetWindowPos(NULL, 8, 10, 443, iRowTop, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SETTING));

	m_nRotateAngle          = pstPgm->nPrRotateAngle;
    m_iPrRetry              = pstPgm->bPrRetry;
	m_nContrast             = pstPgm->nContrast;
	m_nBrightness           = pstPgm->nBrightness;
	m_nRedGain              = pstPgm->nRedGain;
	m_iPrFilter             = pstPgm->nPrFilterId;
    m_nInteliPadLocateDelay = pstPgm->nInteliPadLocateDelay;
    m_nLeadTipOffset        = pstPgm->nLeadTipOffset;
    m_nIllDelay             = pstPgm->nVllDelay;
    m_nLeadToolOffset       = pstPgm->nLeadCapiOffset;
	m_nMultiPr              = (short)pstPgm->bDiePrMultiSrch;
    m_iAGC                  = pstPgm->nAGC;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CAdvVisionCfgPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAdvVisionCfgPage::OnFocusRotateAngle()
{
    GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nRotateAngle, 20, 10, 90);
    UpdateData(false);
}

void CAdvVisionCfgPage::OnDestroy()		//V1.1.W259 edit
{
	PGM_RECORD *pstPgm = thePgm.GetRcd();
    pstPgm->nPrRotateAngle  = m_nRotateAngle;
	pstPgm->bPrRetry        = (m_iPrRetry == 1);
	pstPgm->nContrast       = m_nContrast;
	pstPgm->nBrightness     = m_nBrightness;
	pstPgm->nRedGain        = m_nRedGain;
    pstPgm->nAGC            = (byte) m_iAGC;
	if ((m_iPrFilter > -1) && (m_iPrFilter < 5))
		pstPgm->nPrFilterId = m_iPrFilter;
    pstPgm->nInteliPadLocateDelay = m_nInteliPadLocateDelay;
    pstPgm->nLeadTipOffset  = m_nLeadTipOffset;
    pstPgm->nVllDelay       = m_nIllDelay;
    pstPgm->nLeadCapiOffset = m_nLeadToolOffset;

	CWnd* pWnd = GetDlgItem(IDC_CB_AUTO_PAD_CENTERING);
	pstPgm->bAutoPadCentering=(BYTE)((CComboBox*)pWnd)->GetCurSel();

    theVisionSys.SetRotateAngle(m_nRotateAngle);
	theVisionSys.EnableVideo(true);

    CPropertyPage::OnDestroy();
}

void CAdvVisionCfgPage::OnFocusContrast()
{
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nContrast, 100, 0, 200);
	UpdateData(false);

	OnEditContrast();
}

void CAdvVisionCfgPage::OnFocusRedGain()
{
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nRedGain, 180, 0, 180);
	UpdateData(false);

	OnEditRedGain();
}

void CAdvVisionCfgPage::OnFocusBrightness()
{
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nBrightness, 0, 0, 255);
	UpdateData(false);

	OnEditBrightness();
}

void CAdvVisionCfgPage::OnFocusSaturation()
{
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nReserved, 100, 0, 300);
	UpdateData(false);

	//OnEditSaturation();
}

void CAdvVisionCfgPage::OnEditContrast()
{
	UpdateData(TRUE);
	theVisionSys.SetCameraParameters((BYTE)m_nRedGain,m_nContrast,m_nBrightness,0);
}

void CAdvVisionCfgPage::OnEditRedGain()
{
	UpdateData(TRUE);
	theVisionSys.SetCameraParameters((BYTE)m_nRedGain,m_nContrast,m_nBrightness,0);
}

void CAdvVisionCfgPage::OnEditBrightness()
{
	UpdateData(TRUE);
	theVisionSys.SetCameraParameters((BYTE)m_nRedGain,m_nContrast,m_nBrightness,0);
}

void CAdvVisionCfgPage::OnEditSaturation()
{
	UpdateData(TRUE);
	//theVisionSys.SetCameraParameters((BYTE)m_nRedGain,m_nContrast,m_nBrightness,0);
}

void CAdvVisionCfgPage::OnPrFilter()
{
	UpdateData(TRUE);

	theVisionSys.EnableVideo(false);

	if (m_iPrFilter == 0)
	{
		theVisionSys.SetImageProcess(0);
		theVisionSys.EnableVideo(true);
	}
	else if (m_iPrFilter>0&&m_iPrFilter < 5)
	{
		theVisionSys.SetImageProcess(m_iPrFilter);
		theVisionSys.GrabImage();
	}
}

void CAdvVisionCfgPage::OnFocusIplDelay()
{
    GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nInteliPadLocateDelay, 40, 10, 500);
    UpdateData(false);
}

void CAdvVisionCfgPage::OnFocusLeadTipOffset()
{
    GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nLeadTipOffset, 40, 10, 500);
    UpdateData(false);
}

void CAdvVisionCfgPage::OnFocusIllDelay()
{
    GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nIllDelay, 10, 10, 500);
    UpdateData(false);
}

void CAdvVisionCfgPage::OnFocusLeadToolOffset()
{
    GetDlgItem(IDC_STATIC_SETTING)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nLeadToolOffset, 20, 0, 50);
    UpdateData(false);
}

void CAdvVisionCfgPage::OnMultiPr()
{
	UpdateData(TRUE);
	thePgm.GetRcd()->bDiePrMultiSrch = (m_nMultiPr != 0);
}

void CAdvVisionCfgPage::OnCamerAgc()
{
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();

	UpdateData(true);
	m_iAGC ? theVisionSys.SetAgcAec(true, false) : theVisionSys.SetAgcAec(false, false);
}

void CAdvVisionCfgPage::OnFocusSerialSkipNum()	//V1.1.W262 add
{
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();

	short nSerialSkipNum = (short)GetDlgItemInt(IDC_EDIT_SERIAL_SKIP_NUM);
	if (theKeyPad.GetUserKeyInData(&nSerialSkipNum, nSerialSkipNum, 0, 20) == CANCEL_SELECTED) return;

	thePgm.GetRcd()->nSerialSkipNum = (BYTE)nSerialSkipNum;
	SetDlgItemInt(IDC_EDIT_SERIAL_SKIP_NUM, nSerialSkipNum);
}

void CAdvVisionCfgPage::OnFocusSkipNumPerIndex()	//V1.1.W262 add
{
	GetDlgItem(IDC_STATIC_SETTING)->SetFocus();

	short nSkipNumPerIndex = (short)GetDlgItemInt(IDC_EDIT_SKIP_NUM_PER_INDEX);
	if (theKeyPad.GetUserKeyInData(&nSkipNumPerIndex, nSkipNumPerIndex, 0, 100) == CANCEL_SELECTED) return;

	thePgm.GetRcd()->nSkipNumPerIndex = (BYTE)nSkipNumPerIndex;
	SetDlgItemInt(IDC_EDIT_SKIP_NUM_PER_INDEX, nSkipNumPerIndex);
}
