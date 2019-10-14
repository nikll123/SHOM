#include "Pin.h"
// ------------------------------------
Pin::Pin() : Pin("Pin", 0, UT_PIN) 
{
}

Pin::Pin(String title, uint8_t pin, UnitType ut) : Unit(title, ut)
{
	_pin = pin;
	_state = KS_NONE;
}


// ------------------------------------
PinState Pin::GetState()
{
	return _state;
}

// ------------------------------------
PinInfo Pin::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
    return {ui.Title,
			ui.UnitType, 
			GetPinStateText(_state), 
			_pin}; 
	}

//------------------------------
String Pin::GetPinStateText(PinState instate)
{
switch (instate)
	{
	case KS_NONE		: return "NONE";
	case KS_ON			: return "ON";
	case KS_OFF			: return "OFF";
	default			    : return "GetPinStateText: unknown-" + String(instate);
	}
}
