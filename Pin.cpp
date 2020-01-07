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
	if (ps2.Changed() && !noLog && _pin != 0) 
		LogStates(ps2);
	}
// ------------------------------------
void Pin::LogStates(PinState2 ps2)
	{
	String str  = String(_pin) + "; ";
	if (_pinType == PT_BUTTON)
		{
		if (ps2.Front())
			str += " Rise";
		else if (ps2.Back())
			str += " Fall";
		else  if (ps2.High())
			str += " Pressed";
		else  if (ps2.Low())
			str += " Not pressed";
		}
	else
		str += PinStateText(ps2.Old) + " -> " + PinStateText(ps2.New); 
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
uint8_t	 Pin::GetPin()
	{
	return _pin;
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
	if (pin == 0)
		{
		res = 1;
		_state = KS_ON;
		}
	else if (pin < 100)
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
		//Pin::CanBus.RelayDelay();
		canbusres = Pin::CanBus.GetResponse(id, pin);
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
		unsigned int id = Pin::CanBus.NewMsgId();
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
	_pinmode = pinmode;
	if (_pin < 100)
		{
		pinMode(_pin, _pinmode);
		}
	else
		{
		unsigned int id = Pin::CanBus.NewMsgId();
		byte idh = highByte(id);
		byte idl = lowByte(id);
		Pin::CanBus.ResetData();
		Pin::CanBus.SetDataByte(DATA_ID_HIGH, idh);
		Pin::CanBus.SetDataByte(DATA_ID_LOW, idl);
		Pin::CanBus.SetDataByte(DATA_CMD, CANBUS_MODE);
		Pin::CanBus.SetDataByte(DATA_PIN, _pin - 100);
		Pin::CanBus.SetDataByte(DATA_VALUE, _pinmode);
		//Pin::CanBus.LogData();
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
/*	String res1;
	// INPUT_PULLUP, INPUT, OUTPUT are Arduino's constants
	if(_pinmode == INPUT_PULLUP)
		res1 = " INPUT_PULLUP";
	else if(_pinmode == INPUT)
		res1 = " INPUT";
	else if(_pinmode == OUTPUT)
		res1 = " OUTPUT";
	else
		res1 = " PinModeText: unknown-" + String(_pinmode);
		
	String res = String(_pin) + res1;*/
		
	return " PinModeText: unknown-";
	}

//------------------------------
String Pin::LogicTypeText()
	{
	String res = "";
	if(_logicType == LT_NONE )
		res = "NONE";
	else if (_logicType == LT_NORMAL)
		res = "NORMAL";
	else if (_logicType == LT_INVERSE)
		res =  "INVERSE";
	else
		res = "LogicTypeText: unknown-" + String(_logicType);
	return res;
	}

//------------------------------
void Pin::SetLogicType(LogicType logicType)
	{
	_logicType = logicType; 
	}

//------------------------------
String Pin::PinTypeTxt()
	{
	String res = "";
	if(_pinType == PT_PIN)
		res = "Pin";
	else if (_pinType == PT_PININ)
		res = "PinIn";
	else if (_pinType == PT_PINOUT)
		res = "PinOut";
	else
		res = "PinTypeTxt: unknown-" + String(_pinType);
	return res;
	
	}
	