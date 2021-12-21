#pragma once

// CTopTestingPage dialog
class CTopTestingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopTestingPage)

public:

	CTopTestingPage();
	virtual ~CTopTestingPage();

// Dialog Data
	enum { IDD = IDD_TOP_TEST };

	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEventReset();
	afx_msg void OnDryRunClaw();
	afx_msg void OnClawDryRun();
	afx_msg void OnClawStop();
	afx_msg void OnStartSpeedTest();
	afx_msg void OnSpeedProfile();
	afx_msg void OnStopSpeedTest();
	afx_msg void OnEventDisable();
	afx_msg void OnFocusStartVel();
	afx_msg void OnFocusVel();
	afx_msg void OnFocusMaxVel();
	afx_msg void OnFocusAcc();
	afx_msg void OnFocusJerk();
	afx_msg void OnFocusJerkDec();
    afx_msg void OnEnable();
	afx_msg void OnStartOnloaderTest();
	afx_msg void OnStartOffloaderTest();
	afx_msg void OnStopOnloaderTest();
	afx_msg void OnStopOffloaderTest();
	afx_msg void OnDestroy();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.4.W23 add

	DECLARE_MESSAGE_MAP()

private:

	int m_iDryCycleSpeedId;
	int m_iSpeedProfileNum;
	long m_lStartVel;
	long m_lDriveVel;
	long m_lMaxVel;
	long m_lAcc;
	long m_lJerk;
	long m_lJeckDec;

	bool m_bTopOffloaderTestStop;
	bool m_bTopOnloaderTestStop;

	BOOL m_bBusy;	//V1.4.W23 add

	static UINT TopIndexTest_ThreadProc(LPVOID pParam);
	static UINT TopOnloaderTest_ThreadProc(LPVOID pParam);
	static UINT TopOffloaderTest_ThreadProc(LPVOID pParam);
	static UINT TopGripperDryRun_ThreadProc(LPVOID pParam);
};
