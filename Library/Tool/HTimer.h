//HTimer.h v1.0.T3

#ifndef CHTIMER_H_2009_9_12_1678935
#define CHTIMER_H_2009_9_12_1678935

#include <iostream>
#include <windows.h>

//peek->translate->dispatch message
extern void ProcessMessage();
//sleep with process message
extern void Sleep_x1ms(DWORD lTime);					//v1.0.T2 edit
//sleep with process message
extern void SleepEx_x1ms(LPDWORD plTime);				//v1.0.T2 edit
//wait with process message
extern void WaitThread(HANDLE hThread, DWORD lTime);	//v1.0.T2 edit

class CHTimer		//high performance timer
{
public:
	CHTimer();		//this constructor will call StartTimer() before quit
					/* sample code:
					CHTimer timer;			//call StartTimer to set m_lStartTime
					...
					timer.StartTimer();		//set m_lStartTime again (optional)
					...
					double dDuration = timer.GetTime_x1ms()
					//GetTime_x1ms will set the m_lEndTime, and
					//return the duration in ms from m_lStartTime until m_lEndTime
					*/
	~CHTimer();
public:	//functions
	void StartTimer();
	double GetTime_x1us();
	double GetTime_x10us();
	double GetTime_x100us();
	double GetTime_x1ms();
	double GetTime_x10ms();
	double GetTime_x100ms();
	double GetTime_x1s();
	double GetTime_x1min();		//v1.0.T3 add
	LONGLONG GetFreq();
private:	//variables
	LONGLONG m_lStartTime;
	LONGLONG m_lEndTime;
	LONGLONG m_lFreqPerSecond;
};

#endif