#pragma once


// CCuEliteMagParaPage dialog

class CCuEliteMagParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteMagParaPage)

public:
	CCuEliteMagParaPage();
	virtual ~CCuEliteMagParaPage();

// Dialog Data
	enum { IDD = IDD_TAB_TOP_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	long m_lMag1stLvHt;
	long m_lMagLvPitch;
	short m_lMagSlotNum;
    long  m_lMagWidth;
    long  m_lMagLength;

public:
	afx_msg void OnEnSetfocusEditMagBotmHeight();
	afx_msg void OnEnSetfocusEditMagSlotHeight();
	afx_msg void OnEnSetfocusEditMagSlotNum();
	
	
private:
	long m_lLastSlopToTopH;
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
public:
    afx_msg void OnEnSetfocusEditMagWidth();
    afx_msg void OnEnSetfocusEditMagLength();

};
