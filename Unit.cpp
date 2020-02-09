#include "Arduino.h"
#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit()
	{
	_title = "DummyUnit";
	_type = UT_NONE;
	}

Unit::Unit(char *title, UnitType type)
	{
	_title = title;
	_type = type;
	}

//Log >>
//------------------------------
static void Unit::LogText(char *str)
	{
	Serial.print(str);
	}

//------------------------------
static void Unit::LogTextLn(char *str)
	{
	LogText(str);
	Serial.println("");
	}

//------------------------------
void Unit::Log(char *str)
	{
	Log(str, _logLevel);
	}
//------------------------------
void Unit::Log(char *str, byte ll)
	{
	if (LOGLEVEL >= ll)
		{ 
		LogText(_title);
		LogText(" : ");
		LogTextLn(str);
		}
	}


//------------------------------
void Unit::LogErr(UnitError err)
	{
	char *pref; 
	if (100 < err && err < 200)
		pref = "CS";
	else if (200 < err && err < 300)
		pref = "US";
	else if (300 < err && err < 400)
		pref = "SS";
	else if (400 < err && err < 500)
		pref = "CBS";
	else if (500 < err && err < 600)
		pref = "KS";
	else
		pref = "Unknown";
		
	LogText("   Error! ");
	LogText(_title);
	LogText(" ");
	LogText(pref);
	LogText("_ERR");
	LogText(char(err));
	LogTextLn("");
	}	
//Log <<

//------------------------------
UnitInfo Unit::GetInfo()
	{
	return	{
			_title,
			UnitTypeText()
			};
	}

//------------------------------
void Unit::LogInfo()
	{
	UnitInfo ui = GetInfo();
	LogTextLn(ui.Title);
	LogTextLn(ui.UnitType);
	}

//------------------------------
char *Unit::UnitTypeText()
	{
	switch (_type)
		{
		case UT_NONE 			: return "NONE";
		case UT_LED         	: return "LED";
		case UT_PIN         	: return "PIN";
		case UT_PININ			: return "PININ";
		case UT_PINOUT			: return "PINOUT";
		case UT_BUTTONLED		: return "BUTTONLED";
		case UT_SYSTEM 			: return "SYSTEM";
		case UT_CONTACTOR 		: return "CONTACTOR";
		case UT_CONVEYOR 		: return "CONVEYOR";
		case UT_CONVEYORHANDLER	: return "CONVEYORHANDLER";
		case UT_CANBUS 			: return "CANBUS";
		default			    	: return "UnitTypeText: unknown-" + char(_type);
		}
	}

// ------------------------------------
unsigned long Unit::Time(TimeAct ta)
	{
	unsigned long res = 0; 
	if (ta == TA_FIX)
		_millsCheck = millis();
	else if (ta == TA_GET)
		res = _millsCheck;
	else if (ta == TA_RESET)
		_millsCheck = 0;
	else if (ta == TA_PERIOD)
	    res = millis() - _millsCheck;
	    
	return res;
	}
 
