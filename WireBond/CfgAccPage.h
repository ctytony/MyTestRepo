#pragma once

// CCfgAccPage dialog
class CCfgAccPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCfgAccPage)

public:

	CCfgAccPage();
	virtual ~CCfgAccPage();

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();

    afx_msg void OnTblProfile();
    afx_msg void OnZProfile();
    afx_msg void OnFocusTblAcc();
    afx_msg void OnFocusTblJerk();
    afx_msg void OnFocusBhAcc();
    afx_msg void OnFocusBhJerk();
    afx_msg void OnFocusYAcc();
    afx_msg void OnFocusYJerk();

// Dialog Data
	enum { IDD = IDD_CONFIG_ACC };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

    // Table profile number
    int m_iTblProfileNum;
    // Bond head profile number
    int m_iBondHeadProfileNum;
    // Table max acceleration
    short m_nTblMaxAcc;
    // Table jerk
    int m_iTblJerk;
    // Bond head max acceleration
    short m_nBondHeadMaxAcc;
    // Bond head jerk
    int m_iBondHeadJerk;
    // Y table max acc
    short m_nTblMaxAccY;
    // Y table Jerk
    int m_iTblJerkY;
};
