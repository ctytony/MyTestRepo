/***************************************************************
AdvParaMapDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MathCalc.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Wire.h"
#include "BondPara.h"
#include "BallPara.h"
#include "LoopPara.h"
#include "AdvParaMapDlg.h"
#include "AutoPadMgr.h"
#include "servo.h"
#include "MachineConfig.h"

// CAdvParaMapDlg dialog

IMPLEMENT_DYNAMIC(CAdvParaMapDlg, CDialog)

CAdvParaMapDlg::CAdvParaMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvParaMapDlg::IDD, pParent)
    , m_nTotalWire(0)
    , m_nParaMapType(0)
    , m_iParaApplyMode(0)
{

}

CAdvParaMapDlg::~CAdvParaMapDlg()
{
}

void CAdvParaMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    short i;
    for (i = 1; i <= m_nTotalWire; i++)
        DDX_Text(pDX, IDC_MAP_PARA1 + i, m_anMapPara[i]);

    for (i = 0; i < m_nTotalRow; i++)
        DDX_Text(pDX, IDC_MAP_WIRE_NUM1 + i, m_anRowValue[i]);
}


BEGIN_MESSAGE_MAP(CAdvParaMapDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_VSCROLL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CAdvParaMapDlg message handlers
BOOL CAdvParaMapDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add
    GetDlgItem(IDOK)->SetWindowPos(NULL, 1000, 1000, PARA_COLUMN1_WIDTH, PARA_LABEL_HEIGHT, SWP_NOZORDER);
    m_nTotalWire = thePgm.GetUnitTotalWire();
    
    short i, nTotalPadType;
    CPoint pt;
    CRect Rect;

	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();
    BOND_PARA_LIMIT *pstParaLimit = theBondPara.GetBondParaLimit();
	ST_BUMP_WIRE stBsobWire;
    theBallPara.GetBsobWire(&stBsobWire);
	ST_BUMP_BALL stBsobBall;
	theBallPara.GetBsobBall(&stBsobBall);
	ST_BUMP_SHAPE stBsobShape;
	theBallPara.GetBsobShape(&stBsobShape);

	//v3.0.T434 add
	ST_BUMP_BALL stBbosBall;
	theBallPara.GetBbosBall(&stBbosBall);

	//v3.0.T433 add
	ST_BUMP_SHAPE stBbosShape;
	theBallPara.GetBbosShape(&stBbosShape);

    m_aGroupBrush[0].CreateSolidBrush(BOND_GROUP_COLOR_00);
    m_aGroupBrush[1].CreateSolidBrush(BOND_GROUP_COLOR_01);
    m_aGroupBrush[2].CreateSolidBrush(BOND_GROUP_COLOR_02);
    m_aGroupBrush[3].CreateSolidBrush(BOND_GROUP_COLOR_03);
    m_aGroupBrush[4].CreateSolidBrush(BOND_GROUP_COLOR_04);
    m_aGroupBrush[5].CreateSolidBrush(BOND_GROUP_COLOR_05);
    m_aGroupBrush[6].CreateSolidBrush(BOND_GROUP_COLOR_06);
    m_aGroupBrush[7].CreateSolidBrush(BOND_GROUP_COLOR_07);
    m_aGroupBrush[8].CreateSolidBrush(BOND_GROUP_COLOR_08);
    m_aGroupBrush[9].CreateSolidBrush(BOND_GROUP_COLOR_09);
    m_aGroupBrush[10].CreateSolidBrush(BOND_GROUP_COLOR_10);
    m_aGroupBrush[11].CreateSolidBrush(BOND_GROUP_COLOR_11);
    m_aGroupBrush[12].CreateSolidBrush(BOND_GROUP_COLOR_12);
    m_aGroupBrush[13].CreateSolidBrush(BOND_GROUP_COLOR_13);
    m_aGroupBrush[14].CreateSolidBrush(BOND_GROUP_COLOR_14);

    switch (m_nParaMapType)
    {
    case PARA_TYPE_LOOP_GROUP:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = TOTAL_LOOP_MODE - 1;
        break;
    case PARA_TYPE_NSOP:
        m_lRefPara    = 1;
        m_lLowerLimit = 0;
        m_lUpperLimit = 1;
        break;
    case PARA_TYPE_BUMP_BALL:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = 3;	//v3.1.T91 edit
        break;
	case PARA_TYPE_1ST_BASE_TIME:
		m_lRefPara    = pstBondPara->nBaseTime[0];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 100;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nBaseTimeL[0];
		m_lUpperLimit = pstParaLimit->nBaseTimeH[0];
		break;
	case PARA_TYPE_1ST_BASE_FORCE:
		m_lRefPara    = pstBondPara->nBaseForce[0];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nBaseForceL[0];
		m_lUpperLimit = pstParaLimit->nBaseForceH[0];
		break;
	case PARA_TYPE_2ND_BASE_TIME:
		m_lRefPara    = pstBondPara->nBaseTime[1];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 100;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nBaseTimeL[1];
		m_lUpperLimit = pstParaLimit->nBaseTimeH[1];
		break;
	case PARA_TYPE_2ND_BASE_FORCE:
		m_lRefPara    = pstBondPara->nBaseForce[1];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nBaseForceL[1];
		m_lUpperLimit = pstParaLimit->nBaseForceH[1];
		break;
    case PARA_TYPE_PRE_SHIFT_2:
        m_lRefPara    = pstBondPara->nPreShift2;
        m_lLowerLimit = -30;
        m_lUpperLimit = 30;
        break;
	case PARA_TYPE_1ST_IMPACT_FORCE:
		m_lRefPara    = pstBondPara->nCtactForce[0];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nCtactForceL[0];
		m_lUpperLimit = pstParaLimit->nCtactForceH[0];
		break;
	case PARA_TYPE_2ND_IMPACT_FORCE: 
		m_lRefPara    = pstBondPara->nCtactForce[1];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nCtactForceL[1];
		m_lUpperLimit = pstParaLimit->nCtactForceH[1];
		break;
	case PARA_TYPE_1ST_IMPACT_TIME: 
		m_lRefPara    = pstBondPara->nCtactTime[0];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 100;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nCtactTimeL[0];
		m_lUpperLimit = pstParaLimit->nCtactTimeH[0];
		break;
	case PARA_TYPE_2ND_IMPACT_TIME: 
		m_lRefPara    = pstBondPara->nCtactTime[1];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 100;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nCtactTimeL[1];
		m_lUpperLimit = pstParaLimit->nCtactTimeH[1];
		break;
	case PARA_TYPE_1ST_BASE_POWER:
		m_lRefPara    = round(pstBondPara->fBasePower[0] * 2.55);
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nBasePowerL[0];
		m_lUpperLimit = pstParaLimit->nBasePowerH[0];
		break;
	case PARA_TYPE_2ND_BASE_POWER:
		m_lRefPara    = round(pstBondPara->fBasePower[1] * 2.55);
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nBasePowerL[1];
		m_lUpperLimit = pstParaLimit->nBasePowerH[1];
		break;
	case PARA_TYPE_1ST_IMPACT_POWER:
		m_lRefPara    = round(pstBondPara->fCtactPower[0] * 2.55);
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nCtactPowerL[0];
		m_lUpperLimit = pstParaLimit->nCtactPowerH[0];
		break;
	case PARA_TYPE_2ND_IMPACT_POWER:
		m_lRefPara    = round(pstBondPara->fCtactPower[1] * 2.55);
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nCtactPowerL[1];
		m_lUpperLimit = pstParaLimit->nCtactPowerH[1];
		break;
	case PARA_TYPE_1ST_PRE_POWER:
		m_lRefPara    = round(pstBondPara->fStandbyPower[0] * 2.55);
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nStandbyPowerL[0];
		m_lUpperLimit = pstParaLimit->nStandbyPowerH[0];
		break;
	case PARA_TYPE_1ST_PRE_FORCE:
		m_lRefPara    = pstBondPara->nPreCtactForce[0];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nPreCtactForceL[0];
		m_lUpperLimit = pstParaLimit->nPreCtactForceH[0];
		break;
	case PARA_TYPE_1ST_PRE_TIME:
		m_lRefPara    = pstBondPara->nPreCtactTime[0];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 100;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nPreCtactTimeL[0];
		m_lUpperLimit = pstParaLimit->nPreCtactTimeH[0];
		break;
	case PARA_TYPE_2ND_PRE_POWER:
		m_lRefPara    = round(pstBondPara->fStandbyPower[1] * 2.55);
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nStandbyPowerL[1];
		m_lUpperLimit = pstParaLimit->nStandbyPowerH[1];
		break;
	case PARA_TYPE_2ND_PRE_FORCE:
		m_lRefPara    = pstBondPara->nPreCtactForce[1];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 255;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nPreCtactForceL[1];
		m_lUpperLimit = pstParaLimit->nPreCtactForceH[1];
		break;
	case PARA_TYPE_2ND_PRE_TIME:
		m_lRefPara    = pstBondPara->nPreCtactTime[1];
		//v1.1.288.T21 add, v3.0.T1 delete
		//m_lLowerLimit = 0;
		//m_lUpperLimit = 100;
		//v1.1.288.T21 delete, v3.0.T1 add
		m_lLowerLimit = pstParaLimit->nPreCtactTimeL[1];
		m_lUpperLimit = pstParaLimit->nPreCtactTimeH[1];
		break;
	//v3.1.T401 delete
    //case PARA_TYPE_BUMP_WIRE_BOND_POWER:
    //    m_lRefPara    = round(stBsobWire.fBasePower * 2.55);
    //    m_lLowerLimit = 0;
    //    m_lUpperLimit = 100;
    //    break;
	case PARA_TYPE_BSOB_BUMP_1ST_BOND_POWER:    
		m_lRefPara    = round(stBsobBall.fBasePower[0] * 2.55);
		m_lLowerLimit = 0;
		m_lUpperLimit = 255;  
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_BOND_POWER:	//v3.0.T434 add
		m_lRefPara    = round(stBbosBall.fBasePower[0] * 2.55);
		m_lLowerLimit = 0;
		m_lUpperLimit = 255;  
		break;
	case PARA_TYPE_BSOB_BUMP_2ND_BOND_POWER:    
		m_lRefPara    = round(stBsobBall.fBasePower[1] * 2.55);
		m_lLowerLimit = 0;
		m_lUpperLimit = 255; 
		break;
	case PARA_TYPE_BBOS_BUMP_2ND_BOND_POWER:	//v3.0.T434 add
		m_lRefPara    = round(stBbosBall.fBasePower[1] * 2.55);
		m_lLowerLimit = 0;
		m_lUpperLimit = 255; 
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_BOND_TIME:
		m_lRefPara = stBsobBall.nBaseTime[0];
		m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_BOND_TIME:	//v3.0.T434 add
		m_lRefPara = stBbosBall.nBaseTime[0];
		m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_2ND_BOND_TIME:
		m_lRefPara = stBsobBall.nBaseTime[1];
	    m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_2ND_BOND_TIME:	//v3.0.T434 add
		m_lRefPara = stBbosBall.nBaseTime[1];
	    m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_BOND_FORCE: 
        m_lRefPara    = stBsobBall.nBaseForce[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_BOND_FORCE:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nBaseForce[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_2ND_BOND_FORCE:  
        m_lRefPara    = stBsobBall.nBaseForce[1];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_2ND_BOND_FORCE:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nBaseForce[1];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_POWER:  
        m_lRefPara    = round(stBsobBall.fStandbyPower[0] * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_POWER:	//v3.0.T434 add
        m_lRefPara    = round(stBbosBall.fStandbyPower[0] * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_FORCE:  
        m_lRefPara    = stBsobBall.nPreCtactForce[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_FORCE:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nPreCtactForce[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_TIME:  
        m_lRefPara    = stBsobBall.nPreCtactTime[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_TIME:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nPreCtactTime[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_CTACT_FORCE:  
        m_lRefPara    = stBsobBall.nCtactForce[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_CTACT_FORCE:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nCtactForce[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_CTACT_TIME:  
        m_lRefPara    = stBsobBall.nCtactTime[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_CTACT_TIME:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nCtactTime[0];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_1ST_CTACT_POWER:  
        m_lRefPara    = round(stBsobBall.fCtactPower[0] * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_1ST_CTACT_POWER:	//v3.0.T434 add
        m_lRefPara    = round(stBbosBall.fCtactPower[0] * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_2ND_CTACT_FORCE:  
        m_lRefPara    = stBsobBall.nCtactForce[1];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_2ND_CTACT_FORCE:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nCtactForce[1];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_2ND_CTACT_TIME:  
        m_lRefPara    = stBsobBall.nCtactTime[1];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_2ND_CTACT_TIME:	//v3.0.T434 add
        m_lRefPara    = stBbosBall.nCtactTime[1];
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_BUMP_2ND_CTACT_POWER:  
        m_lRefPara    = round(stBsobBall.fCtactPower[1] * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_BUMP_2ND_CTACT_POWER:	//v3.0.T434 add
        m_lRefPara    = round(stBbosBall.fCtactPower[1] * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_FAB_TYPE:
	case PARA_TYPE_BBOS_FAB_TYPE:	//v3.1.T381 add
		m_lRefPara = 0;
		m_lLowerLimit = 0;
		if (theMcConfig.GetFabType() == 1)	//v3.1.T34 add
			m_lUpperLimit = 3;
		else
			m_lUpperLimit = 1;
        break;
	case PARA_TYPE_WIRE_FAB_TYPE:
		m_lRefPara = 0;
		m_lLowerLimit = 0;
		if (theMcConfig.GetFabType() == 1)	//v3.1.T34 add
			m_lUpperLimit = 3;
		else
			m_lUpperLimit = 1;
        break;
    case PARA_TYPE_WIRE_LENGTH_CTRL:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = 1;
        break;
    case PARA_TYPE_WIRE_LENGTH:
        m_lRefPara    = 500;
        m_lLowerLimit = 100;
        m_lUpperLimit = 5000;
        break;
    case PARA_TYPE_1ST_POINT_PAD_ID:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        nTotalPadType = theAutoPadMgr.GetTotalAutoPad();
        m_lUpperLimit = nTotalPadType;
        break;
    case PARA_TYPE_1ST_POINT_X_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -200;
        m_lUpperLimit =  200;
        break;
    case PARA_TYPE_1ST_POINT_Y_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -200;
        m_lUpperLimit =  200;
        break;
    case PARA_TYPE_WIRE_LENGTH_CORRECT:
        m_lRefPara    = 0;
        m_lLowerLimit = -500;
        m_lUpperLimit = 500;
        break;
    case PARA_TYPE_FAST_LOOP:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = 1;
        break;
    case PARA_TYPE_2ND_POINT_PAD_ID:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        nTotalPadType = theAutoPadMgr.GetTotalAutoPad();
        m_lUpperLimit = nTotalPadType;
        break;
    case PARA_TYPE_2ND_POINT_X_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -200;
        m_lUpperLimit =  200;
        break;
    case PARA_TYPE_2ND_POINT_Y_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -200;
        m_lUpperLimit =  200;
        break;
    case PARA_TYPE_1ST_BOND_SCRUB:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = MAX_SCRUB_GROUP;
        break;
    case PARA_TYPE_2ND_BOND_SCRUB:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = MAX_SCRUB_GROUP;
        break;
    case PARA_TYPE_BUMP_BALL_SCRUB:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = MAX_SCRUB_GROUP;
        break;
    case PARA_TYPE_1ST_POINT_ILL:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = 1;
        break;
    case PARA_TYPE_1ST_POINT_LEAD_CAPI_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = 1;
        break;
    case PARA_TYPE_1ST_POINT_LEAD_X_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -500;
        m_lUpperLimit = 500;
        break;
    case PARA_TYPE_1ST_POINT_LEAD_Y_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -500;
        m_lUpperLimit = 500;
        break;
    case PARA_TYPE_2ND_POINT_ILL:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = 1;
        break;
    case PARA_TYPE_2ND_POINT_LEAD_CAPI_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = 0;
        m_lUpperLimit = 1;
        break;
    case PARA_TYPE_2ND_POINT_LEAD_X_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -500;
        m_lUpperLimit = 500;
        break;
    case PARA_TYPE_2ND_POINT_LEAD_Y_OFFSET:
        m_lRefPara    = 0;
        m_lLowerLimit = -500;
        m_lUpperLimit = 500;
        break;
	case PARA_TYPE_BSOB_SHAPE_BASE_HT:
		m_lRefPara = stBsobShape.nBaseHeight;
		m_lLowerLimit = 10;
		m_lUpperLimit = 400;
		break;
	case PARA_TYPE_BBOS_SHAPE_BASE_HT:	//v3.0.T433 add
		m_lRefPara = stBbosShape.nBaseHeight;
		m_lLowerLimit = 10;
		m_lUpperLimit = 400;
		break;
	case PARA_TYPE_BSOB_SHAPE_SPAN_LEN:
		m_lRefPara = stBsobShape.nSpanDist;
		m_lLowerLimit = -600;
		m_lUpperLimit = 600;
		break;
	case PARA_TYPE_BBOS_SHAPE_SPAN_LEN:		//v3.0.T433 add
		m_lRefPara = stBbosShape.nSpanDist;
		m_lLowerLimit = -600;
		m_lUpperLimit = 600;
		break;
	case PARA_TYPE_BSOB_SHAPE_SCRUB_DIST:
		m_lRefPara = stBsobShape.nScrubDist;
		m_lLowerLimit = -100;
		m_lUpperLimit = 100;
		break;
	case PARA_TYPE_BBOS_SHAPE_SCRUB_DIST:	//v3.0.T433 add
		m_lRefPara = stBbosShape.nScrubDist;
		m_lLowerLimit = -100;
		m_lUpperLimit = 100;
		break;
	case PARA_TYPE_BSOB_SHAPE_BALL_THICKNESS:
		m_lRefPara = stBsobShape.nBallThickness;
		m_lLowerLimit = 10;
		m_lUpperLimit = 300;
		break;
	case PARA_TYPE_BBOS_SHAPE_BALL_THICKNESS:	//v3.0.T433 add
		m_lRefPara = stBbosShape.nBallThickness;
		m_lLowerLimit = 10;
		m_lUpperLimit = 300;
		break;
	case PARA_TYPE_BSOB_SHAPE_CUT_POWER:
		m_lRefPara = stBsobShape.nCuttingPower;
		m_lLowerLimit = 0;
		m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BBOS_SHAPE_CUT_POWER:	//v3.0.T433 add
		m_lRefPara = stBbosShape.nCuttingPower;
		m_lLowerLimit = 0;
		m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_SHAPE_CUT_POWER_DELAY:
		m_lRefPara = stBsobShape.nCuttingPowerDelay;
		m_lLowerLimit = 0;
		m_lUpperLimit = 100;
		break;
	case PARA_TYPE_BBOS_SHAPE_CUT_POWER_DELAY:	//v3.0.T433 add
		m_lRefPara = stBbosShape.nCuttingPowerDelay;
		m_lLowerLimit = 0;
		m_lUpperLimit = 100;
		break;
	case PARA_TYPE_BSOB_SHAPE_MOTION_SPEED:
		m_lRefPara = stBsobShape.nMotionSpeed;
		m_lLowerLimit = 10;
		m_lUpperLimit = 100;
		break;
	case PARA_TYPE_BBOS_SHAPE_MOTION_SPEED:		//v3.0.T433 add
		m_lRefPara = stBbosShape.nMotionSpeed;
		m_lLowerLimit = 10;
		m_lUpperLimit = 100;
		break;
	case PARA_TYPE_BSOB_SHAPE_DIRECTION_ANGLE:
		m_lRefPara = stBsobShape.nDirectionAngle;
		m_lLowerLimit = -135;
		m_lUpperLimit = 135;
		break;
	case PARA_TYPE_BSOB_SHAPE_BUMP_SHAPE_MODE:	//v3.1.T362 add
	case PARA_TYPE_BBOS_SHAPE_BUMP_SHAPE_MODE:	//v3.1.T362 add
		m_lRefPara = 0;
		m_lLowerLimit = 0;
		m_lUpperLimit = 2;
		break;
	case PARA_TYPE_BBOS_SHAPE_DIRECTION_ANGLE:	//v3.0.T433 add
		m_lRefPara = stBbosShape.nDirectionAngle;
		m_lLowerLimit = -135;
		m_lUpperLimit = 135;
		break;
	//v3.0.T2 add
	case PARA_TYPE_1ST_SRCH_HT:
		m_lRefPara = pstBondPara->nSrchHeight1;
		m_lLowerLimit = pstParaLimit->nSrchHeightL[0];
		m_lUpperLimit = pstParaLimit->nSrchHeightH[0];
		break;
	//v3.0.T2 add
	case PARA_TYPE_2ND_SRCH_HT:
		m_lRefPara = pstBondPara->nSrchHeight2;
		m_lLowerLimit = pstParaLimit->nSrchHeightL[1];
		m_lUpperLimit = pstParaLimit->nSrchHeightH[1];
		break;
	case PARA_TYPE_BSOB_WIRE_PRE_CTACT_TIME:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nPreCtactTime;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_PRE_CTACT_FORCE:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nPreCtactForce;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_PRE_CTACT_POWER:	//v3.1.T401 add  
        m_lRefPara    = round(stBsobWire.fStandbyPower * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_CTACT_TIME:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nCtactTime;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_CTACT_FORCE:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nCtactForce;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_CTACT_POWER:	//v3.1.T401 add  
        m_lRefPara    = round(stBsobWire.fCtactPower * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_BOND_TIME:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nBaseTime;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_BOND_FORCE:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nBaseForce;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_BOND_POWER:	//v3.1.T401 add  
        m_lRefPara    = round(stBsobWire.fBasePower * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_RLS_TIME:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nReleaseTime;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_RLS_FORCE:	//v3.1.T401 add  
        m_lRefPara    = stBsobWire.nReleaseForce;
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
	case PARA_TYPE_BSOB_WIRE_RLS_POWER:	//v3.1.T401 add  
        m_lRefPara    = round(stBsobWire.fReleasePower * 2.55);
        m_lLowerLimit = 0;
        m_lUpperLimit = 255;
		break;
    default:
        break;
    }

    const WIRE_RECORD *pstWire;
    for (i = 1; i <= m_nTotalWire; i++)
    {
        pstWire = theWire.GetRcd(i);
        switch (m_nParaMapType)
        {
        case PARA_TYPE_LOOP_GROUP:
            m_anMapPara[i] = pstWire->nLoopMode;
            break;
        case PARA_TYPE_NSOP:
            m_anMapPara[i] = pstWire->bCheckNsop;
            break;
        case PARA_TYPE_BUMP_BALL:
            m_anMapPara[i] = pstWire->nBumpType;
            break;
        case PARA_TYPE_1ST_BASE_TIME:
            m_anMapPara[i] = pstBondPara->nBaseTime[0] + pstWire->nTimeOffset[0];
            break;
        case PARA_TYPE_1ST_BASE_FORCE:
            m_anMapPara[i] = pstBondPara->nBaseForce[0] + pstWire->nForceOffset[0];
            break;
        case PARA_TYPE_2ND_BASE_TIME:
            m_anMapPara[i] = pstBondPara->nBaseTime[1] + pstWire->nTimeOffset[1];
            break;
        case PARA_TYPE_2ND_BASE_FORCE:
		{
			short nForceCompensate;
			float fPowerCompensate;
			theWire.EqualizeForcePower(i, pstBondPara->nForceFactor, pstBondPara->fPowerFactor, &nForceCompensate, &fPowerCompensate);
			m_anMapPara[i] = pstBondPara->nBaseForce[1] + pstWire->nForceOffset[1] + nForceCompensate;
			break;
		}
        case PARA_TYPE_PRE_SHIFT_2:
			m_anMapPara[i] = pstBondPara->nPreShift2 + pstWire->nPreShiftOffset2;
			break;
		case PARA_TYPE_1ST_IMPACT_FORCE: 
			m_anMapPara[i] = pstBondPara->nCtactForce[0] + pstWire->n1stCtactForceOffset;
			break;
        case PARA_TYPE_2ND_IMPACT_FORCE: 
            m_anMapPara[i] = pstBondPara->nCtactForce[1] + pstWire->n2ndCtactForceOffset;
            break;
		case PARA_TYPE_1ST_IMPACT_TIME: 
			m_anMapPara[i] = pstBondPara->nCtactTime[0] + pstWire->nCtactTimeOffset[0];
			break;
		case PARA_TYPE_2ND_IMPACT_TIME: 
			m_anMapPara[i] = pstBondPara->nCtactTime[1] + pstWire->nCtactTimeOffset[1];
			break;
        case PARA_TYPE_1ST_BASE_POWER:
            m_anMapPara[i] = round((pstBondPara->fBasePower[0] + pstWire->fPowerOffset[0]) * 2.55);
            break;
        case PARA_TYPE_2ND_BASE_POWER:
		{
			short nForceCompensate;
			float fPowerCompensate;
			theWire.EqualizeForcePower(i, pstBondPara->nForceFactor, pstBondPara->fPowerFactor, &nForceCompensate, &fPowerCompensate);
			m_anMapPara[i] = round((pstBondPara->fBasePower[1] + pstWire->fPowerOffset[1] + fPowerCompensate) * 2.55);
			break;
		}
		case PARA_TYPE_1ST_IMPACT_POWER:
			m_anMapPara[i] = round((pstBondPara->fCtactPower[0] + pstWire->fCtactPowerOffset[0]) * 2.55);
			break;
		case PARA_TYPE_2ND_IMPACT_POWER:
			m_anMapPara[i] = round((pstBondPara->fCtactPower[1] + pstWire->fCtactPowerOffset[1]) * 2.55);
			break;
		case PARA_TYPE_1ST_PRE_POWER:
			m_anMapPara[i] = round((pstBondPara->fStandbyPower[0] + pstWire->fPrePowerOffset[0]) * 2.55);
			break;
		case PARA_TYPE_1ST_PRE_FORCE:
			m_anMapPara[i] = pstBondPara->nPreCtactForce[0] + pstWire->nPreForceOffset[0];
			break;
		case PARA_TYPE_1ST_PRE_TIME:
			m_anMapPara[i] = pstBondPara->nPreCtactTime[0] + pstWire->nPreTimeOffset[0];
			break;
		case PARA_TYPE_2ND_PRE_POWER:
			m_anMapPara[i] = round((pstBondPara->fStandbyPower[1] + pstWire->fPrePowerOffset[1]) * 2.55);
			break;
		case PARA_TYPE_2ND_PRE_FORCE:
			m_anMapPara[i] = pstBondPara->nPreCtactForce[1] + pstWire->nPreForceOffset[1];
			break;
		case PARA_TYPE_2ND_PRE_TIME:
			m_anMapPara[i] = pstBondPara->nPreCtactTime[1] + pstWire->nPreTimeOffset[1];
			break;
		//v3.1.T401 delete
		//case PARA_TYPE_BUMP_WIRE_BOND_POWER:
		//	m_anMapPara[i] = round((stBsobWire.fBasePower + pstWire->fBumpWirePowerOffset) * 2.55);
		//	break;
		case PARA_TYPE_BSOB_BUMP_1ST_BOND_POWER:    
			m_anMapPara[i] = round((stBsobBall.fBasePower[0] + pstWire->fBumpPowerOffset[0]) * 2.55);
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_BOND_POWER:	//v3.0.T434 add
			m_anMapPara[i] = round((stBbosBall.fBasePower[0] + pstWire->fBbosBumpPowerOffset[0]) * 2.55);
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_BOND_POWER:
			m_anMapPara[i] = round((stBsobBall.fBasePower[1] + pstWire->fBumpPowerOffset[1]) * 2.55);
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_BOND_POWER:	//v3.0.T434 add
			m_anMapPara[i] = round((stBbosBall.fBasePower[1] + pstWire->fBbosBumpPowerOffset[1]) * 2.55);
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_BOND_TIME:
			m_anMapPara[i] = stBsobBall.nBaseTime[0] + pstWire->nBumpTimeOffset[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_BOND_TIME:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nBaseTime[0] + pstWire->nBbosBumpTimeOffset[0];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_BOND_TIME:
			m_anMapPara[i] = stBsobBall.nBaseTime[1] + pstWire->nBumpTimeOffset[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_BOND_TIME:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nBaseTime[1] + pstWire->nBbosBumpTimeOffset[1];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_BOND_FORCE:   
			m_anMapPara[i] = stBsobBall.nBaseForce[0] + pstWire->nBumpForceOffset[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_BOND_FORCE:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nBaseForce[0] + pstWire->nBbosBumpForceOffset[0];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_BOND_FORCE:    
			m_anMapPara[i] = stBsobBall.nBaseForce[1] + pstWire->nBumpForceOffset[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_BOND_FORCE:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nBaseForce[1] + pstWire->nBbosBumpForceOffset[1];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_POWER:
			m_anMapPara[i] = round((stBsobBall.fStandbyPower[0] + pstWire->fBump1stPreCtactPowerOffset) * 2.55);
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_POWER:	//v3.0.T434 add
			m_anMapPara[i] = round((stBbosBall.fStandbyPower[0] + pstWire->fBbosBump1stPreCtactPowerOffset) * 2.55);
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_FORCE:
			m_anMapPara[i] = stBsobBall.nPreCtactForce[0] + pstWire->nBump1stPreCtactForceOffset;
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_FORCE:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nPreCtactForce[0] + pstWire->nBbosBump1stPreCtactForceOffset;
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_TIME:  
			m_anMapPara[i] = stBsobBall.nPreCtactTime[0] + pstWire->nBump1stPreCtactTimeOffset;
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_TIME:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nPreCtactTime[0] + pstWire->nBbosBump1stPreCtactTimeOffset;
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_CTACT_POWER:
			m_anMapPara[i] = round((stBsobBall.fCtactPower[0] + pstWire->fBumpCtactPowerOffset[0]) * 2.55);
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_CTACT_POWER:	//v3.0.T434 add
			m_anMapPara[i] = round((stBbosBall.fCtactPower[0] + pstWire->fBbosBumpCtactPowerOffset[0]) * 2.55);
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_CTACT_FORCE:
			m_anMapPara[i] = stBsobBall.nCtactForce[0] + pstWire->nBumpCtactForceOffset[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_CTACT_FORCE:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nCtactForce[0] + pstWire->nBbosBumpCtactForceOffset[0];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_CTACT_TIME:  
			m_anMapPara[i] = stBsobBall.nCtactTime[0] + pstWire->nBumpCtactTimeOffset[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_CTACT_TIME:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nCtactTime[0] + pstWire->nBbosBumpCtactTimeOffset[0];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_CTACT_POWER:
			m_anMapPara[i] = round((stBsobBall.fCtactPower[1] + pstWire->fBumpCtactPowerOffset[1]) * 2.55);
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_CTACT_POWER:	//v3.0.T434 add
			m_anMapPara[i] = round((stBbosBall.fCtactPower[1] + pstWire->fBbosBumpCtactPowerOffset[1]) * 2.55);
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_CTACT_FORCE:
			m_anMapPara[i] = stBsobBall.nCtactForce[1] + pstWire->nBumpCtactForceOffset[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_CTACT_FORCE:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nCtactForce[1] + pstWire->nBbosBumpCtactForceOffset[1];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_CTACT_TIME:  
			m_anMapPara[i] = stBsobBall.nCtactTime[1] + pstWire->nBumpCtactTimeOffset[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_CTACT_TIME:	//v3.0.T434 add
			m_anMapPara[i] = stBbosBall.nCtactTime[1] + pstWire->nBbosBumpCtactTimeOffset[1];
			break;
		case PARA_TYPE_BSOB_FAB_TYPE:
			m_anMapPara[i] = pstWire->cBsobFabType;
			break;
		case PARA_TYPE_BBOS_FAB_TYPE:	//v3.1.T381 add
			m_anMapPara[i] = pstWire->cBbosFabType;
			break;
		case PARA_TYPE_WIRE_FAB_TYPE:
			m_anMapPara[i] = pstWire->cWireFabType;
			break;
        case PARA_TYPE_WIRE_LENGTH_CTRL:
            m_anMapPara[i] = pstWire->bConstWireLength;
            break;
        case PARA_TYPE_WIRE_LENGTH:
			if (pstWire->bConstWireLength == true)
				m_anMapPara[i] = pstWire->nWireLength;
			else
			{
				double dWireLength = Calc2PtDist(pstWire->stTchPos[0], pstWire->stTchPos[1]);	//v2.0.T5 add
				m_anMapPara[i] = round(dWireLength * theServo.GetResolXY());
			}
			break;
        case PARA_TYPE_1ST_POINT_PAD_ID:
            m_anMapPara[i] = pstWire->chPadType[0];
            break;
        case PARA_TYPE_1ST_POINT_X_OFFSET:
            m_anMapPara[i] = pstWire->n1stBondPtPadOffsetX;
            break;
        case PARA_TYPE_1ST_POINT_Y_OFFSET:
            m_anMapPara[i] = pstWire->n1stBondPtPadOffsetY;
            break;
        case PARA_TYPE_WIRE_LENGTH_CORRECT:
            m_anMapPara[i] = pstWire->nWireLengthCorrect;
            break;
        case PARA_TYPE_FAST_LOOP:
            m_anMapPara[i] = pstWire->bFastLoop;
            break;
        case PARA_TYPE_2ND_POINT_PAD_ID:
            m_anMapPara[i] = pstWire->chPadType[1];
            break;
        case PARA_TYPE_2ND_POINT_X_OFFSET:
            m_anMapPara[i] = pstWire->n2ndBondPtPadOffsetX;
            break;
        case PARA_TYPE_2ND_POINT_Y_OFFSET:
            m_anMapPara[i] = pstWire->n2ndBondPtPadOffsetY;
            break;
        case PARA_TYPE_1ST_BOND_SCRUB:
            m_anMapPara[i] = pstWire->ch1stBondScrubMode;
            break;
        case PARA_TYPE_2ND_BOND_SCRUB:
            m_anMapPara[i] = pstWire->ch2ndBondScrubMode;
            break;
        case PARA_TYPE_BUMP_BALL_SCRUB:
            m_anMapPara[i] = pstWire->chBumpBondScrubMode;
            break;
        case PARA_TYPE_1ST_POINT_ILL:
            m_anMapPara[i] = pstWire->b1stPtILL;
            break;
        case PARA_TYPE_1ST_POINT_LEAD_CAPI_OFFSET:
            m_anMapPara[i] = pstWire->b1stPtLeadCapiOffset;
            break;
        case PARA_TYPE_1ST_POINT_LEAD_X_OFFSET:
            m_anMapPara[i] = pstWire->n1stPtLeadOffsetX;
            break;
        case PARA_TYPE_1ST_POINT_LEAD_Y_OFFSET:
            m_anMapPara[i] = pstWire->n1stPtLeadOffsetY;
            break;
        case PARA_TYPE_2ND_POINT_ILL:
            m_anMapPara[i] = pstWire->b2ndPtILL;
            break;
        case PARA_TYPE_2ND_POINT_LEAD_CAPI_OFFSET:
            m_anMapPara[i] = pstWire->b2ndPtLeadCapiOffset;
            break;
        case PARA_TYPE_2ND_POINT_LEAD_X_OFFSET:
            m_anMapPara[i] = pstWire->n2ndPtLeadOffsetX;
            break;
        case PARA_TYPE_2ND_POINT_LEAD_Y_OFFSET:
            m_anMapPara[i] = pstWire->n2ndPtLeadOffsetY;
            break;
		case PARA_TYPE_BSOB_SHAPE_BASE_HT:
			m_anMapPara[i] = stBsobShape.nBaseHeight + pstWire->nBsobShapeBaseHeightOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_BASE_HT:	//v3.0.T433 add
			m_anMapPara[i] = stBbosShape.nBaseHeight + pstWire->nBbosShapeBaseHeightOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_SPAN_LEN:
			m_anMapPara[i] = stBsobShape.nSpanDist + pstWire->nBsobShapeSpanDistOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_SPAN_LEN:	//v3.0.T433 add
			m_anMapPara[i] = stBbosShape.nSpanDist + pstWire->nBbosShapeSpanDistOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_SCRUB_DIST:
			m_anMapPara[i] = stBsobShape.nScrubDist + pstWire->nBsobShapeScrubDistOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_SCRUB_DIST:	//v3.0.T433 add
			m_anMapPara[i] = stBbosShape.nScrubDist + pstWire->nBbosShapeScrubDistOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_BALL_THICKNESS:
			m_anMapPara[i] = stBsobShape.nBallThickness + pstWire->nBsobShapeBallThicknessOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_BALL_THICKNESS:	//v3.0.T433 add
			m_anMapPara[i] = stBbosShape.nBallThickness + pstWire->nBbosShapeBallThicknessOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_CUT_POWER:
			m_anMapPara[i] = stBsobShape.nCuttingPower + pstWire->nBsobShapeCuttingPowerOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_CUT_POWER:	//v3.0.T433 add
			m_anMapPara[i] = stBbosShape.nCuttingPower + pstWire->nBbosShapeCuttingPowerOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_CUT_POWER_DELAY:
			m_anMapPara[i] = stBsobShape.nCuttingPowerDelay + pstWire->nBsobShapeCuttingPowerDelayOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_CUT_POWER_DELAY:	//v3.0.T433 add
			m_anMapPara[i] = stBbosShape.nCuttingPowerDelay + pstWire->nBbosShapeCuttingPowerDelayOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_MOTION_SPEED:
		    m_anMapPara[i] = stBsobShape.nMotionSpeed + pstWire->nBsobShapeMotionSpeedOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_MOTION_SPEED:		//v3.0.T433 add
		    m_anMapPara[i] = stBbosShape.nMotionSpeed + pstWire->nBbosShapeMotionSpeedOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_DIRECTION_ANGLE:
			m_anMapPara[i] = stBsobShape.nDirectionAngle + pstWire->nBsobShapeDirectionAngleOffset;
			break;
		case PARA_TYPE_BSOB_SHAPE_BUMP_SHAPE_MODE:	//v3.1.T362 add
			m_anMapPara[i] = pstWire->cBsobShapeBumpShapeMode;
			break;
		case PARA_TYPE_BBOS_SHAPE_DIRECTION_ANGLE:	//v3.0.T433 add
			m_anMapPara[i] = stBbosShape.nDirectionAngle + pstWire->nBbosShapeDirectionAngleOffset;
			break;
		case PARA_TYPE_BBOS_SHAPE_BUMP_SHAPE_MODE:	//v3.1.T362 add
			m_anMapPara[i] = pstWire->cBbosShapeBumpShapeMode;
			break;
		//v3.0.T2 add
		case PARA_TYPE_1ST_SRCH_HT:
            m_anMapPara[i] = pstBondPara->nSrchHeight1 + pstWire->nSrchHtOffset[0];
			break;
		//v3.0.T2 add
		case PARA_TYPE_2ND_SRCH_HT:
			m_anMapPara[i] = pstBondPara->nSrchHeight2 + pstWire->nSrchHtOffset[1];
			break;
		case PARA_TYPE_BSOB_WIRE_PRE_CTACT_TIME:	//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nPreCtactTime + pstWire->nBsobWirePreCtactTimeOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_PRE_CTACT_FORCE:	//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nPreCtactForce + pstWire->nBsobWirePreCtactForceOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_PRE_CTACT_POWER:	//v3.1.T401 add
			m_anMapPara[i] = round((stBsobWire.fStandbyPower + pstWire->fBsobWirePreCtactPowerOffset) * 2.55);
			break;
		case PARA_TYPE_BSOB_WIRE_CTACT_TIME:	//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nCtactTime + pstWire->nBsobWireCtactTimeOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_CTACT_FORCE:	//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nCtactForce + pstWire->nBsobWireCtactForceOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_CTACT_POWER:	//v3.1.T401 add
			m_anMapPara[i] = round((stBsobWire.fCtactPower + pstWire->fBsobWireCtactPowerOffset) * 2.55);
			break;
		case PARA_TYPE_BSOB_WIRE_BOND_TIME:		//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nBaseTime + pstWire->nBsobWireBondTimeOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_BOND_FORCE:	//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nBaseForce + pstWire->nBsobWireBondForceOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_BOND_POWER:	//v3.1.T401 add
			m_anMapPara[i] = round((stBsobWire.fBasePower + pstWire->fBsobWireBondPowerOffset) * 2.55);
			break;
		case PARA_TYPE_BSOB_WIRE_RLS_TIME:		//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nReleaseTime + pstWire->nBsobWireRlsTimeOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_RLS_FORCE:	//v3.1.T401 add
			m_anMapPara[i] = stBsobWire.nReleaseForce + pstWire->nBsobWireRlsForceOffset;
			break;
		case PARA_TYPE_BSOB_WIRE_RLS_POWER:	//v3.1.T401 add
			m_anMapPara[i] = round((stBsobWire.fReleasePower + pstWire->fBsobWireRlsPowerOffset) * 2.55);
			break;
        default:
            break;
        }
    }

    m_nTotalRow = m_nTotalWire / 8;
    if (m_nTotalWire % 8 > 0)
        m_nTotalRow += 1;

	pt.x = MAP_PARA_LABEL_LEFT;
	pt.y = MAP_PARA_ROW_GAP; //MAP_PARA_LABEL_TOP;

    CString szString;
	GetParent()->GetWindowRect(&Rect);

    for (i = 0; i < m_nTotalRow; i++)
    {
        m_paRowLabel[i] = new CStatic();
        szString.Format(_T("%d"), i * 8 + 1);
        m_anRowValue[i] = i * 8 + 1;
        m_paRowLabel[i]->Create(szString, ES_CENTER, Rect, this, IDC_MAP_WIRE_NUM1 + i);

        if (i < m_nTotalRow)
        {
            m_paRowLabel[i]->SetWindowPos(NULL, pt.x, pt.y, MAP_PARA_LABEL_WIDTH, MAP_PARA_LABEL_HEIGHT, SWP_NOZORDER);
            m_paRowLabel[i]->SetFont(theApp.GetFont());
            m_paRowLabel[i]->ShowWindow(SW_SHOWNORMAL);

            pt.y += MAP_PARA_LABEL_HEIGHT + MAP_PARA_ROW_GAP;
        }
        else
        {
            m_paRowLabel[i]->ShowWindow(0);
        }
    }

    pt.x = MAP_PARA_COLUMN1_LEFT;
	pt.y = MAP_PARA_ROW_GAP; //MAP_PARA_COLUMN1_TOP;

    for (i = 1; i <= m_nTotalWire; i++)
    {
        m_paMapEdit[i] = new CMapEdit();

        Rect.SetRect(pt.x, pt.y, pt.x + MAP_PARA_COLUMN_WIDTH, pt.y + MAP_PARA_COLUMN_HEIGHT);

        m_paMapEdit[i]->Create(ES_CENTER, Rect, this, IDC_MAP_PARA1 + i);
        m_paMapEdit[i]->SetFont(theApp.GetFont());
        m_paMapEdit[i]->SetParamRange(m_lRefPara, m_lLowerLimit, m_lUpperLimit);
        m_paMapEdit[i]->SetUnitValue(m_anMapPara[i]);
        m_paMapEdit[i]->ShowWindow(SW_SHOWNORMAL);

        if (i % 8 == 0)
        {
            pt.x = MAP_PARA_COLUMN1_LEFT;
            pt.y += MAP_PARA_COLUMN_HEIGHT + MAP_PARA_ROW_GAP;
        }
        else
        {
            pt.x += MAP_PARA_COLUMN_WIDTH + MAP_PARA_COLUMN_GAP;
        }
    }

    if (m_nTotalWire % 8 > 0)
        pt.y += MAP_PARA_COLUMN_HEIGHT + MAP_PARA_ROW_GAP;

    if (pt.y > 496)
    {
        m_nPageSize = (short)pt.y - 496 - MAP_PARA_ROW_GAP;

        EnableScrollBarCtrl(SB_VERT, TRUE);
        SetScrollRange(SB_VERT, 0, m_nPageSize);
    }
    else
        EnableScrollBarCtrl(SB_VERT, FALSE);

    UpdateData(FALSE);

    return CDialog::OnInitDialog();
}

HBRUSH CAdvParaMapDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    if (nCtlColor == CTLCOLOR_EDIT)
    {
        if ((pWnd->GetDlgCtrlID() > IDC_MAP_PARA1) && (pWnd->GetDlgCtrlID() <= IDC_MAP_PARA1 + 5000))
        {
            short nWireNum = pWnd->GetDlgCtrlID() - IDC_MAP_PARA1;
			char cBondGroup = theWire.GetRcd(nWireNum)->chBondGroup;
            switch (cBondGroup)
            {
            case 0:
                pDC->SetBkColor(BOND_GROUP_COLOR_00);
                break;
            case 1:
                pDC->SetBkColor(BOND_GROUP_COLOR_01);
                break;
            case 2:
                pDC->SetBkColor(BOND_GROUP_COLOR_02);
                break;
            case 3:
                pDC->SetBkColor(BOND_GROUP_COLOR_03);
                break;
            case 4:
                pDC->SetBkColor(BOND_GROUP_COLOR_04);
                break;
            case 5:
                pDC->SetBkColor(BOND_GROUP_COLOR_05);
                break;
            case 6:
                pDC->SetBkColor(BOND_GROUP_COLOR_06);
                break;
            case 7:
                pDC->SetBkColor(BOND_GROUP_COLOR_07);
                break;
            case 8:
                pDC->SetBkColor(BOND_GROUP_COLOR_08);
                break;
            case 9:
                pDC->SetBkColor(BOND_GROUP_COLOR_09);
                break;
            case 10:
                pDC->SetBkColor(BOND_GROUP_COLOR_10);
                break;
            case 11:
                pDC->SetBkColor(BOND_GROUP_COLOR_11);
                break;
            case 12:
                pDC->SetBkColor(BOND_GROUP_COLOR_12);
                break;
            case 13:
                pDC->SetBkColor(BOND_GROUP_COLOR_13);
                break;
            case 14:
                pDC->SetBkColor(BOND_GROUP_COLOR_14);
                break;
            default:
                break;
            }

            return m_aGroupBrush[cBondGroup];
        }
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAdvParaMapDlg::RemoveEditFocus(void)
{
    GetDlgItem(IDOK)->SetFocus();
}

void CAdvParaMapDlg::UpdateUnitValue(int iUnitId)
{
    short nWireIdx = iUnitId - IDC_MAP_PARA1;
    m_anMapPara[nWireIdx] = m_paMapEdit[nWireIdx]->GetUnitValue();

    if (m_iParaApplyMode == PARA_APPLY_GROUP)
    {
        char cBondGroup = theWire.GetRcd(nWireIdx)->chBondGroup;

        for (int i = 1; i <= thePgm.GetUnitTotalWire(); i++)
        {
            if (theWire.GetRcd(i)->chBondGroup == cBondGroup)
            {
                m_anMapPara[i] = m_anMapPara[nWireIdx];
                m_paMapEdit[i]->SetUnitValue(m_anMapPara[nWireIdx]);
            }
        }
    }
    else if (m_iParaApplyMode == PARA_APPLY_ALL)
    {
        for (int i = 1; i <= thePgm.GetUnitTotalWire(); i++)
        {
            m_anMapPara[i] = m_anMapPara[nWireIdx];
            m_paMapEdit[i]->SetUnitValue(m_anMapPara[nWireIdx]);
        }
    }
    
    UpdateData(FALSE);
}

void CAdvParaMapDlg::OnDestroy()
{
	BOND_PARA_RCD *pstBondPara = theBondPara.GetRcd();

	ST_BUMP_WIRE stBsobWire;
	theBallPara.GetBsobWire(&stBsobWire);

	ST_BUMP_BALL stBsobBall;  
	theBallPara.GetBsobBall(&stBsobBall);   

	ST_BUMP_SHAPE stBsobShape;  
	theBallPara.GetBsobShape(&stBsobShape);   

	//v3.0.T434 add
	ST_BUMP_BALL stBbosBall;  
	theBallPara.GetBbosBall(&stBbosBall);   

	//v3.0.T433 add
	ST_BUMP_SHAPE stBbosShape;  
	theBallPara.GetBbosShape(&stBbosShape);   

	UpdateData(TRUE);

	short nUnitTotalWire = thePgm.GetUnitTotalWire();

	int i, nWireIdx;
    WIRE_RECORD *pstWire;
	for (i = 1; i <= theWire.GetTotalWireNum(); i++)
    {
		if ((theMcConfig.GetDeviceSeq() == 1) && thePgm.IsEvenCol(i))
			nWireIdx = nUnitTotalWire - ((i-1) % nUnitTotalWire);
		//v3.0.T15 add
		else if ((theMcConfig.GetDeviceSeq() == 2) && thePgm.IsEvenCol(i))
		{
			nWireIdx = nUnitTotalWire - ((i-1) % nUnitTotalWire);
			if (nWireIdx == 1) nWireIdx = 2;
			else if (nWireIdx == 2) nWireIdx = 1;
			else if (nWireIdx == 4) nWireIdx = 5;
			else if (nWireIdx == 5) nWireIdx = 4;
		}
		else
			nWireIdx = ((i-1) % nUnitTotalWire) + 1;
        pstWire = theWire.GetRcd(i);
        switch (m_nParaMapType)
        {
        case PARA_TYPE_LOOP_GROUP:
			pstWire->nLoopMode = m_anMapPara[nWireIdx];
			theLoopPara.CountProgramLoopMode();	//v3.1.T360 add
            break;
        case PARA_TYPE_NSOP:
		{
			if (m_anMapPara[nWireIdx] == 0)
				pstWire->bCheckNsop = false;
			else
				pstWire->bCheckNsop = true;
			break;
		}
        case PARA_TYPE_BUMP_BALL:
            pstWire->nBumpType = m_anMapPara[nWireIdx];
            break;
        case PARA_TYPE_1ST_BASE_TIME:
            pstWire->nTimeOffset[0] = m_anMapPara[nWireIdx] - pstBondPara->nBaseTime[0];
            break;
        case PARA_TYPE_1ST_BASE_FORCE:
            pstWire->nForceOffset[0] = m_anMapPara[nWireIdx] - pstBondPara->nBaseForce[0];
            break;
        case PARA_TYPE_2ND_BASE_TIME:
            pstWire->nTimeOffset[1] = m_anMapPara[nWireIdx] - pstBondPara->nBaseTime[1];
            break;
        case PARA_TYPE_2ND_BASE_FORCE:
		{
			short nForceCompensate;
			float fPowerCompensate;
			theWire.EqualizeForcePower(i, pstBondPara->nForceFactor, pstBondPara->fPowerFactor, &nForceCompensate, &fPowerCompensate);
			pstWire->nForceOffset[1] = m_anMapPara[nWireIdx] - pstBondPara->nBaseForce[1] - nForceCompensate;
			break;
		}
        case PARA_TYPE_PRE_SHIFT_2:
			pstWire->nPreShiftOffset2 = m_anMapPara[nWireIdx] - pstBondPara->nPreShift2;
			break;
		case PARA_TYPE_BSOB_FAB_TYPE:
			pstWire->cBsobFabType = (char)(m_anMapPara[nWireIdx]);
			break;
		case PARA_TYPE_BBOS_FAB_TYPE:	//v3.1.T381 add
			pstWire->cBbosFabType = (char)(m_anMapPara[nWireIdx]);
			break;
		case PARA_TYPE_WIRE_FAB_TYPE:
			pstWire->cWireFabType = (char)(m_anMapPara[nWireIdx]);
			break;
		case PARA_TYPE_1ST_IMPACT_FORCE: 
			pstWire->n1stCtactForceOffset = m_anMapPara[nWireIdx] - pstBondPara->nCtactForce[0];
			break;
		case PARA_TYPE_2ND_IMPACT_FORCE: 
			pstWire->n2ndCtactForceOffset = m_anMapPara[nWireIdx] - pstBondPara->nCtactForce[1];
			break;
		case PARA_TYPE_1ST_IMPACT_TIME: 
			pstWire->nCtactTimeOffset[0] = m_anMapPara[nWireIdx] - pstBondPara->nCtactTime[0];
			break;
		case PARA_TYPE_2ND_IMPACT_TIME: 
			pstWire->nCtactTimeOffset[1] = m_anMapPara[nWireIdx] - pstBondPara->nCtactTime[1];
			break;
        case PARA_TYPE_1ST_BASE_POWER:
            pstWire->fPowerOffset[0] = (float)m_anMapPara[nWireIdx] / (float)2.55 - pstBondPara->fBasePower[0];
            break;
        case PARA_TYPE_2ND_BASE_POWER:
		{
			short nForceCompensate;
			float fPowerCompensate;
			theWire.EqualizeForcePower(i, pstBondPara->nForceFactor, pstBondPara->fPowerFactor, &nForceCompensate, &fPowerCompensate);
			pstWire->fPowerOffset[1] = (float)m_anMapPara[nWireIdx] / (float)2.55 - pstBondPara->fBasePower[1] - fPowerCompensate;
			break;
		}
		case PARA_TYPE_1ST_IMPACT_POWER:
			pstWire->fCtactPowerOffset[0] = (float)m_anMapPara[nWireIdx] / (float)2.55 - pstBondPara->fCtactPower[0];
			break;
		case PARA_TYPE_2ND_IMPACT_POWER:
			pstWire->fCtactPowerOffset[1] = (float)m_anMapPara[nWireIdx] / (float)2.55 - pstBondPara->fCtactPower[1];
			break;
		case PARA_TYPE_1ST_PRE_POWER:
			pstWire->fPrePowerOffset[0] = (float)m_anMapPara[nWireIdx] / (float)2.55 - pstBondPara->fStandbyPower[0];
			break;
		case PARA_TYPE_1ST_PRE_FORCE:
			pstWire->nPreForceOffset[0] = m_anMapPara[nWireIdx] - pstBondPara->nPreCtactForce[0];
			break;
		case PARA_TYPE_1ST_PRE_TIME:
			pstWire->nPreTimeOffset[0] = m_anMapPara[nWireIdx] - pstBondPara->nPreCtactTime[0];
			break;
		case PARA_TYPE_2ND_PRE_POWER:
			pstWire->fPrePowerOffset[1] = (float)m_anMapPara[nWireIdx] / (float)2.55 - pstBondPara->fStandbyPower[1];
			break;
		case PARA_TYPE_2ND_PRE_FORCE:
			pstWire->nPreForceOffset[1] = m_anMapPara[nWireIdx] - pstBondPara->nPreCtactForce[1];
			break;
		case PARA_TYPE_2ND_PRE_TIME:
			pstWire->nPreTimeOffset[1] = m_anMapPara[nWireIdx] - pstBondPara->nPreCtactTime[1];
			break;
		//v3.1.T401 delete
		//case PARA_TYPE_BUMP_WIRE_BOND_POWER:
		//	pstWire->fBumpWirePowerOffset = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBsobWire.fBasePower;
		//	break;
		case PARA_TYPE_BSOB_BUMP_1ST_BOND_POWER:      
			pstWire->fBumpPowerOffset[0] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBsobBall.fBasePower[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_BOND_POWER:	//v3.0.T434 add
			pstWire->fBbosBumpPowerOffset[0] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBbosBall.fBasePower[0];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_BOND_POWER:     
			pstWire->fBumpPowerOffset[1] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBsobBall.fBasePower[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_BOND_POWER:	//v3.0.T434 add
			pstWire->fBbosBumpPowerOffset[1] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBbosBall.fBasePower[1];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_BOND_TIME:
			pstWire->nBumpTimeOffset[0] = (short)m_anMapPara[nWireIdx] - stBsobBall.nBaseTime[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_BOND_TIME:	//v3.0.T434 add
			pstWire->nBbosBumpTimeOffset[0] = (short)m_anMapPara[nWireIdx] - stBbosBall.nBaseTime[0];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_BOND_TIME:
			pstWire->nBumpTimeOffset[1] = (short)m_anMapPara[nWireIdx] - stBsobBall.nBaseTime[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_BOND_TIME:	//v3.0.T434 add
			pstWire->nBbosBumpTimeOffset[1] = (short)m_anMapPara[nWireIdx] - stBbosBall.nBaseTime[1];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_BOND_FORCE:     
			pstWire->nBumpForceOffset[0] = (short)m_anMapPara[nWireIdx] - stBsobBall.nBaseForce[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_BOND_FORCE:	//v3.0.T434 add
			pstWire->nBbosBumpForceOffset[0] = (short)m_anMapPara[nWireIdx] - stBbosBall.nBaseForce[0];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_BOND_FORCE:      
			pstWire->nBumpForceOffset[1] = (short)m_anMapPara[nWireIdx] - stBsobBall.nBaseForce[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_BOND_FORCE:	//v3.0.T434 add
			pstWire->nBbosBumpForceOffset[1] = (short)m_anMapPara[nWireIdx] - stBbosBall.nBaseForce[1];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_POWER:
			pstWire->fBump1stPreCtactPowerOffset = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBsobBall.fStandbyPower[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_POWER:	//v3.0.T434 add
			pstWire->fBbosBump1stPreCtactPowerOffset = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBbosBall.fStandbyPower[0];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_FORCE:
			pstWire->nBump1stPreCtactForceOffset = (short)m_anMapPara[nWireIdx] - stBsobBall.nPreCtactForce[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_FORCE:	//v3.0.T434 add
			pstWire->nBbosBump1stPreCtactForceOffset = (short)m_anMapPara[nWireIdx] - stBbosBall.nPreCtactForce[0];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_PRE_CTACT_TIME: 
			pstWire->nBump1stPreCtactTimeOffset = (short)m_anMapPara[nWireIdx] - stBsobBall.nPreCtactTime[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_PRE_CTACT_TIME:	//v3.0.T434 add
			pstWire->nBbosBump1stPreCtactTimeOffset = (short)m_anMapPara[nWireIdx] - stBbosBall.nPreCtactTime[0];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_CTACT_POWER:
			pstWire->fBumpCtactPowerOffset[0] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBsobBall.fCtactPower[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_CTACT_POWER:	//v3.0.T434 add
			pstWire->fBbosBumpCtactPowerOffset[0] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBbosBall.fCtactPower[0];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_CTACT_FORCE:
			pstWire->nBumpCtactForceOffset[0] = (short)m_anMapPara[nWireIdx] - stBsobBall.nCtactForce[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_CTACT_FORCE:	//v3.0.T434 add
			pstWire->nBbosBumpCtactForceOffset[0] = (short)m_anMapPara[nWireIdx] - stBbosBall.nCtactForce[0];
			break;
		case PARA_TYPE_BSOB_BUMP_1ST_CTACT_TIME: 
			pstWire->nBumpCtactTimeOffset[0] = (short)m_anMapPara[nWireIdx] - stBsobBall.nCtactTime[0];
			break;
		case PARA_TYPE_BBOS_BUMP_1ST_CTACT_TIME:	//v3.0.T434 add
			pstWire->nBbosBumpCtactTimeOffset[0] = (short)m_anMapPara[nWireIdx] - stBbosBall.nCtactTime[0];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_CTACT_POWER:
			pstWire->fBumpCtactPowerOffset[1] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBsobBall.fCtactPower[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_CTACT_POWER:	//v3.0.T434 add
			pstWire->fBbosBumpCtactPowerOffset[1] = (float)m_anMapPara[nWireIdx] / (float)2.55 - stBbosBall.fCtactPower[1];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_CTACT_FORCE:
			pstWire->nBumpCtactForceOffset[1] = (short)m_anMapPara[nWireIdx] - stBsobBall.nCtactForce[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_CTACT_FORCE:	//v3.0.T434 add
			pstWire->nBbosBumpCtactForceOffset[1] = (short)m_anMapPara[nWireIdx] - stBbosBall.nCtactForce[1];
			break;
		case PARA_TYPE_BSOB_BUMP_2ND_CTACT_TIME: 
			pstWire->nBumpCtactTimeOffset[1] = (short)m_anMapPara[nWireIdx] - stBsobBall.nCtactTime[1];
			break;
		case PARA_TYPE_BBOS_BUMP_2ND_CTACT_TIME:	//v3.0.T434 add
			pstWire->nBbosBumpCtactTimeOffset[1] = (short)m_anMapPara[nWireIdx] - stBbosBall.nCtactTime[1];
			break;
        case PARA_TYPE_WIRE_LENGTH_CTRL:
		{
			if (m_anMapPara[nWireIdx] == 0)
				pstWire->bConstWireLength = false;
			else
				pstWire->bConstWireLength = true;
			break;
		}
        case PARA_TYPE_WIRE_LENGTH:
		{
			if (pstWire->bConstWireLength == true)
				pstWire->nWireLength = m_anMapPara[nWireIdx];
			break;
		}
		case PARA_TYPE_1ST_POINT_PAD_ID:
			pstWire->chPadType[0] = (char)m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_1ST_POINT_X_OFFSET:
			pstWire->n1stBondPtPadOffsetX = m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_1ST_POINT_Y_OFFSET:
			pstWire->n1stBondPtPadOffsetY = m_anMapPara[nWireIdx];
			break;
        case PARA_TYPE_WIRE_LENGTH_CORRECT:
            pstWire->nWireLengthCorrect = m_anMapPara[nWireIdx];
			break;
        case PARA_TYPE_FAST_LOOP:
			pstWire->bFastLoop = (m_anMapPara[nWireIdx] == 0 ? false : true);
			break;
		case PARA_TYPE_2ND_POINT_PAD_ID:
			pstWire->chPadType[1] = (char)m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_2ND_POINT_X_OFFSET:
			pstWire->n2ndBondPtPadOffsetX = m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_2ND_POINT_Y_OFFSET:
			pstWire->n2ndBondPtPadOffsetY = m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_1ST_BOND_SCRUB:
			pstWire->ch1stBondScrubMode = (char)m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_2ND_BOND_SCRUB:
			pstWire->ch2ndBondScrubMode = (char)m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_BUMP_BALL_SCRUB:
			pstWire->chBumpBondScrubMode = (char)m_anMapPara[nWireIdx];
			break;
        case PARA_TYPE_1ST_POINT_ILL:
		{
			if (m_anMapPara[nWireIdx] == 0)
				pstWire->b1stPtILL = false;
			else
				pstWire->b1stPtILL = true;
			break;
		}
        case PARA_TYPE_1ST_POINT_LEAD_CAPI_OFFSET:
		{
			if (m_anMapPara[nWireIdx] == 0)
				pstWire->b1stPtLeadCapiOffset = false;
			else
				pstWire->b1stPtLeadCapiOffset = true;
			break;
		}
		case PARA_TYPE_1ST_POINT_LEAD_X_OFFSET:
			pstWire->n1stPtLeadOffsetX = m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_1ST_POINT_LEAD_Y_OFFSET:
			pstWire->n1stPtLeadOffsetY = m_anMapPara[nWireIdx];
			break;
        case PARA_TYPE_2ND_POINT_ILL:
		{
			if (m_anMapPara[nWireIdx] == 0)
				pstWire->b2ndPtILL = false;
			else
				pstWire->b2ndPtILL = true;
			break;
		}
        case PARA_TYPE_2ND_POINT_LEAD_CAPI_OFFSET:
		{
			if (m_anMapPara[nWireIdx] == 0)
				pstWire->b2ndPtLeadCapiOffset = false;
			else
				pstWire->b2ndPtLeadCapiOffset = true;
			break;
		}
		case PARA_TYPE_2ND_POINT_LEAD_X_OFFSET:
			pstWire->n2ndPtLeadOffsetX = m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_2ND_POINT_LEAD_Y_OFFSET:
			pstWire->n2ndPtLeadOffsetY = m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_BSOB_SHAPE_BASE_HT:
			pstWire->nBsobShapeBaseHeightOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nBaseHeight;
			break;
		case PARA_TYPE_BBOS_SHAPE_BASE_HT:	//v3.0.T433 add
			pstWire->nBbosShapeBaseHeightOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nBaseHeight;
			break;
		case PARA_TYPE_BSOB_SHAPE_SPAN_LEN:
			pstWire->nBsobShapeSpanDistOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nSpanDist;
			break;
		case PARA_TYPE_BBOS_SHAPE_SPAN_LEN:	//v3.0.T433 add
			pstWire->nBbosShapeSpanDistOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nSpanDist;
			break;
		case PARA_TYPE_BSOB_SHAPE_SCRUB_DIST:
			pstWire->nBsobShapeScrubDistOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nScrubDist;
			break;
		case PARA_TYPE_BBOS_SHAPE_SCRUB_DIST:	//v3.0.T433 add
			pstWire->nBbosShapeScrubDistOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nScrubDist;
			break;
		case PARA_TYPE_BSOB_SHAPE_BALL_THICKNESS:
			pstWire->nBsobShapeBallThicknessOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nBallThickness;
			break;
		case PARA_TYPE_BBOS_SHAPE_BALL_THICKNESS:	//v3.0.T433 add
			pstWire->nBbosShapeBallThicknessOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nBallThickness;
			break;
		case PARA_TYPE_BSOB_SHAPE_CUT_POWER:
			pstWire->nBsobShapeCuttingPowerOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nCuttingPower;
			break;
		case PARA_TYPE_BBOS_SHAPE_CUT_POWER:	//v3.0.T433 add
			pstWire->nBbosShapeCuttingPowerOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nCuttingPower;
			break;
		case PARA_TYPE_BSOB_SHAPE_CUT_POWER_DELAY:
			pstWire->nBsobShapeCuttingPowerDelayOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nCuttingPowerDelay;
			break;
		case PARA_TYPE_BBOS_SHAPE_CUT_POWER_DELAY:	//v3.0.T433 add
			pstWire->nBbosShapeCuttingPowerDelayOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nCuttingPowerDelay;
			break;
		case PARA_TYPE_BSOB_SHAPE_MOTION_SPEED:
			pstWire->nBsobShapeMotionSpeedOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nMotionSpeed;
			break;
		case PARA_TYPE_BBOS_SHAPE_MOTION_SPEED:		//v3.0.T433 add
			pstWire->nBbosShapeMotionSpeedOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nMotionSpeed;
			break;
		case PARA_TYPE_BSOB_SHAPE_DIRECTION_ANGLE:
			pstWire->nBsobShapeDirectionAngleOffset = (short)m_anMapPara[nWireIdx] - stBsobShape.nDirectionAngle;
			break;
		case PARA_TYPE_BSOB_SHAPE_BUMP_SHAPE_MODE:	//v3.1.T362 add
			pstWire->cBsobShapeBumpShapeMode = (char)m_anMapPara[nWireIdx];
			break;
		case PARA_TYPE_BBOS_SHAPE_DIRECTION_ANGLE:	//v3.0.T433 add
			pstWire->nBbosShapeDirectionAngleOffset = (short)m_anMapPara[nWireIdx] - stBbosShape.nDirectionAngle;
			break;
		case PARA_TYPE_BBOS_SHAPE_BUMP_SHAPE_MODE:	//v3.1.T362 add
			pstWire->cBbosShapeBumpShapeMode = (char)m_anMapPara[nWireIdx];
			break;
		//v3.0.T2 add
		case PARA_TYPE_1ST_SRCH_HT:
			pstWire->nSrchHtOffset[0] = (short)m_anMapPara[nWireIdx] - pstBondPara->nSrchHeight1;
			break;
		//v3.0.T2 add
		case PARA_TYPE_2ND_SRCH_HT:
			pstWire->nSrchHtOffset[1] = (short)m_anMapPara[nWireIdx] - pstBondPara->nSrchHeight2;
			break;
		case PARA_TYPE_BSOB_WIRE_PRE_CTACT_TIME:	//v3.1.T401 add
			pstWire->nBsobWirePreCtactTimeOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nPreCtactTime;
			break;
		case PARA_TYPE_BSOB_WIRE_PRE_CTACT_FORCE:	//v3.1.T401 add
			pstWire->nBsobWirePreCtactForceOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nPreCtactForce;
			break;
		case PARA_TYPE_BSOB_WIRE_PRE_CTACT_POWER:	//v3.1.T401 add
			pstWire->fBsobWirePreCtactPowerOffset = ((float)m_anMapPara[nWireIdx] / (float)2.55) - stBsobWire.fStandbyPower;
			break;
		case PARA_TYPE_BSOB_WIRE_CTACT_TIME:	//v3.1.T401 add
			pstWire->nBsobWireCtactTimeOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nCtactTime;
			break;
		case PARA_TYPE_BSOB_WIRE_CTACT_FORCE:	//v3.1.T401 add
			pstWire->nBsobWireCtactForceOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nCtactForce;
			break;
		case PARA_TYPE_BSOB_WIRE_CTACT_POWER:	//v3.1.T401 add
			pstWire->fBsobWireCtactPowerOffset = ((float)m_anMapPara[nWireIdx] / (float)2.55) - stBsobWire.fCtactPower;
			break;
		case PARA_TYPE_BSOB_WIRE_BOND_TIME:	//v3.1.T401 add
			pstWire->nBsobWireBondTimeOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nBaseTime;
			break;
		case PARA_TYPE_BSOB_WIRE_BOND_FORCE:	//v3.1.T401 add
			pstWire->nBsobWireBondForceOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nBaseForce;
			break;
		case PARA_TYPE_BSOB_WIRE_BOND_POWER:	//v3.1.T401 add
			pstWire->fBsobWireBondPowerOffset = ((float)m_anMapPara[nWireIdx] / (float)2.55) - stBsobWire.fBasePower;
			break;
		case PARA_TYPE_BSOB_WIRE_RLS_TIME:	//v3.1.T401 add
			pstWire->nBsobWireRlsTimeOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nReleaseTime;
			break;
		case PARA_TYPE_BSOB_WIRE_RLS_FORCE:	//v3.1.T401 add
			pstWire->nBsobWireRlsForceOffset = (short)m_anMapPara[nWireIdx] - stBsobWire.nReleaseForce;
			break;
		case PARA_TYPE_BSOB_WIRE_RLS_POWER:	//v3.1.T401 add
			pstWire->fBsobWireRlsPowerOffset = ((float)m_anMapPara[nWireIdx] / (float)2.55) - stBsobWire.fReleasePower;
			break;
        default:
            break;
        }
    }

    for (i = 0; i < m_nTotalRow; i++)
        delete m_paRowLabel[i];

    for (i = 1; i <= m_nTotalWire; i++)
        delete m_paMapEdit[i];

    CDialog::OnDestroy();
}

void CAdvParaMapDlg::SetParaMapType(short nParaType)
{
    m_nParaMapType = nParaType;
}

void CAdvParaMapDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    short nOffset = 0;
    int nMin, nMax, iCurPos = GetScrollPos(SB_VERT);
    SCROLLINFO stScrollInfo;

    GetScrollInfo(SB_VERT, &stScrollInfo);
    nMin = stScrollInfo.nMin;
    nMax = stScrollInfo.nMax;

    switch(nSBCode)
    {
    case SB_LINEDOWN:
	{
		if (iCurPos == nMax) break;

		nOffset = MAP_PARA_LABEL_HEIGHT + MAP_PARA_ROW_GAP;
		if (iCurPos + nOffset > nMax)
			nOffset = nMax - iCurPos;
		break;
	}
    case SB_LINEUP:
	{
		if (iCurPos == nMin) break;

		nOffset = -(MAP_PARA_LABEL_HEIGHT + MAP_PARA_ROW_GAP);
		if (iCurPos + nOffset < nMin)
			nOffset = nMin - iCurPos;
		break;
	}
    case SB_PAGEDOWN:
	{
		if (iCurPos == nMax) break;

		nOffset = 496;
		if (iCurPos + nOffset > nMax)
			nOffset = nMax - iCurPos;
		break;
	}
    case SB_PAGEUP:
	{
		if (iCurPos == nMin) break;

		nOffset = -496;
		if (iCurPos + nOffset < nMin)
			nOffset = nMin - iCurPos;
		break;
	}
    default:
        break;
    }

    SetScrollPos(SB_VERT, iCurPos + nOffset);
    ScrollWindow(0, -nOffset);

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CAdvParaMapDlg::SetParaApplyMode(int iApplyMode)
{
    m_iParaApplyMode = iApplyMode;
}
