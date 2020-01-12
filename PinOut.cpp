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
//	SetOn(0);
SetState(KS_ON, 0);
	}

// ------------------------------------
/*void PinOut::SetOn(bool noLog)
	{
	SetState(KS_ON, noLog);
	}/*/

// ------------------------------------
void PinOut::SetOff()
	{
//	SetOff(0);
	SetState(KS_OFF, 0);
	}

// ------------------------------------
/*void PinOut::SetOff(bool noLog)
	{
	SetState(KS_OFF, noLog);
	}*/

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
