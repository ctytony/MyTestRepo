#pragma once

#include <Gdiplus.h>
using namespace Gdiplus;

// CDlgElevCtrl dialog

enum EN_TOP_ELEV
{
	TOP_ELEV_ONLOAD,
	TOP_ELEV_OFFLOAD,
};

class CDlgElevCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgElevCtrl)
public:
	CDlgElevCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgElevCtrl();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_TOP_ELEV_CTRL };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	//V1.1.W150 add
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);	//V1.1.W150 add
	afx_msg void OnFocusCurrentSlot();
	afx_msg void OnElevInput();
	afx_msg void OnElevOutput();
	afx_msg void OnEnChangeEditCurrentSlot();
	afx_msg void OnUpSlot();
	afx_msg void OnDownSlot();
	afx_msg void OnElevEject();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	bool IsPusherError();
	bool IsLeadframeOnTrack(short nElevType);
	bool IsMagBelow(short nElevType);
private:
	void ShowErrMsg(short nMsgID);
	short m_nCurSlot;
	short m_nElevID;
	Bitmap *m_pbmArrow;		//V1.1.W150 add
	CDC m_dcUp, m_dcDown;	//V1.1.W150 add
};
