#pragma once

// CCuEliteDeviceParaPage 对话框
class CCuEliteDeviceParaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteDeviceParaPage)

public:

	CCuEliteDeviceParaPage();
	virtual ~CCuEliteDeviceParaPage();
	virtual BOOL OnSetActive();

// 对话框数据
	enum { IDD = IDD_CUELITE_DEV_PARA };

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnEditMagBotmHeight();
	afx_msg void OnEditMagSlotHeight();
	afx_msg void OnEditMagSlotNum();
	afx_msg void OnEditSlipRowNum();
	afx_msg void OnEditSlipColumNum();
	afx_msg void OnEditSlipPitch();
	afx_msg void OnEditWndColumNum();
	afx_msg void OnEditMagWidth();
	afx_msg void OnEditMagLength();
	afx_msg void OnEditDeviceWidth();
	afx_msg void OnEditMagTopHeight();
	afx_msg void OnLoadDatabase();
	afx_msg void OnListDeviceDatabase();
	afx_msg void OnEditCenterOffset();
	afx_msg void OnSave();

	//variables
	long m_lSlipCenterOffet;

	//functions
	void HideInputDialog(void);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:	//variables

	long m_lMag1stLvHt;
	long m_lMagLvPitch;
	long m_lMagSlotNum;
	int m_nDeviceRowNum;
	int m_nDeviceColmNum;
	long m_lDeviceColmPitch;
	int m_nWndUnitNum;
	short m_nDeviceId;
	long m_lMagWidth;
	long m_lDeviveWidth;
	long m_lMagLength;
	long m_lLastSlopToTopH;
	int m_nSlipListId;
	short m_nLastHalfIndex;
	long m_lSlipLength;
};
