#pragma once


// CSnsrR2RFrontLayout



class CSnsrR2RFrontLayout : public CDialog
{
	DECLARE_DYNAMIC(CSnsrR2RFrontLayout)

public:
	CSnsrR2RFrontLayout(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnsrR2RFrontLayout();

// Dialog Data
	enum { IDD = IDD_SENSOR_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CWnd* pInfoView;

	int UpdateSensorLayout(POINT point,BOOL bColor);

	afx_msg void OnPaint();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	int MonitorSensorState(void);

	virtual BOOL DestroyWindow();
};