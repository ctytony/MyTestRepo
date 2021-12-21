#pragma once

// CLoopLandingPage dialog
class CLoopLandingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopLandingPage)
public:
	CLoopLandingPage();
	virtual ~CLoopLandingPage();
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_LOOP_LAND };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SaveParameter();	//V1.1.W246 add
	DECLARE_MESSAGE_MAP()
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
    afx_msg void OnFocusTrajFactor();
    afx_msg void OnTrajMode();
    afx_msg void OnLoopTopMode();
    afx_msg void OnFocusCtactOffset();
    afx_msg void OnFocusPullDist();
    afx_msg void OnFocusTrajectorySpeed();
    afx_msg void OnFocusPullSpeed();
public:
    void UpdateMenuVariable(void);
private:
    short m_nTrajFactor;
    int m_nTrajMode;
    int m_nLoopTopMode;
    short m_nCtactLvlOffset;
    short m_nPullDist;
    short m_nTrajSpeed;
    short m_nPullSpeed;
};
