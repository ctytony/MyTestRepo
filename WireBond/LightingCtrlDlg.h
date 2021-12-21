#pragma once

#include "afxcmn.h"
#include "HTimer.h"
#include "Jpeg.h"

// CLightingCtrlDlg dialog

class CLightingCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightingCtrlDlg)
public:
	CLightingCtrlDlg(bool bModeless = true, CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightingCtrlDlg();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_LIGHT_CTRL };
public:
    afx_msg void OnOk();
    afx_msg void OnEditCoaxRed();
    afx_msg void OnEditCoaxBlue();
    afx_msg void OnEditSideRed();
    afx_msg void OnEditSideBlue();
    afx_msg void OnEditThreshold();
    afx_msg void OnBinary();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusCoaxRed();
    afx_msg void OnFocusCoaxBlue();
    afx_msg void OnFocusSideRed();
    afx_msg void OnFocusSideBlue();
    afx_msg void OnFocusThreshold();
    afx_msg void OnAutoThreshold();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:		//function
	void UpdateDisp();
private:	//variable
	bool m_bModeless;
    short m_nCoaxRedLight, m_nCoaxBlueLight, m_nSideRedLight, m_nSideBlueLight;
	short m_nBinaryImage, m_nImageThres;
    CSliderCtrl m_CoaxRedSlider, m_CoaxBlueSlider;
	CSliderCtrl m_SideRedSlider, m_SideBlueSlider, m_ImageThresSlider;
	CHTimer m_Timer;
	CBrush *m_pbrRed, *m_pbrBlue, *m_pbrGreen;
};
