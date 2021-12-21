//MachineData.h v1.1.T23

#pragma once

#include "DataStruct.h"

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct MACHINE_DATA
{
	short       nMcDataVersion;        //Machine data version for load/save bond recipe
    long        lResetLevel;
    long        lRelaxLevel;
    TBL_POS     stBondTipOs;
	double      dHighMagFactX;
	double      dHighMagFactY;
	double      dLowMagFactX;
	double      dLowMagFactY;
    double      dForceFactorKf;        //Force factor 1
    double      dForceFactorKa;        //Position factor (common)
    double      dForceFactorIo;        //Current offset 1
    long        lForceCalibLvl;        //Contact level
    double      dWclForceFactorKf;     //Wire clamp force factor
    double      dWclForceFactorIo;     //Wire clamp current offset
    float       fBsdApplyVoltage;      //BSD apply voltage, v
    float       fDieShortThreshold;    //Die short threshold, v
    float       fLeadShortThreshold;   //Lead short threshold, v
    short       nTailShortSample;      //Tail short sample, sample
    short       nNonStickSample;       //2nd bond nonstick, sample
    TBL_POS     stThreadWirePos;
    long        lCapillaryLimit;
	long        lUSGFreqCalib;
    short       nSearchMode;
    short       nDampGain;
    double      dPosCompKp;
    double      dPosCompOs;
    short       nDampGain2;
    long        lTblXPosLimit;
    long        lTblXNegLimit;
    long        lTblYPosLimit;
    long        lTblYNegLimit;
    bool        bZeroBtosOptics;
    bool        bTscEnable;
    float       fTscFactor;
    short       nPowerSlope;
    short       nPowerOffset;
    long        lZeroOffset;			//length from physical home sensor
	long        lIndexerLimit;			//length from physical home sensor
	TBL_POS     stTbSafeStandby;
	double      dOnloadResolu; 
	double      dOffloadResolu;
	double      dIndexerResolu;
	long        lFlatLevel;				//v1.1.T9 edit
    long        lPwrCompOffsetCapiCount;
    //double      dPwrCompOffset;		//v1.1.T1 delete
	float		fPwrCompOs1;			//v1.1.T1 add
	float		fPwrCompOs2;			//v1.1.T1 add
    long        lPwrCompEndCapiCount;
    short       nTailBreakNonStickSample;	//Tail break 2nd bond nonstick, sample
    bool        bTailCtrlMode;
    bool        bWireFeedCheck;
    bool        bStickCoarseDetect;
    short       nStickCoarseDetectSamples;
    double      dVisionFactCos;
    double      dVisionFactSin;
    char        chUphMode;
    short       nLocateMode;
    //bool		bDieMultiPr;	//delete
	bool		bReserve;		//add
	BYTE        cGapLargeDac;
    short       nImpLowerLimit;
	short       nImpUpperLimit;
    long        lOnLoadMagClamperLimit; 
    long        lOffLoadMagClamperLimit; 
    long        lOnLoadMagClamperPosition;     
    long        lOffLoadMagClamperPosition;

	//machine offset
    long        lOnLoadMagazineElevatorOffset;  //0.1um
    long        lOnLoadMagazineLengthOffset;    //0.1um
    long        lOnLoadMagazineYOneOffset;      //0.1um
    long        lOnLoadMagazineYTwoOffset;      //0.1um
    long        lOffLoadMagazineElevatorOffset; //0.1um
    long        lOffLoadMagazineLengthOffset;   //0.1um
	long        lOffLoadMagazineYOneOffset;     //0.1um
    long        lOffLoadMagazineYTwoOffset;     //0.1um
    long        lLFFrontTrackOffset;            //0.1um
    long        lLFWindowClamperOffset;         //0.1um
	long        lLFIndexerOffset;               //um
    long        lLFRearTrackOffset;             //0.1um

	double      dForceFactorKf2;				//Force factor 2
	double      dForceFactorIo2;				//Current offset 2
	char		chRcdSample;
	char		chRcdTol;
	BYTE		nTransducerIdleTime;			//minute				//v1.0.T4 add
	short		nTransducerLengthComp;			//um, can be +ve or -ve	//v1.0.T4 add, v1.0.T6 edit

	            //this is the BLOCK size, reserve for future
                //MUST reduce this size if add any variable 
	            // and varibable must be added be added in sequence

	//char		cReserve[88];	//[136]->[88]
	//char		cReserve[72];	//[88]->[72]
	//char		cReserve[70];	//[72]->[70]
	char		cReserve[67];	//[70]->[67]	//v1.0.T4 add

    //must init before use, or by default as 0
	MACHINE_DATA():
		nMcDataVersion(),
		lResetLevel(),
		lRelaxLevel(),
		stBondTipOs(),
		dHighMagFactX(),
		dHighMagFactY(),
		dLowMagFactX(),
		dLowMagFactY(),
		dForceFactorKf(),
		dForceFactorKa(),
		dForceFactorIo(),
		lForceCalibLvl(),
		dWclForceFactorKf(),
		dWclForceFactorIo(),
		fBsdApplyVoltage(),
		fDieShortThreshold(),
		fLeadShortThreshold(),
		nTailShortSample(),
		nNonStickSample(),
		stThreadWirePos(),
		lCapillaryLimit(),
		lUSGFreqCalib(),
		nSearchMode(),
		nDampGain(),
		dPosCompKp(),
		dPosCompOs(),
		nDampGain2(),
		lTblXPosLimit(),
		lTblXNegLimit(),
		lTblYPosLimit(),
		lTblYNegLimit(),
		bZeroBtosOptics(),
		bTscEnable(),
		fTscFactor(),
		nPowerSlope(),
		nPowerOffset(),
		lZeroOffset(),
		lIndexerLimit(),
		stTbSafeStandby(),
		dOnloadResolu(),
		dOffloadResolu(),
		dIndexerResolu(),
		lFlatLevel(),			//v1.1.T9 edit
		lPwrCompOffsetCapiCount(),
		//dPwrCompOffset(),		//v1.1.T1 delete
		fPwrCompOs1(),			//v1.1.T1 add
		fPwrCompOs2(),			//v1.1.T1 add
		lPwrCompEndCapiCount(),
		nTailBreakNonStickSample(),
		bTailCtrlMode(),
		bWireFeedCheck(),
		bStickCoarseDetect(),
		nStickCoarseDetectSamples(),
		dVisionFactCos(),
		dVisionFactSin(),
		chUphMode(),
		nLocateMode(),
		cGapLargeDac(),
		nImpLowerLimit(),
		nImpUpperLimit(),
		lOnLoadMagClamperLimit(),
		lOffLoadMagClamperLimit(),
		lOnLoadMagClamperPosition(),
		lOffLoadMagClamperPosition(),
		lOnLoadMagazineElevatorOffset(),
		lOnLoadMagazineLengthOffset(),
		lOnLoadMagazineYOneOffset(),
		lOnLoadMagazineYTwoOffset(),
		lOffLoadMagazineElevatorOffset(),
		lOffLoadMagazineLengthOffset(),
		lOffLoadMagazineYOneOffset(),
		lOffLoadMagazineYTwoOffset(),
		lLFFrontTrackOffset(),
		lLFWindowClamperOffset(),
		lLFIndexerOffset(),
		lLFRearTrackOffset(),
		dForceFactorKf2(),
		dForceFactorIo2(),
		chRcdSample(),
		chRcdTol(),
		nTransducerIdleTime(),				//v1.0.T4 add
		nTransducerLengthComp(){}			//v1.0.T4 add, v1.0.T6 edit
};

struct MACHINE_DATA_LIGHT
{
    LIGHT_PARA  DieLightRcd;
    LIGHT_PARA  LeadLightRcd;
	LIGHT_PARA  PrDieRcd;
	LIGHT_PARA  PrLeadRcd;
	LIGHT_PARA  stIdleLight;
    char        cReserve[52];

    //must init before use, or by default as 0
	MACHINE_DATA_LIGHT():
		DieLightRcd(),
		LeadLightRcd(),
		PrDieRcd(),
		PrLeadRcd(),
		stIdleLight(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

//#define MACHINE_DATA_VER 33		//add 3-pt force calibration
//#define MACHINE_DATA_VER 34		//add robust contact detect
//#define MACHINE_DATA_VER 35		//v1.0.T4 - add transducer position compensation
#define MACHINE_DATA_VER 36		//v1.1.T1 - split dPwrCompOffset to fPwrCompOs1/2

// CMachineData command target

class CMachineData : public CObject
{
public:
	CMachineData();
	virtual ~CMachineData();
public:	//functions
	void VerifyData();						//v1.1.T15 add
	bool IsZeroBtosOptics(void);			//v1.1.T10 add
	long GetFlatLevel(void);				//v1.1.T9 add
	long GetRelaxLevel(void);				//v1.1.T7 add
	long GetResetLevel(void);				//v1.1.T6 add
	long GetIndexerLimit(void);				//v1.1.T5 add
	TBL_POS GetXYStandbyPos(void);			//v1.1.T3 add
	long GetZeroOffset(void);				//v1.1.T2 add
	float GetPwrCompOs2(void);				//v1.1.T1 add
	float GetPwrCompOs1(void);				//v1.1.T1 add
	//double GetPwrCompOffset(void);		//v1.0.T8 add, v1.1.T1 delete
	long GetPwrCompOffsetCapiCount(void);	//v1.0.T8 add
	long GetPwrCompEndCapiCount(void);		//v1.0.T8 add
	long GetCapillaryLimit(void);			//v1.0.T7 add
	char GetUphMode(void);					//v1.0.T7 add
	short GetTransducerIdleTime(void);		//v1.0.T5 add
	short GetTransducerLengthComp(void);	//v1.0.T5 add, v1.0.T6 edit
	void SaveMachineData(void);
    // Load machine dependent data from saving buffer
    int LoadMachineData(void);
	MACHINE_DATA* GetRcd(void);				//v1.1.T11 add
	double Calc2PtBondForceDac(short nForce, long lCtactLvl);
	double Calc3PtBondForceDac(short nForce, long lCtactLvl);
    // Convert wire clamp force to DAC in %
    double ConvertWclForceToDac(short nForce);
    MACHINE_DATA_LIGHT* GetLightRcd(void);	//v1.1.T12 add
    int GetDefaultMachineLightRcd(MACHINE_DATA_LIGHT *pMachineLight);
	TBL_POS GetBondTipOs(void);
	short GetLocateMode(void);
private:	//variables
    MACHINE_DATA m_stMcData;	//v1.1.T4 edit
    MACHINE_DATA_LIGHT m_stMcLight;		//v1.1.T4 edit
};

extern CMachineData theMcData;	//v1.1.T13 edit
