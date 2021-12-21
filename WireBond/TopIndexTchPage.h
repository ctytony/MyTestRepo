#pragma once

struct TOP_PR_INDEX_POS
{
	double tbTopRightPos;	//um
	double tbBottomLeftPos;	//um
	long lCenterPos;		//um
};

// CTopIndexTchPage dialog
class CTopIndexTchPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopIndexTchPage)

public:

	CTopIndexTchPage();
	virtual ~CTopIndexTchPage();

// Dialog Data
	enum { IDD = IDD_TOP_PR_INDEX_TEACH};

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEditSizeX();
	afx_msg void OnEditSizeY();
	afx_msg void OnFocusSizeX();
	afx_msg void OnFocusSizeY();
	afx_msg void OnStart();
	afx_msg void OnContinue();

	virtual BOOL OnSetActive();
	afx_msg void OnResetPr();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W158 add

	DECLARE_MESSAGE_MAP()

private:

	short m_nPatternSizeX, m_nPatternSizeY, m_nStepId;

	CString m_szPRAvailble;

	bool m_bTchNewActived;

	BOOL m_bBusy;	//V1.1.W158 add
};
