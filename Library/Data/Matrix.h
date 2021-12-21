//Matrix.h v1.0.T12

#pragma once

#include "DataStruct.h"

//v1.0.T2 add
#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct MATRIX_RECORD
{
	short       nMatrixNum;
    TBL_POS     stVisionAlignPos[2];	//vision alignment position
    TBL_POS     stOriginalCenterPos;
    TBL_POS     stUpdatedCenterPos;
    double      dSinAln;
    double      dCosAln;
    short       nVisionStatus;			//reset to 0 when start bonding
    char        cReserve[98];

    //v2.19.0.w2 add - must init before use, or by default as 0
	MATRIX_RECORD():
	    nMatrixNum(),
	    stVisionAlignPos(),
	    stOriginalCenterPos(),
	    stUpdatedCenterPos(),
	    dSinAln(),
	    dCosAln(),
		nVisionStatus(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

enum enMatrixVisionState
{
    VISION_NOT_DONE,
    VISION_SUCCESS,
    VISION_SKIP_DIE,
    VISION_FAILURE,
};

// CMatrixMgr command target

class CMatrixMgr : public CObject
{
public:
	CMatrixMgr();
	virtual ~CMatrixMgr();
public:	//functions
	MATRIX_RECORD* GetRcd(short nMatNum);	//v1.0.T4 add
    int TransformPos(short nMatrixNum, TBL_POS *pstBondPt);
    void GetRcd(MATRIX_RECORD *pstMatRcd, short nMatNum);
    void AddRcd(void);	//v1.0.T11 add
    // Coordinate matrix record information with reference record
    int CoordinateMatrixInfo(short nMatNum);
    void SetRcd(MATRIX_RECORD* pstMatRcd, short nMatNum);
    void CalcMatrix(MATRIX_RECORD* pstMatRcd, short nMatNum, short nVisionMode);	//v1.0.T5 edit, v1.0.T6 edit
    void CalcMatrix2(short nMatNum, short nVisionMode);		//v1.0.T7 add
    int ClearAllMatrixRecord(void);
    // Get matrix position shift after PR
    int GetMatrixPosShift(short nMatrixNum, TBL_POS* pstPosShift);
    int ReverseTransformPos(short nMatrixNum, TBL_POS* pstBondPt);
    void DeleteLastRcd(void);
	void SetVisionStatus(short nMatrixNum, short nStatus);
private:	//variables
    short m_nTotalMatrixNum;
    MATRIX_RECORD* m_pstMatrixRecord;
private:	//functions
    int GetDefaultMatrix(MATRIX_RECORD* pstMatRcd);
    int CalcMatrixAngle(MATRIX_RECORD* pstMatRcd);
};

extern CMatrixMgr theMatrix;	//v1.0.T8 edit