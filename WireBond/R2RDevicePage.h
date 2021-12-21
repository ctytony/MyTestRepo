#pragma once


// CR2RDevicePage dialog

class CR2RDevicePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CR2RDevicePage)

public:
	CR2RDevicePage();
	virtual ~CR2RDevicePage();

// Dialog Data

	enum { IDD = IDD_R2R_DEV_PARA };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:


	long m_lMag1stLvHt;
	long m_lMagLvPitch;
	long m_lLastSlopToTopH;
	short m_lMagSlotNum;

	short m_nDeviceRowNum;
	short m_nDeviceColmNum;
	long m_lDeviceColmPitch;
	short m_nWndUnitNum;
	short m_nDeviceId;



public:

	afx_msg void OnEnSetfocusEditSlipRowNum();
	afx_msg void OnEnSetfocusEditSlipColumNum();
	afx_msg void OnEnSetfocusEditSlipPitch();
	afx_msg void OnEnSetfocusEditWndColumNum();
	
public:

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
private:
	long m_lSlipLength;
public:
	afx_msg void OnEnSetfocusEditSlipLen();

};
