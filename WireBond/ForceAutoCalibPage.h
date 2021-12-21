#pragma once


// CForceCalibPage dialog

class CForceAutoCalibPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CForceAutoCalibPage)

public:
	CForceAutoCalibPage();
	virtual ~CForceAutoCalibPage();

// Dialog Data
	enum { IDD = IDD_FORCE_AUTO_CALI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
    // Current force factor Pa
    float m_fCurForceFactorPa;
    // Current force factor Pb
    float m_fCurForceFactorPb;
    // Current force factor Pc
    float m_fCurForceFactorPc;
    // New force factor Pa
    float m_fNewForceFactorPa;
    // New force factor Pb
    float m_fNewForceFactorPb;
    // New force factor Pc
    float m_fNewForceFactorPc;
    // Apply force for verification
    short m_nApplyForce;
    // Measure force of verification
    double m_dMeasureForce;
public:
    afx_msg void OnEnSetfocusEditApplyForce();
    afx_msg void OnEnSetfocusEditMeasureForce();
    afx_msg void OnBnClickedButtonNext();
    afx_msg void OnBnClickedButtonStart();
private:
    // Current operation is calibration
    BOOL m_bVerifyEnable;
    // Calibration step
    short m_nCalibStep;
    // Contact level
    long m_lCtactLvl;
    // Backup force out DAC
    double m_dForceCalibOutputDac[3];
    double m_dForceCalibMeasureForce[2];
    int m_lBkupDac;

public:
void ThreadDoForceCalibration();
void ThreadDoForceVerification();
void RunPosnForceCaliThread();
CWinThread* m_pWinThreadPosnForceCali;
BOOL mFlagStopPosnForceCaliThread;



afx_msg void OnDestroy();
};
