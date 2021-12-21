/***************************************************************
SnsrTopOnOffLoaderLayout.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "SnsrCuEliteOnOffLoaderLayout.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "MachineConfig.h"
#include "Jpeg.h"
#include "CuOnload.h"
#include "CuOffload.h"

// CSnsrCuEliteOnOffLoaderLayout

IMPLEMENT_DYNAMIC(CSnsrCuEliteOnOffLoaderLayout, CDialog)

CSnsrCuEliteOnOffLoaderLayout::CSnsrCuEliteOnOffLoaderLayout()
{

}

CSnsrCuEliteOnOffLoaderLayout::~CSnsrCuEliteOnOffLoaderLayout()
{
}

BEGIN_MESSAGE_MAP(CSnsrCuEliteOnOffLoaderLayout, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CSnsrCuEliteOnOffLoaderLayout message handlers
void CSnsrCuEliteOnOffLoaderLayout::OnTimer(UINT_PTR nIDEvent)
{
	MonitorSensorState();

	CWnd::OnTimer(nIDEvent);
}

BOOL CSnsrCuEliteOnOffLoaderLayout::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);	//V1.1.W25 edit

	if (!theMcConfig.IsBypassAll())
    {
	    SetTimer(1,300,NULL);
    }

	return CDialog::OnInitDialog();
}

BOOL CSnsrCuEliteOnOffLoaderLayout::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll())
    {
	   KillTimer(1);
	}

	return CDialog::DestroyWindow();
}

void CSnsrCuEliteOnOffLoaderLayout::OnPaint()
{
 	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_TOP_SIDES);
	jpeg.Paint(dc.m_hDC,rect);

	MonitorSensorState();
}

void CSnsrCuEliteOnOffLoaderLayout::UpdateSensorLayout(POINT point,BOOL bColor)
{
	CRect   oUpdateRect;
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

void CSnsrCuEliteOnOffLoaderLayout::MonitorSensorState(void)
{
	BOOL bHome, bPLimit, bNLimit;
	int iSensorState = SENSOR_OFF;

	CPoint OnloadPushKickerHome(124,394);
	CPoint OnloadPushKickerLimit(64,394);

	CPoint OnloadPullyHome(297,558);
	CPoint OnloadPullyLimit(302,601);

	CPoint OnloadMagBufferPresent(320,250);
	CPoint OnloadMagDetect(320,280); 
	CPoint OnloadMagDrop(320,360);
	CPoint OnloadPullyMagExist(327,623);

	CPoint OnloadHolderHome(238,435);
	CPoint OnloadHolderLimit(244,210);

	CPoint OnloadClampFrontHome(390,210);
	CPoint OnloadClampBackHome(120,100);
	CPoint OnloadClampLengthHome(50,50);

	CPoint OnloadFork1Present(150,150);
	CPoint OnloadFork2Present(100,150);
	CPoint OnloadForkRemove(50,180);

	CPoint OffloadPullyHome(555,558);
	CPoint OffloadPullyLimit(555,596);

	CPoint OffloadMagBufferPresent(510,250);
	CPoint OffloadMagDetect(510,280);
	CPoint OffloadMagDrop(510,360);

	CPoint OffloadPullyMagExist(544,632);
	CPoint OffloadHolderHome(539,435);
	CPoint OffloadHolderLimit(545,210);

	CPoint OffloadClampFrontHome(460,210);
	CPoint OffloadClampBackHome(710,100);
	CPoint OffloadClampLengthHome(760,50);

	CPoint OffloadFork1Present(650,150);
	CPoint OffloadFork2Present(700,150);
	CPoint OffloadForkRemove(750,180);

	theStepper.GetHomeSensor(CUELITE_ONLOAD_XAXIS,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OnloadClampLengthHome,bHome);

	theStepper.GetHomeSensor(CUELITE_OFFLOAD_XAXIS,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OffloadClampLengthHome,bHome);

	theStepper.GetHomeSensor(CUELITE_ONLOAD_YONE,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OnloadClampBackHome,bHome);

	theStepper.GetHomeSensor(CUELITE_OFFLOAD_YONE,&bHome,&bNLimit,&bPLimit); 
	UpdateSensorLayout(OffloadClampBackHome,bHome);

	theStepper.GetHomeSensor(CUELITE_ONLOAD_YTWO,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OnloadClampFrontHome,bHome);

	theStepper.GetHomeSensor(CUELITE_OFFLOAD_YTWO,&bHome,&bNLimit,&bPLimit); 
	UpdateSensorLayout(OffloadClampFrontHome,bHome);

	theStepper.GetHomeSensor(CUELITE_ONLOAD_ELEVATOR,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OnloadHolderLimit,bPLimit); //@1
	UpdateSensorLayout(OnloadHolderHome,bHome);

	theStepper.GetHomeSensor(CUELITE_OFFLOAD_ELEVATOR,&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(OffloadHolderLimit,bPLimit); //@1
	UpdateSensorLayout(OffloadHolderHome,bHome);

    iSensorState=theCuEliteOnloader.theMagBufferPresentSensor.GetCurrentState();
	UpdateSensorLayout(OnloadMagBufferPresent,iSensorState);

    iSensorState=theCuEliteOnloader.theFork1PresentSensor.GetCurrentState();
	UpdateSensorLayout(OnloadFork1Present,iSensorState);

    iSensorState=theCuEliteOnloader.theFork2PresentSensor.GetCurrentState();
	UpdateSensorLayout(OnloadFork2Present,iSensorState);

    iSensorState=theCuEliteOffloader.theMagBufferPresentSensor.GetCurrentState();
	UpdateSensorLayout(OffloadMagBufferPresent,iSensorState);

    iSensorState=theCuEliteOffloader.theFork1PresentSensor.GetCurrentState();
	UpdateSensorLayout(OffloadFork1Present,iSensorState);

    iSensorState=theCuEliteOffloader.theFork2PresentSensor.GetCurrentState();
	UpdateSensorLayout(OffloadFork2Present,iSensorState);
	
	iSensorState=theCuEliteOnloader.theInjectPusher.theHomeSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPushKickerHome,iSensorState);

	iSensorState=theCuEliteOnloader.theInjectPusher.theLimitSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPushKickerLimit,iSensorState);

    iSensorState=theCuEliteOnloader.theMagPullyDcMotor.theHomeSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPullyHome,iSensorState);

	iSensorState=theCuEliteOnloader.theMagPullyDcMotor.theLimitSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPullyLimit,iSensorState);

	iSensorState=theCuEliteOnloader.theMagDetectedSensor.GetCurrentState();
	UpdateSensorLayout(OnloadMagDetect,iSensorState);

    iSensorState=theCuEliteOnloader.thePullyMagExistSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPullyMagExist,iSensorState);

	iSensorState=theCuEliteOffloader.theMagPullyDcMotor.theHomeSensor.GetCurrentState();
	UpdateSensorLayout(OffloadPullyHome,iSensorState);

	iSensorState=theCuEliteOffloader.theMagPullyDcMotor.theLimitSensor.GetCurrentState();
	UpdateSensorLayout(OffloadPullyLimit,iSensorState);

    iSensorState=theCuEliteOffloader.theMagDetectedSensor.GetCurrentState();
	UpdateSensorLayout(OffloadMagDetect,iSensorState);

    iSensorState=theCuEliteOffloader.thePullyMagExistSensor.GetCurrentState();
	UpdateSensorLayout(OffloadPullyMagExist ,iSensorState);

	iSensorState=theCuEliteOnloader.theMagDropSensor.GetCurrentState();
	UpdateSensorLayout(OnloadMagDrop,iSensorState);

	iSensorState=theCuEliteOnloader.theForkRemoveDetectedSensor.GetCurrentState();
	UpdateSensorLayout(OnloadForkRemove,iSensorState);

    iSensorState=theCuEliteOffloader.theMagDropSensor.GetCurrentState();
	UpdateSensorLayout(OffloadMagDrop,iSensorState);

    iSensorState=theCuEliteOffloader.theForkRemoveDetectedSensor.GetCurrentState();
	UpdateSensorLayout(OffloadForkRemove,iSensorState);
}