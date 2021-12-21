#pragma once
#include "SensorViewPage.h"
#include "ServoAutoTunePage.h"
#include "TopSensorMainPage.h"

// CAfpToolSheet
class CAfpToolSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAfpToolSheet)
public:
	CAfpToolSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAfpToolSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CAfpToolSheet();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.5.W67 add
	virtual void PostNcDestroy();
public:
	void AddAfpToolPages(void);
private:
	CServoAutoTunePage theServoAutoTunePage;
	CSensorViewPage    theSensorViewPage;
	CTopSensorMainPage  theTopSensorViewPage;
};
