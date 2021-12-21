#pragma once
#include "afxdlgs.h"

#include "SnsrR2RFrontLayout.h"
#include "SnsrTopOnOffLoaderLayout.h"

// CR2RSnrMainPage dialog

class CR2RSnrMainPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RSnrMainPage)

public:
	CR2RSnrMainPage();
	virtual ~CR2RSnrMainPage();

// Dialog Data
	enum { IDD = IDD_R2R_SENSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLbnSelchangeListSensor();
private:
	int m_iSensorId;
	CSnsrR2RFrontLayout        *theR2RFrontView;
    void EnableButtons(bool bEnable);
public:
	virtual void OnOK();
	afx_msg void OnBnClickedButtonLeftClawOn();
	afx_msg void OnBnClickedButtonLeftClawOff();
	afx_msg void OnBnClickedButtonRightClawOn();
	afx_msg void OnBnClickedButtonRightClawOff();
	afx_msg void OnBnClickedButtonEjectorOn();
	afx_msg void OnBnClickedButtonEjectorOff();
	afx_msg void OnBnClickedButtonInjectorOn();
	afx_msg void OnBnClickedButtonInjectorOff();
    afx_msg void OnBnClickedButtonLfHolderOn();
    afx_msg void OnBnClickedButtonLfHolderOff();
	afx_msg void OnBnClickedButtonVacuumOnTop();
	afx_msg void OnBnClickedButtonVacuumOffTop();
};
