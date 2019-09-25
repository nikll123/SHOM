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
KeyState2 BaseButton::CheckState()
{
    KeyState1 _stateOld = _state;
	_checkState();
	struct KeyState2 res = {_stateOld, _state};
	return res;
}


// ------------------------------------
void BaseButton::_checkState()
{
	bool newState = digitalRead(_pinButton);
	if (_logicType == LT_INVERSE)
		newState = !newState;

	if (newState == 1)
		{
		_state = KS_ON;				
		}
	else  //  if (newState == 0)
		{
		_state = KS_OFF;				
		}
}

// ------------------------------------
KeyState1  BaseButton::GetState()
{
	return _state;
}

// ------------------------------------
KeyInfo BaseButton::GetInfo()
{
    KeyInfo i ={_state, _title, Core::GetKeyStateText(_state), _pinButton, Core::GetLogicTypeText(_logicType)}; 
    return  i;
}

