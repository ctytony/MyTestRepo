#pragma once

// CCfgBondPage dialog
class CCfgBondPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCfgBondPage)
public:
	CCfgBondPage();
	virtual ~CCfgBondPage();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_CONFIG_BOND };
public:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFocusTailBreakDist();
	afx_msg void OnFocusT1();
	afx_msg void OnFocusT2();
	afx_msg void OnFocusCapiPowerOs1();	//v1.1.288.T15 edit
	afx_msg void OnFocusCapiPowerOs2();	//v1.1.288.T15 add
	afx_msg void OnFocusPwrSlope();
	afx_msg void OnFocusPwrOffset();
	afx_msg void OnFocusPowerOs1();
	afx_msg void OnFocusPowerOs2();
	afx_msg void OnFocusForceOs1();
	afx_msg void OnFocusForceOs2();
	afx_msg void OnFocusTimeOs1();
	afx_msg void OnFocusTimeOs2();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
    short m_nPowerOffset1;
    short m_nPowerOffset2;
    short m_nForceOffset1;
    short m_nForceOffset2;
    short m_nTimeOffset1;
    short m_nTimeOffset2;
    short m_nPowerCompSlope;
    short m_nPowerCompOs;
    short m_nTailBreakDist;
    int m_iTailBreakSpeed;
    long m_lCapiWearOutCount1;
    long m_lCapiWearOutCount2;
    double m_dCapiWearOutPowerOs1;	//v1.1.288.T15 edit
    double m_dCapiWearOutPowerOs2;	//v1.1.288.T15 add
};
