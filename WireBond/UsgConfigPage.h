#pragma once

// CUsgConfigPage dialog
class CUsgConfigPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CUsgConfigPage)
public:
	CUsgConfigPage();
	virtual ~CUsgConfigPage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_USG_CONFIG };
public:
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusImpact1();
    afx_msg void OnFocusImpact2();
    afx_msg void OnFocusBond1();
    afx_msg void OnFocusBond2();
    afx_msg void OnFocusRelease1();
    afx_msg void OnFocusRelease2();
    afx_msg void OnFocusFeedPower();
    afx_msg void OnFocusFeedPowerTime();
	afx_msg void OnFocusSeatPower();
	afx_msg void OnFocusSeatPowerTime();
	afx_msg void OnFocusTailPower();
	afx_msg void OnFocusTailPowerTime();
    afx_msg void OnImpactBoostMode();
    afx_msg void OnImpactAmplitude();
    afx_msg void OnImpactDuration();
    afx_msg void OnBoostMode();
    afx_msg void OnAmplitude();
    afx_msg void OnDuration();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
    short m_nImpactRampTime1;
    short m_nImpactRampTime2;
    short m_nBaseRampTime1;
    short m_nBaseRampTime2;
    short m_nReleaseRampTime1;
    short m_nReleaseRampTime2;
    short m_nFeedPower;
    short m_nFeedPowerTime;
	short m_nSeatPower;
    short m_nSeatTime;
	short m_nTailPower;    
    short m_nTailTime;
    int m_iBoostAmplitude;
    int m_iBoostDuration;
    int m_iBoostMode;
    int m_iImpactBoostMode;
    int m_iImpactBoostAmp;
    int m_iImpactBoostDuration;
};
