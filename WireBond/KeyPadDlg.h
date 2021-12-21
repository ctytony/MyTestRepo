#pragma once

#include "Jpeg.h"		//V1.1.W126 add

// CKeyPadDlg dialog

class CKeyPadDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyPadDlg)
public:
	CKeyPadDlg(CString sRef = _T(""), long lLowLimit = 0, long lUpLimit = 0, bool bPassword = false, CWnd* pParent = NULL);   // standard constructor
	virtual ~CKeyPadDlg();
    virtual BOOL OnInitDialog();
	enum { IDD = IDD_KEY_PAD };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonLine();
	afx_msg void OnNumbers(UINT nID);
	afx_msg void OnAlphas(UINT nID);
    afx_msg void OnBnClickedButtonMinus();
    afx_msg void OnBnClickedButtonDot();
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnExpand();
	afx_msg void OnStnClickedStaticRef();
    afx_msg void OnEnChangeEditKeyin();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);		//V1.1.W126 add
	afx_msg LRESULT OnNcHitTest(CPoint point);	//V1.1.W126 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	bool m_bCancelSelected;
private:
    // Key in value up limit
    CString m_sUpLimit;
    bool m_bExpandState;
    // Key in value lower limit
    CString m_sLowLimit;
    // Keyin string
    CString m_sKeyinString;
    // Display string
    CString m_sDisplayString;
	CString m_sRef;
    // Keyin value low limit
    long m_lLowLimit;
    // Keyin value upper limit
    long m_lUpLimit;
    // Keyin password
    bool m_bKeyinPassword;
};
