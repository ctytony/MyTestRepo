#pragma once

// CHelpMsgDlg dialog

class CHelpMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CHelpMsgDlg)
public:
	CHelpMsgDlg(bool bStopValid = false, bool bSkipValid = false, short nCallBackId = 0, bool bSpeedUpButton=false,CWnd* pParent = NULL,bool bCtrLight=FALSE, short nMode=0);   // standard constructor
	virtual ~CHelpMsgDlg();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_MESSAGE_BOX };
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonStop();
    afx_msg void OnBnClickedButtonSkip();
    afx_msg void OnBnClickedButtonSpeedDown();
    afx_msg void OnBnClickedButtonSpeedUp();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedLight();
public:		//function
    void SetMessageBoxTitle(short nMsgID, short nMsgType);
    void SetMessageList(short nMsgNum, short *pnMsgIndex);
    void SetMessageList(CStringList& sList);
	void CenterDialog(bool bCenter);
public:		//variable
	// Stop button pressed
    bool m_bStopSelected;
    // Skip selected
    bool m_bSkipSelected;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
    virtual void OnOK();
private:	//variable
	bool m_bCenter;
    CString m_sMsg0;
    CString m_sMsg1;
    CString m_sMsg2;
    CString m_sMsg3;
    CString m_sMsgTitle;
    CBrush m_DlgBkBrush;
	CFont font;
    short m_nMsgType;
    bool m_bStopButtonValid;
    bool m_bSkipButtonValid;
    short m_nCallBackId;
    bool m_bSpeedUpButton;
	//light Ctr flag during PR manual alignment
	bool m_bCtrLight;
	//add select different word in display only
	short m_nMode;
};
