#include "Led.h"

//------------------------------------
Led::Led()
{
}

// ------------------------------------
Led::Led(const char *title, uint8_t pin) : Led(title, pin, LED_OFF)
{
}

// ------------------------------------
Led::Led(const char *title, uint8_t pin, LedState ledState) : PinOut(title, pin, LT_NORMAL, UT_LED)
{
	char pinName[STRMAXLEN] = "";
	strcpy(pinName, title);
	strcatShom(pinName, "_pin");
	DoLogChanges = 0; 
    if(pin > 0)
    	_state = ledState;
    else
	    _state = LED_NOTINIT;

}

//------------------------------------
LedState Led::GetState()
{
	return _state;
}

void Led::LogInfo()
{
	Log_(_title);
	Log_(": ");
	Log_(UnitTypeText());
	Log_(", ");
	LogInt_(GetPin());
	Log_(", ");
	Log(GetLedStateText(_state));
}

//------------------------------
const char * Led::GetLedStateText(LedState ls)
{
	switch (ls)
	{
	case LED_NOTINIT:
		return "LED_NOTINIT";
	case LED_ON:
		return "LED_ON";
	case LED_OFF:
		return "LED_OFF";
	case LED_BLINK:
		return "LED_BLINK";
	case LED_BLINKFAST:
		return "LED_BLINKFAST";
	case LED_BLINKSLOW:
		return "LED_BLINKSLOW";
	case LED_BLINK2:
		return "LED_BLINK2";
	case LED_BLINK3:
		return "LED_BLINK3";
	default:
		return "LED_unknown-";
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

//------------------------------------
void Led::_refreshState()
{
	bool b = IsHigh();
	switch (_state)
	{
	case LED_BLINK:
		b = _checkBlink(b, INTERVAL_BLINK);
		break;
	case LED_BLINKFAST:
		b = _checkBlink(b, INTERVAL_BLINKFAST);
		break;
	case LED_BLINKSLOW:
		b = _checkBlink(b, INTERVAL_BLINKSLOW);
		break;
	case LED_ON:
		b = true;
		break;
	default:
		b = false;
	}

	if (b)
		SetON();
	else
		SetOFF();
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
	Log_(GetLedStateText(ls2.Old));
	Log_(" -> ");
	Log(GetLedStateText(ls2.New));
}

//------------------------------------
void Led::SetOn()
{
	_setState(LED_ON);
}

//------------------------------------
void Led::SetOff()
{
	_setState(LED_OFF);
}

//------------------------------------
void Led::Inverse()
{
	if (_state == LED_ON)
		SetOff();
	else
		SetOn();
}

//------------------------------------
void Led::SetBlink()
{
	_setState(LED_BLINK);
}

//------------------------------------
void Led::SetBlinkSlow()
{
	_setState(LED_BLINKSLOW);
}

//------------------------------------
void Led::SetBlinkFast()
{
	_setState(LED_BLINKFAST);
}
