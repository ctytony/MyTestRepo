#pragma once

// CDlgDelBkPR dialog

class CDlgDelBkPR : public CDialog
{
public:

	CDlgDelBkPR(CWnd* pParent);   // standard constructor
	virtual ~CDlgDelBkPR();

// Dialog Data
	enum { IDD = IDD_DEL_BACKUP_PR };

	BYTE IsDirty();

protected:

	BYTE m_bDirty;

	void InitDialog();
	void GetWireNum();
	void UpdateStatus();

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	afx_msg void OnBkSelected(UINT nID);
	afx_msg void OnDelete();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
