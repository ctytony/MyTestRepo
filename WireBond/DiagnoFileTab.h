#pragma once

// CDiagnoFileTab dialog
class CDiagnoFileTab : public CPropertyPage
{
	DECLARE_DYNAMIC(CDiagnoFileTab)

public:

	CDiagnoFileTab();
	virtual ~CDiagnoFileTab();

// Dialog Data
	enum { IDD = IDD_DIAGNOSE_FILE };

	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnControl();
	afx_msg void OnAutoTune();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.4.W43 add

	DECLARE_MESSAGE_MAP()

private:

	BOOL m_bSaveControlPara;
	BOOL m_bLoadSpeedPara;
	BOOL m_bLoadControlPara;
	BOOL m_bTuneX;
	BOOL m_bTuneY;
	BOOL m_bTuneZ;

	BOOL m_bBusy;	//V1.4.W43 add
};
