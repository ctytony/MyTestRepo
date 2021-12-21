
#pragma once


// CDlgTopWhSetUpPage dialog

class CR2RWhSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RWhSetupPage)

public:
	CR2RWhSetupPage();   // standard constructor
	virtual ~CR2RWhSetupPage();

// Dialog Data
	enum { IDD = IDD_R2R_WH_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
private:
	long m_lIndexLogicZero;
	long m_lIndexRightLimit;
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStartTchTable();
	afx_msg void OnBnClickedButtonNext();



    afx_msg void OnEnSetfocusEditMagClamperPosition();
    afx_msg void OnEnChangeEditMagClamperPosition();
    afx_msg void OnEnChangeEditMagClamperLimit();


private:
	double m_dWhTbX;
	double m_dWhTbY;


protected:
	virtual void OnCancel();
};
