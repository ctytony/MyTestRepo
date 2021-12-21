#pragma once


// CCuEliteSettingPage dialog

class CCuEliteSettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteSettingPage)

public:

	CCuEliteSettingPage();
	virtual ~CCuEliteSettingPage();

// Dialog Data
	enum { IDD = IDD_CUELITE_SETTING };

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditHeatDelay();
	virtual void OnOK();
	afx_msg void OnEnSetfocusEditLimitTol();
//	afx_msg void OnEnSetfocusEditXBack();
	afx_msg void OnEnSetfocusEditXStandbyLen();
	afx_msg void OnCbnSelchangeComboIndexSpeed();
	afx_msg void OnCbnSelchangeComboEjectSpeed();
	afx_msg void OnEnSetfocusEdit1stHeatDelay();
	afx_msg void OnCbnSelchangeComboPrIndex();
	afx_msg void OnEnSetfocusEditPrIndexTol();
	afx_msg void OnEnSetfocusEditClawOffset();
	afx_msg void OnEnSetfocusEditOpenDelay();
	afx_msg void OnEnSetfocusEditIndexPrDelay();
//	afx_msg void OnEnChangeEdit1stHeatDelay();
	afx_msg void OnEnSetfocusEditIniDelay();
    afx_msg void OnCbnSelchangeComboVacuumControl();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnStnClickedStaticIniDelay2();
	afx_msg void OnEnSetfocusEditVacumDelay();
	afx_msg void OnEnSetfocusEdit1stUnitDelay();
	afx_msg void OnEnSetfocusEditHeatProtectTime();
    afx_msg void OnCbnSelchangeComboOutputMagazineSlotSequence();
	afx_msg void OnEnSetfocusEditClawDelay();
	afx_msg void OnEnSetfocusEditPrIndexScore();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

	short m_sPRIndexScore;
	int m_nEjectSpeedCoeff;
	long m_lIndexClawOffset;
	long m_l1stSlipHeatDelay;
	int m_iPrIndexEnable;
	long m_lPrIndexTol;
	long m_lOpenClampDelay;
	long m_lIndexPrDelay;
	long m_lIniDelay;
    // Page size
    short m_nPageSize;
    // Vacuum control flag
    int m_iVacuumControl;
	long m_lVacumDelay;
	long m_lHeatDelay;
	long m_lLimitTolerance;
	long m_lSrchBackLen;
	long m_lStandbyLen;
	int m_nIndexSpeedCoeff;
	long m_l1stUnitHeatDelay;
    int m_iOutputMagazineSlotSequence;
    long m_lClawDelay;
    long m_lWindowClamperOverHeatTime;
};
