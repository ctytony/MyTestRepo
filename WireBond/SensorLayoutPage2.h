#pragma once

// CSensorLayoutPage dialog
class CSensorLayoutPage2 : public CDialog
{
	DECLARE_DYNAMIC(CSensorLayoutPage2)

public:

	CSensorLayoutPage2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSensorLayoutPage2();
	virtual BOOL DestroyWindow();

	enum { IDD = IDD_SENSOR_PAGE };

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:

	CWnd* pInfoView;

	void UpdateSensor(POINT point, BOOL bRed);
	void UpdateDisplay(void);
};
