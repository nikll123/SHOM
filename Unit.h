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
					UT_CANBUS,
					UT_SYSTEM
					};	

	//-------------------------------
	struct UnitInfo {
					String 		Title;
					String 		UnitType;
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
		void 		LogErr(String pref, int err);
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

