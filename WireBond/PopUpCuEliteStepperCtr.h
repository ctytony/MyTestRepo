#pragma once

enum POP_UP_CUELITE_DLG_SPEED 
{
	CUELITE_SPEED_NORMAL,
	CUELITE_SPEED_SMALL,
	CUELITE_SPEED_FINE,
};

// CPopUpCuEliteStepperCtr dialog

class CPopUpCuEliteStepperCtr : public CDialog
{
	DECLARE_DYNAMIC(CPopUpCuEliteStepperCtr)

public:

	CPopUpCuEliteStepperCtr(byte nCtrlModuleId, short nSpeed=CUELITE_SPEED_NORMAL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPopUpCuEliteStepperCtr();

// Dialog Data
	enum { IDD = IDD_CUELITE_STEPPER_CTRL };

	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnRight();
	afx_msg void OnLeft();
	afx_msg void OnOk();

	afx_msg void OnBnClickedScaleIncrease();
	afx_msg void OnBnClickedScaleDecrease();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRecover();

	afx_msg void OnClose();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	bool IsOutOfSoftLimit(BYTE bStepperId ,double dStepResol);

	void PresetDlgPos(BYTE nDlgPos);
	void Preset2ndCtrlID(BYTE nID);

protected:

	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void RefreshData();

	 // Left arrow bitmap
    CBitmap m_bitmapLeftArrow;
    // Right arrow bitmap
    CBitmap m_bitmapRightArrow;
    // Up arrow bitmap
    CBitmap m_bitmapUpArrow;
    // Down arrow bitmap
    CBitmap m_bitmapDownArrow;

	byte m_nCtrModule;
	BYTE m_n2ndCtrlID;

	long m_nStepResol;

	short m_nSpeed;
	BYTE m_nDlgPos;
};
