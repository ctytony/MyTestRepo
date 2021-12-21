#pragma once

#define TIMER1_DELAY 1	//second

// CN2h2WaitStableDlg dialog

class CN2h2WaitStableDlg : public CDialog
{
	DECLARE_DYNAMIC(CN2h2WaitStableDlg)

public:
	CN2h2WaitStableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CN2h2WaitStableDlg();
// Dialog Data
	enum { IDD = IDD_UPS_WARN};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	short m_nN2h2WaitStable;
};
