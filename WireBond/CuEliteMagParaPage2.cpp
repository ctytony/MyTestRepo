#include "stdafx.h"
#include "WireBond.h"
#include "CuEliteMagParaPage2.h"
#include "Macdef.h"
#include "MenuConfig.h"

IMPLEMENT_DYNAMIC(CCuEliteMagParaPage2, CDialog)

CCuEliteMagParaPage2::CCuEliteMagParaPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CCuEliteMagParaPage2::IDD, pParent)
	, pInfoView(NULL)
{

}

CCuEliteMagParaPage2::~CCuEliteMagParaPage2()
{
}

void CCuEliteMagParaPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCuEliteMagParaPage2, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSensorLayoutPage message handlers

BOOL CCuEliteMagParaPage2::OnInitDialog()
{

	this->SetWindowPos(NULL, VISION_VIEW_LEFT, 55, 775, 705, SWP_NOZORDER);

	/*pInfoView = GetDlgItem(IDC_IMG_SENSOR);
	pInfoView->SetWindowPos(NULL, 0, 0, 775, 700, SWP_NOZORDER);*/



	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if (theApp.GetSysOperationMode() != SYS_OFFLINE)
    {
	    SetTimer(1,300,NULL);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CCuEliteMagParaPage2::OnPaint()
{

	     CPaintDC   dc(this);      
         CRect   rect;      
         GetClientRect(&rect);      
         CDC   dcMem;      
         dcMem.CreateCompatibleDC(&dc);      
         CBitmap   bmpBackground;      
         bmpBackground.LoadBitmap(IDB_IMG_MAGAZINE);      
    //IMAGE ID      
         BITMAP   bitmap;      
         bmpBackground.GetBitmap(&bitmap);      
         CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);      
         dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,      
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

}


BOOL CCuEliteMagParaPage2::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}
