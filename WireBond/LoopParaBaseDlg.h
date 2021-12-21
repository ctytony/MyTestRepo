#pragma once

#include "AdvLoopParaSheet.h"
#include "afxcmn.h"

// CLoopParaBaseDlg dialog
class CLoopParaBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoopParaBaseDlg)
public:
	CLoopParaBaseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoopParaBaseDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_LOOP_PARA_BASE };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLoopGroup();
	afx_msg void OnPaint();
	afx_msg void OnLoopType();
	afx_msg void OnLoopProfile();
	afx_msg void OnFocusWireNum();
protected:
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.1.W246 add
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();	//V1.1.W246 add
	DECLARE_MESSAGE_MAP()
public:	//variable
	CAdvLoopParaSheet *m_pParaSheet;
public:	//function
	void UpdateLoopGroupMenu(short nWireNum = 0);
	void UpdateLoopDiagram(void);
private:
	int m_iLoopGroup;
	int m_iLoopType;
	int m_iLoopProfile;
	short m_nCtactDiff;
	short m_nWireNum;	//1st wire no. of current loop group
	CString m_sWireLen;
	CString m_sBondOnRef;
	CWnd *m_pWndLoopDisp;
};
