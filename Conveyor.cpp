#include "Conveyor.h"

// ------------------------------------
Conveyor::Conveyor() : Unit("EmptyUzel", UT_CONVEYOR)
	{
	
	}

Conveyor::Conveyor(String title, uint8_t pinBtnOn, uint8_t pinBtnOff, uint8_t pinBtnReset) : Unit(title, UT_CONVEYOR)
	{
	_pinBtnOn = pinBtnOn; 
	_pinBtnOff = pinBtnOff; 
	_pinBtnReset = pinBtnReset; 
	
	}
	

// ------------------------------------
ConveyorInfo Conveyor::GetInfo()
	{
	UnitInfo ui = Unit::GetInfo();
    return {ui.Title,
			ui.UnitType, 
			GetConvStateText(_state), 
			_pinBtnOn,
			_pinBtnOff,
			_pinBtnReset
			}; 
	}

String Conveyor::GetConvStateText(ConveyorState state)
	{
	   return "dummy state";
	}