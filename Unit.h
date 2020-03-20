/*
Calss Unit - basic class for all the rest classes of the library SOHM
The most common andgeneral functionality is implemented here  
*/
#ifndef Unit_h
	#define Unit_h

	#include "Arduino.h"
	#include <Log.h>

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
						Unit(const char * title, UnitType type);
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
		const char 		*_title;
		UnitType		_type;
		byte			_logLevel = LL_NORMAL;
		unsigned long 	_millsCheck = 0;

	private: 
	};
	    
#endif

