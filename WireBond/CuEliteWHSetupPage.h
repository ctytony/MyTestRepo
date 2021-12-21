#pragma once


// CCuEliteWHSetupPage dialog


class CCuEliteWHSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteWHSetupPage)

public:
	CCuEliteWHSetupPage();
	virtual ~CCuEliteWHSetupPage();

// Dialog Data
	enum { IDD = IDD_DLG_CUELITE_WH_SET_UP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


public:
	//virtual BOOL OnSetActive();
    

private:
	long m_lIndexLogicZero;
	long m_lIndexRightLimit;
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStartTchTable();
	afx_msg void OnBnClickedButtonNext();
      
	/*v2.18.5.w4 delete {
    afx_msg void OnBnClickedButtonClampperEdit();
    //afx_msg void OnBnClickedButtonClampperDone();
    afx_msg void OnCbnSelchangeComboElevatorSide();
    afx_msg void OnEnSetfocusEditMagClamperLimit();
    afx_msg void OnCbnSelchangeComboLeadFrameType();
	}*/
    /*afx_msg void OnBnClickedOk();*/

private:
	double m_dWhTbX;
	double m_dWhTbY;
    long m_lMagClamperLimit;
    int m_iElevatorId;
    int m_nLeadFrameTrackType;

	//v2.18.6.w6 add{
	CString m_sID;
	//}

public:

	afx_msg void OnEnSetfocusEditIndexRightLimit();
	afx_msg void OnEnSetfocusEditIndexLogicZero();
	afx_msg void OnBnClickedButtonIndexHome();
	afx_msg void OnBnClickedButtonIndexTeach();
};
