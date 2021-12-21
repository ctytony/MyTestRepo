// InitDlg.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "InitDlg.h"
#include "MachineConfig.h"
#include "MsgDef.h"
#include "MsgManager.h"
#include "Jpeg.h"

// CInitDlg dialog
CInitDlg::CInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitDlg::IDD, pParent)
{
	m_sInit = theMsg.GetMsg(MSG_INITIALIZING);	//v3.0.T137 add
}

CInitDlg::~CInitDlg()
{
}

BEGIN_MESSAGE_MAP(CInitDlg, CDialog)
    ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CInitDlg message handlers
BOOL CInitDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
	SetWindowPos(NULL, 0, 0, 700, 490, SWP_NOZORDER);
	CenterWindow();
	SetDlgItemText(IDC_STATIC_VERSION, theApp.GetSoftwareVersion());
	return FALSE;
}

HBRUSH CInitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC )
    {
		pDC-> SetBkMode(TRANSPARENT);
		return theApp.m_brNull; 
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);;
}

//v3.0.T137 add
void CInitDlg::ShowInitMsg(CString sMsg)
{
	m_sInit = sMsg;
	Invalidate();
}

//v1.6.T7 add
void CInitDlg::ShowInitMsg(LPTSTR psMsg)
{
	m_sInit = psMsg;
	Invalidate();
}

//v1.6.T7 add
void CInitDlg::ShowInfoMsg(LPTSTR psMsg)
{
	m_sInfo = psMsg;
	Invalidate();
}

void CInitDlg::PostNcDestroy()
{
    CDialog::PostNcDestroy();
    delete this;
}

void CInitDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(), IDR_JPEG_INIDLG);
	jpeg.Paint(dc.m_hDC, rect);

	SetBkMode(dc.m_hDC, TRANSPARENT);

	CFont* pOldFont = dc.SelectObject(theApp.GetFont());

	CSize size;
	GetTextExtentPoint32(dc.m_hDC, m_sInit, m_sInit.GetLength(), &size);
	int nLeft = rect.left + (rect.Width() - size.cx) / 2;
	TextOut(dc.m_hDC, nLeft, 300, m_sInit, m_sInit.GetLength());

	GetTextExtentPoint32(dc.m_hDC, m_sInfo, m_sInfo.GetLength(), &size);
	nLeft = rect.left + (rect.Width() - size.cx) / 2;
	TextOut(dc.m_hDC, nLeft, 339, m_sInfo, m_sInfo.GetLength());

	dc.SelectObject(pOldFont);
}
