#pragma once

#include "CuEliteWHSetupPage.h"
#include "CuEliteDeviceOffsetSetupPage.h"     
#include "CuEliteMachineOffsetPage.h"  
#include "CuEliteDeviceParaPage.h"  

class CCuEliteWorkHolderSetupSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCuEliteWorkHolderSetupSheet)

public:
	CCuEliteWorkHolderSetupSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCuEliteWorkHolderSetupSheet(LPCTSTR pszCaption,  CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CCuEliteWorkHolderSetupSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	int AddTchWorkHoldPages(void);

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	/*virtual BOOL DestroyWindow();*/

protected:
	virtual void PostNcDestroy();
public:
    afx_msg void OnDestroy();

private:
	
   // CDlgCuEliteWhSetUpPage           mCuEliteWhSetUpPage;
   CCuEliteWHSetupPage              m_CuEliteWhSetUpPage;
   CCuEliteDeviceOffsetSetupPage    m_CuEliteDeviceOffsetPage;
   //v2.18.5.w4 add{
   CCuEliteMachineOffsetPage        m_CuEliteMachineOffsetPage;
   //}

};


