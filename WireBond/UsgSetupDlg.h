#pragma once

// CUsgSetupDlg dialog

class CUsgSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CUsgSetupDlg)
public:
	CUsgSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUsgSetupDlg();
// Dialog Data
	enum { IDD = IDD_USG_SETUP };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnCalibration();
    afx_msg void OnUsgLevel();
protected:
	virtual void PostNcDestroy();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.1.W144 add
private:
    int m_iUsgLevel;	//USG output level
	HWND m_hwndPage;	//V1.1.W144 add
public:
	afx_msg void OnBnClickedButtonCalibration2();
};
