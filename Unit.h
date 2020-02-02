/*
Calss Unit - basic class for all the rest classes of the library SOHM
The most common andgeneral functionality is implemented here  
*/
#ifndef Unit_h
	#define Unit_h

	#include "Arduino.h"
	//#include <Wire.h>

    #define MAX_UNIT_NUMBER 15

	#define RELAY_DELAY	50 // --10

	#define LL_NONE		0
	#define LL_HIGH		1
	#define LL_NORMAL	2
	#define LL_LOW		3

    #ifndef LOGLEVEL
//		#define LOGLEVEL LL_HIGH
		#define LOGLEVEL LL_NORMAL
	#endif

	#define PortMonitorLog 1

	//-------------------------------
	enum UnitType  {UT_NONE,
					UT_PIN,
					UT_PININ,
					UT_PINOUT,
					UT_LED,
					UT_BUTTONLED,
					UT_AUTOMAT, 
					UT_CONTACTOR,
					UT_CONVEYOR,
					UT_CONVEYORHANDLER,
					UT_CANBUS,
					UT_SYSTEM,
					UT_TIMER,
					};	

	//-------------------------------
	struct UnitInfo {
					String 		Title;
					String 		UnitType;
					};	

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
					SL_ERR602		= 601,
					SL_ERR603		= 601,
					SL_ERR604		= 601,
					SL_ERR605		= 601,
					};	

	//-------------------------------
	enum TimeAct  {	TA_NONE,
					TA_FIX,
					TA_RESET,
					TA_GET,
					TA_PERIOD,
					};	
	//-------------------------------
	class Unit
	{
	public:
						Unit();
						Unit(String title, UnitType type);
		static void		LogText(String);
		static void		LogTextLn(String);
		static void 	LogLn();
		void 			Log(String str);
		void 			Log(String str, byte _logLevel);
		void 			LogErr(UnitError err);
		void 			LogInfo();
		unsigned long	Time(TimeAct ta);
		
		UnitInfo 		GetInfo();
		String			UnitTypeText();
		String 			LogicTypeText();

	protected:
		String			_title;
		UnitType		_type;
		byte			_logLevel = LL_NORMAL;
		unsigned long 	_millsCheck = 0;

	private: 
	};
	    
#endif

