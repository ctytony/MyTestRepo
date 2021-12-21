// UPSWarnPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "UPSWarnPage.h"

// CUPSWarnPage dialog

IMPLEMENT_DYNAMIC(CUPSWarnPage, CDialog)

CUPSWarnPage::CUPSWarnPage(CWnd* pParent /*=NULL*/)
	: CDialog(CUPSWarnPage::IDD, pParent)
{

}

CUPSWarnPage::~CUPSWarnPage()
{
}

void CUPSWarnPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUPSWarnPage, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CUPSWarnPage message handlers
BOOL CUPSWarnPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add

    SetWindowPos(NULL, 0, 0, 400, 120, SWP_NOZORDER);
	CenterWindow();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_WARN_MSG);
    pWnd->SetWindowPos(NULL,135,20,260,100, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_NO_ELEC_POWER));

	SetTimer(1,5000,NULL);

	return TRUE;
}

void CUPSWarnPage::OnPaint()
{
	CPaintDC   dc(this);      
         CRect   rect;      
         GetClientRect(&rect);      
         CDC   dcMem;      
         dcMem.CreateCompatibleDC(&dc);      
         CBitmap   bmpBackground;    
         bmpBackground.LoadBitmap(IDB_BITMAP_UPS);
  	       
    //IMAGE ID      
         BITMAP   bitmap;      
         bmpBackground.GetBitmap(&bitmap);      
         CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);      
         dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,      
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);     
}

HBRUSH CUPSWarnPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC )
    {
		pDC-> SetBkMode(TRANSPARENT);

		return theApp.m_brNull; 
    }

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


void CUPSWarnPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	OnOK();

	CDialog::OnTimer(nIDEvent);
}

