#pragma once


// CR2RTestingPage dialog


class CR2RTestingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RTestingPage)

public:
	CR2RTestingPage();
	virtual ~CR2RTestingPage();

// Dialog Data
	enum { IDD = IDD_R2R_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedButtonDryRun();
	afx_msg void OnBnClickedButtonEventReset();
//	afx_msg void OnBnClickedButtonStop();
//	afx_msg void OnBnClickedButtonTempUse();



public:
//	afx_msg void OnBnClickedCheckOnload();
//	afx_msg void OnBnClickedCheckTrack();
//	afx_msg void OnBnClickedCheckOffload();
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
private:
	int m_iSpeedProfileNum;
	

public:
//	afx_msg void OnBnClickedCheckWithoutLf();
	virtual void OnOK();

	int DryRunProcess();
	afx_msg void OnBnClickedButtonEventDisable();
//	afx_msg void OnEnChangeEdit8();
private:
	long m_lStartVel;
	long m_lDriveVel;
	long m_lMaxVel;
	long m_lAcc;
	long m_lJerk;
public:
	afx_msg void OnEnSetfocusEditStartVel();
	afx_msg void OnEnSetfocusEditVel();
	afx_msg void OnEnSetfocusEditMaxVel();
	afx_msg void OnEnSetfocusEditAcc();
	afx_msg void OnEnSetfocusEditJerk();
private:
	long m_lJeckDec;
public:
	afx_msg void OnEnSetfocusEditJerkDec();
    afx_msg void OnBnClickedButtonEnable();
	afx_msg void OnBnClickedButtonStartOnloaderTest();
	afx_msg void OnBnClickedButtonStartOffloaderTest();
	afx_msg void OnBnClickedButtonStopOnloaderTest();
	afx_msg void OnBnClickedButtonStopOffloaderTest();
};
