// VisionView.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "VisionView.h"
#include "VisionCalibDlg.h"
#include "servo.h"

// CVisionView

IMPLEMENT_DYNCREATE(CVisionView, CView)

CVisionView::CVisionView()
{
	lStep = 100;
}

CVisionView::~CVisionView()
{
}

BEGIN_MESSAGE_MAP(CVisionView, CView)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CVisionView drawing

void CVisionView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CVisionView diagnostics

#ifdef _DEBUG
void CVisionView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CVisionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CVisionView message handlers

void CVisionView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_LEFT:
		theServo.StartJogX(MOTION_POSITIVE);
		break;
	case VK_RIGHT:
		theServo.StartJogX(MOTION_NEGATIVE);
		break;
	case VK_UP:
		theServo.StartJogY(MOTION_POSITIVE);
		break;
	case VK_DOWN:
		theServo.StartJogY(MOTION_NEGATIVE);
		break;
	case VK_PRIOR:
		theServo.JogSpeedUp();
		break;
	case VK_NEXT:
		theServo.JogSpeedDown();
		break;
	default:
		break;
	}			
}

void CVisionView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_LEFT:
	case VK_RIGHT:
		theServo.EndJogX();
		break;
	case VK_UP:
	case VK_DOWN:
		theServo.EndJogY();
		break;
	default:
		break;
	}			
}

void CVisionView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnLButtonDown(nFlags, point);
}

