#include "Contactor.h"

// =========   CONSTRUCTORS   =========
Contactor::Contactor() : Unit("uzel", UT_NONE) 
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
	_state = CS_UNKNOWN;
	}

void Contactor::Init()
	{
 	Log(_title + " Init");
 	KeyOut.SetOff();
	KeyIn.GetState();
	_state = CS_OFF;
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
ContactorState2 Contactor::GetState()
	{
	ContactorState2 cs2 = {_state, _state};
	if (_state != CS_NOTINIT)
		{

		cs2.Old = _state;
	//LogTextLn(GetContactorStateText(cs2.Old));
		PinState stateKeyIn = (KeyIn.GetState()).New;
		PinState stateKeyOut = KeyOut.GetState(); 
		if		(_state == CS_OFF	&& stateKeyOut != KS_OFF) 	_state = CS_ERR101;
		else if (_state == CS_OFF	&& stateKeyIn != KS_OFF) 	_state = CS_ERR102;
		else if (_state == CS_STARTING)
			{
			if(_millsCheck == 0)
				{
				KeyOut.SetOn();
				_millsCheck = millis();
				}
			else if (millis() - _millsCheck > _timeOutOn)
				_state = CS_ON;
	
			stateKeyOut = KeyOut.GetState(); 
			stateKeyIn = (KeyIn.GetState()).New;
	
			if (stateKeyOut != KS_ON) 		_state = CS_ERR103;
			else if (stateKeyIn != KS_ON) 	_state = CS_ERR104;
	
			}
		else if(_state == CS_STOPPING)
			{
			if(_millsCheck == 0)
				{
				_millsCheck = millis();
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
		IfChanged(cs2);

		}
	return cs2;
	}

// ------------------------------------
void Contactor::TurnOn()
	{
 	Log(_title + " TurnOn");
    _Turn(CS_STARTING);
	}

// ------------------------------------
void Contactor::TurnOff()
	{
 	Log(_title + " TurnOff");
    _Turn(CS_STOPPING);
	}

void Contactor::_Turn(ContactorState csNew)
	{
	if (csNew == CS_STARTING || csNew == CS_STOPPING) 
		{
		ContactorState csCurr = (GetState()).New;
		bool err = false; 
		switch (csNew)
			{
			case CS_STARTING :
				if (csCurr == CS_OFF)   // start!
					{
					_millsCheck = 0;
					_state = csNew;
					}
				break;
			case CS_STOPPING :
				if (csCurr == CS_ON || csCurr == CS_STARTING)
					{
					_millsCheck = 0;
					_state = csNew;
					}
				break;
			}
		ContactorState2 cs2(csCurr, csNew); 
		IfChanged(cs2);
		}
	else
		{
		 Log("_Turn: wrong arg " + GetContactorStateText(csNew));
		}
	}
	

// ------------------------------------
/*void Contactor::TurnOffAlarm()
	{
	if (_uzelType == UT_CONTACTOR && _active)
		{
		//_millsCheck = 0;
		digitalWrite(_pinOut, 0);
		_state = US_OFF;
		}
	}*/

void Contactor::IfChanged(ContactorState2 cs2)
	{
	if (cs2.Old != cs2.New)
		Log(GetInfo().Title + " " + GetContactorStateText(cs2.Old) + " -> " + GetContactorStateText(cs2.New));
	}
	
void Contactor::Log(String str)
	{
	if (LOGLEVEL >= LL_NORMAL) LogTextLn(str);
	}
	

/*void Contactor::LogKeysState()
	{
	if (LOGLEVEL > LL_NORMAL) 
		{
		KeyIn.LogState();
		KeyOut.LogState();
		}
	}*/
