#pragma once

// CTopDeviceParaPage dialog
class CTopDeviceParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopDeviceParaPage)
public:
	CTopDeviceParaPage();
	virtual ~CTopDeviceParaPage();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_TOP_DEV_PARA };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFocusMagBotmHeight();
	afx_msg void OnFocusMagSlotHeight();
	afx_msg void OnFocusMagSlotNum();
	afx_msg void OnFocusSlipRowNum();
	afx_msg void OnFocusSlipColumNum();
	afx_msg void OnFocusSlipPitch();
	afx_msg void OnFocusWndColumNum();
	afx_msg void OnFocusSlipLen();
	afx_msg void OnFocusMagTopHeight();
	afx_msg void OnLoadDatabase();
	afx_msg void OnListDatabase();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W158 add
	DECLARE_MESSAGE_MAP()
public:		//function
	void HideInputDialog(void);
private:	//variable
	long m_lMag1stLvHt;
	long m_lMagLvPitch;
	short m_lMagSlotNum;
	short m_nDeviceRowNum;
	short m_nDeviceColmNum;
	long m_lDeviceColmPitch;
	short m_nWndUnitNum;
	short m_nDeviceId;
	long m_lLastSlopToTopH;
	int m_iSlipListId;
	short m_nLastHalfIndex;
	long m_lSlipLength;
	BOOL m_bBusy;	//V1.1.W158 add
};
