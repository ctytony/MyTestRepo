#pragma once

// CBtosPage dialog
class CBtosPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBtosPage)
public:
	CBtosPage();
	virtual ~CBtosPage();
// Dialog Data
	enum { IDD = IDD_BTOS };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDown();
    afx_msg void OnUp();
    afx_msg void OnEditBtosNewX();
    afx_msg void OnEditBtosNewY();
    afx_msg void OnFocusBtosNewX();
    afx_msg void OnFocusBtosNewY();
    afx_msg void OnFocusEditStep();
    afx_msg void OnDestroy();
	afx_msg void OnCorrect();
	afx_msg void OnStart();
	afx_msg void OnVerify();
	afx_msg void OnBnClickedButtonDone();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W153 add
	DECLARE_MESSAGE_MAP()
public:		//function
	void StartAndVerifyHandle(void);
private:	//variable
	long m_lNewBtosX;
	long m_lNewBtosY;
	short m_nSrchSpeed;
	short m_nSrchTol;
	short m_nAdjustStep;
	short m_nBtosStage;
	TBL_POS stOrgPos;
	TBL_POS stNewPos;
	bool m_bAllowAdjust; 
	bool m_bVerify;
	BOOL m_bBusy;	//V1.1.W153 add
};
