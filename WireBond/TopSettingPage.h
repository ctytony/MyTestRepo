#pragma once

// CTopSettingPage dialog

class CTopSettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopSettingPage)

public:
	CTopSettingPage();
	virtual ~CTopSettingPage();

// Dialog Data
	enum { IDD = IDD_TOP_SETTING };
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFocusHeatDelay();
	afx_msg void OnFocusLimitTol();
	afx_msg void OnFocusXStandbyLen();
	afx_msg void OnIndexSpeed();
	afx_msg void OnEjectSpeed();
	afx_msg void OnFocus1stHeatDelay();
	afx_msg void OnPrIndex();
	afx_msg void OnFocusPrIndexTol();
	afx_msg void OnFlattenLF();
	afx_msg void OnFocusClawOffset();
	afx_msg void OnFocusOpenDelay();
	afx_msg void OnFocusIndexPrDelay();
	afx_msg void OnFocusIniDelay();
    afx_msg void OnVacuumControl();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFocusVacumDelay();
	afx_msg void OnFocus1stUnitDelay();
	afx_msg void OnFocusPrIndexScore();
    afx_msg void OnOutputMagazineSlotSequence();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHeatDelayUnit();
	afx_msg void OnLFWaitPos();
	afx_msg void OnFocusSpeed();	//V1.4.W35 add
	afx_msg void OnDestroy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT SaveSetting(WPARAM wParam, LPARAM lParam);	//V1.5.W117 add
	DECLARE_MESSAGE_MAP()
public:		//variable
	short m_sPRIndexScore;
private:	//variable
	long m_lHeatDelay;
	long m_lLimitTolerance;
	long m_lSrchBackLen;
	long m_lStandbyLen;
	int m_iIndexSpeedCo;
	int m_iEjectSpeed;
	long m_l1stSlipHeatDelay;
	int m_iPrIndexEnable;
	long m_lPrIndexTol;
	long m_lIndexClawOffset;
	long m_lOpenClampDelay;
	long m_lIndexPrDelay;
	long m_lIniDelay;
    short m_nPageSize;
    // Vacuum control flag
    int m_iVacuumControl;
	long m_lVacumDelay;
	long m_l1stUnitHeatDelay;
    int m_iOutputMagazineSlotSequence;
	BYTE m_nSel;
private:	//function
	void UpdateUI(BYTE nSel);
};
