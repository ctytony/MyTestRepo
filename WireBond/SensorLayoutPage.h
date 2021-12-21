#pragma once

// CSensorLayoutPage dialog
class CSensorLayoutPage : public CDialog
{
	DECLARE_DYNAMIC(CSensorLayoutPage)

public:

	CSensorLayoutPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSensorLayoutPage();
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
