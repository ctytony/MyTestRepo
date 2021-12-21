#pragma once

// CLoop3rdKinkPage dialog
class CLoop3rdKinkPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoop3rdKinkPage)
public:
	CLoop3rdKinkPage();
	virtual ~CLoop3rdKinkPage();
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_LOOP_3RD_KINK };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnFocusSpan2Length();
	afx_msg void OnSpan2LengthUnit();
	afx_msg void OnFocusKink3HtFactor();
	afx_msg void OnFocusKink3Smooth();
	afx_msg void OnFocusKink3WireFeedSpeed();
	afx_msg void OnFocusKink3WireBentSpeed();
	//v3.1.T407 add
	afx_msg void OnFocusKink3HtFactor2();
	afx_msg void OnFocusKink3ArcFactor();
	afx_msg void OnFocusKink3ArcDelay();
	afx_msg void OnFocusKink3Height();
	afx_msg void OnFocusKink3SpanLen2();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SaveParameter();	//V1.1.W246 add
	DECLARE_MESSAGE_MAP()
public:
    void UpdateMenuVariable(void);
private:
    short m_nSpanLen;		//v3.1.T159 rename
    int m_nSpanLenUnit;		//v3.1.T159 rename
    short m_nHtFactor;		//v3.1.T159 rename
    short m_nSmooth;		//v3.1.T159 rename
    short m_nWireFeedSpeed;	//v3.1.T159 rename
    short m_nWireBentSpeed;	//v3.1.T159 rename
	//v3.1.T407 add
	short m_nHtFactor2;
	short m_nArcFactor;
	short m_nArcDelay;
	short m_nHeight;
	short m_nSpanLen2;
};
