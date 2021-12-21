#pragma once

// CLoop1stKinkPage dialog

class CLoop1stKinkPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoop1stKinkPage)
public:
	CLoop1stKinkPage();
	virtual ~CLoop1stKinkPage();
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_LOOP_1ST_KINK };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnLoopBaseMode();
    afx_msg void OnDestroy();
    afx_msg void OnFocusNeckHeight();
    afx_msg void OnFocusRevDist();
    afx_msg void OnFocusRevAngle();
    afx_msg void OnPreKinkEnable();
    afx_msg void OnFocusPreKinkHt();
    afx_msg void OnFocusPreKinkDist();
    afx_msg void OnFocusPreKinkAngle();
    afx_msg void OnFocusKink1WireFeedSpeed();
    afx_msg void OnFocusKink1WireBentSpeed();
    afx_msg void OnFocusPreKinkWireFeedSpeed();
    afx_msg void OnFocusPreKinkWireBentSpeed();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	//V1.1.W246 add
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	//V1.1.W246 add
	afx_msg void DisableScroll();	//v3.1.L370 add
	//v3.1.T407 add
    afx_msg void OnFocusRevHtFactor2();
    afx_msg void OnFocusRevArcFactor();
    afx_msg void OnFocusRevArcDelay();
    afx_msg void OnFocusPreKinkHtFactor2();
    afx_msg void OnFocusPreKinkArcFactor();
    afx_msg void OnFocusPreKinkArcDelay();
	//v3.1.T575 add
    afx_msg void OnFocusRevSpanLen2();
    afx_msg void OnFocusPreKinkSpanLen2();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SaveParameter();	//V1.1.W246 add
	DECLARE_MESSAGE_MAP()
private:	//function
	void UpdateMenuVariable(void);
private:	//variable
    int m_nLoopBaseMode;
    short m_nRevHt;
    short m_nRevDist;
    short m_nRevAngle;
    int m_nPreKinkEnable;
    short m_nPreKinkHt;
    short m_nPreKinkDist;
    short m_nPreKinkAngle;
    short m_nWireFeedSpeed;	//v3.1.T161 rename
    short m_nWireBentSpeed;	//v3.1.T161 rename
    short m_nPreKinkWireFeedSpeed;
    short m_nPreKinkWireBentSpeed;
    short m_nPageSize;		// Menu page size
	//v3.1.T407 add
	short m_nRevHtFactor2;
	short m_nRevArcFactor;
	short m_nRevArcDelay;
	short m_nPreKinkHtFactor2;
	short m_nPreKinkArcFactor;
	short m_nPreKinkArcDelay;
	//v3.1.T575 add
	short m_nRevSpanLen2;
	short m_nPreKinkSpanLen2;
};
