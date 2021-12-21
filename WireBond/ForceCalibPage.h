#pragma once

#include "MachineData.h"

// CForceCalibPage dialog
class CForceCalibPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CForceCalibPage)
public:
	CForceCalibPage();
	virtual ~CForceCalibPage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_FORCE_CALI };
public:
	afx_msg void OnBnClickedButtonResetForce();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusApplyForce();
    afx_msg void OnFocusMeasureForce();
    afx_msg void OnNext();
    afx_msg void OnStart();
	afx_msg void OnBnClickedButtonDone();
    afx_msg void OnTimer(UINT_PTR nIDEvent);	//v3.0.T478 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
	//for format display
	CString m_sCurForceFactorPa;
	CString m_sCurForceFactorPb;
	CString m_sCurForceFactorPc;
	CString m_sNewForceFactorPa;
	CString m_sNewForceFactorPb;
	CString m_sNewForceFactorPc;

	float m_fCurForceFactorPa;
    float m_fCurForceFactorPb;
    float m_fCurForceFactorPc;
    float m_fNewForceFactorPa;
    float m_fNewForceFactorPb;
    float m_fNewForceFactorPc;
    short m_nApplyForce;
    double m_dMeasureForce;
    BOOL m_bVerifyEnable;
    short m_nCaliStep;
    long m_lCtactLvl;
    double m_adCaliOutputDac[4];
    double m_adCaliMeasureForce[3];
    int m_nBkupMFlag;
	BOOL m_bBusy;			//V1.1.W145 add
private:	//functions
	void ReadForceSensor(short nStep);	//v3.0.T473 add, v3.0.T479 edit
	void Cali3PtNext();
	void Cali3PtNext2();
	void ResumeBondHead();
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W145 add
};
