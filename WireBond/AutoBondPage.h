#pragma once

#include "PositionAdjustDlg.h"
#include "CorrectBondDlg.h"
#include "Wire.h"

// CAutoBondPage dialog

class CAutoBondPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAutoBondPage)
public:
	CAutoBondPage();   // standard constructor
	virtual ~CAutoBondPage();
// Dialog Data
	enum { IDD = IDD_AUTO_BOND };
public:
	virtual BOOL OnInitDialog();
    afx_msg void OnIll();
    afx_msg void OnAutoPad();
	afx_msg void OnSelfDefine();
    afx_msg void OnEditBondPoint();
	afx_msg void OnLastBond();
    afx_msg void OnCompleteBond();
    afx_msg void OnFocusWireNum();
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnStopAll();
    afx_msg void OnRedoPr();
    afx_msg void OnPrev();
    afx_msg void OnNext();
    afx_msg void OnNsop();
    afx_msg void OnNsol();
    afx_msg void OnBallDetect();
    afx_msg void OnDryRun();
    afx_msg void OnVision();
    afx_msg void OnAutoIndex();
    afx_msg void OnEditWireNum();
    afx_msg void OnDummyBond();
	//afx_msg void OnSingleBond();	//v2.0.T39 delete
    afx_msg void OnCorrectBond();
    afx_msg void OnBondTipOffset();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();	//V1.1.W151 add
	afx_msg void OnRework();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:	//functions
    void DisableButtonForAutoBond(bool bIsDisable);
	int WaitN2h2Stablized(void);
	void StartAutoBond(void);
	void StartSingleBond(void);		//v2.0.T39 add
	void StopAutoBond(void);
	static void Clean();	//V1.1.W141 add
	static BYTE IsDirty();	//V1.1.W141 add
private:	//variables
	BOOL m_bBusy;	//v3.1.T448 add
    BOOL m_bIll;	//intelligent lead locate
    BOOL m_bCheckNsop;
    BOOL m_bCheckNsol;
    BOOL m_bCheckBall;
    BOOL m_bDryRun;
    BOOL m_bBondNoVision;
    BOOL m_bAutoIndex;
    BOOL m_bCompleteBond;
	BOOL m_bLastBond;
    BOOL m_bAutoDefineBond;
    short m_nCurWire, m_nCurBondPt;
	CFont m_font;
	short m_nReworkRow, m_nReworkCol, m_nReworkWire;
	static BYTE m_nCount;	//V1.1.W141 add
private:	//functions
	void DuplicatePoints(EN_BOND_PT nBondPt, WIRE_RECORD* pstCurWireRcd, TBL_POS stEditOffset);
	bool ShowCorrectBondDlg();
	bool ShowPosAdjDlg();
	short CalcWireNum();
};
