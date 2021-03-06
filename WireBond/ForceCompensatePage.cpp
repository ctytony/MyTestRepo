// ForceCompensatePage.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "ForceCompensatePage.h"
#include "MenuConfig.h"
#include "KeyPadHandler.h"
#include "ForceSwitch.h"

// CForceCompensatePage 对话框

#define COM_LEFT_ONE       15
#define COM_LEFT_TWO       115
#define COM_LEFT_THREE     215
#define COM_LEFT_FOUR      315
#define COM_SINGLE_WIDTH   160
#define COM_COLUMN2_WIDTH  80

IMPLEMENT_DYNAMIC(CForceCompensatePage, CPropertyPage)

CForceCompensatePage::CForceCompensatePage()
	: CPropertyPage(CForceCompensatePage::IDD)
	, m_nPageSize(0)
	, m_nforce0(0)
	, m_nforce1(0)
	, m_nforce2(0)
	, m_nforce3(0)
	, m_nforce4(0)
	, m_nforce5(0)
	, m_nforce6(0)
	, m_nforce7(0)
	, m_nforce8(0)
	, m_nforce9(0)
	, m_nforce10(0)
	, m_nforce11(0)
	, m_nforce12(0)
	, m_nforce13(0)
	, m_nforce14(0)
	, m_nforce15(0)
	, m_nforce16(0)
	, m_nforce17(0)
	, m_nforce18(0)
	, m_nforce19(0)
	, m_nforce20(0)
	, m_nforce21(0)
	, m_nforce22(0)
	, m_nforce23(0)
	, m_nforce24(0)
	, m_nforce25(0)
	, m_nforce26(0)
	, m_nforce27(0)
	, m_nforce28(0)
	, m_nforce29(0)
	, m_nHmeOne0(0)
	, m_nHmeOne1(0)
	, m_nHmeOne2(0)
	, m_nHmeOne3(0)
	, m_nHmeOne4(0)
	, m_nHmeOne5(0)
	, m_nHmeOne6(0)
	, m_nHmeOne7(0)
	, m_nHmeOne8(0)
	, m_nHmeOne9(0)
	, m_nHmeOne10(0)
	, m_nHmeOne11(0)
	, m_nHmeOne12(0)
	, m_nHmeOne13(0)
	, m_nHmeOne14(0)
	, m_nHmeOne15(0)
	, m_nHmeOne16(0)
	, m_nHmeOne17(0)
	, m_nHmeOne18(0)
	, m_nHmeOne19(0)
	, m_nHmeOne20(0)
	, m_nHmeOne21(0)
	, m_nHmeOne22(0)
	, m_nHmeOne23(0)
	, m_nHmeOne24(0)
	, m_nHmeOne25(0)
	, m_nHmeOne26(0)
	, m_nHmeOne27(0)
	, m_nHmeOne28(0)
	, m_nHmeOne29(0)
	, m_nHmeTwo0(0)
	, m_nHmeTwo1(0)
	, m_nHmeTwo2(0)
	, m_nHmeTwo3(0)
	, m_nHmeTwo4(0)
	, m_nHmeTwo5(0)
	, m_nHmeTwo6(0)
	, m_nHmeTwo7(0)
	, m_nHmeTwo8(0)
	, m_nHmeTwo9(0)
	, m_nHmeTwo10(0)
	, m_nHmeTwo11(0)
	, m_nHmeTwo12(0)
	, m_nHmeTwo13(0)
	, m_nHmeTwo14(0)
	, m_nHmeTwo15(0)
	, m_nHmeTwo16(0)
	, m_nHmeTwo17(0)
	, m_nHmeTwo18(0)
	, m_nHmeTwo19(0)
	, m_nHmeTwo20(0)
	, m_nHmeTwo21(0)
	, m_nHmeTwo22(0)
	, m_nHmeTwo23(0)
	, m_nHmeTwo24(0)
	, m_nHmeTwo25(0)
	, m_nHmeTwo26(0)
	, m_nHmeTwo27(0)
	, m_nHmeTwo28(0)
	, m_nHmeTwo29(0)
{

}

CForceCompensatePage::~CForceCompensatePage()
{
}

void CForceCompensatePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FORCE0, m_nforce0);
	DDX_Text(pDX, IDC_EDIT_FORCE1, m_nforce1);
	DDX_Text(pDX, IDC_EDIT_FORCE2, m_nforce2);
	DDX_Text(pDX, IDC_EDIT_FORCE3, m_nforce3);
	DDX_Text(pDX, IDC_EDIT_FORCE4, m_nforce4);
	DDX_Text(pDX, IDC_EDIT_FORCE5, m_nforce5);
	DDX_Text(pDX, IDC_EDIT_FORCE6, m_nforce6);
	DDX_Text(pDX, IDC_EDIT_FORCE7, m_nforce7);
	DDX_Text(pDX, IDC_EDIT_FORCE8, m_nforce8);
	DDX_Text(pDX, IDC_EDIT_FORCE9, m_nforce9);
	DDX_Text(pDX, IDC_EDIT_FORCE10, m_nforce10);
	DDX_Text(pDX, IDC_EDIT_FORCE11, m_nforce11);
	DDX_Text(pDX, IDC_EDIT_FORCE12, m_nforce12);
	DDX_Text(pDX, IDC_EDIT_FORCE13, m_nforce13);
	DDX_Text(pDX, IDC_EDIT_FORCE14, m_nforce14);
	DDX_Text(pDX, IDC_EDIT_FORCE15, m_nforce15);
	DDX_Text(pDX, IDC_EDIT_FORCE16, m_nforce16);
	DDX_Text(pDX, IDC_EDIT_FORCE17, m_nforce17);
	DDX_Text(pDX, IDC_EDIT_FORCE18, m_nforce18);
	DDX_Text(pDX, IDC_EDIT_FORCE19, m_nforce19);
	DDX_Text(pDX, IDC_EDIT_FORCE20, m_nforce20);
	DDX_Text(pDX, IDC_EDIT_FORCE21, m_nforce21);
	DDX_Text(pDX, IDC_EDIT_FORCE22, m_nforce22);
	DDX_Text(pDX, IDC_EDIT_FORCE23, m_nforce23);
	DDX_Text(pDX, IDC_EDIT_FORCE24, m_nforce24);
	DDX_Text(pDX, IDC_EDIT_FORCE25, m_nforce25);
	DDX_Text(pDX, IDC_EDIT_FORCE26, m_nforce26);
	DDX_Text(pDX, IDC_EDIT_FORCE27, m_nforce27);
	DDX_Text(pDX, IDC_EDIT_FORCE28, m_nforce28);
	DDX_Text(pDX, IDC_EDIT_FORCE29, m_nforce29);
	DDX_Text(pDX, IDC_EDIT_HME1_IDO, m_nHmeOne0);
	DDX_Text(pDX, IDC_EDIT_HME1_ID1, m_nHmeOne1);
	DDX_Text(pDX, IDC_EDIT_HME1_ID2, m_nHmeOne2);
	DDX_Text(pDX, IDC_EDIT_HME1_ID3, m_nHmeOne3);
	DDX_Text(pDX, IDC_EDIT_HME1_ID4, m_nHmeOne4);
	DDX_Text(pDX, IDC_EDIT_HME1_ID5, m_nHmeOne5);
	DDX_Text(pDX, IDC_EDIT_HME1_ID6, m_nHmeOne6);
	DDX_Text(pDX, IDC_EDIT_HME1_ID7, m_nHmeOne7);
	DDX_Text(pDX, IDC_EDIT_HME1_ID8, m_nHmeOne8);
	DDX_Text(pDX, IDC_EDIT_HME1_ID9, m_nHmeOne9);
	DDX_Text(pDX, IDC_EDIT_HME1_ID10, m_nHmeOne10);
	DDX_Text(pDX, IDC_EDIT_HME1_ID11, m_nHmeOne11);
	DDX_Text(pDX, IDC_EDIT_HME1_ID12, m_nHmeOne12);
	DDX_Text(pDX, IDC_EDIT_HME1_ID13, m_nHmeOne13);
	DDX_Text(pDX, IDC_EDIT_HME1_ID14, m_nHmeOne14);
	DDX_Text(pDX, IDC_EDIT_HME1_ID15, m_nHmeOne15);
	DDX_Text(pDX, IDC_EDIT_HME1_ID16, m_nHmeOne16);
	DDX_Text(pDX, IDC_EDIT_HME1_ID17, m_nHmeOne17);
	DDX_Text(pDX, IDC_EDIT_HME1_ID18, m_nHmeOne18);
	DDX_Text(pDX, IDC_EDIT_HME1_ID19, m_nHmeOne19);
	DDX_Text(pDX, IDC_EDIT_HME1_ID20, m_nHmeOne20);
	DDX_Text(pDX, IDC_EDIT_HME1_ID21, m_nHmeOne21);
	DDX_Text(pDX, IDC_EDIT_HME1_ID22, m_nHmeOne22);
	DDX_Text(pDX, IDC_EDIT_HME1_ID23, m_nHmeOne23);
	DDX_Text(pDX, IDC_EDIT_HME1_ID24, m_nHmeOne24);
	DDX_Text(pDX, IDC_EDIT_HME1_ID25, m_nHmeOne25);
	DDX_Text(pDX, IDC_EDIT_HME1_ID26, m_nHmeOne26);
	DDX_Text(pDX, IDC_EDIT_HME1_ID27, m_nHmeOne27);
	DDX_Text(pDX, IDC_EDIT_HME1_ID28, m_nHmeOne28);
	DDX_Text(pDX, IDC_EDIT_HME1_ID29, m_nHmeOne29);
	DDX_Text(pDX, IDC_EDIT_HME2_IDO, m_nHmeTwo0);
	DDX_Text(pDX, IDC_EDIT_HME2_ID1, m_nHmeTwo1);
	DDX_Text(pDX, IDC_EDIT_HME2_ID2, m_nHmeTwo2);
	DDX_Text(pDX, IDC_EDIT_HME2_ID3, m_nHmeTwo3);
	DDX_Text(pDX, IDC_EDIT_HME2_ID4, m_nHmeTwo4);
	DDX_Text(pDX, IDC_EDIT_HME2_ID5, m_nHmeTwo5);
	DDX_Text(pDX, IDC_EDIT_HME2_ID6, m_nHmeTwo6);
	DDX_Text(pDX, IDC_EDIT_HME2_ID7, m_nHmeTwo7);
	DDX_Text(pDX, IDC_EDIT_HME2_ID8, m_nHmeTwo8);
	DDX_Text(pDX, IDC_EDIT_HME2_ID9, m_nHmeTwo9);
	DDX_Text(pDX, IDC_EDIT_HME2_ID10, m_nHmeTwo10);
	DDX_Text(pDX, IDC_EDIT_HME2_ID11, m_nHmeTwo11);
	DDX_Text(pDX, IDC_EDIT_HME2_ID12, m_nHmeTwo12);
	DDX_Text(pDX, IDC_EDIT_HME2_ID13, m_nHmeTwo13);
	DDX_Text(pDX, IDC_EDIT_HME2_ID14, m_nHmeTwo14);
	DDX_Text(pDX, IDC_EDIT_HME2_ID15, m_nHmeTwo15);
	DDX_Text(pDX, IDC_EDIT_HME2_ID16, m_nHmeTwo16);
	DDX_Text(pDX, IDC_EDIT_HME2_ID17, m_nHmeTwo17);
	DDX_Text(pDX, IDC_EDIT_HME2_ID18, m_nHmeTwo18);
	DDX_Text(pDX, IDC_EDIT_HME2_ID19, m_nHmeTwo19);
	DDX_Text(pDX, IDC_EDIT_HME2_ID20, m_nHmeTwo20);
	DDX_Text(pDX, IDC_EDIT_HME2_ID21, m_nHmeTwo21);
	DDX_Text(pDX, IDC_EDIT_HME2_ID22, m_nHmeTwo22);
	DDX_Text(pDX, IDC_EDIT_HME2_ID23, m_nHmeTwo23);
	DDX_Text(pDX, IDC_EDIT_HME2_ID24, m_nHmeTwo24);
	DDX_Text(pDX, IDC_EDIT_HME2_ID25, m_nHmeTwo25);
	DDX_Text(pDX, IDC_EDIT_HME2_ID26, m_nHmeTwo26);
	DDX_Text(pDX, IDC_EDIT_HME2_ID27, m_nHmeTwo27);
	DDX_Text(pDX, IDC_EDIT_HME2_ID28, m_nHmeTwo28);
	DDX_Text(pDX, IDC_EDIT_HME2_ID29, m_nHmeTwo29);
}


BEGIN_MESSAGE_MAP(CForceCompensatePage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_EN_SETFOCUS(IDC_EDIT_FORCE0, &CForceCompensatePage::OnEnSetfocusEditForce0)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE1, &CForceCompensatePage::OnEnSetfocusEditForce1)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE2, &CForceCompensatePage::OnEnSetfocusEditForce2)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE3, &CForceCompensatePage::OnEnSetfocusEditForce3)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE4, &CForceCompensatePage::OnEnSetfocusEditForce4)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE5, &CForceCompensatePage::OnEnSetfocusEditForce5)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE6, &CForceCompensatePage::OnEnSetfocusEditForce6)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE7, &CForceCompensatePage::OnEnSetfocusEditForce7)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE8, &CForceCompensatePage::OnEnSetfocusEditForce8)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE9, &CForceCompensatePage::OnEnSetfocusEditForce9)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE10, &CForceCompensatePage::OnEnSetfocusEditForce10)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE11, &CForceCompensatePage::OnEnSetfocusEditForce11)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE12, &CForceCompensatePage::OnEnSetfocusEditForce12)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE13, &CForceCompensatePage::OnEnSetfocusEditForce13)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE14, &CForceCompensatePage::OnEnSetfocusEditForce14)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE15, &CForceCompensatePage::OnEnSetfocusEditForce15)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE16, &CForceCompensatePage::OnEnSetfocusEditForce16)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE17, &CForceCompensatePage::OnEnSetfocusEditForce17)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE18, &CForceCompensatePage::OnEnSetfocusEditForce18)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE19, &CForceCompensatePage::OnEnSetfocusEditForce19)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE20, &CForceCompensatePage::OnEnSetfocusEditForce20)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE21, &CForceCompensatePage::OnEnSetfocusEditForce21)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE22, &CForceCompensatePage::OnEnSetfocusEditForce22)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE23, &CForceCompensatePage::OnEnSetfocusEditForce23)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE24, &CForceCompensatePage::OnEnSetfocusEditForce24)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE25, &CForceCompensatePage::OnEnSetfocusEditForce25)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE29, &CForceCompensatePage::OnEnSetfocusEditForce29)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE28, &CForceCompensatePage::OnEnSetfocusEditForce28)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE27, &CForceCompensatePage::OnEnSetfocusEditForce27)
	ON_EN_SETFOCUS(IDC_EDIT_FORCE26, &CForceCompensatePage::OnEnSetfocusEditForce26)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_IDO, &CForceCompensatePage::OnEnSetfocusEditHme1Ido)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID1, &CForceCompensatePage::OnEnSetfocusEditHme1Id1)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID2, &CForceCompensatePage::OnEnSetfocusEditHme1Id2)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID3, &CForceCompensatePage::OnEnSetfocusEditHme1Id3)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID4, &CForceCompensatePage::OnEnSetfocusEditHme1Id4)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID5, &CForceCompensatePage::OnEnSetfocusEditHme1Id5)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID6, &CForceCompensatePage::OnEnSetfocusEditHme1Id6)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID7, &CForceCompensatePage::OnEnSetfocusEditHme1Id7)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID8, &CForceCompensatePage::OnEnSetfocusEditHme1Id8)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID9, &CForceCompensatePage::OnEnSetfocusEditHme1Id9)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID10, &CForceCompensatePage::OnEnSetfocusEditHme1Id10)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID11, &CForceCompensatePage::OnEnSetfocusEditHme1Id11)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID12, &CForceCompensatePage::OnEnSetfocusEditHme1Id12)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID13, &CForceCompensatePage::OnEnSetfocusEditHme1Id13)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID14, &CForceCompensatePage::OnEnSetfocusEditHme1Id14)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID15, &CForceCompensatePage::OnEnSetfocusEditHme1Id15)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID16, &CForceCompensatePage::OnEnSetfocusEditHme1Id16)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID17, &CForceCompensatePage::OnEnSetfocusEditHme1Id17)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID18, &CForceCompensatePage::OnEnSetfocusEditHme1Id18)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID19, &CForceCompensatePage::OnEnSetfocusEditHme1Id19)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID20, &CForceCompensatePage::OnEnSetfocusEditHme1Id20)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID21, &CForceCompensatePage::OnEnSetfocusEditHme1Id21)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID22, &CForceCompensatePage::OnEnSetfocusEditHme1Id22)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID23, &CForceCompensatePage::OnEnSetfocusEditHme1Id23)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID24, &CForceCompensatePage::OnEnSetfocusEditHme1Id24)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID25, &CForceCompensatePage::OnEnSetfocusEditHme1Id25)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID26, &CForceCompensatePage::OnEnSetfocusEditHme1Id26)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID27, &CForceCompensatePage::OnEnSetfocusEditHme1Id27)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID28, &CForceCompensatePage::OnEnSetfocusEditHme1Id28)
	ON_EN_SETFOCUS(IDC_EDIT_HME1_ID29, &CForceCompensatePage::OnEnSetfocusEditHme1Id29)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_IDO, &CForceCompensatePage::OnEnSetfocusEditHme2Ido)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID1, &CForceCompensatePage::OnEnSetfocusEditHme2Id1)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID2, &CForceCompensatePage::OnEnSetfocusEditHme2Id2)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID3, &CForceCompensatePage::OnEnSetfocusEditHme2Id3)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID4, &CForceCompensatePage::OnEnSetfocusEditHme2Id4)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID5, &CForceCompensatePage::OnEnSetfocusEditHme2Id5)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID6, &CForceCompensatePage::OnEnSetfocusEditHme2Id6)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID7, &CForceCompensatePage::OnEnSetfocusEditHme2Id7)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID8, &CForceCompensatePage::OnEnSetfocusEditHme2Id8)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID9, &CForceCompensatePage::OnEnSetfocusEditHme2Id9)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID10, &CForceCompensatePage::OnEnSetfocusEditHme2Id10)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID11, &CForceCompensatePage::OnEnSetfocusEditHme2Id11)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID12, &CForceCompensatePage::OnEnSetfocusEditHme2Id12)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID13, &CForceCompensatePage::OnEnSetfocusEditHme2Id13)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID14, &CForceCompensatePage::OnEnSetfocusEditHme2Id14)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID15, &CForceCompensatePage::OnEnSetfocusEditHme2Id15)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID16, &CForceCompensatePage::OnEnSetfocusEditHme2Id16)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID17, &CForceCompensatePage::OnEnSetfocusEditHme2Id17)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID18, &CForceCompensatePage::OnEnSetfocusEditHme2Id18)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID19, &CForceCompensatePage::OnEnSetfocusEditHme2Id19)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID20, &CForceCompensatePage::OnEnSetfocusEditHme2Id20)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID21, &CForceCompensatePage::OnEnSetfocusEditHme2Id21)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID22, &CForceCompensatePage::OnEnSetfocusEditHme2Id22)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID23, &CForceCompensatePage::OnEnSetfocusEditHme2Id23)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID24, &CForceCompensatePage::OnEnSetfocusEditHme2Id24)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID25, &CForceCompensatePage::OnEnSetfocusEditHme2Id25)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID26, &CForceCompensatePage::OnEnSetfocusEditHme2Id26)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID27, &CForceCompensatePage::OnEnSetfocusEditHme2Id27)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID28, &CForceCompensatePage::OnEnSetfocusEditHme2Id28)
	ON_EN_SETFOCUS(IDC_EDIT_HME2_ID29, &CForceCompensatePage::OnEnSetfocusEditHme2Id29)
END_MESSAGE_MAP()


// CForceCompensatePage 消息处理程序

HBRUSH CForceCompensatePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CForceCompensatePage::OnDestroy()
{
	SaveParameter();

	CPropertyPage::OnDestroy();
}

void CForceCompensatePage::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_STATIC_ID)->SetFocus();

	CPropertyPage::OnMouseMove(nFlags, point);
}

BOOL CForceCompensatePage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	PostMessage(WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, NULL);

	return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

void CForceCompensatePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO stScrollInfo;

	if (GetScrollInfo(SB_VERT, &stScrollInfo) == FALSE) return;

	short nOffset = 0;

	int nMin = stScrollInfo.nMin;
	int nMax = stScrollInfo.nMax;
	int iCurPos = GetScrollPos(SB_VERT);

	switch(nSBCode)
	{
	case SB_LINEDOWN:
	{
		if (iCurPos == nMax) break;
		nOffset = PARA_LABEL_HEIGHT + PARA_LINE_SPACE;
		if (iCurPos + nOffset > nMax)
			nOffset = nMax - iCurPos;
		break;
	}
	case SB_LINEUP:
	{
		if (iCurPos == nMin) break;
		nOffset = -(PARA_LABEL_HEIGHT + PARA_LINE_SPACE);
		if (iCurPos + nOffset < nMin)
			nOffset = nMin - iCurPos;
		break;
	}
	case SB_PAGEDOWN:
	{
		if (iCurPos == nMax) break;
		nOffset = m_nPageSize - iCurPos;
		break;
	}
	case SB_PAGEUP:
	{
		if (iCurPos == nMin) break;
		nOffset = -iCurPos;
		break;
	}
	case SB_THUMBTRACK:		
		nOffset = nPos - iCurPos;
		break;
	default:
		break;
	}

	SetScrollPos(SB_VERT, iCurPos + nOffset);
	ScrollWindow(0, -nOffset);

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CForceCompensatePage::OnSetActive()
{
	UpdateMenuVariable();
	return CPropertyPage::OnSetActive();
}

BOOL CForceCompensatePage::OnInitDialog()
{
	theApp.SetDlgFont(this);
	UpdateMenuVariable();
	return CPropertyPage::OnInitDialog();
}

void CForceCompensatePage::SaveParameter(void)
{
	UpdateData();

	FORCE_SWITCH_PARA* pstForceSwitch = theForceSwitch.GetRcd();
	pstForceSwitch->iForceTime[0][0]  = m_nforce0;                //v1.6.P136sc   edit
	pstForceSwitch->iForceTime[1][0]  = m_nforce1;
	pstForceSwitch->iForceTime[2][0]  = m_nforce2;
	pstForceSwitch->iForceTime[3][0]  = m_nforce3;
	pstForceSwitch->iForceTime[4][0]  = m_nforce4;
	pstForceSwitch->iForceTime[5][0]  = m_nforce5;
	pstForceSwitch->iForceTime[6][0]  = m_nforce6;
	pstForceSwitch->iForceTime[7][0]  = m_nforce7;
	pstForceSwitch->iForceTime[8][0]  = m_nforce8;
	pstForceSwitch->iForceTime[9][0]  = m_nforce9;
	pstForceSwitch->iForceTime[10][0] = m_nforce10;
	pstForceSwitch->iForceTime[11][0] = m_nforce11;
	pstForceSwitch->iForceTime[12][0] = m_nforce12;
	pstForceSwitch->iForceTime[13][0] = m_nforce13;
	pstForceSwitch->iForceTime[14][0] = m_nforce14;
	pstForceSwitch->iForceTime[15][0] = m_nforce15;
	pstForceSwitch->iForceTime[16][0] = m_nforce16;
	pstForceSwitch->iForceTime[17][0] = m_nforce17;
	pstForceSwitch->iForceTime[18][0] = m_nforce18;
	pstForceSwitch->iForceTime[19][0] = m_nforce19;
	pstForceSwitch->iForceTime[20][0] = m_nforce20;
	pstForceSwitch->iForceTime[21][0] = m_nforce21;
	pstForceSwitch->iForceTime[22][0] = m_nforce22;
	pstForceSwitch->iForceTime[23][0] = m_nforce23;
	pstForceSwitch->iForceTime[24][0] = m_nforce24;
	pstForceSwitch->iForceTime[25][0] = m_nforce25;
	pstForceSwitch->iForceTime[26][0] = m_nforce26;
	pstForceSwitch->iForceTime[27][0] = m_nforce27;
	pstForceSwitch->iForceTime[28][0] = m_nforce28;
	pstForceSwitch->iForceTime[29][0] = m_nforce29;  
	pstForceSwitch->iForceTime[0][1]  = m_nHmeOne0;
	pstForceSwitch->iForceTime[1][1]  = m_nHmeOne1;
	pstForceSwitch->iForceTime[2][1]  = m_nHmeOne2;
	pstForceSwitch->iForceTime[3][1]  = m_nHmeOne3;
	pstForceSwitch->iForceTime[4][1]  = m_nHmeOne4;
	pstForceSwitch->iForceTime[5][1]  = m_nHmeOne5;
	pstForceSwitch->iForceTime[6][1]  = m_nHmeOne6;
	pstForceSwitch->iForceTime[7][1]  = m_nHmeOne7;
	pstForceSwitch->iForceTime[8][1]  = m_nHmeOne8;
	pstForceSwitch->iForceTime[9][1]  = m_nHmeOne9;
	pstForceSwitch->iForceTime[10][1] = m_nHmeOne10;
	pstForceSwitch->iForceTime[11][1] = m_nHmeOne11;
	pstForceSwitch->iForceTime[12][1] = m_nHmeOne12;
	pstForceSwitch->iForceTime[13][1] = m_nHmeOne13;
	pstForceSwitch->iForceTime[14][1] = m_nHmeOne14;
	pstForceSwitch->iForceTime[15][1] = m_nHmeOne15; 
	pstForceSwitch->iForceTime[16][1] = m_nHmeOne16; 
	pstForceSwitch->iForceTime[17][1] = m_nHmeOne17; 
	pstForceSwitch->iForceTime[18][1] = m_nHmeOne18;
	pstForceSwitch->iForceTime[19][1] = m_nHmeOne19;
	pstForceSwitch->iForceTime[20][1] = m_nHmeOne20;
	pstForceSwitch->iForceTime[21][1] = m_nHmeOne21; 
	pstForceSwitch->iForceTime[22][1] = m_nHmeOne22; 
	pstForceSwitch->iForceTime[23][1] = m_nHmeOne23; 
	pstForceSwitch->iForceTime[24][1] = m_nHmeOne24; 
	pstForceSwitch->iForceTime[25][1] = m_nHmeOne25; 
	pstForceSwitch->iForceTime[26][1] = m_nHmeOne26; 
	pstForceSwitch->iForceTime[27][1] = m_nHmeOne27; 
	pstForceSwitch->iForceTime[28][1] = m_nHmeOne28; 
	pstForceSwitch->iForceTime[29][1] = m_nHmeOne29; 
	pstForceSwitch->iForceTime[0][2]  = m_nHmeTwo0;
	pstForceSwitch->iForceTime[1][2]  = m_nHmeTwo1;
	pstForceSwitch->iForceTime[2][2]  = m_nHmeTwo2;
	pstForceSwitch->iForceTime[3][2]  = m_nHmeTwo3;
	pstForceSwitch->iForceTime[4][2]  = m_nHmeTwo4;
	pstForceSwitch->iForceTime[5][2]  = m_nHmeTwo5;
	pstForceSwitch->iForceTime[6][2]  = m_nHmeTwo6;
	pstForceSwitch->iForceTime[7][2]  = m_nHmeTwo7;
	pstForceSwitch->iForceTime[8][2]  = m_nHmeTwo8;
	pstForceSwitch->iForceTime[9][2]  = m_nHmeTwo9;
	pstForceSwitch->iForceTime[10][2] = m_nHmeTwo10;
	pstForceSwitch->iForceTime[11][2] = m_nHmeTwo11;
	pstForceSwitch->iForceTime[12][2] = m_nHmeTwo12;
	pstForceSwitch->iForceTime[13][2] = m_nHmeTwo13;
	pstForceSwitch->iForceTime[14][2] = m_nHmeTwo14;
	pstForceSwitch->iForceTime[15][2] = m_nHmeTwo15;
	pstForceSwitch->iForceTime[16][2] = m_nHmeTwo16;
	pstForceSwitch->iForceTime[17][2] = m_nHmeTwo17;
	pstForceSwitch->iForceTime[18][2] = m_nHmeTwo18;
	pstForceSwitch->iForceTime[19][2] = m_nHmeTwo19;
	pstForceSwitch->iForceTime[20][2] = m_nHmeTwo20;
	pstForceSwitch->iForceTime[21][2] = m_nHmeTwo21;
	pstForceSwitch->iForceTime[22][2] = m_nHmeTwo22;
	pstForceSwitch->iForceTime[23][2] = m_nHmeTwo23;
	pstForceSwitch->iForceTime[24][2] = m_nHmeTwo24;
	pstForceSwitch->iForceTime[25][2] = m_nHmeTwo25;
	pstForceSwitch->iForceTime[26][2] = m_nHmeTwo26;
	pstForceSwitch->iForceTime[27][2] = m_nHmeTwo27;
	pstForceSwitch->iForceTime[28][2] = m_nHmeTwo28;
	pstForceSwitch->iForceTime[29][2] = m_nHmeTwo29;

	(GetParent()->GetParent())->Invalidate();
}

void CForceCompensatePage::UpdateMenuVariable(void)
{
	FORCE_SWITCH_PARA* pstForceSwitch = theForceSwitch.GetRcd();
	m_nforce0  = pstForceSwitch->iForceTime[0][0];     //v1.6.P136sc   edit
	m_nforce1  = pstForceSwitch->iForceTime[1][0];
	m_nforce2  = pstForceSwitch->iForceTime[2][0];
	m_nforce3  = pstForceSwitch->iForceTime[3][0];
	m_nforce4  = pstForceSwitch->iForceTime[4][0];
	m_nforce5  = pstForceSwitch->iForceTime[5][0];
	m_nforce6  = pstForceSwitch->iForceTime[6][0];
	m_nforce7  = pstForceSwitch->iForceTime[7][0];
	m_nforce8  = pstForceSwitch->iForceTime[8][0];
	m_nforce9  = pstForceSwitch->iForceTime[9][0];
	m_nforce10 = pstForceSwitch->iForceTime[10][0];
	m_nforce11 = pstForceSwitch->iForceTime[11][0];
	m_nforce12 = pstForceSwitch->iForceTime[12][0];
	m_nforce13 = pstForceSwitch->iForceTime[13][0];
	m_nforce14 = pstForceSwitch->iForceTime[14][0];
	m_nforce15 = pstForceSwitch->iForceTime[15][0];
	m_nforce16 = pstForceSwitch->iForceTime[16][0];
	m_nforce17 = pstForceSwitch->iForceTime[17][0];
	m_nforce18 = pstForceSwitch->iForceTime[18][0];
	m_nforce19 = pstForceSwitch->iForceTime[19][0];
	m_nforce20 = pstForceSwitch->iForceTime[20][0];
	m_nforce21 = pstForceSwitch->iForceTime[21][0];
	m_nforce22 = pstForceSwitch->iForceTime[22][0];
	m_nforce23 = pstForceSwitch->iForceTime[23][0];
	m_nforce24 = pstForceSwitch->iForceTime[24][0];
	m_nforce25 = pstForceSwitch->iForceTime[25][0];
	m_nforce26 = pstForceSwitch->iForceTime[26][0];
	m_nforce27 = pstForceSwitch->iForceTime[27][0];
	m_nforce28 = pstForceSwitch->iForceTime[28][0];
	m_nforce29 = pstForceSwitch->iForceTime[29][0];

	m_nHmeOne0  = pstForceSwitch->iForceTime[0][1];
	m_nHmeOne1  = pstForceSwitch->iForceTime[1][1];
	m_nHmeOne2  = pstForceSwitch->iForceTime[2][1];
	m_nHmeOne3  = pstForceSwitch->iForceTime[3][1];
	m_nHmeOne4  = pstForceSwitch->iForceTime[4][1];
	m_nHmeOne5  = pstForceSwitch->iForceTime[5][1];
	m_nHmeOne6  = pstForceSwitch->iForceTime[6][1];
	m_nHmeOne7  = pstForceSwitch->iForceTime[7][1];
	m_nHmeOne8  = pstForceSwitch->iForceTime[8][1];
	m_nHmeOne9  = pstForceSwitch->iForceTime[9][1];
	m_nHmeOne10 = pstForceSwitch->iForceTime[10][1];
	m_nHmeOne11 = pstForceSwitch->iForceTime[11][1];
	m_nHmeOne12 = pstForceSwitch->iForceTime[12][1];
	m_nHmeOne13 = pstForceSwitch->iForceTime[13][1];
	m_nHmeOne14 = pstForceSwitch->iForceTime[14][1];
	m_nHmeOne15 = pstForceSwitch->iForceTime[15][1];
	m_nHmeOne16 = pstForceSwitch->iForceTime[16][1];
	m_nHmeOne17 = pstForceSwitch->iForceTime[17][1];
	m_nHmeOne18 = pstForceSwitch->iForceTime[18][1];
	m_nHmeOne19 = pstForceSwitch->iForceTime[19][1];
	m_nHmeOne20 = pstForceSwitch->iForceTime[20][1];
	m_nHmeOne21 = pstForceSwitch->iForceTime[21][1];
	m_nHmeOne22 = pstForceSwitch->iForceTime[22][1];
	m_nHmeOne23 = pstForceSwitch->iForceTime[23][1];
	m_nHmeOne24 = pstForceSwitch->iForceTime[24][1];
	m_nHmeOne25 = pstForceSwitch->iForceTime[25][1];
	m_nHmeOne26 = pstForceSwitch->iForceTime[26][1];
	m_nHmeOne27 = pstForceSwitch->iForceTime[27][1];
	m_nHmeOne28 = pstForceSwitch->iForceTime[28][1];
	m_nHmeOne29 = pstForceSwitch->iForceTime[29][1];

	m_nHmeTwo0  = pstForceSwitch->iForceTime[0][2];
	m_nHmeTwo1  = pstForceSwitch->iForceTime[1][2];
	m_nHmeTwo2  = pstForceSwitch->iForceTime[2][2];
	m_nHmeTwo3  = pstForceSwitch->iForceTime[3][2];
	m_nHmeTwo4  = pstForceSwitch->iForceTime[4][2];
	m_nHmeTwo5  = pstForceSwitch->iForceTime[5][2];
	m_nHmeTwo6  = pstForceSwitch->iForceTime[6][2];
	m_nHmeTwo7  = pstForceSwitch->iForceTime[7][2];
	m_nHmeTwo8  = pstForceSwitch->iForceTime[8][2];
	m_nHmeTwo9  = pstForceSwitch->iForceTime[9][2];
	m_nHmeTwo10 = pstForceSwitch->iForceTime[10][2];
	m_nHmeTwo11 = pstForceSwitch->iForceTime[11][2];
	m_nHmeTwo12 = pstForceSwitch->iForceTime[12][2];
	m_nHmeTwo13 = pstForceSwitch->iForceTime[13][2];
	m_nHmeTwo14 = pstForceSwitch->iForceTime[14][2];
    m_nHmeTwo15 = pstForceSwitch->iForceTime[15][2];
	m_nHmeTwo16 = pstForceSwitch->iForceTime[16][2];
	m_nHmeTwo17 = pstForceSwitch->iForceTime[17][2];
	m_nHmeTwo18 = pstForceSwitch->iForceTime[18][2];
	m_nHmeTwo19 = pstForceSwitch->iForceTime[19][2];
	m_nHmeTwo20 = pstForceSwitch->iForceTime[20][2];
	m_nHmeTwo21 = pstForceSwitch->iForceTime[21][2];
	m_nHmeTwo22 = pstForceSwitch->iForceTime[22][2];
	m_nHmeTwo23 = pstForceSwitch->iForceTime[23][2];
	m_nHmeTwo24 = pstForceSwitch->iForceTime[24][2];
	m_nHmeTwo25 = pstForceSwitch->iForceTime[25][2];
	m_nHmeTwo26 = pstForceSwitch->iForceTime[26][2];
	m_nHmeTwo27 = pstForceSwitch->iForceTime[27][2];
	m_nHmeTwo28 = pstForceSwitch->iForceTime[28][2];
	m_nHmeTwo29 = pstForceSwitch->iForceTime[29][2];

	int iRowTop = PARA_COLUMN1_TOP;

	//1
	GetDlgItem(IDC_STATIC_ID)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_FORCE)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_HME1)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_HME2)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//2
	GetDlgItem(IDC_STATIC_ID0)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE0)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_IDO)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_IDO)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//3
	GetDlgItem(IDC_STATIC_ID1)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE1)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID1)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID1)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//4
	GetDlgItem(IDC_STATIC_ID2)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE2)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID2)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID2)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//5
	GetDlgItem(IDC_STATIC_ID3)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE3)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID3)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID3)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//6
	GetDlgItem(IDC_STATIC_ID4)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE4)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID4)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID4)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//7
	GetDlgItem(IDC_STATIC_ID5)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE5)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID5)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID5)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//8
	GetDlgItem(IDC_STATIC_ID6)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE6)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID6)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID6)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//9
	GetDlgItem(IDC_STATIC_ID7)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE7)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID7)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID7)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//10
	GetDlgItem(IDC_STATIC_ID8)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE8)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID8)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID8)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//11
	GetDlgItem(IDC_STATIC_ID9)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE9)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID9)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID9)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//12
	GetDlgItem(IDC_STATIC_ID10)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE10)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID10)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID10)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//13
	GetDlgItem(IDC_STATIC_ID11)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE11)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID11)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID11)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//14
	GetDlgItem(IDC_STATIC_ID12)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE12)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID12)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID12)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//15
	GetDlgItem(IDC_STATIC_ID13)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE13)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID13)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID13)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//16
	GetDlgItem(IDC_STATIC_ID14)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE14)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID14)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID14)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//17
	GetDlgItem(IDC_STATIC_ID15)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE15)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID15)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID15)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//18
	GetDlgItem(IDC_STATIC_ID16)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE16)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID16)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID16)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//19
	GetDlgItem(IDC_STATIC_ID17)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE17)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID17)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID17)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//20
	GetDlgItem(IDC_STATIC_ID18)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE18)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID18)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID18)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//21
	GetDlgItem(IDC_STATIC_ID19)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE19)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID19)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID19)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//22
	GetDlgItem(IDC_STATIC_ID20)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE20)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID20)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID20)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//23
	GetDlgItem(IDC_STATIC_ID21)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE21)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID21)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID21)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//24
	GetDlgItem(IDC_STATIC_ID22)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE22)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID22)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID22)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//25
	GetDlgItem(IDC_STATIC_ID23)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE23)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID23)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID23)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//26
	GetDlgItem(IDC_STATIC_ID24)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE24)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID24)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID24)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//27
	GetDlgItem(IDC_STATIC_ID25)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE25)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID25)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID25)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//28
	GetDlgItem(IDC_STATIC_ID26)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE26)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID26)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID26)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//29
	GetDlgItem(IDC_STATIC_ID27)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE27)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID27)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID27)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//30
	GetDlgItem(IDC_STATIC_ID28)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE28)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID28)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID28)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	iRowTop += PARA_LABEL_HEIGHT + PARA_LINE_SPACE;

	//31
	GetDlgItem(IDC_STATIC_ID29)->SetWindowPos(NULL, COM_LEFT_ONE, iRowTop, COM_SINGLE_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_FORCE29)->SetWindowPos(NULL, COM_LEFT_TWO, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME1_ID29)->SetWindowPos(NULL, COM_LEFT_THREE, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
	GetDlgItem(IDC_EDIT_HME2_ID29)->SetWindowPos(NULL, COM_LEFT_FOUR, iRowTop, COM_COLUMN2_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);

	if (iRowTop > LOOP_PARA_PAGE_HEIGHT)
		m_nPageSize = iRowTop - LOOP_PARA_PAGE_HEIGHT - 120;
	else
		m_nPageSize=0;

	SetScrollPos(SB_VERT, 0);
	SetScrollRange(SB_VERT, 0, m_nPageSize);

    UpdateData(FALSE);
}

void CForceCompensatePage::OnEnSetfocusEditForce0()
{
	GetDlgItem(IDC_STATIC_ID0)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce0, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce1()
{
	GetDlgItem(IDC_STATIC_ID1)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce1, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce2()
{
	GetDlgItem(IDC_STATIC_ID2)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce2, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce3()
{
	GetDlgItem(IDC_STATIC_ID3)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce3, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce4()
{
	GetDlgItem(IDC_STATIC_ID4)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce4, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce5()
{
	GetDlgItem(IDC_STATIC_ID5)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce5, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce6()
{
	GetDlgItem(IDC_STATIC_ID6)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce6, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce7()
{
	GetDlgItem(IDC_STATIC_ID7)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce7, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce8()
{
	GetDlgItem(IDC_STATIC_ID8)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce8, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce9()
{
	GetDlgItem(IDC_STATIC_ID9)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce9, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce10()
{
	GetDlgItem(IDC_STATIC_ID10)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce10, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce11()
{
	GetDlgItem(IDC_STATIC_ID11)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce11, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce12()
{
	GetDlgItem(IDC_STATIC_ID12)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce12, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce13()
{
	GetDlgItem(IDC_STATIC_ID13)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce13, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce14()
{
	GetDlgItem(IDC_STATIC_ID14)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce14, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce15()
{
	GetDlgItem(IDC_STATIC_ID15)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce15,0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce16()
{
	GetDlgItem(IDC_STATIC_ID16)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce16,0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce17()
{
	GetDlgItem(IDC_STATIC_ID17)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce17, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce18()
{
	GetDlgItem(IDC_STATIC_ID18)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce18, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce19()
{
	GetDlgItem(IDC_STATIC_ID19)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce19, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce20()
{
	GetDlgItem(IDC_STATIC_ID20)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce20, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce21()
{
	GetDlgItem(IDC_STATIC_ID21)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce21, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce22()
{
	GetDlgItem(IDC_STATIC_ID22)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce22, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce23()
{
	GetDlgItem(IDC_STATIC_ID23)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce23,0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce24()
{
	GetDlgItem(IDC_STATIC_ID24)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce24,0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce25()
{
	GetDlgItem(IDC_STATIC_ID25)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce25,0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce29()
{
	GetDlgItem(IDC_STATIC_ID29)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce29,0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce28()
{
	GetDlgItem(IDC_STATIC_ID28)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce28, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce27()
{
	GetDlgItem(IDC_STATIC_ID27)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce27,0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditForce26()
{
	GetDlgItem(IDC_STATIC_ID26)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nforce26, 0, -200, 200);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Ido()
{
	GetDlgItem(IDC_STATIC_ID0)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne0, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id1()
{
	GetDlgItem(IDC_STATIC_ID1)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne1, 0, 0,100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id2()
{
	GetDlgItem(IDC_STATIC_ID2)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne2, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id3()
{
	GetDlgItem(IDC_STATIC_ID3)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne3, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id4()
{
	GetDlgItem(IDC_STATIC_ID4)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne4, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id5()
{
	GetDlgItem(IDC_STATIC_ID5)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne5, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id6()
{
	GetDlgItem(IDC_STATIC_ID6)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne6, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id7()
{
	GetDlgItem(IDC_STATIC_ID7)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne7, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id8()
{
	GetDlgItem(IDC_STATIC_ID8)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne8, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id9()
{
	GetDlgItem(IDC_STATIC_ID9)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne9, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id10()
{
	GetDlgItem(IDC_STATIC_ID10)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne10, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id11()
{
	GetDlgItem(IDC_STATIC_ID11)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne11, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id12()
{
	GetDlgItem(IDC_STATIC_ID12)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne12, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id13()
{
	GetDlgItem(IDC_STATIC_ID13)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne13, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id14()
{
	GetDlgItem(IDC_STATIC_ID14)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne14, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id15()
{
	GetDlgItem(IDC_STATIC_ID15)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne15, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id16()
{
	GetDlgItem(IDC_STATIC_ID16)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne16, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id17()
{
	GetDlgItem(IDC_STATIC_ID17)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne17, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id18()
{
	GetDlgItem(IDC_STATIC_ID18)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne18, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id19()
{
	GetDlgItem(IDC_STATIC_ID19)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne19, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id20()
{
	GetDlgItem(IDC_STATIC_ID20)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne20, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id21()
{
	GetDlgItem(IDC_STATIC_ID21)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne21, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id22()
{
	GetDlgItem(IDC_STATIC_ID22)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne22, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id23()
{
	GetDlgItem(IDC_STATIC_ID23)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne23, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id24()
{
	GetDlgItem(IDC_STATIC_ID24)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne24, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id25()
{
	GetDlgItem(IDC_STATIC_ID25)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne25, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id26()
{
	GetDlgItem(IDC_STATIC_ID26)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne26, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id27()
{
	GetDlgItem(IDC_STATIC_ID27)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne27, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id28()
{
	GetDlgItem(IDC_STATIC_ID28)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne28, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme1Id29()
{
	GetDlgItem(IDC_STATIC_ID29)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeOne29, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Ido()
{
	GetDlgItem(IDC_STATIC_ID0)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo0, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id1()
{
	GetDlgItem(IDC_STATIC_ID1)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo1, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id2()
{
	GetDlgItem(IDC_STATIC_ID2)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo2, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id3()
{
	GetDlgItem(IDC_STATIC_ID3)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo3, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id4()
{
	GetDlgItem(IDC_STATIC_ID4)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo4, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id5()
{
	GetDlgItem(IDC_STATIC_ID5)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo5, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id6()
{
	GetDlgItem(IDC_STATIC_ID6)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo6, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id7()
{
	GetDlgItem(IDC_STATIC_ID7)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo7, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id8()
{
	GetDlgItem(IDC_STATIC_ID8)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo8, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id9()
{
	GetDlgItem(IDC_STATIC_ID9)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo9, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id10()
{
	GetDlgItem(IDC_STATIC_ID10)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo10, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id11()
{
	GetDlgItem(IDC_STATIC_ID11)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo11, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id12()
{
	GetDlgItem(IDC_STATIC_ID12)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo12, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id13()
{
	GetDlgItem(IDC_STATIC_ID13)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo13, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id14()
{
	GetDlgItem(IDC_STATIC_ID14)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo14, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id15()
{
	GetDlgItem(IDC_STATIC_ID15)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo15, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id16()
{
	GetDlgItem(IDC_STATIC_ID16)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo16, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id17()
{
	GetDlgItem(IDC_STATIC_ID17)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo17, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id18()
{
	GetDlgItem(IDC_STATIC_ID18)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo18, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id19()
{
	GetDlgItem(IDC_STATIC_ID19)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo19, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id20()
{
	GetDlgItem(IDC_STATIC_ID20)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo20, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id21()
{
	GetDlgItem(IDC_STATIC_ID21)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo21, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id22()
{
	GetDlgItem(IDC_STATIC_ID22)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo22, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id23()
{
	GetDlgItem(IDC_STATIC_ID23)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo23, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id24()
{
	GetDlgItem(IDC_STATIC_ID24)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo24, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id25()
{
	GetDlgItem(IDC_STATIC_ID25)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo25, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id26()
{
	GetDlgItem(IDC_STATIC_ID26)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo26, 0, 0, 100);
	UpdateData(false);
	SaveParameter();	
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id27()
{
	GetDlgItem(IDC_STATIC_ID27)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo27, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id28()
{
	GetDlgItem(IDC_STATIC_ID28)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo28, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}

void CForceCompensatePage::OnEnSetfocusEditHme2Id29()
{
	GetDlgItem(IDC_STATIC_ID29)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nHmeTwo29, 0, 0, 100);
	UpdateData(false);
	SaveParameter();
}
