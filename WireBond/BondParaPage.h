#pragma once

#include "Program.h"
#include "BondPara.h"

// CBondParaPage dialog

class CBondParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBondParaPage)
public:
	CBondParaPage();
	virtual ~CBondParaPage();
// Dialog Data
	enum { IDD = IDD_BOND_PARA };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusParaStandbyPower1();
    afx_msg void OnFocusParaStandbyPower2();
    afx_msg void OnFocusParaCtactTime1();
    afx_msg void OnFocusParaCtactTime2();
    afx_msg void OnFocusParaCtactPower1();
    afx_msg void OnFocusParaCtactPower2();
    afx_msg void OnFocusParaCtactForce1();
    afx_msg void OnFocusParaCtactForce2();
    afx_msg void OnFocusParaBaseTime1();
    afx_msg void OnFocusParaBaseTime2();
    afx_msg void OnFocusParaBasePower1();
    afx_msg void OnFocusParaBasePower2();
    afx_msg void OnFocusParaBaseForce1();
    afx_msg void OnFocusParaBaseForce2();
    afx_msg void OnFocusParaRlsTime1();
    afx_msg void OnFocusParaRlsTime2();
    afx_msg void OnFocusParaRlsPower1();
    afx_msg void OnFocusParaRlsPower2();
    afx_msg void OnFocusParaRlsForce1();
    afx_msg void OnFocusParaRlsForce2();
    afx_msg void OnFocusParaPowerFactor();
    afx_msg void OnFocusParaForceFactor();
    afx_msg void OnDestroy();	//V1.1.W151 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variable
    short m_nCtactTime1, m_nCtactTime2, m_nCtactPower1, m_nCtactPower2;
	short m_nCtactForce1, m_nCtactForce2, m_nBaseTime1, m_nBaseTime2;
	short m_nBasePower1, m_nBasePower2, m_nBaseForce1, m_nBaseForce2;
	short m_nRlsTime1, m_nRlsTime2, m_nRlsPower1, m_nRlsPower2;
	short m_nRlsForce1, m_nRlsForce2, m_nPowerFactor, m_nForceFactor;
	short m_nStandbyPower1, m_nStandbyPower2;
private:	//function
	void SetBondPara();
};
