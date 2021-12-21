#pragma once


// CLoop3rdKinkPagePan 对话框

class CLoop3rdKinkPagePan : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoop3rdKinkPagePan)

public:
	CLoop3rdKinkPagePan();
	virtual ~CLoop3rdKinkPagePan();

// 对话框数据
	enum { IDD = IDD_LOOP_3RD_KINK_PAN };

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
	short m_nBelleHT;
	short m_nBelleDist;
	short m_nBelleAngle;
	int m_iBelleAfxe;
public:
	afx_msg void OnEnSetfocusEditBelleHt();
	afx_msg void OnEnSetfocusEditBelleDist();
	afx_msg void OnEnSetfocusEditBelleAngle();
	afx_msg void OnCbnSelchangeComboBelleAfxe();
};
