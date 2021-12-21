#pragma once
#include "SingleTestingPage.h"
#include "SensorViewPage.h"
#include "SingleStepperPage.h"
#include "TopSteppperPage.h"
#include "TopSensorMainPage.h"
#include "TopTestingPage.h"
#include "TopIndexerDebug.h"
#include "CuEliteStepperPage.h"
#include "CuEliteTestingPage.h"
#include "CuEliteSensorMainPage.h"
#include "CuEliteIndexerDebug.h"
#include "R2RSnrMainPage.h"
#include "R2RIndexerDebug.h"
#include "R2RTestingPage.h"

// CMhsUtilitySheet
class CMhsUtilitySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMhsUtilitySheet)
public:
	CMhsUtilitySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMhsUtilitySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMhsUtilitySheet();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W23 add
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
private:
	//Single
	CSingleTestingPage theSingleTestingPage;
	CSensorViewPage    theSensorViewPage;
	CSingleStepperPage theSingleStepperPage;

	//Top
	CTopIndexerDebug m_TopIndexerDebugPage;
	CTopSteppperPage m_TopStepperPage;
	CTopSensorMainPage m_TopSensorPage; 
	CTopTestingPage m_TopTestPage;

	//R2R on TOP add
	CR2RIndexerDebug   m_R2RIndexerDebugPage;
  	CR2RSnrMainPage    m_R2RSensorPage;
	CR2RTestingPage    m_R2RTestPage;

    //CuElite
    CCuEliteIndexerDebug   theCuEliteIndexerDebugPage;
	CCuEliteStepperPage    theCuEliteStepperPage;
	CCuEliteSensorMainPage theCuEliteSensorPage; 
	CCuEliteTestingPage    theCuEliteTestPage;

	void AddPropertyPages(void);
};


