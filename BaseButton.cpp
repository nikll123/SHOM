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
InputState2 BaseButton::CheckState()
{
    InputState _stateOld = _state;
	_checkState();
	struct InputState2 res = {_stateOld, _state};
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
InputState  BaseButton::GetState()
{
	return _state;
}

// ------------------------------------
KeyInfo BaseButton::GetInfo()
{
    return {_state, 
			_title, 
			Core::GetInputStateText(_state), 
			_pinButton, 
			Core::GetLogicTypeText(_logicType)
			}; 
      
}

