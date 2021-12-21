#pragma once

#include "SnsrCuEliteFrontLayout.h"
#include "SnsrCuEliteOnOffLoaderLayout.h"

// CCuEliteSensorMainPage dialog

class CCuEliteSensorMainPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteSensorMainPage)

public:
	CCuEliteSensorMainPage();
	virtual ~CCuEliteSensorMainPage();

// Dialog Data
	enum { IDD = IDD_CUELITE_SENSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnSetActive( );
	virtual BOOL OnInitDialog();
    virtual BOOL OnKillActive();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLbnSelchangeListSensor();
private:
	int m_iSensorId;

	CSnsrCuEliteOnOffLoaderLayout  *theCuEliteOnOffLoaderView;
	CSnsrCuELiteFrontLayout        *theCuEliteFrontView;

public:

	afx_msg void OnBnClickedButtonLeftClawOn();
	afx_msg void OnBnClickedButtonLeftClawOff();
	afx_msg void OnBnClickedButtonRightClawOn();
	afx_msg void OnBnClickedButtonRightClawOff();
	afx_msg void OnBnClickedButtonEjectorOn();
	afx_msg void OnBnClickedButtonEjectorOff();
	afx_msg void OnBnClickedButtonInjectorOn();
	afx_msg void OnBnClickedButtonInjectorOff();
    afx_msg void OnVacuumOn();
    afx_msg void OnVacuumOff();
    afx_msg void OnBnClickedButtonLfHolderOn();
    afx_msg void OnBnClickedButtonLfHolderOff();
};
