#pragma once

// CBallShapeParaPage dialog
class CBallShapeParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBallShapeParaPage)
public:
	CBallShapeParaPage();
	virtual ~CBallShapeParaPage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_BALL_SHAPE_PARA };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusBaseHeight();
    afx_msg void OnFocusSpanLength();
    afx_msg void OnFocusMotionSpeed();
    afx_msg void OnFocusScrubDist();
    afx_msg void OnFocusBallThickness();
    afx_msg void OnFocusDirectionAngle();
    afx_msg void OnBumpShapeMode();
    afx_msg void OnFocusFlexLength();
    afx_msg void OnFocusFlexAngle();
    afx_msg void OnFocusFlexHt();
    afx_msg void OnFocusCuttingPwr();
    afx_msg void OnFocusCuttingPwrDelay();
	afx_msg void OnDestroy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:	//functions
    void UpdateMenuVariable(void);
private:	//variables
    int m_iBallType;
    short m_nBaseHt;
    short m_nSpanLen;
    short m_nMotionSpeed;
    short m_nScrubDist;
    short m_nBallThickness;
    short m_nDirAngle;
    int m_iBumpShapeMode;
    short m_nFlexLen;
    short m_nFlexAngle;
    short m_nFlexHt;
    short m_nCutPower;
    short m_nCutPowerDelay;
};
