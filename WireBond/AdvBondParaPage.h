#pragma once

// CAdvBondParaPage dialog
class CAdvBondParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAdvBondParaPage)

public:
	CAdvBondParaPage();
	virtual ~CAdvBondParaPage();

// Dialog Data
	enum { IDD = IDD_ADV_BOND_PARA };

public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
    afx_msg void OnFocusSrchHt1();
    afx_msg void OnFocusSrchHt2();
    afx_msg void OnFocusSrchTol1();
    afx_msg void OnFocusSrchTol2();
    afx_msg void OnFocusBondSmooth();
    afx_msg void OnFocusPowerDelay1();
    afx_msg void OnFocusPowerDelay2();
    afx_msg void OnFocusTailShift();
    afx_msg void OnFocusSrchDelay1();
    afx_msg void OnFocusSrchDelay2();
    afx_msg void OnFocusPreImpactForce1();
    afx_msg void OnFocusPreImpactForce2();
    afx_msg void OnFocus1stBondShift();
    afx_msg void OnFocusShiftSpeed();
    afx_msg void OnSrchSpeed1();
    afx_msg void OnFocusPreImpactTime1();
    afx_msg void OnFocusPreImpactTime2();
    afx_msg void OnFocusPreShift2();
    afx_msg void OnSrchSpeed2();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
    short m_nSrchHeight1, m_nSrchHeight2;
	short m_nSrchTol1, m_nSrchTol2;
	short m_nPowerDelay1, m_nPowerDelay2;
	short m_nTailShift;
	short m_nSrchDelay1, m_nSrchDelay2;
    short m_nPreCtactForce1, m_nPreCtactForce2;
	short m_nShiftSpeed;
	short m_nBondShift1, m_nBondShift2;
	short m_nPreCtactTime1, m_nPreCtactTime2;
	short m_nPreShift2;
	short m_nSrchSpeedLowLimit1, m_nSrchSpeedLowLimit2;
    int m_iSrchSpeed1, m_iSrchSpeed2;
    int m_iTailBreakEnable;
};
