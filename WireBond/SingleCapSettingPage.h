#pragma once

#define CLAW_ONE_ROUND_SCALE   1600

// CSingleCapSettingPage dialog

class CSingleCapSettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSingleCapSettingPage)

public:
	CSingleCapSettingPage();
	virtual ~CSingleCapSettingPage();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SINGLE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditPreheatTime();
	afx_msg void OnEnSetfocusEditOpenPretrig();
	afx_msg void OnEnSetfocusEditPreclampTime();
	afx_msg void OnCbnSelchangeComboLfUnits();
	afx_msg void OnEnSetfocusEditOffloadForkBuff();
	afx_msg void OnEnSetfocusEditSwitchBuff();
    afx_msg void OnEnSetfocusEditNormalLfPreheatTime();
	afx_msg void OnEnSetfocusEditAvoidCrashTune();
    afx_msg void OnEnSetfocusEditLfPickDelay();
	afx_msg void OnEnSetfocusEditLfPickOvershoot();
	afx_msg void OnCbnSelchangeComboCopperLf();

private:	//variables
	short m_nInitUnit;
	short m_nIndexUnit;
	short m_nOutputSteps;
	//short m_nInjectDelay;
	short m_nOpenPretrig;
	short m_nClampPretirg;
	short m_nPreClampTime;
	int m_iLfUnits;
	short m_nForkBuff;
	// default:1k switch
	short m_nSwitchBuffer;
	short m_nEndOffsetUnit;
	short m_nPreheatTime;
	short m_nAvoidCrashPulse;
    // Normal index LF preheat time
    short m_nNormalLfPreheatTime;
	short m_nAvoidCrashPulseCompen;
    // Lead frame pick delay
    short m_nLfPickDelay;
	long m_lOvershootLen;
	int m_nOnloaderType;
};
