// SensorLayoutPage.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "SensorLayoutPage.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "VledOffload.h"
#include "VEOnload.h"
#include "Jpeg.h"

// CSensorLayoutPage dialog

IMPLEMENT_DYNAMIC(CSensorLayoutPage, CDialog)

CSensorLayoutPage::CSensorLayoutPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSensorLayoutPage::IDD, pParent)
	, pInfoView(NULL)
{
}

CSensorLayoutPage::~CSensorLayoutPage()
{
}

void CSensorLayoutPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSensorLayoutPage, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSensorLayoutPage message handlers

BOOL CSensorLayoutPage::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);

	/*pInfoView = GetDlgItem(IDC_IMG_SENSOR);
	pInfoView->SetWindowPos(NULL, 0, 0, 775, 700, SWP_NOZORDER);*/

	if (!theMcConfig.IsBypassAll())
    {
	    SetTimer(1, 300, NULL);
    }

	return CDialog::OnInitDialog();
}

void CSensorLayoutPage::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_WH);
	jpeg.Paint(dc.m_hDC,rect);

	UpdateDisplay();
}

void CSensorLayoutPage::UpdateSensor(POINT point, BOOL bRed)
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

void CSensorLayoutPage::OnTimer(UINT_PTR nIDEvent)
{
	UpdateDisplay();
	CDialog::OnTimer(nIDEvent);
}

void CSensorLayoutPage::UpdateDisplay(void)
{
	CPoint LimitPos(392, 137);
	CPoint DropLfPos(589, 78);
	CPoint LfExist(293, 145);
	CPoint OnloadForkExist(84, 240);
	CPoint OffloadForkExist(522, 416);

	int iSensorState = SENSOR_OFF;
	BOOL bHome, bPLimit, bNLimit;

	switch (theMcConfig.GetMhsType())	//v3.0.T155 edit
	{
	case MHS_VLED_ELITE:
	{
		theStepper.GetHomeSensor(VE_STEPPER_ONLOAD_PICKER, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(LimitPos, bPLimit);
		UpdateSensor(DropLfPos, bHome);

		iSensorState = theVEliteOnload.theLfExistSensor.GetCurrentState();
		UpdateSensor(LfExist, iSensorState);

		iSensorState = theVEliteOnload.theForkExistSensor.GetCurrentState();
		UpdateSensor(OnloadForkExist, iSensorState);

		//theVledOffload.theOffloadForkExistSensor.UpdateSensorState();
		//iSensorState = theVledOffload.theOffloadForkExistSensor.GetSensorState();
		//UpdateSensor(OffloadForkExist, iSensorState);
		break;
	}
	case MHS_VLED:
	{
		theStepper.GetHomeSensor(SINGLE_CAP_ONLOAD_PICKER, &bHome, &bNLimit, &bPLimit);
		UpdateSensor(LimitPos, bNLimit);
		UpdateSensor(DropLfPos, bHome);

		iSensorState = theVledOnload.theLfExistSensor.GetCurrentState();
		UpdateSensor(LfExist, iSensorState);

		iSensorState = theVledOnload.theForkExistSensor.GetCurrentState();
		UpdateSensor(OnloadForkExist, iSensorState);

		iSensorState = theVledOffload.theOffloadForkExistSensor.GetCurrentState();
		UpdateSensor(OffloadForkExist, iSensorState);
		break;
	}
	}
}

BOOL CSensorLayoutPage::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll()) KillTimer(1);

	return CDialog::DestroyWindow();
}
