#pragma once

#include "BondPara.h"

// CFireLevelDlg dialog

class CFireLevelPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CFireLevelPage)
public:
	CFireLevelPage();
	virtual ~CFireLevelPage();
// Dialog Data
	enum { IDD = IDD_FIRE_LEVEL };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnUp();
    afx_msg void OnDown();
    afx_msg void OnStart();
    afx_msg void OnConfirm();
    afx_msg void OnCancel();
    afx_msg void OnEditNewLvl();
    afx_msg void OnFocusTailLength();
    afx_msg void OnFocusCurLvl();
    afx_msg void OnFocusNewLvl();
    afx_msg void OnFocusStep();
    afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonDone();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.1.W153 add
	DECLARE_MESSAGE_MAP()
private:
    int m_nCurFireLvl;	//v3.0.T455 rename
    int m_nNewFireLvl;	//v3.0.T455 rename
    int m_nTailLen;		//v3.0.T455 rename
    bool m_bAllowAdjust;	//allow adjust bond head position
    short m_nAdjustStep;	//adjust step of bond head level
	BOOL m_bBusy;	//V1.1.W153 add
};
