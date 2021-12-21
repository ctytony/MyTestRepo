#pragma once

#include "BondSafetyPage.h"
#include "UsgConfigPage.h"
#include "LoopConfigPage.h"
#include "BondFeaturePage.h"
#include "CfgBondPage.h"
#include "ParaLimitPage.h"

// CBondProcessSheet
class CBondProcessSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBondProcessSheet)

public:

	CBondProcessSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBondProcessSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual ~CBondProcessSheet();

protected:

	DECLARE_MESSAGE_MAP()

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W25 add

	virtual void PostNcDestroy();

public:

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:

    CBondSafetyPage m_BondSafetyPage;
    CUsgConfigPage m_UsgConfigPage;
    CLoopConfigPage m_LoopConfigPage;
    CCfgBondPage m_BondConfigPage;
    CBondFeaturePage m_BondFeaturePage;
	CParaLimitPage m_ParaLimitPage;

    void AddBondProcessPages(void);
};
