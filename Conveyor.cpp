#include "Conveyor.h"

// =========   CONSTRUCTORS   =========
Conveyor::Conveyor() : Unit("dummy", UT_CONVEYOR) 
{
	_state = US_NOTINIT;
}

Conveyor::Conveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed) : Unit(title, UT_CONVEYOR)
	{
	ContactorConveyor = Contactor(title + "_cont", pinIn, pinOut);
	AtomatConveyor = PinIn(title + "_auto", pinAuto);
	AtomatConveyor.SetLogicType(LogicTypeAutomat); 
	LedConveyor = Led(title + "_led", pinLed);
	_state = US_UNKNOWN;
	_logLevel = LL_NORMAL;
	}

void Conveyor::Init()
	{
	Log("Init");
	_state = US_OFF;
	LedConveyor.SetOff();
	ContactorConveyor.Init();
	GetState();
	}

// ------------------------------------
ConveyorInfo Conveyor::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
	ContactorInfo ci = ContactorConveyor.GetInfo();
	LedInfo li = LedConveyor.GetInfo();
	PinInInfo ai = AtomatConveyor.GetInfo();
    return {ui.Title,
			ui.UnitType, 
			GetConveyorStateText(_state), 
			ci.PinIn, 
			ci.PinOut,
			ai.Pin, 
			li.Pin,
			IsActive(),
			}; 
	}
	
// ------------------------------------
static void Conveyor::SetupLogic(LogicType ltIn, LogicType ltOut, LogicType ltAuto)
	{
	Contactor::SetupLogic(ltIn, ltOut);
	LogicTypeAutomat = ltAuto;
	}

// ------------------------------------
String Conveyor::GetInfoTxt()
	{
	String str = ""; 
	ConveyorInfo ci = GetInfo();
	str = str + ci.Title + "; ";
	str = str + ci.UnitType + "; ";
	str = str + "in-" + String(ci.PinIn) + "; ";
	str = str + "out-" + String(ci.PinOut) + "; ";
	str = str + "auto-" + String(ci.PinAuto) + "; ";
	str = str + "led-" + String(ci.PinLed) + "; ";
	if(ci.Active)
		str = str + "Active; ";
	else
		str = str + "Not active; ";
	
	str = str + ci.State + ".";
	return str;
	}

//------------------------------
static String Conveyor::GetConveyorStateText(ConveyorState state)
{
switch (state)
	{
	case US_NOTINIT		: return "NOTINIT";
	case US_UNKNOWN		: return "UNKNOWN";
	case US_ON			: return "ON";
	case US_OFF			: return "OFF";
	case US_STARTING	: return "STARTING";
	case US_STOPPING	: return "STOPPING";
	default			    : return "ERROR";
	}
}

// ------------------------------------
ConveyorState2 Conveyor::GetState(String logText)
	{
	Log(logText);	
	return GetState();
	}

// ------------------------------------
ConveyorState2 Conveyor::GetState()
	{
	ConveyorState2 us2 = {_state, _state};
	if (_state != US_NOTINIT && _state < US_ERR)
		{
        ContactorState2 cs2 = ContactorConveyor.GetState();
        PinState2 as2 = AtomatConveyor.GetState();
		bool err = false;
        if (cs2.New >= CS_ERR)
        	{
			SetErrState(US_ERR201);        	
			}
		else if (as2.New == KS_ON)
			{
			if (cs2.New == CS_OFF) 				_state = US_OFF;
			else if (cs2.New == CS_ON) 			_state = US_ON;
			else if (cs2.New == CS_STARTING) 	_state = US_STARTING;
			else if (cs2.New == CS_STOPPING) 	_state = US_STOPPING;
			}
		else if(as2.New == KS_OFF)
			{
			if (cs2.New == CS_ON) 				SetErrState(US_ERR202);
			else if (cs2.New == CS_OFF)			SetErrState(US_ERR203); 
			else if (cs2.New == CS_STARTING)	SetErrState(US_ERR205); 
			else if (cs2.New == CS_STOPPING)	SetErrState(US_ERR206);
			else 								SetErrState(US_ERR207);
			}
		else
			{
			SetErrState(US_ERR208);
			}
			
		us2.New = _state;
		IfChanged(us2);
		}
	LedConveyor.Refresh();
	return us2;
	}

void Conveyor::IfChanged(ConveyorState2 cs2)
	{
	if (cs2.Old != cs2.New)
		{
		LogStates(cs2);
		if (cs2.New == US_ON)
			LedConveyor.SetOn();
		else if (cs2.New == US_STARTING || cs2.New == US_STOPPING)
			LedConveyor.SetBlink();
		else if (cs2.New >= US_ERR)
			LedConveyor.SetBlinkFast();
		else 
			LedConveyor.SetOff();
		}
	}


// ------------------------------------
void Conveyor::TurnOn()
	{
	if (_state == US_OFF) 
		{
		Log("TurnOn");
	    _Turn(US_STARTING);
	    }
	}

// ------------------------------------
void Conveyor::TurnOff()
	{
	if (_state == US_ON || _state == US_STARTING) 
		{
		Log("TurnOff");
	    _Turn(US_STOPPING);
	    }
	}

// ------------------------------------
void Conveyor::_Turn(ConveyorState csNew)
	{
	if (csNew == US_STARTING || csNew == US_STOPPING) 
		{
		ConveyorState2 cs2;
		cs2.Old = _state;
		bool err = false; 
		if ((csNew == US_STARTING && cs2.Old == US_OFF)									// start!
			|| (csNew == US_STOPPING && (cs2.Old == US_ON || cs2.Old == US_STARTING)))	// stop!
			{
			if(csNew == US_STARTING)
				ContactorConveyor.TurnOn();
			else
				ContactorConveyor.TurnOff();
			_state = csNew;
			}
		else
			{
			SetErrState(US_ERR209);
			}

		cs2.New = _state;	
		IfChanged(cs2);
		}
	else
		{
		 	Log("_Turn: wrong arg " + GetConveyorStateText(csNew));
		}
	}

// ------------------------------------
void Conveyor::Halt()
	{
	ContactorConveyor.Halt();
	LedConveyor.SetOff();
	_state = US_HALT;
	}

// ------------------------------------
void Conveyor::SetErrState(UnitError err)
	{
	LogErr(err);
	ContactorConveyor.Halt();
	_state = US_ERR;
	}

// ------------------------------------
void Conveyor::LogStates(ConveyorState2 cs2)
	{
	Log(GetConveyorStateText(cs2.Old) + " -> " + GetConveyorStateText(cs2.New));
	}

// ------------------------------------
void Conveyor::LogStatesPrevCurr(ConveyorStatePrevCurr cs2)
	{
	Log("Conveyor states: previous " + GetConveyorStateText(cs2.Prev) + ", current " + GetConveyorStateText(cs2.Curr));
	}
		
// ------------------------------------
bool Conveyor::IsActive()
	{
	return _state != US_NOTINIT;
	}	

