#pragma once


// CSleepModeDlg dialog

class CSleepModeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSleepModeDlg)

public:
	CSleepModeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSleepModeDlg();

// Dialog Data
	enum { IDD = IDD_SLEEP_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual void OnOK();
private:
	bool m_bClickProtectionFlag;
};
