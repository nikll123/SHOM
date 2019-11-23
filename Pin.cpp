#include "Pin.h"
// ------------------------------------
Pin::Pin() : Pin("DummyPin", 0, UT_PIN) 
{
}


Pin::Pin(String title, uint8_t pin, UnitType ut) : Unit(title, ut)
{
	_logLevel = LL_NORMAL;
	_pin = pin;
	_state = KS_NONE;
}

ShomCanBus Pin::CanBus = ShomCanBus(); 


// ------------------------------------
bool Pin::IsHigh()
	{
	return ShomPinRead();
	}

// ------------------------------------
bool Pin::IsLow()
	{
 	return !IsHigh();
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

//------------------------------
bool Pin::ShomPinRead()
	{
	bool res = false;
	if (_pin < 100)
		{
		res = digitalRead(_pin);
		}
	else
		{
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(0, CANBUS_READ);
		Pin::CanBus.SetDataByte(1, _pin - 100);
		Pin::CanBus.LogData();
		Pin::CanBus.Send();
		}
	return res;
	}

//------------------------------
void Pin::ShomPinWrite(bool val)
	{
	if (_pin < 100)
		{
		digitalWrite(_pin, val);
		}
	else
		{
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(0, CANBUS_WRITE);
		Pin::CanBus.SetDataByte(1, _pin - 100);
		Pin::CanBus.LogData();
		Pin::CanBus.Send();
		}
	}
