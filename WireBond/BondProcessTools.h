#pragma once



// CBondProcessTools

class CBondProcessTools : public CPropertySheet
{
	DECLARE_DYNAMIC(CBondProcessTools)

public:
	CBondProcessTools(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBondProcessTools(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CBondProcessTools();

protected:
	DECLARE_MESSAGE_MAP()
};


