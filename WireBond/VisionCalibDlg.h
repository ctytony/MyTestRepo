#pragma once

// CVisionCalibDlg dialog
class CVisionCalibDlg : public CDialog
{
	DECLARE_DYNAMIC(CVisionCalibDlg)

public:
	CVisionCalibDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVisionCalibDlg();

// Dialog Data
	enum { IDD = IDD_VISION_CALI };

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCalibrate();
	afx_msg void OnMag();
    afx_msg void OnSizeX();
    afx_msg void OnSizeY();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();		//V1.1.W142 add
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W142 add
    virtual void PostNcDestroy();
private:	//variables
	// Vision factor menu string
	float m_fHighMagFactorX;
	float m_fHighMagFactorY;
	float m_fLowMagFactorX;
	float m_fLowMagFactorY;
	// Calibration high mag or low mag
	int m_iCalibMag;
    short m_nPatternSizeX;
    short m_nPatternSizeY;
	BOOL m_bBusy;	//V1.1.W142 add
private:	//functions
    int CalcVisionFactor(PR_SRCH_RESULT* pstSrchResult, double dSrchStep, TBL_POS* pstVisionFactor, double* pFactCos, double* pFactSin);
public:
	afx_msg void OnBnClickedButtonDone();
};
