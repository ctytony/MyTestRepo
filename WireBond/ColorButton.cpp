// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorButton::CColorButton()
{
	m_crBk=RGB(255,255,255);
	m_bCheck=FALSE;
}

CColorButton::~CColorButton()
{
}

void CColorButton::SetBkColor(COLORREF cr)
{
	m_crBk=cr;
}

void CColorButton::SetCheck(BYTE bCheck)
{
	m_bCheck=bCheck;
}

BEGIN_MESSAGE_MAP(CColorButton, CButton)
END_MESSAGE_MAP()

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;

	UINT nState = lpDrawItemStruct->itemState;	//Get state of the button

	dc.Attach(lpDrawItemStruct->hDC);		//Get device context object
	CRect rt = lpDrawItemStruct->rcItem;		//Get button rect

	if ( nState & ODS_DISABLED )		// If it is disabled
	{
		dc.FillSolidRect(rt, RGB(244,244,244));		//Fill button with gray color
		dc.DrawEdge(rt,EDGE_SUNKEN,BF_RECT);    // Draw a sunken face
	}
	else
	{
		dc.FillSolidRect(rt, m_crBk);		//Fill button with bk color
		dc.DrawEdge(rt,nState&ODS_SELECTED||m_bCheck ? EDGE_SUNKEN : EDGE_RAISED,BF_RECT);    // If it is pressed
	}

	CString str;
	GetWindowText(str);		// Get the caption which have been set

	dc.SetTextColor(nState&ODS_DISABLED ? RGB(128,128,128) : RGB(10,10,10));
	dc.DrawText(str,rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE);		// Draw out the caption

	if ( nState & ODS_FOCUS )       // If the button is focused
	{
		// Draw a focus rect which indicates the user 
		// that the button is focused
		int iChange = 3;
		rt.top += iChange;
		rt.left += iChange;
		rt.right -= iChange;
		rt.bottom -= iChange;

		dc.DrawFocusRect(rt);
	}

	dc.Detach();
}
