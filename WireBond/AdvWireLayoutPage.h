#pragma once

// CAdvWireLayoutPage dialog
class CAdvWireLayoutPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAdvWireLayoutPage)
public:
	CAdvWireLayoutPage();
	virtual ~CAdvWireLayoutPage();
    virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
    virtual LRESULT OnWizardNext();
// Dialog Data
	enum { IDD = IDD_ADV_WIRE_LAYOUT };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnTchPadType();
    afx_msg void OnSingleTch();
    afx_msg void OnBondPoint();
    afx_msg void OnBondOn();
    //afx_msg void OnPadType();
    afx_msg void OnGroupTch();
    afx_msg void OnEditPad();
    afx_msg void OnDelete();
    afx_msg void OnEdit();
    afx_msg void OnInsert();
    afx_msg void OnAutoPad();
    afx_msg void OnCurrentPad();
    afx_msg void OnFocusCurBondPt();
    afx_msg void OnDestroy();
    afx_msg void OnFocusCheckWire();
    afx_msg void OnBondOn2();
    afx_msg void OnSwap();
    afx_msg void OnIll();
    afx_msg void OnIllLight();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);		//V1.1.W153 add
	DECLARE_MESSAGE_MAP()
public:		//function
	void CopyUI(CWnd* pWnd);
    // Update menu items
    void UpdateMenuItems(void);
    // Check PR alignment status
    int CheckPrAlignment(void);
private:
    // Number of taught pads
    short m_nTaughtPads;
    // Current pad
    int m_iCurrentPad;
    // Current working bond point
    int m_iBondPt;
    // Reference of current bond point
    int m_i1stBondOn;
    // Number of taught bond points
    short m_nTaught1stPoints;
    // Current bond point
    short m_nCurrentBondPoint;
    // Number of taught 2nd bond points
    short m_nTaught2ndPoints;
    // Wire number of checking wire
    short m_nCheckWireNum;
    // 1st bond reference
    CString m_sBondOn1;
    // 2nd bond reference
    CString m_sBondOn2;
    // 2nd bond reference
    int m_i2ndBondOn;
	BOOL m_bBusy;	//V1.1.W153 add
	HWND m_hwndChild;//V1.1.W153 add
};
