#pragma once

// CLoopLastKinkPage dialog

class CLoopLastKinkPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopLastKinkPage)
public:
	CLoopLastKinkPage();
	virtual ~CLoopLastKinkPage();
    virtual void OnOK();
    virtual BOOL OnSetActive();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_LOOP_LAST_KINK };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnEnSetfocusEditKink4WireFeedSpeed();
    afx_msg void OnEnSetfocusEditTwistRevBentSpeed();
    afx_msg void OnEnSetfocusEditTwistFwdBentSpeed();
	afx_msg void OnEnSetfocusEditTwistFwdHtOffset();
    afx_msg void OnEnSetfocusEditSlopeLength();
    afx_msg void OnEnSetfocusEditCtactAngle();
    afx_msg void OnEnSetfocusEditTwistRatio();
public:
    int UpdateMenuVariable(void);
private:
    short m_nSlopeLen;	//v3.1.T163 rename
    short m_nCtactAngleCorrect;	//v3.1.T163 rename
    short m_nTwistRatio;
    short m_nTwistFwdHtOffset;
    short m_nTwistWireFeedSpeed;	//v3.1.T163 rename
    short m_nTwistRevBentSpeed;
    short m_nTwistFwdBentSpeed;
};
