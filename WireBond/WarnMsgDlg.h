#pragma once

#include "resource.h"

// CWarnMsgDlg dialog

class CWarnMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CWarnMsgDlg)
public:
	CWarnMsgDlg(short nCallBackId = 0, short nClearTrack = 0, CWnd* pParent = NULL);   // standard constructor
	virtual ~CWarnMsgDlg();
// Dialog Data
	enum { IDD = IDD_MESSAGE_BOX };
public:
    afx_msg void OnBnClickedButtonClearTrack();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:		//function
	void SetMessageBoxTitle(short nMsgID, short nMsgType);
	void SetMessageList(CStringList& sList);
    virtual BOOL OnInitDialog();
public:		//variable
	CString m_sMsgTitle;
	short m_nMsgType;
	short m_nCallBackId;
	// Clear track flag
	short m_nClearTrack;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy();
    virtual void OnOK();
	DECLARE_MESSAGE_MAP()
private:	//variable
    CString m_sMsg0;
    CString m_sMsg1;
    CString m_sMsg2;
    CString m_sMsg3;
    CBrush m_DlgBkBrush;
	CFont font;
};
