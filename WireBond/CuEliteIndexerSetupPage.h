#pragma once

enum enCuEliteIndexTchMode 
{
	MODE_CUELITE_MANUAL,
	MODE_CUELITE_AUTO,
};

// CCuEliteIndexerSetupPage dialog

class CCuEliteIndexerSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteIndexerSetupPage)

public:
	CCuEliteIndexerSetupPage();
	virtual ~CCuEliteIndexerSetupPage();

// Dialog Data
	enum { IDD = IDD_TOP_INDEXER_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	virtual BOOL OnInitDialog();

	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	
private:
	
	short m_nStep;

	short m_nIndexId;

public:
	afx_msg void OnBnClickedButtonStart();

	afx_msg void OnBnClickedButtonNext();

	
	afx_msg void OnBnClickedButtonStopTch();
private:
	int m_iIndexTchMode;
public:
	afx_msg void OnCbnSelchangeComboTchIndexMode();
	afx_msg void OnLbnDblclkListIndexErrorShow();
};
