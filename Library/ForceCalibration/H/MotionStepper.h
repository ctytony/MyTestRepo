// Module: Motion Stepper
// Template structure is based on ADS-PCI-1240
// History
// YYYYMMDD  Author    Notes
// 

#ifndef __MOTION_STEPPER__
#define __MOTION_STEPPER__

#include "mtnapi.h"

typedef struct
{
	MTN_SPEED_PROFILE astSpeedProfileStepperAxis[MAX_STEPPER_AXIX_ON_BOARD];
	MTN_POSITION_REGISTRATION astPositionRegistration[MAX_STEPPER_AXIX_ON_BOARD];
}STEPPER_SPEED_PROFILE_PER_BOARD;


int mtnstp_upload_parameters_ads1240(BYTE uiCurrBoardId, DWORD *ulErrCode);
int mtnstp_download_parameters_ads1240(BYTE uiCurrBoardId, DWORD *ulErrCode);

int mtnstp_upload_parameters_leetro_2812(BYTE uiCurrBoardId, DWORD *ulErrCode);
int mtnstp_download_parameters_leetro_2812(BYTE uiCurrBoardId, DWORD *ulErrCode);

#define WB_WH_INNER_CLAMPER_STEPPER   0
#define WB_WH_OUTER_CLAMPER_STEPPER   1
#define WB_WH_ONLOADER_STEPPER   2
void mtnstp_wb_set_home_para(unsigned int uiCurrBoardId, unsigned long *ulErrCode);

extern STEPPER_SPEED_PROFILE_PER_BOARD astStepperSpeedProfilePerBoard[];

#endif // __MOTION_STEPPER__