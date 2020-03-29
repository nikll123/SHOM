#include "PinOut.h"
// ------------------------------------
PinOut::PinOut() 
	{
	}

// ------------------------------------
PinOut::PinOut(const char * title, uint8_t pin) : PinOut(title, pin, LT_NORMAL)
	{
	}

// ------------------------------------
PinOut::PinOut(const char * title, uint8_t pin, LogicType lt) : Pin(title, pin, UT_PINOUT)
	{
	Pin::SetLogicType(lt);
	SetOFF();
	ShomPinMode(OUTPUT);
	}
	

// ------------------------------------
void PinOut::Inverse()
	{
	if (_state == KS_ON)
		SetOFF();	
	else
		SetON();
	}

