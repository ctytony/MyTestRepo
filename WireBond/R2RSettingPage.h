#pragma once




// CR2RSettingPage dialog

class CR2RSettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RSettingPage)

public:
	CR2RSettingPage();
	virtual ~CR2RSettingPage();

// Dialog Data
	enum { IDD = IDD_R2R_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	long m_lHeatDelay;
public:
	afx_msg void OnEnSetfocusEditHeatDelay();
	virtual void OnOK();
private:
	long m_lLimitTolerance;
public:
	afx_msg void OnEnSetfocusEditLimitTol();
private:
	long m_lSrchBackLen;
public:
//	afx_msg void OnEnSetfocusEditXBack();
private:
	long m_lStandbyLen;
public:
	afx_msg void OnEnSetfocusEditXStandbyLen();
private:
	int m_iIndexSpeedCo;
public:
	afx_msg void OnCbnSelchangeComboIndexSpeed();
private:
	int m_iEjectSpeed;
public:
	afx_msg void OnCbnSelchangeComboEjectSpeed();
private:
	long m_l1stSlipHeatDelay;
public:
	afx_msg void OnEnSetfocusEdit1stHeatDelay();
private:
	int m_iPrIndexEnable;
public:
	afx_msg void OnCbnSelchangeComboPrIndex();
private:
	long m_lPrIndexTol;
public:
	afx_msg void OnEnSetfocusEditPrIndexTol();
private:
	long m_lIndexClawOffset;
public:
	afx_msg void OnEnSetfocusEditClawOffset();
private:
	long m_lOpenClampDelay;
public:
	afx_msg void OnEnSetfocusEditOpenDelay();
private:
	long m_lIndexPrDelay;
public:
	afx_msg void OnEnSetfocusEditIndexPrDelay();
//	afx_msg void OnEnChangeEdit1stHeatDelay();
private:
	long m_lIniDelay;
public:
	afx_msg void OnEnSetfocusEditIniDelay();
private:
    // Page size
    short m_nPageSize;
    // Vacuum control flag
    int m_iVacuumControl;
public:
    afx_msg void OnCbnSelchangeComboVacuumControl();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnStnClickedStaticIniDelay2();
private:
	long m_lVacumDelay;
public:
	afx_msg void OnEnSetfocusEditVacumDelay();
private:
	long m_l1stUnitHeatDelay;
public:
	afx_msg void OnEnSetfocusEdit1stUnitDelay();
	afx_msg void OnEnSetfocusEditPrIndexScore();
	short m_sPRIndexScore;
};
