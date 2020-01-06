#include "Contactor.h"

// =========   CONSTRUCTORS   =========
Contactor::Contactor() : Unit("Dummy contactor", UT_NONE) 
{
	_state = CS_NOTINIT;
}

Contactor::Contactor(String title, uint8_t pinIn, uint8_t pinOut) : Unit(title, UT_CONTACTOR)
	{
	_logLevel = LL_NORMAL;
	KeyIn = PinIn(title + "_KeyIn", pinIn, LogicTypeIn);
	KeyOut = PinOut(title + "_KeyOut", pinOut, LogicTypeOut);
	Init();
	}

void Contactor::Init()
	{
 	//Log("Init");
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

// ------------------------------------
String Contactor::GetStateTxt()
	{
	String str = "Contactor (in=";
	str += KeyIn.GetPin();
	str += ",out=";
	str += KeyOut.GetPin();
	str += ") state=";
	str += GetContactorStateText(_state);
	return str;
	}

//------------------------------
static String Contactor::GetContactorStateText(ContactorState state)
{
switch (state)
	{
	case CS_NOTINIT		: return "NOTINIT";
	case CS_ON			: return "ON";
	case CS_OFF			: return "OFF";
	case CS_STARTING1	: return "STARTING1";
	case CS_STARTING2	: return "STARTING2";
	case CS_STOPPING1	: return "STOPPING1";
	case CS_STOPPING2	: return "STOPPING2";
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
		_stateOut = KeyOut.GetState();
		_stateIn = (KeyIn.GetState()).New;
		bool notDummyContactor = KeyIn.GetPin() != 0;   // not Dummy conveyor 
		if (_state == CS_OFF)
			{
			if (notDummyContactor)
				{
				if (_stateOut != KS_OFF) SetErrState(CS_ERR101);
				if (_stateIn != KS_OFF) SetErrState(CS_ERR102);
				}
			}
		else if (_state == CS_STARTING1)
			{
			if(_millsCheck == 0)
				{
				KeyOut.SetOn();
				unsigned long sink = Time(TA_FIX);
				}
			else if (Time(TA_PERIOD) > _timeOutRelayOn)	
				_state = CS_STARTING2;
			}
		else if (_state == CS_STARTING2)
			{
			if (Time(TA_PERIOD) > _timeOutOn)
				_state = CS_ON;
				
			_stateOut = KeyOut.GetState(); 
			_stateIn = KeyIn.GetState().New;
	
			if (notDummyContactor)
				{
				if (_stateOut != KS_ON) 		SetErrState(CS_ERR103);
				else if (_stateIn != KS_ON) 	SetErrState(CS_ERR104);
				}	
				
			}
		else if(_state == CS_STOPPING1)
			{
			if(_millsCheck == 0)
				{
				unsigned long sink = Time(TA_FIX);
				}
			else if (Time(TA_PERIOD) > _timeOutRelayOn)
				{
				_state = CS_STOPPING2;
				KeyOut.SetOff();
				}
				
			_stateOut = KeyOut.GetState(); 
			_stateIn = KeyIn.GetState().New;
	
			if (notDummyContactor && _state == CS_STOPPING1)
				{
				if (_stateOut != KS_ON) 		SetErrState(CS_ERR107);
				else if (_stateIn != KS_ON) 	SetErrState(CS_ERR108);
				}
									
			}
		else if (_state == CS_STOPPING2)
			{
			if (Time(TA_PERIOD) >= _timeOutOff)
				{
				_state = CS_OFF;
				_stateOut = KeyOut.GetState(); 
				_stateIn = KeyIn.GetState().New;
				if (notDummyContactor)
					{
					if (_stateOut != KS_OFF) 		SetErrState(CS_ERR105);
					else if (_stateIn != KS_OFF) 	SetErrState(CS_ERR106);
					}
				} 
			}

		else if(_state == CS_ON)
			{
			if (notDummyContactor)
				{
				if (_stateOut != KS_ON) 		SetErrState(CS_ERR109);
				else if (_stateIn != KS_ON) 	SetErrState(CS_ERR110);
				}
			}
	
		cs2.New = _state;
		_logIfChanged(cs2);
		}

	return cs2;
	}

// ------------------------------------
void Contactor::TurnOn()
	{
 	String str = "TurnOn";
	if(_state == CS_OFF)
	    _Turn(CS_STARTING1);
    else
   	 	str = str + " - wrong status";
    Log(str);
	}

// ------------------------------------
void Contactor::TurnOff()
	{
 	String str = "TurnOff";
	if(_state == CS_ON || _state == CS_STARTING1 || _state == CS_STARTING2)
	    _Turn(CS_STOPPING1);
    else
   	 	str = str + " - wrong status";
    Log(str);
	}

// ------------------------------------
void Contactor::_Turn(ContactorState csNew)
	{
	if (csNew == CS_STARTING1 || csNew == CS_STOPPING1) 
		{
		ContactorState csCurr = _state;
		bool err = false; 
		//if (csNew == CS_STARTING1 && csCurr == CS_OFF ||   						// start!
		//	csNew == CS_STOPPING1 && (csCurr == CS_ON || csCurr == CS_STARTING1 || csCurr == CS_STARTING2))  // stop !
		//	{
			Time(TA_RESET);
			_state = csNew;
		//	}
		_logIfChanged({csCurr, csNew});
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

