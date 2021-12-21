#pragma once

// CVisionCfgDlg dialog
class CVisionCfgDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CVisionCfgDlg)

public:
	CVisionCfgDlg();   // standard constructor
	virtual ~CVisionCfgDlg();

// Dialog Data
	enum { IDD = IDD_VISION_CONFIG };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusShortDelay();
    afx_msg void OnFocusLongDelay();
    afx_msg void OnFocusPrScore();
    afx_msg void OnDieRange();
    afx_msg void OnLeadRange();
    afx_msg void OnPrIndexingLeadRange();
    afx_msg void OnFocusDieTol();
    afx_msg void OnFocusLeadTol();
    afx_msg void OnFocusLeadPrScore();
    afx_msg void OnDestroy();		//V1.1.W259 edit
	afx_msg void OnSinglePr();
	afx_msg void OnDebug();
	afx_msg void OnAutoSkip();
    afx_msg void OnLeadPr();
    afx_msg void OnFocusExposureTime();
    afx_msg void OnFocusExposureTime2();
	afx_msg void OnEditExposureTime();
	afx_msg void OnFocusAutoSkipScore();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
	short m_nAutoSkipScore;
	int m_iManualSkip;
    short m_nShortDistSrchDelay;
    short m_nLongDistSrchDelay;
    int m_lSinglePr;		//Single PR search mode enable
    int m_lPreMoveEnable;
    short m_nPrPassScore;
    short m_nImageTransferTime;
    int m_iAutoSkipEnable;	//Auto skip bad die
    int m_iDieSearchRange;
    int m_iLeadSearchRange;
    int m_iPrIndexSearchRange;
    short m_nDieAlignTol;
    short m_nLeadAlignTol;
    short m_nLeadPrPassScore;
    int m_iLeadPrEnable;
    short m_nExposureTime;	//Camera exposure time in us
    short m_RetryExposureTime;
};
