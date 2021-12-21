
#pragma once


// CR2RIndexTchPage dialog


class CR2RIndexTchPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RIndexTchPage)

public:
	CR2RIndexTchPage();
	virtual ~CR2RIndexTchPage();

// Dialog Data
	enum { IDD = IDD_R2R_PR_INDEX_TEACH};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditSizeX();
	afx_msg void OnEnChangeEditSizeY();
	afx_msg void OnEnSetfocusEditSizeX();
	afx_msg void OnEnSetfocusEditSizeY();
private:
	short m_nPatternSizeX;
	short m_nPatternSizeY;
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonContinue();
private:
	short m_nStepId;


	CString m_szPRAvailble;
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();

	afx_msg void OnBnClickedButtonResetPr();
private:
	bool m_bTchNewActived; //pointer to index a new window
};