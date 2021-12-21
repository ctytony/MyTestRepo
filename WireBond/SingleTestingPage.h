#pragma once
#include "afxcmn.h"

// CSingleTestingPage dialog

class CSingleTestingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSingleTestingPage)

public:
	CSingleTestingPage();
	virtual ~CSingleTestingPage();

// Dialog Data
	enum { IDD = IDD_SINGLE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckOnload();
	afx_msg void OnBnClickedCheckTrack();
	afx_msg void OnBnClickedCheckOffload();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedCheckSensorValid();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnBnClickedButtonOnloadTest();
	afx_msg void OnBnClickedButtonOffloadTest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:	//variables
	BOOL m_bOnloadEnable;
	BOOL m_bTrackEnable;
	BOOL m_bForkOffloadEnable;
	BOOL m_bSensorEnable;
	long m_lIndexCycleTime;
	int m_iUph;
};
