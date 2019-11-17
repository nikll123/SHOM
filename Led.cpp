#include "Led.h"

//------------------------------------
Led::Led() : Led("Dummy Led", 0, LEDS_NOTINIT)
{
}

Led::Led(String title, uint8_t pin) : Led(title, pin, LEDS_OFF)
{
}

Led::Led(String title, uint8_t pin, LedState ledState) : Unit(title, UT_LED)
{
    PinOutLed = PinOut(title + "_pin", pin);
    _logLevel = LL_NORMAL;
    if(pin > 0)
    	_state = ledState;
    else
	    _state = LEDS_NOTINIT;
}

//------------------------------------
bool Led::IsActive()
	{
	return _state != LEDS_NOTINIT;;
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
	PinOutInfo pi = PinOutLed.GetInfo();
    return { 
			ui.Title,
			ui.UnitType,
			pi.Pin,
			GetLedStateText(_state)
			}; 
	}

void Led::LogInfo()
	{
	LedInfo li = GetInfo();
	Log(li.UnitType + "; " + String(li.Pin) + "; " + GetLedStateText(_state));
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
	if (ls2.Old != ls2.New) 
		_logState(ls2);
	return ls2;
	}

void Led::_refreshState()
	{
	bool b = PinOutLed.IsHigh();
	bool noLog = false;
	switch (_state)
		{
		case LEDS_BLINK  :
			b = _checkBlink(b, INTERVAL_BLINK);
			noLog = true;
			break;
		case LEDS_BLINKFAST  :
			b = _checkBlink(b, INTERVAL_BLINKFAST);
			noLog = true;
			break;
		case LEDS_BLINKSLOW  :
			b = _checkBlink(b, INTERVAL_BLINKSLOW);
			noLog = true;
			break;
		case LEDS_ON  :
			b = true;
			break;
		default :
			b = false;
		}
	
	if (b)
		PinOutLed.SetOn(noLog);	
	else				
		PinOutLed.SetOff(noLog);
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
		_logState({_state, ls});
		_state = ls;
		_millis = millis();
		_refreshState();
		} 
	}
	
//------------------------------------
void Led::_logState(LedState2 ls2)
	{
	Log(GetLedStateText(ls2.Old) + " -> " + GetLedStateText(ls2.New));
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
