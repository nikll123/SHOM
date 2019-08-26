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
	uint8_t stateA;
	
	if (_initialized)
	{
#ifdef PortMonitorLog
	Serial.print("  ");
	Serial.print(_title);
	Serial.print(".GetState ");
#endif
		bool valueAutomat = digitalRead(_pinAutomat);
		
		if (_logicType == LOGIC_NORMAL)
			{
#ifdef PortMonitorLog
	Serial.print("LOGIC_NORMAL; ");
#endif
			if (valueAutomat == HIGH)
				stateA = STATE_ON;
			else 
				stateA = STATE_OFF;
			}
		else
			{
#ifdef PortMonitorLog
	Serial.print("LOGIC_INVERSE; ");
#endif
			if (valueAutomat == HIGH)
				stateA = STATE_OFF;
			else
				stateA = STATE_ON;
			}
			
#ifdef PortMonitorLog
	Serial.print("automat is ");
	Serial.print(Core::GetStateText(stateA));
	Serial.print("; ");
#endif
			
		if (_unitType == UNIT_CONTACTOR)
			{
#ifdef PortMonitorLog
	Serial.print("CONTACTOR; ");
#endif
			
			bool valueContactor = digitalRead(_pinContactor);
			if (valueContactor == HIGH && stateA == STATE_ON)
				{
#ifdef PortMonitorLog
	Serial.print("HIGH STATE_ON; ");
#endif
				if (_millsCheck != 0 && millis() - _millsCheck < _timeOutOn)
					_state = STATE_STARTING;
				else
					{
					_state = STATE_ON;
					_millsCheck = 0;
					}
				}
			else if (valueContactor == LOW && stateA == STATE_OFF)
				{
#ifdef PortMonitorLog
	Serial.print("LOW STATE_OFF; ");
#endif
				if (_millsCheck != 0 && millis() - _millsCheck < _timeOutOff)
					_state = STATE_STOPPING;
				else
					{
					_state = STATE_OFF;
					_millsCheck = 0;
					}
				}
			else
				{
#ifdef PortMonitorLog
	Serial.print("else; ");
#endif
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
			
	}
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
		_millsCheck = millis();
		digitalWrite(_pinContactor, 1);
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
		_millsCheck = millis();
		digitalWrite(_pinContactor, 0);

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
