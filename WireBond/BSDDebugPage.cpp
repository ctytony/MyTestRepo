// BSDDebugPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "BSDDebugPage.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MenuConfig.h"
#include "Bond.h"
#include "BondStickDetect.h"
//#include "BSD_DLL.h"
#include "BSDDebug.h"



// CBSDDebugPage dialog


IMPLEMENT_DYNAMIC(CBSDDebugPage, CDialog)

CBSDDebugPage::CBSDDebugPage(CWnd* pParent /*=NULL*/)
	: CDialog(CBSDDebugPage::IDD, pParent)
	, m_checkbox(FALSE)
	, m_AutoSampleCheck(FALSE)
	, m_DieVolt(6.8)
	, m_LeadVolt(6.8)
	, m_NSOLThreshd(36)
	, m_TailSrtThreshd(1)
	, m_nCounter(0)
{

}

CBSDDebugPage::~CBSDDebugPage()
{
}

void CBSDDebugPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_BSDEBUG_ENABLE, m_checkbox);
	DDX_Text(pDX, IDC_TXT_DIE_V, m_DieVolt);
	DDX_Text(pDX, IDC_TXT_LEAD_V, m_LeadVolt);
	DDX_Text(pDX, IDC_TXT_NSOL_THOD, m_NSOLThreshd);
	DDX_Text(pDX, IDC_TXT_TAILS_THOD2, m_TailSrtThreshd);
	DDX_Check(pDX, IDC_CHECK_AUTO_SAMPLE, m_AutoSampleCheck);

	DDX_Text(pDX, IDC_EDIT_NCOUNTER, m_nCounter);
}


BEGIN_MESSAGE_MAP(CBSDDebugPage, CDialog)
	 ON_WM_CTLCOLOR()
	 ON_WM_CLOSE()
	 ON_WM_DESTROY()
	 ON_BN_CLICKED(IDOK, &CBSDDebugPage::OnBnClickedOk)
	 //ON_BN_CLICKED(IDC_BUTTON_SAMPLE_BOND, &CBSDDebugPage::OnBnClickedButtonSampleBond)
	 ON_BN_CLICKED(IDC_CHECK_BSDEBUG_ENABLE, &CBSDDebugPage::OnBnClickedCheckBsdebugEnable)
	 //ON_BN_CLICKED(IDC_BUTTON_SAVE_PARA, &CBSDDebugPage::OnBnClickedButtonSavePara)
	 ON_BN_CLICKED(IDC_CHECK_AUTO_SAMPLE, &CBSDDebugPage::OnBnClickedCheckAutoSample)
	 ON_BN_CLICKED(IDC_BUTTON_RESET, &CBSDDebugPage::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CBSDDebugPage message handlers

void CBSDDebugPage::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
    
	CDialog::PostNcDestroy();
	delete this;
	
}

void CBSDDebugPage::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	((CWireBondDlg*)(this->GetParent()))->EnableSystemButton();	

     DestroyWindow();
}

void  CBSDDebugPage::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	DestroyWindow();
}

HBRUSH CBSDDebugPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
       pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}


BOOL CBSDDebugPage::OnInitDialog()
{
	//CDialog::OnInitDialog();
	

    CWnd* pChildWnd = GetWindow(GW_CHILD);

    while (pChildWnd)
    {
        pChildWnd->SetFont(theApp.m_pCurrentFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

	((CWireBondDlg*)(this->GetParent()))->InhibitSystemButton();

	CRect Rect;
	(this->GetParent())->GetWindowRect(&Rect);

	CPoint pt(Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP);
    SetWindowPos(NULL, pt.x, pt.y, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

    CString szString;

	CWnd* pWnd = GetDlgItem(IDC_STATIC_BSD_ENABLE);
    pWnd->SetWindowPos(NULL, FIRST_ROW_TOP, FIRST_ROW_TOP, MODEL_LESS_WIDTH, TREE_INFO_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_BSD_RECORD, szString);
    pWnd->SetWindowTextW(szString);


	pt.y =VISION_VIEW_TOP;

	pWnd = GetDlgItem(IDC_CHECK_BSDEBUG_ENABLE);
    pWnd->SetWindowPos(NULL,TREE_DONE_BUTTON_LEFT, pt.y, COMBO_WIDTH,PARA_LABEL_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_BSD_RECORD, szString);
    pWnd->SetWindowTextW(szString);

	

	pt.y = FIRST_ROW_TOP+TREE_INFO_HEIGHT;

	 pWnd = GetDlgItem(IDC_STATIC_RECOMM_PARA);
    pWnd->SetWindowPos(NULL, FIRST_ROW_TOP, pt.y , MODEL_LESS_WIDTH, CTACT_INFO_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_BSD_PARA, szString);
    pWnd->SetWindowTextW(szString);

	pt.x = TREE_DONE_BUTTON_LEFT;
	pt.y += VISION_VIEW_TOP;

	pWnd = GetDlgItem(IDC_CHECK_AUTO_SAMPLE);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,BTOS_LABEL_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_AUTO_SAMPLE, szString);
    pWnd->SetWindowTextW(szString);

	pt.y += PARA_COLUMN1_TOP+TREE_BUTTON_HEIGHT;

	 pWnd = GetDlgItem(IDC_EDIT_NCOUNTER);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,PARA_COLUMN1_WIDTH, PARA_COLUMN1_HEIGHT, SWP_NOZORDER);
	
	pt.y +=PARA_COLUMN1_HEIGHT+TXT_GAP;

	pWnd = GetDlgItem(IDC_BUTTON_RESET);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_RESET, szString);
    pWnd->SetWindowTextW(szString);



	
   pt.x= CTACT_COLUMN1_LEFT;
   pt.y= SECOND_ROW_TOP;

    pWnd = GetDlgItem(IDC_STATIC_V1);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,BTOS_LABEL_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_DIE_THRESDHOLD, szString);
    pWnd->SetWindowTextW(szString);

   pt.y+=PARA_COLUMN2_HEIGHT+TXT_GAP;

    pWnd = GetDlgItem(IDC_STATIC_V2);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,BTOS_LABEL_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_LEAD_THRESDHOLD, szString);
    pWnd->SetWindowTextW(szString);

   pt.y+=PARA_COLUMN2_HEIGHT+TXT_GAP;

    pWnd = GetDlgItem(IDC_STATIC_T1);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,BTOS_LABEL_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_NONSTICK_THRESDHOLD, szString);
    pWnd->SetWindowTextW(szString);

   pt.y+=PARA_COLUMN2_HEIGHT+TXT_GAP;

    pWnd = GetDlgItem(IDC_STATIC_T2);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,BTOS_LABEL_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_MENU_ITEM_TAIL_SHORT_THRESDHOLD, szString);
    pWnd->SetWindowTextW(szString);


   pt.x =CTACT_COLUMN1_LEFT+BTOS_LABEL_WIDTH;
   pt.y =SECOND_ROW_TOP;

    pWnd = GetDlgItem(IDC_TXT_DIE_V);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	
   pt.y+=PARA_COLUMN2_HEIGHT+TXT_GAP;

    pWnd = GetDlgItem(IDC_TXT_LEAD_V);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	
   pt.y+=PARA_COLUMN2_HEIGHT+TXT_GAP;

    pWnd = GetDlgItem(IDC_TXT_NSOL_THOD);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	
   pt.y+=PARA_COLUMN2_HEIGHT+TXT_GAP;

    pWnd = GetDlgItem(IDC_TXT_TAILS_THOD2);
    pWnd->SetWindowPos(NULL, pt.x, pt.y ,TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	



	pt.x = TREE_DONE_BUTTON_LEFT;
	pt.y = TREE_BUTTON_TOP;

	pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_DONE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
	theMsgMgr.GetMsgString(MSG_DIALOG_BUTTON_DONE, szString);
    pWnd->SetWindowTextW(szString);



	MACHINE_DATA stMachineData;
    theMachineData.GetMachineDataRcd(&stMachineData);

     m_DieVolt         =(double)stMachineData.fDieShortThreshold ; 
     m_LeadVolt        =(double)stMachineData.fLeadShortThreshold;
     m_TailSrtThreshd  =stMachineData.nTailShortSample  ;  
     m_NSOLThreshd     =stMachineData.nNonStickSample ;


	 m_nCounter=theBSDDebug.GetSamplingCounter();
	//UpdateData(false);

	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: O
}




void CBSDDebugPage::OnBnClickedCheckAutoSample()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if(m_AutoSampleCheck==TRUE)
	{
	   theBSDDebug.ResetSampling();
	   theBSDDebug.SetAutoSamplingFlag(true);
	}

	else
       theBSDDebug.SetAutoSamplingFlag(false);

	 m_nCounter=theBSDDebug.GetSamplingCounter();

    UpdateData(false);

}

void CBSDDebugPage::OnBnClickedCheckBsdebugEnable()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

   if( m_checkbox==TRUE)
   
	theBSDDebug.SetErrorLogFlag(true);

   else
    theBSDDebug.SetErrorLogFlag(false);
 

}




void CBSDDebugPage::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	theBSDDebug.ResetFalseAlmcnter();
	theBSDDebug.ResetSampling();
	theBSDDebug.SetAutoSamplingFlag(FALSE);

}
