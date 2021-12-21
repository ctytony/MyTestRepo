#pragma once


// CBSDDebugPage dialog



class CBSDDebugPage : public CDialog
{
	DECLARE_DYNAMIC(CBSDDebugPage)

public:
	CBSDDebugPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBSDDebugPage();

// Dialog Data
	enum { IDD = IDD_DIALOG_BSD_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
    
protected:
	virtual void PostNcDestroy();
	virtual void OnCancel();

	
public:
	
	afx_msg void OnBnClickedCheckBsdebugEnable();
	
	afx_msg void OnBnClickedCheckAutoSample();


private:
	double m_DieVolt;
	double m_LeadVolt;
	int m_NSOLThreshd;
	int m_TailSrtThreshd;
	BOOL m_checkbox;
	BOOL m_AutoSampleCheck;

	
	
public:
	short m_nCounter;
	afx_msg void OnBnClickedButtonReset();
};

