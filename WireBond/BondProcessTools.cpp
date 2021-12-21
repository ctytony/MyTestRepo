// BondProcessTools.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "BondProcessTools.h"


// CBondProcessTools

IMPLEMENT_DYNAMIC(CBondProcessTools, CPropertySheet)

CBondProcessTools::CBondProcessTools(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CBondProcessTools::CBondProcessTools(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CBondProcessTools::~CBondProcessTools()
{
}


BEGIN_MESSAGE_MAP(CBondProcessTools, CPropertySheet)
END_MESSAGE_MAP()


// CBondProcessTools message handlers
