#pragma once

#include "LoopGeneralPage.h"
#include "Loop1stKinkPage.h"
#include "Loop2ndKinkPage.h"
#include "Loop3rdKinkPage.h"
#include "LoopLastKinkPage.h"
#include "LoopLandingPage.h"

#include "LoopGeneralPagePan.h"   
#include "LoopLandingPagePan.h"  
#include "Loop1stKinkPagePan.h"  
#include "LoopSpeedArcPage.h"   
#include "Loop3rdKinkPagePan.h"  
#include "Loop2ndKinkPagePan.h"  
// CAdvLoopParaSheet
class CAdvLoopParaSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAdvLoopParaSheet)

public:

	CAdvLoopParaSheet(UINT nIDCaption, int iLoopGroup, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAdvLoopParaSheet(LPCTSTR pszCaption, int iLoopGroup, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual ~CAdvLoopParaSheet();

    // Add loop parameter pages
    void AddLoopParaPages(void);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    // Get current loop group of menu display
    short GetLoopGroup(void);

protected:

	DECLARE_MESSAGE_MAP()

    virtual void PostNcDestroy();

private:

    // Current loop group
    short m_nCurrentGroup;
    // General loop parameter
    CLoopGeneralPage GeneralPage;
    // 1st kink parameter
    CLoop1stKinkPage Kink1Page;
    // 2nd kink parameter
    CLoop2ndKinkPage Kink2Page;
    // 3rd kink parameter
    CLoop3rdKinkPage Kink3Page;
    // Last kink parameter
    CLoopLastKinkPage LastKinkPage;
	// Landing parameter
	CLoopLandingPage LandingPage;

	CLoopGeneralPagePan GeneralPagePan;    //V1.5.175.P6 add
	CLoopLandingPagePan LandingPagePan;    //V1.5.175.P6 add
	CLoop1stKinkPagePan Kink1PagePan;      //V1.5.175.P6 add
	CLoopSpeedArcPage   SpeedArcPage;      //V1.5.175.P6 add
	CLoop2ndKinkPagePan Kink2PagePan;      //v1.5.175.p6 add
	CLoop3rdKinkPagePan Kink3PagePan;      //v1.5.175.p6 add
};


