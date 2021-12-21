#pragma once


// CJoystickSpeedDlg dialog

class CJoystickSpeedDlg : public CDialog
{
	DECLARE_DYNAMIC(CJoystickSpeedDlg)

public:
	CJoystickSpeedDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJoystickSpeedDlg();

// Dialog Data
	enum { IDD = IDD_JOYSTICK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonIncrease();
    afx_msg void OnBnClickedButtonDecrease();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
