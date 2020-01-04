#include "Automat.h"
// ------------------------------------
Automat::Automat() : Automat("Dummy Automat", 0) 
{
}

Automat::Automat(String title, uint8_t pin) : PinIn(title, pin)
{
	_pinType = PT_AUTOMAT;
}

