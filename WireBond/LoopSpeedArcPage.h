#pragma once


// CLoopSpeedArcPage 对话框

class CLoopSpeedArcPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CLoopSpeedArcPage)

public:
	CLoopSpeedArcPage();
	virtual ~CLoopSpeedArcPage();

// 对话框数据
	enum { IDD = IDD_LOOP_SPEED_ARC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnEnSetfocusEditSpdRh();
	afx_msg void OnEnSetfocusEditSpdRd();
	afx_msg void OnEnSetfocusEditSpdBeuH();
	afx_msg void OnEnSetfocusEditSpdBeuDist();
	afx_msg void OnEnSetfocusEditSpdHl();
	afx_msg void OnEnSetfocusEditSpdRd2();
	afx_msg void OnEnSetfocusEditSpdSl();
	afx_msg void OnEnSetfocusEditSpdTraj();
	afx_msg void OnEnSetfocusEditArcRd();
	afx_msg void OnEnSetfocusEditArcRd2();
	afx_msg void OnEnSetfocusEditArcBeu();
	afx_msg void OnEnSetfocusEditArcLpVop();
	afx_msg void OnEnSetfocusEditArcVaj();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
private:
	short m_nSpdbEeuH;
	short m_nSpdBeuDist;
	short m_nSpdHL;
	short m_nSpdRD2;
	short m_nSpdSL;
	short m_nSpdTraj;
	short m_nArcRD;
	short m_nArcRD2;
	short m_nArcBeu;
	short m_nArcLpVop;
	short m_nArcVaj;
	short m_nSpdRH;
	short m_nSpdRD;
public:
	virtual BOOL OnSetActive();
	void UpdateMenuVariable();
private:
	void SaveParameter();
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	short m_nPageSize;

};
