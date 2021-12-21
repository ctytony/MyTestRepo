#pragma once

#include "DiagnoFileTab.h"
#include "TestMotionTimePage.h"

// CDebugToolSheet
class CDebugToolSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDebugToolSheet)

public:
	CDebugToolSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDebugToolSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDebugToolSheet();
	void AddPages(void);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W43 add
	virtual void PostNcDestroy();
private:
	//CDiagnoFileTab m_DiagnoFileTab;	//v2.0.T68 delete
	CTestMotionTimePage m_MotionTimeDebugPage;
};
