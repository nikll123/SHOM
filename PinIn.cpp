#include "PinIn.h"
// ------------------------------------
PinIn::PinIn() : PinIn("PinIn", 0) 
{
}

PinIn::PinIn(String title, uint8_t pin) : Pin(title, pin, UT_PININ)
{
	LogicNormal();	
}

// ------------------------------------
void PinIn::LogicInverse()
{
	_setLogicType(LT_INVERSE);
}

// ------------------------------------
void PinIn::LogicNormal()
{
	_setLogicType(LT_NORMAL);
}

// ------------------------------------
void PinIn::_setLogicType(LogicType logicType)
{
	uint8_t impmode;
	_logicType = logicType; 
	if (_logicType == LT_NORMAL)
		impmode = INPUT;
	else
		impmode = INPUT_PULLUP;
	pinMode(_pin, impmode);
	_refreshState();
}

// ------------------------------------
PinState2 PinIn::GetState()
{
	PinState2 ps2;
    ps2.Old = _state;
	_refreshState();
	ps2.New = _state;
		
	if (ps2.Old != ps2.New) 
		{
		String str = _title + ": " + GetPinStateText(ps2.Old) + " -> " + GetPinStateText(ps2.New); 
		Log(str);
		}
	return ps2;
}

// ------------------------------------
void PinIn::_refreshState()
{
	bool newState = digitalRead(_pin);
			
	if (_logicType == LT_INVERSE)
		newState = !newState;

	if (newState == 1)
		_state = KS_ON;				
	else  //  if (newState == 0)
		_state = KS_OFF;				
}

// ------------------------------------
PinInInfo PinIn::GetInfo()
	{
	PinInfo pi = Pin::GetInfo();
    return {pi.Title,
			pi.UnitType, 
			GetPinStateText(_state), 
			_pin, 
			GetLogicTypeText()
			}; 
	}

// ------------------------------------
void PinIn::LogInfo()
	{
	PinInInfo pi = GetInfo();
	String str = _title + ": ";
	str = str + (pi.Title + "; ");
	str = str + (pi.UnitType + "; ");
	str = str + (pi.State + "; ");
	str = str + (String(_pin) + "; ");
	str = str + (pi.Logic + "; ");
	Log(str);
	}
	

// ------------------------------------
void PinIn::LogState()
	{
	String str = _title + ": ";
	str = str + GetPinStateText(_state) + "; ";
	Log(str);
	}

//------------------------------
String PinIn::GetLogicTypeText()
{
switch (_logicType)
	{
	case LT_NONE 		: return "NONE";
	case LT_NORMAL	 	: return "NORMAL";
	case LT_INVERSE 	: return "INVERSE";
	default			    : return "GetLogicTypeText: unknown-" + String(_logicType);
	}
}

