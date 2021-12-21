#pragma once

#include "MachineData.h"
#include "Program.h"

// CBondSafetyPage dialog
class CBondSafetyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBondSafetyPage)

public:
	CBondSafetyPage();
	virtual ~CBondSafetyPage();

// Dialog Data
	enum { IDD = IDD_BOND_SAFETY };

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusCapillaryLimit();
    afx_msg void OnDestroy();
	afx_msg void OnFocusClamperOpenTime();
	afx_msg void OnFocusBondForceLimit();
	afx_msg void OnFocusRelaySafetyDuration(); //v3.3.D148
	afx_msg void OnFocusImpLowerLimit();
	afx_msg void OnFocusImpUpperLimit();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    long m_lCapillaryWearOutLimit;
	long m_lOpenClamperDelay;		//prevent oxidation
	short m_nImpUpperLimit;
	short m_nImpLowerLimit;
	short m_nBondForceLimit;
	long m_lRelaySafetyDuration;	//v3.3.D148
};
