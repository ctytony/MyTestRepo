#pragma once

#include "StepperControl.h"

// CTestMotionTimePage dialog

class CTestMotionTimePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTestMotionTimePage)

public:
	CTestMotionTimePage();
	virtual ~CTestMotionTimePage();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_MOTION_DEBUG };

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStart();
	afx_msg void OnMotor();
	afx_msg void OnFocusDist();
	afx_msg void OnFocusVelocity();
	afx_msg void OnFocusMaxVel();
	afx_msg void OnFocusAcc();
	afx_msg void OnFocusJerk();
	afx_msg void OnClear();
	afx_msg void OnDestroy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.4.W43 add
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bCurveType;
	BOOL m_bMoionMode;
	BOOL m_bBusy;	//V1.4.W43 add
	int m_iMotorId;
	long m_lMoveDist;
	long m_lVelocity;
	long m_lMaxVel;
	long m_lAcc;
	long m_lJerk;
	BYTE m_bStepperAddr;
	STEPPER_SPEED_SETTING stMotorSpeedProfile;
};
