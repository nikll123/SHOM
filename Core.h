#ifndef Core_h
	#define Core_h
	
	#include "Arduino.h"
	#include <Wire.h>
	#include <DS1307.h>

	#define LOGIC_NORMAL     0
	#define LOGIC_INVERSE    1

	#define PortMonitorLog 0
	
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
	enum UzelState {US_NOTINIT = 1, 
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
					US_ERROR_04 = -5
					};	

	//-------------------------------
	struct UzelState2 {
					UzelState ValueOld;
					UzelState ValueNew;
					};	

	//-------------------------------
	enum TurnOnUzelAction {
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
	enum TurnOffUzelAction {
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
	enum ConveyorState {CS_NOTINIT = 1, 
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
					 ConveyorState ValueOld; 
					 ConveyorState ValueNew; 
					};	

   	//-------------------------------
	enum KeyState {KS_NONE, 
					KS_ON, 
					KS_OFF,
					KS_FRONT,
					KS_BACK
					};	

	//-------------------------------
	struct KeyState2 {
					KeyState ValueOld;
					KeyState ValueNew;
					};	
	//-------------------------------
	struct KeyInfo {
					KeyState Value;
					String Title;
					String StateText;
					int Pin;
					String Logic;
					};	

	//-------------------------------
	class Core
	{
	public:
		Core();
		
		static String GetUzelStateText(UzelState);
		static String GetUzelTypeText(UzelType);
		static String GetLogicTypeText(LogicType);
		static String GetKeyStateText(KeyState);
		static String GetConveyorStateText(ConveyorState);
		static String GetDateTime();
		static String GetDate();
		static String GetTime();
		static void   LogIntVal(String, int);
		static void   LogText(String);
		static void   LogTextLn(String);
		static void   LogText(String, bool);

	private: 
			
	};

    
#endif

