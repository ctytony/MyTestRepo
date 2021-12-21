/***************************************************************
ForceCalibPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "WclCalibPage.h"
#include "WireBondDlg.h"
#include "servo.h"
#include "Busy.h"
#include "MathCalc.h"

// CWclCalibPage dialog

IMPLEMENT_DYNAMIC(CWclCalibPage, CPropertyPage)

CWclCalibPage::CWclCalibPage()
	: CPropertyPage(CWclCalibPage::IDD)
    , m_fCurForceFactorKf(0.0)
    , m_fCurForceFactorIo(0.0)
    , m_fNewForceFactorKf(0.0)
    , m_fNewForceFactorIo(0.0)
    , m_nApplyForce(abs(WCL_CALIB_FORCE1))
    , m_dMeasureForce(0.0)
    , m_nCaliStep(0)
{
	m_bBusy = FALSE;	//V1.1.W145 add
}

CWclCalibPage::~CWclCalibPage()
{
}

void CWclCalibPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CUR_P0, m_fCurForceFactorKf);
    DDX_Text(pDX, IDC_CUR_P1, m_fCurForceFactorIo);
    DDX_Text(pDX, IDC_NEW_P0, m_fNewForceFactorKf);
    DDX_Text(pDX, IDC_NEW_P1, m_fNewForceFactorIo);
    DDX_Text(pDX, IDC_EDIT_APPLY_FORCE, m_nApplyForce);
    DDX_Text(pDX, IDC_EDIT_MEASURE_FORCE, m_dMeasureForce);
}

BEGIN_MESSAGE_MAP(CWclCalibPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_FORCE_CALIB, OnForceCalib)
    ON_BN_CLICKED(IDC_BUTTON_FORCE_VERIFY, OnForceVerify)
    ON_EN_SETFOCUS(IDC_EDIT_APPLY_FORCE, OnFocusApplyForce)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
    ON_BN_CLICKED(IDC_BUTTON_COMPLETE, OnComplete)
    ON_EN_SETFOCUS(IDC_EDIT_MEASURE_FORCE, OnFocusMeasureForce)
	ON_MESSAGE(WM_ISDIALOGBUSY,IsDialogBusy)	//V1.1.W145 add
	ON_BN_CLICKED(IDC_BUTTON_RESET_TO_DEFAULT, &CWclCalibPage::OnBnClickedButtonResetToDefault)
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CWclCalibPage::OnBnClickedButtonDone)
	ON_BN_CLICKED(IDC_BUTTON_VERIFY_DONE, &CWclCalibPage::OnBnClickedButtonVerifyDone)
END_MESSAGE_MAP()

// CWclCalibPage message handlers
BOOL CWclCalibPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CString szString;

	SetDlgItemTextW(IDC_BUTTON_FORCE_CALIB,theMsg.GetMsg(MSG_CALIBRATE));
	SetDlgItemTextW(IDC_BUTTON_NEXT,theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_BUTTON_DONE,theMsg.GetMsg(MSG_DONE));	//v3.0.T453 add
	SetDlgItemTextW(IDC_BUTTON_FORCE_VERIFY,theMsg.GetMsg(MSG_VERIFY));
	SetDlgItemTextW(IDC_BUTTON_COMPLETE,theMsg.GetMsg(MSG_FINISH));
	SetDlgItemTextW(IDC_BUTTON_VERIFY_DONE,theMsg.GetMsg(MSG_DONE));	//v3.0.T453 add
	SetDlgItemTextW(IDC_STATIC_CALIBRATE_PARA,theMsg.GetMsg(MSG_PARAMETERS));
	SetDlgItemTextW(IDC_STATIC_PARAMETER,theMsg.GetMsg(MSG_FACTORS_COLON));
	SetDlgItemTextW(IDC_STATIC_CURRENT,theMsg.GetMsg(MSG_CURRENT));
	SetDlgItemTextW(IDC_STATIC_NEW,theMsg.GetMsg(MSG_NEW_COLON));
	SetDlgItemTextW(IDC_STATIC_FORCE,theMsg.GetMsg(MSG_CALIBRATION));
	SetDlgItemTextW(IDC_STATIC_VERIFY,theMsg.GetMsg(MSG_VERIFICATION));
	SetDlgItemTextW(IDC_STATIC_MEASURE_FORCE,theMsg.GetMsg(MSG_MEASURE_FORCE_COLON));
	SetDlgItemTextW(IDC_STATIC_APPLY_FORCE,theMsg.GetMsg(MSG_APPLY_FORCE_COLON));
	SetDlgItemTextW(IDC_BUTTON_RESET_TO_DEFAULT,theMsg.GetMsg(MSG_RESET_TO_DEFAULT));   //v1.6.P196sc add

    szString.Format(_T("Kf [%.2f,%.1f]"), WCL_FACTOR_KF_LOW_LMT, WCL_FACTOR_KF_UP_LMT);
    SetDlgItemTextW(IDC_STATIC_KF,szString);

    szString.Format(_T("Io [%.1f,%.1f]"), WCL_FACTOR_IO_LOW_LMT, WCL_FACTOR_IO_UP_LMT);
    SetDlgItemTextW(IDC_STATIC_IO,szString);

    m_fCurForceFactorKf = (float)theMcData.GetRcd()->dWclForceFactorKf;
    m_fCurForceFactorIo = (float)theMcData.GetRcd()->dWclForceFactorIo;

    return CPropertyPage::OnInitDialog();
}

HBRUSH CWclCalibPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CWclCalibPage::OnForceCalib()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

    GetDlgItem(IDC_BUTTON_FORCE_CALIB)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_FORCE_VERIFY)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_COMPLETE)->EnableWindow(false);
    GetDlgItem(IDC_EDIT_APPLY_FORCE)->EnableWindow(false);
    GetDlgItem(IDC_EDIT_MEASURE_FORCE)->EnableWindow(true);
    
    //m_nApplyForce = abs(WCL_CALIB_FORCE1);
    //m_dMeasureForce = abs(WCL_CALIB_FORCE1);
    m_dMeasureForce = 0;
    UpdateData(false);
 
    m_nCaliStep = 0;

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_STEP_1));
    pListBox->InsertString(1, theMsg.GetMsg(MSG_MEASURE_WCL_FORCE));
    pListBox->InsertString(2, theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
    pListBox->InsertString(3, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

    theServo.SetWireClampIdleCurrent(WCL_CALIB_CURRENT1);
    theServo.TriggerWireClampProfile(WCL_THREAD_WIRE_CLOSE);
}

void CWclCalibPage::OnNext()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

    long lState, lServoStatus;
    double dKf, dIo;

    switch (m_nCaliStep)
    {
    case 0:
	{
		lServoStatus = theServo.CheckServoError(CHECK_BUFFER_MODE_NONE);
		if (lServoStatus != FOK)
		{
			theServo.StopBuffer(2);
			Sleep_x1ms(20);
			theServo.InitWireClamp();
			break;
		}

		m_nCaliStep = 1;
		m_adMeasureForce[0] = -m_dMeasureForce;

		//m_nApplyForce = abs(WCL_CALIB_FORCE2);
		//m_dMeasureForce = abs(WCL_CALIB_FORCE2);
		m_dMeasureForce = 0;
		UpdateData(false);

		CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
		pListBox->ResetContent();
		pListBox->InsertString(0, theMsg.GetMsg(MSG_STEP_2));
		pListBox->InsertString(1, theMsg.GetMsg(MSG_MEASURE_WCL_FORCE));
		pListBox->InsertString(2, theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
		pListBox->InsertString(3, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

		theServo.SetWireClampIdleCurrent(WCL_CALIB_CURRENT2);
		theServo.TriggerWireClampProfile(WCL_THREAD_WIRE_CLOSE);
		break;
	}
    case 1:
    {
	    lServoStatus = theServo.CheckServoError(CHECK_BUFFER_MODE_NONE);
	    if (lServoStatus != FOK)
        {
			theServo.StopBuffer(2);
			Sleep_x1ms(20);        
			theServo.InitWireClamp();
			break;
        }

        m_nCaliStep = 2;
        m_adMeasureForce[1] = -m_dMeasureForce;
        lState = CalcWclCalibFactor(m_adMeasureForce[0], m_adMeasureForce[1], &dKf, &dIo);
        theServo.StopBuffer(2);
        Sleep_x1ms(20);
        
        theServo.InitWireClamp();

        m_fNewForceFactorKf = (float)dKf;
        m_fNewForceFactorIo = (float)dIo;
        UpdateData(false);

        GetDlgItem(IDC_BUTTON_FORCE_CALIB)->EnableWindow(true);
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
        GetDlgItem(IDC_BUTTON_FORCE_VERIFY)->EnableWindow(true);
        GetDlgItem(IDC_BUTTON_COMPLETE)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_APPLY_FORCE)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_MEASURE_FORCE)->EnableWindow(false);

        CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
        pListBox->ResetContent();
        
        if (lState == FOK)
        {
            pListBox->InsertString(0, theMsg.GetMsg(MSG_CALI_FINISH));
        }
        else
        {
            pListBox->InsertString(0, theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
            pListBox->InsertString(1, theMsg.GetMsg(MSG_CALI_AGAIN));
        }
		break;
    }
    default:
        break;
    }
}

void CWclCalibPage::OnForceVerify()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

	theServo.StopBuffer(2);
    Sleep_x1ms(20);

    GetDlgItem(IDC_BUTTON_FORCE_CALIB)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_FORCE_VERIFY)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_COMPLETE)->EnableWindow(true);
    GetDlgItem(IDC_EDIT_APPLY_FORCE)->EnableWindow(false);
    UpdateData(true);
 
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_MEASURE_WCL_FORCE));

    ST_WCL_PROFILE stWclCalibProfile;
    theServo.GetWireClampProfile(WCL_THREAD_WIRE_CLOSE, &m_stBkupWclProfile);
    stWclCalibProfile = m_stBkupWclProfile;
    stWclCalibProfile.nInterimForce = -m_nApplyForce;
    stWclCalibProfile.nHoldForce = -m_nApplyForce;
    theServo.SetWireClampProfile(WCL_THREAD_WIRE_CLOSE, &stWclCalibProfile);
    theServo.InitWireClamp();
    theServo.TriggerWireClampProfile(WCL_THREAD_WIRE_CLOSE);
}

void CWclCalibPage::OnComplete()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

    theServo.SetWireClampProfile(WCL_THREAD_WIRE_CLOSE, &m_stBkupWclProfile);
    theServo.StopBuffer(2);
    Sleep_x1ms(20);

    theServo.InitWireClamp();

    GetDlgItem(IDC_BUTTON_FORCE_CALIB)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_FORCE_VERIFY)->EnableWindow(true);
    GetDlgItem(IDC_BUTTON_COMPLETE)->EnableWindow(false);
    GetDlgItem(IDC_EDIT_APPLY_FORCE)->EnableWindow(true);

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_VERIFY_FINISH));
}

void CWclCalibPage::OnFocusApplyForce()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

    GetDlgItem(IDC_STATIC_CALIBRATE_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nApplyForce, 0, 1, 200);
    UpdateData(false);
}

void CWclCalibPage::OnFocusMeasureForce()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

    GetDlgItem(IDC_STATIC_CALIBRATE_PARA)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_dMeasureForce);
    UpdateData(false);
}

LRESULT CWclCalibPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W145 add
{
	return m_bBusy;
}

void CWclCalibPage::OnBnClickedButtonResetToDefault()       //v1.6.P196sc add
{
	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	pstMcData->dWclForceFactorKf = 0.025;	//v1.6.T199 edit
	pstMcData->dWclForceFactorIo = 1.05;	//v1.6.T199 edit

	m_fCurForceFactorKf = (float)pstMcData->dWclForceFactorKf;
	m_fCurForceFactorIo = (float)pstMcData->dWclForceFactorIo;

	UpdateData(false);
}

void CWclCalibPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_SETUP, 0);	//v3.0.T453 add
}

void CWclCalibPage::OnBnClickedButtonVerifyDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_SETUP, 0);	//v3.0.T453 add
}
