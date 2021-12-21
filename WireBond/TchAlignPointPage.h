#pragma once

// CTchAlignPointDlg dialog

class CTchAlignPointPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTchAlignPointPage)
public:
	CTchAlignPointPage();
	virtual ~CTchAlignPointPage();
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
    virtual void OnCancel();
// Dialog Data
	enum { IDD = IDD_TCH_ALIGN_PT };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnStart();
    afx_msg void OnContinue();
    afx_msg void OnEditDieNum();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.1.W153 add
	DECLARE_MESSAGE_MAP()
private:	//variable
    short m_nUnitTotalDie, m_nTotalAlignStep, m_nAlignStep;
	BOOL m_bBusy;	//V1.1.W153 add
};
