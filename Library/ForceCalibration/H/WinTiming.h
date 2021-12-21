
// 
#ifndef _WIN_TIMING_
#define _WIN_TIMING_


#define COUNT_PER_SEC_FOR_10US       (100000)
#define COUNT_PER_SEC_FOR_100US       (10000)

extern void init_win_timing();
extern void exit_win_timing();
void high_precision_sleep_ms(unsigned int uiWait_ms);
unsigned int uint_round_double(double dIn);
bool GetPentiumClockEstimateFromRegistry(unsigned __int64 *frequency);

double GetPentiumTimeAsDouble(unsigned __int64 frequency);

int GetPentiumTimeCount_per_10us(unsigned __int64 frequency);

int GetPentiumTimeCount_per_100us(unsigned __int64 frequency);

int GetPentiumTimeCount_per_1s(unsigned __int64 frequency);

unsigned int mtn_cto_get_start_cnt();

unsigned int mtn_cto_get_curr_cnt();

void mtn_cto_tick_start_time_u1s(LARGE_INTEGER liFreqOperSys);

bool mtn_cto_is_time_out_u1s(LARGE_INTEGER liFreqOperSys, unsigned int uiTimeOut_u1s);

void mtn_cto_tick_start_time_u100us(LARGE_INTEGER liFreqOperSys);

bool mtn_cto_is_time_out(LARGE_INTEGER liFreqOperSys, unsigned int uiTimeOut_u100us);

#endif  // _WIN_TIMING_