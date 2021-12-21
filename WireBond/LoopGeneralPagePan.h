#pragma once


// CLoopGeneralPagePan �Ի���

class CLoopGeneralPagePan : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopGeneralPagePan)

public:
	CLoopGeneralPagePan();
	virtual ~CLoopGeneralPagePan();

// �Ի�������
	enum { IDD = IDD_LOOP_GENERAL_PAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocusEditLoopHeight();
	afx_msg void OnEnSetfocusEditLoopCompensation();
	afx_msg void OnEnSetfocusEditLoopAlpha();
	afx_msg void OnEnSetfocusEditLoopXyDelay();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
private:
	short m_nLoopHeight1;
	short m_nLoopCorrect;
	short m_nAlpha;
	short m_nXydelay;
protected:
	void SaveParameter();
public:
	void UpdateMenuVariable(void);
};
