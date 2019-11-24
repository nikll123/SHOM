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
void Pin::SetState(PinState state, bool noLog)
	{
	PinState2 ps2;
	ps2.Old = _state;
	//digitalWrite(_pin, (state == KS_ON));
	ShomPinWrite(state == KS_ON);
	_state = state; 
	ps2.New = _state;
	if (ps2.Changed() && !noLog) 
		{
		String str = GetPinStateText(ps2.Old) + " -> " + GetPinStateText(ps2.New); 
		Log(str);
		}
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
	//Log("ShomPinRead");
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
		//Pin::CanBus.LogData();
		Pin::CanBus.Send();
		}
	return res;
	}

//------------------------------
void Pin::ShomPinWrite(bool val)
	{
	//Log("ShomPinWrite");
	if (_pin < 100)
		{
		digitalWrite(_pin, val);
		}
	else
		{
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(0, CANBUS_WRITE);
		Pin::CanBus.SetDataByte(1, _pin - 100);
		Pin::CanBus.SetDataByte(2, val);
		//Pin::CanBus.LogData();
		Pin::CanBus.Send();
		}
	}

//------------------------------
void Pin::ShomPinMode(byte pinmode)
	{
	//Log("ShomPinMode");
	if (_pin < 100)
		{
		pinMode(_pin, pinmode);
		}
	else
		{
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(0, CANBUS_MODE);
		Pin::CanBus.SetDataByte(1, _pin - 100);
		Pin::CanBus.SetDataByte(2, pinmode);
		//Pin::CanBus.LogData();
		Pin::CanBus.Send();
		}
	}
