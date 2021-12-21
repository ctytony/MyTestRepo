#pragma once


// CUPSWarnPage dialog

class CUPSWarnPage : public CDialog
{
	DECLARE_DYNAMIC(CUPSWarnPage)

public:
	CUPSWarnPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUPSWarnPage();

// Dialog Data
	enum { IDD = IDD_UPS_WARN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
