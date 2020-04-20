#include "ShomLCD.h"

// ------------------------------------
ShomLCD::ShomLCD()
{
}

// ------------------------------------
ShomLCD::ShomLCD(const char *title, byte pin_sda, byte pin_slc) : Unit(title, UT_LCD)
{
    Setup(title, pin_sda, pin_slc);
}


// ------------------------------------
void ShomLCD::Setup(const char *title, byte pin_sda, byte pin_slc)
{
	_pin_sda = pin_sda;
	_pin_slc = pin_slc;
	_title = title;
}
