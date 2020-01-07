#include "System.h"

// ------------------------------------
System::System()
	{
	}

System::System(String title, uint8_t pinBtnOn, uint8_t pinBtnOff, uint8_t pinBtnReset) : Unit(title, UT_SYSTEM)
	{
	//Serial.println("--- System::System");
	_logLevel = LL_NORMAL;
	Init();
	BtnOn = SetupButton("BtnOn", pinBtnOn);
	BtnOff = SetupButton("BtnOff", pinBtnOff);
	BtnReset = SetupButton("BtnReset", pinBtnReset);
	}
	
	
// ------------------------------------
void System::Init()
	{ 
	//Log("Init");
	for(int i = 0; i < UnitCount; i++)
		{
		ConveyorStates[i] = {US_NOTINIT, US_NOTINIT};
		Conveyors[i].Init();
		ConveyorStates[i] = {US_OFF, US_OFF};
		}
	_setState(SS_OFF);  
	}

// ------------------------------------
PinIn System::SetupButton(String btnTitle, uint8_t pin)
	{
	String _txtbuffer = _title + "." + btnTitle;
	PinIn btn = PinIn(_txtbuffer, pin, LT_INVERSE);
	btn.Init();
	return btn ; 
	}

// ------------------------------------
void System::SetupConveyor(String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed)
	{
	//Serial.println("--- System::SetupConveyor");
	if(UnitCount < MAX_UNIT_NUMBER)
		{
		String _txtbuffer = _title + "." + title + "_" + String(UnitCount); 
		Conveyors[UnitCount] = Conveyor(title, pinIn, pinOut, pinAuto, pinLed);
		UnitCount++;
		}
	}

// ------------------------------------
void  System::Start()
	{
	Log("Start()"); 
	if(_state == SS_OFF)
		_setState(SS_STARTING);
	}
	 		
// ------------------------------------
void  System::Stop()
	{
	Log("Stop()"); 
	if(_state == SS_STARTING || _state == SS_ON)
		_setState(SS_STOPPING);
	}

// ------------------------------------
void  System::Reset()
	{
	Log("Reset()");
	unsigned long sink = Time(TA_RESET); 
	Init();
	} 
	
// ------------------------------------
void System::_setState(SystemState state)
	{
	_logIfChanged({_state, state});
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
	String str;
	if (conv)
		{
		for(int i = 0; i < UnitCount; i++)
			{
			str = "";
			str.concat(i);
			str.concat(") ");
			str.concat(Conveyors[i].GetInfoTxt());
			Serial.println(str);
			}
		}
	str = si.UnitType + "; " + si.State; 
	str.concat("; BtnOn-");
	str.concat(si.PinOn);
	str.concat("; BtnOff-");
	str.concat(si.PinOff);
	str.concat("; BtnReset-");
	str.concat(si.PinReset);
	Serial.println(str);
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
SystemState2 System::GetState()
	{
	//Serial.println('1');
	
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
		else if (_state == SS_SELFTEST)
			ss = _checkSelfTest();
		else
			ss = SS_ERR;	
		_state = ss;
//		Serial.println(_state);
		}
	else
		{
		for(int i = 0; i < UnitCount ; i++)
			{
			Conveyors[i].LedConveyor.Refresh();
			}  
		}  
	ss2.New = _state;
	_logIfChanged(ss2);
	
//	Serial.println('2');
	
	_checkButtons();
	return ss2; 
	}

// ------------------------------------
void System::_checkButtons()
	{
	//Log("_checkButtons()");
	/*Serial.print(BtnOn.GetStateTxt());
	Serial.print(" ");
	Serial.print(BtnOff.GetStateTxt());
	Serial.print(" ");
	Serial.println(BtnReset.GetStateTxt());*/
	
	if (BtnReset.GetState().Front())
		{
//	Serial.print("Reset();");
		Reset();
	//	unsigned long sink = Time(TA_FIX); 
		}
	/*else if (BtnReset.GetState().Back())
		{
		unsigned long sink = Time(TA_RESET); 
		}
	else if (BtnReset.GetState().High() && Time(TA_PERIOD) > _selfTestPause && _state != SS_SELFTEST)
		{
		Log("SelfTest");
		_state = SS_SELFTEST;
		}*/
	else if(_state < SS_ERR && BtnOff.GetState().Front())
		{
//		Serial.print("Stop();");
		Stop();
		}
	else if (_state < SS_ERR && BtnOn.GetState().Front())
		{ 
//		Serial.print("Start();");
		Start();
		}
	
	}

// ------------------------------------
void System::_logIfChanged(SystemState2 ss2)
	{
	if (ss2.Old != ss2.New)
		{
		_logStates(ss2);
		if (ss2.New == SS_ON || ss2.New == SS_STARTING)
			_startMillis = millis();
		else if (ss2.New == SS_ERR || ss2.New == SS_OFF)
			Log(GetWorkTimeText());
		}
	}

// ------------------------------------
void System::_logStates(SystemState2 ss2)
	{
	Log(GetSystemStateText(ss2.Old) + " -> " + GetSystemStateText(ss2.New));
	}

// ------------------------------------
void System::_updateConveyorStates()
	{
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
		ConveyorStates[i] = Conveyors[i].GetState();
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
//		_checkButtons();
		if(_state == SS_STARTING)
			{
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
	//	_checkButtons();
		if(_state == SS_STOPPING)
			{
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
		}
	return _calcState(cntErr, cntOn, cntOff, cntStoping, 0);
	}

// ------------------------------------
SystemState System::_checkStateOff()
	{
	int cntErr=0, cntOff=0;
	bool haltRest = false;


	for(int i = 0; i < UnitCount ; i++)
		{
//Serial.println(_state);
		//_checkButtons();
		if(_state == SS_OFF)
			{
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
		}
	
	if(_state == SS_OFF)
		return _calcState(cntErr, 0, cntOff, 0, 0);
	else
		return _state;        // button has been pressed
	}

// ------------------------------------
SystemState System::_checkStateOn()
	{
	int cntErr=0, cntOn=0;
	bool doHalt = false;
	for(int i = UnitCount - 1; i >= 0 ; i--)
		{
	//	_checkButtons();
		if(_state == SS_ON)
			{
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
	_state = SS_ERR;
	}

// ------------------------------------
String System::GetWorkTimeText()
	{
	unsigned long wt = millis() - _startMillis;
	wt = wt / 1000;	
	int h = wt / (60 * 60);
	wt = wt % (60 * 60);
	int m = wt / 60;
	int s = wt % 60;
	String msg = "work time = "; 
	msg += h;
	msg += ":";
	msg += m;
	msg += ":";
	msg += s;
	return msg; 
	} 

// ------------------------------------
void System::SetErrState(UnitError err, String msg)
	{
	Log(msg);
	SetErrState(err);
	}

// ------------------------------------
SystemState System::_checkSelfTest()
	{
	if(0 == Time(TA_GET) || 1000 <= Time(TA_PERIOD))
		{
		unsigned long sink = Time(TA_FIX); 
		for(int i = 0; i < UnitCount; i++)
			{
			Conveyors[i].LedConveyor.Inverse();
			}
		}
	return SS_SELFTEST;
	}
