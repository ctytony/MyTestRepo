#pragma once

// CBondFromToUnitDlg dialog

class CBondFromToUnitDlg : public CDialog
{
	DECLARE_DYNAMIC(CBondFromToUnitDlg)
public:
	CBondFromToUnitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBondFromToUnitDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_BOND_FROM_TO };
protected:
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditStartId();
	afx_msg void OnEnSetfocusEditEndId();
public:		//variable
	CBrush m_BkBrush;
private:	//variable
	short m_nStartWire;	//v3.0.T188 rename
	short m_nEndWire;	//v3.0.T188 rename
};
