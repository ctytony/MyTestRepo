#pragma once

#include "EfoTestPage.h"

// CHardwareTestSheet
class CHardwareTestSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CHardwareTestSheet)

public:

	CHardwareTestSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CHardwareTestSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CHardwareTestSheet();

	void AddPages(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

protected:

	DECLARE_MESSAGE_MAP()

	CEfoTestPage theEfoTestPage;

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W26 add

	virtual void PostNcDestroy();	//V1.4.W26 add
};
