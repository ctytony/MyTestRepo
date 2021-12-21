// DlgOverView.cpp : implementation file
#include "stdafx.h"
#include "WireBond.h"
#include "DlgOverView.h"
#include "Program.h"
#include "WireBondDlg.h"
#include "..\Bond\Servo.h"

// CDlgOverView dialog
CDlgOverView::CDlgOverView()
{
	m_nRow=1;		//V1.5.W88 add
}

CDlgOverView::~CDlgOverView()
{
}

BEGIN_MESSAGE_MAP(CDlgOverView, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CDlgOverView message handlers
void CDlgOverView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		SetTimer(1000,200,NULL);
	}
	else KillTimer(1000);
}

void CDlgOverView::OnTimer(UINT_PTR nIDEvent)
{
	Sleep(0);

	POINT pt;
	CRect rc;

	GetCursorPos(&pt);
	GetWindowRect(&rc);

	if(rc.PtInRect(pt)==FALSE)ShowWindow(SW_HIDE);

	CDialog::OnTimer(nIDEvent);
}

void CDlgOverView::OnRButtonDown(UINT nFlags, CPoint pt)
{
	CRect rc;
	GetWindowRect(&rc);

	GetParent()->PostMessageW(WM_RBUTTONDOWN, nFlags, MAKELONG(pt.x+rc.left, pt.y+rc.top));

	CDialog::OnRButtonDown(nFlags, pt);
}

void CDlgOverView::DrawMatrixRow()	//V1.5.W88 add
{
	if (thePgm.GetMatrixRow() < 2) return;	//v1.5.T137 edit

	CDC* lpDC=GetDC();

	CFont font;
	font.CreateFontW(10, 0, 0, 0, 100, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Calibri"));

	lpDC->SetBkColor(RGB(0, 0, 0));
	lpDC->SetTextColor(theApp.m_bkColor);

	CFont* lpFont=lpDC->SelectObject(&font);

	CString str=CString(_T("Matrix Row: "))+(char)(m_nRow+0x30)+(char)0x20;
	lpDC->TextOutW(21,8,str,str.GetLength());

	lpDC->SelectObject(lpFont);
	ReleaseDC(lpDC);
}

BOOL CDlgOverView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)	//V1.5.W88 add
{
	if ((zDelta > 0) && (m_nRow > 1))
	{
		m_nRow--;
		DrawMatrixRow();
	}
	else if ((zDelta < 0) && (m_nRow < thePgm.GetMatrixRow()))	//v1.5.T137 edit
	{
		m_nRow++;
		DrawMatrixRow();
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

BYTE CDlgOverView::GetMatrixRow()	//V1.5.W88 add
{
	return m_nRow;
}

BYTE CDlgOverView::SetMatrixRow(BYTE nRow)	//V1.5.W89 add
{
	return m_nRow=nRow;
}
