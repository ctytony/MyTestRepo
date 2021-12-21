#pragma once

#include "TopIndex.h"

enum EN_TEACH_MODE
{
	TCHMODE_MANUAL,
	TCHMODE_AUTO,
};

// CTopIndexerSetupPage dialog
class CTopIndexerSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopIndexerSetupPage)
public:
	CTopIndexerSetupPage();
	virtual ~CTopIndexerSetupPage();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_TOP_INDEXER_SETUP };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStart();
	afx_msg void OnNext();
	afx_msg void OnStop();
	afx_msg void OnIndexMode();
	afx_msg void OnIndexErrorShow();
	afx_msg void OnFocusClampCol();		//v3.0.T86 add
	afx_msg void OnBnClickedButtonDone();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W158 add
	DECLARE_MESSAGE_MAP()
private:
	int m_nWndUnitNum;	//v3.0.T86 add
	short m_nStep, m_nIndexID;
	int m_nTeachMode;
	BOOL m_bBusy;	//V1.1.W158 add
};
