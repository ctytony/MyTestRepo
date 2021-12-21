#pragma once

// CDlgSpider dialog
class CDlgSpider : public CDialog
{
public:
	CDlgSpider();   // standard constructor
	virtual ~CDlgSpider();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// Dialog Data
	enum { IDD = IDD_SPIDER };
protected:
	DECLARE_MESSAGE_MAP()

	CBrush m_brush;
	BYTE m_nSize;
	SHORT m_nPos;
};
