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
	if ( state == STATE_NOTINIT)
		stateText = "Not Init";
	else if ( state == STATE_OFF)
		stateText = "Off";
	else if ( state == STATE_ON)
		stateText = "On";
	else if ( state == STATE_FAULT)
		stateText = "Fault";
	else if ( state == STATE_STARTING)
		stateText = "Starting";
	else if ( state == STATE_STOPPING)
		stateText = "Stopping";
	else if ( state == STATE_ERROR)
		stateText = "Error";
	else if ( state == STATE_UNKNOWN)
		stateText = "Unknown";
	else 
		stateText = "Unknown state:" + String(state);
	
	return stateText;
	}

