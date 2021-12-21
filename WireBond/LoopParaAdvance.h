#pragma once


// CLoopParaAdvance dialog

class CLoopParaAdvance : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopParaAdvance)

public:
	CLoopParaAdvance();
	virtual ~CLoopParaAdvance();

// Dialog Data
	enum { IDD = IDD_TAB_LOOP_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    // Loop group
    int m_iLoopGroup;
    // Trajectory factor
    short m_nTrajFactor;
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnEnSetfocusEditTrajFactor();
    virtual BOOL OnSetActive();
    virtual void OnOK();
private:
    // Trajectory pivot mode
    int m_iTrajectoryMode;
    // Loop top mode
    int m_iLoopTopMode;

	short m_nPullDist;
public:
	afx_msg void OnCbnSelchangeComboTrajMode();
	afx_msg void OnCbnSelchangeComboLoopTopMode();
private:
    // Contact level offset
    short m_nCtactLvlOffset;
public:
    afx_msg void OnEnSetfocusEditCtactOffset();
	afx_msg void OnEnSetfocusEditPullDist();
};
