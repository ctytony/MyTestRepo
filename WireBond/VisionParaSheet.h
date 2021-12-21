#pragma once
#include "VisionCfgDlg.h"
#include "LightCfgPage.h"
#include "AdvVisionCfgPage.h"

// CVisionParaSheet
class CVisionParaSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CVisionParaSheet)

public:

	CVisionParaSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CVisionParaSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CVisionParaSheet();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W259 add
	virtual void PostNcDestroy();	//V1.1.W259 add

	DECLARE_MESSAGE_MAP()

private:

    void AddBondParaPages(void);

    CVisionCfgDlg m_VisionCfgPage;
    CLightCfgPage m_LightCfgPage; 
    CAdvVisionCfgPage m_AdvVisionCfgPage;
};


