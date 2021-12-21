#include "StdAfx.h"
#include "ColorMenu.h"

CColorMenu::CColorMenu(void)
{
	m_crBk=RGB(255,255,255);
	m_lpFont=NULL;
	m_nHeight=16;

	m_nCount=0;
	m_lpstrBuf=NULL;
}

CColorMenu::~CColorMenu(void)
{
	if(m_lpstrBuf)delete[] m_lpstrBuf;
}

void CColorMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if(lpDIS->itemData<0||lpDIS->itemData>m_nCount-1)return;

	CDC* pDC=CDC::FromHandle(lpDIS->hDC);

	CRect itemRC(lpDIS->rcItem);
	COLORREF clrText = GetSysColor(COLOR_WINDOWTEXT);
	COLORREF clrSelected = GetSysColor(COLOR_HIGHLIGHT);

	pDC->FillSolidRect(&lpDIS->rcItem, m_crBk);
	if((lpDIS->itemState&ODS_SELECTED)&&(lpDIS->itemAction&(ODA_SELECT|ODA_DRAWENTIRE)))
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

	pDC->SetBkMode(TRANSPARENT);

	if(m_lpFont)pDC->SelectObject(m_lpFont->m_hObject);
	pDC->DrawText(m_lpstrBuf[lpDIS->itemData], itemRC, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

void CColorMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemWidth=m_nWidth;
	lpMIS->itemHeight=m_nHeight;
}

void CColorMenu::SetBkColor(COLORREF cr)
{
	m_crBk=cr;
}

void CColorMenu::SetSize(BYTE nWidth, BYTE nHeight)
{
	m_nWidth=nWidth;
	m_nHeight=nHeight;
}

void CColorMenu::SetFont(CFont* lpFont)
{
	m_lpFont=lpFont;
}

void CColorMenu::AllocBuf(BYTE nCount)
{
	if(m_lpstrBuf)delete[] m_lpstrBuf;
	m_lpstrBuf=new CString[nCount];
}

void CColorMenu::AppendItem(UINT nID, CString& strItem)
{
	if(m_lpstrBuf==NULL)return;

	m_lpstrBuf[m_nCount]=strItem;
	AppendMenu(MF_STRING|MF_OWNERDRAW, nID, (LPCTSTR)m_nCount++);
}
