#pragma once

#include "datastruct.h"

// CWclCalibPage dialog
class CWclCalibPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWclCalibPage)
public:
	CWclCalibPage();
	virtual ~CWclCalibPage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_WCL_CALI };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnForceCalib();
    afx_msg void OnForceVerify();
    afx_msg void OnFocusApplyForce();
    afx_msg void OnNext();
    afx_msg void OnComplete();
    afx_msg void OnFocusMeasureForce();
	afx_msg void OnBnClickedButtonResetToDefault();
	afx_msg void OnBnClickedButtonDone();
	afx_msg void OnBnClickedButtonVerifyDone();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
    float m_fCurForceFactorKf;
    float m_fCurForceFactorIo;
    float m_fNewForceFactorKf;
    float m_fNewForceFactorIo;
    short m_nApplyForce;
    double m_dMeasureForce;
    short m_nCaliStep;
    ST_WCL_PROFILE m_stBkupWclProfile;
    double m_adMeasureForce[2];
	BOOL m_bBusy;	//V1.1.W145 add
private:
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W145 add
};
