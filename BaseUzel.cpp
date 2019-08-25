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
}

// ------------------------------------
BaseUzel::BaseUzel(String title, uint8_t pinAutomat, uint8_t pinContactor, bool UnitType, bool LogicType) //: Core()
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
uint8_t BaseUzel::GetState()
{
	uint8_t s = _GetState(0);
	return s;	
}


// ------------------------------------
uint8_t BaseUzel::_GetState(uint8_t doLog)
{
	uint8_t stateA;
	
	if (_initialized)
	{
#ifdef PortMonitorLog
	Serial.print("  ");
	Serial.print(_title);
	Serial.print("._GetState ");
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
				if (millis() - _millsCheck < TURN_ON_TIMEOUT)
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
				if (millis() - _millsCheck < TURN_OFF_TIMEOUT)
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
	_GetStateTxt(0);
}

// ------------------------------------
String BaseUzel::GetStateTxtLog()
{
	return _GetStateTxt(1);
}

// ------------------------------------
String BaseUzel::_GetStateTxt(uint8_t doLog)
{
	//if (doLog) Serial.println("_GetStateTxt");
	String txt = GetTitle();
	uint8_t state = _GetState(doLog);
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
