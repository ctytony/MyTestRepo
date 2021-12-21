#pragma once

enum enMotorList 
{
	ID_INDEX_MOTOR =0,
    ID_FRONTRAIL_MOTOR = 1,
    ID_REARRAIL_MOTOR = 2,
};

// CTopClampPage dialog

class CCuEliteIndexerDebug : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteIndexerDebug)

public:
	CCuEliteIndexerDebug();
	virtual ~CCuEliteIndexerDebug();

// Dialog Data
	enum { IDD = IDD_CUELITE_INDEXER_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();

public:
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive( );
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


private:
	
	byte m_bStepperAddr;
	long m_lScale;
	long m_lPreclosePercent;
	long m_lSafeClosePercent;

public:

	afx_msg void OnBnClickedButtonHomeStep();
	afx_msg void OnEnSetfocusEditScale();
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
private:
	int m_StepMotorId;
public:
    afx_msg void OnEnSetfocusEdit2();
public:
    afx_msg void OnLbnSelchangeListStepperMotor();
	afx_msg void OnBnClickedButtonOpenTrack();
	afx_msg void OnBnClickedButtonCloseTrack();
};
