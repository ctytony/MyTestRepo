#pragma once

// CDlgTopWhSetUpPage dialog
class CDlgTopWhSetUpPage : public CDialog
{
	DECLARE_DYNAMIC(CDlgTopWhSetUpPage)
public:
	CDlgTopWhSetUpPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTopWhSetUpPage();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_TOP_WH_SETUP };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnStart();
	afx_msg void OnStartTchTable();
	afx_msg void OnNext();
    afx_msg void OnClampperEdit();
    afx_msg void OnElevatorSide();
    afx_msg void OnFocusMagClamperLimit();
    afx_msg void OnFocusMagClamperPosition();
    afx_msg void OnEditMagClamperPosition();
    afx_msg void OnEditMagClamperLimit();
	afx_msg void OnClampClose();	//V1.5.W83 add
	afx_msg void OnBnClickedButtonDone();	//v3.0.T453 add
	afx_msg void OnBnClickedButtonDone2();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.1.W147 add
	DECLARE_MESSAGE_MAP()
private:	//variables
	long m_lIndexLogicZero;
	long m_lIndexRightLimit;
	double m_dSafePosX;
	double m_dSafePosY;
	CString m_sSafePosX;
	CString m_sSafePosY;
    long m_lMagClamperLimit;
    long m_lMagClamperPosition;
    int m_iElevatorId;
	BOOL m_bBusy;	//V1.1.W147 add
};
