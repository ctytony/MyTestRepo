#pragma once
// CR2RIndexerSetupPage dialog



class CR2RIndexerSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RIndexerSetupPage)

public:
	CR2RIndexerSetupPage();
	virtual ~CR2RIndexerSetupPage();

// Dialog Data
	enum { IDD = IDD_R2R_INDEXER_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	virtual BOOL OnInitDialog();

	virtual BOOL OnSetActive();
	
private:
	
	short m_nStep;

	short m_nIndexId;

public:
	afx_msg void OnBnClickedButtonStart();

	afx_msg void OnBnClickedButtonNext();

	
	afx_msg void OnBnClickedButtonStopTch();
	afx_msg void OnBnClickedButtonResetOffset();

	
private:
	int m_iIndexTchMode;
public:
	afx_msg void OnCbnSelchangeComboTchIndexMode();
	afx_msg void OnLbnDblclkListIndexErrorShow();
};
