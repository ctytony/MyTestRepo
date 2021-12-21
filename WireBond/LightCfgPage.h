#pragma once
#include "datastruct.h"

// CLightCfgPage dialog
class CLightCfgPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLightCfgPage)

public:

	CLightCfgPage();
	virtual ~CLightCfgPage();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();	//V1.1.W259 edit

// Dialog Data
	enum { IDD = IDD_LIGHT_CONFIG };

    afx_msg void OnFocusDieCoblue();
    afx_msg void OnFocusDieCored();
    afx_msg void OnFocusDieSideblue();
    afx_msg void OnFocusDieSidered();
    afx_msg void OnFocusDieBlack();
    afx_msg void OnFocusLeadCoblue();
    afx_msg void OnFocusLeadCored();
    afx_msg void OnFocusLeadSideblue();
    afx_msg void OnFocusLeadSidered();
    afx_msg void OnFocusLeadBlack();
	afx_msg void OnFocusCoaxRed();
	afx_msg void OnFocusCoaxBlue();
	afx_msg void OnFocusSideRed();
	afx_msg void OnFocusSideBlue();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

    short m_Die_Coxblue;
    short m_Die_Coxred;
    short m_Die_Sideblue;
    short m_Die_Sidered;
    short m_Die_black;
    short m_Lead_Coxblue;
    short m_Lead_Coxred;
    short m_Lead_Sideblue;
    short m_Lead_Sidered;
    short m_Lead_black;

	// Coax red light for idle
	short m_nCoaxRed;
	// Coax blue light of idle
	short m_nCoaxBlue;
	// Side red light of idle
	short m_nSideRed;
	// Side blue light of idle
	short m_nSideBlue;
};
