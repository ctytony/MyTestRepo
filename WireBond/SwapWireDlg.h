#pragma once


// CSwapWireDlg dialog

class CSwapWireDlg : public CDialog
{
	DECLARE_DYNAMIC(CSwapWireDlg)

public:
	CSwapWireDlg(short nDefaultWire, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSwapWireDlg();

// Dialog Data
	enum { IDD = IDD_SWAP_WIRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    // First wire number
    short m_nFirstWireNum;
    // Second wire number
    short m_nSecondWireNum;
public:
    afx_msg void OnEnSetfocusEditFirstWire();
    afx_msg void OnEnSetfocusEditSecondWire();
    virtual BOOL OnInitDialog();
protected:
    virtual void OnOK();
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    // Get swap wire number
    int GetSwapWireNumber(short* pn1stSwapWire, short* pn2ndSwapWire);
protected:
    virtual void OnCancel();
};
