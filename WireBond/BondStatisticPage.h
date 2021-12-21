#pragma once

// CBondStatisticPage dialog

class CBondStatisticPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBondStatisticPage)
public:
	CBondStatisticPage();
	virtual ~CBondStatisticPage();
// Dialog Data
	enum { IDD = IDD_BOND_STATISTIC };
public:
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnReset();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnStop();
    afx_msg void OnAutoBond();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);	//V1.1.W266 add
	afx_msg void OnDestroy();	//V1.1.W148 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void PopMCIDDlg(CPoint& pt);	//V1.1.W286 add
	DECLARE_MESSAGE_MAP()
private:	//function
	void UpdateStatDisplay(void);	//v3.0.T118 edit
private:	//variable
	CString m_sLfCycleTime;
    COleDateTime m_StartDateTime;
    CString m_szProductTime, m_szStandbyTime, m_szMTBATime, m_szMTBFTime;
    int m_lOutput;
    long m_lUph, m_lBondedWire, m_lCapillaryCount;
    short m_nAssistCount, m_nFailureCount;
	short m_nNsopCount, m_nNsolCount;
	short m_nEfoOpenCount, m_nShortTailCount;
	short m_nLeadPrErrorCount, m_nDiePrErrorCount;
	short m_nSkipDieCount, m_nMhsCount, m_nWireFeedCount;
	CFont* m_pTextFont;
};
