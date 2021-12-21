#pragma once


// CBallScrubPage dialog

class CBallScrubPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBallScrubPage)

public:
	CBallScrubPage();
	virtual ~CBallScrubPage();

// Dialog Data
	enum { IDD = IDD_BALL_SCRUB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	int m_iBallScrubEnable;
	short m_nBallScrubCycle;
	short m_nBallScrubAmplitude;
	int m_iBallScrubDirection;
	short m_nBallScrubSpeed;
	short m_nBallScrubForce;
	short m_nBallScrubPower;
public:
	afx_msg void OnCbnSelchangeComboBallScrub();
	afx_msg void OnCbnSelchangeComboBallScrubDir();
	afx_msg void OnEnSetfocusEditBallScrubCycle();
	afx_msg void OnEnSetfocusEditBallScrubAmp();
	afx_msg void OnEnSetfocusEditBallScrubSpeed();
	afx_msg void OnEnSetfocusEditBallScrubForce();
	afx_msg void OnEnSetfocusEditBallScrubPower();
	virtual void OnOK();
};
