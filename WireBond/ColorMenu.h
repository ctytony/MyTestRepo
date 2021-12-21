#pragma once
#include "afxwin.h"

class CColorMenu : public CMenu
{
public:

	CColorMenu(void);
	virtual ~CColorMenu(void);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

	void SetBkColor(COLORREF cr);
	void SetSize(BYTE nWidth, BYTE nHeight);
	void SetFont(CFont* lpFont);

	void AllocBuf(BYTE nCount);
	void AppendItem(UINT nID, CString& strItem);

protected:

	COLORREF m_crBk;
	CFont* m_lpFont;

	BYTE m_nWidth,m_nHeight;

	UINT m_nCount;
	CString* m_lpstrBuf;
};
