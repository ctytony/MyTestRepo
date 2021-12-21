#pragma once


// CSensorLayoutPage dialog

class CCuEliteLFParaPage2 : public CDialog
{
	DECLARE_DYNAMIC(CCuEliteLFParaPage2)

public:
	CCuEliteLFParaPage2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCuEliteLFParaPage2();

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
