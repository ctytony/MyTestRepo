// TopResoluCalibPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "TopResoluCalibPage.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBondDlg.h"
#include "MaterialHandlerThread.h"
#include "PopUpTopStepperCtr.h"
#include "..\GkgLib\Motion\StepperControl.h"
#include "..\GkgLib\MathCalc\MathCalc.h"
// CTopResoluCalibPage dialog

IMPLEMENT_DYNAMIC(CTopResoluCalibPage, CPropertyPage)

CTopResoluCalibPage::CTopResoluCalibPage()
	: CPropertyPage(CTopResoluCalibPage::IDD)
{

}

CTopResoluCalibPage::~CTopResoluCalibPage()
{
}

void CTopResoluCalibPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTopResoluCalibPage, CPropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CTopResoluCalibPage message handlers

HBRUSH CTopResoluCalibPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CTopResoluCalibPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	return CPropertyPage::OnInitDialog();
}

void CTopResoluCalibPage::OnOK()
{
	CPropertyPage::OnOK();
}
