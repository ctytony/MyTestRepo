// LoopLastKinkPage.cpp : implementation file

#include "stdafx.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "LoopPara.h"
#include "AdvLoopParaSheet.h"
#include "LoopLastKinkPage.h"
#include "User.h"

// CLoopLastKinkPage dialog

IMPLEMENT_DYNAMIC(CLoopLastKinkPage, CPropertyPage)

CLoopLastKinkPage::CLoopLastKinkPage()
	: CPropertyPage(CLoopLastKinkPage::IDD)
    , m_nSlopeLen(0)
    , m_nCtactAngleCorrect(0)
    , m_nTwistRatio(0)
    , m_nTwistFwdHtOffset(0)
    , m_nTwistWireFeedSpeed(0)
    , m_nTwistRevBentSpeed(0)
    , m_nTwistFwdBentSpeed(0)
{

}

CLoopLastKinkPage::~CLoopLastKinkPage()
{
}

void CLoopLastKinkPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SLOPE_LENGTH, m_nSlopeLen);
    DDX_Text(pDX, IDC_EDIT_CTACT_ANGLE, m_nCtactAngleCorrect);
    DDX_Text(pDX, IDC_EDIT_TWIST_RATIO, m_nTwistRatio);
    DDX_Text(pDX, IDC_EDIT_TWIST_FWD_HT_OFFSET, m_nTwistFwdHtOffset);
    DDX_Text(pDX, IDC_EDIT_KINK4_WIRE_FEED_SPEED, m_nTwistWireFeedSpeed);
    DDX_Text(pDX, IDC_EDIT_TWIST_REV_BENT_SPEED, m_nTwistRevBentSpeed);
    DDX_Text(pDX, IDC_EDIT_TWIST_FWD_BENT_SPEED, m_nTwistFwdBentSpeed);
}


BEGIN_MESSAGE_MAP(CLoopLastKinkPage, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_SLOPE_LENGTH, &CLoopLastKinkPage::OnEnSetfocusEditSlopeLength)
    ON_EN_SETFOCUS(IDC_EDIT_CTACT_ANGLE, &CLoopLastKinkPage::OnEnSetfocusEditCtactAngle)
    ON_EN_SETFOCUS(IDC_EDIT_TWIST_RATIO, &CLoopLastKinkPage::OnEnSetfocusEditTwistRatio)
    ON_EN_SETFOCUS(IDC_EDIT_KINK4_WIRE_FEED_SPEED, &CLoopLastKinkPage::OnEnSetfocusEditKink4WireFeedSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_TWIST_REV_BENT_SPEED, &CLoopLastKinkPage::OnEnSetfocusEditTwistRevBentSpeed)
    ON_EN_SETFOCUS(IDC_EDIT_TWIST_FWD_BENT_SPEED, &CLoopLastKinkPage::OnEnSetfocusEditTwistFwdBentSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_TWIST_FWD_HT_OFFSET, &CLoopLastKinkPage::OnEnSetfocusEditTwistFwdHtOffset)
END_MESSAGE_MAP()


// CLoopLastKinkPage message handlers

HBRUSH CLoopLastKinkPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

BOOL CLoopLastKinkPage::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    CWnd* pWnd = GetDlgItem(IDC_BUTTON_DUMMY);
    pWnd->SetWindowPos(NULL, 1000, 1000, 100, 100, SWP_NOZORDER);

    CPropertyPage::OnInitDialog();

    UpdateMenuVariable();
    return TRUE;
}

int CLoopLastKinkPage::UpdateMenuVariable(void)
{
    short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();
    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    m_nCtactAngleCorrect = stLoopPara.stLoopBase.nCtactAngleCorrect;
    m_nSlopeLen = stLoopPara.stLoopBase.nSlopeLen;
    m_nTwistRatio = stLoopPara.stLoopBase.nTwistRatio;
    m_nTwistFwdHtOffset = stLoopPara.stLoopBase.nTwistFwdHtOffset;
    m_nTwistWireFeedSpeed = stLoopPara.stLoopAdv.nTwistWireFeedSpeed;
    m_nTwistRevBentSpeed = stLoopPara.stLoopAdv.nTwistRevBentSpeed;
    m_nTwistFwdBentSpeed = stLoopPara.stLoopAdv.nTwistFwdBentSpeed;

    int iRowTop = PARA_COLUMN1_TOP;

    CWnd* pWnd;
    pWnd = GetDlgItem(IDC_STATIC_SLOPE_LENGTH);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_SLOPE_LEN));

    pWnd = GetDlgItem(IDC_EDIT_SLOPE_LENGTH);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_TWIST_RATIO);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TWIST_RATIO));

    pWnd = GetDlgItem(IDC_EDIT_TWIST_RATIO);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_TWIST_FWD_HT_OFFSET);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TWIST_FWD_HT_OFFSET));

    pWnd = GetDlgItem(IDC_EDIT_TWIST_FWD_HT_OFFSET);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

    //iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

    pWnd = GetDlgItem(IDC_STATIC_CTACT_ANGLE);
    pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->SetWindowTextW(theMsg.GetMsg(MSG_CTACT_ANGLE_CORRECT));
    pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_EDIT_CTACT_ANGLE);
    pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    pWnd->ShowWindow(SW_HIDE);

    if (theUser.GetUserLevel() > USER_SERVICE)
    {
        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_STATIC_KINK4_WIRE_FEED_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_KINK4_WIRE_FEED_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_KINK4_WIRE_FEED_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_STATIC_TWIST_REV_BENT_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TWIST_REV_BENT_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_TWIST_REV_BENT_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);

        iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

        pWnd = GetDlgItem(IDC_STATIC_TWIST_FWD_BENT_SPEED);
        pWnd->SetWindowPos(NULL, PARA_LABEL_LEFT, iRowTop, SINGLE_PARA_LABEL_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->SetWindowTextW(theMsg.GetMsg(MSG_TWIST_FWD_BENT_SPEED));
        pWnd->ShowWindow(SW_SHOWNORMAL);

        pWnd = GetDlgItem(IDC_EDIT_TWIST_FWD_BENT_SPEED);
        pWnd->SetWindowPos(NULL, LOOP_PARA_LEFT, iRowTop, PARA_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
        pWnd->ShowWindow(SW_SHOWNORMAL);
    }
    else
    {
		GetDlgItem(IDC_STATIC_KINK4_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_KINK4_WIRE_FEED_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TWIST_REV_BENT_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TWIST_REV_BENT_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TWIST_FWD_BENT_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TWIST_FWD_BENT_SPEED)->ShowWindow(SW_HIDE);
    }

    UpdateData(FALSE);
    return 0;
}

void CLoopLastKinkPage::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData();

    short nLoopGroup = ((CAdvLoopParaSheet*)GetParent())->GetLoopGroup();
    ST_LOOP_PARA stLoopPara;
    theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);
    stLoopPara.stLoopBase.nCtactAngleCorrect = m_nCtactAngleCorrect;
    stLoopPara.stLoopBase.nSlopeLen = m_nSlopeLen;
    stLoopPara.stLoopBase.nTwistRatio = m_nTwistRatio;
    stLoopPara.stLoopBase.nTwistFwdHtOffset = m_nTwistFwdHtOffset;
    stLoopPara.stLoopAdv.nTwistWireFeedSpeed = m_nTwistWireFeedSpeed;
    stLoopPara.stLoopAdv.nTwistRevBentSpeed = m_nTwistRevBentSpeed;
    stLoopPara.stLoopAdv.nTwistFwdBentSpeed = m_nTwistFwdBentSpeed;
    theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[nLoopGroup], &stLoopPara);

    (GetParent()->GetParent())->Invalidate();
    CPropertyPage::OnOK();
}

void CLoopLastKinkPage::OnEnSetfocusEditSlopeLength()
{
	GetDlgItem(IDC_BUTTON_DUMMY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nSlopeLen);
	UpdateData(false);
	OnOK();
}

void CLoopLastKinkPage::OnEnSetfocusEditCtactAngle()
{
	GetDlgItem(IDC_BUTTON_DUMMY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nCtactAngleCorrect);
	UpdateData(false);
	OnOK();
}

void CLoopLastKinkPage::OnEnSetfocusEditTwistRatio()
{
	GetDlgItem(IDC_BUTTON_DUMMY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTwistRatio);
	UpdateData(false);
	OnOK();
}

BOOL CLoopLastKinkPage::OnSetActive()
{
    UpdateMenuVariable();
    return CPropertyPage::OnSetActive();
}

void CLoopLastKinkPage::OnEnSetfocusEditKink4WireFeedSpeed()
{
	GetDlgItem(IDC_BUTTON_DUMMY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTwistWireFeedSpeed, 100, 50, 100);
	UpdateData(false);
	OnOK();
}

void CLoopLastKinkPage::OnEnSetfocusEditTwistRevBentSpeed()
{
	GetDlgItem(IDC_BUTTON_DUMMY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTwistRevBentSpeed, 100, 50, 100);
	UpdateData(false);
	OnOK();
}

void CLoopLastKinkPage::OnEnSetfocusEditTwistFwdBentSpeed()
{
	GetDlgItem(IDC_BUTTON_DUMMY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTwistFwdBentSpeed, 100, 50, 100);
	UpdateData(false);
	OnOK();
}

void CLoopLastKinkPage::OnEnSetfocusEditTwistFwdHtOffset()
{
	GetDlgItem(IDC_BUTTON_DUMMY)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nTwistFwdHtOffset, 0, -100, 100);
	UpdateData(false);
	OnOK();
}
