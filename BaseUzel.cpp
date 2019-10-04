#include "Arduino.h"
#include "Core.h"
#include "BaseUzel.h"

// =========   CONSTRUCTORS   =========
BaseUzel::BaseUzel()
{
	_state = US_NOTINIT;
	_title = "";
	_pinIn = 0;
	_pinOut = 0;
	_uzelType = UT_NONE;
	_logicType = LT_NONE;;
	_active = false;
	_timeOutOn = UZEL_TURN_ON_TIMEOUT;
	_timeOutOff = UZEL_TURN_OFF_TIMEOUT;
}

BaseUzel::BaseUzel(String title, uint8_t pinInput, uint8_t pinOutput, UzelType uzelType, LogicType logicType)
		: BaseUzel()
{
	_state = US_OFF;
	_title = title;
	_pinIn = pinInput;
	_pinOut = pinOutput;
	_uzelType = uzelType;
	_logicType = logicType;
	_active = true;
	if (_logicType == LT_NORMAL)
		pinMode(_pinIn, INPUT);
	else             // LT_INVERSE
		pinMode(_pinIn, INPUT_PULLUP);

	pinMode(_pinOut, OUTPUT);
}

BaseUzel::BaseUzel(String title, uint8_t pinInput, uint8_t pinOutput, UzelType uzelType, LogicType logicType, unsigned int timeOutOn, unsigned int timeOutOff)
		: BaseUzel(title, pinInput, pinOutput, uzelType, logicType)
{

	_timeOutOn = timeOutOn;
	_timeOutOff = timeOutOff;
}

// ------------------------------------
UzelInfo BaseUzel::GetInfo()
{
	return  { 
			_uzelType,
			Core::GetUzelTypeText(_uzelType),
			_title,
			_state,
			_pinIn,
			_pinOut,
			Core::GetLogicTypeText(_logicType),
			_inputState,
			_active,
			_timeOutOn,
			_timeOutOff
			};

}

// ------------------------------------
InState BaseUzel::GetInputState()
	{
	return _inputState;
	}

// ------------------------------------
String BaseUzel::GetInStateText()
	{
	return Core::GetInStateText(_inputState);
	}

// ------------------------------------
/*InState2 BaseUzel::CheckInState()
	{
	  return CheckInState(false);
	}*/

InState2 BaseUzel::CheckInState(uint8_t loglevel = LL_NONE)
	{

	InState2 is2;
	is2.Old = _inputState;

	bool value = digitalRead(_pinIn);
    if (_logicType == LT_INVERSE)
		value = !value;

	if (value == HIGH)
		_inputState = KS_ON;
	else
		_inputState = KS_OFF;

	is2.New = _inputState;

	if (LOGLEVEL > LL_NONE) 
		{
		Core::LogTextLn("InState " + GetInfo().Title);
		Core::LogTextLn("  old=" + Core::GetInStateText(is2.Old));
		Core::LogTextLn("  new=" + Core::GetInStateText(is2.New));
		}
	
	return is2;
	}

// ------------------------------------
UzelState BaseUzel::GetState()
	{
	return _state;
	}

// ------------------------------------
String BaseUzel::GetStateText()
	{
	return Core::GetUzelStateText(_state);
	}

// ------------------------------------
bool BaseUzel::IsActive()
	{
	return _active;
	}


// ------------------------------------
UzelState2 BaseUzel::CheckState()
	{
	UzelState2 us2;
	if (_state != US_NOTINIT)
		{
		us2.Old = _state;
		InState stateA = (CheckInState()).New;
		if (_uzelType == UT_CONTACTOR)
			{
			uint8_t valueContactor = digitalRead(_pinOut);
			if(_state == US_OFF)
				{
				if (!(valueContactor == 0 && stateA == KS_OFF))
					_state = US_ERROR_01;
			//Core::LogIntVal("_state", _state);
				}
			else if(_state == US_STARTING)
				{
				if(_millsCheck == 0)
					{
					digitalWrite(_pinOut, 1);
					valueContactor = digitalRead(_pinOut);
					_millsCheck = millis();
					}
				else if (millis() - _millsCheck > _timeOutOn)
					_state = US_ON;

				stateA = (CheckInState()).New;

				if (!(valueContactor == HIGH && stateA == KS_ON))
					_state = US_ERROR_02;
				}

			else if(_state == US_STOPPING)
				{
				if(_millsCheck == 0)
					{
					_millsCheck = millis();
					}
				else if (millis() - _millsCheck > _timeOutOff)
					{
					digitalWrite(_pinOut, 0);
					valueContactor = digitalRead(_pinOut);
					_state = US_OFF;
					}

				if (_state == US_OFF)
					{
					if (!(valueContactor == 0 && stateA == KS_OFF))
						_state = US_ERROR_03;
					}
				else    // US_STOPPING
					{
					if (!(valueContactor == 1 && stateA == KS_ON))
						_state = US_ERROR_05;
					}
				}
			}
		else
			{
			if (stateA == KS_ON)
				_state = US_ON;
			else if (stateA == KS_OFF)
				_state = US_OFF;
			else
				_state = US_ERROR_04;
			}
		us2.New = _state;
		}
	else
		{
		us2.Old = _state;
		us2.New = _state;
		}

	if (LOGLEVEL > LL_NONE) 
		{
		Core::LogTextLn("OutState " + GetInfo().Title);
		Core::LogTextLn("  old=" + Core::GetUzelStateText(us2.Old));
		Core::LogTextLn("  new=" + Core::GetUzelStateText(us2.New));
		}
		

	return us2;
	}

// ------------------------------------
void BaseUzel::TurnOn()
{
	if (_uzelType == UT_CONTACTOR && _active)
	{
		UzelState2 us2;
		us2.Old = _state; 
		_millsCheck = 0;
		_state = US_STARTING;
		us2.New = _state; 
		if (LOGLEVEL > LL_NONE) 
			{
			Core::LogTextLn("TurnOn " + GetInfo().Title);
			Core::LogTextLn("  old=" + Core::GetUzelStateText(us2.Old));
			Core::LogTextLn("  new=" + Core::GetUzelStateText(us2.New));
			}
	}
}

// ------------------------------------
void BaseUzel::TurnOff()
{
	if (_uzelType == UT_CONTACTOR && _active)
	{
		_millsCheck = 0;
		_state = US_STOPPING;
		//Core::LogTextLn(_title + ".TurnOff");
	}
}

// ------------------------------------
void BaseUzel::TurnOffAlarm()
{
	if (_uzelType == UT_CONTACTOR && _active)
	{
		//_millsCheck = 0;
		digitalWrite(_pinOut, 0);
		_state = US_OFF;

		//Core::LogTextLn("TurnOffAlarm");
	}
}

