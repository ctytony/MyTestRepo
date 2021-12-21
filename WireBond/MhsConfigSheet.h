#pragma once

#include "SingleCapTrackPage.h"
#include "SingleCapSettingPage.h"
#include "SensorViewPage.h"
#include "SingleStepperPage.h"
#include "TopSettingPage.h"
#include "TopSteppperPage.h"
#include "TopSensorMainPage.h"
#include "CuEliteSettingPage.h"
#include "CuEliteStepperPage.h"
#include "CuEliteSensorMainPage.h"
#include "R2RSettingPage.h"
#include "R2RSnrMainPage.h"
#include "VledElitePage.h"

// CMaterialHandleConfigSheet
class CMaterialHandleConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMaterialHandleConfigSheet)
public:
	CMaterialHandleConfigSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMaterialHandleConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMaterialHandleConfigSheet();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
protected:
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W248 add
	LRESULT SaveSetting(WPARAM wParam, LPARAM lParam);		//V1.5.W117 add
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
private:	//variable
	//single
	CPropertyPage *m_pPropPage;
	CSingleCapSettingPage m_MhsSettingPage;
	CSensorViewPage theSensorViewPage;
	CSingleStepperPage theSingleStepperPage;

    //top
	CTopSettingPage m_TopSettingPage;
	CTopSteppperPage m_TopStepperPage;
	CTopSensorMainPage m_TopSensorPage; 
    
	//R2R on TOP add
    CR2RSettingPage m_R2RParaSettingPage;
    CR2RSnrMainPage m_R2RSensorPage;   

    //cuelite
    CCuEliteSettingPage m_CuEliteParaSettingPage;
	CCuEliteStepperPage m_CuEliteStepperPage;
	CCuEliteSensorMainPage m_CuEliteSensorPage; 
private:	//function
    void AddPropertyPages(void);
};
