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
	_timeOutOn = TURN_ON_TIMEOUT;
	_timeOutOff = TURN_OFF_TIMEOUT;
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
	_initialized = true;
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
String BaseUzel::GetTitle()
{
	return _title;
}

// ------------------------------------
UzelType BaseUzel::GetUzelType()
{
	return _uzelType; 
}

LogicType BaseUzel::GetLogicType()
{
	return _logicType; 
}

//------------------------------
String BaseUzel::GetUzelTypeText()
{
	return Core::GetUzelTypeText(_uzelType);
}

//------------------------------
String BaseUzel::GetLogicTypeText()
{
	return Core::GetLogicTypeText(_logicType);
}

// ------------------------------------
UzelState BaseUzel::GetState()
{
	return _state;
}

// ------------------------------------
String BaseUzel::GetStateText()
{
	String txt = GetTitle();
    txt = txt + " state is " + Core::GetUzelStateText(_state);
	return txt;
}

// ------------------------------------
String BaseUzel::GetKeyStateText()
{
	String txt = GetTitle();
    txt = txt + " Automat state is " + Core::GetKeyStateText(_automatState);
	return txt;
}


// ------------------------------------
KeyState BaseUzel::CheckAutomatState()
	{

	bool valueAutomat = digitalRead(_pinAutomat);
	
	if (_logicType == LT_NORMAL)
		{
		if (valueAutomat == HIGH)
			_automatState = KS_ON;
		else 
			_automatState = KS_OFF;
		}
	else
		{
		if (valueAutomat == HIGH)
			_automatState = KS_OFF;
		else
			_automatState = KS_ON;
		}
#ifdef PortMonitorLog
	Serial.println(GetKeyStateText());
#endif
	return _automatState;
	}




// ------------------------------------
UzelState BaseUzel::CheckState()
	{
	if (_state != US_NOTINIT)
		{
		LogState("Enter state");
		KeyState stateA;
		if (_uzelType == UT_CONTACTOR)
			{
			uint8_t valueContactor = digitalRead(_pinContactor);
			//Core::LogIntVal("valueContactor", valueContactor);
			if(_state == US_OFF)
				{
				stateA = CheckAutomatState();          
				if (!(valueContactor == 0 && stateA == KS_OFF))
					_state = US_ERROR;
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
					
				stateA = CheckAutomatState();
						
				if (!(valueContactor == HIGH && stateA == KS_ON))
					_state = US_ERROR;
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
				stateA = CheckAutomatState();
						
				if (_state == US_OFF)
					{
					if (!(valueContactor == 0 && stateA == KS_OFF))
						_state = US_ERROR;
					}
				 }
			}
		else
			{
			stateA = CheckAutomatState();          
			if (stateA == KS_ON)
				_state = US_ON;
			else if (stateA == KS_OFF)
				_state = US_OFF;
			else
				_state = US_ERROR_01;  
			} 

			LogState("Result state");

		}
	
	return _state;
	}
	



// ------------------------------------
void BaseUzel::TurnOn()
{
	if (_uzelType == UT_CONTACTOR && _initialized)
	{
		_millsCheck = 0;
		_state = US_STARTING; 
#ifdef PortMonitorLog
	Serial.print(_title);
	Serial.println(".TurnOn");
#endif
	}
}
		
// ------------------------------------
void BaseUzel::TurnOff()
{
	if (_uzelType == UT_CONTACTOR && _initialized)
	{
		_millsCheck = 0;
		_state = US_STOPPING; 

#ifdef PortMonitorLog
	Serial.print(_title);
	Serial.println(".TurnOff");
#endif
	}
}

// ------------------------------------
void BaseUzel::TurnOffAlarm()
{
	if (_uzelType == UT_CONTACTOR && _initialized)
	{
		//_millsCheck = 0;
		digitalWrite(_pinContactor, 0);
		_state = US_OFF; 

	LogState("TurnOffAlarm");
	}
}


// ------------------------------------
void BaseUzel::LogState(String txt)
{
#ifdef PortMonitorLog
	Serial.print(GetStateText());
	Serial.print("; ");
	Serial.print(GetUzelTypeText());
	Serial.print("; ");
	Serial.println(txt);
#endif
}





