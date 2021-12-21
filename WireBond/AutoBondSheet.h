#pragma once

#include "AutoBondPage.h"
#include "bondstatisticpage.h"
#include "BondEditPage.h"
#include "BondParaPage.h"
#include "loopparageneral.h"
#include "TabCtrlEx.h"

// CAutoBondSheet
class CAutoBondSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAutoBondSheet)
public:
	CAutoBondSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAutoBondSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CAutoBondSheet();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W148 add
public:		//variable
    CAutoBondPage m_AutoBondPage;
public:		//function
    void AddAutoBondPages(void);
	void EnablePage(BYTE nPage, BYTE bEnable);	//V1.1.W169 add
private:	//variable
    CBondStatisticPage m_BondStatisticPage;
    CBondEditPage m_BondEditPage;
    CBondParaPage m_BondParaPage;
    CLoopParaGeneral m_LoopParaGeneralPage;
	CTabCtrlEx m_TabCtrl;	//V1.1.W169 add
};
