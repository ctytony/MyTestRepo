#pragma once

// CBondObjectDlg dialog

class CBondObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CBondObjectDlg)
public:
	CBondObjectDlg(char cDisplayOption = 0, CWnd* pParent = NULL);   // standard constructor
	virtual ~CBondObjectDlg();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_BOND_OBJECT };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
    void SetBondObject(int iBondObject);
    int GetBondObject(void);
private:
    char m_cDisplayOption;
    BOOL m_iBondObject;	// Bond object: Wire and Ball; Ball Only; Wire Only
};
