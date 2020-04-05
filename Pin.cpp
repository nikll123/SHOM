#include "Pin.h"

// ------------------------------------
Pin::Pin()
{
}

// ------------------------------------
Pin::Pin(const char *title, uint8_t pin, UnitType ut) : Unit(title, ut)
{
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

uint8_t Pin::GetPin()
{
	return _pin;
}
// ------------------------------------
void Pin::SetState(PinState state, bool doLog)
{
	PinState2 ps2;
	ps2.Old = _state;
	bool val = (state == KS_ON);
	if (_logicType == LT_INVERSE)
		val = !val;
	ShomPinWrite(val);
	_state = state;
	ps2.New = _state;
	if (DoLogChanges || doLog)
	{
		//if (ps2.Changed())
			//LogStates(ps2);
	}
}

// ------------------------------------
void Pin::LogStates(PinState2 ps2)
{
	Log_(_title);
	Log_(": ");
	LogInt_(_pin);
	Log_("; ");
	Log_(PinStateText(ps2.Old));
	Log_(" -> ");
	Log(PinStateText(ps2.New));
}

//------------------------------
const char *Pin::StateText()
{
	return Pin::PinStateText(_state);
}

//------------------------------
const char *Pin::PinStateText(PinState instate)
{
	switch (instate)
	{
	case KS_NONE:
		return "KS_NONE";
	case KS_ON:
		return "KS_ON";
	case KS_OFF:
		return "KS_OFF";
	default:
		return "KS_unknown";
	}
}

//------------------------------
bool Pin::ShomPinRead()
{
	bool res = false;
	CanBusState canbusres;
	_state = KS_NONE;
	byte pin = _pin;
	if (pin < 100)
	{
		res = digitalRead(pin);
		if (res)
			_state = KS_ON;
		else
			_state = KS_OFF;
	}
	else
	{
		pin = _pin - 100;
		unsigned int msgId = Pin::CanBus.SendCmd(CANBUS_READ, pin);
		Pin::CanBus.ResponseDelay();
		canbusres = Pin::CanBus.GetResponse(msgId, pin);
		if (canbusres == CBS_LOW)
			_state = KS_OFF;
		else if (canbusres == CBS_HIGH)
			_state = KS_ON;
		else
			_state = KS_ERR;
		res = (_state == KS_ON);
	}

	return res;
}

//------------------------------
void Pin::ShomPinWrite(bool val)
{
	if (val == true)
		_state = KS_ON;
	else
		_state = KS_OFF;

	if (_pin < 100)
	{
		digitalWrite(_pin, val);
	}
	else
	{
		unsigned int id = Pin::CanBus.SendCmd(CANBUS_WRITE, _pin - 100, val);
	}
}

//------------------------------
void Pin::ShomPinMode(byte pinmode)
{
	_pinmode = pinmode;
	if (_pin < 100)
	{
		pinMode(_pin, _pinmode);
	}
	else
	{
		unsigned int id = Pin::CanBus.SendCmd(CANBUS_MODE, _pin - 100, _pinmode);
	}
	//Log(PinModeText());
}

//------------------------------
void Pin::LogInfo()
{
	Log_(_title);
	Log_(": ");
	Log_(UnitTypeText());
	Log_(", ");
	Log_(PinModeText());
	Log_(", ");
	Log_(StateText());
	Log_(", ");
	LogInt_(_pin);
	Log_(", ");
	Log(LogicTypeText());
}

// ------------------------------------
void Pin::LogState()
{
	Log_(_title);
	Log_(": ");
	LogInt_(_pin);
	Log_(StateText());
	Log("; ");
}

//------------------------------
const char *Pin::PinModeText()
{
	if (_pinmode == INPUT_PULLUP) // INPUT_PULLUP, INPUT, OUTPUT are Arduino's constants
		return "INPUT_PULLUP";
	else if (_pinmode == INPUT)
		return "INPUT";
	else if (_pinmode == OUTPUT)
		return "OUTPUT";
	else
		return "Unknown";
}

//------------------------------
 const char *Pin::LogicTypeText()
{
	switch (_logicType)
	{
	case LT_NONE:
		return "NONE";
	case LT_NORMAL:
		return "NORMAL";
	case LT_INVERSE:
		return "INVERSE";
	default:
		return "LogicTypeText: unknown";
	}
}

//------------------------------
void Pin::SetLogicType(LogicType logicType)
{
	_logicType = logicType;
}

//------------------------------
void Pin::SetON()
{
	SetState(KS_ON, false);
}

//------------------------------
void Pin::SetOFF()
{
	SetState(KS_OFF, false);
}

//------------------------------
void Pin::SetON(bool doLog)
{
	SetState(KS_ON, doLog);
}

//------------------------------
void Pin::SetOFF(bool doLog)
{
	SetState(KS_OFF, doLog);
}
