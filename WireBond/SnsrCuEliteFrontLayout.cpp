/***************************************************************
SnsrTopFrontLayout.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "SnsrCuEliteFrontLayout.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "Jpeg.h"
#include "CuIndex.h"

// CSensorLayoutPage dialog
IMPLEMENT_DYNAMIC(CSnsrCuELiteFrontLayout, CDialog)

CSnsrCuELiteFrontLayout::CSnsrCuELiteFrontLayout(CWnd* pParent /*=NULL*/)
	: CDialog(CSnsrCuELiteFrontLayout::IDD, pParent)
	, pInfoView(NULL)
{
}

CSnsrCuELiteFrontLayout::~CSnsrCuELiteFrontLayout()
{
}

void CSnsrCuELiteFrontLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnsrCuELiteFrontLayout, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSensorLayoutPage message handlers
BOOL CSnsrCuELiteFrontLayout::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);	//V1.1.W25 edit

	/*pInfoView = GetDlgItem(IDC_IMG_SENSOR);
	pInfoView->SetWindowPos(NULL, 0, 0, 775, 700, SWP_NOZORDER);*/

	if (!theMcConfig.IsBypassAll())
    {
	    SetTimer(1,300,NULL);
    }

	return CDialog::OnInitDialog();
}

void CSnsrCuELiteFrontLayout::OnPaint()
{
  	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_TOP_FRONT);
	jpeg.Paint(dc.m_hDC,rect);

	MonitorSensorState();
}

void CSnsrCuELiteFrontLayout::UpdateSensorLayout(POINT point,BOOL bColor)
{
	CRect oUpdateRect;
    CDC* pCtrlDC = GetDC();
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
    ReleaseDC(pCtrlDC);
}

void CSnsrCuELiteFrontLayout::OnTimer(UINT_PTR nIDEvent)
{
	MonitorSensorState();

	CDialog::OnTimer(nIDEvent);
}

BOOL CSnsrCuELiteFrontLayout::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll())
    {
	   KillTimer(1);
	}

	return CDialog::DestroyWindow();
}

void CSnsrCuELiteFrontLayout::MonitorSensorState(void)
{
	BOOL bHome, bPLimit, bNLimit;
	int iSensorState = SENSOR_OFF;

	CPoint InjectSnr(90,109);
	CPoint EjectSnr(668,193);
	CPoint AlignSnr(263,115);
	CPoint EjectJamSnr(567,235);

	CPoint IndexerHome(227,345);
	CPoint IndexerLimit(433,378);
	CPoint ClamperHome(335,490);

	CPoint OnloadFrontClamperHome(200,20);
	CPoint OffloadFrontClamperHome(650,100);

	CPoint FrontRailHome(50,400);
	CPoint RealRailHome(120,210);

	theStepper.GetHomeSensor(CUELITE_FRONT_RAIL,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(FrontRailHome,bHome);

	theStepper.GetHomeSensor(CUELITE_REAR_RAIL,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(RealRailHome,bHome);

	theStepper.GetHomeSensor(CUELITE_OFFLOAD_YTWO,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OnloadFrontClamperHome,bHome);

	theStepper.GetHomeSensor(CUELITE_OFFLOAD_YTWO,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OffloadFrontClamperHome,bHome);

	theStepper.GetHomeSensor(CUELITE_WINDOW_CLAMPER,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(ClamperHome,bHome);

	theStepper.GetHomeSensor(CUELITE_DEVICE_INDEXER,&bHome,&bNLimit,&bPLimit);
	//UpdateSensorLayout(IndexerLimit,bNLimit); 
	UpdateSensorLayout(IndexerLimit,bPLimit);
	UpdateSensorLayout(IndexerHome,bHome);

	iSensorState=theCuEliteTrack.theInjectSnr.GetCurrentState();
	UpdateSensorLayout(InjectSnr,iSensorState);

	iSensorState=theCuEliteTrack.theEjectSnr.GetCurrentState();
	UpdateSensorLayout(EjectSnr,iSensorState);

    iSensorState=theCuEliteTrack.theAlignSnr.GetCurrentState();
	UpdateSensorLayout(AlignSnr,iSensorState);

    iSensorState=theCuEliteTrack.theEjectJamSnr.GetCurrentState();
	UpdateSensorLayout(EjectJamSnr,iSensorState);
}
