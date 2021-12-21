#pragma once


// CBondUSGSetup dialog

class CBondUSGSetup : public CDialog
{
	DECLARE_DYNAMIC(CBondUSGSetup)

public:
	CBondUSGSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBondUSGSetup();

// Dialog Data
	enum { IDD = IDD_TAB_USGCalibrate };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
