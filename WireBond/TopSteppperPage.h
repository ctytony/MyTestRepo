#pragma once

// CTopSteppperPage dialog
class CTopSteppperPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTopSteppperPage)

public:

	CTopSteppperPage();
	virtual ~CTopSteppperPage();

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSideId();
	afx_msg void OnHolderHome();
	afx_msg void OnMagHolderLimit();
	afx_msg void OnPullyHome();
	afx_msg void OnPullyLimit();
    afx_msg void OnClamperHome();
	afx_msg void OnClamperForWard();
    afx_msg void OnClamperBackWard();
    afx_msg void OnClamperStandBy();
    afx_msg void OnClamperStop();
	afx_msg void OnNextSlot();
	afx_msg void OnStandby();
	afx_msg void OnUnload();
	afx_msg void OnPusherHome();
	afx_msg void OnPusherLimit();

// Dialog Data
	enum { IDD = IDD_TOP_ELEV_DEBUG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT IsDialogBusy(WPARAM wParam, LPARAM lParam);	//V1.1.W248 add

	DECLARE_MESSAGE_MAP()

private:

	int m_iElevatorId;
	BOOL m_bBusy;		//V1.1.W248 add
};
