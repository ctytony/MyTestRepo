#pragma once

#include "Program.h"

// CBumpWireParaPage dialog

class CBumpWireParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBumpWireParaPage)
public:
	CBumpWireParaPage();
	virtual ~CBumpWireParaPage();
// Dialog Data
	enum { IDD = IDD_BUMP_WIRE_PARA };
public:
    virtual BOOL OnInitDialog();
    void UpdateMenuVariable(void);
    afx_msg void OnFocusBaseTime();
    afx_msg void OnFocusBondPower();
    afx_msg void OnFocusBaseForce();
    afx_msg void OnFocusSrchSpeed();
    afx_msg void OnFocusBpOffset();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy(void);
    afx_msg void OnFocusSrchTol();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnFocusStandbyPower();
    afx_msg void OnFocusPreImpactTime();
    afx_msg void OnFocusPreImpactForce();
    afx_msg void OnFocusImpactTime();
    afx_msg void OnFocusImpactPower();
    afx_msg void OnFocusImpactForce();
    afx_msg void OnFocusReleaseTime();
    afx_msg void OnFocusReleasePower();
    afx_msg void OnFocusReleaseForce();
    afx_msg void OnFocusSrchHeight();
    afx_msg void OnFocusSrchDelay();
    afx_msg void OnSrchSpeed();
    afx_msg void OnFocusPreShift();
    afx_msg void OnFocusBondShift();
    afx_msg void OnFocusTailShift();
    afx_msg void OnFocusShiftSpeed();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
    int m_iBallType;
    short m_nBaseTime;
    short m_nBasePower;
    short m_nBaseForce;
    short m_nBondPointOffset;
    int m_iSearchSpeed;	// Bump wire 2nd bond search speed
    int m_iTailBreakEnable;
    short m_nSrchTol;
    short m_nStandbyPower;
    short m_nPreCtactTime;
    short m_nPreCtactForce;
    short m_nCtactTime;
    short m_nCtactPower;
    short m_nCtactForce;
    short m_nReleaseTime;
    short m_nReleasePower;
    short m_nReleaseForce;
    short m_nSrchHeight;
    short m_nSrchDelay;
    short m_nPageSize;
    short m_nPreShift;		//Pre shift distance
    short m_nBondShift;		//Bond shift distance
    short m_nTailShift;		//Tail shift distance
    short m_nShiftSpeed;	//Shift speed in %
};
