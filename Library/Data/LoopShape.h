//LoopShape.h v1.0.T64

#pragma once

//v1.0.T17 add
#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct ST_LOOP_SHAPE_PARA	//v1.0.T18 rename
{
    double dLoopHt;				//v1.0.T20 rename
    double dRevHt;				//v1.0.T42 rename
    double dRevDist;			//v1.0.T42 rename
    double dRevAngle;			//v1.0.T42 rename
    double dRevDistHt;			//v1.0.T42 rename
    double dRevDistLen;			//v1.0.T42 rename
    double dRevKinkHt;			//v1.0.T42 rename
    //double dSpanAngle1;		//v1.0.T41 rename, v1.0.T59 delete
	double dRevHt2;				//v1.0.T54 add
	double dRevSpanLen2;		//v1.0.T64 add
    bool   bPreKinkEnable;
    double dPreKinkHt;			//v1.0.T43 rename
    double dPreKinkDist;		//v1.0.T43 rename
    double dPreKinkDistHt;		//v1.0.T43 rename
    double dPreKinkDistLen;		//v1.0.T43 rename
    double dPreKinkKinkHt;		//v1.0.T43 rename
    //double dPreSpanAngle;		//v1.0.T41 rename, v1.0.T59 delete
	double dPreKinkHt2;			//v1.0.T53 add
	double dPreKinkSpanLen2;	//v1.0.T64 add
    bool   bKink2Enable;
    double dKink2SpanLen;		//v1.0.T35 rename, v1.0.T58 rename
    double dKink2Ht;			//v1.0.T44 rename
    double dKink2Dist;			//v1.0.T44 rename
    //double dKink2Angle;		//v1.0.T44 rename, v1.0.T59 delete
    double dKink2DistHt;		//v1.0.T44 rename
    double dKink2DistLen;		//v1.0.T44 rename
    double dKink2KinkHt;		//v1.0.T44 rename
    //double dKink2SpanAngle;	//v1.0.T40 rename, v1.0.T58 rename, v1.0.T59 delete
    double dKink2FlatRad;		//v1.0.T58 rename
	double dKink2Ht2;			//v1.0.T53 add
	double dKink2SpanLen2;		//v1.0.T60 add
    bool   bKink3Enable;
    double dKink3SpanLen;		//v1.0.T36 rename, v1.0.T58 rename
    double dKink3Ht;			//v1.0.T45 rename
    double dKink3Dist;			//v1.0.T45 rename
    //double dKink3Angle;		//v1.0.T45 rename, v1.0.T59 delete
    double dKink3DistHt;		//v1.0.T45 rename
    double dKink3DistLen;		//v1.0.T45 rename
    double dKink3KinkHt;		//v1.0.T45 rename
    //double dKink3SpanAngle;	//v1.0.T39 rename, v1.0.T58 rename, v1.0.T59 delete
    double dKink3FlatRad;		//v1.0.T58 rename
	double dKink3Ht2;			//v1.0.T53 add
	double dKink3SpanLen2;		//v1.0.T60 add
    bool   bTwistEnable;
    double dTwistRevHt;			//v1.0.T37 rename
    double dTwistSpanLen;		//v1.0.T38 rename
    double dTwistRevDist;
    //double dTwistRevAngle;	//v1.0.T59 delete
    double dTwistRevDistHt;		//v1.0.T22 rename
    double dTwistRevDistLen;	//v1.0.T22 rename
    double dTwistKinkHt;		//v1.0.T22 rename
    //double dTwistSpanAngle;	//v1.0.T38 rename, v1.0.T59 delete
    double dTwistFlatRad;		//v1.0.T37 rename, v1.0.T58 rename
    double dTwistFwdDist;
    double dTwistFwdDistHt;		//v1.0.T22 rename
    double dTwistFwdDistLen;	//v1.0.T22 rename
    double dTwistFwdKinkHt;		//v1.0.T22 rename
    double dSlopeLen;			//v1.0.T23 rename
    double dSlopeHt;			//v1.0.T23 rename
    double dCtactRad;			//v1.0.T58 rename
    double dTrajRadius;
    char   cLoopTopMode;		//v1.0.T23 rename
    double dLoopTopHt;			//v1.0.T23 rename
    double dLoopTopLen;			//v1.0.T23 rename
    short  nTrajPoint;
    short  nPullDist;
    short  nTrajFactor;
    char   cTrajMode;			//v1.0.T23 rename
    short  nCtactLvlOffset;
    short  nLAngle;
    short  nWireFeedSpeed;		//v1.0.T32 rename
    short  nWireBentSpeed;		//v1.0.T32 rename
    short  nPreKinkWireFeedSpeed;
    short  nPreKinkWireBentSpeed;
    short  nWireFeedSpeed2;		//v1.0.T32 rename
    short  nWireBentSpeed2;		//v1.0.T32 rename
    short  nWireFeedSpeed3;		//v1.0.T32 rename
    short  nWireBentSpeed3;		//v1.0.T32 rename
    short  nTwistWireFeedSpeed;	//v1.0.T32 rename
    short  nTwistRevBentSpeed;
    short  nTwistFwdBentSpeed;
    short  nTrajSpeed;			//v1.0.T23 rename
    short  nPullSpeed;

    //must init before use, or by default as 0
	ST_LOOP_SHAPE_PARA():
		dLoopHt(),				//v1.0.T20 rename
		dRevHt(),				//v1.0.T42 rename
		dRevDist(),				//v1.0.T42 rename
		dRevAngle(),			//v1.0.T42 rename
		dRevDistHt(),			//v1.0.T42 rename
		dRevDistLen(),			//v1.0.T42 rename
		dRevKinkHt(),			//v1.0.T42 rename
		//dSpanAngle1(),		//v1.0.T41 rename, v1.0.T59 delete
		dRevHt2(),				//v1.0.T54 add
		dRevSpanLen2(),			//v1.0.T64 add
		bPreKinkEnable(),
		dPreKinkHt(),			//v1.0.T43 rename
		dPreKinkDist(),			//v1.0.T43 rename
		dPreKinkDistHt(),		//v1.0.T43 rename
		dPreKinkDistLen(),		//v1.0.T43 rename
		dPreKinkKinkHt(),		//v1.0.T43 rename
		//dPreSpanAngle(),		//v1.0.T41 rename, v1.0.T59 delete
		dPreKinkHt2(),			//v1.0.T53 add
		dPreKinkSpanLen2(),		//v1.0.T64 add
		bKink2Enable(),
		dKink2SpanLen(),		//v1.0.T35 rename, v1.0.T58 rename
		dKink2Ht(),				//v1.0.T44 rename
		dKink2Dist(),			//v1.0.T44 rename
		//dKink2Angle(),		//v1.0.T44 rename, v1.0.T59 delete
		dKink2DistHt(),			//v1.0.T44 rename
		dKink2DistLen(),		//v1.0.T44 rename
		dKink2KinkHt(),			//v1.0.T44 rename
		//dKink2SpanAngle(),	//v1.0.T40 rename, v1.0.T58 rename, v1.0.T59 delete
		dKink2FlatRad(),		//v1.0.T58 rename
		dKink2Ht2(),			//v1.0.T53 add
		dKink2SpanLen2(),		//v1.0.T60 add
		bKink3Enable(),
		dKink3SpanLen(),		//v1.0.T36 rename, v1.0.T58 rename
		dKink3Ht(),				//v1.0.T45 rename
		dKink3Dist(),			//v1.0.T45 rename
		//dKink3Angle(),		//v1.0.T45 rename, v1.0.T59 delete
		dKink3DistHt(),			//v1.0.T45 rename
		dKink3DistLen(),		//v1.0.T45 rename
		dKink3KinkHt(),			//v1.0.T45 rename
		//dKink3SpanAngle(),	//v1.0.T39 rename, v1.0.T58 rename, v1.0.T59 delete
		dKink3FlatRad(),		//v1.0.T58 rename
		dKink3Ht2(),			//v1.0.T53 add
		dKink3SpanLen2(),		//v1.0.T60 add
		bTwistEnable(),
		dTwistRevHt(),			//v1.0.T37 rename
		dTwistSpanLen(),		//v1.0.T38 rename
		dTwistRevDist(),
		//dTwistRevAngle(),		//v1.0.T59 delete
		dTwistRevDistHt(),		//v1.0.T22 rename
		dTwistRevDistLen(),		//v1.0.T22 rename
		dTwistKinkHt(),			//v1.0.T22 rename
		//dTwistSpanAngle(),	//v1.0.T38 rename, v1.0.T59 delete
		dTwistFlatRad(),		//v1.0.T37 rename, v1.0.T58 rename
		dTwistFwdDist(),
		dTwistFwdDistHt(),		//v1.0.T22 rename
		dTwistFwdDistLen(),		//v1.0.T22 rename
		dTwistFwdKinkHt(),		//v1.0.T22 rename
		dSlopeLen(),			//v1.0.T23 rename
		dSlopeHt(),				//v1.0.T23 rename
		dCtactRad(),			//v1.0.T58 rename
		dTrajRadius(),
		cLoopTopMode(),			//v1.0.T23 rename
		dLoopTopHt(),			//v1.0.T23 rename
		dLoopTopLen(),			//v1.0.T23 rename
		nTrajPoint(),
		nPullDist(),
		nTrajFactor(),
		cTrajMode(),			//v1.0.T23 rename
		nCtactLvlOffset(),
		nLAngle(),
		nWireFeedSpeed(),		//v1.0.T32 rename
		nWireBentSpeed(),		//v1.0.T32 rename
		nPreKinkWireFeedSpeed(),
		nPreKinkWireBentSpeed(),
		nWireFeedSpeed2(),		//v1.0.T32 rename
		nWireBentSpeed2(),		//v1.0.T32 rename
		nWireFeedSpeed3(),		//v1.0.T32 rename
		nWireBentSpeed3(),		//v1.0.T32 rename
		nTwistWireFeedSpeed(),	//v1.0.T32 rename
		nTwistRevBentSpeed(),
		nTwistFwdBentSpeed(),
		nTrajSpeed(),			//v1.0.T23 rename
		nPullSpeed(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

// CLoopShape command target

class CLoopShape : public CObject
{
public:
	CLoopShape();
	virtual ~CLoopShape();
private:	//functions
	int CalcLoopSpanAngle(double dLoopHt, double dPreKinkLen,	//v1.0.T5 rename
		double dPreKinkHt, double dSpanLen, short nKinkHtFactor, 
		double dCurKinkRad, double dNextKinkRad, double *pdSpanRad);
	int CalcKink(double dSpanLen, double dSpanRad, short nKinkHtFactor,	//v1.0.T58 edit
		double dLoopHt, double dPreKinkLen, double dPreKinkHt,
		double *pdRevHt, double *pdRevDist, double *pdRevRad,
		double *pdRevDistHt, double *pdRevDistLen, double *pdKinkHt);
	int CalcKinkEx(double dSpanLen, short nKinkHtFactor,	//v1.0.T47 add, v1.0.T49 edit
		double dLoopHt, short nKinkHtPercent, double *pdRevHt, double *pdRevDist,
		double *pdRevDistHt, double *pdRevDistLen, double *pdKinkHt);
	int CalcKinkFlatAngle(double dWireLen, double dCtactDiff, short nLoopMode,
		ST_LOOP_PARA *pstLoopPara, ST_LOOP_SHAPE_PARA *pstLoopShape);	//v1.0.T9 edit, v1.0.T60 edit
public:	//functions
	int CalcLoopShape(double dWireLen, double dCtactLvl1, double dCtactLvl2,	//v1.0.T6 rename
		short nLoopMode, short nWireLenCorrect, ST_LOOP_SHAPE_PARA *pstLoopShape);
	int CalcLoopShapeEx(double dWireLen, double dCtactLvl1, double dCtactLvl2,	//v1.0.T47 add
		short nLoopMode, short nWireLenCorrect, ST_LOOP_SHAPE_PARA *pstLoopShape);
	int CalcLoopProfileDisplay(double dCtactDiff, ST_LOOP_SHAPE_PARA *pstLoopShape,
		LOOP_PROFILE_DISP_PARA *pstLoopDisp);	//v1.0.T8 rename
};

extern CLoopShape theLoopShape;