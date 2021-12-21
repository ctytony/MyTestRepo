#pragma once

// CServoAutoTunePage dialog
class CServoAutoTunePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CServoAutoTunePage)

public:

	CServoAutoTunePage();
	virtual ~CServoAutoTunePage();

// Dialog Data
	enum { IDD = IDD_SERVO_AUTOTUNE };

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnTune();
    afx_msg void OnXyVerify();
    afx_msg void OnStop();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.5.W67 add

	DECLARE_MESSAGE_MAP()

private:

	BOOL m_bBusy;	//V1.5.W67 add
};
