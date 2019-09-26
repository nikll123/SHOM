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

ConveyorState Conveyor::GetState()
{
	return _state;
}

void Conveyor::LogText(String txt1)
{
#ifdef PortMonitorLog
	Core::LogText(txt1); 	
#endif
} 

void Conveyor::LogTextln(String txt1)
{
#ifdef PortMonitorLog
	Core::LogTextLn(txt1); 	
#endif
} 

// ------------------------------------
ConveyorState2 Conveyor::CheckState()
	{
	ConveyorState2 s;
	s.ValueOld = _state; 
	
	if(_state == CS_ERROR)
		{
		s.ValueNew = _state; 
		return s;
		}
		
	uint8_t countAuto = 0;
	uint8_t countAutoOn = 0;
	uint8_t countAutoOff = 0;
	uint8_t countCont = 0;
	uint8_t countContOn = 0;
	uint8_t countContOff = 0;
	uint8_t countContError = 0;
	uint8_t countContStarting = 0;

	bool faultTurnOff = false;     //  flag to turn off in fault situation
	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
		{
		UzelState currUzelState = Uzelki[i].GetState();
		UzelInfo uzelInfo = Uzelki[i].GetInfo();
		if(currUzelState != US_NOTINIT)
			{
			//UzelType uzelType = Uzelki[i].GetUzelType();  
			if (uzelInfo.Type == UT_CONTACTOR)
				{
				countCont++;
				if(faultTurnOff)
					{
					Uzelki[i].TurnOffAlarm();
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
			else //  if (uzelInfo.UzelType == UNIT_AUTOMAT)
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
	String txt = "";
	
	txt += " count: Auto";
	txt += " Off="; 
	txt += String(countAutoOff); 
	txt += " On="; 
	txt += String(countAutoOn); 

	txt += " ; Cont"; 
	txt += " Off="; 
	txt += String(countContOff); 
	txt += " Starting="; 
	txt += String(countContStarting); 
	txt += " On="; 
	txt += String(countContOn); 

	txt += " Error="; 
	txt += String(countContError); 

	Core::LogTextLn(txt);
#endif
	

	if (countCont + countAuto == 0) 
	    _state = CS_NOTINIT;
	else if (countContError > 0 || countAutoOff > 0 || faultTurnOff)
		_state = CS_ERROR;
	else 
		{
		if (countContOn == countCont)
			{
			_state = CS_ON;
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
	
	s.ValueNew = _state; 
	return s;
	
	}

// ------------------------------------
/*String Conveyor::GetStateTxt()
{
	String txt = GetTitle();
    txt = txt + " state is " + Core::GetConveyorStateText(_state);
	return txt;
} */

// ------------------------------------
/*String Conveyor::GetTitle()
{
	return _title;
} */


// ------------------------------------
ConveyorState Conveyor::TurnOn()
{
	
	ConveyorState currConveyorState = _state;
	UzelState prevUzelState = US_ON;
	uint8_t prev_i = 0;

	if (currConveyorState == US_OFF || currConveyorState == US_STARTING || _state == CS_STOPPING)
		{
		bool firstContactor = true;
		for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
			{
			UzelState currUzelState;
            currUzelState = Uzelki[i].GetState();
		    if (currUzelState != US_NOTINIT)
				{
				UzelInfo uzelInfo = Uzelki[i].GetInfo();  

				if (uzelInfo.Type == UT_CONTACTOR)
					{
#ifdef PortMonitorLog
	String txt = "";
	if(!firstContactor)
	{
		UzelInfo uzelInfoPrev = Uzelki[prev_i].GetInfo();  
		txt += uzelInfoPrev.Title; 
		txt += ": prevUzelState="; 
		txt += Core::GetUzelStateText(prevUzelState);
		txt += "; ";  
	}
	txt += uzelInfo.Title;
	txt += " currUzelState=";
	txt += Core::GetUzelStateText(currUzelState);
	txt += "; ";
	Core::LogTextLn(txt);
	firstContactor = false;
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
	
	return currConveyorState; 
}

//------------------------------
TurnOnUzelAction Conveyor::TurnOn_TurnOn_NextAction(UzelState prevUzelState, UzelState currUzelState)
	{
	TurnOnUzelAction res = TON_UA_UNKNOWN; 
	if (prevUzelState == US_OFF && currUzelState == US_ON)
		{
		res = TON_UA_ERROR_01;
		Core::LogTextLn("prev OFF && curr ON; ");
		}
	else if (prevUzelState == US_OFF && currUzelState == US_STARTING)
		{
		res = TON_UA_ERROR_02;
		Core::LogTextLn("prev OFF && curr STARTING; ");
		}
	else if (prevUzelState <= US_ERROR)
		{
		res = TON_UA_ERROR_03;
		Core::LogTextLn("prev ERROR; ");
		}
	else if (currUzelState <= US_ERROR)
		{
		res = TON_UA_ERROR_04;
		Core::LogTextLn("curr ERROR; ");
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

	ConveyorState currConveyorState = _state;
	UzelState prevUzelState = US_OFF;  // assume that the state of the pre-first imaginary uzel is OFF  
	uint8_t prev_i = KOLICHESTVO_UZLOV;

	if (_state == CS_STOPPING || _state == CS_ON || _state == CS_STARTING)
		{
		bool firstContactor = true;
		for (int i = KOLICHESTVO_UZLOV - 1; i > -1; i--)
			{

			uint8_t currUzelState;
            currUzelState = Uzelki[i].GetState();
            if (currUzelState != US_NOTINIT)
				{
				UzelInfo uzelInfo = Uzelki[i].GetInfo();  

				if (uzelInfo.Type == UT_CONTACTOR)
					{
#ifdef PortMonitorLog
	String txt = "";
	if(!firstContactor)
	{
		UzelInfo uzelInfoPrev = Uzelki[prev_i].GetInfo();  
		txt += uzelInfoPrev.Title;
		txt += ": prevUzelState=";
		txt += Core::GetUzelStateText(prevUzelState);
		txt += "; ";
	}
	txt += uzelInfo.Title;
	txt += " currUzelState=";
	txt += Core::GetUzelStateText(currUzelState);
	txt += "; ";
	Core::LogTextLn(txt);
	firstContactor = false;
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
	
	return currConveyorState; 
	}

	
//------------------------------
TurnOffUzelAction Conveyor::TurnOff_NextAction(UzelState prevUzelState, UzelState currUzelState)
	{
	TurnOffUzelAction nextAction = TOFF_UA_NONE; 
	if (prevUzelState == US_ON && currUzelState == US_OFF)
		{
		nextAction = TOFF_UA_ERROR_01;
		Core::LogTextLn("TOFF_UA_ERROR_01 ");
		}
	else if (prevUzelState == US_ON && currUzelState == US_STOPPING)
		{
		nextAction = TOFF_UA_ERROR_02;
		Core::LogTextLn("TOFF_UA_ERROR_02 ");
		}
	else if (prevUzelState == US_ERROR)
		{
		nextAction = TOFF_UA_ERROR_03;
		Core::LogTextLn("TOFF_UA_ERROR_03 ");
		}
	else if (currUzelState == US_ERROR)
		{
		nextAction = TOFF_UA_ERROR_04;
		Core::LogTextLn("TOFF_UA_ERROR_04 ");
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
	
// ---------------------------------------------
// BBB   U   U  TTTTT  TTTTT   OOO   N   N   SSS
// B  B  U   U    T      T    O   O  NN  N  S   
// BBB   U   U    T      T    O   O  N N N   SS
// B  B  U   U    T      T    O   O  N  NN     S
// BBB    UUU     T      T     OOO   N   N  SSS
// ---------------------------------------------
bool Conveyor::ButtonOnIsPressed()
{
	KeyState2 s = _buttonOn.CheckState();    
	return (s.ValueOld == KS_OFF && s.ValueNew == KS_ON);
}

bool Conveyor::ButtonOffIsPressed()
{    
	KeyState2 s = _buttonOff.CheckState();    
	return (s.ValueOld == KS_OFF && s.ValueNew == KS_ON);
}

bool Conveyor::ButtonResetIsPressed()
{        
	KeyState2 s = _buttonReset.CheckState();    
	return (s.ValueOld == KS_OFF && s.ValueNew == KS_ON);
}

// ---------------------------------------------


// ------------------------------------
void Conveyor::Reset()
	{        
	for (uint8_t i = 0; i < KOLICHESTVO_UZLOV; i++)
		{
		uint8_t currUzelState;
		UzelInfo uzelInfo = Uzelki[i].GetInfo();  
		if (uzelInfo.State != US_NOTINIT)
			{
			if (uzelInfo.Type == UT_CONTACTOR)
				{
				Uzelki[i].TurnOffAlarm();
				}
			}
		}
	_state = CS_OFF;
	} 

//------------------------------
String Conveyor::GetTime()
{
	return Core::GetTime();
}