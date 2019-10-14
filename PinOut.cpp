#include "PinOut.h"
// ------------------------------------
PinOut::PinOut() : PinOut("KeyOut", 0) 
{
}

PinOut::PinOut(String title, uint8_t pin) : Pin(title, pin, UT_PINOUT)
{
	pinMode(pin, OUTPUT);
 	SetOff();
}

void PinOut::SetOn()
{
	_setState(KS_ON);
}

void PinOut::SetOff()
{
	_setState(KS_OFF);
}

// ------------------------------------
void PinOut::_setState(PinState state)
{
	PinState2 ps2;
	ps2.Old = _state;
	bool value = state == KS_ON;
	digitalWrite(_pin, value);
	_state = state; 
	ps2.New = _state;
	if (LOGLEVEL > LL_NONE && ps2.Old != ps2.New) 
		{
		LogText(_title);
		LogText(value);
		LogText(String(_pin));
		LogText(" " + GetPinStateText(ps2.Old));
		LogTextLn(" -> " + GetPinStateText(ps2.New));
		}
}

// ------------------------------------
PinState PinOut::GetState()
{
	return _state;
}

// ------------------------------------
PinOutInfo PinOut::GetInfo()
	{
	PinInfo pi = Pin::GetInfo();
    return {pi.Title,
			pi.UnitType, 
			GetPinStateText(_state), 
			_pin
			}; 
	}

// ------------------------------------
void PinOut::LogInfo()
	{
	PinOutInfo pi = GetInfo();
	LogText(pi.Title + "; ");
	LogText(pi.UnitType + "; ");
	LogText(pi.State + "; ");
	LogText(String(_pin) + "; ");
	LogLn();
	}
