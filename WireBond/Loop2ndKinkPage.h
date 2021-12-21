#pragma once

// CLoop2ndKinkPage dialog
class CLoop2ndKinkPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoop2ndKinkPage)
public:
	CLoop2ndKinkPage();
	virtual ~CLoop2ndKinkPage();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_LOOP_2ND_KINK };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFocusSpanLength();
	afx_msg void OnSpanLengthUnit();
	afx_msg void OnFocusKink2htFactor();
	afx_msg void OnDestroy();
	afx_msg void OnFocusKink2WireFeedSpeed();
	afx_msg void OnFocusKink2WireBentSpeed();
	//v3.1.T407 add
	afx_msg void OnFocusKink2HtFactor2();
	afx_msg void OnFocusKink2ArcFactor();
	afx_msg void OnFocusKink2ArcDelay();
	afx_msg void OnFocusKink2Height();
	afx_msg void OnFocusKink2SpanLen2();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SaveParameter();	//V1.1.W246 add
	DECLARE_MESSAGE_MAP()
public:
	void UpdateMenuVariable(void);
private:
    short m_nSpanLen;		//v3.1.T160 rename
    int m_nSpanLenUnit;		//v3.1.T160 rename
    short m_nHtFactor;		//v3.1.T160 rename
    short m_nWireFeedSpeed;	//v3.1.T160 rename
    short m_nWireBentSpeed;	//v3.1.T160 rename
	//v3.1.T407 add
	short m_nHtFactor2;
	short m_nArcFactor;
	short m_nArcDelay;
	short m_nHeight;
	short m_nSpanLen2;
};
