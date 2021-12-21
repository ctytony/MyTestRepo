#pragma once

#include "CuEliteDeviceParaPage.h"
#include "CuEliteLFParaPage.h"

// CTeachCuWorkHolderSheet

typedef enum enCuWhSetupSheetType {WH_CUTEACH, WH_CUEDIT};


class CTeachCuWorkHolderSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTeachCuWorkHolderSheet)

public:
	CTeachCuWorkHolderSheet(UINT nIDCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTeachCuWorkHolderSheet(LPCTSTR pszCaption, int lPageMode, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTeachCuWorkHolderSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	int AddTchWorkHoldPages(void);

protected:
	virtual void PostNcDestroy();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();

	int m_iPageMode;

private:
	

    CCuEliteDeviceParaPage m_CuELiteDeviceParaPage;
    CCuELiteLFParaPage m_CuELiteLFParaPage;


};


