#pragma once


// CCfgTrackPage dialog

class CCfgTrackPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCfgTrackPage)

public:
	CCfgTrackPage();
	virtual ~CCfgTrackPage();

// Dialog Data
	enum { IDD = IDD_TAB_TRACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
    int m_lClampId;
    // Test module
    int m_lTestModule;
public:
    afx_msg void OnBnClickedButtonRun();
    afx_msg void OnBnClickedButtonStop();
};
