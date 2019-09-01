#include "ButtonInverse.h"
#include "Automat.h"
#include "AutomatInverse.h"
#include "Contactor.h"
#include "ContactorInverse.h"
#include "Conveyor.h"

BaseButton		_buttonOn; 
BaseButton		_buttonOff;
BaseButton		_buttonReset;


// ------------------------------------
Conveyor::Conveyor(String title, uint8_t pin_button_on, uint8_t pin_button_off, uint8_t pin_button_reset)
{
	_state = CS_UNKNOWN; 
	_title = title;
	
	_buttonOn = ButtonInverse ("Button ON", pin_button_on);
	_buttonOff = ButtonInverse ("Button OFF", pin_button_off);
	_buttonReset = ButtonInverse ("Button RESET", pin_button_reset);
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
//void Conveyor::GetUzelStates(uint8_t * result)
//{
//	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
//	{
//		result[i] = Uzelki[i].GetState();
//	}
//}

ConveyorState Conveyor::GetState()
{
	return _state;
}

void Conveyor::LogState(String txt)
{
#ifdef PortMonitorLog
	Serial.print(GetStateTxt());
	Serial.print("; ");
	Serial.println(txt);
#endif
}


// ------------------------------------
ConveyorState Conveyor::CheckState()
{

	if(_state == CS_ERROR)
		return _state;
		
	uint8_t countAuto = 0;
	uint8_t countCont = 0;
	uint8_t countAutoOn = 0;
	uint8_t countContOn = 0;
	uint8_t countAutoOff = 0;
	uint8_t countContOff = 0;
	uint8_t countContError = 0;
	uint8_t countContStarting = 0;

	LogState("Enter state");

	bool faultTurnOff = false;     //  flag to turn off in fault situation
	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
	{
	UzelState currUzelState = Uzelki[i].CheckState();
	if(currUzelState != US_NOTINIT)
		{
		UzelType uzelType = Uzelki[i].GetUzelType();  
		if (uzelType == UT_CONTACTOR)
			{
			countCont++;
			if(faultTurnOff)
				{
				Uzelki[i].TurnOff();
				countContOff++;
				}
			else
				{
				if (currUzelState == US_ON)
					countContOn++;
				else if (currUzelState == US_OFF)
					countContOff++;
				else if (currUzelState == US_ERROR)
					{
					countContError++;
					faultTurnOff = true;
					}
				else if (currUzelState == US_STARTING)
					countContStarting++;
				}
			}
		else //  if (uzelType == UNIT_AUTOMAT)
			{
			countAuto++;
			if (currUzelState == US_ON)
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
		_state = CS_ERROR;
	else 
		{
		if (countContOn == countCont)
			{
			_state = CS_ON;
//Core::LogState("_state1", _state);
			}
		else if(countContOff == countCont)
			_state = CS_OFF;
		else if((_state == CS_STARTING || _state == CS_OFF) &&
				 countContOff + countContOn + countContStarting == countCont)
			{
				_state = CS_STARTING;
			}
		else if((_state == CS_STOPPING || _state == CS_ON) && 
			countContOff + countContOn + countContStarting == countCont)
			{
				_state = CS_STOPPING;
			}
		else
			{
			}
		}
	
	LogState("Result state");
	
	return _state;	
}

// ------------------------------------
String Conveyor::GetStateTxt()
{
	String txt = GetTitle();
    txt = txt + " state is " + Core::GetConveyorStateText(_state);
	return txt;
}

// ------------------------------------
String Conveyor::GetTitle()
{
	return _title;
}


// ------------------------------------
ConveyorState Conveyor::TurnOn()
{

	LogState("TurnOn Enter state"); 
	
	ConveyorState currConveyorState = _state;
	UzelState prevUzelState = US_ON;
	uint8_t prev_i = 0;

	if (currConveyorState == US_OFF || currConveyorState == US_STARTING)
		{
		bool firstContactor = true;
		for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
			{
			UzelState currUzelState;
            currUzelState = Uzelki[i].CheckState();
		    if (currUzelState != US_NOTINIT)
				{
				uint8_t uzelType = Uzelki[i].GetUzelType();  

				if (uzelType == UT_CONTACTOR)
					{
#ifdef PortMonitorLog
	if(!firstContactor)
	{
		Serial.print(Uzelki[prev_i].GetTitle());
		Serial.print(": prevUzelState=");
		Serial.print(Core::GetUzelStateText(prevUzelState));
		Serial.print("; ");
	}
	firstContactor = false;
	Serial.print(Uzelki[i].GetTitle());
	Serial.print(" currUzelState=");
	Serial.print(Core::GetUzelStateText(currUzelState));
	Serial.println("; ");
#endif
					currConveyorState = CS_STARTING;
					TurnOnUzelAction nextAction = TurnOn_TurnOn_NextAction(prevUzelState, currUzelState);
				
					if (nextAction == TON_UA_ON)
						{
					    Uzelki[i].TurnOn();
						currConveyorState = CS_STARTING;
					    }
					else if (nextAction == TON_UA_ERROR)
						{
						currConveyorState = CS_ERROR;
						break;
						}
	
					prevUzelState = currUzelState;
					prev_i = i;
					}
				else //  if (uzelType == UNIT_AUTOMAT)
					{
					if (currUzelState == US_OFF)
						{
						currConveyorState = CS_ERROR;
						break;
						}
					}
 				}
			}
		}
	_state = currConveyorState; 	
	
	LogState("TurnOn Exit state"); 
	return currConveyorState; 
}

//------------------------------
TurnOnUzelAction Conveyor::TurnOn_TurnOn_NextAction(UzelState prevUzelState, UzelState currUzelState)
	{
	TurnOnUzelAction res = TON_UA_UNKNOWN; 
	if (prevUzelState == US_OFF && currUzelState == US_ON)
		{
		res = TON_UA_ERROR_01;
#ifdef PortMonitorLog
	Serial.print("prev OFF && curr ON; ");
#endif
		}
	else if (prevUzelState == US_OFF && currUzelState == US_STARTING)
		{
		res = TON_UA_ERROR_02;
#ifdef PortMonitorLog
	Serial.print("prev OFF && curr STARTING; ");
#endif
		}
	else if (prevUzelState == US_ERROR)
		{
		res = TON_UA_ERROR_03;
#ifdef PortMonitorLog
	Serial.print("prev ERROR; ");
#endif
		}
	else if (currUzelState == US_ERROR)
		{
		res = TON_UA_ERROR_03;
#ifdef PortMonitorLog
	Serial.print("curr ERROR; ");
#endif
		}
	
	if (res != TON_UA_ERROR)
		{
		if (prevUzelState == US_ON && currUzelState == US_OFF)
          	{
			res = TON_UA_ON;
          	}
		else if ((prevUzelState == US_ON 		&& currUzelState == US_STARTING)	||
				 (prevUzelState == US_STARTING 	&& currUzelState == US_OFF)			||
				 (prevUzelState == US_ON		&& currUzelState == US_ON)			||
				 (prevUzelState == US_OFF		&& currUzelState == US_OFF))
			{
			res = TON_UA_NONE;
			}
		}
	
	return res;
	}


// ------------------------------------
ConveyorState Conveyor::TurnOff()
	{

	LogState("TurnOff Enter state");
	 
	ConveyorState currConveyorState = _state;
	UzelState prevUzelState = US_OFF;  // assume that the state of the pre-first imaginary uzel is OFF  
	uint8_t prev_i = KOLICHESTVO_UZLOV;

	if (_state == CS_STOPPING || _state == CS_ON)
		{
		bool firstContactor = true;
		for (int i = KOLICHESTVO_UZLOV - 1; i > -1; i--)
			{

			uint8_t currUzelState;
            currUzelState = Uzelki[i].CheckState();
            if (currUzelState != US_NOTINIT)
				{
				uint8_t uzelType = Uzelki[i].GetUzelType();  

				if (uzelType == UT_CONTACTOR)
					{
#ifdef PortMonitorLog
	if(!firstContactor)
	{
		Serial.print(Uzelki[prev_i].GetTitle());
		Serial.print(": prevUzelState=");
		Serial.print(Core::GetUzelStateText(prevUzelState));
		Serial.print("; ");
	}
	firstContactor = false;
	Serial.print(Uzelki[i].GetTitle());
	Serial.print(" currUzelState=");
	Serial.print(Core::GetUzelStateText(currUzelState));
	Serial.println("; ");
#endif

					TurnOffUzelAction nextAction = TurnOff_NextAction(prevUzelState, currUzelState);
				
					if (nextAction == TOFF_UA_OFF)
						{
						Uzelki[i].TurnOff();
						_state = CS_STOPPING;
						}
					else if (nextAction == TOFF_UA_NONE)
						{}	  
					else 
						{
						currConveyorState = CS_ERROR_02;
						break;
						}
	
					prevUzelState = currUzelState;
					prev_i = i;
					}
 				}
			}
		}
	//_state = currConveyorState; 	

	LogState("TurnOff Exit state");
	
	return currConveyorState; 
	}

	
//------------------------------
TurnOffUzelAction Conveyor::TurnOff_NextAction(UzelState prevUzelState, UzelState currUzelState)
	{
	TurnOffUzelAction nextAction = TOFF_UA_NONE; 
	if (prevUzelState == US_ON && currUzelState == US_OFF)
		{
		nextAction = TOFF_UA_ERROR_01;
#ifdef PortMonitorLog
	Serial.print("TOFF_UA_ERROR_01 ");
#endif
		}
	else if (prevUzelState == US_ON && currUzelState == US_STOPPING)
		{
		nextAction = TOFF_UA_ERROR_02;
#ifdef PortMonitorLog
	Serial.print("TOFF_UA_ERROR_02 ");
#endif
		}
	else if (prevUzelState == US_ERROR)
		{
		nextAction = TOFF_UA_ERROR_03;
#ifdef PortMonitorLog
	Serial.print("TOFF_UA_ERROR_03 ");
#endif
		}
	else if (currUzelState == US_ERROR)
		{
		nextAction = TOFF_UA_ERROR_04;
#ifdef PortMonitorLog
	Serial.print("TOFF_UA_ERROR_04 ");
#endif
		}
	
	if (nextAction == TOFF_UA_NONE)
		{
		if (prevUzelState == US_OFF && currUzelState == US_ON)
          	{
			nextAction = TOFF_UA_OFF;
          	}
		if ((prevUzelState == US_OFF 		&& currUzelState == US_STOPPING) ||
			(prevUzelState == US_STOPPING 	&& currUzelState == US_ON)       ||
			(prevUzelState == US_ON 		&& currUzelState == US_ON)       ||
			(prevUzelState == US_OFF 		&& currUzelState == US_OFF))
          	{
			nextAction = TOFF_UA_NONE;
          	}
		}
	
	return nextAction;
	}	


// ------------------------------------
bool Conveyor::ButtonOnIsPressed()
{    
	return (KS_FRONT == _buttonOn.CheckState());
}


// ------------------------------------
bool Conveyor::ButtonOffIsPressed()
{    
	return (KS_FRONT == _buttonOff.CheckState());
}

// ------------------------------------
bool Conveyor::ButtonResetIsPressed()
{        
	return (KS_FRONT == _buttonReset.CheckState());
}

// ------------------------------------
void Conveyor::Reset()
	{        
	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
		{
		uint8_t currUzelState;
          currUzelState = Uzelki[i].GetState();
          if (currUzelState != US_NOTINIT)
			{
			uint8_t uzelType = Uzelki[i].GetUzelType();  

			if (uzelType == UT_CONTACTOR)
				{
				Uzelki[i].TurnOff();
				Uzelki[i].LogState(" Reset Turn off");			
				}
			}
		}
	_state = CS_OFF;

	} 
