/*
Calss Unit - basic class for all the rest classes of the library SOHM
The most common andgeneral functionality is implemented here  
*/
#ifndef Unit_h
	#define Unit_h

	#include "Arduino.h"
	#include <Log.h>

    #define MAXSTRLEN 100
    #define PIN_REMOTE_BASE 100
    #define MAX_UNIT_NUMBER 15

	#define RELAY_DELAY	50 // --10

	//-------------------------------
	enum UnitType  {UT_NONE = 1,
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
		void 			LogErr(UnitError err);
		void 			LogInfo();
		unsigned long	Time(TimeAct ta);
		
		const char *	GetTitle();
		UnitType		_type;
		const char *	UnitTypeText();

	protected:
		const char		*_title;
		unsigned long 	_millsCheck = 0;

	private: 
	};
	    
#endif

