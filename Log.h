/*
Calss Log - static class for SHOM log
*/
#ifndef Log_h
	#define Log_h
	#include "Arduino.h"
    #define LOG_BUFFER_LENGHT 100

	//-------------------------------
	enum UnitError {
					// Contactor
					CS_ERR101		= 101,
					CS_ERR102		= 102,
					CS_ERR103		= 103,
					CS_ERR104		= 104,
					CS_ERR105		= 105,
					CS_ERR106		= 106,
					CS_ERR107		= 107,
					CS_ERR108		= 108,
					CS_ERR109		= 109,
					CS_ERR110		= 110,
					
					// Conveyor
					US_ERR201		= 201,
					US_ERR202		= 202,
					US_ERR203		= 203,
					US_ERR204		= 204,
					US_ERR205		= 205,
					US_ERR206		= 206,
					US_ERR207		= 207,
					US_ERR208		= 208,
					US_ERR209		= 209,
					US_ERR210		= 210,
					
					// system
					SS_ERR301		= 301,
					SS_ERR302		= 302,
					SS_ERR303		= 303,
					SS_ERR304		= 304,
					SS_ERR305		= 305,
					SS_ERR306		= 306,
					SS_ERR307		= 307,
					SS_ERR308		= 308,
					SS_ERR309		= 309,
					SS_ERR310		= 310,
					SS_ERR311		= 311,
					SS_ERR312		= 312,
					SS_ERR313		= 313,
					SS_ERR314		= 314,
					SS_ERR315		= 315,
					SS_ERR316		= 316,

					//  ShomCanBus
					CBS_ERR401		= 401,
					CBS_ERR402		= 402,
					CBS_ERR403		= 403,
					CBS_ERR404		= 404,
					CBS_ERR405		= 405,
					
					// Pin
					KS_ERR501		= 501,
					KS_ERR502		= 502,
					KS_ERR503		= 503,
					KS_ERR504		= 504,
					KS_ERR505		= 505,
					KS_ERR506		= 506,
					KS_ERR507		= 507,
					
					// Slave
					SL_ERR601		= 601,
					SL_ERR602		= 602,
					SL_ERR603		= 603,
					SL_ERR604		= 604,
					SL_ERR605		= 605,
					};	

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
		static void 	BufCat(UnitError x);
		static void 	BufPrint();
        
	protected:

	private: 
	};
	    
#endif

