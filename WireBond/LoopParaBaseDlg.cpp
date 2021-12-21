// LoopParaBaseDlg.cpp : implementation file

#include "stdafx.h"
#include "MathCalc.h"
#include "MenuConfig.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Reference.h"
#include "Wire.h"
#include "LoopPara.h"
#include "LoopShape.h"
#include "AdvLoopParaSheet.h"
#include "LoopParaBaseDlg.h"
#include "MachineConfig.h"
#include <ComUtil.h>

// CLoopParaBaseDlg dialog
IMPLEMENT_DYNAMIC(CLoopParaBaseDlg, CDialog)

CLoopParaBaseDlg::CLoopParaBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoopParaBaseDlg::IDD, pParent)
    , m_iLoopGroup(0)
    , m_iLoopType(0)
    , m_iLoopProfile(0)
    , m_nCtactDiff(0)
    , m_nWireNum(0)
    , m_sWireLen(_T("0"))
    , m_sBondOnRef(_T(""))
	, m_pParaSheet(NULL)
{
}

CLoopParaBaseDlg::~CLoopParaBaseDlg()
{
}

void CLoopParaBaseDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_LOOP_GROUP, m_iLoopGroup);
    DDX_CBIndex(pDX, IDC_COMBO_LOOP_TYPE, m_iLoopType);
    DDX_CBIndex(pDX, IDC_COMBO_LOOP_PROFILE, m_iLoopProfile);
    DDX_Text(pDX, IDC_STATIC_CD, m_nCtactDiff);
    DDX_Text(pDX, IDC_EDIT_WIRE_NUM, m_nWireNum);
    DDX_Text(pDX, IDC_STATIC_WIRE_LENGTH, m_sWireLen);
    DDX_Text(pDX, IDC_STATIC_BOND_ON_VALUE, m_sBondOnRef);
}

BEGIN_MESSAGE_MAP(CLoopParaBaseDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO_LOOP_GROUP, OnLoopGroup)
    ON_WM_PAINT()
    ON_CBN_SELCHANGE(IDC_COMBO_LOOP_TYPE, OnLoopType)
    ON_CBN_SELCHANGE(IDC_COMBO_LOOP_PROFILE, OnLoopProfile)
    ON_EN_SETFOCUS(IDC_EDIT_WIRE_NUM, OnFocusWireNum)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.1.W246 add
END_MESSAGE_MAP()

// CLoopParaBaseDlg message handlers
HBRUSH CLoopParaBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CLoopParaBaseDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	m_pWndLoopDisp = GetDlgItem(IDC_LOOP_DIAGRAM);
	m_pWndLoopDisp->SetWindowPos(NULL, LOOP_DIAGRAM_BOX_LEFT, LOOP_DIAGRAM_BOX_TOP, LOOP_DIAGRAM_BOX_WIDTH, LOOP_DIAGRAM_BOX_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_STATIC_LOOP_GROUP,theMsg.GetMsg(MSG_GROUP));
	SetDlgItemTextW(IDC_STATIC_LOOP_TYPE,theMsg.GetMsg(MSG_TYPE));
	SetDlgItemTextW(IDC_STATIC_WIRE,theMsg.GetMsg(MSG_EDIT_WIRE));
	SetDlgItemTextW(IDC_STATIC_LENGTH,theMsg.GetMsg(MSG_LENGTH_2));
	SetDlgItemTextW(IDC_STATIC_LOOP_PROFILE,theMsg.GetMsg(MSG_PROFILE));
	SetDlgItemTextW(IDC_STATIC_CTACT_DIFFERENCE,theMsg.GetMsg(MSG_CTACT_DIFF));
	SetDlgItemTextW(IDC_STATIC_BOND_ON,theMsg.GetMsg(MSG_REF));

	theLoopPara.CountProgramLoopMode();
	CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOOP_GROUP);
	for (int i = 0; i < theLoopPara.m_nTotalLoopModeUsed; i++)
		pComBox->AddString(_bstr_t(theLoopPara.m_nLoopModeUsed[i]));

	m_iLoopGroup = 0;
	UpdateLoopGroupMenu();

	//short iLoopGroup = GetDlgItemInt(IDC_COMBO_LOOP_GROUP);	//v3.1.T360 delete
	if (m_pParaSheet) delete m_pParaSheet;	//v3.1.T360 add
	m_pParaSheet = new CAdvLoopParaSheet(theMsg.GetMsg(MSG_LOOP_PARAMETER), m_iLoopGroup);	//v3.1.T360 edit
	m_pParaSheet->Create(this);

	UpdateLoopDiagram();
	return CDialog::OnInitDialog();
}

void CLoopParaBaseDlg::OnLoopGroup()
{
	UpdateData();
	m_pParaSheet->PressButton(PSBTN_OK);
	UpdateLoopGroupMenu();

	//short iLoopGroup = GetDlgItemInt(IDC_COMBO_LOOP_GROUP);  //v1.6.P252sc add, v3.1.T360 delete
	if (m_pParaSheet) delete m_pParaSheet;	//v3.1.T360 add
	m_pParaSheet = new CAdvLoopParaSheet(theMsg.GetMsg(MSG_LOOP_PARAMETER), m_iLoopGroup);	//v3.1.T360 edit
	m_pParaSheet->Create(this);
}

void CLoopParaBaseDlg::UpdateLoopGroupMenu(short nWireNum)
{
	ST_LOOP_PARA stLoopPara;
	theLoopPara.GetRcd(theLoopPara.m_nLoopModeUsed[m_iLoopGroup], &stLoopPara);
	m_iLoopType = stLoopPara.stLoopBase.nKinkNum - 1;

	CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_LOOP_PROFILE);
	pComBox->ResetContent();

	if (stLoopPara.stLoopBase.nKinkNum == 1)
	{
		pComBox->AddString(_T("BGA1"));
		pComBox->AddString(_T("VLED"));
		m_iLoopProfile = stLoopPara.stLoopBase.nLoopProfile - LOOP_PROFILE_BGA1;
	}
	else if (stLoopPara.stLoopBase.nKinkNum == 2)
	{
		pComBox->AddString(_T("BGA2"));
		m_iLoopProfile = stLoopPara.stLoopBase.nLoopProfile - LOOP_PROFILE_BGA2;
	}
	else if (stLoopPara.stLoopBase.nKinkNum == 3)
	{
		pComBox->AddString(_T("BGA3"));
		pComBox->AddString(_T("BGA4"));
		m_iLoopProfile = stLoopPara.stLoopBase.nLoopProfile - LOOP_PROFILE_BGA3;
	}

	if (nWireNum == 0)
	{
		for (short i = 1; i <= thePgm.GetTotalWire(); i++)
		{
			if (theWire.GetRcd(i)->nLoopMode == theLoopPara.m_nLoopModeUsed[m_iLoopGroup])
			{
				m_nWireNum = i;
				break;
			}
		}
	}
	else
		m_nWireNum = nWireNum;
	WIRE_RECORD *pstWire = theWire.GetRcd(m_nWireNum);
	m_sWireLen.Format(_T("%d um"), round(theWire.GetWireLength(m_nWireNum)));
	m_sBondOnRef.Format(_T("%d > %d"), pstWire->nRefNum[0], pstWire->nRefNum[1]);
	m_nCtactDiff = (short)(theRef.GetRcd(pstWire->nRefNum[0])->lCtactLvl
							- theRef.GetRcd(pstWire->nRefNum[1])->lCtactLvl);

	UpdateData(FALSE);
	Invalidate();
}

//v3.1.T343 clean
void CLoopParaBaseDlg::UpdateLoopDiagram(void)
{
	WIRE_RECORD *pstWire = theWire.GetRcd(m_nWireNum);
	long lCtactLvl1 = theRef.GetRcd(pstWire->nRefNum[0])->lCtactLvl;
	long lCtactLvl2 = theRef.GetRcd(pstWire->nRefNum[1])->lCtactLvl;

	//v3.1.T71 edit
	double dWireLen = theWire.GetWireLength(m_nWireNum);
	ST_LOOP_SHAPE_PARA stLoopShape;
	if (theMcConfig.GetLoopShapeType() == 1)	//v3.1.T333 add
		theLoopShape.CalcLoopShapeEx(dWireLen, lCtactLvl1, lCtactLvl2,
			pstWire->nLoopMode, pstWire->nWireLengthCorrect, &stLoopShape);
	else
		theLoopShape.CalcLoopShape(dWireLen, lCtactLvl1, lCtactLvl2,
			pstWire->nLoopMode, pstWire->nWireLengthCorrect, &stLoopShape);

	LOOP_PROFILE_DISP_PARA stLoopDisp;
	theLoopShape.CalcLoopProfileDisplay(m_nCtactDiff, &stLoopShape, &stLoopDisp);

	double dSpanX = stLoopDisp.stBondPos[1].dx;
	double dSpanY = stLoopDisp.stKink1.dy;
	dSpanY = max(dSpanY, stLoopDisp.stKink2.dy);
	dSpanY = max(dSpanY, stLoopDisp.stKink3.dy);
	dSpanY = max(dSpanY, stLoopDisp.stKink4.dy);
	dSpanY = max(dSpanY, stLoopDisp.stBondPos[1].dy);

	double dFactorX = (0.8 * LOOP_DIAGRAM_BOX_WIDTH) / dSpanX;
	double dFactorY = (0.8 * LOOP_DIAGRAM_BOX_HEIGHT) / dSpanY;
	if (dFactorY > 0.4) dFactorY = 0.4;

	CDC *pCtrlDC = m_pWndLoopDisp->GetDC();
	m_pWndLoopDisp->Invalidate();
	m_pWndLoopDisp->UpdateWindow();

	//show background
	CBrush BrushBlue(RGB(0,0,255));
	pCtrlDC->SelectObject(&BrushBlue);
	pCtrlDC->Rectangle(0, 0, LOOP_DIAGRAM_BOX_WIDTH-5, LOOP_DIAGRAM_BOX_HEIGHT-5);

	//show 1st & 2nd bond surface
	CPoint Pt;
	CPoint OriPt = CPoint(29, LOOP_DIAGRAM_BOX_HEIGHT - 18);
	CBrush BrushWhite(RGB(255,255,255));
	pCtrlDC->SelectObject(&BrushWhite);
	if (m_nCtactDiff > 0)
	{
		Pt.x = OriPt.x;
		Pt.y = OriPt.y - round(m_nCtactDiff * dFactorY);
		pCtrlDC->Rectangle(Pt.x-20, Pt.y, Pt.x+20, LOOP_DIAGRAM_BOX_HEIGHT-5);
		Pt.x = 261;
		Pt.y = OriPt.y;
		pCtrlDC->Rectangle(Pt.x-20, Pt.y, Pt.x+20, LOOP_DIAGRAM_BOX_HEIGHT-5);
	}
	else
	{
		Pt = OriPt;
		pCtrlDC->Rectangle(Pt.x-20, Pt.y, Pt.x+20, LOOP_DIAGRAM_BOX_HEIGHT-5);
		Pt.x = 261;
		Pt.y = OriPt.y + round(m_nCtactDiff * dFactorY);
		pCtrlDC->Rectangle(Pt.x-20, Pt.y, Pt.x+20, LOOP_DIAGRAM_BOX_HEIGHT-5);
	}

	//show 1st bond ball
	Pt.x = OriPt.x + round(stLoopDisp.stBondPos[0].dx * dFactorX);
	Pt.y = OriPt.y - round(stLoopDisp.stBondPos[0].dy * dFactorY);
	CBrush BrushYellow(RGB(255,255,0));
	pCtrlDC->SelectObject(&BrushYellow);
	pCtrlDC->Ellipse(Pt.x-5, Pt.y-8, Pt.x+5, Pt.y+2);

	//show wire
	CPen PenYellow = CPen(PS_SOLID, 2, RGB(255,255,0));
	pCtrlDC->SelectObject(&PenYellow);
	pCtrlDC->MoveTo(Pt);

	Pt.x = OriPt.x + round(stLoopDisp.stKink1.dx * dFactorX);
	Pt.y = OriPt.y - round(stLoopDisp.stKink1.dy * dFactorY);
	pCtrlDC->LineTo(Pt);

	Pt.x = OriPt.x + round(stLoopDisp.stKink2.dx * dFactorX);
	Pt.y = OriPt.y - round(stLoopDisp.stKink2.dy * dFactorY);
	pCtrlDC->LineTo(Pt);

	Pt.x = OriPt.x + round(stLoopDisp.stKink3.dx * dFactorX);
	Pt.y = OriPt.y - round(stLoopDisp.stKink3.dy * dFactorY);
	pCtrlDC->LineTo(Pt);

	Pt.x = OriPt.x + round(stLoopDisp.stKink4.dx * dFactorX);
	Pt.y = OriPt.y - round(stLoopDisp.stKink4.dy * dFactorY);
	pCtrlDC->LineTo(Pt);

	Pt.x = OriPt.x + round(stLoopDisp.stBondPos[1].dx * dFactorX);
	Pt.y = OriPt.y - round(stLoopDisp.stBondPos[1].dy * dFactorY);
	pCtrlDC->LineTo(Pt);

	pCtrlDC->SelectStockObject(BLACK_BRUSH);
	m_pWndLoopDisp->ReleaseDC(pCtrlDC);
}

void CLoopParaBaseDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    UpdateLoopDiagram();
}

void CLoopParaBaseDlg::OnLoopType()
{
	int iBkupLoopType = m_iLoopType;
	UpdateData();
	if (m_iLoopType != iBkupLoopType)
	{
		short anMsg[1] = {MSG_SURE_TO_CHANGE_LOOP_TYPE};
		if (theMsg.ShowMsg(WARNING_MSG, 1, anMsg, true) == SHOW_MSG_RESULT_STOP)
		{
			m_iLoopType = iBkupLoopType;
			UpdateData(FALSE);
			return;
		}

		short nKinkNum = m_iLoopType + 1;
		m_pParaSheet->PressButton(PSBTN_OK);

		ST_LOOP_PARA stLoopPara;
		theLoopPara.GetDefaultRcd(&stLoopPara, nKinkNum);
		theLoopPara.SetRcd(theLoopPara.m_nLoopModeUsed[m_iLoopGroup], &stLoopPara);
		UpdateLoopGroupMenu();

		//short iLoopGroup = GetDlgItemInt(IDC_COMBO_LOOP_GROUP);	//v3.1.T360 delete
		if (m_pParaSheet) delete m_pParaSheet;	//v3.1.T360 add
		m_pParaSheet = new CAdvLoopParaSheet(theMsg.GetMsg(MSG_LOOP_PARAMETER), m_iLoopGroup);	//v3.1.T360 edit
		m_pParaSheet->Create(this);
	}
}

void CLoopParaBaseDlg::OnLoopProfile()
{
	int iBkupProfile = m_iLoopProfile;
	UpdateData();
	if (iBkupProfile != m_iLoopProfile)
	{
		short anMsg[1] = {MSG_SURE_TO_CHANGE_LOOP_PROFILE};
		if (theMsg.ShowMsg(WARNING_MSG, 1, anMsg, true) == SHOW_MSG_RESULT_STOP)
		{
			m_iLoopProfile = iBkupProfile;
			UpdateData(FALSE);
			return;
		}

		if (m_iLoopType == 0)
			m_iLoopProfile += LOOP_PROFILE_BGA1;
		else if (m_iLoopType == 1)
			m_iLoopProfile += LOOP_PROFILE_BGA2;
		else
			m_iLoopProfile += LOOP_PROFILE_BGA3;

		m_pParaSheet->PressButton(PSBTN_OK);
		theLoopPara.ConvertLoopProfile(m_iLoopGroup, m_iLoopProfile, m_nCtactDiff);
		UpdateLoopGroupMenu();

		//short iLoopGroup = GetDlgItemInt(IDC_COMBO_LOOP_GROUP);	//v3.1.T360 delete
		if (m_pParaSheet) delete m_pParaSheet;	//v3.1.T360 add
		m_pParaSheet = new CAdvLoopParaSheet(theMsg.GetMsg(MSG_LOOP_PARAMETER), m_iLoopGroup);	//v3.1.T360 edit
		m_pParaSheet->Create(this);
	}
}

void CLoopParaBaseDlg::OnFocusWireNum()
{
	GetDlgItem(IDC_LOOP_DIAGRAM)->SetFocus();
	theKeyPad.GetUserKeyInData(&m_nWireNum);
	UpdateData(FALSE);

	for (short i = 0; i < TOTAL_LOOP_MODE; i++)
	{
		if (theLoopPara.m_nLoopModeUsed[i] == theWire.GetRcd(m_nWireNum)->nLoopMode)
		{
			m_iLoopGroup = i;
			break;
		}
	}

	m_pParaSheet->PressButton(PSBTN_OK);
	UpdateData(FALSE);
	UpdateLoopGroupMenu(m_nWireNum);

	//short iLoopGroup = GetDlgItemInt(IDC_COMBO_LOOP_GROUP);	//v3.1.T360 delete
	if (m_pParaSheet) delete m_pParaSheet;	//v3.1.T360 add
	m_pParaSheet = new CAdvLoopParaSheet(theMsg.GetMsg(MSG_LOOP_PARAMETER), m_iLoopGroup);	//v3.1.T360 edit
	m_pParaSheet->Create(this);
}

LRESULT CLoopParaBaseDlg::CloseDialog(WPARAM wParam, LPARAM lParam)	//V1.1.W246 add
{
	if (m_pParaSheet) delete m_pParaSheet;	//v3.1.T360 add
	return DestroyWindow() * DIALOG_CLOSED;
}

void CLoopParaBaseDlg::PostNcDestroy()	//V1.1.W246 add
{
	CDialog::PostNcDestroy();
	delete this;
}
