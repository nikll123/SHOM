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
	
	_refreshState();		
}

// ------------------------------------
InState2 BaseButton::CheckState()
{
	InState2 is2;
    is2.Old = _state;
	_refreshState();
	is2.New = _state;
	if (LOGLEVEL > LL_NONE) 
		{
		Core::LogTextLn("ButtonState " + GetInfo().Title);
		Core::LogTextLn("  old=" + Core::GetInStateText(is2.Old));
		Core::LogTextLn("  new=" + Core::GetInStateText(is2.New));
		}
	return is2;
}


// ------------------------------------
void BaseButton::_refreshState()
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
InState  BaseButton::GetState()
{
	return _state;
}

// ------------------------------------
ButtonInfo BaseButton::GetInfo()
{
    return {_state, 
			_title, 
			Core::GetInStateText(_state), 
			_pinButton, 
			Core::GetLogicTypeText(_logicType)
			}; 
      
}

