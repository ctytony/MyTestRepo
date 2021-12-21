#pragma once
#include "DataStruct.h"
#include "AutoPadMgr.h"
#include "DataStruct.h"

enum enTchPadMode 
{
    AUTO_PAD_TEACH, 
    AUTO_PAD_EDIT
};

// CTchAutoPadPatternDlg dialog

class CTchAutoPadPatternDlg : public CDialog
{
	DECLARE_DYNAMIC(CTchAutoPadPatternDlg)

public:

	CTchAutoPadPatternDlg(char chTchMode, short nPadId, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTchAutoPadPatternDlg();

// Dialog Data
	enum { IDD = IDD_TCH_AUTO_PAD };

    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    // Calculate pad width and height
    void CalculatePadSize(short* pnWidth, short* pnHeight);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual void OnOK();
    virtual void OnCancel();
    virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()

private:

    // Current working pad pattern record
    AUTO_PAD_TYPE m_stCurPadRcd;

    // Teach step
    short m_nTeachStep;
	CFont font;
    // Pad corner positions
    TBL_POS m_astPadCornerPos[4];
    // Auto pad pattern teach mode
    char m_chTchMode;
    // Auto pad pattern edit ID
    short m_nEditPadId;
};
