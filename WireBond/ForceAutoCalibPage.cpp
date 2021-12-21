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
#include "MachineData.h"
#include "Bond.h"
#include "ForceAutoCalibPage.h"
#include "MachineConfig.h"
#include "WireBondDlg.h"		//v2.v20.Q1 add
#include "Debug.h"
#include "servo.h"
#include "MathCalc.h"

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Below is the components added for the WB-ForceSensor Project
/////////////////////////////////////////////////////////////////////////////////////////////
#include "..\Library\ForceCalibration\CommonHeader\MtnApi.h"

extern short acs_init_buff_prog_search_contact_force_control();
extern short acs_init_buffer_prog_srch_contact_f_cali_auto_tune_z();
extern short acs_run_buffer_prog_srch_contact_f_cali_auto_tune_z();
extern short acs_clear_buffer_prog_search_contact_force_control();
extern int sys_init_acs_communication();

// Define for communication with buffer program in controller
#include "..\Library\ForceCalibration\H\ForceCali.h"

static TEACH_CONTACT_INPUT stTeachContactParameter;
static TEACH_CONTACT_OUTPUT stTeachContactResult;
static SEARCH_CONTACT_AND_FORCE_CONTROL_INPUT stSearchContactAndForceControl;
static FORCE_BLOCK_ACS stForceBlock;
static FORCE_CALIBRATION_INPUT stForceCalibrationInput;
static FORCE_CALIBRATION_OUTPUT stForceCalibrationOutput;
static char strDebugText[128];

static FORCE_VERIFY_INPUT stForceVerifyInput;
static FORCE_VERIFY_OUTPUT stForceVerifyOutput;
static MTN_TUNE_POSN_COMPENSATION stPosnCompensationTune ={10, 2000, 7000, 0, 0};
static MTUNE_OUT_POSN_COMPENSATION stPosnCompensationTuneOutput;

int giContactPositionReg;
//static FORCE_BLOCK_ACS stBakForceBlock; // 20081119
static int iContactTuneFlagB1W = 0;

#define FORCE_CONSTANT_GRAM_FULL_ADC          220
#define ACS_ADC_FULL_14BIT                    8192

extern COMM_SETTINGS stServoControllerCommSet;


///// Customer needs to provide below call-back function for debugging
char *pstrDetectionSwitch[] =
{
	"NONE",
	"DOUT",
	"PE",
	"AIN"
};
/*
void mtn_debug_print_to_file(FILE *fpData, SEARCH_CONTACT_AND_FORCE_CONTROL_INPUT *stpSearchContactAndForceControl, TEACH_CONTACT_OUTPUT *stpTeachContactResult)
{

	unsigned int ii, jj;
	FORCE_BLOCK_ACS *stpForceControlBlk;
	stpForceControlBlk = stpSearchContactAndForceControl->stpForceBlkPara;

	fprintf(fpData, "iCountactPositionReg = %d;\n", stpTeachContactResult->iContactPosnReg);
	fprintf(fpData, "fInitForceCommandReadBack = %f;\n", stpTeachContactResult->dInitForceCommandReadBack);

	fprintf(fpData, "SearchHeight_um = %d; \n", (stpSearchContactAndForceControl->stpTeachContactPara->iSearchHeightPosition - stpTeachContactResult->iContactPosnReg));
	fprintf(fpData, "SearchSpd = %d; \n", stpSearchContactAndForceControl->stpTeachContactPara->iSearchVel);
	fprintf(fpData, "DetectionThPE = %d; \n", stpSearchContactAndForceControl->stpTeachContactPara->iSearchTolPE);
	fprintf(fpData, "DetectionFlag = %d; %% %s \n", 
		stpSearchContactAndForceControl->stpTeachContactPara->cFlagDetectionSP, 
		pstrDetectionSwitch[stpSearchContactAndForceControl->stpTeachContactPara->cFlagDetectionSP]);
	fprintf(fpData, "%% Force Command Block Information\n");
	fprintf(fpData, "nNumSeg = %d;\n", stpForceControlBlk->uiNumSegment);
	fprintf(fpData, "aRampCountSeg = [");
	for(ii = 0; ii <stpForceControlBlk->uiNumSegment; ii++)
	{
		if(ii < stpForceControlBlk->uiNumSegment - 1)
		{
			fprintf(fpData, "%d, ", stpForceControlBlk->aiForceBlk_Rampcount[ii]);
		}
		else
		{
			fprintf(fpData, "%d];\n", stpForceControlBlk->aiForceBlk_Rampcount[ii]);
		}
	}
	fprintf(fpData, "aLevelCountSeg = [");
	for(ii = 0; ii <stpForceControlBlk->uiNumSegment; ii++)
	{
		if(ii < stpForceControlBlk->uiNumSegment - 1)
		{
			fprintf(fpData, "%d, ", stpForceControlBlk->aiForceBlk_LevelCount[ii]);
		}
		else
		{
			fprintf(fpData, "%d];\n", stpForceControlBlk->aiForceBlk_LevelCount[ii]);
		}
	}
	fprintf(fpData, "afLevelAmplitude = [");
	for(ii = 0; ii <stpForceControlBlk->uiNumSegment; ii++)
	{
		if(ii < stpForceControlBlk->uiNumSegment - 1)
		{
			fprintf(fpData, "%8.4f, ", stpForceControlBlk->adForceBlk_LevelAmplitude[ii]);
		}
		else
		{
			fprintf(fpData, "%8.4f];\n", stpForceControlBlk->adForceBlk_LevelAmplitude[ii]);
		}
	}
	fprintf(fpData, "%% ACSC Controller, Axis- %d: FPOS, RPOS, PE, iDebCounter, ForceCmd, ForceFb, \n aContactForceControlData = [    ", 
		stpSearchContactAndForceControl->stpTeachContactPara->iAxis);
	for(ii = 0; ii< gstSystemScope.uiDataLen; ii++)
	{

		for(jj = 0; jj< gstSystemScope.uiNumData; jj++)
		{
			if(jj == gstSystemScope.uiNumData - 1)
			{	
				fprintf(fpData, "%8.4f", gdScopeCollectData[jj* gstSystemScope.uiDataLen + ii]);
			}
			else
			{
				fprintf(fpData, "%8.4f,  ", gdScopeCollectData[jj* gstSystemScope.uiDataLen + ii]);
			}
		}
		fprintf(fpData, "\n");
	}
	fprintf(fpData, "];\n");

}
*/

#define THREAD_TEACH_LIMIT                         0
#define THREAD_SEARCH_CONTACT_WITH_FORCE_CONTROL   1
#define THREAD_FORCE_CALIBRATION                   2
#define THREAD_FORCE_VERIFICATION                  3
#define THREAD_FORCE_LOG_VERIFICATION              4
static char cFlagThreadInPosnForceCaliDlg = 0;

unsigned int nCountLoopCheckPositionForce = 10;

UINT PosnForceCaliThreadProc(LPVOID pParam )
{
    CForceAutoCalibPage* pObject = (CForceAutoCalibPage *)pParam;

//	AfxMessageBox("Remember: \n --- Adjust Table s.t. BondHead on Force Sensor \n --- Adjust sensor-amplifier s.t. ForceFbAdc [-7000, -3000]");

	if(cFlagThreadInPosnForceCaliDlg == THREAD_FORCE_CALIBRATION)
	{
//		pObject->ThreadDoForceCalibration();
	}
	else if(cFlagThreadInPosnForceCaliDlg == THREAD_FORCE_VERIFICATION)
	{
//		pObject->ThreadDoForceVerification();
	}

	pObject->mFlagStopPosnForceCaliThread = TRUE;
	return 0;
}




// CForceAutoCalibPage dialog

IMPLEMENT_DYNAMIC(CForceAutoCalibPage, CPropertyPage)

CForceAutoCalibPage::CForceAutoCalibPage()
	: CPropertyPage(CForceAutoCalibPage::IDD)
    , m_fCurForceFactorPa(0.0)
    , m_fCurForceFactorPb(0.0)
    , m_fCurForceFactorPc(0.0)
    , m_fNewForceFactorPa(0.0)
    , m_fNewForceFactorPb(0.0)
    , m_fNewForceFactorPc(0.0)
    , m_nApplyForce(30)
    , m_dMeasureForce(0.0)
    , m_bVerifyEnable(FALSE) //@2
    , m_nCalibStep(0)
    , m_lCtactLvl(0)
    , m_lBkupDac(0)
{

}

CForceAutoCalibPage::~CForceAutoCalibPage()
{
}

void CForceAutoCalibPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CUR_P0, m_fCurForceFactorPa);
    DDX_Text(pDX, IDC_CUR_P1, m_fCurForceFactorPb);
    DDX_Text(pDX, IDC_CUR_P2, m_fCurForceFactorPc);
    DDX_Text(pDX, IDC_NEW_P0, m_fNewForceFactorPa);
    DDX_Text(pDX, IDC_NEW_P1, m_fNewForceFactorPb);
    DDX_Text(pDX, IDC_NEW_P2, m_fNewForceFactorPc);
    DDX_Text(pDX, IDC_EDIT_APPLY_FORCE, m_nApplyForce);
    DDX_Text(pDX, IDC_EDIT_MEASURE_FORCE, m_dMeasureForce);
    DDX_Radio(pDX, IDC_RADIO_CALIB, m_bVerifyEnable);
}


BEGIN_MESSAGE_MAP(CForceAutoCalibPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_APPLY_FORCE, &CForceAutoCalibPage::OnEnSetfocusEditApplyForce)
    ON_EN_SETFOCUS(IDC_EDIT_MEASURE_FORCE, &CForceAutoCalibPage::OnEnSetfocusEditMeasureForce)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, &CForceAutoCalibPage::OnBnClickedButtonNext)
    ON_BN_CLICKED(IDC_BUTTON_START, &CForceAutoCalibPage::OnBnClickedButtonStart)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CForceAutoCalibPage message handlers
BOOL CForceAutoCalibPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CPoint pt;
	pt.x = TREE_BUTTON_LEFT + 115;
	pt.y = TREE_BUTTON_TOP - 150;

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_START));

	pt.x += TREE_BUTTON_WIDTH;

	pWnd = GetDlgItem(IDC_BUTTON_NEXT);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEXT));
    pWnd->EnableWindow(0);

	pWnd = GetDlgItem(IDC_STATIC_PARAMETER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_PARAMETERS));

	pWnd = GetDlgItem(IDC_STATIC_FACTOR);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_FACTORS_COLON));

	pWnd = GetDlgItem(IDC_STATIC_CURRENT);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CURRENT));

    pWnd = GetDlgItem(IDC_STATIC_NEW);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NEW_COLON));

    pWnd = GetDlgItem(IDC_STATIC_FORCE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CALI_FORCE));

    pWnd = GetDlgItem(IDC_STATIC_APPLY_FORCE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_APPLY_FORCE_COLON));

    pWnd = GetDlgItem(IDC_STATIC_MEASURE_FORCE);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_MEASURE_FORCE_COLON));

    pWnd = GetDlgItem(IDC_STATIC_OPERATION);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_OPERATION));

    pWnd = GetDlgItem(IDC_RADIO_CALIB);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CALIBRATE));

    pWnd = GetDlgItem(IDC_RADIO_VERIFY);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_VERIFY));

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	m_fCurForceFactorPa = (float)pstMcData->dForceFactorKf;
    m_fCurForceFactorPb = (float)pstMcData->dForceFactorKa;
    m_fCurForceFactorPc = (float)pstMcData->dForceFactorIo;
    
    m_nApplyForce = abs(BH_CALIB_FORCE1);

    //w.temp 



/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Below is the components added for the WB-ForceSensor Project
/////////////////////////////////////////////////////////////////////////////////////////////


	// initialization
	//w.temp if(sRet = sys_init_acs_communication() != 0)
	{
		fprintf(stderr, "Error initialization of ACS card\n");
	}
	//w.temp if(sRet = acs_clear_buffer_prog_search_contact_force_control() != 0)
	{
		fprintf(stderr, "Error reset the ACS buffer memory\n");
	}
	//w.temp if((sRet = acs_init_buffer_prog_srch_contact_f_cali_auto_tune_z()) != 0)
	{
		fprintf(stderr, "Error downloading the ACE buffer\n");
	}


	// teach contact
	stTeachContactParameter.iAxis = ACSC_AXIS_A; // For WB bondhead
			stTeachContactParameter.iFlagSwitchToForceControl = 0;

			stTeachContactParameter.iResetPosition = 1000; // ??? Need to update with application
			stTeachContactParameter.iSearchHeightPosition = stTeachContactParameter.iResetPosition - 2000;
			stTeachContactParameter.iSearchTolPE = 50;
			stTeachContactParameter.iSearchVel = -15000;
			stTeachContactParameter.iMaxDist = 11000;
			stTeachContactParameter.iMaxJerkMoveSrchHt = 1; // 10 Km/s^3
			stTeachContactParameter.iAntiBounce = 10;
//			stTeachContactParameter.cFlagDetectionSP = 0;
//w.temp			acs_run_buffer_prog_srch_contact_f_cali_auto_tune_z();
			Sleep(200);
			mtn_teach_contact_acs(stServoControllerCommSet.Handle, &stTeachContactParameter, &stTeachContactResult);

			mtnapi_set_speed_profile(stServoControllerCommSet.Handle, stTeachContactParameter.iAxis, &(stTeachContactParameter.stSpeedProfileTeachContact), 0);

			CString cstrTemp;
			// IDC_SHOW_SEARCH_CONTACT_RESULT
			if(stTeachContactResult.iStatus == 0)
			{
				giContactPositionReg = stTeachContactResult.iContactPosnReg;
				fprintf(stderr,"OK: %d\n", stTeachContactResult.iContactPosnReg);
			}
			else if(stTeachContactResult.iStatus == 1)
			{
				fprintf(stderr,"TIME OUT\n");
			}
			else if(stTeachContactResult.iStatus == 2)
			{
				fprintf(stderr,"DISTANCE OUT\n");
			}
			else if(stTeachContactResult.iStatus == 3)
			{
				fprintf(stderr,"Motor Error\n");
			}



//////////////////////////////// End of add-in for WB-ForceSensor Project











    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}




void CForceAutoCalibPage::ThreadDoForceCalibration()
{
	stTeachContactParameter.iSearchHeightPosition = stTeachContactResult.iContactPosnReg + 1000; // Search Height set to application

	stForceCalibrationInput.stForceCaliPara.dDefForceFactor_Kf = DEF_FORCE_FACTOR_KF;//stForceCalibrationOutput.fForceFactor_Kf;
	stForceCalibrationInput.stForceCaliPara.dDefForceOffset_I0 = DEF_FORCE_OFFSET_I0;//stForceCalibrationOutput.fForceOffset_I0;
	stForceCalibrationInput.stForceCaliPara.dDefPosnFactor_Ka = DEF_FORCE_POSN_FACTOR_KA; // stForceCalibrationOutput.fPosnFactor_Ka;
	stForceCalibrationInput.stForceCaliPara.iInitForceHold_cnt = 10; // 10 count, 5 ms
	stForceCalibrationInput.stForceCaliPara.dPreImpForce_DCOM = 
		stForceCalibrationInput.stForceCaliPara.dPreImpForce_gram * DEF_FORCE_FACTOR_KF + DEF_FORCE_OFFSET_I0 +
		stTeachContactResult.iContactPosnReg * DEF_FORCE_POSN_FACTOR_KA;

	stForceCalibrationInput.stForceCaliPara.uiNumPointsSmoothing = 10; // After line-fitting to get Kf and I0, further test same force for this times to smooth I0

	stForceCalibrationInput.stPosnCompensate = stPosnCompensationTune; // from local file static variable


	stForceCalibrationInput.iFlagDebug = 0;

	stForceCalibrationInput.stTeachContactPara = stTeachContactParameter;
	stForceCalibrationInput.stTeachContactPara.iFlagSwitchToForceControl = 1;
	stForceCalibrationInput.stTeachContactPara.iSearchVel = -10000;
	stForceCalibrationInput.stTeachContactPara.iSearchTolPE = 25;
	stForceCalibrationInput.stTeachContactPara.iMaxAccMoveSrchHt = 100;
	stForceCalibrationInput.stTeachContactPara.iMaxJerkMoveSrchHt = 1;

 	mtn_force_calibration(stServoControllerCommSet.Handle, &stForceCalibrationInput, &stForceCalibrationOutput);


	fprintf(stderr, "Pa:%9.7f, Pb:%8.6f, Pc:%8.5f\n", 
		stForceCalibrationOutput.fForceCalibratePa, stForceCalibrationOutput.fForceCalibratePb, stForceCalibrationOutput.fForceCalibratePc);

}



void CForceAutoCalibPage::ThreadDoForceVerification() 
{
	CString cstrTemp;
	stForceVerifyInput.stTeachContactPara = stTeachContactParameter;
	stForceVerifyInput.stTeachContactPara.iFlagSwitchToForceControl = 1;
	stForceVerifyInput.fPosnFactor_Ka = stForceCalibrationOutput.fPosnFactor_Ka;
	stForceVerifyInput.fForceFactor_Kf = stForceCalibrationOutput.fForceFactor_Kf;
	stForceVerifyInput.fForceOffset_I0 = stForceCalibrationOutput.fForceOffset_I0;
	// stForceVerifyInput.iContactPosn, may be changed
	// IDC_CHECK_FORCE_CALI_DEBUG_FLAG
	stForceVerifyInput.iFlagDebug = 0;

	// stForceVerifyInput.dForceVerifyDesiredGram is from the UI

	double dSumForceReadBack_gram = 0, dSumI0 = 0;
	for(unsigned int ii = 0; ii<nCountLoopCheckPositionForce; ii++)
	{
		if(ii == 0)  // Teach contact only for the first time
		{
			stForceVerifyInput.iFlagTeachContact = 1;
		}
		else
		{
			stForceVerifyInput.iFlagTeachContact = 0;
			stForceVerifyInput.iContactPosn = stForceVerifyOutput.iContactPosnReg;
		}
		mtn_force_verify(stServoControllerCommSet.Handle, &stForceVerifyInput, &stForceVerifyOutput);

		fprintf(stderr, "CP: %d, Read back(gram): %8.2f\n", stForceVerifyOutput.iContactPosnReg, stForceVerifyOutput.dForceVerifyReadBack);

		//stForceVerifyInput.fForceOffset_I0 = stForceVerifyInput.fForceOffset_I0 + (stForceVerifyInput.dForceVerifyDesiredGram - stForceVerifyOutput.dForceVerifyReadBack) * stForceVerifyInput.fForceFactor_Kf;

		dSumForceReadBack_gram = dSumForceReadBack_gram + stForceVerifyOutput.dForceVerifyReadBack;
		//dSumI0 = dSumI0 + stForceVerifyInput.fForceOffset_I0;
		Sleep(100);
	}

	if(nCountLoopCheckPositionForce > 0) // 20130102
	{
		fprintf(stderr, "CP: %d, Ave Force(gram): %8.2f\n", 
			stForceVerifyOutput.iContactPosnReg, dSumForceReadBack_gram/nCountLoopCheckPositionForce); // , 20130102

	}

}

void CForceAutoCalibPage::RunPosnForceCaliThread()
{
	mFlagStopPosnForceCaliThread = FALSE;

	//w.temp	acs_run_buffer_prog_srch_contact_f_cali_auto_tune_z(); // 20120203
	Sleep(200);  // 20120203

	m_pWinThreadPosnForceCali = AfxBeginThread(PosnForceCaliThreadProc, this);
	SetPriorityClass(m_pWinThreadPosnForceCali->m_hThread, REALTIME_PRIORITY_CLASS);

	m_pWinThreadPosnForceCali->m_bAutoDelete = FALSE;
}

HBRUSH CForceAutoCalibPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CForceAutoCalibPage::OnEnSetfocusEditApplyForce()
{
    UpdateData(true);
    CWnd* pWnd = GetDlgItem(IDC_RADIO_CALIB);
    pWnd->SetFocus();
    theKeyPad.GetUserKeyInData(&m_nApplyForce, 0, 1, 500);
    UpdateData(false);
}

void CForceAutoCalibPage::OnEnSetfocusEditMeasureForce()
{
    UpdateData(true);

    CWnd* pWnd = GetDlgItem(IDC_RADIO_CALIB);
    pWnd->SetFocus();
    
    theKeyPad.GetUserKeyInData(&m_dMeasureForce);

    UpdateData(false);
}

void CForceAutoCalibPage::OnBnClickedButtonStart()
{
    // TODO: Add your control notification handler code here

	//w.temp
	// Force calibration
	cFlagThreadInPosnForceCaliDlg = THREAD_FORCE_CALIBRATION;
	RunPosnForceCaliThread();

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->EnableWindow(0);
    pWnd = GetDlgItem(IDC_BUTTON_NEXT);
    pWnd->EnableWindow(1);

    return;

    UpdateData();

    m_nCalibStep = 0;

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    pListBox->ResetContent();
    pListBox->InsertString(0, theMsg.GetMsg(MSG_MOVE_TO_CTACT_POS));
    pListBox->InsertString(1, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

    /* w.temp CWnd* */ pWnd = GetDlgItem(IDC_BUTTON_START);
    pWnd->EnableWindow(0);
    pWnd = GetDlgItem(IDC_BUTTON_NEXT);
    pWnd->EnableWindow(1);

    theServo.SetServoControlParameter();
    theServo.SetServoSpeedProfile(MOTOR_Z, 0);

    UpdateData(true);
}

//DCOM=Pa*AppliedForce(g)+Pb*ContactPosn+Pc
//DCOM*32767/100=DOUT[-32768,32767]
//Actual Force = Kf*Kv*Dout*10/32767

void CForceAutoCalibPage::OnBnClickedButtonNext()
{
    // TODO: Add your control notification handler code here

    //w.temp
	// Force verification
	cFlagThreadInPosnForceCaliDlg = THREAD_FORCE_VERIFICATION;
	RunPosnForceCaliThread();

    return;

    int     lStatus, iMotorFlags;
    double  dForceFactorKa, dForceFactorOs, dPosError, dVerifyDac, dZeroForceDac; //@1 @2
	long    lStartTime, lElapseTime;
    CWnd*   pWnd;
    CListBox* pListBox;
    
	pListBox = (CListBox*)GetDlgItem(IDC_LIST_INFO);

    if (m_bVerifyEnable == FALSE)
    {
        switch (m_nCalibStep)
        {
        case 0:
            pWnd = GetDlgItem(IDC_BUTTON_NEXT);
            pWnd->EnableWindow(0);

            lStatus = theBond.CtactSrch(3, BH_FORCE_CALIB_SRCH_TOL, &m_lCtactLvl);
            if (lStatus == FOK)
            {
                lStatus = theServo.PositionCompen(m_lCtactLvl, &dForceFactorKa, &dForceFactorOs);
                if (lStatus == FOK)
                {
                    m_fNewForceFactorPb = (float)(dForceFactorKa);
                    UpdateData(false);

                    m_dForceCalibOutputDac[0] = BH_CALIB_FORCE1 * m_fCurForceFactorPa + m_fNewForceFactorPb * m_lCtactLvl + m_fCurForceFactorPc;
                    m_dForceCalibOutputDac[1] = BH_CALIB_FORCE2 * m_fCurForceFactorPa + m_fNewForceFactorPb * m_lCtactLvl + m_fCurForceFactorPc;
                    m_dForceCalibOutputDac[2] = 0.0 * m_fCurForceFactorPa + m_fNewForceFactorPb * m_lCtactLvl + m_fCurForceFactorPc;

                    theServo.MoveAxisZ(m_lCtactLvl + 1000, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
                    Sleep(50);

                    theServo.StartJogZ(MOTION_NEGATIVE, 2);
	                Sleep(50);

	                lStartTime = GetCurrentTime();
                    if (!theMcConfig.IsBypassAll())
                    {
                        do
						{
                            theServo.GetPositionError(MOTOR_Z, &dPosError);
    
    		                lElapseTime = GetCurrentTime() - lStartTime;
    		                if (lElapseTime > SERVO_TIMEOUT) break;
                        } while (fabs(dPosError) < BH_FORCE_CALIB_SRCH_TOL);
                    }

                    theServo.EndJogZ();

                    if (theServo.ReadBondHeadMFlags(&m_lBkupDac) != FOK)
                    {
                        return;
                    }

                    iMotorFlags = m_lBkupDac | 0x0002;
                    iMotorFlags = iMotorFlags & 0xFFFDFFFF;

                    if (theServo.WriteBondHeadMFlags(iMotorFlags) != FOK)
                    {
                        return;
                    }
                    
                    if (theServo.WriteBondHeadDcom(m_dForceCalibOutputDac[0]) != FOK)
                    {
                        return;
                    }
                    
                    pListBox->ResetContent();
                    pListBox->InsertString(0, theMsg.GetMsg(MSG_STEP_1));
                    pListBox->InsertString(1, theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
                    pListBox->InsertString(2, theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
                    pListBox->InsertString(3, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

                    pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                    pWnd->EnableWindow(1);

                    m_nCalibStep++;
                }
                else
                {
                    pListBox->ResetContent();
                    pListBox->InsertString(0, theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
                    pListBox->InsertString(1, theMsg.GetMsg(MSG_CALI_AGAIN));

                    pWnd = GetDlgItem(IDC_BUTTON_START);
                    pWnd->EnableWindow(1);
                    pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                    pWnd->EnableWindow(0);
                }
            }
            else
            {
                pListBox->ResetContent();
                pListBox->InsertString(0, theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));
                pListBox->InsertString(1, theMsg.GetMsg(MSG_CALI_AGAIN));

                pWnd = GetDlgItem(IDC_BUTTON_START);
                pWnd->EnableWindow(1);
                pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                pWnd->EnableWindow(0);
            }

            break;
        case 1:
            {
                UpdateData(true);

                m_dForceCalibMeasureForce[0] = -m_dMeasureForce;

                m_nApplyForce = abs(BH_CALIB_FORCE2);

				m_dMeasureForce = 0; //@1

			    UpdateData(false);
                
                if (theServo.WriteBondHeadDcom(m_dForceCalibOutputDac[1]) != FOK)
                {
                    return;
                }

                pListBox->ResetContent();
                pListBox->InsertString(0, theMsg.GetMsg(MSG_STEP_2));
                pListBox->InsertString(1, theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
                pListBox->InsertString(2, theMsg.GetMsg(MSG_KEYIN_MEASURED_FORCE));
                pListBox->InsertString(3, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

                pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                pWnd->EnableWindow(1);

                m_nCalibStep++;
            }

            break;
        case 2:
            {
                UpdateData(true);

                m_dForceCalibMeasureForce[1] = -m_dMeasureForce;
                
                if (theServo.WriteBondHeadDcom(m_dForceCalibOutputDac[2]) != FOK)
                {
                    return;
                }

                Sleep(10);

                if (theServo.WriteBondHeadMFlags(m_lBkupDac) != FOK)
                {
                    return;
                }
                
                Sleep(10);

                if (theServo.WriteBondHeadDcom(0.0) != FOK) //@1
                {
                    return;
                }

                theBond.MoveBondHeadToFireLevel();

                m_nCalibStep++;

                pWnd = GetDlgItem(IDC_BUTTON_START);
                pWnd->EnableWindow(1);
                pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                pWnd->EnableWindow(0);

                lStatus = CalcForceCalibFactor(m_fNewForceFactorPb, 
                    m_lCtactLvl, m_dForceCalibOutputDac, m_dForceCalibMeasureForce);

                pListBox->ResetContent();
                if (lStatus == FOK)
                {
                    pListBox->InsertString(0, theMsg.GetMsg(MSG_CALI_FINISH));
                }
                else
                {
                    pListBox->InsertString(0, theMsg.GetMsg(MSG_FACTOR_OUT_RANGE));
                    pListBox->InsertString(1, theMsg.GetMsg(MSG_CALI_AGAIN));
                }
               
				MACHINE_DATA* pstMcData = theMcData.GetRcd();
				
                m_fNewForceFactorPa = (float)pstMcData->dForceFactorKf;
                m_fNewForceFactorPb = (float)pstMcData->dForceFactorKa;
                m_fNewForceFactorPc = (float)pstMcData->dForceFactorIo;

                UpdateData(false);
            }
        default:
            break;
        }
    }
    else //@2
    {
        switch (m_nCalibStep)
        {
        case 0:
            pWnd = GetDlgItem(IDC_BUTTON_NEXT);
            pWnd->EnableWindow(0);

            lStatus = theBond.CtactSrch(3, BH_FORCE_CALIB_SRCH_TOL, &m_lCtactLvl);
            if (lStatus == FOK)
            {
                dVerifyDac = theMcData.Calc3PtBondForceDac(-m_nApplyForce, m_lCtactLvl);
                
                theServo.MoveAxisZ(m_lCtactLvl + 1000, MOTION_ABSOLUTE, MOTION_WITH_WAIT);
                Sleep(50);

                theServo.StartJogZ(MOTION_NEGATIVE, 2);
                Sleep(50);

                lStartTime = GetCurrentTime();
                if (!theMcConfig.IsBypassAll())
                {
                    do
					{
                        theServo.GetPositionError(MOTOR_Z, &dPosError);

                        lElapseTime = GetCurrentTime() - lStartTime;
                        if (lElapseTime > SERVO_TIMEOUT) break;
                    } while (fabs(dPosError) < BH_FORCE_CALIB_SRCH_TOL);
                }

                theServo.EndJogZ();

                if (theServo.ReadBondHeadMFlags(&m_lBkupDac) != FOK)
                {
                    return;
                }

                iMotorFlags = m_lBkupDac | 0x0002;
                iMotorFlags = iMotorFlags & 0xFFFDFFFF;

                if (theServo.WriteBondHeadMFlags(iMotorFlags) != FOK)
                {
                    return;
                }

                if (theServo.WriteBondHeadDcom(dVerifyDac) != FOK)
                {
                    return;
                }

                pListBox->ResetContent();
                pListBox->InsertString(0, theMsg.GetMsg(MSG_STEP_1));
                pListBox->InsertString(1, theMsg.GetMsg(MSG_MEASURE_BH_FORCE));
                pListBox->InsertString(2, theMsg.GetMsg(MSG_NEXT_TO_CONTINUE));

                pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                pWnd->EnableWindow(1);

                m_nCalibStep++;
            }
            else
            {
                pListBox->ResetContent();
                pListBox->InsertString(0, theMsg.GetMsg(MSG_CTACT_SRCH_FAILURE));

                pWnd = GetDlgItem(IDC_BUTTON_START);
                pWnd->EnableWindow(1);
                pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                pWnd->EnableWindow(0);
            }
            break;
        case 1:
            {
                dZeroForceDac = theMcData.Calc3PtBondForceDac(0, m_lCtactLvl);

                if (theServo.WriteBondHeadDcom(dZeroForceDac) != FOK)
                {
                    return;
                }

                Sleep(10);

                if (theServo.WriteBondHeadMFlags(m_lBkupDac) != FOK)
                {
                    return;
                }

                Sleep(10);

                if (theServo.WriteBondHeadDcom(0.0) != FOK)
                {
                    return;
                }

                theBond.MoveBondHeadToFireLevel();

                m_nCalibStep++;

                pWnd = GetDlgItem(IDC_BUTTON_START);
                pWnd->EnableWindow(1);
                pWnd = GetDlgItem(IDC_BUTTON_NEXT);
                pWnd->EnableWindow(0);

                pListBox->ResetContent();
                pListBox->InsertString(0, theMsg.GetMsg(MSG_VERIFY_FINISH));
                UpdateData(false);
            }

            break;
        default:
            break;
        }
    }
}
void CForceAutoCalibPage::OnDestroy()
{
	CPropertyPage::OnDestroy();

	// TODO: Add your message handler code here
	((CWireBondDlg*)(this->GetParent()->GetParent()))->EnableSystemButton();
}
