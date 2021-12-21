#pragma once

#include "AdvParaMapDlg.h"

// CParaMapBase dialog

class CParaMapBase : public CDialog
{
	DECLARE_DYNAMIC(CParaMapBase)
public:
	CParaMapBase(short nParaType, CWnd* pParent = NULL);   // standard constructor
	virtual ~CParaMapBase();
    virtual BOOL OnInitDialog();
	enum { IDD = IDD_PARA_MAP_BASE };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnGrouping();
    afx_msg void OnApplyTo();
    afx_msg void OnDestroy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.5.W27 add
	DECLARE_MESSAGE_MAP()
public:		//function
    void SetHelpMessage(CString& sHelpMsg);
private:	//variable
    CAdvParaMapDlg m_dlgAdvParaMap;
    short m_nParaMapType;
    CString m_sHelpMsg;
    CStatic *m_paTopLabel[8];
    int m_nParaApplyTo;
};
