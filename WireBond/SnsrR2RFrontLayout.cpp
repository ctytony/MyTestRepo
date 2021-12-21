/***************************************************************
SnsrTopFrontLayout.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "SnsrR2RFrontLayout.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "R2RIndex.h"
#include "R2ROnload.h"
#include "R2ROffload.h"
#include "Jpeg.h"

// CSensorLayoutPage dialog

IMPLEMENT_DYNAMIC(CSnsrR2RFrontLayout, CDialog)

CSnsrR2RFrontLayout::CSnsrR2RFrontLayout(CWnd* pParent /*=NULL*/)
	: CDialog(CSnsrR2RFrontLayout::IDD, pParent)
	, pInfoView(NULL)
{

}

CSnsrR2RFrontLayout::~CSnsrR2RFrontLayout()
{
}

void CSnsrR2RFrontLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSnsrR2RFrontLayout, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSensorLayoutPage message handlers

BOOL CSnsrR2RFrontLayout::OnInitDialog()
{
	SetWindowPos(NULL, 495, 60, 785, 705, SWP_NOZORDER);	//V1.1.W25 edit

	/*pInfoView = GetDlgItem(IDC_IMG_SENSOR);
	pInfoView->SetWindowPos(NULL, 0, 0, 775, 700, SWP_NOZORDER);*/



	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if (!theMcConfig.IsBypassAll())
    {
	    SetTimer(1,300,NULL);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSnsrR2RFrontLayout::OnPaint()
{
  	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_TOP_FRONT);
	jpeg.Paint(dc.m_hDC,rect); 

	MonitorSensorState();
}

int CSnsrR2RFrontLayout::UpdateSensorLayout(POINT point,BOOL bColor)
{

	CRect   oUpdateRect;
    CDC* pCtrlDC = this->GetDC();
	CBrush oGreenBrush(RGB(0, 255, 0));
    CBrush oRedBrush(RGB(255, 0, 0));
	CPen pen(PS_NULL,1,RGB(255,0,0));
	CSize sz(20, 20);
	   
	oUpdateRect = CRect(point,sz);
	pCtrlDC->SelectObject(&pen);

	if(bColor==FALSE)
	pCtrlDC->SelectObject(&oGreenBrush);
	else
    pCtrlDC->SelectObject(&oRedBrush);

    pCtrlDC->Ellipse(oUpdateRect);  
    this->ReleaseDC(pCtrlDC);
	


	return 0;
}

void CSnsrR2RFrontLayout::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	MonitorSensorState();

	CDialog::OnTimer(nIDEvent);
}

BOOL CSnsrR2RFrontLayout::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll())
    {
	   KillTimer(1);
	}

	return CDialog::DestroyWindow();
}

int CSnsrR2RFrontLayout::MonitorSensorState(void)
{
	BOOL bHome, bPLimit, bNLimit;
	int iSensorState = SENSOR_OFF;

    CPoint OnloadStopBondSnr(25,100);
	CPoint OnloadErrorSnr(25,120);

    CPoint OffloadStopBondSnr(705,100);
	CPoint OffloadErrorSnr(705,120);

	CPoint AlignSnr(263,115);
	CPoint EjectJamSnr(567,235);

	CPoint IndexerHome(227,345);
	CPoint IndexerLimit(433,378);
	CPoint ClamperHome(335,490);

	theStepper.GetHomeSensor(TOP_WINDOW_CLAMPER,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(ClamperHome,bHome);

	theStepper.GetHomeSensor(TOP_DEVICE_INDEXER,&bHome,&bNLimit,&bPLimit);
	//UpdateSensorLayout(IndexerLimit,bNLimit); 
	UpdateSensorLayout(IndexerLimit,bPLimit);
	UpdateSensorLayout(IndexerHome,bHome);

    theR2ROnloader.theStopBondSnr.UpdateSensorState();   
	iSensorState=theR2ROnloader.theStopBondSnr.GetSensorState();
	UpdateSensorLayout(OnloadStopBondSnr,iSensorState);

    theR2ROnloader.theErrorSnr.UpdateSensorState();   
	iSensorState=theR2ROnloader.theErrorSnr.GetSensorState();
	UpdateSensorLayout(OnloadErrorSnr,iSensorState);

    theR2ROffloader.theStopBondSnr.UpdateSensorState();   
	iSensorState=theR2ROffloader.theStopBondSnr.GetSensorState();
	UpdateSensorLayout(OffloadStopBondSnr,iSensorState);

    theR2ROffloader.theErrorSnr.UpdateSensorState();  
	iSensorState=theR2ROffloader.theErrorSnr.GetSensorState();
	UpdateSensorLayout(OffloadErrorSnr,iSensorState);

	theR2RTrack.theAlignSnr.UpdateSensorState();
    iSensorState=theR2RTrack.theAlignSnr.GetSensorState();
	UpdateSensorLayout(AlignSnr,iSensorState);

	theR2RTrack.theEjectJamSnr.UpdateSensorState();
    iSensorState=theR2RTrack.theEjectJamSnr.GetSensorState();
	UpdateSensorLayout(EjectJamSnr,iSensorState);

	return 0;
}


