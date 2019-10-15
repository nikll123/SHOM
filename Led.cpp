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
	case LEDS_BLINKFAST		: return "BLINKFAST";
	case LEDS_BLINKSLOW		: return "BLINKSLOW";
	case LEDS_BLINK2		: return "BLINK2";
	case LEDS_BLINK3		: return "BLINK3";
	default			    	: return "GetLedStateText: unknown-" + String(ls);
	}
}

//------------------------------------
LedState2 Led::Refresh()
{
	LedState2 ls2;
	ls2.Old = _state; 
	_refreshState();
	ls2.New = _state;
	if (LOGLEVEL >= LL_NORMAL && ls2.Old != ls2.New) 
		{
		LogText(_title);
		LogText("  " + GetLedStateText(ls2.Old));
		LogTextLn(" -> " + GetLedStateText(ls2.New));
		}
	 
	return ls2;
}

void Led::_refreshState()
	{
	bool b = digitalRead(Pin);
	switch (_state)
		{
		case LEDS_BLINK  :
			b = _checkBlink(b, INTERVAL_BLINK);
			break;
		case LEDS_BLINKFAST  :
			b = _checkBlink(b, INTERVAL_BLINKFAST);
			break;
		case LEDS_BLINKSLOW  :
			b = _checkBlink(b, INTERVAL_BLINKSLOW);
			break;
		case LEDS_ON  :
			b = true;
			break;
		default :
			b = false;
		}	
	digitalWrite(Pin, b);				
	}

//------------------------------------
bool Led::_checkBlink(bool b, int interval)
	{
	unsigned long _millis2 = millis(); 
	if (_millis2 > _millis + interval)
		{
		_millis = _millis2;
		b = !b;
		}
	return b;
	}

//------------------------------------
void Led::_setState(LedState ls)
	{
	if (_state != ls)
		{
		if (LOGLEVEL > LL_MIN) 
			{
			LogText(_title);
			LogText(" " + GetLedStateText(_state));
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

void Led::SetBlinkSlow()
	{
	_setState(LEDS_BLINKSLOW);
	}

void Led::SetBlinkFast()
	{
	_setState(LEDS_BLINKFAST);
	}
