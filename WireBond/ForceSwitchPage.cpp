// ForceSwitchPage.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "ForceSwitchPage.h"
#include "MenuConfig.h"
#include "ForceSwitch.h "

// CForceSwitchPage 对话框

IMPLEMENT_DYNAMIC(CForceSwitchPage, CPropertyPage)

CForceSwitchPage::CForceSwitchPage()
	: CPropertyPage(CForceSwitchPage::IDD)
	, m_iFsw1(0)
	, m_iFsw2(0)
	, m_iFsw3(0)
	, m_iFsw4(0)
	, m_iFsw5(0)
	, m_iFsw6(0)
	, m_iFsw7(0)
	, m_iFsw8(0)
{

}

CForceSwitchPage::~CForceSwitchPage()
{
}

void CForceSwitchPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_FSW1, m_iFsw1);
	DDX_CBIndex(pDX, IDC_COMBO_FSW2, m_iFsw2);
	DDX_CBIndex(pDX, IDC_COMBO_FSW3, m_iFsw3);
	DDX_CBIndex(pDX, IDC_COMBO_FSW4, m_iFsw4);
	DDX_CBIndex(pDX, IDC_COMBO_FSW5, m_iFsw5);
	DDX_CBIndex(pDX, IDC_COMBO_FSW6, m_iFsw6);
	DDX_CBIndex(pDX, IDC_COMBO_FSW7, m_iFsw7);
	DDX_CBIndex(pDX, IDC_COMBO_FSW8, m_iFsw8);
}


BEGIN_MESSAGE_MAP(CForceSwitchPage, CPropertyPage)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_FSW1, &CForceSwitchPage::OnCbnSelchangeComboFsw1)
	ON_CBN_SELCHANGE(IDC_COMBO_FSW2, &CForceSwitchPage::OnCbnSelchangeComboFsw2)
	ON_CBN_SELCHANGE(IDC_COMBO_FSW3, &CForceSwitchPage::OnCbnSelchangeComboFsw3)
	ON_CBN_SELCHANGE(IDC_COMBO_FSW4, &CForceSwitchPage::OnCbnSelchangeComboFsw4)
	ON_CBN_SELCHANGE(IDC_COMBO_FSW5, &CForceSwitchPage::OnCbnSelchangeComboFsw5)
	ON_CBN_SELCHANGE(IDC_COMBO_FSW6, &CForceSwitchPage::OnCbnSelchangeComboFsw6)
	ON_CBN_SELCHANGE(IDC_COMBO_FSW7, &CForceSwitchPage::OnCbnSelchangeComboFsw7)
	ON_CBN_SELCHANGE(IDC_COMBO_FSW8, &CForceSwitchPage::OnCbnSelchangeComboFsw8)
END_MESSAGE_MAP()


// CForceSwitchPage 消息处理程序

BOOL CForceSwitchPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	
    UpdateMenuVariable();
	return	CPropertyPage::OnInitDialog();
}

BOOL CForceSwitchPage::OnSetActive()
{
	UpdateMenuVariable();
	return CPropertyPage::OnSetActive();
}

void CForceSwitchPage::OnDestroy()
{
	SaveParameter();
	CPropertyPage::OnDestroy();
}

HBRUSH CForceSwitchPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CForceSwitchPage::UpdateMenuVariable(void)
{
	int iForceSwitch = theForceSwitch.GetRcd()->iForceSwitch;        //v1.6.P136sc  edit      
	UINT iFswBinary[8] = {0};
    for (int i = 0 ; i < 8; i++)
    {
       iFswBinary[i]= iForceSwitch % 2;
	   iForceSwitch = iForceSwitch / 2;
    }

	m_iFsw1 = iFswBinary[0];
	m_iFsw2 = iFswBinary[1];
	m_iFsw3 = iFswBinary[2];
	m_iFsw4 = iFswBinary[3];
	m_iFsw5 = iFswBinary[4];
	m_iFsw6 = iFswBinary[5];
	m_iFsw7 = iFswBinary[6];
	m_iFsw8 = iFswBinary[7];

	int iRowTop = PARA_COLUMN1_TOP;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_FSW1);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW1)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FSW2);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW2)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FSW3);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW3)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FSW4);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW4)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FSW5);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW5)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FSW6);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW6)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FSW7);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW7)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	pWnd = GetDlgItem(IDC_STATIC_FSW8);
	pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FSW8)->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	UpdateData(FALSE);
}

void CForceSwitchPage::SaveParameter(void)
{
	UpdateData();

	int iForceSwitch;                                  //v1.6.P136sc  edit
	char *e;
	char chForceSwitch[9] = ("");
	chForceSwitch[0] = m_iFsw8 == 0 ? 48 : 49;
	chForceSwitch[1] = m_iFsw7 == 0 ? 48 : 49;
	chForceSwitch[2] = m_iFsw6 == 0 ? 48 : 49;
	chForceSwitch[3] = m_iFsw5 == 0 ? 48 : 49;
	chForceSwitch[4] = m_iFsw4 == 0 ? 48 : 49;
	chForceSwitch[5] = m_iFsw3 == 0 ? 48 : 49;
	chForceSwitch[6] = m_iFsw2 == 0 ? 48 : 49;
	chForceSwitch[7] = m_iFsw1 == 0 ? 48 : 49;

	iForceSwitch = strtol(chForceSwitch, &e, 2);   //v1.6.P136sc edit
	theForceSwitch.GetRcd()->iForceSwitch = iForceSwitch;

	(GetParent()->GetParent())->Invalidate();
}

void CForceSwitchPage::OnCbnSelchangeComboFsw1()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}

void CForceSwitchPage::OnCbnSelchangeComboFsw2()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}

void CForceSwitchPage::OnCbnSelchangeComboFsw3()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}

void CForceSwitchPage::OnCbnSelchangeComboFsw4()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}

void CForceSwitchPage::OnCbnSelchangeComboFsw5()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}

void CForceSwitchPage::OnCbnSelchangeComboFsw6()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}

void CForceSwitchPage::OnCbnSelchangeComboFsw7()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}

void CForceSwitchPage::OnCbnSelchangeComboFsw8()
{
	UpdateData(true);
	SaveParameter();
	UpdateData(false);
}
