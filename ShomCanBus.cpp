#include "ShomCanBus.h"

// =========   CONSTRUCTORS   =========

ShomCanBus::ShomCanBus()
	{
	canbus =MCP_CAN(CANBUS_PIN_SS); 
	}


void ShomCanBus::Log(String str)
	{
	Serial.println(str);
	}