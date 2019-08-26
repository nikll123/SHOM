#include "Arduino.h"
#include "Core.h"
#include "BaseUzel.h"

BaseUzel::BaseUzel()
{
	_state = STATE_NOTINIT;
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
	_state = STATE_OFF;
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
	uint8_t stateA;
	
#ifdef PortMonitorLog
	Serial.print("  ");
	Serial.print(_title);
	Serial.print(".CheckState ");
#endif
		bool valueAutomat = digitalRead(_pinAutomat);
		
		if (_logicType == LOGIC_NORMAL)
			{
			if (valueAutomat == HIGH)
				stateA = STATE_ON;
			else 
				stateA = STATE_OFF;
			}
		else
			{
			if (valueAutomat == HIGH)
				stateA = STATE_OFF;
			else
				stateA = STATE_ON;
			}
#ifdef PortMonitorLog
	if (_logicType == LOGIC_NORMAL)
		Serial.print("LOGIC_NORMAL; ");
	else
		Serial.print("LOGIC_INVERSE; ");
	
	Serial.print("automat is ");
	Serial.print(Core::GetStateText(stateA));
	Serial.print("; ");
#endif
			
		if (_unitType == UNIT_CONTACTOR)
			{
			uint8_t valueContactor = digitalRead(_pinContactor);
			if(_state == STATE_OFF)
				{
				if (!(valueContactor == STATE_OFF && stateA == STATE_OFF))
					_state = STATE_ERROR;
				}	
			else if(_state == STATE_STARTING)
				{
				if(_millsCheck == 0)
					{
					digitalWrite(_pinContactor, 1);
					valueContactor = digitalRead(_pinContactor);
					stateA = digitalRead(_pinAutomat);
					_millsCheck = millis();
					}
				else if (millis() - _millsCheck > _timeOutOn)
					_state = STATE_ON;
						
				if ((_state == STATE_ON || _state == STATE_STARTING)  &&
					!(valueContactor == STATE_ON && stateA == STATE_ON))
					_state = STATE_ERROR;
				}
					
			else if(_state == STATE_STOPPING)
				{
				if(_millsCheck == 0)
					_millsCheck = millis();
				else if (millis() - _millsCheck > _timeOutOff)
					{
					//digitalWrite(_pinContactor, 0);
					_state = STATE_OFF;
					}
						
				if (!(_state == STATE_STOPPING && valueContactor == STATE_ON && stateA == STATE_ON))
					_state = STATE_ERROR;
				else if (!(_state == STATE_OFF && valueContactor == STATE_OFF && stateA == STATE_OFF))
					_state = STATE_ERROR;
				}
#ifdef PortMonitorLog
	Serial.print(_title);
	Serial.print(".State is ");
	Serial.print(Core::GetStateText(_state));
#endif
				
			}
		else
			{
				_state = stateA;
			}
#ifdef PortMonitorLog
	Serial.println("");
#endif
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
		//digitalWrite(_pinContactor, 1);
		_state = STATE_STARTING; 
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
		//digitalWrite(_pinContactor, 0);
		_state = STATE_STOPPING; 

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
