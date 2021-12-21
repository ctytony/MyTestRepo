#pragma once

// CStickDetectDlg dialog
class CStickDetectDlg : public CDialog
{
	DECLARE_DYNAMIC(CStickDetectDlg)

public:

	CStickDetectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStickDetectDlg();

// Dialog Data
	enum { IDD = IDD_STICK_DETECT };

	virtual BOOL OnInitDialog() sealed;
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnFocusApplyVoltage();
    afx_msg void OnFocusDieThreshold();
    afx_msg void OnFocusLeadThreshold();
    afx_msg void OnFocusTailShortThreshold();
    afx_msg void OnFocusNonstickThreshold();
    afx_msg void OnFocusTailBreakThreshold();
    afx_msg void OnCoarseDetect();
    afx_msg void OnFocusCoarseDetectSample();
    afx_msg void OnDestroy();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W24 add

	DECLARE_MESSAGE_MAP()

private:

    // Apply voltage
    float m_fApplyVoltage;
    // Die Threshold
    float m_fDieThreshold;
    // Lead threshold
    float m_fLeadThreshold;
    // Tail short threshold
    short m_nTailShortThreshold;
    // 2nd bond nonstick threshold
    short m_nNonStickThreshold;
    // Tail break nonstick threshold
    short m_nTailBreakNonstickThreshold;
    // Coarse detection enable flag
    int m_iCoarseDetectionFlag;
    // Coarse detection sample for stop machine
    short m_nCoarseDetectSample;
};
