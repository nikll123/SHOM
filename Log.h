/*
Calss Log - static class for SHOM log
*/
#ifndef Log_h
	#define Log_h

	#include "Arduino.h"

    #define LOG_BUFFER_LENGHT 100

/*
	#define LL_NONE		0
	#define LL_HIGH		1
	#define LL_NORMAL	2
	#define LL_LOW		3

    #ifndef LOGLEVEL
//		#define LOGLEVEL LL_HIGH
		#define LOGLEVEL LL_NORMAL
	#endif
	#define PortMonitorLog 1
*/


	static char		_logBuffer[LOG_BUFFER_LENGHT];

	//-------------------------------
	class Log
	{
	public:

		static void 	BufClear();
		static void 	BufCat(char *str);
		static void 	BufCat(byte x);
		static void 	BufCat(int x);
		static void 	BufCat(unsigned int x);
		static void 	BufPrint();
        
	protected:

	private: 
	};
	    
#endif

