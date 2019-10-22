#ifndef Unit_h
	#define Unit_h
	
	
	#define LL_NONE		0
	#define LL_HIGH		1
	#define LL_NORMAL	2
	#define LL_LOW		3

	#define LOGLEVEL LL_NORMAL

    #ifndef LOGLEVEL
		#define LOGLEVEL LL_NORMAL
	#endif

	#include "Arduino.h"
	#include <Wire.h>
	
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
					UT_SYSTEM
					};	

	//-------------------------------
	enum TurnOnUnitAction {
					TON_UA_UNKNOWN = 1, 
					TON_UA_NONE = 2, 
					TON_UA_ON = 3, 
					TON_UA_ERROR = -1, 
					TON_UA_ERROR_01 = -2, 
					TON_UA_ERROR_02 = -3, 
					TON_UA_ERROR_03 = -4, 
					TON_UA_ERROR_04 = -5 
					};	

	//-------------------------------
	enum TurnOffUnitAction {
					TOFF_UA_UNKNOWN = 1, 
					TOFF_UA_NONE = 2, 
					TOFF_UA_OFF = 3, 
					TOFF_UA_ERROR = -1, 
					TOFF_UA_ERROR_01 = -2, 
					TOFF_UA_ERROR_02 = -3, 
					TOFF_UA_ERROR_03 = -4, 
					TOFF_UA_ERROR_04 = -5 
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
		void		LogText(String);
		void		LogTextLn(String);
		void		LogText(bool);
		void		LogTextLn(bool);
		void 		LogText(int x);
		void 		LogTextLn(int x);
		void 		LogLn();
		void 		LogInfo();
		
		UnitInfo 	GetInfo();
		String		GetUnitTypeText();
		String 		GetLogicTypeText();

	protected:
		String			_title;
		UnitType		_type;

	private: 
	};
	    
#endif

