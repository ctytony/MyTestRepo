/***************************************************************
SnsrTopOnOffLoaderLayout.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "SnsrTopOnOffLoaderLayout.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "MachineConfig.h"
#include "Jpeg.h"
#include "TopOnload.h"
#include "TopOffload.h"

// CSnsrTopOnOffLoaderLayout
IMPLEMENT_DYNAMIC(CSnsrTopOnOffLoaderLayout, CDialog)

CSnsrTopOnOffLoaderLayout::CSnsrTopOnOffLoaderLayout()
{
}

CSnsrTopOnOffLoaderLayout::~CSnsrTopOnOffLoaderLayout()
{
}

BEGIN_MESSAGE_MAP(CSnsrTopOnOffLoaderLayout, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CSnsrTopOnOffLoaderLayout message handlers
void CSnsrTopOnOffLoaderLayout::OnTimer(UINT_PTR nIDEvent)
{
	MonitorSensorState();

	CWnd::OnTimer(nIDEvent);
}

BOOL CSnsrTopOnOffLoaderLayout::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);	//V1.1.W25 edit

	if (!theMcConfig.IsBypassAll())
    {
	    SetTimer(1,300,NULL);
    }

	return CDialog::OnInitDialog();
}

BOOL CSnsrTopOnOffLoaderLayout::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll())
    {
	   KillTimer(1);
	}

	return CDialog::DestroyWindow();
}

void CSnsrTopOnOffLoaderLayout::OnPaint()
{
 	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_TOP_SIDES);
	jpeg.Paint(dc.m_hDC,rect);

	MonitorSensorState();
}

void CSnsrTopOnOffLoaderLayout::UpdateSensorLayout(POINT point,BOOL bColor)
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

void CSnsrTopOnOffLoaderLayout::MonitorSensorState(void)
{
	BOOL bHome, bPLimit, bNLimit;
	int iSensorState = SENSOR_OFF;

	CPoint OnloadPushKickerHome(124,394);
	CPoint OnloadPushKickerLimit(64,394);
	
	CPoint OnloadPullyHome(297,558);
	CPoint OnloadPullyLimit(302,601);
	CPoint OnloadMagDetect(311,283);
	CPoint OnloadPullyMagExist(327,623);
	CPoint OffloadPullyHome(555,558);
	CPoint OffloadPullyLimit(556,596);
	CPoint OffloadMagDetect(546,283);
	CPoint OffloadPullyMagExist(544,632);

	CPoint OnloadHolderHome(238,435);
	CPoint OnloadHolderLimit(244,213);
	CPoint OffloadHolderHome(539,444);
	CPoint OffloadHolderLimit(537,214);

	/*CPoint OnloadCylinderRear(242,138);
	CPoint OnloadCylinderFront(251,161);
	CPoint OffloadCylinderRear(528,141);
	CPoint OffloadCylinderFront(529,163);*/

	CPoint OnloadMagBufferEmpty(311,243);
	CPoint OnloadMagDrop(311,383);
	CPoint OffloadMagBufferEmpty(546,243);
	CPoint OffloadMagDrop(546,383);

	theStepper.GetHomeSensor(theTopOnloader.m_nMagHolderStepperId,&bHome,&bNLimit,&bPLimit);
	
	//UpdateSensorLayout(OnloadHolderLimit,bNLimit);
	UpdateSensorLayout(OnloadHolderLimit,bPLimit); //@1
	UpdateSensorLayout(OnloadHolderHome,bHome);

	theStepper.GetHomeSensor(theTopOffloader.m_nMagHolderStepperId,&bHome,&bNLimit,&bPLimit);
	
	//UpdateSensorLayout(OffloadHolderLimit,bNLimit);
	UpdateSensorLayout(OffloadHolderLimit,bPLimit); //@1
	UpdateSensorLayout(OffloadHolderHome,bHome);

	iSensorState=theTopOnloader.m_InjectPusher.theHomeSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPushKickerHome,iSensorState);

	iSensorState=theTopOnloader.m_InjectPusher.theLimitSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPushKickerLimit,iSensorState);

    iSensorState=theTopOnloader.m_MagPullyDcMotor.theHomeSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPullyHome,iSensorState);

	iSensorState=theTopOnloader.m_MagPullyDcMotor.theLimitSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPullyLimit,iSensorState);

	iSensorState=theTopOnloader.m_MagDetectedSensor.GetCurrentState();
	UpdateSensorLayout(OnloadMagDetect,iSensorState);

    iSensorState=theTopOnloader.m_PullyMagExistSensor.GetCurrentState();
	UpdateSensorLayout(OnloadPullyMagExist,iSensorState);

	iSensorState=theTopOffloader.m_MagPullyDcMotor.theHomeSensor.GetCurrentState();
	UpdateSensorLayout(OffloadPullyHome,iSensorState);

	iSensorState=theTopOffloader.m_MagPullyDcMotor.theLimitSensor.GetCurrentState();
	UpdateSensorLayout(OffloadPullyLimit,iSensorState);

    iSensorState=theTopOffloader.m_MagDetectedSensor.GetCurrentState();
	UpdateSensorLayout(OffloadMagDetect,iSensorState);

    iSensorState=theTopOffloader.m_PullyMagExistSensor.GetCurrentState();
	UpdateSensorLayout(OffloadPullyMagExist ,iSensorState);

	//theTopOnloader.theMagClampCylinder.theRearSensor.UpdateSensorState();
	//iSensorState=theTopOnloader.theMagClampCylinder.theRearSensor.GetSensorState();
	//UpdateSensorLayout(OnloadCylinderRear,iSensorState);

	/*theTopOnloader.theMagClampCylinder.theFrontSensor.UpdateSensorState();
	iSensorState=theTopOnloader.theMagClampCylinder.theFrontSensor.GetSensorState();
	UpdateSensorLayout(OnloadCylinderFront,iSensorState);*/

	/*theTopOffloader.theMagClampCylinder.theRearSensor.UpdateSensorState();
    iSensorState=theTopOffloader.theMagClampCylinder.theRearSensor.GetSensorState();
	UpdateSensorLayout(OffloadCylinderRear,iSensorState);

	theTopOffloader.theMagClampCylinder.theFrontSensor.UpdateSensorState();
    iSensorState=theTopOffloader.theMagClampCylinder.theFrontSensor.GetSensorState();
	UpdateSensorLayout(OffloadCylinderFront ,iSensorState)*/;

	iSensorState=theTopOnloader.m_MagBufferPresentSensor.GetCurrentState();
	UpdateSensorLayout(OnloadMagBufferEmpty,iSensorState);

	iSensorState = theTopOnloader.m_MagDropSensor.GetCurrentState();
	UpdateSensorLayout(OnloadMagDrop,iSensorState);

	iSensorState=theTopOffloader.m_MagBufferPresentSensor.GetCurrentState();
	UpdateSensorLayout(OffloadMagBufferEmpty,iSensorState);

	iSensorState=theTopOffloader.m_MagDropSensor.GetCurrentState();
	UpdateSensorLayout(OffloadMagDrop,iSensorState);
}
