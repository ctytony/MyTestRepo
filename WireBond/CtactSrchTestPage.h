#pragma once

// CCtactSrchTestPage dialog
class CCtactSrchTestPage : public CDialog
{
	DECLARE_DYNAMIC(CCtactSrchTestPage)
public:
	CCtactSrchTestPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCtactSrchTestPage();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_CTACT_SRCH_TEST };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCtact();
	afx_msg void OnFocusCtactTol();
	afx_msg void OnIndex();
	afx_msg void OnCtactSrchTestSpeed();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W39 add
private:
	int m_nSrchSpeed;
	short m_nSrchTol;
	CString m_szString;
	BOOL m_bBusy;	//V1.4.W39 add
};
