#pragma once

// CDlgOverView dialog
class CDlgOverView : public CDialog
{
public:

	CDlgOverView();   // standard constructor
	virtual ~CDlgOverView();

// Dialog Data
	enum { IDD = IDD_OVERVIEW };

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	//V1.5.W88 add

	BYTE GetMatrixRow();	//V1.5.W88 add
	BYTE SetMatrixRow(BYTE nRow);	//V1.5.W89 add
	void DrawMatrixRow();	//V1.5.W88 add

protected:

	DECLARE_MESSAGE_MAP()

	BYTE m_nRow;		//V1.5.W88 add
};
