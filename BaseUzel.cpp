#include "Arduino.h"
#include "Core.h"
#include "BaseUzel.h"

BaseUzel::BaseUzel()
{
	_state = NOTINIT;
	_title = "";
	_pinAutomat = 0;
	_pinContactor = 0;
	_unitType = 0;
	_logicType = 0;
	_timeOutOn = TURN_ON_TIMEOUT;
	_timeOutOff = TURN_OFF_TIMEOUT;
}

// ------------------------------------
BaseUzel::BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, bool UnitType, bool LogicType) 
		: BaseUzel()
{
	_state = OFF;
	_title = title;
	_pinAutomat = pinAutomat;
	_pinContactor = pinContactor;
	_unitType = UnitType;
	_logicType = LogicType;
	_initialized = true;
	if (_logicType == LOGIC_NORMAL)  
		pinMode(_pinAutomat, INPUT);
	else             // LOGIC_INVERSE
		pinMode(_pinAutomat, INPUT_PULLUP);
		
	pinMode(_pinContactor, OUTPUT);
}

// ------------------------------------
BaseUzel::BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, bool UnitType, bool LogicType, unsigned int timeOutOn, unsigned int timeOutOff) 
		: BaseUzel(title, pinAutomat, pinContactor, UnitType, LogicType)
{
	
	_timeOutOn = timeOutOn;   
	_timeOutOff = timeOutOff;   
}

// ------------------------------------
uint8_t BaseUzel::GetState()
{
	return _state;
}

// ------------------------------------
uint8_t BaseUzel::CheckState()
	{
	if (_state != NOTINIT)
		{
#ifdef PortMonitorLog
	Serial.print("  ");
	Serial.print(_title);
	Serial.print(".CheckState ");
	Serial.print(".Enter state is ");
	Serial.print(Core::GetStateText(_state));
	Serial.print(" ");
#endif

		if (_unitType == UNIT_CONTACTOR)
			{
			uint8_t stateA;
			uint8_t valueContactor = digitalRead(_pinContactor);
			if(_state == OFF)
				{
				stateA = CheckAutomatState();
				if (!(valueContactor == OFF && stateA == OFF))
					_state = ERROR;
				}	
			else if(_state == STARTING)
				{
				if(_millsCheck == 0)
					{
					digitalWrite(_pinContactor, 1);
					valueContactor = digitalRead(_pinContactor);
					_millsCheck = millis();
					}
				else if (millis() - _millsCheck > _timeOutOn)
					_state = ON;
					
				stateA = CheckAutomatState();
						
				if (!(valueContactor == ON && stateA == ON))
					_state = ERROR;
				}
					
			else if(_state == STOPPING)
				{
				if(_millsCheck == 0)
					{
					_millsCheck = millis();
					}
				else if (millis() - _millsCheck > _timeOutOff)
					{
					digitalWrite(_pinContactor, 0);
					valueContactor = digitalRead(_pinContactor);
					_state = OFF;
					}
				stateA = CheckAutomatState();
						
				if (_state == OFF)
					{
					if (!(valueContactor == OFF && stateA == OFF))
						_state = ERROR;
					}
				 }
#ifdef PortMonitorLog
	Serial.print(_title);
	Serial.print(" Result state is ");
	Serial.print(Core::GetStateText(_state));
#endif
				
			}
		else
			{
			_state = CheckAutomatState();
			}

#ifdef PortMonitorLog
	Serial.println("");
#endif

		}
	//	_prevState = _state; 		
	
	return _state;
	}

// ------------------------------------
String BaseUzel::GetTitle()
{
	return _title;
}

// ------------------------------------
String BaseUzel::GetStateTxt()
{
	String txt = GetTitle();
	uint8_t state = GetState();
    txt = txt + " is " + Core::GetStateText(state);
	return txt;
}

// ------------------------------------
void BaseUzel::TurnOn()
{
	if (_unitType == UNIT_CONTACTOR && _initialized)
	{
		_millsCheck = 0;
		_state = STARTING; 
#ifdef PortMonitorLog
	Serial.print(_title);
	Serial.println(".TurnOn");
#endif
	}
}
		
// ------------------------------------
void BaseUzel::TurnOff()
{
	if (_unitType == UNIT_CONTACTOR && _initialized)
	{
		_millsCheck = 0;
		_state = STOPPING; 

#ifdef PortMonitorLog
	Serial.print(_title);
	Serial.println(".TurnOff");
#endif
	}
}

// ------------------------------------
bool BaseUzel::GetUzelType()
{
	return _unitType; 
}


// ------------------------------------
uint8_t BaseUzel::CheckAutomatState()
	{
	uint8_t stateA;

	bool valueAutomat = digitalRead(_pinAutomat);
	
	if (_logicType == LOGIC_NORMAL)
		{
		if (valueAutomat == HIGH)
			stateA = ON;
		else 
			stateA = OFF;
		}
	else
		{
		if (valueAutomat == HIGH)
			stateA = OFF;
		else
			stateA = ON;
		}
#ifdef PortMonitorLog
/*	if (_logicType == LOGIC_NORMAL)
		Serial.print("LOGIC_NORMAL; ");
	else
		Serial.print("LOGIC_INVERSE; ");*/
	
	Serial.print("automat is ");
	Serial.print(Core::GetStateText(stateA));
	Serial.print(" ");
#endif
	return stateA;
	}
