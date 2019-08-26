#include "Button.h"
#include "Automat.h"
#include "AutomatInverse.h"
#include "Contactor.h"
#include "ContactorInverse.h"
#include "Conveyor.h"

// ------------------------------------
Conveyor::Conveyor(String title, uint8_t pin_button_on, uint8_t pin_button_off, uint8_t pin_button_reset)
{
	_state = NOTINIT; 
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

	if(_state == ERROR)
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
	if(currUnitState != NOTINIT)
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
				if (currUnitState == ON)
					countContOn++;
				else if (currUnitState == OFF)
					countContOff++;
				else if (currUnitState == ERROR)
					{
					countContError++;
					faultTurnOff = true;
					}
				else if (currUnitState == STARTING)
					countContStarting++;
				}
			}
		else //  if (uzelType == UNIT_AUTOMAT)
			{
			countAuto++;
			if (currUnitState == ON)
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
	
//	_state = UNKNOWN;

//Core::LogState("_state0", _state);
	
	if (countContError > 0 || countAutoOff > 0 || faultTurnOff)
		_state = ERROR;
	else 
		{
		if (countContOn == countCont)
			{
			_state = ON;
//Core::LogState("_state1", _state);
			}
		else if(countContOff == countCont)
			_state = OFF;
		else if((_state == STARTING || _state == OFF) &&
				 countContOff + countContOn + countContStarting == countCont)
			{
				_state = STARTING;
Core::LogState("_state2", _state);
			}
		else if((_state == STOPPING || _state == ON) && 
			countContOff + countContOn + countContStarting == countCont)
			{
				_state = STOPPING;
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

	uint8_t prevUnitState = ON;
	uint8_t prev_i = 0;
	uint8_t currConveyorState = _state;

	if (_state == OFF || _state == STARTING)
		{
		for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
			{
			uint8_t currUnitState;
            currUnitState = Uzelki[i].CheckState();
		    if (currUnitState != NOTINIT)
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
					currConveyorState = STARTING;
					uint8_t chk_state = TurnOn_CheckUnitStates(prevUnitState, currUnitState);
				
					if (chk_state == TURN_STATE_ON)
						{
					    Uzelki[i].TurnOn();
					    }
					else if (chk_state == STARTING || chk_state == TURN_STATE_NEXT)
						{
					    }
					else // if (chk_state == TURN_STATE_ERROR)
						{
						currConveyorState = ERROR;
						break;
						}
	
					prevUnitState = currUnitState;
					prev_i = i;
					}
				else //  if (uzelType == UNIT_AUTOMAT)
					{
					if (currUnitState == OFF)
						{
						currConveyorState = ERROR;
						break;
						}
					}
 				}
			}
		}
	_state = currConveyorState; 	
	return currConveyorState; 
	
#ifdef PortMonitorLog
	Serial.println("");
#endif
}

//------------------------------
uint8_t Conveyor::TurnOn_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState)
	{
	uint8_t res = TURN_STATE_UNKNOWN; 
	if (prevUnitState == TURN_STATE_OFF && currUnitState == TURN_STATE_ON)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev OFF && curr ON; ");
#endif
		}
	else if (prevUnitState == TURN_STATE_OFF && currUnitState == TURN_STATE_STARTING)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev OFF && curr STARTING; ");
#endif
		}
	else if (prevUnitState == TURN_STATE_ERROR)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev ERROR; ");
#endif
		}
	else if (currUnitState == TURN_STATE_ERROR)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("curr ERROR; ");
#endif
		}
	
	if (res != TURN_STATE_ERROR)
		{
		if (prevUnitState == TURN_STATE_ON && currUnitState == TURN_STATE_OFF)
          	{
			res = TURN_STATE_ON;
          	}
		else if (prevUnitState == TURN_STATE_ON && currUnitState == TURN_STATE_STARTING)
			{
			res = STARTING;
			}
		else if ((prevUnitState == TURN_STATE_STARTING && currUnitState == TURN_STATE_OFF) ||
				 (prevUnitState == TURN_STATE_ON       && currUnitState == TURN_STATE_ON)  ||
				 (prevUnitState == TURN_STATE_OFF      && currUnitState == TURN_STATE_OFF))
			{
			res = TURN_STATE_NEXT;
			}
		}
	
	return res;
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
	uint8_t prevUnitState = OFF;
	uint8_t prev_i = KOLICHESTVO_UZLOV;

	if (_state == STOPPING || _state == ON)
		{
		for (int i = KOLICHESTVO_UZLOV - 1; i > -1; i--)
			{

			uint8_t currUnitState;
            currUnitState = Uzelki[i].CheckState();
            if (currUnitState != NOTINIT)
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

					uint8_t chk_state = TurnOff_CheckUnitStates(prevUnitState, currUnitState);
				
					if (chk_state == TURN_STATE_OFF)
						Uzelki[i].TurnOff();
					else if (chk_state == TURN_STATE_NEXT || chk_state == STOPPING)
						{}	  
					else // if (chk_state == TURN_STATE_ERROR)
						{
						currConveyorState = ERROR;
						break;
						}
	
					prevUnitState = currUnitState;
					prev_i = i;
					}
 				}
			}
		}
	//_state = currConveyorState; 	
#ifdef PortMonitorLog

	Serial.print("currConveyorState  ");
	Serial.print(currConveyorState);
	Serial.println("");
#endif

	return currConveyorState; 
	}

//------------------------------
uint8_t Conveyor::TurnOff_CheckUnitStates(uint8_t prevUnitState, uint8_t currUnitState)
	{
	uint8_t res = TURN_STATE_UNKNOWN; 
	if (prevUnitState == TURN_STATE_ON && currUnitState == TURN_STATE_OFF)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev ON && curr OFF; ");
#endif
		}
	else if (prevUnitState == TURN_STATE_ON && currUnitState == TURN_STATE_STOPPING)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev ON && curr STOPPING; ");
#endif
		}
	else if (prevUnitState == TURN_STATE_ERROR)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("prev ERROR; ");
#endif
		}
	else if (currUnitState == TURN_STATE_ERROR)
		{
		res = TURN_STATE_ERROR;
#ifdef PortMonitorLog
	Serial.print("curr ERROR; ");
#endif
		}
	
	if (res != TURN_STATE_ERROR)
		{
		if (prevUnitState == TURN_STATE_OFF && currUnitState == TURN_STATE_ON)
          	{
			res = TURN_STATE_OFF;
          	}
		else if (prevUnitState == TURN_STATE_OFF && currUnitState == TURN_STATE_STOPPING)
			{
			res = TURN_STATE_STOPPING;
			}
		else if ((prevUnitState == TURN_STATE_STOPPING && currUnitState == TURN_STATE_ON) || 
				 (prevUnitState == TURN_STATE_ON && currUnitState == TURN_STATE_ON)       ||
				 (prevUnitState == TURN_STATE_OFF && currUnitState == TURN_STATE_OFF))
			{
			res = TURN_STATE_NEXT;
			}
		}
	
	return res;
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
          if (currUnitState != NOTINIT)
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
	_state = OFF;

	} 
