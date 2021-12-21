#pragma once

#include "HTimer.h"

// CSingleStepperPage dialog

class CSingleStepperPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSingleStepperPage)

public:
	CSingleStepperPage();
	virtual ~CSingleStepperPage();

// Dialog Data
	enum { IDD = IDD_SINGLE_STEPPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonLcawStop();
	afx_msg void OnBnClickedButtonRclawStop();
	afx_msg void OnBnClickedButtonLclawRun();
	afx_msg void OnBnClickedButtonRclawRun();
	afx_msg void OnBnClickedButtonPickerHome();
	afx_msg void OnBnClickedButtonPickerLimit();
    afx_msg void OnBnClickedButtonLeftClawHome();
    afx_msg void OnBnClickedButtonRightClawHome();
    afx_msg void OnCbnSelchangeComboClawSpeed();
    afx_msg void OnBnClickedButtonEnableStepper();
    afx_msg void OnBnClickedButtonDisableStepper();
    afx_msg void OnBnClickedButtonReset();
private:	//variables
    // Claw speed
    int m_lClawSpeed;
	bool m_bStopStepper;
	bool m_bStopRoll;
};
