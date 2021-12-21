#include "stdafx.h"
#include "WireBond.h"
#include "SensorLayoutPage3.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "StepperControl.h"
#include "VEIndex.h"
#include "Jpeg.h"

// CSensorLayoutPage dialog

IMPLEMENT_DYNAMIC(CSensorLayoutPage3, CDialog)

CSensorLayoutPage3::CSensorLayoutPage3(CWnd* pParent /*=NULL*/)
	: CDialog(CSensorLayoutPage3::IDD, pParent)
	, pInfoView(NULL)
{
}

CSensorLayoutPage3::~CSensorLayoutPage3()
{
}

void CSensorLayoutPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSensorLayoutPage3, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSensorLayoutPage message handlers
BOOL CSensorLayoutPage3::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);

	if (!theMcConfig.IsBypassAll())
		SetTimer(1, 200, NULL);

	return CDialog::OnInitDialog();
}

void CSensorLayoutPage3::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_TRACK_BACK);
	jpeg.Paint(dc.m_hDC,rect);

	UpdateDisplay();
}

void CSensorLayoutPage3::UpdateSensor(POINT point, BOOL bRed)
{
	CRect oUpdateRect;
    CDC* pCtrlDC = GetDC();
	CBrush oGreenBrush(RGB(0, 255, 0));
    CBrush oRedBrush(RGB(255, 0, 0));
	CPen pen(PS_NULL,1,RGB(255, 0, 0));
	CSize sz(20, 20);
	   
	oUpdateRect = CRect(point,sz);
	pCtrlDC->SelectObject(&pen);

	if (bRed == TRUE)
		pCtrlDC->SelectObject(&oRedBrush);
	else
		pCtrlDC->SelectObject(&oGreenBrush);

    pCtrlDC->Ellipse(oUpdateRect);  
    ReleaseDC(pCtrlDC);
}

void CSensorLayoutPage3::OnTimer(UINT_PTR nIDEvent)
{
	UpdateDisplay();

	CDialog::OnTimer(nIDEvent);
}

void CSensorLayoutPage3::UpdateDisplay(void)
{
	CPoint Track(545, 262);
	CPoint PreBond(453, 273);
	CPoint KickerHome(259, 345);
	CPoint KickerLimit(177, 360);
	
	int iSensorState = SENSOR_OFF;
	BOOL bHome, bPLimit, bNLimit;

	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
	{
		iSensorState = theVEliteTrack.theOnloadTrackSensor.GetCurrentState();
		UpdateSensor(Track, iSensorState);

		iSensorState = theVEliteTrack.thePreBondSensor.GetCurrentState();
		UpdateSensor(PreBond, iSensorState);

		theStepper.GetHomeSensor(VE_STEPPER_OFFLOAD_KICKER, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(KickerLimit, bPLimit);
		UpdateSensor(KickerHome, bHome);
		break;
	}
	case MHS_VLED:
	{
		iSensorState = theVledTrack.theOnloadTrackSensor.GetCurrentState();
		UpdateSensor(Track, iSensorState);

		iSensorState = theVledTrack.thePreBondSensor.GetCurrentState();
		UpdateSensor(PreBond, iSensorState);

		theStepper.GetHomeSensor(SINGLE_CAP_OFFLOAD_KICKER, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(KickerLimit, bPLimit);
		UpdateSensor(KickerHome, bHome);
		break;
	}
	}
}

BOOL CSensorLayoutPage3::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll()) KillTimer(1);

	return CDialog::DestroyWindow();
}
