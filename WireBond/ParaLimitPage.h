#pragma once
#include "afxwin.h"

// CParaLimitPage dialog

class CParaLimitPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CParaLimitPage)

public:
	CParaLimitPage();
	virtual ~CParaLimitPage();
    virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_PARA_LIMIT };
public:
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusPrepowerL();
    afx_msg void OnFocusPrepowerU();
    afx_msg void OnFocusCompatimeL();
    afx_msg void OnFocusCompatimeU();
    afx_msg void OnFocusCompowerL();
    afx_msg void OnFocusCompowerU();
    afx_msg void OnFocusCompapresL();
    afx_msg void OnFocusCompapresU();
    afx_msg void OnFocusBondtimeL();
    afx_msg void OnFocusBondtimeU();
    afx_msg void OnFocusBondpwrL();
    afx_msg void OnFocusBondpwrU();
    afx_msg void OnFocusBondpresL();
    afx_msg void OnFocusBondpresU();
    afx_msg void OnFocusPwraveindexL();
    afx_msg void OnFocusPwraveindexU();
    afx_msg void OnFocusPresaveIndexL();
    afx_msg void OnFocusPresaveIndexU();
    afx_msg void OnFocusUsgdelayL();
    afx_msg void OnFocusUsgdelayU();
    afx_msg void OnFocusSearchhighL();
    afx_msg void OnFocusSearchhighU();
    afx_msg void OnFocusSearchthreshL();
    afx_msg void OnFocusSearchthreshU();
    afx_msg void OnLimit();
    afx_msg void OnFocusPreImpactForceLow();
    afx_msg void OnFocusPreImpactForceHigh();
    afx_msg void OnFocusPreImpactTimeLow();
    afx_msg void OnFocusPreImpactTimeHigh();
    afx_msg void OnFocusSparkDelayLow();
    afx_msg void OnFocusSparkDelayHigh();
    afx_msg void OnFocusTailLengthLow();
    afx_msg void OnFocusTailLengthHigh();
    afx_msg void OnFocusPostDampForceLow();
    afx_msg void OnFocusPostDampForceHigh();
    afx_msg void OnFocusPostDampPowerLow();		//v3.1.T564 add
    afx_msg void OnFocusPostDampPowerHigh();	//v3.1.T564 add
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnFocusTailLength2Low();
    afx_msg void OnFocusTailLength2High();
    afx_msg void OnFocusFireLevelL();
    afx_msg void OnFocusFireLevelH();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
	short m_nScaleFactor;		//v1.6.T18 add
    short m_nStandbyPower1;
    short m_nStandbyPower2;
    short m_nCtactTime1;
    short m_nCtactTime2;
    short m_nCtactPower1;
    short m_nCtactPower2;
    short m_nCtactForce1;
    short m_nCtactForce2;
    short m_nBaseTime1;
    short m_nBaseTime2;
    short m_dBasePower1;
    short m_dBasePower2;
    short m_nBaseForce1;
    short m_nBaseForce2;
    short m_nPowerFactor1;
    short m_nPowerFactor2;
    short m_nForceFactor1;
    short m_nForceFactor2;
    short m_nPowerDelay1;
    short m_nPowerDelay2;
    short m_nSrchHeight1;
    short m_nSrchHeight2;
    short m_nSrchThresh1;
    short m_nSrchThresh2;
	short m_nRlsForceL;			//v3.1.T563 rename
    short m_nRlsForceH;			//v3.1.T563 rename
	short m_nRlsPowerL;			//v3.1.T564 add
    short m_nRlsPowerH;			//v3.1.T564 add
    int m_SelectStep;
	short m_nPowerH;
    short m_nPreCtactForceL;
    short m_nPreCtactForceH;
    short m_nPreCtactTimeL;
    short m_nPreCtactTimeH;
    // Spark delay low limit
    short m_nSparkDelayL;
    // Spark delay high limit
    short m_nSparkDelayH;
    // Tail length low limit
    short m_nTailLengthL;
    // Tail length high limit
    short m_nTailLengthH;
    // Page size
    short m_nPageSize;
    // Tail length 2 Low limit
    short m_nTailLength2L;
    // Tail length 2 upper limit
    short m_nTailLength2H;
    // Fire level low limit
    short m_nFireLevelL;
    // Fire level upper limit
    short m_nFireLevelH;
	int m_nSrchSpeedL;
	int m_nSrchSpeedH;
private:	//functions
	// Show menu item for ball and wedge bond
    void ShowMenuItem(void);
};
