#pragma once

enum BK_PR_OP_TYPE
{
	NEW_BK_PR,
	EDIT_BK_PR
};  

// CBondEditPage dialog

class CBondEditPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBondEditPage)
public:
	CBondEditPage();
	virtual ~CBondEditPage();
// Dialog Data
	enum { IDD = IDD_BOND_EDIT };
	DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnEditWireNum();
    afx_msg void OnFocusWireNum();
    afx_msg void OnUpdate();
    afx_msg void OnStart();
	afx_msg void OnBkPr1();
	afx_msg void OnBkPr2();
	afx_msg void OnBkPr3();
	afx_msg void OnBkPr4();
	afx_msg void OnDelete();
	afx_msg void OnReplaceAll();
	afx_msg void OnAutoSwapPr();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	void UpdateBkPrView(void);
	void PrepareTchBkPr(short nMode = NEW_BK_PR);
	void DeleteAllBackupPr(void);
private:
	short m_nBakUpId;
	short m_nCurrentWireNum;
    short m_nAlignNum;
    short m_nBondPoint;
	int m_iAutoSwapMainPr;
};
