//FlatButton.cpp : implementation file
#include "stdafx.h"
#include "FlatButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CFlatButton
CFlatButton::CFlatButton()
{
	m_bOnTop=FALSE;
	m_nTimerID=1;

	m_crBk=RGB(255,255,255);
}

CFlatButton::~CFlatButton()
{
}

void CFlatButton::SetBkColor(COLORREF cr)
{
	m_crBk=cr;
}

void CFlatButton::SetImage(UINT nID)
{
	m_bmBk.LoadBitmapW(nID);
}

BEGIN_MESSAGE_MAP(CFlatButton,CButton)
	//{{AFX_MSG_MAP(CFlatButton)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//CFlatButton message handlers
void CFlatButton::OnMouseMove(UINT nFlags,CPoint pt) 
{
	if(m_bmBk.m_hObject!=NULL&&m_bOnTop==FALSE)
	{
		m_bOnTop=TRUE;
		Invalidate();

		SetTimer(m_nTimerID,200,NULL);
	}

	CButton::OnMouseMove(nFlags, pt);
}

void CFlatButton::OnTimer(UINT nIDEvent) 
{
	Sleep(0);

	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	CRect rect;
	GetClientRect(rect);

	if(rect.PtInRect(pt)==FALSE||IsWindowEnabled()==FALSE)
	{
		m_bOnTop=FALSE;
		KillTimer(m_nTimerID);

		Invalidate();
	}

	CButton::OnTimer(nIDEvent);
}

void CFlatButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	if(m_bmBk.m_hObject==NULL)return;

	CRect rect(lpDIS->rcItem);

	FillRect(lpDIS->hDC,&rect,CBrush(m_crBk));

	if(lpDIS->itemState&ODS_SELECTED)
		DrawFrameControl(lpDIS->hDC,rect,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);
	else if(m_bOnTop)
		FrameRect(lpDIS->hDC,&rect,CBrush(RGB(128,128,255)));

	BITMAP bm;
	m_bmBk.GetBitmap(&bm);

	HDC hDC=CreateCompatibleDC(lpDIS->hDC);
	HGDIOBJ hOld=SelectObject(hDC,m_bmBk);

	BLENDFUNCTION bf={AC_SRC_OVER,0,lpDIS->itemState&ODS_DISABLED ? 0x6f : 0xff,0};
	AlphaBlend(lpDIS->hDC,rect.left+m_bOnTop,rect.top+m_bOnTop,rect.Width()-m_bOnTop*2,rect.Height()-m_bOnTop*2,hDC,0,0,bm.bmWidth,bm.bmHeight,bf);

	SelectObject(hDC,hOld);
	DeleteDC(hDC);
}

void CFlatButton::OnEnable(BOOL bEnable)
{
	CButton::OnEnable(bEnable);
	Invalidate();
}
