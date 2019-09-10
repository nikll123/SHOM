#ifndef Core_h
	#define Core_h
	
	#include "Arduino.h"
	#include <Wire.h>
	#include <DS1307.h>

	#define LOGIC_NORMAL     0
	#define LOGIC_INVERSE    1

	//#define PortMonitorLog 0
	
	#define UZEL_TURN_ON_TIMEOUT 2000
	#define UZEL_TURN_OFF_TIMEOUT 1000

	//-------------------------------
	enum UzelType  {UT_NONE,
					UT_AUTOMAT, 
					UT_CONTACTOR
					};	
		
	//-------------------------------
	enum LogicType {LT_NONE,
					LT_NORMAL, 
					LT_INVERSE
					};	

	//-------------------------------
	enum UzelState {US_NOTINIT, 
					US_ON, 
					US_OFF, 
					US_FAULT, 
					US_STARTING, 
					US_STOPPING, 
					US_UNKNOWN, 
					US_ERROR,
					US_ERROR_01,
					US_ERROR_02,
					US_ERROR_03,
					US_ERROR_04,
					US_ERROR_05
					};	

	//-------------------------------
	enum TurnOnUzelAction {
					TON_UA_UNKNOWN, 
					TON_UA_NONE, 
					TON_UA_ON, 
					TON_UA_ERROR_01, 
					TON_UA_ERROR_02, 
					TON_UA_ERROR_03, 
					TON_UA_ERROR_04, 
					TON_UA_ERROR 
					};	

	//-------------------------------
	enum TurnOffUzelAction {
					TOFF_UA_UNKNOWN, 
					TOFF_UA_NONE, 
					TOFF_UA_OFF, 
					TOFF_UA_ERROR_01, 
					TOFF_UA_ERROR_02, 
					TOFF_UA_ERROR_03, 
					TOFF_UA_ERROR_04 
					};	

   	//-------------------------------
	enum ConveyorState {CS_NOTINIT, 
						CS_ON, 
						CS_OFF, 
						CS_FAULT, 
						CS_STARTING, 
						CS_STOPPING, 
						CS_UNKNOWN, 
						CS_ERROR_01,
						CS_ERROR_02,
						CS_ERROR
						};	

	//-------------------------------
	enum KeyState {KS_NONE, 
					KS_ON, 
					KS_OFF,
					KS_FRONT,
					KS_BACK
					};	
		
	//-------------------------------
	class Core
	{
	public:
		Core();
		
		static String GetUzelStateText(UzelState state);
		static String GetUzelTypeText(UzelType ut);
		static String GetLogicTypeText(LogicType lt);
		static String GetKeyStateText(KeyState as);
		static String GetConveyorStateText(ConveyorState cs);
		static void   LogIntVal(String txt, int val);
		static void   LogText(String txt);
	};

    
#endif

