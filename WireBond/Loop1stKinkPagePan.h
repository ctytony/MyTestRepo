#pragma once


// CLoop1stKinkPagePan 对话框

class CLoop1stKinkPagePan : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoop1stKinkPagePan)

public:
	CLoop1stKinkPagePan();
	virtual ~CLoop1stKinkPagePan();

// 对话框数据
	enum { IDD = IDD_LOOP_1ST_KINK_PAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboRhAdj();
	afx_msg void OnCbnSelchangeComboNeckHepht();
	afx_msg void OnEnSetfocusEditNeckAngle();
	afx_msg void OnEnSetfocusEditRhAdy();
	afx_msg void OnEnSetfocusEditNeckHephtAdj();


private:
	int m_iNeckHephtUnit;
	short m_nNeckAngle;
	short m_nRHadj;
	int m_iRDadjUnit;
	short m_nNeckHephtadj;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	void UpdateMenuVariable(void);
	void SaveParameter(void);
};
