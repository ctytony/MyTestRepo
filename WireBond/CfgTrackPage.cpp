// CfgTrackPage.cpp : implementation file
//

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "CfgTrackPage.h"
#include "MaterialHandlerThread.h"


// CCfgTrackPage dialog

IMPLEMENT_DYNAMIC(CCfgTrackPage, CPropertyPage)

CCfgTrackPage::CCfgTrackPage()
	: CPropertyPage(CCfgTrackPage::IDD)
    , m_lClampId(0)
    , m_lTestModule(0)
{

}

CCfgTrackPage::~CCfgTrackPage()
{
}

void CCfgTrackPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_CLAMP_ID, m_lClampId);
    DDX_CBIndex(pDX, IDC_COMBO_MODULE, m_lTestModule);
}


BEGIN_MESSAGE_MAP(CCfgTrackPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_RUN, &CCfgTrackPage::OnBnClickedButtonRun)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CCfgTrackPage::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CCfgTrackPage message handlers

BOOL CCfgTrackPage::OnInitDialog()
{
    CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        pChildWnd->SetFont(theApp.m_pCurrentFont); //all children of the property page
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

	CString szString;
    CWnd* pWnd = GetDlgItem(IDC_STATIC_CLAMPER);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_CLAMPER, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_STATIC_TRACK);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_TRACK_COLON, szString);
    pWnd->SetWindowTextW(szString);

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CLAMP_ID);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_INNER_TRACK, szString);
    pComboBox->InsertString(0, szString);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_OUTER_TRACK, szString);
    pComboBox->InsertString(1, szString);

    pWnd = GetDlgItem(IDC_STATIC_ACTION);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_ACTION, szString);
    pWnd->SetWindowTextW(szString);

    pWnd = GetDlgItem(IDC_STATIC_MODULE);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_MODULE_COLON, szString);
    pWnd->SetWindowTextW(szString);

    pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MODULE);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_ONLOAD_TRACK, szString);
    pComboBox->InsertString(0, szString);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_PRE_TRACK, szString);
    pComboBox->InsertString(1, szString);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_POST_TRACK, szString);
    pComboBox->InsertString(2, szString);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_STOPPER, szString);
    pComboBox->InsertString(3, szString);

    CPoint pt;
    pt.x = TREE_BUTTON_LEFT + 15;
	pt.y = TREE_BUTTON_TOP - 425;

    pWnd = GetDlgItem(IDC_BUTTON_RUN);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_ON, szString);
    pWnd->SetWindowTextW(szString);

	pt.x += TREE_BUTTON_WIDTH;

	pWnd = GetDlgItem(IDC_BUTTON_STOP);
    pWnd->SetWindowPos(NULL, pt.x, pt.y, TREE_BUTTON_WIDTH, TREE_BUTTON_HEIGHT, SWP_NOZORDER);
    theMsgMgr.GetMsgString(MSG_MENU_ITEM_OFF, szString);
    pWnd->SetWindowTextW(szString);

    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCfgTrackPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(RGB(0, 128, 128));
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CCfgTrackPage::OnBnClickedButtonRun()
{
    // TODO: Add your control notification handler code here
    UpdateData(true);

    CLeadFrameTrack* pLfTrack;

    if (m_lClampId == INNER_TRACK)
        pLfTrack = &theLfInnerTrack;
    else
        pLfTrack = &theLfOuterTrack;

    switch (m_lTestModule)
    {
    case 0:
        pLfTrack->theOnloadTrackRoller.SetRelayState(RELAY_ON);
        break;
    case 1:
        pLfTrack->thePreBondTrackRoller.SetRelayState(RELAY_ON);
        break;
    case 2:
        pLfTrack->thePostBondTrackRoller.SetRelayState(RELAY_ON);
        break;
    case 3:
        pLfTrack->theBondTrackStopper.SetRelayState(RELAY_ON);
        break;
    default:
        break;
    }
}

void CCfgTrackPage::OnBnClickedButtonStop()
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    
    CLeadFrameTrack* pLfTrack;

    if (m_lClampId == INNER_TRACK)
        pLfTrack = &theLfInnerTrack;
    else
        pLfTrack = &theLfOuterTrack;

    switch (m_lTestModule)
    {
    case 0:
        pLfTrack->theOnloadTrackRoller.SetRelayState(RELAY_OFF);
        break;
    case 1:
        pLfTrack->thePreBondTrackRoller.SetRelayState(RELAY_OFF);
        break;
    case 2:
        pLfTrack->thePostBondTrackRoller.SetRelayState(RELAY_OFF);
        break;
    case 3:
        pLfTrack->theBondTrackStopper.SetRelayState(RELAY_OFF);
        break;
    default:
        break;
    }
}
