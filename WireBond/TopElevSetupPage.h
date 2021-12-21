#pragma once

// CTopElevSetupPage dialog

class CTopElevSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopElevSetupPage)
public:
	CTopElevSetupPage();
	virtual ~CTopElevSetupPage();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_TOP_ELEV_SETUP };
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStart();
	afx_msg void OnNext();
	afx_msg void OnElevatorSide();
	afx_msg void OnFocusSlotNum();
	afx_msg void OnNextSlot();
	afx_msg void OnEditSlotPos();
	afx_msg void OnEditSlotNum();
	afx_msg void OnResetOffset();
	afx_msg void OnBnClickedButtonDone();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.1.W158 add
	DECLARE_MESSAGE_MAP()
private:
	long m_lOnload1stSlotLvl;
	long m_lOffload1stSlotLvl;
	long m_lOnloadTotalSlot;
	long m_lOffloadTotalSlot;
	short m_nStep;
	int m_nElevID;
	short m_nSlotIndex;
	long m_lLastSlotPos;
	long m_lMagSlotPitch;
	long m_lMagSwapPitch;
	BOOL m_bBusy;	//V1.1.W158 add
};
