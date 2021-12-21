/***************************************************************
AdvVisionCfgPage.cpp : implementation file
****************************************************************/

#pragma once

#include "afxwin.h"
#include "Program.h"

// CAdvVisionCfgPage dialog

class CAdvVisionCfgPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAdvVisionCfgPage)

public:

	CAdvVisionCfgPage();
	virtual ~CAdvVisionCfgPage();

// Dialog Data
	enum { IDD = IDD_ADV_VISION_CONFIG };

    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();	//V1.1.W259 edit
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusLeadTipOffset();
    afx_msg void OnFocusIllDelay();
    afx_msg void OnFocusLeadToolOffset();
    afx_msg void OnFocusRotateAngle();
	afx_msg void OnFocusContrast();
	afx_msg void OnFocusRedGain();
	afx_msg void OnFocusBrightness();
	afx_msg void OnFocusSaturation();
	afx_msg void OnEditContrast();
	afx_msg void OnEditRedGain();
	afx_msg void OnEditBrightness();
	afx_msg void OnEditSaturation();
	afx_msg void OnPrFilter();
    afx_msg void OnFocusIplDelay();
	afx_msg void OnMultiPr();
	afx_msg void OnCamerAgc();
	afx_msg void OnFocusSerialSkipNum();	//V1.1.W262 add
	afx_msg void OnFocusSkipNumPerIndex();	//V1.1.W262 add

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    // PR rotate angle range
    short m_nRotateAngle;
    int   m_iPrRetry;
	short m_nContrast;
	short m_nBrightness;
	short m_nRedGain;
	short m_nReserved;
	int m_iPrFilter;
    int m_iAGC;
    // Inteli pad locate delay
    short m_nInteliPadLocateDelay;
    // Lead tip offset
    short m_nLeadTipOffset;
    // Inteligent lead location delay
    short m_nIllDelay;
    short m_nLeadToolOffset;
	int m_nMultiPr;
};
