#pragma once

// CDebugDlg dialog

class CDebugDlg : public CDialog
{
	DECLARE_DYNAMIC(CDebugDlg)
public:
	CDebugDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugDlg();
// Dialog Data
	enum { IDD = IDD_DEBUG };
public:
    virtual BOOL OnInitDialog();
    void UpdateDebugInfo(CString &sDebugInfo);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnApply();
    afx_msg void OnDebugMode();
    afx_msg void OnEditNum();
    afx_msg void OnWaveform();
    afx_msg void OnInfo();
    afx_msg void OnItem();
	afx_msg void OnClear();
	afx_msg void OnCorrectBond();
protected:
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W245 add
    afx_msg void OnDestroy();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
private:	//variables
    int m_nItem;		//program, reference, matrix, wire, or bond info
    short m_nItemIdx;	//0,1,2...
    CString m_sInfo;
    int m_nMode;
	BOOL m_bCorrectBond;
};
