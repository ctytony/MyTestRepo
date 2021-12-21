#pragma once

#include "Program.h"

// CTchSequencePage dialog
class CTchSequencePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTchSequencePage)
public:
	CTchSequencePage(UINT nID);	//V1.5.W100 edit
	virtual ~CTchSequencePage();
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
// Dialog Data
	enum { IDD = IDD_EDIT_SEQ };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnStart();
    afx_msg void OnContinue();
    afx_msg void OnBondSeq();
    afx_msg void OnFocusRow();
    afx_msg void OnFocusColumn();
    afx_msg void OnProgramType();
    afx_msg void OnFocusAheadCol();
    afx_msg void OnBack();
    afx_msg void OnFocusMatRow();
    afx_msg void OnFocusMatCol();
	afx_msg void OnDestroy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W153 add
	DECLARE_MESSAGE_MAP()
public:	//functions
    void UpdateSubUnitLayout(void);
    void FreeButtonArray(void);
private:	//variables
    int m_nDevSeq;
    short m_nUnitRow;
    short m_nUnitCol;
    int m_nBondSeqOrientation;	//v2.0.T36 rename
    CTypedPtrArray<CObArray, CButton *> m_pButtonArray;
    short m_nTeachStep;
    TBL_POS m_stTopRightPt;
    TBL_POS m_stBottomLeftPt;
	TBL_POS m_stTopLeftPt;
    TBL_POS m_stMatRow2TopRightPt;
	TBL_POS m_stMatCol2TopRightPt;
    CPoint m_LayoutStartPt;
    short m_nUnitWidth;
    short m_nAheadCol;
    short m_nMatRow;
    short m_nMatCol;
    int m_nTrackType;
    TBL_POS m_stTrackOffset;
	BOOL m_bBusy;	//V1.1.W153 add
private:	//functions
    void CaliUnitPitch(void);
    void PlotSeqArrow(CPoint* pCurPt, CPoint* pNextPt, CDC* pCtrlDC);
    void UpdateDevSeq(void);
};
