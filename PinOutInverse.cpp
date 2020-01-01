#include "PinOutInverse.h"
// ------------------------------------
PinOutInverse::PinOutInverse() : PinOutInverse("Dummy PinOut", 0) 
	{
	}

PinOutInverse::PinOutInverse(String title, uint8_t pin) : PinOut(title, pin)
	{
	SetLogicType(LT_INVERSE);
	SetOff();
	//ShomPinMode(OUTPUT);
	}

