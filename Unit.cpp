#include "Arduino.h"
#include "Unit.h"

// =========   CONSTRUCTORS   =========
Unit::Unit()
	{
	_title = "DummyUnit";
	_type = UT_NONE;
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
void Unit::LogErr(String pref, int err)
	{
	LogTextLn("   Error! " + _title + " " + pref + String(err));
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
		case UT_NONE 		: return "NONE";
		case UT_LED         : return "LED";
		case UT_PIN         : return "PIN";
		case UT_PININ		: return "PININ";
		case UT_PINOUT		: return "PINOUT";
		case UT_BUTTONLED	: return "BUTTONLED";
		case UT_SYSTEM 		: return "SYSTEM";
		case UT_CONTACTOR 	: return "CONTACTOR";
		case UT_CONVEYOR 	: return "CONVEYOR";
		default			    : return "GetUnitTypeText: unknown-" + String(_type);
		}
	}

