#include "Pin.h"

// ------------------------------------
Pin::Pin() 
{
}

// ------------------------------------
Pin::Pin(char* title, uint8_t pin, UnitType ut) : Unit(title, ut)
{
	_pin = pin;
	_state = KS_NONE;
}

ShomCanBus Pin::CanBus = ShomCanBus("PinCanBus"); 

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
	bool val = (state == KS_ON);
	if (_logicType == LT_INVERSE)
		 val = !val;
	ShomPinWrite(val);
	_state = state; 
	ps2.New = _state;
	if(DoLogChanges)
		{
		if (ps2.Changed() && !noLog) 
			LogStates(ps2);
		}
	}

// ------------------------------------
void Pin::LogStates(PinState2 ps2)
	{
	char str[100];
	strcpy(str, char(_pin));
	strcat(str, "; ");
	strcat(str, "; ");
	strcat(str, PinStateText(ps2.Old));
	strcat(str, " -> ");
	strcat(str, PinStateText(ps2.New));
	Log(str);
	}

	/*
// ------------------------------------
PinInfo Pin::GetInfo()
	{
	return {Unit::_title,
			Unit::UnitTypeText(),
			PinModeText(), 
			StateText(), 
			_pin}; 
	}


// ------------------------------------
String Pin::Title1()
	{
    return Unit::_title + "_" + String(_pin); 
	}
*/

//------------------------------
char* Pin::StateText()
	{
	return Pin::PinStateText(_state);
	}

//------------------------------
static char* Pin::PinStateText(PinState instate)
{
switch (instate)
	{
	case KS_NONE		: return "KS_NONE";
	case KS_ON			: return "KS_ON";
	case KS_OFF			: return "KS_OFF";
	default			    : return "KS_unknown-" + char(instate);
	}
}

//------------------------------
bool Pin::ShomPinRead()
	{
	bool res = false;
	CanBusState	canbusres;
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
		if(canbusres == CBS_LOW)
			_state = KS_OFF;
		else if (canbusres == CBS_HIGH)
			_state = KS_ON;
		else
			_state = KS_ERR;
		res = (canbusres == CBS_HIGH);
		}
		
	return res;
	}

//------------------------------
void Pin::ShomPinWrite(bool val)
	{
	if(val == true)
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
	Log(PinModeText());
	}

//------------------------------
void Pin::LogInfo()
	{
	Log::BufClear();
	Log::BufCat(_title);
	Log::BufCat(" ");
	Log::BufCat(PinModeText());
	Log::BufCat(" ");
	Log::BufCat(StateText());
	Log::BufCat(" ");
	Log::BufCat(_pin);
	Log::BufPrint();
  	}

//------------------------------
char* Pin::PinModeText()
	{
	if(_pinmode == INPUT_PULLUP)  // INPUT_PULLUP, INPUT, OUTPUT are Arduino's constants  
		return "INPUT_PULLUP";
	else if(_pinmode == INPUT)
		return "INPUT";
	else if(_pinmode == OUTPUT)
		return "OUTPUT";
	else
		return "Unknown";
	}

//------------------------------
char* Pin::LogicTypeText()
	{
	switch (_logicType)
		{
		case LT_NONE 		: return "NONE";
		case LT_NORMAL	 	: return "NORMAL";
		case LT_INVERSE 	: return "INVERSE";
		default			    : return "LogicTypeText: unknown-" + char(_logicType);
		}
	}

//------------------------------
void Pin::SetLogicType(LogicType logicType)
	{
	_logicType = logicType; 
	}
