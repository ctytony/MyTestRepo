#pragma once

#include "Sensor.h"
#include "DcMotor.h"
#include "VEOffload.h"
#include "HTimer.h"

// CVledElitePage dialog

class CVledElitePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CVledElitePage)

public:
	CVledElitePage();
	virtual ~CVledElitePage();

// Dialog Data
	enum { IDD = IDD_VLED_ELITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnBnClickedButtonInjectHome();
	afx_msg void OnBnClickedButtonInjectStop();
	afx_msg void OnBnClickedButtonInjectPos1();
	afx_msg void OnBnClickedButtonInjectPos2();
	afx_msg void OnEnSetfocusEditClampOpenWidth();
	afx_msg void OnEnSetfocusEditDbCheckPos();
	afx_msg void OnEnSetfocusEditInjectPos();
	afx_msg void OnBnClickedButtonClampHome();
	afx_msg void OnBnClickedButtonClampOpen();
	afx_msg void OnBnClickedButtonClampClose();
	afx_msg void OnLfKickOutPos();
	afx_msg void OnMagSupportPosY();
	afx_msg void OnMagSupportPosZ();
	afx_msg void OnMagDropPosZ();
	afx_msg void On1stSlotPosY();
	afx_msg void On1stSlotPosZ();
	afx_msg void OnMagKickerPosY();
	afx_msg void OnMagKickerPosZ();
	afx_msg void OnPitchWidth();
	afx_msg void OnPitchNum();
	afx_msg void OnKickerHome();
	afx_msg void OnKickerForward();
	afx_msg void OnKickerStop();
	afx_msg void OnEnSetfocusEditClampClose();
	afx_msg void OnBnClickedButtonClampStop();

private:	//variables
	long m_lDbCheckPos;
	long m_lInjectPos;
	long m_lOpenPos;
	long m_lClosePos;
	VE_OFFLOAD_RCD m_vor;
	CSensor m_MagKickerHomeSensor;
	CSensor m_MagKickerLimitSensor;
	CDcMotor m_MagKicker;

	static DWORD WINAPI StopProc(LPVOID lpVoid);
};
