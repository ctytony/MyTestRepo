#pragma once

#include "ForceCompensatePage.h"   
#include "ForceSwitchPage.h"  
#include "ImpactModePage.h"

// CForceSwitchSheet

class CForceSwitchSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CForceSwitchSheet)
public:
	CForceSwitchSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CForceSwitchSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CForceSwitchSheet();
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
public:		//function
	void AddForceSwitchPages(void);
public:		//variable
	CForceCompensatePage m_ForceCompensatePage;
	CForceSwitchPage m_ForceSwitchPage;
	CImpactModePage m_ImpactModePage;
};


