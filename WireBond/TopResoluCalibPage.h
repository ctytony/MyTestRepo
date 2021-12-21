#pragma once


// CTopResoluCalibPage dialog

class CTopResoluCalibPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopResoluCalibPage)

public:
	CTopResoluCalibPage();
	virtual ~CTopResoluCalibPage();

// Dialog Data
	enum { IDD = IDD_TAB_TOP_CALIBRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
