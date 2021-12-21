#pragma once

#include "ColorButton.h"

// CMatrixSchedule dialog

class CMatrixSchedule : public CDialog
{
	DECLARE_DYNAMIC(CMatrixSchedule)

public:
	CMatrixSchedule(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMatrixSchedule();

// Dialog Data
	enum { IDD = IDD_MAT_SCHEDULE };
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnReschedule();
    afx_msg void OnPrevious();
    afx_msg void OnNext();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();
	LRESULT CloseDialog(WPARAM wParam, LPARAM lParam);	//V1.1.W156 add
private:	//variable
	short m_nIndexTime, m_nTotalRow, m_nTotalCol;
	short m_nCurRow, m_nCurCol, m_nCurMat;
    // Button array
    CTypedPtrArray<CObArray, CColorButton*> m_pButtonArray;
private:	//function
	void FreeButtonArray(void);
    void HighLightCurrentMatrix(short nMatrix, BOOL bState);
    void SetMatrixScheduleState(short nMatrix, bool bState);
};
