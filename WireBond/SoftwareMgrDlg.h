#pragma once

// CSoftwareMgrDlg dialog
class CSoftwareMgrDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftwareMgrDlg)

public:

	CSoftwareMgrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoftwareMgrDlg();

// Dialog Data
	enum { IDD = IDD_SOFTWARE_MGR };

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.1.W163 add
	virtual void PostNcDestroy();						//V1.1.W163 add

	DECLARE_MESSAGE_MAP()

private:	//variables

	CString m_szUsgVersion, m_szBsdVersion, m_szServoVersion, m_szEfoVersion;
};
