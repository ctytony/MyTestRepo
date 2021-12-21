/***************************************************************
ForceCalibPage.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "math.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Bond.h"
#include "ForceCalibPage.h"
#include "MachineConfig.h"
#include "WireBondDlg.h"
#include "Debug.h"
#include "servo.h"
#include "Busy.h"
#include "MathCalc.h"
#include "LogTrace.h"

// CForceCalibPage dialog
IMPLEMENT_DYNAMIC(CForceCalibPage, CPropertyPage)

CForceCalibPage::CForceCalibPage()
	: CPropertyPage(CForceCalibPage::IDD)
    , m_fCurForceFactorPa(0.0)
    , m_fCurForceFactorPb(0.0)
    , m_fCurForceFactorPc(0.0)
    , m_fNewForceFactorPa(0.0)
    , m_fNewForceFactorPb(0.0)
    , m_fNewForceFactorPc(0.0)
    , m_nApplyForce(30)
    , m_dMeasureForce(0.0)
    , m_bVerifyEnable(FALSE)
    , m_nCaliStep(0)
    , m_lCtactLvl(0)
{
	m_bBusy = FALSE;	//V1.1.W145 add
	theServo.ReadBondHeadMFlags(&m_nBkupMFlag);	//v3.0.T298 add
}

CForceCalibPage::~CForceCalibPage()
{
	//v3.0.T298 add
	theServo.WriteBondHeadMFlags(m_nBkupMFlag);
	Sleep_x1ms(10);
	theServo.WriteBondHeadDcom(0.0);
	Sleep_x1ms(10);
	theBond.MoveBondHeadToFireLevel();
}

void CForceCalibPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CUR_P0, m_sCurForceFactorPa);
    DDX_Text(pDX, IDC_CUR_P1, m_sCurForceFactorPb);
    DDX_Text(pDX, IDC_CUR_P2, m_sCurForceFactorPc);
    DDX_Text(pDX, IDC_NEW_P0, m_sNewForceFactorPa);
    DDX_Text(pDX, IDC_NEW_P1, m_sNewForceFactorPb);
    DDX_Text(pDX, IDC_NEW_P2, m_sNewForceFactorPc);
    DDX_Text(pDX, IDC_EDIT_APPLY_FORCE, m_nApplyForce);
    DDX_Text(pDX, IDC_EDIT_MEASURE_FORCE, m_dMeasureForce);
    DDX_Radio(pDX, IDC_RADIO_CALIB, m_bVerifyEnable);
}

BEGIN_MESSAGE_MAP(CForceCalibPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()	//v3.0.T478 add
    ON_EN_SETFOCUS(IDC_EDIT_APPLY_FORCE, OnFocusApplyForce)
    ON_EN_SETFOCUS(IDC_EDIT_MEASURE_FORCE, OnFocusMeasureForce)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
    ON_BN_CLICKED(IDC_BUTTON_START, OnStart)
	ON_MESSAGE(WM_ISDIALOGBUSY,IsDialogBusy)	//V1.1.W145 add
	ON_BN_CLICKED(IDC_BUTTON_RESET_FORCE, &CForceCalibPage::OnBnClickedButtonResetForce)
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CForceCalibPage::OnBnClickedButtonDone)
END_MESSAGE_MAP()

// CForceCalibPage message handlers
BOOL CForceCalibPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetDlgItemTextW(IDC_BUTTON_START, theMsg.GetMsg(MSG_START));
    SetDlgItemTextW(IDC_BUTTON_NEXT, theMsg.GetMsg(MSG_NEXT));
    SetDlgItemTextW(IDC_BUTTON_DONE, theMsg.GetMsg(MSG_DONE));
    SetDlgItemTextW(IDC_STATIC_PARAMETER, theMsg.GetMsg(MSG_PARAMETERS));
    SetDlgItemTextW(IDC_STATIC_FACTOR, theMsg.GetMsg(MSG_FACTORS_COLON));
    SetDlgItemTextW(IDC_STATIC_CURRENT, theMsg.GetMsg(MSG_CURRENT));
    SetDlgItemTextW(IDC_STATIC_NEW, theMsg.GetMsg(MSG_NEW_COLON));
    SetDlgItemTextW(IDC_STATIC_FORCE, theMsg.GetMsg(MSG_CALI_FORCE));
    SetDlgItemTextW(IDC_STATIC_APPLY_FORCE, theMsg.GetMsg(MSG_APPLY_FORCE_COLON));
    SetDlgItemTextW(IDC_STATIC_MEASURE_FORCE, theMsg.GetMsg(MSG_MEASURE_FORCE_COLON));
    SetDlgItemTextW(IDC_STATIC_OPERATION, theMsg.GetMsg(MSG_OPERATION));
    SetDlgItemTextW(IDC_RADIO_CALIB, theMsg.GetMsg(MSG_CALIBRATE));
    SetDlgItemTextW(IDC_RADIO_VERIFY, theMsg.GetMsg(MSG_VERIFY));
	SetDlgItemTextW(IDC_BUTTON_RESET_FORCE, theMsg.GetMsg(MSG_RESET_TO_DEFAULT));  //v1.6.P203sc add

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
    m_fCurForceFactorPa = (float)pstMcData->dForceFactorKf;
    m_fCurForceFactorPb = (float)pstMcData->dForceFactorKa;
    m_fCurForceFactorPc = (float)pstMcData->dForceFactorIo;

	//format display
	m_sCurForceFactorPa.Format(_T("%.6f"), m_fCurForceFactorPa);
	m_sCurForceFactorPb.Format(_T("%.6f"), m_fCurForceFactorPb);
	m_sCurForceFactorPc.Format(_T("%.6f"), m_fCurForceFactorPc);

    m_nApplyForce = abs(BH_CALIB_FORCE1);

	//v3.0.T453 add
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_SELECT_OPER_PRESS_START));
	pListBox->InsertString(1, theMsg.GetMsg(MSG_DONE_TO_QUIT));

    return CPropertyPage::OnInitDialog();
}

HBRUSH CForceCalibPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if ((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CForceCalibPage::OnFocusApplyForce()
{
	CBusy bs(&m_bBusy);	//V1.1.W145 add
	UpdateData(true);
	GetDlgItem(IDC_LIST_INFO)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nApplyForce, 0, 1, 500);
	UpdateData(false);
}

void CForceCalibPage::OnFocusMeasureForce()
{
	CBusy bs(&m_bBusy);	//V1.1.W145 add
    UpdateData(true);
    GetDlgItem(IDC_RADIO_CALIB)->SetFocus();
    theKeyPad.GetUserKeyInData(&m_dMeasureForce);
    UpdateData(false);
}

void CForceCalibPage::OnStart()
{
	CBusy bs(&m_bBusy);	//V1.1.W145 add

    UpdateData();
    m_nCaliStep = 0;

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->AddString(theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
    pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MEASURE_FORCE)->EnableWindow(!m_bVerifyEnable);
	GetDlgItem(IDC_RADIO_CALIB)->EnableWindow(!m_bVerifyEnable);
	GetDlgItem(IDC_RADIO_VERIFY)->EnableWindow(m_bVerifyEnable);

	//v3.0.T479 add
	theServo.EnableForceSensor(true);
	Sleep_x1ms(1000);
	ReadForceSensor(0);

	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);	//enable next button last

	theServo.SetServoControlParameter();
	theServo.SetServoSpeedProfile(MOTOR_Z, 0);

    UpdateData(true);
	
	//v3.1.T472 add, v3.1.T493 delete
	//if (theMcConfig.IsForceCaliReset())
	//	OnBnClickedButtonResetForce();

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
    m_fCurForceFactorPa = (float)pstMcData->dForceFactorKf;
    m_fCurForceFactorPb = (float)pstMcData->dForceFactorKa;
    m_fCurForceFactorPc = (float)pstMcData->dForceFactorIo;

	//format display
	m_sCurForceFactorPa.Format(_T("%.6f"), m_fCurForceFactorPa);
	m_sCurForceFactorPb.Format(_T("%.6f"), m_fCurForceFactorPb);
	m_sCurForceFactorPc.Format(_T("%.6f"), m_fCurForceFactorPc);

	m_sNewForceFactorPa.Format(_T(""));
	m_sNewForceFactorPb.Format(_T(""));
	m_sNewForceFactorPc.Format(_T(""));

	m_dMeasureForce = m_nApplyForce = abs(BH_CALIB_FORCE1);
	//m_dMeasureForce = 0;
	UpdateData(false);

	//v3.0.T298 add
	if (m_bVerifyEnable)
	{
		if (theMcConfig.GetForceCaliType() == 1)
			theLog.WriteLine(_T("NewForceVerify..."), true);
		else
			theLog.WriteLine(_T("ForceVerify..."), true);
	}
	else
	{
		//v3.1.T493 add
		if (theMcConfig.IsForceCaliReset())
		{
			OnBnClickedButtonResetForce();
			Sleep_x1ms(100);
		}

		if (theMcConfig.GetForceCaliType() == 1)
			theLog.WriteLine(_T("NewForceCali..."), true);
		else
			theLog.WriteLine(_T("ForceCali..."), true);

		CString str;
		str.Format(_T("Cur_%.6f_%.6f_%.6f"), m_fCurForceFactorPa,
							m_fCurForceFactorPb, m_fCurForceFactorPc);
		theLog.WriteLine(str, true);
	}
}

void CForceCalibPage::OnNext()
{
	CBusy bs(&m_bBusy);		//V1.1.W145 add

	if (theMcConfig.GetForceCaliType() == 1)
		Cali3PtNext2();
	else
		Cali3PtNext();
}

void CForceCalibPage::Cali3PtNext()
{
    int nMFlag;
    double dForceFactorKa, dForceFactorOs, dPosError, dVerifyDac;
	CHTimer timer;
    CListBox *pListBox;
	CString str;

	pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);

    if (m_bVerifyEnable == FALSE)	//calibrate
    {
        switch (m_nCaliStep)
        {
        case 0:
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

            if (theBond.CtactSrch(3, BH_FORCE_CALIB_SRCH_TOL, &m_lCtactLvl) != FOK)
            {
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));
                pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));
                
                GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
                GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
				break;
            }

            if (theServo.PositionCompen(m_lCtactLvl, &dForceFactorKa, &dForceFactorOs) != FOK)
            {
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
                pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));
                
                GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
                GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
				break;
            }

			m_fNewForceFactorPb = (float)(dForceFactorKa);
			m_sNewForceFactorPb.Format(_T("%.6f"), m_fNewForceFactorPb);
            UpdateData(false);

            m_adCaliOutputDac[0] = (m_fCurForceFactorPa * BH_CALIB_FORCE1)
										+ (m_fNewForceFactorPb * m_lCtactLvl)
										+ m_fCurForceFactorPc;
            m_adCaliOutputDac[1] = (m_fCurForceFactorPa * BH_CALIB_FORCE2)
										+ (m_fNewForceFactorPb * m_lCtactLvl)
										+ m_fCurForceFactorPc;
            m_adCaliOutputDac[2] = (m_fCurForceFactorPa * BH_CALIB_FORCE3)
										+ (m_fNewForceFactorPb * m_lCtactLvl)
										+ m_fCurForceFactorPc;
			str.Format(_T("Cali DAC = %.2f, %.2f, %.2f"),	//show calc dac later
							m_adCaliOutputDac[0],
							m_adCaliOutputDac[1],
							m_adCaliOutputDac[2]);

			if (m_adCaliOutputDac[0] >= 0)
				m_adCaliOutputDac[0] = -0.6;

			pListBox->ResetContent();
			if (theMcConfig.IsDebugCaliForce())
			{
	            pListBox->AddString(str);	//display calc dac

				str.Format(_T("CtactLvl=%d_Pb=%.6f"), m_lCtactLvl, m_fNewForceFactorPb);
	            pListBox->AddString(str);

				str.Format(_T("%s_CaliDAC1=%.2f"),
								theMsg.GetMsg(MSG_STEP_1),
								m_adCaliOutputDac[0]);
	            pListBox->AddString(str);
			}
			else
	            pListBox->AddString(theMsg.GetMsg(MSG_STEP_1));
            pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
            pListBox->AddString(theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
            pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

			theServo.MoveAxisZ(m_lCtactLvl + 1000, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
            Sleep_x1ms(50);

            //theServo.StartJogZ(MOTION_NEGATIVE, 2);	//v3.0.T430 delete
			short nSrchSpd = theMcConfig.GetForceCaliSrchSpeed();
			if (nSrchSpd == 0) nSrchSpd = 2000;
            theServo.ForceCaliSrchCtact(nSrchSpd);	//v3.0.T430 add
            Sleep_x1ms(50);

            if (!theMcConfig.IsBypassServo())
			{
				short nSrchThres = theMcConfig.GetForceCaliSrchThres();
				if (nSrchThres == 0) nSrchThres = BH_FORCE_CALIB_SRCH_TOL;
				timer.StartTimer();
                do
				{
					theServo.GetPositionError(MOTOR_Z, &dPosError);
                } while ((fabs(dPosError) < nSrchThres)
							&& (timer.GetTime_x1ms() < SERVO_TIMEOUT));
            }

			theServo.EndJogZ();
            Sleep_x1ms(100);

			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[0]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				return;
			}
            Sleep_x1ms(50);

            if (theServo.ReadBondHeadMFlags(&m_nBkupMFlag) != FOK)
			{
				AfxMessageBox(_T("ReadBondHeadMFlags failed!"));
				return;
			}
            Sleep_x1ms(50);

			nMFlag = m_nBkupMFlag | 0x0002;	//set to open loop
			nMFlag = nMFlag & 0xFFFDFFFF;
            if (theServo.WriteBondHeadMFlags(nMFlag) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadMFlags failed!"));
				return;
			}
            Sleep_x1ms(50);

			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[0]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				return;
			}
            Sleep_x1ms(50);

            GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);

			m_nCaliStep++;
            break;
		}
        case 1:	//start calibrate pt 2
		{
			UpdateData(true);
			m_adCaliMeasureForce[0] = -m_dMeasureForce;
			//v3.0.T298 add
			str.Format(_T("Step2_Apply=%d_Measure=%.0f"), m_nApplyForce, m_dMeasureForce);
			theLog.WriteLine(str, true);

			m_dMeasureForce = m_nApplyForce = abs(BH_CALIB_FORCE2);
			//m_dMeasureForce = 0;
			UpdateData(false);

			if (m_adCaliOutputDac[1] >= 0)
				m_adCaliOutputDac[1] = -1.2;

			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[1]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				return;
			}

			pListBox->ResetContent();
			if (theMcConfig.IsDebugCaliForce())
			{
				str.Format(_T("%s_CaliDAC2=%.2f"),
							theMsg.GetMsg(MSG_STEP_2),
							m_adCaliOutputDac[1]);
	            pListBox->AddString(str);
			}
			else
	            pListBox->AddString(theMsg.GetMsg(MSG_STEP_2));
			pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);

			m_nCaliStep++;
			break;
		}
        case 2:	//finishing calibrate pt 2
		{
			UpdateData(true);
			m_adCaliMeasureForce[1] = -m_dMeasureForce;
			//v3.0.T298 add
			str.Format(_T("Step3_Apply=%d_Measure=%.0f"), m_nApplyForce, m_dMeasureForce);
			theLog.WriteLine(str, true);

			m_dMeasureForce = m_nApplyForce = abs(BH_CALIB_FORCE3);
			//m_dMeasureForce = 0;
			UpdateData(false);

			if (m_adCaliOutputDac[2] >= 0)
				m_adCaliOutputDac[2] = -2.4;

			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[2]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				return;
			}

			pListBox->ResetContent();
			if (theMcConfig.IsDebugCaliForce())
			{
				str.Format(_T("%s_CaliDAC3=%.2f"),
							theMsg.GetMsg(MSG_STEP3_2),
							m_adCaliOutputDac[2]);
	            pListBox->AddString(str);
			}
			else
	            pListBox->AddString(theMsg.GetMsg(MSG_STEP3_2));
			pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

			if (CalcForceCalibFactor(m_fNewForceFactorPb, m_lCtactLvl,
							m_adCaliOutputDac, m_adCaliMeasureForce) != FOK)
			{
				pListBox->ResetContent();
				pListBox->AddString(theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
				pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));

				GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

				m_nCaliStep = -1;	//end calibration
			}
			else
			{
				MACHINE_DATA* pstMcData = theMcData.GetRcd();
				m_fNewForceFactorPa = (float)pstMcData->dForceFactorKf;
				m_fNewForceFactorPb = (float)pstMcData->dForceFactorKa;
				m_fNewForceFactorPc = (float)pstMcData->dForceFactorIo;

				//format display
				m_sNewForceFactorPa.Format(_T("%.6f"), m_fNewForceFactorPa);
				m_sNewForceFactorPb.Format(_T("%.6f"), m_fNewForceFactorPb);
				m_sNewForceFactorPc.Format(_T("%.6f"), m_fNewForceFactorPc);
				UpdateData(false);

				//v3.0.T298 add
				str.Format(_T("New_%.6f_%.6f_%.6f"), m_fNewForceFactorPa,
									m_fNewForceFactorPb, m_fNewForceFactorPc);
				theLog.WriteLine(str, true);

				m_nCaliStep++;
			}
			break;
		}
        case 3:	//finishing calibrate pt 3
		{
			UpdateData(true);
			m_adCaliMeasureForce[2] = -m_dMeasureForce;
			//v3.0.T298 add
			str.Format(_T("Step4_Apply=%d_Measure=%.0f"), m_nApplyForce, m_dMeasureForce);
			theLog.WriteLine(str, true);
		    
			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[2]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom(Dac2) failed!"));
				return;
			}
			Sleep_x1ms(10);

			if (theServo.WriteBondHeadMFlags(m_nBkupMFlag) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadMFlags failed!"));
				return;
			}
			Sleep_x1ms(10);

			if (theServo.WriteBondHeadDcom(0.0) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom(0) failed!"));
				return;
			}

			theBond.MoveBondHeadToFireLevel();

			GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
			//v3.0.T298 add
			GetDlgItem(IDC_EDIT_MEASURE_FORCE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CALIB)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_VERIFY)->EnableWindow(TRUE);

			MACHINE_DATA* pstMcData = theMcData.GetRcd();
			m_fCurForceFactorPa = (float)pstMcData->dForceFactorKf2;
			m_fCurForceFactorPb = (float)pstMcData->dForceFactorKa;
			m_fCurForceFactorPc = (float)pstMcData->dForceFactorIo2;

			//format display
			m_sCurForceFactorPa.Format(_T("%.6f"), m_fCurForceFactorPa);
			m_sCurForceFactorPb.Format(_T("%.6f"), m_fCurForceFactorPb);
			m_sCurForceFactorPc.Format(_T("%.6f"), m_fCurForceFactorPc);

			pListBox->ResetContent();
			if (CalcForceCalibFactor2(m_fNewForceFactorPb, m_lCtactLvl,
						m_adCaliOutputDac, m_adCaliMeasureForce) == FOK)
			{
				pListBox->AddString(theMsg.GetMsg(MSG_CALI_FINISH));
			}
			else
			{
				pListBox->AddString(theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
				pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));
			}

			pstMcData = theMcData.GetRcd();
			m_fNewForceFactorPa = (float)pstMcData->dForceFactorKf2;
			m_fNewForceFactorPb = (float)pstMcData->dForceFactorKa;
			m_fNewForceFactorPc = (float)pstMcData->dForceFactorIo2;

			//format display
			m_sNewForceFactorPa.Format(_T("%.6f"), m_fNewForceFactorPa);
			m_sNewForceFactorPb.Format(_T("%.6f"), m_fNewForceFactorPb);
			m_sNewForceFactorPc.Format(_T("%.6f"), m_fNewForceFactorPc);
			UpdateData(false);

			//v3.0.T298 add
			str.Format(_T("Cur2_%.6f_%.6f_%.6f"), m_fCurForceFactorPa,
								m_fCurForceFactorPb, m_fCurForceFactorPc);
			theLog.WriteLine(str, true);
			str.Format(_T("New2_%.6f_%.6f_%.6f"), m_fNewForceFactorPa,
								m_fNewForceFactorPb, m_fNewForceFactorPc);
			theLog.WriteLine(str, true);

			m_nCaliStep++;
			theLog.WriteLine(_T("ForceCaliEnd"), true);
			break;
		}
        default:
            break;
        }
    }
    else	//verify
    {
        switch (m_nCaliStep)
        {
        case 0:
		{
            GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

            if (theBond.CtactSrch(3, BH_FORCE_CALIB_SRCH_TOL, &m_lCtactLvl) != FOK)
            {
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));
                
                GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
                GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
				break;
            }

			dVerifyDac = theMcData.Calc3PtBondForceDac(-m_nApplyForce, m_lCtactLvl);
            
            theServo.MoveAxisZ(m_lCtactLvl + 1000, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
            Sleep_x1ms(50);

            //theServo.StartJogZ(MOTION_NEGATIVE, 2);	//v3.0.T430 delete
			short nSrchSpd = theMcConfig.GetForceCaliSrchSpeed();
			if (nSrchSpd == 0) nSrchSpd = 2000;
            theServo.ForceCaliSrchCtact(nSrchSpd);		//v3.0.T430 add
            Sleep_x1ms(50);

            if (!theMcConfig.IsBypassServo())
			{
				short nSrchThres = theMcConfig.GetForceCaliSrchThres();
				if (nSrchThres == 0) nSrchThres = BH_FORCE_CALIB_SRCH_TOL;
				timer.StartTimer();
                do
				{
					theServo.GetPositionError(MOTOR_Z, &dPosError);
                } while ((fabs(dPosError) < nSrchThres)
							&& (timer.GetTime_x1ms() < SERVO_TIMEOUT));
			}

            theServo.EndJogZ();

			if (theServo.ReadBondHeadMFlags(&m_nBkupMFlag) != FOK)
			{
				AfxMessageBox(_T("ReadBondHeadMFlags failed!"));
				return;
			}
            nMFlag = m_nBkupMFlag | 0x0002;	//set to open loop
            nMFlag = nMFlag & 0xFFFDFFFF;
			if (theServo.WriteBondHeadMFlags(nMFlag) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadMFlags failed!"));
				return;
			}
			if (theServo.WriteBondHeadDcom(dVerifyDac) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				return;
			}

            pListBox->ResetContent();
			if (theMcConfig.IsDebugCaliForce())
			{
				str.Format(_T("%s_VerifyDAC=%.2f"),
							theMsg.GetMsg(MSG_STEP_1), dVerifyDac);
	            pListBox->AddString(str);
			}
			else
	            pListBox->AddString(theMsg.GetMsg(MSG_STEP_1));
            pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
            pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

            GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);

            m_nCaliStep++;
            break;
		}
        case 1:
        {
            double dZeroForceDac = theMcData.Calc3PtBondForceDac(0, m_lCtactLvl);
            if (theServo.WriteBondHeadDcom(dZeroForceDac) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom(Dac) failed!"));
				return;
			}
            Sleep_x1ms(10);

            if (theServo.WriteBondHeadMFlags(m_nBkupMFlag) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadMFlags failed!"));
				return;
			}
            Sleep_x1ms(10);

            if (theServo.WriteBondHeadDcom(0.0) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom(0) failed!"));
				return;
			}

            theBond.MoveBondHeadToFireLevel();

            m_nCaliStep++;

            GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
			//v3.0.T298 add
			GetDlgItem(IDC_EDIT_MEASURE_FORCE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CALIB)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_VERIFY)->EnableWindow(TRUE);

            pListBox->ResetContent();
            pListBox->AddString(theMsg.GetMsg(MSG_VERIFY_FINISH));
            UpdateData(false);

			theLog.WriteLine(_T("ForceVerifyEnd"), true);
	        break;
        }
        default:
            break;
        }
    }
}

LRESULT CForceCalibPage::IsDialogBusy(WPARAM wParam, LPARAM lParam)		//V1.1.W145 add
{
	return m_bBusy;
}

void CForceCalibPage::OnBnClickedButtonResetForce()    //v1.6.P203sc  add
{
	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	pstMcData->dForceFactorKf = 0.0173;	//v1.6.T216 edit
	pstMcData->dForceFactorKa = 0.00013;
	pstMcData->dForceFactorIo = 1.1;

	m_fCurForceFactorPa = (float)pstMcData->dForceFactorKf;
	m_fCurForceFactorPb = (float)pstMcData->dForceFactorKa;
	m_fCurForceFactorPc = (float)pstMcData->dForceFactorIo;
	m_sCurForceFactorPa.Format(_T("%.6f"), m_fCurForceFactorPa);
	m_sCurForceFactorPb.Format(_T("%.6f"), m_fCurForceFactorPb);
	m_sCurForceFactorPc.Format(_T("%.6f"), m_fCurForceFactorPc);
	UpdateData(FALSE);
}

//v3.0.T298 add
void CForceCalibPage::ResumeBondHead()
{
	KillTimer(1);	//v3.0.T478 add

	theServo.EnableForceSensor(false);
	theServo.WriteBondHeadMFlags(m_nBkupMFlag);
	Sleep_x1ms(100);
	theServo.WriteBondHeadDcom(0.0);
	Sleep_x1ms(100);
	theBond.MoveBondHeadToFireLevel();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MEASURE_FORCE)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_CALIB)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_VERIFY)->EnableWindow(TRUE);
	UpdateData(false);
}

//v3.0.T298 add
void CForceCalibPage::Cali3PtNext2()
{
    int nMFlag;
    double dForceFactorKa, dForceFactorOs, dPosError, dVerifyDac;
	CHTimer timer;
	CString str;

    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);

    if (m_bVerifyEnable == FALSE)	//calibrate
    {
        switch (m_nCaliStep)
        {
        case 0:
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

            if (theBond.CtactSrch(3, BH_FORCE_CALIB_SRCH_TOL, &m_lCtactLvl) != FOK)
            {
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));
                pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));
				ResumeBondHead();
				break;
            }

            if (theServo.PositionCompen(m_lCtactLvl, &dForceFactorKa, &dForceFactorOs) != FOK)
            {
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
                pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));
				ResumeBondHead();
				break;
            }

			m_fNewForceFactorPb = (float)(dForceFactorKa);
			m_sNewForceFactorPb.Format(_T("%.6f"), m_fNewForceFactorPb);
            UpdateData(false);

            m_adCaliOutputDac[0] = (m_fCurForceFactorPa * BH_CALIB_FORCE1)
										+ (m_fNewForceFactorPb * m_lCtactLvl)
										+ m_fCurForceFactorPc;
            m_adCaliOutputDac[1] = (m_fCurForceFactorPa * BH_CALIB_FORCE2)
										+ (m_fNewForceFactorPb * m_lCtactLvl)
										+ m_fCurForceFactorPc;
            m_adCaliOutputDac[2] = (m_fCurForceFactorPa * BH_CALIB_FORCE3)
										+ (m_fNewForceFactorPb * m_lCtactLvl)
										+ m_fCurForceFactorPc;
			str.Format(_T("CaliDAC=%.2f_%.2f_%.2f"),	//show calc dac later
							m_adCaliOutputDac[0],
							m_adCaliOutputDac[1],
							m_adCaliOutputDac[2]);

			if (m_adCaliOutputDac[0] >= 0)
				m_adCaliOutputDac[0] = -0.6;

			pListBox->ResetContent();
			if (theMcConfig.IsDebugCaliForce())
			{
	            pListBox->AddString(str);	//display calc dac
				str.Format(_T("CtactLvl=%d_Pb=%.6f"), m_lCtactLvl, m_fNewForceFactorPb);
	            pListBox->AddString(str);
				str.Format(_T("%s_CaliDAC1=%.2f"), theMsg.GetMsg(MSG_STEP_1),
													m_adCaliOutputDac[0]);
	            pListBox->AddString(str);
			}
			else
	            pListBox->AddString(theMsg.GetMsg(MSG_STEP_1));
            pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
            pListBox->AddString(theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
            pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

			theServo.MoveAxisZ(m_lCtactLvl + 1000, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
            Sleep_x1ms(50);

			//theServo.EnableForceSensor(true);	//v3.0.T479 delete
            //theServo.StartJogZ(MOTION_NEGATIVE, 2);	//v3.0.T430 delete
			short nSrchSpd = theMcConfig.GetForceCaliSrchSpeed();
			if (nSrchSpd == 0) nSrchSpd = 2000;
            theServo.ForceCaliSrchCtact(nSrchSpd);	//v3.0.T430 add
            Sleep_x1ms(50);
			if (!theMcConfig.IsBypassServo())
			{
				short nSrchThres = theMcConfig.GetForceCaliSrchThres();
				if (nSrchThres == 0) nSrchThres = BH_FORCE_CALIB_SRCH_TOL;
				timer.StartTimer();
				do
				{
					if (theServo.GetPositionError(MOTOR_Z, &dPosError) != FOK)
						break;	//v3.1.T535 edit
				} while ((fabs(dPosError) < nSrchThres)
							&& (timer.GetTime_x1ms() < SERVO_TIMEOUT));
			}
			theServo.EndJogZ();
			//v3.1.T536 add
			int nDelay = theMcConfig.GetSwitchForceDelay();
			if (nDelay == 0) nDelay = 100;
			Sleep_x1ms(nDelay);	//v3.1.T536 edit

			//v3.1.T534 add
			short nForcePercent = theMcConfig.GetSwitchForcePercent();
			double dForce = m_adCaliOutputDac[0];
			if (nForcePercent > 0)
				dForce = dForce * nForcePercent / 100.0;
			if (theServo.WriteBondHeadDcom(dForce) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				ResumeBondHead();
				return;
			}
			Sleep_x1ms(nDelay);	//v3.1.T536 edit

			nMFlag = m_nBkupMFlag | 0x0002;	//set to open loop
			nMFlag = nMFlag & 0xFFFDFFFF;
            if (theServo.WriteBondHeadMFlags(nMFlag) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadMFlags failed!"));
				ResumeBondHead();
				return;
			}
            Sleep_x1ms(nDelay);	//v3.1.T536 edit

			if (nForcePercent > 0)	//v3.1.T534 add
			{
				if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[0]) != FOK)
				{
					AfxMessageBox(_T("WriteBondHeadDcom failed!"));
					ResumeBondHead();
					return;
				}
				Sleep_x1ms(nDelay);	//v3.1.T536 edit
			}

			ReadForceSensor(1);	//v3.0.T475 add, v3.0.T479 edit
			SetTimer(1, 1000, NULL);	//v3.0.T478 add
            GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
			m_nCaliStep++;
            break;
		}
        case 1:	//start calibrate pt 2
		{
			KillTimer(1);	//v3.0.T478 add

			UpdateData(true);
			m_adCaliMeasureForce[0] = -m_dMeasureForce;
			//v3.0.T298 add
			str.Format(_T("Step2_Apply=%d_Measure=%.0f"), m_nApplyForce, m_dMeasureForce);
			theLog.WriteLine(str, true);

			m_dMeasureForce = m_nApplyForce = abs(BH_CALIB_FORCE2);
			//m_dMeasureForce = 0;
			UpdateData(false);

			if (m_adCaliOutputDac[1] >= 0)
				m_adCaliOutputDac[1] = -1.2;

			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[1]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				ResumeBondHead();
				return;
			}

			pListBox->ResetContent();
			if (theMcConfig.IsDebugCaliForce())
			{
				str.Format(_T("%s_CaliDAC2=%.2f"),
							theMsg.GetMsg(MSG_STEP_2),
							m_adCaliOutputDac[1]);
	            pListBox->AddString(str);
			}
			else
	            pListBox->AddString(theMsg.GetMsg(MSG_STEP_2));
			pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

			ReadForceSensor(2);	//v3.0.T475 add, v3.0.T479 edit
			SetTimer(1, 1000, NULL);	//v3.0.T478 add
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
			m_nCaliStep++;
			break;
		}
        case 2:	//finishing calibrate pt 2
		{
			KillTimer(1);	//v3.0.T478 add

			UpdateData(true);
			m_adCaliMeasureForce[1] = -m_dMeasureForce;
			//v3.0.T298 add
			str.Format(_T("Step3_Apply=%d_Measure=%.0f"), m_nApplyForce, m_dMeasureForce);
			theLog.WriteLine(str, true);

			m_dMeasureForce = m_nApplyForce = abs(BH_CALIB_FORCE3);
			//m_dMeasureForce = 0;
			UpdateData(false);

			if (m_adCaliOutputDac[2] >= 0)
				m_adCaliOutputDac[2] = -2.4;

			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[2]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				ResumeBondHead();
				return;
			}

			pListBox->ResetContent();
			if (theMcConfig.IsDebugCaliForce())
			{
				str.Format(_T("%s_CaliDAC3=%.2f"),
							theMsg.GetMsg(MSG_STEP3_2),
							m_adCaliOutputDac[2]);
	            pListBox->AddString(str);
			}
			else
	            pListBox->AddString(theMsg.GetMsg(MSG_STEP3_2));
			pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
			pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

			ReadForceSensor(3);	//v3.0.T475 add, v3.0.T479 edit
			SetTimer(1, 1000, NULL);	//v3.0.T478 add

			if (CalcForceCalibFactor(m_fNewForceFactorPb, m_lCtactLvl,
							m_adCaliOutputDac, m_adCaliMeasureForce) != FOK)
			{
				pListBox->ResetContent();
				pListBox->AddString(theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
				pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));

				GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

				m_nCaliStep = -1;	//end calibration
			}
			else
			{
				MACHINE_DATA* pstMcData = theMcData.GetRcd();
				m_fNewForceFactorPa = (float)pstMcData->dForceFactorKf;
				m_fNewForceFactorPb = (float)pstMcData->dForceFactorKa;
				m_fNewForceFactorPc = (float)pstMcData->dForceFactorIo;

				//format display
				m_sNewForceFactorPa.Format(_T("%.6f"), m_fNewForceFactorPa);
				m_sNewForceFactorPb.Format(_T("%.6f"), m_fNewForceFactorPb);
				m_sNewForceFactorPc.Format(_T("%.6f"), m_fNewForceFactorPc);
				UpdateData(false);

				//v3.0.T298 add
				str.Format(_T("New_%.6f_%.6f_%.6f"), m_fNewForceFactorPa,
									m_fNewForceFactorPb, m_fNewForceFactorPc);
				theLog.WriteLine(str, true);

				m_nCaliStep++;
			}
			break;
		}
        case 3:	//finishing calibrate pt 3
		{
			KillTimer(1);	//v3.0.T478 add

			UpdateData(true);
			m_adCaliMeasureForce[2] = -m_dMeasureForce;
			//v3.0.T298 add
			str.Format(_T("Step4_Apply=%d_Measure=%.0f"), m_nApplyForce, m_dMeasureForce);
			theLog.WriteLine(str, true);

			if (theServo.WriteBondHeadDcom(m_adCaliOutputDac[2]) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom(Dac2) failed!"));
				ResumeBondHead();
				return;
			}
			Sleep_x1ms(100);

			ResumeBondHead();

			MACHINE_DATA* pstMcData = theMcData.GetRcd();
			m_fCurForceFactorPa = (float)pstMcData->dForceFactorKf2;
			m_fCurForceFactorPb = (float)pstMcData->dForceFactorKa;
			m_fCurForceFactorPc = (float)pstMcData->dForceFactorIo2;

			//format display
			m_sCurForceFactorPa.Format(_T("%.6f"), m_fCurForceFactorPa);
			m_sCurForceFactorPb.Format(_T("%.6f"), m_fCurForceFactorPb);
			m_sCurForceFactorPc.Format(_T("%.6f"), m_fCurForceFactorPc);

			pListBox->ResetContent();
			if (CalcForceCalibFactor2(m_fNewForceFactorPb, m_lCtactLvl,
						m_adCaliOutputDac, m_adCaliMeasureForce) == FOK)
			{
				pListBox->AddString(theMsg.GetMsg(MSG_CALI_FINISH));
			}
			else
			{
				pListBox->AddString(theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
				pListBox->AddString(theMsg.GetMsg(MSG_CALI_AGAIN));
			}

			pstMcData = theMcData.GetRcd();
			m_fNewForceFactorPa = (float)pstMcData->dForceFactorKf2;
			m_fNewForceFactorPb = (float)pstMcData->dForceFactorKa;
			m_fNewForceFactorPc = (float)pstMcData->dForceFactorIo2;

			//format display
			m_sNewForceFactorPa.Format(_T("%.6f"), m_fNewForceFactorPa);
			m_sNewForceFactorPb.Format(_T("%.6f"), m_fNewForceFactorPb);
			m_sNewForceFactorPc.Format(_T("%.6f"), m_fNewForceFactorPc);
			UpdateData(false);

			//v3.0.T298 add
			str.Format(_T("Cur2_%.6f_%.6f_%.6f"), m_fCurForceFactorPa,
								m_fCurForceFactorPb, m_fCurForceFactorPc);
			theLog.WriteLine(str, true);
			str.Format(_T("New2_%.6f_%.6f_%.6f"), m_fNewForceFactorPa,
								m_fNewForceFactorPb, m_fNewForceFactorPc);
			theLog.WriteLine(str, true);

			m_nCaliStep++;
			theLog.WriteLine(_T("NewForceCaliEnd"), true);
			break;
		}
        default:
            break;
        }
    }
    else	//verify
    {
        switch (m_nCaliStep)
        {
        case 0:
		{
			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

			if (theBond.CtactSrch(3, BH_FORCE_CALIB_SRCH_TOL, &m_lCtactLvl) != FOK)
            {
                pListBox->ResetContent();
                pListBox->AddString(theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));
				ResumeBondHead();
				break;
            }

			dVerifyDac = theMcData.Calc3PtBondForceDac(-m_nApplyForce, m_lCtactLvl);
            
            theServo.MoveAxisZ(m_lCtactLvl + 1000, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
            Sleep_x1ms(50);

            //theServo.StartJogZ(MOTION_NEGATIVE, 2);	//v3.0.T430 delete
			short nSrchSpd = theMcConfig.GetForceCaliSrchSpeed();
			if (nSrchSpd == 0) nSrchSpd = 2000;
            theServo.ForceCaliSrchCtact(nSrchSpd);		//v3.0.T430 add
			Sleep_x1ms(50);
			if (!theMcConfig.IsBypassServo())
			{
				short nSrchThres = theMcConfig.GetForceCaliSrchThres();
				if (nSrchThres == 0) nSrchThres = BH_FORCE_CALIB_SRCH_TOL;
				timer.StartTimer();
				do
				{
					if (theServo.GetPositionError(MOTOR_Z, &dPosError) != FOK)
						break;	//v3.1.T535 edit
				} while ((fabs(dPosError) < nSrchThres)
							&& (timer.GetTime_x1ms() < SERVO_TIMEOUT));
			}
			theServo.EndJogZ();
			//v3.1.T536 add
			int nDelay = theMcConfig.GetSwitchForceDelay();
			if (nDelay == 0) nDelay = 100;
			Sleep_x1ms(nDelay);	//v3.1.T533 add, v3.1.T536 edit

			//v3.1.T534 add
			short nForcePercent = theMcConfig.GetSwitchForcePercent();
			double dForce = dVerifyDac;
			if (nForcePercent > 0)
				dForce = dForce * nForcePercent / 100.0;

			//v3.1.T533 add
			if (theServo.WriteBondHeadDcom(dForce) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom failed!"));
				ResumeBondHead();
				return;
			}
			Sleep_x1ms(nDelay);	//v3.1.T536 edit

			nMFlag = m_nBkupMFlag | 0x0002;	//set to open loop
            nMFlag = nMFlag & 0xFFFDFFFF;
			if (theServo.WriteBondHeadMFlags(nMFlag) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadMFlags failed!"));
				ResumeBondHead();
				return;
			}
			Sleep_x1ms(nDelay);	//v3.1.T533 add, v3.1.T536 edit

			if (nForcePercent > 0)	//v3.1.T534 add
			{
				if (theServo.WriteBondHeadDcom(dVerifyDac) != FOK)
				{
					AfxMessageBox(_T("WriteBondHeadDcom failed!"));
					ResumeBondHead();
					return;
				}
				Sleep_x1ms(nDelay);	//v3.1.T536 add
			}

            pListBox->ResetContent();
			str.Format(_T("CtactLvl=%d_Apply=%d_Dac=%.3f"),
								m_lCtactLvl, m_nApplyForce, dVerifyDac);
			if (theMcConfig.IsDebugCaliForce())
	            pListBox->AddString(str);
			theLog.WriteLine(_T("Step1_") + str, true);	//v3.0.T298 add

            pListBox->AddString(theMsg.GetMsg(MSG_STEP_1));
            pListBox->AddString(theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
            pListBox->AddString(theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));
            GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);

            m_nCaliStep++;
            break;
		}
        case 1:
        {
			double dZeroForceDac = theMcData.Calc3PtBondForceDac(0, m_lCtactLvl);
			if (theServo.WriteBondHeadDcom(dZeroForceDac) != FOK)
			{
				AfxMessageBox(_T("WriteBondHeadDcom(Dac) failed!"));
				ResumeBondHead();
				return;
			}
			Sleep_x1ms(100);

			ResumeBondHead();

			pListBox->ResetContent();
            pListBox->AddString(theMsg.GetMsg(MSG_VERIFY_FINISH));
            UpdateData(false);

			m_nCaliStep++;
			theLog.WriteLine(_T("NewForceVerifyEnd"), true);	//v3.0.T298 add
	        break;
        }
        default:
            break;
        }
    }
}

void CForceCalibPage::OnBnClickedButtonDone()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,
					WM_CLICK_KEYBOARDSHORTCUT, KEY_SETUP, 0);	//v3.0.T453 add
}

//v3.0.T475 add, v3.0.T479 edit
void CForceCalibPage::ReadForceSensor(short nStep)
{
	if (!theMcConfig.IsForceSnrEnable()) return;

	CString str;
	switch (nStep)
	{
	case 0: str.Format(_T("Preload")); break;
	case 1: str.Format(_T("50g")); break;
	case 2: str.Format(_T("100g")); break;
	case 3: str.Format(_T("200g")); break;
	default: str.Format(_T("Err"));
	}
	for (short i = 0; i < 10; i++)
	{
		Sleep_x1ms(100);
		str.AppendFormat(_T("_%d"), theServo.ReadForceSensor());
	}
	theLog.WriteLine(str, true);
    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	pListBox->InsertString(0, str);
	UpdateData(false);
}

//v3.0.T478 add
void CForceCalibPage::OnTimer(UINT_PTR nIDEvent)
{
	if (theMcConfig.IsDebugCaliForce() && theMcConfig.IsForceSnrEnable())
	{
	    CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
		CString str;
		str.Format(_T("ForceSensor_%d"), theServo.ReadForceSensor());
		pListBox->InsertString(1, str);
		UpdateData(false);
	}
    CPropertyPage::OnTimer(nIDEvent);
}
