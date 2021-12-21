#pragma once


// CSensorLayoutPage dialog

class CCuEliteMagParaPage2 : public CDialog
{
	DECLARE_DYNAMIC(CCuEliteMagParaPage2)

public:
	CCuEliteMagParaPage2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCuEliteMagParaPage2();

// Dialog Data
	enum { IDD = IDD_DIG_SENSOR_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CWnd* pInfoView;
	int UpdateSensorLayout(POINT point,BOOL bColor);
	afx_msg void OnPaint();
	virtual BOOL DestroyWindow();
};
