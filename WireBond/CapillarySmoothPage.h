#pragma once

// CCapillarySmoothPage dialog
class CCapillarySmoothPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCapillarySmoothPage)

public:
	CCapillarySmoothPage();
	virtual ~CCapillarySmoothPage();

// Dialog Data
	enum { IDD = IDD_CAP_SMOOTH };

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnFocusForce();
    afx_msg void OnFocusPower();
    afx_msg void OnFocusAmplitude();
    afx_msg void OnFocusCycle();
    afx_msg void OnStart();
    afx_msg void OnNext();
    afx_msg void OnSrchSpeed();
    afx_msg void OnFocusThreshold();
    afx_msg void OnStartClean();
    afx_msg void OnCleanNext();
    afx_msg void OnDestroy();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

    // Capillary smooth force
    short m_nSmoothForce;
    // Capillary smooth power
    float m_fSmoothPwr;
    // Capillary smooth amplitude
    short m_nSmoothAmplitude;
    // Capillary smooth cycle
    short m_nSmoothCycle;
    // Search speed
    int m_iSrchSpeed;
    // Search threshold
    short m_nSrchThreshold;
};
