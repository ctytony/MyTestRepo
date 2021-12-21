#pragma once

// CThreadWireDlg dialog

class CThreadWireDlg : public CDialog
{
	DECLARE_DYNAMIC(CThreadWireDlg)

public:
	CThreadWireDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThreadWireDlg();

// Dialog Data
	enum { IDD = IDD_THREAD_WIRE };

protected:
    virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonUpdate();
    afx_msg void OnBnClickedButtonReset();
    afx_msg void OnBnClickedButtonResetWireUsage();
	afx_msg void OnBnClickedButtonUsg();
private:
    long m_lThreadWirePosX;
    long m_lThreadWirePosY;
    long m_lCapillaryLimit;
    long m_lCapillaryCount;
    long m_lWireUsage;
};
