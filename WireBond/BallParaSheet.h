#pragma once

#include "BallBondParaPage.h"
#include "BallShapeParaPage.h"
#include "BallScrubPage.h"
#include "BumpWireParaPage.h"

// CBallParaSheet
class CBallParaSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBallParaSheet)

public:

	CBallParaSheet(UINT nIDCaption, char chBumpBallType, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBallParaSheet(LPCTSTR pszCaption, char chBumpBallType, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CBallParaSheet();

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    // Get current bump ball type
    char GetBumpBallType(void);

protected:

	virtual void PostNcDestroy();

	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);		//V1.1.W244 add

	DECLARE_MESSAGE_MAP()

private:

    CBallBondParaPage m_BallBondParaPage;
    CBallShapeParaPage m_BallShapeParaPage;
    CBumpWireParaPage m_BumpWireParaPage;

    void AddBallParaPages(void);

    char m_chBumpBallType;
};
