#include "Led.h"

//------------------------------------
Led::Led(String title, uint8_t pin) : Unit(title, UT_LED)
{
    _pin = pin;
    _state = LED_NOTINIT;

}

//------------------------------------
LedInfo Led::GetInfo()
{
	UnitInfo ui = Unit::GetInfo();
    return { 
			ui.Title,
			ui.UnitType,
			_pin,
			GetLedStateText()
			}; 
}

void Led::LogInfo()
{

}


//------------------------------
String Led::GetLedStateText()
{
switch (_state)
	{
	case LED_NOTINIT		: return "NOTINIT";
	case LED_ON				: return "ON";
	case LED_OFF			: return "OFF";
	case LED_BLINK			: return "BLINK";
	default			    	: return "GetLedStateText: unknown-" + String(_state);
	}
}
