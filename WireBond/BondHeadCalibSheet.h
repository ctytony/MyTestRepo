#pragma once

#include "ForceCalibPage.h"
#include "WclCalibPage.h"
#include "PositionCalibPage.h"
#include "ForceAutoCalibPage.h"  

// CBondHeadCalibSheet

class CBondHeadCalibSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBondHeadCalibSheet)
public:
	CBondHeadCalibSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBondHeadCalibSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CBondHeadCalibSheet();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();	//V1.1.W145 add
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W145 add
private:
	LPCTSTR m_pszCaption;
	void AddPropertyPages(void);
	CForceCalibPage m_ForceCalibPage;
	CWclCalibPage m_WclCalibPage;
	CPositionCalibPage m_PositionCalibPage;
	HWND m_hwndPage;	//V1.1.W145 add
};
