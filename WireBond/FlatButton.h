#if !defined(AFX_FlatButton_H__10CEB9E1_11F9_11D4_A2EA_0048543D92F7__INCLUDED_)
#define AFX_FlatButton_H__10CEB9E1_11F9_11D4_A2EA_0048543D92F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//FlatButton.h : header file

//CFlatButton window
class CFlatButton : public CButton
{
public:

	CFlatButton();
	virtual ~CFlatButton();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

	void SetBkColor(COLORREF cr);
	void SetImage(UINT nID);

	afx_msg void OnEnable(BOOL bEnable);

protected:

	//{{AFX_MSG(CFlatButton)
	afx_msg void OnMouseMove(UINT nFlags,CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	BYTE m_bOnTop;
	UINT m_nTimerID;

	COLORREF m_crBk;

	CBitmap m_bmBk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FlatButton_H__10CEB9E1_11F9_11D4_A2EA_0048543D92F7__INCLUDED_)
