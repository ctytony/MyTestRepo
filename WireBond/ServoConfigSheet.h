#pragma once

#include "ServoGeneralPage.h"
#include "CfgAccPage.h"
#include "ServoControlPage.h"
#include "CfgWclProfilePage.h"

// CServoConfigSheet
class CServoConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CServoConfigSheet)

public:

	CServoConfigSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CServoConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CServoConfigSheet();

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:

	virtual void PostNcDestroy();	//V1.4.W21 add
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.4.W21 add

	DECLARE_MESSAGE_MAP()

private:

    CServoGeneralPage m_CfgServoGeneralPage;
    CCfgAccPage m_CfgAccPage;
    CServoControlPage m_ServoCtrlPage;
    CCfgWclProfilePage m_CfgWclProfilePage;

    void AddPropertyPages(void);
};


