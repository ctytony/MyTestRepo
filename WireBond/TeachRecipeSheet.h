#pragma once

#include "TchAlignPointPage.h"
#include "TchVisionPatternPage.h"
#include "AdvWireLayoutPage.h"
#include "TchSequencePage.h"
#include "CtactSrchPage.h"
#include "FireLevelPage.h"
#include "BtosPage.h"
#include "DummyBondPage.h"

enum enRecipeSheetType
{
	RECIPE_TEACH,
	RECIPE_EDIT,
	RECIPE_SETUP
};

// CTeachRecipeSheet

class CTeachRecipeSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTeachRecipeSheet)

public:
	CTeachRecipeSheet(UINT nIDCaption, int lRecipeMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTeachRecipeSheet(LPCTSTR pszCaption, int lRecipeMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTeachRecipeSheet();
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();
	BYTE IsBusy();		//V1.1.W153 add
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W153 add
public:		//variable
    CTchAlignPointPage m_TchAlignPointPage;
    CTchVisionPatternPage m_TchVisionPatternPage;
    CAdvWireLayoutPage m_TchAdvWireLayoutPage;
    CTchSequencePage *m_pTchSequencePage;	//V1.5.W100 edit
    CCtactSrchPage m_CtactSrchPage;
    CFireLevelPage m_FireLevelPage;
    CBtosPage m_BondTipOffsetPage;
    CDummyBondPage m_DummyBondPage;
    bool m_bPrAlignDone;
    int m_lRecipeOperateMode;
public:		//function
    void AddTeachRecipePages(void);
private:	//variable
    LPCTSTR m_pszCaption;
};
