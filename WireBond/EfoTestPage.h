#pragma once

// CEfoTestPage dialog

class CEfoTestPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEfoTestPage)
public:
	CEfoTestPage();
	virtual ~CEfoTestPage();
// Dialog Data
	enum { IDD = IDD_EFO_TEST };
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStartBurn();
	afx_msg void OnStopBurn();
	afx_msg void OnFocusBurnCycle();
	afx_msg void OnFocusBurnTimes();
	afx_msg void OnDryRunMode();
	afx_msg void OnTriggMode();
	afx_msg void OnGetVersion();
	afx_msg void OnGetProfile();
	afx_msg void OnDualFabEnable();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.4.W26 add
	DECLARE_MESSAGE_MAP()
private:	//variables
	int m_nCycleTime;
	bool m_bStopFiring;
	long m_lCycleCount;
	long m_lTotalCycle;
	CString m_sMsg;
	BOOL m_bDualFab;
    short m_nFailCount;
	BOOL m_bBusy;			//V1.4.W26 add
};
