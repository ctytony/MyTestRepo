#pragma once

#include "afxwin.h"
#include "LightingCtrlDlg.h"
#include "Jpeg.h"
#include <Gdiplus.h>
using namespace Gdiplus;

enum DLG_TYPE 
{
	CORRECT_BOND_DLG,
	BTOS_DLG,	
};

// CPositionAdjustDlg dialog

class CPositionAdjustDlg : public CDialog
{
	DECLARE_DYNAMIC(CPositionAdjustDlg)

public:
	CPositionAdjustDlg(short nDlgType=CORRECT_BOND_DLG, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPositionAdjustDlg();

// Dialog Data
	enum { IDD = IDD_POS_ADJUST };
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
	afx_msg void OnAjustLight();
	afx_msg void OnBtos2ndEnable();
    afx_msg void OnFaster();
    afx_msg void OnSlower();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);	//V1.1.W140 add
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);		//V1.1.W140 add
	afx_msg LRESULT OnNcHitTest(CPoint pt);		//V1.1.W140 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:	//variables
	bool m_bSelectCancel;
public:	//functions
	void SetAdjustMax(short nMax);
private:	//variables
	CString m_sAdjustX, m_sAdjustY, m_sAdjStep;
	short m_nAdjStep;	//v2.0.T87 add
	short m_nAdjustX, m_nAdjustY, m_nAdjustMax;
	short m_nCircleRadius, m_nDlgType;
	BOOL m_b2ndBondEnable;
	CLightingCtrlDlg *m_pLightCtrlDlg;
	Bitmap *m_pbmArrow, *m_pbmFaster, *m_pbmSlower;		//V1.1.W140 add
	CDC m_dcLeft, m_dcRight, m_dcUp, m_dcDown;			//V1.1.W140 add
	CDC m_dcZUp, m_dcZDown, m_dcFaster, m_dcSlower;		//V1.1.W140 add
private:	//functions
	void UpdateDisplay();	//v2.0.T87 add
	void Exit();			//V1.1.W141 add
};
