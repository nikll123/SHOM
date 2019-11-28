#ifndef Unit_h
	#define Unit_h

	#include "Arduino.h"
	//#include <Wire.h>
	
	#define LL_NONE		0
	#define LL_HIGH		1
	#define LL_NORMAL	2
	#define LL_LOW		3

    #ifndef LOGLEVEL
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

					//  ShomCanBus
					CBS_ERR401		= 401,
					CBS_ERR402		= 402,
					CBS_ERR403		= 403,
					CBS_ERR404		= 404,
					CBS_ERR405		= 405
					
					};	

	//-------------------------------
	class Unit
	{
	public:
					Unit();
					Unit(String title, UnitType type);
		static void	LogText(String);
		static void	LogTextLn(String);
		static void LogLn();
		void 		Log(String);
		void 		LogErr(UnitError err);
		void 		LogInfo();
		
		UnitInfo 	GetInfo();
		String		GetUnitTypeText();
		String 		GetLogicTypeText();

	protected:
		String			_title;
		UnitType		_type;
		byte			_logLevel;

	private: 
	};
	    
#endif

