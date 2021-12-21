
#ifndef __ROYA_DEF_H__
#define __ROYA_DEF_H__

#define ADC_2_BYTE

#ifdef  ADC_2_BYTE
typedef unsigned short ADC_TYPE;
#else
typedef unsigned int ADC_TYPE;
#endif

typedef struct
{
unsigned int  uiCounterKeyInt;
unsigned int  uiCounterKeyBSD;
unsigned int  uiWireFeedSensorTriggerCounter;
}KEY_INTERRUPT_COUNTER;

typedef struct
{
    unsigned int nCycleISR;
	unsigned int nRefSignalCycle;
	double dFreqISR_Hz;
}SIG_GEN_SINE_OUT_CFG;

#define ROYA_BSD_ADC_2_BYTE    0
#define ROYA_BSD_ADC_4_BYTE    1

#define ROYA_BSD_DAC_10b       10
#define ROYA_BSD_DAC_12b       12

#define ROYA_BSD_YEAR_2_BYTE   0
#define ROYA_BSD_YEAR_4_BYTE   1

#define ROYA_BSD_ADC_POLARITY_SINGLE   0
#define ROYA_BSD_ADC_BI_POLAR          1

#define DAC_OFFSET_10BIT    512
#define DAC_OFFSET_12BIT    2048
#define DAC_SIGNED_MAX_10BIT  (DAC_OFFSET_10BIT - 1)
#define DAC_SIGNED_MAX_12BIT  (DAC_OFFSET_12BIT - 1)

#endif // __ROYA_DEF_H__

