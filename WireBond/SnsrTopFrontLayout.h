#pragma once

// CSnsrTopFrontLayout
class CSnsrTopFrontLayout : public CDialog
{
	DECLARE_DYNAMIC(CSnsrTopFrontLayout)

public:

	CSnsrTopFrontLayout(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnsrTopFrontLayout();

// Dialog Data
	enum { IDD = IDD_SENSOR_PAGE };

	virtual BOOL OnInitDialog();

	CWnd* pInfoView;

	void UpdateSensorLayout(POINT& pt,BOOL bColor);

	afx_msg void OnPaint();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void MonitorSensorState(void);

	virtual BOOL DestroyWindow();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};