#pragma once


// CLoop2ndKinkPagePan 对话框

class CLoop2ndKinkPagePan : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoop2ndKinkPagePan)

public:
	CLoop2ndKinkPagePan();
	virtual ~CLoop2ndKinkPagePan();

// 对话框数据
	enum { IDD = IDD_LOOP_2ND_KINK_PAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	void UpdateMenuVariable(void);
	void SaveParameter(void);
private:
	short m_nFLengh;
	int m_iFLenghUnit;
	short m_nHLadj;
	int m_iHLadjUnit;
	short m_nFLadj;
	int m_iFLadjUnit;
	short m_nAngComp;
	short m_nKintHt;
public:
	afx_msg void OnCbnSelchangeComboFlEnghUnit();
	afx_msg void OnEnSetfocusEditHlAdj();
	afx_msg void OnCbnSelchangeComboHlAdjUnit();
	afx_msg void OnEnSetfocusEditFlAdj();
	afx_msg void OnCbnSelchangeComboFlAdjUnit();
	afx_msg void OnEnSetfocusEditAngComp();
	afx_msg void OnEnSetfocusEditFlEngh();
	afx_msg void OnEnSetfocusEditKintHt();
};
