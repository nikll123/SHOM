#include "Arduino.h"
#include "Core.h"
#include "BaseUzel.h"

// =========   CONSTRUCTORS   =========
BaseUzel::BaseUzel()
{
	_state = US_NOTINIT;
	_title = "";
	_pinAutomat = 0;
	_pinContactor = 0;
	_uzelType = UT_NONE;
	_logicType = LT_NONE;;
	_timeOutOn = UZEL_TURN_ON_TIMEOUT;
	_timeOutOff = UZEL_TURN_OFF_TIMEOUT;
}

BaseUzel::BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, UzelType uzelType, LogicType logicType)
		: BaseUzel()
{
	_state = US_OFF;
	_title = title;
	_pinAutomat = pinAutomat;
	_pinContactor = pinContactor;
	_uzelType = uzelType;
	_logicType = logicType;
	_active = true;
	if (_logicType == LT_NORMAL)
		pinMode(_pinAutomat, INPUT);
	else             // LT_INVERSE
		pinMode(_pinAutomat, INPUT_PULLUP);

	pinMode(_pinContactor, OUTPUT);
}

BaseUzel::BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, UzelType uzelType, LogicType logicType, unsigned int timeOutOn, unsigned int timeOutOff)
		: BaseUzel(title, pinAutomat, pinContactor, uzelType, logicType)
{

	_timeOutOn = timeOutOn;
	_timeOutOff = timeOutOff;
}

// ------------------------------------
UzelInfo BaseUzel::GetInfo()
{
	UzelInfo ui = { _uzelType,
					Core::GetUzelTypeText(_uzelType),
					_title,
					_state,
					_pinAutomat,
					_pinContactor,
					Core::GetLogicTypeText(_logicType),
					_automatState,
					_active,
					_timeOutOn,
					_timeOutOff
					};
}

// ------------------------------------
KeyState BaseUzel::GetAutomatState()
	{
	return _automatState;
	}

// ------------------------------------
KeyState2 BaseUzel::CheckAutomatState()
	{

	KeyState2 s;
	s.ValueOld = _automatState;

	bool valueAutomat = digitalRead(_pinAutomat);
    if (_logicType == LT_INVERSE) 
		valueAutomat = !valueAutomat;   
	
	if (valueAutomat == HIGH)
		_automatState = KS_ON;
	else
		_automatState = KS_OFF;

	//Core::LogTextLn(GetKeyStateText());
	s.ValueNew = _automatState;
	return s;
	}

// ------------------------------------
UzelState BaseUzel::GetState()
	{
	return _state;
	}

// ------------------------------------
UzelState2 BaseUzel::CheckState()
	{
	UzelState2 s;
	if (_state != US_NOTINIT)
		{
		s.ValueOld = _state;
		KeyState stateA;
		if (_uzelType == UT_CONTACTOR)
			{
			uint8_t valueContactor = digitalRead(_pinContactor);
			//Core::LogIntVal("valueContactor", valueContactor);
			if(_state == US_OFF)
				{
				stateA = _automatState;
				if (!(valueContactor == 0 && stateA == KS_OFF))
					_state = US_ERROR_01;
			//Core::LogIntVal("_state", _state);
				}
			else if(_state == US_STARTING)
				{
				if(_millsCheck == 0)
					{
					digitalWrite(_pinContactor, 1);
					valueContactor = digitalRead(_pinContactor);
					_millsCheck = millis();
					}
				else if (millis() - _millsCheck > _timeOutOn)
					_state = US_ON;

				stateA = _automatState;

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
					digitalWrite(_pinContactor, 0);
					valueContactor = digitalRead(_pinContactor);
					_state = US_OFF;
					}
				stateA = _automatState;

				if (_state == US_OFF)
					{
					if (!(valueContactor == 0 && stateA == KS_OFF))
						_state = US_ERROR_03;
					}
				 }
			}
		else
			{
			stateA = _automatState;
			if (stateA == KS_ON)
				_state = US_ON;
			else if (stateA == KS_OFF)
				_state = US_OFF;
			else
				_state = US_ERROR_04;
			}
		s.ValueOld = _state;
		}
	else
		{
		s.ValueOld = _state;
		s.ValueNew = _state;
		}

	return s;
	}

// ------------------------------------
void BaseUzel::TurnOn()
{
	if (_uzelType == UT_CONTACTOR && _active)
	{
		_millsCheck = 0;
		_state = US_STARTING;
		//Core::LogTextLn(_title + ".TurnOn");
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
		digitalWrite(_pinContactor, 0);
		_state = US_OFF;

		//Core::LogTextLn("TurnOffAlarm");
	}
}
