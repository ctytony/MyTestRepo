#pragma once

// CScrubParaPage dialog
class CScrubParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CScrubParaPage)

public:

	CScrubParaPage();
	virtual ~CScrubParaPage();

// Dialog Data
	enum { IDD = IDD_SCRUB_PAGE };

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnDestroy();
    afx_msg void OnFocusScrubPosOffset1();
    afx_msg void OnFocusScrubPosOffset2();
    afx_msg void OnFocusScrubCycle();
    afx_msg void OnFocusAmplitude();
    afx_msg void OnFocusScrubSpeed();
    afx_msg void OnFocus2ndScrubForce();
    afx_msg void OnFocus2ndScrubPower();
	afx_msg void On2ndScrub();
    afx_msg void On1stScrub();
    afx_msg void OnFocus1stScrubCycle();
    afx_msg void OnFocus1stScrubAmp();
    afx_msg void On1stScrubDir();
    afx_msg void OnFocus1stScrubSpeed();
    afx_msg void OnFocus1stScrubForce();
    afx_msg void OnFocus1stScrubPower();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

    int m_i2ndBondScrubEnable,m_i2ndScrubDirection,m_i1stBondScrubEnable,m_i1stScrubDirection;

    short m_nScrubPosOffset1,m_nScrubPosOffset2,m_n2ndScrubCycle,m_n2ndScrubAmplitude,m_n2ndScrubSpeed,m_n2ndScrubForce;
	short m_n2ndScrubPower,m_n1stScrubCycle,m_n1stScrubAmplitude,m_n1stScrubSpeed,m_n1stScrubForce,m_n1stScrubPower;
};
