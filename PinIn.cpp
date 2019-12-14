#include "PinIn.h"
// ------------------------------------
PinIn::PinIn() : PinIn("Dummy PinIn", 0) 
{
}

PinIn::PinIn(String title, uint8_t pin) : Pin(title, pin, UT_PININ)
{
	SetLogicType(LT_NORMAL);	
	Init();
}

// ------------------------------------
void PinIn::Init()
{
	_refreshState();
}

// ------------------------------------
void PinIn::SetLogicType(LogicType logicType) 
{
	uint8_t inpmode;
	Pin::SetLogicType(logicType);
	//_logicType = logicType; 
	if (_logicType == LT_NORMAL)
		inpmode = INPUT;
	else
		inpmode = INPUT_PULLUP;
	ShomPinMode(inpmode);
}

// ------------------------------------
PinState2 PinIn::GetState()
{
	PinState2 ps2;
    ps2.Old = _state;
	_refreshState();
	ps2.New = _state;
		
	if (ps2.Old != ps2.New) 
		LogStates(ps2);
		
	return ps2;
}

// ------------------------------------
void PinIn::_refreshState()
{
	bool newState = ShomPinRead();
			
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
			Pin::PinStateText(_state), 
			_pin, 
			LogicTypeText()
			}; 
	}

// ------------------------------------
void PinIn::LogInfo()
	{
	PinInInfo pi = GetInfo();
	String str = (pi.Title + "; ");
	str = str + (pi.UnitType + "; ");
	str = str + (pi.State + "; ");
	str = str + (String(_pin) + "; ");
	str = str + (pi.Logic + "; ");
	Log(str);
	}
	

// ------------------------------------
void PinIn::LogState()
	{
	String str = Pin::PinStateText(_state) + "; ";
	Log(str);
	}

