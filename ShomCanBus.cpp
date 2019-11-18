#include "ShomCanBus.h"

// =========   CONSTRUCTORS   =========
ShomCanBus::ShomCanBus() : Unit("dummy", UT_CANBUS)
	{
	}


ShomCanBus::ShomCanBus(String title, uint8_t pin_ss, uint8_t canbus_id) :  Unit(title, UT_CANBUS)
	{
	_logLevel = LL_NORMAL;
	_canbus_id = canbus_id; 
	MCP_CAN canbus(pin_ss);
	int i = 1;
	bool canbus_ok = false;
	while (!canbus_ok)
		{
			canbus_ok = (CAN_OK == canbus.begin(CAN_50KBPS));
			
			if(canbus_ok || i > CREATE_TRY_MAX)
				break;
			else
				LogErr("CBS_ERR", CBS_ERR401);
			delay(100);
			i++;	
		}
	
	if (canbus_ok)
		{	
		_state = CBS_ON;
		Log("Init OK");
		}
	else
		{
		SetErrState(CBS_ERR402);
		}
	}

void ShomCanBus::SetErrState(CanBusState err)
	{
	LogErr("CBS_ERR", err);
	_state = CBS_ERR;
	}
