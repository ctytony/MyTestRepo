#pragma once


// CDlgCuEliteElevCtrl dialog

#define INPUT_ELEVATOR          0
#define OUTPUT_ELEVATOR         1

class CDlgCuEliteElevCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgCuEliteElevCtrl)

public:
	CDlgCuEliteElevCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCuEliteElevCtrl();

// Dialog Data
	enum { IDD = IDD_CUELITE_ELEV_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

private:
	CBrush m_BkBrush;
	 // Up arrow bitmap
    CBitmap m_bitmapUpArrow;
    // Down arrow bitmap
    CBitmap m_bitmapDownArrow;

protected:
	virtual void OnOK();

private:
	short m_nCurrentSlot;
public:
	afx_msg void OnEnSetfocusEditCurrentSlot();
private:
	short m_nElevId;
public:
	afx_msg void OnBnClickedButtonElevInput();
	afx_msg void OnBnClickedButtonElevOutput();
	afx_msg void OnEnChangeEditCurrentSlot();
	afx_msg void OnBnClickedButtonUpSlot();
	afx_msg void OnBnClickedButtonDownSlot();
	afx_msg void OnBnClickedButtonElevEject();
	
};
