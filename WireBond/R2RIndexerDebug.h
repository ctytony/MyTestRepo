#pragma once

// CR2RIndexerDebug dialog

class CR2RIndexerDebug : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RIndexerDebug)

public:
	CR2RIndexerDebug();
	virtual ~CR2RIndexerDebug();

// Dialog Data
	enum { IDD = IDD_R2R_INDEXER_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();

public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


private:
	
	byte m_bStepperAddr;
	long m_lScale;
	long m_lPreclosePercent;
	long m_lSafeClosePercent;

public:
	afx_msg void OnBnClickedButtonForward();
	afx_msg void OnBnClickedButtonBackward();
	afx_msg void OnBnClickedButtonHomeStep();
	afx_msg void OnEnSetfocusEditScale();
	afx_msg void OnEnChangeEditScale();
	afx_msg void OnBnClickedButtonClampHome();
	afx_msg void OnBnClickedButtonClampClose();
	afx_msg void OnBnClickedButtonClampOpen();
	afx_msg void OnBnClickedButtonClampPreclose();
	afx_msg void OnEnSetfocusEditClampPercent();
	afx_msg void OnEnSetfocusEditPreclampPercent();
	afx_msg void OnEnChangeEditClampPercent();
	afx_msg void OnEnChangeEditPreclampPercent();

	
	afx_msg void OnBnClickedButtonStandBy();
	afx_msg void OnBnClickedButtonClampRemove();
	afx_msg void OnBnClickedButtonClamp();
    afx_msg void OnEnSetfocusClampPosition();
private:
    // Clamp Position in percentage
    long m_lClampPosition;
    // Jerk of clamper
    long m_lJerk;   
public:
    afx_msg void OnEnSetfocusEdit2();
};
