#ifndef Unit_h
	#define Unit_h
	
	#ifndef LOGLEVEL
		#define LOGLEVEL 2
	#endif
	
	#define LL_NONE		0
	#define LL_MIN		1
	#define LL_NORMAL	2
	#define LL_MAX		3

	#include "Arduino.h"
	#include <Wire.h>
	#include <DS1307.h>

	#define PortMonitorLog 1
	
	#define UNIT_TURN_ON_TIMEOUT 2000
	#define UNIT_TURN_OFF_TIMEOUT 1000

	//-------------------------------
	enum UnitType  {UT_NONE,
					UT_BUTTON,
					UT_LED,
					UT_AUTOMAT, 
					UT_CONTACTOR
					};	

	//-------------------------------
	enum UnitState {US_NOTINIT = 1, 
					US_ON = 2, 
					US_OFF = 3, 
					US_FAULT = 4, 
					US_STARTING = 5, 
					US_STOPPING = 6, 
					US_UNKNOWN = 7, 
					US_ERROR = -1,
					US_ERROR_01 = -2,
					US_ERROR_02 = -3,
					US_ERROR_03 = -4,
					US_ERROR_04 = -5,
					US_ERROR_05 = -6
					};	

	//-------------------------------
	struct UnitState2 {
					UnitState Old;
					UnitState New;
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
	/*enum ConveyorState {CS_NOTINIT = 1, 
						CS_ON = 2, 
						CS_OFF = 3, 
						CS_FAULT = 4, 
						CS_STARTING = 5, 
						CS_STOPPING = 6, 
						CS_UNKNOWN = 7 , 
						CS_ERROR = -1,
						CS_ERROR_01 = -2, 
						CS_ERROR_02 = -3
						};

   	//-------------------------------
	struct ConveyorState2 
					{
					 ConveyorState Old; 
					 ConveyorState New; 
					};	      */


	//-------------------------------
	struct UnitInfo {
					String 		Title;
					String 		UnitType;
					};	

	//-------------------------------
	class Unit
	{
	public:
					Unit(String title, UnitType type);
		void		LogText(String);
		void		LogTextLn(String);
		void 		LogText(String txt, bool ln);
		UnitInfo 	GetInfo();
		String		GetUnitTypeText();

	protected:
		String			_title;
		UnitType		_type;

	private: 
	};
	    
#endif

