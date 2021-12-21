
// 20100311, Change to int year
#ifndef __RT_CLOCK_DEF__
#define __RT_CLOCK_DEF__


typedef struct rtc_tm {  
        unsigned int  year; /* xxxx 4 digits :) // 20100311 */                          
        unsigned short   secs;                                                 
        unsigned short   mins;                                                 
        unsigned short   hours;                                                
        unsigned short   mday;                                                 
        unsigned short   mon;   
        unsigned short   wday;                                                 
        unsigned short   vl;     
}RTC_TM;


#endif // __RT_CLOCK_DEF__
