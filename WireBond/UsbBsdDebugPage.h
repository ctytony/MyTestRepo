#pragma once

// CUsbBsdDebugPage dialog
class CUsbBsdDebugPage : public CDialog
{
	DECLARE_DYNAMIC(CUsbBsdDebugPage)

public:
	CUsbBsdDebugPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUsbBsdDebugPage();

// Dialog Data
	enum { IDD = IDD_USB_BSD_DEBUG };
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBsdConn();
	afx_msg void OnBsdClose();
	afx_msg void OnClearInfo();
	afx_msg void OnFocusDac1();
	afx_msg void OnFocusDac2();
	afx_msg void OnEditDac1();
	afx_msg void OnEditDac2();
	afx_msg void OnDataRetrieve();
	afx_msg void OnBsdReset();
	afx_msg void OnBsdResult();
	afx_msg void OnBsdDebugMode();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.4.W38 add
	virtual void PostNcDestroy();
private:
	CString m_sDebugInfo;
	int m_iDebugMode;
	BYTE m_nDac1;
	BYTE m_nDac2;
	BOOL m_bBusy;		//V1.4.W38 add
};
