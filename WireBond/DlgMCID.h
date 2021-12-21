#pragma once

// CDlgMCID dialog
class CDlgMCID : public CDialog
{
public:

	CDlgMCID(CWnd* pParent);   // standard constructor
	virtual ~CDlgMCID();

// Dialog Data
	enum { IDD = IDD_MCID };

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnFocusMcid();

protected:

	DECLARE_MESSAGE_MAP()
};
