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
	bool val = (state == KS_ON);
	if (_logicType == LT_INVERSE)
		 val = !val;
	ShomPinWrite(val);
	_state = state; 
	ps2.New = _state;
	if (ps2.Changed() && !noLog) 
		LogStates(ps2);
	}
// ------------------------------------
void Pin::LogStates(PinState2 ps2)
	{
	String str  = String(_pin) + "; ";
	str = str + PinStateText(ps2.Old) + " -> " + PinStateText(ps2.New); 
	Log(str);
	}

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

//------------------------------
String Pin::StateText()
	{
	return Pin::PinStateText(_state);
	}

//------------------------------
static String Pin::PinStateText(PinState instate)
{
switch (instate)
	{
	case KS_NONE		: return "KS_NONE";
	case KS_ON			: return "KS_ON";
	case KS_OFF			: return "KS_OFF";
	default			    : return "KS_unknown-" + String(instate);
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
		unsigned int id = Pin::CanBus.SendCmd(CANBUS_READ, pin);
		Pin::CanBus.RelayDelay();
		canbusres = Pin::CanBus.GetResponse(id);
		//canbusres = Pin::CanBus.GetResponse(id, pin);
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
		unsigned int id = Pin::CanBus.GetMsgId();
		byte idh = highByte(id);
		byte idl = lowByte(id);
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(DATA_ID_HIGH, idh);
		Pin::CanBus.SetDataByte(DATA_ID_LOW, idl);
		Pin::CanBus.SetDataByte(DATA_CMD, CANBUS_WRITE);
		Pin::CanBus.SetDataByte(DATA_PIN, _pin - 100);
		Pin::CanBus.SetDataByte(DATA_VALUE, val);
		//Pin::CanBus.LogData();
		Pin::CanBus.Send();
		}
	}

//------------------------------
void Pin::ShomPinMode(byte pinmode)
	{
	Log("ShomPinMode " + PinModeText());
	_pinmode = pinmode;
	if (_pin < 100)
		{
		pinMode(_pin, _pinmode);
		}
	else
		{
		unsigned int id = Pin::CanBus.GetMsgId();
		byte idh = highByte(id);
		byte idl = lowByte(id);
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(DATA_ID_HIGH, idh);
		Pin::CanBus.SetDataByte(DATA_ID_LOW, idl);
		Pin::CanBus.SetDataByte(DATA_CMD, CANBUS_MODE);
		Pin::CanBus.SetDataByte(DATA_PIN, _pin - 100);
		Pin::CanBus.SetDataByte(DATA_VALUE, _pinmode);
		Pin::CanBus.Send();
		}
	}

//------------------------------
void Pin::LogInfo()
	{
	PinInfo	pi = GetInfo();
	String str = pi.UnitType + ", " + pi.PinModeText + ", " + pi.State + ", " + String(pi.Pin);
	Log(str);   
  	}

//------------------------------
String Pin::PinModeText()
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
String Pin::LogicTypeText()
	{
	switch (_logicType)
		{
		case LT_NONE 		: return "NONE";
		case LT_NORMAL	 	: return "NORMAL";
		case LT_INVERSE 	: return "INVERSE";
		default			    : return "LogicTypeText: unknown-" + String(_logicType);
		}
	}

//------------------------------
void Pin::SetLogicType(LogicType logicType)
	{
	_logicType = logicType; 
	}
