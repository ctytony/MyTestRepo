#pragma once

// CInitDlg dialog
class CInitDlg : public CDialog
{
public:
	CInitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitDlg();

// Dialog Data
	enum { IDD = IDD_INIT };

public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
protected:
	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();
public:
    void ShowInitMsg(CString sMsg);		//v3.0.T137 add
    void ShowInitMsg(LPTSTR psMsg);		//v1.6.T7 add
    void ShowInfoMsg(LPTSTR psMsg);		//v1.6.T7 add
private:
	CString m_sInit, m_sInfo;
};
