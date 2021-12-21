#pragma once

// CCfgWclProfilePage dialog
class CCfgWclProfilePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCfgWclProfilePage)
public:
	CCfgWclProfilePage();
	virtual ~CCfgWclProfilePage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_WCL_PROFILE };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
    afx_msg void OnProfile();
    afx_msg void OnEditInterimForce();
    afx_msg void OnEditInterimRampTime();
    afx_msg void OnEditInterimHoldTime();
    afx_msg void OnEditHoldForce();
    afx_msg void OnEditHoldRampTime();
    afx_msg void OnFocusOpen();
    afx_msg void OnFocusClose();
    afx_msg void OnFocusHold();
    afx_msg void OnFocusInterimForce();
    afx_msg void OnFocusInterimRampTime();
    afx_msg void OnFocusInterimHoldTime();
    afx_msg void OnFocusHoldForce();
    afx_msg void OnFocusHoldRampTime();
    afx_msg void OnFocusStep2Force();
    afx_msg void OnFocusStep2RampTime();
    afx_msg void OnFocusStep2HoldTime();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
    void UpdateWclProfile(void);
    short m_nThreadWireOpenForce;
    short m_nThreadWireCloseForce;
    short m_nHoldWireCloseForce;
    int m_lProfileType;
    short m_nInterimForce;
    short m_nInterimRampTime;
    short m_nInterimHoldTime;
    short m_nHoldForce;
    short m_nHoldRampTime;
    short m_nStep2Force;
    short m_nStep2RampTime;
    short m_nStep2HoldTime;
};
