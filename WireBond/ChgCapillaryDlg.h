#pragma once

#define CHANGE_CAPILLARY_FORCE 50

// CChgCapillaryDlg dialog

class CChgCapillaryDlg : public CDialog
{
	DECLARE_DYNAMIC(CChgCapillaryDlg)

public:
	CChgCapillaryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChgCapillaryDlg();

// Dialog Data
	enum { IDD = IDD_CHANGE_CAPILLARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
   	CFont font;
protected:
    virtual void PostNcDestroy();
public:
    afx_msg void OnDestroy();
protected:
    virtual void OnOK();
private:
    // Step of procedure
    short m_nStep;
    // Backup of motion axis flag
    int m_lBkupDac;
    // Contact search page
    CTeachRecipeSheet* m_ptheTeachRecipeSheet;
protected:
    virtual void OnCancel();
};
