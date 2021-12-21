#pragma once

enum enElevator 
{
	ELEVATOR_ONLOADER,
	ELEVATOR_OFFLOADER,
};

// CCuEliteStepperPage 对话框

class CCuEliteStepperPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteStepperPage)

public:
	CCuEliteStepperPage();
	virtual ~CCuEliteStepperPage();

// 对话框数据
	enum { IDD = IDD_CUELITE_STEPPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iElevatorId;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboElevator();
	afx_msg void OnBnClickedButtonMagLengthHome();
	afx_msg void OnBnClickedButtonMagLengthStandbyPos();
	afx_msg void OnBnClickedButtonMagLengthStop();
	afx_msg void OnBnClickedButtonMagClampBackHome();
	afx_msg void OnBnClickedButtonMagClampBackStandbyPos();
	afx_msg void OnBnClickedButtonMagClampBackStop();
	afx_msg void OnBnClickedButtonMagClampFrontHome();
	afx_msg void OnBnClickedButtonMagClampFrontStandbyPos();
	afx_msg void OnBnClickedButtonMagClampFrontStop();
	afx_msg void OnBnClickedButtonPullyHome();
	afx_msg void OnBnClickedButtonPullyLimit();
	afx_msg void OnBnClickedButtonHolderHome();
	afx_msg void OnBnClickedButtonHolderStop();
	afx_msg void OnBnClickedButtonNextSlot();
	afx_msg void OnBnClickedButtonHolderStandby();
	afx_msg void OnBnClickedButtonUnload();
	afx_msg void OnBnClickedButtonPusherHome();
	afx_msg void OnBnClickedButtonPusherLimit();
	afx_msg void OnEnSetfocusEditCurrentSlot();
	afx_msg void OnEnChangeEditCurrentSlot();
	short m_nCurrentSlot;
};
