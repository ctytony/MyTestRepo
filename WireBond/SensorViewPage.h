#pragma once
#include "SensorLayoutPage.h"
#include "SensorLayoutPage2.h"
#include "SensorLayoutPage3.h"
#include "SensorLayoutPage4.h"

// CSensorViewPage dialog
class CSensorViewPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSensorViewPage)

public:
	CSensorViewPage();
	virtual ~CSensorViewPage();

// Dialog Data
	enum { IDD = IDD_SENSOR_VIEW };

	virtual BOOL OnInitDialog();

	afx_msg void OnDestroy();
	afx_msg void OnMagKickerOn();
	afx_msg void OnMagKickerOff();
	afx_msg void OnLfKickerSolOn();
	afx_msg void OnLfKickerSolOff();
	afx_msg void OnClawOn();
	afx_msg void OnClawOff();
	afx_msg void OnPusherOn();
	afx_msg void OnPusherOff();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnListSensor();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:	//variables

	int m_iSensorId;
	CSensorLayoutPage *m_SensorPage;
	CSensorLayoutPage2 *m_SensorPage2;
	CSensorLayoutPage3 *m_SensorPage3;
	CSensorLayoutPage4 *m_SensorPage4;
};
