#pragma once

#include "MachineData.h"

// CPositionCalibPage dialog

class CPositionCalibPage : public CPropertyPage
{
    DECLARE_DYNAMIC(CPositionCalibPage)
public:
    CPositionCalibPage();
    virtual ~CPositionCalibPage();
    virtual BOOL OnInitDialog();
    // Dialog Data
    enum { IDD = IDD_POS_CALI };
public:
	afx_msg void OnBnClickedButtonResetPosition();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnStart();
    afx_msg void OnNext();
    afx_msg void OnStartSrchLmt();
	afx_msg void OnBnClickedButtonDone();
	afx_msg void OnBnClickedButtonSrchDone();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()
private:	//variable
    long m_lRelaxLevel;
    float m_fPosCompKp;
    float m_fPosCompOs;
    float m_fNewCompKp;
    float m_fNewCompOs;
    long m_lNegLimitX;
    long m_lPosLimitX;
    long m_lNegLimitY;
    long m_lPosLimitY;
	short m_nCaliStep;
	long m_lFlatLevel;
	BOOL m_bBusy;				//V1.1.W145 add
private:	//function
	void ResumeBondHead();		//v3.0.T144 add
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W145 add
};
