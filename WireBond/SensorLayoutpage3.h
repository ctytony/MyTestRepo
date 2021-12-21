#pragma once

// CSensorLayoutPage dialog
class CSensorLayoutPage3 : public CDialog
{
	DECLARE_DYNAMIC(CSensorLayoutPage3)

public:

	CSensorLayoutPage3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSensorLayoutPage3();
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
