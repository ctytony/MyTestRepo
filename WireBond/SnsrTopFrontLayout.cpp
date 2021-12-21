/***************************************************************
SnsrTopFrontLayout.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "WireBond.h"
#include "SnsrTopFrontLayout.h"
#include "Macdef.h"
#include "MathCalc.h"
#include "Msgdef.h"
#include "MenuConfig.h"
#include "MaterialHandlerThread.h"
#include "StepperControl.h"
#include "Jpeg.h"
#include "TopClamp.h"
#include "R2RIndex.h"

// CSensorLayoutPage dialog
IMPLEMENT_DYNAMIC(CSnsrTopFrontLayout, CDialog)

CSnsrTopFrontLayout::CSnsrTopFrontLayout(CWnd* pParent /*=NULL*/)
	: CDialog(CSnsrTopFrontLayout::IDD, pParent)
	, pInfoView(NULL)
{
}

CSnsrTopFrontLayout::~CSnsrTopFrontLayout()
{
}

void CSnsrTopFrontLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnsrTopFrontLayout, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSensorLayoutPage message handlers
BOOL CSnsrTopFrontLayout::OnInitDialog()
{
	SetWindowPos(NULL, VISION_VIEW_LEFT, 60, 785, 705, SWP_NOZORDER);	//V1.1.W25 edit

	if (!theMcConfig.IsBypassAll())
    {
	    SetTimer(1,300,NULL);
    }

	return CDialog::OnInitDialog();
}

void CSnsrTopFrontLayout::OnPaint()
{
  	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CJpeg jpeg;
	jpeg.Load(AfxGetInstanceHandle(),IDR_JPEG_TOP_FRONT);
	jpeg.Paint(dc.m_hDC,rect);

	MonitorSensorState();
}

void CSnsrTopFrontLayout::UpdateSensorLayout(POINT& pt,BOOL bColor)
{
    CDC* pCtrlDC = GetDC();
	CBrush oGreenBrush(RGB(0, 255, 0));
    CBrush oRedBrush(RGB(255, 0, 0));
	CPen pen(PS_NULL,1,RGB(255,0,0));

	CPen* lpPen=pCtrlDC->SelectObject(&pen);
	CBrush* lpBrush=pCtrlDC->SelectObject(bColor==FALSE ? &oGreenBrush : &oRedBrush);

    pCtrlDC->Ellipse(CRect(pt,CSize(20, 20)));

	pCtrlDC->SelectObject(lpPen);
	pCtrlDC->SelectObject(lpBrush);

    ReleaseDC(pCtrlDC);
}

void CSnsrTopFrontLayout::OnTimer(UINT_PTR nIDEvent)
{
	MonitorSensorState();

	CDialog::OnTimer(nIDEvent);
}

BOOL CSnsrTopFrontLayout::DestroyWindow()
{
	if (!theMcConfig.IsBypassAll())
    {
		KillTimer(1);
	}

	return CDialog::DestroyWindow();
}

void CSnsrTopFrontLayout::MonitorSensorState(void)
{
	BOOL bHome, bPLimit, bNLimit;

	CPoint InjectSnr(90,109);
	CPoint EjectSnr(668,193);
	CPoint AlignSnr(263,115);
	CPoint EjectJamSnr(567,235);

	CPoint IndexerHome(227,345);
	CPoint IndexerLimit(433,378);
	CPoint ClamperHome(335,490);

	theStepper.GetHomeSensor(theTopClamp.GetStepID(),&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(ClamperHome,bHome);

	theStepper.GetHomeSensor(theTopTrack.GetIndexStepID(),&bHome,&bNLimit,&bPLimit);
	UpdateSensorLayout(IndexerLimit,bPLimit);
	UpdateSensorLayout(IndexerHome,bHome);

	UpdateSensorLayout(InjectSnr,theTopTrack.m_InjectSensor.GetCurrentState());
	UpdateSensorLayout(EjectSnr,theTopTrack.m_EjectSensor.GetCurrentState());
	UpdateSensorLayout(AlignSnr,theTopTrack.m_AlignSensor.GetCurrentState());
	UpdateSensorLayout(CPoint(629,149),theTopTrack.m_TrackFrontHome.GetCurrentState());	//V1.4.W37 add, V1.5.W11 edit
	UpdateSensorLayout(CPoint(599,145),theTopTrack.m_TrackBackHome.GetCurrentState());	//V1.4.W37 add, V1.5.W11 edit
	UpdateSensorLayout(EjectJamSnr,theTopTrack.m_EjectJamSensor.GetCurrentState());
}
