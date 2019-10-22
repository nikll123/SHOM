#include "System.h"

// ------------------------------------
System::System() : Unit("Dummy system", UT_CONVEYOR)
	{
	}

System::System(String title, uint8_t pinBtnOn, uint8_t pinBtnOff, uint8_t pinBtnReset) : Unit(title, UT_SYSTEM)
	{
	_pinBtnOn = pinBtnOn; 
	_pinBtnOff = pinBtnOff; 
	_pinBtnReset = pinBtnReset; 
	}

	

// ------------------------------------
SystemInfo System::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
    return {ui.Title,
			ui.UnitType, 
			GetConvSystemText(_state), 
			_pinBtnOn,
			_pinBtnOff,
			_pinBtnReset
			}; 
	}

// ------------------------------------
String System::GetConvSystemText(SystemState state)
	{
	   return "dummy state";
	}


// ------------------------------------
void System::SetupConveyor(uint8_t index, String title, uint8_t pinIn, uint8_t pinOut, uint8_t pinAuto, uint8_t pinLed)
	{
	Conveyors[index] = Conveyor(title, pinIn, pinOut, pinAuto, pinLed);
	Conveyors[index].Init();
	}