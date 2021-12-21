// DlgSpider.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "DlgSpider.h"
#include "MenuConfig.h"
#include "VisionSystem.h"

// CDlgSpider dialog

CDlgSpider::CDlgSpider()
{
	m_nSize = 1;
	m_nPos = 0;
}

CDlgSpider::~CDlgSpider()
{
}

BEGIN_MESSAGE_MAP(CDlgSpider, CDialog)
	ON_MESSAGE(WM_REFRESH, OnRefresh)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDlgSpider message handlers
BOOL CDlgSpider::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_brush.CreateSolidBrush(RGB(33,33,33));
	MoveWindow(m_nPos, 0, m_nSize, m_nSize);
	return FALSE;
}

LRESULT CDlgSpider::OnRefresh(WPARAM wParam, LPARAM lParam)
{
	MoveWindow(++m_nPos, 0, m_nSize, m_nSize);
	if (m_nPos >= VISION_VIEW_WIDTH) m_nPos = 0;
	return NOERROR;
}

HBRUSH CDlgSpider::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_brush;
}
