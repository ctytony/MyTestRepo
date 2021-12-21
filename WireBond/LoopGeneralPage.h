#pragma once

// CLoopGeneralPage dialog
class CLoopGeneralPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopGeneralPage)
public:
	CLoopGeneralPage();
	virtual ~CLoopGeneralPage();
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_LOOP_GENERAL };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
    afx_msg void OnFocusLoopHt();
    afx_msg void OnFocusLoopHtCorrect();
    afx_msg void OnFocusLAngle();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SaveParameter();	//V1.1.W246 add
	DECLARE_MESSAGE_MAP()
public:
	void UpdateMenuVariable(void);
private:
    short m_nLoopHeight;
    short m_nLoopHtCorrect;
	short m_nLAngle;
};
