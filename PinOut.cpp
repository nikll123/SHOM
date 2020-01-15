#include "PinOut.h"
// ------------------------------------
PinOut::PinOut() 
	{
	}

PinOut::PinOut(String title, uint8_t pin) : PinOut(title, pin, LT_NORMAL)
	{
	}

PinOut::PinOut(String title, uint8_t pin, LogicType lt) : Pin(title, pin, UT_PINOUT)
	{
	Pin::SetLogicType(lt);
	SetOff();
	ShomPinMode(OUTPUT);
	}
	
// ------------------------------------
void PinOut::SetOn()
	{
	SetState(KS_ON, 0);
	}


// ------------------------------------
void PinOut::SetOff()
	{
	SetState(KS_OFF, 0);
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
			_pin,
			LogicTypeText()
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
	LogText(pi.Logic + "; ");
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
