#pragma once

// CWireGroupDlg dialog
class CWireGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CWireGroupDlg)

public:

	CWireGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWireGroupDlg();

// Dialog Data
	enum { IDD = IDD_WIRE_GROUP };

    afx_msg void OnConfirm();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnFocusStartWire();
    afx_msg void OnFocusEndWire();
    afx_msg void OnFocusGroup();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

    short m_nStartWire;
    short m_nEndWire;
    short m_nGroupNum;

    CBrush m_aGroupBrush[15];
};
