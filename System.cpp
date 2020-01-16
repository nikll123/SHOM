#include "System.h"

// ------------------------------------
System::System()
	{
	}

System::System(String title, uint8_t pinBtnOn, uint8_t pinBtnOff, uint8_t pinBtnReset) : Unit(title, UT_SYSTEM)
	{
	Init();
	BtnOn = SetupButton("BtnOn", pinBtnOn);
	BtnOff = SetupButton("BtnOff", pinBtnOff);
	BtnReset = SetupButton("BtnReset", pinBtnReset);
    Unit Timer = Unit("Timer", UT_TIMER); 
	}
	
// ------------------------------------
PinIn System::SetupButton(String btnTitle, uint8_t pin)
	{
	String title = _title + "." + btnTitle;
	PinIn btn = PinIn(title, pin, LT_INVERSE);  
	return btn ; 
	}
	
// ------------------------------------
void System::Init()
	{ 
	Log("Init", LL_HIGH);
	for(int i = 0; i < UnitCount; i++)
		{
		ConveyorStates[i] = {US_NOTINIT, US_NOTINIT};
		if(Conveyors[i].IsActive())
			{
			Conveyors[i].Init();
			ConveyorStates[i] = {US_OFF, US_OFF};
			}
		}
	_setState(SS_OFF);
	for (int i = 0; i < 3; i++)
		{
		TurnLeds(1);
		delay(100);
		TurnLeds(0);
		delay(70);
		}  
	}

// ------------------------------------
void  System::Start()
	{
	Log("Start()", LL_HIGH); 
	if(_state == SS_OFF)
		_setState(SS_STARTING);
	}
	 		
// ------------------------------------
void  System::Stop()
	{
	Log("Stop()", LL_HIGH); 
	if(_state == SS_STARTING || _state == SS_ON)
		_setState(SS_STOPPING);
	}

// ------------------------------------
void  System::Reset()
	{
	Log("Reset()", LL_HIGH);

	for(int i = 0; i < UnitCount; i++)
		{
		Conveyors[i].Halt();
		}

	Init();
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
	String str = si.UnitType + "; " + si.State; 
	str = str + "; BtnOn-" + String(si.PinOn) + "; BtnOff-" + String(si.PinOff) + "; BtnReset-" + String(si.PinReset);
	Log(str, LL_HIGH);
	if (conv)
		{
		for(int i = 0; i < UnitCount; i++)
			{
			if(Conveyors[i].IsActive())
				{
				String str = String(i) + ") ";
				str = str + Conveyors[i].GetInfoTxt();
				Log(str, LL_HIGH);
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
		default			    : return "ERROR";
		}
	}

// ------------------------------------
void System::SetupConveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed)
	{
	if(UnitCount < MAX_UNIT_NUMBER)
		{
		title = _title + "." + title + "_" + String(UnitCount); 
		Conveyors[UnitCount] = Conveyor(title, pinIn, pinOut, pinAuto, pinLed);
		Conveyors[UnitCount].Init();
		UnitCount++;
		}
	}
	
// ------------------------------------
SystemState2 System::GetState()
	{
	SystemState2 ss2 = {_state, _state};
	if (_state < SS_ERR)       // no system error 
		{
		_updateConveyorStates();
		SystemState ss;
		if (_state == SS_STARTING)
			ss = _checkStateStarting();
		else if (_state == SS_STOPPING)
			ss = _checkStateStopping();
		else if (_state == SS_OFF)
			ss = _checkStateOff();
		else if (_state == SS_ON)
			ss = _checkStateOn();
		else
			ss = SS_ERR;	
		_state = ss;
		}  
	ss2.New = _state;
	_logIfChanged(ss2);
	
	_ledRefresh();
	bool x = _checkButtons();
		
	return ss2; 
	}


// ------------------------------------
void System::_ledRefresh()
	{
	if (_state == SS_ERR_CONN)
		{
		TurnLeds(1);
		delay(50);
		TurnLeds(0);
		delay(50);
		}
	else
		{
		for(int i = 0; i < UnitCount ; i++)
			{
			Conveyors[i].LedConveyor.Refresh();
			}
		}
	}
	
// ------------------------------------
bool System::_checkButtons()
	{
    SystemState _oldstate = _state; 
	if (BtnReset.GetState().Front())
		{
		Reset();
		unsigned long sink = Time(TA_FIX); 
		}
	else if(_state < SS_ERR && BtnOff.GetState().Front())
		Stop();
	else if (_state < SS_ERR && BtnOn.GetState().Front()) 
		Start();

	return _state != _oldstate;
	}

// ------------------------------------
void System::_logIfChanged(SystemState2 ss2)
	{
	if (ss2.Old != ss2.New)
		{
		_logStates(ss2);
		}
	}

// ------------------------------------
void System::_logStates(SystemState2 ss2)
	{
	Log(GetSystemStateText(ss2.Old) + " -> " + GetSystemStateText(ss2.New), LL_HIGH);
	}

// ------------------------------------
void System::_updateConveyorStates()
	{
	
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		if( _checkButtons())
			break;

		if(Conveyors[i].IsActive())
			{
			ConveyorStates[i] = Conveyors[i].GetState();
			}
		}
	}
	
// ------------------------------------
SystemState System::_checkStateStarting()
	{
	bool doHalt = false;
	int cntErr=0, cntOn=0, cntOff=0, cntStarting=0;
	ConveyorStatePrevCurr cspc2; 
	cspc2.Prev = US_ON; 	
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		if( _checkButtons())
			return _state;

		if (doHalt)
			{
			Conveyors[i].Halt();
			cntErr++;
			SetErrState(SS_ERR305);
			}
		else
			{
			cspc2.Curr = ConveyorStates[i].New;
			if (cspc2.Prev == US_ON)
				{ 
				if (cspc2.Curr == US_OFF)
					{
					Conveyors[i].TurnOn();
					cspc2.Curr = US_STARTING; 
					cntStarting++;
					}
				else if (cspc2.Curr == US_STARTING)
					cntStarting++;
				else if (cspc2.Curr == US_ON)
					cntOn++;
				else
					{
					cntErr++;
					Conveyors[i].LogStatesPrevCurr(cspc2.Curr);
					SetErrState(SS_ERR301);
					}
				}
			else if (cspc2.Prev == US_STARTING)
				{ 
				if (cspc2.Curr == US_OFF)
					{
					cspc2.Prev = US_STARTING;
					cntOff++;
					}
				else
					{
					cntErr++;
					SetErrState(SS_ERR302);
					}
				}
			else if (cspc2.Prev == US_OFF)
				{ 
				if (cspc2.Curr == US_OFF)
					cntOff++;
				else
					{
					cntErr++;
					SetErrState(SS_ERR303);
					}
				}
			else
				{
				cntErr++;
				SetErrState(SS_ERR304);
				}
				
			cspc2.Prev = cspc2.Curr; 
			doHalt = (cntErr > 0);
			}
		}
	return _calcState(cntErr, cntOn, cntOff, 0, cntStarting);
	}

// ------------------------------------
SystemState System::_checkStateStopping()
	{
	bool doHalt = false;
	int cntErr=0, cntOn=0, cntOff=0, cntStoping=0;
	ConveyorStatePrevCurr cspc2; 
	cspc2.Prev = US_OFF; 	
	for(int i = 0; i < UnitCount ; i++)
		{
		if( _checkButtons())
			return _state;

		if (doHalt)
			{
			Conveyors[i].Halt();
			cntErr++;
			SetErrState(SS_ERR306);
			}
		else
			{
			cspc2.Curr = ConveyorStates[i].New;
			if (cspc2.Prev == US_OFF)
				{ 
				if (cspc2.Curr == US_ON || cspc2.Curr == US_STARTING)
					{
					Conveyors[i].TurnOff();
					cntStoping++;
					}
				else if (cspc2.Curr == US_STOPPING)
					cntStoping++;
				else if (cspc2.Curr == US_OFF)
					cntOff++;
				else
					{
					cntErr++;
					SetErrState(SS_ERR307);
					}
				}
			else if (cspc2.Prev == US_STOPPING)
				{ 
				if (cspc2.Curr == US_ON)
					cntOn++;
				else
					{
					cntErr++;
					SetErrState(SS_ERR308);
					}
				}
			else if (cspc2.Prev == US_ON || cspc2.Prev == US_STARTING)
				{ 
				if (cspc2.Curr == US_ON)
					cntOn++;
				else
					{
					cntErr++;
					SetErrState(SS_ERR309);
					}
				}
			else
				{
				cntErr++;
				SetErrState(SS_ERR310, "Wrong ConveyorStatePrev = " + Conveyor::GetConveyorStateText(cspc2.Prev));
				}
				
			cspc2.Prev = cspc2.Curr; 
			doHalt = (cntErr > 0);
			}
		}
	return _calcState(cntErr, cntOn, cntOff, cntStoping, 0);
	}

// ------------------------------------
SystemState System::_checkStateOff()
	{
	int cntErr=0, cntOff=0;
	bool haltRest = false;
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		if( _checkButtons())
			return _state;

		bool err = false;
		if (haltRest)
			{
			Conveyors[i].Halt();
			cntErr++;
			SetErrState(SS_ERR311);
			}
		//else
			{
			if (ConveyorStates[i].New == US_OFF)
				cntOff++;
			else
				{
				err = true;
				cntErr++;
				SetErrState(SS_ERR312);
				}
			haltRest = (cntErr > 0);
			}
		if(err) 
			Conveyors[i].LedConveyor.SetBlinkFast();
		else
			Conveyors[i].LedConveyor.SetOff();
			
		}
	return _calcState(cntErr, 0, cntOff, 0, 0);
	}

// ------------------------------------
SystemState System::_checkStateOn()
	{
	int cntErr=0, cntOn=0;
	bool doHalt = false;
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		if( _checkButtons())
			return _state;
			
		if (doHalt)
			{
			Conveyors[i].Halt();
			cntErr++;
			SetErrState(SS_ERR313);
			}
		else
			{
			if (ConveyorStates[i].New == US_ON)
				cntOn++;
			else
				{
				cntErr++;
				SetErrState(SS_ERR314);
				}
			doHalt = (cntErr > 0);
			}
		}
	return _calcState(cntErr, cntOn, 0, 0, 0);
	}

// ------------------------------------
SystemState System::_calcState(int cntErr, int cntOn, int cntOff, int cntStoping, int cntStarting)
	{
	SystemState ss = SS_UNKNOWN; 
	if (cntErr > 0)
		ss = SS_ERR; 
	else if (cntStoping > 0)
		ss = SS_STOPPING; 
	else if (cntStarting > 0)
		ss = SS_STARTING; 
	else if (cntOff == UnitCount)
		ss = SS_OFF;
	else if (cntOn == UnitCount)
		ss = SS_ON;
	else
		{
		SetErrState(SS_ERR315);
		ss = SS_ERR;
		} 
	return ss;
	}

// ------------------------------------
void System::SetErrState(UnitError err)
	{
	LogErr(err);
	if (err < SS_ERR_CONN)
		_state = SS_ERR;
	}

// ------------------------------------
void System::SetErrState(UnitError err, String msg)
	{
	Log(msg, LL_HIGH);
	LogErr(err);
	_state = SS_ERR;
	}

// ------------------------------------
void System::TurnLeds(bool on)
	{
	for(int i = 0; i < UnitCount; i++)
		{
		if (on)
			Conveyors[i].LedConveyor.SetOn();
		else
			Conveyors[i].LedConveyor.SetOff();
		}
	}

// ------------------------------------
void System::CheckConnection()
	{
	if (Timer.Time(TA_PERIOD) < CANBUS_TIMEOUT)
		Timer.Time(TA_FIX);
	else
		{
		Reset();
		_state = SS_ERR_CONN;			// Connection error 
		}
	}
	
