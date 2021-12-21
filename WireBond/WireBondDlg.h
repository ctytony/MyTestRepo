/***************************************************************
WireBondDlg.h : implementation file
****************************************************************/

#pragma once

#include "BondToolBar.h"
#include "afxwin.h"
#include "BtnST.h"
#include "datastruct.h"
#include "TeachRecipeSheet.h"
#include "IndexCtrlDlg.h"
#include "LightingCtrlDlg.h"
#include "InitDlg.h"
#include "DlgOverView.h"	//V1.1.W116 add
#include "FlatButton.h"		//V1.1.W128 add
#include <Gdiplus.h>
#include "DlgSpider.h"		//V1.1.W161 add

using namespace Gdiplus;

#define TIMER1_INTERVAL			500
#define TIMER2_INTERVAL			100
#define TIMER3_INTERVAL			60000
#define N2H2_AUTO_OFF_DELAY		50		//second

enum EN_INFOVIEW_TYPE	//v3.0.T110 rename
{
	INFOVIEW_ALL,
	INFOVIEW_WIRE_INFO,
	INFOVIEW_HEATER,
	INFOVIEW_TABLE_POSITION,
};

// CWireBondDlg dialog

class CWireBondDlg : public CDialog
{
// Construction
public:
	CWireBondDlg(CWnd* pParent = NULL);
	~CWireBondDlg();	//V1.1.W116 add
// Dialog Data
	enum { IDD = IDD_WIREBOND };
public:
    afx_msg void OnUnplug();
	afx_msg void OnSpeedUp();
	afx_msg void OnSpeedDown();
	afx_msg void OnElevator();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDebug();
	afx_msg LRESULT OnClickKeyboardShortcut(WPARAM wParam,  LPARAM lParam);
	afx_msg void OnSetup();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnProgram();
	afx_msg void OnBond();
	afx_msg void OnConfig();
	afx_msg void OnMenuTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLight();
    afx_msg void OnIndex();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnUserLvl(UINT nID);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:	//variable
	CIndexCtrlDlg *m_pIndexCtrlDlg;
	CLightingCtrlDlg *m_pLightCtrlDlg;
    // Vision image
    CStatic m_Image;
public:	//function
	void ShowUsgInfoDialog();	//v3.0.T179 add
	void SetProcessKeyboard(bool bProcess);
	int SystemPowerOn(void);
	void SetDrawAreaEnable(HWND hClient);
	void GetScreenResolution(void);
    // Enable or disable tool bar button
    void EnableButton(int nID, BOOL bEnable = FALSE);
    // Table position fine adjust tool bar
    void OnAdjust(void);
    // Thread wire tool bar
    void OnThread(void);
    // Joystick control tool bar
    void OnToolBarJoystickCtrl(void);
	void UpdateMenuTree(short nMenuTreeIndex);
	void InhibitSystemButton(void);
	void EnableSystemButton(void);
    void UpdateOverView(void);
    void UpdateInfoView(EN_INFOVIEW_TYPE nType);	//v3.0.T110 edit
	void ShowButton(int nID, BYTE bShow);	//V1.1.W93 add
	BYTE ContactSearch();		//V1.1.W141 add
	Bitmap *Load(HINSTANCE hInstance, UINT nRes, LPTSTR lpszType);	//V1.1.W147 add
	void CreateBttnDC(CWnd *lpBttn, CDC *lpDC, CDC &dc, Bitmap *lpBM);	//V1.1.W147 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	HICON m_hIcon;
	CToolTipCtrl m_ToolTip;
	// Generated message map functions
	DECLARE_MESSAGE_MAP()
private:	//variable
	BOOL m_bBusy;
	TBL_POS m_st1stMeasurePt;
	bool m_b1stMeasurePtSelected;	//v3.0.T313 add
    // Indicate which menu tree button is pressed.
    short m_nMenuTreeIndex;
    // Icon for menu tree 
    CImageList m_ImageList;
    CDlgOverView *m_pDlgOverView;	//V1.1.W116 edit
    CWnd *m_pInfoView;
	CFlatButton m_btnUnplug, m_btnLight, m_btnIndex;
	CFlatButton m_btnAdjust, m_btnWire, m_btnElevator;
	CFlatButton m_btnUp, m_btnDown;			//V1.1.W128 add
	CFlatButton m_btnSetup, m_btnProgram, m_btnBond;
	CFlatButton m_btnDebug, m_btnConfig;	//V1.1.W128 add
    bool m_bButtonInhibited;
    TBL_POS m_CurPos;
    double m_dCurBondHeadLevel;
    UINT m_iUserLevel;
	short m_nScreenType;
	long m_lOxidationPreventTimer;
	TBL_POS m_stLastPos;
	CPoint m_LeftBtnDownPoint;
	short m_nN2h2TimeInterval;
    // Overview drawing coordinate of XY original point
    TBL_POS m_stOriPos;
    double m_dPlotScaleX;
    double m_dPlotScaleY;
	bool m_bProcessKeyboard;
	CInitDlg *m_pInitDlg;
	HWND m_hwndClient;
	HWND m_hwndFront;		//V1.1.W142 add
	ULONG_PTR m_lToken;
	CDlgSpider* m_pDlgSpider;	//V1.1.W161 add
private:	//function
	void VerifyRecipe(void);	//v2.0.T55 add
	CString GetVisionStatusString(short nVisionStatus);
	void SetDefaultMachineOffset(short nMaterialHandlerType);
	static DWORD WINAPI InitProc(LPVOID lpVoid);
	void AutoPadCentering();
	void ConvertRecipeName(LPWSTR psBuf, BYTE nSize);
	void ShutDown();
	void DisplayUserLevel();	//V1.1.W38 add
	CString GetUserLevel();		//V1.1.W38 add
	void ToggleLanguage();		//V1.1.W50 add
	void MinimizeWindow();		//V1.1.W51 add
	bool LoadRecipe();			//V1.1.W141 add
	BYTE CloseFrontWindow();	//V1.1.W142 add
	BYTE HideFrontWindow();		//V1.1.W155 add
	BYTE SwapFrontWindow(HWND hwndFront);	//V1.1.W142 add
	void SetTooltip();			//V1.1.W149 add
	BYTE CalcMatrixRow();		//V1.5.W89 add
};
