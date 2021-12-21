#pragma once

enum POP_UP_DLG_SPEED 
{
	SPEED_NORMAL,
	SPEED_SMALL,
	SPEED_FINE,     
};

// CPopUpTopStepperCtr dialog
class CPopUpTopStepperCtr : public CDialog
{
public:

	DECLARE_DYNAMIC(CPopUpTopStepperCtr)

	CPopUpTopStepperCtr(byte nCtrlModuleId, short nSpeed=SPEED_NORMAL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPopUpTopStepperCtr();

// Dialog Data
	enum { IDD = IDD_TOP_STEPPER_CTRL };

	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);		//V1.1.W147 add
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);	//V1.1.W147 add

	afx_msg void OnUpCtr();
	afx_msg void OnDownCtr();
	afx_msg void OnRightCtr();
	afx_msg void OnLeftCtrl();
	afx_msg void OnOk();

	afx_msg void OnScaleIncrease();
	afx_msg void OnScaleDecrease();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRecover();

	void PresetDlgPos(BYTE nDlgPos);
	void Preset2ndCtrlID(BYTE nID);

protected:

	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:

	byte m_nCtrModule;
	BYTE m_n2ndCtrlID;

	short m_nStepResol;

	short m_nSpeed;
	BYTE m_nDlgPos;

	CDC m_dcLeft,m_dcRight,m_dcUp,m_dcDown;	//V1.1.W147 add
};
