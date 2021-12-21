#pragma once

#include "BondPara.h"

// CBallBondParaPage dialog

class CBallBondParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBallBondParaPage)
public:
	CBallBondParaPage();
	virtual ~CBallBondParaPage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_BALL_PARA };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusPrePower1();
    afx_msg void OnFocusPrePower2();
    afx_msg void OnFocusCtactTime1();
    afx_msg void OnFocusCtactTime2();
    afx_msg void OnFocusCtactPower1();
    afx_msg void OnFocusCtactPower2();
    afx_msg void OnFocusCtactForce1();
    afx_msg void OnFocusCtactForce2();
    afx_msg void OnFocusBondTime1();
    afx_msg void OnFocusBondTime2();
    afx_msg void OnFocusBondPower1();
    afx_msg void OnFocusBondPower2();
    afx_msg void OnFocusBondForce1();
    afx_msg void OnFocusBondForce2();
    afx_msg void OnFocusSrchHeight1();
    afx_msg void OnFocusSrchTol1();
    afx_msg void OnFocusSrchTol2();
    afx_msg void OnFabType();
    afx_msg void OnTailBondEnable();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnFocusPreImpactTime1();
    afx_msg void OnFocusPreImpactTime2();
    afx_msg void OnFocusPreImpactForce1();
    afx_msg void OnFocusPreImpactForce2();
    afx_msg void OnFocusSrchDelay1();
    afx_msg void OnStickDetection();
    afx_msg void OnSrchSpeed1();
    afx_msg void OnSrchSpeed2();
    afx_msg void OnWireTailBreak();
    afx_msg void OnFocusRlsTime1();
    afx_msg void OnFocusRlsTime2();
    afx_msg void OnFocusRlsForce1();
    afx_msg void OnFocusRlsForce2();
    afx_msg void OnFocusRlsPower1();
    afx_msg void OnFocusRlsPower2();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy(void);
	//v3.1.T410 add
    afx_msg void OnFocusPosOffset();
    afx_msg void OnFocusSpanLenDelay();
    afx_msg void OnFocusArcSpan();
    afx_msg void OnFocusArcSpanDelay();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//function
    void UpdateMenuVariable(void);
    void TailBondMenuUpdate(void);
private:	//variable
    int m_iBallType;
    short m_nStandbyPower1;
    short m_nStandbyPower2;
    short m_nCtactTime1;
    short m_nCtactTime2;
    short m_nCtactPower1;
    short m_nCtactPower2;
    short m_nCtactForce1;
    short m_nCtactForce2;
    short m_nBondTime1;
    short m_nBondTime2;
    short m_nBondPower1;
    short m_nBondPower2;
    short m_nBondForce1;
    short m_nBondForce2;
    short m_nSearchHeight1;
    short m_nSearchTol1;
    short m_nSearchTol2;
    int m_iSearchSpeed1;
    int m_iSearchSpeed2;
    int m_iTailBreakEnable;
    int m_iFabType;
    int m_iTailBondEnable;
    short m_nPageSize;
    short m_nPreCtactTime1;
    short m_nPreCtactTime2;
    short m_nPreCtactForce1;
    short m_nPreCtactForce2;
    short m_nSrchDelay1;
    int m_iStickDetection;
    short m_nReleaseTime1;
    short m_nReleaseTime2;
    short m_nReleaseForce1;
    short m_nReleaseForce2;
    short m_nReleasePower1;
	short m_nReleasePower2;
	//v3.1.T410 add
	short m_nPosOffset;
	short m_nSpanLenDelay;
	short m_nArcSpan;
	short m_nArcSpanDelay;
};
