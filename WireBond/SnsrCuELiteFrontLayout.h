#pragma once


// CSnsrCuELiteFrontLayout



class CSnsrCuELiteFrontLayout : public CDialog
{
	DECLARE_DYNAMIC(CSnsrCuELiteFrontLayout)

public:

	CSnsrCuELiteFrontLayout(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnsrCuELiteFrontLayout();

	enum { IDD = IDD_SENSOR_PAGE };

	virtual BOOL OnInitDialog();

	CWnd* pInfoView;

	void UpdateSensorLayout(POINT point,BOOL bColor);

	afx_msg void OnPaint();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void MonitorSensorState(void);

	virtual BOOL DestroyWindow();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};