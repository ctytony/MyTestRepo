#pragma once

// CServoGeneralPage dialog
class CServoGeneralPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CServoGeneralPage)

public:

	CServoGeneralPage();
	virtual ~CServoGeneralPage();

// Dialog Data
	enum { IDD = IDD_SERVO_GENERAL };

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnXEnable();
    afx_msg void OnXDisable();
    afx_msg void OnXHome();
    afx_msg void OnYEnable();
    afx_msg void OnYDisable();
    afx_msg void OnYHome();
    afx_msg void OnZEnable();
    afx_msg void OnZDisable();
    afx_msg void OnZHome();
    afx_msg void OnWclEnable();
    afx_msg void OnWclDisable();
    afx_msg void OnInitialize();
    afx_msg void OnRecover();
    afx_msg void OnFocusDampGain2();
    afx_msg void OnFocusSearchMode();
    afx_msg void OnFocusDampGain();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:	//variables

    short m_nSearchMode;
    short m_nDampGain;		//1st bond
    short m_nDampGain2;		//2nd bond
};
