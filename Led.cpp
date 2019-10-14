#include "Led.h"

//------------------------------------
Led::Led() : Led("", 0)
{
}

Led::Led(String title, uint8_t pin) : Led(title, pin, LEDS_OFF)
{
}

Led::Led(String title, uint8_t pin, LedState ledState) : Unit(title, UT_LED)
{
    Pin = pin;
    pinMode (Pin, OUTPUT);
    _state = ledState;

}

//------------------------------------
LedState Led::GetState()
{
	return _state;
}

//------------------------------------
LedInfo Led::GetInfo()
{
	UnitInfo ui = Unit::GetInfo();
    return { 
			ui.Title,
			ui.UnitType,
			Pin,
			GetLedStateText(_state)
			}; 
}

void Led::LogInfo()
{
   LedInfo li = GetInfo();
   LogTextLn(li.Title + "; " + li.UnitType + "; " + String(li.Pin) + "; " + GetLedStateText(_state));
}


//------------------------------
String Led::GetLedStateText(LedState ls)
{
switch (ls)
	{
	case LEDS_NOTINIT		: return "NOTINIT";
	case LEDS_ON			: return "ON";
	case LEDS_OFF			: return "OFF";
	case LEDS_BLINK			: return "BLINK";
	default			    	: return "GetLedStateText: unknown-" + String(ls);
	}
}

//------------------------------------
LedState2 Led::CheckState()
{
	LedState2 ls2;
	ls2.Old = _state; 
	_refreshState();
	ls2.New = _state;
/*	if (LOGLEVEL > LL_NONE && ls2.Old != ls2.New) 
		{
		LogText(_title);
		LogText("  " + GetLedStateText(ls2.Old));
		LogTextLn(" -> " + GetLedStateText(ls2.New));
		}*/
	 
	return ls2;
}

void Led::_refreshState()
	{
	bool b = digitalRead(Pin);
	unsigned long _millis2; 
	switch (_state)
		{
		case LEDS_BLINK  :
			_millis2 = millis(); 
			if (_millis2 > _millis + INTERVAL_BLINK1)
				{
				_millis = _millis2;
				b = !b;
				}
			break;
		case LEDS_ON  :
			b = 1;
			break;
		default :
			b = 0;
		}	
	digitalWrite(Pin, b);				
	}

//------------------------------------
void Led::_setState(LedState ls)
	{
	if (_state != ls)
		{
		if (LOGLEVEL > LL_NONE) 
			{
			LogText(_title);
			LogText("  " + GetLedStateText(_state));
			LogTextLn(" -> " + GetLedStateText(ls));
			}
		_state = ls;
		_millis = millis();
		_refreshState();
		} 
	}

void Led::SetOn()
	{
	_setState(LEDS_ON);
	}

void Led::SetOff()
	{
	_setState(LEDS_OFF);
	}

void Led::SetBlink()
	{
	_setState(LEDS_BLINK);
	}
