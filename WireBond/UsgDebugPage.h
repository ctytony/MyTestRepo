#pragma once

// CUsgDebugPage dialog
class CUsgDebugPage : public CDialog
{
	DECLARE_DYNAMIC(CUsgDebugPage)

public:

	CUsgDebugPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUsgDebugPage();

// Dialog Data
	enum { IDD = IDD_USG_DEBUG };

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRevData();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.4.W41 add

	virtual void PostNcDestroy();

private:

	CString m_szString;

	BOOL m_bBusy;	//V1.4.W41 add
};
