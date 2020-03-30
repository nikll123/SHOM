#include "Contactor.h"

// =========   CONSTRUCTORS   =========
Contactor::Contactor()
{
	_state = CS_NOTINIT;
}

// ------------------------------------
Contactor::Contactor(const char *title, uint8_t pinIn, uint8_t pinOut) : Unit(title, UT_CONTACTOR)
{
	Log_(_title);
	Log(": Creating");

	KeyIn = PinIn(title, pinIn, LogicTypeIn);
	KeyIn._title2 = "_KeyIn";
	KeyOut = PinOut(title, pinOut, LogicTypeOut);
	KeyOut._title2 = "_KeyOut";
	Init();
}

// ------------------------------------
void Contactor::Init()
{
	Log_(_title);
	Log(": Init");
	KeyIn.GetState();
	ContactorState2 cs2;
	cs2.Old = _state;
	_state = CS_OFF;
	cs2.New = _state;
	_logIfChanged(cs2);
	GetState();
}

// ------------------------------------
void Contactor::SetupLogic(LogicType ltIn, LogicType ltOut)
{
	LogicTypeIn = ltIn;
	LogicTypeOut = ltOut;
}

// ------------------------------------
void Contactor::LogInfo()
{
	Log_(_title);
	Log_(": ");
	Log_(UnitTypeText());
	Log_("; in-");
	LogInt_(KeyIn.GetPin());
	Log_("; out-");
	LogInt_(KeyOut.GetPin());
	Log_("; ");
	Log(GetContactorStateText(_state));
}

//------------------------------
const char *Contactor::GetContactorStateText(ContactorState state)
{
	switch (state)
	{
	case CS_NOTINIT:
		return "NOTINIT";
	case CS_ON:
		return "ON";
	case CS_OFF:
		return "OFF";
	case CS_STARTING:
		return "STARTING";
	case CS_STOPPING:
		return "STOPPING";
	case CS_UNKNOWN:
		return "UNKNOWN";
	default:
		return "ERROR";
	}
}

// ------------------------------------
/*ContactorState2 Contactor::GetState(String logText)
{
	//Log(logText);
	return GetState();
}*/

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
			if (_stateOut != KS_OFF)
				SetErrState(CS_ERR101);
			if (_stateIn != KS_OFF)
				SetErrState(CS_ERR102);
		}
		else if (_state == CS_STARTING)
		{
			if (_millsCheck == 0)
			{
				KeyOut.SetON();
				delay(RELAY_DELAY);
				unsigned long sink = Time(TA_FIX);
			}
			else if (Time(TA_PERIOD) > _timeOutOn)
				_state = CS_ON;

			_stateOut = KeyOut.GetState();
			_stateIn = (KeyIn.GetState()).New;

			if (_stateOut != KS_ON)
				SetErrState(CS_ERR103);
			else if (_stateIn != KS_ON)
				SetErrState(CS_ERR104);
		}
		else if (_state == CS_STOPPING)
		{
			KeyOut.SetOFF();
			delay(RELAY_DELAY);
			_state = CS_OFF;

			_stateOut = KeyOut.GetState();
			_stateIn = (KeyIn.GetState()).New;

			if (_state == CS_OFF)
			{
				if (_stateOut != KS_OFF)
					SetErrState(CS_ERR105);
				else if (_stateIn != KS_OFF)
					SetErrState(CS_ERR106);
			}
			else // US_STOPPING
			{
				if (_stateOut != KS_ON)
					SetErrState(CS_ERR107);
				else if (_stateIn != KS_ON)
					SetErrState(CS_ERR108);
			}
		}
		else if (_state == CS_ON)
		{
			if (_stateOut != KS_ON)
				SetErrState(CS_ERR109);
			else if (_stateIn != KS_ON)
				SetErrState(CS_ERR110);
		}

		cs2.New = _state;
		_logIfChanged(cs2);
	}

	return cs2;
}

// ------------------------------------
void Contactor::TurnOn()
{
	if (_state == CS_OFF)
	{
		Log("Contactor TurnOn");
		_Turn(CS_STARTING);
	}
	else
		Log("Contactor TurnOn - wrong status");
}

// ------------------------------------
void Contactor::TurnOff()
{
	if (_state == CS_ON || _state == CS_STARTING)
	{
		Log("Contactor TurnOff");
		_Turn(CS_STOPPING);
	}
	else
		Log("Contactor TurnOff - wrong status");

}

// ------------------------------------
void Contactor::_Turn(ContactorState csNew)
{
	if (csNew == CS_STARTING || csNew == CS_STOPPING)
	{
		ContactorState csCurr = _state;
		bool err = false;
		if (csNew == CS_STARTING && csCurr == CS_OFF ||							// start!
			csNew == CS_STOPPING && (csCurr == CS_ON || csCurr == CS_STARTING)) // stop !
		{
			Time(TA_RESET);
			_state = csNew;
		}
		ContactorState2 cs2(csCurr, csNew);
		_logIfChanged(cs2);
	}
	else if (csNew == CS_HALT)
	{
		KeyOut.SetOFF();
		_state = CS_HALT;
	}
	else
	{
		char str[STRMAXLEN] = "_Turn: wrong arg ";
		strcatShom(str, GetContactorStateText(csNew));
		Log(str);
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
	{
		Log_(_title);
		Log_(", changed: ");
		Log_(GetContactorStateText(cs2.Old));
		Log_(" -> ");
		Log(GetContactorStateText(cs2.New));
	}
}

// ------------------------------------
void Contactor::SetErrState(UnitError err)
{
	Log_("Error states Out=");
	Log_(KeyOut.StateText());
	Log_("(");
	LogInt_(KeyOut.GetPin());
	Log_(")");
	Log_(" != In=");
	Log_(KeyIn.StateText());
	Log_("(");
	LogInt_(KeyIn.GetPin());
	Log(")");
	LogErr(err);
	_state = CS_ERR;
}
