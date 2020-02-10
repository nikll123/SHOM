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
static void Unit::Log(char *str)
	{
	Serial.print(str);
	}

//------------------------------
static void Unit::LogLn(char *str)
	{
	Log(str);
	Serial.println("");
	}

//------------------------------
static void Unit::Log(byte x)
	{
  	char buf[3];
	itoa(x, buf, 10);
  	Serial.print(buf);
	}

//------------------------------
static void Unit::LogLn(byte x)
	{
	Log(x);
	Serial.println("");
	}

//------------------------------
static void Unit::Log(int x)
	{
  	char buf[5];
	itoa(x, buf, 10);
  	Serial.print(buf);
	}

//------------------------------
static void Unit::LogLn(int x)
	{
	Log(x);
	Serial.println("");
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
		
	Log("   Error! ");
	Log(_title);
	Log(" ");
	Log(pref);
	Log("_ERR");
	Log(err);
	LogLn("");
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
	LogLn(ui.Title);
	LogLn(ui.UnitType);
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
 
