#ifndef Core_h
	#define Core_h
	
	#include "Arduino.h"

	#define UNIT_AUTOMAT   	 0
	#define UNIT_CONTACTOR	 1
	
	#define LOGIC_NORMAL     0
	#define LOGIC_INVERSE    1
	
	//#define STATE_UNKNOWN    0
	#define STATE_NOTINIT    255
	#define STATE_OFF   	 0
	#define STATE_ON    	 1
	#define STATE_FAULT 	 2
	#define STATE_STARTING 	 3
	#define STATE_ERROR 	 4
	#define STATE_STOPPING 	 5
	#define STATE_UNKNOWN 	 6
    
	#define PortMonitorLog 1

	#define TURN_ON_UNKNOWN	0
	#define TURN_ON_WAIT	1
	#define TURN_ON_ON		2
	#define TURN_ON_ERROR	3
	#define TURN_ON_NEXT	4
	#define TURN_ON_OFF		5

   	#define TURN_ON_TIMEOUT 2000
   	#define TURN_OFF_TIMEOUT 1000

	
    class Core
	{
	public:
		Core();
		static void LogState(String string, uint8_t state);
		static String GetStateText(uint8_t state);
		static uint8_t TurnOn_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState);
		static uint8_t TurnOff_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState);
	};

    
#endif

