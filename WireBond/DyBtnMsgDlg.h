#pragma once


// CDyBtnMsgDlg dialog

class CDyBtnMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CDyBtnMsgDlg)
public:
	CDyBtnMsgDlg(short nBtnOKMsgID=41, short nBtnCancelMsgID=253, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDyBtnMsgDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_BTN_MSG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:	//variable
	CBrush m_DlgBkBrush;
	CFont font;
	short m_nOkBtnText;
	short m_nStopBtnText;
	CString m_sMsg0;
	CString m_sMsg1;
	CString m_sMsg2;
	CString m_sMsg3;
public:		//variable
	bool m_bOkSelected;
	bool m_bCancelSelected;
public:		//function
    int SetMessageList(short nMsgNum, short *pnMsgIndex);
    int SetMessageList(CStringList& sList);
};
