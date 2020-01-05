#include "Button.h"
// ------------------------------------
Button::Button() : Button("Dummy Btn", 0) 
{
}

Button::Button(String title, uint8_t pin) :  PinIn(title, pin)
{
	_pinType = PT_BUTTON;
}

Button::Button(String title, uint8_t pin, LogicType lt) : PinIn(title, pin, lt)
{
	_pinType = PT_BUTTON;
}

