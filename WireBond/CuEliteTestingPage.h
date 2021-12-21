#pragma once


// CCuEliteTestingPage dialog


class CCuEliteTestingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteTestingPage)

public:
	CCuEliteTestingPage();
	virtual ~CCuEliteTestingPage();

// Dialog Data
	enum { IDD = IDD_CUELITE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonEventReset();


private:

	int m_iDryCycleSpeedId;
	CWinThread* m_pWinThread;
	CWinThread* m_pWinThreadSpeed;
	CWinThread* m_pDryWinThread;
	CWinThread* m_pWinThreadOnloader;
	CWinThread* m_pWinThreadOffloader;
	
public:
	afx_msg void OnCbnSelchangeComboDryRunClaw();
	afx_msg void OnBnClickedButtonClawDryRun();
	afx_msg void OnBnClickedButtonClawStop();

		

	afx_msg void OnBnClickedButtonStartSpeedTest();
	afx_msg void OnCbnSelchangeComboSpeedProfile();
	afx_msg void OnBnClickedButtonStopSpeedTest();
	

public:
	virtual void OnOK();
	int DryRunProcess();
	afx_msg void OnBnClickedButtonEventDisable();
    afx_msg void OnBnClickedButtonEnable();
	afx_msg void OnBnClickedButtonStartOnloaderTest();
	afx_msg void OnBnClickedButtonStopOnloaderTest();
	afx_msg void OnBnClickedButtonStartOffloaderTest();
	afx_msg void OnBnClickedButtonStopOffloaderTest();
	int m_iDryRunSpeedID;
	long m_lMoveStep;
	afx_msg void OnBnClickedButtonBackward();
	afx_msg void OnBnClickedButtonForward();
	int m_iMotorID;
	byte m_ucStepperID;
	afx_msg void OnBnClickedButtonHomeStep();
	afx_msg void OnBnClickedButtonStandBy();
};
