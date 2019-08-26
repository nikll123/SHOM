#include "Button.h"
#include "Automat.h"
#include "AutomatInverse.h"
#include "Contactor.h"
#include "ContactorInverse.h"
#include "Conveyor.h"

// ------------------------------------
Conveyor::Conveyor(String title, uint8_t pin_button_on, uint8_t pin_button_off, uint8_t pin_button_reset)
{
	_state = STATE_NOTINIT; 
	_title = title;
	
	_buttonOn.InitPin(pin_button_on, "Button ON");
	_buttonOff.InitPin(pin_button_off, "Button OFF");
	_buttonReset.InitPin(pin_button_reset, "Button RESET");
}
	
// ------------------------------------
void Conveyor::SetupUzelAutomat(uint8_t index, String title, uint8_t pinAutomat)
{
	Automat automat1 (title, pinAutomat);	
    Uzelki[index]= automat1;  
}

// ------------------------------------
void Conveyor::SetupUzelAutomatInverse(uint8_t index, String title, uint8_t pinAutomat)
{
	AutomatInverse automat1 (title, pinAutomat);	
    Uzelki[index]= automat1;  
}

// ------------------------------------
void Conveyor::SetupUzelContactor(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor)
{
	Contactor contactor1 (title, pinAutomat, pinContactor);	
    Uzelki[index]= contactor1;  
}

void Conveyor::SetupUzelContactor(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff)
{
	Contactor contactor1 (title, pinAutomat, pinContactor, timeOutOn, timeOutOff);	
    Uzelki[index]= contactor1;  
}

// ------------------------------------
void Conveyor::SetupUzelContactorInverse(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor)
{
	ContactorInverse contactor1 (title, pinAutomat, pinContactor);	
    Uzelki[index]= contactor1;  
}

void Conveyor::SetupUzelContactorInverse(uint8_t index, String title, uint8_t pinAutomat, uint8_t pinContactor, unsigned int timeOutOn, unsigned int timeOutOff)
{
	ContactorInverse contactor1 (title, pinAutomat, pinContactor, timeOutOn, timeOutOff);	
    Uzelki[index]= contactor1;  
}

// ------------------------------------
void Conveyor::GetUzelStates(uint8_t * result)
{
	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
	{
		result[i] = Uzelki[i].GetState();
	}
}

// ------------------------------------
uint8_t Conveyor::GetState()
{

	if(_state == STATE_ERROR)
		return _state;
		
	uint8_t countAuto = 0;
	uint8_t countCont = 0;
	uint8_t countAutoOn = 0;
	uint8_t countContOn = 0;
	uint8_t countAutoOff = 0;
	uint8_t countContOff = 0;
	uint8_t countContError = 0;
	uint8_t countContStarting = 0;

#ifdef PortMonitorLog
	Serial.print("GetState Enter state: ");
	Serial.println(GetStateTxt());
#endif
	bool faultTurnOff = false;
	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
	{
	uint8_t currUnitState = Uzelki[i].CheckState();
	if(currUnitState != STATE_NOTINIT)
		{
		uint8_t uzelType = Uzelki[i].GetUzelType();  
		if (uzelType == UNIT_CONTACTOR)
			{
			countCont++;
			if(faultTurnOff)
				{
				Uzelki[i].TurnOff();
				countContOff++;
				}
			else
				{
				if (currUnitState == STATE_ON)
					countContOn++;
				else if (currUnitState == STATE_OFF)
					countContOff++;
				else if (currUnitState == STATE_ERROR)
					{
					countContError++;
					faultTurnOff = true;
					}
				else if (currUnitState == STATE_STARTING)
					countContStarting++;
				}
			}
		else //  if (uzelType == UNIT_AUTOMAT)
			{
			countAuto++;
			if (currUnitState == STATE_ON)
				countAutoOn++;
			else
				{
				countAutoOff++;
				faultTurnOff = true;
				}
			}
		}
	}
#ifdef PortMonitorLog
	Serial.print(" count: Auto");
	Serial.print(" On=");
	Serial.print(countAutoOn);
	Serial.print(" Off=");
	Serial.print(countAutoOff);

	Serial.print(" count: Cont");
	Serial.print(" Off=");
	Serial.print(countContOff);
	Serial.print(" Starting=");
	Serial.print(countContStarting);
	Serial.print(" On=");
	Serial.print(countContOn);
	Serial.print(" Error=");
	Serial.print(countContError);
	Serial.println("");
#endif
	
//	_state = STATE_UNKNOWN;

Core::LogState("_state0", _state);
	
	if (countContError > 0 || countAutoOff > 0 || faultTurnOff)
		_state = STATE_ERROR;
	else 
		{
		if (countContOn == countCont)
			{
			_state = STATE_ON;
Core::LogState("_state1", _state);
			}
		else if(countContOff == countCont)
			_state = STATE_OFF;
		else if((_state == STATE_STARTING || _state == STATE_OFF) &&
				 countContOff + countContOn + countContStarting == countCont)
			{
				_state = STATE_STARTING;
Core::LogState("_state2", _state);
			}
		else if((_state == STATE_STOPPING || _state == STATE_ON) && 
			countContOff + countContOn + countContStarting == countCont)
			{
				_state = STATE_STOPPING;
Core::LogState("_state3", _state);
			}
		else
			{
Core::LogState("_state4", _state);
			}
		}
	
	return _state;	
}

// ------------------------------------
String Conveyor::GetStateTxt()
{
	String txt = GetTitle();
    txt = txt + " state is " + Core::GetStateText(_state);
	return txt;
}

// ------------------------------------
String Conveyor::GetTitle()
{
	return _title;
}


// ------------------------------------
uint8_t Conveyor::TurnOn()
{
#ifdef PortMonitorLog
	Serial.print("TurnOn Enter state: ");
	Serial.println(GetStateTxt());
	bool firstContactor = true;
#endif

	uint8_t prevUnitState = STATE_ON;
	uint8_t prev_i = 0;
	uint8_t currConveyorState = _state;

	if (_state == STATE_OFF || _state == STATE_STARTING)
		{
		for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
			{
			uint8_t currUnitState;
            currUnitState = Uzelki[i].GetState();
            if (currUnitState != STATE_NOTINIT)
				{
				uint8_t uzelType = Uzelki[i].GetUzelType();  

				if (uzelType == UNIT_CONTACTOR)
					{
#ifdef PortMonitorLog
	if(!firstContactor)
	{
		Serial.print(Uzelki[prev_i].GetTitle());
		Serial.print(": prevUnitState=");
		Serial.print(Core::GetStateText(prevUnitState));
		Serial.print("; ");
	}
	firstContactor = false;
	Serial.print(Uzelki[i].GetTitle());
	Serial.print(" currUnitState=");
	Serial.print(Core::GetStateText(currUnitState));
	Serial.println("; ");
#endif
					uint8_t chk_state = Core::TurnOn_CheckUnitStates(prevUnitState, currUnitState);
				
					if (chk_state == TURN_ON_ON)
					    Uzelki[i].TurnOn();
					else if (chk_state == TURN_ON_WAIT || chk_state == TURN_ON_NEXT)
						{}
					else // if (chk_state == TURN_ON_ERROR)
						{
						currConveyorState = STATE_ERROR;
						break;
						}
	
					prevUnitState = currUnitState;
					prev_i = i;
					}
				else //  if (uzelType == UNIT_AUTOMAT)
					{
					if (currUnitState == STATE_OFF)
						{
						currConveyorState = STATE_ERROR;
						break;
						}
					}
 				}
			}
		}
	//_state = currConveyorState; 	
	return currConveyorState; 
	
#ifdef PortMonitorLog
	Serial.println("");
#endif
}

// ------------------------------------
uint8_t Conveyor::TurnOff()
{
#ifdef PortMonitorLog
	Serial.print("TurnOff Enter state: ");
	Serial.println(GetStateTxt());
	bool firstContactor = true;
#endif
	uint8_t currConveyorState = _state;
	uint8_t prevUnitState = STATE_OFF;
	uint8_t prev_i = KOLICHESTVO_UZLOV;

	if (_state == STATE_STOPPING || _state == STATE_ON)
		{
		for (char i = KOLICHESTVO_UZLOV - 1; i > -1; i--)
			{
			uint8_t currUnitState;
            currUnitState = Uzelki[i].GetState();
            if (currUnitState != STATE_NOTINIT)
				{
				uint8_t uzelType = Uzelki[i].GetUzelType();  

				if (uzelType == UNIT_CONTACTOR)
					{
#ifdef PortMonitorLog
	if(!firstContactor)
	{
		Serial.print(Uzelki[prev_i].GetTitle());
		Serial.print(": prevUnitState=");
		Serial.print(Core::GetStateText(prevUnitState));
		Serial.print("; ");
	}
	firstContactor = false;
	Serial.print(Uzelki[i].GetTitle());
	Serial.print(" currUnitState=");
	Serial.print(Core::GetStateText(currUnitState));
	Serial.println("; ");
#endif
					uint8_t chk_state = Core::TurnOff_CheckUnitStates(prevUnitState, currUnitState);
				
					if (chk_state == TURN_ON_OFF)
						Uzelki[i].TurnOff();
					else if (chk_state == TURN_ON_NEXT || chk_state == TURN_ON_WAIT)
						{}	  
					else // if (chk_state == TURN_ON_ERROR)
						{
						currConveyorState = STATE_ERROR;
						break;
						}
	
					prevUnitState = currUnitState;
					prev_i = i;
					}
 				}
			}
		}
	//_state = currConveyorState; 	
	return currConveyorState; 
	
#ifdef PortMonitorLog
	Serial.println("");
#endif
}

// ------------------------------------
bool Conveyor::ButtonOnIsPressed()
{    
	return _buttonOn.GetState();
}

// ------------------------------------
bool Conveyor::ButtonOffIsPressed()
{    
	return _buttonOff.GetState();
}

// ------------------------------------
bool Conveyor::ButtonResetIsPressed()
{        
	return _buttonReset.GetState();
} 

// ------------------------------------
void Conveyor::Reset()
{        
	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
		{
		uint8_t currUnitState;
          currUnitState = Uzelki[i].GetState();
          if (currUnitState != STATE_NOTINIT)
			{
			uint8_t uzelType = Uzelki[i].GetUzelType();  

			if (uzelType == UNIT_CONTACTOR)
				{
				Uzelki[i].TurnOff();
#ifdef PortMonitorLog
	Serial.print(Uzelki[i].GetTitle());
	Serial.print(" Reset Turn off");
#endif
				}
			}
		}
	_state = STATE_OFF;

} 
