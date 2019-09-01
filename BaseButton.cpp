#include "Arduino.h"
#include "Core.h"
#include "BaseButton.h"

// ------------------------------------
BaseButton::BaseButton() : BaseButton("btn", 0, LT_NONE)
{

}
// ------------------------------------
BaseButton::BaseButton(String title, uint8_t pinButton, LogicType logicType)
{
	_title = title;
	_pinButton = pinButton;
	_logicType = logicType;
	_state = KS_NONE;
	if (_logicType == LT_NORMAL)
		pinMode(_pinButton, INPUT);
	else
		pinMode(_pinButton, INPUT_PULLUP);
	
	_checkState();		
}

// ------------------------------------
KeyState BaseButton::CheckState()
{
    _checkState();
	LogState ("Result state");

	return _state;
}


// ------------------------------------
KeyState BaseButton::_checkState()
{
	bool newState = digitalRead(_pinButton);
	if (_logicType == LT_INVERSE)
		newState = !newState;

	if (newState == 1)
		{
			if (_state == KS_OFF)
				_state = KS_FRONT;
			else
				_state = KS_ON;				
		}
	else  //  if (newState == 0)
		{
			if (_state == KS_ON)
				_state = KS_BACK;
			else
				_state = KS_OFF;				
		}
		
	return _state;
}

// ------------------------------------
String BaseButton::GetTitle()
{
	return _title;
}

// ------------------------------------
String BaseButton::GetStateText()
{
	String txt = GetTitle();
    txt = txt + " state is " + Core::GetKeyStateText(_state);
	return txt;
}

 // ------------------------------------
void BaseButton::LogState(String txt)
{
#ifdef PortMonitorLog
	Serial.print(GetStateText());
	Serial.print("; ");
	Serial.println(txt);
#endif
}


