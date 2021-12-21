#pragma once

// CUsgInfoDispPage dialog

class CUsgInfoDispPage : public CDialog
{
	DECLARE_DYNAMIC(CUsgInfoDispPage)
public:
	CUsgInfoDispPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUsgInfoDispPage();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_USG_INFO_DISP };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOk();
	afx_msg void OnRefresh();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	CBrush m_BkBrush;
};
