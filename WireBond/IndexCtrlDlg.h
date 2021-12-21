#pragma once

#include "ColorButton.h"

// CIndexCtrlDlg dialog

class CIndexCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CIndexCtrlDlg)

public:
	CIndexCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIndexCtrlDlg();
    virtual BOOL OnInitDialog();
	void UpdateMatrix();

// Dialog Data
	enum { IDD = IDD_INDEX_CONTROL };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMatPrev();
	afx_msg void OnMatNext();
    afx_msg void OnLfIndex();
    afx_msg void OnRestOrAdjust();
    afx_msg void OnClamp();
	afx_msg void OnClearTrack();
    afx_msg void OnEditIndex();
    afx_msg void OnEditRow();
    afx_msg void OnEditColumn();
    afx_msg void OnFocusIndex();
    afx_msg void OnFocusRow();
    afx_msg void OnFocusColumn();
    afx_msg void OnPrevSubunit();
    afx_msg void OnNextSubunit();
	afx_msg void OnLoadMag();
	afx_msg void OnMovePitchForward();
	afx_msg void OnMovePitchBackward();
	afx_msg void OnEjectMag();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);	//V1.1.W155 add
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
    short m_nPrevUnit, m_nCurUnit, m_nCurRow, m_nCurCol;
	short m_nMatRow, m_nMatCol, m_nCurMat;
    CTypedPtrArray<CObArray, CColorButton*> m_apButton;
	BOOL m_bBusy;	//V1.1.W155 add
private:	//functions
	void FreeButtonArray(void);
    void DisplayTableIndex(short nIndex);
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.1.W155 add
};
