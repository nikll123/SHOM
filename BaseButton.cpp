#include "BaseButton.h"

// ------------------------------------
BaseButton::BaseButton(String title, uint8_t pinButton, LogicType logicType) : BaseButton(title, pinButton, logicType, 0) 
{

}

BaseButton::BaseButton(String title, uint8_t pinButton, LogicType logicType, uint8_t _pinLed) : Unit(title, UT_BUTTON)
{

	_pinButton = pinButton;
	_pinLed = _pinLed;
	_in_state = KS_NONE;
	_logicType = logicType;
		
	uint8_t impmode;
	if (_logicType == LT_NORMAL)
		impmode = INPUT;
	else
		impmode = INPUT_PULLUP;
	
	pinMode(_pinButton, impmode);
	_refreshState();


}


// ------------------------------------
InState2 BaseButton::CheckState()
{
	InState2 is2;
    is2.Old = _in_state;
	_refreshState();
	is2.New = _in_state;
	if (LOGLEVEL > LL_NONE) 
		{
		LogText(_title);
		LogText("  " + GetInStateText(is2.Old));
		LogTextLn(" -> " + GetInStateText(is2.New));
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
		_in_state = KS_ON;				
	else  //  if (newState == 0)
		_in_state = KS_OFF;				
}

//------------------------------
String BaseButton::GetLogicTypeText()
{
switch (_logicType)
	{
	case LT_NONE 		: return "NONE";
	case LT_NORMAL	 	: return "NORMAL";
	case LT_INVERSE 	: return "INVERSE";
	default			    : return "GetLogicTypeText: unknown-" + String(_logicType);
	}
}

// ------------------------------------
InState  BaseButton::GetState()
{
	return _in_state;
}

// ------------------------------------
ButtonInfo BaseButton::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
    return {_in_state, 
			ui.Title,
			ui.UnitType, 
			GetInStateText(_in_state), 
			_pinButton, 
			GetLogicTypeText(),
			_pinLed 
			}; 
	}

// ------------------------------------
void BaseButton::LogInfo()
	{
	ButtonInfo bi = GetInfo();
	LogText(bi.Title + "; ");
	LogText(bi.UnitType + "; ");
	LogText(String(_pinButton) + "; ");
	LogText(bi.Logic + "; ");
	LogText(String(_pinLed) + "; ");
	LogTextLn(bi.State + ".");
	}

//------------------------------
String BaseButton::GetInStateText(InState instate)
{
switch (instate)
	{
	case KS_NONE		: return "NONE";
	case KS_ON			: return "ON";
	case KS_OFF			: return "OFF";
	default			    : return "GetInStateText: unknown-" + String(instate);
	}
}
