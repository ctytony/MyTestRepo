#pragma once

// CSnsrCuEliteOnOffLoaderLayout
class CSnsrCuEliteOnOffLoaderLayout : public CDialog
{
	DECLARE_DYNAMIC(CSnsrCuEliteOnOffLoaderLayout)

public:

	CSnsrCuEliteOnOffLoaderLayout();
	virtual ~CSnsrCuEliteOnOffLoaderLayout();

	// Dialog Data
	enum { IDD = IDD_SENSOR_PAGE };

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	void MonitorSensorState(void);
	void UpdateSensorLayout(POINT point,BOOL bColor);

protected:

	DECLARE_MESSAGE_MAP()
};


