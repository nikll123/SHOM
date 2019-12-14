#include "PinOut.h"
// ------------------------------------
PinOut::PinOut() : PinOut("Dummy PinOut", 0) 
	{
	}

PinOut::PinOut(String title, uint8_t pin) : Pin(title, pin, UT_PINOUT)
	{
	ShomPinMode(OUTPUT);
	Init();
	}

// ------------------------------------
void PinOut::Init()
	{
	SetLogicType(LT_NORMAL);
	}   

// ------------------------------------
void PinOut::SetOn()
	{
	SetOn(0);
	}

// ------------------------------------
void PinOut::SetOn(bool noLog)
	{
	SetState(KS_ON, noLog);
	}

// ------------------------------------
void PinOut::SetOff()
	{
	SetOff(0);
	}

// ------------------------------------
void PinOut::SetOff(bool noLog)
	{
	SetState(KS_OFF, noLog);
	}

// ------------------------------------
void PinOut::Inverse()
	{
	if (_state == KS_ON)
		SetOff();	
	else
		SetOn();
	}

// ------------------------------------
PinOutInfo PinOut::GetInfo()
	{
	PinInfo pi = Pin::GetInfo();
    return {pi.Title,
			pi.UnitType, 
			StateText(),
			_pin
			}; 
	}

// ------------------------------------
void PinOut::LogInfo()
	{
	PinOutInfo pi = GetInfo();
	LogText(pi.Title + "; ");
	LogText(pi.UnitType + "; ");
	LogText(pi.State + "; ");
	LogText(String(_pin) + "; ");
	LogLn();
	}

// ------------------------------------
void PinOut::LogState()
	{
	PinOutInfo pi = GetInfo();
	LogText(pi.Title + "; ");
	LogText(pi.State + "; ");
	LogLn();
	}

// ------------------------------------
void PinOut::SetLogicType(LogicType logicType)
	{
	Pin::SetLogicType(logicType);
 	SetOff();
	}
