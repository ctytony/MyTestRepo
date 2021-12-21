#include "stdafx.h"
#include "WireBond.h"
#include "CuEliteLFParaPage2.h"
#include "Macdef.h"
#include "MenuConfig.h"

IMPLEMENT_DYNAMIC(CCuEliteLFParaPage2, CDialog)

CCuEliteLFParaPage2::CCuEliteLFParaPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CCuEliteLFParaPage2::IDD, pParent)
	, pInfoView(NULL)
{

}

CCuEliteLFParaPage2::~CCuEliteLFParaPage2()
{
}

void CCuEliteLFParaPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCuEliteLFParaPage2, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSensorLayoutPage message handlers

BOOL CCuEliteLFParaPage2::OnInitDialog()
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



void CCuEliteLFParaPage2::OnPaint()
{

	     CPaintDC   dc(this);      
         CRect   rect;      
         GetClientRect(&rect);      
         CDC   dcMem;      
         dcMem.CreateCompatibleDC(&dc);      
         CBitmap   bmpBackground;      
         bmpBackground.LoadBitmap(IDB_IMG_LEADFRAME);      
    //IMAGE ID      
         BITMAP   bitmap;      
         bmpBackground.GetBitmap(&bitmap);      
         CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);      
         dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,      
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

}


BOOL CCuEliteLFParaPage2::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}
