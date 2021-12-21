#pragma once

#define	DISPLAY_IMG_WIDTH       512
#define DISPLAY_IMG_HEIGHT      384

//#define	ZOOMED_IMG_WIDTH        691  //*1.35
#define	ZOOMED_IMG_WIDTH        690  //*1.347656
#define ZOOMED_IMG_HEIGHT       518  //*1.348958

#define MAX_VISION_PATTERN       50

#define PATTERN_MASK_BRUSH_SIZE  3

#define OLD_WORK_SPACE_ROOT     "C:\\WBDATA\\"
#define WORK_SPACE_ROOT         "D:\\WBDATA\\"
#define WORK_DIRECTION          "D:\\WBDATA\\VISION\\"
#define RECIPE_DIRECTION        "D:\\RecipeDir\\"

typedef enum VISION_ERROR
{
    VISION_NO_ERROR,
    VISION_MALLOC_APP_ERROR,
    VISION_MALLOC_SYS_ERROR,
    VISION_CAMERA_NUM_ERROR,
    VISION_MALLOC_DISP_ERROR,
    VISION_MALLOC_DIGITIZER_ERROR,
    VISION_MALLOC_IMAGE_ERROR,
    VISION_MALLOC_OVERLAY_ERROR,
    VISION_MALLOC_PATTERN_RESULT_ERROR,
    VISION_OPEN_FILE_FAIL,
    VISION_PATTERN_RESTORE_FAIL,
    VISION_PATTERN_MISS_MATCH,
} _VISION_ERROR;

typedef enum VISION_CLIENT
{
    VISION_BONDER,
    VISION_WH,
} _VISION_CLIENT;


typedef enum VISION_CLEAR_TYPE
{
    VISION_CLEAR_ALL =0,
    VISION_CLEAR_BOND_PR,
	VISION_CLEAR_INDEX_PR,

}_VISION_CLEAR_TYPE;


#pragma pack(push, 1)
typedef struct PATTERN_RECORD    //@1
{
    bool bValid;
    long lPatternId;
	bool bIndexPr;

} _PATTERN_RECORD;

typedef struct SEARCH_PATTERN_RESULT
{
    double dOffsetX;
    double dOffsetY;
    double dRotateAngle;
    double dScore;
} _SEARCH_PATTERN_RESULT;
#pragma pack(pop)
