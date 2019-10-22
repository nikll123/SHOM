#include "Conveyor.h"

// =========   CONSTRUCTORS   =========
Conveyor::Conveyor() : Unit("dummy uzel", UT_NONE) 
{
	_state = US_NOTINIT;
}

Conveyor::Conveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed) : Conveyor(title, pinIn, pinOut, pinAuto, pinLed,  TURN_ON_TIMEOUT, TURN_OFF_TIMEOUT) 
{
}


Conveyor::Conveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed, unsigned long timeOutOn, unsigned long timeOutOff) : Unit(title, UT_CONVEYOR)
	{
	ContactorConveyor = Contactor(title + "_cont", pinIn, pinOut, timeOutOn, timeOutOff);
	AtomatConveyor = PinIn(title + "_auto", pinAuto); 
	LedConveyor = Led(title + "_led", pinLed);
	_state = US_UNKNOWN;
	
	}

void Conveyor::Init()
	{
	Log(_title + " Init");
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
			_state != US_NOTINIT
			}; 
	}
	
// ------------------------------------
void Conveyor::LogInfo()
	{
	ConveyorInfo ci = GetInfo();
	LogText(ci.Title + "; ");
	LogText(ci.UnitType + "; ");
	LogText("in-" + String(ci.PinIn) + "; ");
	LogText("out-" + String(ci.PinOut) + "; ");
	LogText("auto-" + String(ci.PinAuto) + "; ");
	LogText("led-" + String(ci.PinLed) + "; ");
	if(ci.Active)
		LogText(String("Active; "));
	else
		LogText(String("Not active; "));
	LogText(ci.State + ".");
	LogLn();
	}

//------------------------------
String Conveyor::GetConveyorStateText(ConveyorState state)
{
switch (state)
	{
	case US_NOTINIT		: return "NOTINIT";
	case US_ON			: return "ON";
	case US_OFF			: return "OFF";
	case US_STARTING	: return "STARTING";
	case US_STOPPING	: return "STOPPING";
	case US_UNKNOWN		: return "UNKNOWN";
	default			    : return "Error " + String(state);
	}
}

// ------------------------------------
ConveyorState2 Conveyor::GetState()
	{
	ConveyorState2 us2 = {_state, _state};
	if (_state != US_NOTINIT && _state < US_ERR1000)
		{
        ContactorState2 cs2 = ContactorConveyor.GetState();
        PinState2 as2 = AtomatConveyor.GetState();

        if (cs2.New >= CS_ERR100) _state = US_ERR1001;
		else if (cs2.New == CS_ON		&& as2.New == KS_OFF) 	_state = US_ERR1002;
		else if (cs2.New == CS_OFF		&& as2.New == KS_ON) 	_state = US_ERR1003;
		else if (cs2.New == CS_STARTING	&& as2.New == KS_OFF) 	_state = US_ERR1005;

		else if (cs2.New == CS_OFF		&& as2.New == KS_OFF) 	_state = US_OFF;
		else if (cs2.New == CS_ON		&& as2.New == KS_ON) 	_state = US_ON;
		else if (cs2.New == CS_STARTING	&& as2.New == KS_ON) 	_state = US_STARTING;
		else if (cs2.New == CS_STOPPING	&& as2.New == KS_ON) 	_state = US_STOPPING;

		else
			{
			Log("US_ERR1000 : " + ContactorConveyor.GetContactorStateText(cs2.New) + " && " + AtomatConveyor.GetPinStateText(as2.New));
			_state = US_ERR1000;
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
		else if (cs2.New >= US_ERR1000)
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
		Log(_title + " TurnOn");
	    _Turn(US_STARTING);
	    }
	}

// ------------------------------------
void Conveyor::TurnOff()
	{
	if (_state == US_ON || _state == US_STARTING) 
		{
		Log(_title + " TurnOff");
	    _Turn(US_STOPPING);
	    }
	}

void Conveyor::_Turn(ConveyorState csNew)
	{
	if (csNew == US_STARTING || csNew == US_STOPPING) 
		{
		ConveyorState2 cs2;
		cs2.Old = (GetState()).New;
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
			_state = US_ERR1004;

		cs2.New = _state;	
		IfChanged(cs2);
		}
	else
		{
		 	Log("_Turn: wrong arg " + GetConveyorStateText(csNew));
		}
	}
	


// ------------------------------------
/*void Conveyor::TurnOffAlarm()
	{
	if (_uzelType == UT_CONTACTOR && _active)
		{
		//_millsCheck = 0;
		digitalWrite(_pinOut, 0);
		_state = US_OFF;
		}
	}*/


void Conveyor::Log(String str)
	{
	if (LOGLEVEL >= LL_NORMAL) LogTextLn(str);
	}

void Conveyor::LogStates(ConveyorState2 cs2)
	{
	Log(GetInfo().Title + " " + GetConveyorStateText(cs2.Old) + " -> " + GetConveyorStateText(cs2.New));
	}
		
	

/*void Conveyor::LogKeysState()
	{
	if (LOGLEVEL > LL_NORMAL) 
		{
		KeyIn.LogState();
		KeyOut.LogState();
		}
	}*/
