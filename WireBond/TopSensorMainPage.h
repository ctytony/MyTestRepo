#pragma once

#include "SnsrTopFrontLayout.h"
#include "SnsrTopOnOffLoaderLayout.h"

// CTopSensorMainPage dialog
class CTopSensorMainPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopSensorMainPage)

public:
	CTopSensorMainPage();
	virtual ~CTopSensorMainPage();

// Dialog Data
	enum { IDD = IDD_TOP_SENSOR };

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnListSensor();
	afx_msg void OnDestroy();
	afx_msg void OnLeftClawOn();
	afx_msg void OnLeftClawOff();
	afx_msg void OnRightClawOn();
	afx_msg void OnRightClawOff();
	afx_msg void OnEjectorOn();
	afx_msg void OnEjectorOff();
	afx_msg void OnInjectorOn();
	afx_msg void OnInjectorOff();
    afx_msg void OnLfHolderOn();
    afx_msg void OnLfHolderOff();
	afx_msg void OnVacuumOnTop();
	afx_msg void OnVacuumOffTop();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:
	int m_iSensorId;
	CSnsrTopOnOffLoaderLayout *m_pTopOnOffLoaderView;
	CSnsrTopFrontLayout *m_pTopFrontView;
    void EnableButtons(bool bEnable);
};
