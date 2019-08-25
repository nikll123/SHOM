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

//------------------------------
	static uint8_t Core::TurnOn_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState)
	{
	uint8_t res = TURN_ON_UNKNOWN; 
	if (prevUnitState == STATE_OFF && currUnitState == STATE_ON)
		{
		res = TURN_ON_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev OFF && curr ON; ");
#endif
		}
	else if (prevUnitState == STATE_OFF && currUnitState == STATE_STARTING)
		{
		res = TURN_ON_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev OFF && curr STARTING; ");
#endif
		}
	else if (prevUnitState == STATE_ERROR)
		{
		res = TURN_ON_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev ERROR; ");
#endif
		}
	else if (currUnitState == STATE_ERROR)
		{
		res = TURN_ON_ERROR;
#ifdef PortMonitorLog
	Serial.print("curr ERROR; ");
#endif
		}
	
	if (res != TURN_ON_ERROR)
		{
		if (prevUnitState == STATE_ON && currUnitState == STATE_OFF)
          	{
			res = TURN_ON_ON;
          	}
		else if (prevUnitState == STATE_ON && currUnitState == STATE_STARTING)
			{
			res = TURN_ON_WAIT;
			}
		else if (prevUnitState == STATE_ON && currUnitState == STATE_ON)
			{
			res = TURN_ON_NEXT;
			}
		else if (prevUnitState == STATE_STARTING && currUnitState == STATE_OFF)
			{
			res = TURN_ON_NEXT;
			}
		else if (prevUnitState == STATE_OFF && currUnitState == STATE_OFF)
			{
			res = TURN_ON_NEXT;
			}
		}
	
	return res;
	}

//------------------------------
	static uint8_t Core::TurnOff_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState)
	{
	uint8_t res = TURN_ON_UNKNOWN; 
	if (prevUnitState == STATE_ON)
		{
		res = TURN_ON_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev ON; ");
#endif
		}
	else if (prevUnitState == STATE_ERROR)
		{
		res = TURN_ON_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev ERROR; ");
#endif
		}
	else if (currUnitState == STATE_ERROR)
		{
		res = TURN_ON_ERROR;
#ifdef PortMonitorLog
	Serial.print("curr ERROR; ");
#endif
		}
	
	if (res != TURN_ON_ERROR)
		{
		if (prevUnitState == STATE_OFF && currUnitState == STATE_ON)
          	{
			res = TURN_ON_OFF;
          	}
		else if (prevUnitState == STATE_OFF && currUnitState == STATE_STARTING)
			{
			res = TURN_ON_WAIT;
			}
		else if (prevUnitState == STATE_OFF && currUnitState == STATE_OFF)
			{
			res = TURN_ON_NEXT;
			}
		else if (prevUnitState == STATE_STARTING && currUnitState == STATE_ON)
			{
			res = TURN_ON_NEXT;
			}
		else if (prevUnitState == STATE_ON && currUnitState == STATE_ON)
			{
			res = TURN_ON_NEXT;
			}
		}
	
	return res;
	}	