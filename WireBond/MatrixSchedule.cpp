// MatrixSchedule.cpp : implementation file

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MathCalc.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "Bond.h"
#include "MatrixSchedule.h"
#include "MaterialHandlerThread.h"

// CMatrixSchedule dialog
IMPLEMENT_DYNAMIC(CMatrixSchedule, CDialog)

CMatrixSchedule::CMatrixSchedule(CWnd* pParent /*=NULL*/)
	: CDialog(CMatrixSchedule::IDD, pParent)
	, m_nIndexTime(0)
	, m_nTotalRow(0)
	, m_nTotalCol(0)
	, m_nCurRow(0)
	, m_nCurCol(0)
    , m_nCurMat(0)
{
}

CMatrixSchedule::~CMatrixSchedule()
{
    FreeButtonArray();
}

void CMatrixSchedule::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_INDEX_TIME, m_nIndexTime);
    DDX_Text(pDX, IDC_EDIT_TOTAL_ROW, m_nTotalRow);
    DDX_Text(pDX, IDC_EDIT_TOTAL_COLUMN, m_nTotalCol);
    DDX_Text(pDX, IDC_EDIT_CURRENT_ROW, m_nCurRow);
    DDX_Text(pDX, IDC_EDIT_CURRENT_COLUMN, m_nCurCol);
    DDX_Text(pDX, IDC_EDIT_CURRENT_MAT, m_nCurMat);
}

BEGIN_MESSAGE_MAP(CMatrixSchedule, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RESCHEDULE, OnReschedule)
    ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnPrevious)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNext)
	ON_MESSAGE(WM_CLOSEDIALOG,CloseDialog)	//V1.1.W156 add
END_MESSAGE_MAP()

// CMatrixSchedule message handlers
BOOL CMatrixSchedule::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);

	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemTextW(IDC_STATIC_MAT_INFO,theMsg.GetMsg(MSG_MAT_INFO));
	SetDlgItemTextW(IDC_STATIC_MAT_LAYOUT,theMsg.GetMsg(MSG_MAT_LAYOUT));
	SetDlgItemTextW(IDC_STATIC_INDEX_TIME,theMsg.GetMsg(MSG_INDEX_TIME));
	SetDlgItemTextW(IDC_STATIC_TOTAL_ROW,theMsg.GetMsg(MSG_TOTAL_ROW));
	SetDlgItemTextW(IDC_STATIC_TOTAL_COLUMN,theMsg.GetMsg(MSG_TOTAL_COL));
	SetDlgItemTextW(IDC_STATIC_CURRENT_MAT,theMsg.GetMsg(MSG_CUR_MAT));
	SetDlgItemTextW(IDC_STATIC_CURRENT_ROW,theMsg.GetMsg(MSG_CUR_ROW));
	SetDlgItemTextW(IDC_STATIC_CURRENT_COLUMN,theMsg.GetMsg(MSG_CUR_COL));
	SetDlgItemTextW(IDC_BUTTON_PREVIOUS,theMsg.GetMsg(MSG_PREV));
	SetDlgItemTextW(IDC_BUTTON_NEXT,theMsg.GetMsg(MSG_NEXT));
	SetDlgItemTextW(IDC_BUTTON_RESCHEDULE,theMsg.GetMsg(MSG_RESCHEDULE));

	m_nCurRow = 1;
	m_nCurCol = 1;

	if (thePgm.IsProgramValid()
		&& (theMHS.GetCurBondUnit() > 0)
		&& ((thePgm.GetDeviceSeq() == PGM_MAT_DIE_AHEAD)
			|| (thePgm.GetDeviceSeq() == PGM_MAT_HYBRID)))
	{
		m_nIndexTime   = theMHS.GetCurBondUnit();
		m_nTotalRow    = thePgm.GetMatrixRow();
		m_nTotalCol = thePgm.GetMatrixCol();

		if ((m_nTotalCol == 0) || (m_nTotalRow == 0)) //throw "w.temp.15" ;
		{
			AfxMessageBox(_T("temp.12, TotalCol || TotalRow = 0"));
			return 0;
		}

		short nUnitWidth  = round((double)LF_INDEX_AREA_WIDTH  / m_nTotalCol);
		short nUnitHeight = round((double)LF_INDEX_AREA_HEIGHT / m_nTotalRow);
		nUnitWidth = min(nUnitWidth, nUnitHeight);

		CPoint pt, StartPt, pCentre;
		CRect  pUnitRect;

		pCentre.x = LF_INDEX_AREA_LEFT + LF_INDEX_AREA_WIDTH / 2 + 14;
		pCentre.y = LF_INDEX_AREA_TOP + LF_INDEX_AREA_HEIGHT / 2 - 16;

		pt.x = pCentre.x + round(nUnitWidth * (m_nTotalCol - 1) / 2.0);
		pt.y = pCentre.y - round(nUnitWidth * (m_nTotalRow - 1) / 2.0);

		StartPt = pt;

		short nMatNum, nUnitId = IDB_SUBUNIT_BUTTON0;

		for (short i = 0; i < m_nTotalCol; i++)
		{
			pt.y = StartPt.y;

			for (short j = 0; j < m_nTotalRow; j++)
			{
				CColorButton* pButton = new CColorButton();
				pUnitRect.SetRect(pt.x - nUnitWidth / 2, pt.y - nUnitWidth / 2, pt.x + nUnitWidth / 2, pt.y + nUnitWidth / 2);
				pButton->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, pUnitRect, this, nUnitId++);

				m_pButtonArray.Add(pButton);

				nMatNum = thePgm.GetMatNum(j + 1, i + 1);
				SetMatrixScheduleState(nMatNum, thePgm.GetIndexMatrixSchedule(m_nIndexTime, nMatNum));

				pt.y += nUnitWidth;
			}

			pt.x -= nUnitWidth;
		}

		m_nCurMat = thePgm.GetMatNum(m_nCurRow, m_nCurCol);
		HighLightCurrentMatrix(m_nCurMat, TRUE);

		GetDlgItem(IDC_BUTTON_RESCHEDULE)->EnableWindow(TRUE);
	}
	else
	{
		m_nIndexTime     = 0;
		m_nTotalRow      = 1;
		m_nTotalCol   = 1;
		m_nCurMat = 1;

		GetDlgItem(IDC_BUTTON_RESCHEDULE)->EnableWindow(FALSE);
	}

	return CDialog::OnInitDialog();
}

HBRUSH CMatrixSchedule::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CMatrixSchedule::FreeButtonArray(void)
{
	while (m_pButtonArray.GetCount())
	{
		delete m_pButtonArray.GetAt(0);
		m_pButtonArray.RemoveAt(0);
	}
}

void CMatrixSchedule::HighLightCurrentMatrix(short nMatrix, BOOL bState)
{
    short nRow, nCol;
    thePgm.GetMatRowCol(nMatrix, &nRow, &nCol);
    nMatrix = thePgm.GetMatrixRow() * (nCol - 1) + nRow - 1;

	CColorButton* pButton = m_pButtonArray.GetAt(nMatrix);
	pButton->SetCheck(bState);
	pButton->Invalidate();
}

void CMatrixSchedule::SetMatrixScheduleState(short nMatrix, bool bState)
{
	short nRow, nCol;
	thePgm.GetMatRowCol(nMatrix, &nRow, &nCol);
	nMatrix = thePgm.GetMatrixRow() * (nCol - 1) + nRow - 1;

	CColorButton* pButton = m_pButtonArray.GetAt(nMatrix);
	pButton->SetBkColor(bState ? RGB(255,255,0) : theApp.m_bkColor);
	pButton->Invalidate();
}

void CMatrixSchedule::OnReschedule()
{
    bool bMatSchedule = !thePgm.GetIndexMatrixSchedule(m_nIndexTime, m_nCurMat);
    thePgm.SetIndexMatrixSchedule(m_nIndexTime, m_nCurMat, bMatSchedule);
    SetMatrixScheduleState(m_nCurMat, bMatSchedule);
}

void CMatrixSchedule::OnPrevious()
{
	if (m_nCurMat < 2) return;

	HighLightCurrentMatrix(m_nCurMat--, FALSE);
	thePgm.GetMatRowCol(m_nCurMat, &m_nCurRow, &m_nCurCol);
	HighLightCurrentMatrix(m_nCurMat, TRUE);
	UpdateData(FALSE);
}

void CMatrixSchedule::OnNext()
{
	if (m_nCurMat >= m_nTotalRow * m_nTotalCol) return;

	HighLightCurrentMatrix(m_nCurMat++, FALSE);
	thePgm.GetMatRowCol(m_nCurMat, &m_nCurRow, &m_nCurCol);
	HighLightCurrentMatrix(m_nCurMat, TRUE);
	UpdateData(FALSE);
}

void CMatrixSchedule::PostNcDestroy()
{
    CDialog::PostNcDestroy();

    delete this;
}

LRESULT CMatrixSchedule::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.1.W156 add
{
	return DestroyWindow() * DIALOG_CLOSED;
}
