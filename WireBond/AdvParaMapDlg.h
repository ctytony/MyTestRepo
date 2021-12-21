#pragma once

#include "MapEdit.h"

#define BOND_GROUP_COLOR_00		RGB(255,255,255)  //WHITE
#define BOND_GROUP_COLOR_01		RGB(255,0,0)      //RED
#define BOND_GROUP_COLOR_02		RGB(0,255,0)      //GREEN
#define BOND_GROUP_COLOR_03		RGB(0,0,255)      //BLUE
#define BOND_GROUP_COLOR_04		RGB(255,255,0)    //YELLOW
#define BOND_GROUP_COLOR_05		RGB(255,0,255)    //MAGENTA
#define BOND_GROUP_COLOR_06		RGB(0,255,255)    //CYAN
#define BOND_GROUP_COLOR_07		RGB(192,220,192)  //LIGHT_GREEN
#define BOND_GROUP_COLOR_08		RGB(166,202,240)  //LIGHT_BLUE
#define BOND_GROUP_COLOR_09		RGB(128,0,0)      //DARK_RED
#define BOND_GROUP_COLOR_10		RGB(0,128,0)      //DARK_GREEN
#define BOND_GROUP_COLOR_11		RGB(0,0,128)      //DARK_BLUE
#define BOND_GROUP_COLOR_12		RGB(128,128,0)    //DARK_YELLOW
#define BOND_GROUP_COLOR_13		RGB(128,0,128)    //DARK_MAGENTA
#define BOND_GROUP_COLOR_14		RGB(0,128,128)    //DARK_CYAN

enum enParaApplyMode
{
    PARA_APPLY_GROUP,
    PARA_APPLY_SINGLE,
    PARA_APPLY_ALL,
};

// CAdvParaMapDlg dialog
class CAdvParaMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdvParaMapDlg)
public:
	CAdvParaMapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdvParaMapDlg();
    virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_ADV_PARA_MAP };

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:		//function
    void SetParaApplyMode(int iApplyMode);
    void SetParaMapType(short nParaType);
    void RemoveEditFocus(void);
    void UpdateUnitValue(int iUnitId);
private:	//variable
    short m_nParaMapType;
    short m_nTotalWire;
    CMapEdit *m_paMapEdit[5000];
    short m_nTotalRow;
    CStatic *m_paRowLabel[625];
    short m_anRowValue[625];
    short m_anMapPara[5000];
    short m_nPageSize;	//page scroll size
    long  m_lRefPara;
    long  m_lLowerLimit;
    long  m_lUpperLimit;
    // Brush array for bond parameter group
    CBrush m_aGroupBrush[15];
    int m_iParaApplyMode;
};
