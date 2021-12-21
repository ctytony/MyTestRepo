#pragma once


// CCuELiteLFParaPage dialog

class CCuELiteLFParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuELiteLFParaPage)

public:
	CCuELiteLFParaPage();
	virtual ~CCuELiteLFParaPage();

// Dialog Data
	enum { IDD = IDD_TAB_TOP_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	short m_nDeviceRowNum;
	short m_nDeviceColmNum;
	long m_lDeviceColmPitch;
	short m_nWndUnitNum;
	short m_nDeviceId;
    long m_lDeviceWidth;


public:
	afx_msg void OnEnSetfocusEditMagBotmHeight();
	afx_msg void OnEnSetfocusEditMagSlotHeight();
	afx_msg void OnEnSetfocusEditMagSlotNum();
	afx_msg void OnEnSetfocusEditSlipRowNum();
	afx_msg void OnEnSetfocusEditSlipColumNum();
//	afx_msg void OnEnSetfocusEditSlip1stToEdgeLen();
	afx_msg void OnEnSetfocusEditSlipPitch();
	afx_msg void OnEnSetfocusEditWndColumNum();
	
public:
	afx_msg void OnEnSetfocusEditMagTopHeight();
	afx_msg void OnBnClickedButtonLoadDatabase();
	afx_msg void OnLbnSelchangeListDeviceDatabase();
private:
	int m_iSlipListId;
public:
	virtual BOOL OnSetActive();
private:
	short m_nLastHalfIndex;
public:
	
	int HideInputDialog(void);
//	afx_msg void OnEnChangeEditSlipPitch2();
private:
	long m_lSlipLength;
public:
	afx_msg void OnEnSetfocusEditSlipLen();
    afx_msg void OnEnSetfocusEditSlipWidth();
};
