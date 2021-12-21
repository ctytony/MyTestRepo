#pragma once

// CTopClampPage dialog
class CTopIndexerDebug : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopIndexerDebug)

public:

	CTopIndexerDebug();
	virtual ~CTopIndexerDebug();

// Dialog Data
	enum { IDD = IDD_TOP_INDEXER_DEBUG };

    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive( );
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnForward();
	afx_msg void OnBackward();
	afx_msg void OnHomeStep();
	afx_msg void OnFocusScale();
	afx_msg void OnEditScale();
	afx_msg void OnClampHome();
	afx_msg void OnClampClose();
	afx_msg void OnClampOpen();
	afx_msg void OnClampPreclose();
	afx_msg void OnFocusClampPercent();
	afx_msg void OnFocusPreclampPercent();
	afx_msg void OnEditClampPercent();
	afx_msg void OnEditPreclampPercent();	
	afx_msg void OnStandBy();
	afx_msg void OnClampRemove();
	afx_msg void OnClamp();
    afx_msg void OnFocusClampPosition();
    afx_msg void OnFocus2();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.4.W23 add

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:	//variables

	byte m_bStepperAddr;
	long m_lScale;
	long m_lPreclosePercent;
	long m_lSafeClosePercent;
    // Clamp Position in percentage
    long m_lClampPosition;
    // Jerk of clamper
    long m_lJerk;

	BOOL m_bBusy;	//V1.4.W23 add
};
