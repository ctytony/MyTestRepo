#if !defined(AFX_COLORCOMBOX_H__C83A6741_0950_41B1_90A7_10E666D45621__INCLUDED_)
#define AFX_COLORCOMBOX_H__C83A6741_0950_41B1_90A7_10E666D45621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorCombox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorCombo window

class CColorCombo : public CComboBox
{
public:

 CColorCombo();
 virtual ~CColorCombo();

 void SetBkColor(COLORREF cr);
 void SetHeight(BYTE nHeight);

protected:

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

	DECLARE_MESSAGE_MAP()

	COLORREF m_crBk;
	BYTE m_nHeight;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCOMBOX_H__C83A6741_0950_41B1_90A7_10E666D45621__INCLUDED_)