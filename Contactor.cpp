#include "Contactor.h"

// =========   CONSTRUCTORS   =========
Contactor::Contactor() 
{
	_state = CS_NOTINIT;
}

Contactor::Contactor(String title, uint8_t pinIn, uint8_t pinOut) : Unit(title, UT_CONTACTOR)
	{
	KeyIn = PinIn(title + "_KeyIn", pinIn);
	KeyIn.SetLogicType(LogicTypeIn);
	KeyOut = PinOut(title + "_KeyOut", pinOut, LogicTypeOut);
//	KeyOut.SetLogicType(LogicTypeOut);
	Init();
	}

void Contactor::Init()
	{
 	Log("Init");
 	KeyOut.SetOff();
	KeyIn.GetState();
	ContactorState2 cs2;
	cs2.Old = _state; 
	_state = CS_OFF;
	cs2.New = _state; 
	_logIfChanged(cs2);
	GetState();
	}

// ------------------------------------
static void Contactor::SetupLogic(LogicType ltIn, LogicType ltOut)
	{
	LogicTypeIn = ltIn;   
	LogicTypeOut = ltOut;
	}

// ------------------------------------
ContactorInfo Contactor::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
	PinInInfo kii = KeyIn.GetInfo();
	PinOutInfo koi = KeyOut.GetInfo();
    return {ui.Title,
			ui.UnitType, 
			GetContactorStateText(_state), 
			kii.Pin, 
			koi.Pin, 
			_timeOutOn,
			_timeOutOff 
			}; 
	}
	
// ------------------------------------
void Contactor::LogInfo()
	{
	ContactorInfo ci = GetInfo();
	LogText(ci.Title + "; ");
	LogText(ci.UnitType + "; ");
	LogText("in-" + String(ci.PinIn) + "; ");
	LogText("out-" + String(ci.PinOut) + "; ");
	LogText(ci.State + ".");
	LogLn();
	}

//------------------------------
String Contactor::GetContactorStateText(ContactorState state)
{
switch (state)
	{
	case CS_NOTINIT		: return "NOTINIT";
	case CS_ON			: return "ON";
	case CS_OFF			: return "OFF";
	case CS_STARTING	: return "STARTING";
	case CS_STOPPING	: return "STOPPING";
	case CS_UNKNOWN		: return "UNKNOWN";
	default			    : return "ERROR";
	}
}

// ------------------------------------
ContactorState2 Contactor::GetState(String logText)
	{
	Log(logText);
	return GetState();	
	}

// ------------------------------------
ContactorState2 Contactor::GetState()
	{
	ContactorState2 cs2 = {_state, _state};
	if (_state != CS_NOTINIT)
		{

		cs2.Old = _state;
		_stateIn = (KeyIn.GetState()).New;
		_stateOut = KeyOut.GetState(); 
		if (_state == CS_OFF)
			{
			if (_stateOut != KS_OFF) SetErrState(CS_ERR101);
			if (_stateIn != KS_OFF) SetErrState(CS_ERR102);
			}
		else if (_state == CS_STARTING)
			{
			if(_millsCheck == 0)
				{
				KeyOut.SetOn();
				delay(RELAY_DELAY);
				unsigned long sink = Time(TA_FIX);
				}
			else if (Time(TA_PERIOD) > _timeOutOn)
				_state = CS_ON;
	
			_stateOut = KeyOut.GetState(); 
			_stateIn = (KeyIn.GetState()).New;
	
			if (_stateOut != KS_ON) 		SetErrState(CS_ERR103);
			else if (_stateIn != KS_ON) 	SetErrState(CS_ERR104);
	
			}
		else if(_state == CS_STOPPING)
			{
			if(_millsCheck == 0)
				{
				unsigned long sink = Time(TA_FIX);
				}
			else if (Time(TA_PERIOD) > _timeOutOff)
				{
				KeyOut.SetOff();
				delay(RELAY_DELAY);
				_state = CS_OFF;
				}
				
			_stateOut = KeyOut.GetState(); 
			_stateIn = (KeyIn.GetState()).New;
	
			if (_state == CS_OFF)
				{
				if (_stateOut != KS_OFF) 		SetErrState(CS_ERR105);
				else if (_stateIn != KS_OFF) 	SetErrState(CS_ERR106);
				}
			else    // US_STOPPING
				{
				if (_stateOut != KS_ON) 		SetErrState(CS_ERR107);
				else if (_stateIn != KS_ON) 	SetErrState(CS_ERR108);
				}
			}
		else if(_state == CS_ON)
			{
			if (_stateOut != KS_ON) 		SetErrState(CS_ERR109);
			else if (_stateIn != KS_ON) 	SetErrState(CS_ERR110);
			}
	
		cs2.New = _state;
		_logIfChanged(cs2);
		}

	//Log("_millsCheck __3 " + String (_millsCheck));

	return cs2;
	}

// ------------------------------------
void Contactor::TurnOn()
	{
 	String str = "TurnOn";
	if(_state == CS_OFF)
	    _Turn(CS_STARTING);
    else
   	 	str = str + " - wrong status";
    Log(str);
	}

// ------------------------------------
void Contactor::TurnOff()
	{
 	String str = "TurnOff";
	if(_state == CS_ON || _state == CS_STARTING)
	    _Turn(CS_STOPPING);
    else
   	 	str = str + " - wrong status";
    Log(str);
	}

// ------------------------------------
void Contactor::_Turn(ContactorState csNew)
	{
	if (csNew == CS_STARTING || csNew == CS_STOPPING) 
		{
		ContactorState csCurr = _state;
		bool err = false; 
		if (csNew == CS_STARTING && csCurr == CS_OFF ||   						// start!
			csNew == CS_STOPPING && (csCurr == CS_ON || csCurr == CS_STARTING))  // stop !
			{
			Time(TA_RESET);
			_state = csNew;
			}
		ContactorState2 cs2(csCurr, csNew); 
		_logIfChanged(cs2);
		}
	else if (csNew == CS_HALT)
		{
		KeyOut.SetOff();
		_state = CS_HALT;
		}
	else
		{
		 Log("_Turn: wrong arg " + GetContactorStateText(csNew));
		}
	}

// ------------------------------------
void Contactor::Halt()
	{
	_Turn(CS_HALT);
	Log("Halt()");
	}

// ------------------------------------
void Contactor::_logIfChanged(ContactorState2 cs2)
	{
	if (cs2.Old != cs2.New)
		Log(GetContactorStateText(cs2.Old) + " -> " + GetContactorStateText(cs2.New));
	}
	
// ------------------------------------
void Contactor::SetErrState(UnitError err)
	{
	String msg = "Error states Out="  + KeyOut.StateText() +"(" + String(KeyOut.GetInfo().Pin) + ")";
	msg = msg +" != In="  + KeyIn.StateText()  +"(" + String(KeyIn.GetInfo().Pin) + ")"; 
	Log(msg);
	LogErr(err);
	_state = CS_ERR;
	}

