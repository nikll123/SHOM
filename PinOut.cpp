#include "PinOut.h"
// ------------------------------------
PinOut::PinOut()
{
}

// ------------------------------------
PinOut::PinOut(const char *title, uint8_t pin) : PinOut(title, pin, LT_NORMAL, UT_PINOUT)
{
}

// ------------------------------------
PinOut::PinOut(const char *title, uint8_t pin, LogicType lt) : PinOut(title, pin, lt, UT_PINOUT)
{
}

// ------------------------------------
PinOut::PinOut(const char *title, uint8_t pin, LogicType lt, UnitType ut) : Pin(title, pin, ut)
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
