#pragma once

// CFnkCaliPage dialog

class CFnkCaliPage : public CDialog
{
	DECLARE_DYNAMIC(CFnkCaliPage)
public:
	CFnkCaliPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFnkCaliPage();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_FNK_CALI };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOk();
	afx_msg void OnRefresh();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnCaliPower();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	CBrush m_BkBrush;
private:
	void AutoCaliPower();	//v3.1.T1 add
};
