// SensorLayoutPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "SensorLayoutPage4.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "VEOffload.h"
#include "Jpeg.h"

// CSensorLayoutPage4 dialog
IMPLEMENT_DYNAMIC(CSensorLayoutPage4, CDialog)

CSensorLayoutPage4::CSensorLayoutPage4(CWnd* pParent /*=NULL*/)
	: CDialog(CSensorLayoutPage4::IDD, pParent)
	, pInfoView(NULL)
{
}

CSensorLayoutPage4::~CSensorLayoutPage4()
{
}

void CSensorLayoutPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSensorLayoutPage4, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSensorLayoutPage4 message handlers
BOOL CSensorLayoutPage4::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);

	if (!theMcConfig.IsBypassAll())
    {
	    SetTimer(1, 300, NULL);
    }

	return CDialog::OnInitDialog();
}

void CSensorLayoutPage4::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_VE_OFFLOAD);
	jpeg.Paint(dc.m_hDC,rect);

	UpdateDisplay();
}

void CSensorLayoutPage4::UpdateSensor(POINT point, BOOL bRed)
{
	CRect oUpdateRect;
    CDC* pCtrlDC = GetDC();
	CBrush oGreenBrush(RGB(0, 255, 0));
    CBrush oRedBrush(RGB(255, 0, 0));
	CPen pen(PS_NULL, 1, RGB(255, 0, 0));
	CSize sz(20, 20);
	   
	oUpdateRect = CRect(point, sz);
	pCtrlDC->SelectObject(&pen);

	if (bRed == TRUE)
		pCtrlDC->SelectObject(&oRedBrush);
	else
		pCtrlDC->SelectObject(&oGreenBrush);

    pCtrlDC->Ellipse(oUpdateRect);  
    ReleaseDC(pCtrlDC);
}

void CSensorLayoutPage4::OnTimer(UINT_PTR nIDEvent)
{
	UpdateDisplay();
	CDialog::OnTimer(nIDEvent);
}

void CSensorLayoutPage4::UpdateDisplay(void)
{
	CPoint ptMagDropSensor(492, 237);
	CPoint ptMagBufSensor(392, 250);
	CPoint ptLfKickerJamSensor(313, 265);
	CPoint ptLfOutputJamSensor(392, 350);
	CPoint ptMagArmSensor(408, 393);
	CPoint ptMagKickerSensor(302, 397);
	CPoint ptMagFullSensor(114, 548);
	CPoint ptMagArmYHomeSensor(470, 438);
	CPoint ptMagArmYLimitSensor(370, 483);
	CPoint ptMagArmZHomeSensor(350, 508);
	CPoint ptMagArmZLimitSensor(350, 585);
	CPoint ptMagKickerHomeSensor(314, 458);
	CPoint ptMagKickerLimitSensor(225, 498);

	BOOL bHome, bPLimit, bNLimit;

	theStepper.GetHomeSensor(VE_STEPPER_OFFLOAD_MAG_Y, &bHome, &bNLimit, &bPLimit);
	UpdateSensor(ptMagArmYLimitSensor, bPLimit);
	UpdateSensor(ptMagArmYHomeSensor, bHome);

	theStepper.GetHomeSensor(VE_STEPPER_OFFLOAD_MAG_Z, &bHome, &bNLimit, &bPLimit);
	UpdateSensor(ptMagArmZLimitSensor, bPLimit);
	UpdateSensor(ptMagArmZHomeSensor, bHome);

	int iSensorState = SENSOR_OFF;

	theVEliteOffload.m_MagBufPresenceSensor.UpdateSensorState();
	iSensorState = theVEliteOffload.m_MagBufPresenceSensor.GetSensorState();
	UpdateSensor(ptMagBufSensor, iSensorState);

	iSensorState = theVEliteOffload.m_MagBufDropSensor.GetCurrentState();
	UpdateSensor(ptMagDropSensor, iSensorState);

	iSensorState = theVEliteOffload.m_MagArmPresenceSensor.GetCurrentState();
	UpdateSensor(ptMagArmSensor, iSensorState);

	iSensorState = theVEliteOffload.m_MagKickerPresenceSensor.GetCurrentState();
	UpdateSensor(ptMagKickerSensor, iSensorState);

	iSensorState = theVEliteOffload.m_MagKickerHomeSensor.GetCurrentState();
	UpdateSensor(ptMagKickerHomeSensor, iSensorState);

	iSensorState = theVEliteOffload.m_MagKickerLimitSensor.GetCurrentState();
	UpdateSensor(ptMagKickerLimitSensor, iSensorState);

	iSensorState = theVEliteOffload.m_MagFullSensor.GetCurrentState();
	UpdateSensor(ptMagFullSensor, iSensorState);

	iSensorState = theVEliteOffload.m_LfKickerJamSensor.GetCurrentState();
	UpdateSensor(ptLfKickerJamSensor, iSensorState);

	iSensorState = theVEliteOffload.m_LfOutputJamSensor.GetCurrentState();
	UpdateSensor(ptLfOutputJamSensor, iSensorState);
}

BOOL CSensorLayoutPage4::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll()) KillTimer(1);

	return CDialog::DestroyWindow();
}
