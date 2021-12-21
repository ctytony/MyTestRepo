#pragma once
#include "loopparageneral.h"
#include "LoopParaAdvance.h"


// CLoopParaSheet

class CLoopParaSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CLoopParaSheet)

public:
	CLoopParaSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CLoopParaSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CLoopParaSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
    // Add loop parameter pages to loop parameter sheet
    int AddLoopParaPages(void);
    // Loop parameter general page
    CLoopParaGeneral m_LoopParaGeneralPage;
    CLoopParaAdvance m_LoopParaAdvancePage;
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


