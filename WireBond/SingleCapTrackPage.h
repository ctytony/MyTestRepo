#pragma once

#include "HTimer.h"

// CSingleCapTrackPage dialog

class CSingleCapTrackPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSingleCapTrackPage)

public:
	CSingleCapTrackPage();
	virtual ~CSingleCapTrackPage();

// Dialog Data
	enum { IDD = IDD_SINGLE_TRACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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
	afx_msg void OnBnClickedButtonKickeHome();
	afx_msg void OnBnClickedButtonKickerForward();
	afx_msg void OnBnClickedButtonKickerBk();
	afx_msg void OnBnClickedButtonKickerStop();
	afx_msg void OnEnSetfocusEditKickerPushDist();
	afx_msg void OnEnSetfocusEditClampClose();
    afx_msg void OnEnSetfocusEditPushPos2();
    afx_msg void OnBnClickedButtonkickForward2();
	afx_msg void OnBnClickedButtonClampStop();
private:	//variables
    // Kicker push limit position 2
    long m_lKickerPushPos2;
	long m_lDbCheckPos;
	long m_lInjectPos;
	long m_lOpenPos;
	long m_lClosePos;
	long m_lKickerLimit;
	CHTimer m_Timer;
};
