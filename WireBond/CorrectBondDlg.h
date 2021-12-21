#pragma once

#include "afxwin.h"
#include "LightingCtrlDlg.h"
#include "Jpeg.h"
#include <Gdiplus.h>
using namespace Gdiplus;

// CCorrectBondDlg dialog

class CCorrectBondDlg : public CDialog
{
	DECLARE_DYNAMIC(CCorrectBondDlg)

public:
	CCorrectBondDlg(short nWireNum, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCorrectBondDlg();

// Dialog Data
	enum { IDD = IDD_CORRECT_BOND };
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnUp();
    afx_msg void OnLeft();
    afx_msg void OnDown();
    afx_msg void OnRight();
    afx_msg void OnZUp();
    afx_msg void OnZDown();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnAdjustLight();
    afx_msg void OnFaster();
    afx_msg void OnSlower();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);	//V1.1.W140 add
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);		//V1.1.W140 add
	afx_msg LRESULT OnNcHitTest(CPoint pt);		//V1.1.W140 add
	afx_msg void OnFocusStandbyPower1();
	afx_msg void OnFocusStandbyPower2();
	afx_msg void OnFocusPower1();
	afx_msg void OnFocusPower2();
	afx_msg void OnFocusForce1();
	afx_msg void OnFocusForce2();
	afx_msg void OnFocusTime1();
	afx_msg void OnFocusTime2();
    afx_msg void OnShowParaOffset();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:	//variables
	bool m_bSelectCancel;
public:	//functions
	void SetAdjustMax(short nMax);
private:	//variables
	BOOL m_bShowParaOffset;
	CString m_sAdjustX, m_sAdjustY, m_sAdjStep;
	short m_nWireNum, m_nPower1, m_nPower2;
	short m_nStandbyPower1, m_nStandbyPower2;
	short m_nForce1, m_nForce2, m_nTime1, m_nTime2;
	short m_nAdjStep;	//v2.0.T87 add
	short m_nAdjustX, m_nAdjustY, m_nAdjustMax;
	short m_nCircleRadius;
	CLightingCtrlDlg *m_pLightCtrlDlg;
	Bitmap *m_pbmArrow, *m_pbmFaster, *m_pbmSlower;		//V1.1.W140 add
	CDC m_dcLeft, m_dcRight, m_dcUp, m_dcDown;			//V1.1.W140 add
	CDC m_dcZUp, m_dcZDown, m_dcFaster, m_dcSlower;		//V1.1.W140 add
private:	//functions
	void UpdateBondPara();
	void UpdateDisplay();	//v2.0.T87 add
	void Exit();			//V1.1.W141 add
};
