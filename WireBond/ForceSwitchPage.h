#pragma once

// CForceSwitchPage 对话框

class CForceSwitchPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CForceSwitchPage)
public:
	CForceSwitchPage();
	virtual ~CForceSwitchPage();
// 对话框数据
	enum { IDD = IDD_FORCE_SWITCH };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	void UpdateMenuVariable(void);
	void SaveParameter(void);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboFsw1();
	afx_msg void OnCbnSelchangeComboFsw2();
	afx_msg void OnCbnSelchangeComboFsw3();
	afx_msg void OnCbnSelchangeComboFsw4();
	afx_msg void OnCbnSelchangeComboFsw5();
	afx_msg void OnCbnSelchangeComboFsw6();
	afx_msg void OnCbnSelchangeComboFsw7();
	afx_msg void OnCbnSelchangeComboFsw8();
private:
	int m_iFsw1;
	int m_iFsw2;
	int m_iFsw3;
	int m_iFsw4;
	int m_iFsw5;
	int m_iFsw6;
	int m_iFsw7;
	int m_iFsw8;
};
