#include "Arduino.h"
#include "Core.h"
	
	Core::Core();

// -----------------------------
static void Core::LogState(String string, uint8_t state)
{
	Serial.print(string);
	Serial.print(": ");
	Serial.println(state);
}
	  
//------------------------------
	static String Core::GetStateText(uint8_t state)
	{
	String stateText = "";
	if ( state == NOTINIT)
		stateText = "Not Init";
	else if ( state == OFF)
		stateText = "Off";
	else if ( state == ON)
		stateText = "On";
	else if ( state == FAULT)
		stateText = "Fault";
	else if ( state == STARTING)
		stateText = "Starting";
	else if ( state == STOPPING)
		stateText = "Stopping";
	else if ( state == ERROR)
		stateText = "Error";
	else if ( state == UNKNOWN)
		stateText = "Unknown";
	else 
		stateText = "Unknown state:" + String(state);
	
	return stateText;
	}

