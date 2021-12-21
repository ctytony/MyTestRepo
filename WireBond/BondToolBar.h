#pragma once


// CBondToopBar

class CBondToolBar : public CToolBarCtrl
{
	DECLARE_DYNAMIC(CBondToolBar)
private:
	int         m_nButtonCount;
	TBBUTTON    *m_pToolBarButtons;
	using CToolBarCtrl::Create;

public:
	CBondToolBar();
	virtual ~CBondToolBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};


