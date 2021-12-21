#pragma once

// CCuEliteElevSetupPage dialog

#define ROUGH_TRACK_HEIGHT    190000           //um

class CCuEliteElevSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteElevSetupPage)

public:

	CCuEliteElevSetupPage();
	virtual ~CCuEliteElevSetupPage();

// Dialog Data
	enum { IDD = IDD_TOP_ELEV_SETUP };

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

	afx_msg void OnStart();
	afx_msg void OnNext();

	afx_msg void OnCbnSelchangeComboElevatorSide();

	afx_msg void OnEnSetfocusEditSlotNum();
	afx_msg void OnBnClickedButtonNextSlot();
	afx_msg void OnBnClickedButtonEditSlotPos();
	afx_msg void OnEnChangeEditSlotNum();

	afx_msg void OnBnClickedButtonResetOffset();

	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();

	afx_msg void OnEnSetfocusEditTotalSlotNum();
	afx_msg void OnEnChangeEditTotalSlotNum();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

	long m_lOnload1stSlotLv;
	long m_lOffload1stSlotLv;
	short m_nStep;

	int m_iElevatorId;

	short m_nSlotIndex;

	long m_lLastSlotPos;

	long m_lMagSlotPitch;
	long m_lMagSwapPitch;
	short m_nTotalSlotNum;

	CUELITE_ONLOAD_RCD* m_lpstOnloadData;
	CUELITE_OFFLOAD_RCD* m_lpstOffloadData;
	CUELITE_DEVICE_RECORD* m_lpstCuEliteDevicePara;
public:
	afx_msg void OnDestroy();
};
