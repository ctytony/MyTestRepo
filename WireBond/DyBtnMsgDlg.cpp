/***************************************************************
DyBtnMsgDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "DyBtnMsgDlg.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "Jpeg.h"

extern CJpeg theJpegHelp;

// CDyBtnMsgDlg dialog
IMPLEMENT_DYNAMIC(CDyBtnMsgDlg, CDialog)

CDyBtnMsgDlg::CDyBtnMsgDlg(short nBtnOKMsgID,short nBtnCancelMsgID, CWnd* pParent /*=NULL*/)
	: CDialog(CDyBtnMsgDlg::IDD, pParent)
	, m_nOkBtnText(0)
	, m_nStopBtnText(0)
	, m_sMsg0(_T(""))
	, m_sMsg1(_T(""))
	, m_sMsg2(_T(""))
	, m_sMsg3(_T(""))
	, m_bOkSelected(false)
	, m_bCancelSelected(false)
{
	m_nOkBtnText = nBtnOKMsgID;
	m_nStopBtnText = nBtnCancelMsgID;
}

CDyBtnMsgDlg::~CDyBtnMsgDlg()
{
}

void CDyBtnMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MSG_LIST0, m_sMsg0);
	DDX_Text(pDX, IDC_MSG_LIST1, m_sMsg1);
	DDX_Text(pDX, IDC_MSG_LIST2, m_sMsg2);
	DDX_Text(pDX, IDC_MSG_LIST3, m_sMsg3);
}


BEGIN_MESSAGE_MAP(CDyBtnMsgDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CDyBtnMsgDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDyBtnMsgDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDyBtnMsgDlg message handlers

BOOL CDyBtnMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add
    SetWindowPos(NULL, 510, LIGHT_DLG_TOP, 470, LIGHT_DLG_HEIGHT, SWP_NOZORDER);
    
    CPoint pt;

    CWnd* pWnd = GetDlgItem(IDC_MSG_TITLE);
	pWnd->SetWindowPos(NULL, 135, 15, 250, 100, SWP_NOZORDER);
	
	font.CreateFontW(38, 0, 0, 0, 500, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	pWnd->SetFont(&font);

	pt.x = 130;
    pt.y = 240;

    pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(m_nOkBtnText));

	pt.x += BUTTON_BOND_WIDTH;

    pWnd = GetDlgItem(IDCANCEL);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, BUTTON_BOND_WIDTH, BUTTON_BOND_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(m_nStopBtnText));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDyBtnMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
    {
		pDC-> SetBkMode(TRANSPARENT);
		return theApp.m_brNull;  
    }

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDyBtnMsgDlg::OnPaint()
{
  	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	theJpegHelp.Paint(dc.m_hDC, rect);
}

void CDyBtnMsgDlg::OnBnClickedOk()
{
	m_bOkSelected = true;
	OnOK();
}

void CDyBtnMsgDlg::OnBnClickedCancel()
{
    m_bCancelSelected = true;
	OnOK();
}

int CDyBtnMsgDlg::SetMessageList(short nMsgNum, short *pnMsgIndex)
{
	if (nMsgNum > 0)
		m_sMsg0 = theMsg.GetMsg(*(pnMsgIndex + 0));
	if (nMsgNum > 1)
		m_sMsg1 = theMsg.GetMsg(*(pnMsgIndex + 1));
	if (nMsgNum > 2)
		m_sMsg2 = theMsg.GetMsg(*(pnMsgIndex + 2));
	if (nMsgNum > 3)
		m_sMsg3 = theMsg.GetMsg(*(pnMsgIndex + 3));

    return 0;
}

int CDyBtnMsgDlg::SetMessageList(CStringList& sList)
{
    short nMsgNum = sList.GetSize();

	if( nMsgNum  > 4 )
	{
		if (nMsgNum-4 > 0)
			m_sMsg0 = sList.GetAt(sList.FindIndex(nMsgNum-4));
		if (nMsgNum-3 > 0)
			m_sMsg1 = sList.GetAt(sList.FindIndex(nMsgNum-3));
		if (nMsgNum-2 > 0)
			m_sMsg2 = sList.GetAt(sList.FindIndex(nMsgNum-2));
		if (nMsgNum-1  > 0)
			m_sMsg3 = sList.GetAt(sList.FindIndex(nMsgNum-1));
	}
	else
	{
		if (nMsgNum > 0)
			m_sMsg0 = sList.GetAt(sList.FindIndex(0));
		if (nMsgNum > 1)
			m_sMsg1 = sList.GetAt(sList.FindIndex(1));
		if (nMsgNum > 2)
			m_sMsg2 = sList.GetAt(sList.FindIndex(2));
		if (nMsgNum > 3)
			m_sMsg3 = sList.GetAt(sList.FindIndex(3)); 
	}

    return 0;
}
