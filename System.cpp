#include "System.h"

// ------------------------------------
System::System() : System("DummySystem", 0, 0, 0)
	{
	}

System::System(String title, uint8_t pinBtnOn, uint8_t pinBtnOff, uint8_t pinBtnReset) : Unit(title, UT_SYSTEM)
	{
	Init();
	BtnOn = SetupButton("_BtnOn", pinBtnOn);
	BtnOff = SetupButton("_BtnOff", pinBtnOff);
	BtnReset = SetupButton("_BtnReset", pinBtnReset);
	}
	
// ------------------------------------
PinIn System::SetupButton(String suffix,uint8_t pin)
	{
	PinIn btn = PinIn(_title + suffix, pin);
	btn.LogicInverse();
	return btn ; 
	}
	
// ------------------------------------
void System::Init()
	{ 
	Log(_title + ": Init");
	/*for(int i = 0; i < UnitCount; i++)
		{
		ConveyorStates[i] = {US_NOTINIT, US_NOTINIT};
		Conveyor cnv = Conveyors[i]; 
		if(cnv.IsActive())
			{
			cnv.Init();
			ConveyorStates[i] = {US_OFF, US_OFF};
			}
		}
		*/
	_setState(SS_OFF);  
	}

// ------------------------------------
void  System::Start()
	{
	Log(_title + ": Start()"); 
	if(_state == SS_OFF)
		_setState(SS_STARTING);
	}
	 		
// ------------------------------------
void System::_setState(SystemState state)
	{
	_logStates({_state, state});
	_state = state;
	}	

// ------------------------------------
SystemInfo System::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
	PinInInfo bOni = BtnOn.GetInfo();
	PinInInfo bOffi = BtnOff.GetInfo();
	PinInInfo bReseti = BtnReset.GetInfo();
    return {ui.Title,
			ui.UnitType, 
			GetSystemStateText(_state),
			bOni.Pin, 
			bOffi.Pin, 
			bReseti.Pin 
			}; 
	}

// ------------------------------------
void System::LogInfo()
	{
	LogInfo(false);
	}

// ------------------------------------
void System::LogInfo(bool conv)
	{
	SystemInfo si = GetInfo();
	String str = si.Title + "; " + si.UnitType + "; " + si.State; 
	str = str + "; BtnOn-" + String(si.PinOn) + "; BtnOff-" + String(si.PinOff) + "; BtnReset-" + String(si.PinReset);
	Log(str);
	if (conv)
		{
		for(int i = 0; i < UnitCount; i++)
			{
			Conveyor cnv = Conveyors[i]; 
			if(cnv.IsActive())
				{
				LogText("i=" + String(i) + "; ");
				cnv.LogInfo();
				}
			}
		} 		
	}

// ------------------------------------
String System::GetSystemStateText(SystemState state)
	{
	switch (state)
		{
		case SS_NOTINIT		: return "NOTINIT";
		case SS_UNKNOWN		: return "UNKNOWN";
		case SS_ON			: return "ON";
		case SS_OFF			: return "OFF";
		case SS_STARTING	: return "STARTING";
		case SS_STOPPING	: return "STOPPING";
		default			    : return "Error " + String(state);
		}
	}

// ------------------------------------
void System::SetupConveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed)
	{
	if(UnitCount < MAX_UNIT_NUMBER)
		{
		title = title + "_" + String(UnitCount); 
		Conveyors[UnitCount] = Conveyor(title, pinIn, pinOut, pinAuto, pinLed);
		Conveyors[UnitCount].Init();
		UnitCount++;
		}
	}
	
// ------------------------------------
void System::Log(String str)
	{
	if (LOGLEVEL >= LL_HIGH) LogTextLn(str);
	}

// ------------------------------------
SystemState2 System::GetState()
	{
	SystemState2 ss2 = {_state, _state};
	if (_state < SS_ERR300)
		{
		_updateConveyorStates();
		
		if (_state == SS_STARTING)
			_checkStateStarting();
		
		/*if (_state == SS_OFF)
			_checkStateOff();
		else if (_state == SS_ON)
			_checkStateOn();

		for(int i = UnitCount - 1; i >= 0 ; i--)
			{
			Conveyor cnv = Conveyors[i];
			ConveyorState2 cs2; 
			if(cnv.IsActive())
				{
				cs2 = cnv.GetState();
				if (_state == SS_OFF)
					{
					if (cs2.New != US_OFF)
						_setState(SS_ERR301);
					}
				else if  (_state == SS_ON)
					{
					if (cs2.New != US_ON)
						_setState(SS_ERR302);
					}
				else if  (_state == SS_STARTING)
					{
				//Log("SS_STARTING : " + String(_state));
				//cnv.LogInfo();
				_checkStateStarting();
				
					if (cs2.New != US_ON && cs2.New != US_OFF && cs2.New != US_STARTING)
						_setState(SS_ERR303);
					}
				else if  (_state == SS_STOPPING)
					{
					if (cs2.New != US_ON && cs2.New != US_OFF && cs2.New != US_STOPPING) 
						_setState(SS_ERR304);
					}
				else
					{
					Log("SS_ERR305 : " + String(i) + " " + String(_state) + " " + GetSystemStateText(_state) + " && " +cnv.GetConveyorStateText(cs2.New));
					_setState(SS_ERR305);
					}
				}
			}*/
		}  
			
		for(int i = UnitCount - 1; i >= 0 ; i--)
			{
			Conveyor cnv = Conveyors[i];
			cnv.LedConveyor.Refresh();
			}
	ss2.New = _state;
	_ifChanged(ss2);

	///   BUTTONS
	PinState2 pi = BtnOn.GetState();
	if(pi.Old == KS_OFF && pi.New == KS_ON)
		{
		Start();
		}
	
	return ss2; 
	}

// ------------------------------------
void System::_ifChanged(SystemState2 ss2)
	{
	if (ss2.Old != ss2.New)
		{
		_logStates(ss2);
		/*if (cs2.New == US_ON)
			LedConveyor.SetOn();
		else if (cs2.New == US_STARTING || cs2.New == US_STOPPING)
			LedConveyor.SetBlink();
		else if (cs2.New >= US_ERR200)
			LedConveyor.SetBlinkFast();
		else 
			LedConveyor.SetOff();*/
		}
	}

// ------------------------------------
void System::_logStates(SystemState2 ss2)
	{
	Log(_title + " " + GetSystemStateText(ss2.Old) + " -> " + GetSystemStateText(ss2.New));
	}

// ------------------------------------
void System::_updateConveyorStates()
	{
	Log("_updateConveyorStates()");
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		Conveyor cnv = Conveyors[i];
		if(cnv.IsActive())
			{
			ConveyorStates[i] = cnv.GetState();
			}
		}
	}
	
// ------------------------------------
void System::_checkStateOff()
	{
	bool alarmTurnOff = false;
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		if (alarmTurnOff)
			Conveyors[i].TurnOffAlarm();
		else
			{
			if (ConveyorStates[i].New != US_OFF)
				_state = SS_ERR300; 	
				alarmTurnOff = true;
			}
		}
	}

// ------------------------------------
void System::_checkStateOn()
	{
	bool alarmTurnOff = false;
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		if (alarmTurnOff)
			Conveyors[i].TurnOffAlarm();
		else
			{
			if (ConveyorStates[i].New != US_OFF)
				_state = SS_ERR300; 	
				alarmTurnOff = true;
			}
		}

	}

// ------------------------------------
void System::_checkStateStarting()
	{

	bool alarmTurnOff = false;
	ConveyorStatePrevCurr cs; 
	cs.Prev = US_ON; 	
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		if (alarmTurnOff)
			Conveyors[i].TurnOffAlarm();
		else
			{
	Log("_checkStateStarting()");
			Conveyors[i].LogInfo();
			cs.Curr = ConveyorStates[i].New;
			if (cs.Prev == US_ON)
				{ 
				if (cs.Curr == US_OFF)
					Conveyors[i].TurnOn();
				else if (cs.Curr == US_STARTING || cs.Curr == US_ON)
					{}
				else
					_state = SS_ERR300;
				}
			else if (cs.Prev == US_STARTING)
				{ 
				if (cs.Curr == US_OFF)
					{}
				else
					_state = SS_ERR300;
				}
			else if (cs.Prev == US_OFF)
				{ 
				if (cs.Curr == US_OFF)
					{}
				else
					_state = SS_ERR300;
				}
			else
				_state = SS_ERR300;
				
			alarmTurnOff = (_state >= SS_ERR300);
			}
		}	
	}
