#include "Arduino.h"
#include "Core.h"
#include "Button.h"

// ------------------------------------
Button::Button()
{
	_title = "btn";
	_pinButton = 0;
	//pinMode(_pinButton, INPUT_PULLUP);
}
// ------------------------------------
/*Button::Button(String title, uint8_t pinButton)
{
	_title = title;
	_pinButton = pinButton;
} */

// ------------------------------------
uint8_t Button::GetState()
{
	_state = !digitalRead(_pinButton);
#ifdef PortMonitorLog
   Serial.println(GetStateTxt());
#endif
	return _state;
}

// ------------------------------------
void Button::InitPin(uint8_t pinButton, String title)
{
	_pinButton = pinButton;
	_title = title; 
	pinMode(_pinButton, INPUT_PULLUP);
}

// ------------------------------------
String Button::GetTitle()
{
	return _title;
}

// ------------------------------------
String Button::GetStateTxt()
{
	String txt = GetTitle();
    txt = txt + " is " + Core::GetStateText(_state);
	return txt;
}


