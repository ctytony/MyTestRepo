#pragma once

#include "MachineData.h"
#include "afxwin.h"
#include "EfoProfile.h"

// CSparkParaPage dialog
class CSparkParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSparkParaPage)
public:
	CSparkParaPage();   // standard constructor
	virtual ~CSparkParaPage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_SPARK_PARA };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusTailLength();
    afx_msg void OnFocusTailLen();	//v3.1.T63 add
    afx_msg void OnFocusSparkDelay();
    afx_msg void OnCopperSequence();
    afx_msg void OnFocusFab2TailLength();
    afx_msg void OnTailMonitor();
	afx_msg void OnFocusSeg1I();
	afx_msg void OnFocusSeg1T();
	afx_msg void OnFocusSeg2I();
	afx_msg void OnFocusSeg2T();
	afx_msg void OnSelFab();
	afx_msg void OnFocusGapLargeDac();
	afx_msg void OnEnSetfocusEditTailLengthSmpl();
	afx_msg void OnEnSetfocusEditTailBreakUm();
	afx_msg void OnEnSetfocusEditTailBreakSmpl();
	afx_msg void OnEnSetfocusEditFab2TailLengthSmpl();
	afx_msg void OnEnSetfocusEditFab2TailBreak();
	afx_msg void OnEnSetfocusEditFab2TailBreakSmpl();
	afx_msg void OnFocusSeg3T();
	afx_msg void OnFocusSeg3I();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
	short m_nTailLen;	//v3.1.T63 add
	short m_anTailLen[MAX_PROFILE_NUM];	//v3.1.T61 add
	short m_nSparkDelay;
	short m_nCurrent1, m_nCurrent2, m_nCurrent3;
	short m_nTime1, m_nTime2, m_nTime3;
	short m_nGapLargeDac;
    int m_iCopperSequence, m_iFabType;
	bool m_bStopFiring;
    BOOL m_bTailMonitor;
	short m_nTailTime;
	short m_nTailBreakDist;
	short m_nTailBreakTime;
	short m_nFab2TailTime;
	short m_nFab2TailBreakDist;
	short m_nFab2TailBreakTime;
};
