#pragma once

// CServoControlPage dialog
class CServoControlPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CServoControlPage)

public:
	CServoControlPage();
	virtual ~CServoControlPage();
    virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SERVO_CONTROL };

public:
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSettingNum();
    afx_msg void OnFocusVelGain();
    afx_msg void OnFocusVelIntegrator();
    afx_msg void OnFocusPosGain();
    afx_msg void OnFocusFfc();
    afx_msg void OnFocusXVelGain();
    afx_msg void OnFocusYVelGain();
    afx_msg void OnFocusXVelIntegrator();
    afx_msg void OnFocusYVelIntegrator();
    afx_msg void OnFocusXPosGain();
    afx_msg void OnFocusYPosGain();
    afx_msg void OnFocusXFfc();
    afx_msg void OnFocusYFfc();
    afx_msg void OnBlockNumX();
    afx_msg void OnBlockNumY();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:	//variables
    int m_nCtrlProfileZ;
    long m_lVelGain;
    long m_lVelIntegrator;
    long m_lPosGain;
    long m_lAccFeedForward;
    long m_lVelGainX;
    long m_lVelGainY;
    long m_lVelIntegratorX;
    long m_lVelIntegratorY;
    long m_lPosGainX;
    long m_lPosGainY;
    long m_lAccFeedForwardX;
    long m_lAccFeedForwardY;
    int m_nCtrlProfileX;
    int m_nCtrlProfileY;
};
