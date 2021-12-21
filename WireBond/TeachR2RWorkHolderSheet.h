#pragma once

#include "R2RDevicePage.h"
#include "R2RIndexerSetupPage.h"
#include "R2RIndexTchPage.h"
#include "TopElevSetupPage.h"
#include "TopIndexTchPage.h"
#include "TopDeviceParaPage.h"
#include "TopIndexerSetupPage.h"

// CTeachR2RWorkHolderSheet

class CTeachR2RWorkHolderSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTeachR2RWorkHolderSheet)

public:
	CTeachR2RWorkHolderSheet(UINT nIDCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTeachR2RWorkHolderSheet(LPCTSTR pszCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTeachR2RWorkHolderSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int AddTchWorkHoldPages(void);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();
	int m_iPageMode;
private:
	enum enR2RWhSetupSheetType {WH_TEACH, WH_EDIT};
	CR2RDevicePage       m_R2RDeviceage;
    CR2RIndexerSetupPage m_R2RIndexerSetUpPage;
	CR2RIndexTchPage      m_R2RTchPrIndexPage;
};
