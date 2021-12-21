// 20100301
#pragma once

#include "USG_BLK_DEF.h" // 20100301

void init_usg_blk_seg();
void usg_blk_set_segment(unsigned int uiBlk, unsigned int uiSeg, USG_UNIT_SEG *stpUSG_Seg);
void usg_blk_get_segment(unsigned int uiBlk, unsigned int uiSeg, USG_UNIT_SEG *stpUSG_Seg);
void usg_blk_set_trigger_address(unsigned int uiBlk, int iAddr);
int usg_blk_get_trigger_address(unsigned int uiBlk);
void usg_blk_set_nextblk_index(unsigned int uiBlk, unsigned int uiNextBlk);
unsigned int usg_blk_get_nextblk_index(unsigned int uiBlk);
void usg_blk_set_max_count_wait_trig_prot(unsigned int uiBlk, unsigned int uiWaitCnt);
unsigned int usg_blk_get_max_count_wait_trig_prot(unsigned int uiBlk);
