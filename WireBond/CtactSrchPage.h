#pragma once

#include "Program.h"
#include "MachineData.h"
#include "Wire.h"

// CCtactSrchPage dialog
class CCtactSrchPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCtactSrchPage)
public:
	CCtactSrchPage();   // standard constructor
	virtual ~CCtactSrchPage();
// Dialog Data
	enum { IDD = IDD_CTACT_SRCH };
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnStart();
    afx_msg void OnNext();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnFocusCtactTol();
    afx_msg void OnDestroy();
	afx_msg void OnPrev();
	afx_msg void OnBnClickedButtonDone();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:		//functions
    void UpdateRefInfo(void);
private:	//variables
    short m_nTotalRef;
    short m_nCurRef;
    short m_nSrchTol;
    int m_nSrchSpeed;
    int m_nObjectWireNum;
};
