#include "Uzel.h"

// =========   CONSTRUCTORS   =========
Uzel::Uzel() : Unit("contactor", UT_NONE) 
{
	_state = CS_NOTINIT;
}


Uzel::Uzel(UnitType ut, String title, uint8_t pinIn, uint8_t pinOut) : Uzel(ut, title, pinIn, pinOut, 0) 
{
}

Uzel::Uzel(UnitType ut, String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinLed) : Uzel(ut, title, pinIn, pinOut, pinLed, TURN_ON_TIMEOUT, TURN_OFF_TIMEOUT) 
{
}


Uzel::Uzel(UnitType ut, String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinLed, unsigned long timeOutOn, unsigned long timeOutOff) : Unit(title, ut)
	{
	_timeOutOn = timeOutOn;
	_timeOutOff = timeOutOff;
	KeyIn = PinIn(title + "_KeyIn", pinIn);
	KeyOut = PinOut(title + "_KeyOut", pinOut);
	LedIndicator = Led(title + "_led", pinLed);
	LogTextLn(String(ut) + "ut; ");
	LogTextLn(String(_type) + "_type; ");
	
	
	if (!(ut == UT_AUTOMAT || ut == UT_CONTACTOR))
		_state = CS_NOTINIT;
	else
		_state = CS_UNKNOWN;
	
	}

void Uzel::Init()
	{
 	LogTextLn(_title + " Init");
	KeyIn.GetState();
	if (_type == UT_AUTOMAT)
		{
		KeyOut.SetOff();
		KeyOut.GetState();
		}
	_state = CS_OFF;
	LedIndicator.SetOff();
	GetState();
	}

// ------------------------------------
UzelInfo Uzel::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
	PinInInfo kii = KeyIn.GetInfo();
	PinOutInfo koi = KeyOut.GetInfo();
	LedInfo li = LedIndicator.GetInfo();
    return {ui.Title,
			ui.UnitType, 
			GetUzelStateText(_state), 
			kii.Pin, 
			koi.Pin, 
			li.Pin,
			_timeOutOn,
			_timeOutOff 
			}; 
	}
	
// ------------------------------------
void Uzel::LogInfo()
	{
	UzelInfo ci = GetInfo();
	LogText(ci.Title + "; ");
	LogText(ci.UnitType + "; ");
	LogText("in-" + String(ci.PinIn) + "; ");
	LogText("out-" + String(ci.PinOut) + "; ");
	LogText("led-" + String(ci.PinLed) + "; ");
	LogText(ci.State + ".");
	LogLn();
	}

//------------------------------
String Uzel::GetUzelStateText(UzelState state)
{
switch (state)
	{
	case CS_NOTINIT		: return "NOTINIT";
	case CS_ON			: return "ON";
	case CS_OFF			: return "OFF";
	case CS_STARTING	: return "STARTING";
	case CS_STOPPING	: return "STOPPING";
	case CS_UNKNOWN		: return "UNKNOWN";
	default			    : return "UzelState error " + String(state);
	}
}

// ------------------------------------
UzelState2 Uzel::GetState()
	{
	UzelState2 cs2 = {_state, _state};
	if (_state != CS_NOTINIT)
		{
		if (_type == UT_CONTACTOR) 
			cs2 = _getStateContactor();
		else  //  UT_AUTOMAT
			cs2 = _getStateAutomat();
	
		LedIndicator.Refresh();
		}
	return cs2;
	}

// ------------------------------------
UzelState2 Uzel::_getStateAutomat()
	{
	UzelState2 cs2;
	PinState2 stateKeyIn = KeyIn.GetState();

	if (stateKeyIn.Old == KS_ON)
		cs2.Old = CS_ON;
	else		
		cs2.Old = CS_OFF;
	
	if (stateKeyIn.New == KS_ON)
		cs2.New = CS_ON;
	else		
		cs2.New = CS_OFF;

	IfChanged(cs2);
	return cs2;
	}

UzelState2 Uzel::_getStateContactor()
	{
	UzelState2 cs2;
	cs2.Old = _state;
//LogTextLn(GetUzelStateText(cs2.Old));
	PinState stateKeyIn = (KeyIn.GetState()).New;
	PinState stateKeyOut = KeyOut.GetState(); 
	if(_state == CS_OFF)
		{
		if (stateKeyOut != KS_OFF) 		_state = CS_ERR101;
		else if (stateKeyIn != KS_OFF) 	_state = CS_ERR102;
		}
	else if(_state == CS_STARTING)
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
	return cs2;
	}

// ------------------------------------
void Uzel::TurnOn()
	{
 	LogTextLn(_title + " TurnOn");
    _Turn(CS_STARTING);
	}

// ------------------------------------
void Uzel::TurnOff()
	{
	if (_type = UT_CONTACTOR)
		{
	 	LogTextLn(_title + " TurnOff");
	    _Turn(CS_STOPPING);
	    }
	}

void Uzel::_Turn(UzelState csNew)
	{
	if ((_type = UT_CONTACTOR) &&
		(csNew == CS_STARTING || csNew == CS_STOPPING)) 
		{
		UzelState csCurr = (GetState()).New;
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
		UzelState2 cs2(csCurr, csNew); 
		IfChanged(cs2);
		}
	else
		{
		 	LogTextLn("_Turn: wrong arg " + GetUzelStateText(csNew));
		}
	}
	

// ------------------------------------
/*void Uzel::TurnOffAlarm()
	{
	if (_uzelType == UT_CONTACTOR && _active)
		{
		//_millsCheck = 0;
		digitalWrite(_pinOut, 0);
		_state = US_OFF;
		}
	}*/

void Uzel::IfChanged(UzelState2 cs2)
	{
	if (cs2.Old != cs2.New)
		{
		if (cs2.New == CS_ON)
			LedIndicator.SetOn();
		else if (cs2.New == CS_STARTING || cs2.New == CS_STOPPING)
			LedIndicator.SetBlink();
		else if (cs2.New >= CS_ERR100)
			LedIndicator.SetBlinkFast();
		else 
			LedIndicator.SetOff();

		if (LOGLEVEL > LL_MIN) 
			{
			LogText(GetInfo().Title);
			LogText(" " + GetUzelStateText(cs2.Old));
			LogText(" -> " + GetUzelStateText(cs2.New));
			LogLn();
			}
		}
	}
	

/*void Uzel::LogKeysState()
	{
	if (LOGLEVEL > LL_NORMAL) 
		{
		KeyIn.LogState();
		KeyOut.LogState();
		}
	}*/
