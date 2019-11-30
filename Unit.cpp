#include "Arduino.h"
#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit()
	{
	_title = "DummyUnit";
	_type = UT_NONE;
	_logLevel = LL_NONE;
	}

Unit::Unit(String title, UnitType type)
	{
	_title = title;
	_type = type;
	}

//------------------------------
static void Unit::LogTextLn(String txt)
	{
	LogText(txt);
	LogLn();
	}
	
//------------------------------
static void Unit::LogText(String txt)
	{
	Serial.print(txt);
	}

//------------------------------
static void Unit::LogLn()
	{
	Serial.println("");
	}

//------------------------------
void Unit::Log(String str)
	{
	if (LOGLEVEL >= _logLevel)
		{ 
		str = _title + " : " + str;
		LogTextLn(str);
		}
	}
	
//------------------------------
void Unit::LogErr(UnitError err)
	{
	String pref; 
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
		
	LogTextLn("   Error! " + _title + " " + pref + "_ERR" + String(err));
	}	

//------------------------------
UnitInfo Unit::GetInfo()
	{
	return	{
			_title,
			GetUnitTypeText()
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
String Unit::GetUnitTypeText()
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
		default			    	: return "GetUnitTypeText: unknown-" + String(_type);
		}
	}

