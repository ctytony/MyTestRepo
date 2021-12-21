#pragma once

// CDlgRework dialog

class CDlgRework : public CDialog
{
public:
	CDlgRework(CWnd* pParent, CFont* pFont);   // standard constructor
	virtual ~CDlgRework();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_REWORK };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFocusRow();
	afx_msg void OnFocusColumn();
	afx_msg void OnFocusWireNum();
	afx_msg void OnReset();
public:		//variable
	short m_nReworkRow, m_nReworkCol, m_nReworkWire;
protected:
	CFont* m_pFont;
	DECLARE_MESSAGE_MAP()
};
