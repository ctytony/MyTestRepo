#if !defined(AFX_ColorButton_H__8E7A4504_82E5_4C81_8A30_642AC65A4550__INCLUDED_)
#define AFX_ColorButton_H__8E7A4504_82E5_4C81_8A30_642AC65A4550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//

class CColorButton : public CButton
{
public:

	CColorButton();
	virtual ~CColorButton();

	void SetBkColor(COLORREF cr);
	void SetCheck(BYTE bCheck);

protected:

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()

	COLORREF m_crBk;
	BYTE m_bCheck;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ColorButton_H__8E7A4504_82E5_4C81_8A30_642AC65A4550__INCLUDED_)
