// BondToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "BondToolBar.h"


// CBondToopBar

IMPLEMENT_DYNAMIC(CBondToolBar, CToolBarCtrl)

CBondToolBar::CBondToolBar()
{

}

CBondToolBar::~CBondToolBar()
{
}


BEGIN_MESSAGE_MAP(CBondToolBar, CToolBarCtrl)
END_MESSAGE_MAP()



// CBondToopBar message handlers



BOOL CBondToolBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
    BOOL bRet = CToolBarCtrl::Create(dwStyle, rect, pParentWnd, nID);

    // TODO: Add your specialized code here and/or call the base class
    m_nButtonCount = 5;

    SetBitmapSize(CSize(80,80));
    
    //VERIFY(AddBitmap(m_nButtonCount,IDB_BITMAP_TOOLBAR) != -1);

    m_pToolBarButtons = new TBBUTTON[m_nButtonCount];

    int nIndex;
	for (nIndex = 0; nIndex < m_nButtonCount; nIndex++)
	{
		CString szString;
        //theMsg.GetMsg(MSG_VISION_LIGHTING_SETUP, szString);
        szString.Empty();
        szString = _T("A");

		// Add second '\0'
		int nStringLength = szString.GetLength() + 1;
		TCHAR* pString = szString.GetBufferSetLength(nStringLength);
		pString[nStringLength] = 0;

		VERIFY((m_pToolBarButtons[nIndex].iString = AddStrings(pString)) != -1);

		szString.ReleaseBuffer();

        m_pToolBarButtons[nIndex].fsState = TBSTATE_ENABLED;
		m_pToolBarButtons[nIndex].fsStyle = TBSTYLE_BUTTON;
		m_pToolBarButtons[nIndex].dwData = 0;
		m_pToolBarButtons[nIndex].iBitmap = nIndex;
		m_pToolBarButtons[nIndex].idCommand = nIndex + ID_TOOL_BAR_LIGHT;
	}

    AddButtons(1,&m_pToolBarButtons[0]);
    AddButtons(1,&m_pToolBarButtons[1]);
    AddButtons(1,&m_pToolBarButtons[2]);
    AddButtons(1,&m_pToolBarButtons[3]);
    AddButtons(1,&m_pToolBarButtons[4]);
	//AddButtons(1,&m_pToolBarButtons[5]);

    return bRet;
}
