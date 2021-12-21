#include "stdafx.h"
#include "WireBond.h"
#include "SensorLayoutPage2.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "VledIndex.h"
#include "VledOffload.h"
#include "StepperControl.h"
#include "VEIndex.h"
#include "VEOffload.h"
#include "Jpeg.h"

// CSensorLayoutPage dialog

IMPLEMENT_DYNAMIC(CSensorLayoutPage2, CDialog)

CSensorLayoutPage2::CSensorLayoutPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CSensorLayoutPage2::IDD, pParent)
	, pInfoView(NULL)
{
}

CSensorLayoutPage2::~CSensorLayoutPage2()
{
}

void CSensorLayoutPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSensorLayoutPage2, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSensorLayoutPage message handlers
BOOL CSensorLayoutPage2::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);

	/*pInfoView = GetDlgItem(IDC_IMG_SENSOR);
	pInfoView->SetWindowPos(NULL, 0, 0, 775, 700, SWP_NOZORDER);*/

	if (!theMcConfig.IsBypassAll())
		SetTimer(1, 300, NULL);

	return CDialog::OnInitDialog();
}

void CSensorLayoutPage2::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_TRACK_FRONT);
	jpeg.Paint(dc.m_hDC,rect);

	UpdateDisplay();
}

void CSensorLayoutPage2::UpdateSensor(POINT point, BOOL bRed)
{
	CRect oUpdateRect;
    CDC* pCtrlDC = GetDC();
	CBrush oGreenBrush(RGB(0, 255, 0));
    CBrush oRedBrush(RGB(255, 0, 0));
	CPen pen(PS_NULL,1,RGB(255, 0, 0));
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

void CSensorLayoutPage2::UpdateDisplay(void)
{
	CPoint DbCheck(358, 268);
	CPoint CapExist(406, 302);
	CPoint PostBond(676, 355);
	CPoint Rear(662, 262);
	CPoint Front(649, 274);
	CPoint InjectHome(117, 258);
	CPoint InjectLimit(273, 284);
	CPoint LeftClawHome(352, 324);
	CPoint RightClawHome(461, 346);
	CPoint ServoHome(406, 418);

	int iSensorState = SENSOR_OFF;
	BOOL bHome, bPLimit, bNLimit;

	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
	{
		iSensorState = theVEliteTrack.theDbCheckSensor.GetCurrentState();
		UpdateSensor(DbCheck, iSensorState);

		//theVEliteTrack.theCapExistSensor.UpdateSensorState();
		//iSensorState = theVEliteTrack.theCapExistSensor.GetSensorState();
		//UpdateSensor(CapExist, iSensorState);

		iSensorState = theVEliteTrack.theOffloadTrackSensor.GetCurrentState();
		UpdateSensor(PostBond, iSensorState);

		iSensorState = theVEliteOffload.m_LfKickerJamSensor.GetCurrentState();
		UpdateSensor(Front, iSensorState);

		//theVledOffload.thePusherCylinder.theRearSensor.UpdateSensorState();
		//iSensorState = theVledOffload.thePusherCylinder.theRearSensor.GetSensorState();
		//UpdateSensor(Rear, iSensorState);

		theStepper.GetHomeSensor(VE_STEPPER_ONLOAD_INJECTOR, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(InjectLimit, bPLimit);
		UpdateSensor(InjectHome, bHome);

		theStepper.GetHomeSensor(VE_STEPPER_INDEX_LEFT_CLAW, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(LeftClawHome, bHome);

		theStepper.GetHomeSensor(VE_STEPPER_INDEX_RIGHT_CLAW, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(RightClawHome,bHome);

		theStepper.GetHomeSensor(VE_STEPPER_INDEX_CLAMPER, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(ServoHome, bHome);
		break;
	}
	case MHS_VLED:
	{
		iSensorState = theVledTrack.theDbCheckSensor.GetCurrentState();
		UpdateSensor(DbCheck, iSensorState);

		iSensorState = theVledTrack.theCapExistSensor.GetCurrentState();
		UpdateSensor(CapExist, iSensorState);

		iSensorState = theVledTrack.theOffloadTrackSensor.GetCurrentState();
		UpdateSensor(PostBond, iSensorState);

		iSensorState = theVledOffload.thePusherCylinder.theFrontSensor.GetCurrentState();
		UpdateSensor(Front, iSensorState);

		iSensorState = theVledOffload.thePusherCylinder.theRearSensor.GetCurrentState();
		UpdateSensor(Rear, iSensorState);

		theStepper.GetHomeSensor(SINGLE_CAP_INJECTOR, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(InjectLimit, bPLimit);
		UpdateSensor(InjectHome, bHome);

		theStepper.GetHomeSensor(SINGLE_CAP_LEFT_CLAW, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(LeftClawHome, bHome);

		theStepper.GetHomeSensor(SINGLE_CAP_RIGHT_CLAW, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(RightClawHome,bHome);

		theStepper.GetHomeSensor(SINGLE_CAP_CLAMPER, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(ServoHome, bHome);
		break;
	}
	}
}

void CSensorLayoutPage2::OnTimer(UINT_PTR nIDEvent)
{
	UpdateDisplay();
	CDialog::OnTimer(nIDEvent);
}

BOOL CSensorLayoutPage2::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll()) KillTimer(1);

	return CDialog::DestroyWindow();
}
