#pragma once

#include "BondPara.h"

// CDummyBondPage dialog

class CDummyBondPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDummyBondPage)
public:
	CDummyBondPage();
	virtual ~CDummyBondPage();
// Dialog Data
	enum { IDD = IDD_DUMMY_BOND };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusSrchSpeed();
    afx_msg void OnFocusSrchTol();
    afx_msg void OnFocusBondTime();
    afx_msg void OnFocusBondPower();
    afx_msg void OnFocusBondForce();
    afx_msg void OnTeach();
    afx_msg void OnContinue();
    afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonDone();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.1.W153 add
	DECLARE_MESSAGE_MAP()
private:
    CString m_sTeachStatus;
    short m_nSrchSpeed, m_nSrchTol;
	short m_nBondTime, m_nBondPower, m_nBondForce, m_nBondAreaStep;
	BOOL m_bBusy;	//V1.1.W153 add
	long m_lBondPosX;	//v3.0.T324 add
    long m_lBondPosY;	//v3.0.T324 add
};
