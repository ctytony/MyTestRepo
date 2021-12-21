#pragma once


// CLoopLandingPagePan 对话框

class CLoopLandingPagePan : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopLandingPagePan)

public:
	CLoopLandingPagePan();
	virtual ~CLoopLandingPagePan();

// 对话框数据
	enum { IDD = IDD_LOOP_LAND_PAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnEnSetfocusEditSlAey();
	afx_msg void OnEnSetfocusEditAboneLoopLop();
	afx_msg void OnEnSetfocusEditCtactLolOffset();
	afx_msg void OnEnSetfocusEditPullDistH();
	afx_msg void OnEnSetfocusEditPullDistV();
	afx_msg void OnEnSetfocusEditPullDistVTime();
	afx_msg void OnCbnSelchangeComboSlAeyUnit();
	
private:
	short m_nSLaey;
	short m_nAboneLoopLop;
	short m_nCtactLolOffset;
	short m_nPullDistH;
	short m_nPullDistV;
	short m_nPullVTime;
	int m_iSLaeyUnit;
public:
	void UpdateMenuVariable(void);
protected:
	void SaveParameter(void);
};
