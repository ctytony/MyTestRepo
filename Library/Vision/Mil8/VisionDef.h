//VisionDef.h v1.8.T19

#pragma once

#define	DISPLAY_IMG_WIDTH		512
#define DISPLAY_IMG_HEIGHT		384

#define	DAHENG_IMG_WIDTH		1280	//v1.8.L56 add
#define DAHENG_IMG_HEIGHT		1024	//v1.8.L56 add

//#define	ZOOMED_IMG_WIDTH	691  //*1.35
#define	ZOOMED_IMG_WIDTH		780  //*1.347656	//V7.2 edit
#define ZOOMED_IMG_HEIGHT		586  //*1.348958	//V7.3 edit

#define MAX_VISION_PATTERN		50
#define PATTERN_MASK_BRUSH_SIZE	3
#define DIR_VISION				"D:\\WBDATA\\VISION\\"			//v1.7.T15 edit
#define DIR_VISION_IMAGE		"D:\\WBDATA\\VISION\\IMAGE\\"	//v1.8.T19 add

enum EN_VISION_ERROR	//v1.7.T29 edit
{
    VISION_OK,
    VISERR_MALLOC_APP,
    VISERR_MALLOC_SYS,
    VISERR_CAMERA_NUM,
    VISERR_MALLOC_DISPLAY,
    VISERR_MALLOC_DIGITIZER,
    VISERR_MALLOC_IMAGE,
    VISERR_MALLOC_OVERLAY,
    VISERR_MALLOC_PATTERN_RESULT,
    VISERR_OPEN_FILE,
    VISERR_PATTERN_RESTORE,
    VISERR_PATTERN_MISMATCH,
	VISERR_INIT_CAMERA,			//v1.7.T28 add
};

enum EN_VISION_CLEAR_TYPE
{
    VISION_CLEAR_ALL = 0,
    VISION_CLEAR_BOND_PR,
	VISION_CLEAR_INDEX_PR,
};

#pragma pack(push, 1)
struct ST_PATTERN_RECORD	//v1.8.T8 rename
{
    bool bValid;
    long lPatID;	//v1.8.T8 rename
	bool bIndexPR;	//v1.8.T8 rename

	//must init before use, or by default as 0
	ST_PATTERN_RECORD():
		bValid(),
		lPatID(),
		bIndexPR(){}
};

struct ST_SEARCH_PATTERN_RESULT	//v1.8.T13 rename
{
    double dOffsetX;
    double dOffsetY;
    double dRotateAngle;
    double dScore;

	//must init before use, or by default as 0
	ST_SEARCH_PATTERN_RESULT():
		dOffsetX(),
		dOffsetY(),
		dRotateAngle(),
		dScore(){}
};
#pragma pack(pop)
