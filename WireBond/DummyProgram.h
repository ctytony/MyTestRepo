#pragma once
#include "afxcmn.h"

// CDummyProgram dialog

class CDummyProgram : public CDialog
{
	DECLARE_DYNAMIC(CDummyProgram)
public:
	CDummyProgram(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDummyProgram();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DUMMY_PROGRAM };
public:
    afx_msg void OnStart();
    afx_msg void OnNext();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusWirePitch();
    afx_msg void OnFocusWireLength();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.4.W42 add
    virtual void PostNcDestroy();
private:
    int m_nPgmType;
    short m_nWirePitch;
    CString m_InfoMessage;
    short m_nTeachStep;
    TBL_POS m_stLeftTopPos, m_stRightBottomPos;
    short m_nWireLen;
	BOOL m_bBusy;	//V1.4.W42 add
};
