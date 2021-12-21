#pragma once

// CImpactModePage 对话框

class CImpactModePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CImpactModePage)
public:
	CImpactModePage();
	virtual ~CImpactModePage();
// 对话框数据
	enum { IDD = IDD_IMPACT_MODE };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	void SaveParameter(void);
	void UpdateMenuVariable(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnEnSetfocusEditSpeed1();
	afx_msg void OnEnSetfocusEditSpeed2();
	afx_msg void OnEnSetfocusEditSpeed3();
	afx_msg void OnEnSetfocusEditMode1();
	afx_msg void OnEnSetfocusEditMode2();
	afx_msg void OnEnSetfocusEditMode3();
private:
	short m_nSpeed1;
	short m_nSpeed2;
	short m_nSpeed3;
	short m_nMode1;
	short m_nMode2;
	short m_nMode3;
};
