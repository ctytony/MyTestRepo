#pragma once

#include "TopDeviceParaPage.h"
#include "TopIndexerSetupPage.h"
#include "TopElevSetupPage.h"
#include "TopIndexTchPage.h"

enum enWhSetupSheetType {WH_TEACH, WH_EDIT};

// CTeachWorkHolderSheet
class CTeachWorkHolderSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTeachWorkHolderSheet)

public:

	CTeachWorkHolderSheet(UINT nIDCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTeachWorkHolderSheet(LPCTSTR pszCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTeachWorkHolderSheet();

	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();

	int m_iPageMode;

protected:

	DECLARE_MESSAGE_MAP()

	virtual void PostNcDestroy();

	BYTE IsBusy();		//V1.1.W158 add
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W158 add

private:

	CTopDeviceParaPage    m_TopDeviceParaPage;
	CTopElevSetupPage    theElevPartSetUpPage;
    CTopIndexerSetupPage theIndexerPartSetUpPage;
	CTopIndexTchPage      m_TopTchPrIndexPage;

	void AddTchWorkHoldPages(void);
};
