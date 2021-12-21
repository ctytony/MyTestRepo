#pragma once

#include "CtactSrchPage.h"
#include "FireLevelPage.h"
#include "BtosPage.h"

// BondSetupSheet

class CBondSetupSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBondSetupSheet)

public:
	CBondSetupSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBondSetupSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CBondSetupSheet();

protected:
	DECLARE_MESSAGE_MAP()
private:
    //Contact search page
    CCtactSrchPage m_CtactSrchPage;
    //Adjust fire level page
    CFireLevelPage m_FireLevelPage;
    //Adjust bond tip offset page
    CBtosPage m_BondTipOffsetPage;
    // Add bond setup pages
    int AddBondSetupPages(void);
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


