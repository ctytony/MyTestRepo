#pragma once

enum PR_ALGORITHM
{
	PR_GRAY_LEVEL,
	PR_BINARY, 
	PR_SHAPE_REG, 
};

// CTchVisionPatternPage dialog

class CTchVisionPatternPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTchVisionPatternPage)
public:
	CTchVisionPatternPage();
	virtual ~CTchVisionPatternPage();
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_TCH_VISION_PATTERN };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnStart();
    afx_msg void OnContinue();
    afx_msg void OnLensMag();
    afx_msg void OnEditSizeX();
    afx_msg void OnEditSizeY();
    afx_msg void OnFocusSizeX();
    afx_msg void OnFocusSizeY();
    afx_msg void OnLocateMode();
    afx_msg void OnAlgorithm();
    afx_msg void OnSkip();
    afx_msg void OnMask();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.1.W153 add
	DECLARE_MESSAGE_MAP()
private:
	int m_nCopyFrom;	//v3.1.T422 add - copy pat ID from another die
    int m_nVisionAlgo;	//vision pattern recognition algorithm
    short m_nPatSizeX;
    short m_nPatSizeY;
    int m_nLensMag;
    int m_nLocateMode;		//reference locate mode
	short m_nLastRef;
    short m_nTotalAlignStep;	//total alignment step based on die reference number
    short m_nAlignStep;		//current alignment step
	BOOL m_bBusy;	//V1.1.W153 add
	BYTE CheckPatternQuality();
	LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
};
