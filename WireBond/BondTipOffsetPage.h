#pragma once


// CBondTipOffsetPage dialog

class CBondTipOffsetPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBondTipOffsetPage)

public:
	CBondTipOffsetPage();
	virtual ~CBondTipOffsetPage();

// Dialog Data
	enum { IDD = IDD_TAB_BTOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};
