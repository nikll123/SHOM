#include "Contactor.h"

// =========   CONSTRUCTORS   =========
Contactor::Contactor() : Unit("Dummy contactor", UT_NONE) 
{
	_state = CS_NOTINIT;
}

Contactor::Contactor(String title, uint8_t pinIn, uint8_t pinOut) : Contactor(title, pinIn, pinOut, TURN_ON_TIMEOUT, TURN_OFF_TIMEOUT) 
{
}


Contactor::Contactor(String title, uint8_t pinIn, uint8_t pinOut, unsigned long timeOutOn, unsigned long timeOutOff) : Unit(title, UT_CONTACTOR)
	{
	_timeOutOn = timeOutOn;
	_timeOutOff = timeOutOff;
	KeyIn = PinIn(title + "_KeyIn", pinIn);
	KeyOut = PinOut(title + "_KeyOut", pinOut);
	Init();
	}

void Contactor::Init()
	{
 	Log(_title + ": Init");
 	KeyOut.SetOff();
	KeyIn.GetState();
	ContactorState2 cs2;
	cs2.Old = _state; 
	_state = CS_OFF;
	cs2.New = _state; 
	_ifChanged(cs2);
	GetState();
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
	default			    : return "Error " + String(state);
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
//Log("_millsCheck -----1 " + String (_millsCheck));
			
	ContactorState2 cs2 = {_state, _state};
	if (_state != CS_NOTINIT)
		{

		cs2.Old = _state;
	//LogTextLn(GetContactorStateText(cs2.Old));
		PinState stateKeyIn = (KeyIn.GetState()).New;
		PinState stateKeyOut = KeyOut.GetState(); 
		if (_state == CS_OFF)
			{
			if (stateKeyOut != KS_OFF) _state = CS_ERR101;
			if (stateKeyIn != KS_OFF) _state = CS_ERR102;
			}
		else if (_state == CS_STARTING)
			{
			if(_millsCheck == 0)
				{
		//Log("_millsCheck 0 " + String (_millsCheck));
				KeyOut.SetOn();
				//_millsCheck = millis();
				FixTime(1);
		//Log("_millsCheck 1 " + String (_millsCheck));
				}
			else if (millis() - _millsCheck > _timeOutOn)
				_state = CS_ON;
	
		//Log("_millsCheck " + String (_millsCheck));
	
			stateKeyOut = KeyOut.GetState(); 
			stateKeyIn = (KeyIn.GetState()).New;
	
			if (stateKeyOut != KS_ON) 		_state = CS_ERR103;
			else if (stateKeyIn != KS_ON) 	_state = CS_ERR104;
	
			}
		else if(_state == CS_STOPPING)
			{
			if(_millsCheck == 0)
				{
				//_millsCheck = millis();
				FixTime(1);
				}
			else if (millis() - _millsCheck > _timeOutOff)
				{
				KeyOut.SetOff();
				_state = CS_OFF;
				stateKeyOut = KeyOut.GetState(); 
				stateKeyIn = (KeyIn.GetState()).New;
				}
	
			if (_state == CS_OFF)
				{
				if (stateKeyOut != KS_OFF) 		_state = CS_ERR105;
				else if (stateKeyIn != KS_OFF) 	_state = CS_ERR106;
				}
			else    // US_STOPPING
				{
				if (stateKeyOut != KS_ON) 		_state = CS_ERR107;
				else if (stateKeyIn != KS_ON) 	_state = CS_ERR108;
				}
			}
		else if(_state == CS_ON)
			{
			if (stateKeyOut != KS_ON) 		_state = CS_ERR109;
			else if (stateKeyIn != KS_ON) 	_state = CS_ERR110;
			}
	
		cs2.New = _state;
		_ifChanged(cs2);
		}

	//Log("_millsCheck __3 " + String (_millsCheck));

	return cs2;
	}

// ------------------------------------
void Contactor::TurnOn()
	{
 	String str = _title + ": TurnOn";
	if(_state == CS_OFF)
	    _Turn(CS_STARTING);
    else
   	 	str = str + " - wrong status";
    Log(str);
	}

// ------------------------------------
void Contactor::TurnOff()
	{
 	String str = _title + ": TurnOff";
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
		switch (csNew)
			{
			case CS_STARTING :
				if (csCurr == CS_OFF)   // start!
					{
					//_millsCheck = 0;
					FixTime(0);
					_state = csNew;
					}
				break;
			case CS_STOPPING :
				if (csCurr == CS_ON || csCurr == CS_STARTING)
					{
					//_millsCheck = 0;
					FixTime(0);
					_state = csNew;
					}
				break;
			}
		ContactorState2 cs2(csCurr, csNew); 
		_ifChanged(cs2);
		}
	else
		{
		 Log("_Turn: wrong arg " + GetContactorStateText(csNew));
		}
	}
	

// ------------------------------------
void Contactor::TurnOffAlarm()
	{
	TurnOff();
	}

void Contactor::_ifChanged(ContactorState2 cs2)
	{
	if (cs2.Old != cs2.New)
		Log(GetInfo().Title + ": " + GetContactorStateText(cs2.Old) + " -> " + GetContactorStateText(cs2.New));
	}
	
void Contactor::Log(String str)
	{
	if (LOGLEVEL >= LL_NORMAL) LogTextLn(str);
	}

 void Contactor::FixTime(bool x)
	 {
	if (x)
		_millsCheck = millis();
	else
		_millsCheck = 0;
	 }
 