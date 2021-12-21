#pragma once

#include "LoopPara.h"

// CLoopParaGeneral dialog
class CLoopParaGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopParaGeneral)
public:
	CLoopParaGeneral();
	virtual ~CLoopParaGeneral();
// Dialog Data
	enum { IDD = IDD_LOOP_PARA_GENERAL };
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnKinkNum();
    afx_msg void OnLoopGroup();
    afx_msg void OnSpanLengthUnit();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusLoopHt();
    afx_msg void OnFocusRevDist();
    afx_msg void OnFocusRevAngle();
    afx_msg void OnFocusLoophtCor();
    afx_msg void OnFocusSpanLength();
    afx_msg void OnFocusKink2htFactor();
    afx_msg void OnFocusLAngle();
    afx_msg void OnLoopBaseMode();
    afx_msg void OnFocusNeckHeight();
    afx_msg void OnPreKinkEnable();
    afx_msg void OnFocusPreKinkHt();
    afx_msg void OnFocusPreKinkDist();
    afx_msg void OnFocusPreKinkAngle();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnDestroy();	//V1.1.W151 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:		//variable
    int m_nLoopGroup;
private:	//variable
    int m_nKinkNum, m_nLoopBaseMode;
	int m_nKink2SpanLenUnit, m_nPreKinkEnable;
    short m_nLoopHt, m_nRevDist, m_nRevAngle;
	short m_nLoopHtCorrect, m_nKink2SpanLen, m_nKink2HtFactor;
	short m_nLAngle, m_nRevHt, m_nPreKinkHt, m_nPreKinkDist;
	short m_nPreKinkAngle, m_nPageSize;
private:	//function
	void SetLoopPara();		//v3.0.T365 add
    void UpdateLoopParaMenu(void);
};
