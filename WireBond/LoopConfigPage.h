#pragma once

// CLoopConfigPage dialog
class CLoopConfigPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopConfigPage)
public:
	CLoopConfigPage();
	virtual ~CLoopConfigPage();
// Dialog Data
	enum { IDD = IDD_LOOP_CONFIG };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
    afx_msg void OnFocusNeckHeightPretrig();
    afx_msg void OnFocusNeckDistPretrig();
    afx_msg void OnFocusRevHeight2Pretrig();
    afx_msg void OnFocusRevDist2Pretrig();
    afx_msg void OnFocusLoopTopPretrig();
    afx_msg void OnFocusTailHeightWclPretrig();
    afx_msg void OnFocusLoopPeakWclPretrig();
    afx_msg void OnFocus1stSrchWclPretrig();
    afx_msg void OnFocus2ndSrchWclPretrig();
    afx_msg void OnFocusRevHeight3Pretrig();
    afx_msg void OnFocusRevDist3Pretrig();
    afx_msg void OnFocusRevHeight4Pretrig();
    afx_msg void OnFocusTwistPretrig();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
    short m_nRevHtPremove;
    short m_nRevDistPremove;
    short m_nKink2HtPremove;
    short m_nKink2DistPremove;
    short m_nLoopTopPremove;
    short m_nWclTailClosePretrig;
    short m_nWclLoopPeakClosePretrig;
    short m_nWcl1stSrchHtPretrig;
    short m_nWcl2ndSrchHtPretrig;
    short m_nKink3HtPremove;
    short m_nTwistHtPremove;
    short m_nKink3DistPremove;
    short m_nTwistFwdDistPremove;
};
