#pragma once

#include "CuEliteDeviceParaPage.h"
#include "CuEliteElevSetupPage.h"     
#include "CuEliteIndexerSetupPage.h" 
#include "CuEliteIndexTchPage.h"

enum enCuWhSetupSheetType
{
	WH_CUELITE_TEACH,
	WH_CUELITE_EDIT
};

// CCuEliteWorkHolderTeachSheet

class CCuEliteWorkHolderTeachSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCuEliteWorkHolderTeachSheet)

public:
	CCuEliteWorkHolderTeachSheet(UINT nIDCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCuEliteWorkHolderTeachSheet(LPCTSTR pszCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CCuEliteWorkHolderTeachSheet();

protected:
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
public:	//variables
	int m_iPageMode;
public:	//functions
	int AddTchWorkHoldPages(void);
private:	//variable
    CCuEliteDeviceParaPage m_CuEliteDeviceParaPage;
    CCuEliteElevSetupPage m_CuEliteElevPartSetUpPage;
	CCuEliteIndexerSetupPage m_CuEliteIndexerPartSetUpPage;
	CCuEliteIndexTchPage m_CuEliteTchPrIndexPage;
};
