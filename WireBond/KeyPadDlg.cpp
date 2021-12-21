/***************************************************************
KeyPadDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "string.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "KeyPadDlg.h"

extern CJpeg theJpegHelp;

// CKeyPadDlg dialog
IMPLEMENT_DYNAMIC(CKeyPadDlg, CDialog)

CKeyPadDlg::CKeyPadDlg(CString sRef, long lLowLimit, long lUpLimit, bool bPassword, CWnd* pParent /*=NULL*/)
	: CDialog(CKeyPadDlg::IDD, pParent)
    , m_sKeyinString(_T(""))
    , m_sDisplayString(_T(""))
    , m_sLowLimit(_T(""))
    , m_sUpLimit(_T(""))
    , m_bExpandState(false)
	, m_sRef(_T(""))
{
    m_lLowLimit = lLowLimit;
    m_lUpLimit = lUpLimit;
    m_bKeyinPassword = bPassword;
    m_bExpandState = false;
	m_bCancelSelected = false;
	m_sRef = sRef;
}

CKeyPadDlg::~CKeyPadDlg()
{
}

void CKeyPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEYIN, m_sDisplayString);
	DDX_Text(pDX, IDC_STATIC_LOW_LMT, m_sLowLimit);
	DDX_Text(pDX, IDC_STATIC_HIGH_LMT, m_sUpLimit);
	DDX_Text(pDX, IDC_STATIC_REF, m_sRef);
}

BEGIN_MESSAGE_MAP(CKeyPadDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()	//V1.1.W126 add
	ON_COMMAND_RANGE(IDC_BUTTON_NUM_0, IDC_BUTTON_NUM_9, OnNumbers)
	ON_COMMAND_RANGE(IDC_BUTTON_A, IDC_BUTTON_Z, OnAlphas)
    ON_BN_CLICKED(IDC_BUTTON_MINUS, &CKeyPadDlg::OnBnClickedButtonMinus)
    ON_BN_CLICKED(IDC_BUTTON_DOT, &CKeyPadDlg::OnBnClickedButtonDot)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CKeyPadDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDOK, &CKeyPadDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CKeyPadDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_EXPAND, OnExpand)
    ON_BN_CLICKED(IDC_BUTTON_LINE, &CKeyPadDlg::OnBnClickedButtonLine)
	ON_STN_CLICKED(IDC_STATIC_REF, &CKeyPadDlg::OnStnClickedStaticRef)
    ON_EN_CHANGE(IDC_EDIT_KEYIN, &CKeyPadDlg::OnEnChangeEditKeyin)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CKeyPadDlg message handlers

BOOL CKeyPadDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
	SetWindowPos(NULL, VISION_VIEW_WIDTH/2, VISION_VIEW_HEIGHT/3,
					426, 401, SWP_NOZORDER);	//V1.1.W38 edit

    SetDlgItemTextW(IDC_BUTTON_EXPAND, theMsg.GetMsg(MSG_KEYBOARD_EXPAND));
    SetDlgItemTextW(IDC_BUTTON_CLEAR, theMsg.GetMsg(MSG_CLEAR));
    SetDlgItemTextW(IDOK, theMsg.GetMsg(MSG_DONE));
    SetDlgItemTextW(IDCANCEL, theMsg.GetMsg(MSG_CANCEL));

    if ((m_lLowLimit == 0) && (m_lUpLimit == 0))
    {
        GetDlgItem(IDC_STATIC_RANGE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_LOW_LMT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_HIGH_LMT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_REF)->ShowWindow(SW_HIDE);
    }
    else
    {
        GetDlgItem(IDC_STATIC_RANGE)->ShowWindow(SW_SHOWNORMAL);
        GetDlgItem(IDC_STATIC_LOW_LMT)->ShowWindow(SW_SHOWNORMAL);
        GetDlgItem(IDC_STATIC_HIGH_LMT)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_STATIC_REF)->ShowWindow(SW_SHOWNORMAL);

        m_sLowLimit.Format(_T("%d"), m_lLowLimit);
        m_sUpLimit.Format(_T("%d"), m_lUpLimit);
    }

    return CDialog::OnInitDialog();
}

HBRUSH CKeyPadDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)	//V1.1.W126 edit
{
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_RANGE)
	{
		pDC->SetBkMode(TRANSPARENT);
		return theApp.m_brNull;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CKeyPadDlg::OnNumbers(UINT nID)
{
	CString sz((TCHAR)(nID - IDC_BUTTON_NUM_0 + '0'));

    m_sKeyinString.Append(sz);
	m_sDisplayString.Append(m_bKeyinPassword ? _T("*") : sz);
    
    UpdateData(false);
}

void CKeyPadDlg::OnAlphas(UINT nID)
{
	CString sz((TCHAR)(nID - IDC_BUTTON_A + 'A'));

    m_sKeyinString.Append(sz);
	m_sDisplayString.Append(m_bKeyinPassword ? _T("*") : sz);

    UpdateData(false);
}

void CKeyPadDlg::OnBnClickedButtonMinus()
{
    m_sKeyinString.Insert(0, _T("-"));
    if (m_bKeyinPassword)
        m_sDisplayString.Append(_T("*"));
    else
        m_sDisplayString.Append(_T("-"));
    
    UpdateData(false);
}

void CKeyPadDlg::OnBnClickedButtonDot()
{
    m_sKeyinString.Append(_T("."));
    if (m_bKeyinPassword)
        m_sDisplayString.Append(_T("*"));
    else
        m_sDisplayString.Append(_T("."));
    
    UpdateData(false);
}

void CKeyPadDlg::OnBnClickedButtonClear()
{
    m_sKeyinString.Empty();
    m_sDisplayString.Empty();
    
    UpdateData(false);
}

void CKeyPadDlg::OnBnClickedOk()
{
    //wchar_t chDataBuffer[255];

    //m_sKeyinString.CopyChars(chDataBuffer, m_sKeyinString.GetLength());
    //chDataBuffer = m_sKeyinString.GetBuffer();

	if (m_sKeyinString.IsEmpty())
		m_sKeyinString = _T("empty");
   
    theKeyPad.UpdateKeyInData(m_sKeyinString.GetBuffer());

    CDialog::OnOK(); //OnOK();
}

void CKeyPadDlg::OnBnClickedCancel()
{
	m_bCancelSelected = true;
    CDialog::OnOK();
}

void CKeyPadDlg::OnExpand()
{
    if (m_bExpandState == false)
    {
        SetWindowPos(NULL, VISION_VIEW_WIDTH/2, VISION_VIEW_TOP+20,
										475, 645, SWP_NOZORDER);
		SetDlgItemTextW(IDC_BUTTON_EXPAND,
						theMsg.GetMsg(MSG_KEYBOARD_CLOSE));
    }
    else
    {
        SetWindowPos(NULL, VISION_VIEW_WIDTH/2, VISION_VIEW_HEIGHT/3,
										426, 401, SWP_NOZORDER);
		SetDlgItemTextW(IDC_BUTTON_EXPAND,
						theMsg.GetMsg(MSG_KEYBOARD_EXPAND));
    }

	m_bExpandState = !m_bExpandState;
	Invalidate();
}

void CKeyPadDlg::OnBnClickedButtonLine()
{
    m_sKeyinString.Append(_T("_"));
    if (m_bKeyinPassword)
        m_sDisplayString.Append(_T("*"));
    else
        m_sDisplayString.Append(_T("_"));
    
    UpdateData(false);
}

void CKeyPadDlg::OnStnClickedStaticRef()
{
	m_sDisplayString = m_sRef;
	UpdateData(false);
}

void CKeyPadDlg::OnEnChangeEditKeyin()
{
    UpdateData();

    if (m_bKeyinPassword)
    {
        short nLength = m_sKeyinString.GetLength();
        m_sKeyinString += m_sDisplayString.GetAt(nLength);
        m_sDisplayString.SetAt(nLength, _T('*'));
        
        UpdateData(FALSE);

        CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_KEYIN);
        pEdit->SetSel(nLength + 1, nLength + 1);
    }
    else
        m_sKeyinString = m_sDisplayString;    
}

BOOL CKeyPadDlg::OnEraseBkgnd(CDC* pDC)	//V1.1.W126 add
{
	CRect rc;
	GetClientRect(&rc);
	theJpegHelp.Paint(pDC->m_hDC,rc);

	return TRUE;
}

LRESULT CKeyPadDlg::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}
