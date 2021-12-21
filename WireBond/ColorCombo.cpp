// ColorCombox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorCombo::CColorCombo()
{
	m_nHeight=16;
}

CColorCombo::~CColorCombo()
{
}

void CColorCombo::SetBkColor(COLORREF cr)
{
	m_crBk=cr;
}

void CColorCombo::SetHeight(BYTE nHeight)
{
	m_nHeight=nHeight;
}

BEGIN_MESSAGE_MAP(CColorCombo, CComboBox)
END_MESSAGE_MAP()

void CColorCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);
	CDC* pDC=CDC::FromHandle(lpDIS->hDC);

	CRect itemRC(lpDIS->rcItem);
	COLORREF clrText = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF clrSelected = GetSysColor(COLOR_HIGHLIGHT);

	int nState = lpDIS->itemState;
	if(nState & ODS_SELECTED)
	{
		pDC->SetTextColor(~clrText & 0x00FFFFFF);
		pDC->SetBkColor(clrSelected);
		pDC->FillSolidRect(&itemRC, clrSelected);
	}
	else
	{
		COLORREF clrNormal = GetSysColor(COLOR_WINDOW);

		pDC->SetTextColor(clrText);
		pDC->SetBkColor(clrNormal);
		pDC->FillSolidRect(&itemRC, clrNormal);
	}

	if (lpDIS->itemID != -1)
	{
		CString str;
		GetLBText(lpDIS->itemID, str);

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(str, itemRC, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
}

void CColorCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemHeight=m_nHeight;
}
