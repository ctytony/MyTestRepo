#pragma once

#include "BondParaPage.h"
#include "AdvBondParaPage.h"
#include "SparkParaPage.h"
#include "ScrubParaPage.h"

// CBondParaSheet
class CBondParaSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBondParaSheet)

public:

	CBondParaSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBondParaSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CBondParaSheet();

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:

	virtual void PostNcDestroy();

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W159 add

	DECLARE_MESSAGE_MAP()

private:

    CBondParaPage m_BondParaPage;
    CAdvBondParaPage m_AdvBondParaPage;
    CScrubParaPage m_ScrubParaPage;
    CSparkParaPage m_SparkParaPage;

    void AddBondParaPages(void);
};


