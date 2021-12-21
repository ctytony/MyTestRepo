//DeviceData.h v2.0.T2

#pragma once

#include "DataStruct.h"

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct TOP_DEVICE_RECORD	//v1.0.T10 add. v1.0.T11 edit
{
	long  lDeviceTotalLen;
	long  lDeviceEdgeLen;
	long  lDeviceColmPitch;
	short nDeviceColmNum;
	short nDeviceRowNum;
	short nWndUnitNum;
	long  lMagLvPitch;
	long  lMag1stLvHt;
	short lMagSlotNum;
	long  lMagLastLvHt;
	short nHalfIndexOn;    // Last index half pitch
	short nDeviceId; 
	char  chReserve[146];

    //must init before use, or by default as 0
	TOP_DEVICE_RECORD():
		lDeviceTotalLen(),
		lDeviceEdgeLen(),
		lDeviceColmPitch(),
		nDeviceColmNum(),
		nDeviceRowNum(),
		nWndUnitNum(),
		lMagLvPitch(),
		lMag1stLvHt(),
		lMagSlotNum(),
		lMagLastLvHt(),
		nHalfIndexOn(),
		nDeviceId(){}
};

struct CUELITE_DEVICE_RECORD	//v1.0.T10 add, v1.0.T11 edit
{
	long  lDeviceTotalLen;
	long  lDeviceEdgeLen;
	long  lDeviceColmPitch;
	short nDeviceColmNum;
	short nDeviceRowNum;
	short nWndUnitNum;
	long  lMagLvPitch;
	long  lMag1stLvHt;
	short lMagSlotNum;
	long  lMagLastLvHt;
	short nHalfIndexOn;			//Last index half pitch
	short nDeviceId; 
	long  lMagWidth;
	long  lMagLength;
	long  lDeviceTotalWidth;
	long  lDeviceCenterOffset;
	short nSlipListId;

	//this is the BLOCK size, reserve for future
	//MUST reduce this size if add any variable 
	// and varibable must be added be added in sequence
	char  chReserve[142];	//144->142

    //must init before use, or by default as 0
	CUELITE_DEVICE_RECORD():
	lDeviceTotalLen(),
	lDeviceEdgeLen(),
	lDeviceColmPitch(),
	nDeviceColmNum(),
	nDeviceRowNum(),
	nWndUnitNum(),
	lMagLvPitch(),
	lMag1stLvHt(),
	lMagSlotNum(),
	lMagLastLvHt(),
	nHalfIndexOn(),			//Last index half pitch
	nDeviceId(),
	lMagWidth(),
	lMagLength(),
	lDeviceTotalWidth(),
	lDeviceCenterOffset(),
	nSlipListId(){}
};

struct INDEX_PR_REF	//v1.0.T10 add
{	
	bool        bIndexPrAvail;
    TBL_POS     stManuAlignPos;     //Manual alignment position
    TBL_POS     stVisionAlignPos;   //Vision Alignment position
    LIGHT_PARA  stManuAlignLightPara;
    LIGHT_PARA  stVisionLightPara;
    ULONG       ulManuAlignPatternID;
    ULONG       ulVisionAlignPatternID;
    short       nManuAlignMag;
    short       nVisionAlignMag;
    short       nManuSrchSizeX;
    short       nManuSrchSizeY;
    short       nVisionSrchSizeX;
    short       nVisionSrchSizeY;
    bool        bVisionBkupLight;
    LIGHT_PARA  stVisionBkupLightPara;
    TBL_POS     stVisionBkupAlignPos;  
    ULONG       ulVisionBkupPatternID; 
    short       nBkupAlignSrchSizeX; 
    short       nBkupAlignSrchSizeY;
    char        cReserve[98];

    //must init before use, or by default as 0
	INDEX_PR_REF():
		bIndexPrAvail(),
		stManuAlignPos(),
		stVisionAlignPos(),
		stManuAlignLightPara(),
		ulManuAlignPatternID(),
		ulVisionAlignPatternID(),
		nManuAlignMag(),
		nVisionAlignMag(),
		nManuSrchSizeX(),
		nManuSrchSizeY(),
		nVisionSrchSizeX(),
		nVisionSrchSizeY(),
		bVisionBkupLight(),
		stVisionBkupLightPara(),
		stVisionBkupAlignPos(),
		ulVisionBkupPatternID(),
		nBkupAlignSrchSizeX(),
		nBkupAlignSrchSizeY(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CDeviceDataMgr command target

//enum enDeviceList	//v1.0.T9 delete
enum EN_DEVICE_ID	//v1.0.T9 add
{
	ID_5050_LF = 1,
	ID_14R5C_LF,
	ID_14R6C_LF,
	ID_5R3C_LF,
    ID_14R4C_LF,
    ID_10R3C_LF,
    ID_10R6C_LF,
    ID_10W_HIGHPWR,
	ID_100W_HIGHPWR,
	ID_PCB_LONG,
	ID_DIC_0603,
	ID_SOP16,
	ID_SOP7_1C,
	ID_SOP7_2C,
	ID_CEDAR,
};

#define CUELITE_MAG_MINI_LENTH		1370000		//careful here, dimension is in 0.1um
#define CUELITE_MAG_MEDIUM_LENTH	1650000
#define CUELITE_MAG_MEDIUM2_LENTH	2350000
#define CUELITE_MAG_MAX_LENTH		3100000		//max length magizine can be opened
#define CUELITE_MAG_MAX_WIDTH		980000		//max width magizine can be opened
#define CUELITE_MAG_MIN_WIDTH		250000

#define CUELITE_LF_MAX_WIDTH_SET1	600000
#define CUELITE_LF_MIN_WIDTH_SET1	200000
#define CUELITE_LF_MIN_WIDTH_SET2	200000
#define CUELITE_LF_MAX_WIDTH_SET2	600000		//max width track can be opened

class CDeviceDataMgr : public CObject
{
public:
	CDeviceDataMgr();
	virtual ~CDeviceDataMgr();
public:
	TOP_DEVICE_RECORD *GetTopRcd(void);		//v1.0.T13
	void GetTopRcd(TOP_DEVICE_RECORD *pstDeviceRcd);	//v1.0.T12 edit
	void SetTopRcd(TOP_DEVICE_RECORD *pstDeviceRcd);	//v1.0.T12 edit
	void SaveTopRcd(void);	//v1.0.T12 edit
	void LoadTopRcd(void);	//v1.0.T12 edit
	void GetIndexPrRef(INDEX_PR_REF *pstIndexPrRef);
	void SetIndexPrRef(INDEX_PR_REF *pstIndexPrRef);	//v1.0.T12 edit
	void DeleteIndexPrRef(void);
    short GetLastIndexWndColumn(void);	//Get the device column of last index
    void GetCuEliteRcd(CUELITE_DEVICE_RECORD *pstDeviceRcd);	//v1.0.T12 edit
	void SetCuEliteRcd(CUELITE_DEVICE_RECORD *pstDeviceRcd);	//v1.0.T12 edit
    void SaveCuEliteRcd(void);	//v1.0.T12 edit
	void LoadCuEliteRcd(void);	//v1.0.T12 edit
    void Init();	//v1.0.T3 edit
private:	//variables
	TOP_DEVICE_RECORD m_stTopDevice;	//v1.0.T7 edit
    CUELITE_DEVICE_RECORD m_stCuEliteDevice;	//v1.0.T7 edit
	INDEX_PR_REF m_stIndexPrRef;
private:	//functions
	void SetDefaultTopRcd();
	void SetDefaultIndexPrRef();
    void SetDefaultCuEliteRcd();
};

extern CDeviceDataMgr theDevice;	//v1.0.T12 edit