#pragma once
#include "capillarysmoothpage.h"

// CBondToolsSheet
class CBondToolsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBondToolsSheet)

public:

	CBondToolsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBondToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CBondToolsSheet();

    // Add bond tool pages
    void AddBondToolsPages(void);
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W22 add

	virtual void PostNcDestroy();		//V1.4.W22 add

	DECLARE_MESSAGE_MAP()

private:

    // Capillary smooth page
    CCapillarySmoothPage m_CapiSmoothPage;
};
