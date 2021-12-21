#pragma once

#include "atltypes.h"

// CWaveForm dialog

class CWaveForm : public CDialog
{
	DECLARE_DYNAMIC(CWaveForm)

public:
	CWaveForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaveForm();

// Dialog Data
	enum { IDD = IDD_WAVEFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnPaint();
    afx_msg void OnBnClickedRadioTime();
    afx_msg void OnBnClickedRadioSpace();
    afx_msg void OnBnClickedCheckRvx();
    afx_msg void OnBnClickedCheckFvx();
    afx_msg void OnBnClickedCheckRpx();
    afx_msg void OnBnClickedCheckFpx();
    afx_msg void OnBnClickedCheckRvy();
    afx_msg void OnBnClickedCheckFvy();
    afx_msg void OnBnClickedCheckRpy();
    afx_msg void OnBnClickedCheckFpy();
    afx_msg void OnBnClickedCheckRvz();
    afx_msg void OnBnClickedCheckFvz();
    afx_msg void OnBnClickedCheckRpz();
    afx_msg void OnBnClickedCheckFpz();
    afx_msg void OnBnClickedCheckDw();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnNMReleasedcaptureSliderBondPt(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonSave();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedCheckDz();
public:	//variables
    CString m_sDeformInfo;
private:	//variables
    CPen m_pen[MAX_SCOPE_DATA + 1];
    BOOL m_bRefVelX;		//ref vel x
    BOOL m_bFbVelX;			//feedback vel x
    BOOL m_bRefPosX;		//ref pos x
    BOOL m_bFbPosX;			//feedback pos x
    BOOL m_bRefVelY;		//ref vel y
    BOOL m_bFbVelY;			//feedback vel y
    BOOL m_bRefPosY;		//ref pos y
    BOOL m_bFbPosY;			//feedback pos y
    BOOL m_bRefVelZ;		//ref vel z
    BOOL m_bFbVelZ;			//feedback vel z
    BOOL m_bRefPosZ;		//ref pos z
    BOOL m_bFbPosZ;			//feedback pos z
    BOOL m_bDcomZ;
    BOOL m_bDcomWcl;
    BOOL m_bShowTraj;		//space
    double m_adData[MAX_SCOPE_DATA * SERVO_SCOPE_DATA_LENGTH];
    int m_anSpData[SERVO_SP_SCOPE_DATA_LENGTH];	//SP Position Error of Z
    short m_bZoomIn;
    CPoint m_TopLeftPt;
    CPoint m_BottomRightPt;
    double m_dFactorX;
    double m_dFactorY;
    CPoint m_OriPt;
    int m_iServoStatus;
    int m_iBondPtCount;		//bond point count of the shown waveform
    CSliderCtrl m_BondPtSlider;
private:	//functions
    void CalcDeformInfo(void);
    int ReadMotionDataFromFile(double* pdWaveformData);
    void ShowTraj(void);
    void ShowWaveform(void);
};
